/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description   : Relize kernel mode Function
 * Author        : Hisilicon multimedia software group
 * Created       : 2009/07/16
 */
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "hi_osal.h"
#include "osal_mmz.h"
#include "pciv.h"
#include "pciv_drvadp.h"

#define PCIV_MAX_DMA_TASK (1 << 5)
#define WINDOW_BUFF_SIZE (7 * 1024 * 1024)
typedef struct {
    volatile hi_bool    create;
    volatile hi_bool    start;
    volatile hi_bool    config;         /* the flag of config or not */
    volatile hi_bool    hide;           /* the flag of hide or not */

    volatile hi_u32     get_cnt;        /* record the times of reading recycle_cb or pic_cb */
    volatile hi_u32     send_cnt;       /* record the times of writing recycle_cb or pic_cb */
    volatile hi_u32     resp_cnt;       /* record the times of finishing response interrupt */
    volatile hi_u32     lost_cnt;       /* record the times of dropping the image for no idle buffer */
    volatile hi_u32     notify_cnt;     /* the times of notify(receive the message of read_done or write_done) */

    hi_pciv_attr        pciv_attr;   /* record the dest image info and the opposite end dev info */
    volatile hi_pciv_buff_status aen_buff_status[PCIV_MAX_BUF_NUM];  /* used by sender. */
    volatile hi_u32     buf_use_cnt[PCIV_MAX_BUF_NUM];               /* used by sender. */
    volatile hi_bool    can_recv;       /* the flag of memory info synchronous, used in the receiver end */

    struct semaphore pciv_mutex;
} pciv_channel;

typedef struct {
    struct list_head    list;
    wait_queue_head_t   wq_dma_done;
    hi_bool             dma_done;
} pciv_user_dma_node;

#define PCIV_MIN_BUF_NUM        2
#define PCIV_SCHEDULE_TIMEOUT   2
#define PCIV_PROC_SHOW_NUM      (PCIV_MAX_BUF_NUM * 2 + 1)
#define PCIV_SLEEP_TIME         10
#define PCIV_WAIT_ENENT_TIMEOUT 200



static pciv_channel         g_pciv_chn[PCIV_MAX_CHN_NUM] = { 0 };
static pciv_user_dma_node   g_user_dma_pool[PCIV_MAX_DMA_TASK] = { 0 };
static struct list_head     g_list_head_user_dma;

extern hi_u32 g_rdone_time[PCIV_MAX_CHN_NUM];
extern hi_u32 g_rdone_gap[PCIV_MAX_CHN_NUM];
extern hi_u32 g_max_rdone_gap[PCIV_MAX_CHN_NUM];
extern hi_u32 g_min_rdone_gap[PCIV_MAX_CHN_NUM];
extern hi_u32 g_wdone_time[PCIV_MAX_CHN_NUM];
extern hi_u32 g_wdone_gap[PCIV_MAX_CHN_NUM];
extern hi_u32 g_max_wdone_gap[PCIV_MAX_CHN_NUM];
extern hi_u32 g_min_wdone_gap[PCIV_MAX_CHN_NUM];

static spinlock_t g_pciv_lock;
#define PCIV_SPIN_LOCK   spin_lock_irqsave(&g_pciv_lock, flags)
#define PCIV_SPIN_UNLOCK spin_unlock_irqrestore(&g_pciv_lock, flags)

#define PCIV_MUTEX_DOWN(sema)         \
do {                                  \
    if (down_interruptible(&(sema))) \
        return 0 - ERESTARTSYS;       \
} while (0)

#define PCIV_MUTEX_UP(sema) up(&(sema))

hi_s32 pciv_check_attr(const hi_pciv_attr *attr)
{
    hi_s32 i;

    /* check the number of the buffer is valide */
    if ((attr->count < PCIV_MIN_BUF_NUM) || (attr->count > PCIV_MAX_BUF_NUM)) {
        PCIV_ERR_TRACE("buffer count(%u) not invalid,should in [2,%u]\n", attr->count, PCIV_MAX_BUF_NUM);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }

    /* check the physical address */
    for (i = 0; i < attr->count; i++) {
        if (!attr->phy_addr[i]) {
            PCIV_ERR_TRACE("attr->phy_addr[%d]:0x%llx invalid\n", i, attr->phy_addr[i]);
            return HI_ERR_PCIV_ILLEGAL_PARAM;
        }
    }
    /* check the valid of the remote device */
    if ((attr->remote_obj.chip_id < 0) ||
        (attr->remote_obj.chip_id > PCIV_MAX_CHIPNUM) ||
        (attr->remote_obj.pciv_chn < 0) ||
        (attr->remote_obj.pciv_chn >= PCIV_MAX_CHN_NUM)) {
        PCIV_ERR_TRACE("invalid remote object(%d,%d)\n", attr->remote_obj.chip_id, attr->remote_obj.pciv_chn);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 pciv_is_support(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr, hi_s32 local_id)
{
    hi_s32 i;

    if (local_id != 0) {
        /* slave chip can not bind slave chip */
        if (attr->remote_obj.chip_id != 0) {
            PCIV_ERR_TRACE("a slave chip bind the other slave chip or it self, chip_id of src:%d, \
                chip_id of dst:%d\n",
                local_id, attr->remote_obj.chip_id);
            return HI_ERR_PCIV_NOT_SUPPORT;
        }

        /* DTS2016060809388--slave's different chn bind host's same chn */
        for (i = 0; i < PCIV_MAX_CHN_NUM; i++) {
            if ((attr->remote_obj.pciv_chn == g_pciv_chn[i].pciv_attr.remote_obj.pciv_chn) && (pciv_chn != i)) {
                PCIV_ERR_TRACE("two slave's chns bind the host's same pciv chn,pciv_chn:%d, \
                    attr->remote_obj.pciv_chn:%d\n",
                    pciv_chn, attr->remote_obj.pciv_chn);
                return HI_ERR_PCIV_NOT_SUPPORT;
            }
        }
    }

    /* DTS2016060809164  DTS2016060809307--host and slave remote_obj.chip_id cannot be itsself */
    if (attr->remote_obj.chip_id == local_id) {
        PCIV_ERR_TRACE("remote_obj chip_id can not be itself! remote_obj.chip_id:%d local_id:%d\n",
            attr->remote_obj.chip_id, local_id);
        return HI_ERR_PCIV_NOT_SUPPORT;
    }
    return HI_SUCCESS;
}

static hi_void pciv_reset_chn(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr, pciv_channel *chn)
{
    hi_ulong    flags;
    hi_s32      i;

    PCIV_SPIN_LOCK;

    for (i = 0; i < PCIV_MAX_BUF_NUM; i++) {
        chn->aen_buff_status[i] = BUFF_BUSY;
    }

    for (i = 0; i < attr->count; i++) {
        if (attr->phy_addr[i] != 0) {
            chn->aen_buff_status[i] = BUFF_FREE;
        }
    }

    osal_memcpy(&chn->pciv_attr, attr, sizeof(hi_pciv_attr));

    chn->start      = HI_FALSE;
    chn->config     = HI_TRUE;
    chn->create     = HI_TRUE;
    chn->can_recv   = HI_FALSE;
    chn->hide       = HI_FALSE; /* reset to show */
    chn->get_cnt    = 0;
    chn->send_cnt   = 0;
    chn->resp_cnt   = 0;
    chn->lost_cnt   = 0;
    chn->notify_cnt = 0;

    g_rdone_time[pciv_chn]      = 0;
    g_rdone_gap[pciv_chn]       = 0;
    g_max_rdone_gap[pciv_chn]   = 0;
    g_min_rdone_gap[pciv_chn]   = 0;
    g_wdone_time[pciv_chn]      = 0;
    g_wdone_gap[pciv_chn]       = 0;
    g_max_wdone_gap[pciv_chn]   = 0;
    g_min_wdone_gap[pciv_chn]   = 0;

    PCIV_SPIN_UNLOCK;
}

hi_s32 pciv_create(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr)
{
    hi_ulong        flags;
    hi_s32          ret;
    hi_s32          local_id;
    pciv_channel    *chn = HI_NULL;

    PCIV_CHECK_CHNID(pciv_chn);
    PCIV_CHECK_PTR(attr);

    PCIV_MUTEX_DOWN(g_pciv_chn[pciv_chn].pciv_mutex);

    PCIV_SPIN_LOCK;
    chn = &g_pciv_chn[pciv_chn];
    if (chn->create == HI_TRUE) {
        PCIV_ERR_TRACE("channel %d has been created\n", pciv_chn);
        PCIV_SPIN_UNLOCK;
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return HI_ERR_PCIV_EXIST;
    }

    if (pciv_check_attr(attr) != HI_SUCCESS) {
        PCIV_SPIN_UNLOCK;
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }

    local_id = pciv_drv_adp_get_local_id();
    PCIV_SPIN_UNLOCK;

    ret = pciv_is_support(pciv_chn, attr, local_id);
    if (ret != HI_SUCCESS) {
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return ret;
    }

    ret = pciv_firmware_create(pciv_chn, attr, local_id);
    if (ret != HI_SUCCESS) {
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return ret;
    }

    pciv_reset_chn(pciv_chn, attr, chn);

    PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);

    PCIV_INFO_TRACE("pciv chn %d create ok\n", pciv_chn);
    return ret;
}

hi_s32 pciv_destroy(hi_pciv_chn pciv_chn)
{
    hi_ulong        flags;
    pciv_channel    *chn = HI_NULL;

    PCIV_CHECK_CHNID(pciv_chn);

    PCIV_MUTEX_DOWN(g_pciv_chn[pciv_chn].pciv_mutex);
    PCIV_SPIN_LOCK;
    chn = &g_pciv_chn[pciv_chn];
    if (chn->create == HI_FALSE) {
        PCIV_NOTICE_TRACE("channel %d has not been created\n", pciv_chn);
        PCIV_SPIN_UNLOCK;
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return HI_SUCCESS;
    }
    if (chn->start == HI_TRUE) {
        PCIV_ERR_TRACE("pciv chn%d should stop first then destroy \n", pciv_chn);
        PCIV_SPIN_UNLOCK;
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return HI_ERR_PCIV_NOT_PERM;
    }
    PCIV_SPIN_UNLOCK;

    (hi_void)pciv_firmware_destroy(pciv_chn);
    PCIV_SPIN_LOCK;
    chn->create = HI_FALSE;
    PCIV_SPIN_UNLOCK;
    PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);

    PCIV_INFO_TRACE("pciv chn %d destroy ok\n", pciv_chn);
    return HI_SUCCESS;
}

static hi_s32 pciv_check_state_and_attr(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr, pciv_channel *chn)
{
    if (chn->create == HI_FALSE) {
        PCIV_ERR_TRACE("channel %d has not been created\n", pciv_chn);
        return HI_ERR_PCIV_UNEXIST;
    }

    if (chn->start == HI_TRUE) {
        PCIV_ERR_TRACE("channel %d is running\n", pciv_chn);
        return HI_ERR_PCIV_NOT_PERM;
    }

    if (pciv_check_attr(attr) != HI_SUCCESS) {
        PCIV_ERR_TRACE("channel %d attribute error\n", pciv_chn);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    return HI_SUCCESS;
}


hi_s32 pciv_set_attr(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr)
{
    hi_ulong        flags;
    hi_s32          ret;
    hi_s32          local_id;
    pciv_channel    *chn = HI_NULL;

    PCIV_CHECK_CHNID(pciv_chn);
    PCIV_CHECK_PTR(attr);

    PCIV_MUTEX_DOWN(g_pciv_chn[pciv_chn].pciv_mutex);

    chn = &g_pciv_chn[pciv_chn];

    ret = pciv_check_state_and_attr(pciv_chn, attr, chn);
    if (ret != HI_SUCCESS) {
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return ret;
    }

    local_id = pciv_drv_adp_get_local_id();

    ret = pciv_is_support(pciv_chn, attr, local_id);
    if (ret != HI_SUCCESS) {
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return ret;
    }

    ret = pciv_firmware_set_attr(pciv_chn, attr, local_id);
    if (ret != HI_SUCCESS) {
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return ret;
    }

    PCIV_SPIN_LOCK;
    osal_memcpy(&chn->pciv_attr, attr, sizeof(hi_pciv_attr));
    chn->config = HI_TRUE;
    PCIV_SPIN_UNLOCK;

    PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);

    PCIV_INFO_TRACE("pciv chn %d set attr ok\n", pciv_chn);
    return HI_SUCCESS;
}

hi_s32 pciv_get_attr(hi_pciv_chn pciv_chn, hi_pciv_attr *attr)
{
    pciv_channel *chn = HI_NULL;

    PCIV_CHECK_CHNID(pciv_chn);
    PCIV_CHECK_PTR(attr);

    chn = &g_pciv_chn[pciv_chn];

    if (chn->create == HI_FALSE) {
        PCIV_ERR_TRACE("channel %d has not been created\n", pciv_chn);

        return HI_ERR_PCIV_UNEXIST;
    }

    if (chn->config != HI_TRUE) {
        PCIV_ERR_TRACE("attr of channel %d has not been setted\n", pciv_chn);

        return HI_ERR_PCIV_NOT_PERM;
    }

    osal_memcpy(attr, &chn->pciv_attr, sizeof(hi_pciv_attr));

    return HI_SUCCESS;
}

hi_s32 pciv_start(hi_pciv_chn pciv_chn)
{
    hi_ulong            flags;
    hi_u32              i;
    hi_s32              local_id;
    hi_pciv_remote_obj  remote_obj;
    pciv_pic            recv_pic;
    pciv_channel        *chn = HI_NULL;

    PCIV_CHECK_CHNID(pciv_chn);

    PCIV_MUTEX_DOWN(g_pciv_chn[pciv_chn].pciv_mutex);
    PCIV_SPIN_LOCK;

    chn = &g_pciv_chn[pciv_chn];
    if (chn->create != HI_TRUE) {
        PCIV_ERR_TRACE("channel %d not create\n", pciv_chn);

        PCIV_SPIN_UNLOCK;
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return HI_ERR_PCIV_UNEXIST;
    }

    if (chn->start == HI_TRUE) {
        PCIV_INFO_TRACE("channel %d is running\n", pciv_chn);

        PCIV_SPIN_UNLOCK;
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return HI_SUCCESS;
    }

    remote_obj.pciv_chn = chn->pciv_attr.remote_obj.pciv_chn;
    remote_obj.chip_id = 0;
    recv_pic.filed = -1;
    local_id = pciv_drv_adp_get_local_id();
    if (local_id != 0) {
        for (i = 0; i < chn->pciv_attr.count; i++) {
            chn->aen_buff_status[i] = BUFF_FREE;
        }
        pciv_drv_adp_send_msg(&remote_obj, PCIV_MSGTYPE_FREE, &recv_pic);
    }
    chn->can_recv = HI_TRUE;

    PCIV_SPIN_UNLOCK;
    pciv_firmware_start(pciv_chn);
    PCIV_SPIN_LOCK;
    chn->start = HI_TRUE;

    PCIV_SPIN_UNLOCK;
    PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);

    PCIV_INFO_TRACE("pciv chn %d start ok\n", pciv_chn);
    return HI_SUCCESS;
}

hi_s32 pciv_stop(hi_pciv_chn pciv_chn)
{
    hi_ulong        flags;
    hi_s32          ret;
    hi_s32          local_id;
    pciv_channel    *chn = HI_NULL;

    PCIV_CHECK_CHNID(pciv_chn);

    PCIV_MUTEX_DOWN(g_pciv_chn[pciv_chn].pciv_mutex);

    chn = &g_pciv_chn[pciv_chn];
    if (chn->create != HI_TRUE) {
        PCIV_ERR_TRACE("channel %d not create\n", pciv_chn);

        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return HI_ERR_PCIV_UNEXIST;
    }
    if (chn->start != HI_TRUE) {
        PCIV_INFO_TRACE("channel %d has stoped\n", pciv_chn);

        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return HI_SUCCESS;
    }

    PCIV_SPIN_LOCK;
    /* first set the stop flag */
    chn->start = HI_FALSE;
    PCIV_SPIN_UNLOCK;

    /* wait for the PCI task finished */
    local_id = pciv_drv_adp_get_local_id();
    if (local_id != 0) {
        while (chn->send_cnt != chn->resp_cnt) {
            set_current_state(TASK_INTERRUPTIBLE);
            (hi_void)schedule_timeout(PCIV_SCHEDULE_TIMEOUT);
            continue;
        }
    }

    /* then stop the media releated work */
    ret = pciv_firmware_stop(pciv_chn);
    if (ret) {
        PCIV_SPIN_LOCK;
        chn->start = HI_TRUE;
        PCIV_SPIN_UNLOCK;

        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return ret;
    }

    PCIV_SPIN_LOCK;
    chn->can_recv = HI_FALSE;
    PCIV_SPIN_UNLOCK;

    PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);

    PCIV_INFO_TRACE("pciv chn %d stop ok\n", pciv_chn);
    return HI_SUCCESS;
}

hi_s32 pciv_hide(hi_pciv_chn pciv_chn, hi_bool hide)
{
    pciv_channel *chn = HI_NULL;

    PCIV_CHECK_CHNID(pciv_chn);

    PCIV_MUTEX_DOWN(g_pciv_chn[pciv_chn].pciv_mutex);

    chn = &g_pciv_chn[pciv_chn];
    if (chn->create != HI_TRUE) {
        PCIV_ERR_TRACE("channel %d not created\n", pciv_chn);
        PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
        return HI_ERR_PCIV_UNEXIST;
    }

    chn->hide = hide;

    PCIV_INFO_TRACE("pciv chn %d hide%d ok\n", pciv_chn, hide);

    PCIV_MUTEX_UP(g_pciv_chn[pciv_chn].pciv_mutex);
    return HI_SUCCESS;
}

/* only when  the slave chip reciver data or image, we shoud config the window vb */
hi_s32 pciv_window_vb_create(const hi_pciv_win_vb_cfg *cfg)
{
    hi_pciv_win_vb_cfg  vb_cfg;
    hi_u32              i, j, size, count;
    hi_s32              local_id;

    PCIV_CHECK_PTR(cfg);

    local_id = pciv_drv_adp_get_local_id();

    /* on the host chip, the action creating the special region is not supported */
    if (local_id == 0) {
        PCIV_ERR_TRACE("master chip does not support!\n");
        return HI_ERR_PCIV_NOT_SUPPORT;
    }

    if (cfg->pool_count > PCIV_MAX_VBCOUNT || cfg->pool_count == 0) {
        PCIV_ERR_TRACE("pool_count:%d is illegal,which should between [1,8]!\n",
            cfg->pool_count);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }

    /* rank the pool in accordance with the size, in convenient of the used of follow-up */
    osal_memcpy(&vb_cfg, cfg, sizeof(vb_cfg));
    for (i = 0; i < cfg->pool_count; i++) {
        if ((cfg->blk_size[i] == 0) || (cfg->blk_size[i] > WINDOW_BUFF_SIZE)) {
            PCIV_ERR_TRACE("blk_size[%d]:%d is illegal\n", i, cfg->blk_size[i]);
            return HI_ERR_PCIV_ILLEGAL_PARAM;
        }
        if (cfg->blk_count[i] == 0) {
            PCIV_ERR_TRACE("blk_count[%d]:%d is illegal\n",
                i, cfg->blk_count[i]);
            return HI_ERR_PCIV_ILLEGAL_PARAM;
        }
    }
    for (i = 0; i < cfg->pool_count - 1; i++) {
        for (j = i + 1; j < cfg->pool_count; j++) {
            if (vb_cfg.blk_size[j] < vb_cfg.blk_size[i]) {
                size = vb_cfg.blk_size[i];
                count = vb_cfg.blk_count[i];

                vb_cfg.blk_size[i] = vb_cfg.blk_size[j];
                vb_cfg.blk_count[i] = vb_cfg.blk_count[j];

                vb_cfg.blk_size[j] = size;
                vb_cfg.blk_count[j] = count;
            }
        }
    }

    return pciv_firmware_window_vb_create(&vb_cfg);
}

hi_s32 pciv_window_vb_destroy(hi_void)
{
    hi_s32 local_id = pciv_drv_adp_get_local_id();

    /* in the master chip,not support destroy the special area */
    if (local_id == 0) {
        PCIV_ERR_TRACE("master chip does not support!\n");
        return HI_ERR_PCIV_NOT_SUPPORT;
    }

    return pciv_firmware_window_vb_destroy();
}

hi_s32 pciv_malloc(hi_u32 size, hi_u64 *phy_addr)
{
    hi_s32 local_id = pciv_drv_adp_get_local_id();

    return pciv_firmware_malloc(size, local_id, phy_addr);
}

hi_s32 pciv_free(hi_u64 phy_addr)
{
    hi_s32 local_id = pciv_drv_adp_get_local_id();

    return pciv_firmware_free(local_id, phy_addr);
}

hi_s32 pciv_malloc_chn_buffer(hi_pciv_chn pciv_chn, hi_u32 index, hi_u32 size, hi_u64 *phy_addr)
{
    hi_s32 local_id = pciv_drv_adp_get_local_id();

    return pciv_firmware_malloc_chn_buffer(pciv_chn, index, size, local_id, phy_addr);
}

hi_s32 pciv_free_chn_buffer(hi_pciv_chn pciv_chn, const hi_u32 index)
{
    hi_s32 local_id = pciv_drv_adp_get_local_id();

    return pciv_firmware_free_chn_buffer(pciv_chn, index, local_id);
}

hi_void pciv_user_dma_done(pciv_send_task *task)
{
    pciv_user_dma_node *user_dma_node = HI_NULL;

    /* assert the finished DMA task is the last one */
    HI_ASSERT((task->prv_data[0] + 1) == task->prv_data[1]);

    user_dma_node = (pciv_user_dma_node *)(hi_ulong)task->prv_data[2];
    user_dma_node->dma_done = HI_TRUE;
    wake_up(&user_dma_node->wq_dma_done);
}

static hi_s32 pciv_user_dma_add_task(hi_pciv_dma_task *task)
{
    hi_ulong            flags;
    hi_s32              i;
    pciv_send_task      pci_task;
    hi_s32              ret             = HI_SUCCESS;
    pciv_user_dma_node  *user_dma_node  = HI_NULL;

    PCIV_SPIN_LOCK;
    user_dma_node = list_entry(g_list_head_user_dma.next, pciv_user_dma_node, list);
    list_del(g_list_head_user_dma.next);
    PCIV_SPIN_UNLOCK;

    user_dma_node->dma_done = HI_FALSE;

    for (i = 0; i < task->count; i++) {
        pci_task.src_phy_addr = task->block[i].src_addr;
        pci_task.dst_phy_addr = task->block[i].dst_addr;
        pci_task.len    = task->block[i].blk_size;
        pci_task.read   = task->read;
        pci_task.prv_data[0] = i;
        pci_task.prv_data[1] = task->count;
        pci_task.prv_data[2] = (hi_u64)(hi_ulong)user_dma_node;
        pci_task.call_back = HI_NULL;

        /* if this is the last task node, we set the callback */
        if ((i + 1) == task->count) {
            pci_task.call_back = pciv_user_dma_done;
        }

        if (pciv_drv_adp_add_dma_task(&pci_task) != HI_SUCCESS) {
            ret = HI_ERR_PCIV_NOMEM;
            break;
        }
    }

    if (ret == HI_SUCCESS) {
        hi_s32 time_left;
        time_left = wait_event_timeout(user_dma_node->wq_dma_done,
            (user_dma_node->dma_done == HI_TRUE), PCIV_WAIT_ENENT_TIMEOUT);
        if (time_left == 0) {
            PCIV_WARN_TRACE("wait_event_timeout \n");
            ret = HI_ERR_PCIV_TIMEOUT;
        }
    }

    PCIV_SPIN_LOCK;
    list_add_tail(&user_dma_node->list, &g_list_head_user_dma);
    PCIV_SPIN_UNLOCK;
    return ret;
}

hi_s32 pciv_user_dma_task(hi_pciv_dma_task *task)
{
    hi_s32 i;
    hi_s32 ret;
    hi_s32 local_id;

    PCIV_CHECK_PTR(task);
    PCIV_CHECK_PTR(task->block);

    if (list_empty(&g_list_head_user_dma)) {
        return HI_ERR_PCIV_BUSY;
    }

    if ((task->read != HI_TRUE) && (task->read != HI_FALSE)) {
        PCIV_ERR_TRACE("DMA size is illeage! read:%d\n", task->read);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }

    local_id = pciv_drv_adp_get_local_id();

    if (local_id == 0) {
        for (i = 0; i < task->count; i++) {
            if (pciv_drv_adp_addr_check(&task->block[i], task->read) != HI_SUCCESS) {
                PCIV_ERR_TRACE("Error slave addr and size! task->block[%d]: \
                    src_addr:0x%llx, dst_addr:0x%llx, blk_size:%x\n",
                    i,
                    task->block[i].src_addr,
                    task->block[i].dst_addr,
                    task->block[i].blk_size);
                return HI_ERR_PCIV_ILLEGAL_PARAM;
            }
        }
    }

    for (i = 0; i < task->count; i++) {
        if ((task->block[i].src_addr == 0) || (task->block[i].dst_addr == 0) ||
            ((task->block[i].src_addr & 0x3) != 0) || ((task->block[i].dst_addr & 0x3) != 0)) {
            PCIV_ERR_TRACE(" src_addr:0x%llx dst_addr:0x%llx is illeage! \n",
                task->block[i].src_addr, task->block[i].dst_addr);
            return HI_ERR_PCIV_ILLEGAL_PARAM;
        }
    }

    ret = pciv_user_dma_add_task(task);
    return ret;
}

static hi_void pciv_check_notify_cnt(hi_pciv_chn pciv_chn, hi_u32 index, hi_u32 count)
{
    pciv_channel *chn = &g_pciv_chn[pciv_chn];

    if (count == 0) {
        chn->notify_cnt = 0;
    } else {
        chn->notify_cnt++;
        if (count != chn->notify_cnt) {
            PCIV_WARN_TRACE("warnning: pciv_chn:%d, read_done msg_seq -> (%u,%u),bufindex:%u \n",
                pciv_chn, count, chn->notify_cnt, index);
        }
    }
}

/* when recieved the message of release the shared memory,
this interface is been called to set the memory flag to idle */
hi_s32 pciv_free_share_buf(hi_pciv_chn pciv_chn, hi_u32 index, hi_u32 count)
{
    pciv_channel    *chn;
    hi_ulong        flags;

    PCIV_CHECK_CHNID(pciv_chn);

    chn = &g_pciv_chn[pciv_chn];

    PCIV_SPIN_LOCK;
    if (chn->start != HI_TRUE) {
        PCIV_ERR_TRACE("pciv channel %d not start!\n", pciv_chn);
        PCIV_SPIN_UNLOCK;
        return HI_ERR_PCIV_UNEXIST;
    }

    if (index >= PCIV_MAX_BUF_NUM) {
        PCIV_ERR_TRACE("buffer index %u is too larger!\n", index);
        PCIV_SPIN_UNLOCK;
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    HI_ASSERT(index < chn->pciv_attr.count);

    /* check the serial number of the message is same or not with the loacal serial number */
    pciv_check_notify_cnt(pciv_chn, index, count);

    /* set the buffer flag to idle */
    chn->aen_buff_status[index] = BUFF_FREE;
    PCIV_SPIN_UNLOCK;

    return HI_SUCCESS;
}

hi_s32 pciv_free_all_buf(hi_pciv_chn pciv_chn)
{
    hi_u32          i;
    hi_ulong        flags;
    pciv_channel    *chn;

    chn = &g_pciv_chn[pciv_chn];

    PCIV_SPIN_LOCK;
    if (chn->create != HI_TRUE) {
        PCIV_SPIN_UNLOCK;
        return HI_SUCCESS;
    }

    for (i = 0; i < chn->pciv_attr.count; i++) {
        chn->aen_buff_status[i] = BUFF_FREE;
    }

    chn->can_recv = HI_TRUE;
    PCIV_SPIN_UNLOCK;
    return HI_SUCCESS;
}

/* when start DMA transmission, the interface must be called first to get a valid shared buffer */
static hi_s32 pciv_get_share_buf(hi_pciv_chn pciv_chn, hi_u32 *cur_index)
{
    hi_u32          i;
    pciv_channel    *chn;

    chn = &g_pciv_chn[pciv_chn];

    for (i = 0; i < chn->pciv_attr.count; i++) {
        if (chn->aen_buff_status[i] == BUFF_HOLD) {
            *cur_index = i;
            return HI_SUCCESS;
        }
    }

    for (i = 0; i < chn->pciv_attr.count; i++) {
        if (chn->aen_buff_status[i] == BUFF_FREE) {
            *cur_index = i;
            return HI_SUCCESS;
        }
    }

    return HI_FAILURE;
}

static hi_s32 pciv_get_share_buf_state(hi_pciv_chn pciv_chn)
{
    hi_ulong        flags;
    hi_u32          i;
    pciv_channel    *chn;

    PCIV_SPIN_LOCK;
    chn = &g_pciv_chn[pciv_chn];
    for (i = 0; i < chn->pciv_attr.count; i++) {
        if (chn->aen_buff_status[i] == BUFF_FREE) {
            chn->aen_buff_status[i] = BUFF_HOLD;
            PCIV_SPIN_UNLOCK;
            return HI_SUCCESS;
        }
    }
    PCIV_SPIN_UNLOCK;
    return HI_FAILURE;
}

hi_void pciv_src_pic_send_done(pciv_send_task *task)
{
    hi_pciv_chn     pciv_chn;
    pciv_channel    *chn;
    pciv_pic        recv_pic;
    pciv_pic        src_pic;

    pciv_chn = task->prv_data[0];
    HI_ASSERT((pciv_chn >= 0) && (pciv_chn < PCIV_MAX_CHN_NUM));

    chn = &g_pciv_chn[pciv_chn];

    osal_memcpy(&recv_pic, (pciv_pic *)task->pv_prv_data, sizeof(pciv_pic));
    kfree(task->pv_prv_data);
    task->pv_prv_data = HI_NULL;
    (void)pciv_drv_adp_dma_end_notify(&chn->pciv_attr.remote_obj, &recv_pic);

    src_pic.phy_addr = task->src_phy_addr;
    src_pic.pool_id = task->prv_data[1];
    (hi_void)pciv_src_pic_free(pciv_chn, &src_pic);

    chn->resp_cnt++;

    return;
}

static hi_s32 pciv_src_pic_send_pre(hi_pciv_chn pciv_chn, pciv_pic *src_pic, hi_u32 *cur_index, pciv_pic **recv_pic)
{
    hi_s32          ret;
    pciv_channel    *chn = &g_pciv_chn[pciv_chn];

    if (chn->start != HI_TRUE) {
        PCIV_ERR_TRACE("pciv chn: %d is not enable!\n", pciv_chn);
        return HI_FAILURE;
    }

    HI_ASSERT(src_pic->src_type < PCIV_BIND_BUTT);
    HI_ASSERT(src_pic->filed < VIDEO_FIELD_BUTT);

    chn->get_cnt++;

    ret = pciv_get_share_buf(pciv_chn, cur_index);
    if (ret != HI_SUCCESS) {
        chn->lost_cnt++;
        PCIV_ERR_TRACE("no free buf, chn%d,src_type:%d\n", pciv_chn, src_pic->src_type);
        return HI_ERR_PCIV_NOBUF;
    }

    *recv_pic = (pciv_pic *)kmalloc(sizeof(pciv_pic), GFP_ATOMIC);
    if (*recv_pic == HI_NULL) {
        PCIV_EMERG_TRACE("kmalloc pciv_recvpic err! chn%d\n", pciv_chn);
        chn->lost_cnt++;
        return HI_ERR_PCIV_NOMEM;
    }
    return HI_SUCCESS;
}

static hi_void pciv_set_recv_pic_info(pciv_channel *chn, hi_s32 cur_index, pciv_pic *src_pic, pciv_pic *recv_pic)
{
    recv_pic->phy_addr      = 0;
    recv_pic->pool_id       = 0;
    recv_pic->index         = cur_index;
    recv_pic->count         = chn->send_cnt;
    recv_pic->pts           = src_pic->pts;
    recv_pic->time_ref      = src_pic->time_ref;
    recv_pic->src_type      = src_pic->src_type;
    recv_pic->filed         = src_pic->filed;
    recv_pic->mod_id        = src_pic->mod_id;
    recv_pic->block         = src_pic->block;
    recv_pic->color_gamut   = src_pic->color_gamut;
    recv_pic->compress_mode = src_pic->compress_mode;
    recv_pic->dynamic_range = src_pic->dynamic_range;
    recv_pic->video_format  = src_pic->video_format;
    recv_pic->pixel_format  = src_pic->pixel_format;
}

/* after dealing with the source image,the interface is auto called to send
the image to the PCI target by PCI DMA mode */
hi_s32 pciv_src_pic_send(hi_pciv_chn pciv_chn, pciv_pic *src_pic)
{
    hi_ulong        flags;
    hi_s32          ret;
    hi_u32          cur_index;
    pciv_send_task  pci_task;
    pciv_channel    *chn        = &g_pciv_chn[pciv_chn];
    pciv_pic        *recv_pic   = HI_NULL;

    /* pay attention to the possibility of the pciv and vfwm called each othe */
    PCIV_SPIN_LOCK;
    ret = pciv_src_pic_send_pre(pciv_chn, src_pic, &cur_index, &recv_pic);
    if (ret != HI_SUCCESS) {
        PCIV_SPIN_UNLOCK;
        return ret;
    }
    pciv_set_recv_pic_info(chn, cur_index, src_pic, recv_pic);

    /* hide the channel,that is the PCIV channel will not send the image to the
    target by DMA mode, only go on message-based communication */
    pci_task.len = chn->hide ? 0 : chn->pciv_attr.blk_size;
    pci_task.src_phy_addr = src_pic->phy_addr;
    pci_task.dst_phy_addr = chn->pciv_attr.phy_addr[cur_index];
    pci_task.read = HI_FALSE;
    pci_task.prv_data[0] = pciv_chn;            /* channel num */
    pci_task.prv_data[1] = src_pic->pool_id; /* src image pool_id */
    pci_task.pv_prv_data = (hi_void *)recv_pic;
    pci_task.call_back = pciv_src_pic_send_done; /* register PCI DMA finished callback */
    ret = pciv_drv_adp_add_dma_task(&pci_task);
    if (ret != HI_SUCCESS) {
        PCIV_EMERG_TRACE("DMA task err! chn%d\n", pciv_chn);
        kfree(recv_pic);
        chn->lost_cnt++;
        PCIV_SPIN_UNLOCK;
        return ret;
    }

    /* set the serial number of the buffer not idle state */
    HI_ASSERT((chn->aen_buff_status[cur_index] == BUFF_HOLD) || (chn->aen_buff_status[cur_index] == BUFF_FREE));
    chn->aen_buff_status[cur_index] = BUFF_BUSY;
    chn->send_cnt++;
    PCIV_SPIN_UNLOCK;

    return HI_SUCCESS;
}

/* after the PCV DMA task finished,this interface is been called to release the image buffer */
hi_s32 pciv_src_pic_free(hi_pciv_chn pciv_chn, pciv_pic *src_pic)
{
    return pciv_firmware_src_pic_free(pciv_chn, src_pic);
}

/* after recieving the image through the_pciv channel, this interface is been called to send the image
    to VO for display or VPSS for using or VENC for coding */
hi_s32 pciv_receive_pic(hi_pciv_chn pciv_chn, pciv_pic *recv_pic)
{
    hi_s32          ret;
    hi_ulong        flags;
    pciv_channel    *chn;

    HI_ASSERT(pciv_chn < PCIV_MAX_CHN_NUM);
    HI_ASSERT(recv_pic->filed < VIDEO_FIELD_BUTT);
    HI_ASSERT(recv_pic->src_type < PCIV_BIND_BUTT);
    HI_ASSERT(recv_pic->index < g_pciv_chn[pciv_chn].pciv_attr.count);

    PCIV_SPIN_LOCK;
    chn = &g_pciv_chn[pciv_chn];

    /* ***********************************************************************************
     *  [HSCP201308020003]  l00181524,2013.08.16,when the master chip and slave chip re-creat and re-destroy, it is possibile the message is store in the buffer memory,
     * it will lead that when master chip is not booted or just created, at the same time, it recieved the slave chip image before destroying, it will occupy the buffer, but the
     * buffer-index idle-flag is true, in this situation, will appear that it will used the buffer-index after the slave chip  re-creat, but the  master buffer-index is been occupied,
     * then an assert will occur,
     * so we introduce the can_recv flag, when the slave chip is re-start, it will send a message to the master chip,notice the master chip to release all vb to keep synchronous
     * with the slave chip, because the mechanism of recieving message is trig by software interrupt, but 3531 and 3536 is double core, can_recv is needed to do mutual exclusion
     * on the double core system.
     ************************************************************************************* */
    if ((chn->start != HI_TRUE) || (chn->can_recv != HI_TRUE)) {
        chn->aen_buff_status[recv_pic->index] = BUFF_BUSY;
        PCIV_SPIN_UNLOCK;
        recv_pic->pts = 0;
        pciv_recv_pic_free(pciv_chn, recv_pic);
        PCIV_ERR_TRACE("pciv_chn:%d hasn't be ready to receive pic, start: %d, can_recv: %d\n",
            pciv_chn, chn->start, chn->can_recv);
        return HI_ERR_PCIV_SYS_NOTREADY; /* the vdec situation must be in our consideration */
    }
    chn->get_cnt++;

    /* before this, the image buffer-flag is idle, pay attention to this situation,
    if the called order is not reasonable, and the upper do not assure that master chip and slave chip re-creat when return the HI_SUCCESS,at this time ,it is possible occur assert */
    HI_ASSERT(chn->aen_buff_status[recv_pic->index] == BUFF_FREE);

    /* in spite of sending to vo display success or not, here must set the buffer-flag to idle */
    chn->aen_buff_status[recv_pic->index] = BUFF_BUSY;
    PCIV_SPIN_UNLOCK;

    /* the firmware interface is been called to send the image to VO display */
    ret = pciv_firmware_recv_pic_and_send(pciv_chn, recv_pic);
    PCIV_SPIN_LOCK;
    if (ret != HI_SUCCESS) {
        chn->lost_cnt++;
        PCIV_ERR_TRACE("pciv_firmware_recv_pic_and_send err,chn:%d, return value: 0x%x \n", pciv_chn, ret);
        PCIV_SPIN_UNLOCK;
        return ret;
    }

    chn->send_cnt++;
    PCIV_SPIN_UNLOCK;
    return HI_SUCCESS;
}

/* after used by VO or VPSS or VENC, in the firmware, this interface is been called auto to return the image buffer */
hi_s32 pciv_recv_pic_free(hi_pciv_chn pciv_chn, pciv_pic *recv_pic)
{
    hi_s32          ret;
    pciv_channel    *chn    = HI_NULL;

    PCIV_CHECK_CHNID(pciv_chn);

    chn = &g_pciv_chn[pciv_chn];

    HI_ASSERT(recv_pic->index < chn->pciv_attr.count);
    HI_ASSERT(recv_pic->pts == 0);

    /* only when the buffer state is been setted to used, the buffer release action  occur */
    if (chn->aen_buff_status[recv_pic->index] != BUFF_BUSY) {
        PCIV_WARN_TRACE("Buffer is not been used, chn%d\n", pciv_chn);
        return HI_ERR_PCIV_BUF_EMPTY;
    }

    /* buffer state is set to idle */
    chn->aen_buff_status[recv_pic->index] = BUFF_FREE;

    recv_pic->count = chn->resp_cnt;

    /* the READDONE message is send to the sender to notice that do free source releated action */
    ret = pciv_drv_adp_buf_free_notify(&chn->pciv_attr.remote_obj, recv_pic);
    if (ret != HI_SUCCESS) {
        PCIV_ERR_TRACE("pciv_drv_adp_buf_free_notify err,chn%d\n", pciv_chn);
        return ret;
    }

    if (chn->can_recv == HI_TRUE && chn->start == HI_TRUE) {
        chn->resp_cnt++;
    }

    return HI_SUCCESS;
}

hi_s32 pciv_init(void)
{
    hi_s32              i;
    pciv_fmw_callback   firmware_call_back;

    spin_lock_init(&g_pciv_lock);
    INIT_LIST_HEAD(&g_list_head_user_dma);
    for (i = 0; i < PCIV_MAX_DMA_TASK; i++) {
        init_waitqueue_head(&g_user_dma_pool[i].wq_dma_done);
        g_user_dma_pool[i].dma_done = HI_TRUE;
        list_add_tail(&g_user_dma_pool[i].list, &g_list_head_user_dma);
    }

    osal_memset(g_pciv_chn, 0, sizeof(g_pciv_chn));
    for (i = 0; i < PCIV_MAX_CHN_NUM; i++) {
        g_pciv_chn[i].create = HI_FALSE;
        g_pciv_chn[i].pciv_attr.remote_obj.chip_id = -1;

        // DTS2016060809388
        g_pciv_chn[i].pciv_attr.remote_obj.pciv_chn = -1;

        sema_init(&g_pciv_chn[i].pciv_mutex, 1);
    }

    firmware_call_back.pf_src_send_pic = pciv_src_pic_send;
    firmware_call_back.pf_recv_pic_free = pciv_recv_pic_free;
    firmware_call_back.pf_query_pciv_chn_share_buf_state = pciv_get_share_buf_state;
    (hi_void)pciv_firmware_register_func(&firmware_call_back);

    pciv_drv_adp_init();

    return HI_SUCCESS;
}

hi_void pciv_exit(void)
{
    hi_s32 i, ret;

    for (i = 0; i < PCIV_MAX_CHN_NUM; i++) {
        if (g_pciv_chn[i].create != HI_TRUE) {
            msleep(PCIV_SLEEP_TIME);
            continue;
        }

        ret = pciv_stop(i);
        if (ret != HI_SUCCESS) {
            PCIV_ERR_TRACE("pciv_stop err,chn%d\n", i);
            return;
        }

        ret = pciv_destroy(i);
        if (ret != HI_SUCCESS) {
            PCIV_ERR_TRACE("pciv_destroy err,chn%d\n", i);
            return;
        }
    }

    pciv_drv_adp_exit();
    return;
}

#ifndef DISABLE_DEBUG_INFO

static hi_char *print_pix_format(hi_pixel_format pix_formt)
{
    switch (pix_formt) {
        case PIXEL_FORMAT_YVU_SEMIPLANAR_420: return "sp420";
        case PIXEL_FORMAT_YVU_SEMIPLANAR_422: return "sp422";
        case PIXEL_FORMAT_YVU_PLANAR_420:     return "p420";
        case PIXEL_FORMAT_YVU_PLANAR_422:     return "p422";
        case PIXEL_FORMAT_UYVY_PACKAGE_422:   return "uyvy422";
        case PIXEL_FORMAT_YUYV_PACKAGE_422:   return "yuyv422";
        case PIXEL_FORMAT_VYUY_PACKAGE_422:   return "vyuy422";
        default: return NULL;
    }
}

inline static hi_char *print_field(hi_video_field field)
{
    switch (field) {
        case VIDEO_FIELD_TOP:        return "top";
        case VIDEO_FIELD_BOTTOM:     return "bot";
        case VIDEO_FIELD_FRAME:      return "frm";
        case VIDEO_FIELD_INTERLACED: return "intl";
        default: return NULL;
    }
}

inline static hi_char *print_hide(hi_bool hide)
{
    if (hide) {
        return "Y";
    } else {
        return "N";
    }
}

static hi_void pciv_proc_show_chn_attr(osal_proc_entry_t *s)
{
    pciv_channel    *chn;
    hi_pciv_attr    *attr;
    hi_pciv_chn     pciv_chn;

    osal_seq_printf(s, "\n-----PCIV CHN ATTR--------------------------------------------------------------\n");
    osal_seq_printf(s, "%6s"     "%8s"    "%8s"     "%8s"     "%8s"
                    "%8s"     "%8s"     "%10s"     "%17s" "\n",
                    "PciChn", "Width", "Height", "Stride", "Field",
                    "PixFmt", "BufCnt", "BufSize", "PhyAddr0");

    for (pciv_chn = 0; pciv_chn < PCIV_MAX_CHN_NUM; pciv_chn++) {
        chn = &g_pciv_chn[pciv_chn];

        if (chn->create == HI_FALSE) {
            continue;
        }

        attr = &chn->pciv_attr;
        osal_seq_printf(s, "%6d" "%8d" "%8d" "%8d" "%8s" "%8s" "%8d" "%10d" "%17llx" "\n",
                        pciv_chn,
                        attr->pic_attr.width,
                        attr->pic_attr.height,
                        attr->pic_attr.stride[0],
                        print_field(attr->pic_attr.field),
                        print_pix_format(attr->pic_attr.pixel_format),
                        attr->count,
                        attr->blk_size,
                        attr->phy_addr[0]);
    }
}

static hi_void pciv_proc_show_chn_status(osal_proc_entry_t *s)
{
    hi_s32          i;
    pciv_channel    *chn;
    hi_pciv_chn     pciv_chn;
    hi_char         c_string[PCIV_PROC_SHOW_NUM] = { 0 };

    osal_seq_printf(s, "\n-----PCIV CHN STATUS------------------------------------------------------------\n");
    osal_seq_printf(s, "%6s"    "%8s"   "%8s"   "%8s"   "%12s"  "%12s"
                    "%12s"     "%12s"     "%12s"     "%12s"    "\n",
                    "PciChn",  "bHide",   "RemtChp", "RemtChn", "GetCnt",
                    "SendCnt", "RespCnt", "LostCnt", "NtfyCnt", "BuffStatus");

    for (pciv_chn = 0; pciv_chn < PCIV_MAX_CHN_NUM; pciv_chn++) {
        chn = &g_pciv_chn[pciv_chn];
        if (chn->create == HI_FALSE) {
            continue;
        }

        for (i = 0; i < chn->pciv_attr.count; i++) {
            osal_sprintf(&c_string[i << 1], "%2d", chn->aen_buff_status[i]);
        }

        osal_seq_printf(s, "%6d" "%8s"  "%8d" "%8d" "%12d" "%12d" "%12d" "%12d" "%12d" "%12s" "\n",
                        pciv_chn,
                        print_hide(chn->hide),
                        chn->pciv_attr.remote_obj.chip_id,
                        chn->pciv_attr.remote_obj.pciv_chn,
                        chn->get_cnt,
                        chn->send_cnt,
                        chn->resp_cnt,
                        chn->lost_cnt,
                        chn->notify_cnt,
                        c_string);
    }
}

static hi_void pciv_proc_show_msg_status(osal_proc_entry_t *s)
{
    pciv_channel    *chn;
    hi_pciv_chn     pciv_chn;

    osal_seq_printf(s, "\n-----PCIV MSG STATUS------------------------------------------------------------\n");
    osal_seq_printf(s, "%6s"     "%11s"      "%11s"     "%11s"
                    "%11s"      "%11s"      "%11s"    "\n",
                    "PciChn", "RdoneGap", "MaxRDGap", "MinRDGap",
                    "WdoneGap", "MaxWDGap", "MinWDGap");

    for (pciv_chn = 0; pciv_chn < PCIV_MAX_CHN_NUM; pciv_chn++) {
        chn = &g_pciv_chn[pciv_chn];
        if (chn->create == HI_FALSE) {
            continue;
        }

        osal_seq_printf(s, "%6d" "%11u" "%11u" "%11u" "%11u" "%11u" "%11u" "\n",
                        pciv_chn,
                        g_rdone_gap[pciv_chn],
                        g_max_rdone_gap[pciv_chn],
                        g_min_rdone_gap[pciv_chn],
                        g_wdone_gap[pciv_chn],
                        g_max_wdone_gap[pciv_chn],
                        g_min_wdone_gap[pciv_chn]);
    }
}

hi_s32 pciv_proc_show(osal_proc_entry_t *s)
{
    osal_seq_printf(s, "\n[PCIV] Version: [" MPP_VERSION "], Build Time:["__DATE__", "__TIME__"]\n\n");

    pciv_proc_show_chn_attr(s);
    pciv_proc_show_chn_status(s);
    pciv_proc_show_msg_status(s);

    return HI_SUCCESS;
}

#endif

