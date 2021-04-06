/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description   : Picture trans and buffer config
 * Author        : Hisilicon multimedia software group
 * Created       : 2009/07/16
 */
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/module.h>

#include "hi_osal.h"
#include "osal_mmz.h"
#include "hi_common_adapt.h"
#include "hi_comm_pciv_adapt.h"
#include "hi_comm_region_adapt.h"
#include "hi_comm_vo_adapt.h"
#include "pciv_firmware.h"
#include "mod_ext.h"
#include "dev_ext.h"
#include "vb_ext.h"
#include "vgs_ext.h"
#include "sys_ext.h"
#include "proc_ext.h"
#include "pciv_fmwext.h"
#include "pciv_pic_queue.h"
#include "vpss_ext.h"
#include "region_ext.h"
#include "mm_ext.h"

typedef enum {
    PCIVFMW_STATE_STARTED   = 0,
    PCIVFMW_STATE_STOPPING  = 1,
    PCIVFMW_STATE_STOPED    = 2,
} pciv_fmw_state;

/* get the function entrance */
#define FUNC_ENTRANCE(type, id) ((type *)(g_ast_modules[id].export_funcs))

static pciv_vb_pool g_vb_pool;

#define FMW_YUV_OFFSET_NUM 3
#define FMW_MEMERY_STRIDE  64
#define FMW_QP_ABS_MAX     51
#define FMW_QP_ABS_MIN     0
#define FMW_QP_REL_MAX     51
#define FMW_QP_REL_MIN     (-51)
#define FMW_ALPHA_MAX      128
#define FMW_ALPHA_MIN      0
#define FMW_GLOBAL_ALPHA   255
#define FMW_PIXL_FMT_NUM   3
#define FMW_RGN_LAYER_MIN  0
#define FMW_RGN_LAYER_MAX  (OVERLAYEX_MAX_NUM_PCIV - 1)

#define FMW_SEND_ING_SLEEP_TIME 10
#define FMW_PCIV_TIMER_EXPIRES  (3 * PCIV_TIMER_EXPIRES)


#define FMW_POINT_START_X_ALIGN        2
#define FMW_POINT_START_Y_ALIGN        2
#define FMW_RGN_SIZE_WIDTH_ALIGN       2
#define FMW_RGN_SIZE_HEIGHT_ALIGN      2
#define FMW_RGN_SIZE_MIN_WIDTH_ALIGN   2
#define FMW_RGN_SIZE_MIN_HEIGHT_ALIGN  2

typedef enum {
    PCIVFMW_SEND_OK = 0,
    PCIVFMW_SEND_NOK,
    PCIVFMW_SEND_ING,
    PCIVFMW_SEND_BUTT
} pciv_fmw_send_state;

typedef struct {
    hi_s32 vgs_in_vb_handle;
    hi_s32 vgs_out_vb_handle;
    hi_u64 in_vb_phy_addr;
    hi_u64 out_vb_phy_addr;
}pciv_fmw_vgs_task;

typedef struct {
    hi_bool create;
    hi_bool start;
    hi_bool block;
    hi_bool master;   /* the flag of master or not */

    hi_u32  rgn_num;   /* number of channel region */
    hi_u32  time_ref;  /* the serial number of VI source image */
    hi_u32  get_cnt;   /* the times of sender get VI image,
                          or the receiver get the image fo VO's dispay */
    hi_u32  send_cnt;  /* the times of sender send the image,
                          or the receiver send to VO displaying */
    hi_u32  resp_cnt;  /* the times of sender finish sending the image and releasing,
                          or the receiver finsh sending to VO displaying */
    hi_u32  lost_cnt;  /* the times of sender fail sending the image,
                          or the receiver fail sending to VO displaying */
    hi_u32  timer_cnt; /* the times of the timer runing to send VDEC image */

    hi_u32  add_job_suc_cnt;    /* success submitting the job */
    hi_u32  add_job_fail_cnt;   /* fail submitting the job */

    hi_u32  move_task_suc_cnt;  /* move task success */
    hi_u32  move_task_fail_cnt; /* move task fail */

    hi_u32  osd_task_suc_cnt;   /* osd  task success */
    hi_u32  osd_task_fail_cnt;  /* osd task  fail */

    hi_u32  zoom_task_suc_cnt;  /* zoom task success */
    hi_u32  zoom_task_fail_cnt; /* zoom task fail */

    hi_u32  end_job_suc_cnt;    /* vgs end job success */
    hi_u32  end_job_fail_cnt;   /* vgs end job fail */

    hi_u32  move_cb_cnt;        /* vgs move callback success */
    hi_u32  osd_cb_cnt;         /* vgs osd callback success */
    hi_u32  zoom_cb_cnt;        /* vgs zoom callback success */

    hi_u32  new_do_cnt;
    hi_u32  old_undo_cnt;

    pciv_fmw_send_state send_state;
    hi_mod_id           mod_id;

    pciv_pic_queue  pic_queue;   /* vdec image queue */
    pciv_pic_node   *cur_vdec_node; /* the current vdec node */

    /* record the tartget image attr after zoom */
    hi_pciv_pic_attr    pic_attr; /* record the target image attr of PCI transmit */
    hi_u32              offset[FMW_YUV_OFFSET_NUM];
    hi_u32              blk_size;
    hi_u32              count; /* the total buffer count */
    hi_u64              phy_addr[PCIV_MAX_BUF_NUM];
    hi_u32              au32_pool_id[PCIV_MAX_BUF_NUM];
    vb_blk_handle       vb_blk_hdl[PCIV_MAX_BUF_NUM]; /* vb handle,used to check the VB is release by VO or not */
    hi_bool             pciv_hold[PCIV_MAX_BUF_NUM];     /* buffer is been hold by the pciv queue or not */
    vgs_online_opt      vgs_opt;

    struct timer_list   buf_timer;
} pciv_fmw_channel;

static pciv_fmw_channel     g_fmw_pciv_chn[PCIV_FMW_MAX_CHN_NUM] = {0};
static struct timer_list    g_timer_pciv_send_vdec_pic;

static osal_spinlock_t g_pciv_fmw_lock;
#define PCIV_FMW_SPIN_LOCK(flags)   osal_spin_lock_irqsave(&(g_pciv_fmw_lock), &(flags))
#define PCIV_FMW_SPIN_UNLOCK(flags) osal_spin_unlock_irqrestore(&(g_pciv_fmw_lock), &(flags))

const int VDEC_MAX_SEND_CNT = 6;

static pciv_fmw_callback g_pciv_fmw_call_back;

static hi_u32 g_pciv_fmw_state = PCIVFMW_STATE_STOPED;

static hi_s32 g_drop_err_timeref = 1;

hi_void pciv_firmware_recv_pic_free(hi_ulong data);
hi_void pciv_fmw_put_region(hi_pciv_chn pciv_chn, hi_rgn_type type);

hi_s32 pciv_check_input_frame_size(hi_pciv_chn pciv_chn, const hi_video_frame *v_frame)
{
    pciv_fmw_channel    *fmw_chn = NULL;
    vb_base_info        base_info;
    hi_vb_cal_config    vb_cal_config;

    HI_ASSERT(v_frame != HI_NULL);
    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    base_info.is_3dnr_buffer = HI_FALSE;
    base_info.align = 0;

    base_info.compress_mode = v_frame->compress_mode;
    base_info.dynamic_range = v_frame->dynamic_range;
    base_info.video_format = v_frame->video_format;

    base_info.pixel_format = v_frame->pixel_format;
    base_info.width = v_frame->width;
    base_info.height = v_frame->height;

    if (!ckfn_sys_get_vb_cfg()) {
        PCIV_FMW_ERR_TRACE("sys_get_vb_cfg is NULL!\n");
        return HI_FAILURE;
    }
    call_sys_get_vb_cfg(&base_info, &vb_cal_config);

    if (vb_cal_config.vb_size > fmw_chn->blk_size) {
        PCIV_FMW_ERR_TRACE("the size(%d) of input image is big than the pciv(%d) chn buff(%d)!\n",
            vb_cal_config.vb_size, pciv_chn, fmw_chn->blk_size);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_void pciv_firmware_wait_send_end(pciv_fmw_channel *fmw_chn)
{
    hi_ulong flags;
    hi_s32   ret;
    while (1) {
        PCIV_FMW_SPIN_LOCK(flags);
        if (fmw_chn->send_state == PCIVFMW_SEND_ING) {
            PCIV_FMW_SPIN_UNLOCK(flags);
            msleep(FMW_SEND_ING_SLEEP_TIME);
            continue;
        } else {
            /* if send fail, release the vdec buffer */
            if ((fmw_chn->send_state == PCIVFMW_SEND_NOK) && (fmw_chn->mod_id == HI_ID_VDEC)) {
                HI_ASSERT(fmw_chn->cur_vdec_node != NULL);
                ret = call_vb_user_sub(fmw_chn->cur_vdec_node->pciv_pic.video_frame.pool_id,
                    fmw_chn->cur_vdec_node->pciv_pic.video_frame.v_frame.phy_addr[0],
                    VB_UID_PCIV);
                HI_ASSERT(ret == HI_SUCCESS);

                pciv_pic_queue_put_free(&fmw_chn->pic_queue, fmw_chn->cur_vdec_node);
                fmw_chn->send_state = PCIVFMW_SEND_OK;
                fmw_chn->cur_vdec_node = NULL;
            }
            PCIV_FMW_SPIN_UNLOCK(flags);
            break;
        }
    }
}

hi_s32 pciv_firmware_reset_chn_queue(hi_pciv_chn pciv_chn)
{
    hi_ulong            flags;
    hi_s32              ret;
    hi_u32              busy_num;
    hi_s32              i           = 0;
    pciv_pic_node       *node       = NULL;
    pciv_fmw_channel    *fmw_chn    = NULL;

    PCIV_FMW_SPIN_LOCK(flags);
    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    if (fmw_chn->start == HI_TRUE) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("pciv_chn:%d has not stop, please stop it first!\n", pciv_chn);
        return HI_FAILURE;
    }
    PCIV_FMW_SPIN_UNLOCK(flags);

    /* it is in the sending proccess,wait for sending finshed */
    pciv_firmware_wait_send_end(fmw_chn);

    PCIV_FMW_SPIN_LOCK(flags);
    /* put the node in the busy queue to free queue */
    busy_num = pciv_pic_queue_get_busy_num(&fmw_chn->pic_queue);

    for (i = 0; i < busy_num; i++) {
        node = pciv_pic_queue_get_busy(&fmw_chn->pic_queue);
        if (node == NULL) {
            PCIV_FMW_SPIN_UNLOCK(flags);
            PCIV_FMW_ERR_TRACE("pciv_pic_queue_get_busy failed! pciv chn %d.\n", pciv_chn);
            return HI_FAILURE;
        }

        if (fmw_chn->master == HI_FALSE) {
            ret = call_vb_user_sub(node->pciv_pic.video_frame.pool_id,
                node->pciv_pic.video_frame.v_frame.phy_addr[0],
                VB_UID_PCIV);
            HI_ASSERT(ret == HI_SUCCESS);
        }

        pciv_pic_queue_put_free(&fmw_chn->pic_queue, node);
        node = NULL;
    }

    PCIV_FMW_SPIN_UNLOCK(flags);

    return HI_SUCCESS;
}

static hi_void pciv_firmware_chn_base_init(pciv_fmw_channel *fmw_chn)
{
    hi_s32 i;

    fmw_chn->start              = HI_FALSE;
    fmw_chn->block              = HI_FALSE;
    /* [HSCP201307040004],l00181524 the member shoud initialize to 0,otherwise int re-creat and re-dostroy and switch the bind-ship situation,
    it may occur receive the wrong image for out-of-order */
    fmw_chn->time_ref           = 0;
    fmw_chn->send_cnt           = 0;
    fmw_chn->get_cnt            = 0;
    fmw_chn->resp_cnt           = 0;
    fmw_chn->lost_cnt           = 0;
    fmw_chn->new_do_cnt         = 0;
    fmw_chn->old_undo_cnt       = 0;
    fmw_chn->send_state         = PCIVFMW_SEND_OK;
    fmw_chn->timer_cnt          = 0;
    fmw_chn->rgn_num            = 0;

    fmw_chn->add_job_suc_cnt    = 0;
    fmw_chn->add_job_fail_cnt   = 0;

    fmw_chn->move_task_suc_cnt  = 0;
    fmw_chn->move_task_fail_cnt = 0;

    fmw_chn->osd_task_suc_cnt   = 0;
    fmw_chn->osd_task_fail_cnt  = 0;

    fmw_chn->zoom_task_suc_cnt  = 0;
    fmw_chn->zoom_task_fail_cnt = 0;

    fmw_chn->end_job_suc_cnt    = 0;
    fmw_chn->end_job_fail_cnt   = 0;

    fmw_chn->move_cb_cnt        = 0;
    fmw_chn->osd_cb_cnt         = 0;
    fmw_chn->zoom_cb_cnt        = 0;

    for (i = 0; i < PCIV_MAX_BUF_NUM; i++) {
        fmw_chn->pciv_hold[i] = HI_FALSE;
    }
}


hi_s32 pciv_firmware_create(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr, hi_s32 local_id)
{
    hi_ulong            flags;
    hi_s32              ret;
    hi_u32              node_num;
    pciv_fmw_channel    *fmw_chn = NULL;

    PCIVFMW_CHECK_CHNID(pciv_chn);

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    if (fmw_chn->create == HI_TRUE) {
        PCIV_FMW_ERR_TRACE("pciv chn%d have already created \n", pciv_chn);
        return HI_ERR_PCIV_EXIST;
    }

    ret = pciv_firmware_set_attr(pciv_chn, attr, local_id);
    if (ret != HI_SUCCESS) {
        PCIV_FMW_ERR_TRACE("attr of pciv chn%d is invalid \n", pciv_chn);
        return ret;
    }

    pciv_firmware_chn_base_init(fmw_chn);

    /* master chip */
    if (local_id == 0) {
        node_num = attr->count;
    } else {
        node_num = VDEC_MAX_SEND_CNT;
    }

    ret = pciv_creat_pic_queue(&fmw_chn->pic_queue, node_num);
    if (ret != HI_SUCCESS) {
        PCIV_FMW_ERR_TRACE("pciv chn%d create pic queue failed\n", pciv_chn);
        return ret;
    }
    fmw_chn->cur_vdec_node = NULL;

    PCIV_FMW_SPIN_LOCK(flags);
    fmw_chn->create = HI_TRUE;
    if (local_id == 0) {
        fmw_chn->master = HI_TRUE;
    } else {
        fmw_chn->master = HI_FALSE;
    }
    PCIV_FMW_SPIN_UNLOCK(flags);
    PCIV_FMW_INFO_TRACE("pciv chn%d create ok \n", pciv_chn);
    return HI_SUCCESS;
}

hi_s32 pciv_firmware_destroy(hi_pciv_chn pciv_chn)
{
    hi_ulong            flags;
    hi_s32              ret;
    pciv_fmw_channel    *fmw_chn = NULL;

    PCIVFMW_CHECK_CHNID(pciv_chn);

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    if (fmw_chn->create == HI_FALSE) {
        return HI_SUCCESS;
    }
    if (fmw_chn->start == HI_TRUE) {
        PCIV_FMW_ERR_TRACE("pciv chn%d is running,you should stop first \n", pciv_chn);
        return HI_ERR_PCIV_NOT_PERM;
    }

    ret = pciv_firmware_reset_chn_queue(pciv_chn);
    if (ret != HI_SUCCESS) {
        PCIV_FMW_ERR_TRACE("pcivfmw chn%d stop failed!\n", pciv_chn);
        return ret;
    }
    PCIV_FMW_SPIN_LOCK(flags);
    pciv_destroy_pic_queue(&fmw_chn->pic_queue);

    fmw_chn->create = HI_FALSE;
    PCIV_FMW_SPIN_UNLOCK(flags);
    PCIV_FMW_INFO_TRACE("pciv chn%d destroy ok \n", pciv_chn);
    return HI_SUCCESS;
}

static hi_s32 pciv_firmware_check_pic_attr(const hi_pciv_attr *attr)
{
    /* Check The Image Width and Height */
    if (!attr->pic_attr.height || !attr->pic_attr.width ||
        (attr->pic_attr.height & 1) || (attr->pic_attr.width & 1)) {
        PCIV_FMW_ERR_TRACE("pic w:%d, h:%d invalid\n",
            attr->pic_attr.width, attr->pic_attr.height);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if (attr->pic_attr.stride[0] < attr->pic_attr.width) {
        PCIV_FMW_ERR_TRACE("pic stride0:%dinvalid\n",
            attr->pic_attr.stride[0]);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if ((attr->pic_attr.stride[0] & 0xf)) {
        PCIV_FMW_ERR_TRACE("illegal param: not align stride(y_stride:%d)\n",
            attr->pic_attr.stride[0]);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if ((attr->pic_attr.pixel_format != PIXEL_FORMAT_YVU_SEMIPLANAR_420)
        && (attr->pic_attr.pixel_format != PIXEL_FORMAT_YVU_SEMIPLANAR_422)) {
        PCIV_FMW_ERR_TRACE("illegal param: illegal pixel_format:%d)\n",
            attr->pic_attr.pixel_format);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if ((attr->pic_attr.compress_mode < COMPRESS_MODE_NONE) ||
        (attr->pic_attr.compress_mode >= COMPRESS_MODE_BUTT)) {
        PCIV_FMW_ERR_TRACE("illegal param: not permmit compress mode:%d\n",
            attr->pic_attr.compress_mode);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if ((attr->pic_attr.dynamic_range < DYNAMIC_RANGE_SDR8) ||
        (attr->pic_attr.dynamic_range >= DYNAMIC_RANGE_BUTT)) {
        PCIV_FMW_ERR_TRACE("illegal param: not permmit dynamic_range mode:%d\n",
            attr->pic_attr.dynamic_range);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if ((attr->pic_attr.video_format < VIDEO_FORMAT_LINEAR) ||
        (attr->pic_attr.video_format >= VIDEO_FORMAT_BUTT)) {
        PCIV_FMW_ERR_TRACE("illegal param: not permmit video_format mode:%d\n",
            attr->pic_attr.video_format);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if ((attr->pic_attr.field < VIDEO_FIELD_TOP) || (attr->pic_attr.field >= VIDEO_FIELD_BUTT)) {
        PCIV_FMW_ERR_TRACE("illegal param: not permmit field mode:%d\n",
            attr->pic_attr.field);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    return HI_SUCCESS;
}

hi_s32 pciv_firmware_check_attr(const hi_pciv_attr *attr)
{
    hi_s32              ret;
    vb_base_info        base_info;
    hi_vb_cal_config    vb_cal_config;

    /* Check The Image attr */
    ret = pciv_firmware_check_pic_attr(attr);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    base_info.is_3dnr_buffer = HI_FALSE;
    base_info.align = 0;

    base_info.compress_mode = attr->pic_attr.compress_mode;
    base_info.dynamic_range = attr->pic_attr.dynamic_range;
    base_info.video_format  = attr->pic_attr.video_format;

    base_info.pixel_format  = attr->pic_attr.pixel_format;
    base_info.width         = attr->pic_attr.width;
    base_info.height        = attr->pic_attr.height;

    if (!ckfn_sys_get_vb_cfg()) {
        PCIV_FMW_ERR_TRACE("sys_get_vb_cfg is NULL!\n");
        return HI_FAILURE;
    }
    call_sys_get_vb_cfg(&base_info, &vb_cal_config);

    /* Check the Image attr is match or not with the buffer size */
    if (attr->blk_size < vb_cal_config.vb_size) {
        PCIV_FMW_ERR_TRACE("Buffer block is too small(%d < %d)\n", attr->blk_size, vb_cal_config.vb_size);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 pciv_firmware_set_fmw_chn(hi_pciv_chn pciv_chn,
    pciv_fmw_channel *fmw_chn, const hi_pciv_attr *attr, hi_s32 local_id)
{
    hi_s32 i, ret;

    osal_memcpy(&fmw_chn->pic_attr, &attr->pic_attr, sizeof(hi_pciv_pic_attr));
    if (local_id == 0) {
        for (i = 0; i < attr->count; i++) {
            if (fmw_chn->phy_addr[i] == 0) {
                PCIV_FMW_ERR_TRACE("pciv channel has not malloc buff\n");
                return HI_ERR_PCIV_NOT_PERM;
            }
            ret = cmpi_check_mmz_phy_addr(attr->phy_addr[i], attr->blk_size);
            if (ret != HI_SUCCESS) {
                PCIV_FMW_ERR_TRACE("attr->phy_addr[%d]:%llx and attr->blk_size:%u is illegal!\n",
                    i, attr->phy_addr[i], attr->blk_size);
                return HI_ERR_PCIV_ILLEGAL_PARAM;
            }
            if (fmw_chn->phy_addr[i] != attr->phy_addr[i]) {
                PCIV_FMW_ERR_TRACE("pciv chn: %d, buffer address: 0x%llx is invalid!\n",
                    pciv_chn, attr->phy_addr[i]);
                return HI_ERR_PCIV_NOT_PERM;
            }
        }
    } else {
        osal_memcpy(fmw_chn->phy_addr, attr->phy_addr, sizeof(attr->phy_addr));
    }
    return HI_SUCCESS;
}


hi_s32 pciv_firmware_set_attr(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr, hi_s32 local_id)
{
    hi_ulong            flags;
    hi_s32              ret;
    pciv_fmw_channel    *fmw_chn = NULL;

    PCIVFMW_CHECK_CHNID(pciv_chn);
    PCIVFMW_CHECK_PTR(attr);

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    PCIV_FMW_SPIN_LOCK(flags);

    /* The channel is in the process of start,it cannot alter */
    if (fmw_chn->start == HI_TRUE) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("pciv chn%d is running\n", pciv_chn);
        return HI_ERR_PCIV_NOT_PERM;
    }

    /* check the valid of attr */
    ret = pciv_firmware_check_attr(attr);
    if (ret != HI_SUCCESS) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        return ret;
    }

    ret = pciv_firmware_set_fmw_chn(pciv_chn, fmw_chn, attr, local_id);
    if (ret != HI_SUCCESS) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        return ret;
    }

    fmw_chn->blk_size = attr->blk_size;
    fmw_chn->count = attr->count;

    /* Set the YUV offset of tartget Image */
    fmw_chn->offset[0] = 0;
    switch (attr->pic_attr.pixel_format) {
        case PIXEL_FORMAT_YVU_SEMIPLANAR_420:
            /* fall through */
        case PIXEL_FORMAT_YVU_SEMIPLANAR_422: {
            /* Sem-planar format do not need u32Offset[2](V offset) */
            fmw_chn->offset[1] = attr->pic_attr.stride[0] * attr->pic_attr.height;
            break;
        }

        default: {
            PCIV_FMW_SPIN_UNLOCK(flags);
            PCIV_FMW_ERR_TRACE("Pixel format(%d) unsupported\n", attr->pic_attr.pixel_format);
            return HI_ERR_PCIV_NOT_SUPPORT;
        }
    }

    PCIV_FMW_SPIN_UNLOCK(flags);

    return HI_SUCCESS;
}

hi_s32 pciv_firmware_start(hi_pciv_chn pciv_chn)
{
    hi_ulong            flags;
    pciv_fmw_channel    *fmw_chn = NULL;

    PCIVFMW_CHECK_CHNID(pciv_chn);

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    if (fmw_chn->create != HI_TRUE) {
        PCIV_FMW_ERR_TRACE("pciv chn%d not create\n", pciv_chn);
        return HI_ERR_PCIV_UNEXIST;
    }

    PCIV_FMW_SPIN_LOCK(flags);
    fmw_chn->start = HI_TRUE;
    PCIV_FMW_SPIN_UNLOCK(flags);
    PCIV_FMW_INFO_TRACE("pciv chn%d start ok \n", pciv_chn);
    return HI_SUCCESS;
}

hi_s32 pciv_firmware_stop(hi_pciv_chn pciv_chn)
{
    hi_ulong flags;

    PCIVFMW_CHECK_CHNID(pciv_chn);

    PCIV_FMW_SPIN_LOCK(flags);
    g_fmw_pciv_chn[pciv_chn].start = HI_FALSE;

    if (g_fmw_pciv_chn[pciv_chn].rgn_num != 0) {
        PCIV_FMW_INFO_TRACE("region number of channel %d is %d, now free the region!\n",
            pciv_chn, g_fmw_pciv_chn[pciv_chn].rgn_num);
        pciv_fmw_put_region(pciv_chn, OVERLAYEX_RGN);
        g_fmw_pciv_chn[pciv_chn].rgn_num = 0;
    }

    PCIV_FMW_SPIN_UNLOCK(flags);
    PCIV_FMW_INFO_TRACE("pcivfmw chn%d stop ok \n", pciv_chn);
    return HI_SUCCESS;
}

hi_s32 pciv_firmware_window_vb_create(hi_pciv_win_vb_cfg *cfg)
{
    hi_s32  ret, i;
    hi_u32  pool_id;
    hi_char *buf_name = "pciv_vb_from_window";

    if (g_vb_pool.pool_count != 0) {
        PCIV_FMW_ERR_TRACE("video buffer pool has created\n");
        return HI_ERR_PCIV_BUSY;
    }

    for (i = 0; i < cfg->pool_count; i++) {
        ret = call_vb_create_pool(&pool_id, cfg->blk_count[i],
            cfg->blk_size[i], RESERVE_MMZ_NAME,
            buf_name, VB_REMAP_MODE_NONE);
        if (ret != HI_SUCCESS) {
            PCIV_FMW_ALERT_TRACE("create pool(index=%d, cnt=%d, size=%d) fail\n",
                i, cfg->blk_count[i], cfg->blk_size[i]);
            break;
        }
        g_vb_pool.pool_count = i + 1;
        g_vb_pool.pool_id[i] = pool_id;
        g_vb_pool.size[i] = cfg->blk_size[i];
    }

    /* if one pool created not success, then rollback */
    if (g_vb_pool.pool_count != cfg->pool_count) {
        for (i = 0; i < g_vb_pool.pool_count; i++) {
            ret = call_vb_destroy_pool(g_vb_pool.pool_id[i]);
            if (ret != HI_SUCCESS) {
                PCIV_FMW_ERR_TRACE("destroy VB pool faild!\n");
                return HI_ERR_PCIV_NOT_PERM;
            }
            g_vb_pool.pool_id[i] = VB_INVALID_POOLID;
        }

        g_vb_pool.pool_count = 0;

        return HI_ERR_PCIV_NOMEM;
    }

    return HI_SUCCESS;
}

hi_s32 pciv_firmware_window_vb_destroy(hi_void)
{
    hi_s32 i;
    hi_s32 j    = 0;
    hi_s32 flag = 0;
    hi_s32 ret  = HI_SUCCESS;

    for (i = 0; i < g_vb_pool.pool_count; i++) {
        ret = call_vb_destroy_pool(g_vb_pool.pool_id[i]);
        if (ret != HI_SUCCESS) {
            PCIV_FMW_ERR_TRACE("destroy VB pool_id[%d]:%d faild!\n", i, g_vb_pool.pool_id[i]);
            j++;
            continue;
        }
        g_vb_pool.pool_id[i] = VB_INVALID_POOLID;
    }

    for (i = 0; i < g_vb_pool.pool_count; i++) {
        flag += (g_vb_pool.pool_id[i] != VB_INVALID_POOLID);
    }
    if (flag) {
        g_vb_pool.pool_count = j;
        return HI_ERR_PCIV_NOT_PERM;
    }

    g_vb_pool.pool_count = 0;
    return HI_SUCCESS;
}

hi_s32 pciv_firmware_malloc(hi_u32 size, hi_s32 local_id, hi_u64 *phy_addr)
{
    hi_s32          i;
    hi_u32          pool_id;
    hi_s32          ret                             = HI_SUCCESS;
    vb_blk_handle   handle                          = VB_INVALID_HANDLE;
    hi_char         az_mmz_name[MAX_MMZ_NAME_LEN]   = {0};

    PCIVFMW_CHECK_PTR(phy_addr);

    if (size == 0) {
        PCIV_FMW_ERR_TRACE("call_vb_get_blk_by_size fail,size:%d!\n", size);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if (local_id == 0) {
        handle = call_vb_get_blk_by_size(size, VB_UID_PCIV, az_mmz_name);
        if (handle == VB_INVALID_HANDLE) {
            PCIV_FMW_ERR_TRACE("call_vb_get_blk_by_size fail,size:%d!\n", size);
            return HI_ERR_PCIV_NOBUF;
        }

        *phy_addr = call_vb_handle_to_phys(handle);

        pool_id = call_vb_handle_to_pool_id(handle);

        ret = call_vb_user_add(pool_id, *phy_addr, VB_UID_USER);
        HI_ASSERT(ret == HI_SUCCESS);
        return HI_SUCCESS;
    }

    /* if in the slave chip, then alloc buffer from special VB */
    for (i = 0; i < g_vb_pool.pool_count; i++) {
        if (size > g_vb_pool.size[i]) {
            continue;
        }

        handle = call_vb_get_blk_by_pool_id(g_vb_pool.pool_id[i], VB_UID_PCIV);

        if (handle != VB_INVALID_HANDLE) {
            break;
        }
    }

    if (handle == VB_INVALID_HANDLE) {
        PCIV_FMW_ERR_TRACE("call_vb_get_blk_by_size fail,size:%d!\n", size);
        return HI_ERR_PCIV_NOBUF;
    }

    *phy_addr = call_vb_handle_to_phys(handle);

    pool_id = call_vb_handle_to_pool_id(handle);

    ret = call_vb_user_add(pool_id, *phy_addr, VB_UID_USER);
    HI_ASSERT(ret == HI_SUCCESS);
    return HI_SUCCESS;
}

hi_s32 pciv_firmware_free(hi_s32 local_id, hi_u64 phy_addr)
{
    hi_ulong        flags;
    vb_blk_handle   vb_handle;
    hi_s32          ret = HI_SUCCESS;

    PCIV_FMW_SPIN_LOCK(flags);
    vb_handle = call_vb_phy_to_handle(phy_addr);
    if (vb_handle == VB_INVALID_HANDLE) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("invalid physical address 0x%llx\n", phy_addr);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }

    if (call_vb_inquire_one_user_cnt(vb_handle, VB_UID_USER) > 0) {
        ret = call_vb_user_sub(call_vb_handle_to_pool_id(vb_handle), phy_addr, VB_UID_PCIV);
        ret |= call_vb_user_sub(call_vb_handle_to_pool_id(vb_handle), phy_addr, VB_UID_USER);
    }
    PCIV_FMW_SPIN_UNLOCK(flags);

    return ret;
}

hi_s32 pciv_firmware_malloc_chn_buffer(hi_pciv_chn pciv_chn, hi_u32 index, hi_u32 size, hi_s32 local_id,
    hi_u64 *phy_addr)
{
    hi_ulong            flags;
    vb_blk_handle       handle;
    hi_char             az_mmz_name[MAX_MMZ_NAME_LEN]   = {0};
    pciv_fmw_channel    *fmw_chn                        = NULL;

    PCIVFMW_CHECK_CHNID(pciv_chn);
    PCIVFMW_CHECK_PTR(phy_addr);

    if (index >= PCIV_MAX_BUF_NUM) {
        PCIV_FMW_ERR_TRACE("the index(%u) of chnbuff illegal!\n", index);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if (size == 0) {
        PCIV_FMW_ERR_TRACE("call_vb_get_blk_by_size fail,size:%d!\n", size);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    PCIV_FMW_SPIN_LOCK(flags);
    if (local_id != 0) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("slave chip: %d, doesn't need chn buffer!\n", local_id);
        return HI_ERR_PCIV_NOT_PERM;
    }

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];
    if (fmw_chn->start == HI_TRUE) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("pciv chn: %d, has start already!\n", pciv_chn);
        return HI_ERR_PCIV_NOT_PERM;
    }

    if (fmw_chn->phy_addr[index] != 0) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("pciv chn: %d, has malloc chn buffer already!\n", pciv_chn);
        return HI_ERR_PCIV_NOT_PERM;
    }

    handle = call_vb_get_blk_by_size(size, VB_UID_PCIV, az_mmz_name);
    if (handle == VB_INVALID_HANDLE) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("call_vb_get_blk_by_size fail,size:%d!\n", size);
        return HI_ERR_PCIV_NOBUF;
    }

    *phy_addr = call_vb_handle_to_phys(handle);
    fmw_chn->phy_addr[index] = *phy_addr;
    PCIV_FMW_SPIN_UNLOCK(flags);

    return HI_SUCCESS;
}

hi_s32 pciv_firmware_free_chn_buffer(hi_pciv_chn pciv_chn, hi_u32 index, hi_s32 local_id)
{
    hi_ulong            flags;
    hi_s32              ret;
    vb_blk_handle       vb_handle;
    pciv_fmw_channel    *fmw_chn = NULL;

    PCIVFMW_CHECK_CHNID(pciv_chn);

    if (index >= PCIV_MAX_BUF_NUM) {
        PCIV_FMW_ERR_TRACE("the index(%u) of chnbuff illegal!\n", index);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }

    PCIV_FMW_SPIN_LOCK(flags);
    if (local_id != 0) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("slave chip: %d, has no chn buffer, doesn't need to free chn buffer!\n", local_id);
        return HI_ERR_PCIV_NOT_PERM;
    }

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];
    if (fmw_chn->start == HI_TRUE) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("pciv chn: %d has not stopped! please stop it first!\n", pciv_chn);
        return HI_ERR_PCIV_NOT_PERM;
    }

    vb_handle = call_vb_phy_to_handle(fmw_chn->phy_addr[index]);
    if (vb_handle == VB_INVALID_HANDLE) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("invalid physical address 0x%llx\n", fmw_chn->phy_addr[index]);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }

    PCIV_FMW_WARN_TRACE("start to free buffer, pool_id: %d, phy_addr: 0x%llx.\n",
        call_vb_handle_to_pool_id(vb_handle), fmw_chn->phy_addr[index]);
    ret = call_vb_user_sub(call_vb_handle_to_pool_id(vb_handle), fmw_chn->phy_addr[index], VB_UID_PCIV);
    PCIV_FMW_WARN_TRACE("finish to free buffer, pool_id: %d, phy_addr: 0x%llx.\n",
        call_vb_handle_to_pool_id(vb_handle), fmw_chn->phy_addr[index]);

    fmw_chn->phy_addr[index] = 0;

    PCIV_FMW_SPIN_UNLOCK(flags);

    return ret;
}

hi_s32 pciv_firmware_put_pic_to_queue(hi_pciv_chn pciv_chn, const hi_video_frame_info *video_frm_info, hi_u32 index,
                                      hi_bool block)
{
    pciv_pic_node       *node       = NULL;
    pciv_fmw_channel    *fmw_chn    = NULL;

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    if (fmw_chn->start != HI_TRUE) {
        return HI_ERR_PCIV_SYS_NOTREADY;
    }

    fmw_chn->pciv_hold[index] = HI_TRUE;
    node = pciv_pic_queue_get_free(&fmw_chn->pic_queue);
    if (node == NULL) {
        PCIV_FMW_ERR_TRACE("pciv_chn:%d no free node\n", pciv_chn);
        return HI_FAILURE;
    }

    node->pciv_pic.mod_id = HI_ID_VDEC;
    node->pciv_pic.block = block;
    node->pciv_pic.index = index;
    osal_memcpy(&node->pciv_pic.video_frame, video_frm_info, sizeof(hi_video_frame_info));

    pciv_pic_queue_put_busy(&fmw_chn->pic_queue, node);

    return HI_SUCCESS;
}

hi_s32 pciv_firmware_get_pic_from_queue_and_send(hi_pciv_chn pciv_chn)
{
    hi_s32              ret             = HI_SUCCESS;
    hi_s32              dev_id          = 0;
    pciv_fmw_channel    *fmw_chn        = NULL;
    hi_video_frame_info *v_frame_info   = NULL;

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    if (fmw_chn->start != HI_TRUE) {
        return HI_ERR_PCIV_SYS_NOTREADY;
    }

    /* send the data in cycle queue, until the data in queue is not less or send fail */
    while (pciv_pic_queue_get_busy_num(&fmw_chn->pic_queue)) {
        fmw_chn->cur_vdec_node = pciv_pic_queue_get_busy(&fmw_chn->pic_queue);
        if (fmw_chn->cur_vdec_node == NULL) {
            PCIV_FMW_ERR_TRACE("pciv_chn:%d busy list is empty, no vdec pic\n", pciv_chn);
            return HI_FAILURE;
        }

        /* send the vdec image to vpss or venc or vo, if success, put the node to fee queue, else nothing to do
        it will send by the time or next interrupt */
        fmw_chn->send_state = PCIVFMW_SEND_ING;
        v_frame_info = &fmw_chn->cur_vdec_node->pciv_pic.video_frame;
        PCIV_FMW_WARN_TRACE("pciv_chn:%d, start: %d, start send pic to VPSS/VO.\n", pciv_chn, fmw_chn->start);
        ret = call_sys_send_data(HI_ID_PCIV, dev_id, pciv_chn, fmw_chn->cur_vdec_node->pciv_pic.block,
            MPP_DATA_VDEC_FRAME, v_frame_info);
        if ((ret != HI_SUCCESS) && (fmw_chn->cur_vdec_node->pciv_pic.block == HI_TRUE)) {
            /* bBlock is ture(playback mode),if failed,get out of the circle,
                do nothing ,it will send by the timer or next DMA interrupt */
            /* set the point NULL,put the node to the head of busy,whie
                send again ,get it from the header of busy */
            PCIV_FMW_WARN_TRACE("pciv_chn:%d, start: %d, finish send pic to VPSS/VO.\n", pciv_chn, fmw_chn->start);
            PCIV_FMW_INFO_TRACE("pciv_chn:%d send pic failed, put to queue and send again. ret: 0x%x\n", pciv_chn, ret);
            pciv_pic_queue_put_busy_head(&fmw_chn->pic_queue, fmw_chn->cur_vdec_node);
            fmw_chn->send_state = PCIVFMW_SEND_NOK;
            fmw_chn->cur_vdec_node = NULL;
            ret = HI_SUCCESS;
            break;
        } else {
            /* bBlock is true(playback mode),if success, put the node to free */
            /* bBlock is false(preview mode),no matter success or not, put the
               node to free,and do not send the Image again */
            PCIV_FMW_WARN_TRACE("pciv_chn:%d, start: %d, finish send pic to VPSS/VO.\n", pciv_chn, fmw_chn->start);
            PCIV_FMW_INFO_TRACE("pciv_chn:%d send pic ok\n", pciv_chn);
            HI_ASSERT(fmw_chn->cur_vdec_node != NULL);
            fmw_chn->pciv_hold[fmw_chn->cur_vdec_node->pciv_pic.index] = HI_FALSE;
            pciv_pic_queue_put_free(&fmw_chn->pic_queue, fmw_chn->cur_vdec_node);
            fmw_chn->send_state = PCIVFMW_SEND_OK;
            fmw_chn->cur_vdec_node = NULL;
            ret = HI_SUCCESS;
        }
    }

    return ret;
}

static hi_s32 pciv_firmware_receiver_send_vdec_pic(hi_pciv_chn pciv_chn,
    hi_video_frame_info *video_frm_info, hi_u32 index, hi_bool block)
{
    hi_s32              ret;
    hi_s32              chn_id;
    hi_u32              busy_num;
    hi_s32              dev_id      = 0;
    pciv_fmw_channel    *fmw_chn    = NULL;

    chn_id  = pciv_chn;
    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    if (fmw_chn->start != HI_TRUE) {
        return HI_ERR_PCIV_SYS_NOTREADY;
    }

     /* When the DMA arrive,first query the queue if has data or not,
        if yes,send the data in the queue first,
        if not,send the cuurent Image directly; if success, return,
        else put the node queue,and trig by the timer next time */
    busy_num = pciv_pic_queue_get_busy_num(&fmw_chn->pic_queue);
    if (busy_num != 0) {
        /* if the current queue has data, put the image to the tail of the queue */
        ret = pciv_firmware_put_pic_to_queue(pciv_chn, video_frm_info, index, block);
        if (ret != HI_SUCCESS) {
            return HI_FAILURE;
        }

        /* Get the data from the header to send */
        ret = pciv_firmware_get_pic_from_queue_and_send(pciv_chn);
        if (ret != HI_SUCCESS) {
            return ret;
        }
    } else {
        /* if the current queue has no data,
           send the current Image directly, if success,return success,
           else return failure,put the Image to the tail of the queue */
        PCIV_FMW_WARN_TRACE("pciv_chn:%d, start: %d, start send pic to VPSS/VO.\n", pciv_chn, fmw_chn->start);
        fmw_chn->pciv_hold[index] = HI_TRUE;
        ret = call_sys_send_data(HI_ID_PCIV, dev_id, chn_id, block, MPP_DATA_VDEC_FRAME, video_frm_info);
        if ((ret != HI_SUCCESS) && (block == HI_TRUE)) {
            /* bBlock is true(playback mode),if failure,put the Image to
               the tail of the queue */
            /* bBlock is false(preview mode),no matter success or not,
               we think it as success, do not put it to queue to send again */
            PCIV_FMW_WARN_TRACE("pciv_chn:%d, start: %d, finish send pic to VPSS/VO.\n",
                pciv_chn, fmw_chn->start);
            PCIV_FMW_INFO_TRACE("pciv_chn:%d send pic failed, put to queue and send again. ret: 0x%x\n",
                pciv_chn, ret);
            if (pciv_firmware_put_pic_to_queue(pciv_chn, video_frm_info, index, block)) {
                return HI_FAILURE;
            }
            ret = HI_SUCCESS;
        } else {
            PCIV_FMW_WARN_TRACE("pciv_chn:%d, start: %d, finish send pic to VPSS/VO.\n",
                pciv_chn, fmw_chn->start);
            PCIV_FMW_INFO_TRACE("pciv_chn:%d send pic ok\n", pciv_chn);
            fmw_chn->pciv_hold[index] = HI_FALSE;
            ret = HI_SUCCESS;
        }
    }

    return ret;
}

static hi_s32 pciv_firmware_recv_get_vb_cal_config(hi_pciv_chn pciv_chn, const pciv_pic *recv_pic,
    hi_vb_cal_config *vb_cal_config)
{
    vb_base_info        base_info;
    pciv_fmw_channel    *fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    base_info.is_3dnr_buffer = HI_FALSE;
    base_info.align = 0;

    base_info.compress_mode = recv_pic->compress_mode;
    base_info.dynamic_range = recv_pic->dynamic_range;
    base_info.video_format = recv_pic->video_format;

    base_info.pixel_format = recv_pic->pixel_format;
    base_info.width = fmw_chn->pic_attr.width;
    base_info.height = fmw_chn->pic_attr.height;

    if (!ckfn_sys_get_vb_cfg()) {
        PCIV_FMW_ERR_TRACE("sys_get_vb_cfg is NULL!\n");
        fmw_chn->lost_cnt++;
        return HI_FAILURE;
    }
    call_sys_get_vb_cfg(&base_info, vb_cal_config);
    return HI_SUCCESS;
}

static hi_void pciv_firmware_recv_set_v_frame(hi_video_frame_info *video_frm_info, pciv_fmw_channel *fmw_chn,
    pciv_pic *recv_pic, vb_blk_handle vb_handle, hi_vb_cal_config *vb_cal_config)
{
    hi_video_frame *vfrm = NULL;

    video_frm_info->pool_id  = call_vb_handle_to_pool_id(vb_handle);
    video_frm_info->mod_id   = recv_pic->mod_id;
    vfrm                    = &video_frm_info->v_frame;

    vfrm->width         = fmw_chn->pic_attr.width;
    vfrm->height        = fmw_chn->pic_attr.height;
    vfrm->pixel_format  = recv_pic->pixel_format;

    vfrm->pts           = recv_pic->pts;
    vfrm->time_ref      = recv_pic->time_ref;
    vfrm->field         = recv_pic->filed;
    vfrm->dynamic_range = recv_pic->dynamic_range;
    vfrm->compress_mode = recv_pic->compress_mode;
    vfrm->video_format  = recv_pic->video_format;
    vfrm->color_gamut   = recv_pic->color_gamut;

    vfrm->header_phy_addr[0] = call_vb_handle_to_phys(vb_handle);
    vfrm->header_phy_addr[1] = vfrm->header_phy_addr[0] + vb_cal_config->head_y_size;
    vfrm->header_phy_addr[2] = vfrm->header_phy_addr[1];

    vfrm->header_vir_addr[0] = call_vb_handle_to_kern(vb_handle);
    vfrm->header_vir_addr[1] = vfrm->header_vir_addr[0] + vb_cal_config->head_y_size;
    vfrm->header_vir_addr[2] = vfrm->header_vir_addr[1];

    vfrm->header_stride[0] = vb_cal_config->head_stride;
    vfrm->header_stride[1] = vb_cal_config->head_stride;
    vfrm->header_stride[2] = vb_cal_config->head_stride;

    vfrm->phy_addr[0] = vfrm->header_phy_addr[0] + vb_cal_config->head_size;
    vfrm->phy_addr[1] = vfrm->phy_addr[0] + vb_cal_config->main_y_size;
    vfrm->phy_addr[2] = vfrm->phy_addr[1];

    vfrm->vir_addr[0] = vfrm->header_vir_addr[0] + vb_cal_config->head_size;
    vfrm->vir_addr[1] = vfrm->vir_addr[0] + vb_cal_config->main_y_size;
    vfrm->vir_addr[2] = vfrm->vir_addr[1];

    vfrm->stride[0] = vb_cal_config->main_stride;
    vfrm->stride[1] = vb_cal_config->main_stride;
    vfrm->stride[2] = vb_cal_config->main_stride;

    vfrm->ext_phy_addr[0] = vfrm->phy_addr[0] + vb_cal_config->main_size;
    vfrm->ext_phy_addr[1] = vfrm->ext_phy_addr[0] + vb_cal_config->ext_y_size;
    vfrm->ext_phy_addr[2] = vfrm->ext_phy_addr[1];

    vfrm->ext_vir_addr[0] = vfrm->vir_addr[0] + vb_cal_config->main_size;
    vfrm->ext_vir_addr[1] = vfrm->ext_vir_addr[0] + vb_cal_config->ext_y_size;
    vfrm->ext_vir_addr[2] = vfrm->ext_vir_addr[1];

    vfrm->ext_stride[0] = vb_cal_config->ext_stride;
    vfrm->ext_stride[1] = vb_cal_config->ext_stride;
    vfrm->ext_stride[2] = vb_cal_config->ext_stride;
}

static hi_s32 pciv_firmware_send_data(hi_pciv_chn pciv_chn, pciv_fmw_channel *fmw_chn, pciv_pic *recv_pic,
    hi_video_frame_info *video_frm_info)
{
    hi_s32 ret;
    hi_s32 dev_id = 0;
    hi_s32 chn_id = pciv_chn;
    if (recv_pic->src_type == PCIV_BIND_VI) {
        ret = call_sys_send_data(HI_ID_PCIV, dev_id, chn_id, recv_pic->block,
                                 MPP_DATA_VI_FRAME, video_frm_info);
    } else if (recv_pic->src_type == PCIV_BIND_VO) {
        ret = call_sys_send_data(HI_ID_PCIV, dev_id, chn_id, recv_pic->block,
                                 MPP_DATA_VOU_FRAME, video_frm_info);
    } else if (recv_pic->src_type == PCIV_BIND_VDEC) {
        ret = pciv_firmware_receiver_send_vdec_pic(pciv_chn, video_frm_info, recv_pic->index, recv_pic->block);
    } else {
        PCIV_FMW_ERR_TRACE("pciv chn %d bind type error, type value: %d.\n", pciv_chn, recv_pic->src_type);
        ret = HI_FAILURE;
    }

    if ((ret != HI_SUCCESS) && (ret != HI_ERR_VO_CHN_NOT_ENABLE)) {
        PCIV_FMW_ERR_TRACE("pciv chn %d send failed, ret:0x%x\n", pciv_chn, ret);
        fmw_chn->lost_cnt++;
    } else {
        fmw_chn->send_cnt++;
    }
    return ret;
}

hi_s32 pciv_firmware_recv_pic_and_send(hi_pciv_chn pciv_chn, pciv_pic *recv_pic)
{
    hi_ulong            flags;
    hi_s32              ret;
    pciv_fmw_channel    *fmw_chn;
    hi_vb_cal_config    vb_cal_config;
    vb_blk_handle       vb_handle;
    hi_video_frame_info video_frm_info  = {0};

    PCIVFMW_CHECK_CHNID(pciv_chn);

    PCIV_FMW_SPIN_LOCK(flags);
    fmw_chn = &g_fmw_pciv_chn[pciv_chn];
    if (fmw_chn->start != HI_TRUE) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        return HI_ERR_PCIV_SYS_NOTREADY;
    }
    fmw_chn->get_cnt++;
    ret = pciv_firmware_recv_get_vb_cal_config(pciv_chn, recv_pic, &vb_cal_config);
    if (ret != HI_SUCCESS) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        return ret;
    }

    vb_handle = call_vb_phy_to_handle(fmw_chn->phy_addr[recv_pic->index]);
    if (vb_handle == VB_INVALID_HANDLE) {
        PCIV_FMW_ERR_TRACE("pcivfmw %d get buffer fail!\n", pciv_chn);
        fmw_chn->lost_cnt++;
        PCIV_FMW_SPIN_UNLOCK(flags);
        return HI_FAILURE;
    }

    pciv_firmware_recv_set_v_frame(&video_frm_info, fmw_chn, recv_pic, vb_handle, &vb_cal_config);

    ret = pciv_firmware_send_data(pciv_chn,  fmw_chn,  recv_pic, &video_frm_info);

    /* HSCP201306030001 when start timer, PcivFirmWareVoPicFree not have lock synchronous
      int the PcivFirmWareVoPicFree function, need separate lock */
    PCIV_FMW_SPIN_UNLOCK(flags);
    pciv_firmware_recv_pic_free(pciv_chn);

    return ret;
}

static hi_bool pciv_firmware_is_vb_can_release(pciv_fmw_channel *fmw_chn, hi_s32 index)
{
    /* if bPcivHold is false,only when the VB count occupied by vo/vpss/venc is 0,the VB can release */
    if (call_vb_inquire_one_user_cnt(call_vb_phy_to_handle(fmw_chn->phy_addr[index]), VB_UID_VO) != 0) {
        return HI_FALSE;
    }
    if (call_vb_inquire_one_user_cnt(call_vb_phy_to_handle(fmw_chn->phy_addr[index]), VB_UID_VPSS) != 0) {
        return HI_FALSE;
    }
    if (call_vb_inquire_one_user_cnt(call_vb_phy_to_handle(fmw_chn->phy_addr[index]), VB_UID_VENC) != 0) {
        return HI_FALSE;
    }
    if (fmw_chn->pciv_hold[index] == HI_TRUE) {
        return HI_FALSE;
    }
    return HI_TRUE;
}

/* After vo displaying and vpss and venc used,the function register by PCIV or FwmDccs mode is been called */
hi_void pciv_firmware_recv_pic_free(hi_ulong data)
{
    hi_ulong            flags;
    hi_s32              ret;
    pciv_pic            recv_pic;
    pciv_fmw_channel    *fmw_chn;
    hi_u32              i           = 0;
    hi_u32              count       = 0;
    hi_bool             hit         = HI_FALSE;
    hi_pciv_chn         pciv_chn    = (hi_pciv_chn)data;

    PCIV_FMW_SPIN_LOCK(flags);
    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    if (fmw_chn->start != HI_TRUE) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        return;
    }

    for (i = 0; i < fmw_chn->count; i++) {
        if (!pciv_firmware_is_vb_can_release(fmw_chn, i)) {
            continue;
        }
        /* The function register by the PCIV is called to handle the action
        after the VO displaying or vpss using and venc coding */
        if (g_pciv_fmw_call_back.pf_recv_pic_free) {
            recv_pic.index = i;        /* the index of buffer can release */
            recv_pic.pts = 0;          /* PTS */
            recv_pic.count = count;    /* not used */
            ret = g_pciv_fmw_call_back.pf_recv_pic_free(pciv_chn, &recv_pic);

            if (ret == HI_SUCCESS) {
                hit = HI_TRUE;
                fmw_chn->resp_cnt++;
                continue;
            }
            if (ret != HI_ERR_PCIV_BUF_EMPTY) {
                PCIV_FMW_ERR_TRACE("pcivfmw chn%d pf_recv_pic_free failed with:0x%x.\n", pciv_chn, ret);
                continue;
            }

        }
    }

    /* if the buffer has not release by vo/vpss/venc,then start the time after 10ms to check */
    if (hit != HI_TRUE) {
        fmw_chn->buf_timer.function = pciv_firmware_recv_pic_free;
        fmw_chn->buf_timer.data = data;
        mod_timer(&(fmw_chn->buf_timer), jiffies + 1);
    }
    PCIV_FMW_SPIN_UNLOCK(flags);

    return;
}

/* After transmit, release the Image buffer after VGS Zoom */
hi_s32 pciv_firmware_src_pic_free(hi_pciv_chn pciv_chn, pciv_pic *src_pic)
{
    hi_s32      ret;

    g_fmw_pciv_chn[pciv_chn].resp_cnt++;

    /* if the mpp is deinit,the sys will release the buffer */
    if (g_pciv_fmw_state == PCIVFMW_STATE_STOPED) {
        return HI_SUCCESS;
    }

    PCIV_FMW_DEBUG_TRACE("- --> addr:0x%llx\n", src_pic->phy_addr);
    ret = call_vb_user_sub(src_pic->pool_id, src_pic->phy_addr, VB_UID_PCIV);
    return ret;
}

static hi_void pciv_fmw_set_src_pic(pciv_pic *src_pic, hi_pciv_bind_obj *bind_obj,
    pciv_fmw_channel *fmw_chn, const hi_video_frame_info *v_frame)
{
    src_pic->pool_id         = v_frame->pool_id;
    src_pic->src_type        = bind_obj->type;
    src_pic->block           = fmw_chn->block;
    src_pic->mod_id          = v_frame->mod_id;
    src_pic->pts             = v_frame->v_frame.pts;
    src_pic->time_ref        = v_frame->v_frame.time_ref;
    src_pic->filed           = v_frame->v_frame.field;
    src_pic->color_gamut     = v_frame->v_frame.color_gamut;
    src_pic->compress_mode   = v_frame->v_frame.compress_mode;
    src_pic->dynamic_range   = v_frame->v_frame.dynamic_range;
    src_pic->video_format    = v_frame->v_frame.video_format;
    src_pic->pixel_format    = v_frame->v_frame.pixel_format;
    /* The reason why here add if&else is that threr is some situation that
    some modual doesn't set value to u64HeaderPhyAddr in the mode of unCompress */
    if (v_frame->v_frame.compress_mode == COMPRESS_MODE_NONE) {
        src_pic->phy_addr = v_frame->v_frame.phy_addr[0];
    } else {
        src_pic->phy_addr = v_frame->v_frame.header_phy_addr[0];
    }
    return;
}

static hi_void pciv_fmw_src_pic_release(hi_pciv_chn pciv_chn, hi_pciv_bind_obj *bind_obj,
    const hi_video_frame_info *v_frame, const hi_video_frame_info *vdec_frame)
{
    hi_ulong            flags;
    hi_s32              ret;
    pciv_fmw_channel    *fmw_chn;

    if ((vdec_frame != NULL) && (bind_obj->type == PCIV_BIND_VDEC) && (bind_obj->vpss_send == HI_FALSE)) {
        /* if success,the Image send by vdec directly bind PCIV need release here */
        if (v_frame->v_frame.compress_mode == COMPRESS_MODE_NONE) {
            ret = call_vb_user_sub(vdec_frame->pool_id, vdec_frame->v_frame.phy_addr[0], VB_UID_PCIV);
        } else {
            ret = call_vb_user_sub(vdec_frame->pool_id, vdec_frame->v_frame.header_phy_addr[0], VB_UID_PCIV);
        }
        HI_ASSERT(ret == HI_SUCCESS);

        PCIV_FMW_SPIN_LOCK(flags);
        fmw_chn = &g_fmw_pciv_chn[pciv_chn];
        HI_ASSERT(fmw_chn->cur_vdec_node != NULL);
        pciv_pic_queue_put_free(&fmw_chn->pic_queue, fmw_chn->cur_vdec_node);
        fmw_chn->cur_vdec_node = NULL;
        PCIV_FMW_SPIN_UNLOCK(flags);
    }
}

static hi_s32 pciv_fmw_src_pic_send(hi_pciv_chn pciv_chn, hi_pciv_bind_obj *bind_obj,
    const hi_video_frame_info *v_frame, const hi_video_frame_info *vdec_frame)
{
    hi_ulong            flags;
    hi_s32              ret;
    pciv_pic            src_pic;
    pciv_fmw_channel    *fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    pciv_fmw_set_src_pic(&src_pic, bind_obj, fmw_chn, v_frame);

    PCIV_FMW_SPIN_LOCK(flags);
    if (fmw_chn->start != HI_TRUE) {
        PCIV_FMW_INFO_TRACE("pciv chn %d have stoped \n", pciv_chn);
        PCIV_FMW_SPIN_UNLOCK(flags);
        return HI_FAILURE;
    }

    ret = pciv_check_input_frame_size(pciv_chn, &v_frame->v_frame);
    if (ret != HI_SUCCESS) {
        fmw_chn->lost_cnt++;
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_ERR_TRACE("pic from mod(%d) send to pciv failed!\n", v_frame->mod_id);
        return HI_FAILURE;
    }
    PCIV_FMW_SPIN_UNLOCK(flags);

    /* add the VB( release in pciv_firmware_src_pic_free) */
    if (v_frame->v_frame.compress_mode == COMPRESS_MODE_NONE) {
        ret = call_vb_user_add(v_frame->pool_id, v_frame->v_frame.phy_addr[0], VB_UID_PCIV);
    } else {
        ret = call_vb_user_add(v_frame->pool_id, v_frame->v_frame.header_phy_addr[0], VB_UID_PCIV);
    }
    HI_ASSERT(ret == HI_SUCCESS);

    /* the callback function register by the upper mode is called to send the zoom Image by VGS */
    ret = g_pciv_fmw_call_back.pf_src_send_pic(pciv_chn, &src_pic);
    if (ret != HI_SUCCESS) {
        pciv_firmware_src_pic_free(pciv_chn, &src_pic);
        PCIV_FMW_SPIN_LOCK(flags);
        fmw_chn->lost_cnt++;
        PCIV_FMW_INFO_TRACE("pciv chn %d pf_src_send_pic failed! ret: 0x%x.\n", pciv_chn, ret);
        PCIV_FMW_SPIN_UNLOCK(flags);
        return HI_FAILURE;
    }

    pciv_fmw_src_pic_release(pciv_chn, bind_obj, v_frame, vdec_frame);

    PCIV_FMW_SPIN_LOCK(flags);
    fmw_chn->send_cnt++;
    PCIV_FMW_SPIN_UNLOCK(flags);

    return HI_SUCCESS;
}

hi_s32 pciv_fmw_get_region(hi_pciv_chn pciv_chn, hi_rgn_type type, rgn_info *info)
{
    hi_s32      ret;
    hi_mpp_chn  chn;

    if (!ckfn_rgn() || !ckfn_rgn_get_region()) {
        return HI_FAILURE;
    }

    chn.mod_id = HI_ID_PCIV;
    chn.chn_id = pciv_chn;
    chn.dev_id = 0;
    ret = call_rgn_get_region(type, &chn, info);
    HI_ASSERT(ret == HI_SUCCESS);

    return ret;
}

hi_void pciv_fmw_put_region(hi_pciv_chn pciv_chn, hi_rgn_type type)
{
    hi_s32      ret;
    hi_mpp_chn  chn;

    if (!ckfn_rgn() || !ckfn_rgn_put_region()) {
        return;
    }

    chn.mod_id = HI_ID_PCIV;
    chn.chn_id = pciv_chn;
    chn.dev_id = 0;

    ret = call_rgn_put_region(type, &chn, HI_NULL);
    HI_ASSERT(ret == HI_SUCCESS);
    return;
}

hi_s32 pciv_fmw_get_vb_cal_config(hi_pciv_chn pciv_chn, const hi_video_frame_info *src_frame,
    hi_vb_cal_config *vb_cal_config)
{
    vb_base_info        base_info;
    pciv_fmw_channel    *fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    base_info.is_3dnr_buffer = HI_FALSE;
    base_info.align = 0;

    base_info.compress_mode = src_frame->v_frame.compress_mode;
    base_info.dynamic_range = src_frame->v_frame.dynamic_range;
    base_info.video_format  = src_frame->v_frame.video_format;

    base_info.pixel_format  = src_frame->v_frame.pixel_format;
    base_info.width         = fmw_chn->pic_attr.width;
    base_info.height        = fmw_chn->pic_attr.height;

    if (!ckfn_sys_get_vb_cfg()) {
        PCIV_FMW_ERR_TRACE("sys_get_vb_cfg is NULL!\n");
        return HI_FAILURE;
    }
    call_sys_get_vb_cfg(&base_info, vb_cal_config);
    return HI_SUCCESS;
}

hi_s32 pciv_fmw_get_vgs_out_vb(hi_pciv_chn pciv_chn, vb_blk_handle *vb_handle)
{
    hi_mpp_chn          mpp_chn;
    hi_void             *mmz_name = HI_NULL;
    pciv_fmw_channel    *fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    /* the output Image is same to pciv channel size, request VB */
    if (!ckfn_sys_get_mmz_name()) {
        PCIV_FMW_ERR_TRACE("GetMmzName is NULL!\n");
        return HI_FAILURE;
    }

    mpp_chn.mod_id = HI_ID_PCIV;
    mpp_chn.dev_id = 0;
    mpp_chn.chn_id = pciv_chn;
    call_sys_get_mmz_name(&mpp_chn, &mmz_name);

    *vb_handle = call_vb_get_blk_by_size(fmw_chn->blk_size, VB_UID_PCIV, mmz_name);
    if (*vb_handle == VB_INVALID_HANDLE) {
        PCIV_FMW_ERR_TRACE("=======get VB(%d_byte) buffer for image out fail,chn: %d.=======\n",
            fmw_chn->blk_size, pciv_chn);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_void pciv_fmw_config_vgs_opt(hi_s32 value, rgn_info *info, vgs_online_opt *vgs_opt)
{
    hi_s32 i;
    /* config vgs task, put osd or not */
    if (value == HI_SUCCESS && info->num > 0) {
        HI_ASSERT(info->num <= OVERLAYEX_MAX_NUM_PCIV);

        for (i = 0; i < info->num; i++) {
            vgs_opt->osd_opt[i].osd_en = HI_TRUE;
            vgs_opt->osd_opt[i].global_alpha = FMW_GLOBAL_ALPHA;

            vgs_opt->osd_opt[i].osd.phy_addr = info->get_info->comm->phy_addr;
            vgs_opt->osd_opt[i].osd.pixel_format = info->get_info->comm->pixel_format;
            vgs_opt->osd_opt[i].osd.stride = info->get_info->comm->stride;

            if (vgs_opt->osd_opt[i].osd.pixel_format == PIXEL_FORMAT_ARGB_1555) {
                vgs_opt->osd_opt[i].osd.alpha_ext1555 = HI_TRUE;
                vgs_opt->osd_opt[i].osd.alpha0 = info->get_info->comm->bg_alpha;
                vgs_opt->osd_opt[i].osd.alpha1 = info->get_info->comm->fg_alpha;
            }

            vgs_opt->osd_opt[i].osd_rect.x = info->get_info->comm->point.x;
            vgs_opt->osd_opt[i].osd_rect.y = info->get_info->comm->point.y;
            vgs_opt->osd_opt[i].osd_rect.height = info->get_info->comm->size.height;
            vgs_opt->osd_opt[i].osd_rect.width = info->get_info->comm->size.width;
        }
        vgs_opt->osd = HI_TRUE;
    }
}

static hi_s32 pciv_fmw_zoom_cb_check(hi_pciv_chn pciv_chn, pciv_fmw_channel *fmw_chn,
    hi_pciv_bind_obj *bind_obj, const vgs_task_data *vgs_task)
{
    hi_ulong flags;

    /* In VGS interrupt,maybe the pcivchn is stopped */
    if (fmw_chn->start != HI_TRUE) {
        PCIV_FMW_INFO_TRACE("pciv chn %d have stoped \n", pciv_chn);
        if (bind_obj->type == PCIV_BIND_VDEC) {
            PCIV_FMW_SPIN_LOCK(flags);
            fmw_chn->send_state = PCIVFMW_SEND_NOK;
            PCIV_FMW_SPIN_UNLOCK(flags);
        }
        return HI_FAILURE;
    }

    /* **********************************************************************************
    * [HSCP201308020003] l00181524,2013.08.16,if the task fail,maybe the action of cancle job occur in pciv ower interrupt
    * maybe out of its ower job,it need judge next step, we cannot add lock,else maybe lock itself and other abnormal
    *********************************************************************************** */
    /* if the vgs task fail,then retuen with failure */
    if (vgs_task->finish_stat != VGS_TASK_FNSH_STAT_OK) {
        PCIV_FMW_ERR_TRACE("vgs task finish status is no ok, chn:%d\n", pciv_chn);
        if (bind_obj->type == PCIV_BIND_VDEC) {
            fmw_chn->send_state = PCIVFMW_SEND_NOK;
        }
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_void pciv_fmw_cb_release_vb(const hi_video_frame_info *ref_img, const hi_video_frame_info *rel_img)
{
    hi_s32 ret;

    if (ref_img->v_frame.compress_mode == COMPRESS_MODE_NONE) {
        ret = call_vb_user_sub(rel_img->pool_id, rel_img->v_frame.phy_addr[0], VB_UID_PCIV);
    } else {
        ret = call_vb_user_sub(rel_img->pool_id, rel_img->v_frame.header_phy_addr[0], VB_UID_PCIV);
    }
    HI_ASSERT(ret == HI_SUCCESS);
    return;
}

static hi_s32 pciv_fmw_zoom_pre(hi_pciv_chn pciv_chn, vb_blk_handle *vb_handle, vgs_task_data **vgs_task,
    hi_vb_cal_config *vb_cal_config, const hi_video_frame_info *src_frame)
{
    hi_s32          ret;
    vgs_export_func *pfn_vgs_exp_func = (vgs_export_func *)cmpi_get_module_func_by_id(HI_ID_VGS);

    ret = pciv_fmw_get_vgs_out_vb(pciv_chn, vb_handle);
    if (ret != HI_SUCCESS) {
        PCIV_FMW_ERR_TRACE("pciv_fmw get vgs out_put VB failed!\n");
        return ret;
    }

    ret = pciv_fmw_get_vb_cal_config(pciv_chn, src_frame, vb_cal_config);
    if (ret != HI_SUCCESS) {
        PCIV_FMW_ERR_TRACE("pciv_fmw get vgs vb_cal_config failed!\n");
        return ret;
    }

    *vgs_task = pfn_vgs_exp_func->pfn_vgs_get_free_task();
    if (*vgs_task == HI_NULL) {
        PCIV_FMW_ERR_TRACE("pciv get task faild\n");
        return HI_FAILURE;
    }

    osal_memset(*vgs_task, 0, sizeof(vgs_task_data));

    /* configure the input video frame */
    osal_memcpy(&(*vgs_task)->img_in, src_frame, sizeof(hi_video_frame_info));

    /* the output image is same with the input image */
    osal_memcpy(&(*vgs_task)->img_out, &(*vgs_task)->img_in, sizeof(hi_video_frame_info));
    return HI_SUCCESS;
}

static hi_void pciv_fmw_config_out_frame(vb_blk_handle vb_handle,
    hi_video_frame *out_frame, const hi_vb_cal_config *cal_config,
    const pciv_fmw_channel *fmw_chn, const hi_video_frame_info *src_frame)
{
    out_frame->width        = fmw_chn->pic_attr.width;
    out_frame->height       = fmw_chn->pic_attr.height;

    if (src_frame != HI_NULL) {
        out_frame->pixel_format = src_frame->v_frame.pixel_format;
    } else {
        out_frame->pixel_format = fmw_chn->pic_attr.pixel_format;
    }

    out_frame->header_phy_addr[0] = call_vb_handle_to_phys(vb_handle);
    out_frame->header_phy_addr[1] = out_frame->header_phy_addr[0] + cal_config->head_y_size;
    out_frame->header_phy_addr[2] = out_frame->header_phy_addr[1];

    out_frame->header_vir_addr[0] = call_vb_handle_to_kern(vb_handle);
    out_frame->header_vir_addr[1] = out_frame->header_vir_addr[0] + cal_config->head_y_size;
    out_frame->header_vir_addr[2] = out_frame->header_vir_addr[1];

    out_frame->header_stride[0] = cal_config->head_stride;
    out_frame->header_stride[1] = cal_config->head_stride;
    out_frame->header_stride[2] = cal_config->head_stride;

    out_frame->phy_addr[0] = out_frame->header_phy_addr[0] + cal_config->head_size;
    out_frame->phy_addr[1] = out_frame->phy_addr[0] + cal_config->main_y_size;
    out_frame->phy_addr[2] = out_frame->phy_addr[1];

    out_frame->vir_addr[0] = out_frame->header_vir_addr[0] + cal_config->head_size;
    out_frame->vir_addr[1] = out_frame->vir_addr[0] + cal_config->main_y_size;
    out_frame->vir_addr[2] = out_frame->vir_addr[1];

    out_frame->stride[0] = cal_config->main_stride;
    out_frame->stride[1] = cal_config->main_stride;
    out_frame->stride[2] = cal_config->main_stride;

    out_frame->ext_phy_addr[0] = out_frame->phy_addr[0] + cal_config->main_size;
    out_frame->ext_phy_addr[1] = out_frame->ext_phy_addr[0] + cal_config->ext_y_size;
    out_frame->ext_phy_addr[2] = out_frame->ext_phy_addr[1];

    out_frame->ext_vir_addr[0] = out_frame->vir_addr[0] + cal_config->main_size;
    out_frame->ext_vir_addr[1] = out_frame->ext_vir_addr[0] + cal_config->ext_y_size;
    out_frame->ext_vir_addr[2] = out_frame->ext_vir_addr[1];

    out_frame->ext_stride[0] = cal_config->ext_stride;
    out_frame->ext_stride[1] = cal_config->ext_stride;
    out_frame->ext_stride[2] = cal_config->ext_stride;
}

static hi_void pciv_fmw_config_vgs_task(hi_pciv_chn pciv_chn, vgs_task_data *vgs_task, hi_pciv_bind_obj *obj,
    hi_video_frame *out_frame, pciv_fmw_vgs_task *fmw_task)
{
    hi_s32 ret;

    vgs_task->call_mod_id       = HI_ID_PCIV;
    vgs_task->call_dev_id       = 0;
    vgs_task->call_chn_id       = pciv_chn;
    vgs_task->private_data[0]   = obj->type;
    vgs_task->private_data[1]   = obj->vpss_send;
    vgs_task->reserved          = 0;

    fmw_task->vgs_in_vb_handle    = vgs_task->img_in.pool_id;
    fmw_task->vgs_out_vb_handle   = vgs_task->img_out.pool_id;
    if (out_frame->compress_mode != COMPRESS_MODE_NONE) {
        fmw_task->in_vb_phy_addr  = vgs_task->img_in.v_frame.header_phy_addr[0];
        fmw_task->out_vb_phy_addr = vgs_task->img_out.v_frame.header_phy_addr[0];
    } else {
        fmw_task->in_vb_phy_addr  = vgs_task->img_in.v_frame.phy_addr[0];
        fmw_task->out_vb_phy_addr = vgs_task->img_out.v_frame.phy_addr[0];
    }

    ret = call_vb_user_add(fmw_task->vgs_in_vb_handle, fmw_task->in_vb_phy_addr, VB_UID_PCIV);
    HI_ASSERT(ret == HI_SUCCESS);
}

static hi_void  pciv_fmw_release_vb(pciv_fmw_vgs_task *fmw_task)
{
    call_vb_user_sub(fmw_task->vgs_out_vb_handle, fmw_task->out_vb_phy_addr, VB_UID_PCIV);
    call_vb_user_sub(fmw_task->vgs_in_vb_handle, fmw_task->in_vb_phy_addr, VB_UID_PCIV);
    return;
}

static hi_void pciv_fmw_src_pic_zoom_cb(hi_mod_id call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id,
    const vgs_task_data *vgs_task)
{
    hi_ulong                    flags;
    hi_s32                      ret;
    hi_pciv_chn                 pciv_chn;
    hi_pciv_bind_obj            bind_obj = {0};
    pciv_fmw_channel            *fmw_chn = NULL;
    const hi_video_frame_info   *img_in  = NULL;
    const hi_video_frame_info   *img_out = NULL;

    if (vgs_task == NULL) {
        PCIV_FMW_ERR_TRACE("in function pciv_fmw_src_pic_zoom_cb: vgs_task is null, return!\n");
        return;
    }

    img_in   = &vgs_task->img_in;
    img_out  = &vgs_task->img_out;
    pciv_chn = vgs_task->call_chn_id;
    HI_ASSERT((pciv_chn >= 0) && (pciv_chn < PCIV_FMW_MAX_CHN_NUM));
    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    /* the image finish used should released */
    pciv_fmw_cb_release_vb(img_in, img_in);

    fmw_chn->zoom_cb_cnt++;

    bind_obj.type = vgs_task->private_data[0];
    bind_obj.vpss_send = vgs_task->private_data[1];

    if (pciv_fmw_zoom_cb_check(pciv_chn, fmw_chn, &bind_obj, vgs_task)) {
        goto out;
    }

    /* send the video after zoom */
    ret = pciv_fmw_src_pic_send(pciv_chn, &bind_obj, img_out, img_in);
    if (ret != HI_SUCCESS) {
        if (bind_obj.type == PCIV_BIND_VDEC) {
            PCIV_FMW_SPIN_LOCK(flags);
            fmw_chn->send_state = PCIVFMW_SEND_NOK;
            PCIV_FMW_SPIN_UNLOCK(flags);
        }
        goto out;
    }

    if (bind_obj.type == PCIV_BIND_VDEC) {
        PCIV_FMW_SPIN_LOCK(flags);
        fmw_chn->send_state = PCIVFMW_SEND_OK;
        PCIV_FMW_SPIN_UNLOCK(flags);
    }

    if (fmw_chn->rgn_num != 0) {
        fmw_chn->rgn_num = 0;
    }

out:
    /* no matter success or not,in this callback ,the output vb will released */
    pciv_fmw_cb_release_vb(img_in, img_out);
    return;
}

static hi_s32 pciv_fmw_src_pic_zoom_work(hi_pciv_chn pciv_chn, pciv_fmw_channel *fmw_chn, vgs_task_data *vgs_task)
{
    hi_s32          ret;
    vgs_job_data    job_data;
    hi_vgs_handle   vgs_handle;
    vgs_export_func *pfn_vgs_exp_func = (vgs_export_func *)cmpi_get_module_func_by_id(HI_ID_VGS);


    job_data.job_type       = VGS_JOB_TYPE_NORMAL;
    job_data.job_call_back  = HI_NULL;

    /* 1.begin VGS job------------------------------------------------------ */
    ret = pfn_vgs_exp_func->pfn_vgs_begin_job(&vgs_handle, VGS_JOB_PRI_NORMAL, HI_ID_PCIV, 0, pciv_chn, &job_data);
    if (ret != HI_SUCCESS) {
        fmw_chn->add_job_fail_cnt++;
        PCIV_FMW_ERR_TRACE("pfn_vgs_begin_job failed ! pciv_chn:%d \n", pciv_chn);
        pfn_vgs_exp_func->pfn_vgs_put_free_task(vgs_task);
        return HI_FAILURE;
    }
    fmw_chn->add_job_suc_cnt++;

    /* 2.zoom the pic------------------------------------------------------ */
    /* configure other item of the vgs task info(in the callback of vgs will perform the action send image) */
    ret = pfn_vgs_exp_func->pfn_vgs_add_online_task(vgs_handle, vgs_task, &fmw_chn->vgs_opt);
    if (ret != HI_SUCCESS) {
        fmw_chn->zoom_task_fail_cnt++;
        PCIV_FMW_ERR_TRACE("create vgs task failed,errno:%x,will lost this frame\n", ret);
        pfn_vgs_exp_func->pfn_vgs_cancel_job(vgs_handle);
        return HI_FAILURE;
    }

    fmw_chn->zoom_task_suc_cnt++;

    /* 3.end VGS job------------------------------------------------------- */
    /* notes: if end_job failed, callback will called auto */
    ret = pfn_vgs_exp_func->pfn_vgs_end_job(vgs_handle, HI_TRUE, HI_NULL);
    if (ret != HI_SUCCESS) {
        fmw_chn->end_job_fail_cnt++;
        PCIV_FMW_ERR_TRACE("pfn_vgs_end_job failed ! pciv_chn:%d \n", pciv_chn);
        pfn_vgs_exp_func->pfn_vgs_cancel_job(vgs_handle);
        return HI_FAILURE;
    }

    fmw_chn->end_job_suc_cnt++;
    return HI_SUCCESS;
}

static hi_s32 pciv_fmw_src_pic_zoom(hi_pciv_chn pciv_chn, hi_pciv_bind_obj *obj, const hi_video_frame_info *src_frame)
{
    hi_s32              ret;
    vb_blk_handle       vb_handle;
    hi_vb_cal_config    vb_cal_config;
    pciv_fmw_vgs_task   fmw_task;
    pciv_fmw_channel    *fmw_chn    = NULL;
    hi_video_frame      *out_frame  = NULL;
    vgs_task_data       *vgs_task   = HI_NULL;

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    if (src_frame->v_frame.video_format != VIDEO_FORMAT_LINEAR
        && src_frame->v_frame.video_format != VIDEO_FORMAT_LINEAR_DISCRETE) {
        PCIV_FMW_ERR_TRACE(
            "vgs do not support the input image videoformat(%d)!\n",
            src_frame->v_frame.video_format);
        return HI_FAILURE;
    }

    ret = pciv_fmw_zoom_pre(pciv_chn, &vb_handle, &vgs_task, &vb_cal_config, src_frame);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    /* config the output video info */
    vgs_task->img_out.pool_id   = call_vb_handle_to_pool_id(vb_handle);
    out_frame                   = &vgs_task->img_out.v_frame;
    pciv_fmw_config_out_frame(vb_handle,  out_frame,  &vb_cal_config, fmw_chn, HI_NULL);

    /* config the member or vgs task structure(sending image in VGS callback function) */
    vgs_task->call_back = pciv_fmw_src_pic_zoom_cb;
    pciv_fmw_config_vgs_task(pciv_chn, vgs_task, obj, out_frame, &fmw_task);

    ret = pciv_fmw_src_pic_zoom_work(pciv_chn, fmw_chn, vgs_task);
    if (ret != HI_SUCCESS) {
        goto OUT;
    }

    return HI_SUCCESS;

OUT:
    pciv_fmw_release_vb(&fmw_task);
    return HI_FAILURE;
}

static hi_void pciv_fmw_src_pic_move_osd_zoom_cb(hi_mod_id call_mod_id,
    hi_u32 call_dev_id, hi_u32 call_chn_id, const vgs_task_data *vgs_task)
{
    hi_ulong                    flags;
    hi_s32                      ret;
    hi_pciv_chn                 pciv_chn;
    hi_pciv_bind_obj            bind_obj = {0};
    pciv_fmw_channel            *fmw_chn = NULL;
    const hi_video_frame_info   *img_in  = NULL;
    const hi_video_frame_info   *img_out = NULL;

    if (vgs_task == NULL) {
        PCIV_FMW_ERR_TRACE("in function pciv_fmw_src_pic_move_osd_zoom_cb: vgs_task is null, return!\n");
        return;
    }

    img_in   = &vgs_task->img_in;
    img_out  = &vgs_task->img_out;
    pciv_chn = vgs_task->call_chn_id;
    HI_ASSERT((pciv_chn >= 0) && (pciv_chn < PCIV_FMW_MAX_CHN_NUM));
    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    /* the image finish used should released */
    pciv_fmw_cb_release_vb(img_in, img_in);
    fmw_chn->move_cb_cnt++;
    fmw_chn->osd_cb_cnt++;
    fmw_chn->zoom_cb_cnt++;

    /* release the region gotten */
    pciv_fmw_put_region(pciv_chn, OVERLAYEX_RGN);
    fmw_chn->rgn_num = 0;

    bind_obj.type       = vgs_task->private_data[0];
    bind_obj.vpss_send  = vgs_task->private_data[1];
    if (pciv_fmw_zoom_cb_check(pciv_chn, fmw_chn, &bind_obj, vgs_task)) {
            goto out;
    }

    /* send the video after zoom */
    ret = pciv_fmw_src_pic_send(pciv_chn, &bind_obj, img_out, img_in);
    if (ret != HI_SUCCESS) {
        if (bind_obj.type == PCIV_BIND_VDEC) {
            PCIV_FMW_SPIN_LOCK(flags);
            fmw_chn->send_state = PCIVFMW_SEND_NOK;
            PCIV_FMW_SPIN_UNLOCK(flags);
        }
        goto out;
    }
    if (bind_obj.type == PCIV_BIND_VDEC) {
        PCIV_FMW_SPIN_LOCK(flags);
        fmw_chn->send_state = PCIVFMW_SEND_OK;
        PCIV_FMW_SPIN_UNLOCK(flags);
    }

out:
    /* no matter success or not,this callback mst release the VB */
    pciv_fmw_cb_release_vb(img_in, img_out);
    return;
}

static hi_s32 pciv_fmw_src_pic_move_osd_zoom_work(hi_pciv_chn pciv_chn, pciv_fmw_channel *fmw_chn,
    vgs_task_data *vgs_task, vgs_online_opt *vgs_opt)
{
    hi_s32          ret;
    vgs_job_data    job_data;
    hi_vgs_handle   vgs_handle;
    vgs_export_func *pfn_vgs_exp_func = (vgs_export_func *)cmpi_get_module_func_by_id(HI_ID_VGS);

    job_data.job_type       = VGS_JOB_TYPE_NORMAL;
    job_data.job_call_back  = HI_NULL;
    /* 1.begin VGS job------------------------------------------------------ */
    ret = pfn_vgs_exp_func->pfn_vgs_begin_job(&vgs_handle, VGS_JOB_PRI_NORMAL, HI_ID_PCIV, 0, pciv_chn, &job_data);
    if (ret != HI_SUCCESS) {
        fmw_chn->add_job_fail_cnt++;
        PCIV_FMW_ERR_TRACE("pfn_vgs_begin_job failed ! pciv_chn:%d \n", pciv_chn);
        pfn_vgs_exp_func->pfn_vgs_put_free_task(vgs_task);
        return HI_FAILURE;
    }
    fmw_chn->add_job_suc_cnt++;

    /* 2.move the picture, add osd, scale picture-------------------------- */
    /* add task to VGS job */
    ret = pfn_vgs_exp_func->pfn_vgs_add_online_task(vgs_handle, vgs_task, vgs_opt);
    if (ret != HI_SUCCESS) {
        fmw_chn->move_task_fail_cnt++;
        fmw_chn->osd_task_fail_cnt++;
        fmw_chn->zoom_task_fail_cnt++;
        PCIV_FMW_ERR_TRACE("create vgs task failed,errno:%x,will lost this frame\n", ret);
        pfn_vgs_exp_func->pfn_vgs_cancel_job(vgs_handle);
        return HI_FAILURE;
    }

    fmw_chn->move_task_suc_cnt++;
    fmw_chn->osd_task_suc_cnt++;
    fmw_chn->zoom_task_suc_cnt++;

    /* 3.end DSU job-------------------------------------------------------*/
    /* notes: if end_job failed, callback will called auto */
    ret = pfn_vgs_exp_func->pfn_vgs_end_job(vgs_handle, HI_TRUE, HI_NULL);
    if (ret != HI_SUCCESS) {
        fmw_chn->end_job_fail_cnt++;
        PCIV_FMW_ERR_TRACE("pfn_vgs_end_job failed ! pciv_chn:%d \n", pciv_chn);
        pfn_vgs_exp_func->pfn_vgs_cancel_job(vgs_handle);
        return HI_FAILURE;
    }

    fmw_chn->end_job_suc_cnt++;
    return HI_SUCCESS;
}

static hi_s32 pciv_fmw_src_pic_move_osd_zoom(hi_pciv_chn pciv_chn, hi_pciv_bind_obj *obj,
    const hi_video_frame_info *src_frame)
{
    hi_s32                  ret;
    hi_s32                  value;
    rgn_info                info;
    vb_blk_handle           vb_handle;
    static vgs_online_opt   vgs_opt;
    hi_vb_cal_config        vb_cal_config;
    pciv_fmw_vgs_task       fmw_task;
    vgs_task_data           *vgs_task   = HI_NULL;
    hi_video_frame          *out_frame  = NULL;
    pciv_fmw_channel        *fmw_chn    = &g_fmw_pciv_chn[pciv_chn];

    info.num = 0;
    ret = pciv_fmw_get_region(pciv_chn, OVERLAYEX_RGN, &info);
    value = ret;
    if (info.num <= 0) {
        pciv_fmw_put_region(pciv_chn, OVERLAYEX_RGN);
        ret = pciv_fmw_src_pic_zoom(pciv_chn, obj, src_frame);
        return ret;
    }
    fmw_chn->rgn_num = info.num;

    /* config VGS optional */
    osal_memcpy(&vgs_opt, &fmw_chn->vgs_opt, sizeof(vgs_online_opt));
    pciv_fmw_config_vgs_opt(value, &info, &vgs_opt);

    ret = pciv_fmw_zoom_pre(pciv_chn, &vb_handle, &vgs_task, &vb_cal_config, src_frame);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    /* config the outout video info */
    vgs_task->img_out.pool_id   = call_vb_handle_to_pool_id(vb_handle);
    out_frame                   = &vgs_task->img_out.v_frame;
    pciv_fmw_config_out_frame(vb_handle,  out_frame,  &vb_cal_config, fmw_chn, src_frame);

    /* config the member or vgs task structure(sending image in VGS callback function) */
    vgs_task->call_back = pciv_fmw_src_pic_move_osd_zoom_cb;
    pciv_fmw_config_vgs_task(pciv_chn, vgs_task, obj, out_frame, &fmw_task);

    ret = pciv_fmw_src_pic_move_osd_zoom_work(pciv_chn, fmw_chn, vgs_task, &vgs_opt);
    if (ret != HI_SUCCESS) {
        goto OUT;
    }
    return HI_SUCCESS;

OUT:
    pciv_fmw_put_region(pciv_chn, OVERLAYEX_RGN);
    pciv_fmw_release_vb(&fmw_task);
    return HI_FAILURE;
}

static hi_s32 pciv_firmware_src_preproc(hi_pciv_chn pciv_chn, hi_pciv_bind_obj *obj,
    const hi_video_frame_info *src_frame)
{
    hi_s32              ret  = HI_SUCCESS;
    pciv_fmw_channel    *fmw_chn = NULL;

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];
    if (g_drop_err_timeref == 1) {
        /* prevent out-of-order when send VI  or VO source image,
        drop the out-of-order fram in the video(time_ref must increased) */
        if ((obj->type == PCIV_BIND_VI) || (obj->type == PCIV_BIND_VO)) {
            if (src_frame->v_frame.time_ref < fmw_chn->time_ref) {
                PCIV_FMW_ERR_TRACE("pciv %d, time_ref err, (%d,%d)\n",
                    pciv_chn, src_frame->v_frame.time_ref, fmw_chn->time_ref);
                return HI_FAILURE;
            }
            fmw_chn->time_ref = src_frame->v_frame.time_ref;
        }
    }

    /* if need put osd to the source vide, process:move->put OSD->zoom */
    if ((obj->type == PCIV_BIND_VI) || (obj->type == PCIV_BIND_VO) || (obj->type == PCIV_BIND_VDEC)) {
        PCIV_FMW_INFO_TRACE("pciv channel %d support osd right now\n", pciv_chn);
        ret = pciv_fmw_src_pic_move_osd_zoom(pciv_chn, obj, src_frame);
    } else {
        PCIV_FMW_ERR_TRACE("pciv channel %d not support type:%d\n", pciv_chn, obj->type);
    }

    return ret;
}

/* be called in VIU interrupt handler or VDEC interrupt handler or vpss send handler . */
hi_s32 pciv_firmware_src_pic_send(hi_pciv_chn pciv_chn, hi_pciv_bind_obj *obj, const hi_video_frame_info *pic_info)
{
    pciv_fmw_channel *fmw_chn = NULL;

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    /* ***************************************************************************************
    * the image send to PCIV from VDEC is 16-byte align,but the image send by pciv is not align.then when PCIV send the image date from vdec directly, at this
    * time, when the master chip get data,it will calculate the address of UV, the err of 8 lines image data dislocation will appear, so we propose to the data should
    * through VPSS or VGS in the slave chip.it will reload as the format that PCIV sending need.
    * but,if the image data do not through VPSS, because of the performance of VGS if weak, at this time, the system send spead is limited by VGS, so for the diffrence
    * diffrence of align format, PCIV cannot send the image directly from vdec, but through this function handle.
    *************************************************************************************** */
    /* PCIV channel must be start */
    if (fmw_chn->start != HI_TRUE) {
        return HI_FAILURE;
    }

    /* perform the pre_process then send the image */
    if (pciv_firmware_src_preproc(pciv_chn, obj, pic_info)) {
        fmw_chn->lost_cnt++;
        return HI_FAILURE;
    }

    if (obj->type == PCIV_BIND_VDEC) {
        fmw_chn->send_state = PCIVFMW_SEND_ING;
    }

    return HI_SUCCESS;
}

/* be called in VIU interrupt handler */
hi_s32 pciv_firmware_viu_send_pic(VI_DEV vi_dev, VI_CHN vi_chn,
                                  const hi_video_frame_info *pic_info)
{
    return HI_SUCCESS;
}

static hi_s32 pciv_send_vdec_pic_timer_state_check(hi_pciv_chn pciv_chn, pciv_fmw_channel *fmw_chn)
{
    if (fmw_chn->start == HI_FALSE) {
        return HI_FAILURE;
    }

    if (fmw_chn->send_state == PCIVFMW_SEND_ING) {
        return HI_FAILURE;
    }
    if (fmw_chn->send_state == PCIVFMW_SEND_OK) {
        /* check the last time is success or not(the first is the success state)
        get the new vdec image info */
        fmw_chn->cur_vdec_node = pciv_pic_queue_get_busy(&fmw_chn->pic_queue);
        if (fmw_chn->cur_vdec_node == NULL) {
            PCIV_FMW_INFO_TRACE("pciv_chn:%d no vdec pic\n", pciv_chn);
            return HI_FAILURE;
        }
        return HI_SUCCESS;
    }
    if (fmw_chn->send_state == PCIVFMW_SEND_NOK) {
        if (fmw_chn->master == HI_TRUE) {
            /* if the last time is not success,get and send  the data of last time again */
            fmw_chn->cur_vdec_node = pciv_pic_queue_get_busy(&fmw_chn->pic_queue);
        }
        if (fmw_chn->cur_vdec_node == NULL) {
            PCIV_FMW_INFO_TRACE("pciv_chn:%d no vdec pic\n", pciv_chn);
            return HI_FAILURE;
        }
        return HI_SUCCESS;
    }
    PCIV_FMW_INFO_TRACE("pciv_chn %d send vdec pic state error %#x\n", pciv_chn, fmw_chn->send_state);
    return HI_FAILURE;
}

static hi_s32 pciv_send_vdec_pic_timer_work(hi_pciv_chn pciv_chn, pciv_fmw_channel *fmw_chn)
{
    hi_s32              ret;
    hi_s32              dev_id  = 0;
    hi_pciv_bind_obj    obj     = {0};
    hi_video_frame_info *v_frame_info;

    if (fmw_chn->master == HI_TRUE) {
        fmw_chn->send_state = PCIVFMW_SEND_ING;
        v_frame_info = &fmw_chn->cur_vdec_node->pciv_pic.video_frame;

        /* send the image frome vdec to VPSS/VENC/VO */
        PCIV_FMW_WARN_TRACE("pciv_chn:%d, start: %d, start send pic to VPSS/VO.\n", pciv_chn, fmw_chn->start);
        ret = call_sys_send_data(HI_ID_PCIV, dev_id, pciv_chn, fmw_chn->cur_vdec_node->pciv_pic.block,
                                 MPP_DATA_VDEC_FRAME, v_frame_info);
        if ((ret != HI_SUCCESS) &&
            (fmw_chn->cur_vdec_node->pciv_pic.block == HI_TRUE)) {
            PCIV_FMW_WARN_TRACE("pciv_chn:%d, start: %d, finish send pic to VPSS/VO.\n",
                pciv_chn, fmw_chn->start);
            PCIV_FMW_INFO_TRACE("pciv_chn:%d send pic failed, put to queue and send again. ret: 0x%x\n",
                pciv_chn, ret);
            pciv_pic_queue_put_busy_head(&fmw_chn->pic_queue, fmw_chn->cur_vdec_node);
            fmw_chn->send_state = PCIVFMW_SEND_NOK;
            fmw_chn->cur_vdec_node = NULL;
        } else {
            PCIV_FMW_WARN_TRACE("pciv_chn:%d, start: %d, finish send pic to VPSS/VO.\n",
                pciv_chn, fmw_chn->start);
            PCIV_FMW_INFO_TRACE("pciv_chn:%d send pic ok\n", pciv_chn);
            HI_ASSERT(fmw_chn->cur_vdec_node != NULL);
            fmw_chn->pciv_hold[fmw_chn->cur_vdec_node->pciv_pic.index] = HI_FALSE;
            pciv_pic_queue_put_free(&fmw_chn->pic_queue, fmw_chn->cur_vdec_node);
            fmw_chn->send_state = PCIVFMW_SEND_OK;
            fmw_chn->cur_vdec_node = NULL;
            return HI_SUCCESS;
        }
    } else {
        v_frame_info = &fmw_chn->cur_vdec_node->pciv_pic.video_frame;

        /* send the vdec image to PCI target */
        obj.type        = PCIV_BIND_VDEC;
        obj.vpss_send   = HI_FALSE;
        fmw_chn->mod_id = HI_ID_VDEC;
        ret = pciv_firmware_src_pic_send(pciv_chn, &obj, v_frame_info);
        /* if send failed,the next time use the backup data */
        if (ret != HI_SUCCESS) {
            fmw_chn->send_state = PCIVFMW_SEND_NOK;
        }
    }
    return HI_FAILURE;
}

/* timer function of receiver(master chip): get data from pciv and send to vpss/venc/vo */
/* timer function of sender(slave chip): get data from vdec and send to pciv */
hi_void pciv_send_vdec_pic_timer_func(hi_ulong data)
{
    hi_ulong    flags;
    hi_s32      ret;
    hi_pciv_chn pciv_chn;
    pciv_fmw_channel *fmw_chn;


    /* timer will be restarted after 1 tick */
    g_timer_pciv_send_vdec_pic.expires = jiffies + msecs_to_jiffies(PCIV_TIMER_EXPIRES);
    g_timer_pciv_send_vdec_pic.function = pciv_send_vdec_pic_timer_func;
    g_timer_pciv_send_vdec_pic.data = 0;
    if (!timer_pending(&g_timer_pciv_send_vdec_pic)) {
        add_timer(&g_timer_pciv_send_vdec_pic);
    }

    for (pciv_chn = 0; pciv_chn < PCIV_FMW_MAX_CHN_NUM; pciv_chn++) {
        PCIV_FMW_SPIN_LOCK(flags);
        fmw_chn = &g_fmw_pciv_chn[pciv_chn];

        fmw_chn->timer_cnt++;

        ret = pciv_send_vdec_pic_timer_state_check(pciv_chn, fmw_chn);
        if (ret != HI_SUCCESS) {
            PCIV_FMW_SPIN_UNLOCK(flags);
            continue;
        }
        ret = pciv_send_vdec_pic_timer_work(pciv_chn, fmw_chn);
        PCIV_FMW_SPIN_UNLOCK(flags);
        if (ret == HI_SUCCESS) {
            pciv_firmware_recv_pic_free(pciv_chn);
        }
    }

    return;
}

static hi_s32 pciv_firmware_send_pic_from_vi(hi_s32 dev_id, hi_pciv_chn pciv_chn, hi_bool block, hi_void *pic_info)
{
    hi_ulong            flags;
    hi_s32              ret;
    hi_pciv_bind_obj    obj         = {0};
    hi_video_frame_info *vif_info   = NULL;
    pciv_fmw_channel    *fmw_chn    = NULL;
    rgn_info            info;

    obj.type = PCIV_BIND_VI;
    obj.vpss_send = HI_FALSE;
    vif_info = (hi_video_frame_info *)pic_info;

    info.num = 0;
    ret = pciv_fmw_get_region(pciv_chn, OVERLAYEX_RGN, &info);
    if (ret != HI_SUCCESS) {
        pciv_fmw_put_region(pciv_chn, OVERLAYEX_RGN);
        return ret;
    }

    PCIV_FMW_SPIN_LOCK(flags);

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];
    fmw_chn->block = block;
    fmw_chn->mod_id = HI_ID_VI;

    if (fmw_chn->pic_attr.width != vif_info->v_frame.width
        || fmw_chn->pic_attr.height != vif_info->v_frame.height
        || (info.num > 0)) {
        pciv_fmw_put_region(pciv_chn, OVERLAYEX_RGN);
        /* the pic size is not the same or it needs to add osd */
        ret = pciv_firmware_src_pic_send(pciv_chn, &obj, vif_info);
    } else {
        /* send picture directly */
        PCIV_FMW_SPIN_UNLOCK(flags);
        ret = pciv_fmw_src_pic_send(pciv_chn, &obj, vif_info, NULL);
        PCIV_FMW_SPIN_LOCK(flags);
    }
    PCIV_FMW_SPIN_UNLOCK(flags);

    if (ret != HI_SUCCESS) {
        PCIV_FMW_DEBUG_TRACE("pciv_chn:%d viu frame pic send failed\n", dev_id);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 pciv_firmware_send_pic_from_vo(hi_s32 dev_id, hi_pciv_chn pciv_chn, hi_bool block, hi_void *pic_info)
{
    hi_ulong            flags;
    hi_s32              ret;
    hi_pciv_bind_obj    obj         = {0};
    hi_video_frame_info *vof_info   = NULL;
    pciv_fmw_channel    *fmw_chn    = NULL;

    obj.type = PCIV_BIND_VO;
    obj.vpss_send = HI_FALSE;

    vof_info = (hi_video_frame_info *)pic_info;

    PCIV_FMW_SPIN_LOCK(flags);

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];
    fmw_chn->block = block;
    fmw_chn->mod_id = HI_ID_VO;

    ret = pciv_firmware_src_pic_send(pciv_chn, &obj, vof_info);
    PCIV_FMW_SPIN_UNLOCK(flags);

    if (ret != HI_SUCCESS) {
        PCIV_FMW_DEBUG_TRACE("pciv_chn:%d virtual vou frame pic send failed\n", dev_id);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 pciv_firmware_send_pic_from_vdec(hi_s32 dev_id, hi_pciv_chn pciv_chn, hi_bool block, hi_void *pic_info)
{
    hi_ulong            flags;
    hi_s32              ret;
    pciv_pic_node       *node       = NULL;
    pciv_fmw_channel    *fmw_chn        = NULL;
    hi_video_frame_info *vdecf_info = NULL;

    vdecf_info = (hi_video_frame_info *)pic_info;

    PCIV_FMW_SPIN_LOCK(flags);

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    if (fmw_chn->start != HI_TRUE) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        PCIV_FMW_INFO_TRACE("pciv chn %d have stoped \n", pciv_chn);
        return HI_FAILURE;
    }

    fmw_chn->block = block;

    node = pciv_pic_queue_get_free(&fmw_chn->pic_queue);
    if (node == NULL) {
        fmw_chn->lost_cnt++;
        PCIV_FMW_DEBUG_TRACE("pciv_chn:%d no free node\n", dev_id);
        PCIV_FMW_SPIN_UNLOCK(flags);
        return HI_FAILURE;
    }

    node->pciv_pic.mod_id = HI_ID_VDEC;

    osal_memcpy(&node->pciv_pic.video_frame, vdecf_info, sizeof(hi_video_frame_info));
    if (vdecf_info->v_frame.compress_mode != COMPRESS_MODE_NONE) {
        ret = call_vb_user_add(vdecf_info->pool_id, vdecf_info->v_frame.header_phy_addr[0], VB_UID_PCIV);
    } else {
        ret = call_vb_user_add(vdecf_info->pool_id, vdecf_info->v_frame.phy_addr[0], VB_UID_PCIV);
    }
    HI_ASSERT(ret == HI_SUCCESS);

    pciv_pic_queue_put_busy(&fmw_chn->pic_queue, node);

    PCIV_FMW_SPIN_UNLOCK(flags);
    return HI_SUCCESS;
}



/* called in VIU, virtual VOU or VDEC interrupt handler */
hi_s32 pciv_firmware_send_pic(hi_s32 dev_id, hi_s32 chn_id, hi_bool block, mpp_data_type data_type,
    hi_void *pic_info)
{
    hi_ulong            flags;
    pciv_fmw_channel    *fmw_chn = NULL;
    hi_pciv_chn         pciv_chn = chn_id;

    PCIVFMW_CHECK_CHNID(pciv_chn);
    PCIVFMW_CHECK_PTR(pic_info);

    PCIV_FMW_SPIN_LOCK(flags);
    fmw_chn = &g_fmw_pciv_chn[pciv_chn];
    fmw_chn->get_cnt++;
    PCIV_FMW_SPIN_UNLOCK(flags);

    if (data_type == MPP_DATA_VI_FRAME ||
        data_type == MPP_DATA_AVS_FRAME ||
        data_type == MPP_DATA_VPSS_FRAME) {
        return pciv_firmware_send_pic_from_vi(dev_id, pciv_chn, block, pic_info);
    }
    if (data_type == MPP_DATA_VOU_FRAME) {
        return pciv_firmware_send_pic_from_vo(dev_id, pciv_chn, block, pic_info);
    }
    if (data_type == MPP_DATA_VDEC_FRAME) {
        return pciv_firmware_send_pic_from_vdec(dev_id, pciv_chn, block, pic_info);
    }
    PCIV_FMW_DEBUG_TRACE("pciv_chn:%d data type:%d error\n", dev_id, data_type);
    return HI_FAILURE;
}

static hi_s32 pciv_fmw_vpss_query_state_check(const pciv_fmw_channel *fmw_chn)
{
    if (fmw_chn->create == HI_FALSE) {
        PCIV_FMW_ALERT_TRACE("pciv channel doesn't exist, please create it!\n");
        return HI_FAILURE;
    }

    if (fmw_chn->start == HI_FALSE) {
        PCIV_FMW_ALERT_TRACE("pciv channel has stoped!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_s32 pciv_fmw_vpss_query_mod_check(hi_mod_id mod_id, hi_pciv_chn pciv_chn,
    const pciv_fmw_channel *fmw_chn, const vpss_query_info *query_info)
{
    hi_s32 ret;
    hi_u32 cur_time_ref;

    if (query_info->src_pic_info == NULL) {
        PCIV_FMW_INFO_TRACE("pciv chn:%d src_pic_info is NULL!\n", pciv_chn);
        return HI_FAILURE;
    }

    cur_time_ref = query_info->src_pic_info->video_frame.v_frame.time_ref;
    if (((mod_id == HI_ID_VI) || (mod_id == HI_ID_VO)) && (fmw_chn->time_ref == cur_time_ref)) {
        // duplicate frame not recived again
        PCIV_FMW_ERR_TRACE("pciv chn:%d duplicated frame!\n", pciv_chn);
        return HI_FAILURE;
    }

    if (mod_id == HI_ID_VDEC) {
        // duplicate frame not recived again
        ret = g_pciv_fmw_call_back.pf_query_pciv_chn_share_buf_state(pciv_chn);
        if (ret == HI_FAILURE) {
            PCIV_FMW_INFO_TRACE("pciv chn:%d has no free share buf to receive pic!\n", pciv_chn);
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static hi_void pciv_fmw_vpss_query_set_v_frame(vb_blk_handle vb_handle, const hi_vb_cal_config *vb_config,
    const pciv_fmw_channel* fmw_chn, const vpss_query_info *query_info, hi_video_frame *v_frame)
{
    hi_u32 pic_width;
    hi_u32 pic_height;

    if (query_info->scale_cap == HI_TRUE) {
        pic_width = fmw_chn->pic_attr.width;
        pic_height = fmw_chn->pic_attr.height;

    } else {
        pic_width = query_info->src_pic_info->video_frame.v_frame.width;
        pic_height = query_info->src_pic_info->video_frame.v_frame.height;
    }

    v_frame->width          = pic_width;
    v_frame->height         = pic_height;
    v_frame->pixel_format   = fmw_chn->pic_attr.pixel_format;
    v_frame->compress_mode  = fmw_chn->pic_attr.compress_mode;
    v_frame->dynamic_range  = fmw_chn->pic_attr.dynamic_range;
    v_frame->video_format   = fmw_chn->pic_attr.video_format;
    v_frame->field          = VIDEO_FIELD_FRAME;

    v_frame->header_phy_addr[0] = call_vb_handle_to_phys(vb_handle);
    v_frame->header_phy_addr[1] = v_frame->header_phy_addr[0] + vb_config->head_y_size;
    v_frame->header_phy_addr[2] = v_frame->header_phy_addr[1];

    v_frame->header_vir_addr[0] = call_vb_handle_to_kern(vb_handle);
    v_frame->header_vir_addr[1] = v_frame->header_vir_addr[0] + vb_config->head_y_size;
    v_frame->header_vir_addr[2] = v_frame->header_vir_addr[1];

    v_frame->header_stride[0] = vb_config->head_stride;
    v_frame->header_stride[1] = vb_config->head_stride;
    v_frame->header_stride[2] = vb_config->head_stride;

    v_frame->phy_addr[0] = v_frame->header_phy_addr[0] + vb_config->head_size;
    v_frame->phy_addr[1] = v_frame->phy_addr[0] + vb_config->main_y_size;
    v_frame->phy_addr[2] = v_frame->phy_addr[1];

    v_frame->vir_addr[0] = v_frame->header_vir_addr[0] + vb_config->head_size;
    v_frame->vir_addr[1] = v_frame->vir_addr[0] + vb_config->main_y_size;
    v_frame->vir_addr[2] = v_frame->vir_addr[1];

    v_frame->stride[0] = vb_config->main_stride;
    v_frame->stride[1] = vb_config->main_stride;
    v_frame->stride[2] = vb_config->main_stride;

    v_frame->ext_phy_addr[0] = v_frame->phy_addr[0] + vb_config->main_size;
    v_frame->ext_phy_addr[1] = v_frame->ext_phy_addr[0] + vb_config->ext_y_size;
    v_frame->ext_phy_addr[2] = v_frame->ext_phy_addr[1];

    v_frame->ext_vir_addr[0] = v_frame->vir_addr[0] + vb_config->main_size;
    v_frame->ext_vir_addr[1] = v_frame->ext_vir_addr[0] + vb_config->ext_y_size;
    v_frame->ext_vir_addr[2] = v_frame->ext_vir_addr[1];

    v_frame->ext_stride[0] = vb_config->ext_stride;
    v_frame->ext_stride[1] = vb_config->ext_stride;
    v_frame->ext_stride[2] = vb_config->ext_stride;
}

static hi_s32 pciv_fmw_vpss_query_user(hi_mod_id mod_id, hi_pciv_chn pciv_chn,
    pciv_fmw_channel* fmw_chn, vpss_inst_info *inst_info)
{
    /* if picture from VIU or USER, send to PCIV firmware directly */
    if ((mod_id == HI_ID_VI) || (mod_id == HI_ID_USER)) {
        inst_info->new_frame = HI_TRUE;
        inst_info->vpss_proc = HI_TRUE;
    }
    /* if picture from VEDC, send to PCIV queue and then deal with DSU, check the queue is full or not */
    else if (mod_id == HI_ID_VDEC) {
        if (pciv_pic_queue_get_free_num(&(fmw_chn->pic_queue)) == 0) {
            /* if PCIV queue is full, old undo */
            PCIV_FMW_ERR_TRACE("pciv chn:%d queue is full!\n", pciv_chn);
            return HI_FAILURE;
        }

        inst_info->new_frame = HI_TRUE;
        inst_info->vpss_proc = HI_TRUE;
    }
    return HI_SUCCESS;
}

static hi_s32 pciv_fmw_vpss_query_auto(hi_s32 dev_id, hi_s32 chn_id,
    const vpss_query_info *query_info, const pciv_fmw_channel *fmw_chn, vpss_inst_info *inst_info)
{

    hi_s32              ret;
    vb_base_info        base_info;
    vb_blk_handle       vb_handle;
    hi_vb_cal_config    vb_cal_config;
    hi_video_frame      *v_frame;
    hi_mpp_chn          mpp_chn     = {0};
    hi_char             *mmz_name   = NULL;

    base_info.is_3dnr_buffer = HI_FALSE;
    base_info.align = 0;

    base_info.compress_mode = fmw_chn->pic_attr.compress_mode;
    base_info.dynamic_range = fmw_chn->pic_attr.dynamic_range;
    base_info.video_format  = fmw_chn->pic_attr.video_format;

    base_info.pixel_format  = fmw_chn->pic_attr.pixel_format;
    base_info.width         = fmw_chn->pic_attr.width;
    base_info.height        = fmw_chn->pic_attr.height;

    call_sys_get_vb_cfg(&base_info, &vb_cal_config);

    mpp_chn.mod_id = HI_ID_PCIV;
    mpp_chn.dev_id = dev_id;
    mpp_chn.chn_id = chn_id;
    ret = call_sys_get_mmz_name(&mpp_chn, (hi_void **)&mmz_name);
    HI_ASSERT(ret == HI_SUCCESS);

    vb_handle = call_vb_get_blk_by_size(fmw_chn->blk_size, VB_UID_VPSS, mmz_name);
    if (vb_handle == VB_INVALID_HANDLE) {
        PCIV_FMW_ALERT_TRACE("get VB for pic_size: %d from ddr:%s fail, for grp %d VPSS query\n",
            fmw_chn->blk_size, mmz_name, dev_id);
        return HI_FAILURE;
    }

    inst_info->dest_pic_info[0].video_frame.pool_id = call_vb_handle_to_pool_id(vb_handle);
    v_frame = &inst_info->dest_pic_info[0].video_frame.v_frame;

    pciv_fmw_vpss_query_set_v_frame(vb_handle, &vb_cal_config, fmw_chn, query_info, v_frame);

    inst_info->vpss_proc = HI_TRUE;
    inst_info->new_frame = HI_TRUE;
    return HI_SUCCESS;
}

static hi_void pciv_fmw_vpss_query_old_undo(pciv_fmw_channel *fmw_chn, vpss_inst_info *inst_info)
{
    inst_info->vpss_proc     = HI_FALSE;
    inst_info->new_frame     = HI_FALSE;
    inst_info->double_frame  = HI_FALSE;
    inst_info->update_backup = HI_FALSE;
    fmw_chn->old_undo_cnt++;
}

hi_s32 pciv_fmw_vpss_query(hi_s32 dev_id, hi_s32 chn_id,
    vpss_query_info *query_info, vpss_inst_info *inst_info)
{
    hi_ulong            flags;
    hi_mod_id           mod_id;
    hi_s32              ret;
    pciv_fmw_channel    *fmw_chn    = NULL;
    hi_pciv_chn         pciv_chn    = chn_id;

    PCIVFMW_CHECK_CHNID(pciv_chn);
    PCIVFMW_CHECK_PTR(query_info);
    PCIVFMW_CHECK_PTR(inst_info);

    PCIV_FMW_SPIN_LOCK(flags);
    fmw_chn = &g_fmw_pciv_chn[pciv_chn];
    mod_id = query_info->src_pic_info->mod_id;

    ret = pciv_fmw_vpss_query_state_check(fmw_chn);
    if (ret != HI_SUCCESS) {
        PCIV_FMW_SPIN_UNLOCK(flags);
        return ret;
    }

    ret = pciv_fmw_vpss_query_mod_check(mod_id, pciv_chn, fmw_chn, query_info);
    if (ret != HI_SUCCESS) {
        goto OLD_UNDO;
    }
    if (query_info->malloc_buffer == HI_FALSE) {
        /* user */
        ret = pciv_fmw_vpss_query_user(mod_id, pciv_chn, fmw_chn, inst_info);
        if (ret != HI_SUCCESS) {
            goto OLD_UNDO;
        }
    } else {
        /* auto */
        if (!ckfn_sys_get_vb_cfg()) {
            PCIV_FMW_ERR_TRACE("sys_get_vb_cfg is NULL!\n");
            PCIV_FMW_SPIN_UNLOCK(flags);
            return HI_FAILURE;
        }
        ret = pciv_fmw_vpss_query_auto(dev_id, chn_id, query_info, fmw_chn, inst_info);
        if (ret != HI_SUCCESS) {
            goto OLD_UNDO;
        }
    }
    fmw_chn->new_do_cnt++;
    PCIV_FMW_SPIN_UNLOCK(flags);
    return HI_SUCCESS;

OLD_UNDO:
    PCIV_FMW_SPIN_UNLOCK(flags);
    pciv_fmw_vpss_query_old_undo(fmw_chn, inst_info);
    return HI_SUCCESS;
}

static hi_s32 pciv_fmw_vpss_send_state_check(pciv_fmw_channel *fmw_chn, vpss_send_info *send_info)
{
    if (fmw_chn->create == HI_FALSE) {
        PCIV_FMW_ALERT_TRACE("pciv channel doesn't exist, please create it!\n");
        return HI_FAILURE;
    }

    if (fmw_chn->start == HI_FALSE) {
        PCIV_FMW_ALERT_TRACE("pciv channel has stoped!\n");
        return HI_FAILURE;
    }

    if (send_info->suc == HI_FALSE) {
        PCIV_FMW_ERR_TRACE("bsuc is failure.\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static hi_void pciv_fmw_vpss_send_get_bind_obj(vpss_send_info *send_info, hi_pciv_bind_obj *bind_obj)
{
    hi_mod_id mod_id;

    mod_id = send_info->dest_pic_info[0]->mod_id;
    if (mod_id == HI_ID_VDEC) {
        bind_obj->type = PCIV_BIND_VDEC;
    } else if (mod_id == HI_ID_VO) {
        bind_obj->type = PCIV_BIND_VO;
    } else {
        bind_obj->type = PCIV_BIND_VI;
    }

    bind_obj->vpss_send = HI_TRUE;
    return;
}

hi_s32 pciv_fmw_vpss_send(hi_s32 dev_id, hi_s32 chn_id, vpss_send_info *send_info)
{
    hi_ulong            flags;
    hi_s32              ret;
    hi_pciv_bind_obj    bind_obj;
    rgn_info            info        = {0};
    hi_pciv_chn         pciv_chn    = chn_id;
    pciv_fmw_channel    *fmw_chn    = NULL;

    PCIVFMW_CHECK_CHNID(pciv_chn);
    PCIVFMW_CHECK_PTR(send_info);
    PCIVFMW_CHECK_PTR(send_info->dest_pic_info[0]);

    fmw_chn = &g_fmw_pciv_chn[pciv_chn];

    ret = pciv_fmw_vpss_send_state_check(fmw_chn, send_info);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    PCIV_FMW_SPIN_LOCK(flags);
    fmw_chn->get_cnt++;
    PCIV_FMW_SPIN_UNLOCK(flags);

    pciv_fmw_vpss_send_get_bind_obj(send_info, &bind_obj);

    ret = pciv_fmw_get_region(pciv_chn, OVERLAYEX_RGN, &info);

    if (fmw_chn->pic_attr.width != send_info->dest_pic_info[0]->video_frame.v_frame.width ||
        fmw_chn->pic_attr.height != send_info->dest_pic_info[0]->video_frame.v_frame.height ||
        (info.num > 0)) {
        pciv_fmw_put_region(pciv_chn, OVERLAYEX_RGN);
        PCIV_FMW_SPIN_LOCK(flags);
        fmw_chn->block = send_info->dest_pic_info[0]->block_mode;
        ret = pciv_firmware_src_pic_send(pciv_chn, &bind_obj, &(send_info->dest_pic_info[0]->video_frame));
        if (ret != HI_SUCCESS) {
            PCIV_FMW_ALERT_TRACE("pciv_firmware_src_pic_send failed! pciv chn %d\n", pciv_chn);
            PCIV_FMW_SPIN_UNLOCK(flags);
            return HI_FAILURE;
        }
        PCIV_FMW_SPIN_UNLOCK(flags);
    } else {
        /* bypass: Send pic directly */
        /* [HSCP201308020003] l00181524,2013.08.16, pciv_fmw_src_pic_send add
        lock inner, if add lock the will appeare lock itself bug,cannot add lock */
        PCIV_FMW_SPIN_LOCK(flags);
        fmw_chn->block = send_info->dest_pic_info[0]->block_mode;
        PCIV_FMW_SPIN_UNLOCK(flags);
        ret = pciv_fmw_src_pic_send(pciv_chn, &bind_obj, &(send_info->dest_pic_info[0]->video_frame), NULL);

        if (ret != HI_SUCCESS) {
            PCIV_FMW_ALERT_TRACE("pciv_fmw_src_pic_send failed! pciv chn %d, ret: 0x%x.\n", pciv_chn, ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

hi_s32 pciv_fmw_reset_call_back(hi_s32 dev_id, hi_s32 chn_id, hi_void *pv_data)
{
    hi_ulong            flags;
    hi_s32              i;
    hi_s32              ret;
    hi_u32              busy_num;
    hi_pciv_chn         pciv_chn;
    pciv_fmw_channel    *fmw_chn = NULL;
    pciv_pic_node       *node = NULL;

    pciv_chn = chn_id;
    PCIV_FMW_SPIN_LOCK(flags);
    fmw_chn = &g_fmw_pciv_chn[pciv_chn];
    fmw_chn->start = HI_FALSE;
    PCIV_FMW_SPIN_UNLOCK(flags);

    pciv_firmware_wait_send_end(fmw_chn);

    PCIV_FMW_SPIN_LOCK(flags);

    /* put the node from busy to free */
    busy_num = pciv_pic_queue_get_busy_num(&fmw_chn->pic_queue);
    for (i = 0; i < busy_num; i++) {
        node = pciv_pic_queue_get_busy(&fmw_chn->pic_queue);
        if (node == NULL) {
            PCIV_FMW_SPIN_UNLOCK(flags);
            PCIV_FMW_ERR_TRACE("pciv_pic_queue_get_busy failed! pciv chn %d.\n", pciv_chn);
            return HI_FAILURE;
        }

        ret = call_vb_user_sub(node->pciv_pic.video_frame.pool_id,
                               node->pciv_pic.video_frame.v_frame.phy_addr[0], VB_UID_PCIV);
        HI_ASSERT(ret == HI_SUCCESS);

        pciv_pic_queue_put_free(&fmw_chn->pic_queue, node);
    }

    fmw_chn->pic_queue.max      = VDEC_MAX_SEND_CNT;
    fmw_chn->pic_queue.free_num = VDEC_MAX_SEND_CNT;
    fmw_chn->pic_queue.busy_num = 0;

    PCIV_FMW_SPIN_UNLOCK(flags);

    return HI_SUCCESS;
}

hi_s32 pciv_firmware_register_func(pciv_fmw_callback *call_back)
{
    PCIVFMW_CHECK_PTR(call_back);
    PCIVFMW_CHECK_PTR(call_back->pf_src_send_pic);
    PCIVFMW_CHECK_PTR(call_back->pf_recv_pic_free);
    PCIVFMW_CHECK_PTR(call_back->pf_query_pciv_chn_share_buf_state);

    osal_memcpy(&g_pciv_fmw_call_back, call_back, sizeof(pciv_fmw_callback));

    return HI_SUCCESS;
}

static hi_void pciv_firmware_init_vgs_opt(hi_pciv_chn pciv_chn)
{
    vgs_online_opt      *vgs_opt = &g_fmw_pciv_chn[pciv_chn].vgs_opt;

    /* init VGS opt */
    osal_memset(vgs_opt, 0, sizeof(vgs_online_opt));

    vgs_opt->crop = HI_FALSE;
    vgs_opt->cover = HI_FALSE;
    vgs_opt->osd = HI_FALSE;
    vgs_opt->force_h_filt = HI_FALSE;
    vgs_opt->force_v_filt = HI_FALSE;
    vgs_opt->deflicker = HI_FALSE;
}

static hi_void pciv_fmw_init_base(hi_void)
{
    hi_s32              i, ret;
    bind_sender_info    sender_info;
    bind_receiver_info  receiver;

    sender_info.mod_id = HI_ID_PCIV;
    sender_info.max_dev_cnt = 1;
    sender_info.max_chn_cnt = PCIV_MAX_CHN_NUM;
    sender_info.give_bind_call_back = HI_NULL;
    ret = call_sys_register_sender(&sender_info);
    HI_ASSERT(ret == HI_SUCCESS);

    receiver.mod_id = HI_ID_PCIV;
    receiver.max_dev_cnt = 1;
    receiver.max_chn_cnt = PCIV_MAX_CHN_NUM;
    receiver.call_back = pciv_firmware_send_pic;
    receiver.reset_call_back = pciv_fmw_reset_call_back;
    ret = call_sys_register_receiver(&receiver);
    HI_ASSERT(ret == HI_SUCCESS);

    init_timer(&g_timer_pciv_send_vdec_pic);
    g_timer_pciv_send_vdec_pic.expires  = jiffies + msecs_to_jiffies(FMW_PCIV_TIMER_EXPIRES);
    g_timer_pciv_send_vdec_pic.function = pciv_send_vdec_pic_timer_func;
    g_timer_pciv_send_vdec_pic.data     = 0;
    add_timer(&g_timer_pciv_send_vdec_pic);

    g_vb_pool.pool_count = 0;

    osal_memset(g_fmw_pciv_chn, 0, sizeof(g_fmw_pciv_chn));
    for (i = 0; i < PCIV_FMW_MAX_CHN_NUM; i++) {
        g_fmw_pciv_chn[i].start     = HI_FALSE;
        g_fmw_pciv_chn[i].send_cnt  = 0;
        g_fmw_pciv_chn[i].get_cnt   = 0;
        g_fmw_pciv_chn[i].resp_cnt  = 0;
        g_fmw_pciv_chn[i].count     = 0;
        g_fmw_pciv_chn[i].rgn_num   = 0;
        init_timer(&g_fmw_pciv_chn[i].buf_timer);
        pciv_firmware_init_vgs_opt(i);
    }

}

static hi_void pciv_fmw_init_vpss(hi_void)
{
    hi_s32              ret;
    vpss_register_info  vpss_rgst_info;

    vpss_rgst_info.mod_id           = HI_ID_PCIV;
    vpss_rgst_info.vpss_query       = pciv_fmw_vpss_query;
    vpss_rgst_info.vpss_send        = pciv_fmw_vpss_send;
    vpss_rgst_info.reset_call_back  = pciv_fmw_reset_call_back;
    ret = call_vpss_register(&vpss_rgst_info);
    HI_ASSERT(ret == HI_SUCCESS);
}

static hi_void pciv_fmw_init_rgn_base(rgn_capacity *capacity)
{
    /* the level of region */
    capacity->pfn_check_attr            = HI_NULL;
    capacity->pfn_check_chn_capacity    = HI_NULL;
    capacity->layer.is_comm             = HI_FALSE;

    capacity->layer.spt_reset = HI_TRUE;
    capacity->layer.layer_min = FMW_RGN_LAYER_MIN;
    capacity->layer.layer_max = FMW_RGN_LAYER_MAX;

    /* the start position of region */
    capacity->point.is_comm         = HI_FALSE;
    capacity->point.spt_reset       = HI_TRUE;
    capacity->point.start_x_align   = FMW_POINT_START_X_ALIGN;
    capacity->point.start_y_align   = FMW_POINT_START_Y_ALIGN;
    capacity->point.point_min.x     = RGN_OVERLAYEX_MIN_X;
    capacity->point.point_min.y     = RGN_OVERLAYEX_MIN_Y;
    capacity->point.point_max.x     = RGN_OVERLAYEX_MAX_X;
    capacity->point.point_max.y     = RGN_OVERLAYEX_MAX_Y;

    /* the weight and height of region */
    capacity->size.is_comm          = HI_TRUE;
    capacity->size.spt_reset        = HI_FALSE;
    capacity->size.width_align      = FMW_RGN_SIZE_WIDTH_ALIGN;
    capacity->size.height_align     = FMW_RGN_SIZE_HEIGHT_ALIGN;
    capacity->size.size_min.width   = FMW_RGN_SIZE_MIN_WIDTH_ALIGN;
    capacity->size.size_min.height  = FMW_RGN_SIZE_MIN_HEIGHT_ALIGN;
    capacity->size.size_max.width   = RGN_OVERLAYEX_MAX_WIDTH;
    capacity->size.size_max.height  = RGN_OVERLAYEX_MAX_HEIGHT;
    capacity->size.max_area         = RGN_OVERLAYEX_MAX_WIDTH * RGN_OVERLAYEX_MAX_HEIGHT;

    /* the pixel format of region */
    capacity->spt_pixl_fmt              = HI_TRUE;
    capacity->pixl_fmt.is_comm          = HI_TRUE;
    capacity->pixl_fmt.spt_reset        = HI_FALSE;
    capacity->pixl_fmt.pixl_fmt_num     = FMW_PIXL_FMT_NUM;
    capacity->pixl_fmt.pixel_format[0]  = PIXEL_FORMAT_ARGB_1555;
    capacity->pixl_fmt.pixel_format[1]  = PIXEL_FORMAT_ARGB_4444;
    capacity->pixl_fmt.pixel_format[2]  = PIXEL_FORMAT_ARGB_8888;
}

static hi_void pciv_fmw_init_rgn_other(rgn_capacity *capacity, rgn_register_info *rgn_info)
{
    /* the front alpha of region */
    capacity->spt_fg_alpha          = HI_TRUE;
    capacity->fg_alpha.is_comm      = HI_FALSE;
    capacity->fg_alpha.spt_reset    = HI_TRUE;
    capacity->fg_alpha.alpha_max    = FMW_ALPHA_MAX;
    capacity->fg_alpha.alpha_min    = FMW_ALPHA_MIN;

    /* the back alpha of region */
    capacity->spt_bg_alpha          = HI_TRUE;
    capacity->bg_alpha.is_comm      = HI_FALSE;
    capacity->bg_alpha.spt_reset    = HI_TRUE;
    capacity->bg_alpha.alpha_max    = FMW_ALPHA_MAX;
    capacity->bg_alpha.alpha_min    = FMW_ALPHA_MIN;

    /* the panoramic alpha of region */
    capacity->spt_global_alpha          = HI_FALSE;
    capacity->global_alpha.is_comm      = HI_FALSE;
    capacity->global_alpha.spt_reset    = HI_TRUE;
    capacity->global_alpha.alpha_max    = FMW_ALPHA_MAX;
    capacity->global_alpha.alpha_min    = FMW_ALPHA_MIN;

    /* suport backcolor or not */
    capacity->spt_bg_clr        = HI_TRUE;
    capacity->bg_clr.is_comm    = HI_TRUE;
    capacity->bg_clr.spt_reset  = HI_TRUE;

    /* rank */
    capacity->chn_sort.is_sort      = HI_TRUE;
    capacity->chn_sort.key          = RGN_SORT_BY_LAYER;
    capacity->chn_sort.small_to_big = HI_TRUE;

    /* QP */
    capacity->spt_qp        = HI_FALSE;
    capacity->qp.is_comm    = HI_FALSE;
    capacity->qp.spt_reset  = HI_TRUE;
    capacity->qp.qp_abs_max = FMW_QP_ABS_MAX;
    capacity->qp.qp_abs_min = FMW_QP_ABS_MIN;
    capacity->qp.qp_rel_max = FMW_QP_REL_MAX;
    capacity->qp.qp_rel_min = FMW_QP_REL_MIN;

    /* suport bitmap or not */
    capacity->spt_bitmap        = HI_TRUE;

    /* suport overlap or not */
    capacity->spt_overlap       = HI_TRUE;

    /* memery STRIDE */
    capacity->stride            = FMW_MEMERY_STRIDE;

    capacity->rgn_num_in_chn    = OVERLAYEX_MAX_NUM_PCIV;

    rgn_info->mod_id            = HI_ID_PCIV;
    rgn_info->max_dev_cnt       = 1;
    rgn_info->max_chn_cnt       = PCIV_MAX_CHN_NUM;
}

static hi_void pciv_fmw_init_rgn(hi_void)
{
    hi_s32              ret;
    rgn_capacity        capacity;
    rgn_register_info   rgn_info;

    osal_memset(&capacity, 0, sizeof(rgn_capacity));

    pciv_fmw_init_rgn_base(&capacity);

    pciv_fmw_init_rgn_other(&capacity, &rgn_info);

    /* register OVERLAY */
    ret = call_rgn_register_mod(OVERLAYEX_RGN, &capacity, &rgn_info);
    HI_ASSERT(ret == HI_SUCCESS);
}

hi_s32 pciv_fmw_init(hi_void *p)
{

    if (g_pciv_fmw_state == PCIVFMW_STATE_STARTED) {
        return HI_SUCCESS;
    }

    pciv_fmw_init_base();

    if ((CKFN_VPSS_ENTRY()) && (ckfn_vpss_register())) {
        pciv_fmw_init_vpss();
    }

    /* register OVERLAY to region */
    if ((ckfn_rgn()) && (ckfn_rgn_register_mod())) {
        pciv_fmw_init_rgn();
    }

    g_pciv_fmw_state = PCIVFMW_STATE_STARTED;
    return HI_SUCCESS;
}

hi_void pciv_fmw_exit(hi_void)
{
    hi_s32 i, ret;

    if (g_pciv_fmw_state == PCIVFMW_STATE_STOPED) {
        return;
    }

    call_sys_unregister_receiver(HI_ID_PCIV);
    call_sys_unregister_sender(HI_ID_PCIV);

    for (i = 0; i < PCIV_FMW_MAX_CHN_NUM; i++) {
        /* stop channel */
        HI_ASSERT(pciv_firmware_stop(i) == HI_SUCCESS);
        /* destroy channel */
        HI_ASSERT(pciv_firmware_destroy(i) == HI_SUCCESS);

        del_timer_sync(&g_fmw_pciv_chn[i].buf_timer);
    }

    del_timer_sync(&g_timer_pciv_send_vdec_pic);

    if ((CKFN_VPSS_ENTRY()) && (ckfn_vpss_un_register())) {
        HI_ASSERT(call_vpss_un_register(HI_ID_PCIV) == HI_SUCCESS);
    }

    if ((ckfn_rgn()) && (ckfn_rgn_unregister_mod())) {
        /* unregister OVERLAY */
        ret = call_rgn_unregister_mod(OVERLAYEX_RGN, HI_ID_PCIV);
        HI_ASSERT(ret == HI_SUCCESS);
    }

    g_pciv_fmw_state = PCIVFMW_STATE_STOPED;
    return;
}

static hi_void pciv_fmw_notify(mod_notice_id notice)
{
    if (notice == MOD_NOTICE_STOP) {
        g_pciv_fmw_state = PCIVFMW_STATE_STOPPING;
    }
    return;
}

static hi_void pciv_fmw_query_state(mod_state *state)
{
    *state = MOD_STATE_FREE;
    return;
}

#ifndef DISABLE_DEBUG_INFO

static hi_char *pciv_pf_to_string(hi_pixel_format pixel_format)
{
    hi_char *pf_string = HI_NULL;
    switch (pixel_format) {
        case PIXEL_FORMAT_YVU_SEMIPLANAR_422:
            pf_string = "YVU_SP_422";
            break;
        case PIXEL_FORMAT_YVU_SEMIPLANAR_420:
            pf_string = "YVU_SP_420";
            break;
        default:
            pf_string = "BUTT";
            break;
    }
    return pf_string;
}

static hi_char *pciv_dr_to_string(hi_dynamic_range dynamic_range)
{
    hi_char *dr_string = HI_NULL;
    switch (dynamic_range) {
        case DYNAMIC_RANGE_SDR8:
            dr_string = "SDR8";
            break;
        case DYNAMIC_RANGE_SDR10:
            dr_string = "SDR10";
            break;
        case DYNAMIC_RANGE_HDR10:
            dr_string = "HDR10";
            break;
        case DYNAMIC_RANGE_HLG:
            dr_string = "HLG";
            break;
        case DYNAMIC_RANGE_SLF:
            dr_string = "SLF";
            break;
        case DYNAMIC_RANGE_XDR:
            dr_string = "XDR";
            break;
        default:
            dr_string = "BUTT";
            break;
    }
    return dr_string;
}

static hi_char *pciv_cm_to_string(hi_compress_mode compress_mode)
{
    hi_char *cm_string = HI_NULL;
    switch (compress_mode) {
        case COMPRESS_MODE_NONE:
            cm_string = "NONE";
            break;
        case COMPRESS_MODE_SEG:
            cm_string = "SEG";
            break;
        case COMPRESS_MODE_TILE:
            cm_string = "TILE";
            break;
        case COMPRESS_MODE_LINE:
            cm_string = "LINE";
            break;
        case COMPRESS_MODE_FRAME:
            cm_string = "FRAME";
            break;
        default:
            cm_string = "BUTT";
            break;
    }
    return cm_string;
}

static hi_char *pciv_vf_to_string(hi_video_format video_format)
{
    hi_char *vf_string = HI_NULL;
    switch (video_format) {
        case VIDEO_FORMAT_LINEAR:
            vf_string = "LINEAR";
            break;
        case VIDEO_FORMAT_TILE_64x16:
            vf_string = "TILE_64x16";
            break;
        case VIDEO_FORMAT_TILE_16x8:
            vf_string = "TILE_16x8";
            break;
        case VIDEO_FORMAT_LINEAR_DISCRETE:
            vf_string = "LINEAR_DISCRETE";
            break;
        default:
            vf_string = "BUTT";
            break;
    }
    return vf_string;
}

static hi_void pciv_fmw_proc_show_chn_info(osal_proc_entry_t *s)
{
    pciv_fmw_channel    *chn_ctx;
    hi_pciv_chn         pciv_chn;

    osal_seq_printf(s, "\n-----PCIV FMW CHN INFO----------------------------------------------------------\n");
    osal_seq_printf(s, "%6s"     "%12s"    "%12s"     "%12s"
                    "%12s"     "%12s"   "%12s"     "%8s\n",
                    "PciChn", "GetCnt", "SendCnt", "RespCnt",
                    "LostCnt", "NewDo", "OldUndo", "PoolId0");
    for (pciv_chn = 0; pciv_chn < PCIV_FMW_MAX_CHN_NUM; pciv_chn++) {
        chn_ctx = &g_fmw_pciv_chn[pciv_chn];
        if (chn_ctx->create == HI_FALSE) {
            continue;
        }
        osal_seq_printf(s, "%6d"    "%12d"  "%12d"  "%12d"  "%12d"  "%12d"  "%12d"  "%8d\n",
                        pciv_chn,
                        chn_ctx->get_cnt,
                        chn_ctx->send_cnt,
                        chn_ctx->resp_cnt,
                        chn_ctx->lost_cnt,
                        chn_ctx->new_do_cnt,
                        chn_ctx->old_undo_cnt,
                        chn_ctx->au32_pool_id[0]);
    }
}

static hi_void pciv_fmw_proc_show_chn_pic_attr_info(osal_proc_entry_t *s)
{
    pciv_fmw_channel    *chn_ctx;
    hi_pciv_chn         pciv_chn;

    osal_seq_printf(s, "\n-----PCIV FMW CHN PIC ATTR INFO--------------------------------------------------\n");
    osal_seq_printf(s, "%6s"     "%8s"    "%8s"     "%8s"
                    "%12s"       "%8s"       "%10s"       "%17s\n",
                    "PciChn", "Width", "Height", "stride",
                    "PixFormat", "Dynamic",  "Compress",  "VideoFormat");
    for (pciv_chn = 0; pciv_chn < PCIV_FMW_MAX_CHN_NUM; pciv_chn++) {
        chn_ctx = &g_fmw_pciv_chn[pciv_chn];
        if (chn_ctx->create == HI_FALSE) {
            continue;
        }
        osal_seq_printf(s, "%6d"    "%8d"   "%8d"   "%8d"   "%12s"  "%8s"   "%10s"   "%17s\n",
                        pciv_chn,
                        chn_ctx->pic_attr.width,
                        chn_ctx->pic_attr.height,
                        chn_ctx->pic_attr.stride[0],
                        pciv_pf_to_string(chn_ctx->pic_attr.pixel_format),
                        pciv_dr_to_string(chn_ctx->pic_attr.dynamic_range),
                        pciv_cm_to_string(chn_ctx->pic_attr.compress_mode),
                        pciv_vf_to_string(chn_ctx->pic_attr.video_format));
    }
}

static hi_void pciv_fmw_proc_show_chn_queue_info(osal_proc_entry_t *s)
{
    pciv_fmw_channel    *chn_ctx;
    hi_pciv_chn         pciv_chn;

    osal_seq_printf(s, "\n-----PCIV CHN QUEUE INFO----------------------------------------------------------\n");
    osal_seq_printf(s, "%6s"     "%8s"     "%8s"     "%8s"    "%12s\n",
                    "PciChn", "busynum", "freenum", "state", "Timer");
    for (pciv_chn = 0; pciv_chn < PCIV_FMW_MAX_CHN_NUM; pciv_chn++) {
        chn_ctx = &g_fmw_pciv_chn[pciv_chn];
        if (chn_ctx->create == HI_FALSE) {
            continue;
        }
        osal_seq_printf(s, "%6d"    "%8d"   "%8d"   "%8d"   "%12d\n",
                        pciv_chn,
                        chn_ctx->pic_queue.busy_num,
                        chn_ctx->pic_queue.free_num,
                        chn_ctx->send_state,
                        chn_ctx->timer_cnt);
    }
}

static hi_void pciv_fmw_proc_show_chn_call_vgs_info(osal_proc_entry_t *s)
{
    pciv_fmw_channel    *chn_ctx;
    hi_pciv_chn         pciv_chn;

    osal_seq_printf(s, "\n-----PCIV CHN CALL VGS INFO----------------------------------------------------------\n");
    osal_seq_printf(s, "%6s"    "%12s"     "%12s"     "%12s" \
                    "%12s"      "%12s"      "%12s"     "%12s"     "%12s" \
                    "%12s"      "%12s"      "%12s"     "%12s"     "%12s\n",
                    "PciChn",   "JobSuc",   "JobFail", "EndSuc",  "EndFail",
                    "MoveSuc",  "MoveFail", "OsdSuc",  "OsdFail", "ZoomSuc",
                    "ZoomFail", "MoveCb",   "OsdCb",   "ZoomCb");

    for (pciv_chn = 0; pciv_chn < PCIV_FMW_MAX_CHN_NUM; pciv_chn++) {
        chn_ctx = &g_fmw_pciv_chn[pciv_chn];
        if (chn_ctx->create == HI_FALSE) {
            continue;
        }
        osal_seq_printf(s, "%6d"    "%12d"  "%12d"  "%12d"  "%12d"  "%12d"  \
                        "%12d"  "%12d"  "%12d"  "%12d"  "%12d"  "%12d"  "%12d"  "%12d\n",
                        pciv_chn,
                        chn_ctx->add_job_suc_cnt,
                        chn_ctx->add_job_fail_cnt,
                        chn_ctx->end_job_suc_cnt,
                        chn_ctx->end_job_fail_cnt,
                        chn_ctx->move_task_suc_cnt,
                        chn_ctx->move_task_fail_cnt,
                        chn_ctx->osd_task_suc_cnt,
                        chn_ctx->osd_task_fail_cnt,
                        chn_ctx->zoom_task_suc_cnt,
                        chn_ctx->zoom_task_fail_cnt,
                        chn_ctx->move_cb_cnt,
                        chn_ctx->osd_cb_cnt,
                        chn_ctx->zoom_cb_cnt);
    }
}


hi_s32 pciv_fmw_proc_show(osal_proc_entry_t *s)
{
    osal_seq_printf(s, "\n[PCIVF] Version: [" MPP_VERSION "], Build Time:["__DATE__", "__TIME__"]\n");

    pciv_fmw_proc_show_chn_info(s);

    pciv_fmw_proc_show_chn_pic_attr_info(s);

    pciv_fmw_proc_show_chn_queue_info(s);

    pciv_fmw_proc_show_chn_call_vgs_info(s);

    return HI_SUCCESS;
}
#endif

static pciv_fmw_export_func g_export_funcs = {
    .pfn_pciv_send_pic = pciv_firmware_viu_send_pic,
};

static hi_u32 pciv_get_ver_magic(hi_void)
{
    return VERSION_MAGIC;
}

static umap_module g_pciv_fmw_module = {
    .mod_id = HI_ID_PCIVFMW,
    .mod_name = "pcivfmw",

    .pfn_init = pciv_fmw_init,
    .pfn_exit = pciv_fmw_exit,
    .pfn_query_state = pciv_fmw_query_state,
    .pfn_notify = pciv_fmw_notify,
    .pfn_ver_checker = pciv_get_ver_magic,

    .export_funcs = &g_export_funcs,
    .data = HI_NULL,
};

static int __init pciv_fmw_mod_init(hi_void)
{
#ifndef DISABLE_DEBUG_INFO
    osal_proc_entry_t *proc = HI_NULL;

    proc = osal_create_proc_entry(PROC_ENTRY_PCIVFMW, NULL);

    if (proc == HI_NULL) {
        osal_printk("PCIV firmware create proc error\n");
        goto FAIL0;
    }

    proc->read = pciv_fmw_proc_show;
#endif

    if (cmpi_register_module(&g_pciv_fmw_module)) {
        osal_printk("regist pciv firmware module err.\n");
        goto FAIL1;
    }

    osal_spin_lock_init(&g_pciv_fmw_lock);

    osal_printk("load pciv_firmware.ko ....OK!\n");

    return HI_SUCCESS;
FAIL1:
    osal_remove_proc_entry(PROC_ENTRY_PCIVFMW, NULL);
FAIL0:
    osal_printk("load pciv_firmware.ko for %s...fail!\n", CHIP_NAME);
    return HI_FAILURE;
}

static hi_void __exit pciv_fmw_mod_exit(hi_void)
{
#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(PROC_ENTRY_PCIVFMW, NULL);
#endif
    cmpi_unregister_module(HI_ID_PCIVFMW);

    osal_spin_lock_destory(&g_pciv_fmw_lock);

    osal_printk("unload pciv_firmware.ko for %s...fail!\n", CHIP_NAME);
    return;
}

EXPORT_SYMBOL(pciv_firmware_create);
EXPORT_SYMBOL(pciv_firmware_destroy);
EXPORT_SYMBOL(pciv_firmware_set_attr);
EXPORT_SYMBOL(pciv_firmware_start);
EXPORT_SYMBOL(pciv_firmware_stop);
EXPORT_SYMBOL(pciv_firmware_window_vb_create);
EXPORT_SYMBOL(pciv_firmware_window_vb_destroy);
EXPORT_SYMBOL(pciv_firmware_malloc);
EXPORT_SYMBOL(pciv_firmware_free);
EXPORT_SYMBOL(pciv_firmware_malloc_chn_buffer);
EXPORT_SYMBOL(pciv_firmware_free_chn_buffer);
EXPORT_SYMBOL(pciv_firmware_recv_pic_and_send);
EXPORT_SYMBOL(pciv_firmware_src_pic_free);
EXPORT_SYMBOL(pciv_firmware_register_func);

module_init(pciv_fmw_mod_init);
module_exit(pciv_fmw_mod_exit);

MODULE_AUTHOR("HiMPP GRP");
MODULE_LICENSE("GPL");
MODULE_VERSION(MPP_VERSION);
