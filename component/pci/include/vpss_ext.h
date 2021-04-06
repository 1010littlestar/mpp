/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: vpss extern  mkp header file
 * Author: Hisilicon multimedia software group
 * Create: 2016/09/27
 */

#include "hi_type.h"
#include "mod_ext.h"
#include "hi_comm_sys_adapt.h"
#include "hi_comm_vpss_adapt.h"
#include "hi_comm_vo_adapt.h"

#include "mkp_sys.h"

#ifndef __VPSS_EXT_H__
#define __VPSS_EXT_H__

typedef struct {
    hi_bool tunl_frame;
    hi_u64 tunl_phy_addr;
    hi_void *tunl_vir_addr;
    hi_void *frame_ok;
} vpss_low_delay_cfg;

typedef struct {
    hi_video_frame_info video_frame;
    hi_mod_id mod_id;
    hi_bool block_mode; /* flashed video frame or not. */
    hi_u64 start_time;
    hi_u64 node_index; /* node index */
    hi_u64 pts[VPSS_MAX_PHY_CHN_NUM];
} vpss_pic_info;

typedef struct {
    vpss_pic_info *src_pic_info; /* information of source pic */
    vpss_pic_info *old_pic_info; /* information of backup pic */
    hi_bool scale_cap; /* whether scaling */
    hi_bool trans_cap; /* whether the frame rate is doubled */
    hi_bool malloc_buffer; /* whether malloc frame buffer */
} vpss_query_info;

typedef struct {
    hi_bool new_frame; /* whether use new pic to query */
    hi_bool vpss_proc; /* whether vpss need to process */
    hi_bool double_frame; /* whether the frame rate is doubled */
    hi_bool update_backup; /* whether update backup pic */
    hi_compress_mode compress_mode; /* compress mode */
    vpss_pic_info dest_pic_info[2];
    hi_aspect_ratio aspect_ratio; /* aspect ratio configuration */
    hi_vo_part_mode part_mode;
} vpss_inst_info;

typedef struct {
    hi_bool suc; /* whether successful completion */
    hi_vpss_grp grp;
    hi_vpss_chn chn;
    vpss_pic_info *dest_pic_info[2]; /* pic processed by vpss.0:top field 1:bottom field */
    hi_u64 node_index;
} vpss_send_info;

typedef struct {
    hi_mod_id mod_id;
    hi_s32(*vpss_query)(hi_s32 dev_id, hi_s32 chn_id, vpss_query_info *query_info,
           vpss_inst_info *inst_info);
    hi_s32 (*vpss_send)(hi_s32 dev_id, hi_s32 chn_id, vpss_send_info *send_info);
    hi_s32 (*reset_call_back)(hi_s32 dev_id, hi_s32 chn_id, hi_void *pv_data);

} vpss_register_info;

typedef struct {
    hi_bool vpss_en[VPSS_IP_NUM];
} vpss_ip_enable;

typedef struct {
    hi_s32 (*pfn_vpss_register)(const vpss_register_info *info);
    hi_s32 (*pfn_vpss_un_register)(hi_mod_id mod_id);
    hi_void (*pfn_vpss_update_vi_vpss_mode)(const hi_vi_vpss_mode *vi_vpss_mode);
    hi_s32(*pfn_vpss_vi_submit_task)(hi_vpss_grp grp, const hi_video_frame_info *vi_frame, hi_bool lost_frame,
           hi_pixel_format pixel_format);
    hi_s32 (*pfn_vpss_vi_start_task)(hi_u32 vpss_id);
    hi_s32 (*pfn_vpss_vi_task_done)(hi_s32 vpss_id, hi_s32 state);
    hi_s32 (*pfn_vpss_get_chn_video_format)(hi_vpss_grp grp, hi_vpss_chn chn, hi_video_format *video_format);
    hi_s32 (*pfn_vpss_get_ip_en)(vpss_ip_enable *vpss_ip_en);
    hi_s32 (*pfn_vpss_down_semaphore)(hi_vpss_grp grp);
    hi_s32 (*pfn_vpss_up_semaphore)(hi_vpss_grp grp);
    hi_s32 (*pfn_vpss_vi_early_irq_proc)(hi_vpss_grp grp);
    hi_bool (*pfn_vpss_is_grp_existed)(hi_vpss_grp grp);
} vpss_export_func;

/* globle variable declare */
extern hi_u32 g_vpss_state;

#define CKFN_VPSS_ENTRY() CHECK_FUNC_ENTRY(HI_ID_VPSS)

#define ckfn_vpss_register() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_register != NULL)
#define call_vpss_register(info) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_register(info)

#define ckfn_vpss_un_register() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_un_register != NULL)
#define call_vpss_un_register(mod_id) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_un_register(mod_id)

#define ckfn_vpss_update_vi_vpss_mode() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_update_vi_vpss_mode != NULL)
#define call_vpss_update_vi_vpss_mode(vi_vpss_mode) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_update_vi_vpss_mode(vi_vpss_mode)

#define ckfn_vpss_vi_submit_task() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_vi_submit_task != NULL)
#define call_vpss_vi_submit_task(grp, vi_frame, lost_frame, pixel_format) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_vi_submit_task(grp, vi_frame, lost_frame, pixel_format)

#define ckfn_vpss_vi_start_task() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_vi_start_task != NULL)
#define call_vpss_vi_start_task(vpss_id) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_vi_start_task(vpss_id)

#define ckfn_vpss_vi_task_done() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_vi_task_done != NULL)
#define call_vpss_vi_task_done(vpss_id, state) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_vi_task_done(vpss_id, state)

#define ckfn_vpss_get_chn_video_format() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_get_chn_video_format != NULL)
#define call_vpss_get_chn_video_format(grp, chn, video_format) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_get_chn_video_format(grp, chn, video_format)

#define ckfn_vpss_get_ip_en() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_get_ip_en != NULL)
#define call_vpss_get_ip_en(vpss_ip_en) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_get_ip_en(vpss_ip_en)

#define ckfn_vpss_down_semaphore() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_down_semaphore != NULL)
#define call_vpss_down_semaphore(grp) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_down_semaphore(grp)

#define ckfn_vpss_up_semaphore() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_up_semaphore != NULL)
#define call_vpss_up_semaphore(grp) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_up_semaphore(grp)

#define ckfn_vpss_vi_early_irq_proc() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_vi_early_irq_proc != NULL)
#define call_vpss_vi_early_irq_proc(grp) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_vi_early_irq_proc(grp)

#define ckfn_vpss_is_grp_existed() \
    (FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_is_grp_existed != NULL)
#define call_vpss_is_grp_existed(grp) \
    FUNC_ENTRY(vpss_export_func, HI_ID_VPSS)->pfn_vpss_is_grp_existed(grp)

#endif /* __VPSS_EXT_H__ */








