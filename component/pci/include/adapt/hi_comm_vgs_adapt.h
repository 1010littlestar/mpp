/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2019. All rights reserved.
 * Description: Common struct definition for VGS
 * Author: Hisilicon multimedia software group
 * Create: 2019-05-05
 */

#ifndef __HI_COMM_VGS_ADAPT_H__
#define __HI_COMM_VGS_ADAPT_H__

#include "hi_type.h"
#include "hi_comm_video_adapt.h"
#include "hi_comm_vgs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef VGS_HANDLE hi_vgs_handle;
typedef VGS_COLOR_REVERT_MODE_E hi_vgs_color_revert_mode;
typedef VGS_COVER_TYPE_E hi_vgs_cover_type;
typedef VGS_SCLCOEF_MODE_E hi_vgs_sclcoef_mode;

typedef struct {
    hi_rect src_rect;                           /* OSD color revert area */
    hi_vgs_color_revert_mode color_revert_mode; /* OSD color revert mode */
} hi_vgs_osd_revert;

typedef struct {
    hi_video_frame_info img_in;  /* input picture */
    hi_video_frame_info img_out; /* output picture */
    hi_u64 private_data[4];      /* RW; private data of task */
    hi_u32 reserved;             /* RW; debug information,state of current picture */
} hi_vgs_task_attr;

typedef struct {
    hi_point start_point; /* line start point */
    hi_point end_point;   /* line end point */
    hi_u32 thick;         /* RW; width of line */
    hi_u32 color;         /* RW; range: [0,0xFFFFFF]; color of line */
} hi_vgs_draw_line;

typedef struct {
    hi_bool solid;     /* solid or hollow cover */
    hi_u32 thick;      /* RW; range: [2,8]; thick of the hollow quadrangle */
    hi_point point[4]; /* four points of the quadrangle */
} hi_vgs_quadrangle_cover;

typedef struct {
    hi_vgs_cover_type cover_type; /* cover type */
    union {
        hi_rect dst_rect;                    /* the rectangle area */
        hi_vgs_quadrangle_cover quad_rangle; /* the quadrangle area */
    };
    hi_u32 color; /* RW; range: [0,0xFFFFFF]; color of cover */
} hi_vgs_add_cover;

typedef struct {
    hi_rect rect;                 /* osd area */
    hi_u32 bg_color;              /* RW; background color of osd, depends on pixel format of osd,
                                   ARGB8888:[0,0xFFFFFFFF], ARGB4444:[0,0xFFFF], ARGB1555:[0,0x1FFF] */
    hi_pixel_format pixel_fmt;    /* pixel format of osd */
    hi_u64 phy_addr;              /* RW; physical address of osd */
    hi_u32 stride;                /* RW; range: [32,8192]; stride of osd */
    hi_u32 bg_alpha;              /* RW; range: [0,255]; background alpha of osd */
    hi_u32 fg_alpha;              /* RW; range: [0,255]; foreground alpha of osd */
    hi_bool revert_en;            /* RW; enable osd color revert */
    hi_vgs_osd_revert osd_revert; /* osd color revert information */
    hi_u16 color_lut[2];
} hi_vgs_add_osd;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_COMM_VGS_ADAPT_H__ */
