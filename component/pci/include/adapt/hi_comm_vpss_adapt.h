/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: Vpss common interface
 * Author: Hisilicon multimedia software group
 * Create: 2019/05/6
 */

#ifndef __HI_COMM_VPSS_ADAPT_H__
#define __HI_COMM_VPSS_ADAPT_H__

#include "hi_type.h"
#include "hi_common_adapt.h"
#include "hi_errno_adapt.h"
#include "hi_comm_video_adapt.h"
#include "hi_comm_vpss.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef VPSS_NR_TYPE_E hi_vpss_nr_type;

typedef NR_MOTION_MODE_E hi_nr_motion_mode;

typedef struct {
    hi_vpss_nr_type      nr_type;
    hi_compress_mode     compress_mode;   /* RW; reference frame compress mode */
    hi_nr_motion_mode    nr_motion_mode;   /* RW; NR motion compensate mode. */
} hi_vpss_nr_attr;

typedef struct {
    /* RW; range: hi3559_av100 = [64, 16384] | hi3519_av100 = [64, 8192] | hi3516_cv500 = [64, 2304] |
        hi3516_av300 = [64, 3840] | hi3516_dv300 = [64, 2688] | hi3556_v200 = [64, 4608] | hi3559_v200 = [64, 4608] |
        hi3516_ev200 = [64, 4096]; width of source image. */
    hi_u32                     max_w;
    /* RW; range: hi3559_av100 = [64, 16384] | hi3519_av100 = [64, 8192] | hi3516_cv500 = [64, 2304] |
        hi3516_av300 = [64, 3840] | hi3516_dv300 = [64, 2688] | hi3556_v200 = [64, 4608] | hi3559_v200 = [64, 4608] |
        hi3516_ev200 = [64, 4096]; height of source image. */
    hi_u32                     max_h;
    hi_pixel_format             pixel_format;     /* RW; pixel format of source image. */
    hi_dynamic_range            dynamic_range;    /* RW; dynamic_range of source image. */
    hi_frame_rate_ctrl          frame_rate;       /* grp frame rate contrl. */
    hi_bool                    nr_en;             /* RW;range: [0, 1];  NR enable. */
    hi_vpss_nr_attr             nr_attr;          /* RW; NR attr. */
} hi_vpss_grp_attr;

typedef VPSS_CHN_MODE_E hi_vpss_chn_mode;

typedef struct {
    hi_vpss_chn_mode       chn_mode;          /* RW; vpss channel's work mode. */
    /* RW; range: hi3559_av100 = [64, 16384] | hi3519_av100 = [64, 8192] | hi3516_cv500 = [64, 8192] |
        hi3516_av300 = [64, 8192] | hi3516_dv300 = [64, 8192] | hi3556_v200 = [64, 8192] | hi3559_v200 = [64, 8192] |
        hi3516_ev200 = [64, 4096]; width of target image. */
    hi_u32              width;
    /* RW; range: hi3559_av100 = [64, 16384] | hi3519_av100 = [64, 8192] | hi3516_cv500 = [64, 8192] |
        hi3516_av300 = [64, 8192] | hi3516_dv300 = [64, 8192] | hi3556_v200 = [64, 8192] | hi3559_v200 = [64, 8192] |
        hi3516_ev200 = [64, 4096]; height of target image. */
    hi_u32              height;
    hi_video_format     video_format;      /* RW; video format of target image. */
    hi_pixel_format     pixel_format;      /* RW; pixel format of target image. */
    hi_dynamic_range    dynamic_range;     /* RW; dynamic_range of target image. */
    hi_compress_mode    compress_mode;     /* RW; compression mode of the output. */
    hi_frame_rate_ctrl  frame_rate;        /* frame rate control info */
    hi_bool             mirror;            /* RW; mirror enable. */
    hi_bool             flip;              /* RW; flip enable. */
    hi_u32              depth;           /* RW; range: [0, 8]; user get list depth. */
    hi_aspect_ratio     aspect_ratio;      /* aspect ratio info. */
} hi_vpss_chn_attr;

typedef VPSS_CROP_COORDINATE_E hi_vpss_crop_coordinate;

typedef struct {
    hi_bool                 enable;            /* RW; range: [0, 1];  CROP enable. */
    hi_vpss_crop_coordinate  crop_coordinate;   /* RW;  range: [0, 1]; coordinate mode of the crop start point. */
    hi_rect                  crop_rect;         /* CROP rectangular. */
} hi_vpss_crop_info;

/* only used for hi3519_av100/hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    hi_bool     enable;                        /* RW; range: [0, 1]; whether LDC is enbale */
    hi_ldc_attr  attr;
} hi_vpss_ldc_attr;

/* only used for hi3516_ev200 */
typedef struct {
    hi_bool     enable;                        /* RW;whether LDC is enbale */
    hi_ldc_v3_attr  attr;
} hi_vpss_ldcv3_attr;

typedef struct {
    hi_bool       enable;                      /* whether rotate_ex is enbale */
    hi_rotation_ex rotation_ex;                 /* rotate attribute */
} hi_vpss_rotation_ex_attr;

typedef struct {
    hi_bool enable;          /* RW; low delay enable. */
    hi_u32 line_cnt;        /* RW; range: [16, 16384]; low delay shoreline. */
} hi_vpss_low_delay_info;

typedef struct {
    /* RW; range: [0, 3]; channel bind to. */
    hi_vpss_chn           bind_chn;
    /* RW; range: hi3559_av100 = [64, 16384] | hi3519_av100 = [64, 8192] | hi3516_cv500 = [64, 8192] |
        hi3516_av300 = [64, 8192] | hi3516_dv300 = [64, 8192] | hi3556_v200 = [64, 8192] | hi3559_v200 = [64, 8192] |
        hi3516_ev200 = [64, 4096]; width of target image. */
    hi_u32             width;
    /* RW; range: hi3559_av100 = [64, 16384] | hi3519_av100 = [64, 8192] | hi3516_cv500 = [64, 8192] |
        hi3516_av300 = [64, 8192] | hi3516_dv300 = [64, 8192] | hi3556_v200 = [64, 8192] | hi3559_v200 = [64, 8192] |
        hi3516_ev200 = [64, 4096]; height of target image. */
    hi_u32             height;
    /* RW; video format of target image. */
    hi_video_format     video_format;
    /* RW; pixel format of target image. */
    hi_pixel_format     pixel_format;
    /* RW; dynamic range. */
    hi_dynamic_range    dynamic_range;
    /* RW; compression mode of the output. */
    hi_compress_mode    compress_mode;
    /* RW; range: [0, 8]; user get list depth. */
    hi_u32             depth;
    /* frame rate control info */
    hi_frame_rate_ctrl  frame_rate;
} hi_vpss_extchn_attr;

/* only used for hi3559_av100/hi3519_av100 */
typedef struct {
    /* RW; range: [0, 4095]; undirectional sharpen strength for texture and detail enhancement */
    hi_u16 texture_str[VPSS_SHARPEN_GAIN_NUM];
    /* RW; range: [0, 4095]; directional sharpen strength for edge enhancement */
    hi_u16 edge_str[VPSS_SHARPEN_GAIN_NUM];
    /* RW; range: [0, 4095]; texture frequency adjustment. texture and detail will be finer when it increase */
    hi_u16 texture_freq;
    /* RW; range: [0, 4095]; edge frequency adjustment. edge will be narrower and thiner when it increase */
    hi_u16 edge_freq;
    /* RW; range: [0, 127]; overshoot_amt */
    hi_u8  over_shoot;
    /* RW; range: [0, 127]; undershoot_amt */
    hi_u8  under_shoot;
    /* RW; range: [0, 255]; overshoot and undershoot suppression strength,
        the amplitude and width of shoot will be decrease when shoot_sup_st increase */
    hi_u8  shoot_sup_str;
    /* RW; range: [0, 255]; different sharpen strength for detail and edge.
        when it is bigger than 128, detail sharpen strength will be stronger than edge. */
    hi_u8  detail_ctrl;
    /* RW; Range: [0, 63]; Edge smooth strength. */
    hi_u8  edge_filt_str;
    /* RW; Range: [0, 255]; The threshold of DetailCtrl, it is used to distinguish detail and edge. */
    hi_u8  detail_ctrl_thr;
} hi_vpss_grp_sharpen_manual_attr;

/* only used for hi3559_av100/hi3519_av100 */
typedef struct {
    /* RW; range: [0, 4095]; undirectional sharpen strength for texture and detail enhancement */
    hi_u16 texture_str[VPSS_SHARPEN_GAIN_NUM][VPSS_AUTO_ISO_STRENGTH_NUM];
    /* RW; range: [0, 4095]; directional sharpen strength for edge enhancement */
    hi_u16 edge_str[VPSS_SHARPEN_GAIN_NUM][VPSS_AUTO_ISO_STRENGTH_NUM];
    /* RW; range: [0, 4095]; texture frequency adjustment. texture and detail will be finer when it increase */
    hi_u16 texture_freq[VPSS_AUTO_ISO_STRENGTH_NUM];
    /* RW; range: [0, 4095]; edge frequency adjustment. edge will be narrower and thiner when it increase */
    hi_u16 edge_freq[VPSS_AUTO_ISO_STRENGTH_NUM];
    /* RW; range: [0, 127]; overshoot_amt */
    hi_u8  over_shoot[VPSS_AUTO_ISO_STRENGTH_NUM];
    /* RW; range: [0, 127]; undershoot_amt */
    hi_u8  under_shoot[VPSS_AUTO_ISO_STRENGTH_NUM];
    /* RW; range: [0, 255]; overshoot and undershoot suppression strength,
        the amplitude and width of shoot will be decrease when shoot_sup_st increase */
    hi_u8  shoot_sup_str[VPSS_AUTO_ISO_STRENGTH_NUM];
    /* RW; range: [0, 255]; different sharpen strength for detail and edge.
        when it is bigger than 128, detail sharpen strength will be stronger than edge. */
    hi_u8  detail_ctrl[VPSS_AUTO_ISO_STRENGTH_NUM];
    /* RW; Range: [0, 63]; Edge smooth strength. */
    hi_u8  edge_filt_str[VPSS_AUTO_ISO_STRENGTH_NUM];
    /* RW; Range: [0, 255]; The threshold of DetailCtrl, it is used to distinguish detail and edge. */
    hi_u8  detail_ctrl_thr[VPSS_AUTO_ISO_STRENGTH_NUM];
} hi_vpss_grp_sharpen_auto_attr;

/* only used for hi3559_av100/hi3519_av100 */
typedef struct {
    /* RW;  range: [0, 1];sharpen enable. */
    hi_bool                         enable;
    /* RW; sharpen operation mode. */
    hi_operation_mode                op_type;
    /* RW; range: [0, 127]; sharpen weight based on loacal luma */
    hi_u8                           luma_wgt[VPSS_YUV_SHPLUMA_NUM];
    /* RW; sharpen manual attribute */
    hi_vpss_grp_sharpen_manual_attr  sharpen_manual_attr;
    /* RW; sharpen auto attribute */
    hi_vpss_grp_sharpen_auto_attr    sharpen_auto_attr;
} hi_vpss_grp_sharpen_attr;

/* only used for hi3519_av100 */
typedef struct {
    /* ies0~4 ; range: [0, 255]; the gains of edge and texture enhancement. 0~3 for different frequency response. */
    hi_u8  ies0, ies1, ies2, ies3;
    /* iedz   ; range: [0, 999]; the threshold to control the generated artifacts. */
    hi_u16 iedz : 10, reserved : 6;
} hi_v56a_vpss_iey;

/* only used for hi3519_av100 */
typedef struct {
    /* spn6; range: [0,   4];  the selection of filters to be mixed for NO.6 filter. */
    /* sfr ; range: [0,  31];  the relative NR strength to the s_fi and s_fk filter. */
    hi_u8  spn6 : 3, sfr  : 5;
    /* sbn6; range: [0,   4];  the selection of filters to be mixed for NO.6 filter. */
    /* pbr6; range: [0,  16];  the mix ratio between spn6 and sbn6. */
    hi_u8  sbn6 : 3, pbr6 : 5;
    /* sfs2, sft2, sbr2; range: [0, 255];  the NR strength parameters for NO.1 and NO.2 filters. */
    hi_u8  sfs2, sft2, sbr2;
    /* sfs4, sft4, sbr4; range: [0, 255];  the NR strength parameters for NO.3 and NO.4 filters. */
    hi_u8  sfs4, sft4, sbr4;

    /* sth1~3; range: [0, 511]; the thresholds for protection of edges from blurring */
    /* sfn0~3; range: [0,   6]; filter selection for different image areas based on STH1~3. */
    /* nr_y_en ; range: [0,   1]; the NR switches */
    hi_u16 sth1 : 9,  sfn1 : 3, sfn0  : 3, nr_y_en : 1;
    /* bwsf4 ; range: [0,   1]; the NR window size for the NO.3 and NO.4 filters.  */
    /* k_mode ; range: [0,   3]; a. selection of s_fi and s_fk type filter. b.
        the denoise mode based on image brightness. */
    hi_u16 sth2 : 9,  sfn2 : 3, bwsf4 : 1, k_mode : 3;
    /* t_edge ; range: [0,   3]; NR strength control mode for the image background */
    /* trith ; range: [0,   1]; the switch to choose 3 STH threshold or 2 STH threshold */
    hi_u16 sth3 : 9,  sfn3 : 3, t_edge : 2, tri_th : 1, reserved  : 1;
} hi_v56a_vpss_sfy;

/* only used for hi3519_av100 */
typedef struct {
    /* madz;   range: [0, 999]; the blending ratio between mai2 and mai1 based on image statistics. */
    /* mai0~2; range: [0,   3]; the three blending results between spatial and temporal filtering. */
    hi_u16 madz : 10, mai0 : 2, mai1 : 2,  mai2 : 2;
    /* madk;   range: [0, 255]; the blending ratio between mai2 and mai1 based on brightness. (low limit). */
    /* mabr;   range: [0, 255]; the blending ratio between mai2 and mai1 based on brightness. (high limit). */
    hi_u8  madk,      mabr;
    /* math;   range: [0, 999]; the theshold for motion detection. */
    /* mate;   range: [0,   8]; the motion index for smooth image area. */
    /* matw;   range: [0,   3]; the motion index for prevention of motion ghost. */
    hi_u16 math : 10, mate : 4, matw : 2;
    /* masw;   range: [0,  15]; the motion index for low-frequency noises. */
    /* mabw;   range: [0,   4]; the window size for motion detection. */
    /* maxn;   range: [0,   1]; not for tunning. */
    hi_u8  masw :  4, mabw : 3, maxn : 1, reserved;
} hi_v56a_vpss_mdy;

/* only used for hi3519_av100 */
typedef struct {
    /* tfs;    range: [0,  15]; the NR strength for temporal filtering. */
    /* tdz;    range: [0, 999]; protection of the weak texture area from temporal filtering.  */
    /* tdx;    range: [0,   3]; not for tuning. */
    hi_u16 tfs : 4,  tdz : 10, tdx : 2;
    /* tfr[5]; range: [0,  31]; the temoproal NR strength control for background (static) area. */
    /* tss;    range: [0,  15]; the ratio for blending spatial NR with the temproal NR results. */
    /* tsi;    range: [0,   1]; the selection of blending filter for tss. */
    hi_u8  tfr[5],   tss : 4,  tsi : 1, reserved : 2;
    /* sdz;    range: [0, 999]; the threshold of NR control for result mai1. */
    /* str;    range: [0,  31]; the strength of NR control for result mai1.  */
    /* ref;   range: [0,   1]; the switch for temproal filtering.  */
    hi_u16 sdz : 10, str : 5,  ref : 1;
} hi_v56a_vpss_tfy;

/* only used for hi3519_av100 */
typedef struct {
    /* sfc;    range: [0, 255]; spatial NR strength for the first level. */
    /* tfc;    range: [0,  63]; temporal NR strength. */
    hi_u8  sfc, reserved : 2, tfc : 6;
    /* csfs;   range: [0, 999]; spatial NR strength for the second level. */
    /* csfr;   range: [0,  32]; spatial NR strength control. */
    hi_u16 csfs : 10,    csfr : 6;
    /* ctfr;   range: [0, 999]; temporal NR strength control. */
    /* ctfs;   range: [0,  15]; temporal NR filtering strength */
    /* ciir;   range: [0,   1]; spatial NR mode for the first level. */
    hi_u16 ctfr : 11,    ctfs : 4,    ciir : 1;
    /* mode;    range: [0,  1]; The switch for new chroma denoise mode. */
    hi_u8 mode : 1, reserved_1 : 7;
    /* presfc;    range: [0,  32]; The strength for chroma pre spatial filter. */
    hi_u8  presfc : 6, reserved_2 : 2;
} hi_v56a_vpss_nrc;

/* only used for hi3519_av100 */
typedef struct {
    hi_v56a_vpss_iey iey[2];
    hi_v56a_vpss_sfy sfy[4];
    hi_v56a_vpss_mdy mdy[2];
    hi_v56a_vpss_tfy tfy[2];
    hi_v56a_vpss_nrc nr_c;

    /* sbs_k2[32], sds_k2[32]; range [0, 8192];  spatial NR strength based on brightness. */
    hi_u16 sbs_k2[32], sds_k2[32];
    /* sbs_k3[32], sds_k3[32]; range [0, 8192];  spatial NR strength based on brightness. */
    hi_u16 sbs_k3[32], sds_k3[32];
} hi_vpss_nrx_v1;

/* only used for hi3519_av100 */
typedef struct {
    hi_vpss_nrx_v1 nrx_param;
} hi_vpss_nrx_param_manual_v1;

/* only used for hi3519_av100 */
typedef struct {
    hi_u32 param_num;
    hi_u32 *iso;
    hi_vpss_nrx_v1 *nrx_param;
} hi_vpss_nrx_param_auto_v1;

/* only used for hi3519_av100 */
typedef struct {
    hi_operation_mode           opt_mode;           /* RW;adaptive NR */
    hi_vpss_nrx_param_manual_v1 nrx_manual;         /* RW;NRX V1 param for manual video */
    hi_vpss_nrx_param_auto_v1   nrx_auto;           /* RW;NRX V1 param for auto video */
} hi_vpss_nrx_param_v1;

/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    /* ies0~4 ; range: [0, 255]; the gains of edge and texture enhancement. 0~3 for different frequency response. */
    hi_u8  ies0, ies1, ies2, ies3;
    hi_u16 iedz : 10, reserved : 6;     /* iedz   ; range: [0, 999]; the threshold to control the generated artifacts. */
} hi_v500_vpss_iey;

/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    /* spn6; range: [0,   5];  the selection of filters to be mixed for NO.6 filter. */
    /* sfr ; range: [0,  31];  the relative NR strength in the s_fi and s_fk filter. */
    /* sbn6; range: [0,   5];  the selection of filters to be mixed for NO.6 filter. */
    /* pbr6; range: [0,  16];  the mix ratio between spn6 and sbn6. */
    hi_u8  spn6 : 3, sfr  : 5;
    hi_u8  sbn6 : 3, pbr6 : 5;

    /* j_mode;      range: [0,   4]; the selection modes for the blending of spatial filters */
    /* srt0, srt1; range: [0,  16]; the blending ratio of different filters. (used in serial filtering mode (SFM).) */
    /* de_idx;      range: [3,   6]; the selection number of filters that textures and details will be added to. */
    hi_u16 srt0 : 5, srt1 : 5, j_mode : 3,  de_idx : 3;
    /* de_rate;     range: [0, 255]; the enhancement strength for the SFM (when de_rate > 0, the SFM will be activated) */
    /* sfr6;       range: [0,  31]; the relative NR strength for NO.6 filter. (effective when j_mode = 4) */
    hi_u8  de_rate, sfr6[3];

    /* sfs1, sft1, sbr1; range: [0, 255];  the NR strength parameters for NO.1 filter. */
    /* sfs2, sft2, sbr2; range: [0, 255];  the NR strength parameters for NO.2 filter. */
    /* sfs4, sft4, sbr4; range: [0, 255];  the NR strength parameters for NO.3 and NO.4 filters. */
    hi_u8  sfs1,  sft1,  sbr1;
    hi_u8  sfs2,  sft2,  sbr2;
    hi_u8  sfs4,  sft4,  sbr4;

    /* sth1~3; range: [0, 511]; the thresholds for protection of edges from blurring */
    /* nr_y_en ; range: [0,   1]; the NR switches */
    /* sfn0~3; range: [0,   6]; filter selection for different image areas based on sth1~3. */
    /* bwsf4 ; range: [0,   1]; the NR window size for the NO.3 and NO.4 filters.  */
    /* k_mode ; range: [0,   3]; the denoise mode based on image brightness. */
    /* trith ; range: [0,   1]; the switch to choose 3 sth threshold or 2 STH threshold */
    hi_u16 sth1 : 9, sfn1 : 3, sfn0  : 3, nr_y_en : 1;
    hi_u16 sth2 : 9, sfn2 : 3, bwsf4 : 1, k_mode : 3;
    hi_u16 sth3 : 9, sfn3 : 3, tri_th : 1, reserved : 3;

    /* sbs_k[32], sds_k[32]; range [0, 8191];  spatial NR strength based on brightness. */
    hi_u16 sbs_k[32], sds_k[32];
} hi_v500_vpss_sfy;

/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    /* madz0, madz1;     range: [0, 511]; the blending ratio between mai2 and mai1 based on image statistics. */
    /* mai00~02,mai10~12 range: [0,   3]; the three blending results between spatial and temporal filtering. */
    /* mabr0, mabr1;  range: [0, 255]; the blending ratio between mai2 and mai1 based on brightness.  */
    /* bi_path;           range: [0,   1]; the switch for single path or dual path. 0: single path; 1: dual path. */
    hi_u16 madz0   : 9,  mai00    : 2,  mai01  : 2,  mai02    : 2,  bi_path  : 1;
    hi_u16 madz1   : 9,  mai10    : 2,  mai11  : 2,  mai12    : 2,  reserved : 1;
    hi_u8  mabr0, mabr1;

    /* math0,math1;   range: [0, 999]; the theshold for motion detection. */
    /* mate0,mate1;   range: [0,   8]; the motion index for smooth image area. */
    /* matw;   range: [0,   3]; the motion index for prevention of motion ghost. */
    hi_u16 math0   : 10,  mate0   : 4,  matw   : 2;
    hi_u16 math1   : 10,  mate1   : 4,  reserved_1  : 2;


    /* masw;   range: [0,  15]; the motion index for low-frequency noises. */
    /* mabw0,mabw1;   range: [0,   9]; the window size for motion detection. */
    hi_u8  masw    :  4,  reserved_2   : 4;
    hi_u8  mabw0   :  4,  mabw1   : 4;
} hi_v500_vpss_mdy;
/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    /* tfs0,tfs1;        range: [0,  15]; the NR strength for temporal filtering. */
    /* tdz0,tdz1;        range: [0, 999]; protection of the weak texture area from temporal filtering.  */
    /* tdx0,tdx1;        range: [0,   3]; not for tuning. */
    hi_u16 tfs0  :  4, tdz0   : 10, tdx0    : 2;
    hi_u16 tfs1  :  4, tdz1   : 10, tdx1    : 2;

    /* sdz0,sdz1;        range: [0, 999]; the threshold of NR control for result mai1. */
    /* str0,str1;        range: [0,  31]; the strength of NR control for result mai1.  */
    /* dz_mode0, dz_mode1; range: [0,   1]; the selection mode for tdz0 and tdz1, respectively.  */
    hi_u16 sdz0  : 10, str0   : 5,  dz_mode0 : 1;
    hi_u16 sdz1  : 10, str1   : 5,  dz_mode1 : 1;

    /* tfr0,tfr1;        range: [0,  31]; the temoproal NR strength control for background (static) area. */
    /* tss0,tss1;        range: [0,  15]; the ratio for blending spatial NR with the temproal NR results. */
    /* tsi0,tsi1;        range: [0,   1]; the selection of blending filter for TSS. */
    hi_u8  tfr0[6],    tss0   : 4,  tsi0    : 4;
    hi_u8  tfr1[6],    tss1   : 4,  tsi1    : 4;

    /* t_edge;            range: [0,   3]; NR strength control mode for the updating background. */
    /* rfi;              range: [0,   4]; reference mode. (used in when NR_MOTION_MODE_COMPENSATE are selected). */
    /* ref;             range: [0,   1]; the switch for temproal filtering. */
    hi_u8  rfi   : 3,  t_edge  : 2,  ref    : 1,  reserved : 2;
} hi_v500_vpss_tfy;

/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    /* adv_math;        range: [0,   1]; the switch for advanced motion dection.  */
    /* rfui;           range: [0,   4]; the modes for updating reference for n_ry leve 2,
        (used in when NR_MOTION_MODE_COMPENSATE are selected). */
    hi_u16 adv_math : 1, rfdz  : 9,    reserved : 6;
    /* rfdz;           rnage: [0, 511]; the threshold for rfi0 and rfi1 mode 3 and 4. */
    /* rfslp;          rnage: [0,  31]; the strength for rfi0 and rfi1 mode 3 and 4. */
    hi_u8  rfui    : 3, rfslp : 5;
} hi_v500_vpss_rfs;

/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    hi_v500_vpss_iey  iey;
    hi_v500_vpss_sfy  sfy;
    hi_u8 nr_c_en : 1, reserved : 7;
} hi_v500_vpss_nr_c;

/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    /* sfc;    range: [0, 255]; spatial NR strength. */
    hi_u8  sfc;
    /* tfc;    range: [0,  63]; temporal NR strength relative to spatial NR. */
    /* ctfs;   range: [0,  15]; absolute temporal NR strength. */
    hi_u16 ctfs : 4, tfc : 6, reserved : 6;
    /* mode;    range: [0,  1]; The switch for new chroma denoise mode. */
    hi_u8 mode : 1, reserved_1 : 7;
    /* presfc;    range: [0,  32]; The strength for chroma pre spatial filter. */
    hi_u8  presfc : 6, reserved_2 : 2;

} hi_v500_vpss_p_nr_c;

/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    hi_v500_vpss_iey  iey[3];
    hi_v500_vpss_sfy  sfy[3];
    hi_v500_vpss_mdy  mdy[2];
    hi_v500_vpss_rfs  rfs;
    hi_v500_vpss_tfy  tfy[2];
    hi_v500_vpss_p_nr_c p_nr_c;
    hi_v500_vpss_nr_c  nr_c;
} hi_vpss_nrx_v2;

/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    hi_vpss_nrx_v2 nrx_param;
} hi_vpss_nrx_param_manual_v2;

/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    hi_u32 param_num;
    hi_u32 *iso;
    hi_vpss_nrx_v2 *nrx_param;
} hi_vpss_nrx_param_auto_v2;

/* only used for hi3516_cv500/hi3516_av300/hi3516_dv300/hi3556_v200/hi3559_v200 */
typedef struct {
    hi_operation_mode           opt_mode;           /* RW;adaptive NR */
    hi_vpss_nrx_param_manual_v2 nrx_manual;         /* RW;NRX V2 param for manual video */
    hi_vpss_nrx_param_auto_v2   nrx_auto;           /* RW;NRX V2 param for auto video */
} hi_vpss_nrx_param_v2;

/* only used for hi3516_ev200 */
typedef struct {
    /* ies0~4 ; range: [0, 255]; the gains of edge and texture enhancement. 0~3 for different frequency response. */
    hi_u8  ies0, ies1, ies2, ies3;
    /* iedz   ; range: [0, 999]; the threshold to control the generated artifacts. */
    hi_u16  iedz : 10, ie_en : 1, reserved : 5;
} hi_v200_vpss_iey;

/* only used for hi3516_ev200 */
typedef struct {
    hi_u8  spn6 : 3, sfr  : 5;                                      /* spn6, sbn6:  [0, 5]; */

    hi_u8  sbn6 : 3, pbr6 : 5;                                    /* sfr: [0,31];  pbr6: [0,15]; */

    /* j_mode;      range: [0,   4]; the selection modes for the blending of spatial filters */
    /* srt0, srt1; range: [0,  16]; the blending ratio of different filters. (used in serial filtering mode (SFM).) */
    /* de_idx;      range: [3,   6]; the selection number of filters that textures and details will be added to. */
    hi_u16  srt0 : 5, srt1 : 5, j_mode : 3, de_idx : 3;

    /* de_rate;     range: [0, 255]; the enhancement strength for the SFM (when de_rate > 0, the SFM will be activated) */
    /* sfr6[4];    range: [0,  31]; the relative NR strength for NO.6 filter. (effective when j_mode = 4) */
    /* sbr6[2];    range: [0,  15]; the control of overshoot and undershoot. */
    hi_u8  sfr6[4], sbr6[2], de_rate;

    /* sfs1, sft1, sbr1; range: [0, 255];  the NR strength parameters for NO.1 filter. */
    hi_u8  sfs1,  sft1,  sbr1;
    /* sfs2, sft2, sbr2; range: [0, 255];  the NR strength parameters for NO.2 filter. */
    hi_u8  sfs2,  sft2,  sbr2;
    /* sfs4, sft4, sbr4; range: [0, 255];  the NR strength parameters for NO.3 and NO.4 filters. */
    hi_u8  sfs4,  sft4,  sbr4;

    /* sth1, sth2, sth_d1, sth_d2; range: [0, 511]; the thresholds for protection of edges from blurring */
    /* nr_y_en;      range: [0,   1]; the NR switches */
    /* sfn0~2;     range: [0,   6]; filter selection for different image areas based on sth1~3. */
    /* k_mode ;     range: [0,   3]; the denoise mode based on image brightness. */
    /* sbs_k[32], sds_k[32]; range [0, 8191];  spatial NR strength based on brightness. */
    hi_u16  sth1 : 9,  sfn1 : 3, sfn0  : 3, nr_y_en   : 1;
    hi_u16  sth_d1 : 9, reserved : 7;
    hi_u16  sth2 : 9,  sfn2 : 3, k_mode : 3, reserved_1   : 1;
    hi_u16  sth_d2 : 9, reserved_2 : 7;
    hi_u16  sbs_k[32], sds_k[32];
} hi_v200_vpss_sfy;

/* only used for hi3516_ev200 */
typedef struct {
    /* tfs0,tfs1;          range: [0,  15]; the NR strength for temporal filtering. */
    /* tdz0,tdz1;          range: [0, 999]; protection of the weak texture area from temporal filtering.  */
    /* tdx0,tdx1;          range: [0,   3]; not for tuning. */
    hi_u16  tfs0 : 4,   tdz0 : 10,  tdx0    : 2;
    hi_u16  tfs1 : 4,   tdz1 : 10,  tdx1    : 2;

    /* sdz0,sdz1;          range: [0, 999]; the threshold of NR control for result mai1. */
    /* str0,str1;          range: [0,  31]; the strength of NR control for result mai1.  */
    /* dz_mode0, dz_mode1;   range: [0,   1]; the selection mode for tdz0 and tdz1, respectively.  */
    hi_u16  sdz0 : 10,  str0 : 5,   dz_mode0 : 1;
    hi_u16  sdz1 : 10,  str1 : 5,   dz_mode1 : 1;

    /* tfr0,tfr1;          range: [0,  31]; the temoproal NR strength control for background (static) area. */
    /* tss0,tss1;          range: [0,  15]; the ratio for blending spatial NR with the temproal NR results. */
    /* tsi0,tsi1;          range: [0,   1]; the selection of blending filter for TSS. */
    hi_u8  tss0 : 4,   tsi0 : 4,  tfr0[6];
    hi_u8  tss1 : 4,   tsi1 : 4,  tfr1[6];

    /* ted;  range: [0,   3]; NR strength control mode for the updating background. */
    /* ref;   range: [0,   1]; the switch for temproal filtering.  */
    /* tfrs;   range: [0,  15]; spatial filtering strength for static area. */
    hi_u8  tfrs : 4,   ted  : 2,   ref    : 1,  reserved : 1;
} hi_v200_vpss_tfy;

/* only used for hi3516_ev200 */
typedef struct {
    /* PATH0 */
    /* madz0, madz1;     range: [0, 511]; the blending ratio between mai2 and mai1 based on image statistics. */
    /* mai00~02,mai10~12 range: [0,   3]; the three blending results between spatial and temporal filtering. */
    hi_u16  madz0   : 9,   mai00 : 2,  mai01  : 2, mai02 : 2, reserved : 1;
    hi_u16  madz1   : 9,   mai10 : 2,  mai11  : 2, mai12 : 2, reserved_1 : 1;
    /* mabr0, mabr1;     range: [0, 255]; the blending ratio between mai2 and mai1 based on brightness.  */
    hi_u8  mabr0, mabr1;

    /* math0,math1, math_d0, math_d1;     range: [0, 999]; the theshold for motion detection. */
    /* mate0,mate1;   range: [0,   8]; the motion index for smooth image area. */
    /* matw;          range: [0,   3]; the motion index for prevention of motion ghost. */
    /* masw;          range: [0,  15]; the motion index for low-frequency noises. */
    /* mabw0,mabw1;   range: [0,   9]; the window size for motion detection. */
    hi_u16  math0   : 10,  mate0 : 4,  matw   : 2;
    hi_u16  math_d0  : 10,  reserved_2 : 6;
    hi_u16  math1   : 10,  reserved_3 : 6;
    hi_u16  math_d1  : 10,  reserved_4 : 6;
    hi_u8  masw    :  4,  mate1 : 4;
    hi_u8  mabw0   :  4,  mabw1 : 4;

    /* adv_math:  range: [0, 1]; the switch to active the advanced mode. */
    /* adv_th:    range: [0, 999]; the threshold to control the effects of the adv_math. */
    hi_u16  adv_math : 1,   adv_th : 12, reserved_5  : 3;
} hi_v200_vpss_mdy;

/* only used for hi3516_ev200 */
typedef struct {
    /* sfc;    range: [0, 255]; spatial NR strength. */
    /* tfc;    range: [0,  32]; temporal NR strength relative to spatial NR. */
    hi_u8  sfc, tfc : 6, reserved : 2;
    /* trc;    range: [0, 255]; control of color bleeding in */
    /* tpc;    range: [0,  32]; type of temporal NR. */
    hi_u8  trc, tpc : 6, reserved_1 : 2;
    /* mode;    range: [0,  1]; The switch for new chroma denoise mode. */
    hi_u8 mode : 1, reserved_2 : 7;
    /* presfc;    range: [0,  32]; The strength for chroma pre spatial filter. */
    hi_u8  presfc : 6, reserved_3 : 2;

} hi_v200_vpss_nr_c;

/* only used for hi3516_ev200 */
typedef struct {
    hi_v200_vpss_iey  iey[5];
    hi_v200_vpss_sfy  sfy[5];
    hi_v200_vpss_mdy  mdy[2];
    hi_v200_vpss_tfy  tfy[3];
    hi_v200_vpss_nr_c  nr_c;
} hi_vpss_nrx_v3;

/* only used for hi3516_ev200 */
typedef struct {
    hi_vpss_nrx_v3 nrx_param;
} hi_vpss_nrx_param_manual_v3;

/* only used for hi3516_ev200 */
typedef struct {
    hi_u32 param_num;
    hi_u32 *iso;
    hi_vpss_nrx_v3 *nrx_param;
} hi_vpss_nrx_param_auto_v3;

/* only used for hi3516_ev200 */
typedef struct {
    hi_operation_mode           opt_mode;           /* RW;adaptive NR */
    hi_vpss_nrx_param_manual_v3 nrx_manual;         /* RW;NRX V3 param for manual video */
    hi_vpss_nrx_param_auto_v3   nrx_auto;           /* RW;NRX V3 param for auto video */
} hi_vpss_nrx_param_v3;

/* not support for hi3559_av100 */
typedef VPSS_NR_VER_E hi_vpss_nr_ver;

/* not support for hi3559_av100 */
typedef struct {
    hi_vpss_nr_ver nr_ver;
    union {
        hi_vpss_nrx_param_v1 nrx_param_v1;   /* interface X V1 for hi3519_av100 */
        hi_vpss_nrx_param_v2 nrx_param_v2;   /* interface X V2 for hi3516_cv500 */
        hi_vpss_nrx_param_v3 nrx_param_v3;   /* interface X V3 for hi3516_ev200 */
    };

} hi_vpss_grp_nrx_param;

typedef struct {
    hi_bool enable;
    hi_u32  buf_line;             /* RW; range: [128, H]; chn buffer allocated by line. */
    hi_u32  wrap_buffer_size; /* RW; whether to allocate buffer according to compression. */
} hi_vpss_chn_buf_wrap;

typedef struct {
    hi_bool one_buf_for_low_delay;
    hi_u32  vpss_vb_source;
    hi_u32  vpss_split_node_num;
    hi_bool hdr_support;
    hi_bool nr_quick_start;
} hi_vpss_mod_param;

typedef VPSS_CHN_PROC_MODE_E hi_vpss_chn_proc_mode;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __HI_COMM_VPSS_ADAPT_H__ */

