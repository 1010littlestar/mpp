/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2008-2019. All rights reserved.
 * Description   : Connect user mode and kernel mode
 * Author        : Hisilicon multimedia software group
 * Created       : 2008/06/16
 */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/version.h>

#include "pciv_ext.h"
#include "hi_osal.h"
#include "hi_errno.h"
#include "hi_debug.h"
#include "mod_ext.h"
#include "dev_ext.h"
#include "proc_ext.h"
#include "pciv.h"
#include "pciv_drvadp.h"
#include "mkp_pciv.h"

typedef enum {
    PCIV_STATE_STARTED  = 0,
    PCIV_STATE_STOPPING = 1,
    PCIV_STATE_STOPPED  = 2
} pciv_state;

static osal_dev_t       *g_ast_pciv_device;
static pciv_state       g_pciv_state = PCIV_STATE_STOPPED;
static osal_atomic_t    g_pciv_user_ref = OSAL_ATOMIC_INIT(0);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
    static DECLARE_MUTEX(g_pciv_ioctl_mutex);
#else
    static DEFINE_SEMAPHORE(g_pciv_ioctl_mutex);
#endif

#define PCIV_IOCTL_DWON()                           \
do {                                                \
    hi_s32 tmp_;                                    \
    tmp_ = down_interruptible(&g_pciv_ioctl_mutex); \
    if (tmp_) {                                     \
        osal_atomic_dec_return(&g_pciv_user_ref);   \
        return tmp_;                                \
    }                                               \
} while (0)

#define PCIV_IOCTL_UP() up(&g_pciv_ioctl_mutex)

static int pciv_open(hi_void *private_data)
{
    return 0;
}

static int pciv_close(hi_void *private_data)
{
    return 0;
}

static hi_s32 pciv_bind_chn_to_fd_ctrl(hi_void *private_data, hi_void *arg)
{
    hi_pciv_chn chn;

    chn = UMAP_GET_CHN(private_data);
    if ((chn < 0) || (chn >= PCIV_MAX_CHN_NUM)) {
        osal_printk("invalid chn id:%d \n", chn);
        return HI_ERR_PCIV_INVALID_CHNID;
    }
    UMAP_SET_CHN(private_data, *((hi_u32 *)arg));
    return HI_SUCCESS;
}

static hi_s32 pciv_create_ctrl(hi_pciv_chn chn, hi_void *arg)
{
    hi_s32 ret;
    hi_pciv_attr *attr = (hi_pciv_attr *)arg;

    ret = pciv_create(chn, attr);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV create failed !\n");
    }
    return ret;
}

static hi_s32 pciv_destroy_ctrl(hi_pciv_chn chn)
{
    hi_s32 ret;

    ret = pciv_destroy(chn);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV destroy failed !\n");
    }
    return ret;
}

static hi_s32 pciv_set_attr_ctrl(hi_pciv_chn chn, hi_void *arg)
{
    hi_s32 ret;
    hi_pciv_attr *attr = (hi_pciv_attr *)arg;

    ret = pciv_set_attr(chn, attr);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV set_attr failed !\n");
    }
    return ret;
}

static hi_s32 pciv_get_attr_ctrl(hi_pciv_chn chn, hi_void *arg)
{
    hi_s32 ret;
    hi_pciv_attr *attr = (hi_pciv_attr *)arg;

    ret = pciv_get_attr(chn, attr);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV get_attr failed !\n");
    }
    return ret;
}

static hi_s32 pciv_start_ctrl(hi_pciv_chn chn)
{
    hi_s32 ret;

    ret = pciv_start(chn);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV start failed !\n");
    }
    return ret;
}

static hi_s32 pciv_stop_ctrl(hi_pciv_chn chn)
{
    hi_s32 ret;

    ret = pciv_stop(chn);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV stop failed !\n");
    }
    return ret;
}

static hi_s32 pciv_malloc_ctrl(hi_void *arg)
{
    hi_s32 ret;
    hi_pciv_ioctl_malloc *malloc_buf = (hi_pciv_ioctl_malloc *)arg;

    if (malloc_buf == HI_NULL) {
        osal_printk("malloc is NULL!\n\n");
        return HI_ERR_PCIV_NULL_PTR;
    }
    ret = pciv_malloc(malloc_buf->size, &(malloc_buf->phy_addr));
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV malloc failed !\n");
    }
    return ret;
}

static hi_s32 pciv_free_ctrl(hi_void *arg)
{
    hi_s32 ret;
    hi_u64 *phy_addr = (hi_u64 *)arg;

    ret = pciv_free(*phy_addr);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV free failed !\n");
    }
    return ret;
}

static hi_s32 pciv_malloc_chn_buf_ctrl(hi_void *arg)
{
    hi_s32 ret;
    hi_pciv_ioctl_malloc_chn_buf *malloc_chn_buf = (hi_pciv_ioctl_malloc_chn_buf *)arg;

    if (malloc_chn_buf == HI_NULL) {
        osal_printk("malloc is NULL!\n\n");
        return HI_ERR_PCIV_NULL_PTR;
    }
    ret = pciv_malloc_chn_buffer(malloc_chn_buf->chn_id, malloc_chn_buf->index, malloc_chn_buf->size,
                                 &(malloc_chn_buf->phy_addr));
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV malloc_chn_buffer failed !\n");
    }
    return ret;
}

static hi_s32 pciv_free_chn_buf_ctrl(hi_pciv_chn chn, hi_void *arg)
{
    hi_s32 ret;
    hi_u32 *index = (hi_u32 *)arg;

    ret = pciv_free_chn_buffer(chn, *index);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV free_chn_buffer failed !\n");
    }
    return ret;
}

static hi_s32 pciv_get_base_window_ctrl(hi_void *arg)
{
    hi_s32 ret;
    hi_pciv_base_window *base = (hi_pciv_base_window *)arg;

    ret = pciv_drv_adp_get_base_window(base);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV_GETBASEWINDOW_CTRL ioctl err! ret:0x%x\n\n", ret);
    }
    return ret;
}

static hi_s32 pciv_dma_task_ctrl(hi_void *arg)
{
    hi_s32 ret = -ENOIOCTLCMD;
    hi_u32 cpy_size;
    hi_u32 i;
    hi_pciv_dma_task *task;
    static hi_pciv_dma_block dma_blk[PCIV_MAX_DMABLK];

    task = (hi_pciv_dma_task *)arg;
    if (task == HI_NULL) {
        osal_printk("task->block is NULL!\n\n");
        return HI_ERR_PCIV_NULL_PTR;
    }

    if (task->block == HI_NULL) {
        osal_printk("task->block is NULL!\n");
        return HI_ERR_PCIV_NULL_PTR;
    }

    if (task->count > PCIV_MAX_DMABLK || task->count == 0) {
        osal_printk("task count err,should be [%d, %d]\n", 1, PCIV_MAX_DMABLK);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if ((task->read != HI_TRUE) && (task->read != HI_FALSE)) {
        osal_printk("read:%d is wrong!\n", task->read);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }

    cpy_size = sizeof(hi_pciv_dma_block) * task->count;
    if (osal_copy_from_user(dma_blk, task->block, cpy_size)) {
        osal_printk("copy from user failed!\n");
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    task->block = dma_blk;
    for (i = 0; i < task->count; i++) {
        if (task->block[i].blk_size == 0) {
            osal_printk("blk_size:%d is illegal!\n\n", task->block->blk_size);
            return HI_ERR_PCIV_ILLEGAL_PARAM;
        }
    }
    if (ret != HI_ERR_PCIV_ILLEGAL_PARAM) {
        ret = pciv_user_dma_task(task);
        if (ret != HI_SUCCESS) {
            osal_printk("PCIV user_dma_task failed !\n");
        }
    }
    return ret;
}

static hi_s32 pciv_get_local_id_ctrl(hi_void *arg)
{
    hi_s32 *local_id = (hi_s32 *)arg;

    *local_id = pciv_drv_adp_get_local_id();
    return HI_SUCCESS;
}

static hi_s32 pciv_enum_chip_id_ctrl(hi_void *arg)
{
    hi_s32 ret;
    hi_pciv_ioctl_enum_chip *chip = (hi_pciv_ioctl_enum_chip *)arg;

    if (chip == HI_NULL) {
        osal_printk("chip is NULL!\n");
        return HI_ERR_PCIV_NULL_PTR;
    }
    ret = pciv_drv_adp_enum_chip(chip->chip_array);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV drv_adp enum_chip failed !\n");
    }
    return ret;
}

static hi_s32 pciv_win_vb_create_ctrl(hi_void *arg)
{
    hi_s32 ret;
    hi_pciv_win_vb_cfg *config = (hi_pciv_win_vb_cfg *)arg;

    ret = pciv_window_vb_create(config);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV window_vb_create failed !\n");
    }
    return ret;
}

static hi_s32 pciv_win_vb_destroy_ctrl(hi_void)
{
    hi_s32 ret;

    ret = pciv_window_vb_destroy();
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV window_vb_destroy failed !\n");
    }
    return ret;
}

static hi_s32 pciv_hide_ctrl(hi_pciv_chn chn, hi_bool is_hide)
{
    hi_s32 ret;

    ret = pciv_hide(chn, is_hide);
    if (ret != HI_SUCCESS) {
        osal_printk("PCIV show failed !\n");
    }
    return ret;
}

static long pciv_dev_ioctl(hi_u32 cmd, hi_ulong argv, hi_void *private_data)
{
    hi_bool     down    = HI_TRUE;
    hi_s32      ret     = -ENOIOCTLCMD;
    hi_void     *arg    = (hi_void *)argv;
    hi_pciv_chn chn     = UMAP_GET_CHN(private_data);

    /* if the system has received the notice to stop or the system has been stopped */
    if (g_pciv_state != PCIV_STATE_STARTED) {
        return HI_ERR_PCIV_SYS_NOTREADY;
    }

    PCIV_IOCTL_DWON();

    switch (cmd) {
        case PCIV_BINDCHN2FD_CTRL:
            ret = pciv_bind_chn_to_fd_ctrl(private_data, arg);
            break;
        case PCIV_CREATE_CTRL:
            ret = pciv_create_ctrl(chn, arg);
            break;
        case PCIV_DESTROY_CTRL:
            ret = pciv_destroy_ctrl(chn);
            break;
        case PCIV_SETATTR_CTRL:
            ret = pciv_set_attr_ctrl(chn, arg);
            break;
        case PCIV_GETATTR_CTRL:
            ret = pciv_get_attr_ctrl(chn, arg);
            break;
        case PCIV_START_CTRL:
            ret = pciv_start_ctrl(chn);
            break;
        case PCIV_STOP_CTRL:
            ret = pciv_stop_ctrl(chn);
            break;
        case PCIV_MALLOC_CTRL:
            ret = pciv_malloc_ctrl(arg);
            break;
        case PCIV_FREE_CTRL:
            ret = pciv_free_ctrl(arg);
            break;
        case PCIV_MALLOC_CHN_BUF_CTRL:
            ret = pciv_malloc_chn_buf_ctrl(arg);
            break;
        case PCIV_FREE_CHN_BUF_CTRL:
            ret = pciv_free_chn_buf_ctrl(chn, arg);
            break;
        case PCIV_GETBASEWINDOW_CTRL:
            ret = pciv_get_base_window_ctrl(arg);
            break;
        case PCIV_DMATASK_CTRL:
            ret = pciv_dma_task_ctrl(arg);
            break;
        case PCIV_GETLOCALID_CTRL:
            ret = pciv_get_local_id_ctrl(arg);
            break;
        case PCIV_ENUMCHIPID_CTRL:
            ret = pciv_enum_chip_id_ctrl(arg);
            break;
        case PCIV_WINVBCREATE_CTRL:
            ret = pciv_win_vb_create_ctrl(arg);
            break;
        case PCIV_WINVBDESTROY_CTRL:
            ret = pciv_win_vb_destroy_ctrl();
            break;
        case PCIV_SHOW_CTRL:
            ret = pciv_hide_ctrl(chn, HI_FALSE);
            break;
        case PCIV_HIDE_CTRL:
            ret = pciv_hide_ctrl(chn, HI_TRUE);
            break;
        default:
            ret = -ENOIOCTLCMD;
            break;
    }

    if (down == HI_TRUE) {
        PCIV_IOCTL_UP();
    }

    return ret;
}

static long pciv_ioctl(hi_u32 cmd, hi_ulong arg, hi_void *private_data)
{
    int ret;

    osal_atomic_inc_return(&g_pciv_user_ref);

    ret = pciv_dev_ioctl(cmd, arg, private_data);
    osal_atomic_dec_return(&g_pciv_user_ref);

    return ret;
}

hi_s32 pciv_ext_init(hi_void *p)
{
    /* as long as it is not in stop state,it will not need to initialize,only retrun success */
    if (g_pciv_state != PCIV_STATE_STOPPED) {
        return HI_SUCCESS;
    }

    if (pciv_init() != HI_SUCCESS) {
        HI_ERR_TRACE(HI_ID_PCIV, "pciv_init failed\n");
        return HI_FAILURE;
    }

    HI_INFO_TRACE(HI_ID_PCIV, "pciv_init success\n");
    g_pciv_state = PCIV_STATE_STARTED;
    return HI_SUCCESS;
}

hi_void pciv_ext_exit(hi_void)
{
    /* if it is stopped ,retrun success,else the exit function is been called */
    if (g_pciv_state == PCIV_STATE_STOPPED) {
        return;
    }
    pciv_exit();
    g_pciv_state = PCIV_STATE_STOPPED;
}

static hi_void pciv_notify(mod_notice_id notice)
{
    g_pciv_state = PCIV_STATE_STOPPING; /* the new IOCT is not continue received */
    /* pay attention to wake all user */
    return;
}

static hi_void pciv_query_state(mod_state *state)
{
    if (osal_atomic_read(&g_pciv_user_ref) == 0) {
        *state = MOD_STATE_FREE;
    } else {
        *state = MOD_STATE_BUSY;
    }
    return;
}

static hi_u32 pciv_get_ver_magic(hi_void)
{
    return VERSION_MAGIC;
}

static hi_s32 pciv_freeze(osal_dev_t *pdev)
{
    HI_PRINT("%s %d\n", __func__, __LINE__);
    return HI_SUCCESS;
}
static hi_s32 pciv_restore(osal_dev_t *pdev)
{
    HI_PRINT("%s %d\n", __func__, __LINE__);
    return HI_SUCCESS;
}
static umap_module g_pciv_module = {
    .mod_id = HI_ID_PCIV,
    .mod_name = "pciv",

    .pfn_init = pciv_ext_init,
    .pfn_exit = pciv_ext_exit,
    .pfn_query_state = pciv_query_state,
    .pfn_notify = pciv_notify,
    .pfn_ver_checker = pciv_get_ver_magic,
    .data = HI_NULL,
};

static struct osal_fileops g_pciv_fops = {
    .open = pciv_open,
    .unlocked_ioctl = pciv_ioctl,
    .release = pciv_close
};

struct osal_pmops g_pciv_drv_ops = {
    .pm_freeze = pciv_freeze,
    .pm_restore = pciv_restore,
};
static int __init pciv_mod_init(hi_void)
{
    hi_s32 ret = HI_SUCCESS;
#ifndef DISABLE_DEBUG_INFO
    osal_proc_entry_t *proc;
#endif
    hi_char buf[20];

    osal_snprintf(buf, 20, "%s", UMAP_DEVNAME_PCIV_BASE);
    g_ast_pciv_device = osal_createdev(buf);
    g_ast_pciv_device->fops = &g_pciv_fops;
    g_ast_pciv_device->minor = UMAP_PCIV_MINOR_BASE;
    g_ast_pciv_device->osal_pmops = &g_pciv_drv_ops;
    if (osal_registerdevice(g_ast_pciv_device) < 0) {
        printk("regist pciv device err.\n");
        osal_destroydev(g_ast_pciv_device);
        return HI_FAILURE;
    }

    if (cmpi_register_module(&g_pciv_module)) {
        osal_deregisterdevice(g_ast_pciv_device);
        osal_destroydev(g_ast_pciv_device);
        printk("regist pciv module err.\n");
        return HI_FAILURE;
    }

#ifndef DISABLE_DEBUG_INFO
    proc = osal_create_proc_entry(PROC_ENTRY_PCIV, NULL);
    if (proc == HI_NULL) {
        cmpi_unregister_module(HI_ID_PCIV);
        osal_deregisterdevice(g_ast_pciv_device);
        osal_destroydev(g_ast_pciv_device);
        printk("PCIV create proc error\n");
        return -1;
    }
    proc->read = pciv_proc_show;
#endif
    ret = osal_atomic_init(&g_pciv_user_ref);
    if (ret != HI_SUCCESS) {
        HI_PRINT("osal_atomic_init failed\n");
#ifndef DISABLE_DEBUG_INFO
        osal_remove_proc_entry(PROC_ENTRY_PCIV, NULL);
#endif
        cmpi_unregister_module(HI_ID_PCIV);
        osal_deregisterdevice(g_ast_pciv_device);
        osal_destroydev(g_ast_pciv_device);
        return -1;
    }
    osal_printk("load pciv.ko  ...ok!\n");
    osal_atomic_set(&g_pciv_user_ref, 0);
    return HI_SUCCESS;
}

static hi_void __exit pciv_mod_exit(hi_void)
{
#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(PROC_ENTRY_PCIV, NULL);
#endif
    cmpi_unregister_module(HI_ID_PCIV);
    osal_atomic_destory(&g_pciv_user_ref);
    osal_deregisterdevice(g_ast_pciv_device);
    osal_destroydev(g_ast_pciv_device);
    osal_printk("unload pciv.ko for %s...ok!\n", CHIP_NAME);
    return;
}

module_init(pciv_mod_init);
module_exit(pciv_mod_exit);

MODULE_AUTHOR("HiMPP GRP");
MODULE_LICENSE("GPL");
MODULE_VERSION(MPP_VERSION);
