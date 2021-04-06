/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#include "gf_ghdr_lut.h"

/************* DeGamma Luts **************/
/************* ToneMapping Luts **************/
// ToneMapping Lut: Default
HI_U32 g_u32TMLut_G[VO_MAX_TONEMAP_LUT_LEN] = {  // U8.8 [0,65535]
    4160, 4160, 4160, 4160, 4160, 4160, 3771, 3418,
    3134, 2899, 2700, 2531, 2383, 2254, 2140, 2038,
    1946, 1788, 1638, 1489, 1365, 1260, 1170, 1092,
    1024, 819, 683, 512, 410, 341, 293, 256
};

HI_U32 g_u32TMLutSDR2HDR10[VO_MAX_TONEMAP_LUT_LEN] = {  // U8.8 [0,65535]
    102, 102, 102, 102, 102, 102, 102, 102,
    102, 102, 102, 102, 102, 102, 102, 102,
    102, 102, 102, 102, 102, 102, 102, 102,
    102, 102, 102, 102, 102, 102, 102, 102
};

HI_U32 g_u32TMLutSDR2BBC[VO_MAX_TONEMAP_LUT_LEN] = {  // U8.8 [0,65535]
    256, 256, 256, 256, 256, 256, 256, 256,
    256, 256, 256, 256, 256, 256, 256, 256,
    256, 256, 256, 256, 256, 256, 256, 256,
    256, 256, 256, 256, 256, 256, 256, 256
};

/************* Gamma Luts **************/
// g_u32PQLut_G Lut: Default
/******************* Gmm-8 Lut Params ************/
HI_U32 g_u32PQLut_G[VO_MAX_GAMMA_LUT_LEN] = {  // U12.0 [0,4095]
    205, 264, 320, 370, 420, 657, 749, 820,
    878, 927, 970, 1008, 1129, 1220, 1354, 1454,
    1533, 1600, 1708, 1794, 1865, 1926, 1980, 2146,
    2267, 2361, 2440, 2506, 2564, 2616, 2662, 2704,
    2742, 2777, 2810, 2841, 2869, 2969, 3050, 3119,
    3178, 3231, 3278, 3321, 3360, 3395, 3428, 3459,
    3488, 3515, 3540, 3564, 3587, 3668, 3736, 3794,
    3846, 3892, 3933, 3971, 4006, 4038, 4067, 4095
};

// PQ Curve
HI_U32 g_u32PQLut2500[VO_MAX_GAMMA_LUT_LEN] = {  // U12.0 [0,4095]
    0, 167, 227, 270, 304, 400, 466, 517,
    559, 596, 628, 657, 749, 820, 927, 1008,
    1074, 1129, 1220, 1293, 1354, 1407, 1454, 1600,
    1708, 1794, 1865, 1926, 1980, 2028, 2071, 2110,
    2146, 2179, 2210, 2239, 2267, 2361, 2440, 2506,
    2564, 2616, 2662, 2704, 2742, 2777, 2810, 2841,
    2869, 2896, 2922, 2946, 2969, 3050, 3119, 3178,
    3231, 3278, 3321, 3360, 3395, 3428, 3459, 3488
};

// HLG Curve
HI_U32 g_u32HLGLut_G[VO_MAX_GAMMA_LUT_LEN] = {  // U12.0 [0,4095]
    0, 28, 39, 48, 55, 78, 96, 111,
    124, 136, 147, 157, 192, 222, 271, 313,
    350, 384, 443, 496, 543, 586, 627, 768,
    887, 991, 1086, 1173, 1254, 1330, 1402, 1470,
    1536, 1598, 1659, 1717, 1773, 1982, 2165, 2313,
    2436, 2541, 2633, 2714, 2788, 2854, 2916, 2972,
    3024, 3073, 3119, 3162, 3203, 3346, 3466, 3569,
    3660, 3740, 3812, 3878, 3939, 3995, 4047, 4095
};

// HDR COEF
const vo_gfx_degmm g_DEGMM_OF_SDR_IN_HDR10_OUT = {
    /* degmm step */
    4, 4, 4, 5,
    /* degmm pos */
    672, 832, 992, 1023,
    /* degmm num */
    42, 10, 10, 1
};

const vo_gfx_ghdr_gamut g_GHDR_GAMUT_OF_SDR_IN_HDR10_OUT = {  // BT709 to BT2020
    /* gamut coef */
    10280, 5396, 709,
    1133, 15066, 185,
    269, 1442, 14672,
    /* scale, clip_min, clip_max */
    14, 0, 65535
};

const vo_gfx_tmap g_TMAP_OF_SDR_IN_HDR10_OUT = {
    /* tmap step */
    9, 10, 12, 13,
    /* tmap pos */
    8192, 16384, 24576, 65535,
    /* tmap num */
    16, 8, 2, 5
};

const vo_gfx_ghdr_tmap g_GHDR_TMAP_OF_SDR_IN_HDR10_OUT = {
    /* tmap luma coef */
    1076, 2777, 243,
    /* scale */
    12, 8,
    /* clip */
    0, 65535
};

const vo_gfx_gmm g_GMM_OF_SDR_IN_HDR10_OUT = {
    /* gmm step */
    0, 2, 4, 5, 6, 8, 10, 12,
    /* gmm pos */
    4, 32, 64, 192, 512, 4096, 20480, 65535,
    /* gmm num */
    4, 11, 13, 17, 22, 36, 52, 63
};

const vo_gfx_ghdr_dither g_GHDR_DITHER_OF_SDR_IN_HDR10_OUT = {
    65535, 0,
    2147483647, 2147483647, 2147483647, 2147483647,
    2140000000, 2140000000, 2140000000, 2140000000
};

const vo_gfx_degmm g_DEGMM_OF_SDR_IN_HLG_OUT = {
    /* degmm step */
    4, 4, 4, 5,
    /* degmm pos */
    672, 832, 992, 1023,
    /* degmm num */
    42, 10, 10, 1
};

const vo_gfx_ghdr_gamut g_GHDR_GAMUT_OF_SDR_IN_HLG_OUT = {  // BT709 to BT2020
    /* gamut coef */
    10280, 5396, 709,
    1133, 15066, 185,
    269, 1442, 14672,
    /* scale, clip_min, clip_max */
    14, 0, 65535
};

const vo_gfx_tmap g_TMAP_OF_SDR_IN_HLG_OUT = {
    /* tmap step */
    9, 10, 12, 13,
    /* tmap pos */
    8192, 16384, 24576, 65535,
    /* tmap num */
    16, 8, 2, 5
};

const vo_gfx_ghdr_tmap g_GHDR_TMAP_OF_SDR_IN_HLG_OUT = {
    /* tmap luma coef */
    1076, 2777, 243,
    /* scale */
    12, 8,
    /* clip */
    0, 65535
};

const vo_gfx_gmm g_GMM_OF_SDR_IN_HLG_OUT = {
    /* gmm step */
    0, 2, 4, 5, 6, 8, 10, 12,
    /* gmm pos */
    4, 32, 64, 192, 512, 4096, 20480, 65535,
    /* gmm num */
    4, 11, 13, 17, 22, 36, 52, 63
};

const vo_gfx_ghdr_dither g_GHDR_DITHER_OF_SDR_IN_HLG_OUT = {
    65535, 0,
    2147483647, 2147483647, 2147483647, 2147483647,
    2140000000, 2140000000, 2140000000, 2140000000
};

const vo_gfx_degmm g_DEGMM_OF_SDR_PREMULT = {
    /* degmm step */
    4, 4, 4, 5,
    /* degmm pos */
    672, 832, 992, 1023,
    /* degmm num */
    42, 10, 10, 1
};

const vo_gfx_ghdr_gamut g_GHDR_GAMUT_OF_SDR_PREMULT = {  // BT709 to BT2020
    /* gamut coef */
    10280, 5396, 709,
    1133, 15066, 185,
    269, 1442, 14672,
    /* scale, clip_min, clip_max */
    14, 0, 65535
};

const vo_gfx_tmap g_TMAP_OF_SDR_PREMULT = {
    /* tmap step */
    9, 10, 12, 13,
    /* tmap pos */
    8192, 16384, 24576, 65535,
    /* tmap num */
    16, 8, 2, 5
};

const vo_gfx_ghdr_tmap g_GHDR_TMAP_OF_SDR_PREMULT = {
    /* tmap luma coef */
    1076, 2777, 243,
    /* scale */
    12, 8,
    /* clip */
    0, 65535
};

const vo_gfx_gmm g_GMM_OF_SDR_PREMULT = {
    /* gmm step */
    0, 2, 4, 5, 6, 8, 10, 12,
    /* gmm pos */
    4, 32, 64, 192, 512, 4096, 20480, 65535,
    /* gmm num */
    4, 11, 13, 17, 22, 36, 52, 63
};

const vo_gfx_ghdr_dither g_GHDR_DITHER_OF_SDR_PREMULT = {
    65535, 0,
    2147483647, 2147483647, 2147483647, 2147483647,
    2140000000, 2140000000, 2140000000, 2140000000
};

const vo_gfx_degmm g_DEGMM_OF_SDR709_IN_2020_OUT = {
    /* degmm step */
    4, 4, 4, 5,
    /* degmm pos */
    672, 832, 992, 1023,
    /* degmm num */
    42, 10, 10, 1
};

const vo_gfx_ghdr_gamut g_GHDR_GAMUT_OF_SDR709_IN_2020_OUT = {
    /* gamut coef */
    27209, -9632, -1194,
    -2045, 18565, -138,
    -297, -1648, 18330,
    /* scale, clip_min, clip_max */
    14, 0, 65535
};

const vo_gfx_tmap g_TMAP_OF_SDR709_IN_2020_OUT = {
    /* tmap step */
    9, 10, 12, 13,
    /* tmap pos */
    8192, 16384, 24576, 65535,
    /* tmap num */
    16, 8, 2, 5
};

const vo_gfx_ghdr_tmap g_GHDR_TMAP_OF_SDR709_IN_2020_OUT = {
    /* tmap luma coef */
    1076, 2777, 243,
    /* scale */
    12, 8,
    /* clip */
    0, 65535
};

const vo_gfx_gmm g_GMM_OF_SDR709_IN_2020_OUT = {
    /* gmm step */
    0, 2, 4, 5, 6, 8, 10, 12,
    /* gmm pos */
    4, 32, 64, 192, 512, 4096, 20480, 65535,
    /* gmm num */
    4, 11, 13, 17, 22, 36, 52, 63
};

const vo_gfx_ghdr_dither g_GHDR_DITHER_OF_SDR709_IN_2020_OUT = {
    65535, 0,
    2147483647, 2147483647, 2147483647, 2147483647,
    2140000000, 2140000000, 2140000000, 2140000000
};

const vo_gfx_degmm g_DEGMM_OF_SDR_IN_DEF_OUT = {
    /* degmm step */
    4, 4, 4, 5,
    /* degmm pos */
    672, 832, 992, 1023,
    /* degmm num */
    42, 10, 10, 1
};

const vo_gfx_ghdr_gamut g_GHDR_GAMUT_OF_SDR_IN_DEF_OUT = {
    /* gamut coef */
    27209, -9632, -1194,
    -2045, 18565, -138,
    -297, -1648, 18330,
    /* scale, clip_min, clip_max */
    14, 0, 65535
};

const vo_gfx_tmap g_TMAP_OF_SDR_IN_DEF_OUT = {
    /* tmap step */
    9, 10, 12, 13,
    /* tmap pos */
    8192, 16384, 24576, 65535,
    /* tmap num */
    16, 8, 2, 5
};

const vo_gfx_ghdr_tmap g_GHDR_TMAP_OF_SDR_IN_DEF_OUT = {
    /* tmap luma coef */
    1076, 2777, 243,
    /* scale */
    12, 8,
    /* clip */
    0, 65535
};

const vo_gfx_gmm g_GMM_OF_SDR_IN_DEF_OUT = {
    /* gmm step */
    0, 2, 4, 5, 6, 8, 10, 12,
    /* gmm pos */
    4, 32, 64, 192, 512, 4096, 20480, 65535,
    /* gmm num */
    4, 11, 13, 17, 22, 36, 52, 63
};

const vo_gfx_ghdr_dither g_GHDR_DITHER_OF_SDR_IN_DEF_OUT = {
    65535, 0,
    2147483647, 2147483647, 2147483647, 2147483647,
    2140000000, 2140000000, 2140000000, 2140000000
};


