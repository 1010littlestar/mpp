/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#ifndef __GF_GHDR_LUT_H__
#define __GF_GHDR_LUT_H__

#include "hi_type.h"

#define VO_MAX_TONEMAP_LUT_LEN 32
#define VO_MAX_GAMMA_LUT_LEN   64

typedef struct {
    HI_U32 g_degmm_x1_step;
    HI_U32 g_degmm_x2_step;
    HI_U32 g_degmm_x3_step;
    HI_U32 g_degmm_x4_step;
    HI_U32 g_degmm_x1_pos;
    HI_U32 g_degmm_x2_pos;
    HI_U32 g_degmm_x3_pos;
    HI_U32 g_degmm_x4_pos;
    HI_U32 g_degmm_x1_num;
    HI_U32 g_degmm_x2_num;
    HI_U32 g_degmm_x3_num;
    HI_U32 g_degmm_x4_num;
} vo_gfx_degmm;

typedef struct {
    HI_S32 ghdr_gamut_coef00;
    HI_S32 ghdr_gamut_coef01;
    HI_S32 ghdr_gamut_coef02;
    HI_S32 ghdr_gamut_coef10;
    HI_S32 ghdr_gamut_coef11;
    HI_S32 ghdr_gamut_coef12;
    HI_S32 ghdr_gamut_coef20;
    HI_S32 ghdr_gamut_coef21;
    HI_S32 ghdr_gamut_coef22;
    HI_U32 ghdr_gamut_scale;
    HI_U32 ghdr_gamut_clip_min;
    HI_U32 ghdr_gamut_clip_max;
} vo_gfx_ghdr_gamut;

typedef struct {
    HI_U32 g_tmap_x1_step;
    HI_U32 g_tmap_x2_step;
    HI_U32 g_tmap_x3_step;
    HI_U32 g_tmap_x4_step;
    HI_U32 g_tmap_x1_pos;
    HI_U32 g_tmap_x2_pos;
    HI_U32 g_tmap_x3_pos;
    HI_U32 g_tmap_x4_pos;
    HI_U32 g_tmap_x1_num;
    HI_U32 g_tmap_x2_num;
    HI_U32 g_tmap_x3_num;
    HI_U32 g_tmap_x4_num;
} vo_gfx_tmap;

typedef struct {
    HI_U32 ghdr_tmap_luma_coef0;
    HI_U32 ghdr_tmap_luma_coef1;
    HI_U32 ghdr_tmap_luma_coef2;
    HI_U32 ghdr_tmap_luma_scale;
    HI_U32 ghdr_tmap_coef_scale;
    HI_U32 ghdr_tmap_out_clip_min;
    HI_U32 ghdr_tmap_out_clip_max;
} vo_gfx_ghdr_tmap;

typedef struct {
    HI_U32 g_gmm_x1_step;
    HI_U32 g_gmm_x2_step;
    HI_U32 g_gmm_x3_step;
    HI_U32 g_gmm_x4_step;
    HI_U32 g_gmm_x5_step;
    HI_U32 g_gmm_x6_step;
    HI_U32 g_gmm_x7_step;
    HI_U32 g_gmm_x8_step;
    HI_U32 g_gmm_x1_pos;
    HI_U32 g_gmm_x2_pos;
    HI_U32 g_gmm_x3_pos;
    HI_U32 g_gmm_x4_pos;
    HI_U32 g_gmm_x5_pos;
    HI_U32 g_gmm_x6_pos;
    HI_U32 g_gmm_x7_pos;
    HI_U32 g_gmm_x8_pos;
    HI_U32 g_gmm_x1_num;
    HI_U32 g_gmm_x2_num;
    HI_U32 g_gmm_x3_num;
    HI_U32 g_gmm_x4_num;
    HI_U32 g_gmm_x5_num;
    HI_U32 g_gmm_x6_num;
    HI_U32 g_gmm_x7_num;
    HI_U32 g_gmm_x8_num;
} vo_gfx_gmm;

typedef struct {
    HI_U32 ghdr_dither_thr_max;
    HI_U32 ghdr_dither_thr_min;
    HI_U32 ghdr_dither_sed_y0;
    HI_U32 ghdr_dither_sed_u0;
    HI_U32 ghdr_dither_sed_v0;
    HI_U32 ghdr_dither_sed_w0;
    HI_U32 ghdr_dither_sed_y1;
    HI_U32 ghdr_dither_sed_u1;
    HI_U32 ghdr_dither_sed_v1;
    HI_U32 ghdr_dither_sed_w1;
} vo_gfx_ghdr_dither;

/************* DeGamma Luts **************/
/************* ToneMapping Luts **************/
// ToneMapping Lut: Default
extern HI_U32 g_u32TMLut_G[VO_MAX_TONEMAP_LUT_LEN];
extern HI_U32 g_u32TMLutSDR2HDR10[VO_MAX_TONEMAP_LUT_LEN];
extern HI_U32 g_u32TMLutSDR2BBC[VO_MAX_TONEMAP_LUT_LEN];

/************* Gamma Luts **************/
// g_u32PQLut_G Lut: Default
/******************* Gmm-8 Lut Params ************/
extern HI_U32 g_u32PQLut_G[VO_MAX_GAMMA_LUT_LEN];

// PQ Curve
extern HI_U32 g_u32PQLut2500[VO_MAX_GAMMA_LUT_LEN];

// HLG Curve
extern HI_U32 g_u32HLGLut_G[VO_MAX_GAMMA_LUT_LEN];

// HDR COEF
extern const vo_gfx_degmm g_DEGMM_OF_SDR_IN_HDR10_OUT;
extern const vo_gfx_ghdr_gamut g_GHDR_GAMUT_OF_SDR_IN_HDR10_OUT;
extern const vo_gfx_tmap g_TMAP_OF_SDR_IN_HDR10_OUT;
extern const vo_gfx_ghdr_tmap g_GHDR_TMAP_OF_SDR_IN_HDR10_OUT;
extern const vo_gfx_gmm g_GMM_OF_SDR_IN_HDR10_OUT;
extern const vo_gfx_ghdr_dither g_GHDR_DITHER_OF_SDR_IN_HDR10_OUT;

extern const vo_gfx_degmm g_DEGMM_OF_SDR_IN_HLG_OUT;
extern const vo_gfx_ghdr_gamut g_GHDR_GAMUT_OF_SDR_IN_HLG_OUT;
extern const vo_gfx_tmap g_TMAP_OF_SDR_IN_HLG_OUT;
extern const vo_gfx_ghdr_tmap g_GHDR_TMAP_OF_SDR_IN_HLG_OUT;
extern const vo_gfx_gmm g_GMM_OF_SDR_IN_HLG_OUT;
extern const vo_gfx_ghdr_dither g_GHDR_DITHER_OF_SDR_IN_HLG_OUT;

extern const vo_gfx_degmm g_DEGMM_OF_SDR_PREMULT;
extern const vo_gfx_ghdr_gamut g_GHDR_GAMUT_OF_SDR_PREMULT;
extern const vo_gfx_tmap g_TMAP_OF_SDR_PREMULT;
extern const vo_gfx_ghdr_tmap g_GHDR_TMAP_OF_SDR_PREMULT;
extern const vo_gfx_gmm g_GMM_OF_SDR_PREMULT;
extern const vo_gfx_ghdr_dither g_GHDR_DITHER_OF_SDR_PREMULT;

extern const vo_gfx_degmm g_DEGMM_OF_SDR709_IN_2020_OUT;
extern const vo_gfx_ghdr_gamut g_GHDR_GAMUT_OF_SDR709_IN_2020_OUT;
extern const vo_gfx_tmap g_TMAP_OF_SDR709_IN_2020_OUT;
extern const vo_gfx_ghdr_tmap g_GHDR_TMAP_OF_SDR709_IN_2020_OUT;
extern const vo_gfx_gmm g_GMM_OF_SDR709_IN_2020_OUT;
extern const vo_gfx_ghdr_dither g_GHDR_DITHER_OF_SDR709_IN_2020_OUT;

extern const vo_gfx_degmm g_DEGMM_OF_SDR_IN_DEF_OUT;
extern const vo_gfx_ghdr_gamut g_GHDR_GAMUT_OF_SDR_IN_DEF_OUT;
extern const vo_gfx_tmap g_TMAP_OF_SDR_IN_DEF_OUT;
extern const vo_gfx_ghdr_tmap g_GHDR_TMAP_OF_SDR_IN_DEF_OUT;
extern const vo_gfx_gmm g_GMM_OF_SDR_IN_DEF_OUT;
extern const vo_gfx_ghdr_dither g_GHDR_DITHER_OF_SDR_IN_DEF_OUT;

#endif
