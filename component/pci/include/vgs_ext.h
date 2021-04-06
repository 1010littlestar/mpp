/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: vgs_ext.h
 * Author: Hisilicon multimedia software group
 * Create: 2013-07-24
 */

#ifndef __VGS_EXT_H__
#define __VGS_EXT_H__

#include "hi_comm_video_adapt.h"
#include "hi_comm_vgs_adapt.h"
#include "hi_errno.h"
#include "vb_ext.h"

#define VGS_INVALD_HANDLE (-1UL) /* invalid job handle */

#define VGS_MAX_JOB_NUM 400 /* max job number */
#define VGS_MIN_JOB_NUM 20  /* min job number */

#define VGS_MAX_TASK_NUM 800 /* max task number */
#define VGS_MIN_TASK_NUM 20  /* min task number */

#define VGS_MAX_NODE_NUM 800 /* max node number */
#define VGS_MIN_NODE_NUM 20  /* min node number */

#define VGS_MAX_WEIGHT_THRESHOLD 100 /* max weight threshold */
#define VGS_MIN_WEIGHT_THRESHOLD 1   /* min weight threshold */

#define MAX_VGS_COVER VGS_MAX_COVER_NUM
#define MAX_VGS_OSD   VGS_MAX_OSD_NUM

#define FD_GRID_SZ  4 /* grid size used in feature detection, in this version, the image is processed with 4x4 grid */
#define FD_CELL_NUM (FD_GRID_SZ * FD_GRID_SZ) /* blk number in total */

#define VGS_MAX_STITCH_NUM 4

/* The type of job */
typedef enum {
    VGS_JOB_TYPE_BYPASS = 0, /* BYPASS job,can only contain bypass task */
    VGS_JOB_TYPE_NORMAL = 1, /* normal job,can contain any task except bypass task and lumastat task */
    VGS_JOB_TYPE_BUTT
} vgs_job_type;

/* The completion status of task */
typedef enum {
    VGS_TASK_FNSH_STAT_OK = 1,       /* task has been completed correctly */
    VGS_TASK_FNSH_STAT_FAIL = 2,     /* task failed because of exception or not completed */
    VGS_TASK_FNSH_STAT_CANCELED = 3, /* task has been cancelled */
    VGS_TASK_FNSH_STAT_BUTT
} vgs_task_fnsh_stat;

/* the priority of VGS task */
typedef enum {
    VGS_JOB_PRI_HIGH = 0,   /* high priority */
    VGS_JOB_PRI_NORMAL = 1, /* normal priority */
    VGS_JOB_PRI_LOW = 2,    /* low priority */
    VGS_JOB_PRI_BUTT
} vgs_job_pri;

/* The status after VGS cancle job */
typedef struct {
    hi_u32 jobs_canceled; /* The count of cancelled job */
    hi_u32 jobs_left;     /* The count of the rest job */
} vgs_cancel_stat;

/* The completion status of job */
typedef enum {
    VGS_JOB_FNSH_STAT_OK = 1,       /* job has been completed correctly */
    VGS_JOB_FNSH_STAT_FAIL = 2,     /* job failed because of exception or not completed */
    VGS_JOB_FNSH_STAT_CANCELED = 3, /* job has been cancelled */
    VGS_JOB_FNSH_STAT_BUTT
} vgs_job_fnsh_stat;

/* The struct of vgs job.
After complete the job,VGS call the callback function to notify the caller with this struct as an parameter.
*/
typedef struct hi_vgs_job_data {
    hi_u64 private_data[VGS_PRIVATE_DATA_LEN]; /* private data of job */
    vgs_job_fnsh_stat job_finish_stat;         /* output param:job finish status(ok or nok) */
    vgs_job_type job_type;
    void (*job_call_back)(hi_mod_id call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id,
                          struct hi_vgs_job_data *job_data); /* callback */
} vgs_job_data;

/* The struct of vgs task ,include input and output frame buffer,caller,and callback function .
After complete the task,VGS call the callback function to notify the caller with this struct as an parameter.
*/
typedef struct hi_vgs_task_data {
    hi_video_frame_info img_in;  /* input picture */
    hi_video_frame_info img_out; /* output picture */
    hi_u64 private_data[VGS_PRIVATE_DATA_LEN]; /* task's private data */
    void (*call_back)(hi_mod_id call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id,
                      const struct hi_vgs_task_data *task); /* callback */
    hi_mod_id call_mod_id;          /* module ID */
    hi_u32 call_dev_id;             /* device ID */
    hi_u32 call_chn_id;             /* chnnel ID */
    vgs_task_fnsh_stat finish_stat; /* output param:task finish status(ok or nok) */
    hi_u32 reserved;
} vgs_task_data;

typedef struct hi_vgs_task_data_stitch {
    hi_video_frame_info img_in[VGS_MAX_STITCH_NUM]; /* input picture */
    hi_video_frame_info img_out;                    /* output picture */
    hi_u64 private_data[VGS_PRIVATE_DATA_LEN];      /* task's private data */
    void (*call_back)(hi_mod_id call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id,
                      const struct hi_vgs_task_data_stitch *task); /* callback */
    hi_mod_id call_mod_id;                            /* module ID */
    hi_u32 call_dev_id;                               /* device ID */
    hi_u32 call_chn_id;                               /* chnnel ID */
    vgs_task_fnsh_stat finish_stat; /* output param:task finish status(ok or nok) */
    hi_u32 reserved;
} vgs_task_data_stitch;

typedef enum {
    GHDR_SDR_IN_HDR10_OUT = 0,
    GHDR_SDR_IN_HLG_OUT,
    GHDR_SDR_PREMULT,
    GHDR_SDR709_IN_2020_OUT,    // reserved!
    GHDR_BUTT
} vgs_ghdr_scene_mode;

typedef enum {
    VGS_HIHDR_G_TYP = 0,
    VGS_HIHDR_G_TYP1,
    VGS_HIHDR_G_RAND,
    VGS_HIHDR_G_MAX,
    VGS_HIHDR_G_MIN,
    VGS_HIHDR_G_ZERO,
    VGS_HIHDR_G_BUTT
} vgs_ghdr_g_mode;

/* The information of OSD */
typedef struct {
    /* first address of bitmap */
    hi_u64 phy_addr;

    hi_pixel_format pixel_format;

    hi_u32 stride;

    /* Alpha bit should be extended by setting alpha0 and Alpha1, when pixel_format is PIXEL_FORMAT_RGB_1555 */
    hi_bool alpha_ext1555; /* whether alpha bit should be extended */
    hi_u8 alpha0;          /* alpha0 will be valid where alpha bit is 0 */
    hi_u8 alpha1;          /* alpha1 will be valid where alpha bit is 1 */
} vgs_osd;

/* The struct of OSD operation */
typedef struct {
    hi_bool osd_en;

    hi_u8 global_alpha;
    vgs_osd osd;
    hi_rect osd_rect;

    hi_bool revert_en;
    hi_vgs_osd_revert osd_revert;

    hi_bool ghdr_en;
} vgs_osd_opt;

typedef struct {
    hi_rect rect; /* the regions to get lumainfo */
    hi_u64 *virt_addr_for_result;
    hi_u64 phys_addr_for_result;
    hi_u64 *virt_addr_for_user;
} vgs_lumastat_opt;

typedef struct {
    hi_bool cover_en;
    hi_vgs_cover_type cover_type;
    union {
        hi_rect dst_rect;                    /* rectangle */
        hi_vgs_quadrangle_cover quad_rangle; /* arbitrary quadrilateral */
    };
    hi_u32 cover_color;
} vgs_cover_opt;

typedef struct {
    hi_rect dest_rect;
} vgs_crop_opt;

typedef enum {
    VGS_SCALE_COEF_NORMAL = 0,  /* normal scale coefficient */
    VGS_SCALE_COEF_TAP2 = 1,    /* scale coefficient of 2 tap for IVE */
    VGS_SCALE_COEF_TAP4 = 2,    /* scale coefficient of 4 tap for IVE */
    VGS_SCALE_COEF_TAP6 = 3,    /* scale coefficient of 6 tap for IVE */
    VGS_SCALE_COEF_TAP8 = 4,    /* scale coefficient of 8 tap for IVE */
    VGS_SCALE_COEF_PYRAMID = 5, /* scale coefficient for DIS pyramid */
    VGS_SCALE_COEF_BUTT
} vgs_scale_coef_mode;

typedef struct {
    hi_u32 proc_bit_depth;  // 4bits; u; 4.0
    hi_bool do_detection;    // 1bit; bool

    hi_u16 grid_rows;  // 3bits; u; 3.0
    hi_u16 grid_cols;  // 3bits; u; 3.0
    hi_u16 grid_nums;  // 5bits; u,; 5.0

    hi_u8 quality_level;   // 3bits; u; 3.0
    hi_u16 min_eigen_val;  // 16bits; u; 16.0
    hi_u8 harris_k;        // 4btis; u; 4.0
    hi_u16 min_distance;  // 5bits; u; 5.0

    hi_u16 cell_row[FD_GRID_SZ + 1];  // 9bits; u; 9.0 corner postion of the cells
    hi_u16 cell_col[FD_GRID_SZ + 1];  // 9bits; u; 9.0

    hi_u16 pts_num_per_cell[FD_CELL_NUM];  // 6bits; u; 6.0 maximum number of keypoints allowed in each block
    hi_u16 max_pts_num_in_use;               // 9bits; u; 9.0
    hi_bool set_pts_num_per_cell;              // 1bit; bool
} vgs_fpd_hw_cfg;

typedef struct {
    hi_rect video_rect;
    hi_u32 bg_color;
} vgs_aspectratio_opt;

typedef struct {
    hi_bool crop; /* if enable crop */
    vgs_crop_opt crop_opt;
    hi_bool cover; /* if enable cover */
    vgs_cover_opt cover_opt[MAX_VGS_COVER];
    hi_bool osd; /* if enable osd */
    vgs_osd_opt osd_opt[MAX_VGS_OSD];

    hi_bool mirror;     /* if enable mirror */
    hi_bool flip;       /* if enable flip */
    hi_bool force_h_filt; /* whether to force the horizontal direction filtering, it can be
                                                set while input and output pic are same size at horizontal direction */
    hi_bool force_v_filt; /* whether to force the vertical direction filtering, it can be
                                                set while input and output pic are same size at vertical direction */
    hi_bool deflicker;  /* whether decrease flicker */
    vgs_scale_coef_mode vgs_scl_coef_mode;

    hi_bool gdc; /* the operation is belong to fisheye */
    hi_rect gdc_rect;

    hi_bool fpd;         /* if enable fpd */
    hi_u64 fpd_phy_addr; /* physical address of fpd */
    vgs_fpd_hw_cfg fpd_opt;

    hi_bool aspect_ratio; /* if enable LBA */
    vgs_aspectratio_opt aspect_ratio_opt;
} vgs_online_opt;

typedef struct {
    hi_u32 stitch_num;
} vgs_stitch_opt;

/* vertical scanning direction */
typedef enum {
    VGS_SCAN_UP_DOWN = 0, /* form up to down */
    VGS_SCAN_DOWN_UP = 1  /* form down to up */
} vgs_drv_vscan;

/* horizontal scanning direction */
typedef enum {
    VGS_SCAN_LEFT_RIGHT = 0, /* form left to right */
    VGS_SCAN_RIGHT_LEFT = 1  /* form right to left */
} vgs_drv_hscan;

/* Definition on scanning direction */
typedef struct {
    /* vertical scanning direction */
    vgs_drv_vscan v_scan;

    /* horizontal scanning direction */
    vgs_drv_hscan h_scan;
} vgs_scandirection;

typedef struct {
    hi_rect src_rect;
    hi_rect dest_rect;
} vgs_osd_quickcopy_opt;

typedef enum {
    VGS_CSC_V0_TYP = 0,
    VGS_CSC_V0_TYP1,
    VGS_CSC_V0_RAND,
    VGS_CSC_V0_MAX,
    VGS_CSC_V0_MIN,
    VGS_CSC_V0_ZERO,
    VGS_CSC_V0_BUTT
} vgs_csc_v0_mode;

typedef enum {
    VGS_VHDR_V_TYP = 0,
    VGS_VHDR_V_TYP1,
    VGS_VHDR_V_RAND,
    VGS_VHDR_V_MAX,
    VGS_VHDR_V_MIN,
    VGS_VHDR_V_ZERO,
    VGS_VHDR_V_BUTT
} vgs_vhdr_v_mode;

typedef enum {
    VGS_RM_COEF_MODE_TYP = 0x0,
    VGS_RM_COEF_MODE_RAN = 0x1,
    VGS_RM_COEF_MODE_MIN = 0x2,
    VGS_RM_COEF_MODE_MAX = 0x3,
    VGS_RM_COEF_MODE_ZRO = 0x4,
    VGS_RM_COEF_MODE_CUS = 0x5,
    VGS_RM_COEF_MODE_UP = 0x6,
    VGS_RM_COEF_MODE_BUTT
} vgs_rm_coef_mode;

typedef struct {
    hi_u32 vgs_isp_cds_cdsh_en;
    hi_u32 vgs_isp_cds_cdsv_en;
    hi_u32 vgs_isp_cds_uv2c_mode;
    hi_s32 vgs_isp_cds_coefh[8];
    hi_u32 vgs_isp_cds_coefv0;
    hi_u32 vgs_isp_cds_coefv1;
} vgs_hdr_cds_cfg;

typedef enum {
    VGS_VHDR_HDR10_IN_SDR_OUT = 0,
    VGS_VHDR_HDR10_IN_HLG_OUT,
    VGS_VHDR_HLG_IN_SDR_OUT,
    VGS_VHDR_HLG_IN_HDR10_OUT,

    VGS_VHDR_SLF_IN_HDR10_OUT,
    VGS_VHDR_HDR10_IN_HDR10_OUT,
    VGS_HDR_HDR10_IN_SDR2020_OUT,
    VGS_VHDR_SDR2020_IN_SDR2020_OUT,
    VGS_VHDR_HLG_IN_SDR2020_OUT,
    VGS_VHDR_SLF_IN_SDR_OUT,
    VGS_VHDR_HLG_IN_SDR10_OUT,
    VGS_VHDR_SLF_IN_SDR10_OUT,
    VGS_VHDR_SDR2020_IN_SDR10_OUT,
    VGS_VHDR_SDR10_IN_SDR10_OUT,

    VGS_VHDR_SDR2020_IN_709_OUT,
    VGS_VHDR_XVYCC,
    VGS_VHDR_SDR2020CL_IN_709_OUT,

    VGS_VHDR_BUTT
} vgs_vhdr_scene_mode;

typedef struct {
    hi_void *reg_data;
} vgs_gme_opt;

typedef hi_s32 fn_vgs_begin_job(hi_vgs_handle *handle, vgs_job_pri pri_level,
                                hi_mod_id call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id,
                                vgs_job_data *job_data);

typedef hi_s32 fn_vgs_end_job(hi_vgs_handle handle, hi_bool sort, vgs_job_data *job_data);

typedef hi_s32 fn_vgs_end_job_block(hi_vgs_handle handle);

typedef hi_s32 fn_vgs_cancel_job(hi_vgs_handle handle);

typedef hi_s32 fn_vgs_cancel_job_by_mod_dev(hi_mod_id call_mod_id, hi_u32 call_dev_id, hi_u32 call_chn_id,
                                            vgs_cancel_stat *cancel_stat);

typedef hi_s32 fn_vgs_add_cover_task(hi_vgs_handle handle, vgs_task_data *task, vgs_cover_opt *cover_opt);

typedef hi_s32 fn_vgs_add_osd_task(hi_vgs_handle handle, vgs_task_data *task, vgs_osd_opt *osd_opt);

typedef hi_s32 fn_vgs_add_online_task(hi_vgs_handle handle, vgs_task_data *task, vgs_online_opt *online_opt);

typedef hi_s32 fn_vgs_add_2scale_task(hi_vgs_handle handle, vgs_task_data *task);

typedef hi_s32 fn_vgs_add_get_luma_stat_task(hi_vgs_handle handle, vgs_task_data *task,
                                             vgs_lumastat_opt *luma_info_opt);

typedef hi_s32 fn_vgs_add_quick_copy_task(hi_vgs_handle handle, vgs_task_data *task);

typedef hi_s32 fn_vgs_add_rotation_task(hi_vgs_handle handle, vgs_task_data *task, hi_rotation angle);

typedef hi_s32 fn_vgs_add_bypass_task(hi_vgs_handle handle, vgs_task_data *task);

typedef hi_s32 fn_vgs_add_gme_task(hi_vgs_handle handle, vgs_task_data *task, vgs_gme_opt *gme_opt);

/* only for test */
typedef hi_void fn_vgs_get_max_job_num(hi_s32 *max_job_num);
typedef hi_void fn_vgs_get_max_task_num(hi_s32 *max_task_num);

typedef vgs_task_data *fn_vgs_get_free_task(hi_void);
typedef hi_void fn_vgs_put_free_task(vgs_task_data *task_data);

#ifdef CONFIG_HI_VGS_STITCH_SUPPORT
typedef hi_s32 fn_vgs_add_stitch_task(hi_vgs_handle handle, vgs_task_data_stitch *task,
                                      vgs_stitch_opt *stitch_opt);
typedef vgs_task_data_stitch *fn_vgs_get_free_stitch_task(hi_void);
typedef hi_void fn_vgs_put_free_stitch_task(vgs_task_data_stitch *task_data_stitch);
#endif

typedef struct {
    fn_vgs_begin_job *pfn_vgs_begin_job;
    fn_vgs_cancel_job *pfn_vgs_cancel_job;
    fn_vgs_cancel_job_by_mod_dev *pfn_vgs_cancel_job_by_mod_dev;
    fn_vgs_end_job *pfn_vgs_end_job;
    fn_vgs_add_cover_task *pfn_vgs_add_cover_task;
    fn_vgs_add_osd_task *pfn_vgs_add_osd_task;
    fn_vgs_add_bypass_task *pfn_vgs_add_bypass_task;
    fn_vgs_add_get_luma_stat_task *pfn_vgs_get_luma_stat_task;
    fn_vgs_add_online_task *pfn_vgs_add_online_task;
    /* for jpeg */
    fn_vgs_add_2scale_task *pfn_vgs_add_2scale_task;
    /* for region */
    fn_vgs_add_quick_copy_task *pfn_vgs_add_quick_copy_task;

    fn_vgs_add_rotation_task *pfn_vgs_add_rotation_task;

    fn_vgs_add_gme_task *pfn_vgs_add_gme_task;

    /* for ive */
    fn_vgs_end_job_block *pfn_vgs_end_job_block;

    /* only for test */
    fn_vgs_get_max_job_num *pfn_vgs_get_max_job_num;
    fn_vgs_get_max_task_num *pfn_vgs_get_max_task_num;

    fn_vgs_get_free_task *pfn_vgs_get_free_task;
    fn_vgs_put_free_task *pfn_vgs_put_free_task;

#ifdef CONFIG_HI_VGS_STITCH_SUPPORT
    fn_vgs_add_stitch_task *pfn_vgs_add_stitch_task;
    fn_vgs_get_free_stitch_task *pfn_vgs_get_free_stitch_task;
    fn_vgs_put_free_stitch_task *pfn_vgs_put_free_stitch_task;
#endif
} vgs_export_func;

#endif /* __VGS_EXT_H__ */

