// ******************************************************************************
// Copyright     :  Copyright (C) 2017, Hisilicon Technologies Co., Ltd.
// File name     : isp_lut_config.h
// Author        :
// Version       :  1.0
// Date          :  2017-02-23
// Description   :  Define all registers/tables
// History       :  2017-02-23 Create file
// ******************************************************************************
#ifndef __ISP_LUT_CONFIG_H__
#define __ISP_LUT_CONFIG_H__

#include "hi_debug.h"
#include "hi_isp_debug.h"
#include "isp_vreg.h"
#include "isp_main.h"
#include "isp_regcfg.h"
#include "isp_lut_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

// ******************************************************************************
//  Function    : iSetISP_AE_WEIGHTisp_ae_weight2
//  Description : Set the value of the member ISP_AE_WEIGHT.isp_ae_weight2
//  Input       : HI_U32 *upisp_ae_weight2: 4 bits
// ******************************************************************************
static __inline HI_VOID isp_ae_weight_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_ae_weight)
{
    HI_S32 i;

    for (i = 0; i < 64; i++) {
        pstBeReg->stIspBeLut.ISP_AE_WEIGHT[i].u32 = upisp_ae_weight[i];
    }
}

static __inline HI_VOID isp_lsc_rgain_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_lsc_rgain)
{
    HI_S32 i;
    for (i = 0; i < 1089; i++) {
        pstBeReg->stIspBeLut.ISP_LSC_RGAIN[i].u32 = upisp_lsc_rgain[i];
    }
}

static __inline HI_VOID isp_lsc_grgain_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_lsc_grgain)
{
    HI_S32 i;
    for (i = 0; i < 1089; i++) {
        pstBeReg->stIspBeLut.ISP_LSC_GRGAIN[i].u32 = upisp_lsc_grgain[i];
    }
}

static __inline HI_VOID isp_lsc_bgain_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_lsc_bgain)
{
    HI_S32 i;
    for (i = 0; i < 1089; i++) {
        pstBeReg->stIspBeLut.ISP_LSC_BGAIN[i].u32 = upisp_lsc_bgain[i];
    }
}

static __inline HI_VOID isp_lsc_gbgain_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_lsc_gbgain)
{
    HI_S32 i;
    for (i = 0; i < 1089; i++) {
        pstBeReg->stIspBeLut.ISP_LSC_GBGAIN[i].u32 = upisp_lsc_gbgain[i];
    }
}

// ******************************************************************************
//  Function    : iSetISP_FPN_LINE_WLUTisp_fpn_line_wlut
//  Description : Set the value of the member ISP_FPN_LINE_WLUT.isp_fpn_line_wlut
//  Input       : HI_U32 *upisp_fpn_line_wlut: 32 bits
// ******************************************************************************
static __inline HI_U8 isp_fpn_line_wlut_write(VI_PIPE ViPipe, BLK_DEV BlkDev, HI_U32 *upisp_fpn_line_wlut)
{
    S_ISPBE_REGS_TYPE *pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, BlkDev);
    HI_S32 i;
    ISP_CHECK_NULLPTR(pstBeReg);
    for (i = 0; i < 4352; i++) {
        pstBeReg->stIspBeLut.ISP_FPN_LINE_WLUT[i].u32 = upisp_fpn_line_wlut[i];
    }

    return 1;
}

static __inline HI_VOID isp_dpc_bpt_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_dpc_bpt_wlut)
{
    HI_S32 i;

    for (i = 0; i < 4096; i++) {
        pstBeReg->stIspBeLut.ISP_DPC_BPT_WLUT[i].u32 = upisp_dpc_bpt_wlut[i];
    }
}


// ******************************************************************************
//  Function    : iSetISP_SHARPEN_MFGAINDisp_sharpen_mfgaind
//  Description : Set the value of the member ISP_SHARPEN_MFGAIND.isp_sharpen_mfgaind
//  Input       : HI_U32 *upisp_sharpen_mfgaind: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_sharpen_mfgaind_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_sharpen_mfgaind)
{
    U_ISP_SHARPEN_MFGAIND o_isp_sharpen_mfgaind;
    HI_S32 i;

    for (i = 0; i < 64; i++) {
        o_isp_sharpen_mfgaind.u32 = pstBeReg->stIspBeLut.ISP_SHARPEN_MFGAIND[i].u32;
        o_isp_sharpen_mfgaind.bits.isp_sharpen_mfgaind = upisp_sharpen_mfgaind[i];
        pstBeReg->stIspBeLut.ISP_SHARPEN_MFGAIND[i].u32 = o_isp_sharpen_mfgaind.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_SHARPEN_MFGAINUDisp_sharpen_mfgainud
//  Description : Set the value of the member ISP_SHARPEN_MFGAINUD.isp_sharpen_mfgainud
//  Input       : HI_U32 *upisp_sharpen_mfgainud: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_sharpen_mfgainud_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_sharpen_mfgainud)
{
    U_ISP_SHARPEN_MFGAINUD o_isp_sharpen_mfgainud;
    HI_S32 i;

    for (i = 0; i < 64; i++) {
        o_isp_sharpen_mfgainud.u32 = pstBeReg->stIspBeLut.ISP_SHARPEN_MFGAINUD[i].u32;
        o_isp_sharpen_mfgainud.bits.isp_sharpen_mfgainud = upisp_sharpen_mfgainud[i];
        pstBeReg->stIspBeLut.ISP_SHARPEN_MFGAINUD[i].u32 = o_isp_sharpen_mfgainud.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_SHARPEN_HFGAINDisp_sharpen_hfgaind
//  Description : Set the value of the member ISP_SHARPEN_HFGAIND.isp_sharpen_hfgaind
//  Input       : HI_U32 *upisp_sharpen_hfgaind: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_sharpen_hfgaind_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_sharpen_hfgaind)
{
    U_ISP_SHARPEN_HFGAIND o_isp_sharpen_hfgaind;
    HI_S32 i;

    for (i = 0; i < 64; i++) {
        o_isp_sharpen_hfgaind.u32 = pstBeReg->stIspBeLut.ISP_SHARPEN_HFGAIND[i].u32;
        o_isp_sharpen_hfgaind.bits.isp_sharpen_hfgaind = upisp_sharpen_hfgaind[i];
        pstBeReg->stIspBeLut.ISP_SHARPEN_HFGAIND[i].u32 = o_isp_sharpen_hfgaind.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_SHARPEN_HFGAINUDisp_sharpen_hfgainud
//  Description : Set the value of the member ISP_SHARPEN_HFGAINUD.isp_sharpen_hfgainud
//  Input       : HI_U32 *upisp_sharpen_hfgainud: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_sharpen_hfgainud_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_sharpen_hfgainud)
{
    U_ISP_SHARPEN_HFGAINUD o_isp_sharpen_hfgainud;
    HI_S32 i;

    for (i = 0; i < 64; i++) {
        o_isp_sharpen_hfgainud.u32 = pstBeReg->stIspBeLut.ISP_SHARPEN_HFGAINUD[i].u32;
        o_isp_sharpen_hfgainud.bits.isp_sharpen_hfgainud = upisp_sharpen_hfgainud[i];
        pstBeReg->stIspBeLut.ISP_SHARPEN_HFGAINUD[i].u32 = o_isp_sharpen_hfgainud.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_NDDM_GF_LUTisp_nddm_gflut
//  Description : Set the value of the member ISP_NDDM_GF_LUT.isp_nddm_gflut
//  Input       : HI_U32 *upisp_nddm_gflut: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_nddm_gflut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_nddm_gflut)
{
    U_ISP_NDDM_GF_LUT o_isp_nddm_gf_lut;
    HI_S32 i;

    for (i = 0; i < 17; i++) {
        o_isp_nddm_gf_lut.u32 = pstBeReg->stIspBeLut.ISP_NDDM_GF_LUT[i].u32;
        o_isp_nddm_gf_lut.bits.isp_nddm_gflut = upisp_nddm_gflut[i];
        pstBeReg->stIspBeLut.ISP_NDDM_GF_LUT[i].u32 = o_isp_nddm_gf_lut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_NDDM_USM_LUTisp_nddm_usmlut
//  Description : Set the value of the member ISP_NDDM_USM_LUT.isp_nddm_usmlut
//  Input       : HI_U32 *upisp_nddm_usmlut: 8 bits
// ******************************************************************************
static __inline HI_VOID isp_nddm_usmlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U8 *upisp_nddm_usmlut)
{
    U_ISP_NDDM_USM_LUT o_isp_nddm_usm_lut;
    HI_S32 i;

    for (i = 0; i < 17; i++) {
        o_isp_nddm_usm_lut.u32 = pstBeReg->stIspBeLut.ISP_NDDM_USM_LUT[i].u32;
        o_isp_nddm_usm_lut.bits.isp_nddm_usmlut = upisp_nddm_usmlut[i];
        pstBeReg->stIspBeLut.ISP_NDDM_USM_LUT[i].u32 = o_isp_nddm_usm_lut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_BNR_LMT_EVENisp_bnr_lmt_even
//  Description : Set the value of the member ISP_BNR_LMT_EVEN.isp_bnr_lmt_even
//  Input       : HI_U32 *upisp_bnr_lmt_even: 8 bits
// ******************************************************************************
static __inline HI_VOID isp_bnr_lmt_even_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U8 *upisp_bnr_lmt_even)
{
    U_ISP_BNR_LMT_EVEN o_isp_bnr_lmt_even;
    HI_S32 i;

    for (i = 0; i < 65; i++) {
        o_isp_bnr_lmt_even.u32 = pstBeReg->stIspBeLut.ISP_BNR_LMT_EVEN[i].u32;
        o_isp_bnr_lmt_even.bits.isp_bnr_lmt_even = upisp_bnr_lmt_even[2 * i];
        pstBeReg->stIspBeLut.ISP_BNR_LMT_EVEN[i].u32 = o_isp_bnr_lmt_even.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_BNR_LMT_ODDisp_bnr_lmt_odd
//  Description : Set the value of the member ISP_BNR_LMT_ODD.isp_bnr_lmt_odd
//  Input       : HI_U32 *upisp_bnr_lmt_odd: 8 bits
// ******************************************************************************
static __inline HI_VOID isp_bnr_lmt_odd_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U8 *upisp_bnr_lmt_odd)
{
    U_ISP_BNR_LMT_ODD o_isp_bnr_lmt_odd;
    HI_S32 i;

    for (i = 0; i < 64; i++) {
        o_isp_bnr_lmt_odd.u32 = pstBeReg->stIspBeLut.ISP_BNR_LMT_ODD[i].u32;
        o_isp_bnr_lmt_odd.bits.isp_bnr_lmt_odd = upisp_bnr_lmt_odd[2 * i + 1];
        pstBeReg->stIspBeLut.ISP_BNR_LMT_ODD[i].u32 = o_isp_bnr_lmt_odd.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_BNR_COR_EVENisp_bnr_cor_even
//  Description : Set the value of the member ISP_BNR_COR_EVEN.isp_bnr_cor_even
//  Input       : HI_U32 *upisp_bnr_cor_even: 14 bits
// ******************************************************************************
static __inline HI_VOID isp_bnr_cor_even_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_bnr_cor_even)
{
    U_ISP_BNR_COR_EVEN o_isp_bnr_cor_even;
    HI_S32 i;

    for (i = 0; i < 17; i++) {
        o_isp_bnr_cor_even.u32 = pstBeReg->stIspBeLut.ISP_BNR_COR_EVEN[i].u32;
        o_isp_bnr_cor_even.bits.isp_bnr_cor_even = upisp_bnr_cor_even[2 * i];
        pstBeReg->stIspBeLut.ISP_BNR_COR_EVEN[i].u32 = o_isp_bnr_cor_even.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_BNR_COR_ODDisp_bnr_cor_odd
//  Description : Set the value of the member ISP_BNR_COR_ODD.isp_bnr_cor_odd
//  Input       : HI_U32 *upisp_bnr_cor_odd: 14 bits
// ******************************************************************************
static __inline HI_VOID isp_bnr_cor_odd_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_bnr_cor_odd)
{
    U_ISP_BNR_COR_ODD o_isp_bnr_cor_odd;
    HI_S32 i;

    for (i = 0; i < 16; i++) {
        o_isp_bnr_cor_odd.u32 = pstBeReg->stIspBeLut.ISP_BNR_COR_ODD[i].u32;
        o_isp_bnr_cor_odd.bits.isp_bnr_cor_odd = upisp_bnr_cor_odd[2 * i + 1];
        pstBeReg->stIspBeLut.ISP_BNR_COR_ODD[i].u32 = o_isp_bnr_cor_odd.u32;
    }
}

static __inline HI_VOID isp_bnr_lsc_rgain_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_bnr_lsc_rgain)
{
    HI_S32 i;
    for (i = 0; i < 1089; i++) {
        pstBeReg->stIspBeLut.ISP_BNR_LSC_RGAIN[i].u32 = upisp_bnr_lsc_rgain[i];
    }
}

static __inline HI_VOID isp_bnr_lsc_grgain_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_bnr_lsc_grgain)
{
    HI_S32 i;
    for (i = 0; i < 1089; i++) {
        pstBeReg->stIspBeLut.ISP_BNR_LSC_GRGAIN[i].u32 = upisp_bnr_lsc_grgain[i];
    }
}

static __inline HI_VOID isp_bnr_lsc_bgain_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_bnr_lsc_bgain)
{
    HI_S32 i;
    for (i = 0; i < 1089; i++) {
        pstBeReg->stIspBeLut.ISP_BNR_LSC_BGAIN[i].u32 = upisp_bnr_lsc_bgain[i];
    }
}

static __inline HI_VOID isp_bnr_lsc_gbgain_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_bnr_lsc_gbgain)
{
    HI_S32 i;
    for (i = 0; i < 1089; i++) {
        pstBeReg->stIspBeLut.ISP_BNR_LSC_GBGAIN[i].u32 = upisp_bnr_lsc_gbgain[i];
    }
}

// ******************************************************************************
//  Function    : iSetISP_WDR_NOSLUT129X8isp_wdr_noslut129x8
//  Description : Set the value of the member ISP_WDR_NOSLUT129X8.isp_wdr_noslut129x8
//  Input       : HI_U32 *upisp_wdr_noslut129x8: 8 bits
// ******************************************************************************
static __inline HI_VOID isp_wdr_noslut129x8_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_S32 *upisp_wdr_noslut129x8)
{
    U_ISP_WDR_NOSLUT129X8 o_isp_wdr_noslut129x8;
    HI_S32 i;

    for (i = 0; i < 129; i++) {
        o_isp_wdr_noslut129x8.u32 = pstBeReg->stIspBeLut.ISP_WDR_NOSLUT129X8[i].u32;
        o_isp_wdr_noslut129x8.bits.isp_wdr_noslut129x8 = upisp_wdr_noslut129x8[i];
        pstBeReg->stIspBeLut.ISP_WDR_NOSLUT129X8[i].u32 = o_isp_wdr_noslut129x8.u32;
    }
}

static __inline HI_VOID isp_dehaze_prestat_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_dehaze_prestat)
{
    HI_S32 i;
    for (i = 0; i < 512; i++) {
        pstBeReg->stIspBeLut.ISP_DEHAZE_PRESTAT[i].u32 = upisp_dehaze_prestat[i];
    }
}


// ******************************************************************************
//  Function    : iSetISP_DEHAZE_LUTisp_dehaze_dehaze_lut
//  Description : Set the value of the member ISP_DEHAZE_LUT.isp_dehaze_dehaze_lut
//  Input       : HI_U32 *upisp_dehaze_dehaze_lut: 8 bits
// ******************************************************************************
static __inline HI_VOID isp_dehaze_dehaze_lut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U8 *upisp_dehaze_dehaze_lut)
{
    U_ISP_DEHAZE_LUT o_isp_dehaze_lut;
    HI_S32 i;

    for (i = 0; i < 256; i++) {
        o_isp_dehaze_lut.u32 = pstBeReg->stIspBeLut.ISP_DEHAZE_LUT[i].u32;
        o_isp_dehaze_lut.bits.isp_dehaze_dehaze_lut = upisp_dehaze_dehaze_lut[i];
        pstBeReg->stIspBeLut.ISP_DEHAZE_LUT[i].u32 = o_isp_dehaze_lut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_PREGAMMA_LUTisp_pregamma_lut
//  Description : Set the value of the member ISP_PREGAMMA_LUT.isp_pregamma_lut
//  Input       : HI_U32 *upisp_pregamma_lut: 21 bits
// ******************************************************************************
static __inline HI_VOID isp_pregamma_lut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_pregamma_lut)
{
    U_ISP_PREGAMMA_LUT o_isp_pregamma_lut;
    HI_S32 i;

    for (i = 0; i < 257; i++) {
        o_isp_pregamma_lut.u32 = pstBeReg->stIspBeLut.ISP_PREGAMMA_LUT[i].u32;
        o_isp_pregamma_lut.bits.isp_pregamma_lut = upisp_pregamma_lut[i];
        pstBeReg->stIspBeLut.ISP_PREGAMMA_LUT[i].u32 = o_isp_pregamma_lut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_GAMMA_LUTisp_gamma_lut
//  Description : Set the value of the member ISP_GAMMA_LUT.isp_gamma_lut
//  Input       : HI_U32 *upisp_gamma_lut: 14 bits
// ******************************************************************************
static __inline HI_VOID isp_gamma_lut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_gamma_lut)
{
    U_ISP_GAMMA_LUT o_isp_gamma_lut;
    HI_S32 i;

    for (i = 0; i < 1025; i++) {
        o_isp_gamma_lut.u32 = pstBeReg->stIspBeLut.ISP_GAMMA_LUT[i].u32;
        o_isp_gamma_lut.bits.isp_gamma_lut = upisp_gamma_lut[i] << 2;
        pstBeReg->stIspBeLut.ISP_GAMMA_LUT[i].u32 = o_isp_gamma_lut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_CA_LUTisp_ca_lut
//  Description : Set the value of the member ISP_CA_LUT.isp_ca_lut
//  Input       : HI_U32 *upisp_ca_lut: 24 bits
// ******************************************************************************
static __inline HI_VOID isp_ca_lut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_ca_lut)
{
    U_ISP_CA_LUT o_isp_ca_lut;
    HI_S32 i;

    for (i = 0; i < 256; i++) {
        o_isp_ca_lut.u32 = pstBeReg->stIspBeLut.ISP_CA_LUT[i].u32;
        o_isp_ca_lut.bits.isp_ca_lut = upisp_ca_lut[i];
        pstBeReg->stIspBeLut.ISP_CA_LUT[i].u32 = o_isp_ca_lut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_CLUT_LUT0_WLUTisp_clut_lut0
//  Description : Set the value of the member ISP_CLUT_LUT0_WLUT.isp_clut_lut0
//  Input       : HI_U32 *upisp_clut_lut0: 32 bits
// ******************************************************************************
static __inline HI_VOID isp_clut_lut0_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_clut_lut0)
{
    HI_S32 i;
    for (i = 0; i < 3087; i++) {
        pstBeReg->stIspBeLut.ISP_CLUT_LUT0_WLUT[i].u32 = upisp_clut_lut0[i];
    }
}


// ******************************************************************************
//  Function    : iSetISP_CLUT_LUT1_WLUTisp_clut_lut1
//  Description : Set the value of the member ISP_CLUT_LUT1_WLUT.isp_clut_lut1
//  Input       : HI_U32 *upisp_clut_lut1: 32 bits
// ******************************************************************************
static __inline HI_VOID isp_clut_lut1_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_clut_lut1)
{
    HI_S32 i;
    for (i = 0; i < 2871; i++) {
        pstBeReg->stIspBeLut.ISP_CLUT_LUT1_WLUT[i].u32 = upisp_clut_lut1[i];
    }
}


// ******************************************************************************
//  Function    : iSetISP_CLUT_LUT2_WLUTisp_clut_lut2
//  Description : Set the value of the member ISP_CLUT_LUT2_WLUT.isp_clut_lut2
//  Input       : HI_U32 *upisp_clut_lut2: 32 bits
// ******************************************************************************
static __inline HI_VOID isp_clut_lut2_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_clut_lut2)
{
    HI_S32 i;
    for (i = 0; i < 2871; i++) {
        pstBeReg->stIspBeLut.ISP_CLUT_LUT2_WLUT[i].u32 = upisp_clut_lut2[i];
    }
}


// ******************************************************************************
//  Function    : iSetISP_CLUT_LUT3_WLUTisp_clut_lut3
//  Description : Set the value of the member ISP_CLUT_LUT3_WLUT.isp_clut_lut3
//  Input       : HI_U32 *upisp_clut_lut3: 32 bits
// ******************************************************************************
static __inline HI_VOID isp_clut_lut3_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_clut_lut3)
{
    HI_S32 i;
    for (i = 0; i < 2664; i++) {
        pstBeReg->stIspBeLut.ISP_CLUT_LUT3_WLUT[i].u32 = upisp_clut_lut3[i];
    }
}


// ******************************************************************************
//  Function    : iSetISP_CLUT_LUT4_WLUTisp_clut_lut4
//  Description : Set the value of the member ISP_CLUT_LUT4_WLUT.isp_clut_lut4
//  Input       : HI_U32 *upisp_clut_lut4: 32 bits
// ******************************************************************************
static __inline HI_VOID isp_clut_lut4_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_clut_lut4)
{
    HI_S32 i;
    for (i = 0; i < 2871; i++) {
        pstBeReg->stIspBeLut.ISP_CLUT_LUT4_WLUT[i].u32 = upisp_clut_lut4[i];
    }
}


// ******************************************************************************
//  Function    : iSetISP_CLUT_LUT5_WLUTisp_clut_lut5
//  Description : Set the value of the member ISP_CLUT_LUT5_WLUT.isp_clut_lut5
//  Input       : HI_U32 *upisp_clut_lut5: 32 bits
// ******************************************************************************
static __inline HI_VOID isp_clut_lut5_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_clut_lut5)
{
    HI_S32 i;
    for (i = 0; i < 2664; i++) {
        pstBeReg->stIspBeLut.ISP_CLUT_LUT5_WLUT[i].u32 = upisp_clut_lut5[i];
    }
}


// ******************************************************************************
//  Function    : iSetISP_CLUT_LUT6_WLUTisp_clut_lut6
//  Description : Set the value of the member ISP_CLUT_LUT6_WLUT.isp_clut_lut6
//  Input       : HI_U32 *upisp_clut_lut6: 32 bits
// ******************************************************************************
static __inline HI_VOID isp_clut_lut6_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_clut_lut6)
{
    HI_S32 i;
    for (i = 0; i < 2664; i++) {
        pstBeReg->stIspBeLut.ISP_CLUT_LUT6_WLUT[i].u32 = upisp_clut_lut6[i];
    }
}


// ******************************************************************************
//  Function    : iSetISP_CLUT_LUT7_WLUTisp_clut_lut7
//  Description : Set the value of the member ISP_CLUT_LUT7_WLUT.isp_clut_lut7
//  Input       : HI_U32 *upisp_clut_lut7: 32 bits
// ******************************************************************************
static __inline HI_VOID isp_clut_lut7_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_clut_lut7)
{
    HI_S32 i;
    for (i = 0; i < 2475; i++) {
        pstBeReg->stIspBeLut.ISP_CLUT_LUT7_WLUT[i].u32 = upisp_clut_lut7[i];
    }
}


// ******************************************************************************
//  Function    : iSetISP_LDCI_DRC_WLUTisp_ldci_calcdrc_wlut
//  Description : Set the value of the member ISP_LDCI_DRC_WLUT.isp_ldci_calcdrc_wlut
//  Input       : HI_U32 *upisp_ldci_calcdrc_wlut: 10 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_calcdrc_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_S16 *upisp_ldci_calcdrc_wlut)
{
    U_ISP_LDCI_DRC_WLUT o_isp_ldci_drc_wlut;
    HI_S32 i;
    for (i = 0; i < 65; i++) {
        o_isp_ldci_drc_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_DRC_WLUT[i].u32;
        o_isp_ldci_drc_wlut.bits.isp_ldci_calcdrc_wlut = upisp_ldci_calcdrc_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_DRC_WLUT[i].u32 = o_isp_ldci_drc_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_DRC_WLUTisp_ldci_statdrc_wlut
//  Description : Set the value of the member ISP_LDCI_DRC_WLUT.isp_ldci_statdrc_wlut
//  Input       : HI_U32 *upisp_ldci_statdrc_wlut: 10 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_statdrc_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_S16 *upisp_ldci_statdrc_wlut)
{
    U_ISP_LDCI_DRC_WLUT o_isp_ldci_drc_wlut;
    HI_S32 i;
    for (i = 0; i < 65; i++) {
        o_isp_ldci_drc_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_DRC_WLUT[i].u32;
        o_isp_ldci_drc_wlut.bits.isp_ldci_statdrc_wlut = upisp_ldci_statdrc_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_DRC_WLUT[i].u32 = o_isp_ldci_drc_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_HE_WLUTisp_ldci_hepos_wlut
//  Description : Set the value of the member ISP_LDCI_HE_WLUT.isp_ldci_hepos_wlut
//  Input       : HI_U32 *upisp_ldci_hepos_wlut: 7 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_hepos_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_ldci_hepos_wlut)
{
    U_ISP_LDCI_HE_WLUT o_isp_ldci_he_wlut;
    HI_S32 i;

    for (i = 0; i < 33; i++) {
        o_isp_ldci_he_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_HE_WLUT[i].u32;
        o_isp_ldci_he_wlut.bits.isp_ldci_hepos_wlut = upisp_ldci_hepos_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_HE_WLUT[i].u32 = o_isp_ldci_he_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_HE_WLUTisp_ldci_heneg_wlut
//  Description : Set the value of the member ISP_LDCI_HE_WLUT.isp_ldci_heneg_wlut
//  Input       : HI_U32 *upisp_ldci_heneg_wlut: 7 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_heneg_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_ldci_heneg_wlut)
{
    U_ISP_LDCI_HE_WLUT o_isp_ldci_he_wlut;
    HI_S32 i;

    for (i = 0; i < 33; i++) {
        o_isp_ldci_he_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_HE_WLUT[i].u32;
        o_isp_ldci_he_wlut.bits.isp_ldci_heneg_wlut = upisp_ldci_heneg_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_HE_WLUT[i].u32 = o_isp_ldci_he_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_DE_USM_WLUTisp_ldci_usmpos_wlut
//  Description : Set the value of the member ISP_LDCI_DE_USM_WLUT.isp_ldci_usmpos_wlut
//  Input       : HI_U32 *upisp_ldci_usmpos_wlut: 9 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_usmpos_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_ldci_usmpos_wlut)
{
    U_ISP_LDCI_DE_USM_WLUT o_isp_ldci_de_usm_wlut;
    HI_S32 i;

    for (i = 0; i < 33; i++) {
        o_isp_ldci_de_usm_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_DE_USM_WLUT[i].u32;
        o_isp_ldci_de_usm_wlut.bits.isp_ldci_usmpos_wlut = upisp_ldci_usmpos_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_DE_USM_WLUT[i].u32 = o_isp_ldci_de_usm_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_DE_USM_WLUTisp_ldci_usmneg_wlut
//  Description : Set the value of the member ISP_LDCI_DE_USM_WLUT.isp_ldci_usmneg_wlut
//  Input       : HI_U32 *upisp_ldci_usmneg_wlut: 9 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_usmneg_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_ldci_usmneg_wlut)
{
    U_ISP_LDCI_DE_USM_WLUT o_isp_ldci_de_usm_wlut;
    HI_S32 i;

    for (i = 0; i < 33; i++) {
        o_isp_ldci_de_usm_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_DE_USM_WLUT[i].u32;
        o_isp_ldci_de_usm_wlut.bits.isp_ldci_usmneg_wlut = upisp_ldci_usmneg_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_DE_USM_WLUT[i].u32 = o_isp_ldci_de_usm_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_DE_USM_WLUTisp_ldci_delut_wlut
//  Description : Set the value of the member ISP_LDCI_DE_USM_WLUT.isp_ldci_delut_wlut
//  Input       : HI_U32 *upisp_ldci_delut_wlut: 9 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_delut_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_ldci_delut_wlut)
{
    U_ISP_LDCI_DE_USM_WLUT o_isp_ldci_de_usm_wlut;
    HI_S32 i;

    for (i = 0; i < 33; i++) {
        o_isp_ldci_de_usm_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_DE_USM_WLUT[i].u32;
        o_isp_ldci_de_usm_wlut.bits.isp_ldci_delut_wlut = upisp_ldci_delut_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_DE_USM_WLUT[i].u32 = o_isp_ldci_de_usm_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_CGAIN_WLUTisp_ldci_cgain_wlut
//  Description : Set the value of the member ISP_LDCI_CGAIN_WLUT.isp_ldci_cgain_wlut
//  Input       : HI_U32 *upisp_ldci_cgain_wlut: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_cgain_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_ldci_cgain_wlut)
{
    U_ISP_LDCI_CGAIN_WLUT o_isp_ldci_cgain_wlut;
    HI_S32 i;

    for (i = 0; i < 65; i++) {
        o_isp_ldci_cgain_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_CGAIN_WLUT[i].u32;
        o_isp_ldci_cgain_wlut.bits.isp_ldci_cgain_wlut = upisp_ldci_cgain_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_CGAIN_WLUT[i].u32 = o_isp_ldci_cgain_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_POLYP_WLUTisp_ldci_poply1_wlut
//  Description : Set the value of the member ISP_LDCI_POLYP_WLUT.isp_ldci_poply1_wlut
//  Input       : HI_U32 *upisp_ldci_poply1_wlut: 10 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_poply1_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_S16 *upisp_ldci_poply1_wlut)
{
    U_ISP_LDCI_POLYP_WLUT o_isp_ldci_polyp_wlut;
    HI_S32 i;

    for (i = 0; i < 65; i++) {
        o_isp_ldci_polyp_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32;
        o_isp_ldci_polyp_wlut.bits.isp_ldci_poply1_wlut = upisp_ldci_poply1_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32 = o_isp_ldci_polyp_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_POLYP_WLUTisp_ldci_poply2_wlut
//  Description : Set the value of the member ISP_LDCI_POLYP_WLUT.isp_ldci_poply2_wlut
//  Input       : HI_U32 *upisp_ldci_poply2_wlut: 10 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_poply2_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_S16 *upisp_ldci_poply2_wlut)
{
    U_ISP_LDCI_POLYP_WLUT o_isp_ldci_polyp_wlut;
    HI_S32 i;

    for (i = 0; i < 65; i++) {
        o_isp_ldci_polyp_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32;
        o_isp_ldci_polyp_wlut.bits.isp_ldci_poply2_wlut = upisp_ldci_poply2_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32 = o_isp_ldci_polyp_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_POLYP_WLUTisp_ldci_poply3_wlut
//  Description : Set the value of the member ISP_LDCI_POLYP_WLUT.isp_ldci_poply3_wlut
//  Input       : HI_U32 *upisp_ldci_poply3_wlut: 10 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_poply3_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_S16 *upisp_ldci_poply3_wlut)
{
    U_ISP_LDCI_POLYP_WLUT o_isp_ldci_polyp_wlut;
    HI_S32 i;

    for (i = 0; i < 65; i++) {
        o_isp_ldci_polyp_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32;
        o_isp_ldci_polyp_wlut.bits.isp_ldci_poply3_wlut = upisp_ldci_poply3_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_POLYP_WLUT[i].u32 = o_isp_ldci_polyp_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_POLYQ01_WLUTisp_ldci_plyq0_wlut
//  Description : Set the value of the member ISP_LDCI_POLYQ01_WLUT.isp_ldci_plyq0_wlut
//  Input       : HI_U32 *upisp_ldci_plyq0_wlut: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_plyq0_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_S16 *upisp_ldci_plyq0_wlut)
{
    U_ISP_LDCI_POLYQ01_WLUT o_isp_ldci_polyq01_wlut;
    HI_S32 i;

    for (i = 0; i < 65; i++) {
        o_isp_ldci_polyq01_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_POLYQ01_WLUT[i].u32;
        o_isp_ldci_polyq01_wlut.bits.isp_ldci_plyq0_wlut = upisp_ldci_plyq0_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_POLYQ01_WLUT[i].u32 = o_isp_ldci_polyq01_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_POLYQ01_WLUTisp_ldci_plyq1_wlut
//  Description : Set the value of the member ISP_LDCI_POLYQ01_WLUT.isp_ldci_plyq1_wlut
//  Input       : HI_U32 *upisp_ldci_plyq1_wlut: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_plyq1_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_S16 *upisp_ldci_plyq1_wlut)
{
    U_ISP_LDCI_POLYQ01_WLUT o_isp_ldci_polyq01_wlut;
    HI_S32 i;

    for (i = 0; i < 65; i++) {
        o_isp_ldci_polyq01_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_POLYQ01_WLUT[i].u32;
        o_isp_ldci_polyq01_wlut.bits.isp_ldci_plyq1_wlut = upisp_ldci_plyq1_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_POLYQ01_WLUT[i].u32 = o_isp_ldci_polyq01_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_POLYQ23_WLUTisp_ldci_plyq2_wlut
//  Description : Set the value of the member ISP_LDCI_POLYQ23_WLUT.isp_ldci_plyq2_wlut
//  Input       : HI_U32 *upisp_ldci_plyq2_wlut: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_plyq2_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_S16 *upisp_ldci_plyq2_wlut)
{
    U_ISP_LDCI_POLYQ23_WLUT o_isp_ldci_polyq23_wlut;
    HI_S32 i;

    for (i = 0; i < 65; i++) {
        o_isp_ldci_polyq23_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_POLYQ23_WLUT[i].u32;
        o_isp_ldci_polyq23_wlut.bits.isp_ldci_plyq2_wlut = upisp_ldci_plyq2_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_POLYQ23_WLUT[i].u32 = o_isp_ldci_polyq23_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LDCI_POLYQ23_WLUTisp_ldci_plyq3_wlut
//  Description : Set the value of the member ISP_LDCI_POLYQ23_WLUT.isp_ldci_plyq3_wlut
//  Input       : HI_U32 *upisp_ldci_plyq3_wlut: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_ldci_plyq3_wlut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_S16 *upisp_ldci_plyq3_wlut)
{
    U_ISP_LDCI_POLYQ23_WLUT o_isp_ldci_polyq23_wlut;
    HI_S32 i;

    for (i = 0; i < 65; i++) {
        o_isp_ldci_polyq23_wlut.u32 = pstBeReg->stIspBeLut.ISP_LDCI_POLYQ23_WLUT[i].u32;
        o_isp_ldci_polyq23_wlut.bits.isp_ldci_plyq3_wlut = upisp_ldci_plyq3_wlut[i];
        pstBeReg->stIspBeLut.ISP_LDCI_POLYQ23_WLUT[i].u32 = o_isp_ldci_polyq23_wlut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_DRC_TMLUT0isp_drc_tmlut0_diff
//  Description : Set the value of the member ISP_DRC_TMLUT0.isp_drc_tmlut0_diff
//  Input       : HI_U32 *upisp_drc_tmlut0_diff: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_drc_tmlut0_diff_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_drc_tmlut0_diff)
{
    U_ISP_DRC_TMLUT0 o_isp_drc_tmlut0;
    HI_S32 i;

    for (i = 0; i < 200; i++) {
        o_isp_drc_tmlut0.u32 = pstBeReg->stIspBeLut.ISP_DRC_TMLUT0[i].u32;
        o_isp_drc_tmlut0.bits.isp_drc_tmlut0_diff = upisp_drc_tmlut0_diff[i];
        pstBeReg->stIspBeLut.ISP_DRC_TMLUT0[i].u32 = o_isp_drc_tmlut0.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_DRC_TMLUT0isp_drc_tmlut0_value
//  Description : Set the value of the member ISP_DRC_TMLUT0.isp_drc_tmlut0_value
//  Input       : HI_U32 *upisp_drc_tmlut0_value: 16 bits
// ******************************************************************************
static __inline HI_VOID isp_drc_tmlut0_value_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_drc_tmlut0_value)
{
    U_ISP_DRC_TMLUT0 o_isp_drc_tmlut0;
    HI_S32 i;

    for (i = 0; i < 200; i++) {
        o_isp_drc_tmlut0.u32 = pstBeReg->stIspBeLut.ISP_DRC_TMLUT0[i].u32;
        o_isp_drc_tmlut0.bits.isp_drc_tmlut0_value = upisp_drc_tmlut0_value[i];
        pstBeReg->stIspBeLut.ISP_DRC_TMLUT0[i].u32 = o_isp_drc_tmlut0.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_DRC_TMLUT1isp_drc_tmlut1_diff
//  Description : Set the value of the member ISP_DRC_TMLUT1.isp_drc_tmlut1_diff
//  Input       : HI_U32 *upisp_drc_tmlut1_diff: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_drc_tmlut1_diff_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_drc_tmlut1_diff)
{
    U_ISP_DRC_TMLUT1 o_isp_drc_tmlut1;
    HI_S32 i;

    for (i = 0; i < 200; i++) {
        o_isp_drc_tmlut1.u32 = pstBeReg->stIspBeLut.ISP_DRC_TMLUT1[i].u32;
        o_isp_drc_tmlut1.bits.isp_drc_tmlut1_diff = upisp_drc_tmlut1_diff[i];
        pstBeReg->stIspBeLut.ISP_DRC_TMLUT1[i].u32 = o_isp_drc_tmlut1.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_DRC_TMLUT1isp_drc_tmlut1_value
//  Description : Set the value of the member ISP_DRC_TMLUT1.isp_drc_tmlut1_value
//  Input       : HI_U32 *upisp_drc_tmlut1_value: 16 bits
// ******************************************************************************
static __inline HI_VOID isp_drc_tmlut1_value_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_drc_tmlut1_value)
{
    U_ISP_DRC_TMLUT1 o_isp_drc_tmlut1;
    HI_S32 i;

    for (i = 0; i < 200; i++) {
        o_isp_drc_tmlut1.u32 = pstBeReg->stIspBeLut.ISP_DRC_TMLUT1[i].u32;
        o_isp_drc_tmlut1.bits.isp_drc_tmlut1_value = upisp_drc_tmlut1_value[i];
        pstBeReg->stIspBeLut.ISP_DRC_TMLUT1[i].u32 = o_isp_drc_tmlut1.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_DRC_CCLUTisp_drc_cclut
//  Description : Set the value of the member ISP_DRC_CCLUT.isp_drc_cclut
//  Input       : HI_U32 *upisp_drc_cclut: 12 bits
// ******************************************************************************
static __inline HI_VOID isp_drc_cclut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_drc_cclut)
{
    U_ISP_DRC_CCLUT o_isp_drc_cclut;
    HI_S32 i;

    for (i = 0; i < 33; i++) {
        o_isp_drc_cclut.u32 = pstBeReg->stIspBeLut.ISP_DRC_CCLUT[i].u32;
        o_isp_drc_cclut.bits.isp_drc_cclut = upisp_drc_cclut[i];
        pstBeReg->stIspBeLut.ISP_DRC_CCLUT[i].u32 = o_isp_drc_cclut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_WDRSPLIT_LUTisp_wdrsplit_lut
//  Description : Set the value of the member ISP_WDRSPLIT_LUT.isp_wdrsplit_lut
//  Input       : HI_U32 *upisp_wdrsplit_lut: 16 bits
// ******************************************************************************
static __inline HI_VOID isp_wdrsplit_lut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *upisp_wdrsplit_lut)
{
    U_ISP_WDRSPLIT_LUT o_isp_wdrsplit_lut;
    HI_S32 i;

    for (i = 0; i < 129; i++) {
        o_isp_wdrsplit_lut.u32 = pstBeReg->stIspBeLut.ISP_WDRSPLIT_LUT[i].u32;
        o_isp_wdrsplit_lut.bits.isp_wdrsplit_lut = upisp_wdrsplit_lut[i];
        pstBeReg->stIspBeLut.ISP_WDRSPLIT_LUT[i].u32 = o_isp_wdrsplit_lut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_LOGLUT_LUTisp_loglut_lut
//  Description : Set the value of the member ISP_LOGLUT_LUT.isp_loglut_lut
//  Input       : HI_U32 *upisp_loglut_lut: 21 bits
// ******************************************************************************
static __inline HI_VOID isp_loglut_lut_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_loglut_lut)
{
    U_ISP_LOGLUT_LUT o_isp_loglut_lut;
    HI_S32 i;

    for (i = 0; i < 1025; i++) {
        o_isp_loglut_lut.u32 = pstBeReg->stIspBeLut.ISP_LOGLUT_LUT[i].u32;
        o_isp_loglut_lut.bits.isp_loglut_lut = upisp_loglut_lut[i];
        pstBeReg->stIspBeLut.ISP_LOGLUT_LUT[i].u32 = o_isp_loglut_lut.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_RLSC_LUT0isp_rlsc_lut0_ch0
//  Description : Set the value of the member ISP_RLSC_LUT0.isp_rlsc_lut0_ch0
//  Input       : HI_U32 *upisp_rlsc_lut0_ch0: 16 bits
// ******************************************************************************
static __inline HI_VOID isp_rlsc_lut0_ch0_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_rlsc_lut0_ch0)
{
    U_ISP_RLSC_LUT0 o_isp_rlsc_lut0;
    HI_S32 i;
    for (i = 0; i < 130; i++) {
        o_isp_rlsc_lut0.u32 = pstBeReg->stIspBeLut.ISP_RLSC_LUT0[i].u32;
        o_isp_rlsc_lut0.bits.isp_rlsc_lut0_ch0 = upisp_rlsc_lut0_ch0[i];
        pstBeReg->stIspBeLut.ISP_RLSC_LUT0[i].u32 = o_isp_rlsc_lut0.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_RLSC_LUT0isp_rlsc_lut0_ch1
//  Description : Set the value of the member ISP_RLSC_LUT0.isp_rlsc_lut0_ch1
//  Input       : HI_U32 *upisp_rlsc_lut0_ch1: 16 bits
// ******************************************************************************
static __inline HI_VOID isp_rlsc_lut0_ch1_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_rlsc_lut0_ch1)
{
    U_ISP_RLSC_LUT0 o_isp_rlsc_lut0;
    HI_S32 i;
    for (i = 0; i < 130; i++) {
        o_isp_rlsc_lut0.u32 = pstBeReg->stIspBeLut.ISP_RLSC_LUT0[i].u32;
        o_isp_rlsc_lut0.bits.isp_rlsc_lut0_ch1 = upisp_rlsc_lut0_ch1[i];
        pstBeReg->stIspBeLut.ISP_RLSC_LUT0[i].u32 = o_isp_rlsc_lut0.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_RLSC_LUT1isp_rlsc_lut1_ch2
//  Description : Set the value of the member ISP_RLSC_LUT1.isp_rlsc_lut1_ch2
//  Input       : HI_U32 *upisp_rlsc_lut1_ch2: 16 bits
// ******************************************************************************
static __inline HI_VOID isp_rlsc_lut1_ch2_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_rlsc_lut1_ch2)
{
    U_ISP_RLSC_LUT1 o_isp_rlsc_lut1;
    HI_S32 i;
    for (i = 0; i < 130; i++) {
        o_isp_rlsc_lut1.u32 = pstBeReg->stIspBeLut.ISP_RLSC_LUT1[i].u32;
        o_isp_rlsc_lut1.bits.isp_rlsc_lut1_ch2 = upisp_rlsc_lut1_ch2[i];
        pstBeReg->stIspBeLut.ISP_RLSC_LUT1[i].u32 = o_isp_rlsc_lut1.u32;
    }
}

// ******************************************************************************
//  Function    : iSetISP_RLSC_LUT1isp_rlsc_lut1_ch3
//  Description : Set the value of the member ISP_RLSC_LUT1.isp_rlsc_lut1_ch3
//  Input       : HI_U32 *upisp_rlsc_lut1_ch3: 16 bits
// ******************************************************************************
static __inline HI_VOID isp_rlsc_lut1_ch3_write(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *upisp_rlsc_lut1_ch3)
{
    U_ISP_RLSC_LUT1 o_isp_rlsc_lut1;
    HI_S32 i;
    for (i = 0; i < 130; i++) {
        o_isp_rlsc_lut1.u32 = pstBeReg->stIspBeLut.ISP_RLSC_LUT1[i].u32;
        o_isp_rlsc_lut1.bits.isp_rlsc_lut1_ch3 = upisp_rlsc_lut1_ch3[i];
        pstBeReg->stIspBeLut.ISP_RLSC_LUT1[i].u32 = o_isp_rlsc_lut1.u32;
    }
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
