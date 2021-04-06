/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"
#include <math.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static const  HI_U8 g_au8AntiFalseColorThreshold[ISP_AUTO_ISO_STRENGTH_NUM] = {8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0};
static const  HI_U8 g_au8AntiFalseColorStrength[ISP_AUTO_ISO_STRENGTH_NUM]  = {8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0};

typedef struct hiISP_ANTIFALSECOLOR_S {
    HI_BOOL bEnable;                    /* RW;Range:[0x0,0x1];Format:1.0; AntiFalseColor Enable */
    HI_U8   u8WdrMode;
    HI_U8   au8LutAntiFalseColorGrayRatio[ISP_AUTO_ISO_STRENGTH_NUM];             // u5.0,
    HI_U8   au8LutAntiFalseColorCmaxSel[ISP_AUTO_ISO_STRENGTH_NUM];               // u4.0,
    HI_U8   au8LutAntiFalseColorDetgSel[ISP_AUTO_ISO_STRENGTH_NUM];               // u4.0,
    HI_U16  au16AntiFalseColorHfThreshLow[ISP_AUTO_ISO_STRENGTH_NUM];             // 10.0,
    HI_U16  au16AntiFalseColorHfThreshHig[ISP_AUTO_ISO_STRENGTH_NUM];             // 10.0,
    HI_U16  u16AntiFalseColorThr;                                                 // u12.0,

    ISP_OP_TYPE_E enOpType;
    ISP_ANTIFALSECOLOR_AUTO_ATTR_S stAuto;
    ISP_ANTIFALSECOLOR_MANUAL_ATTR_S stManual;
} ISP_ANTIFALSECOLOR_S;

ISP_ANTIFALSECOLOR_S *g_pastAntiFalseColorCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define ANTIFALSECOLOR_GET_CTX(dev, pstCtx)   (pstCtx = g_pastAntiFalseColorCtx[dev])
#define ANTIFALSECOLOR_SET_CTX(dev, pstCtx)   (g_pastAntiFalseColorCtx[dev] = pstCtx)
#define ANTIFALSECOLOR_RESET_CTX(dev)         (g_pastAntiFalseColorCtx[dev] = HI_NULL)

HI_S32 AntiFalseColorCtxInit(VI_PIPE ViPipe)
{
    ISP_ANTIFALSECOLOR_S *pastAntiFalseColorCtx = HI_NULL;

    ANTIFALSECOLOR_GET_CTX(ViPipe, pastAntiFalseColorCtx);

    if (pastAntiFalseColorCtx == HI_NULL) {
        pastAntiFalseColorCtx = (ISP_ANTIFALSECOLOR_S *)ISP_MALLOC(sizeof(ISP_ANTIFALSECOLOR_S));
        if (pastAntiFalseColorCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] AntiFalseColorCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastAntiFalseColorCtx, 0, sizeof(ISP_ANTIFALSECOLOR_S));

    ANTIFALSECOLOR_SET_CTX(ViPipe, pastAntiFalseColorCtx);

    return HI_SUCCESS;
}

HI_VOID AntiFalseColorCtxExit(VI_PIPE ViPipe)
{
    ISP_ANTIFALSECOLOR_S *pastAntiFalseColorCtx = HI_NULL;

    ANTIFALSECOLOR_GET_CTX(ViPipe, pastAntiFalseColorCtx);
    ISP_FREE(pastAntiFalseColorCtx);
    ANTIFALSECOLOR_RESET_CTX(ViPipe);
}

static HI_VOID  AntiFalseColorInitFwWdr(VI_PIPE ViPipe)
{
    ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = HI_NULL;

    HI_U8   au8LutAntiFalseColorGrayRatio[ISP_AUTO_ISO_STRENGTH_NUM] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    HI_U8   au8LutAntiFalseColorCmaxSel[ISP_AUTO_ISO_STRENGTH_NUM]   = {4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    HI_U8   au8LutAntiFalseColorDetgSel[ISP_AUTO_ISO_STRENGTH_NUM]   = {4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    HI_U16  au16AntiFalseColorHfThreshLow[ISP_AUTO_ISO_STRENGTH_NUM] = {96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96};
    HI_U16  au16AntiFalseColorHfThreshHig[ISP_AUTO_ISO_STRENGTH_NUM] = {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256};

    ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);

    memcpy(pstAntiFalseColor->au8LutAntiFalseColorGrayRatio, au8LutAntiFalseColorGrayRatio, sizeof(HI_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstAntiFalseColor->au8LutAntiFalseColorCmaxSel,   au8LutAntiFalseColorCmaxSel,   sizeof(HI_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstAntiFalseColor->au8LutAntiFalseColorDetgSel,   au8LutAntiFalseColorDetgSel,   sizeof(HI_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstAntiFalseColor->au16AntiFalseColorHfThreshLow, au16AntiFalseColorHfThreshLow, sizeof(HI_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstAntiFalseColor->au16AntiFalseColorHfThreshHig, au16AntiFalseColorHfThreshHig, sizeof(HI_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
}

static HI_VOID  AntiFalseColorInitFwLinear(VI_PIPE ViPipe)
{
    ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = HI_NULL;

    HI_U8   au8LutAntiFalseColorGrayRatio[ISP_AUTO_ISO_STRENGTH_NUM] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    HI_U8   au8LutAntiFalseColorCmaxSel[ISP_AUTO_ISO_STRENGTH_NUM]   = {4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    HI_U8   au8LutAntiFalseColorDetgSel[ISP_AUTO_ISO_STRENGTH_NUM]   = {4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    HI_U16  au16AntiFalseColorHfThreshLow[ISP_AUTO_ISO_STRENGTH_NUM] = {30, 30, 35, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48};
    HI_U16  au16AntiFalseColorHfThreshHig[ISP_AUTO_ISO_STRENGTH_NUM] = {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};

    ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);

    memcpy(pstAntiFalseColor->au8LutAntiFalseColorGrayRatio, au8LutAntiFalseColorGrayRatio, sizeof(HI_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstAntiFalseColor->au8LutAntiFalseColorCmaxSel,   au8LutAntiFalseColorCmaxSel,   sizeof(HI_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstAntiFalseColor->au8LutAntiFalseColorDetgSel,   au8LutAntiFalseColorDetgSel,   sizeof(HI_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstAntiFalseColor->au16AntiFalseColorHfThreshLow, au16AntiFalseColorHfThreshLow, sizeof(HI_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstAntiFalseColor->au16AntiFalseColorHfThreshHig, au16AntiFalseColorHfThreshHig, sizeof(HI_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
}

static HI_S32 AntiFalseColor_SetLongFrameMode(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (IS_LINEAR_MODE(pstIspCtx->sns_wdr_mode) ||
        (pstIspCtx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (pstIspCtx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        AntiFalseColorInitFwLinear(ViPipe);
    } else {
        AntiFalseColorInitFwWdr(ViPipe);
    }

    return HI_SUCCESS;
}

static HI_S32 AntiFalseColorCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_anti_false_color  *anti_false_color)
{
    HI_U8   i;

    ISP_CHECK_BOOL(anti_false_color->enable);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (anti_false_color->threshold[i] > 0x20) {
            ISP_ERR_TRACE("Invalid au8AntiFalseColorThreshold %d!\n", anti_false_color->threshold[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (anti_false_color->strength[i] > 0x1f) {
            ISP_ERR_TRACE("Invalid au8AntiFalseColorStrength %d!\n", anti_false_color->strength[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 AntiFalseColorExtRegsInitialize(VI_PIPE ViPipe)
{
    HI_U32 i;
    HI_S32 s32Ret;
    ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    isp_sensor_get_default(ViPipe, &sns_dft);
    ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);

    pstAntiFalseColor->u8WdrMode = pstIspCtx->sns_wdr_mode;

    if (pstAntiFalseColor->u8WdrMode != 0) {
        AntiFalseColorInitFwWdr(ViPipe);
    } else {
        AntiFalseColorInitFwLinear(ViPipe);
    }

    hi_ext_system_antifalsecolor_manual_mode_write(ViPipe, HI_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_MODE_DEFAULT);

    pstAntiFalseColor->stManual.u8AntiFalseColorThreshold    = HI_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_THRESHOLD_DEFAULT;
    pstAntiFalseColor->stManual.u8AntiFalseColorStrength     = HI_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_STRENGTH_DEFAULT;

    hi_ext_system_antifalsecolor_manual_threshold_write(ViPipe, pstAntiFalseColor->stManual.u8AntiFalseColorThreshold);
    hi_ext_system_antifalsecolor_manual_strenght_write(ViPipe, pstAntiFalseColor->stManual.u8AntiFalseColorStrength);

    if (sns_dft->key.bit1_anti_false_color) {
        ISP_CHECK_POINTER(sns_dft->anti_false_color);

        s32Ret = AntiFalseColorCheckCmosParam(ViPipe, sns_dft->anti_false_color);
        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        hi_ext_system_antifalsecolor_enable_write(ViPipe, sns_dft->anti_false_color->enable);

        memcpy(pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold, sns_dft->anti_false_color->threshold, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U8));
        memcpy(pstAntiFalseColor->stAuto.au8AntiFalseColorStrength,  sns_dft->anti_false_color->strength,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U8));
    } else {
        hi_ext_system_antifalsecolor_enable_write(ViPipe, HI_EXT_SYSTEM_ANTIFALSECOLOR_ENABLE_DEFAULT);
        memcpy(pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold, g_au8AntiFalseColorThreshold, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U8));
        memcpy(pstAntiFalseColor->stAuto.au8AntiFalseColorStrength,  g_au8AntiFalseColorStrength,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U8));
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        hi_ext_system_antifalsecolor_auto_threshold_write(ViPipe, i, pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold[i]);
        hi_ext_system_antifalsecolor_auto_strenght_write(ViPipe, i, pstAntiFalseColor->stAuto.au8AntiFalseColorStrength[i]);
    }

    return HI_SUCCESS;
}

static HI_VOID AntiFalseColorRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg  *pstRegCfg)
{
    HI_U32 i;
    ISP_ANTIFALSECOLOR_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_ANTIFALSECOLOR_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stAntiFalseColorRegCfg.bFcrEnable = HI_ISP_DEMOSAIC_FCR_EN_DEFAULT;
        pstStaticRegCfg = &pstRegCfg->alg_reg_cfg[i].stAntiFalseColorRegCfg.stStaticRegCfg;
        pstDynaRegCfg   = &pstRegCfg->alg_reg_cfg[i].stAntiFalseColorRegCfg.stDynaRegCfg;

        // static
        pstStaticRegCfg->bResh        = HI_TRUE;
        pstStaticRegCfg->u16FcrLimit1 = HI_ISP_DEMOSAIC_FCR_LIMIT1_DEFAULT;
        pstStaticRegCfg->u16FcrLimit2 = HI_ISP_DEMOSAIC_FCR_LIMIT2_DEFAULT;
        pstStaticRegCfg->u16FcrThr    = HI_ISP_DEMOSAIC_FCR_THR_DEFAULT;

        /* dynamic */
        pstDynaRegCfg->bResh             = HI_TRUE;
        pstDynaRegCfg->u8FcrGain         = HI_ISP_DEMOSAIC_FCR_GAIN_DEFAULT;
        pstDynaRegCfg->u8FcrRatio        = HI_ISP_DEMOSAIC_FCR_RATIO_DEFAULT;
        pstDynaRegCfg->u8FcrGrayRatio    = HI_ISP_DEMOSAIC_FCR_GRAY_RATIO_DEFAULT;
        pstDynaRegCfg->u8FcrCmaxSel      = HI_ISP_DEMOSAIC_FCR_CMAX_SEL_DEFAULT;
        pstDynaRegCfg->u8FcrDetgSel      = HI_ISP_DEMOSAIC_FCR_DETG_SEL_DEFAULT;
        pstDynaRegCfg->u16FcrHfThreshLow = HI_ISP_DEMOSAIC_FCR_HF_THRESH_LOW_DEFAULT;
        pstDynaRegCfg->u16FcrHfThreshHig = HI_ISP_DEMOSAIC_FCR_HF_THRESH_HIGH_DEFAULT;
    }

    pstRegCfg->cfg_key.bit1FcrCfg = 1;
}

static HI_S32 AntiFalseColorReadExtregs(VI_PIPE ViPipe)
{
    HI_U32 i;
    ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = HI_NULL;

    ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);

    pstAntiFalseColor->enOpType = hi_ext_system_antifalsecolor_manual_mode_read(ViPipe);

    if (pstAntiFalseColor->enOpType == OP_TYPE_AUTO) {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold[i] = hi_ext_system_antifalsecolor_auto_threshold_read(ViPipe, i);
            pstAntiFalseColor->stAuto.au8AntiFalseColorStrength[i]  = hi_ext_system_antifalsecolor_auto_strenght_read(ViPipe, i);
        }
    } else if (pstAntiFalseColor->enOpType == OP_TYPE_MANUAL) {
        pstAntiFalseColor->stManual.u8AntiFalseColorThreshold = hi_ext_system_antifalsecolor_manual_threshold_read(ViPipe);
        pstAntiFalseColor->stManual.u8AntiFalseColorStrength  = hi_ext_system_antifalsecolor_manual_strenght_read(ViPipe);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AntiFalseColorInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    isp_reg_cfg  *pstRegCfg  = (isp_reg_cfg *)pRegCfg;

    s32Ret = AntiFalseColorCtxInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    AntiFalseColorRegsInitialize(ViPipe, pstRegCfg);
    s32Ret = AntiFalseColorExtRegsInitialize(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_BOOL __inline CheckAntiFalseColorOpen(ISP_ANTIFALSECOLOR_S *pstAntiFalseColor)
{
    return (pstAntiFalseColor->bEnable == HI_TRUE);
}

static HI_VOID AntiFalseColorCfg(ISP_ANTIFALSECOLOR_DYNA_CFG_S *pstAntiFalseColorDynaCfg, ISP_ANTIFALSECOLOR_S *pstAntiFalseColor, HI_U32 u32Iso,
                                 HI_U32 u32ISO2, HI_U32 u32ISO1, HI_U8 u8IsoIndexUpper, HI_U8 u8IsoIndexLower)
{
    pstAntiFalseColorDynaCfg->u8FcrGrayRatio    = (HI_U8)LinearInter( u32Iso, u32ISO1, pstAntiFalseColor->au8LutAntiFalseColorGrayRatio[u8IsoIndexLower], \
                                                                              u32ISO2, pstAntiFalseColor->au8LutAntiFalseColorGrayRatio[u8IsoIndexUpper]);
    pstAntiFalseColorDynaCfg->u8FcrCmaxSel      = (HI_U8)LinearInter( u32Iso, u32ISO1, pstAntiFalseColor->au8LutAntiFalseColorCmaxSel[u8IsoIndexLower], \
                                                                              u32ISO2, pstAntiFalseColor->au8LutAntiFalseColorCmaxSel[u8IsoIndexUpper]);
    pstAntiFalseColorDynaCfg->u8FcrDetgSel      = (HI_U8)LinearInter( u32Iso, u32ISO1, pstAntiFalseColor->au8LutAntiFalseColorDetgSel[u8IsoIndexLower], \
                                                                              u32ISO2, pstAntiFalseColor->au8LutAntiFalseColorDetgSel[u8IsoIndexUpper]);
    pstAntiFalseColorDynaCfg->u16FcrHfThreshLow = (HI_U16)LinearInter(u32Iso, u32ISO1, pstAntiFalseColor->au16AntiFalseColorHfThreshLow[u8IsoIndexLower],\
                                                                              u32ISO2, pstAntiFalseColor->au16AntiFalseColorHfThreshLow[u8IsoIndexUpper]);
    pstAntiFalseColorDynaCfg->u16FcrHfThreshHig = (HI_U16)LinearInter(u32Iso, u32ISO1, pstAntiFalseColor->au16AntiFalseColorHfThreshHig[u8IsoIndexLower],\
                                                                              u32ISO2, pstAntiFalseColor->au16AntiFalseColorHfThreshHig[u8IsoIndexUpper]);
    pstAntiFalseColorDynaCfg->bResh = HI_TRUE;
}

static HI_S32 ISP_AntiFalseColor_Fw(HI_U32 u32Iso, VI_PIPE ViPipe, HI_U8 u8CurBlk, isp_reg_cfg *pstReg)
{
    HI_U8  u8IsoIndexUpper, u8IsoIndexLower;
    HI_U32 u32ISO1 = 0;
    HI_U32 u32ISO2 = 0;
    ISP_ANTIFALSECOLOR_DYNA_CFG_S *pstAntiFalseColorDynaCfg = &pstReg->alg_reg_cfg[u8CurBlk].stAntiFalseColorRegCfg.stDynaRegCfg;
    ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = HI_NULL;

    ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);

    u8IsoIndexUpper = GetIsoIndex(u32Iso);
    u8IsoIndexLower = MAX2((HI_S8)u8IsoIndexUpper - 1, 0);
    u32ISO1 = g_au32IsoLut[u8IsoIndexLower];
    u32ISO2 = g_au32IsoLut[u8IsoIndexUpper];

    AntiFalseColorCfg(pstAntiFalseColorDynaCfg, pstAntiFalseColor, u32Iso, u32ISO2, u32ISO1, u8IsoIndexUpper, u8IsoIndexLower);

    if (pstAntiFalseColor->enOpType == OP_TYPE_AUTO) {
        pstAntiFalseColorDynaCfg->u8FcrRatio = (HI_U8)LinearInter(u32Iso, u32ISO1, pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold[u8IsoIndexLower], \
                                                                          u32ISO2, pstAntiFalseColor->stAuto.au8AntiFalseColorThreshold[u8IsoIndexUpper]);
        pstAntiFalseColorDynaCfg->u8FcrGain  = (HI_U8)LinearInter(u32Iso, u32ISO1, pstAntiFalseColor->stAuto.au8AntiFalseColorStrength[u8IsoIndexLower],\
                                                                          u32ISO2, pstAntiFalseColor->stAuto.au8AntiFalseColorStrength[u8IsoIndexUpper]);
    } else if (pstAntiFalseColor->enOpType == OP_TYPE_MANUAL) {
        pstAntiFalseColorDynaCfg->u8FcrRatio = pstAntiFalseColor->stManual.u8AntiFalseColorThreshold;
        pstAntiFalseColorDynaCfg->u8FcrGain  = pstAntiFalseColor->stManual.u8AntiFalseColorStrength;
    }

    pstAntiFalseColorDynaCfg->bResh = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 ISP_AntiFalseColorRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo, HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_U8 i;
    isp_reg_cfg *pstReg = (isp_reg_cfg *)pRegCfg;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    ISP_ANTIFALSECOLOR_S *pstAntiFalseColor = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ANTIFALSECOLOR_GET_CTX(ViPipe, pstAntiFalseColor);
    ISP_CHECK_POINTER(pstAntiFalseColor);

    if (pstIspCtx->linkage.fswdr_mode != pstIspCtx->linkage.pre_fswdr_mode) {
        AntiFalseColor_SetLongFrameMode(ViPipe);
    }

    /* calculate every two interrupts */
    if ((pstIspCtx->frame_cnt % 2 != 0) && (pstIspCtx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    pstAntiFalseColor->bEnable = hi_ext_system_antifalsecolor_enable_read(ViPipe);

    for (i = 0; i < pstReg->cfg_num; i++) {
        pstReg->alg_reg_cfg[i].stAntiFalseColorRegCfg.bFcrEnable = pstAntiFalseColor->bEnable;
    }

    pstReg->cfg_key.bit1FcrCfg = 1;

    /* check hardware setting */
    if (!CheckAntiFalseColorOpen(pstAntiFalseColor)) {
        return HI_SUCCESS;
    }

    AntiFalseColorReadExtregs(ViPipe);

    for (i = 0; i < pstReg->cfg_num; i++) {
        ISP_AntiFalseColor_Fw(pstIspCtx->linkage.iso, ViPipe, i, pstReg);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AntiFalseColorCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr  *pRegCfg   = HI_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);

            ISP_AntiFalseColorInit(ViPipe, (HI_VOID *)&pRegCfg->reg_cfg);
            break;

        default :
            break;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AntiFalseColorExit(VI_PIPE ViPipe)
{
    HI_U16 i;
    isp_reg_cfg_attr  *pRegCfg   = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stAntiFalseColorRegCfg.bFcrEnable = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1FcrCfg = 1;

    AntiFalseColorCtxExit(ViPipe);

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_fcr(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_ANTIFALSECOLOR;
    pstAlgs->alg_func.pfn_alg_init = ISP_AntiFalseColorInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_AntiFalseColorRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_AntiFalseColorCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_AntiFalseColorExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
