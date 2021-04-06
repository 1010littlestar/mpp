/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include <stdio.h>
#include <string.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_sensor.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define WEIGHT_TABLE_WIDTH  17
#define WEIGHT_TABLE_HEIGHT 15

static HI_U32 AePirisLinToFNO(HI_U32 u32Value)
{
    HI_U32 i = 0;
    HI_U32 u32Tmp = u32Value;

    if (u32Value <= 1) {
        i = 0;
    } else {
        while (u32Tmp > 1) {
            u32Tmp /= 2;
            i++;
        }
    }

    return i;
}

static HI_S32 AeGetDCFInfo(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    hi_isp_dcf_update_info *pstIspUpdateInfo = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    pstIspUpdateInfo = &pstIspCtx->dcf_update_info;

    if (pstIspUpdateInfo == HI_NULL) {
        return HI_FAILURE;
    }

    pstIspUpdateInfo->exposure_bias_value = pstAeResult->stUpdateInfo.u32ExposureBiasValue;
    pstIspUpdateInfo->iso_speed_ratings   = pstAeResult->stUpdateInfo.u32ISOSpeedRatings;
    pstIspUpdateInfo->exposure_program    = pstAeResult->stUpdateInfo.u8ExposureProgram;
    pstIspUpdateInfo->exposure_mode       = pstAeResult->stUpdateInfo.u8ExposureMode;
    pstIspUpdateInfo->exposure_time       = pstAeResult->stUpdateInfo.u32ExposureTime;
    pstIspUpdateInfo->max_aperture_value  = pstAeResult->stUpdateInfo.u32MaxApertureValue;
    pstIspUpdateInfo->f_number            = pstAeResult->stUpdateInfo.u32FNumber;

    return HI_SUCCESS;
}

static HI_S32 AeGetFrameInfo(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    hi_isp_frame_info *pstIspFrame = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    pstIspFrame = &pstIspCtx->frame_info;

    if (pstIspFrame == HI_NULL) {
        return HI_FAILURE;
    }

    pstIspFrame->exposure_time = pstAeResult->u32IntTime[0] >> 4;
    pstIspFrame->iso = pstAeResult->u32Iso;
    pstIspFrame->isp_dgain = pstAeResult->u32IspDgain << 2;
    pstIspFrame->again = pstAeResult->u32Again;
    pstIspFrame->dgain = pstAeResult->u32Dgain;

    if (IS_2to1_WDR_MODE(pstIspCtx->sns_wdr_mode)) {
        pstIspFrame->ratio[0] = ((HI_U64)(pstAeResult->u32IntTime[1]) * (pstAeResult->u32Iso << 2) + ((pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF) >> 4) / 2) / DIV_0_TO_1((pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF) >> 4);
        pstIspFrame->ratio[1] = 64;
        pstIspFrame->ratio[2] = 64;
    } else if (IS_3to1_WDR_MODE(pstIspCtx->sns_wdr_mode)) {
        pstIspFrame->ratio[0] = (pstAeResult->u32IntTime[1] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[0] >> 4);
        pstIspFrame->ratio[1] = (pstAeResult->u32IntTime[2] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[1] >> 4);
        pstIspFrame->ratio[2] = 64;
    } else if (IS_4to1_WDR_MODE(pstIspCtx->sns_wdr_mode)) {
        pstIspFrame->ratio[0] = (pstAeResult->u32IntTime[1] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[0] >> 4);
        pstIspFrame->ratio[1] = (pstAeResult->u32IntTime[2] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[1] >> 4);
        pstIspFrame->ratio[2] = (pstAeResult->u32IntTime[3] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[2] >> 4);
    } else {
        pstIspFrame->ratio[0] = 64;
        pstIspFrame->ratio[1] = 64;
        pstIspFrame->ratio[2] = 64;
    }

    pstIspFrame->f_number = AePirisLinToFNO(pstAeResult->u32PirisGain);
    pstIspFrame->hmax_times = pstAeResult->u32HmaxTimes;
    pstIspFrame->vmax = pstAeResult->u32Vmax;

    return HI_SUCCESS;
}

static HI_VOID AeRegsRangeCheck(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult)
{
    HI_U32 i, j;

    pstAeResult->stStatAttr.u8AEBESel       = MIN2(pstAeResult->stStatAttr.u8AEBESel, 2);
    pstAeResult->stStatAttr.u8FourPlaneMode = MIN2(pstAeResult->stStatAttr.u8FourPlaneMode, 1);
    if (pstAeResult->stStatAttr.u8FourPlaneMode) {
        pstAeResult->stStatAttr.u8HistSkipX = MIN2(pstAeResult->stStatAttr.u8HistSkipX, 6);
    } else {
        pstAeResult->stStatAttr.u8HistSkipX = MIN2(pstAeResult->stStatAttr.u8HistSkipX, 6);
        pstAeResult->stStatAttr.u8HistSkipX = MAX2(pstAeResult->stStatAttr.u8HistSkipX, 1);
    }
    pstAeResult->stStatAttr.u8HistSkipY   = MIN2(pstAeResult->stStatAttr.u8HistSkipY, 6);
    pstAeResult->stStatAttr.u8HistOffsetX = MIN2(pstAeResult->stStatAttr.u8HistOffsetX, 1);
    pstAeResult->stStatAttr.u8HistOffsetY = MIN2(pstAeResult->stStatAttr.u8HistOffsetY, 1);

    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            pstAeResult->stStatAttr.au8WeightTable[ViPipe][i][j] = MIN2(pstAeResult->stStatAttr.au8WeightTable[ViPipe][i][j], 15);
        }
    }

    pstAeResult->stStatAttr.u8HistMode    = MIN2(pstAeResult->stStatAttr.u8HistMode, 1);
    pstAeResult->stStatAttr.u8AverMode    = MIN2(pstAeResult->stStatAttr.u8AverMode, 1);
    pstAeResult->stStatAttr.u8MaxGainMode = MIN2(pstAeResult->stStatAttr.u8MaxGainMode, 1);
}

static HI_VOID AeResRegsDefault(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8  i;
    HI_U8  u8BlockNum = 0;
    HI_U16 u16Overlap;

    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_rect stBlockRect;
    ISP_MG_STATIC_CFG_S    *pstMgStaticRegCfg  = HI_NULL;
    ISP_AE_STATIC_CFG_S    *pstAeStaticRegCfg  = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8BlockNum = pstIspCtx->block_attr.block_num;
    u16Overlap = pstIspCtx->block_attr.over_lap;

    /* AE BE Configs */
    for (i = 0; i < u8BlockNum; i++) {
        /* AE&MG&DG BLC Configs */
        pstAeStaticRegCfg = &pstRegCfg->alg_reg_cfg[i].stAeRegCfg.stStaticRegCfg;
        pstMgStaticRegCfg = &pstRegCfg->alg_reg_cfg[i].stMgRegCfg.stStaticRegCfg;

        /* AE&MG&DG Size Configs */
        ISP_GetBlockRect(&stBlockRect, &pstIspCtx->block_attr, i);

        pstAeStaticRegCfg->u16BECropPosY      = 0;
        pstAeStaticRegCfg->u16BECropOutHeight = stBlockRect.height;
        pstMgStaticRegCfg->u16CropPosY        = 0;
        pstMgStaticRegCfg->u16CropOutHeight   = stBlockRect.height;

        /* AE&MG Overlap Configs */
        if (i == 0) {
            if (u8BlockNum > 1) {
                pstAeStaticRegCfg->u16BECropPosX      = 0;
                pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.width - u16Overlap;

                pstMgStaticRegCfg->u16CropPosX        = 0;
                pstMgStaticRegCfg->u16CropOutWidth    = stBlockRect.width - u16Overlap;
            } else {
                pstAeStaticRegCfg->u16BECropPosX      = 0;
                pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.width;

                pstMgStaticRegCfg->u16CropPosX        = 0;
                pstMgStaticRegCfg->u16CropOutWidth    = stBlockRect.width;
            }
        } else if (i == (u8BlockNum - 1)) {
            pstAeStaticRegCfg->u16BECropPosX      = u16Overlap;
            pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.width - u16Overlap;

            pstMgStaticRegCfg->u16CropPosX        = u16Overlap;
            pstMgStaticRegCfg->u16CropOutWidth    = stBlockRect.width - u16Overlap;
        } else {
            pstAeStaticRegCfg->u16BECropPosX      = u16Overlap;
            pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.width - (u16Overlap << 1);

            pstMgStaticRegCfg->u16CropPosX        = u16Overlap;
            pstMgStaticRegCfg->u16CropOutWidth    = stBlockRect.width - (u16Overlap << 1);
        }
    }

    /* AE FE Configs */
    pstAeStaticRegCfg = &pstRegCfg->alg_reg_cfg[0].stAeRegCfg.stStaticRegCfg;

    /* Crop Configs */
    pstAeStaticRegCfg->u16FECropPosX = 0;
    pstAeStaticRegCfg->u16FECropPosY = 0;
    pstAeStaticRegCfg->u16FECropOutHeight = pstIspCtx->sys_rect.height;
    pstAeStaticRegCfg->u16FECropOutWidth  = pstIspCtx->sys_rect.width;

    hi_ext_system_ae_crop_en_write(ViPipe, HI_EXT_SYSTEM_CROP_EN_DEFAULT);
    hi_ext_system_ae_crop_x_write(ViPipe, 0);
    hi_ext_system_ae_crop_y_write(ViPipe, 0);
    hi_ext_system_ae_crop_height_write(ViPipe, pstIspCtx->block_attr.frame_rect.height);
    hi_ext_system_ae_crop_width_write(ViPipe, pstIspCtx->block_attr.frame_rect.width);

    pstRegCfg->cfg_key.bit1AeCfg1 = HI_TRUE;
    pstRegCfg->cfg_key.bit1AeCfg2 = HI_TRUE;
}

static HI_VOID AeResReadExtregs(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8  i;
    HI_U8  u8BlockNum = 0;
    HI_U8  u8CropEn;
    HI_U16 u16Overlap;
    HI_U32 u16CropX, u16CropY, u16CropHeight, u16CropWidth;

    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_rect stBlockRect;
    ISP_MG_STATIC_CFG_S    *pstMgStaticRegCfg = HI_NULL;
    ISP_AE_STATIC_CFG_S    *pstAeStaticRegCfg  = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8BlockNum = pstIspCtx->block_attr.block_num;
    u16Overlap = pstIspCtx->block_attr.over_lap;

    u8CropEn = hi_ext_system_ae_crop_en_read(ViPipe);
    u16CropX = (hi_ext_system_ae_crop_x_read(ViPipe) >> 2) << 2;
    u16CropY = (hi_ext_system_ae_crop_y_read(ViPipe) >> 2) << 2;
    u16CropHeight = (hi_ext_system_ae_crop_height_read(ViPipe) >> 2) << 2;
    u16CropWidth = (hi_ext_system_ae_crop_width_read(ViPipe) >> 2) << 2;

    u16CropWidth  = MAX2(u16CropWidth,  AE_MIN_WIDTH);
    u16CropHeight = MAX2(u16CropHeight, AE_MIN_HEIGHT);
    u16CropWidth  = MIN2(u16CropWidth, pstIspCtx->block_attr.frame_rect.width);
    u16CropHeight = MIN2(u16CropHeight, pstIspCtx->block_attr.frame_rect.height);
    u16CropX      = MIN2(u16CropX, (pstIspCtx->block_attr.frame_rect.width - u16CropWidth));
    u16CropY      = MIN2(u16CropY, (pstIspCtx->block_attr.frame_rect.height - u16CropHeight));

    /* AE BE Configs */
    for (i = 0; i < u8BlockNum; i++) {
        /* AE&MG&DG BLC Configs */
        pstAeStaticRegCfg = &pstRegCfg->alg_reg_cfg[i].stAeRegCfg.stStaticRegCfg;
        pstMgStaticRegCfg = &pstRegCfg->alg_reg_cfg[i].stMgRegCfg.stStaticRegCfg;

        /* AE&MG&DG Size Configs */
        ISP_GetBlockRect(&stBlockRect, &pstIspCtx->block_attr, i);

        /* AE&MG Overlap Configs */
        if (i == 0) {
            if (u8BlockNum > 1) {
                pstAeStaticRegCfg->u16BECropPosX      = 0;
                pstAeStaticRegCfg->u16BECropPosY      = 0;
                pstAeStaticRegCfg->u16BECropOutHeight = stBlockRect.height;
                pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.width - u16Overlap;

                pstMgStaticRegCfg->u16CropPosX      = 0;
                pstMgStaticRegCfg->u16CropPosY      = 0;
                pstMgStaticRegCfg->u16CropOutHeight = stBlockRect.height;
                pstMgStaticRegCfg->u16CropOutWidth  = stBlockRect.width - u16Overlap;
            } else {
                if (u8CropEn) {
                    pstAeStaticRegCfg->u16BECropPosX      = u16CropX;
                    pstAeStaticRegCfg->u16BECropPosY      = u16CropY;
                    pstAeStaticRegCfg->u16BECropOutHeight = u16CropHeight;
                    pstAeStaticRegCfg->u16BECropOutWidth  = u16CropWidth;

                    pstMgStaticRegCfg->u16CropPosX      = u16CropX;
                    pstMgStaticRegCfg->u16CropPosY      = u16CropY;
                    pstMgStaticRegCfg->u16CropOutHeight = u16CropHeight;
                    pstMgStaticRegCfg->u16CropOutWidth  = u16CropWidth;
                } else {
                    pstAeStaticRegCfg->u16BECropPosX      = 0;
                    pstAeStaticRegCfg->u16BECropPosY      = 0;
                    pstAeStaticRegCfg->u16BECropOutHeight = stBlockRect.height;
                    pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.width;

                    pstMgStaticRegCfg->u16CropPosX      = 0;
                    pstMgStaticRegCfg->u16CropPosY      = 0;
                    pstMgStaticRegCfg->u16CropOutHeight = stBlockRect.height;
                    pstMgStaticRegCfg->u16CropOutWidth  = stBlockRect.width;
                }
            }
        } else if (i == (u8BlockNum - 1)) {
            pstAeStaticRegCfg->u16BECropPosX      = u16Overlap;
            pstAeStaticRegCfg->u16BECropPosY      = 0;
            pstAeStaticRegCfg->u16BECropOutHeight = stBlockRect.height;
            pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.width - u16Overlap;

            pstMgStaticRegCfg->u16CropPosX      = u16Overlap;
            pstMgStaticRegCfg->u16CropPosY      = 0;
            pstMgStaticRegCfg->u16CropOutHeight = stBlockRect.height;
            pstMgStaticRegCfg->u16CropOutWidth  = stBlockRect.width - u16Overlap;
        } else {
            pstAeStaticRegCfg->u16BECropPosX      = u16Overlap;
            pstAeStaticRegCfg->u16BECropPosY      = 0;
            pstAeStaticRegCfg->u16BECropOutHeight = stBlockRect.height;
            pstAeStaticRegCfg->u16BECropOutWidth  = stBlockRect.width - (u16Overlap << 1);

            pstMgStaticRegCfg->u16CropPosX      = u16Overlap;
            pstMgStaticRegCfg->u16CropPosY      = 0;
            pstMgStaticRegCfg->u16CropOutHeight = stBlockRect.height;
            pstMgStaticRegCfg->u16CropOutWidth  = stBlockRect.width - (u16Overlap << 1);
        }
    }

    /* AE FE Configs */
    pstAeStaticRegCfg = &pstRegCfg->alg_reg_cfg[0].stAeRegCfg.stStaticRegCfg;

    /* Crop Configs */
    if (u8CropEn) {
        pstAeStaticRegCfg->u16FECropPosX = u16CropX;
        pstAeStaticRegCfg->u16FECropPosY = u16CropY;
        pstAeStaticRegCfg->u16FECropOutHeight = u16CropHeight;
        pstAeStaticRegCfg->u16FECropOutWidth  = u16CropWidth;
    } else {
        pstAeStaticRegCfg->u16FECropPosX = 0;
        pstAeStaticRegCfg->u16FECropPosY = 0;
        pstAeStaticRegCfg->u16FECropOutHeight = pstIspCtx->sys_rect.height;
        pstAeStaticRegCfg->u16FECropOutWidth  = pstIspCtx->sys_rect.width;
    }

    pstRegCfg->cfg_key.bit1AeCfg1 = HI_TRUE;
    pstRegCfg->cfg_key.bit1AeCfg2 = HI_TRUE;

}


static HI_VOID AeRegsDefault(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i, j, k;
    HI_U8 u8WDRMode = 0;
    HI_U8 u8BlockNum = 0;
    HI_U8 u8HistSkipX = 0;
    HI_U8 u8HistSkipY = 0;
    HI_U8 u8HistOffsetX = 0;
    HI_U8 u8HistOffsetY = 0;
    HI_U8 u8BlockOffsetX = 0;
    HI_U8 u8SensorPatternType = 0;

    isp_usr_ctx *pstIspCtx = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level   = HI_NULL;
    ISP_MG_DYNA_CFG_S      *pstMgDynaRegCfg    = HI_NULL;
    ISP_MG_STATIC_CFG_S    *pstMgStaticRegCfg  = HI_NULL;
    ISP_AE_STATIC_CFG_S    *pstAeStaticRegCfg  = HI_NULL;
    ISP_AE_DYNA_CFG_S      *pstAeDynaRegCfg    = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    isp_sensor_get_blc(ViPipe, &sns_black_level);

    u8BlockNum = pstIspCtx->block_attr.block_num;
    u8WDRMode  = pstIspCtx->sns_wdr_mode;
    u8SensorPatternType = hi_ext_system_rggb_cfg_read(ViPipe);

    HI_U8 au8WeightTable[WEIGHT_TABLE_HEIGHT][WEIGHT_TABLE_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},
        {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},
        {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) {
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                au8WeightTable[i][j] = 1;
            }
        }
    }

    if ((u8SensorPatternType == HI_ISP_TOP_RGGB_START_R_GR_GB_B) || (u8SensorPatternType == HI_ISP_TOP_RGGB_START_B_GB_GR_R)) {
        u8HistSkipX = 1;
        u8HistSkipY = 1;
        u8HistOffsetX = 1;
        u8HistOffsetY = 0;
    } else {
        u8HistSkipX = 1;
        u8HistSkipY = 1;
        u8HistOffsetX = 0;
        u8HistOffsetY = 0;
    }

    /* AE BE Configs */
    for (i = 0; i < u8BlockNum; i++) {
        /* AE&MG&DG BLC Configs */
        pstAeDynaRegCfg   = &pstRegCfg->alg_reg_cfg[i].stAeRegCfg.stDynaRegCfg;
        pstAeStaticRegCfg = &pstRegCfg->alg_reg_cfg[i].stAeRegCfg.stStaticRegCfg;

        pstMgDynaRegCfg   = &pstRegCfg->alg_reg_cfg[i].stMgRegCfg.stDynaRegCfg;
        pstMgStaticRegCfg = &pstRegCfg->alg_reg_cfg[i].stMgRegCfg.stStaticRegCfg;

        pstAeStaticRegCfg->u8BEEnable  = HI_TRUE;
        pstMgStaticRegCfg->u8Enable    = HI_TRUE;

        /* AE&MG WDR Configs */
        pstAeDynaRegCfg->u8BEBitMove    = 0;
        pstAeDynaRegCfg->u8BEGammaLimit = 6;
        pstMgDynaRegCfg->u8BitMove      = 0;
        pstMgDynaRegCfg->u8GammaLimit   = 3;
        if (IS_LINEAR_MODE(u8WDRMode)) {
            pstAeDynaRegCfg->u8BEHistGammaMode = 0;
            pstAeDynaRegCfg->u8BEAverGammaMode = 0;
            pstMgDynaRegCfg->u8GammaMode       = 0;
        } else {
            pstAeDynaRegCfg->u8BEHistGammaMode = 1;
            pstAeDynaRegCfg->u8BEAverGammaMode = 1;
            pstMgDynaRegCfg->u8GammaMode       = 1;
        }

        /* MPI Configs */
        pstAeDynaRegCfg->u8BEAESel         = 1;
        pstAeDynaRegCfg->u8BEFourPlaneMode = 0;
        pstAeDynaRegCfg->u8BEHistSkipX     = u8HistSkipX;
        pstAeDynaRegCfg->u8BEHistSkipY     = u8HistSkipY;
        pstAeDynaRegCfg->u8BEHistOffsetX   = u8HistOffsetX;
        pstAeDynaRegCfg->u8BEHistOffsetY   = u8HistOffsetY;

        /* Weight Table Configs */
        pstAeDynaRegCfg->u8BEWightTableUpdate = HI_TRUE;
        if (i < WEIGHT_TABLE_WIDTH % DIV_0_TO_1(u8BlockNum)) {
            pstAeDynaRegCfg->u8BEWeightTableWidth = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum) + 1;
            pstMgDynaRegCfg->u8ZoneWidth          = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum) + 1;
        } else {
            pstAeDynaRegCfg->u8BEWeightTableWidth = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum);
            pstMgDynaRegCfg->u8ZoneWidth          = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum);
        }
        pstAeDynaRegCfg->u8BEWeightTableHeight = WEIGHT_TABLE_HEIGHT;
        pstMgDynaRegCfg->u8ZoneHeight          = WEIGHT_TABLE_HEIGHT;
        for (j = 0; j < pstAeDynaRegCfg->u8BEWeightTableHeight; j++) {
            for (k = 0; k < pstAeDynaRegCfg->u8BEWeightTableWidth; k++) {
                pstAeDynaRegCfg->au8BEWeightTable[j][k] = au8WeightTable[j][k + u8BlockOffsetX];
            }
        }
        u8BlockOffsetX += pstAeDynaRegCfg->u8BEWeightTableWidth;
    }

    /* AE FE Configs */
    pstAeDynaRegCfg   = &pstRegCfg->alg_reg_cfg[0].stAeRegCfg.stDynaRegCfg;
    pstAeStaticRegCfg = &pstRegCfg->alg_reg_cfg[0].stAeRegCfg.stStaticRegCfg;
    pstMgStaticRegCfg = &pstRegCfg->alg_reg_cfg[0].stMgRegCfg.stStaticRegCfg;

    /* BLC Configs */
    pstAeStaticRegCfg->u8FEEnable = HI_TRUE;

    /* WDR Configs */
    pstAeDynaRegCfg->u8FEBitMove       = 0;
    pstAeDynaRegCfg->u8FEGammaLimit    = 6;
    pstAeDynaRegCfg->u8FEHistGammaMode = 0;
    pstAeDynaRegCfg->u8FEAverGammaMode = 0;

    /* MPI Configs */
    pstAeDynaRegCfg->u8FEFourPlaneMode = 0;
    pstAeDynaRegCfg->u8FEHistSkipX     = u8HistSkipX;
    pstAeDynaRegCfg->u8FEHistSkipY     = u8HistSkipY;
    pstAeDynaRegCfg->u8FEHistOffsetX   = u8HistOffsetX;
    pstAeDynaRegCfg->u8FEHistOffsetY   = u8HistOffsetY;

    /* Weight Tbale Configs */
    pstAeDynaRegCfg->u8FEWightTableUpdate  = HI_TRUE;
    pstAeDynaRegCfg->u8FEWeightTableWidth  = WEIGHT_TABLE_WIDTH;
    pstAeDynaRegCfg->u8FEWeightTableHeight = WEIGHT_TABLE_HEIGHT;

    memcpy(pstAeDynaRegCfg->au8FEWeightTable, au8WeightTable, WEIGHT_TABLE_HEIGHT * WEIGHT_TABLE_WIDTH * sizeof(HI_U8));

    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            hi_ext_system_ae_weight_table_write(ViPipe, (i * AE_ZONE_COLUMN + j), au8WeightTable[i][j]);
        }
    }

    hi_ext_system_ae_be_sel_write(ViPipe, pstAeDynaRegCfg->u8BEAESel);
    hi_ext_system_ae_fourplanemode_write(ViPipe, pstAeDynaRegCfg->u8BEFourPlaneMode);

    hi_ext_system_ae_hist_skip_x_write(ViPipe, u8HistSkipX);
    hi_ext_system_ae_hist_skip_y_write(ViPipe, u8HistSkipY);
    hi_ext_system_ae_hist_offset_x_write(ViPipe, u8HistOffsetX);
    hi_ext_system_ae_hist_offset_y_write(ViPipe, u8HistOffsetY);

    hi_ext_system_ae_fe_en_write(ViPipe, pstAeStaticRegCfg->u8FEEnable);
    hi_ext_system_ae_be_en_write(ViPipe, pstAeStaticRegCfg->u8BEEnable);
    hi_ext_system_mg_en_write(ViPipe, pstMgStaticRegCfg->u8Enable);

    if (IS_LINEAR_MODE(u8WDRMode)) {
        hi_ext_system_ae_histmode_write(ViPipe, 0);
        hi_ext_system_ae_avermode_write(ViPipe, 0);
        hi_ext_system_ae_maxgainmode_write(ViPipe, 0);
    } else {
        hi_ext_system_ae_histmode_write(ViPipe, 1);
        hi_ext_system_ae_avermode_write(ViPipe, 1);
        hi_ext_system_ae_maxgainmode_write(ViPipe, 1);
    }

    AeResRegsDefault(ViPipe, pstRegCfg);

    hi_ext_system_smart_update_write(ViPipe, HI_EXT_SYSTEM_SMART_UPDATE_DEFAULT);
    for (i = 0; i < SMART_CLASS_MAX; i++) {
        hi_ext_system_smart_enable_write(ViPipe, i, HI_EXT_SYSTEM_SMART_ENABLE_DEFAULT);
        hi_ext_system_smart_available_write(ViPipe, i, HI_EXT_SYSTEM_SMART_AVAILABLE_DEFAULT);
        hi_ext_system_smart_luma_write(ViPipe, i, HI_EXT_SYSTEM_SMART_LUMA_DEFAULT);
    }

}

static HI_VOID AeReadExtregs(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult)
{
    HI_U32 i, j;

    if (!pstAeResult->stStatAttr.bHistAdjust) {
        if (pstAeResult->stStatAttr.u8AEBESel != hi_ext_system_ae_be_sel_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = HI_TRUE;
            pstAeResult->stStatAttr.u8AEBESel = hi_ext_system_ae_be_sel_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8FourPlaneMode != hi_ext_system_ae_fourplanemode_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = HI_TRUE;
            pstAeResult->stStatAttr.u8FourPlaneMode = hi_ext_system_ae_fourplanemode_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8HistOffsetX != hi_ext_system_ae_hist_offset_x_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = HI_TRUE;
            pstAeResult->stStatAttr.u8HistOffsetX = hi_ext_system_ae_hist_offset_x_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8HistOffsetY != hi_ext_system_ae_hist_offset_y_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = HI_TRUE;
            pstAeResult->stStatAttr.u8HistOffsetY = hi_ext_system_ae_hist_offset_y_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8HistSkipX != hi_ext_system_ae_hist_skip_x_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = HI_TRUE;
            pstAeResult->stStatAttr.u8HistSkipX = hi_ext_system_ae_hist_skip_x_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8HistSkipY != hi_ext_system_ae_hist_skip_y_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = HI_TRUE;
            pstAeResult->stStatAttr.u8HistSkipY = hi_ext_system_ae_hist_skip_y_read(ViPipe);
        }
    } else {
        hi_ext_system_ae_be_sel_write(ViPipe, pstAeResult->stStatAttr.u8AEBESel);
        hi_ext_system_ae_fourplanemode_write(ViPipe, pstAeResult->stStatAttr.u8FourPlaneMode);
        hi_ext_system_ae_hist_skip_x_write(ViPipe, pstAeResult->stStatAttr.u8HistSkipX);
        hi_ext_system_ae_hist_skip_y_write(ViPipe, pstAeResult->stStatAttr.u8HistSkipY);
        hi_ext_system_ae_hist_offset_x_write(ViPipe, pstAeResult->stStatAttr.u8HistOffsetX);
        hi_ext_system_ae_hist_offset_y_write(ViPipe, pstAeResult->stStatAttr.u8HistOffsetY);
    }

    if (!pstAeResult->stStatAttr.bWightTableUpdate) {
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                if (pstAeResult->stStatAttr.au8WeightTable[ViPipe][i][j] != hi_ext_system_ae_weight_table_read(ViPipe, (i * AE_ZONE_COLUMN + j))) {
                    pstAeResult->stStatAttr.bChange = HI_TRUE;
                    pstAeResult->stStatAttr.au8WeightTable[ViPipe][i][j] = hi_ext_system_ae_weight_table_read(ViPipe, (i * AE_ZONE_COLUMN + j));
                }
            }
        }
    } else {
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                hi_ext_system_ae_weight_table_write(ViPipe, (i * AE_ZONE_COLUMN + j), pstAeResult->stStatAttr.au8WeightTable[ViPipe][i][j]);
            }
        }
    }

    if (!pstAeResult->stStatAttr.bModeUpdate) {
        if (pstAeResult->stStatAttr.u8HistMode != hi_ext_system_ae_histmode_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = HI_TRUE;
            pstAeResult->stStatAttr.u8HistMode = hi_ext_system_ae_histmode_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8AverMode != hi_ext_system_ae_avermode_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = HI_TRUE;
            pstAeResult->stStatAttr.u8AverMode = hi_ext_system_ae_avermode_read(ViPipe);
        }

        if (pstAeResult->stStatAttr.u8MaxGainMode != hi_ext_system_ae_maxgainmode_read(ViPipe)) {
            pstAeResult->stStatAttr.bChange = HI_TRUE;
            pstAeResult->stStatAttr.u8MaxGainMode = hi_ext_system_ae_maxgainmode_read(ViPipe);
        }
    } else {
        hi_ext_system_ae_histmode_write(ViPipe, pstAeResult->stStatAttr.u8HistMode);
        hi_ext_system_ae_avermode_write(ViPipe, pstAeResult->stStatAttr.u8AverMode);
        hi_ext_system_ae_maxgainmode_write(ViPipe, pstAeResult->stStatAttr.u8MaxGainMode);
    }
}

static HI_VOID AeUpdateConfig(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult, isp_reg_cfg *pstRegCfg)
{
    HI_U8  u8BlockNum = 0;
    HI_U8  u8BlockOffsetX = 0;
    HI_S32 i, j, k = 0;

    ISP_AE_STAT_ATTR_S *pstStatAttr      = HI_NULL;
    isp_usr_ctx          *pstIspCtx        = HI_NULL;
    ISP_AE_DYNA_CFG_S  *pstAeDynaRegCfg  = HI_NULL;
    ISP_AE_STATIC_CFG_S *pstAeStatRegCfg = HI_NULL;
    ISP_AE_REG_CFG_2_S *pstAeRegCfg2     = HI_NULL;
    ISP_MG_DYNA_CFG_S  *pstMgDynaRegCfg  = HI_NULL;
    ISP_MG_STATIC_CFG_S *pstMgStatRegCfg = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8BlockNum   = pstIspCtx->block_attr.block_num;
    pstStatAttr  = &pstAeResult->stStatAttr;
    pstAeRegCfg2 = &pstRegCfg->alg_reg_cfg[0].stAeRegCfg2;

    for (i = 0; i < u8BlockNum; i++) {
        pstAeStatRegCfg = &pstRegCfg->alg_reg_cfg[i].stAeRegCfg.stStaticRegCfg;
        pstMgStatRegCfg = &pstRegCfg->alg_reg_cfg[i].stMgRegCfg.stStaticRegCfg;

        pstAeStatRegCfg->u8FEEnable = hi_ext_system_ae_fe_en_read(ViPipe);
        pstAeStatRegCfg->u8BEEnable = hi_ext_system_ae_be_en_read(ViPipe);
        pstMgStatRegCfg->u8Enable   = hi_ext_system_mg_en_read(ViPipe);
    }

    /* BE Configs Update */
    if (pstStatAttr->bHistAdjust) {
        pstAeDynaRegCfg = &pstRegCfg->alg_reg_cfg[0].stAeRegCfg.stDynaRegCfg;

        if ((pstAeDynaRegCfg->u8BEAESel != pstStatAttr->u8AEBESel)
            || (pstAeDynaRegCfg->u8BEFourPlaneMode != pstStatAttr->u8FourPlaneMode)
            || (pstAeDynaRegCfg->u8BEHistSkipX != pstStatAttr->u8HistSkipX)
            || (pstAeDynaRegCfg->u8BEHistSkipY != pstStatAttr->u8HistSkipY)
            || (pstAeDynaRegCfg->u8BEHistOffsetX != pstStatAttr->u8HistOffsetX)
            || (pstAeDynaRegCfg->u8BEHistOffsetY != pstStatAttr->u8HistOffsetY)) {
            pstStatAttr->bChange = HI_TRUE;
        }
    }

    if (pstStatAttr->bModeUpdate) {
        pstAeDynaRegCfg = &pstRegCfg->alg_reg_cfg[0].stAeRegCfg.stDynaRegCfg;
        pstMgDynaRegCfg = &pstRegCfg->alg_reg_cfg[0].stMgRegCfg.stDynaRegCfg;

        if ((pstAeDynaRegCfg->u8BEHistGammaMode != pstStatAttr->u8HistMode)
            || (pstAeDynaRegCfg->u8BEAverGammaMode != pstStatAttr->u8AverMode)
            || (pstMgDynaRegCfg->u8GammaMode != pstStatAttr->u8MaxGainMode)) {
            pstStatAttr->bChange = HI_TRUE;
        }
    }

    if (pstStatAttr->bWightTableUpdate) {
        pstAeDynaRegCfg = &pstRegCfg->alg_reg_cfg[0].stAeRegCfg.stDynaRegCfg;

        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                if (pstAeDynaRegCfg->au8BEWeightTable[i][j] != pstStatAttr->au8WeightTable[ViPipe][i][j]) {
                    pstStatAttr->bChange = HI_TRUE;
                }
            }
        }
    }

    for (i = 0; i < u8BlockNum; i++) {
        pstAeDynaRegCfg = &pstRegCfg->alg_reg_cfg[i].stAeRegCfg.stDynaRegCfg;
        pstMgDynaRegCfg = &pstRegCfg->alg_reg_cfg[i].stMgRegCfg.stDynaRegCfg;

        pstAeDynaRegCfg->u8BEAESel         = pstStatAttr->u8AEBESel;
        pstAeDynaRegCfg->u8BEFourPlaneMode = pstStatAttr->u8FourPlaneMode;
        pstAeDynaRegCfg->u8BEHistSkipX     = pstStatAttr->u8HistSkipX;
        pstAeDynaRegCfg->u8BEHistSkipY     = pstStatAttr->u8HistSkipY;
        pstAeDynaRegCfg->u8BEHistOffsetX   = pstStatAttr->u8HistOffsetX;
        pstAeDynaRegCfg->u8BEHistOffsetY   = pstStatAttr->u8HistOffsetY;

        pstAeDynaRegCfg->u8BEHistGammaMode = pstStatAttr->u8HistMode;
        pstAeDynaRegCfg->u8BEAverGammaMode = pstStatAttr->u8AverMode;
        pstMgDynaRegCfg->u8GammaMode       = pstStatAttr->u8MaxGainMode;

        pstAeDynaRegCfg->u8BEWightTableUpdate = HI_TRUE;

        if (i < WEIGHT_TABLE_WIDTH % DIV_0_TO_1(u8BlockNum)) {
            pstAeDynaRegCfg->u8BEWeightTableWidth = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum) + 1;
        } else {
            pstAeDynaRegCfg->u8BEWeightTableWidth = WEIGHT_TABLE_WIDTH / DIV_0_TO_1(u8BlockNum);
        }

        pstAeDynaRegCfg->u8BEWeightTableHeight = WEIGHT_TABLE_HEIGHT;
        for (j = 0; j < pstAeDynaRegCfg->u8BEWeightTableHeight; j++) {
            for (k = 0; k < pstAeDynaRegCfg->u8BEWeightTableWidth; k++) {
                pstAeDynaRegCfg->au8BEWeightTable[j][k] = pstStatAttr->au8WeightTable[ViPipe][j][k + u8BlockOffsetX];
            }
        }

        u8BlockOffsetX += pstAeDynaRegCfg->u8BEWeightTableWidth;

        pstRegCfg->cfg_key.bit1AeCfg1 = HI_TRUE;
        pstRegCfg->cfg_key.bit1AeCfg2 = HI_TRUE;
    }

    /* FE Configs Update */
    pstAeDynaRegCfg = &pstRegCfg->alg_reg_cfg[0].stAeRegCfg.stDynaRegCfg;

    pstAeDynaRegCfg->u8FEFourPlaneMode = pstStatAttr->u8FourPlaneMode;
    pstAeDynaRegCfg->u8FEHistSkipX     = pstStatAttr->u8HistSkipX;
    pstAeDynaRegCfg->u8FEHistSkipY     = pstStatAttr->u8HistSkipY;
    pstAeDynaRegCfg->u8FEHistOffsetX   = pstStatAttr->u8HistOffsetX;
    pstAeDynaRegCfg->u8FEHistOffsetY   = pstStatAttr->u8HistOffsetY;

    pstAeDynaRegCfg->u8FEWightTableUpdate  = HI_TRUE;
    pstAeDynaRegCfg->u8FEWeightTableWidth  = WEIGHT_TABLE_WIDTH;
    pstAeDynaRegCfg->u8FEWeightTableHeight = WEIGHT_TABLE_HEIGHT;

    for (j = 0; j < WEIGHT_TABLE_HEIGHT; j++) {
        for (k = 0; k < WEIGHT_TABLE_WIDTH; k++) {
            pstAeDynaRegCfg->au8FEWeightTable[j][k] = pstStatAttr->au8WeightTable[ViPipe][j][k];
        }
    }

    pstStatAttr->bChange = HI_FALSE;

    pstAeRegCfg2->u32IntTime[0] = pstAeResult->u32IntTime[0];
    pstAeRegCfg2->u32IntTime[1] = pstAeResult->u32IntTime[1];
    pstAeRegCfg2->u32IntTime[2] = pstAeResult->u32IntTime[2];
    pstAeRegCfg2->u32IntTime[3] = pstAeResult->u32IntTime[3];
    pstAeRegCfg2->u32IspDgain   = pstAeResult->u32IspDgain;
    pstAeRegCfg2->bPirisValid   = pstAeResult->bPirisValid;
    pstAeRegCfg2->s32PirisPos   = pstAeResult->s32PirisPos;
    pstAeRegCfg2->enFSWDRMode   = pstAeResult->enFSWDRMode;
    for (i = 0; i < 4; i++) {
        pstAeResult->au32WDRGain[i] = MAX2(pstAeResult->au32WDRGain[i], 0x100);
        pstAeRegCfg2->au32WDRGain[i] = pstAeResult->au32WDRGain[i];
    }
    /* be careful avoid overflow */
    if (IS_2to1_WDR_MODE(pstIspCtx->sns_wdr_mode)) {
        if (pstAeResult->bPirisValid == HI_TRUE) {
            pstAeRegCfg2->u64Exposure = (HI_U64)pstAeResult->u32IntTime[1] * pstAeResult->u32Iso * pstAeResult->u32PirisGain;
            pstAeRegCfg2->u64ExposureSF = (HI_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF * pstAeResult->u32PirisGain;
        } else {
            pstAeRegCfg2->u64Exposure = (HI_U64)pstAeResult->u32IntTime[1] * pstAeResult->u32Iso;
            pstAeRegCfg2->u64ExposureSF = (HI_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF;
        }
    } else {
        if (pstAeResult->bPirisValid == HI_TRUE) {
            pstAeRegCfg2->u64Exposure = (HI_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32Iso * pstAeResult->u32PirisGain;
        } else {
            pstAeRegCfg2->u64Exposure = (HI_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32Iso;
        }
    }
}

static HI_VOID AeUpdateLinkage(VI_PIPE ViPipe, ISP_AE_RESULT_S *pstAeResult, isp_linkage *pstLinkage)
{
    HI_S32 i = 0;
    HI_U8 u8DelayMax;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_U32 u32ExpRatioTmp;
    HI_U32 au32ExpRatioTmp[4];

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstLinkage->isp_dgain       = pstAeResult->u32IspDgain;
    pstLinkage->dgain           = pstAeResult->u32Dgain;
    pstLinkage->again           = pstAeResult->u32Again;
    pstLinkage->isp_dgain_shift = 8;

    for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
        pstLinkage->sync_iso_buf[i] = pstLinkage->sync_iso_buf[i - 1];
    }

    pstLinkage->sync_iso_buf[0] = pstAeResult->u32Iso;

    u8DelayMax = pstIspCtx->linkage.cfg2valid_delay_max;
    u8DelayMax = CLIP3(u8DelayMax, 1, ISP_SYNC_ISO_BUF_MAX - 1);

    if (pstLinkage->update_pos == 0) {
        pstLinkage->iso = MAX2(pstLinkage->sync_iso_buf[u8DelayMax], 100);
    } else {
        pstLinkage->iso = MAX2(pstLinkage->sync_iso_buf[u8DelayMax - 1], 100);
    }

    if (pstIspCtx->linkage.run_once == HI_TRUE) {
        pstLinkage->iso = MAX2(pstLinkage->sync_iso_buf[0], 100);
    }

    pstIspCtx->linkage.iso_done_frm_cnt = ISP_FrameCntGet(ViPipe);

    pstLinkage->sensor_iso = ((HI_U64)pstAeResult->u32Iso << 8) / DIV_0_TO_1(pstAeResult->u32IspDgain);
    pstLinkage->sensor_iso = (pstLinkage->sensor_iso < 100) ? 100 : pstLinkage->sensor_iso;
    pstLinkage->int_time = pstAeResult->u32IntTime[0] >> 4;
    pstLinkage->ae_run_interval = pstAeResult->u8AERunInterval;

    for (i = 0; i < 4; i++) {
        pstLinkage->wdr_gain[i] = 256;
    }

    if (IS_2to1_WDR_MODE(pstIspCtx->sns_wdr_mode)) {
        /* WDR exposure ratio is 6bit precision */
        pstLinkage->int_time = pstAeResult->u32IntTime[1] >> 4;
        u32ExpRatioTmp = (((HI_U64)pstAeResult->u32IntTime[1] * pstAeResult->u32Iso << 6) + (((HI_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF) / 2)) / DIV_0_TO_1((HI_U64)pstAeResult->u32IntTime[0] * pstAeResult->u32IsoSF);
        for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
            pstLinkage->sync_all_exp_ratio_buf[i] = pstLinkage->sync_all_exp_ratio_buf[i - 1];
            pstLinkage->sync_exp_ratio_buf[0][i] = pstLinkage->sync_exp_ratio_buf[0][i - 1];
        }

        pstLinkage->sync_all_exp_ratio_buf[0] = u32ExpRatioTmp;
        pstLinkage->sync_exp_ratio_buf[0][0] = u32ExpRatioTmp;

        if (pstLinkage->update_pos == 0) {
            pstLinkage->exp_ratio = pstLinkage->sync_all_exp_ratio_buf[u8DelayMax];
            pstLinkage->exp_ratio_lut[0] = pstLinkage->sync_exp_ratio_buf[0][u8DelayMax];
        } else {
            pstLinkage->exp_ratio = pstLinkage->sync_all_exp_ratio_buf[u8DelayMax - 1];
            pstLinkage->exp_ratio_lut[0] = pstLinkage->sync_exp_ratio_buf[0][u8DelayMax - 1];
        }

        if (pstIspCtx->linkage.run_once == HI_TRUE) {
            pstLinkage->exp_ratio = pstLinkage->sync_all_exp_ratio_buf[0];
            pstLinkage->exp_ratio_lut[0] = pstLinkage->sync_exp_ratio_buf[0][0];
        }

        hi_ext_system_actual_wdr_exposure_ratio_write(ViPipe, 0, pstLinkage->exp_ratio_lut[0]);
        hi_ext_system_actual_wdr_exposure_ratio_write(ViPipe, 1, 64);
        hi_ext_system_actual_wdr_exposure_ratio_write(ViPipe, 2, 64);

        pstLinkage->wdr_gain[0] = pstAeResult->au32WDRGain[0];
        pstLinkage->wdr_gain[1] = pstAeResult->au32WDRGain[1];
    } else if (IS_3to1_WDR_MODE(pstIspCtx->sns_wdr_mode)) {
        pstLinkage->int_time = pstAeResult->u32IntTime[2] >> 4;
        u32ExpRatioTmp = (pstAeResult->u32IntTime[2] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[0] >> 4);
        au32ExpRatioTmp[0] = (pstAeResult->u32IntTime[1] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[0] >> 4);
        au32ExpRatioTmp[1] = (pstAeResult->u32IntTime[2] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[1] >> 4);

        for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
            pstLinkage->sync_all_exp_ratio_buf[i] = pstLinkage->sync_all_exp_ratio_buf[i - 1];
            pstLinkage->sync_exp_ratio_buf[0][i] = pstLinkage->sync_exp_ratio_buf[0][i - 1];
            pstLinkage->sync_exp_ratio_buf[1][i] = pstLinkage->sync_exp_ratio_buf[1][i - 1];
        }

        pstLinkage->sync_all_exp_ratio_buf[0] = u32ExpRatioTmp;
        pstLinkage->sync_exp_ratio_buf[0][0] = au32ExpRatioTmp[0];
        pstLinkage->sync_exp_ratio_buf[1][0] = au32ExpRatioTmp[1];

        if (pstLinkage->update_pos == 0) {
            pstLinkage->exp_ratio = pstLinkage->sync_all_exp_ratio_buf[u8DelayMax];
            pstLinkage->exp_ratio_lut[0] = pstLinkage->sync_exp_ratio_buf[0][u8DelayMax];
            pstLinkage->exp_ratio_lut[1] = pstLinkage->sync_exp_ratio_buf[1][u8DelayMax];
        } else {
            pstLinkage->exp_ratio = pstLinkage->sync_all_exp_ratio_buf[u8DelayMax - 1];
            pstLinkage->exp_ratio_lut[0] = pstLinkage->sync_exp_ratio_buf[0][u8DelayMax - 1];
            pstLinkage->exp_ratio_lut[1] = pstLinkage->sync_exp_ratio_buf[1][u8DelayMax - 1];
        }

        if (pstIspCtx->linkage.run_once == HI_TRUE) {
            pstLinkage->exp_ratio = pstLinkage->sync_all_exp_ratio_buf[0];
            pstLinkage->exp_ratio_lut[0] = pstLinkage->sync_exp_ratio_buf[0][0];
            pstLinkage->exp_ratio_lut[1] = pstLinkage->sync_exp_ratio_buf[1][0];
        }

        hi_ext_system_actual_wdr_exposure_ratio_write(ViPipe, 0, pstLinkage->exp_ratio_lut[0]);
        hi_ext_system_actual_wdr_exposure_ratio_write(ViPipe, 1, pstLinkage->exp_ratio_lut[1]);
        hi_ext_system_actual_wdr_exposure_ratio_write(ViPipe, 2, 64);

        pstLinkage->wdr_gain[0] = pstAeResult->au32WDRGain[0];
        pstLinkage->wdr_gain[1] = pstAeResult->au32WDRGain[1];
        pstLinkage->wdr_gain[2] = pstAeResult->au32WDRGain[2];
    } else if (IS_4to1_WDR_MODE(pstIspCtx->sns_wdr_mode)) {
        pstLinkage->int_time = pstAeResult->u32IntTime[3] >> 4;
        u32ExpRatioTmp = (pstAeResult->u32IntTime[3] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[0] >> 4);
        au32ExpRatioTmp[0] = (pstAeResult->u32IntTime[1] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[0] >> 4);
        au32ExpRatioTmp[1] = (pstAeResult->u32IntTime[2] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[1] >> 4);
        au32ExpRatioTmp[2] = (pstAeResult->u32IntTime[3] << 2) / DIV_0_TO_1(pstAeResult->u32IntTime[2] >> 4);

        for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
            pstLinkage->sync_all_exp_ratio_buf[i] = pstLinkage->sync_all_exp_ratio_buf[i - 1];
            pstLinkage->sync_exp_ratio_buf[0][i] = pstLinkage->sync_exp_ratio_buf[0][i - 1];
            pstLinkage->sync_exp_ratio_buf[1][i] = pstLinkage->sync_exp_ratio_buf[1][i - 1];
            pstLinkage->sync_exp_ratio_buf[2][i] = pstLinkage->sync_exp_ratio_buf[2][i - 1];
        }

        pstLinkage->sync_all_exp_ratio_buf[0] = u32ExpRatioTmp;
        pstLinkage->sync_exp_ratio_buf[0][0] = au32ExpRatioTmp[0];
        pstLinkage->sync_exp_ratio_buf[1][0] = au32ExpRatioTmp[1];
        pstLinkage->sync_exp_ratio_buf[2][0] = au32ExpRatioTmp[2];

        if (pstLinkage->update_pos == 0) {
            pstLinkage->exp_ratio = pstLinkage->sync_all_exp_ratio_buf[u8DelayMax];
            pstLinkage->exp_ratio_lut[0] = pstLinkage->sync_exp_ratio_buf[0][u8DelayMax];
            pstLinkage->exp_ratio_lut[1] = pstLinkage->sync_exp_ratio_buf[1][u8DelayMax];
            pstLinkage->exp_ratio_lut[2] = pstLinkage->sync_exp_ratio_buf[1][u8DelayMax];
        } else {
            pstLinkage->exp_ratio = pstLinkage->sync_all_exp_ratio_buf[u8DelayMax - 1];
            pstLinkage->exp_ratio_lut[0] = pstLinkage->sync_exp_ratio_buf[0][u8DelayMax - 1];
            pstLinkage->exp_ratio_lut[1] = pstLinkage->sync_exp_ratio_buf[1][u8DelayMax - 1];
            pstLinkage->exp_ratio_lut[2] = pstLinkage->sync_exp_ratio_buf[2][u8DelayMax - 1];
        }

        if (pstIspCtx->linkage.run_once == HI_TRUE) {
            pstLinkage->exp_ratio = pstLinkage->sync_all_exp_ratio_buf[0];
            pstLinkage->exp_ratio_lut[0] = pstLinkage->sync_exp_ratio_buf[0][0];
            pstLinkage->exp_ratio_lut[1] = pstLinkage->sync_exp_ratio_buf[1][0];
            pstLinkage->exp_ratio_lut[2] = pstLinkage->sync_exp_ratio_buf[2][0];
        }

        hi_ext_system_actual_wdr_exposure_ratio_write(ViPipe, 0, pstLinkage->exp_ratio_lut[0]);
        hi_ext_system_actual_wdr_exposure_ratio_write(ViPipe, 1, pstLinkage->exp_ratio_lut[1]);
        hi_ext_system_actual_wdr_exposure_ratio_write(ViPipe, 2, pstLinkage->exp_ratio_lut[2]);

        pstLinkage->wdr_gain[0] = pstAeResult->au32WDRGain[0];
        pstLinkage->wdr_gain[1] = pstAeResult->au32WDRGain[1];
        pstLinkage->wdr_gain[2] = pstAeResult->au32WDRGain[2];
        pstLinkage->wdr_gain[3] = pstAeResult->au32WDRGain[3];
    } else {

    }

    if (pstAeResult->bPirisValid == HI_TRUE) {
        pstLinkage->piris_gain = pstAeResult->u32PirisGain;
    } else {
        pstLinkage->piris_gain = 0;
    }

    pstLinkage->pre_fswdr_mode = pstLinkage->fswdr_mode;
    pstLinkage->fswdr_mode     = pstAeResult->enFSWDRMode;
}

HI_S32 ISP_AeCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue);
HI_S32 ISP_AeInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 i;
    ISP_AE_PARAM_S stAeParam;
    isp_usr_ctx      *pstIspCtx = HI_NULL;
    isp_lib_node *pstLib    = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    AeRegsDefault(ViPipe, (isp_reg_cfg *)pRegCfg);
    isp_sensor_get_blc(ViPipe, &sns_black_level);

    stAeParam.SensorId      = pstIspCtx->bind_attr.sensor_id;
    stAeParam.u8WDRMode     = pstIspCtx->sns_wdr_mode;
    stAeParam.u8HDRMode     = pstIspCtx->hdr_attr.dynamic_range;
    stAeParam.enBayer       = hi_ext_system_rggb_cfg_read(ViPipe);
    stAeParam.f32Fps        = pstIspCtx->sns_image_mode.fps;
    stAeParam.u16BlackLevel = sns_black_level->black_level[1];

    stAeParam.stStitchAttr.bMainPipe       = pstIspCtx->stitch_attr.main_pipe;
    stAeParam.stStitchAttr.bStitchEnable   = pstIspCtx->stitch_attr.stitch_enable;
    stAeParam.stStitchAttr.u8StitchPipeNum = pstIspCtx->stitch_attr.stitch_pipe_num;
    memcpy(stAeParam.stStitchAttr.as8StitchBindId, pstIspCtx->stitch_attr.stitch_bind_id, sizeof(hi_s8) * VI_MAX_PIPE_NUM);

    /* init all registered ae libs */
    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (pstIspCtx->ae_lib_info.libs[i].used) {
            pstLib = &pstIspCtx->ae_lib_info.libs[i];
            if (pstLib->ae_regsiter.stAeExpFunc.pfn_ae_init != HI_NULL) {
                pstLib->ae_regsiter.stAeExpFunc.pfn_ae_init(pstLib->alg_lib.s32Id, &stAeParam);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AeRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                 HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 i;
    HI_U8  u8BayerFormat = 0;
    HI_S8  s8StitchMainPipe;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U16 u16BlackOffset = 0;
    HI_U8  u8RCEnable = 0;
    static HI_U32 u32UpdateCnt = 0;
    static HI_U32 u32WaitCnt = 0;
    static HI_BOOL bInTime = HI_FALSE;
    ISP_AE_INFO_S       stAeInfo    = {0};
    ISP_AE_RESULT_S     stAeResult  = {{0}};

    isp_linkage           *pstLinkage   = HI_NULL;
    isp_usr_ctx               *pstIspCtx    = HI_NULL;
    isp_lib_node          *pstLib       = HI_NULL;
    isp_stat              *pIspStatInfo = HI_NULL;
    isp_reg_cfg           *pstRegCfg    = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstLib       = &pstIspCtx->ae_lib_info.libs[pstIspCtx->ae_lib_info.active_lib];
    pstRegCfg    = (isp_reg_cfg *)pRegCfg;
    pIspStatInfo = (isp_stat *)pStatInfo;
    pstLinkage   = &pstIspCtx->linkage;

    if (pstLinkage->defect_pixel) {
        return HI_SUCCESS;
    }

    if (pstLinkage->snap_state) {
        return HI_SUCCESS;
    }
    if ((IS_ONLINE_MODE(pstIspCtx->block_attr.running_mode)\
         || IS_SIDEBYSIDE_MODE(pstIspCtx->block_attr.running_mode))\
        && (pstIspCtx->linkage.snap_pipe_mode == ISP_SNAP_PICTURE)) {
        ISP_CHECK_PIPE(pstIspCtx->linkage.preview_pipe_id);
        pstIspCtx->linkage.iso_done_frm_cnt = ISP_FrameCntGet(pstIspCtx->linkage.preview_pipe_id);
        return HI_SUCCESS;
    }

    if (pstIspCtx->linkage.stat_ready  == HI_FALSE) {
        return HI_SUCCESS;
    }

    stAeInfo.u32FrameCnt = pstIspCtx->frame_cnt;

    stAeInfo.pstFEAeStat1 = &pIspStatInfo->stFEAeStat1;
    stAeInfo.pstFEAeStat2 = &pIspStatInfo->stFEAeStat2;
    stAeInfo.pstFEAeStat3 = &pIspStatInfo->stFEAeStat3;
    stAeInfo.pstBEAeStat1 = &pIspStatInfo->stBEAeStat1;
    stAeInfo.pstBEAeStat2 = &pIspStatInfo->stBEAeStat2;
    stAeInfo.pstBEAeStat3 = &pIspStatInfo->stBEAeStat3;

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        stAeInfo.stSmartInfo.stROI[i].bEnable    = hi_ext_system_smart_enable_read(ViPipe, i);
        stAeInfo.stSmartInfo.stROI[i].u8Luma     = hi_ext_system_smart_luma_read(ViPipe, i);
        stAeInfo.stSmartInfo.stROI[i].bAvailable = hi_ext_system_smart_available_read(ViPipe, i);
    }

    if (hi_ext_system_smart_update_read(ViPipe)) {
        u32UpdateCnt++;
        hi_ext_system_smart_update_write(ViPipe, HI_FALSE);
    } else {
        u32WaitCnt++;
    }

    if (u32UpdateCnt) {
        if (u32WaitCnt < 20) {
            bInTime = HI_TRUE;
        }
        u32UpdateCnt = 0;
        u32WaitCnt   = 0;
    }
    if(u32WaitCnt >= 20) {
        bInTime = HI_FALSE;
    }

    if (!bInTime) {
        for (i = 0; i < SMART_CLASS_MAX; i++) {
            stAeInfo.stSmartInfo.stROI[i].bAvailable = HI_FALSE;
        }
    }

    if (pstLib->ae_regsiter.stAeExpFunc.pfn_ae_ctrl != HI_NULL) {
        u8BayerFormat = hi_ext_system_rggb_cfg_read(ViPipe);
        pstLib->ae_regsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->alg_lib.s32Id,
                                                     ISP_AE_BAYER_FORMAT_SET, (HI_VOID *)&u8BayerFormat);
        u16BlackOffset = hi_ext_system_black_level_query_00_read(ViPipe);
        pstLib->ae_regsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->alg_lib.s32Id,
                                                     ISP_AE_BLC_SET, (HI_VOID *)&u16BlackOffset);
        u8RCEnable = hi_ext_system_rc_en_read(ViPipe);
        pstLib->ae_regsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->alg_lib.s32Id,
                                                     ISP_AE_RC_SET, (HI_VOID *)&u8RCEnable);
    }
    if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) {
        s8StitchMainPipe = pstIspCtx->stitch_attr.stitch_bind_id[0];

        if (IS_STITCH_MAIN_PIPE(ViPipe, s8StitchMainPipe)) {
            stAeInfo.pstFEAeStat1 = &pIspStatInfo->stitch_stat.stFEAeStat1;
            stAeInfo.pstFEAeStat2 = &pIspStatInfo->stitch_stat.stFEAeStat2;
            stAeInfo.pstBEAeStat1 = &pIspStatInfo->stitch_stat.stBEAeStat1;
            stAeInfo.pstBEAeStat2 = &pIspStatInfo->stitch_stat.stBEAeStat2;
            stAeInfo.pstFEAeStiStat = &pIspStatInfo->stitch_stat.stFEAeStat3;
            stAeInfo.pstBEAeStiStat = &pIspStatInfo->stitch_stat.stBEAeStat3;

            if (pstLib->ae_regsiter.stAeExpFunc.pfn_ae_run != HI_NULL) {
                s32Ret = pstLib->ae_regsiter.stAeExpFunc.pfn_ae_run(pstLib->alg_lib.s32Id, &stAeInfo, &stAeResult, 0);
                if (s32Ret != HI_SUCCESS) {
                    ISP_ERR_TRACE("WARNING!! ISP[%d] run ae lib err!\n", ViPipe);
                }
            }

            memcpy(&pstIspCtx->ae_result, &stAeResult, sizeof(ISP_AE_RESULT_S));
        } else {
            memcpy(&stAeResult, &g_astIspCtx[s8StitchMainPipe].ae_result, sizeof(ISP_AE_RESULT_S));
        }
    } else {
        if (pstLib->ae_regsiter.stAeExpFunc.pfn_ae_run != HI_NULL) {
            s32Ret = pstLib->ae_regsiter.stAeExpFunc.pfn_ae_run(pstLib->alg_lib.s32Id, &stAeInfo, &stAeResult, 0);
            if (s32Ret != HI_SUCCESS) {
                ISP_ERR_TRACE("WARNING!! ISP[%d] run ae lib err!\n", ViPipe);
            }
        }
    }
    if (stAeResult.u32AgainSF == 0 || stAeResult.u32DgainSF == 0 || stAeResult.u32IspDgainSF == 0 || stAeResult.u32IsoSF == 0) {
        stAeResult.u32AgainSF = stAeResult.u32Again;
        stAeResult.u32DgainSF = stAeResult.u32Dgain;
        stAeResult.u32IspDgainSF = stAeResult.u32IspDgain;
        stAeResult.u32IsoSF = stAeResult.u32Iso;
    }
    hi_ext_system_sys_iso_write(ViPipe, stAeResult.u32Iso);

    AeReadExtregs(ViPipe, &stAeResult);
    AeResReadExtregs(ViPipe, pstRegCfg);
    AeRegsRangeCheck(ViPipe, &stAeResult);
    AeUpdateConfig(ViPipe, &stAeResult, pstRegCfg);
    AeUpdateLinkage(ViPipe, &stAeResult, pstLinkage);
    if (pstIspCtx->linkage.snap_pipe_mode != ISP_SNAP_PICTURE) {
        AeGetFrameInfo(ViPipe, &stAeResult);
        AeGetDCFInfo(ViPipe, &stAeResult);
    }

    return s32Ret;
}

HI_S32 ISP_AeCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    HI_S32  i, s32Ret = HI_FAILURE;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_lib_node *pstLib = HI_NULL;
    isp_reg_cfg_attr  *pstRegCfg = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);

    pstLib = &pstIspCtx->ae_lib_info.libs[pstIspCtx->ae_lib_info.active_lib];

    if (u32Cmd == ISP_PROC_WRITE) {
        if (pstLib->used) {
            if (pstLib->ae_regsiter.stAeExpFunc.pfn_ae_ctrl != HI_NULL) {
                s32Ret = pstLib->ae_regsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->alg_lib.s32Id, u32Cmd, pValue);
            }
        }
    } else {
        for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
            if (pstIspCtx->ae_lib_info.libs[i].used) {
                pstLib = &pstIspCtx->ae_lib_info.libs[i];
                if (pstLib->ae_regsiter.stAeExpFunc.pfn_ae_ctrl != HI_NULL) {
                    s32Ret = pstLib->ae_regsiter.stAeExpFunc.pfn_ae_ctrl(pstLib->alg_lib.s32Id, u32Cmd, pValue);
                }
            }
        }
    }

    if (u32Cmd == ISP_WDR_MODE_SET) {
        AeRegsDefault(ViPipe, &pstRegCfg->reg_cfg);
    }

    if (u32Cmd == ISP_CHANGE_IMAGE_MODE_SET) {
        AeRegsDefault(ViPipe, &pstRegCfg->reg_cfg);
    }

    return s32Ret;
}

HI_S32 ISP_AeExit(VI_PIPE ViPipe)
{
    HI_S32 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_lib_node *pstLib = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (pstIspCtx->ae_lib_info.libs[i].used) {
            pstLib = &pstIspCtx->ae_lib_info.libs[i];
            if (pstLib->ae_regsiter.stAeExpFunc.pfn_ae_exit != HI_NULL) {
                pstLib->ae_regsiter.stAeExpFunc.pfn_ae_exit(pstLib->alg_lib.s32Id);
            }
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_ae(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_AE;
    pstAlgs->alg_func.pfn_alg_init = ISP_AeInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_AeRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_AeCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_AeExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
