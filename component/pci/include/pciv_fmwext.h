/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2008-2019. All rights reserved.
 * Description   : pciv firmware functions declaration
 * Author        : Hisilicon multimedia software group
 * Created       : 2008/06/16
 */
#ifndef __PCIV_FMWEXT_H__
#define __PCIV_FMWEXT_H__

#include "hi_comm_pciv_adapt.h"

#define PCIV_FMW_MAX_CHN_NUM PCIV_MAX_CHN_NUM

typedef struct {
    hi_u32              index;        /* index of pciv channnel buffer */
    hi_u32              count;        /* total number of picture */

    hi_u64              phy_addr;     /* physical address of video buffer which store pcicture info */
    hi_u32              pool_id;      /* pool id of video buffer which store picture info */

    hi_u64              pts;          /* time stamp */
    hi_u32              time_ref;     /* time reference */
    hi_pciv_bind_type   src_type;     /* bind type for pciv */
    hi_video_field      filed;        /* video field type */
    hi_bool             block;        /* the flag of block */
    hi_video_format     video_format; /* the style of video */
    hi_compress_mode    compress_mode; /* the mode of compress */
    hi_dynamic_range    dynamic_range;
    hi_color_gamut      color_gamut;
    hi_pixel_format     pixel_format;
    hi_mod_id           mod_id;
} pciv_pic;

typedef hi_s32 fn_pciv_src_send_pic(hi_pciv_chn pciv_chn, pciv_pic *src_pic);
typedef hi_s32 fn_pciv_recv_pic_free(hi_pciv_chn pciv_chn, pciv_pic *recv_pic);
typedef hi_s32 fn_pciv_get_chn_share_buf_state(hi_pciv_chn pciv_chn);

typedef struct {
    fn_pciv_src_send_pic              *pf_src_send_pic;
    fn_pciv_recv_pic_free             *pf_recv_pic_free;
    fn_pciv_get_chn_share_buf_state   *pf_query_pciv_chn_share_buf_state;
} pciv_fmw_callback;

#define PCIV_FMW_ERR_TRACE(fmt...)\
do { \
    HI_ERR_TRACE(HI_ID_PCIVFMW, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__);\
    HI_ERR_TRACE(HI_ID_PCIVFMW, ##fmt);\
}while (0)

#define PCIV_FMW_WARN_TRACE(fmt...)\
do { \
    HI_WARN_TRACE(HI_ID_PCIVFMW, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__);\
    HI_WARN_TRACE(HI_ID_PCIVFMW, ##fmt);\
}while (0)

#define PCIV_FMW_INFO_TRACE(fmt...)\
do { \
    HI_INFO_TRACE(HI_ID_PCIVFMW, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__);\
    HI_INFO_TRACE(HI_ID_PCIVFMW, ##fmt);\
}while (0)

#define PCIV_FMW_DEBUG_TRACE(fmt...)\
do { \
    HI_DEBUG_TRACE(HI_ID_PCIVFMW, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__);\
    HI_DEBUG_TRACE(HI_ID_PCIVFMW, ##fmt);\
}while (0)

#define PCIV_FMW_ALERT_TRACE(fmt...)\
do { \
    HI_ALERT_TRACE(HI_ID_PCIVFMW, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__);\
    HI_ALERT_TRACE(HI_ID_PCIVFMW, ##fmt);\
}while (0)

#define PCIVFMW_CHECK_CHNID(chn_id)\
do {\
    if(((chn_id) < 0) || ((chn_id) >= PCIV_FMW_MAX_CHN_NUM))\
    {\
        PCIV_FMW_ERR_TRACE("invalid chn id:%d \n", chn_id);\
        return HI_ERR_PCIV_INVALID_CHNID;\
    }\
}while (0)

#define PCIVFMW_CHECK_PTR(ptr)\
do {\
    if((ptr) == HI_NULL)\
    {\
        PCIV_FMW_ERR_TRACE("PTR is NULL!\n");\
        return HI_ERR_PCIV_NULL_PTR;\
    }\
}while (0)

hi_s32  pciv_firmware_create(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr, hi_s32 local_id);
hi_s32  pciv_firmware_destroy(hi_pciv_chn pciv_chn);
hi_s32  pciv_firmware_set_attr(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr, hi_s32 local_id);
hi_s32  pciv_firmware_start(hi_pciv_chn pciv_chn);
hi_s32  pciv_firmware_stop(hi_pciv_chn pciv_chn);
hi_s32  pciv_firmware_malloc(hi_u32 size, hi_s32 local_id, hi_u64 *phy_addr);
hi_s32  pciv_firmware_free(hi_s32 local_id, hi_u64 phy_addr);
hi_s32  pciv_firmware_malloc_chn_buffer(hi_pciv_chn pciv_chn, hi_u32 index,
    hi_u32 size, hi_s32 local_id, hi_u64 *phy_addr);
hi_s32  pciv_firmware_free_chn_buffer(hi_pciv_chn pciv_chn, hi_u32 index, hi_s32 local_id);
hi_s32  pciv_firmware_src_pic_free(hi_pciv_chn pciv_chn, pciv_pic *src_pic);
hi_s32  pciv_firmware_recv_pic_and_send(hi_pciv_chn pciv_chn, pciv_pic *recv_pic);
hi_s32  pciv_firmware_window_vb_create(hi_pciv_win_vb_cfg *cfg);
hi_s32  pciv_firmware_window_vb_destroy(hi_void);
hi_s32  pciv_firmware_register_func(pciv_fmw_callback *call_back);

/*------------------------------------------------------------------------------------------*/

typedef struct {
    hi_s32 (*pfn_pciv_send_pic)(hi_vi_dev vi_dev_id, hi_vi_chn vi_chn, const hi_video_frame_info *pic);
} pciv_fmw_export_func;

#endif

