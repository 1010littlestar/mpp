/*
* Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description: Common defination of video output
* Author: Hisilicon multimedia software group
* Create: 2016/11/09
*/
#ifndef __HI_COMM_VO_ADAPT_H__
#define __HI_COMM_VO_ADAPT_H__

#include "hi_type.h"
#include "hi_comm_video_adapt.h"
#include "hi_comm_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

typedef EN_VOU_ERR_CODE_E hi_vou_err_code;

typedef hi_u32 hi_vo_intf_type;

typedef VO_INTF_SYNC_E hi_vo_intf_sync;

typedef VO_ZOOM_IN_E hi_vo_zoom_in;

typedef VO_CSC_MATRIX_E hi_vo_csc_matrix;

typedef struct {
    hi_u32 priority; /* video out overlay pri sd */
    hi_rect rect; /* rectangle of video output channel */
    hi_bool deflicker; /* deflicker or not sd */
} hi_vo_chn_attr;

typedef struct {
    hi_aspect_ratio aspect_ratio; /* RW; aspect ratio */
} hi_vo_chn_param;

typedef struct {
    hi_bool border_en; /* RW; do frame or not */
    hi_border border; /* RW; frame's top, bottom, left, right width and color */
} hi_vo_border;

typedef struct {
    hi_u32 chn_buf_used; /* channel buffer that been occupied */
} hi_vo_query_status;

typedef struct {
    hi_bool synm; /* RW; sync mode(0:timing,as BT.656; 1:signal,as LCD) */
    hi_bool iop; /* RW; interlaced or progressive display(0:i; 1:p) */
    hi_u8 intfb; /* RW; interlace bit width while output */

    hi_u16 vact; /* RW; vertical active area */
    hi_u16 vbb; /* RW; vertical back blank porch */
    hi_u16 vfb; /* RW; vertical front blank porch */

    hi_u16 hact; /* RW; horizontal active area */
    hi_u16 hbb; /* RW; horizontal back blank porch */
    hi_u16 hfb; /* RW; horizontal front blank porch */
    hi_u16 hmid; /* RW; bottom horizontal active area */

    hi_u16 bvact; /* RW; bottom vertical active area */
    hi_u16 bvbb; /* RW; bottom vertical back blank porch */
    hi_u16 bvfb; /* RW; bottom vertical front blank porch */

    hi_u16 hpw; /* RW; horizontal pulse width */
    hi_u16 vpw; /* RW; vertical pulse width */

    hi_bool idv; /* RW; inverse data valid of output */
    hi_bool ihs; /* RW; inverse horizontal synch signal */
    hi_bool ivs; /* RW; inverse vertical synch signal */
} hi_vo_sync_info;

typedef struct {
    hi_u32 bg_color; /* RW; background color of a device, in RGB format. */
    hi_vo_intf_type intf_type; /* RW; type of a VO interface */
    hi_vo_intf_sync intf_sync; /* RW; type of a VO interface timing */
    hi_vo_sync_info sync_info; /* RW; information about VO interface timings */
} hi_vo_pub_attr;

typedef struct {
    hi_size target_size; /* RW; WBC zoom target size */
    hi_pixel_format pixel_format; /* RW; the pixel format of WBC output */
    hi_u32 frame_rate; /* RW; frame rate control */
    hi_dynamic_range dynamic_range; /* RW; write back dynamic range type */
    hi_compress_mode compress_mode; /* RW; write back compressing mode   */
} hi_vo_wbc_attr;

typedef VO_WBC_MODE_E hi_vo_wbc_mode;

typedef VO_WBC_SOURCE_TYPE_E hi_vo_wbc_source_type;

typedef struct {
    hi_vo_wbc_source_type source_type; /* RW; WBC source's type */
    hi_u32 source_id; /* RW; WBC source's ID */
} hi_vo_wbc_source;

typedef VO_PART_MODE_E hi_vo_part_mode;

typedef struct {
    hi_rect disp_rect; /* RW; display resolution */
    hi_size image_size; /* RW; canvas size of the video layer */
    hi_u32 disp_frm_rt; /* RW; display frame rate */
    hi_pixel_format pix_format; /* RW; pixel format of the video layer */
    hi_bool double_frame; /* RW; whether to double frames */
    hi_bool cluster_mode; /* RW; whether to take cluster way to use memory */
    hi_dynamic_range dst_dynamic_range; /* RW; video layer output dynamic range type */
} hi_vo_video_layer_attr;

typedef struct {
    hi_aspect_ratio aspect_ratio; /* RW; aspect ratio */
} hi_vo_layer_param;

typedef struct {
    hi_u32 x_ratio; /* RW; range: [0, 1000]; x_ratio = x * 1000 / W,
                              x means the start point to be zoomed, W means displaying channel's width. */
    hi_u32 y_ratio; /* RW; range: [0, 1000]; y_ratio = y * 1000 / H,
                              y means the start point to be zoomed, H means displaying channel's height. */
    hi_u32 w_ratio; /* RW; range: [0, 1000]; w_ratio = w * 1000 / W,
                              w means the width to be zoomed, W means displaying channel's width. */
    hi_u32 h_ratio; /* RW; range: [0, 1000]; h_ratio = h * 1000 / H,
                              h means the height to be zoomed, H means displaying channel's height. */
} hi_vo_zoom_ratio;

typedef struct {
    hi_vo_zoom_in zoom_type; /* choose the type of zoom in */
    union {
        hi_rect zoom_rect; /* zoom in by rect */
        hi_vo_zoom_ratio zoom_ratio; /* zoom in by ratio */
    };
} hi_vo_zoom_attr;

typedef struct {
    hi_vo_csc_matrix csc_matrix; /* CSC matrix */
    hi_u32 luma; /* RW; range:    [0, 100]; luminance, default: 50 */
    hi_u32 contrast; /* RW; range:    [0, 100]; contrast, default: 50 */
    hi_u32 hue; /* RW; range:    [0, 100]; hue, default: 50 */
    hi_u32 satuature; /* RW; range:    [0, 100]; satuature, default: 50 */
} hi_vo_csc;

typedef struct {
    hi_u32 region_num; /* count of the region */
    hi_rect *ATTRIBUTE region; /* region attribute */
} hi_vo_region_info;

typedef struct {
    hi_u32 width;
    hi_u32 color[2];
} hi_vo_layer_boundary;

typedef struct {
    hi_bool boundary_en; /* do frame or not */
    hi_u32 color_index; /* the index of frame color,[0,1] */
} hi_vo_chn_boundary;

typedef struct {
    hi_bool transparent_transmit; /* RW, range: [0, 1];  YC(luminance and chrominance) changes or not
                                            when passing through VO */
    hi_bool exit_dev;
    hi_bool wbc_bg_black_en;
    hi_bool dev_clk_ext_en;
    hi_bool save_buf_mode[VO_MAX_PHY_DEV_NUM]; /* save buff mode */
} hi_vo_mod_param;

typedef VO_CLK_SOURCE_E hi_vo_clk_source;

typedef struct {
    hi_u32 fbdiv;
    hi_u32 frac;
    hi_u32 refdiv;
    hi_u32 postdiv1;
    hi_u32 postdiv2;
} hi_vo_user_intfsync_pll;

typedef struct {
    hi_vo_clk_source clk_source;

    union {
        hi_vo_user_intfsync_pll user_sync_pll;
        hi_u32 lcd_m_clk_div;
    };
} hi_vo_user_intfsync_attr;

typedef struct {
    hi_vo_user_intfsync_attr user_intf_sync_attr;
    hi_u32 pre_div;
    hi_u32 dev_div;
    hi_bool clk_reverse;
} hi_vo_user_intfsync_info;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __HI_COMM_VO_ADAPT_H__ */


