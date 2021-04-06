/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description   : pciv functions declaration
 * Author        : Hisilicon multimedia software group
 * Created       : 2009/07/03
 */
#ifndef __PCIV_H__
#define __PCIV_H__

#include <linux/list.h>
#include <linux/fs.h>

#include "hi_comm_pciv_adapt.h"
#include "pciv_fmwext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

hi_s32 pciv_create(hi_pciv_chn chn, const hi_pciv_attr *attr);
hi_s32 pciv_destroy(hi_pciv_chn chn);
hi_s32 pciv_set_attr(hi_pciv_chn chn, const hi_pciv_attr *attr);
hi_s32 pciv_get_attr(hi_pciv_chn chn, hi_pciv_attr *attr);
hi_s32 pciv_start(hi_pciv_chn chn);
hi_s32 pciv_stop(hi_pciv_chn chn);
hi_s32 pciv_hide(hi_pciv_chn chn, hi_bool hide);
hi_s32 pciv_window_vb_create(const hi_pciv_win_vb_cfg *cfg);
hi_s32 pciv_window_vb_destroy(hi_void);
hi_s32 pciv_malloc(hi_u32 size, hi_u64 *phy_addr);
hi_s32 pciv_free(hi_u64 phy_addr);
hi_s32 pciv_malloc_chn_buffer(hi_pciv_chn chn, hi_u32 index, hi_u32 size, hi_u64 *phy_addr);
hi_s32 pciv_free_chn_buffer(hi_pciv_chn chn, const hi_u32 index);
hi_s32 pciv_user_dma_task(hi_pciv_dma_task *task);
hi_s32 pciv_free_share_buf(hi_pciv_chn chn, hi_u32 index, hi_u32 count);
hi_s32 pciv_free_all_buf(hi_pciv_chn chn);
hi_s32 pciv_src_pic_send(hi_pciv_chn chn, pciv_pic *src_pic);
hi_s32 pciv_src_pic_free(hi_pciv_chn chn, pciv_pic *src_pic);
hi_s32 pciv_receive_pic(hi_pciv_chn chn, pciv_pic *recv_pic);
hi_s32 pciv_recv_pic_free(hi_pciv_chn chn, pciv_pic *recv_pic);
hi_s32 pciv_init(void);
hi_void pciv_exit(void);
hi_s32 pciv_proc_show(osal_proc_entry_t *s);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

