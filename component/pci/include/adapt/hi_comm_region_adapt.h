/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2010-2019. All rights reserved.
 * Description: hi_comm_region.h
 * Author:
 * Create: 2010-12-13
 */
#ifndef __HI_COMM_REGION_ADAPT_H__
#define __HI_COMM_REGION_ADAPT_H__

#include "hi_common_adapt.h"
#include "hi_comm_video_adapt.h"
#include "hi_errno_adapt.h"
#include "hi_defines.h"
#include "hi_comm_region.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef RGN_HANDLE hi_rgn_handle;
typedef RGN_HANDLEGROUP hi_rgn_handle_group;

/* type of video regions */
typedef RGN_TYPE_E hi_rgn_type;

typedef INVERT_COLOR_MODE_E hi_invert_color_mode;

typedef struct {
    hi_bool abs_qp;
    hi_s32 qp;
    hi_bool qp_disable;
} hi_overlay_qp_info;

typedef struct {
    hi_size inv_col_area;  // it must be multipe of 16 but not more than 64.
    hi_u32 lum_thresh;  // the threshold to decide whether invert the OSD's color or not.
    hi_invert_color_mode chg_mod;
    hi_bool inv_col_en;  // the switch of inverting color.
} hi_overlay_invert_color;

typedef ATTACH_DEST_E hi_attach_dest;

typedef struct {
    /* bitmap pixel format,now only support ARGB1555 or ARGB4444 */
    hi_pixel_format pixel_format;

    /* background color, pixel format depends on "pixel_format" */
    hi_u32 bg_color;

    /* region size,W:[2,RGN_OVERLAY_MAX_WIDTH],align:2,H:[2,RGN_OVERLAY_MAX_HEIGHT],align:2 */
    hi_size size;
    hi_u32 canvas_num;
} hi_overlay_attr;

typedef struct {
    /* X:[0,OVERLAY_MAX_X_VENC],align:2,Y:[0,OVERLAY_MAX_Y_VENC],align:2 */
    hi_point point;

    /* background an foreground transparence when pixel format is ARGB1555
      * the pixel format is ARGB1555,when the alpha bit is 1 this alpha is value!
      * range:[0,128]
      */
    hi_u32 fg_alpha;

    /* background an foreground transparence when pixel format is ARGB1555
      * the pixel format is ARGB1555,when the alpha bit is 0 this alpha is value!
      * range:[0,128]
      */
    hi_u32 bg_alpha;

    hi_u32 layer; /* OVERLAY region layer range:[0,7] */

    hi_overlay_qp_info qp_info;

    hi_overlay_invert_color invert_color;

    hi_attach_dest venc_chn;

    hi_u16 color_lut[RGN_COLOR_LUT_NUM];
} hi_overlay_chn_attr;

typedef RGN_AREA_TYPE_E hi_rgn_area_type;

typedef RGN_COORDINATE_E hi_rgn_coordinate;

typedef struct {
    hi_bool solid;     /* whether solid or dashed quadrangle */
    hi_u32 thick;    /* line width of quadrangle, valid when dashed quadrangle */
    hi_point point[4]; /* points of quadrilateral */
} hi_rgn_quadrangle;

typedef struct {
    hi_rgn_area_type cover_type; /* rect or arbitary quadrilateral COVER */
    union {
        hi_rect rect;                 /* config of rect */
        hi_rgn_quadrangle quad_rangle; /* config of arbitary quadrilateral COVER */
    };
    hi_u32 bg_color;
    hi_u32 layer;               /* COVER region layer */
    hi_rgn_coordinate coordinate; /* ratio coordiante or abs coordinate */
} hi_cover_chn_attr;

typedef struct {
    hi_rgn_area_type cover_type; /* rect or arbitary quadrilateral COVER */
    union {
        hi_rect rect;                 /* config of rect */
        hi_rgn_quadrangle quad_rangle; /* config of arbitary quadrilateral COVER */
    };
    hi_u32 bg_color;
    hi_u32 layer; /* COVEREX region layer range */
} hi_coverex_chn_attr;

typedef MOSAIC_BLK_SIZE_E hi_mosaic_blk_size;

typedef struct {
    hi_rect rect;               /* location of MOSAIC */
    hi_mosaic_blk_size mosaic_blk_size; /* block size of MOSAIC */
    hi_u32 layer;             /* MOSAIC region layer range:[0,3] */
} hi_mosaic_chn_attr;

typedef struct {
    hi_pixel_format pixel_format;

    /* background color, pixel format depends on "pixel_format" */
    hi_u32 bg_color;

    /* region size,W:[2,RGN_OVERLAY_MAX_WIDTH],align:2,H:[2,RGN_OVERLAY_MAX_HEIGHT],align:2 */
    hi_size size;
    hi_u32 canvas_num;
} hi_overlayex_attr;

typedef struct {
    /* X:[0,RGN_OVERLAY_MAX_X],align:2,Y:[0,RGN_OVERLAY_MAX_Y],align:2 */
    hi_point point;

    /* background an foreground transparence when pixel format is ARGB1555
      * the pixel format is ARGB1555,when the alpha bit is 1 this alpha is value!
      * range:[0,255]
      */
    hi_u32 fg_alpha;

    /* background an foreground transparence when pixel format is ARGB1555
      * the pixel format is ARGB1555,when the alpha bit is 0 this alpha is value!
      * range:[0,255]
      */
    hi_u32 bg_alpha;

    hi_u32 layer; /* OVERLAYEX region layer range:[0,15] */

    hi_u16 color_lut[RGN_COLOR_LUT_NUM];
} hi_overlayex_chn_attr;

typedef union {
    hi_overlay_attr overlay;     /* attribute of overlay region */
    hi_overlayex_attr overlayex; /* attribute of overlayex region */
} hi_rgn_type_attr;

typedef union {
    hi_overlay_chn_attr overlay_chn;     /* attribute of overlay region */
    hi_cover_chn_attr cover_chn;         /* attribute of cover region */
    hi_coverex_chn_attr coverex_chn;     /* attribute of coverex region */
    hi_overlayex_chn_attr overlayex_chn; /* attribute of overlayex region */
    hi_mosaic_chn_attr mosaic_chn;       /* attribute of mosic region */
} hi_rgn_type_chn_attr;

/* attribute of a region */
typedef struct {
    hi_rgn_type type; /* region type */
    hi_rgn_type_attr attr; /* region attribute */
} hi_rgn_attr;

/* attribute of a region */
typedef struct {
    hi_bool is_show;
    hi_rgn_type type;        /* region type */
    hi_rgn_type_chn_attr attr; /* region attribute */
} hi_rgn_chn_attr;

typedef struct {
    hi_point point;
    hi_bitmap bmp;
    hi_u32 stride;
} hi_rgn_bmp_update;

typedef struct {
    hi_u32 bmp_cnt;
    hi_rgn_bmp_update bmp_update[RGN_MAX_BMP_UPDATE_NUM];
} hi_rgn_bmp_update_cfg;

typedef struct {
    hi_u64 phy_addr;
    hi_u64 virt_addr;
    hi_size size;
    hi_u32 stride;
    hi_pixel_format pixel_fmt;
} hi_rgn_canvas_info;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __HI_COMM_REGION_H__ */
