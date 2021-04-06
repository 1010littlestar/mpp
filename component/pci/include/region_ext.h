/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2010-2019. All rights reserved.
 * Description: region_ext.h
 * Author:
 * Create: 2010-12-18
 */
#ifndef __REGION_EXT_H__
#define __REGION_EXT_H__

#include "hi_common_adapt.h"
#include "hi_comm_video_adapt.h"
#include "hi_comm_region_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef hi_void (*rgn_detch_chn)(hi_s32 dev_id, hi_s32 chn_id, hi_bool detch_chn);

/* the max pixel format of region support */
#define PIXEL_FORMAT_NUM_MAX    4
#define RGN_HANDLE_IN_BATCH_MAX 24

typedef hi_bool (*rgn_check_chn_capacity)(hi_rgn_type type, const hi_mpp_chn *chn);

typedef hi_bool (*rgn_check_chn_attr)(hi_rgn_type type, const hi_mpp_chn *chn, const hi_rgn_chn_attr *rgn_chn);

typedef struct {
    hi_bool is_show;
    hi_rgn_area_type cover_type;   /* COVER type */
    hi_point point;               /* start position */
    hi_size size;                 /* region size(width and height) */
    hi_rgn_quadrangle quad_rangle; /* config of  quadrangle */
    hi_u32 layer;               /* layer of region */
    hi_u32 bg_color;             /* background color of region */
    hi_u32 global_alpha;         /* overall perspective ALPHA */
    hi_u32 fg_alpha;             /* foreground ALPHA */
    hi_u32 bg_alpha;             /* background ALPHA */
    hi_u64 phy_addr;             /* physical address of region memory */
    hi_u64 virt_addr;            /* virtual address of region memory */
    hi_u32 stride;              /* stride address of region memory data */
    hi_u32 canvas_num;              /* buffer number of region used */
    hi_pixel_format pixel_format;     /* pixel formatof region */
    hi_video_field field;   /* field of region attach */
    hi_overlay_qp_info qp_info; /* QP information */
    hi_overlay_invert_color inv_col_info; /* inverse color information */
    hi_attach_dest venc_chn; /* OSD attach JPEG information */
    hi_mosaic_blk_size mosaic_blk_size; /* MOSAIC block size */
    hi_rgn_coordinate coordinate; /* COVER coordinate */
    hi_u16 color_lut[RGN_COLOR_LUT_NUM];
} rgn_comm;

typedef struct {
    hi_u32 buff_index; /* index of buffer */
    hi_u32 handle;      /* handle of buffer */
} rgn_buf_info;

typedef struct {
    rgn_buf_info buf_info;
    rgn_comm *comm; /* address of common information of  point array */
} rgn_get_info;

typedef struct {
    hi_u32 num;                  /* number of region */
    hi_bool modify;                /* modify of not */
    rgn_get_info *get_info; /* address of common information of  point array */
} rgn_info;

typedef struct {
    hi_u32 num; /* number of region */
    rgn_buf_info *buf_info;
} rgn_put_info;

typedef struct {
    hi_mod_id mod_id;
    hi_u32 max_dev_cnt; /* if no dev id, should set it 1 */
    hi_u32 max_chn_cnt;
    rgn_detch_chn pfn_rgn_detch_chn;
} rgn_register_info;

typedef struct {
    hi_bool is_comm;
    hi_bool spt_reset;

    /* region layer [min,max] */
    hi_u32 layer_min;
    hi_u32 layer_max;
} rgn_capacity_layer;

typedef struct {
    hi_bool is_comm;
    hi_bool spt_reset;

    hi_point point_min;
    hi_point point_max;

    /* X (start position) of pixel align number */
    hi_u32 start_x_align;

    /* Y (start position) of pixel align number */
    hi_u32 start_y_align;

} rgn_capacity_point;

typedef struct {
    hi_bool is_comm;
    hi_bool spt_reset;

    hi_size size_min;
    hi_size size_max;

    /* region width of pixel align number */
    hi_u32 width_align;

    /* region height of pixel align number */
    hi_u32 height_align;

    /* maximum area of region */
    hi_u32 max_area;
} rgn_capacity_size;

typedef struct {
    hi_bool is_comm;
    hi_bool spt_reset;

    hi_u32 min_thick;
    hi_u32 max_thick;
    hi_point point_min;
    hi_point point_max;
    /* X (start position) of pixel align number */
    hi_u32 start_x_align;

    /* Y (start position) of pixel align number */
    hi_u32 start_y_align;
} rgn_capacity_quadrangle;

typedef struct {
    hi_bool is_comm;
    hi_bool spt_reset;

    /* pixel format number of region */
    hi_u32 pixl_fmt_num;
    /* all pixel format type of region supported----------related item check :8.1,
     * check channel all pixel pormat is same or not.
     */
    hi_pixel_format pixel_format[PIXEL_FORMAT_NUM_MAX];
} rgn_capacity_pixlfmt;

typedef struct {
    hi_bool is_comm;
    hi_bool spt_reset;

    hi_u32 alpha_max;
    hi_u32 alpha_min;
} rgn_capacity_alpha;

typedef struct {
    hi_bool is_comm;
    hi_bool spt_reset;
} rgn_capacity_bgclr;

typedef enum {
    RGN_SORT_BY_LAYER = 0,
    RGN_SORT_BY_POSITION,
    RGN_SRT_BUTT
} rgn_sort_key;

typedef struct {
    /* sort or not */
    hi_bool is_sort;

    /* key word used in sort */
    rgn_sort_key key;

    /* the sort way: 1/true: small to larger;2/true: larger to small; */
    hi_bool small_to_big;
} rgn_capacity_sort;

typedef struct {
    hi_bool is_comm;
    hi_bool spt_reset;

    hi_bool spt_qp_disable;

    hi_s32 qp_abs_min;
    hi_s32 qp_abs_max;

    hi_s32 qp_rel_min;
    hi_s32 qp_rel_max;

} rgn_capacity_qp;

typedef struct {
    hi_bool is_comm;
    hi_bool spt_reset;

    rgn_capacity_size size_cap;

    hi_u32 lum_min;
    hi_u32 lum_max;

    hi_invert_color_mode inv_mod_min;
    hi_invert_color_mode inv_mod_max;

    /* if inversing function used,X (start position) of pixel align number */
    hi_u32 start_x_align;
    /* if inversing function used,Y (start position) of pixel align number */
    hi_u32 start_y_align;
    /* if inversing function used,width of pixel align number */
    hi_u32 width_align;
    /* if inversing function used,height of pixel align number */
    hi_u32 height_align;

} rgn_capacity_invcolor;

typedef enum {
    RGN_CAPACITY_QUADRANGLE_UNSUPORT = 0x0,
    RGN_CAPACITY_QUADRANGLE_TYPE_SOLID = 0x1,
    RGN_CAPACITY_QUADRANGLE_TYPE_UNSOLID = 0x2,
    RGN_CAPACITY_QUADRANGLE_TYPE_BUTT = 0x4,
} rgn_capacity_quadrangle_type;

typedef struct {
    hi_bool spt_rect;
    rgn_capacity_quadrangle_type quad_rangle_type;
} rgn_capacity_cover_attr;

typedef struct {
    hi_bool is_comm;
    hi_bool spt_reset;

    hi_u32 msc_blk_sz_max;
    hi_u32 msc_blk_sz_min;
} rgn_capacity_mscblksz;

typedef struct {
    /* layer of region */
    rgn_capacity_layer layer;

    /* start position */
    rgn_capacity_point point;

    /* region size(width and height) */
    rgn_capacity_size size;

    /* attribute of quadrangle */
    rgn_capacity_quadrangle quad_rangle;

    /* pixel format */
    hi_bool spt_pixl_fmt;
    rgn_capacity_pixlfmt pixl_fmt;

    /* alpha */
    /* foreground alpha */
    hi_bool spt_fg_alpha;
    rgn_capacity_alpha fg_alpha;

    /* background alpha */
    hi_bool spt_bg_alpha;
    rgn_capacity_alpha bg_alpha;

    /* overall perspective alpha */
    hi_bool spt_global_alpha;
    rgn_capacity_alpha global_alpha;

    /* background color of region */
    hi_bool spt_bg_clr;
    rgn_capacity_bgclr bg_clr;

    /* sort */
    hi_bool spt_sort;
    rgn_capacity_sort chn_sort;

    /* capability of QP */
    hi_bool spt_qp;
    rgn_capacity_qp qp;

    rgn_capacity_cover_attr cover_attr; /* cover attribute */

    /* msc attribute */
    hi_bool spt_msc;
    rgn_capacity_mscblksz msc_blk_sz;

    /* support bitmap or not */
    hi_bool spt_bitmap;

    /* support overlap or not */
    hi_bool spt_overlap;

    /* stride align */
    hi_u32 stride;

    /* the maximumegion of channel */
    hi_u32 rgn_num_in_chn;

    rgn_check_chn_capacity pfn_check_chn_capacity; /* check whether channel support a region type */

    rgn_check_chn_attr pfn_check_attr; /* check whether attribute is legal */

} rgn_capacity;

typedef struct {
    hi_s32 (*pfn_rgn_register_mod)(hi_rgn_type type, const rgn_capacity *capacity, const rgn_register_info *register_info);
    hi_s32 (*pfn_rgn_unregister_mod)(hi_rgn_type type, hi_mod_id mod_id);
    hi_s32 (*pfn_rgn_get_region)(hi_rgn_type type, const hi_mpp_chn *chn, rgn_info *info);
    hi_s32 (*pfn_rgn_put_region)(hi_rgn_type type, const hi_mpp_chn *chn, rgn_put_info *info);
    hi_s32 (*pfn_rgn_set_modify_false)(hi_rgn_type type, const hi_mpp_chn *chn);
} rgn_export_func;

#define ckfn_rgn() \
    (FUNC_ENTRY(rgn_export_func, HI_ID_RGN) != NULL)

#define ckfn_rgn_register_mod() \
    (FUNC_ENTRY(rgn_export_func, HI_ID_RGN)->pfn_rgn_register_mod != NULL)

#define call_rgn_register_mod(type, capacity, register_info) \
    FUNC_ENTRY(rgn_export_func, HI_ID_RGN)->pfn_rgn_register_mod(type, capacity, register_info)

#define ckfn_rgn_unregister_mod() \
    (FUNC_ENTRY(rgn_export_func, HI_ID_RGN)->pfn_rgn_unregister_mod != NULL)

#define call_rgn_unregister_mod(type, mod_id) \
    FUNC_ENTRY(rgn_export_func, HI_ID_RGN)->pfn_rgn_unregister_mod(type, mod_id)

#define ckfn_rgn_get_region() \
    (FUNC_ENTRY(rgn_export_func, HI_ID_RGN)->pfn_rgn_get_region != NULL)

#define call_rgn_get_region(type, chn, info) \
    FUNC_ENTRY(rgn_export_func, HI_ID_RGN)->pfn_rgn_get_region(type, chn, info)

#define ckfn_rgn_put_region() \
    (FUNC_ENTRY(rgn_export_func, HI_ID_RGN)->pfn_rgn_put_region != NULL)

#define call_rgn_put_region(type, chn, info) \
    FUNC_ENTRY(rgn_export_func, HI_ID_RGN)->pfn_rgn_put_region(type, chn, info)

#define ckfn_rgn_set_modify_false() \
    (FUNC_ENTRY(rgn_export_func, HI_ID_RGN)->pfn_rgn_set_modify_false != NULL)

#define call_rgn_set_modify_false(type, chn) \
    FUNC_ENTRY(rgn_export_func, HI_ID_RGN)->pfn_rgn_set_modify_false(type, chn)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __REGION_EXT_H__ */
