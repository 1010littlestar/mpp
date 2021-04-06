/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <stdio.h>
#include "isp_sensor.h"
#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */
#define ISP_DPCC_MODE            (35)
#define ISP_DPCC_HOT_MODE        (7)
#define ISP_DPCC_DEAD_MODE       (71)
#define ISP_DPCC_HIGHLIGHT_MODE  (160)
#define ISP_HOT_DEV_THRESH       (20)
#define ISP_DEAD_DEV_THRESH      (15)

#define ISP_DPCC_PAR_MAX_COUNT   (9)
#define ISP_DPC_SLOPE_GRADE      (5)
#define ISP_DPC_SOFT_SLOPE_GRADE (5)

static const HI_S32 g_as32IsoLutLow[16]     = {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800};
static const HI_U16 g_au16DpcStrength[16]   = {0, 0, 0, 152, 200, 200, 220, 220, 220, 220, 152, 152, 152, 152, 152, 152};
static const HI_U16 g_au16DpcBlendRatio[16] = {0, 0, 0,  0,  0,  0,  0,  0,  0,  0, 50, 50, 50, 50, 50, 50};
static const HI_U16 g_au16DpcLineThr1[27]       =   {0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0707, 0x0707, 0x0707, 0x0606, 0x0606, 0x0505, 0x0505, 0x0404, 0x0404, 0x0303, 0x0303, 0x0202, 0x0202, 0x0202, 0x0202};
static const HI_U16 g_au16DpcLineMadFac1[27]    =   {0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303};
static const HI_U16 g_au16DpcPgFac1[27]         =   {0x0404, 0x0404, 0x0404, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303};
static const HI_U16 g_au16DpcRndThr1[27]        =   {0x0a0a, 0x0a0a, 0x0a0a, 0x0a0a, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0707, 0x0606, 0x0505, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404, 0x0404};
static const HI_U16 g_au16DpcRgFac1[27]         =   {0x1f1f, 0x1e1e, 0x1d1d, 0x1d1d, 0x1d1d, 0x1b1b, 0x1919, 0x1717, 0x1515, 0x1313, 0x1111, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x0d0d, 0x0c0c, 0x0a0a, 0x0a0a, 0x0a0a, 0x0808, 0x0808, 0x0808, 0x0606, 0x0404, 0x0202};
static const HI_U16 g_au16DpcRoLimits1[27]      =   {0x0dfa, 0x0dfa, 0x0efe, 0x0efe, 0x0efe, 0x0efe, 0x0efe, 0x0efe, 0x0efe, 0x0efe, 0x0efe, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff};
static const HI_U16 g_au16DpcRndOffs1[27]       =   {0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff};
static const HI_U8  g_au8SlopeGrade[5]          =   {0, 76, 99, 100, 127};

static const HI_U16 g_au16SoftLineThr[5]     = {0x5454, 0x1818, 0x1212, 0x0a0a, 0x0a0a};
static const HI_U16 g_au16SoftLineMadFac[5]  = {0x1810, 0x1810, 0x1810, 0x1010, 0x0a0a};
static const HI_U8  g_au8SoftSlopeGrade[5]   = {0, 76, 100, 115, 120};

typedef struct hiISP_DEFECT_PIXEL_S {
    /* Public */
    HI_BOOL bEnable;                // enable dpc module
    HI_BOOL bStatEn;
    HI_U16  u16DpccMode;
    HI_U32  u32DpccBadThresh;
    // static calib
    HI_BOOL bStaCalibrationEn;      // enable static calibration
    HI_U8   u8PixelDetectType;       // 0: hot pixel detect; 1: dead pixel detect;
    HI_U8   u8FrameCnt;
    HI_U8   u8StaticDPThresh;
    HI_U8   u8TrialCount;
    HI_U8   u8TrialCntLimit;
    HI_U8   u8CalibStarted;
    HI_U8   u8CalibFinished;
    HI_U8   u8HotDevThresh;
    HI_U8   u8DeadDevThresh;
    HI_U16  u16DpCountMax;
    HI_U16  u16DpCountMin;
    HI_U16  u16BptCalibNum;
    HI_U16  au16BlkBpCalibNum[ISP_STRIPING_MAX_NUM];
    HI_U32  *pau32BptCalibTable[ISP_STRIPING_MAX_NUM]; // for calibration,max size: ISP_STRIPING_MAX_NUM * ISP_DPC_MAX_BPT_NUM_NORMAL

    // static cor
    HI_BOOL bStaticEnable;
    HI_BOOL bStaicShow;
    HI_BOOL bStaticAttrUpdate;
    HI_U16  u16BptCorNum;
    HI_U16  au16Offset[ISP_STRIPING_MAX_NUM + 1];
    HI_U16  au16OffsetForSplit[ISP_STRIPING_MAX_NUM + 1];
    // dynamic cor
    HI_BOOL bDynamicEnable;
    HI_BOOL bDynamicManual;
    HI_BOOL bSupTwinkleEn;
    HI_BOOL bDynaAttrUpdateEn;
    HI_U16  u16BlendRatio;
    HI_U16  u16Strength;
    HI_S8   s8SupTwinkleThr;
    HI_U8   u8SupTwinkleSlope;
    HI_BOOL bSupTwinkleEnAdapt;

    hi_isp_cmos_dpc stCmosDpc;
} ISP_DEFECT_PIXEL_S;

typedef struct {
    HI_U8  u8DpccSetUse;
    HI_U16 u16DpccMethodsSet1;
    HI_U16 u16DpccMethodsSet2;
    HI_U16 u16DpccMethodsSet3;
    HI_U32 u32DpccBadThresh;
} ISP_DPC_CFG_S;

static const ISP_DPC_CFG_S g_stDpcDefCfg[ISP_DPCC_PAR_MAX_COUNT] = {
    {0x0001, 0x1F1F, 0x0707, 0x1F1F, 0xff800080},  // 0~75
    {0x0003, 0x1F1F, 0x0707, 0x1F1F, 0xff800080},  // ori set 1  (76)
    {0x0003, 0x1F1F, 0x0707, 0x1F1F, 0xff800080},  // ori set 2 (99)
    {0x0007, 0x1F1F, 0x0707, 0x1F1F, 0xff800080},  // set 23(RB set3, G set2) (100)
    {0x0007, 0x1F1F, 0x0707, 0x1F1F, 0xff800080},  // 101 ~127
};

typedef struct {
    HI_U8  au8DpccLineThr[2][3];
    HI_U8  au8DpccLineMadFac[2][3];
    HI_U8  au8DpccPgFac[2][3];
    HI_U8  au8DpccRndThr[2][3];
    HI_U8  au8DpccRgFac[2][3];
    HI_U8  au8DpccRo[2][3];
    HI_U8  au8DpccRndOffs[2][3];
} ISP_DPCC_DERIVED_PARAM_S;

static const ISP_DPCC_DERIVED_PARAM_S g_stDpcDerParam[5] = {
    {{{0x54, 0x21, 0x20}, {0x54, 0x21, 0x20}}, {{0x1B, 0x18, 0x04}, {0x1B, 0x10, 0x04}}, {{0x08, 0x0B, 0x0A}, {0x08, 0x0B, 0x0A}}, {{0x0A, 0x08, 0x08}, {0x0A, 0x08, 0x06}}, {{0x26, 0x08, 0x04}, {0x26, 0x08, 0x04}}, {{0x1, 0x2, 0x2}, {0x1, 0x2, 0x1}}, {{0x2, 0x2, 0x2}, {0x2, 0x2, 0x2}}}, // 0
    {{{0x08, 0x21, 0x20}, {0x08, 0x21, 0x20}}, {{0x1B, 0x18, 0x04}, {0x1B, 0x10, 0x04}}, {{0x08, 0x0B, 0x0A}, {0x08, 0x0B, 0x0A}}, {{0x0A, 0x08, 0x08}, {0x0A, 0x08, 0x06}}, {{0x26, 0x08, 0x04}, {0x26, 0x08, 0x04}}, {{0x1, 0x2, 0x2}, {0x1, 0x2, 0x1}}, {{0x2, 0x2, 0x2}, {0x2, 0x2, 0x2}}}, // 76
    {{{0x08, 0x10, 0x20}, {0x08, 0x10, 0x20}}, {{0x04, 0x18, 0x04}, {0x04, 0x10, 0x04}}, {{0x08, 0x08, 0x0A}, {0x08, 0x06, 0x0A}}, {{0x0A, 0x08, 0x08}, {0x0A, 0x08, 0x06}}, {{0x20, 0x08, 0x04}, {0x20, 0x08, 0x04}}, {{0x2, 0x3, 0x2}, {0x2, 0x3, 0x1}}, {{0x3, 0x3, 0x3}, {0x3, 0x3, 0x3}}}, // 99
    {{{0x08, 0x10, 0x20}, {0x08, 0x10, 0x20}}, {{0x04, 0x18, 0x04}, {0x04, 0x10, 0x04}}, {{0x08, 0x08, 0x0A}, {0x08, 0x06, 0x0A}}, {{0x0A, 0x08, 0x08}, {0x0A, 0x08, 0x06}}, {{0x20, 0x08, 0x04}, {0x20, 0x08, 0x04}}, {{0x2, 0x3, 0x2}, {0x2, 0x3, 0x1}}, {{0x3, 0x3, 0x3}, {0x3, 0x3, 0x3}}}, // 100
    {{{0x01, 0x10, 0x20}, {0x01, 0x10, 0x20}}, {{0x03, 0x18, 0x04}, {0x03, 0x10, 0x04}}, {{0x03, 0x08, 0x0A}, {0x03, 0x06, 0x0A}}, {{0x04, 0x08, 0x08}, {0x04, 0x08, 0x06}}, {{0x08, 0x08, 0x04}, {0x08, 0x08, 0x04}}, {{0x2, 0x3, 0x2}, {0x2, 0x3, 0x1}}, {{0x3, 0x3, 0x3}, {0x3, 0x3, 0x3}}}, // 127
};

ISP_DEFECT_PIXEL_S *g_pastDpCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define DP_GET_CTX(dev, pstCtx)   (pstCtx = g_pastDpCtx[dev])
#define DP_SET_CTX(dev, pstCtx)   (g_pastDpCtx[dev] = pstCtx)
#define DP_RESET_CTX(dev)         (g_pastDpCtx[dev] = HI_NULL)

HI_S32 DpCtxInit(VI_PIPE ViPipe)
{
    ISP_DEFECT_PIXEL_S *pastDpCtx = HI_NULL;

    DP_GET_CTX(ViPipe, pastDpCtx);

    if (pastDpCtx == HI_NULL) {
        pastDpCtx = (ISP_DEFECT_PIXEL_S *)ISP_MALLOC(sizeof(ISP_DEFECT_PIXEL_S));
        if (pastDpCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] DpCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastDpCtx, 0, sizeof(ISP_DEFECT_PIXEL_S));

    DP_SET_CTX(ViPipe, pastDpCtx);

    return HI_SUCCESS;
}

HI_VOID DpCtxExit(VI_PIPE ViPipe)
{
    ISP_DEFECT_PIXEL_S *pastDpCtx = HI_NULL;

    DP_GET_CTX(ViPipe, pastDpCtx);
    ISP_FREE(pastDpCtx);
    DP_RESET_CTX(ViPipe);
}

HI_U8 DpGetChnNum(HI_U8 u8WDRMode)
{
    if (IS_LINEAR_MODE(u8WDRMode)) {
        return 1;
    } else if (IS_BUILT_IN_WDR_MODE(u8WDRMode)) {
        return 1;
    } else if (IS_2to1_WDR_MODE(u8WDRMode)) {
        return 2;
    } else if (IS_3to1_WDR_MODE(u8WDRMode)) {
        return 3;
    } else if (IS_4to1_WDR_MODE(u8WDRMode)) {
        return 4;
    } else {
        /* unknow u8Mode */
        return 1;
    }
}

HI_S32 Dpc_SafeFreeCalibLut(ISP_DEFECT_PIXEL_S *pstDp, HI_U8 u8Cnt)
{
    HI_U8 i;

    for (i = 0; i < u8Cnt; i++) {
        ISP_FREE(pstDp->pau32BptCalibTable[i]);
    }

    return HI_SUCCESS;
}
static HI_VOID ISP_DpEnableCfg(VI_PIPE ViPipe, HI_U8 cfg_num, isp_reg_cfg *pstRegCfg)
{
    HI_U8     i, j, u8ChnNum;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8ChnNum = DpGetChnNum(pstIspCtx->sns_wdr_mode);

    for (i = 0; i < cfg_num; i++) {
        for (j = 0; j < 4; j++) {
            pstRegCfg->alg_reg_cfg[i].stDpRegCfg.abDpcEn[j] = (j < u8ChnNum) ? (HI_TRUE) : (HI_FALSE);
        }
    }
}

static HI_VOID DpStaticRegsInitialize(ISP_DPC_STATIC_CFG_S *pstStaticRegCfg)
{
    pstStaticRegCfg->u32DpccBptCtrl    = HI_ISP_DPC_DEFAULT_BPT_CTRL;
    pstStaticRegCfg->u8DpccOutputMode  = HI_ISP_DPC_DEFAULT_OUTPUT_MODE;
    pstStaticRegCfg->bStaticResh       = HI_TRUE;

    return;
}

static HI_VOID DpUsrRegsInitialize(ISP_DPC_USR_CFG_S *pstUsrRegCfg)
{
    pstUsrRegCfg->stUsrDynaCorRegCfg.bDpccHardThrEn         = HI_ISP_DPC_DEFAULT_HARD_THR_ENABLE;
    pstUsrRegCfg->stUsrDynaCorRegCfg.s8DpccSupTwinkleThrMax = HI_ISP_DPC_DEFAULT_SOFT_THR_MAX;
    pstUsrRegCfg->stUsrDynaCorRegCfg.s8DpccSupTwinkleThrMin = HI_ISP_DPC_DEFAULT_SOFT_THR_MIN;
    pstUsrRegCfg->stUsrDynaCorRegCfg.u16DpccRakeRatio       = HI_ISP_DPC_DEFAULT_SOFT_RAKE_RATIO;
    pstUsrRegCfg->stUsrDynaCorRegCfg.bResh                  = HI_TRUE;

    pstUsrRegCfg->stUsrStaCorRegCfg.u16DpccBptNumber        = HI_ISP_DPC_DEFAULT_BPT_NUMBER;
    pstUsrRegCfg->stUsrStaCorRegCfg.u32UpdateIndex          = 1;
    pstUsrRegCfg->stUsrStaCorRegCfg.bResh                   = HI_FALSE;

    return;
}

static HI_VOID DpDynaRegsInitialize(ISP_DPC_DYNA_CFG_S *pstDynaRegCfg)
{
    pstDynaRegCfg->bResh                 = HI_TRUE;
    pstDynaRegCfg->bDpcStatEn            = 0;
    pstDynaRegCfg->u32DpccAlpha          = HI_ISP_DPC_DEFAULT_ALPHA;
    pstDynaRegCfg->u16DpccMode           = HI_ISP_DPC_DEFAULT_MODE;
    pstDynaRegCfg->u8DpccSetUse         = HI_ISP_DPC_DEFAULT_SET_USE;
    pstDynaRegCfg->u16DpccMethodsSet1    = HI_ISP_DPC_DEFAULT_METHODS_SET_1;
    pstDynaRegCfg->u16DpccMethodsSet2    = HI_ISP_DPC_DEFAULT_METHODS_SET_2;
    pstDynaRegCfg->u16DpccMethodsSet3    = HI_ISP_DPC_DEFAULT_METHODS_SET_3;
    pstDynaRegCfg->au16DpccLineThr[0]    = HI_ISP_DPC_DEFAULT_LINE_THRESH_1;
    pstDynaRegCfg->au16DpccLineMadFac[0] = HI_ISP_DPC_DEFAULT_LINE_MAD_FAC_1;
    pstDynaRegCfg->au16DpccPgFac[0]      = HI_ISP_DPC_DEFAULT_PG_FAC_1;
    pstDynaRegCfg->au16DpccRndThr[0]     = HI_ISP_DPC_DEFAULT_RND_THRESH_1;
    pstDynaRegCfg->au16DpccRgFac[0]      = HI_ISP_DPC_DEFAULT_RG_FAC_1;
    pstDynaRegCfg->au16DpccLineThr[1]    = HI_ISP_DPC_DEFAULT_LINE_THRESH_2;
    pstDynaRegCfg->au16DpccLineMadFac[1] = HI_ISP_DPC_DEFAULT_LINE_MAD_FAC_2;
    pstDynaRegCfg->au16DpccPgFac[1]      = HI_ISP_DPC_DEFAULT_PG_FAC_2;
    pstDynaRegCfg->au16DpccRndThr[1]     = HI_ISP_DPC_DEFAULT_RND_THRESH_2;
    pstDynaRegCfg->au16DpccRgFac[1]      = HI_ISP_DPC_DEFAULT_RG_FAC_2;
    pstDynaRegCfg->au16DpccLineThr[2]    = HI_ISP_DPC_DEFAULT_LINE_THRESH_3;
    pstDynaRegCfg->au16DpccLineMadFac[2] = HI_ISP_DPC_DEFAULT_LINE_MAD_FAC_3;
    pstDynaRegCfg->au16DpccPgFac[2]      = HI_ISP_DPC_DEFAULT_PG_FAC_3;
    pstDynaRegCfg->au16DpccRndThr[2]     = HI_ISP_DPC_DEFAULT_RND_THRESH_3;
    pstDynaRegCfg->au16DpccRgFac[2]      = HI_ISP_DPC_DEFAULT_RG_FAC_3;
    pstDynaRegCfg->u16DpccRoLimits       = HI_ISP_DPC_DEFAULT_RO_LIMITS;
    pstDynaRegCfg->u16DpccRndOffs        = HI_ISP_DPC_DEFAULT_RND_OFFS;
    pstDynaRegCfg->u32DpccBadThresh      = HI_ISP_DPC_DEFAULT_BPT_THRESH;

    pstDynaRegCfg->au8DpccLineStdThr[0]  = HI_ISP_DPC_DEFAULT_LINE_STD_THR_1;
    pstDynaRegCfg->au8DpccLineStdThr[1]  = HI_ISP_DPC_DEFAULT_LINE_STD_THR_2;
    pstDynaRegCfg->au8DpccLineStdThr[2]  = HI_ISP_DPC_DEFAULT_LINE_STD_THR_3;
    pstDynaRegCfg->au8DpccLineStdThr[3]  = HI_ISP_DPC_DEFAULT_LINE_STD_THR_4;
    pstDynaRegCfg->au8DpccLineStdThr[4]  = HI_ISP_DPC_DEFAULT_LINE_STD_THR_5;


    pstDynaRegCfg->au8DpccLineDiffThr[0] = HI_ISP_DPC_DEFAULT_LINE_DIFF_THR_1;
    pstDynaRegCfg->au8DpccLineDiffThr[1] = HI_ISP_DPC_DEFAULT_LINE_DIFF_THR_2;
    pstDynaRegCfg->au8DpccLineDiffThr[2] = HI_ISP_DPC_DEFAULT_LINE_DIFF_THR_3;
    pstDynaRegCfg->au8DpccLineDiffThr[3] = HI_ISP_DPC_DEFAULT_LINE_DIFF_THR_4;
    pstDynaRegCfg->au8DpccLineDiffThr[4] = HI_ISP_DPC_DEFAULT_LINE_DIFF_THR_5;

    pstDynaRegCfg->au8DpccLineAverFac[0] = HI_ISP_DPC_DEFAULT_LINE_AVER_FAC_1;
    pstDynaRegCfg->au8DpccLineAverFac[1] = HI_ISP_DPC_DEFAULT_LINE_AVER_FAC_2;
    pstDynaRegCfg->au8DpccLineAverFac[2] = HI_ISP_DPC_DEFAULT_LINE_AVER_FAC_3;
    pstDynaRegCfg->au8DpccLineAverFac[3] = HI_ISP_DPC_DEFAULT_LINE_AVER_FAC_4;
    pstDynaRegCfg->au8DpccLineAverFac[4] = HI_ISP_DPC_DEFAULT_LINE_AVER_FAC_5;

    pstDynaRegCfg->u8DpccLineKerdiffFac  = HI_ISP_DPC_DEFAULT_LINE_KERDIFF_FAC;
    pstDynaRegCfg->u8DpccBlendMode       = HI_ISP_DPC_DEFAULT_BLEND_MODE;
    pstDynaRegCfg->u8DpccBitDepthSel     = HI_ISP_DPC_DEFAULT_BIT_DEPTH_SEL;

    return;
}

static HI_VOID DpRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8     i;

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        DpStaticRegsInitialize(&pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stStaticRegCfg);
        DpDynaRegsInitialize(&pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stDynaRegCfg);
        DpUsrRegsInitialize(&pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stUsrRegCfg);
    }

    ISP_DpEnableCfg(ViPipe, pstRegCfg->cfg_num, pstRegCfg);

    pstRegCfg->cfg_key.bit1DpCfg = 1;

    return;
}

static HI_VOID DpExtRegsInitialize(VI_PIPE ViPipe)
{
    HI_U8 i;
    ISP_DEFECT_PIXEL_S *pstDp     = HI_NULL;

    DP_GET_CTX(ViPipe,  pstDp);
    ISP_CHECK_POINTER_VOID(pstDp);

    // dynamic attr
    for (i = 0; i < 16; i++) {
        hi_ext_system_dpc_dynamic_strength_table_write(ViPipe, i, pstDp->stCmosDpc.strength[i]);
        hi_ext_system_dpc_dynamic_blend_ratio_table_write(ViPipe, i, pstDp->stCmosDpc.blend_ratio[i]);
    }

    hi_ext_system_dpc_dynamic_cor_enable_write(ViPipe, pstDp->bEnable);
    hi_ext_system_dpc_manual_mode_write(ViPipe, HI_EXT_SYSTEM_DPC_MANU_MODE_DEFAULT);
    hi_ext_system_dpc_dynamic_manual_enable_write(ViPipe, HI_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DEFAULT);
    hi_ext_system_dpc_dynamic_strength_write(ViPipe, HI_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DEFAULT);
    hi_ext_system_dpc_dynamic_blend_ratio_write(ViPipe, HI_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DEFAULT);
    hi_ext_system_dpc_suppress_twinkle_enable_write(ViPipe, HI_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_ENABLE_DEFAULT);
    hi_ext_system_dpc_suppress_twinkle_thr_write(ViPipe, HI_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_THR_DEFAULT);
    hi_ext_system_dpc_suppress_twinkle_slope_write(ViPipe, HI_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_SLOPE_DEFAULT);
    hi_ext_system_dpc_dynamic_attr_update_write(ViPipe, HI_TRUE);

    // static calib
    hi_ext_system_dpc_static_calib_enable_write(ViPipe, HI_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DEFAULT);
    hi_ext_system_dpc_count_max_write(ViPipe, HI_EXT_SYSTEM_DPC_COUNT_MAX_DEFAULT);
    hi_ext_system_dpc_count_min_write(ViPipe, HI_EXT_SYSTEM_DPC_COUNT_MIN_DEFAULT);
    hi_ext_system_dpc_start_thresh_write(ViPipe, HI_EXT_SYSTEM_DPC_START_THRESH_DEFAULT);
    hi_ext_system_dpc_trigger_status_write(ViPipe, HI_EXT_SYSTEM_DPC_TRIGGER_STATUS_DEFAULT);
    hi_ext_system_dpc_trigger_time_write(ViPipe, HI_EXT_SYSTEM_DPC_TRIGGER_TIME_DEFAULT);
    hi_ext_system_dpc_static_defect_type_write(ViPipe, HI_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DEFAULT);
    hi_ext_system_dpc_finish_thresh_write(ViPipe, HI_EXT_SYSTEM_DPC_START_THRESH_DEFAULT);
    hi_ext_system_dpc_bpt_calib_number_write(ViPipe, HI_EXT_SYSTEM_DPC_BPT_CALIB_NUMBER_DEFAULT);
    // static attr
    hi_ext_system_dpc_bpt_cor_number_write(ViPipe, HI_EXT_SYSTEM_DPC_BPT_COR_NUMBER_DEFAULT);
    hi_ext_system_dpc_static_cor_enable_write(ViPipe, pstDp->bEnable);
    hi_ext_system_dpc_static_dp_show_write(ViPipe, HI_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DEFAULT);
    hi_ext_system_dpc_static_attr_update_write(ViPipe, HI_TRUE);

    // debug
    hi_ext_system_dpc_alpha0_rb_write(ViPipe, HI_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_RB_DEFAULT);
    hi_ext_system_dpc_alpha0_g_write(ViPipe,  HI_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_G_DEFAULT);

    return;
}

static HI_S32 DpcImageSize(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp, isp_reg_cfg *pstRegCfg)
{
    HI_U8      i;
    isp_rect stBlockRect;
    isp_usr_ctx  *pstIspCtx  = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        ISP_GetBlockRect(&stBlockRect, &pstIspCtx->block_attr, i);

        pstDp->au16Offset[i] = stBlockRect.x;
        pstDp->au16OffsetForSplit[i] = (i == 0) ? 0 : (pstDp->au16Offset[i] + pstIspCtx->block_attr.over_lap);
    }

    pstDp->au16OffsetForSplit[pstRegCfg->cfg_num] = pstIspCtx->block_attr.frame_rect.width;
    pstDp->au16Offset[pstRegCfg->cfg_num]         = pstIspCtx->block_attr.frame_rect.width;

    return HI_SUCCESS;
}

static HI_S32 DpcCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_dpc *cmos_dpc)
{
    HI_U8 i;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (cmos_dpc->strength[i] > 255) {
            ISP_ERR_TRACE("Invalid au16Strength[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_dpc->blend_ratio[i] > 0x80) {
            ISP_ERR_TRACE("Invalid au16BlendRatio[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 DpInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_S32 s32Ret;
    ISP_DEFECT_PIXEL_S  *pstDp      = HI_NULL;
    isp_usr_ctx           *pstIspCtx  = HI_NULL;
    hi_isp_cmos_default  *sns_dft  = HI_NULL;

    DP_GET_CTX(ViPipe, pstDp);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    isp_sensor_get_default(ViPipe, &sns_dft);

    ISP_CHECK_POINTER(pstDp);

    if (pstIspCtx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR) {
        pstDp->bEnable = HI_FALSE;
    } else {
        pstDp->bEnable = HI_TRUE;
    }

    if (sns_dft->key.bit1_dpc) {
        ISP_CHECK_POINTER(sns_dft->dpc);

        s32Ret = DpcCheckCmosParam(ViPipe, sns_dft->dpc);
        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        memcpy(&pstDp->stCmosDpc, sns_dft->dpc, sizeof(hi_isp_cmos_dpc));
    } else {
        memcpy(pstDp->stCmosDpc.strength,   g_au16DpcStrength,   ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U16));
        memcpy(pstDp->stCmosDpc.blend_ratio, g_au16DpcBlendRatio, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U16));
    }

    pstDp->u8TrialCount     = 0;
    pstDp->u8CalibStarted   = 0;
    pstDp->u8CalibFinished  = 0;
    pstDp->bStatEn          = 0;
    pstDp->u8HotDevThresh   = ISP_HOT_DEV_THRESH;
    pstDp->u8DeadDevThresh  = ISP_DEAD_DEV_THRESH;
    pstDp->u8FrameCnt       = 0;
    pstDp->u32DpccBadThresh = 0xff800080;

    DpcImageSize(ViPipe, pstDp, pstRegCfg);

    return HI_SUCCESS;
}

static HI_S32 DpEnter(VI_PIPE ViPipe, HI_U8 u8BlkIdx, ISP_DEFECT_PIXEL_S *pstDp, isp_alg_reg_cfg  *alg_reg_cfg)
{
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;

    isp_sensor_get_blc(ViPipe, &sns_black_level);
    ISP_SensorSetPixelDetect(ViPipe, HI_TRUE);

    /* Should bypass digital gain when enter defect pixel calibration */

    alg_reg_cfg->stDgRegCfg.bDgEn                        = HI_FALSE;

    pstDp->u8StaticDPThresh = hi_ext_system_dpc_start_thresh_read(ViPipe);
    pstDp->u8CalibStarted   = 1;

    return HI_SUCCESS;
}

static HI_S32 DpExit(VI_PIPE ViPipe, HI_U8 u8BlkIdx, ISP_DEFECT_PIXEL_S *pstDp, isp_reg_cfg *pstRegCfg)
{
    DpInitialize(ViPipe, pstRegCfg);

    ISP_SensorSetPixelDetect(ViPipe, HI_FALSE);
    pstDp->u8CalibStarted  = 0;
    pstDp->u8CalibFinished = 1;

    return HI_SUCCESS;
}

static HI_S32 DpReadStaticCalibExtregs(VI_PIPE ViPipe)
{
    ISP_DEFECT_PIXEL_S *pstDp = HI_NULL;

    DP_GET_CTX(ViPipe, pstDp);
    ISP_CHECK_POINTER(pstDp);

    pstDp->u8PixelDetectType = hi_ext_system_dpc_static_defect_type_read(ViPipe);
    pstDp->u8TrialCntLimit   = (HI_U8)(hi_ext_system_dpc_trigger_time_read(ViPipe) >> 3);
    pstDp->u16DpCountMax     = hi_ext_system_dpc_count_max_read(ViPipe);
    pstDp->u16DpCountMin     = hi_ext_system_dpc_count_min_read(ViPipe);

    return HI_SUCCESS;
}

HI_S32 Dpc_Read_Calib_Num(VI_PIPE ViPipe, HI_U8 u8BlkNum, ISP_DEFECT_PIXEL_S *pstDp)
{
    HI_U8  i;
    HI_U16 u16BptCnt = 0, j;
    HI_U16 u16CntTemp = 0;
    HI_U16 au16BpCalibNum[ISP_STRIPING_MAX_NUM] = {0};
    HI_U32 u32BptValue;
    isp_usr_ctx *pstIspCtx  = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    /* u8blkNum == 1,only read au16BpCalibNum;
          u8blkNum >1,nead to exclude bad pixel in overlap area */
    if (IS_ONLINE_MODE(pstIspCtx->block_attr.running_mode)) {
        pstDp->u16BptCalibNum = isp_dpc_bpt_calib_number_read(ViPipe, 0);
    } else if (IS_SIDEBYSIDE_MODE(pstIspCtx->block_attr.running_mode)) {
        for (i = 0; i < u8BlkNum; i++) {
            ISP_CHECK_POINTER(pstDp->pau32BptCalibTable[i]);
            u16CntTemp = isp_dpc_bpt_calib_number_read(ViPipe, i);

            isp_dpc_bpt_raddr_write(ViPipe, i, 0);

            for (j = 0; j < u16CntTemp; j++) {
                u32BptValue = isp_dpc_bpt_rdata_read(ViPipe, i) + pstDp->au16Offset[i];

                if ((u32BptValue & 0x1FFF) < pstDp->au16Offset[i + 1]) {
                    pstDp->pau32BptCalibTable[i][au16BpCalibNum[i]++] = u32BptValue;
                    u16BptCnt++;
                }
            }

            pstDp->au16BlkBpCalibNum[i] = au16BpCalibNum[i];
        }

        pstDp->u16BptCalibNum = u16BptCnt;
    }

    return HI_SUCCESS;
}

HI_VOID Dpc_Calib_TimeOut(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp)
{
    printf("BAD PIXEL CALIBRATION TIME OUT  0x%x\n", pstDp->u8TrialCntLimit);
    pstDp->bStaCalibrationEn = HI_FALSE;
    hi_ext_system_dpc_static_calib_enable_write(ViPipe, HI_FALSE);
    hi_ext_system_dpc_finish_thresh_write(ViPipe, pstDp->u8StaticDPThresh);
    hi_ext_system_dpc_trigger_status_write(ViPipe, 0x2);
}

HI_VOID Dpc_Calib_Max(HI_U16 u16BadPixelsCount, ISP_DEFECT_PIXEL_S *pstDp)
{
    printf("BAD_PIXEL_COUNT_UPPER_LIMIT 0x%x, 0x%x\n", pstDp->u8StaticDPThresh, u16BadPixelsCount);
    pstDp->u8FrameCnt = 2;
    pstDp->u8TrialCount ++;
}

HI_VOID Dpc_Calib_Min(HI_U16 u16BadPixelsCount, ISP_DEFECT_PIXEL_S *pstDp)
{
    printf("BAD_PIXEL_COUNT_LOWER_LIMIT 0x%x, 0x%x\n", pstDp->u8StaticDPThresh, u16BadPixelsCount);
    pstDp->u8FrameCnt = 2;

    pstDp->u8TrialCount ++;
}

HI_S32 SortingDpCalibLut(HI_U32 *pu32Lut0, HI_U32 *pu32Lut1, HI_U16 u16Cnt0, HI_U16 u16Cnt1)
{
    HI_U16 i = 0;
    HI_U16 j = 0;
    HI_U16 u16CntSum = 0;
    HI_U32 *pu32TempLut = HI_NULL;

    pu32TempLut = (HI_U32 *)ISP_MALLOC((u16Cnt0 + u16Cnt1) * sizeof(HI_U32));

    if (pu32TempLut == HI_NULL) {
        return HI_FAILURE;
    }

    while ((i <  u16Cnt0) && (j < u16Cnt1)) {
        if (pu32Lut0[i] > (pu32Lut1[j])) {
            pu32TempLut[u16CntSum++] = pu32Lut1[j++];
        } else if (pu32Lut0[i] < (pu32Lut1[j])) {
            pu32TempLut[u16CntSum++] = pu32Lut0[i++];
        } else {
            pu32TempLut[u16CntSum++] = pu32Lut0[i];
            i++;
            j++;
        }
    }

    if (i >=  u16Cnt0) {
        while (j < u16Cnt1) {
            pu32TempLut[u16CntSum++] = pu32Lut1[j++];
        }
    }

    if (j >=  u16Cnt1) {
        while (i < u16Cnt0) {
            pu32TempLut[u16CntSum++] = pu32Lut0[i++];
        }
    }

    memcpy(pu32Lut0, pu32TempLut, u16CntSum * sizeof(HI_U32));

    ISP_FREE(pu32TempLut);

    return u16CntSum;
}

HI_S32 MergingDpCalibLut(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S  *pstDp, HI_U8 u8BlkNum)
{
    HI_U8  k;
    HI_U16 i = 0;
    HI_U16 u16BpNum = 0;
    HI_S32 s32CntTemp = 0;
    HI_U32 *pu32BpTable = HI_NULL;

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(pstDp->pau32BptCalibTable[k]);
    }

    pu32BpTable = (HI_U32 *)ISP_MALLOC(pstDp->u16BptCalibNum * sizeof(HI_U32));

    if (pu32BpTable == HI_NULL) {
        return HI_FAILURE;
    }

    memcpy(pu32BpTable, pstDp->pau32BptCalibTable[0], pstDp->au16BlkBpCalibNum[0] * sizeof(HI_U32));
    u16BpNum = pstDp->au16BlkBpCalibNum[0];

    for (k = 1; k < u8BlkNum; k++) {
        s32CntTemp = SortingDpCalibLut(pu32BpTable, pstDp->pau32BptCalibTable[k], u16BpNum, pstDp->au16BlkBpCalibNum[k]);

        if (s32CntTemp < 0) {
            ISP_FREE(pu32BpTable);
            return HI_FAILURE;
        }

        u16BpNum = s32CntTemp;
    }

    for (i = 0; i < pstDp->u16BptCalibNum; i++) {
        hi_ext_system_dpc_calib_bpt_write(ViPipe, i, pu32BpTable[i]);
    }

    ISP_FREE(pu32BpTable);

    return HI_SUCCESS;
}

HI_VOID Dpc_Calib_Success(VI_PIPE ViPipe, HI_U8 u8BlkNum, ISP_DEFECT_PIXEL_S *pstDp)
{
    HI_U16 j;

    isp_usr_ctx *pstIspCtx  = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    printf("trial: 0x%x, findshed: 0x%x\n", pstDp->u8TrialCount, pstDp->u16BptCalibNum);

    hi_ext_system_dpc_bpt_calib_number_write(ViPipe, pstDp->u16BptCalibNum);

    if (IS_ONLINE_MODE(pstIspCtx->block_attr.running_mode)) {
        isp_dpc_bpt_raddr_write(ViPipe, 0, 0);

        for (j = 0; j < pstDp->u16BptCalibNum; j++) {
            hi_ext_system_dpc_calib_bpt_write(ViPipe, j, isp_dpc_bpt_rdata_read(ViPipe, 0));
        }
    } else {
        MergingDpCalibLut(ViPipe, pstDp, u8BlkNum);
    }

    pstDp->u16DpccMode = (ISP_DPCC_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);
    pstDp->bStatEn     = 0;
    pstDp->bStaCalibrationEn = HI_FALSE;
    hi_ext_system_dpc_static_calib_enable_write(ViPipe, HI_FALSE);
    hi_ext_system_dpc_finish_thresh_write(ViPipe, pstDp->u8StaticDPThresh);
    hi_ext_system_dpc_trigger_status_write(ViPipe, 0x1);
}

HI_VOID Dpc_Hot_Calib(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S  *pstDp, HI_U8 u8BlkNum, isp_reg_cfg *pstRegCfg)
{
    HI_U8  i;

    if (pstDp->u8FrameCnt < 9) {
        if (pstDp->u8FrameCnt == 1) {
            hi_ext_system_dpc_trigger_status_write(ViPipe, ISP_STATE_INIT);
            for (i = 0; i < u8BlkNum; i++) {
                DpEnter(ViPipe, i, pstDp, &pstRegCfg->alg_reg_cfg[i]);
            }
        }

        pstDp->u8FrameCnt++;

        if (pstDp->u8FrameCnt == 3) {
            pstDp->u32DpccBadThresh = (pstDp->u8StaticDPThresh << 24) + (((50 + 0x80 * pstDp->u8HotDevThresh) / 100) << 16) + 0x00000080;
            pstDp->u16DpccMode      = (ISP_DPCC_HOT_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);
            pstDp->bStatEn          = 1;
        }

        /* calibrate Frame 4 */
        if (pstDp->u8FrameCnt == 5) {
            pstDp->u16DpccMode = (ISP_DPCC_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);
            pstDp->bStatEn     = 0;
        }

        if (pstDp->u8FrameCnt == 6) {
            Dpc_Read_Calib_Num(ViPipe, u8BlkNum, pstDp);

            if (pstDp->u8TrialCount >= pstDp->u8TrialCntLimit) { /* TIMEOUT */
                Dpc_Calib_TimeOut(ViPipe, pstDp);

                for (i = 0; i < u8BlkNum; i++) {
                    DpExit(ViPipe, i, pstDp, pstRegCfg);
                }
            } else if (pstDp->u16BptCalibNum > pstDp->u16DpCountMax) {
                Dpc_Calib_Max(pstDp->u16BptCalibNum, pstDp);
                if (pstDp->u8StaticDPThresh == 255) {
                    pstDp->u8TrialCount = pstDp->u8TrialCntLimit;
                }
                pstDp->u8StaticDPThresh++;
            } else if (pstDp->u16BptCalibNum < pstDp->u16DpCountMin) {
                Dpc_Calib_Min(pstDp->u16BptCalibNum, pstDp);
                if (pstDp->u8StaticDPThresh == 1) {
                    pstDp->u8TrialCount = pstDp->u8TrialCntLimit;
                }

                pstDp->u8StaticDPThresh--;
            } else { /* SUCCESS */
                Dpc_Calib_Success(ViPipe, u8BlkNum, pstDp);
                for (i = 0; i < u8BlkNum; i++) {
                    DpExit(ViPipe, i, pstDp, pstRegCfg);
                }
            }
        }
    }
}

HI_VOID Dpc_Dark_Calib(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S  *pstDp, HI_U8 u8BlkNum, isp_reg_cfg *pstRegCfg)
{
    if (pstDp->u8FrameCnt < 9) {
        if (pstDp->u8FrameCnt == 0) {
            hi_ext_system_dpc_trigger_status_write(ViPipe, ISP_STATE_INIT);
            pstDp->u8CalibStarted = 1;
            pstDp->u8StaticDPThresh = hi_ext_system_dpc_start_thresh_read(ViPipe);
        }

        pstDp->u8FrameCnt++;

        if (pstDp->u8FrameCnt == 3) {
            pstDp->u32DpccBadThresh = 0xFF800000 + (pstDp->u8StaticDPThresh << 8) + ((0x80 * pstDp->u8DeadDevThresh) / 100);
            pstDp->u16DpccMode      = (ISP_DPCC_DEAD_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);
            pstDp->bStatEn          = 1;
        }

        if (pstDp->u8FrameCnt == 5) {
            pstDp->u16DpccMode = (ISP_DPCC_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);
            pstDp->bStatEn     = 0;
        }

        if (pstDp->u8FrameCnt == 6) {
            Dpc_Read_Calib_Num(ViPipe, u8BlkNum, pstDp);

            if (pstDp->u8TrialCount >= pstDp->u8TrialCntLimit) {
                Dpc_Calib_TimeOut(ViPipe, pstDp);
                DpInitialize(ViPipe, pstRegCfg);
                pstDp->u8CalibStarted  = 0;
                pstDp->u8CalibFinished = 1;
            } else if (pstDp->u16BptCalibNum > pstDp->u16DpCountMax) {
                Dpc_Calib_Max(pstDp->u16BptCalibNum, pstDp);
                pstDp->u8StaticDPThresh--;
            } else if (pstDp->u16BptCalibNum < pstDp->u16DpCountMin) {
                Dpc_Calib_Min(pstDp->u16BptCalibNum, pstDp);
                if (pstDp->u8StaticDPThresh == 255) {
                    pstDp->u8TrialCount = pstDp->u8TrialCntLimit;
                }
                pstDp->u8StaticDPThresh++;
            } else {
                Dpc_Calib_Success(ViPipe, u8BlkNum, pstDp);
                DpInitialize(ViPipe, pstRegCfg);

                pstDp->u8CalibStarted  = 0;
                pstDp->u8CalibFinished = 1;
            }
        }
    }
}

HI_VOID ISP_Dpc_StaticCalibration(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S  *pstDp, HI_U8 u8BlkNum, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    if ((pstDp->u8PixelDetectType != 1) && (pstDp->u8PixelDetectType != 0)) {
        ISP_ERR_TRACE("invalid static defect pixel detect type!\n");
        return;
    }

    if ((pstDp->u8CalibStarted == 0) && (u8BlkNum > 1)) {
        for (i = 0; i < u8BlkNum; i++) {
            pstDp->pau32BptCalibTable[i] = (HI_U32 *)ISP_MALLOC(STATIC_DP_COUNT_NORMAL * sizeof(HI_U32));

            if (pstDp->pau32BptCalibTable[i] == HI_NULL) {
                ISP_ERR_TRACE("malloc dpc calibration table buffer failed\n");
                Dpc_SafeFreeCalibLut(pstDp, u8BlkNum);
                return;
            }
        }
    }

    pstIspCtx->linkage.defect_pixel = HI_TRUE;

    if (pstDp->u8PixelDetectType == 0) {
        Dpc_Hot_Calib(ViPipe, pstDp, u8BlkNum, pstRegCfg);
        return;
    }

    if (pstDp->u8PixelDetectType == 1) {
        Dpc_Dark_Calib(ViPipe, pstDp, u8BlkNum, pstRegCfg);
        return;
    }
}

HI_S32 ISP_Dpc_Calib_Mode(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;

    DpReadStaticCalibExtregs(ViPipe);
    ISP_Dpc_StaticCalibration(ViPipe, pstDp, pstRegCfg->cfg_num, pstRegCfg);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stDynaRegCfg.u32DpccBadThresh  = pstDp->u32DpccBadThresh;
        pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stDynaRegCfg.u16DpccMode       = pstDp->u16DpccMode;
        pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stDynaRegCfg.bDpcStatEn        = pstDp->bStatEn;
        pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stDynaRegCfg.bResh             = HI_TRUE;
    }

    return HI_SUCCESS;
}

static HI_S32 DpReadExtregs(VI_PIPE ViPipe)
{
    HI_U16 i;
    ISP_DEFECT_PIXEL_S *pstDp = HI_NULL;

    DP_GET_CTX(ViPipe, pstDp);
    ISP_CHECK_POINTER(pstDp);

    pstDp->bStaticAttrUpdate    = hi_ext_system_dpc_static_attr_update_read(ViPipe);
    hi_ext_system_dpc_static_attr_update_write(ViPipe, HI_FALSE);

    if (pstDp->bStaticAttrUpdate) {

        pstDp->u16BptCorNum         = hi_ext_system_dpc_bpt_cor_number_read(ViPipe);
        pstDp->bStaicShow           = hi_ext_system_dpc_static_dp_show_read(ViPipe);
    }

    pstDp->bDynaAttrUpdateEn = hi_ext_system_dpc_dynamic_attr_update_read(ViPipe);
    hi_ext_system_dpc_dynamic_attr_update_write(ViPipe, HI_FALSE);

    if (pstDp->bDynaAttrUpdateEn) {
        pstDp->bDynamicManual       = hi_ext_system_dpc_dynamic_manual_enable_read(ViPipe);

        for (i = 0; i < 16; i++) {
            pstDp->stCmosDpc.strength[i]    = hi_ext_system_dpc_dynamic_strength_table_read(ViPipe, i);
            pstDp->stCmosDpc.blend_ratio[i]  = hi_ext_system_dpc_dynamic_blend_ratio_table_read(ViPipe, i);
        }

        pstDp->u16BlendRatio        = hi_ext_system_dpc_dynamic_blend_ratio_read(ViPipe);
        pstDp->u16Strength          = hi_ext_system_dpc_dynamic_strength_read(ViPipe);
        pstDp->bSupTwinkleEn        = hi_ext_system_dpc_suppress_twinkle_enable_read(ViPipe);
        pstDp->s8SupTwinkleThr      = hi_ext_system_dpc_suppress_twinkle_thr_read(ViPipe);
        pstDp->u8SupTwinkleSlope    = hi_ext_system_dpc_suppress_twinkle_slope_read(ViPipe);
    }

    return HI_SUCCESS;
}

HI_S32  SplitDpCorLut(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S  *pstDp, isp_reg_cfg *pstRegCfg, HI_U8 u8BlkNum)
{
    HI_S8  j;
    HI_U16 au16BptNum[ISP_STRIPING_MAX_NUM] = {0};
    HI_U16 i;
    HI_U16 u16XValue;
    HI_U32 u32BptValue;

    for (j = 0; j < u8BlkNum; j++) {
        memset(pstRegCfg->alg_reg_cfg[j].stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.au32DpccBpTable, 0, STATIC_DP_COUNT_NORMAL * sizeof(HI_U32));
    }

    for (j = (HI_S8)u8BlkNum - 1; j >= 0; j--) {
        for (i = 0; i < pstDp->u16BptCorNum; i++) {
            u32BptValue = hi_ext_system_dpc_cor_bpt_read(ViPipe, i);

            u16XValue = u32BptValue & 0x1FFF;

            if ((u16XValue >= (pstDp->au16OffsetForSplit[j])) && (u16XValue < pstDp->au16OffsetForSplit[j + 1])) {
                pstRegCfg->alg_reg_cfg[j].stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.au32DpccBpTable[au16BptNum[j]] = u32BptValue - pstDp->au16Offset[j];
                au16BptNum[j]++;

                if (au16BptNum[j] >= STATIC_DP_COUNT_NORMAL) {
                    break;
                }
            }
        }
    }

    for (j = 0; j < (HI_S8)u8BlkNum; j++) {
        pstRegCfg->alg_reg_cfg[j].stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.u16DpccBptNumber = au16BptNum[j];
        pstRegCfg->alg_reg_cfg[j].stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.bResh            = HI_TRUE;
        pstRegCfg->alg_reg_cfg[j].stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.u32UpdateIndex  += 1;
    }

    return HI_SUCCESS;
}

static HI_S32 CalcRakeRatio(HI_S32 x0, HI_S32 y0, HI_S32 x1, HI_S32 y1, HI_S32 shift)
{
    if (x0 == x1) {
        return 0;
    } else {
        return ((y1 - y0) << shift) / DIV_0_TO_1(x1 - x0);
    }
}

HI_S32 ISP_Dpc_Usr_Cfg(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    ISP_DPC_USR_DYNA_COR_CFG_S *pstUsrDynaCorRegCfg;

    if (pstDp->bStaticAttrUpdate) {
        if (pstDp->bStaicShow || pstDp->bStaticEnable) {
            SplitDpCorLut(ViPipe, pstDp, pstRegCfg, pstRegCfg->cfg_num);
        }
    }

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstUsrDynaCorRegCfg = &pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stUsrRegCfg.stUsrDynaCorRegCfg;

        pstUsrDynaCorRegCfg->bDpccHardThrEn         = pstDp->bSupTwinkleEnAdapt ? (HI_FALSE) : (HI_TRUE);
        pstUsrDynaCorRegCfg->s8DpccSupTwinkleThrMax = CLIP3(pstDp->s8SupTwinkleThr, -128, 127);
        pstUsrDynaCorRegCfg->s8DpccSupTwinkleThrMin = CLIP3(pstUsrDynaCorRegCfg->s8DpccSupTwinkleThrMax - pstDp->u8SupTwinkleSlope, -128, 127);
        pstUsrDynaCorRegCfg->u16DpccRakeRatio       = CalcRakeRatio(pstUsrDynaCorRegCfg->s8DpccSupTwinkleThrMin, 0, pstUsrDynaCorRegCfg->s8DpccSupTwinkleThrMax, 128, 2);
        pstUsrDynaCorRegCfg->bResh                  = HI_TRUE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_Dpc_Show_Mode(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp, isp_reg_cfg *pstRegCfg, isp_usr_ctx *pstIspCtx)
{
    HI_U8 i;

    pstIspCtx->linkage.defect_pixel = HI_FALSE;

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stDynaRegCfg.u16DpccMode = ISP_DPCC_HIGHLIGHT_MODE;
        pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stDynaRegCfg.bResh       = HI_TRUE;
    }

    return HI_SUCCESS;
}

static HI_VOID soft_inter(ISP_DPC_DYNA_CFG_S *pstDpcHwCfg, HI_U8 u8DpccStat)
{
    HI_U8  u8StatIdxUp, u8StatIdxLow;
    HI_U8  u8StatUpper, u8StatLower;
    HI_U8  u8DpccLineThrRb1, u8DpccLineThrG1, u8DpccLineMadFacRb1, u8DpccLineMadFacG1;
    HI_U8  i;

    u8StatIdxUp = ISP_DPC_SOFT_SLOPE_GRADE - 1;
    for (i = 0; i < ISP_DPC_SOFT_SLOPE_GRADE; i++) {
        if (u8DpccStat < g_au8SoftSlopeGrade[i]) {
            u8StatIdxUp = i;
            break;
        }
    }
    u8StatIdxLow = MAX2((HI_S8)u8StatIdxUp - 1, 0);

    u8StatUpper = g_au8SoftSlopeGrade[u8StatIdxUp];
    u8StatLower = g_au8SoftSlopeGrade[u8StatIdxLow];

    u8DpccLineThrRb1 = (HI_U8)LinearInter(u8DpccStat,
                                          u8StatLower, (g_au16SoftLineThr[u8StatIdxLow] & 0xFF00) >> 8,
                                          u8StatUpper, (g_au16SoftLineThr[u8StatIdxUp] & 0xFF00) >> 8);
    u8DpccLineThrG1  = (HI_U8)LinearInter(u8DpccStat,
                                          u8StatLower, g_au16SoftLineThr[u8StatIdxLow] & 0xFF,
                                          u8StatUpper, g_au16SoftLineThr[u8StatIdxUp] & 0xFF);

    u8DpccLineMadFacRb1 = (HI_U8)LinearInter(u8DpccStat,
                                             u8StatLower, (g_au16SoftLineMadFac[u8StatIdxLow] & 0xFF00) >> 8,
                                             u8StatUpper, (g_au16SoftLineMadFac[u8StatIdxUp] & 0xFF00) >> 8);
    u8DpccLineMadFacG1 = (HI_U8)LinearInter(u8DpccStat,
                                            u8StatLower, g_au16SoftLineMadFac[u8StatIdxLow] & 0xFF,
                                            u8StatUpper, g_au16SoftLineMadFac[u8StatIdxUp] & 0xFF);
    pstDpcHwCfg->au16DpccLineThr[0]    = (((HI_U16)u8DpccLineThrRb1) << 8) + u8DpccLineThrG1;
    pstDpcHwCfg->au16DpccLineMadFac[0] = (((HI_U16)(u8DpccLineMadFacRb1 & 0x3F)) << 8) + (u8DpccLineMadFacG1 & 0x3F);
}

static HI_VOID set_dpcc_parameters_inter(ISP_DPC_DYNA_CFG_S *pstIspDpccHwCfg, HI_U8 u8DpccStat)
{
    HI_U8 i, j;
    HI_U8 u8StatUpper, u8StatLower;
    HI_U8 u8StatIdxUp, u8StatIdxLow;
    ISP_DPCC_DERIVED_PARAM_S stDpcDerParam;

    u8StatIdxUp = ISP_DPC_SLOPE_GRADE - 1;
    for (i = 0; i < ISP_DPC_SLOPE_GRADE; i++) {
        if (u8DpccStat < g_au8SlopeGrade[i]) {
            u8StatIdxUp = i;
            break;
        }
    }
    u8StatIdxLow = MAX2((HI_S8)u8StatIdxUp - 1, 0);

    u8StatUpper = g_au8SlopeGrade[u8StatIdxUp];
    u8StatLower = g_au8SlopeGrade[u8StatIdxLow];

    pstIspDpccHwCfg->u8DpccSetUse       = g_stDpcDefCfg[u8StatIdxLow].u8DpccSetUse;
    pstIspDpccHwCfg->u16DpccMethodsSet1 = g_stDpcDefCfg[u8StatIdxLow].u16DpccMethodsSet1;
    pstIspDpccHwCfg->u16DpccMethodsSet2 = g_stDpcDefCfg[u8StatIdxLow].u16DpccMethodsSet2;
    pstIspDpccHwCfg->u16DpccMethodsSet3 = g_stDpcDefCfg[u8StatIdxLow].u16DpccMethodsSet3;
    pstIspDpccHwCfg->u32DpccBadThresh   = g_stDpcDefCfg[u8StatIdxLow].u32DpccBadThresh;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            stDpcDerParam.au8DpccLineThr[i][j]    = (HI_U8)LinearInter(u8DpccStat,
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccLineThr[i][j],
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccLineThr[i][j]);
            stDpcDerParam.au8DpccLineMadFac[i][j] = (HI_U8)LinearInter(u8DpccStat,
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccLineMadFac[i][j],
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccLineMadFac[i][j]);
            stDpcDerParam.au8DpccPgFac[i][j]      = (HI_U8)LinearInter(u8DpccStat,
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccPgFac[i][j],
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccPgFac[i][j]);
            stDpcDerParam.au8DpccRgFac[i][j]      = (HI_U8)LinearInter(u8DpccStat,
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccRgFac[i][j],
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccRgFac[i][j]);
            stDpcDerParam.au8DpccRndThr[i][j]     = (HI_U8)LinearInter(u8DpccStat,
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccRndThr[i][j],
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccRndThr[i][j]);
            stDpcDerParam.au8DpccRndOffs[i][j]    = (HI_U8)LinearInter(u8DpccStat,
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccRndOffs[i][j],
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccRndOffs[i][j]);
            stDpcDerParam.au8DpccRo[i][j]         = (HI_U8)LinearInter(u8DpccStat,
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccRo[i][j],
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccRo[i][j]);
        }
    }

    for (j = 0; j < 3; j++) {
        pstIspDpccHwCfg->au16DpccLineThr[j]    = ((HI_U16)(stDpcDerParam.au8DpccLineThr[0][j]) << 8) + (stDpcDerParam.au8DpccLineThr[1][j]);
        pstIspDpccHwCfg->au16DpccLineMadFac[j] = ((HI_U16)(stDpcDerParam.au8DpccLineMadFac[0][j] & 0x3F) << 8) + (stDpcDerParam.au8DpccLineMadFac[1][j] & 0x3F);
        pstIspDpccHwCfg->au16DpccPgFac[j]      = ((HI_U16)(stDpcDerParam.au8DpccPgFac[0][j]      & 0x3F) << 8) + (stDpcDerParam.au8DpccPgFac[1][j]      & 0x3F);
        pstIspDpccHwCfg->au16DpccRndThr[j]     = ((HI_U16)(stDpcDerParam.au8DpccRndThr[0][j]) << 8) + (stDpcDerParam.au8DpccRndThr[1][j]);
        pstIspDpccHwCfg->au16DpccRgFac[j]      = ((HI_U16)(stDpcDerParam.au8DpccRgFac[0][j]      & 0x3F) << 8) + (stDpcDerParam.au8DpccRgFac[1][j]     & 0x3F);
    }

    pstIspDpccHwCfg->u16DpccRoLimits    = ((HI_U16)(stDpcDerParam.au8DpccRo[0][2] & 0x3) << 10) + ((HI_U16)(stDpcDerParam.au8DpccRo[1][2] & 0x3) << 8) + ((stDpcDerParam.au8DpccRo[0][1] & 0x3) << 6) +
                                          ((stDpcDerParam.au8DpccRo[1][1] & 0x3) << 4) + ((stDpcDerParam.au8DpccRo[0][0] & 0x3) << 2) + (stDpcDerParam.au8DpccRo[1][0] & 0x3);

    pstIspDpccHwCfg->u16DpccRndOffs     = ((HI_U16)(stDpcDerParam.au8DpccRndOffs[0][2] & 0x3) << 10) + ((HI_U16)(stDpcDerParam.au8DpccRndOffs[1][2] & 0x3) << 8) + ((stDpcDerParam.au8DpccRndOffs[0][1] & 0x3) << 6) +
                                          ((stDpcDerParam.au8DpccRndOffs[1][1] & 0x3) << 4) + ((stDpcDerParam.au8DpccRndOffs[0][0] & 0x3) << 2) + (stDpcDerParam.au8DpccRndOffs[1][0] & 0x3);

    return;
}

HI_S32 ISP_Dynamic_set(HI_S32 s32Iso, ISP_DPC_DYNA_CFG_S *pstDpcHwCfg, ISP_DEFECT_PIXEL_S *pstDpcFwCfg)
{
    HI_U8  i = 0;
    HI_U8  u8Alpha0RB = 0;
    HI_U8  u8Alpha0G  = 0;  /* the blend ratio of 5 & 9 */
    HI_U8  u8Alpha1RB = 0;
    HI_U8  u8Alpha1G  = 0;  /* the blend ratio of input data and filtered result */
    HI_U8  u8DpccStat;
    HI_U8  u8IsoIndexUpper, u8IsoIndexLower;
    HI_U16 u16BlendRatio = 0x0;
    HI_U16 u16Strength   = 0;
    HI_U16 u16DpccMode   = pstDpcFwCfg->u16DpccMode;
    hi_isp_cmos_dpc *pstDpc = &pstDpcFwCfg->stCmosDpc;

    if (pstDpcFwCfg->bDynamicManual) {
        u16Strength = pstDpcFwCfg->u16Strength;
        u16BlendRatio = pstDpcFwCfg->u16BlendRatio;
    } else {
        u8IsoIndexUpper = ISP_AUTO_ISO_STRENGTH_NUM - 1;

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            if (s32Iso <= g_as32IsoLutLow[i]) {
                u8IsoIndexUpper = i;
                break;
            }
        }

        u8IsoIndexLower = MAX2((HI_S8)u8IsoIndexUpper - 1, 0);

        u16Strength     = (HI_U16)LinearInter(s32Iso,
                                              g_as32IsoLutLow[u8IsoIndexLower], (HI_S32)pstDpc->strength[u8IsoIndexLower],
                                              g_as32IsoLutLow[u8IsoIndexUpper], (HI_S32)pstDpc->strength[u8IsoIndexUpper]);
        u16BlendRatio   = (HI_U16)LinearInter(s32Iso,
                                              g_as32IsoLutLow[u8IsoIndexLower], (HI_S32)pstDpc->blend_ratio[u8IsoIndexLower],
                                              g_as32IsoLutLow[u8IsoIndexUpper], (HI_S32)pstDpc->blend_ratio[u8IsoIndexUpper]);
    }

    u8DpccStat = u16Strength >> 1;
    set_dpcc_parameters_inter(pstDpcHwCfg, u8DpccStat);
    if (u8DpccStat == 0) {
        u16DpccMode &= 0xFFFC;
    } else if (u8DpccStat > 100) {
        pstDpcHwCfg->u8DpccSetUse          = 0x7;
        pstDpcHwCfg->u16DpccMethodsSet1    = 0x1f1f;
        pstDpcHwCfg->au16DpccLineThr[0]    = g_au16DpcLineThr1[u8DpccStat - 101];
        pstDpcHwCfg->au16DpccLineMadFac[0] = g_au16DpcLineMadFac1[u8DpccStat - 101];
        pstDpcHwCfg->au16DpccPgFac[0]      = g_au16DpcPgFac1[u8DpccStat - 101];
        pstDpcHwCfg->au16DpccRndThr[0]     = g_au16DpcRndThr1[u8DpccStat - 101];
        pstDpcHwCfg->au16DpccRgFac[0]      = g_au16DpcRgFac1[u8DpccStat - 101];
        pstDpcHwCfg->u16DpccRoLimits       = g_au16DpcRoLimits1[u8DpccStat - 101];
        pstDpcHwCfg->u16DpccRndOffs        = g_au16DpcRndOffs1[u8DpccStat - 101];
    }

    pstDpcFwCfg->bSupTwinkleEnAdapt = pstDpcFwCfg->bSupTwinkleEn;
    if (pstDpcFwCfg->bSupTwinkleEn) {
        if ((u8DpccStat == 0) || !((pstDpcFwCfg->u16DpccMode & 0x2) >> 1)) {
            pstDpcFwCfg->bSupTwinkleEnAdapt = 0;
        } else {
            soft_inter(pstDpcHwCfg, u8DpccStat);
        }
    }

    if (!((u16DpccMode & 0x2) >> 1)) {
        u16BlendRatio = 0;
    }
    u8Alpha0RB = (u16BlendRatio > 0x80) ? (u16BlendRatio - 0x80) : 0x0;
    u8Alpha1RB = (u16BlendRatio > 0x80) ? 0x80 : u16BlendRatio;
    pstDpcHwCfg->u32DpccAlpha  = (u8Alpha0RB << 24) + (u8Alpha0G << 16) + (u8Alpha1RB << 8) + u8Alpha1G;
    pstDpcHwCfg->u16DpccMode   = u16DpccMode;

    return HI_SUCCESS;
}

HI_S32 ISP_Dpc_Normal_Mode(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp, isp_reg_cfg *pstRegCfg, isp_usr_ctx *pstIspCtx)
{
    HI_U8 i;

    pstIspCtx->linkage.defect_pixel = HI_FALSE;

    pstDp->u16DpccMode = (ISP_DPCC_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        ISP_Dynamic_set((HI_S32)pstIspCtx->linkage.iso, &pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stDynaRegCfg, pstDp);
        pstRegCfg->alg_reg_cfg[i].stDpRegCfg.stDynaRegCfg.bResh = HI_TRUE;
    }

    return HI_SUCCESS;
}

static HI_VOID ISP_DpWdrModeSet(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    isp_reg_cfg *pstRegCfg    = (isp_reg_cfg *)pRegCfg;

    ISP_DpEnableCfg(ViPipe, pstRegCfg->cfg_num, pstRegCfg);

    pstRegCfg->cfg_key.bit1DpCfg = 1;
}

HI_S32 ISP_DpInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    isp_reg_cfg *pstRegCfg    = (isp_reg_cfg *)pRegCfg;

    s32Ret = DpCtxInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = DpInitialize(ViPipe, pstRegCfg);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    DpRegsInitialize(ViPipe, pstRegCfg);
    DpExtRegsInitialize(ViPipe);

    return HI_SUCCESS;
}

HI_S32 ISP_DpRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                 HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_U8 i;
    isp_usr_ctx           *pstIspCtx  = HI_NULL;
    ISP_DEFECT_PIXEL_S  *pstDp      = HI_NULL;
    isp_reg_cfg       *pstRegCfg  = (isp_reg_cfg *)pRegCfg;

    DP_GET_CTX(ViPipe, pstDp);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_CHECK_POINTER(pstDp);

    if (pstIspCtx->linkage.stat_ready  == HI_FALSE) {
        return HI_SUCCESS;
    }

    pstRegCfg->cfg_key.bit1DpCfg = 1;

    pstDp->bStaCalibrationEn = hi_ext_system_dpc_static_calib_enable_read(ViPipe);
    pstDp->bDynamicEnable    = hi_ext_system_dpc_dynamic_cor_enable_read(ViPipe);
    pstDp->bStaticEnable     = hi_ext_system_dpc_static_cor_enable_read(ViPipe);

    if ((!pstDp->bStaCalibrationEn) && (pstDp->u8CalibStarted == 1)) {
        for (i = 0; i < pstRegCfg->cfg_num; i++) {
            DpExit(ViPipe, i, pstDp, pstRegCfg);
        }

        Dpc_SafeFreeCalibLut(pstDp, pstRegCfg->cfg_num);
    }

    if (pstDp->bStaCalibrationEn) { /* calibration mode */
        if ((pstIspCtx->block_attr.running_mode == ISP_MODE_RUNNING_OFFLINE) ||  \
            (pstIspCtx->block_attr.running_mode == ISP_MODE_RUNNING_STRIPING)) {
            return HI_FAILURE;
        }

        ISP_Dpc_Calib_Mode(ViPipe, pstDp, pstRegCfg);

        return HI_SUCCESS;
    }

    if (pstDp->u8CalibFinished == 1) {
        Dpc_SafeFreeCalibLut(pstDp, pstRegCfg->cfg_num);
    }

    DpReadExtregs(ViPipe);

    if (pstDp->bStaicShow) { /* highlight static defect pixels mode */
        ISP_Dpc_Show_Mode(ViPipe, pstDp, pstRegCfg, pstIspCtx);
    } else { /* normal detection and correction mode */
        ISP_Dpc_Normal_Mode(ViPipe, pstDp, pstRegCfg, pstIspCtx);
    }

    ISP_Dpc_Usr_Cfg(ViPipe, pstDp, pstRegCfg);

    return HI_SUCCESS;
}

HI_S32 ISP_DpCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr        *pRegCfg = HI_NULL;
    ISP_DEFECT_PIXEL_S  *pstDp   = HI_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_DpWdrModeSet(ViPipe, (HI_VOID *)&pRegCfg->reg_cfg);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            DP_GET_CTX(ViPipe, pstDp);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_CHECK_POINTER(pstDp);
            DpcImageSize(ViPipe, pstDp, &pRegCfg->reg_cfg);
            break;
        default :
            break;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DpExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stDpRegCfg.abDpcEn[0] = HI_FALSE;
        pRegCfg->reg_cfg.alg_reg_cfg[i].stDpRegCfg.abDpcEn[1] = HI_FALSE;
        pRegCfg->reg_cfg.alg_reg_cfg[i].stDpRegCfg.abDpcEn[2] = HI_FALSE;
        pRegCfg->reg_cfg.alg_reg_cfg[i].stDpRegCfg.abDpcEn[3] = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1DpCfg = 1;

    DpCtxExit(ViPipe);

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_dpc(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_DP;
    pstAlgs->alg_func.pfn_alg_init = ISP_DpInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_DpRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_DpCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_DpExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
