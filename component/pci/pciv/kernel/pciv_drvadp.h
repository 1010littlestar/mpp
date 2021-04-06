/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2008-2019. All rights reserved.
 * Description   : pciv_drvadp functions declaration
 * Author        : Hisilicon multimedia software group
 * Created       : 2009/07/03
 */
#ifndef __PCIV_DRVADP_H__
#define __PCIV_DRVADP_H__

#include <linux/list.h>
#include <linux/fs.h>

#include "hi_comm_pciv_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct tag_pciv_send_task{
    struct list_head list;

    hi_bool     read;
    hi_ulong    src_phy_addr;
    hi_ulong    dst_phy_addr;
    hi_u32      len;
    hi_u64      prv_data[5];
    hi_u64      prv_data2[2];
    hi_void     *pv_prv_data;
    hi_void     (*call_back)(struct tag_pciv_send_task *task);
} pciv_send_task;

typedef struct {
    hi_pciv_chn  pciv_chn;       /* the PCI device to be notified */
    pciv_pic     pic_info;
} pciv_notify_pic_end;


typedef enum {
    PCIV_MSGTYPE_CREATE,
    PCIV_MSGTYPE_START,
    PCIV_MSGTYPE_DESTROY,
    PCIV_MSGTYPE_SETATTR,
    PCIV_MSGTYPE_GETATTR,
    PCIV_MSGTYPE_CMDECHO,
    PCIV_MSGTYPE_WRITEDONE,
    PCIV_MSGTYPE_READDONE,
    PCIV_MSGTYPE_NOTIFY,
    PCIV_MSGTYPE_MALLOC,
    PCIV_MSGTYPE_FREE,
    PCIV_MSGTYPE_BIND,
    PCIV_MSGTYPE_UNBIND,
    PCIV_MSGTYPE_BUTT
} pciv_msg_type;

#define PCIV_MSG_HEADLEN (16)
#define PCIV_MSG_MAXLEN  (384 - PCIV_MSG_HEADLEN)

typedef struct {
    hi_u32          target; /* the final user of this message */
    pciv_msg_type   msg_type;
    hi_u32          dev_type;
    hi_u32          msg_len;
    hi_u8           c_msg_body[PCIV_MSG_MAXLEN];
} pciv_msg;


hi_s32 pciv_drv_adp_add_dma_task(pciv_send_task *task);
hi_s32 pciv_drv_adp_dma_end_notify(hi_pciv_remote_obj *remote_obj, pciv_pic *recv_pic);
hi_s32 pciv_drv_adp_buf_free_notify(hi_pciv_remote_obj *remote_obj, pciv_pic *recv_pic);
hi_s32 pciv_drv_adp_addr_check(hi_pciv_dma_block *block, hi_bool is_read);
hi_s32 pciv_drv_adp_send_msg(hi_pciv_remote_obj *remote_obj, pciv_msg_type type, pciv_pic *recv_pic);
hi_s32 pciv_drv_adp_get_base_window(hi_pciv_base_window *base_win);
hi_s32 pciv_drv_adp_get_local_id(hi_void);
hi_s32 pciv_drv_adp_enum_chip(hi_s32 chip_array[PCIV_MAX_CHIPNUM]);
hi_s32 pciv_drv_adp_check_remote(hi_s32 remote_id);
hi_s32 pciv_drv_adp_init(hi_void);
hi_void pciv_drv_adp_exit(hi_void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


