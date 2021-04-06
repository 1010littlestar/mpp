/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#ifndef __HIFB_HAL_H__
#define __HIFB_HAL_H__

#include "hifb.h"
#include "hifb_reg.h"
#include "hifb_def.h"
#include "hifb_coef_org.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef HI_U32 VO_INTF_TYPE_E;

HI_VOID HAL_VOU_Init(HI_VOID);
HI_VOID HAL_VOU_Exit(HI_VOID);

HI_VOID HAL_WriteReg(HI_U32 *pAddress, HI_U32 Value);
HI_U32 HAL_ReadReg(HI_U32 *pAddress);

/*****************************************************************************
 Prototype       : sys Relative
 Description     :

*****************************************************************************/
/*****************************************************************************
 Prototype       : device Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_DISP_GetIntfEnable(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL *pbIntfEn);
HI_BOOL HAL_DISP_GetIntfSync(HAL_DISP_OUTPUTCHANNEL_E enChan,
                             HAL_DISP_SYNCINFO_S *pstSyncInfo);
HI_BOOL HAL_DISP_GetDispIoP(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL *pbIop);
HI_BOOL HAL_DISP_GetIntfMuxSel(HAL_DISP_OUTPUTCHANNEL_E enChan, VO_INTF_TYPE_E *pbenIntfType);
HI_BOOL HAL_DISP_GetVtThdMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL *pbFieldMode);

HI_BOOL HAL_DISP_SetIntMask(HI_U32 u32MaskEn);
HI_BOOL HAL_DISP_ClrIntMask(HI_U32 u32MaskEn);
HI_U32 HAL_DISP_GetIntStatus(HI_U32 u32IntMsk);
HI_BOOL HAL_DISP_ClearIntStatus(HI_U32 u32IntMsk);

/*****************************************************************************
 Prototype       : video layer Relative
 Description     :

*****************************************************************************/

HI_BOOL HAL_VIDEO_SetLayerDispRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect);
HI_BOOL HAL_VIDEO_SetLayerVideoRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect);

HI_VOID HAL_HIHDR_G_SetHihdrR2yEn(HI_U32 hihdr_r2y_en);
HI_BOOL HAL_GRAPHIC_SetGfxDcmpEnable(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_VOID HAL_FDR_GFX_SetDcmpEn(HI_U32 u32Data, HI_U32 dcmp_en);
HI_BOOL HAL_GRAPHIC_GetGfxDcmpEnableState(HAL_DISP_LAYER_E enLayer, HI_BOOL *pbEnable);
HI_BOOL HAL_GRAPHIC_SetGfxDcmpAddr(HAL_DISP_LAYER_E enLayer, HI_U64 addr_AR, HI_U64 addr_GB);

HI_BOOL HAL_LAYER_SetSrcResolution(HAL_DISP_LAYER_E enLayer,
                                   HIFB_RECT *pstRect);

HI_VOID HAL_FDR_GFX_SetSourceMode(HI_U32 u32Data, HI_U32 source_mode);

HI_VOID HAL_FDR_GFX_SetCmpMode(HI_U32 u32Data, HI_U32 cmp_mode);
HI_VOID HAL_FDR_GFX_SetIsLosslessA(HI_U32 u32Data, HI_U32 is_lossless_a);
HI_VOID HAL_FDR_GFX_SetIsLossless(HI_U32 u32Data, HI_U32 is_lossless);
HI_VOID HAL_FDR_GFX_SetFrameHeight(HI_U32 u32Data, HI_U32 frame_height);
HI_VOID HAL_FDR_GFX_SetFrameWidth(HI_U32 u32Data, HI_U32 frame_width);

HI_VOID HAL_FDR_GFX_SetFrameSize0(HI_U32 u32Data, HI_U32 frame_size0);
HI_VOID HAL_FDR_GFX_SetFrameSize1(HI_U32 u32Data, HI_U32 frame_size1);

/*****************************************************************************
 Prototype       : layer Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_LAYER_EnableLayer(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_BOOL HAL_LAYER_SetLayerDataFmt(HAL_DISP_LAYER_E enLayer,
                                  HAL_DISP_PIXEL_FORMAT_E enDataFmt);
HI_BOOL HAL_LAYER_GetLayerDataFmt(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32Fmt);
HI_BOOL HAL_LAYER_SetCscCoef(HAL_DISP_LAYER_E enLayer, CscCoef_S *pstCscCoef);
HI_BOOL HAL_LAYER_SetCscMode(HAL_DISP_LAYER_E enLayer, HI_BOOL bIsHCModeBy709);
HI_BOOL HAL_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn);
HI_BOOL HAL_LAYER_SetLayerInRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect);
HI_BOOL HAL_LAYER_SetLayerOutRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect);
HI_BOOL HAL_LAYER_SetLayerGAlpha(HAL_DISP_LAYER_E enLayer,
                                 HI_U8 u8Alpha0);
HI_BOOL HAL_LAYER_GetLayerGAlpha(HAL_DISP_LAYER_E enLayer, HI_U8 *pu8Alpha0);

HI_BOOL HAL_LAYER_SetRegUp(HAL_DISP_LAYER_E enLayer);

/*****************************************************************************
 Prototype       : graphic layer Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_GRAPHIC_SetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 u64LAddr);
HI_BOOL HAL_GRAPHIC_GetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 *pu64GfxAddr);
HI_BOOL HAL_GRAPHIC_SetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U16 u16pitch);
HI_BOOL HAL_GRAPHIC_GetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32GfxStride);
HI_BOOL HAL_GRAPHIC_SetGfxExt(HAL_DISP_LAYER_E enLayer,
                              HAL_GFX_BITEXTEND_E enMode);
HI_BOOL HAL_GRAPHIC_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_BOOL HAL_GRAPHIC_SetGfxPalpha(HAL_DISP_LAYER_E enLayer,
                                 HI_U32 bAlphaEn, HI_U32 bArange,
                                 HI_U8 u8Alpha0, HI_U8 u8Alpha1);
HI_BOOL HAL_GRAPHIC_GetGfxPalpha(HAL_DISP_LAYER_E enLayer, HI_U32 *pbAlphaEn,
                                 HI_U8 *pu8Alpha0, HI_U8 *pu8Alpha1);

HI_BOOL HAL_GRAPHIC_SetGfxKeyEn(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyEnable);
HI_BOOL HAL_GRAPHIC_SetGfxKeyMode(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyOut);
HI_BOOL HAL_GRAPHIC_SetColorKeyValue(HAL_DISP_LAYER_E enLayer,
                                     HAL_GFX_KEY_MAX_S stKeyMax, HAL_GFX_KEY_MIN_S stKeyMin);
HI_BOOL HAL_GRAPHIC_SetColorKeyMask(HAL_DISP_LAYER_E enLayer, HAL_GFX_MASK_S stMsk);

HI_BOOL HAL_GRAPHIC_GetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 *pbEnable);

/*****************************************************************************
 Prototype       : cbm layer Relative
 Description     :

*****************************************************************************/

HI_BOOL HAL_CBM_SetCbmBkg(HI_U32 bMixerId, HAL_DISP_BKCOLOR_S *pstBkg);
HI_VOID HAL_LINK_GetHcLink(HI_U32 *pu32Data);

HI_VOID HAL_HIHDR_G_SetHihdrR2yScale2p(HI_U32 hihdr_r2y_scale2p);
HI_VOID HAL_HIHDR_G_SetHihdrR2yClipMin(HI_U32 hihdr_r2y_clip_min);
HI_VOID HAL_HIHDR_G_SetHihdrR2yClipMax(HI_U32 hihdr_r2y_clip_max);

/**********************************************************************************
*  Begin : Graphic layer HDR relative hal functions.
**********************************************************************************/
HI_VOID HAL_HIHDR_G_SetGhdrDemoPos(HI_U32 ghdr_demo_pos);
HI_VOID HAL_HIHDR_G_SetGhdrDemoMode(HI_U32 ghdr_demo_mode);
HI_VOID HAL_HIHDR_G_SetGhdrDemoEn(HI_U32 ghdr_demo_en);
HI_VOID HAL_HIHDR_G_SetGhdrCkGtEn(HI_U32 ghdr_ck_gt_en);
HI_VOID HAL_HIHDR_G_SetGhdrEn(HI_U32 ghdr_en);
HI_VOID HAL_HIHDR_G_SetGhdrDegmmEn(HI_U32 ghdr_degmm_en);
HI_VOID HAL_HIHDR_G_SetGDegmmX4Step(HI_U32 g_degmm_x4_step);
HI_VOID HAL_HIHDR_G_SetGDegmmX3Step(HI_U32 g_degmm_x3_step);
HI_VOID HAL_HIHDR_G_SetGDegmmX2Step(HI_U32 g_degmm_x2_step);
HI_VOID HAL_HIHDR_G_SetGDegmmX1Step(HI_U32 g_degmm_x1_step);
HI_VOID HAL_HIHDR_G_SetGDegmmX2Pos(HI_U32 g_degmm_x2_pos);
HI_VOID HAL_HIHDR_G_SetGDegmmX1Pos(HI_U32 g_degmm_x1_pos);
HI_VOID HAL_HIHDR_G_SetGDegmmX4Pos(HI_U32 g_degmm_x4_pos);
HI_VOID HAL_HIHDR_G_SetGDegmmX3Pos(HI_U32 g_degmm_x3_pos);
HI_VOID HAL_HIHDR_G_SetGDegmmX4Num(HI_U32 g_degmm_x4_num);
HI_VOID HAL_HIHDR_G_SetGDegmmX3Num(HI_U32 g_degmm_x3_num);
HI_VOID HAL_HIHDR_G_SetGDegmmX2Num(HI_U32 g_degmm_x2_num);
HI_VOID HAL_HIHDR_G_SetGDegmmX1Num(HI_U32 g_degmm_x1_num);
HI_VOID HAL_HIHDR_G_SetGhdrGamutEn(HI_U32 ghdr_gamut_en);
HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef00(HI_U32 ghdr_gamut_coef00);
HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef01(HI_U32 ghdr_gamut_coef01);
HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef02(HI_U32 ghdr_gamut_coef02);
HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef10(HI_U32 ghdr_gamut_coef10);
HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef11(HI_U32 ghdr_gamut_coef11);
HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef12(HI_U32 ghdr_gamut_coef12);
HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef20(HI_U32 ghdr_gamut_coef20);
HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef21(HI_U32 ghdr_gamut_coef21);
HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef22(HI_U32 ghdr_gamut_coef22);
HI_VOID HAL_HIHDR_G_SetGhdrGamutScale(HI_U32 ghdr_gamut_scale);
HI_VOID HAL_HIHDR_G_SetGhdrGamutClipMin(HI_U32 ghdr_gamut_clip_min);
HI_VOID HAL_HIHDR_G_SetGhdrGamutClipMax(HI_U32 ghdr_gamut_clip_max);
HI_VOID HAL_HIHDR_G_SetGhdrTmapEn(HI_U32 ghdr_tmap_en);
HI_VOID HAL_HIHDR_G_SetGTmapX4Step(HI_U32 g_tmap_x4_step);
HI_VOID HAL_HIHDR_G_SetGTmapX3Step(HI_U32 g_tmap_x3_step);
HI_VOID HAL_HIHDR_G_SetGTmapX2Step(HI_U32 g_tmap_x2_step);
HI_VOID HAL_HIHDR_G_SetGTmapX1Step(HI_U32 g_tmap_x1_step);
HI_VOID HAL_HIHDR_G_SetGTmapX1Pos(HI_U32 g_tmap_x1_pos);
HI_VOID HAL_HIHDR_G_SetGTmapX2Pos(HI_U32 g_tmap_x2_pos);
HI_VOID HAL_HIHDR_G_SetGTmapX3Pos(HI_U32 g_tmap_x3_pos);
HI_VOID HAL_HIHDR_G_SetGTmapX4Pos(HI_U32 g_tmap_x4_pos);
HI_VOID HAL_HIHDR_G_SetGTmapX4Num(HI_U32 g_tmap_x4_num);
HI_VOID HAL_HIHDR_G_SetGTmapX3Num(HI_U32 g_tmap_x3_num);
HI_VOID HAL_HIHDR_G_SetGTmapX2Num(HI_U32 g_tmap_x2_num);
HI_VOID HAL_HIHDR_G_SetGTmapX1Num(HI_U32 g_tmap_x1_num);
HI_VOID HAL_HIHDR_G_SetGhdrTmapLumaCoef0(HI_U32 ghdr_tmap_luma_coef0);
HI_VOID HAL_HIHDR_G_SetGhdrTmapLumaCoef1(HI_U32 ghdr_tmap_luma_coef1);
HI_VOID HAL_HIHDR_G_SetGhdrTmapLumaCoef2(HI_U32 ghdr_tmap_luma_coef2);
HI_VOID HAL_HIHDR_G_SetGhdrTmapLumaScale(HI_U32 ghdr_tmap_luma_scale);
HI_VOID HAL_HIHDR_G_SetGhdrTmapCoefScale(HI_U32 ghdr_tmap_coef_scale);
HI_VOID HAL_HIHDR_G_SetGhdrTmapOutClipMin(HI_U32 ghdr_tmap_out_clip_min);
HI_VOID HAL_HIHDR_G_SetGhdrTmapOutClipMax(HI_U32 ghdr_tmap_out_clip_max);
HI_VOID HAL_HIHDR_G_SetGhdrGmmEn(HI_U32 ghdr_gmm_en);
HI_VOID HAL_HIHDR_G_SetGGmmX4Step(HI_U32 g_gmm_x4_step);
HI_VOID HAL_HIHDR_G_SetGGmmX3Step(HI_U32 g_gmm_x3_step);
HI_VOID HAL_HIHDR_G_SetGGmmX2Step(HI_U32 g_gmm_x2_step);
HI_VOID HAL_HIHDR_G_SetGGmmX1Step(HI_U32 g_gmm_x1_step);
HI_VOID HAL_HIHDR_G_SetGGmmX8Step(HI_U32 g_gmm_x8_step);
HI_VOID HAL_HIHDR_G_SetGGmmX7Step(HI_U32 g_gmm_x7_step);
HI_VOID HAL_HIHDR_G_SetGGmmX6Step(HI_U32 g_gmm_x6_step);
HI_VOID HAL_HIHDR_G_SetGGmmX5Step(HI_U32 g_gmm_x5_step);
HI_VOID HAL_HIHDR_G_SetGGmmX2Pos(HI_U32 g_gmm_x2_pos);
HI_VOID HAL_HIHDR_G_SetGGmmX1Pos(HI_U32 g_gmm_x1_pos);
HI_VOID HAL_HIHDR_G_SetGGmmX4Pos(HI_U32 g_gmm_x4_pos);
HI_VOID HAL_HIHDR_G_SetGGmmX3Pos(HI_U32 g_gmm_x3_pos);
HI_VOID HAL_HIHDR_G_SetGGmmX6Pos(HI_U32 g_gmm_x6_pos);
HI_VOID HAL_HIHDR_G_SetGGmmX5Pos(HI_U32 g_gmm_x5_pos);
HI_VOID HAL_HIHDR_G_SetGGmmX8Pos(HI_U32 g_gmm_x8_pos);
HI_VOID HAL_HIHDR_G_SetGGmmX7Pos(HI_U32 g_gmm_x7_pos);
HI_VOID HAL_HIHDR_G_SetGGmmX4Num(HI_U32 g_gmm_x4_num);
HI_VOID HAL_HIHDR_G_SetGGmmX3Num(HI_U32 g_gmm_x3_num);
HI_VOID HAL_HIHDR_G_SetGGmmX2Num(HI_U32 g_gmm_x2_num);
HI_VOID HAL_HIHDR_G_SetGGmmX1Num(HI_U32 g_gmm_x1_num);
HI_VOID HAL_HIHDR_G_SetGGmmX8Num(HI_U32 g_gmm_x8_num);
HI_VOID HAL_HIHDR_G_SetGGmmX7Num(HI_U32 g_gmm_x7_num);
HI_VOID HAL_HIHDR_G_SetGGmmX6Num(HI_U32 g_gmm_x6_num);
HI_VOID HAL_HIHDR_G_SetGGmmX5Num(HI_U32 g_gmm_x5_num);
HI_VOID HAL_HIHDR_G_SetGhdrDitherRoundUnlim(HI_U32 ghdr_dither_round_unlim);
HI_VOID HAL_HIHDR_G_SetGhdrDitherEn(HI_U32 ghdr_dither_en);
HI_VOID HAL_HIHDR_G_SetGhdrDitherMode(HI_U32 ghdr_dither_mode);
HI_VOID HAL_HIHDR_G_SetGhdrDitherRound(HI_U32 ghdr_dither_round);
HI_VOID HAL_HIHDR_G_SetGhdrDitherDomainMode(HI_U32 ghdr_dither_domain_mode);
HI_VOID HAL_HIHDR_G_SetGhdrDitherTapMode(HI_U32 ghdr_dither_tap_mode);
HI_VOID HAL_HIHDR_G_SetGhdrDitherThrMax(HI_U32 ghdr_dither_thr_max);
HI_VOID HAL_HIHDR_G_SetGhdrDitherThrMin(HI_U32 ghdr_dither_thr_min);
HI_VOID HAL_HIHDR_G_SetGhdrDitherSedY0(HI_U32 ghdr_dither_sed_y0);
HI_VOID HAL_HIHDR_G_SetGhdrDitherSedU0(HI_U32 ghdr_dither_sed_u0);
HI_VOID HAL_HIHDR_G_SetGhdrDitherSedV0(HI_U32 ghdr_dither_sed_v0);
HI_VOID HAL_HIHDR_G_SetGhdrDitherSedW0(HI_U32 ghdr_dither_sed_w0);
HI_VOID HAL_HIHDR_G_SetGhdrDitherSedY1(HI_U32 ghdr_dither_sed_y1);
HI_VOID HAL_HIHDR_G_SetGhdrDitherSedU1(HI_U32 ghdr_dither_sed_u1);
HI_VOID HAL_HIHDR_G_SetGhdrDitherSedV1(HI_U32 ghdr_dither_sed_v1);
HI_VOID HAL_HIHDR_G_SetGhdrDitherSedW1(HI_U32 ghdr_dither_sed_w1);
HI_VOID HAL_HIHDR_G_SetHihdrR2yEn(HI_U32 hihdr_r2y_en);
HI_VOID HAL_HIHDR_G_SetHihdrR2yScale2p(HI_U32 hihdr_r2y_scale2p);
HI_VOID HAL_HIHDR_G_SetHihdrR2yClipMin(HI_U32 hihdr_r2y_clip_min);
HI_VOID HAL_HIHDR_G_SetHihdrR2yClipMax(HI_U32 hihdr_r2y_clip_max);

/**********************************************************************************
*  End   : Graphic layer HDR relative hal functions.
**********************************************************************************/
/***************************************************************************************************
*  Begin : Parameter Address distribute
***************************************************************************************************/
HI_VOID HAL_PARA_SetParaAddrVhdChn04(HI_U64 para_addr_vhd_chn04);
HI_VOID HAL_PARA_SetParaAddrVhdChn05(HI_U64 para_addr_vhd_chn05);
HI_VOID HAL_PARA_SetParaAddrVhdChn06(HI_U64 para_addr_vhd_chn06);
HI_VOID HAL_PARA_SetParaUpVhdChn(HI_U32 u32ChnNum);
HI_VOID HAL_PARA_SetParaUpVhdChnAll(void);

/***************************************************************************************************
*  End   : Parameter Address distribute.
***************************************************************************************************/
/**********************************************************************************
*  Begin   : Graphic layer ZME relative hal functions.
**********************************************************************************/
HI_VOID HAL_G0_ZME_SetCkGtEn(HI_U32 ck_gt_en);
HI_VOID HAL_G0_ZME_SetOutWidth(HI_U32 out_width);
HI_VOID HAL_G0_ZME_SetHfirEn(HI_U32 hfir_en);
HI_VOID HAL_G0_ZME_SetAhfirMidEn(HI_U32 ahfir_mid_en);
HI_VOID HAL_G0_ZME_SetLhfirMidEn(HI_U32 lhfir_mid_en);
HI_VOID HAL_G0_ZME_SetChfirMidEn(HI_U32 chfir_mid_en);
HI_VOID HAL_G0_ZME_SetLhfirMode(HI_U32 lhfir_mode);
HI_VOID HAL_G0_ZME_SetAhfirMode(HI_U32 ahfir_mode);
HI_VOID HAL_G0_ZME_SetHfirOrder(HI_U32 hfir_order);
HI_VOID HAL_G0_ZME_SetHratio(HI_U32 hratio);
HI_VOID HAL_G0_ZME_SetLhfirOffset(HI_U32 lhfir_offset);
HI_VOID HAL_G0_ZME_SetChfirOffset(HI_U32 chfir_offset);
HI_VOID HAL_G0_ZME_SetOutPro(HI_U32 out_pro);
HI_VOID HAL_G0_ZME_SetOutHeight(HI_U32 out_height);
HI_VOID HAL_G0_ZME_SetVfirEn(HI_U32 vfir_en);
HI_VOID HAL_G0_ZME_SetAvfirMidEn(HI_U32 avfir_mid_en);
HI_VOID HAL_G0_ZME_SetLvfirMidEn(HI_U32 lvfir_mid_en);
HI_VOID HAL_G0_ZME_SetCvfirMidEn(HI_U32 cvfir_mid_en);
HI_VOID HAL_G0_ZME_SetLvfirMode(HI_U32 lvfir_mode);
HI_VOID HAL_G0_ZME_SetVafirMode(HI_U32 vafir_mode);
HI_VOID HAL_G0_ZME_SetVratio(HI_U32 vratio);
HI_VOID HAL_G0_ZME_SetVtpOffset(HI_U32 vtp_offset);
HI_VOID HAL_G0_ZME_SetVbtmOffset(HI_U32 vbtm_offset);

/**********************************************************************************
*  End     : Graphic layer ZME relative hal functions.
**********************************************************************************/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* End of __VOU_HAL_H__ */

