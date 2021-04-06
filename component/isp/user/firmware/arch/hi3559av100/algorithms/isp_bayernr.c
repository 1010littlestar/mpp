/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <math.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_ISP_BAYERNR_BITDEP (16)
#define HI_ISP_NR_LUT_LEN     (16)
#define HI_WDR_EINIT_BLCNR    (64)

static const  HI_U32 g_au32BayernrIsoLut[ISP_AUTO_ISO_STRENGTH_NUM] = {
    100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800
};

static const  HI_U16 g_au16LutCoringRatio[HI_ISP_BAYERNR_LUT_LENGTH] = {
    60, 60, 60, 60, 65, 65, 65, 65, 70, 70, 70, 70, 70, 70, 70, 70, 80,
    80, 80, 85, 85, 85, 90, 90, 90, 95, 95, 95, 100, 100, 100, 100, 100
};
static const  HI_U8  g_au8LutFineStr[ISP_AUTO_ISO_STRENGTH_NUM] = {
    70, 70, 70, 50, 48, 37, 28, 24, 20, 20, 20, 16, 16, 16, 16, 16
};
static const  HI_U8  g_au8ChromaStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM] = {
    { 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
    { 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
    { 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
    { 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
};
static const  HI_U16 g_au16LutCoringWgt[ISP_AUTO_ISO_STRENGTH_NUM] = {
    30, 35, 40, 80, 100, 140, 200, 240, 280, 280, 300, 400, 400, 400, 400, 400
};
static const  HI_U16 g_au16CoarseStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM] = {
    { 120, 120, 120, 120, 120, 120, 120, 140, 160, 160, 180, 200, 200, 200, 200, 200 },
    { 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110 },
    { 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110 },
    { 120, 120, 120, 120, 120, 120, 120, 140, 160, 160, 180, 200, 200, 200, 200, 200 }
};
static const  HI_U8  g_au8WDRFrameStr[WDR_MAX_FRAME_NUM] = {0, 0, 0, 0};

typedef struct hiISP_NR_AUTO_S {
    HI_U8   au8ChromaStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8   au8FineStr[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U16  au16CoarseStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U16  au16CoringWgt[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_NR_AUTO_S;

typedef struct hiISP_NR_MANUAL_S {
    HI_U8   au8ChromaStr[ISP_BAYER_CHN_NUM];
    HI_U8   u8FineStr;
    HI_U16  au16CoarseStr[ISP_BAYER_CHN_NUM];
    HI_U16  u16CoringWgt;
} ISP_NR_MANUAL_S;

typedef struct hiIISP_NR_WDR_S {
    HI_U8    au8WDRFrameStr[WDR_MAX_FRAME_NUM];
} ISP_NR_WDR_S;

typedef struct hiISP_BAYERNR_S {
    HI_BOOL  bInit;
    HI_BOOL  bEnable;
    HI_BOOL  bLowPowerEnable;
    HI_BOOL  bNrLscEnable;
    HI_BOOL  bBnrMonoSensorEn;
    HI_BOOL  bTriSadEn;                         // u1.0,
    HI_BOOL  bLutUpdate;
    HI_BOOL  bSkipEnable;                       // u1.0
    HI_BOOL  bSkipLevel1Enable;                 // u1.0
    HI_BOOL  bSkipLevel2Enable;                 // u1.0
    HI_BOOL  bSkipLevel3Enable;                 // u1.0
    HI_BOOL  bSkipLevel4Enable;                 // u1.0

    HI_U8    u8NrLscRatio;
    HI_U8    u8WdrFramesMerge;
    HI_U8    u8FineStr;
    HI_U16   u16WDRBlcThr;
    HI_U16   u16CoringLow;

    HI_U8    au8JnlmLimitLut[HI_ISP_BAYERNR_LMTLUTNUM]; // u8.0
    HI_U8    au8LutChromaRatio[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8    au8LutWDRChromaRatio[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8    au8LutAmedMode[ISP_BAYER_CHN_NUM];          // u1.0
    HI_U16   au16WDRFrameThr[WDR_MAX_FRAME_NUM + 2];
    HI_U16   au16LutCoringLow[ISP_AUTO_ISO_STRENGTH_NUM];           // u14.0
    HI_U16   au16LutCoringHig[ISP_AUTO_ISO_STRENGTH_NUM];           // u14.0
    HI_U16   au16LutCoringRatio[HI_ISP_BAYERNR_LUT_LENGTH];
    HI_U16   au16LutB[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U16   au16CoarseStr[ISP_BAYER_CHN_NUM];
    HI_U32   au32JnlmLimitMultGain[2][ISP_BAYER_CHN_NUM];  // u25.0
    HI_FLOAT afExpoValues[WDR_MAX_FRAME_NUM];
    HI_U32   au32ExpoValues[WDR_MAX_FRAME_NUM];
    HI_U32   au32LutCoringHig[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U32   au32LutCoringRatio[HI_ISP_BAYERNR_LUT_LENGTH];
    HI_U16   au16WDRSyncFrameThr[CFG2VLD_DLY_LIMIT][WDR_MAX_FRAME_NUM + 2];

    ISP_OP_TYPE_E enOpType;
    ISP_NR_AUTO_S stAuto;
    ISP_NR_MANUAL_S stManual;
    ISP_NR_WDR_S  stWDR;
} ISP_BAYERNR_S;

ISP_BAYERNR_S *g_pastBayerNrCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define BAYERNR_GET_CTX(dev, pstCtx)   (pstCtx = g_pastBayerNrCtx[dev])
#define BAYERNR_SET_CTX(dev, pstCtx)   (g_pastBayerNrCtx[dev] = pstCtx)
#define BAYERNR_RESET_CTX(dev)         (g_pastBayerNrCtx[dev] = HI_NULL)

HI_S32 BayerNrCtxInit(VI_PIPE ViPipe)
{
    ISP_BAYERNR_S *pastBayerNrCtx = HI_NULL;

    BAYERNR_GET_CTX(ViPipe, pastBayerNrCtx);

    if (pastBayerNrCtx == HI_NULL) {
        pastBayerNrCtx = (ISP_BAYERNR_S *)ISP_MALLOC(sizeof(ISP_BAYERNR_S));
        if (pastBayerNrCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] BayerNrCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastBayerNrCtx, 0, sizeof(ISP_BAYERNR_S));

    BAYERNR_SET_CTX(ViPipe, pastBayerNrCtx);

    return HI_SUCCESS;
}

HI_VOID BayerNrCtxExit(VI_PIPE ViPipe)
{
    ISP_BAYERNR_S *pastBayerNrCtx = HI_NULL;

    BAYERNR_GET_CTX(ViPipe, pastBayerNrCtx);
    ISP_FREE(pastBayerNrCtx);
    BAYERNR_RESET_CTX(ViPipe);
}

static HI_VOID  NrInitFw(VI_PIPE ViPipe)
{
    ISP_BAYERNR_S *pstBayernr = HI_NULL;

    HI_U8 au8LutChromaRatio[ISP_BAYER_CHN_NUM][HI_ISP_NR_LUT_LEN] = {
        { 0, 0, 1, 2, 4, 6, 8, 9, 10, 15, 18, 20, 25, 28, 30, 32 },  // ChromaRatioR
        { 0, 0, 0, 0, 1, 4, 6, 8, 10, 15, 18, 20, 25, 28, 30, 32 },  // ChromaRatioGr
        { 0, 0, 0, 0, 1, 4, 6, 8, 10, 15, 18, 20, 25, 28, 30, 32 },  // ChromaRatioGb
        { 0, 0, 1, 2, 4, 6, 8, 9, 10, 15, 18, 20, 25, 28, 30, 32 }   // ChromaRatioB
    };
    HI_U8 au8LutWDRChromaRatio[ISP_BAYER_CHN_NUM][HI_ISP_NR_LUT_LEN] = {
        {0, 0, 0, 2, 4, 6, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10}, // ChromaRatioR
        {0, 0, 0, 2, 4, 6, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10}, // ChromaRatioGr
        {0, 0, 0, 2, 4, 6, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10}, // ChromaRatioGb
        {0, 0, 0, 2, 4, 6, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10}  // ChromaRatioB
    };
    HI_U16 au16LutCoringHig[HI_ISP_NR_LUT_LEN] = {
        3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200
    };

    BAYERNR_GET_CTX(ViPipe, pstBayernr);
    ISP_CHECK_POINTER_VOID(pstBayernr);

    memcpy(pstBayernr->au8LutChromaRatio,    au8LutChromaRatio,    sizeof(HI_U8)*HI_ISP_NR_LUT_LEN * ISP_BAYER_CHN_NUM);
    memcpy(pstBayernr->au8LutWDRChromaRatio, au8LutWDRChromaRatio, sizeof(HI_U8)*HI_ISP_NR_LUT_LEN * ISP_BAYER_CHN_NUM);
    memcpy(pstBayernr->au16LutCoringHig,     au16LutCoringHig,     sizeof(HI_U16)*HI_ISP_NR_LUT_LEN);

    return;
}

static HI_S32 BayernrCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_bayernr *bayer_nr)
{
    HI_U8 i, j;

    ISP_CHECK_BOOL(bayer_nr->enable);
    ISP_CHECK_BOOL(bayer_nr->low_power_enable);
    ISP_CHECK_BOOL(bayer_nr->bnr_mono_sensor_en);
    ISP_CHECK_BOOL(bayer_nr->nr_lsc_enable);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (bayer_nr->lut_fine_str[i] > 128) {
            ISP_ERR_TRACE("Invalid au8LutFineStr[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (bayer_nr->lut_coring_wgt[i] > 3200) {
            ISP_ERR_TRACE("Invalid au16CoringWgt[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
        if (bayer_nr->lut_coring_ratio[i] > 0x3ff) {
            ISP_ERR_TRACE("Invalid au16CoringRatio[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            if (bayer_nr->chroma_str[j][i] > 3) {
                ISP_ERR_TRACE("Invalid au8ChromaStr[%d][%d]!\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }

            if (bayer_nr->coarse_str[j][i] > 0x360) {
                ISP_ERR_TRACE("Invalid au16CoarseStr[%d][%d]!\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
    }

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        if (bayer_nr->wdr_frame_str[i] > 80) {
            ISP_ERR_TRACE("Invalid au8WDRFrameStr[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 BayernrExtRegsInitialize(VI_PIPE ViPipe)
{
    HI_U8  i;
    HI_S32 s32Ret;
    ISP_BAYERNR_S      *pstBayernr = HI_NULL;
    hi_isp_cmos_default *sns_dft  = HI_NULL;

    BAYERNR_GET_CTX(ViPipe, pstBayernr);
    isp_sensor_get_default(ViPipe, &sns_dft);

    ISP_CHECK_POINTER(pstBayernr);

    NrInitFw(ViPipe);

    hi_ext_system_bayernr_manual_mode_write(ViPipe, HI_EXT_SYSTEM_BAYERNR_MANU_MODE_DEFAULT);
    hi_ext_system_bayernr_enable_write(ViPipe, HI_EXT_SYSTEM_BAYERNR_ENABLE_DEFAULT);
    hi_ext_system_bayernr_low_power_enable_write(ViPipe, HI_EXT_SYSTEM_BAYERNR_LOW_POWER_ENABLE_DEFAULT);
    hi_ext_system_bayernr_lsc_enable_write(ViPipe, HI_EXT_SYSTEM_BAYERNR_LSC_ENABLE_DEFAULT);
    hi_ext_system_bayernr_lsc_nr_ratio_write(ViPipe, HI_EXT_SYSTEM_BAYERNR_LSC_NR_RATIO_DEFAULT);
    hi_ext_system_bayernr_mono_sensor_write(ViPipe, HI_EXT_SYSTEM_BAYERNR_MONO_SENSOR_ENABLE_DEFAULT);

    for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
        pstBayernr->au16LutCoringRatio[i] = g_au16LutCoringRatio[i];
        hi_ext_system_bayernr_coring_ratio_write(ViPipe, i, pstBayernr->au16LutCoringRatio[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstBayernr->stAuto.au8FineStr[i]       = g_au8LutFineStr[i];
        pstBayernr->stAuto.au8ChromaStr[0][i]  = g_au8ChromaStr[0][i];
        pstBayernr->stAuto.au8ChromaStr[1][i]  = g_au8ChromaStr[1][i];
        pstBayernr->stAuto.au8ChromaStr[2][i]  = g_au8ChromaStr[2][i];
        pstBayernr->stAuto.au8ChromaStr[3][i]  = g_au8ChromaStr[3][i];
        pstBayernr->stAuto.au16CoarseStr[0][i] = g_au16CoarseStr[0][i];
        pstBayernr->stAuto.au16CoarseStr[1][i] = g_au16CoarseStr[1][i];
        pstBayernr->stAuto.au16CoarseStr[2][i] = g_au16CoarseStr[2][i];
        pstBayernr->stAuto.au16CoarseStr[3][i] = g_au16CoarseStr[3][i];
        pstBayernr->stAuto.au16CoringWgt[i]    = g_au16LutCoringWgt[i];

        hi_ext_system_bayernr_auto_fine_strength_write(ViPipe, i, pstBayernr->stAuto.au8FineStr[i]);
        hi_ext_system_bayernr_auto_chroma_strength_r_write(ViPipe, i, pstBayernr->stAuto.au8ChromaStr[0][i]);
        hi_ext_system_bayernr_auto_chroma_strength_gr_write(ViPipe, i, pstBayernr->stAuto.au8ChromaStr[1][i]);
        hi_ext_system_bayernr_auto_chroma_strength_gb_write(ViPipe, i, pstBayernr->stAuto.au8ChromaStr[2][i]);
        hi_ext_system_bayernr_auto_chroma_strength_b_write(ViPipe, i, pstBayernr->stAuto.au8ChromaStr[3][i]);
        hi_ext_system_bayernr_auto_coarse_strength_r_write(ViPipe, i, pstBayernr->stAuto.au16CoarseStr[0][i]);
        hi_ext_system_bayernr_auto_coarse_strength_gr_write(ViPipe, i, pstBayernr->stAuto.au16CoarseStr[1][i]);
        hi_ext_system_bayernr_auto_coarse_strength_gb_write(ViPipe, i, pstBayernr->stAuto.au16CoarseStr[2][i]);
        hi_ext_system_bayernr_auto_coarse_strength_b_write(ViPipe, i, pstBayernr->stAuto.au16CoarseStr[3][i]);
        hi_ext_system_bayernr_auto_coring_weight_write(ViPipe, i, pstBayernr->stAuto.au16CoringWgt[i]);

    }

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        pstBayernr->stWDR.au8WDRFrameStr[i] = g_au8WDRFrameStr[i];
        hi_ext_system_bayernr_wdr_frame_strength_write(ViPipe, i, pstBayernr->stWDR.au8WDRFrameStr[i]);
    }

    // Manual
    pstBayernr->stManual.u8FineStr        = HI_EXT_SYSTEM_BAYERNR_MANU_FINE_STRENGTH_DEFAULT;
    pstBayernr->stManual.au8ChromaStr[0]  = HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT;
    pstBayernr->stManual.au8ChromaStr[1]  = HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT;
    pstBayernr->stManual.au8ChromaStr[2]  = HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT;
    pstBayernr->stManual.au8ChromaStr[3]  = HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT;
    pstBayernr->stManual.au16CoarseStr[0] = HI_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DEFAULT;
    pstBayernr->stManual.au16CoarseStr[1] = HI_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DEFAULT;
    pstBayernr->stManual.au16CoarseStr[2] = HI_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DEFAULT;
    pstBayernr->stManual.au16CoarseStr[3] = HI_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DEFAULT;
    pstBayernr->stManual.u16CoringWgt     = HI_EXT_SYSTEM_BAYERNR_MANU_CORING_WEIGHT_DEFAULT;

    hi_ext_system_bayernr_manual_fine_strength_write(ViPipe, pstBayernr->stManual.u8FineStr);
    hi_ext_system_bayernr_manual_chroma_strength_write(ViPipe, 0, pstBayernr->stManual.au8ChromaStr[0]);
    hi_ext_system_bayernr_manual_chroma_strength_write(ViPipe, 1, pstBayernr->stManual.au8ChromaStr[1]);
    hi_ext_system_bayernr_manual_chroma_strength_write(ViPipe, 2, pstBayernr->stManual.au8ChromaStr[2]);
    hi_ext_system_bayernr_manual_chroma_strength_write(ViPipe, 3, pstBayernr->stManual.au8ChromaStr[3]);
    hi_ext_system_bayernr_manual_coarse_strength_write(ViPipe, 0, pstBayernr->stManual.au16CoarseStr[0]);
    hi_ext_system_bayernr_manual_coarse_strength_write(ViPipe, 1, pstBayernr->stManual.au16CoarseStr[1]);
    hi_ext_system_bayernr_manual_coarse_strength_write(ViPipe, 2, pstBayernr->stManual.au16CoarseStr[2]);
    hi_ext_system_bayernr_manual_coarse_strength_write(ViPipe, 3, pstBayernr->stManual.au16CoarseStr[3]);
    hi_ext_system_bayernr_manual_coring_weight_write(ViPipe, pstBayernr->stManual.u16CoringWgt);

    if (sns_dft->key.bit1_bayer_nr) {
        ISP_CHECK_POINTER(sns_dft->bayer_nr);

        s32Ret = BayernrCheckCmosParam(ViPipe, sns_dft->bayer_nr);
        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        hi_ext_system_bayernr_enable_write(ViPipe, sns_dft->bayer_nr->enable);
        hi_ext_system_bayernr_low_power_enable_write(ViPipe, sns_dft->bayer_nr->low_power_enable);
        hi_ext_system_bayernr_lsc_enable_write(ViPipe, sns_dft->bayer_nr->nr_lsc_enable);
        hi_ext_system_bayernr_lsc_nr_ratio_write(ViPipe, sns_dft->bayer_nr->nr_lsc_ratio);
        hi_ext_system_bayernr_mono_sensor_write(ViPipe, sns_dft->bayer_nr->bnr_mono_sensor_en);

        for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
            pstBayernr->au16LutCoringRatio[i] = sns_dft->bayer_nr->lut_coring_ratio[i];
            hi_ext_system_bayernr_coring_ratio_write(ViPipe, i, pstBayernr->au16LutCoringRatio[i]);
        }

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {  // Auto
            pstBayernr->stAuto.au8FineStr[i]       = sns_dft->bayer_nr->lut_fine_str[i];
            pstBayernr->stAuto.au8ChromaStr[0][i]  = sns_dft->bayer_nr->chroma_str[0][i];
            pstBayernr->stAuto.au8ChromaStr[1][i]  = sns_dft->bayer_nr->chroma_str[1][i];
            pstBayernr->stAuto.au8ChromaStr[2][i]  = sns_dft->bayer_nr->chroma_str[2][i];
            pstBayernr->stAuto.au8ChromaStr[3][i]  = sns_dft->bayer_nr->chroma_str[3][i];
            pstBayernr->stAuto.au16CoarseStr[0][i] = sns_dft->bayer_nr->coarse_str[0][i];
            pstBayernr->stAuto.au16CoarseStr[1][i] = sns_dft->bayer_nr->coarse_str[1][i];
            pstBayernr->stAuto.au16CoarseStr[2][i] = sns_dft->bayer_nr->coarse_str[2][i];
            pstBayernr->stAuto.au16CoarseStr[3][i] = sns_dft->bayer_nr->coarse_str[3][i];
            pstBayernr->stAuto.au16CoringWgt[i]    = sns_dft->bayer_nr->lut_coring_wgt[i];

            hi_ext_system_bayernr_auto_fine_strength_write(ViPipe, i, pstBayernr->stAuto.au8FineStr[i]);
            hi_ext_system_bayernr_auto_chroma_strength_r_write(ViPipe, i, pstBayernr->stAuto.au8ChromaStr[0][i]);
            hi_ext_system_bayernr_auto_chroma_strength_gr_write(ViPipe, i, pstBayernr->stAuto.au8ChromaStr[1][i]);
            hi_ext_system_bayernr_auto_chroma_strength_gb_write(ViPipe, i, pstBayernr->stAuto.au8ChromaStr[2][i]);
            hi_ext_system_bayernr_auto_chroma_strength_b_write(ViPipe, i, pstBayernr->stAuto.au8ChromaStr[3][i]);
            hi_ext_system_bayernr_auto_coarse_strength_r_write(ViPipe, i, pstBayernr->stAuto.au16CoarseStr[0][i]);
            hi_ext_system_bayernr_auto_coarse_strength_gr_write(ViPipe, i, pstBayernr->stAuto.au16CoarseStr[1][i]);
            hi_ext_system_bayernr_auto_coarse_strength_gb_write(ViPipe, i, pstBayernr->stAuto.au16CoarseStr[2][i]);
            hi_ext_system_bayernr_auto_coarse_strength_b_write(ViPipe, i, pstBayernr->stAuto.au16CoarseStr[3][i]);
            hi_ext_system_bayernr_auto_coring_weight_write(ViPipe, i, pstBayernr->stAuto.au16CoringWgt[i]);
        }

        for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
            pstBayernr->stWDR.au8WDRFrameStr[i] = sns_dft->bayer_nr->wdr_frame_str[i];
            hi_ext_system_bayernr_wdr_frame_strength_write(ViPipe, i, pstBayernr->stWDR.au8WDRFrameStr[i]);
        }
    }

    pstBayernr->bInit = HI_TRUE;

    return HI_SUCCESS;
}

static HI_VOID BayernrStaticRegsInitialize(VI_PIPE ViPipe, ISP_BAYERNR_STATIC_CFG_S *pstStaticRegCfg, HI_U8 i)
{
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    isp_sensor_get_blc(ViPipe, &sns_black_level);

    pstStaticRegCfg->u16RLmtBlc          = sns_black_level->black_level[0] >> 4;
    pstStaticRegCfg->bBnrDetailEnhanceEn = HI_ISP_BNR_DEFAULT_DE_ENABLE;
    pstStaticRegCfg->bSkipEnable         = HI_ISP_BNR_DEFAULT_SKIP_ENABLE;
    pstStaticRegCfg->bSkipLevel1Enable   = HI_ISP_BNR_DEFAULT_SKIP_LEV1_ENABLE;
    pstStaticRegCfg->bSkipLevel3Enable   = HI_ISP_BNR_DEFAULT_SKIP_LEV3_ENABLE;
    pstStaticRegCfg->u8JnlmSel           = HI_ISP_BNR_DEFAULT_JNLM_SEL;
    pstStaticRegCfg->u8BnrDePosClip      = HI_ISP_BNR_DEFAULT_DE_POS_CLIP;
    pstStaticRegCfg->u8BnrDeNegClip      = HI_ISP_BNR_DEFAULT_DE_NEG_CLIP;
    pstStaticRegCfg->u8WtiSvalThr        = HI_ISP_BNR_DEFAULT_WTI_SVAL_THR;
    pstStaticRegCfg->u8WtiCoefMid        = HI_ISP_BNR_DEFAULT_WTI_MID_COEF;
    pstStaticRegCfg->u8WtiDvalThr        = HI_ISP_BNR_DEFAULT_WTI_DVAL_THR;
    pstStaticRegCfg->s16WtiDenomOffset   = HI_ISP_BNR_DEFAULT_WTI_DENOM_OFFSET;
    pstStaticRegCfg->u16WtiCoefMax       = HI_ISP_BNR_DEFAULT_WTI_MAX_COEF;
    pstStaticRegCfg->u16JnlmMaxWtCoef    = HI_ISP_BNR_DEFAULT_JNLM_MAX_WT_COEF;
    pstStaticRegCfg->u16BnrDeBlcValue    = HI_ISP_BNR_DEFAULT_DE_BLC_VALUE;

    pstStaticRegCfg->bResh = HI_TRUE;

    return;
}

static HI_VOID BayernrDynaRegsInitialize(VI_PIPE ViPipe, ISP_BAYERNR_DYNA_CFG_S *pstDynaRegCfg, isp_usr_ctx *pstIspCtx)
{
    HI_U8  u8WDRMode;
    HI_U16 j;

    u8WDRMode = pstIspCtx->sns_wdr_mode;

    ISP_BAYERNR_S *pstBayernr = HI_NULL;
    BAYERNR_GET_CTX(ViPipe, pstBayernr);
    ISP_CHECK_POINTER_VOID(pstBayernr);

    pstDynaRegCfg->bMedcEnable       = HI_TRUE;
    pstDynaRegCfg->bTriSadEn         = HI_ISP_BNR_DEFAULT_TRISAD_ENABLE;
    pstDynaRegCfg->bSkipLevel2Enable = HI_ISP_BNR_DEFAULT_SKIP_LEV2_ENABLE;
    pstDynaRegCfg->bSkipLevel4Enable = HI_ISP_BNR_DEFAULT_SKIP_LEV4_ENABLE;
    pstDynaRegCfg->au8BnrCRatio[0]   = HI_ISP_BNR_DEFAULT_C_RATIO_R;
    pstDynaRegCfg->au8BnrCRatio[1]   = HI_ISP_BNR_DEFAULT_C_RATIO_GR;
    pstDynaRegCfg->au8BnrCRatio[2]   = HI_ISP_BNR_DEFAULT_C_RATIO_GB;
    pstDynaRegCfg->au8BnrCRatio[3]   = HI_ISP_BNR_DEFAULT_C_RATIO_B;
    pstDynaRegCfg->au8AmedMode[0]    = HI_ISP_BNR_DEFAULT_AMED_MODE_R;
    pstDynaRegCfg->au8AmedMode[1]    = HI_ISP_BNR_DEFAULT_AMED_MODE_GR;
    pstDynaRegCfg->au8AmedMode[2]    = HI_ISP_BNR_DEFAULT_AMED_MODE_GB;
    pstDynaRegCfg->au8AmedMode[3]    = HI_ISP_BNR_DEFAULT_AMED_MODE_B;
    pstDynaRegCfg->au8AmedLevel[0]   = HI_ISP_BNR_DEFAULT_AMED_LEVEL_R;
    pstDynaRegCfg->au8AmedLevel[1]   = HI_ISP_BNR_DEFAULT_AMED_LEVEL_GR;
    pstDynaRegCfg->au8AmedLevel[2]   = HI_ISP_BNR_DEFAULT_AMED_LEVEL_GB;
    pstDynaRegCfg->au8AmedLevel[3]   = HI_ISP_BNR_DEFAULT_AMED_LEVEL_B;
    pstDynaRegCfg->u16LmtNpThresh    = HI_ISP_BNR_DEFAULT_NP_THRESH;
    pstDynaRegCfg->u8JnlmGain        = HI_ISP_BNR_DEFAULT_JNLM_GAIN;
    pstDynaRegCfg->u16JnlmCoringHig  = HI_ISP_BNR_DEFAULT_JNLM_CORING_HIGH;
    pstDynaRegCfg->u16RLmtRgain      = HI_ISP_BNR_DEFAULT_RLMT_RGAIN;
    pstDynaRegCfg->u16RLmtBgain      = HI_ISP_BNR_DEFAULT_RLMT_BGAIN;

    for (j = 0; j < HI_ISP_BAYERNR_LMTLUTNUM; j++) {
        pstDynaRegCfg->au8JnlmLimitLut[j] = 0;
    }
    for (j = 0; j < HI_ISP_BAYERNR_LUT_LENGTH; j++) {
        pstDynaRegCfg->au16JnlmCoringLowLUT[j] = 0;
    }

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        pstDynaRegCfg->au32JnlmLimitMultGain[0][j] = 0;
        pstDynaRegCfg->au32JnlmLimitMultGain[1][j] = 0;
    }
    pstDynaRegCfg->bBnrLutUpdateEn = HI_TRUE;

    if (IS_2to1_WDR_MODE(u8WDRMode)) {
        pstBayernr->u8WdrFramesMerge = 2;
        pstDynaRegCfg->u8JnlmSymCoef = HI_ISP_BNR_DEFAULT_JNLM_SYMCOEF_WDR;
    } else if (IS_3to1_WDR_MODE(u8WDRMode)) {
        pstBayernr->u8WdrFramesMerge = 3;
        pstDynaRegCfg->u8JnlmSymCoef = HI_ISP_BNR_DEFAULT_JNLM_SYMCOEF_WDR;
    } else if (IS_4to1_WDR_MODE(u8WDRMode)) {
        pstBayernr->u8WdrFramesMerge = 4;
        pstDynaRegCfg->u8JnlmSymCoef = HI_ISP_BNR_DEFAULT_JNLM_SYMCOEF_WDR;
    } else {
        pstBayernr->u8WdrFramesMerge = 1;
        pstDynaRegCfg->u8JnlmSymCoef = HI_ISP_BNR_DEFAULT_JNLM_SYMCOEF_LINEAR;
    }

    pstDynaRegCfg->bResh = HI_TRUE;

    return;
}

static HI_VOID BayernrUsrRegsInitialize(ISP_BAYERNR_USR_CFG_S *pstUsrRegCfg, hi_isp_cmos_default *sns_dft)
{
    ISP_CHECK_POINTER_VOID(sns_dft->bayer_nr);

    pstUsrRegCfg->bBnrLscEn        = sns_dft->bayer_nr->nr_lsc_enable;
    pstUsrRegCfg->bBnrMonoSensorEn = sns_dft->bayer_nr->bnr_mono_sensor_en;
    pstUsrRegCfg->u8BnrLscRatio    = sns_dft->bayer_nr->nr_lsc_ratio;
    pstUsrRegCfg->bResh            = HI_TRUE;

    return;
}

static HI_VOID BayernrRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8  i;
    HI_U8  u8BlockNum;
    isp_usr_ctx   *pstIspCtx = HI_NULL;
    hi_isp_cmos_default      *sns_dft        = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    ISP_BAYERNR_STATIC_CFG_S *pstStaticRegCfg  = HI_NULL;
    ISP_BAYERNR_DYNA_CFG_S   *pstDynaRegCfg    = HI_NULL;
    ISP_BAYERNR_USR_CFG_S    *pstUsrRegCfg     = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    isp_sensor_get_default(ViPipe, &sns_dft);
    isp_sensor_get_blc(ViPipe, &sns_black_level);

    u8BlockNum = pstIspCtx->block_attr.block_num;

    for (i = 0; i < u8BlockNum; i++) {
        pstStaticRegCfg = &pstRegCfg->alg_reg_cfg[i].stBnrRegCfg.stStaticRegCfg;
        pstDynaRegCfg   = &pstRegCfg->alg_reg_cfg[i].stBnrRegCfg.stDynaRegCfg;
        pstUsrRegCfg    = &pstRegCfg->alg_reg_cfg[i].stBnrRegCfg.stUsrRegCfg;

        pstRegCfg->alg_reg_cfg[i].stBnrRegCfg.bBnrEnable = HI_TRUE;
        BayernrStaticRegsInitialize(ViPipe, pstStaticRegCfg, i);
        BayernrDynaRegsInitialize(ViPipe, pstDynaRegCfg, pstIspCtx);
        BayernrUsrRegsInitialize(pstUsrRegCfg, sns_dft);
    }

    pstRegCfg->cfg_key.bit1BayernrCfg = 1;

    return;
}

static HI_S32 BayernrReadExtregs(VI_PIPE ViPipe)
{
    HI_U8 i;
    HI_U16 u16BlackLevel;
    HI_U32 au32ExpRatio[3] = {0};
    ISP_BAYERNR_S *pstBayernr = HI_NULL;
    isp_usr_ctx     *pstIspCtx  = HI_NULL;
    BAYERNR_GET_CTX(ViPipe, pstBayernr);
    ISP_CHECK_POINTER(pstBayernr);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstBayernr->enOpType           = hi_ext_system_bayernr_manual_mode_read(ViPipe);
    pstBayernr->bLowPowerEnable    = hi_ext_system_bayernr_low_power_enable_read(ViPipe);
    pstBayernr->bNrLscEnable       = hi_ext_system_bayernr_lsc_enable_read(ViPipe);
    pstBayernr->bBnrMonoSensorEn   = hi_ext_system_bayernr_mono_sensor_read(ViPipe);
    pstBayernr->u8NrLscRatio       = hi_ext_system_bayernr_lsc_nr_ratio_read(ViPipe);

    u16BlackLevel = hi_ext_system_black_level_query_00_read(ViPipe);
    memcpy(au32ExpRatio, pstIspCtx->linkage.exp_ratio_lut, sizeof(au32ExpRatio));

    switch (pstBayernr->u8WdrFramesMerge) {
        case 2:
            pstBayernr->au32ExpoValues[0] = 64;
            pstBayernr->au32ExpoValues[1] = (HI_U32)au32ExpRatio[0];
            pstBayernr->u16WDRBlcThr      = u16BlackLevel << 2;
            break;
        case 3:
            pstBayernr->au32ExpoValues[0] = 64;
            pstBayernr->au32ExpoValues[1] = (HI_U32)au32ExpRatio[0];
            pstBayernr->au32ExpoValues[2] = (HI_U32)(au32ExpRatio[0] * au32ExpRatio[1]);
            pstBayernr->u16WDRBlcThr      = u16BlackLevel << 2;
            break;
        case 4:
            pstBayernr->au32ExpoValues[0] = 64;
            pstBayernr->au32ExpoValues[1] = (HI_U32)au32ExpRatio[0];
            pstBayernr->au32ExpoValues[2] = (HI_U32)(au32ExpRatio[0] * au32ExpRatio[1]);
            pstBayernr->au32ExpoValues[3] = (HI_U32)(au32ExpRatio[0] * au32ExpRatio[1] * au32ExpRatio[2]);
            pstBayernr->u16WDRBlcThr      = u16BlackLevel << 2;
            break;
        default:
            pstBayernr->u16WDRBlcThr      = 0;
            break;
    }

    for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
        pstBayernr->au16LutCoringRatio[i] = hi_ext_system_bayernr_coring_ratio_read(ViPipe, i);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstBayernr->stAuto.au8FineStr[i]       = hi_ext_system_bayernr_auto_fine_strength_read(ViPipe, i);
        pstBayernr->stAuto.au16CoringWgt[i]    = hi_ext_system_bayernr_auto_coring_weight_read(ViPipe, i);
        pstBayernr->stAuto.au8ChromaStr[0][i]  = hi_ext_system_bayernr_auto_chroma_strength_r_read(ViPipe, i);
        pstBayernr->stAuto.au8ChromaStr[1][i]  = hi_ext_system_bayernr_auto_chroma_strength_gr_read(ViPipe, i);
        pstBayernr->stAuto.au8ChromaStr[2][i]  = hi_ext_system_bayernr_auto_chroma_strength_gb_read(ViPipe, i);
        pstBayernr->stAuto.au8ChromaStr[3][i]  = hi_ext_system_bayernr_auto_chroma_strength_b_read(ViPipe, i);
        pstBayernr->stAuto.au16CoarseStr[0][i] = hi_ext_system_bayernr_auto_coarse_strength_r_read(ViPipe, i);
        pstBayernr->stAuto.au16CoarseStr[1][i] = hi_ext_system_bayernr_auto_coarse_strength_gr_read(ViPipe, i);
        pstBayernr->stAuto.au16CoarseStr[2][i] = hi_ext_system_bayernr_auto_coarse_strength_gb_read(ViPipe, i);
        pstBayernr->stAuto.au16CoarseStr[3][i] = hi_ext_system_bayernr_auto_coarse_strength_b_read(ViPipe, i);
    }

    pstBayernr->stManual.u8FineStr        = hi_ext_system_bayernr_manual_fine_strength_read(ViPipe);
    pstBayernr->stManual.u16CoringWgt     = hi_ext_system_bayernr_manual_coring_weight_read(ViPipe);
    pstBayernr->stManual.au8ChromaStr[0]  = hi_ext_system_bayernr_manual_chroma_strength_read(ViPipe, 0);
    pstBayernr->stManual.au8ChromaStr[1]  = hi_ext_system_bayernr_manual_chroma_strength_read(ViPipe, 1);
    pstBayernr->stManual.au8ChromaStr[2]  = hi_ext_system_bayernr_manual_chroma_strength_read(ViPipe, 2);
    pstBayernr->stManual.au8ChromaStr[3]  = hi_ext_system_bayernr_manual_chroma_strength_read(ViPipe, 3);
    pstBayernr->stManual.au16CoarseStr[0] = hi_ext_system_bayernr_manual_coarse_strength_read(ViPipe, 0);
    pstBayernr->stManual.au16CoarseStr[1] = hi_ext_system_bayernr_manual_coarse_strength_read(ViPipe, 1);
    pstBayernr->stManual.au16CoarseStr[2] = hi_ext_system_bayernr_manual_coarse_strength_read(ViPipe, 2);
    pstBayernr->stManual.au16CoarseStr[3] = hi_ext_system_bayernr_manual_coarse_strength_read(ViPipe, 3);

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        pstBayernr->stWDR.au8WDRFrameStr[i] =  hi_ext_system_bayernr_wdr_frame_strength_read(ViPipe, i);
    }

    return 0;
}

static HI_S32 BayernrReadProMode(VI_PIPE ViPipe)
{
    HI_U8 i;
    ISP_BAYERNR_S *pstBayernr = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_U8 u8Index = 0;
    HI_U8 u8IndexMaxValue = 0;
    BAYERNR_GET_CTX(ViPipe, pstBayernr);
    ISP_CHECK_POINTER(pstBayernr);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    if (pstIspCtx->pro_nr_param_ctrl.pro_nr_param->enable == HI_TRUE) {
        u8Index = pstIspCtx->linkage.pro_index;
        u8IndexMaxValue = MIN2(pstIspCtx->pro_shp_param_ctrl.pro_shp_param->param_num, PRO_MAX_FRAME_NUM);
        if (u8Index > u8IndexMaxValue) {
            u8Index =  u8IndexMaxValue;
        }

        if (u8Index < 1) {
            return HI_SUCCESS;
        }
        u8Index -= 1;
    } else {
        return HI_SUCCESS;
    }
    pstBayernr->enOpType = OP_TYPE_AUTO;
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        pstBayernr->stAuto.au8FineStr[i]       = pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[u8Index].fine_str[i];
        pstBayernr->stAuto.au16CoringWgt[i]    = pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[u8Index].coring_wgt[i];
        pstBayernr->stAuto.au8ChromaStr[0][i]  = pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[u8Index].chroma_str[0][i];
        pstBayernr->stAuto.au8ChromaStr[1][i]  = pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[u8Index].chroma_str[1][i];
        pstBayernr->stAuto.au8ChromaStr[2][i]  = pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[u8Index].chroma_str[2][i];
        pstBayernr->stAuto.au8ChromaStr[3][i]  = pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[u8Index].chroma_str[3][i];
        pstBayernr->stAuto.au16CoarseStr[0][i] = pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[u8Index].coarse_str[0][i];
        pstBayernr->stAuto.au16CoarseStr[1][i] = pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[u8Index].coarse_str[1][i];
        pstBayernr->stAuto.au16CoarseStr[2][i] = pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[u8Index].coarse_str[2][i];
        pstBayernr->stAuto.au16CoarseStr[3][i] = pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[u8Index].coarse_str[3][i];
    }
    return 0;
}
HI_S32 BayernrProcWrite(VI_PIPE ViPipe, hi_isp_ctrl_proc_write *pstProc)
{
    hi_isp_ctrl_proc_write stProcTmp;
    ISP_BAYERNR_S *pstBayernr = HI_NULL;

    BAYERNR_GET_CTX(ViPipe, pstBayernr);
    ISP_CHECK_POINTER(pstBayernr);

    if ((pstProc->proc_buff == HI_NULL) || (pstProc->buff_len == 0)) {
        return HI_FAILURE;
    }

    stProcTmp.proc_buff = pstProc->proc_buff;
    stProcTmp.buff_len = pstProc->buff_len;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "-----BAYERNR INFO--------------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s"      "%16s"         "%16s"        "%16s"      "%16s"       "%16s"        "%16s\n",
                    "Enable", "NrLscEnable", "NrLscRatio", "CoarseStr0", "CoarseStr1", "CoarseStr2", "CoarseStr3");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16u"   "%16u"   "%16u"   "%16u"   "%16u"  "%16u"   "%16u\n",
                    pstBayernr->bEnable,
                    (HI_U16)pstBayernr->bNrLscEnable,
                    (HI_U16)pstBayernr->u8NrLscRatio,
                    (HI_U16)pstBayernr->au16CoarseStr[0],
                    (HI_U16)pstBayernr->au16CoarseStr[1],
                    (HI_U16)pstBayernr->au16CoarseStr[2],
                    (HI_U16)pstBayernr->au16CoarseStr[3]);

    pstProc->write_len += 1;

    return HI_SUCCESS;
}

HI_S32 ISP_BayernrInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    s32Ret = BayerNrCtxInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = BayernrExtRegsInitialize(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    BayernrRegsInitialize(ViPipe, pstRegCfg);

    return HI_SUCCESS;
}

HI_U32 NRGetValueFromLut(HI_U32 u32IsoLevel, HI_S32 s32Y2, HI_S32 s32Y1, HI_S32 s32X2, HI_S32 s32X1, HI_S32 s32Iso)
{
    HI_U32 u32Offset = 0;

    if (s32X1 == s32X2) {
        u32Offset = s32Y2;
    } else if (s32Y1 <= s32Y2) {
        u32Offset = s32Y1 + ((HI_U64)ABS(s32Y2 - s32Y1) * (HI_U64)ABS(s32Iso - s32X1) + (HI_U32)ABS((s32X2 - s32X1) / 2)) / (HI_U32)ABS((s32X2 - s32X1));
    } else if (s32Y1 > s32Y2) {
        u32Offset = s32Y1 - ((HI_U64)ABS(s32Y2 - s32Y1) * (HI_U64)ABS(s32Iso - s32X1) + (HI_U32)ABS((s32X2 - s32X1) / 2)) / (HI_U32)ABS((s32X2 - s32X1));
    }

    return u32Offset;
}

HI_U16 NRGetValueFromLut_fix(HI_U32 x, HI_U32 *pLutX, HI_U16 *pLutY, HI_U32 length)
{
    HI_S32 j;

    if (x <= pLutX[0]) {
        return pLutY[0];
    }
    for (j = 1; j < length; j++) {
        if (x <= pLutX[j]) {
            if (pLutY[j] < pLutY[j - 1]) {
                return (HI_U16)(pLutY[j - 1] - (pLutY[j - 1] - pLutY[j]) * (HI_U16)(x - pLutX[j - 1]) / DIV_0_TO_1((HI_U16)(pLutX[j] - pLutX[j - 1])));
            } else {
                return (HI_U16)(pLutY[j - 1] + (pLutY[j] - pLutY[j - 1]) * (HI_U16)(x - pLutX[j - 1]) / DIV_0_TO_1((HI_U16)(pLutX[j] - pLutX[j - 1])));
            }
        }
    }

    return pLutY[length - 1];
}

#define  BNR_EPS (0.000001f)
#define  BNR_COL_ISO      0
#define  BNR_COL_K        1
#define  BNR_COL_B        2

static HI_FLOAT Bayernr_getKfromNoiseLut(HI_FLOAT (*pRecord)[3], HI_U16 recordNum, HI_S32 iso)
{
    HI_S32 i = 0;
    HI_FLOAT  y_diff = 0, x_diff = 0;
    HI_FLOAT k = 0.0f;

    if (recordNum > BAYER_CALIBTAION_MAX_NUM) {
        k = pRecord[BAYER_CALIBTAION_MAX_NUM - 1][BNR_COL_K];
        return k;
    }

    // record: iso - k
    if (iso <= pRecord[0][BNR_COL_ISO]) {
        k = pRecord[0][BNR_COL_K];
    }

    if (iso >= pRecord[recordNum - 1][BNR_COL_ISO]) {
        k = pRecord[recordNum - 1][BNR_COL_K];
    }

    for (i = 0; i < recordNum - 1; i++) {
        if (iso >= pRecord[i][BNR_COL_ISO] && iso <= pRecord[i + 1][BNR_COL_ISO]) {
            x_diff = pRecord[i + 1][BNR_COL_ISO] - pRecord[i][BNR_COL_ISO];  // iso diff
            y_diff = pRecord[i + 1][BNR_COL_K]  - pRecord[i][BNR_COL_K];     // k diff
            k = pRecord[i][BNR_COL_K] + y_diff * (iso - pRecord[i][BNR_COL_ISO]) / DIV_0_TO_1_FLOAT(x_diff + BNR_EPS);
        }
    }

    return k;
}

static HI_FLOAT Bayernr_getBfromNoiseLut(HI_FLOAT (*pRecord)[3], HI_U16 recordNum, HI_S32 iso)
{
    HI_S32 i = 0;
    HI_FLOAT  y_diff = 0, x_diff = 0;
    HI_FLOAT b = 0.0f;

    if (recordNum > BAYER_CALIBTAION_MAX_NUM) {
        b = pRecord[BAYER_CALIBTAION_MAX_NUM - 1][BNR_COL_B];
        return b;
    }
    // record: iso - b
    if (iso <= pRecord[0][BNR_COL_ISO]) {
        b = pRecord[0][BNR_COL_B];
    }

    if (iso >= pRecord[recordNum - 1][BNR_COL_ISO]) {
        b = pRecord[recordNum - 1][BNR_COL_B];
    }

    for (i = 0; i < recordNum - 1; i++) {
        if (iso >= pRecord[i][BNR_COL_ISO] && iso <= pRecord[i + 1][BNR_COL_ISO]) {
            x_diff = pRecord[i + 1][BNR_COL_ISO] - pRecord[i][BNR_COL_ISO];  // iso diff
            y_diff = pRecord[i + 1][BNR_COL_B]  - pRecord[i][BNR_COL_B];     // k diff
            b = pRecord[i][BNR_COL_B] + y_diff * (iso - pRecord[i][BNR_COL_ISO]) / DIV_0_TO_1_FLOAT(x_diff + BNR_EPS);
        }
    }

    return b;
}


static __inline HI_U16 BayernrOffsetCalculate(const HI_U16 u16Y2, const HI_U16 u16Y1, const HI_U32 u32X2,
                                              const HI_U32 u32X1, const HI_U32 u32Iso)
{
    HI_U32 u32Offset;
    if (u32X1 == u32X2) {
        u32Offset = u16Y2;
    } else if (u16Y1 <= u16Y2) {
        u32Offset = u16Y1 + (ABS(u16Y2 - u16Y1) * (u32Iso - u32X1) + (u32X2 - u32X1) / 2) / (u32X2 - u32X1);
    } else if (u16Y1 > u16Y2) {
        u32Offset = u16Y1 - (ABS(u16Y2 - u16Y1) * (u32Iso - u32X1) + (u32X2 - u32X1) / 2) / (u32X2 - u32X1);
    }

    return (HI_U16)u32Offset;
}

static HI_U32 NRGetIsoIndex(HI_U32 u32Iso)
{
    HI_U32 u32Index;

    for (u32Index = 1; u32Index < HI_ISP_NR_LUT_LEN - 1; u32Index++) {
        if (u32Iso <= g_au32BayernrIsoLut[u32Index]) {
            break;
        }
    }

    return u32Index;
}

HI_S32 NRCfg(VI_PIPE ViPipe, ISP_BAYERNR_DYNA_CFG_S *pstDynaCfg, ISP_BAYERNR_S *pstBayernr, HI_U32 u32IsoLevel,
             HI_U32 u32ISO2, HI_U32 u32ISO1, HI_U32 u32Iso)
{
    HI_U8  u8MaxCRaio, u8MaxCRaio01, u8MaxCRaio23;
    HI_S32 s32Y1, s32Y2;
    isp_usr_ctx *pstIspCtx  = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    if ((pstBayernr->u8WdrFramesMerge == 1) ||
        (pstIspCtx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (pstIspCtx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_RGGB][u32IsoLevel - 1] : (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_RGGB][0];
        s32Y2 = (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_RGGB][u32IsoLevel];
        pstDynaCfg->au8BnrCRatio[BAYER_RGGB]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_GRBG][u32IsoLevel - 1] : (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_GRBG][0];
        s32Y2 = (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_GRBG][u32IsoLevel];
        pstDynaCfg->au8BnrCRatio[BAYER_GRBG]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_GBRG][u32IsoLevel - 1] : (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_GBRG][0];
        s32Y2 = (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_GBRG][u32IsoLevel];
        pstDynaCfg->au8BnrCRatio[BAYER_GBRG]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_BGGR][u32IsoLevel - 1] : (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_BGGR][0];
        s32Y2 = (HI_S32)pstBayernr->au8LutChromaRatio[BAYER_BGGR][u32IsoLevel];
        pstDynaCfg->au8BnrCRatio[BAYER_BGGR]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);
    } else {
        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_RGGB][u32IsoLevel - 1] : (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_RGGB][0];
        s32Y2 = (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_RGGB][u32IsoLevel];
        pstDynaCfg->au8BnrCRatio[BAYER_RGGB]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_GRBG][u32IsoLevel - 1] : (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_GRBG][0];
        s32Y2 = (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_GRBG][u32IsoLevel];
        pstDynaCfg->au8BnrCRatio[BAYER_GRBG]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_GBRG][u32IsoLevel - 1] : (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_GBRG][0];
        s32Y2 = (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_GBRG][u32IsoLevel];
        pstDynaCfg->au8BnrCRatio[BAYER_GBRG]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_BGGR][u32IsoLevel - 1] : (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_BGGR][0];
        s32Y2 = (HI_S32)pstBayernr->au8LutWDRChromaRatio[BAYER_BGGR][u32IsoLevel];
        pstDynaCfg->au8BnrCRatio[BAYER_BGGR]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);
    }

    s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->au16LutCoringHig[u32IsoLevel - 1] : (HI_S32)pstBayernr->au16LutCoringHig[0];
    s32Y2 = (HI_S32)pstBayernr->au16LutCoringHig[u32IsoLevel];
    pstDynaCfg->u16JnlmCoringHig  = (HI_U16)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);
    pstDynaCfg->u16JnlmCoringHig  = (HI_U16)(256 * ((HI_FLOAT)pstDynaCfg->u16JnlmCoringHig / (HI_FLOAT)HI_ISP_BAYERNR_STRENGTH_DIVISOR));

    pstDynaCfg->au8AmedMode[BAYER_RGGB] = (u32Iso < 5000) ? 0 : 1;
    pstDynaCfg->au8AmedMode[BAYER_GRBG] = 0;
    pstDynaCfg->au8AmedMode[BAYER_GBRG] = 0;
    pstDynaCfg->au8AmedMode[BAYER_BGGR] = (u32Iso < 5000) ? 0 : 1;

    u8MaxCRaio01 = MAX2(pstDynaCfg->au8BnrCRatio[BAYER_RGGB], pstDynaCfg->au8BnrCRatio[BAYER_GRBG]);
    u8MaxCRaio23 = MAX2(pstDynaCfg->au8BnrCRatio[BAYER_GBRG], pstDynaCfg->au8BnrCRatio[BAYER_BGGR]);
    u8MaxCRaio   = MAX2(u8MaxCRaio01, u8MaxCRaio23);

    if (u8MaxCRaio <= 4) {
        pstDynaCfg->bMedcEnable = HI_FALSE;
    } else {
        pstDynaCfg->bMedcEnable = HI_TRUE;
    }

    if (u8MaxCRaio >= 20) {
        pstDynaCfg->bTriSadEn   = HI_TRUE;
    }

    return HI_SUCCESS;
}

HI_S32 NRExtCfg(VI_PIPE ViPipe, ISP_BAYERNR_DYNA_CFG_S *pstDynaCfg, ISP_BAYERNR_S *pstBayernr,
                HI_U32 u32IsoLevel, HI_U32 u32ISO2, HI_U32 u32ISO1, HI_U32 u32Iso)
{
    HI_U8  u8SadFac = 25;
    HI_U16 u16JnlmScale = 49;
    HI_U16 u16JnlmShotScale;
    HI_U16 au16LmtStrength[4] = {0};
    HI_U32 i = 0;
    HI_S32 s32Y1, s32Y2;
    HI_U32 u32CoringLow = 1;
    HI_U16 u16ShotCoef = 2;

    if (pstBayernr->enOpType == OP_TYPE_AUTO) {
        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->stAuto.au16CoringWgt[u32IsoLevel - 1] : (HI_S32)pstBayernr->stAuto.au16CoringWgt[0];
        s32Y2 = (HI_S32)pstBayernr->stAuto.au16CoringWgt[u32IsoLevel];
        pstBayernr->u16CoringLow  = (HI_U16)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);
        u32CoringLow  = 256 * (HI_U32)pstBayernr->u16CoringLow;

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->stAuto.au8FineStr[u32IsoLevel - 1] : (HI_S32)pstBayernr->stAuto.au8FineStr[0];
        s32Y2 = (HI_S32)pstBayernr->stAuto.au8FineStr[u32IsoLevel];
        pstDynaCfg->u8JnlmGain  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);
        pstBayernr->u8FineStr = pstDynaCfg->u8JnlmGain;

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_RGGB][u32IsoLevel - 1] : (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_RGGB][0];
        s32Y2 = (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_RGGB][u32IsoLevel];
        pstDynaCfg->au8AmedLevel[BAYER_RGGB]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_GRBG][u32IsoLevel - 1] : (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_GRBG][0];
        s32Y2 = (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_GRBG][u32IsoLevel];
        pstDynaCfg->au8AmedLevel[BAYER_GRBG]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_GBRG][u32IsoLevel - 1] : (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_GBRG][0];
        s32Y2 = (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_GBRG][u32IsoLevel];
        pstDynaCfg->au8AmedLevel[BAYER_GBRG]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_BGGR][u32IsoLevel - 1] : (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_BGGR][0];
        s32Y2 = (HI_S32)pstBayernr->stAuto.au8ChromaStr[BAYER_BGGR][u32IsoLevel];
        pstDynaCfg->au8AmedLevel[BAYER_BGGR]  = (HI_U8)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_RGGB][u32IsoLevel - 1] : (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_RGGB][0];
        s32Y2 = (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_RGGB][u32IsoLevel];
        au16LmtStrength[BAYER_RGGB]  = (HI_U16)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_GRBG][u32IsoLevel - 1] : (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_GRBG][0];
        s32Y2 = (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_GRBG][u32IsoLevel];
        au16LmtStrength[BAYER_GRBG]  = (HI_U16)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_GBRG][u32IsoLevel - 1] : (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_GBRG][0];
        s32Y2 = (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_GBRG][u32IsoLevel];
        au16LmtStrength[BAYER_GBRG]  = (HI_U16)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_BGGR][u32IsoLevel - 1] : (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_BGGR][0];
        s32Y2 = (HI_S32)pstBayernr->stAuto.au16CoarseStr[BAYER_BGGR][u32IsoLevel];
        au16LmtStrength[BAYER_BGGR]  = (HI_U16)NRGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);
    } else if (pstBayernr->enOpType == OP_TYPE_MANUAL) {
        pstBayernr->u16CoringLow  = pstBayernr->stManual.u16CoringWgt;
        u32CoringLow  = 256 * (HI_U32)pstBayernr->u16CoringLow;
        pstDynaCfg->u8JnlmGain               = pstBayernr->stManual.u8FineStr;
        pstDynaCfg->au8AmedLevel[BAYER_RGGB] = pstBayernr->stManual.au8ChromaStr[BAYER_RGGB];
        pstDynaCfg->au8AmedLevel[BAYER_GRBG] = pstBayernr->stManual.au8ChromaStr[BAYER_GRBG];
        pstDynaCfg->au8AmedLevel[BAYER_GBRG] = pstBayernr->stManual.au8ChromaStr[BAYER_GBRG];
        pstDynaCfg->au8AmedLevel[BAYER_BGGR] = pstBayernr->stManual.au8ChromaStr[BAYER_BGGR];

        au16LmtStrength[BAYER_RGGB] = pstBayernr->stManual.au16CoarseStr[BAYER_RGGB];
        au16LmtStrength[BAYER_GRBG] = pstBayernr->stManual.au16CoarseStr[BAYER_GRBG];
        au16LmtStrength[BAYER_GBRG] = pstBayernr->stManual.au16CoarseStr[BAYER_GBRG];
        au16LmtStrength[BAYER_BGGR] = pstBayernr->stManual.au16CoarseStr[BAYER_BGGR];
    }

    pstBayernr->au16CoarseStr[0] = au16LmtStrength[BAYER_RGGB];
    pstBayernr->au16CoarseStr[1] = au16LmtStrength[BAYER_GRBG];
    pstBayernr->au16CoarseStr[2] = au16LmtStrength[BAYER_GBRG];
    pstBayernr->au16CoarseStr[3] = au16LmtStrength[BAYER_BGGR];

    hi_ext_system_bayernr_actual_coring_weight_write(ViPipe, pstBayernr->u16CoringLow);
    hi_ext_system_bayernr_actual_fine_strength_write(ViPipe, pstDynaCfg->u8JnlmGain);
    hi_ext_system_bayernr_actual_nr_lsc_ratio_write(ViPipe, pstBayernr->u8NrLscRatio);

    for (i = 0; i < 4; i++) {
        hi_ext_system_bayernr_actual_coarse_strength_write(ViPipe, i, au16LmtStrength[i]);
        hi_ext_system_bayernr_actual_chroma_strength_write(ViPipe, i, pstDynaCfg->au8AmedLevel[i]);
        hi_ext_system_bayernr_actual_wdr_frame_strength_write(ViPipe, i, pstBayernr->stWDR.au8WDRFrameStr[i]);
    }

    if (pstBayernr->bLowPowerEnable == HI_TRUE) {
        pstDynaCfg->bSkipLevel4Enable = HI_TRUE;
        pstDynaCfg->bSkipLevel2Enable = HI_FALSE;
    } else {
        pstDynaCfg->bSkipLevel4Enable = HI_FALSE;
        pstDynaCfg->bSkipLevel2Enable = HI_TRUE;
    }

    for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
        pstDynaCfg->au16JnlmCoringLowLUT[i] = (HI_U16)(pstBayernr->au16LutCoringRatio[i] * u32CoringLow / HI_ISP_BAYERNR_CORINGLOW_STRENGTH_DIVISOR);
        pstDynaCfg->au16JnlmCoringLowLUT[i] = MIN2(16383, pstDynaCfg->au16JnlmCoringLowLUT[i]);
    }

    u16JnlmShotScale   = 128 + CLIP3((HI_S32)(u16JnlmScale * u16ShotCoef), 0, 255);
    u16JnlmScale       = u16JnlmScale + 128;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        pstDynaCfg->au32JnlmLimitMultGain[0][i] = (pstDynaCfg->u16LmtNpThresh * au16LmtStrength[i] * u8SadFac) >> 7;
        pstDynaCfg->au32JnlmLimitMultGain[1][i] = pstDynaCfg->au32JnlmLimitMultGain[0][i];
        pstDynaCfg->au32JnlmLimitMultGain[0][i] = (pstDynaCfg->au32JnlmLimitMultGain[0][i] * u16JnlmScale) >> 7;
        pstDynaCfg->au32JnlmLimitMultGain[1][i] = (pstDynaCfg->au32JnlmLimitMultGain[1][i] * u16JnlmShotScale) >> 7;
    }

    return HI_SUCCESS;
}

HI_S32 NRLimitLut(VI_PIPE ViPipe, ISP_BAYERNR_DYNA_CFG_S *pstDynaCfg, ISP_BAYERNR_S *pstBayernr, HI_U32 u32Iso)
{
    HI_U16 u16BlackLevel, str;
    HI_U32 u32LmtNpThresh;
    HI_U32 i = 0, n = 0;
    HI_U16 u16BitMask = ((1 << (HI_ISP_BAYERNR_BITDEP - 1)) - 1);
    HI_U32 lutN[2] = { 16, 45 };
    HI_U32 k = 0, b = 0;
    HI_U32 sigma = 0, sigma_max = 0;
    HI_U16 DarkStrength = 230;   // 1.8f*128
    HI_U16 lutStr[2] = { 96, 128 };  //  {0.75f, 1.0f}*128
    HI_FLOAT fCalibrationCoef = 0.0f;

    hi_isp_cmos_default *sns_dft = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    isp_sensor_get_default(ViPipe, &sns_dft);
    isp_sensor_get_blc(ViPipe, &sns_black_level);

    fCalibrationCoef = Bayernr_getKfromNoiseLut(sns_dft->noise_calibration.calibration_coef, sns_dft->noise_calibration.calibration_lut_num, u32Iso);
    k     = (HI_U32)(fCalibrationCoef * ISP_BITFIX(14));
    fCalibrationCoef = Bayernr_getBfromNoiseLut(sns_dft->noise_calibration.calibration_coef, sns_dft->noise_calibration.calibration_lut_num, u32Iso);
    b     = (HI_U32)(fCalibrationCoef * ISP_BITFIX(14));

    u16BlackLevel = sns_black_level->black_level[0] >> 4;

    sigma_max = (HI_U32)(MAX2((HI_S32)(k * (HI_S32)(255 - u16BlackLevel) + (HI_S32)b), 0));
    sigma_max = (HI_U32)Sqrt32(sigma_max);

    u32LmtNpThresh = (HI_U32)(sigma_max * (1 << (HI_ISP_BAYERNR_BITDEP - 8 - 7))); // sad win size, move to hw
    pstDynaCfg->u16LmtNpThresh = (HI_U16)((u32LmtNpThresh > u16BitMask) ? u16BitMask : u32LmtNpThresh);

    lutStr[0] = DarkStrength;

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        sigma = (HI_U32)MAX2(((HI_S32)(k * (i * 255 - 128 * u16BlackLevel)) / (HI_S32)128) + (HI_S32)b, 0);
        sigma = (HI_U32)Sqrt32(sigma);
        str = NRGetValueFromLut_fix(2 * i, lutN, lutStr, 2);
        sigma = sigma * str;

        pstDynaCfg->au8JnlmLimitLut[i] = (HI_U8)((sigma + sigma_max / 2) / DIV_0_TO_1(sigma_max));
    }

    // copy the first non-zero value to its left side
    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        if (pstDynaCfg->au8JnlmLimitLut[i] > 0) {
            n = i;
            break;
        }
    }

    for (i = 0; i < n; i++) {
        pstDynaCfg->au8JnlmLimitLut[i] = pstDynaCfg->au8JnlmLimitLut[n];
    }

    return HI_SUCCESS;
}

HI_S32 hiisp_bayernr_fw(HI_U32 u32Iso, VI_PIPE ViPipe,
                        ISP_BAYERNR_DYNA_CFG_S *pstDynaCfg, ISP_BAYERNR_USR_CFG_S *pstUsrCfg)
{
    HI_U32 i = 0;
    HI_U32 u32IsoLevel;
    HI_U32 u32ISO1 = 0, u32ISO2 = 0;

    ISP_BAYERNR_S      *pstBayernr = HI_NULL;
    hi_isp_cmos_default *sns_dft  = HI_NULL;
    isp_usr_ctx          *pstIspCtx  = HI_NULL;

    isp_sensor_get_default(ViPipe, &sns_dft);
    BAYERNR_GET_CTX(ViPipe, pstBayernr);
    ISP_CHECK_POINTER(pstBayernr);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstUsrCfg->bBnrMonoSensorEn = pstBayernr->bBnrMonoSensorEn;     // MonoSensor, waiting to get
    pstUsrCfg->bBnrLscEn        = pstBayernr->bNrLscEnable;
    pstUsrCfg->u8BnrLscRatio    = pstBayernr->u8NrLscRatio;

    if (u32Iso > g_au32BayernrIsoLut[HI_ISP_NR_LUT_LEN - 1]) {
        u32IsoLevel = HI_ISP_NR_LUT_LEN - 1;
        u32ISO1 = g_au32BayernrIsoLut[HI_ISP_NR_LUT_LEN - 1];
        u32ISO2 = g_au32BayernrIsoLut[HI_ISP_NR_LUT_LEN - 1];
    } else if (u32Iso <= g_au32BayernrIsoLut[0]) {
        u32IsoLevel = 0;
        u32ISO1     = 0;
        u32ISO2     = g_au32BayernrIsoLut[0];
    } else {
        u32IsoLevel = NRGetIsoIndex(u32Iso);
        u32ISO1     = g_au32BayernrIsoLut[u32IsoLevel - 1];
        u32ISO2     = g_au32BayernrIsoLut[u32IsoLevel];
    }

    NRLimitLut(ViPipe, pstDynaCfg, pstBayernr, u32Iso);
    NRCfg(ViPipe, pstDynaCfg, pstBayernr, u32IsoLevel, u32ISO2, u32ISO1, u32Iso);
    NRExtCfg(ViPipe,  pstDynaCfg, pstBayernr, u32IsoLevel, u32ISO2, u32ISO1, u32Iso);

    if (pstUsrCfg->bBnrMonoSensorEn == HI_TRUE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstDynaCfg->au8BnrCRatio[i] = 0;
            pstDynaCfg->au8AmedLevel[i] = 0;
            pstDynaCfg->bMedcEnable     = HI_FALSE;
        }
    }

    pstDynaCfg->bBnrLutUpdateEn = HI_TRUE;
    pstDynaCfg->u16RLmtRgain    = pstIspCtx->linkage.white_balance_gain[0] >> 8;
    pstDynaCfg->u16RLmtBgain    = pstIspCtx->linkage.white_balance_gain[3] >> 8;

    pstDynaCfg->bResh = HI_TRUE;
    pstUsrCfg->bResh  = HI_TRUE;

    return  HI_SUCCESS;
}

// WDR FW: ADJ_C(2) + ADJ_D(4) = 6
#define  ADJ_C  2
#define  ADJ_D  4

static HI_U16 BCOM(HI_U64 x)
{
    HI_U64 out = (x << 22) / DIV_0_TO_1((x << 6) + (((1 << 20) - x) << ADJ_C));
    return (HI_U16)out;
}

// 16bit -> 20bit
static HI_U32 BDEC(HI_U64 y)
{
    HI_U64 out = (y << 26) / DIV_0_TO_1((y << 6) + (((1 << 16) - y) << (ADJ_D + 6)));
    return (HI_U32)out;
}

HI_S32 NRLimitLut_WDR2to1(ISP_BAYERNR_DYNA_CFG_S *pstDynaCfg, ISP_BAYERNR_S *pstBayernr, HI_U32 k, HI_U32 cprClip)
{
    HI_U32 i;
    HI_U16 u16BitMask = ((1 << (HI_ISP_BAYERNR_BITDEP - 1)) - 1);
    HI_U32 au32WDRFrameThr[WDR_MAX_FRAME_NUM + 2];
    HI_U32 u32WDR_PxValue, u32n8b_cur_axs;
    HI_U32 u32LmtNpThresh;
    HI_U32 sigma = 0, sigma_max = 0;
    HI_U32 WDR_JNLM_Limit_LUT[HI_ISP_BAYERNR_LMTLUTNUM];
    HI_U32 n8b_pre_pos, n8b_pre_axs;

    pstBayernr->au32ExpoValues[1] = (pstBayernr->au32ExpoValues[1] == 0) ? 64 : pstBayernr->au32ExpoValues[1];

    // intensity threshold
    au32WDRFrameThr[0] = ((HI_U32)pstBayernr->au16WDRFrameThr[0] << 14) / pstBayernr->au32ExpoValues[1] + (HI_U32)pstBayernr->u16WDRBlcThr;
    au32WDRFrameThr[1] = ((HI_U32)pstBayernr->au16WDRFrameThr[1] << 14) / pstBayernr->au32ExpoValues[1] + (HI_U32)pstBayernr->u16WDRBlcThr;

    au32WDRFrameThr[0] = CLIP3(au32WDRFrameThr[0],0,((1<<20)-1));
    au32WDRFrameThr[1] = CLIP3(au32WDRFrameThr[1],0,((1<<20)-1));

    pstBayernr->au16WDRFrameThr[0] = BCOM((HI_U64)au32WDRFrameThr[0]);
    pstBayernr->au16WDRFrameThr[1] = BCOM((HI_U64)au32WDRFrameThr[1]);

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        u32WDR_PxValue = i * 512;
        u32n8b_cur_axs = i * 2;
        n8b_pre_pos = MAX2((HI_S32)BDEC((HI_U64)u32n8b_cur_axs << 8) - (HI_S32)pstBayernr->u16WDRBlcThr, 0);
        n8b_pre_axs = BDEC((HI_U64)u32n8b_cur_axs << 8);

        sigma = k * (n8b_pre_pos + HI_WDR_EINIT_BLCNR * ISP_BITFIX(12));
        sigma = (HI_U32)Sqrt32(sigma);
        sigma = (HI_U32)((HI_U64)sigma * u32n8b_cur_axs * ISP_BITFIX(12) / DIV_0_TO_1(MAX2(n8b_pre_axs, cprClip)));

        if (u32WDR_PxValue <= pstBayernr->au16WDRFrameThr[0]) {
            // long frame strength
            sigma /= DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[1]));
            sigma  = sigma * pstBayernr->stWDR.au8WDRFrameStr[1] / 16;
        } else if (u32WDR_PxValue > pstBayernr->au16WDRFrameThr[0] && u32WDR_PxValue < pstBayernr->au16WDRFrameThr[1]) {
            HI_U32   sgmFS = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[0]));
            HI_U32   sgmFL = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[1]));
            HI_U32   bldr  = (u32WDR_PxValue - (HI_U32)pstBayernr->au16WDRFrameThr[0]) *  ISP_BITFIX(8) / MAX2((pstBayernr->au16WDRFrameThr[1] - pstBayernr->au16WDRFrameThr[0]), 1);

            sgmFS = sgmFS * pstBayernr->stWDR.au8WDRFrameStr[0] / 16;
            sgmFL = sgmFL * pstBayernr->stWDR.au8WDRFrameStr[1] / 16;
            sigma = (sgmFS * bldr + sgmFL * (256 - bldr)) / 256;
        } else {
            // short frame strength
            sigma /= DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[0]));
            sigma  = sigma * pstBayernr->stWDR.au8WDRFrameStr[0] / 16;
        }
        WDR_JNLM_Limit_LUT[i] = sigma;
        sigma_max = (sigma_max < sigma) ? sigma : sigma_max;
    }

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        pstDynaCfg->au8JnlmLimitLut[i] = (HI_U8)((WDR_JNLM_Limit_LUT[i] * 128 + sigma_max / 2) / DIV_0_TO_1(sigma_max));
    }

    u32LmtNpThresh = (HI_U32)(sigma_max * (1 << (HI_ISP_BAYERNR_BITDEP - 8 - 3)) / ISP_BITFIX(10)); // sad win size, move to hw
    pstDynaCfg->u16LmtNpThresh = (u32LmtNpThresh > u16BitMask) ? u16BitMask : u32LmtNpThresh;

    return HI_SUCCESS;
}

HI_S32 NRLimitLut_WDR3to1(ISP_BAYERNR_DYNA_CFG_S *pstDynaCfg, ISP_BAYERNR_S *pstBayernr, HI_U32 k, HI_U32 cprClip)
{
    HI_U8  i;
    HI_U16 u16BitMask = ((1 << (HI_ISP_BAYERNR_BITDEP - 1)) - 1);
    HI_U32 au32WDRFrameThr[WDR_MAX_FRAME_NUM + 2];
    HI_U32 u32WDR_PxValue, u32n8b_cur_axs;
    HI_U32 u32LmtNpThresh;

    HI_U32 sigma = 0, sigma_max = 0;
    HI_U32 WDR_JNLM_Limit_LUT[HI_ISP_BAYERNR_LMTLUTNUM];
    HI_U32 n8b_pre_pos, n8b_pre_axs;

    pstBayernr->au32ExpoValues[1] = (pstBayernr->au32ExpoValues[1] == 0) ? 64 : pstBayernr->au32ExpoValues[1];
    pstBayernr->au32ExpoValues[2] = (pstBayernr->au32ExpoValues[2] == 0) ? 64 : pstBayernr->au32ExpoValues[2];

    // intensity threshold
    au32WDRFrameThr[0] = (HI_U32)((pstBayernr->au16WDRFrameThr[0] << 14) / pstBayernr->au32ExpoValues[2]) + pstBayernr->u16WDRBlcThr;
    au32WDRFrameThr[1] = (HI_U32)((pstBayernr->au16WDRFrameThr[1] << 14) / pstBayernr->au32ExpoValues[2]) + pstBayernr->u16WDRBlcThr;
    au32WDRFrameThr[2] = (HI_U32)((pstBayernr->au16WDRFrameThr[0] << 14) / pstBayernr->au32ExpoValues[1]) + pstBayernr->u16WDRBlcThr;
    au32WDRFrameThr[3] = (HI_U32)((pstBayernr->au16WDRFrameThr[1] << 14) / pstBayernr->au32ExpoValues[1]) + pstBayernr->u16WDRBlcThr;

    pstBayernr->au16WDRFrameThr[0] = BCOM((HI_U64)au32WDRFrameThr[0]);
    pstBayernr->au16WDRFrameThr[1] = BCOM((HI_U64)au32WDRFrameThr[1]);
    pstBayernr->au16WDRFrameThr[2] = BCOM((HI_U64)au32WDRFrameThr[2]);
    pstBayernr->au16WDRFrameThr[3] = BCOM((HI_U64)au32WDRFrameThr[3]);

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        u32WDR_PxValue = i * 512;
        u32n8b_cur_axs = i * 2;

        n8b_pre_pos = MAX2((HI_S32)BDEC((HI_U64)u32n8b_cur_axs << 8) - (HI_S32)pstBayernr->u16WDRBlcThr, 0);
        n8b_pre_axs = MAX2((HI_S32)BDEC((HI_U64)u32n8b_cur_axs << 8), 0);

        sigma = k * (n8b_pre_pos + HI_WDR_EINIT_BLCNR * ISP_BITFIX(12));
        sigma = (HI_U32)Sqrt32(sigma);
        sigma = (HI_U32)((HI_U64)sigma * u32n8b_cur_axs * ISP_BITFIX(12) / DIV_0_TO_1(MAX2(n8b_pre_axs, cprClip)));

        if (u32WDR_PxValue <= pstBayernr->au16WDRFrameThr[0]) {
            // long frame strength
            sigma /= DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[2]));
            sigma  = sigma * pstBayernr->stWDR.au8WDRFrameStr[2] / 16;
        } else if (u32WDR_PxValue > pstBayernr->au16WDRFrameThr[0] && u32WDR_PxValue < pstBayernr->au16WDRFrameThr[1]) {
            HI_U32   sgmFS = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[1]));
            HI_U32   sgmFL = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[2]));
            HI_U32   bldr  = (u32WDR_PxValue - (HI_U32)pstBayernr->au16WDRFrameThr[0]) *  ISP_BITFIX(8) / MAX2(pstBayernr->au16WDRFrameThr[1] - pstBayernr->au16WDRFrameThr[0], 1);

            sgmFS = sgmFS * pstBayernr->stWDR.au8WDRFrameStr[1] / 16;
            sgmFL = sgmFL * pstBayernr->stWDR.au8WDRFrameStr[2] / 16;
            sigma = (sgmFS * bldr + sgmFL * (256 - bldr)) / 256;
        } else if (u32WDR_PxValue > pstBayernr->au16WDRFrameThr[1] && u32WDR_PxValue < pstBayernr->au16WDRFrameThr[2]) {
            // medium frame strength
            sigma /= DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[1]));
            sigma  = sigma * pstBayernr->stWDR.au8WDRFrameStr[1] / 16;
        } else if (u32WDR_PxValue > pstBayernr->au16WDRFrameThr[2] && u32WDR_PxValue < pstBayernr->au16WDRFrameThr[3]) {
            HI_U32   sgmFS = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[0]));
            HI_U32   sgmFL = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[1]));
            HI_U32   bldr  = (u32WDR_PxValue - (HI_U32)pstBayernr->au16WDRFrameThr[2]) *  ISP_BITFIX(8) / MAX2(pstBayernr->au16WDRFrameThr[3] - pstBayernr->au16WDRFrameThr[2], 1);

            sgmFS = sgmFS * pstBayernr->stWDR.au8WDRFrameStr[0] / 16;
            sgmFL = sgmFL * pstBayernr->stWDR.au8WDRFrameStr[1] / 16;
            sigma = (sgmFS * bldr + sgmFL * (256 - bldr)) / 256;
        } else {
            // short frame strength
            sigma /= DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[0]));
            sigma  = sigma * pstBayernr->stWDR.au8WDRFrameStr[0] / 16;
        }

        WDR_JNLM_Limit_LUT[i] = sigma;
        sigma_max = (sigma_max < sigma) ? sigma : sigma_max;
    }

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        pstDynaCfg->au8JnlmLimitLut[i] = (HI_U8)((WDR_JNLM_Limit_LUT[i] * 128 + sigma_max / 2) / DIV_0_TO_1(sigma_max));
    }

    u32LmtNpThresh = (HI_U32)(sigma_max * (1 << (HI_ISP_BAYERNR_BITDEP - 8 - 3)) / ISP_BITFIX(10)); // sad win size, move to hw
    pstDynaCfg->u16LmtNpThresh = (u32LmtNpThresh > u16BitMask) ? u16BitMask : u32LmtNpThresh;

    return HI_SUCCESS;
}

HI_S32 NRLimitLut_WDR4to1(ISP_BAYERNR_DYNA_CFG_S *pstDynaCfg, ISP_BAYERNR_S *pstBayernr, HI_U32 k, HI_U32 cprClip)
{
    HI_U8  i;
    HI_U16 u16BitMask = ((1 << (HI_ISP_BAYERNR_BITDEP - 1)) - 1);
    HI_U32 au32WDRFrameThr[WDR_MAX_FRAME_NUM + 2];
    HI_U32 u32WDR_PxValue, u32n8b_cur_axs;
    HI_U32 u32LmtNpThresh;

    HI_U32 sigma = 0, sigma_max = 0;
    HI_U32 WDR_JNLM_Limit_LUT[HI_ISP_BAYERNR_LMTLUTNUM];
    HI_U32 n8b_pre_pos, n8b_pre_axs;

    pstBayernr->au32ExpoValues[1] = (pstBayernr->au32ExpoValues[1] == 0) ? 64 : pstBayernr->au32ExpoValues[1];
    pstBayernr->au32ExpoValues[2] = (pstBayernr->au32ExpoValues[2] == 0) ? 64 : pstBayernr->au32ExpoValues[2];
    pstBayernr->au32ExpoValues[3] = (pstBayernr->au32ExpoValues[3] == 0) ? 64 : pstBayernr->au32ExpoValues[3];

    // intensity threshold
    au32WDRFrameThr[0] = (HI_U32)((pstBayernr->au16WDRFrameThr[0] << 14) / pstBayernr->au32ExpoValues[3]) + pstBayernr->u16WDRBlcThr;
    au32WDRFrameThr[1] = (HI_U32)((pstBayernr->au16WDRFrameThr[1] << 14) / pstBayernr->au32ExpoValues[3]) + pstBayernr->u16WDRBlcThr;
    au32WDRFrameThr[2] = (HI_U32)((pstBayernr->au16WDRFrameThr[0] << 14) / pstBayernr->au32ExpoValues[2]) + pstBayernr->u16WDRBlcThr;
    au32WDRFrameThr[3] = (HI_U32)((pstBayernr->au16WDRFrameThr[1] << 14) / pstBayernr->au32ExpoValues[2]) + pstBayernr->u16WDRBlcThr;
    au32WDRFrameThr[4] = (HI_U32)((pstBayernr->au16WDRFrameThr[0] << 14) / pstBayernr->au32ExpoValues[1]) + pstBayernr->u16WDRBlcThr;
    au32WDRFrameThr[5] = (HI_U32)((pstBayernr->au16WDRFrameThr[1] << 14) / pstBayernr->au32ExpoValues[1]) + pstBayernr->u16WDRBlcThr;

    pstBayernr->au16WDRFrameThr[0] = BCOM((HI_U64)au32WDRFrameThr[0]);
    pstBayernr->au16WDRFrameThr[1] = BCOM((HI_U64)au32WDRFrameThr[1]);
    pstBayernr->au16WDRFrameThr[2] = BCOM((HI_U64)au32WDRFrameThr[2]);
    pstBayernr->au16WDRFrameThr[3] = BCOM((HI_U64)au32WDRFrameThr[3]);
    pstBayernr->au16WDRFrameThr[4] = BCOM((HI_U64)au32WDRFrameThr[4]);
    pstBayernr->au16WDRFrameThr[5] = BCOM((HI_U64)au32WDRFrameThr[5]);

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        u32WDR_PxValue = i * 512;
        u32n8b_cur_axs = i * 2;
        n8b_pre_pos = MAX2((HI_S32)BDEC((HI_U64)u32n8b_cur_axs << 8) - (HI_S32)pstBayernr->u16WDRBlcThr, 0);
        n8b_pre_axs = MAX2((HI_S32)BDEC((HI_U64)u32n8b_cur_axs << 8), 0);

        sigma = k * (n8b_pre_pos + HI_WDR_EINIT_BLCNR * ISP_BITFIX(12));
        sigma = (HI_U32)Sqrt32(sigma);
        sigma = (HI_U32)((HI_U64)sigma * u32n8b_cur_axs * ISP_BITFIX(12) / DIV_0_TO_1(MAX2(n8b_pre_axs, cprClip)));

        if (u32WDR_PxValue <= pstBayernr->au16WDRFrameThr[0]) {
            // long frame strength
            sigma /= DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[3]));
            sigma  = sigma * pstBayernr->stWDR.au8WDRFrameStr[3] / 16;
        } else if (u32WDR_PxValue > pstBayernr->au16WDRFrameThr[0] && u32WDR_PxValue < pstBayernr->au16WDRFrameThr[1]) {
            HI_U32   sgmFS = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[2]));
            HI_U32   sgmFL = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[3]));
            HI_U32   bldr  = (u32WDR_PxValue - (HI_U32)pstBayernr->au16WDRFrameThr[0]) *  ISP_BITFIX(8) / MAX2(pstBayernr->au16WDRFrameThr[1] - pstBayernr->au16WDRFrameThr[0], 1);

            sgmFS = sgmFS * pstBayernr->stWDR.au8WDRFrameStr[2] / 16;
            sgmFL = sgmFL * pstBayernr->stWDR.au8WDRFrameStr[3] / 16;
            sigma = (sgmFS * bldr + sgmFL * (256 - bldr)) / 256;
        } else if (u32WDR_PxValue > pstBayernr->au16WDRFrameThr[1] && u32WDR_PxValue < pstBayernr->au16WDRFrameThr[2]) {
            // medium frame strength
            sigma /= DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[2]));
            sigma  = sigma * pstBayernr->stWDR.au8WDRFrameStr[2] / 16;
        } else if (u32WDR_PxValue > pstBayernr->au16WDRFrameThr[2] && u32WDR_PxValue < pstBayernr->au16WDRFrameThr[3]) {
            HI_U32   sgmFS = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[1]));
            HI_U32   sgmFL = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[2]));
            HI_U32   bldr  = (u32WDR_PxValue - (HI_U32)pstBayernr->au16WDRFrameThr[2]) *  ISP_BITFIX(8) / MAX2(pstBayernr->au16WDRFrameThr[3] - pstBayernr->au16WDRFrameThr[2], 1);

            sgmFS = sgmFS * pstBayernr->stWDR.au8WDRFrameStr[1] / 16;
            sgmFL = sgmFL * pstBayernr->stWDR.au8WDRFrameStr[2] / 16;
            sigma = (sgmFS * bldr + sgmFL * (256 - bldr)) / 256;
        } else if (u32WDR_PxValue > pstBayernr->au16WDRFrameThr[3] && u32WDR_PxValue < pstBayernr->au16WDRFrameThr[4]) {
            // short frame strength
            sigma /= DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[1]));
            sigma  = sigma * pstBayernr->stWDR.au8WDRFrameStr[1] / 16;
        } else if (u32WDR_PxValue > pstBayernr->au16WDRFrameThr[4] && u32WDR_PxValue < pstBayernr->au16WDRFrameThr[5]) {
            HI_U32   sgmFS = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[0]));
            HI_U32   sgmFL = sigma / DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[1]));
            HI_U32   bldr  = (u32WDR_PxValue - (HI_U32)pstBayernr->au16WDRFrameThr[4]) *  ISP_BITFIX(8) / MAX2(pstBayernr->au16WDRFrameThr[5] - pstBayernr->au16WDRFrameThr[4], 1);

            sgmFS = sgmFS * pstBayernr->stWDR.au8WDRFrameStr[0] / 16;
            sgmFL = sgmFL * pstBayernr->stWDR.au8WDRFrameStr[1] / 16;
            sigma = (sgmFS * bldr + sgmFL * (256 - bldr)) / 256;
        } else {
            // short short frame strength
            sigma /= DIV_0_TO_1((HI_U32)Sqrt32(pstBayernr->au32ExpoValues[0]));
            sigma  = sigma * pstBayernr->stWDR.au8WDRFrameStr[0] / 16;
        }

        WDR_JNLM_Limit_LUT[i] = sigma;
        sigma_max = (sigma_max < sigma) ? sigma : sigma_max;
    }

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        pstDynaCfg->au8JnlmLimitLut[i] = (HI_U8)((WDR_JNLM_Limit_LUT[i] * 128 + sigma_max / 2) / DIV_0_TO_1(sigma_max));
    }

    u32LmtNpThresh = (HI_U32)(sigma_max * (1 << (HI_ISP_BAYERNR_BITDEP - 8 - 3)) / ISP_BITFIX(10)); // sad win size, move to hw
    pstDynaCfg->u16LmtNpThresh = (u32LmtNpThresh > u16BitMask) ? u16BitMask : u32LmtNpThresh;

    return HI_SUCCESS;
}

static HI_VOID isp_bayernr_get_sync_framethr(VI_PIPE ViPipe, isp_usr_ctx *pstIspCtx, ISP_BAYERNR_S *pstBayernr)
{
    HI_U8 u8SyncIndex;
    HI_S8 i;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;
    ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);

    if (IS_OFFLINE_MODE(pstIspCtx->block_attr.running_mode) ||
        IS_STRIPING_MODE(pstIspCtx->block_attr.running_mode)) {    /* offline mode */
        if (IS_HALF_WDR_MODE(pstIspCtx->sns_wdr_mode)) {
            u8SyncIndex = MIN2(pstSnsRegsInfo->u8Cfg2ValidDelayMax, CFG2VLD_DLY_LIMIT - 1);
        } else {
            u8SyncIndex = MIN2(pstSnsRegsInfo->u8Cfg2ValidDelayMax + 1, CFG2VLD_DLY_LIMIT - 1);
        }
    } else {    /* online mode */
        if (IS_HALF_WDR_MODE(pstIspCtx->sns_wdr_mode)) {
            u8SyncIndex = CLIP3((HI_S8)pstSnsRegsInfo->u8Cfg2ValidDelayMax - 1, 0, CFG2VLD_DLY_LIMIT - 1);
        } else {
            u8SyncIndex = MIN2(pstSnsRegsInfo->u8Cfg2ValidDelayMax, CFG2VLD_DLY_LIMIT - 1);
        }
    }

    for (i = CFG2VLD_DLY_LIMIT - 2; i >= 0; i--) {
        pstBayernr->au16WDRSyncFrameThr[i + 1][0] = pstBayernr->au16WDRSyncFrameThr[i][0];
        pstBayernr->au16WDRSyncFrameThr[i + 1][1] = pstBayernr->au16WDRSyncFrameThr[i][1];
    }

    pstBayernr->au16WDRSyncFrameThr[0][0] = hi_ext_system_wdr_longthr_read(ViPipe);
    pstBayernr->au16WDRSyncFrameThr[0][1] = hi_ext_system_wdr_shortthr_read(ViPipe);

    memcpy(pstBayernr->au16WDRFrameThr, pstBayernr->au16WDRSyncFrameThr[u8SyncIndex], (WDR_MAX_FRAME_NUM + 2) * sizeof(HI_U16));
}


HI_S32 hiisp_bayernr_fw_wdr(HI_U32 u32Iso, VI_PIPE ViPipe, ISP_BAYERNR_DYNA_CFG_S *pstDynaCfg,
                            ISP_BAYERNR_USR_CFG_S *pstUsrCfg)
{
    HI_U8  i;
    HI_U32 u32IsoLevel;
    HI_U32 u32ISO1 = 0, u32ISO2 = 0;
    HI_U32 k, cprClip;
    HI_FLOAT fCalibrationCoef = 0.0f;
    ISP_BAYERNR_S      *pstBayernr = HI_NULL;
    hi_isp_cmos_default *sns_dft  = HI_NULL;
    isp_usr_ctx          *pstIspCtx  = HI_NULL;

    isp_sensor_get_default(ViPipe, &sns_dft);
    BAYERNR_GET_CTX(ViPipe, pstBayernr);
    ISP_CHECK_POINTER(pstBayernr);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(sns_dft->bayer_nr);

    isp_bayernr_get_sync_framethr(ViPipe, pstIspCtx, pstBayernr);


    pstUsrCfg->bBnrMonoSensorEn = sns_dft->bayer_nr->bnr_mono_sensor_en;     // MonoSensor, waiting to get
    pstUsrCfg->bBnrLscEn        = pstBayernr->bNrLscEnable;
    pstUsrCfg->u8BnrLscRatio    = pstBayernr->u8NrLscRatio;

    // Noise LUT
    fCalibrationCoef = Bayernr_getKfromNoiseLut(sns_dft->noise_calibration.calibration_coef, sns_dft->noise_calibration.calibration_lut_num, u32Iso);
    k     = (HI_U32)(fCalibrationCoef * ISP_BITFIX(14));
    cprClip = ISP_BITFIX(12) / ISP_BITFIX(ADJ_D); // the slope of the beginning of Compression curve

    if ((pstIspCtx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (pstIspCtx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        NRLimitLut(ViPipe, pstDynaCfg, pstBayernr, u32Iso);
    } else {
        switch (pstBayernr->u8WdrFramesMerge) {
            case 2:  // WDR mode: 2 to 1
                NRLimitLut_WDR2to1(pstDynaCfg, pstBayernr, k, cprClip);
                break;

            case 3:  // WDR mode: 3 to 1
                NRLimitLut_WDR3to1(pstDynaCfg, pstBayernr, k, cprClip);
                break;

            case 4:  // WDR mode: 4 to 1
                NRLimitLut_WDR4to1(pstDynaCfg, pstBayernr, k, cprClip);
                break;
            default:
                break;
        }
    }

    if (0 == HI_WDR_EINIT_BLCNR) {
        pstDynaCfg->au8JnlmLimitLut[0] = pstDynaCfg->au8JnlmLimitLut[3];
        pstDynaCfg->au8JnlmLimitLut[1] = pstDynaCfg->au8JnlmLimitLut[3];
        pstDynaCfg->au8JnlmLimitLut[2] = pstDynaCfg->au8JnlmLimitLut[3];
    }

    if (u32Iso > g_au32BayernrIsoLut[HI_ISP_NR_LUT_LEN - 1]) {
        u32IsoLevel = HI_ISP_NR_LUT_LEN - 1;
        u32ISO1 = g_au32BayernrIsoLut[HI_ISP_NR_LUT_LEN - 1];
        u32ISO2 = g_au32BayernrIsoLut[HI_ISP_NR_LUT_LEN - 1];
    } else if (u32Iso <= g_au32BayernrIsoLut[0]) {
        u32IsoLevel = 0;
        u32ISO1 = 0;
        u32ISO2 = g_au32BayernrIsoLut[0];
    } else {
        u32IsoLevel = NRGetIsoIndex(u32Iso);
        u32ISO1 = g_au32BayernrIsoLut[u32IsoLevel - 1];
        u32ISO2 = g_au32BayernrIsoLut[u32IsoLevel];
    }

    NRCfg(ViPipe, pstDynaCfg, pstBayernr, u32IsoLevel, u32ISO2, u32ISO1, u32Iso);
    NRExtCfg(ViPipe, pstDynaCfg, pstBayernr, u32IsoLevel, u32ISO2, u32ISO1, u32Iso);

    if (pstUsrCfg->bBnrMonoSensorEn == 1) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstDynaCfg->bMedcEnable     = HI_FALSE;
            pstDynaCfg->au8BnrCRatio[i] = 0;
            pstDynaCfg->au8AmedLevel[i] = 0;
        }
    }

    pstDynaCfg->bBnrLutUpdateEn = HI_TRUE;
    pstDynaCfg->u16RLmtRgain = pstIspCtx->linkage.white_balance_gain[0];
    pstDynaCfg->u16RLmtBgain = pstIspCtx->linkage.white_balance_gain[3];

    pstDynaCfg->bResh = HI_TRUE;
    pstUsrCfg->bResh  = HI_TRUE;

    return HI_SUCCESS;
}

static HI_BOOL __inline CheckBnrOpen(ISP_BAYERNR_S *pstBayernr)
{
    return (pstBayernr->bEnable == HI_TRUE);
}

HI_S32 ISP_BayernrRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                      HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_U8 i;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    ISP_BAYERNR_S *pstBayernr = HI_NULL;

    BAYERNR_GET_CTX(ViPipe, pstBayernr);
    ISP_CHECK_POINTER(pstBayernr);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    /* calculate every two interrupts */
    if (!pstBayernr->bInit) {
        return HI_SUCCESS;
    }

    pstBayernr->bEnable = hi_ext_system_bayernr_enable_read(ViPipe);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stBnrRegCfg.bBnrEnable = pstBayernr->bEnable;
    }

    pstRegCfg->cfg_key.bit1BayernrCfg = 1;

    /* check hardware setting */
    if (!CheckBnrOpen(pstBayernr)) {
        return HI_SUCCESS;
    }

    BayernrReadExtregs(ViPipe);
    BayernrReadProMode(ViPipe);

    if (pstBayernr->u8WdrFramesMerge > 1) {
        hiisp_bayernr_fw_wdr(pstIspCtx->linkage.iso, ViPipe, &pstRegCfg->alg_reg_cfg[0].stBnrRegCfg.stDynaRegCfg,
                             &pstRegCfg->alg_reg_cfg[0].stBnrRegCfg.stUsrRegCfg);
    } else {
        hiisp_bayernr_fw(pstIspCtx->linkage.iso, ViPipe, &pstRegCfg->alg_reg_cfg[0].stBnrRegCfg.stDynaRegCfg,
                         &pstRegCfg->alg_reg_cfg[0].stBnrRegCfg.stUsrRegCfg);
    }

    for (i = 1; i < pstIspCtx->block_attr.block_num; i++) {
        memcpy(&pstRegCfg->alg_reg_cfg[i].stBnrRegCfg.stDynaRegCfg, &pstRegCfg->alg_reg_cfg[0].stBnrRegCfg.stDynaRegCfg,
               sizeof(ISP_BAYERNR_DYNA_CFG_S));

        memcpy(&pstRegCfg->alg_reg_cfg[i].stBnrRegCfg.stUsrRegCfg, &pstRegCfg->alg_reg_cfg[0].stBnrRegCfg.stUsrRegCfg,
               sizeof(ISP_BAYERNR_USR_CFG_S));
    }

    return HI_SUCCESS;
}


static __inline HI_S32 BayerNrImageResWrite(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{
    return HI_SUCCESS;
}

HI_S32 ISP_BayernrCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    ISP_BAYERNR_S *pstBayernr = HI_NULL;
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            BAYERNR_GET_CTX(ViPipe, pstBayernr);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_CHECK_POINTER(pstBayernr);

            pstBayernr->bInit = HI_FALSE;
            ISP_BayernrInit(ViPipe, (HI_VOID *)&pRegCfg->reg_cfg);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            break;
        case ISP_PROC_WRITE:
            BayernrProcWrite(ViPipe, (hi_isp_ctrl_proc_write *)pValue);
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_BayernrExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr *pRegCfg   = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stBnrRegCfg.bBnrEnable = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1BayernrCfg = 1;

    BayerNrCtxExit(ViPipe);

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_bayer_nr(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_BAYERNR;
    pstAlgs->alg_func.pfn_alg_init = ISP_BayernrInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_BayernrRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_BayernrCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_BayernrExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* End of #ifdef __cplusplus */
