/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#ifndef __HIFB_COEF_H__
#define __HIFB_COEF_H__

#include "hi_type.h"
#include "hi_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
    notice:
    in register ratio = input/output
    in algorithm ratio = output/input   (HERE USE)
*/
typedef enum hiVOU_ZOOM_COEF_E {
    VOU_ZOOM_COEF_1 = 0,
    VOU_ZOOM_COEF_EQU_1,
    VOU_ZOOM_COEF_075,
    VOU_ZOOM_COEF_05,
    VOU_ZOOM_COEF_033,
    VOU_ZOOM_COEF_025,
    VOU_ZOOM_COEF_0,
    VOU_ZOOM_COEF_BUTT
} VOU_ZOOM_COEF_E;

typedef enum hiVOU_ZOOM_TAP_E {
    VOU_ZOOM_TAP_8LH = 0,
    VOU_ZOOM_TAP_4CH,
    VOU_ZOOM_TAP_6LV,
    VOU_ZOOM_TAP_4LV,
    VOU_ZOOM_TAP_4CV,
    // VOU_ZOOM_TAP_2CV ,
    VOU_ZOOM_TAP_BUTT
} VOU_ZOOM_TAP_E;

typedef struct hiVO_ZOOM_BIT_S {
    HI_S32 bits_0 : 10;
    HI_S32 bits_1 : 10;
    HI_S32 bits_2 : 10;
    HI_S32 bits_32 : 2;
    HI_S32 bits_38 : 8;
    HI_S32 bits_4 : 10;
    HI_S32 bits_5 : 10;
    HI_S32 bits_64 : 4;
    HI_S32 bits_66 : 6;
    HI_S32 bits_7 : 10;
    HI_S32 bits_8 : 10;
    HI_S32 bits_96 : 6;
    HI_S32 bits_94 : 4;
    HI_S32 bits_10 : 10;
    HI_S32 bits_11 : 10;
    HI_S32 bits_12 : 8;
} VO_ZOOM_BIT_S;

typedef struct hiVO_ZOOM_BITARRAY_S {
    HI_U32 u32Size;
    VO_ZOOM_BIT_S stBit[12];
} VO_ZOOM_BITARRAY_S;

/*************************************
 *  COLOR SPACE CONVERT DEFINITION   *
 *************************************/
typedef struct {
    // for old version csc
    HI_S32 csc_coef00;
    HI_S32 csc_coef01;
    HI_S32 csc_coef02;

    HI_S32 csc_coef10;
    HI_S32 csc_coef11;
    HI_S32 csc_coef12;

    HI_S32 csc_coef20;
    HI_S32 csc_coef21;
    HI_S32 csc_coef22;

    HI_S32 csc_in_dc0;
    HI_S32 csc_in_dc1;
    HI_S32 csc_in_dc2;

    HI_S32 csc_out_dc0;
    HI_S32 csc_out_dc1;
    HI_S32 csc_out_dc2;

    // for new version csc
    HI_S32 new_csc_coef00;
    HI_S32 new_csc_coef01;
    HI_S32 new_csc_coef02;
    HI_S32 new_csc_coef03;
    HI_S32 new_csc_coef04;
    HI_S32 new_csc_coef05;
    HI_S32 new_csc_coef06;
    HI_S32 new_csc_coef07;
    HI_S32 new_csc_coef08;

    HI_S32 new_csc_coef10;
    HI_S32 new_csc_coef11;
    HI_S32 new_csc_coef12;
    HI_S32 new_csc_coef13;
    HI_S32 new_csc_coef14;
    HI_S32 new_csc_coef15;
    HI_S32 new_csc_coef16;
    HI_S32 new_csc_coef17;
    HI_S32 new_csc_coef18;

    HI_S32 new_csc_scale2p;

    HI_S32 new_csc_in_dc00;
    HI_S32 new_csc_in_dc01;
    HI_S32 new_csc_in_dc02;

    HI_S32 new_csc_out_dc00;
    HI_S32 new_csc_out_dc01;
    HI_S32 new_csc_out_dc02;

    HI_S32 new_csc_in_dc10;
    HI_S32 new_csc_in_dc11;
    HI_S32 new_csc_in_dc12;

    HI_S32 new_csc_out_dc10;
    HI_S32 new_csc_out_dc11;
    HI_S32 new_csc_out_dc12;

    HI_S32 new_csc_clip_min;
    HI_S32 new_csc_clip_max;

} CscCoef_S;

typedef struct {
    HI_S32 csc_scale2p;
    HI_S32 csc_clip_min;
    HI_S32 csc_clip_max;
} CscCoefParam_S;

/*************************************
 * Vga Sharpen HF Coefficient  *
 *************************************/
typedef struct {
    HI_S32 vga_hsp_tmp0;
    HI_S32 vga_hsp_tmp1;
    HI_S32 vga_hsp_tmp2;
    HI_S32 vga_hsp_tmp3;
    HI_U32 vga_hsp_coring;
    HI_S32 vga_hsp_gainneg;
    HI_S32 vga_hsp_gainpos;
    HI_S32 vga_hsp_adpshooten;
    HI_U32 vga_hsp_winsize;
    HI_U32 vga_hsp_mixratio;
    HI_U32 vga_hsp_underth;
    HI_U32 vga_hsp_overth;
} HspHfCoef_S;

typedef struct {
    HI_U32 vga_hsp_hf_shootdiv;
    HI_U32 vga_hsp_lti_ratio;
    HI_U32 vga_hsp_ldti_gain;
    HI_U32 vga_hsp_cdti_gain;
    HI_U32 vga_hsp_peak_ratio;
    HI_U32 vga_hsp_glb_overth;
    HI_U32 vga_hsp_glb_unferth;
} HspCoef_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

