/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <math.h>
#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_ISP_GE_MAX_BLK_NUM_X    (17)
#define HI_ISP_GE_MAX_BLK_NUM_Y    (15)
#define HI_MINIISP_BITDEPTH        (14)
#define ISP_GE_SLOPE_DEFAULT       (9)
#define ISP_GE_SENSI_SLOPE_DEFAULT (9)
#define ISP_GE_SENSI_THR_DEFAULT   (300)

static const  HI_U16 g_au16Threshold[ISP_AUTO_ISO_STRENGTH_NUM] = { 300, 300, 300, 300, 310, 310, 310,  310,  320, 320, 320, 320, 330, 330, 330, 330};
static const  HI_U16 g_au16Strength[ISP_AUTO_ISO_STRENGTH_NUM]  = { 128, 128, 128, 128, 129, 129, 129,  129,  130, 130, 130, 130, 131, 131, 131, 131};
static const  HI_U16 g_au16NpOffset[ISP_AUTO_ISO_STRENGTH_NUM]  = {1024, 1024, 1024, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048};

typedef struct hiISP_GE_S {
    HI_BOOL bEnable;
    HI_BOOL bCoefUpdateEn;
    HI_U8   u8GeChnNum;

    HI_S32 grgb_w;
    HI_S32 grgb_h;
    HI_U32 bitDepth;
    hi_isp_cmos_ge stCmosGe;
} ISP_GE_S;

ISP_GE_S *g_pastGeCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define GE_GET_CTX(dev, pstCtx)   (pstCtx = g_pastGeCtx[dev])
#define GE_SET_CTX(dev, pstCtx)   (g_pastGeCtx[dev] = pstCtx)
#define GE_RESET_CTX(dev)         (g_pastGeCtx[dev] = HI_NULL)

static HI_S32 GeCtxInit(VI_PIPE ViPipe)
{
    ISP_GE_S *pastGeCtx = HI_NULL;

    GE_GET_CTX(ViPipe, pastGeCtx);

    if (pastGeCtx == HI_NULL) {
        pastGeCtx = (ISP_GE_S *)ISP_MALLOC(sizeof(ISP_GE_S));
        if (pastGeCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] GeCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastGeCtx, 0, sizeof(ISP_GE_S));

    GE_SET_CTX(ViPipe, pastGeCtx);

    return HI_SUCCESS;
}

static HI_VOID GeCtxExit(VI_PIPE ViPipe)
{
    ISP_GE_S *pastGeCtx = HI_NULL;

    GE_GET_CTX(ViPipe, pastGeCtx);
    ISP_FREE(pastGeCtx);
    GE_RESET_CTX(ViPipe);
}

static HI_S32 GeCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_ge *cmos_ge)
{
    HI_U8 i;

    ISP_CHECK_BOOL(cmos_ge->enable);

    if (cmos_ge->slope > 0xE) {
        ISP_ERR_TRACE("Invalid u8Slope!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (cmos_ge->sensi_slope > 0xE) {
        ISP_ERR_TRACE("Invalid u8SensiSlope!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (cmos_ge->sensi_thr > 0x3FFF) {
        ISP_ERR_TRACE("Invalid u16SensiThr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (cmos_ge->strength[i] > 0x100) {
            ISP_ERR_TRACE("Invalid au16Strength[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_ge->np_offset[i] > 0x3FFF || cmos_ge->np_offset[i] < 0x200) {
            ISP_ERR_TRACE("Invalid au16NpOffset[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_ge->threshold[i] > 0x3FFF) {
            ISP_ERR_TRACE("Invalid au16Threshold[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 GeInitialize(VI_PIPE ViPipe)
{
    HI_S32     s32Ret;
    ISP_GE_S   *pstGe = HI_NULL;
    hi_isp_cmos_default  *sns_dft = HI_NULL;

    GE_GET_CTX(ViPipe, pstGe);
    isp_sensor_get_default(ViPipe,&sns_dft);

    if (sns_dft->key.bit1_ge) {
        ISP_CHECK_POINTER(sns_dft->ge);

        s32Ret = GeCheckCmosParam(ViPipe, sns_dft->ge);
        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        memcpy(&pstGe->stCmosGe, sns_dft->ge, sizeof(hi_isp_cmos_ge));
    } else {
        pstGe->stCmosGe.enable      = HI_TRUE;
        pstGe->stCmosGe.slope      = ISP_GE_SLOPE_DEFAULT;
        pstGe->stCmosGe.sensi_slope = ISP_GE_SENSI_SLOPE_DEFAULT;
        pstGe->stCmosGe.sensi_thr  = ISP_GE_SENSI_THR_DEFAULT;
        memcpy(pstGe->stCmosGe.strength,  g_au16Strength,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U16));
        memcpy(pstGe->stCmosGe.threshold, g_au16Threshold, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U16));
        memcpy(pstGe->stCmosGe.np_offset,  g_au16NpOffset,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U16));
    }

    pstGe->bEnable = pstGe->stCmosGe.enable;

    pstGe->grgb_w     = HI_ISP_GE_MAX_BLK_NUM_X;
    pstGe->grgb_h     = HI_ISP_GE_MAX_BLK_NUM_Y;

    pstGe->bitDepth   = HI_MINIISP_BITDEPTH;

    return HI_SUCCESS;
}

static HI_VOID GeExtRegsInitialize(VI_PIPE ViPipe)
{
    HI_U32 i;
    ISP_GE_S *pstGe = HI_NULL;

    GE_GET_CTX(ViPipe, pstGe);

    /* initial ext register of Ge */
    hi_ext_system_ge_enable_write(ViPipe, pstGe->bEnable);
    hi_ext_system_ge_slope_write(ViPipe, pstGe->stCmosGe.slope);
    hi_ext_system_ge_sensitivity_write(ViPipe, pstGe->stCmosGe.sensi_slope);
    hi_ext_system_ge_sensithreshold_write(ViPipe, pstGe->stCmosGe.sensi_thr);
    hi_ext_system_ge_coef_update_en_write(ViPipe, HI_TRUE);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        hi_ext_system_ge_strength_write(ViPipe, i, pstGe->stCmosGe.strength[i]);
        hi_ext_system_ge_npoffset_write(ViPipe, i, pstGe->stCmosGe.np_offset[i]);
        hi_ext_system_ge_threshold_write(ViPipe, i, pstGe->stCmosGe.threshold[i]);
    }
}

static HI_U8 GeGetChnNum(HI_U8 u8WDRMode)
{
    HI_U8 u8ChnNum;

    if (IS_LINEAR_MODE(u8WDRMode)) {
        u8ChnNum = 1;
    } else if (IS_BUILT_IN_WDR_MODE(u8WDRMode)) {
        u8ChnNum = 1;
    } else if (IS_2to1_WDR_MODE(u8WDRMode)) {
        u8ChnNum = 2;
    } else if (IS_3to1_WDR_MODE(u8WDRMode)) {
        u8ChnNum = 3;
    } else if (IS_4to1_WDR_MODE(u8WDRMode)) {
        u8ChnNum = 4;
    } else {
        /* unknow u8Mode */
        u8ChnNum = 1;
    }

    return u8ChnNum;
}

static HI_VOID GeStaticRegsInitialize(VI_PIPE ViPipe, ISP_GE_STATIC_CFG_S *pstGeStaticRegCfg)
{
    ISP_GE_S   *pstGe = HI_NULL;

    GE_GET_CTX(ViPipe, pstGe);

    pstGeStaticRegCfg->bGeGrGbEn   = HI_TRUE;
    pstGeStaticRegCfg->bGeGrEn     = HI_FALSE;
    pstGeStaticRegCfg->bGeGbEn     = HI_FALSE;
    pstGeStaticRegCfg->u8GeNumV    = pstGe->grgb_h;
    pstGeStaticRegCfg->bStaticResh = HI_TRUE;
}

static HI_VOID GeImageSize(VI_PIPE ViPipe, HI_U8 i, ISP_GE_USR_CFG_S *pstUsrRegCfg, isp_usr_ctx *pstIspCtx)
{
    HI_U8      u8BlkNum = pstIspCtx->block_attr.block_num;
    HI_U16     u16Overlap;
    isp_rect stBlockRect;
    ISP_GE_S   *pstGe = HI_NULL;

    GE_GET_CTX(ViPipe, pstGe);
    ISP_CHECK_POINTER_VOID(pstGe);

    u16Overlap = pstIspCtx->block_attr.over_lap;
    if (i < (pstGe->grgb_w % DIV_0_TO_1(u8BlkNum))) {
        pstUsrRegCfg->u8GeNumH = pstGe->grgb_w / DIV_0_TO_1(u8BlkNum) + 1;
    } else {
        pstUsrRegCfg->u8GeNumH = pstGe->grgb_w / DIV_0_TO_1(u8BlkNum);
    }

    ISP_GetBlockRect(&stBlockRect, &pstIspCtx->block_attr, i);

    pstUsrRegCfg->u16GeCropPosY      = 0;
    pstUsrRegCfg->u16GeCropOutHeight = stBlockRect.height;

    if (i == 0) {
        if (u8BlkNum > 1) {
            pstUsrRegCfg->u16GeCropPosX      = 0;
            pstUsrRegCfg->u16GeCropOutWidth  = stBlockRect.width - u16Overlap;
        } else {
            pstUsrRegCfg->u16GeCropPosX      = 0;
            pstUsrRegCfg->u16GeCropOutWidth  = stBlockRect.width;
        }

    } else if (i == (u8BlkNum - 1)) {
        pstUsrRegCfg->u16GeCropPosX      = u16Overlap;
        pstUsrRegCfg->u16GeCropOutWidth  = stBlockRect.width - u16Overlap;
    } else {
        pstUsrRegCfg->u16GeCropPosX      = u16Overlap;
        pstUsrRegCfg->u16GeCropOutWidth  = stBlockRect.width - (u16Overlap << 1);
    }
}

static HI_VOID GeUsrRegsInitialize(VI_PIPE ViPipe, HI_U8 i, HI_U8 u8ChnNum, ISP_GE_USR_CFG_S *pstGeUsrRegCfg, isp_usr_ctx *pstIspCtx)
{
    HI_U8 j;

    for (j = 0; j < u8ChnNum; j++) {
        pstGeUsrRegCfg->au16GeCtTh2[j]   = HI_ISP_GE_SENSITHR_DEFAULT;
        pstGeUsrRegCfg->au8GeCtSlope1[j] = HI_ISP_GE_SENSISLOPE_DEFAULT;
        pstGeUsrRegCfg->au8GeCtSlope2[j] = HI_ISP_GE_SLOPE_DEFAULT;
    }

    GeImageSize(ViPipe, i, pstGeUsrRegCfg, pstIspCtx);
    pstGeUsrRegCfg->bResh = HI_TRUE;
}

static HI_VOID GeDynaRegsInitialize(HI_U8 u8ChnNum, ISP_GE_DYNA_CFG_S *pstGeDynaRegCfg)
{
    HI_U8 i;

    for (i = 0; i < u8ChnNum; i++) {
        pstGeDynaRegCfg->au16GeCtTh1[i] = HI_ISP_GE_NPOFFSET_DEFAULT;
        pstGeDynaRegCfg->au16GeCtTh3[i] = HI_ISP_GE_THRESHOLD_DEFAULT;
    }
    pstGeDynaRegCfg->u16GeStrength = HI_ISP_GE_STRENGTH_DEFAULT;
    pstGeDynaRegCfg->bResh         = HI_TRUE;
}

static HI_VOID GeRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8      i, j;
    isp_usr_ctx  *pstIspCtx    = HI_NULL;
    ISP_GE_S   *pstGe = HI_NULL;
    HI_U8       u8GeChnNum;

    GE_GET_CTX(ViPipe, pstGe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8GeChnNum = GeGetChnNum(pstIspCtx->sns_wdr_mode);
    pstGe->u8GeChnNum = u8GeChnNum;

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        GeStaticRegsInitialize(ViPipe, &pstRegCfg->alg_reg_cfg[i].stGeRegCfg.stStaticRegCfg);
        GeUsrRegsInitialize(ViPipe, i, u8GeChnNum, &pstRegCfg->alg_reg_cfg[i].stGeRegCfg.stUsrRegCfg, pstIspCtx);
        GeDynaRegsInitialize(u8GeChnNum, &pstRegCfg->alg_reg_cfg[i].stGeRegCfg.stDynaRegCfg);
        pstRegCfg->alg_reg_cfg[i].stGeRegCfg.u8ChnNum = u8GeChnNum;

        for (j = 0; j < u8GeChnNum; j++) {
            pstRegCfg->alg_reg_cfg[i].stGeRegCfg.abGeEn[j] = pstGe->bEnable;
        }

        for (j = u8GeChnNum; j < 4; j++) {
            pstRegCfg->alg_reg_cfg[i].stGeRegCfg.abGeEn[j] = HI_FALSE;
        }
    }

    pstRegCfg->cfg_key.bit1GeCfg = 1;
}

static HI_S32 GeReadExtregs(VI_PIPE ViPipe)
{
    HI_U32 i;
    ISP_GE_S *pstGe = HI_NULL;

    GE_GET_CTX(ViPipe, pstGe);

    /* read ext register of Ge */
    pstGe->bCoefUpdateEn         = hi_ext_system_ge_coef_update_en_read(ViPipe);
    hi_ext_system_ge_coef_update_en_write(ViPipe, HI_FALSE);

    if (pstGe->bCoefUpdateEn) {
        pstGe->stCmosGe.slope      = hi_ext_system_ge_slope_read(ViPipe);
        pstGe->stCmosGe.sensi_slope = hi_ext_system_ge_sensitivity_read(ViPipe);
        pstGe->stCmosGe.sensi_thr  = hi_ext_system_ge_sensithreshold_read(ViPipe);

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            pstGe->stCmosGe.strength[i]  = hi_ext_system_ge_strength_read(ViPipe, i);
            pstGe->stCmosGe.np_offset[i]  = hi_ext_system_ge_npoffset_read(ViPipe, i);
            pstGe->stCmosGe.threshold[i] = hi_ext_system_ge_threshold_read(ViPipe, i);
        }
    }

    return HI_SUCCESS;
}

static HI_VOID Ge_Usr_Fw(ISP_GE_S *pstGe, ISP_GE_USR_CFG_S *pstGeUsrRegCfg)
{
    HI_U8 j;

    for (j = 0; j < pstGe->u8GeChnNum; j++) {
        pstGeUsrRegCfg->au16GeCtTh2[j]   = MIN2(pstGe->stCmosGe.sensi_thr, (1 << pstGe->bitDepth));
        pstGeUsrRegCfg->au8GeCtSlope1[j] = MIN2(pstGe->stCmosGe.sensi_slope, pstGe->bitDepth);
        pstGeUsrRegCfg->au8GeCtSlope2[j] = MIN2(pstGe->stCmosGe.slope, pstGe->bitDepth);
    }

    pstGeUsrRegCfg->bResh    = HI_TRUE;
}

static HI_VOID Ge_Dyna_Fw(HI_U32 u32Iso, ISP_GE_S *pstGe, ISP_GE_DYNA_CFG_S *pstGeDynaRegCfg)
{
    HI_U8 i;
    HI_U8 u8IndexUpper = GetIsoIndex(u32Iso);
    HI_U8 u8IndexLower = MAX2((HI_S8)u8IndexUpper - 1, 0);

    for (i = 0; i < pstGe->u8GeChnNum; i++) {
        pstGeDynaRegCfg->au16GeCtTh3[i] = CLIP3(LinearInter(u32Iso, g_au32IsoLut[u8IndexLower], pstGe->stCmosGe.threshold[u8IndexLower],
                                                            g_au32IsoLut[u8IndexUpper], pstGe->stCmosGe.threshold[u8IndexUpper]), 0, (1 << pstGe->bitDepth));

        pstGeDynaRegCfg->au16GeCtTh1[i] = (HI_U16)LinearInter(u32Iso, g_au32IsoLut[u8IndexLower], pstGe->stCmosGe.np_offset[u8IndexLower],
                                                              g_au32IsoLut[u8IndexUpper], pstGe->stCmosGe.np_offset[u8IndexUpper]);
    }

    pstGeDynaRegCfg->u16GeStrength = (HI_U16)LinearInter(u32Iso, g_au32IsoLut[u8IndexLower], pstGe->stCmosGe.strength[u8IndexLower],
                                                         g_au32IsoLut[u8IndexUpper], pstGe->stCmosGe.strength[u8IndexUpper]);
    pstGeDynaRegCfg->bResh = HI_TRUE;
}

static HI_BOOL __inline CheckGeOpen(ISP_GE_S *pstGe)
{
    return (pstGe->bEnable == HI_TRUE);
}

static HI_VOID Ge_Bypass(isp_reg_cfg *pstReg, ISP_GE_S *pstGe)
{
    HI_U8 i, j;

    for (i = 0; i < pstReg->cfg_num; i++) {
        for (j = 0; j < pstGe->u8GeChnNum; j++) {
            pstReg->alg_reg_cfg[i].stGeRegCfg.abGeEn[j] = HI_FALSE;
        }
    }

    pstReg->cfg_key.bit1GeCfg = 1;
}

static __inline HI_S32 GeImageResWrite(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < pstIspCtx->block_attr.block_num; i++) {
        GeImageSize(ViPipe, i, &pstRegCfg->alg_reg_cfg[i].stGeRegCfg.stUsrRegCfg, pstIspCtx);
        pstRegCfg->alg_reg_cfg[i].stGeRegCfg.stUsrRegCfg.bResh = HI_TRUE;
    }

    pstRegCfg->cfg_key.bit1GeCfg = 1;

    return HI_SUCCESS;
}

HI_S32 ISP_GeInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    s32Ret = GeCtxInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = GeInitialize(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    GeRegsInitialize(ViPipe, pstRegCfg);
    GeExtRegsInitialize(ViPipe);

    return HI_SUCCESS;
}

HI_S32 ISP_GeRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                 HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_U8 i, j;
    ISP_GE_S  *pstGe = HI_NULL;
    isp_usr_ctx *pstIspCtx  = HI_NULL;
    isp_reg_cfg *pstReg = (isp_reg_cfg *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    GE_GET_CTX(ViPipe, pstGe);
    ISP_CHECK_POINTER(pstGe);

    /* calculate every two interrupts */

    if ((pstIspCtx->frame_cnt % 2 != 0) && (pstIspCtx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    if (pstIspCtx->linkage.defect_pixel) {
        Ge_Bypass(pstReg, pstGe);
        return HI_SUCCESS;
    }

    pstGe->bEnable = hi_ext_system_ge_enable_read(ViPipe);

    for (i = 0; i < pstReg->cfg_num; i++) {
        for (j = 0; j < pstGe->u8GeChnNum; j++) {
            pstReg->alg_reg_cfg[i].stGeRegCfg.abGeEn[j] = pstGe->bEnable;
        }

        for (j = pstGe->u8GeChnNum; j < 4; j++) {
            pstReg->alg_reg_cfg[i].stGeRegCfg.abGeEn[j] = HI_FALSE;
        }
    }

    pstReg->cfg_key.bit1GeCfg = 1;

    /* check hardware setting */
    if (!CheckGeOpen(pstGe)) {
        return HI_SUCCESS;
    }

    GeReadExtregs(ViPipe);

    if (pstGe->bCoefUpdateEn) {
        for (i = 0; i < pstReg->cfg_num; i++) {
            Ge_Usr_Fw(pstGe, &pstReg->alg_reg_cfg[i].stGeRegCfg.stUsrRegCfg);
        }
    }

    for (i = 0; i < pstReg->cfg_num; i++) {
        Ge_Dyna_Fw(pstIspCtx->linkage.iso, pstGe, &pstReg->alg_reg_cfg[i].stGeRegCfg.stDynaRegCfg);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_GeCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr  *pRegCfg   = HI_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_GeInit(ViPipe, (HI_VOID *)&pRegCfg->reg_cfg);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            GeImageResWrite(ViPipe, &pRegCfg->reg_cfg);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_GeExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr  *pRegCfg   = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stGeRegCfg.abGeEn[0] = HI_FALSE;
        pRegCfg->reg_cfg.alg_reg_cfg[i].stGeRegCfg.abGeEn[1] = HI_FALSE;
        pRegCfg->reg_cfg.alg_reg_cfg[i].stGeRegCfg.abGeEn[2] = HI_FALSE;
        pRegCfg->reg_cfg.alg_reg_cfg[i].stGeRegCfg.abGeEn[3] = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1GeCfg = 1;

    GeCtxExit(ViPipe);

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_ge(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_GE;
    pstAlgs->alg_func.pfn_alg_init = ISP_GeInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_GeRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_GeCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_GeExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
