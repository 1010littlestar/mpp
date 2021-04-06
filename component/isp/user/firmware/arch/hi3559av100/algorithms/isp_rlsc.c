/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <stdio.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_sensor.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* ----------------------------------------------*
 * module-wide global variables                 *
 * ---------------------------------------------- */
static const  HI_U16  g_au16GainStrDef[14] = {32768, 16384, 8192, 4096, 2048, 1024, 512, 0, 0, 0, 0, 0, 0, 0};

typedef struct hiISP_RLSC_CALI_S {
    HI_U16 u16WBRGain;
    HI_U16 u16WBBGain;

    HI_U16 au16RGain[HI_ISP_RLSC_POINTS];
    HI_U16 au16GrGain[HI_ISP_RLSC_POINTS];
    HI_U16 au16GbGain[HI_ISP_RLSC_POINTS];
    HI_U16 au16BGain[HI_ISP_RLSC_POINTS];
} ISP_RLSC_CALI_S;

typedef struct hiISP_RLSC {
    // Enable/Disable, ext reg: yes, PQTools: yes:
    HI_BOOL bLscEnable;         // General rlsc enable
    HI_BOOL bRLscFuncEn;
    HI_BOOL bRadialCropEn;      // Radial Crop Function enable
    OPERATION_MODE_E enLightMode; // Switch to select auto/manual mode

    // Manual related value, ext reg: yes, PQTools: yes:
    HI_U16 u16BlendRatio;         // Manual mode, light blending weight
    HI_U8  u8LightType1;   // Manual mode, light source 1 info
    HI_U8  u8LightType2;   // Manual mode, light source 2 info
    HI_U8  u8RadialScale;     // Select Radial Scale mode
    // Correction control coefficients, ext reg: yes, PQTools: yes:
    HI_U32 u32ValidRadius;    // square Valid radius, Enable when Radial crop is on
    HI_U16 u16RadialStrength; // correction strength

    // Update Booleans, ext reg: yes, PQTools: no:
    HI_BOOL bLscCoefUpdate;    // Coefficient update flag
    HI_BOOL bLutUpdate;        // LUT update flag

    // Center point info, ext reg: yes, PQTools: yes,
    HI_U16  u16CenterRX;
    HI_U16  u16CenterRY;
    HI_U16  u16CenterGrX;
    HI_U16  u16CenterGrY;
    HI_U16  u16CenterGbX;
    HI_U16  u16CenterGbY;
    HI_U16  u16CenterBX;
    HI_U16  u16CenterBY;

    HI_U16  u16OffCenterR;
    HI_U16  u16OffCenterGr;
    HI_U16  u16OffCenterGb;
    HI_U16  u16OffCenterB;

    // White balance info, ext reg:no, PQTools: no
    HI_U16  u16WbRGain;
    HI_U16  u16WbBGain;

    ISP_RLSC_CALI_S stRLscCaliResult[3];

} ISP_RLSC_S;

ISP_RLSC_S *g_astRLscCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define RLSC_GET_CTX(dev, pstCtx)   (pstCtx = g_astRLscCtx[dev])
#define RLSC_SET_CTX(dev, pstCtx)   (g_astRLscCtx[dev] = pstCtx)
#define RLSC_RESET_CTX(dev)         (g_astRLscCtx[dev] = HI_NULL)

HI_S32 RLscCtxInit(VI_PIPE ViPipe)
{
    ISP_RLSC_S *pastRLscCtx = HI_NULL;

    RLSC_GET_CTX(ViPipe, pastRLscCtx);

    if (pastRLscCtx == HI_NULL) {
        pastRLscCtx = (ISP_RLSC_S *)ISP_MALLOC(sizeof(ISP_RLSC_S));
        if (pastRLscCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] RLscCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastRLscCtx, 0, sizeof(ISP_RLSC_S));

    RLSC_SET_CTX(ViPipe, pastRLscCtx);

    return HI_SUCCESS;
}

HI_VOID RLscCtxExit(VI_PIPE ViPipe)
{
    ISP_RLSC_S *pastRLscCtx = HI_NULL;

    RLSC_GET_CTX(ViPipe, pastRLscCtx);
    ISP_FREE(pastRLscCtx);
    RLSC_RESET_CTX(ViPipe);
}

static HI_VOID RLscCalcWeight(ISP_RLSC_S *pstRLsc, HI_U16 *pu16BlendRatio, HI_U8 *pu8LscLightInfo1, HI_U8 *pu8LscLightInfo2)
{
    HI_U8  i;
    HI_S16 x11, y11;
    HI_S16 s16WbRGain, s16WbBGain;

    HI_U32 u32MinD1, u32MinD2;
    HI_U32 d[3];

    // Using input white balance info to calculate weight
    // Parameter Declaration Ends
    s16WbRGain = (HI_S16)pstRLsc->u16WbRGain;
    s16WbBGain = (HI_S16)pstRLsc->u16WbBGain;

    *pu8LscLightInfo1 = 0;
    *pu8LscLightInfo2 = *pu8LscLightInfo1;
    u32MinD1 = 1U << 31;
    u32MinD2 = 1U << 31;

    for (i = 0; i < 3; i++) {
        x11  = ABS(s16WbRGain - (HI_S16)pstRLsc->stRLscCaliResult[i].u16WBRGain);
        y11  = ABS(s16WbBGain - (HI_S16)pstRLsc->stRLscCaliResult[i].u16WBBGain);
        d[i] = x11 * x11 + y11 * y11;

        if (d[i] < u32MinD1) {
            u32MinD1 = d[i];
            *pu8LscLightInfo1 = i;
        }
    }

    d[*pu8LscLightInfo1] = u32MinD2;

    for (i = 0; i < 3; i++) {
        if (d[i] < u32MinD2) {
            u32MinD2 = d[i];
            *pu8LscLightInfo2 = i;
        }
    }

    *pu16BlendRatio = (HI_U16)(u32MinD2 * 256 / DIV_0_TO_1(u32MinD1 + u32MinD2));

    return;
}

static HI_VOID AdjustCenterPoint(VI_PIPE ViPipe, ISP_RLSC_S *pstRLsc)
{
    HI_U16 u16Width, u16Height;
    isp_usr_ctx *pstIspCtx;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    u16Width  = pstIspCtx->block_attr.frame_rect.width;
    u16Height = pstIspCtx->block_attr.frame_rect.height;

    switch (pstIspCtx->bayer) {
        case BAYER_RGGB :
            pstRLsc->u16CenterGrX = MIN2(pstRLsc->u16CenterGrX + 1, u16Width - 1);
            pstRLsc->u16CenterGbY = MIN2(pstRLsc->u16CenterGbY + 1, u16Height - 1);
            pstRLsc->u16CenterBX  = MIN2(pstRLsc->u16CenterBX + 1, u16Width - 1);
            pstRLsc->u16CenterBY  = MIN2(pstRLsc->u16CenterBY + 1, u16Height - 1);
            break;

        case BAYER_GRBG :
            pstRLsc->u16CenterRX  = MIN2(pstRLsc->u16CenterRX + 1, u16Width - 1);
            pstRLsc->u16CenterBY  = MIN2(pstRLsc->u16CenterBY + 1, u16Height - 1);
            pstRLsc->u16CenterGbX = MIN2(pstRLsc->u16CenterGbX + 1, u16Width - 1);
            pstRLsc->u16CenterGbY = MIN2(pstRLsc->u16CenterGbY + 1, u16Height - 1);
            break;

        case BAYER_GBRG :
            pstRLsc->u16CenterBX  = MIN2(pstRLsc->u16CenterBX + 1, u16Width - 1);
            pstRLsc->u16CenterRY  = MIN2(pstRLsc->u16CenterRY + 1, u16Height - 1);
            pstRLsc->u16CenterGrX = MIN2(pstRLsc->u16CenterGrX + 1, u16Width - 1);
            pstRLsc->u16CenterGrY = MIN2(pstRLsc->u16CenterGrY + 1, u16Height - 1);
            break;

        case BAYER_BGGR :
            pstRLsc->u16CenterGbX = MIN2(pstRLsc->u16CenterGbX + 1, u16Width - 1);
            pstRLsc->u16CenterGrY = MIN2(pstRLsc->u16CenterGrY + 1, u16Height - 1);
            pstRLsc->u16CenterRX  = MIN2(pstRLsc->u16CenterRX + 1, u16Width - 1);
            pstRLsc->u16CenterRY  = MIN2(pstRLsc->u16CenterRY + 1, u16Height - 1);
            break;

        default:
            break;
    }

    return;
}

static HI_VOID CalcRadius(VI_PIPE ViPipe, ISP_RLSC_S *pstRLsc)
{
    HI_U8  i;
    HI_U32 u32Radius;
    HI_U16 u16OffCenter[4];
    HI_U16 u16MaxOffCenter = 0;
    HI_U32 temp = (1U << 31);

    u16OffCenter[0] = pstRLsc->u16OffCenterR;
    u16OffCenter[1] = pstRLsc->u16OffCenterGr;
    u16OffCenter[2] = pstRLsc->u16OffCenterGb;
    u16OffCenter[3] = pstRLsc->u16OffCenterB;

    for (i = 0; i < 4; i++) {
        if (u16OffCenter[i] >= u16MaxOffCenter) {
            u16MaxOffCenter = u16OffCenter[i];
        }
    }
    u32Radius = temp / (HI_U32)DIV_0_TO_1(u16MaxOffCenter);
    pstRLsc->u32ValidRadius = u32Radius;

    return;
}

/*
 * [RLscGetGainLut:]
 * [Used to calculate actual LUT gain value]
 * @param  ViPipe       [Input]  Vi Pipe No.
 * @param  pstRLsc      [Input]  Radial LSC structure, contains original LUT gain(three)
 * @param  pstUsrRegCfg [Output] User Register config, need to update its LUT gain value
 * @return              [Void]   No return value
 */
static HI_VOID RLscGetGainLut(VI_PIPE ViPipe, ISP_RLSC_S *pstRLsc, ISP_RLSC_USR_CFG_S *pstUsrRegCfg)
{
    HI_U16 i;
    HI_U16 u16BlendRatio   = 0;
    HI_U8  u8LscLightInfo1 = 0;
    HI_U8  u8LscLightInfo2 = 0;

    HI_U32 au32RGain[HI_ISP_RLSC_POINTS + 1];
    HI_U32 au32GrGain[HI_ISP_RLSC_POINTS + 1];
    HI_U32 au32GbGain[HI_ISP_RLSC_POINTS + 1];
    HI_U32 au32BGain[HI_ISP_RLSC_POINTS + 1];

    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    // Get Weight, lightinfo1 and lightinfo2
    if (pstRLsc->enLightMode == OPERATION_MODE_MANUAL) {
        u16BlendRatio = pstRLsc->u16BlendRatio;
        u8LscLightInfo1 = pstRLsc->u8LightType1;
        u8LscLightInfo2 = pstRLsc->u8LightType2;
    } else {
        RLscCalcWeight(pstRLsc, &u16BlendRatio, &u8LscLightInfo1, &u8LscLightInfo2);
    }

    // Use weight, lightinfo1 and lightinfo2 to calculate actural gain value
    for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
        au32RGain[i]  = ((pstRLsc->stRLscCaliResult[u8LscLightInfo1].au16RGain[i]) * u16BlendRatio + (pstRLsc->stRLscCaliResult[u8LscLightInfo2].au16RGain[i]) * (256 - u16BlendRatio)) >> HI_ISP_RLSC_WEIGHT_Q_BITS;
        au32GrGain[i] = ((pstRLsc->stRLscCaliResult[u8LscLightInfo1].au16GrGain[i]) * u16BlendRatio + (pstRLsc->stRLscCaliResult[u8LscLightInfo2].au16GrGain[i]) * (256 - u16BlendRatio)) >> HI_ISP_RLSC_WEIGHT_Q_BITS;
        au32GbGain[i] = ((pstRLsc->stRLscCaliResult[u8LscLightInfo1].au16GbGain[i]) * u16BlendRatio + (pstRLsc->stRLscCaliResult[u8LscLightInfo2].au16GbGain[i]) * (256 - u16BlendRatio)) >> HI_ISP_RLSC_WEIGHT_Q_BITS;
        au32BGain[i]  = ((pstRLsc->stRLscCaliResult[u8LscLightInfo1].au16BGain[i]) * u16BlendRatio + (pstRLsc->stRLscCaliResult[u8LscLightInfo2].au16BGain[i]) * (256 - u16BlendRatio)) >> HI_ISP_RLSC_WEIGHT_Q_BITS;
    }
    // Copy the last node
    au32RGain[HI_ISP_RLSC_POINTS]  = au32RGain[HI_ISP_RLSC_POINTS - 1];
    au32GrGain[HI_ISP_RLSC_POINTS] = au32GrGain[HI_ISP_RLSC_POINTS - 1];
    au32GbGain[HI_ISP_RLSC_POINTS] = au32GbGain[HI_ISP_RLSC_POINTS - 1];
    au32BGain[HI_ISP_RLSC_POINTS]  = au32BGain[HI_ISP_RLSC_POINTS - 1];
    // allocate to this right position according to rggb pattern:
    switch (pstIspCtx->bayer) {
        case BAYER_RGGB :
            memcpy(pstUsrRegCfg->au32Lut0Chn0, au32RGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut0Chn1, au32GrGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut1Chn2, au32GbGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut1Chn3, au32BGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            break;

        case BAYER_GRBG :
            memcpy(pstUsrRegCfg->au32Lut0Chn0, au32GrGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut0Chn1, au32RGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut1Chn2, au32BGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut1Chn3, au32GbGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            break;

        case BAYER_GBRG :
            memcpy(pstUsrRegCfg->au32Lut0Chn0, au32GbGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut0Chn1, au32BGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut1Chn2, au32RGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut1Chn3, au32GrGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            break;

        case BAYER_BGGR :
            memcpy(pstUsrRegCfg->au32Lut0Chn0, au32BGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut0Chn1, au32GbGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut1Chn2, au32GrGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            memcpy(pstUsrRegCfg->au32Lut1Chn3, au32RGain, (HI_ISP_RLSC_POINTS + 1)*sizeof(HI_U32));
            break;

        default:
            break;
    }

    return;
}

static HI_VOID RLscGetCaliRadiusInfo(ISP_RLSC_S *pstRLsc, ISP_RLSC_USR_CFG_S *pstUsrRegCfg)
{
    pstUsrRegCfg->u16CenterRX  = pstRLsc->u16CenterRX;
    pstUsrRegCfg->u16CenterRY  = pstRLsc->u16CenterRY;
    pstUsrRegCfg->u16CenterGrX = pstRLsc->u16CenterGrX;
    pstUsrRegCfg->u16CenterGrY = pstRLsc->u16CenterGrY;
    pstUsrRegCfg->u16CenterGbX = pstRLsc->u16CenterGbX;
    pstUsrRegCfg->u16CenterGbY = pstRLsc->u16CenterGbY;
    pstUsrRegCfg->u16CenterBX  = pstRLsc->u16CenterBX;
    pstUsrRegCfg->u16CenterBY  = pstRLsc->u16CenterBY;

    pstUsrRegCfg->u16OffCenterR  = pstRLsc->u16OffCenterR;
    pstUsrRegCfg->u16OffCenterGr = pstRLsc->u16OffCenterGr;
    pstUsrRegCfg->u16OffCenterGb = pstRLsc->u16OffCenterGb;
    pstUsrRegCfg->u16OffCenterB  = pstRLsc->u16OffCenterB;

    pstUsrRegCfg->u32ValidRadius = pstRLsc->u32ValidRadius;
}

static HI_VOID RLscStaticRegsInitialize(VI_PIPE ViPipe, ISP_RLSC_STATIC_CFG_S *pstStaticRegCfg)
{
    pstStaticRegCfg->u16NodeNum = HI_ISP_RLSC_POINTS;

    pstStaticRegCfg->bStaticResh = HI_TRUE;

    return;
}

static HI_VOID RLscUsrRegsInitialize(HI_U8 u8CurBlk, VI_PIPE ViPipe, ISP_RLSC_USR_CFG_S *pstUsrRegCfg)
{
    isp_usr_ctx  *pstIspCtx;
    ISP_RLSC_S *pstRLsc    = HI_NULL;
    isp_rect stBlockRect;

    RLSC_GET_CTX(ViPipe, pstRLsc);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER_VOID(pstRLsc);

    ISP_GetBlockRect(&stBlockRect, &pstIspCtx->block_attr, u8CurBlk);
    pstUsrRegCfg->u16WidthOffset = ABS(stBlockRect.x);
    pstUsrRegCfg->u16GainStr  = pstRLsc->u16RadialStrength;
    pstUsrRegCfg->u8GainScale = pstRLsc->u8RadialScale;

    RLscGetCaliRadiusInfo(pstRLsc, pstUsrRegCfg);

    RLscGetGainLut(ViPipe, pstRLsc, pstUsrRegCfg);

    pstUsrRegCfg->u32UpdateIndex = 1;

    pstUsrRegCfg->bLutUpdate     = HI_TRUE;
    pstUsrRegCfg->bCoefUpdate    = HI_TRUE;
    pstUsrRegCfg->bRadialCropEn  = HI_TRUE;
    pstUsrRegCfg->bRLscFuncEn    = pstRLsc->bRLscFuncEn;
    pstUsrRegCfg->bUsrResh       = HI_TRUE;

    return;
}

/*
 * [RLscRegsInitialize description]
 * @param  ViPipe    [Input]  ViPipe Number
 * @param  pstRegCfg [Output] Register config
 * @return           [Void]   No return value
 */
static HI_VOID RLscRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_S32 i;
    ISP_RLSC_S *pstRLsc    = HI_NULL;

    RLSC_GET_CTX(ViPipe, pstRLsc);
    ISP_CHECK_POINTER_VOID(pstRLsc);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        RLscStaticRegsInitialize(ViPipe, &pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.stStaticRegCfg);
        RLscUsrRegsInitialize(i, ViPipe, &pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.stUsrRegCfg);

        pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.bRLscEn = pstRLsc->bLscEnable;
    }

    pstRegCfg->cfg_key.bit1RLscCfg = 1;

    return;
}

/*
 * [RLscExtRegsInitialize: Initialize ext registers, which is mainly used by tools]
 * @param  ViPipe    [Input]  ViPipe Number
 * @return           [Void]   No return value
 */
static HI_VOID RLscExtRegsInitialize(VI_PIPE ViPipe)
{
    HI_U16 i;
    ISP_RLSC_S  *pstRLsc    = HI_NULL;

    RLSC_GET_CTX(ViPipe, pstRLsc);
    ISP_CHECK_POINTER_VOID(pstRLsc);

    hi_ext_system_isp_radial_shading_enable_write(ViPipe, pstRLsc->bLscEnable);

    hi_ext_system_isp_radial_shading_coefupdate_write(ViPipe, HI_FALSE);
    hi_ext_system_isp_radial_shading_lutupdate_write(ViPipe, HI_FALSE);

    // Coef update
    hi_ext_system_isp_radial_shading_strength_write(ViPipe, pstRLsc->u16RadialStrength);

    // Lut update
    hi_ext_system_isp_radial_shading_lightmode_write(ViPipe, pstRLsc->enLightMode);
    hi_ext_system_isp_radial_shading_blendratio_write(ViPipe, pstRLsc->u16BlendRatio);
    hi_ext_system_isp_radial_shading_scale_write(ViPipe, pstRLsc->u8RadialScale);

    // light info, manual mode
    hi_ext_system_isp_radial_shading_lightinfo_write(ViPipe, 0, pstRLsc->u8LightType1);
    hi_ext_system_isp_radial_shading_lightinfo_write(ViPipe, 1, pstRLsc->u8LightType2);

    hi_ext_system_isp_radial_shading_centerrx_write(ViPipe, pstRLsc->u16CenterRX);
    hi_ext_system_isp_radial_shading_centerry_write(ViPipe, pstRLsc->u16CenterRY);
    hi_ext_system_isp_radial_shading_centergrx_write(ViPipe, pstRLsc->u16CenterGrX);
    hi_ext_system_isp_radial_shading_centergry_write(ViPipe, pstRLsc->u16CenterGrY);
    hi_ext_system_isp_radial_shading_centergbx_write(ViPipe, pstRLsc->u16CenterGbX);
    hi_ext_system_isp_radial_shading_centergby_write(ViPipe, pstRLsc->u16CenterGbY);
    hi_ext_system_isp_radial_shading_centerbx_write(ViPipe, pstRLsc->u16CenterBX);
    hi_ext_system_isp_radial_shading_centerby_write(ViPipe, pstRLsc->u16CenterBY);
    hi_ext_system_isp_radial_shading_offcenterr_write(ViPipe, pstRLsc->u16OffCenterR);
    hi_ext_system_isp_radial_shading_offcentergr_write(ViPipe, pstRLsc->u16OffCenterGr);
    hi_ext_system_isp_radial_shading_offcentergb_write(ViPipe, pstRLsc->u16OffCenterGb);
    hi_ext_system_isp_radial_shading_offcenterb_write(ViPipe, pstRLsc->u16OffCenterB);

    for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
        hi_ext_system_isp_radial_shading_r_gain0_write(ViPipe, i, pstRLsc->stRLscCaliResult[0].au16RGain[i]);
        hi_ext_system_isp_radial_shading_r_gain1_write(ViPipe, i, pstRLsc->stRLscCaliResult[1].au16RGain[i]);
        hi_ext_system_isp_radial_shading_r_gain2_write(ViPipe, i, pstRLsc->stRLscCaliResult[2].au16RGain[i]);

        hi_ext_system_isp_radial_shading_gr_gain0_write(ViPipe, i, pstRLsc->stRLscCaliResult[0].au16GrGain[i]);
        hi_ext_system_isp_radial_shading_gr_gain1_write(ViPipe, i, pstRLsc->stRLscCaliResult[1].au16GrGain[i]);
        hi_ext_system_isp_radial_shading_gr_gain2_write(ViPipe, i, pstRLsc->stRLscCaliResult[2].au16GrGain[i]);

        hi_ext_system_isp_radial_shading_gb_gain0_write(ViPipe, i, pstRLsc->stRLscCaliResult[0].au16GbGain[i]);
        hi_ext_system_isp_radial_shading_gb_gain1_write(ViPipe, i, pstRLsc->stRLscCaliResult[1].au16GbGain[i]);
        hi_ext_system_isp_radial_shading_gb_gain2_write(ViPipe, i, pstRLsc->stRLscCaliResult[2].au16GbGain[i]);

        hi_ext_system_isp_radial_shading_b_gain0_write(ViPipe, i, pstRLsc->stRLscCaliResult[0].au16BGain[i]);
        hi_ext_system_isp_radial_shading_b_gain1_write(ViPipe, i, pstRLsc->stRLscCaliResult[1].au16BGain[i]);
        hi_ext_system_isp_radial_shading_b_gain2_write(ViPipe, i, pstRLsc->stRLscCaliResult[2].au16BGain[i]);
    }

    return;
}

static HI_VOID RLscReadExtRegs(VI_PIPE ViPipe)
{
    HI_U16  i;
    HI_U32  u32WbRGain, u32WbGGain, u32WbBGain;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    ISP_RLSC_S *pstRLsc      = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    RLSC_GET_CTX(ViPipe, pstRLsc);
    ISP_CHECK_POINTER_VOID(pstRLsc);

    u32WbRGain = pstIspCtx->linkage.white_balance_gain[0];
    u32WbGGain = (pstIspCtx->linkage.white_balance_gain[1] + pstIspCtx->linkage.white_balance_gain[2]) >> 1;
    u32WbBGain = pstIspCtx->linkage.white_balance_gain[3];
    pstRLsc->u16WbRGain = (HI_U16)(u32WbRGain * 256 / DIV_0_TO_1(u32WbGGain));
    pstRLsc->u16WbBGain = (HI_U16)(u32WbBGain * 256 / DIV_0_TO_1(u32WbGGain));

    // Read Coef udpate, then re-config to HI_FALSE
    pstRLsc->bLscCoefUpdate = hi_ext_system_isp_radial_shading_coefupdate_read(ViPipe);
    hi_ext_system_isp_radial_shading_coefupdate_write(ViPipe, HI_FALSE);

    if (pstRLsc->bLscCoefUpdate) {
        pstRLsc->u16RadialStrength = hi_ext_system_isp_radial_shading_strength_read(ViPipe);
    }

    pstRLsc->bLutUpdate = hi_ext_system_isp_radial_shading_lutupdate_read(ViPipe);
    hi_ext_system_isp_radial_shading_lutupdate_write(ViPipe, HI_FALSE);

    if (pstRLsc->bLutUpdate) {

        pstRLsc->enLightMode   = hi_ext_system_isp_radial_shading_lightmode_read(ViPipe);
        pstRLsc->u16BlendRatio = hi_ext_system_isp_radial_shading_blendratio_read(ViPipe);
        pstRLsc->u8LightType1  = hi_ext_system_isp_radial_shading_lightinfo_read(ViPipe, 0);
        pstRLsc->u8LightType2  = hi_ext_system_isp_radial_shading_lightinfo_read(ViPipe, 1);

        pstRLsc->u8RadialScale = hi_ext_system_isp_radial_shading_scale_read(ViPipe);

        pstRLsc->u16CenterRX   = hi_ext_system_isp_radial_shading_centerrx_read(ViPipe);
        pstRLsc->u16CenterRY   = hi_ext_system_isp_radial_shading_centerry_read(ViPipe);
        pstRLsc->u16OffCenterR = hi_ext_system_isp_radial_shading_offcenterr_read(ViPipe);

        pstRLsc->u16CenterGrX  = hi_ext_system_isp_radial_shading_centergrx_read(ViPipe);
        pstRLsc->u16CenterGrY  = hi_ext_system_isp_radial_shading_centergry_read(ViPipe);
        pstRLsc->u16OffCenterGr = hi_ext_system_isp_radial_shading_offcentergr_read(ViPipe);

        pstRLsc->u16CenterGbX  = hi_ext_system_isp_radial_shading_centergbx_read(ViPipe);
        pstRLsc->u16CenterGbY  = hi_ext_system_isp_radial_shading_centergby_read(ViPipe);
        pstRLsc->u16OffCenterGb = hi_ext_system_isp_radial_shading_offcentergb_read(ViPipe);

        pstRLsc->u16CenterBX   = hi_ext_system_isp_radial_shading_centerbx_read(ViPipe);
        pstRLsc->u16CenterBY   = hi_ext_system_isp_radial_shading_centerby_read(ViPipe);
        pstRLsc->u16OffCenterB = hi_ext_system_isp_radial_shading_offcenterb_read(ViPipe);

        for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
            pstRLsc->stRLscCaliResult[0].au16RGain[i]  = hi_ext_system_isp_radial_shading_r_gain0_read(ViPipe, i);
            pstRLsc->stRLscCaliResult[1].au16RGain[i]  = hi_ext_system_isp_radial_shading_r_gain1_read(ViPipe, i);
            pstRLsc->stRLscCaliResult[2].au16RGain[i]  = hi_ext_system_isp_radial_shading_r_gain2_read(ViPipe, i);

            pstRLsc->stRLscCaliResult[0].au16GrGain[i] = hi_ext_system_isp_radial_shading_gr_gain0_read(ViPipe, i);
            pstRLsc->stRLscCaliResult[1].au16GrGain[i] = hi_ext_system_isp_radial_shading_gr_gain1_read(ViPipe, i);
            pstRLsc->stRLscCaliResult[2].au16GrGain[i] = hi_ext_system_isp_radial_shading_gr_gain2_read(ViPipe, i);

            pstRLsc->stRLscCaliResult[0].au16GbGain[i] = hi_ext_system_isp_radial_shading_gb_gain0_read(ViPipe, i);
            pstRLsc->stRLscCaliResult[1].au16GbGain[i] = hi_ext_system_isp_radial_shading_gb_gain1_read(ViPipe, i);
            pstRLsc->stRLscCaliResult[2].au16GbGain[i] = hi_ext_system_isp_radial_shading_gb_gain2_read(ViPipe, i);

            pstRLsc->stRLscCaliResult[0].au16BGain[i]  = hi_ext_system_isp_radial_shading_b_gain0_read(ViPipe, i);
            pstRLsc->stRLscCaliResult[1].au16BGain[i]  = hi_ext_system_isp_radial_shading_b_gain1_read(ViPipe, i);
            pstRLsc->stRLscCaliResult[2].au16BGain[i]  = hi_ext_system_isp_radial_shading_b_gain2_read(ViPipe, i);
        }
    }

    CalcRadius(ViPipe, pstRLsc);
    return;
}

static HI_S32 RLscCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_rlsc *r_lsc)
{
    hi_u16     width, height;
    isp_usr_ctx  *isp_ctx  = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    width  = isp_ctx->block_attr.frame_rect.width;
    height = isp_ctx->block_attr.frame_rect.height;

    if (r_lsc->scale > 13) {
        ISP_ERR_TRACE("Invalid u8RadialScale!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((r_lsc->center_r_x  >= width) || (r_lsc->center_gr_x >= width) || \
        (r_lsc->center_gb_x >= width) || (r_lsc->center_b_x  >= width)) {
        ISP_ERR_TRACE("Invalid CenterX!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((r_lsc->center_r_y >= height) || (r_lsc->center_gr_y >= height) || \
        (r_lsc->center_gb_y >= height) || (r_lsc->center_b_y >= height)) {
        ISP_ERR_TRACE("Invalid CenterY!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static HI_S32 RLscInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U16     i, j;
    HI_S32     s32Ret;
    HI_U32     u32DefGain;
    HI_U32     u32Width, u32Height;
    HI_U32     u32OffCenter;
    HI_U32     temp = (1U << 31);
    ISP_RLSC_S *pstRLsc    = HI_NULL;
    isp_usr_ctx  *pstIspCtx = HI_NULL;
    hi_isp_cmos_default    *sns_dft  = HI_NULL;
    const hi_isp_cmos_rlsc *cmos_lsc = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    RLSC_GET_CTX(ViPipe, pstRLsc);
    isp_sensor_get_default(ViPipe, &sns_dft);
    ISP_CHECK_POINTER(pstRLsc);

    pstRLsc->bRLscFuncEn   = HI_TRUE;
    pstRLsc->bRadialCropEn = HI_TRUE; // Initialize as enable
    pstRLsc->enLightMode   = OPERATION_MODE_AUTO; // Initialize as AUTO mode
    pstRLsc->u16RadialStrength = HI_ISP_RLSC_DEFAULT_RADIAL_STR;

    pstRLsc->u16BlendRatio = HI_ISP_RLSC_DEFAULT_MANUAL_WEIGHT;
    pstRLsc->u8LightType1  = HI_ISP_RLSC_DEFAULT_LIGHT;
    pstRLsc->u8LightType2  = HI_ISP_RLSC_DEFAULT_LIGHT;

    pstRLsc->u16WbRGain = HI_ISP_RLSC_DEFAULT_WBGAIN;
    pstRLsc->u16WbBGain = HI_ISP_RLSC_DEFAULT_WBGAIN;

    u32Width  = pstIspCtx->block_attr.frame_rect.width;
    u32Height = pstIspCtx->block_attr.frame_rect.height;

    if (sns_dft->key.bit1_r_lsc) {
        ISP_CHECK_POINTER(sns_dft->r_lsc);

        s32Ret = RLscCheckCmosParam(ViPipe, sns_dft->r_lsc);

        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        cmos_lsc = sns_dft->r_lsc;

        pstRLsc->u8RadialScale = cmos_lsc->scale;

        pstRLsc->u16CenterRX  = cmos_lsc->center_r_x;
        pstRLsc->u16CenterRY  = cmos_lsc->center_r_y;
        pstRLsc->u16CenterGrX = cmos_lsc->center_gr_x;
        pstRLsc->u16CenterGrY = cmos_lsc->center_gr_y;
        pstRLsc->u16CenterGbX = cmos_lsc->center_gb_x;
        pstRLsc->u16CenterGbY = cmos_lsc->center_gb_y;
        pstRLsc->u16CenterBX  = cmos_lsc->center_b_x;
        pstRLsc->u16CenterBY  = cmos_lsc->center_b_y;

        pstRLsc->u16OffCenterR  = cmos_lsc->off_center_r;
        pstRLsc->u16OffCenterGr = cmos_lsc->off_center_gr;
        pstRLsc->u16OffCenterGb = cmos_lsc->off_center_gb;
        pstRLsc->u16OffCenterB  = cmos_lsc->off_center_b;

        CalcRadius(ViPipe, pstRLsc);

        for (j = 0; j < 3; j++) {
            // Initialize White Balance gain
            pstRLsc->stRLscCaliResult[j].u16WBRGain = cmos_lsc->lsc_calib_table[j].wb_r_gain;
            pstRLsc->stRLscCaliResult[j].u16WBBGain = cmos_lsc->lsc_calib_table[j].wb_b_gain;

            // Initialize
            for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
                pstRLsc->stRLscCaliResult[j].au16RGain[i]  = cmos_lsc->lsc_calib_table[j].r_gain[i];
                pstRLsc->stRLscCaliResult[j].au16GrGain[i] = cmos_lsc->lsc_calib_table[j].gr_gain[i];
                pstRLsc->stRLscCaliResult[j].au16GbGain[i] = cmos_lsc->lsc_calib_table[j].gb_gain[i];
                pstRLsc->stRLscCaliResult[j].au16BGain[i]  = cmos_lsc->lsc_calib_table[j].b_gain[i];
            }
        }
    } else {
        // With no cmos
        pstRLsc->u8RadialScale = HI_ISP_RLSC_DEFAULT_SCALE; // default scale: 4.12

        pstRLsc->u16CenterRX  = u32Width  / 2;
        pstRLsc->u16CenterRY  = u32Height / 2;
        pstRLsc->u16CenterGrX = u32Width  / 2;
        pstRLsc->u16CenterGrY = u32Height / 2;
        pstRLsc->u16CenterGbX = u32Width  / 2;
        pstRLsc->u16CenterGbY = u32Height / 2;
        pstRLsc->u16CenterBX  = u32Width  / 2;
        pstRLsc->u16CenterBY  = u32Height / 2;

        u32OffCenter = temp / DIV_0_TO_1(pstRLsc->u16CenterRX * pstRLsc->u16CenterRX + pstRLsc->u16CenterRY * pstRLsc->u16CenterRY);

        pstRLsc->u16OffCenterR  = u32OffCenter;
        pstRLsc->u16OffCenterGr = u32OffCenter;
        pstRLsc->u16OffCenterGb = u32OffCenter;
        pstRLsc->u16OffCenterB  = u32OffCenter;

        AdjustCenterPoint(ViPipe, pstRLsc);
        CalcRadius(ViPipe, pstRLsc);

        u32DefGain = g_au16GainStrDef[pstRLsc->u8RadialScale];

        for (j = 0; j < 3; j++) {
            pstRLsc->stRLscCaliResult[j].u16WBRGain = HI_ISP_RLSC_DEFAULT_WBGAIN;
            pstRLsc->stRLscCaliResult[j].u16WBBGain = HI_ISP_RLSC_DEFAULT_WBGAIN;

            for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
                pstRLsc->stRLscCaliResult[j].au16RGain[i]  = u32DefGain;
                pstRLsc->stRLscCaliResult[j].au16GrGain[i] = u32DefGain;
                pstRLsc->stRLscCaliResult[j].au16GbGain[i] = u32DefGain;
                pstRLsc->stRLscCaliResult[j].au16BGain[i]  = u32DefGain;
            }
        }
    }

    pstRLsc->bLutUpdate     = HI_TRUE;
    pstRLsc->bLscCoefUpdate = HI_TRUE;

    pstRLsc->bLscEnable = HI_FALSE;

    return HI_SUCCESS;
}

static HI_VOID RLsc_Usr_Fw(VI_PIPE ViPipe, HI_U8 u8CurBlk, ISP_RLSC_S *pstRLsc, ISP_RLSC_USR_CFG_S *pstUsrRegCfg)
{
    RLscGetGainLut(ViPipe, pstRLsc, pstUsrRegCfg);

    pstUsrRegCfg->u8GainScale     = pstRLsc->u8RadialScale;
    pstUsrRegCfg->bLutUpdate      = HI_TRUE;

    pstUsrRegCfg->u16CenterRX     = pstRLsc->u16CenterRX;
    pstUsrRegCfg->u16CenterRY     = pstRLsc->u16CenterRY;
    pstUsrRegCfg->u16OffCenterR   = pstRLsc->u16OffCenterR;

    pstUsrRegCfg->u16CenterGrX    = pstRLsc->u16CenterGrX;
    pstUsrRegCfg->u16CenterGrY    = pstRLsc->u16CenterGrY;
    pstUsrRegCfg->u16OffCenterGr  = pstRLsc->u16OffCenterGr;

    pstUsrRegCfg->u16CenterGbX    = pstRLsc->u16CenterGbX;
    pstUsrRegCfg->u16CenterGbY    = pstRLsc->u16CenterGbY;
    pstUsrRegCfg->u16OffCenterGb  = pstRLsc->u16OffCenterGb;

    pstUsrRegCfg->u16CenterBX     = pstRLsc->u16CenterBX;
    pstUsrRegCfg->u16CenterBY     = pstRLsc->u16CenterBY;
    pstUsrRegCfg->u16OffCenterB   = pstRLsc->u16OffCenterB;

    pstUsrRegCfg->u32ValidRadius  = pstRLsc->u32ValidRadius;

    pstUsrRegCfg->u32UpdateIndex += 1;
    return;
}


static HI_S32 RLscImageSize(HI_U8 i, ISP_RLSC_USR_CFG_S *pstRLscUsrRegCfg, isp_block_attr *pstBlockAttr)
{
    isp_rect stBlockRect;

    ISP_GetBlockRect(&stBlockRect, pstBlockAttr, i);
    pstRLscUsrRegCfg->u16WidthOffset = ABS(stBlockRect.x);

    return HI_SUCCESS;
}

static __inline HI_S32 RLscImageResWrite(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < pstIspCtx->block_attr.block_num; i++) {
        RLscImageSize(i, &pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.stUsrRegCfg, &pstIspCtx->block_attr);
        pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.stUsrRegCfg.u32UpdateIndex += 1;
        pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.stUsrRegCfg.bUsrResh        = HI_TRUE;
    }

    pstRegCfg->cfg_key.bit1RLscCfg = 1;

    return HI_SUCCESS;
}

static HI_BOOL __inline CheckRLscOpen(ISP_RLSC_S *pstRLsc)
{
    return (pstRLsc->bLscEnable == HI_TRUE);
}

HI_S32 ISP_RLscInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    s32Ret = RLscCtxInit(ViPipe);

    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = RLscInitialize(ViPipe, pstRegCfg);

    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    RLscRegsInitialize(ViPipe, pstRegCfg);
    RLscExtRegsInitialize(ViPipe);

    return HI_SUCCESS;
}

HI_S32 ISP_RLscRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                   HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    ISP_RLSC_S *pstRLsc = HI_NULL;
    isp_reg_cfg *pstRegCfg  = (isp_reg_cfg *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    RLSC_GET_CTX(ViPipe, pstRLsc);
    ISP_CHECK_POINTER(pstRLsc);

    if (pstIspCtx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    pstRLsc->bLscEnable = hi_ext_system_isp_radial_shading_enable_read(ViPipe);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.bRLscEn = pstRLsc->bLscEnable;
    }

    pstRegCfg->cfg_key.bit1RLscCfg = 1;

    /* check hardware setting */
    if (!CheckRLscOpen(pstRLsc)) {
        return HI_SUCCESS;
    }

    RLscReadExtRegs(ViPipe);

    if (pstRLsc->bLscCoefUpdate) {
        for (i = 0; i < pstRegCfg->cfg_num; i++) {
            pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.stUsrRegCfg.bCoefUpdate    = HI_TRUE;
            pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.stUsrRegCfg.bRLscFuncEn    = pstRLsc->bRLscFuncEn;
            pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.stUsrRegCfg.bRadialCropEn  = pstRLsc->bRadialCropEn;
            pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.stUsrRegCfg.u16GainStr     = pstRLsc->u16RadialStrength;
        }
    }

    if (pstRLsc->bLutUpdate) {
        for (i = 0; i < pstRegCfg->cfg_num; i++) {
            RLsc_Usr_Fw(ViPipe, i, pstRLsc, &pstRegCfg->alg_reg_cfg[i].stRLscRegCfg.stUsrRegCfg);
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RLscCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    switch (u32Cmd) {
        case ISP_CHANGE_IMAGE_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            RLscImageResWrite(ViPipe, &pRegCfg->reg_cfg);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_RLscExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stRLscRegCfg.bRLscEn = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1RLscCfg = 1;

    RLscCtxExit(ViPipe);

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_rlsc(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_RLSC;
    pstAlgs->alg_func.pfn_alg_init = ISP_RLscInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_RLscRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_RLscCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_RLscExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
