/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2008-2019. All rights reserved.
 * Description   : Adapt for DMA
 * Author        : Hisilicon multimedia software group
 * Created       : 2008/06/16
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "sys_ext.h"
#include "hi_comm_pciv_adapt.h"
#include "pci_trans.h"
#include "pciv.h"
#include "pciv_drvadp.h"
#include "hi_mcc_usrdev.h"

#define PCIV_DRV_INVALID_CHIP_ID (-1)
#define PCIV_DRV_BASE_WIN_MIN_OFFSET  0x100000
#define PCIV_DRV_BASE_WIN_MAX_OFFSET  0x800000

struct hi_mcc_handle_attr g_msg_handle_attr = { 0, PCIV_MSGPORT_KERNEL, 0 };

static hi_s32 g_local_id = PCIV_DRV_INVALID_CHIP_ID;
static hi_pciv_base_window g_base_window[HISI_MAX_MAP_DEV];

static hios_mcc_handle_t *g_msg_handle[HISI_MAX_MAP_DEV] = { NULL };

static struct list_head g_list_dma_task;
static spinlock_t g_lock_dma_task;
static spinlock_t g_lock_mcc_msg;

hi_u64 g_rdone_time[PCIV_MAX_CHN_NUM]       = { 0 };
hi_u32 g_rdone_gap[PCIV_MAX_CHN_NUM]        = { 0 };
hi_u32 g_max_rdone_gap[PCIV_MAX_CHN_NUM]    = { 0 };
hi_u32 g_min_rdone_gap[PCIV_MAX_CHN_NUM]    = { 0 };

hi_u64 g_wdone_time[PCIV_MAX_CHN_NUM]       = { 0 };
hi_u32 g_wdone_gap[PCIV_MAX_CHN_NUM]        = { 0 };
hi_u32 g_max_wdone_gap[PCIV_MAX_CHN_NUM]    = { 0 };
hi_u32 g_min_wdone_gap[PCIV_MAX_CHN_NUM]    = { 0 };

hi_void pciv_drv_adp_dma_finish(struct pcit_dma_task *task);

/*  the interupter must be lock when call this function  */
hi_void pciv_drv_adp_start_dma_task(hi_void)
{
    pciv_send_task          *task;
    struct pcit_dma_task    pci_task;

    while (!list_empty(&(g_list_dma_task))) {
        task = list_entry(g_list_dma_task.next, pciv_send_task, list);

        pci_task.dir = task->read ? HI_PCIT_DMA_READ : HI_PCIT_DMA_WRITE;
        pci_task.src = task->src_phy_addr;
        pci_task.dest = task->dst_phy_addr;
        pci_task.len = task->len;
        pci_task.finish = pciv_drv_adp_dma_finish;
        pci_task.private_data = task;                      /*  point to address of task  */
        if (pcit_create_task(&pci_task) == HI_SUCCESS) {
            /*  if create task success, create next task */
            list_del(g_list_dma_task.next);
        } else {
            /* if create task fail, wait next dma task start */
            return;
        }
    }
}

hi_void pciv_drv_adp_dma_finish(struct pcit_dma_task *task)
{
    pciv_send_task *stask = (pciv_send_task *)task->private_data;

    if (stask != NULL) {
        if (stask->call_back != NULL) {
            stask->call_back(stask);
        }
        kfree(stask);
        stask = NULL;
    }
}

hi_s32 pciv_drv_adp_add_dma_task(pciv_send_task *task)
{
    pciv_send_task  *task_tmp;
    hi_ulong        lock_flag;

    if (task->len == 0) {
        task->call_back(task);
        return HI_SUCCESS;
    }

    task_tmp = kmalloc(sizeof(pciv_send_task), GFP_ATOMIC);
    if (!task_tmp) {
        printk("alloc memory send_task failed!\n");
        return HI_FAILURE;
    }
    osal_memcpy(task_tmp, task, sizeof(pciv_send_task));

    spin_lock_irqsave(&(g_lock_dma_task), lock_flag);

    list_add_tail(&(task_tmp->list), &(g_list_dma_task));

    pciv_drv_adp_start_dma_task();

    spin_unlock_irqrestore(&(g_lock_dma_task), lock_flag);

    return HI_SUCCESS;
}

hi_s32 pciv_drv_adp_addr_check(hi_pciv_dma_block *block, hi_bool is_read)
{
    hi_s32 i;
    hi_u64 temp_addr;
    hi_u64 temp_pf_base;
    for (i = 0; i < HISI_MAX_MAP_DEV; i++) {
        if (g_base_window[i].chip_id == PCIV_DRV_INVALID_CHIP_ID) {
            break;
        }
        temp_addr = is_read ? block->src_addr : block->dst_addr;
        temp_pf_base = g_base_window[i].pf_win_base;
        if ((temp_addr >= (temp_pf_base + PCIV_DRV_BASE_WIN_MIN_OFFSET)) &&
            ((temp_addr + block->blk_size) <= (temp_pf_base + PCIV_DRV_BASE_WIN_MAX_OFFSET))) {
            return HI_SUCCESS;
        }
    }
    return HI_FAILURE;
}

/*  it's a half work  */
hi_s32 pciv_drv_adp_send_msg(hi_pciv_remote_obj *remote_obj, pciv_msg_type type, pciv_pic *recv_pic)
{
    static pciv_msg     msg;
    hi_ulong            lock_flag;
    pciv_notify_pic_end *notify = NULL;
    hi_s32              ret     = HI_FAILURE;

    spin_lock_irqsave(&(g_lock_mcc_msg), lock_flag);  //  lock handle
    msg.target = remote_obj->chip_id;
    msg.msg_type = type;
    msg.msg_len = sizeof(pciv_notify_pic_end) + PCIV_MSG_HEADLEN;
    notify = (pciv_notify_pic_end *)msg.c_msg_body;

    osal_memcpy(&notify->pic_info, recv_pic, sizeof(pciv_pic));
    notify->pciv_chn = remote_obj->pciv_chn;

    if (g_local_id == 0) {
        if (g_msg_handle[msg.target] != NULL) {
            /*  danger!! but I don't known how to deal with it if send fail  */
            ret = hios_mcc_sendto(g_msg_handle[msg.target], &msg, msg.msg_len);
        }
    } else {
        if (g_msg_handle[0] != NULL) {
            /*  danger!! but I don't known how to deal with it if send fail  */
            ret = hios_mcc_sendto(g_msg_handle[0], &msg, msg.msg_len);
        }
    }

    if (msg.msg_len == ret) {
        ret = HI_SUCCESS;
    } else {
        printk("Send Msg Error tar:%d, handle:%p, type:%u, len:%u, ret:%d\n",
               msg.target, g_msg_handle[msg.target], type, msg.msg_len, ret);
        panic("-------------------Msg Error---------------------------\n");
    }

    spin_unlock_irqrestore(&(g_lock_mcc_msg), lock_flag);

    return ret;
}

hi_s32 pciv_drv_adp_dma_end_notify(hi_pciv_remote_obj *remote_obj, pciv_pic *recv_pic)
{
    return pciv_drv_adp_send_msg(remote_obj, PCIV_MSGTYPE_WRITEDONE, recv_pic);
}

hi_s32 pciv_drv_adp_buf_free_notify(hi_pciv_remote_obj *remote_obj, pciv_pic *recv_pic)
{
    return pciv_drv_adp_send_msg(remote_obj, PCIV_MSGTYPE_READDONE, recv_pic);
}

static hi_s32 pciv_drv_adp_isr_write_done(pciv_notify_pic_end *notify)
{
    pciv_pic recv_pic;

    hi_u64 time = call_sys_get_time_stamp();

    if (g_wdone_time[notify->pciv_chn] == 0) {
        /*  initial  */
        g_wdone_time[notify->pciv_chn] = time;
        g_min_wdone_gap[notify->pciv_chn] = 0xFFFF;
    }

    g_wdone_gap[notify->pciv_chn] = time - g_wdone_time[notify->pciv_chn];

    g_max_wdone_gap[notify->pciv_chn] =
        (g_wdone_gap[notify->pciv_chn] >
         g_max_wdone_gap[notify->pciv_chn]) ?
        g_wdone_gap[notify->pciv_chn] :
        g_max_wdone_gap[notify->pciv_chn];

    if (g_wdone_gap[notify->pciv_chn] != 0) {
        g_min_wdone_gap[notify->pciv_chn] =
            (g_wdone_gap[notify->pciv_chn] <
             g_min_wdone_gap[notify->pciv_chn]) ?
            g_wdone_gap[notify->pciv_chn] :
            g_min_wdone_gap[notify->pciv_chn];
    }

    g_wdone_time[notify->pciv_chn] = time;

    osal_memcpy(&recv_pic, &notify->pic_info, sizeof(pciv_pic));
    pciv_receive_pic(notify->pciv_chn, &recv_pic);
    return HI_SUCCESS;
}

static hi_s32 pciv_drv_adp_isr_read_done(pciv_notify_pic_end *notify)
{
    hi_u64 time = call_sys_get_time_stamp();

    if (g_rdone_time[notify->pciv_chn] == 0) {
        /*  initial  */
        g_rdone_time[notify->pciv_chn] = time;
        g_min_rdone_gap[notify->pciv_chn] = 0xFFFF;
    }

    g_rdone_gap[notify->pciv_chn] = time - g_rdone_time[notify->pciv_chn];

    g_max_rdone_gap[notify->pciv_chn] =
        (g_rdone_gap[notify->pciv_chn] >
         g_max_rdone_gap[notify->pciv_chn]) ?
        g_rdone_gap[notify->pciv_chn] :
        g_max_rdone_gap[notify->pciv_chn];

    if (g_rdone_gap[notify->pciv_chn] != 0) {
        g_min_rdone_gap[notify->pciv_chn] =
            (g_rdone_gap[notify->pciv_chn] <
             g_min_rdone_gap[notify->pciv_chn]) ?
            g_rdone_gap[notify->pciv_chn] :
            g_min_rdone_gap[notify->pciv_chn];
    }

    g_rdone_time[notify->pciv_chn] = time;

    pciv_free_share_buf(notify->pciv_chn, notify->pic_info.index, notify->pic_info.count);
    return HI_SUCCESS;
}


hi_s32 pciv_drv_adp_msg_isr(hi_void *handle, hi_void *buf, hi_u32 data_len)
{
    hi_s32              ret     = HI_FAILURE;
    pciv_msg            *msg    = (pciv_msg *)buf;
    pciv_notify_pic_end *notify = (pciv_notify_pic_end *)msg->c_msg_body;

    if (msg->target > HISI_MAX_MAP_DEV) {
        printk("no this target %u\n", msg->target);
        return -1;
    }

    if (g_local_id != msg->target) {
        if ((g_local_id != 0)) {
            /* On the slave chip, only receive the master chip message */
            printk("who are you? target=%u\n", msg->target);
            return 0;
        }

        /* On the master chip, if the message is from other slave chip, retransmission the message */
        if (msg->target >= HISI_MAX_MAP_DEV) {
            printk("msg->target=%u\n", msg->target);
            HI_ASSERT(0);
        }
        if (g_msg_handle[msg->target] != NULL) {
            hi_ulong lock_flag;

            /*  Danger!! But I don't known how to deal with it if send fail  */
            spin_lock_irqsave(&(g_lock_mcc_msg), lock_flag);

            (void)hios_mcc_sendto(g_msg_handle[msg->target], buf, data_len);

            spin_unlock_irqrestore(&(g_lock_mcc_msg), lock_flag);
        }
        return HI_SUCCESS;
    }
    switch (msg->msg_type) {
        /*
         * The end of sending call the DMA task interface to send the WRITEDONE message to the receive end,
         * The local receiving end affter getting the message, get the image information, send to VO to display.
         */
        case PCIV_MSGTYPE_WRITEDONE: {
            ret = pciv_drv_adp_isr_write_done(notify);
            break;
        }

        /*
         * The end of reciving the image after display the image on VO, then send the READDONE message to the end of sending
         * The local end of sending after getting the message, get the buffer information from the message and update the buffer state idle
         */
        case PCIV_MSGTYPE_READDONE: {
            ret = pciv_drv_adp_isr_read_done(notify);
            break;
        }
        case PCIV_MSGTYPE_FREE: {
            ret = pciv_free_all_buf(notify->pciv_chn);
            break;
        }
        default:
            printk("Unknown message:%u\n", msg->msg_type);
            break;
    }

    if (ret != HI_SUCCESS) {
        printk("Unknown how to process\n");
    }
    return ret;
}

hi_s32 pciv_drv_adp_get_base_window(hi_pciv_base_window *base_win)
{
    hi_s32 i, ret;

    HI_ASSERT(base_win != NULL);

    ret = HI_ERR_PCIV_ILLEGAL_PARAM;
    if (base_win->chip_id < 0 || base_win->chip_id >= HISI_MAX_MAP_DEV) {
        printk("invalid pcie device id:%d.\n", base_win->chip_id);
        return HI_ERR_PCIV_NOT_PERM;
    }

    for (i = 0; i < HISI_MAX_MAP_DEV; i++) {
        if (g_base_window[i].chip_id == base_win->chip_id) {
            osal_memcpy(base_win, &g_base_window[i], sizeof(hi_pciv_base_window));
            ret = HI_SUCCESS;
            break;
        }
    }
    if (ret != HI_SUCCESS) {
        printk("illegal pcie device id:%d\n", base_win->chip_id);
    }
    return ret;
}

hi_s32 pciv_drv_adp_get_local_id(hi_void)
{
    return g_local_id;
}

hi_s32 pciv_drv_adp_enum_chip(hi_s32 chip_array[PCIV_MAX_CHIPNUM])
{
    hi_s32 i;
    for (i = 0; i < HISI_MAX_MAP_DEV; i++) {
        chip_array[i] = PCIV_DRV_INVALID_CHIP_ID;

        if (g_base_window[i].chip_id == PCIV_DRV_INVALID_CHIP_ID) {
            break;
        }

        chip_array[i] = g_base_window[i].chip_id;
    }
    return HI_SUCCESS;
}

hi_s32 pciv_drv_adp_check_remote(hi_s32 remote_id)
{
    hi_s32 ret;

    if (!g_msg_handle[remote_id]) {
        printk("%s g_msg_handle is NULL \n", __FUNCTION__);
        return HI_ERR_PCIV_NOT_PERM;
    }

    ret = hios_mcc_check_remote(remote_id, g_msg_handle[remote_id]);
    if (ret != HI_SUCCESS) {
        printk("Err : pci target id:%d not checked !!! \n", remote_id);
        return ret;
    }
    return HI_SUCCESS;
}

static hi_s32 pciv_drv_adp_host_init(hi_pciv_base_window *base_win, int n_remot_id[])
{
    hi_s32                  i;
    hios_mcc_handle_opt_t   stopt;

    for (i = 0; i < HISI_MAX_MAP_DEV; i++) {
        if (n_remot_id[i] != PCIV_DRV_INVALID_CHIP_ID) {
            hios_mcc_check_remote(n_remot_id[i], NULL);

            g_msg_handle_attr.target_id = n_remot_id[i];
            g_msg_handle[n_remot_id[i]] = hios_mcc_open(&g_msg_handle_attr);
            if (g_msg_handle[n_remot_id[i]] == NULL) {
                printk("hios_mcc_open err, id:%d\n", n_remot_id[i]);
                continue;
            }

            stopt.recvfrom_notify = pciv_drv_adp_msg_isr;
            hios_mcc_setopt(g_msg_handle[n_remot_id[i]], &stopt);

            base_win->chip_id = n_remot_id[i];
            base_win->pf_win_base = get_pf_window_base(n_remot_id[i]);
            base_win->pf_ahb_addr = 0;

            base_win++;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 pciv_drv_adp_slave_init(hi_pciv_base_window *base_win)
{
    hios_mcc_handle_opt_t stopt;

    g_msg_handle[0] = hios_mcc_open(&g_msg_handle_attr);
    if (g_msg_handle[0] == NULL) {
        printk("can't open mcc device 0!\n");
        return HI_FAILURE;
    }

    stopt.recvfrom_notify = pciv_drv_adp_msg_isr;
    stopt.data = g_local_id;
    hios_mcc_setopt(g_msg_handle[0], &stopt);

    base_win->chip_id = 0;
    base_win->np_win_base = 0;
    base_win->pf_win_base = 0;
    base_win->cfg_win_base = 0;
    base_win->pf_ahb_addr = get_pf_window_base(0);
    return HI_SUCCESS;
}


hi_s32 pciv_drv_adp_init(hi_void)
{
    hi_s32              i, ret;
    hi_s32              n_remot_id[HISI_MAX_MAP_DEV];
    hi_pciv_base_window *base_win = &g_base_window[0];

    INIT_LIST_HEAD(&(g_list_dma_task));
    spin_lock_init(&(g_lock_dma_task));
    spin_lock_init(&g_lock_mcc_msg);

    for (i = 0; i < HISI_MAX_MAP_DEV; i++) {
        n_remot_id[i] = PCIV_DRV_INVALID_CHIP_ID;
        g_base_window[i].chip_id = PCIV_DRV_INVALID_CHIP_ID;
    }

    /*  [HSCP201307020005]  */
    hios_mcc_getremoteids(n_remot_id, NULL);
    if (n_remot_id[0] == 0) { // slave
        hios_mcc_check_remote(0, NULL);
    }

    g_local_id = hios_mcc_getlocalid(NULL);

    /*  pci host ---------------------------------------------------------*/
    if (g_local_id == 0) {
        ret = pciv_drv_adp_host_init(base_win, n_remot_id);
    }

    /*  pci device--------------------------------------------------------*/
    else {
        ret = pciv_drv_adp_slave_init(base_win);
    }
    return ret;
}

hi_void pciv_drv_adp_exit(hi_void)
{
    hi_s32 i;
    for (i = 0; i < HISI_MAX_MAP_DEV; i++) {
        if (g_msg_handle[i] != NULL) {
            hios_mcc_close(g_msg_handle[i]);
        }
    }
    return;
}


