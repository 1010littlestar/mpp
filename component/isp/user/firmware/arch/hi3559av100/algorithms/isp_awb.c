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
#include "hi_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* Convert to direct presentation (used in the ISP) */
static HI_U16 AwbComplementToDirect(HI_S16 s16Value)
{
    HI_U16 u16Result;

    if (s16Value >= 0) {
        u16Result = s16Value;
    } else {
        u16Result = -s16Value;
        u16Result |= (1 << 15);
    }

    return u16Result;
}

/* Convert to complement presentation (used in the firmware for calculations) */
static HI_S16 AwbDirectToComplement(HI_U16 u16Value)
{
    HI_S16 s16Result;

    s16Result = u16Value & (~(1 << 15));

    if (u16Value & (1 << 15)) {
        s16Result = -s16Result;
    }

    return s16Result;
}

/* Multiply the two matrixes a1[dim1 x dim2] a2[dim2 x dim3] */
static HI_VOID AwbMatrixMultiply(HI_S16 *ps16Matrix1, HI_S16 *ps16Matrix2,
                                 HI_S16 *ps16Result, HI_S32 s32Dim1, HI_S32 s32Dim2, HI_S32 s32Dim3)
{
    HI_S32 i, j, k;
    HI_S32 s32Temp;

    for (i = 0; i < s32Dim1; ++i) {
        for (j = 0; j < s32Dim3; ++j) {
            s32Temp = 0;

            for (k = 0; k < s32Dim2; ++k) {
                s32Temp += (((HI_S32)ps16Matrix1[i * s32Dim2 + k] * ps16Matrix2[k * s32Dim3 + j]));
            }

            ps16Result[i * s32Dim3 + j] = (HI_S16)((s32Temp + 128) / 256);
        }
    }

    for (i = 0; i < s32Dim1; ++i) {
        s32Temp = 0;

        for (j = 0; j < s32Dim3; ++j) {
            s32Temp += (HI_S32)ps16Result[i * s32Dim3 + j];
        }

        if (0x0 != s32Temp) {
            for (j = 0; j < s32Dim3; ++j) {
                ps16Result[i * s32Dim3 + j] = (HI_S16)(ps16Result[i * s32Dim3 + j] * 0x100 / DIV_0_TO_1(s32Temp));
            }
        }
    }

    for (i = 0; i < s32Dim1; ++i) {
        s32Temp = 0;

        for (j = 0; j < s32Dim3; ++j) {
            s32Temp += (HI_S16)ps16Result[i * s32Dim3 + j];
        }

        if (0x100 != s32Temp) {
            ps16Result[i * s32Dim3 + i] += (0x100 - s32Temp);
        }
    }

    return;
}

static HI_VOID AwbResRegsDefault(VI_PIPE ViPipe, HI_U8 i, HI_U8 u8BlockNum, ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg)
{
    HI_U16 u16Overlap;
    isp_rect stBlockRect;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    // BEDYNAMIC
    ISP_GetBlockRect(&stBlockRect, &pstIspCtx->block_attr, i);
    pstAwbRegDynCfg->u32BEWidth = stBlockRect.width;
    pstAwbRegDynCfg->u32BEHeight = stBlockRect.height;

    u16Overlap = pstIspCtx->block_attr.over_lap;

    // awb overlap configs
    if (i == 0) {
        if (u8BlockNum > 1) {
            pstAwbRegDynCfg->u32BECropPosX = 0;
            pstAwbRegDynCfg->u32BECropPosY = 0;
            pstAwbRegDynCfg->u32BECropOutWidth = pstAwbRegDynCfg->u32BEWidth - u16Overlap;
            pstAwbRegDynCfg->u32BECropOutHeight = pstAwbRegDynCfg->u32BEHeight;
        } else {
            pstAwbRegDynCfg->u32BECropPosX = 0;
            pstAwbRegDynCfg->u32BECropPosY = 0;
            pstAwbRegDynCfg->u32BECropOutHeight = stBlockRect.height;
            pstAwbRegDynCfg->u32BECropOutWidth = stBlockRect.width;
        }
    } else if (i == (u8BlockNum - 1)) {
        pstAwbRegDynCfg->u32BECropPosX = u16Overlap;
        pstAwbRegDynCfg->u32BECropPosY = 0;
        pstAwbRegDynCfg->u32BECropOutWidth = pstAwbRegDynCfg->u32BEWidth - u16Overlap;
        pstAwbRegDynCfg->u32BECropOutHeight = pstAwbRegDynCfg->u32BEHeight;
    } else {
        pstAwbRegDynCfg->u32BECropPosX = u16Overlap;
        pstAwbRegDynCfg->u32BECropPosY = 0;
        pstAwbRegDynCfg->u32BECropOutWidth = pstAwbRegDynCfg->u32BEWidth - (u16Overlap << 1);
        pstAwbRegDynCfg->u32BECropOutHeight = pstAwbRegDynCfg->u32BEHeight;
    }

    hi_ext_system_awb_crop_en_write(ViPipe, HI_EXT_SYSTEM_AWB_CROP_EN_DEFAULT);
    hi_ext_system_awb_crop_x_write(ViPipe, 0);
    hi_ext_system_awb_crop_y_write(ViPipe, 0);
    hi_ext_system_awb_crop_height_write(ViPipe, pstIspCtx->block_attr.frame_rect.height);
    hi_ext_system_awb_crop_width_write(ViPipe, pstIspCtx->block_attr.frame_rect.width);
}

static HI_VOID AwbResReadExtregs(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i, u8AWBZoneRow, u8AWBZoneCol;
    HI_U8 u8BlockNum = 0;
    HI_U8 u8CropEn;
    HI_U16 u16Overlap;
    HI_U32 u16CropX, u16CropY, u16CropHeight, u16CropWidth;
    isp_rect stBlockRect;

    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg = HI_NULL;
    u8BlockNum = ((isp_reg_cfg *)pstRegCfg)->cfg_num;
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8BlockNum = pstIspCtx->block_attr.block_num;
    u16Overlap = pstIspCtx->block_attr.over_lap;

    u8CropEn = hi_ext_system_awb_crop_en_read(ViPipe);
    u16CropX = (hi_ext_system_awb_crop_x_read(ViPipe) >> 1) << 1;
    u16CropY = (hi_ext_system_awb_crop_y_read(ViPipe) >> 1) << 1;
    u16CropHeight = (hi_ext_system_awb_crop_height_read(ViPipe) >> 1) << 1;
    u16CropWidth = (hi_ext_system_awb_crop_width_read(ViPipe) >> 1) << 1;

    u8AWBZoneRow = hi_ext_system_awb_vnum_read(ViPipe);
    u8AWBZoneCol = hi_ext_system_awb_hnum_read(ViPipe);

    u16CropHeight = CLIP3(u16CropHeight, u8AWBZoneRow * AWB_MIN_HEIGHT, pstIspCtx->block_attr.frame_rect.height);
    u16CropWidth = CLIP3(u16CropWidth, u8AWBZoneCol * AWB_MIN_WIDTH, pstIspCtx->block_attr.frame_rect.width);
    u16CropY = MIN2(u16CropY, (pstIspCtx->block_attr.frame_rect.height - u16CropHeight));
    u16CropX = MIN2(u16CropX, (pstIspCtx->block_attr.frame_rect.width - u16CropWidth));

    /* AWB BE Configs */
    for (i = 0; i < u8BlockNum; i++) {
        /* AWB Configs */
        pstAwbRegDynCfg = &pstRegCfg->alg_reg_cfg[i].stAwbRegCfg.stAwbRegDynCfg;

        /* AWB Size Configs */
        ISP_GetBlockRect(&stBlockRect, &pstIspCtx->block_attr, i);

        /* AWB Overlap Configs */
        if (i == 0) {
            if (u8BlockNum > 1) {
                pstAwbRegDynCfg->u32BECropPosX = 0;
                pstAwbRegDynCfg->u32BECropPosY = 0;
                pstAwbRegDynCfg->u32BECropOutHeight = stBlockRect.height;
                pstAwbRegDynCfg->u32BECropOutWidth = stBlockRect.width - u16Overlap;
            } else {
                if (u8CropEn) {
                    pstAwbRegDynCfg->u32BECropPosX = u16CropX;
                    pstAwbRegDynCfg->u32BECropPosY = u16CropY;
                    pstAwbRegDynCfg->u32BECropOutHeight = u16CropHeight;
                    pstAwbRegDynCfg->u32BECropOutWidth = u16CropWidth;
                } else {
                    pstAwbRegDynCfg->u32BECropPosX = 0;
                    pstAwbRegDynCfg->u32BECropPosY = 0;
                    pstAwbRegDynCfg->u32BECropOutHeight = stBlockRect.height;
                    pstAwbRegDynCfg->u32BECropOutWidth = stBlockRect.width;
                }
            }
        } else if (i == (u8BlockNum - 1)) {
            pstAwbRegDynCfg->u32BECropPosX = u16Overlap;
            pstAwbRegDynCfg->u32BECropPosY = 0;
            pstAwbRegDynCfg->u32BECropOutHeight = stBlockRect.height;
            pstAwbRegDynCfg->u32BECropOutWidth = stBlockRect.width - u16Overlap;
        } else {
            pstAwbRegDynCfg->u32BECropPosX = u16Overlap;
            pstAwbRegDynCfg->u32BECropPosY = 0;
            pstAwbRegDynCfg->u32BECropOutHeight = stBlockRect.height;
            pstAwbRegDynCfg->u32BECropOutWidth = stBlockRect.width - (u16Overlap << 1);
        }
    }
    pstRegCfg->cfg_key.bit1AwbDynCfg = 1;
    pstRegCfg->kernel_reg_cfg.cfg_key.bit1AwbCfg = 1;
}

static HI_VOID AwbImageModeSet(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    HI_U8 u8AWBZoneCol, u8AWBZoneRow;
    HI_U16 u16Height, u16Width;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    ISP_AWB_REG_USR_CFG_S *pstAwbRegUsrCfg = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u16Height = pstIspCtx->block_attr.frame_rect.height;
    u16Width = pstIspCtx->block_attr.frame_rect.width;
    u8AWBZoneCol = hi_ext_system_awb_hnum_read(ViPipe);
    u8AWBZoneRow = hi_ext_system_awb_vnum_read(ViPipe);

    if (u8AWBZoneCol * AWB_MIN_WIDTH > u16Width) {
        u8AWBZoneCol = u16Width / AWB_MIN_WIDTH;
    }

    if (u8AWBZoneRow * AWB_MIN_HEIGHT > u16Height) {
        u8AWBZoneRow = u16Height / AWB_MIN_HEIGHT;
    }

    u8AWBZoneCol = CLIP3(u8AWBZoneCol, pstIspCtx->block_attr.block_num, AWB_ZONE_ORIG_COLUMN);
    u8AWBZoneRow = CLIP3(u8AWBZoneRow, 1, AWB_ZONE_ORIG_ROW);

    hi_ext_system_awb_hnum_write(ViPipe, u8AWBZoneCol);
    hi_ext_system_awb_vnum_write(ViPipe, u8AWBZoneRow);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstAwbRegUsrCfg = &pstRegCfg->alg_reg_cfg[i].stAwbRegCfg.stAwbRegUsrCfg;

        AwbResRegsDefault(ViPipe, i, pstRegCfg->cfg_num, &pstRegCfg->alg_reg_cfg[i].stAwbRegCfg.stAwbRegDynCfg);

        if (i < u8AWBZoneCol % DIV_0_TO_1(pstRegCfg->cfg_num)) {
            pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(pstRegCfg->cfg_num) + 1;
        } else {
            pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(pstRegCfg->cfg_num);
        }

        pstAwbRegUsrCfg->u16BEZoneRow = u8AWBZoneRow;

        pstAwbRegUsrCfg->bResh = HI_TRUE;
        pstAwbRegUsrCfg->u32UpdateIndex += 1;
    }

    pstRegCfg->cfg_key.bit1AwbDynCfg = 1;
}

static HI_VOID AwbRegsDefault(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg, HI_U8 u8AWBZoneRow, HI_U8 u8AWBZoneCol, HI_U8 u8AWBZoneBin)
{
    HI_U8 i = 0;
    HI_U8 u8BlockNum = 0;
    ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg = HI_NULL;
    ISP_AWB_REG_STA_CFG_S *pstAwbRegStaCfg = HI_NULL;
    ISP_AWB_REG_USR_CFG_S *pstAwbRegUsrCfg = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    u8BlockNum = pstIspCtx->block_attr.block_num;

    pstAwbRegDynCfg = &pstRegCfg->alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg;  // dynamic
    pstAwbRegStaCfg = &pstRegCfg->alg_reg_cfg[0].stAwbRegCfg.stAwbRegStaCfg;  // static

    // FE DYNAMIC
    pstAwbRegDynCfg->au32FEWhiteBalanceGain[0] = 0x1ED;
    pstAwbRegDynCfg->au32FEWhiteBalanceGain[1] = 0x100;
    pstAwbRegDynCfg->au32FEWhiteBalanceGain[2] = 0x100;
    pstAwbRegDynCfg->au32FEWhiteBalanceGain[3] = 0x1AB;
    pstAwbRegDynCfg->u8FEWbWorkEn = HI_TRUE;

    // FE STATIC
    pstAwbRegStaCfg->bFEAwbStaCfg = 1;

    pstAwbRegStaCfg->u32FEClipValue = 0xFFFFF;

    pstRegCfg->cfg_key.bit1AwbDynCfg = 1;

    for (i = 0; i < u8BlockNum; i++) {
        pstAwbRegStaCfg = &(pstRegCfg->alg_reg_cfg[i].stAwbRegCfg.stAwbRegStaCfg);  // static
        pstAwbRegDynCfg = &(pstRegCfg->alg_reg_cfg[i].stAwbRegCfg.stAwbRegDynCfg);  // dynamic
        pstAwbRegUsrCfg = &(pstRegCfg->alg_reg_cfg[i].stAwbRegCfg.stAwbRegUsrCfg);  // user

        // BE STATIC
        pstAwbRegStaCfg->bBEAwbStaCfg = 1;
        pstAwbRegStaCfg->u8BEAwbBitmove = 0x0;
        pstAwbRegStaCfg->u8BEWbWorkEn = HI_TRUE;
        pstAwbRegStaCfg->u8BEAwbWorkEn = HI_TRUE;

        pstAwbRegStaCfg->u32BEAwbStatRaddr = 0x000;
        pstAwbRegStaCfg->u8BECcColortoneEn = 0x0;

        pstAwbRegStaCfg->u32BETopK = 0x0;
        pstAwbRegStaCfg->u32BETopB = 0xFFFFF;
        pstAwbRegStaCfg->u32BEBotK = 0x0;
        pstAwbRegStaCfg->u32BEBotB = 0x0;

        pstAwbRegStaCfg->u32BECcInDc0 = 0x0;
        pstAwbRegStaCfg->u32BECcInDc1 = 0x0;
        pstAwbRegStaCfg->u32BECcInDc2 = 0x0;
        pstAwbRegStaCfg->u32BECcOutDc0 = 0x0;
        pstAwbRegStaCfg->u32BECcOutDc1 = 0x0;
        pstAwbRegStaCfg->u32BECcOutDc2 = 0x0;
        pstAwbRegStaCfg->u32BEWbClipValue = 0xFFFFF;
        pstAwbRegStaCfg->u16BEAwbOffsetComp = HI_ISP_AWB_OFFSET_COMP_DEF;

        AwbResRegsDefault(ViPipe, i, u8BlockNum, pstAwbRegDynCfg);

        if (i < u8AWBZoneCol % DIV_0_TO_1(u8BlockNum)) {
            pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(u8BlockNum) + 1;
        } else {
            pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(u8BlockNum);
        }

        pstAwbRegDynCfg->au16BEColorMatrix[0] = 0x100;
        pstAwbRegDynCfg->au16BEColorMatrix[1] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[2] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[3] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[4] = 0x100;
        pstAwbRegDynCfg->au16BEColorMatrix[5] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[6] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[7] = 0x0;
        pstAwbRegDynCfg->au16BEColorMatrix[8] = 0x100;
        pstAwbRegDynCfg->au32BEWhiteBalanceGain[0] = 0x1ED;
        pstAwbRegDynCfg->au32BEWhiteBalanceGain[1] = 0x100;
        pstAwbRegDynCfg->au32BEWhiteBalanceGain[2] = 0x100;
        pstAwbRegDynCfg->au32BEWhiteBalanceGain[3] = 0x1AB;

        pstAwbRegDynCfg->u16BEMeteringWhiteLevelAwb = 0xffff;
        pstAwbRegDynCfg->u16BEMeteringBlackLevelAwb = 0x0;
        pstAwbRegDynCfg->u16BEMeteringCrRefMaxAwb = 0x180;
        pstAwbRegDynCfg->u16BEMeteringCrRefMinAwb = 0x40;
        pstAwbRegDynCfg->u16BEMeteringCbRefMaxAwb = 0x180;
        pstAwbRegDynCfg->u16BEMeteringCbRefMinAwb = 0x40;

        pstAwbRegDynCfg->u8BEWbWorkEn = HI_TRUE;
        pstAwbRegDynCfg->u8BECcEn = HI_TRUE;

        pstAwbRegDynCfg->u16BECcColortoneEn = HI_ISP_CCM_COLORTONE_EN_DEFAULT;
        pstAwbRegDynCfg->u16BECcRGain = HI_ISP_CCM_COLORTONE_RGAIN_DEFAULT;
        pstAwbRegDynCfg->u16BECcGGain = HI_ISP_CCM_COLORTONE_GGAIN_DEFAULT;
        pstAwbRegDynCfg->u16BECcBGain = HI_ISP_CCM_COLORTONE_BGAIN_DEFAULT;

        pstAwbRegUsrCfg->u16BEZoneRow = u8AWBZoneRow;
        pstAwbRegUsrCfg->u16BEZoneBin = u8AWBZoneBin;
        pstAwbRegUsrCfg->u16BEMeteringBinHist0 = 0xffff;
        pstAwbRegUsrCfg->u16BEMeteringBinHist1 = 0xffff;
        pstAwbRegUsrCfg->u16BEMeteringBinHist2 = 0xffff;
        pstAwbRegUsrCfg->u16BEMeteringBinHist3 = 0xffff;
        pstAwbRegUsrCfg->enBEAWBSwitch = ISP_AWB_AFTER_DG;
        pstAwbRegUsrCfg->bResh = HI_TRUE;
        pstAwbRegUsrCfg->u32UpdateIndex = 1;

        pstRegCfg->kernel_reg_cfg.cfg_key.bit1AwbCfg = 1;
        pstRegCfg->kernel_reg_cfg.alg_kernel_cfg[i].stAWBZoneCfg.u32Row = u8AWBZoneRow;
        pstRegCfg->kernel_reg_cfg.alg_kernel_cfg[i].stAWBZoneCfg.u32Colum = u8AWBZoneCol;
        pstRegCfg->kernel_reg_cfg.alg_kernel_cfg[i].stAWBZoneCfg.u32ZoneBin = u8AWBZoneBin;

        hi_ext_system_cc_enable_write(ViPipe, pstAwbRegDynCfg->u8BECcEn);
        hi_ext_system_awb_gain_enable_write(ViPipe, pstAwbRegDynCfg->u8BEWbWorkEn);
        hi_ext_system_awb_white_level_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringWhiteLevelAwb);
        hi_ext_system_awb_black_level_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringBlackLevelAwb);
        hi_ext_system_awb_cr_ref_max_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringCrRefMaxAwb);
        hi_ext_system_awb_cr_ref_min_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringCrRefMinAwb);
        hi_ext_system_awb_cb_ref_max_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringCbRefMaxAwb);
        hi_ext_system_awb_cb_ref_min_write(ViPipe, pstAwbRegDynCfg->u16BEMeteringCbRefMinAwb);
        hi_ext_system_cc_colortone_rgain_write(ViPipe, pstAwbRegDynCfg->u16BECcRGain);
        hi_ext_system_cc_colortone_ggain_write(ViPipe, pstAwbRegDynCfg->u16BECcGGain);
        hi_ext_system_cc_colortone_bgain_write(ViPipe, pstAwbRegDynCfg->u16BECcBGain);

        // User
        hi_ext_system_awb_sta_enable_write(ViPipe, pstAwbRegStaCfg->u8BEAwbWorkEn);
        hi_ext_system_wb_statistics_mpi_update_en_write(ViPipe, pstAwbRegUsrCfg->bResh);
        hi_ext_system_awb_switch_write(ViPipe, pstAwbRegUsrCfg->enBEAWBSwitch);
        hi_ext_system_awb_hnum_write(ViPipe, u8AWBZoneCol);  // the col num of the whole picture
        hi_ext_system_awb_vnum_write(ViPipe, pstAwbRegUsrCfg->u16BEZoneRow);
        hi_ext_system_awb_zone_bin_write(ViPipe, pstAwbRegUsrCfg->u16BEZoneBin);
        hi_ext_system_awb_hist_bin_thresh0_write(ViPipe, pstAwbRegUsrCfg->u16BEMeteringBinHist0);
        hi_ext_system_awb_hist_bin_thresh1_write(ViPipe, pstAwbRegUsrCfg->u16BEMeteringBinHist1);
        hi_ext_system_awb_hist_bin_thresh2_write(ViPipe, pstAwbRegUsrCfg->u16BEMeteringBinHist2);
        hi_ext_system_awb_hist_bin_thresh3_write(ViPipe, pstAwbRegUsrCfg->u16BEMeteringBinHist3);
    }

    return;
}

HI_S32 AwbCfgReg(VI_PIPE ViPipe, ISP_AWB_RESULT_S *pstAwbResult, isp_reg_cfg *pstRegCfg, HI_U8 u8AwbGainSwitch)
{
    HI_BOOL bUsrResh;
    HI_S32 i, k;
    HI_U32 au32WbGain[4];
    HI_U8 u8BlkNum;
    HI_U8 u8AWBZoneCol = AWB_ZONE_ORIG_COLUMN;
    ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg = HI_NULL;
    ISP_AWB_REG_USR_CFG_S *pstAwbRegUsrCfg = HI_NULL;
    ISP_AWB_REG_STA_CFG_S *pstAwbRegStaCfg = HI_NULL;
    u8BlkNum = ((isp_reg_cfg *)pstRegCfg)->cfg_num;

    bUsrResh = hi_ext_system_wb_statistics_mpi_update_en_read(ViPipe);
    hi_ext_system_wb_statistics_mpi_update_en_write(ViPipe, HI_FALSE);

    if (pstAwbResult->stRawStatAttr.bStatCfgUpdate) { // CbCr
        hi_ext_system_awb_cr_ref_max_write(ViPipe, MIN2(pstAwbResult->stRawStatAttr.u16MeteringCrRefMaxAwb, 0xFFF));
        hi_ext_system_awb_cr_ref_min_write(ViPipe, MIN2(pstAwbResult->stRawStatAttr.u16MeteringCrRefMinAwb, 0xFFF));
        hi_ext_system_awb_cb_ref_max_write(ViPipe, MIN2(pstAwbResult->stRawStatAttr.u16MeteringCbRefMaxAwb, 0xFFF));
        hi_ext_system_awb_cb_ref_min_write(ViPipe, MIN2(pstAwbResult->stRawStatAttr.u16MeteringCbRefMinAwb, 0xFFF));
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        au32WbGain[i] = pstAwbResult->au32WhiteBalanceGain[i];
        au32WbGain[i] = (au32WbGain[i] + 128) >> 8;
        au32WbGain[i] = (au32WbGain[i] > 0xFFF) ? 0xFFF : au32WbGain[i];
    }

    for (k = 0; k < u8BlkNum; k++) {
        pstAwbRegDynCfg = &(pstRegCfg->alg_reg_cfg[k].stAwbRegCfg.stAwbRegDynCfg);  // dynamic
        pstAwbRegUsrCfg = &(pstRegCfg->alg_reg_cfg[k].stAwbRegCfg.stAwbRegUsrCfg);  // user
        pstAwbRegStaCfg = &(pstRegCfg->alg_reg_cfg[k].stAwbRegCfg.stAwbRegStaCfg);  // sta

        pstAwbRegDynCfg->u16BEMeteringCrRefMaxAwb = hi_ext_system_awb_cr_ref_max_read(ViPipe);
        pstAwbRegDynCfg->u16BEMeteringCrRefMinAwb = hi_ext_system_awb_cr_ref_min_read(ViPipe);
        pstAwbRegDynCfg->u16BEMeteringCbRefMaxAwb = hi_ext_system_awb_cb_ref_max_read(ViPipe);
        pstAwbRegDynCfg->u16BEMeteringCbRefMinAwb = hi_ext_system_awb_cb_ref_min_read(ViPipe);
        pstAwbRegDynCfg->u16BEMeteringWhiteLevelAwb = hi_ext_system_awb_white_level_read(ViPipe);
        pstAwbRegDynCfg->u16BEMeteringBlackLevelAwb = hi_ext_system_awb_black_level_read(ViPipe);

        for (i = 0; i < 9; i++) {
            pstAwbRegDynCfg->au16BEColorMatrix[i] = pstAwbResult->au16ColorMatrix[i];
        }

        if (pstAwbRegStaCfg->bSensorGetWBGainEn == HI_TRUE) {
            isp_sensor_get_awb_gain(ViPipe, au32WbGain);
        }
		
        if (pstAwbRegStaCfg->enAWBGainSwitch == ISP_AWB_GAIN_IN_SENSOR) {
            for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
                pstAwbRegDynCfg->au32SensorWBGain[i] = au32WbGain[i];
                pstAwbRegDynCfg->au32WDRWBGain[i] = 0x100;
                pstAwbRegDynCfg->au32BEWhiteBalanceGain[i] = 0x100;
            }
        } else {
            if (u8AwbGainSwitch == WDR_WBGAIN_IN_DG1) {
                for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
                    pstAwbRegDynCfg->au32WDRWBGain[i] = au32WbGain[i];
                    pstAwbRegDynCfg->au32BEWhiteBalanceGain[i] = 0x100;
                    pstAwbRegDynCfg->au32SensorWBGain[i] = 0x100;
                }
            } else {
                for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
                    pstAwbRegDynCfg->au32WDRWBGain[i] = 0x100;
                    pstAwbRegDynCfg->au32BEWhiteBalanceGain[i] = au32WbGain[i];
                    pstAwbRegDynCfg->au32SensorWBGain[i] = 0x100;
                }
            }
        }

        u8AWBZoneCol = hi_ext_system_awb_hnum_read(ViPipe);

        if (bUsrResh == HI_TRUE) {
            if (k < u8AWBZoneCol % DIV_0_TO_1(u8BlkNum)) {
                pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(u8BlkNum) + 1;
            } else {
                pstAwbRegUsrCfg->u16BEZoneCol = u8AWBZoneCol / DIV_0_TO_1(u8BlkNum);
            }

            pstAwbRegUsrCfg->u32UpdateIndex += 1;

            pstAwbRegUsrCfg->enBEAWBSwitch = hi_ext_system_awb_switch_read(ViPipe);
            pstAwbRegUsrCfg->u16BEZoneRow = hi_ext_system_awb_vnum_read(ViPipe);
            pstAwbRegUsrCfg->u16BEZoneBin = hi_ext_system_awb_zone_bin_read(ViPipe);

            pstAwbRegUsrCfg->u16BEMeteringBinHist0 = hi_ext_system_awb_hist_bin_thresh0_read(ViPipe);
            pstAwbRegUsrCfg->u16BEMeteringBinHist1 = hi_ext_system_awb_hist_bin_thresh1_read(ViPipe);
            pstAwbRegUsrCfg->u16BEMeteringBinHist2 = hi_ext_system_awb_hist_bin_thresh2_read(ViPipe);
            pstAwbRegUsrCfg->u16BEMeteringBinHist3 = hi_ext_system_awb_hist_bin_thresh3_read(ViPipe);
            pstAwbRegUsrCfg->bResh = HI_TRUE;
        }

        pstAwbRegDynCfg->u8BECcEn = hi_ext_system_cc_enable_read(ViPipe);
        pstAwbRegDynCfg->u16BECcBGain = hi_ext_system_cc_colortone_bgain_read(ViPipe);
        pstAwbRegDynCfg->u16BECcGGain = hi_ext_system_cc_colortone_ggain_read(ViPipe);
        pstAwbRegDynCfg->u16BECcRGain = hi_ext_system_cc_colortone_rgain_read(ViPipe);

        pstAwbRegDynCfg->u8BEWbWorkEn = hi_ext_system_awb_gain_enable_read(ViPipe);
        pstAwbRegDynCfg->u8FEWbWorkEn = hi_ext_system_awb_gain_enable_read(ViPipe);
        pstAwbRegStaCfg->u8BEAwbWorkEn = hi_ext_system_awb_sta_enable_read(ViPipe);

        pstRegCfg->kernel_reg_cfg.alg_kernel_cfg[k].stAWBZoneCfg.u32Row = pstAwbRegUsrCfg->u16BEZoneRow;
        pstRegCfg->kernel_reg_cfg.alg_kernel_cfg[k].stAWBZoneCfg.u32Colum = u8AWBZoneCol;
        pstRegCfg->kernel_reg_cfg.alg_kernel_cfg[k].stAWBZoneCfg.u32ZoneBin = pstAwbRegUsrCfg->u16BEZoneBin;
    }

    // FE
    for (i = 0; i < 4; i++) {
        pstRegCfg->alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au32FEWhiteBalanceGain[i] = au32WbGain[i];
    }

    if (pstRegCfg->alg_reg_cfg[0].stAwbRegCfg.stAwbRegStaCfg.enAWBGainSwitch == ISP_AWB_GAIN_IN_SENSOR) {
        for (i = 0; i < 4; i++) {
            pstRegCfg->alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au32FEWhiteBalanceGain[i] = 0x100;
        }
    }
    pstRegCfg->cfg_key.bit1AwbDynCfg = 1;
    pstRegCfg->kernel_reg_cfg.cfg_key.bit1AwbCfg = 1;

    return HI_SUCCESS;
}

HI_VOID AwbCfgInitReg(VI_PIPE ViPipe, ISP_AWB_RESULT_S *pstAwbResult, isp_reg_cfg *pstRegCfg, hi_isp_awb_gain_switch awb_gain_switch)
{
    HI_S32 i, k;
    HI_U32 au32WbGain[4];
    HI_U8 u8BlkNum;
    ISP_AWB_REG_DYN_CFG_S  *pstAwbRegDynCfg = HI_NULL;
    ISP_AWB_REG_STA_CFG_S  *pstAwbRegStaCfg = HI_NULL;
    u8BlkNum = ((isp_reg_cfg *)pstRegCfg)->cfg_num;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        au32WbGain[i] = pstAwbResult->au32WhiteBalanceGain[i];
        au32WbGain[i] = (au32WbGain[i] + 128) >> 8;
        au32WbGain[i] = (au32WbGain[i] > 0xFFF) ? 0xFFF : au32WbGain[i];
        pstRegCfg->alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au32FEWhiteBalanceGain[i] = (HI_U16)au32WbGain[i];
    }

    for (k = 0; k < u8BlkNum; k++) {
        HI_S32 s32Ret = HI_FAILURE; 
        pstAwbRegDynCfg = &(pstRegCfg->alg_reg_cfg[k].stAwbRegCfg.stAwbRegDynCfg); // dynamic
        pstAwbRegStaCfg = &(pstRegCfg->alg_reg_cfg[k].stAwbRegCfg.stAwbRegStaCfg); // static

        for (i = 0; i < 9; i++) {
            pstAwbRegDynCfg->au16BEColorMatrix[i] = pstAwbResult->au16ColorMatrix[i];
        }

        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstAwbRegDynCfg->au32BEWhiteBalanceGain[i] = (HI_U16)au32WbGain[i];
        }

        s32Ret = isp_sensor_get_awb_gain(ViPipe, au32WbGain);
        if (s32Ret == HI_SUCCESS) {
            pstAwbRegStaCfg->bSensorGetWBGainEn = HI_TRUE;
        } else {
            pstAwbRegStaCfg->bSensorGetWBGainEn = HI_FALSE;
        }

        if ((awb_gain_switch == ISP_AWB_GAIN_IN_SENSOR) && (pstAwbRegStaCfg->bSensorGetWBGainEn == HI_TRUE)) {
            pstAwbRegStaCfg->enAWBGainSwitch = ISP_AWB_GAIN_IN_SENSOR;
        } else {
            pstAwbRegStaCfg->enAWBGainSwitch = ISP_AWB_GAIN_IN_ISP;
        }

        if (pstAwbRegStaCfg->enAWBGainSwitch == ISP_AWB_GAIN_IN_SENSOR) {
            for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
                pstAwbRegDynCfg->au32SensorWBGain[i] = au32WbGain[i];
                pstAwbRegDynCfg->au32BEWhiteBalanceGain[i] = 0x100;
                pstAwbRegDynCfg->au32WDRWBGain[i] = 0x100;
            }
        } else {
            for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
                pstAwbRegDynCfg->au32BEWhiteBalanceGain[i] = au32WbGain[i];
                pstAwbRegDynCfg->au32SensorWBGain[i] = 0x100;
                pstAwbRegDynCfg->au32WDRWBGain[i] = 0x100;
            }
        }
    }

    pstRegCfg->cfg_key.bit1AwbDynCfg = 1;
}

HI_S32 ISP_AwbCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue);
HI_S32 ISP_AwbInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_U8 u8ActualZoneCol, u8ActualZoneRow;
    HI_U16 u16Height, u16Width;
    HI_S32 i;
    ISP_AWB_PARAM_S stAwbParam;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_lib_node *pstLib = HI_NULL;
    ISP_AWB_RESULT_S stAwbInitResult = {{0}};
    hi_isp_cmos_default *sns_dft  = HI_NULL;
    hi_isp_awb_gain_switch awb_gain_switch = ISP_AWB_GAIN_IN_ISP;

    isp_sensor_get_default(ViPipe, &sns_dft);

    if (sns_dft->key.bit1_awb == 1) {
        awb_gain_switch = sns_dft->awb_attr.awb_gain_switch;
    }

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u16Height = pstIspCtx->block_attr.frame_rect.height;
    u16Width = pstIspCtx->block_attr.frame_rect.width;

    if (u16Width < AWB_ZONE_ORIG_COLUMN * AWB_MIN_WIDTH) {
        u8ActualZoneCol = u16Width / AWB_MIN_WIDTH;
    } else {
        u8ActualZoneCol = AWB_ZONE_ORIG_COLUMN;
    }

    if (u16Height < AWB_ZONE_ORIG_ROW * AWB_MIN_HEIGHT) {
        u8ActualZoneRow = u16Height / AWB_MIN_HEIGHT;
    } else {
        u8ActualZoneRow = AWB_ZONE_ORIG_ROW;
    }

    stAwbParam.SensorId = pstIspCtx->bind_attr.sensor_id;
    stAwbParam.u8WDRMode = pstIspCtx->sns_wdr_mode;
    stAwbParam.stStitchAttr.bMainPipe       = pstIspCtx->stitch_attr.main_pipe;
    stAwbParam.stStitchAttr.bStitchEnable   = pstIspCtx->stitch_attr.stitch_enable;
    stAwbParam.stStitchAttr.u8StitchPipeNum = pstIspCtx->stitch_attr.stitch_pipe_num;
    memcpy(stAwbParam.stStitchAttr.as8StitchBindId, pstIspCtx->stitch_attr.stitch_bind_id, sizeof(hi_s8) * VI_MAX_PIPE_NUM);

    if (stAwbParam.stStitchAttr.bStitchEnable == 1) {
        HI_U32 u32BlockWidth = u8ActualZoneCol;

        AwbRegsDefault(ViPipe, (isp_reg_cfg *)pRegCfg, u8ActualZoneRow, u32BlockWidth, 1);
        stAwbParam.u8AWBZoneRow = (HI_U8)(u8ActualZoneRow);
        stAwbParam.u8AWBZoneCol = MIN2(255, (u32BlockWidth * stAwbParam.stStitchAttr.u8StitchPipeNum));
        stAwbParam.u8AWBZoneBin = 1;
    } else {
        AwbRegsDefault(ViPipe, (isp_reg_cfg *)pRegCfg, u8ActualZoneRow, u8ActualZoneCol, 1);

        stAwbParam.u8AWBZoneRow = u8ActualZoneRow;
        stAwbParam.u8AWBZoneCol = u8ActualZoneCol;
        stAwbParam.u8AWBZoneBin = 1;
    }

    stAwbParam.u16AWBWidth = pstIspCtx->block_attr.frame_rect.width;
    stAwbParam.u16AWBHeight = pstIspCtx->block_attr.frame_rect.height;

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (pstIspCtx->awb_lib_info.libs[i].used) {
            pstLib = &pstIspCtx->awb_lib_info.libs[i];

            if (pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_init != HI_NULL) {
                pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_init(pstLib->alg_lib.s32Id, &stAwbParam, &stAwbInitResult);
            }
        }
    }

    AwbCfgInitReg(ViPipe, &stAwbInitResult,  (isp_reg_cfg *)pRegCfg, awb_gain_switch);
    return HI_SUCCESS;
}

HI_S32 ISP_AwbRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                  HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 i, s32Ret = HI_FAILURE;
    ISP_AWB_PARAM_S stAwbParam;
    ISP_AWB_INFO_S stAwbInfo = { 0 };
    ISP_AWB_RESULT_S stAwbResult = { { 0 } };
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_lib_node *pstLib = HI_NULL;
    HI_BOOL bLoadCCM;
    HI_U32 u32DiffGain;
    HI_S8 s8StitchMainPipe;
    AWB_CCM_CONFIG_S stCCMConf, stCCMConfDef;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstLib = &pstIspCtx->awb_lib_info.libs[pstIspCtx->awb_lib_info.active_lib];

    if (pstIspCtx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    bLoadCCM = pstIspCtx->linkage.load_ccm;

    if (pstIspCtx->linkage.snap_state == HI_TRUE) {
        if (bLoadCCM == HI_TRUE) {
            return HI_SUCCESS;
        } else {
            if (pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl != HI_NULL) {
                pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->alg_lib.s32Id,
                                                                AWB_CCM_CONFIG_GET, (HI_VOID *)&stCCMConfDef);

                stCCMConf.bAWBBypassEn = HI_TRUE;
                stCCMConf.bManualTempEn = HI_TRUE;
                stCCMConf.u32ManualTempValue = pstIspCtx->linkage.color_temp;
                stCCMConf.u16CCMSpeed = 0xfff;

                stCCMConf.bManualSatEn = stCCMConfDef.bManualSatEn;
                stCCMConf.u32ManualSatValue = stCCMConfDef.u32ManualSatValue;
                pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->alg_lib.s32Id,
                                                                AWB_CCM_CONFIG_SET, (HI_VOID *)&stCCMConf);
            }
        }
    } else {
        if (pstIspCtx->linkage.stat_ready  == HI_FALSE) {
            return HI_SUCCESS;
        }
    }

    if ((IS_ONLINE_MODE(pstIspCtx->block_attr.running_mode)\
         || IS_SIDEBYSIDE_MODE(pstIspCtx->block_attr.running_mode))\
        && (pstIspCtx->linkage.snap_pipe_mode == ISP_SNAP_PICTURE)) {
        return HI_SUCCESS;
    }
    AwbResReadExtregs(ViPipe, (isp_reg_cfg *)pRegCfg);
    /* get statistics */
    stAwbInfo.u32FrameCnt = pstIspCtx->frame_cnt;

    stAwbInfo.u8AwbGainSwitch = hi_ext_system_wdr_wbgain_position_read(ViPipe);
    stAwbInfo.u8AwbStatSwitch = hi_ext_system_awb_switch_read(ViPipe);
    stAwbInfo.bWbGainInSensor = (((isp_reg_cfg *)pRegCfg)->alg_reg_cfg[0].stAwbRegCfg.stAwbRegStaCfg.enAWBGainSwitch == ISP_AWB_GAIN_IN_SENSOR);
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        stAwbInfo.au32WDRWBGain[i] = ((isp_reg_cfg *)pRegCfg)->alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au32WDRWBGain[i];
    }
    stAwbParam.u8AWBZoneRow = hi_ext_system_awb_vnum_read(ViPipe);
    stAwbParam.u8AWBZoneCol = hi_ext_system_awb_hnum_read(ViPipe);
    if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) {
        stAwbParam.u8AWBZoneCol = MIN2(255, (stAwbParam.u8AWBZoneCol * pstIspCtx->stitch_attr.stitch_pipe_num));
    }

    /* linkage with the iso of ae */
    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (pstIspCtx->awb_lib_info.libs[i].used) {
            pstLib = &pstIspCtx->awb_lib_info.libs[i];

            if (pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl != HI_NULL) {
                pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->alg_lib.s32Id,
                                                                ISP_AWB_ISO_SET, (HI_VOID *)&pstIspCtx->linkage.iso);
                pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->alg_lib.s32Id,
                                                                ISP_AWB_INTTIME_SET, (HI_VOID *)&pstIspCtx->linkage.int_time);
                pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->alg_lib.s32Id,
                                                                ISP_AWB_SNAP_MODE_SET, (HI_VOID *)&pstIspCtx->linkage.snap_pipe_mode);
                pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->alg_lib.s32Id,
                                                                ISP_AWB_ZONE_ROW_SET, (HI_VOID *)&stAwbParam.u8AWBZoneRow);
                pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->alg_lib.s32Id,
                                                                ISP_AWB_ZONE_COL_SET, (HI_VOID *)&stAwbParam.u8AWBZoneCol);
            }
        }
    }

    pstLib = &pstIspCtx->awb_lib_info.libs[pstIspCtx->awb_lib_info.active_lib];
    stAwbInfo.pstAwbStat1 = &((isp_stat *)pStatInfo)->stAwbStat1;
    stAwbInfo.stAwbStat2.pau16ZoneAvgR = (((isp_stat *)pStatInfo)->stAwbStat2.au16MeteringMemArrayAvgR);
    stAwbInfo.stAwbStat2.pau16ZoneAvgG = (((isp_stat *)pStatInfo)->stAwbStat2.au16MeteringMemArrayAvgG);
    stAwbInfo.stAwbStat2.pau16ZoneAvgB = (((isp_stat *)pStatInfo)->stAwbStat2.au16MeteringMemArrayAvgB);
    stAwbInfo.stAwbStat2.pau16ZoneCount = (((isp_stat *)pStatInfo)->stAwbStat2.au16MeteringMemArrayCountAll);

    if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) {
        s8StitchMainPipe = pstIspCtx->stitch_attr.stitch_bind_id[0];

        if (IS_STITCH_MAIN_PIPE(ViPipe, s8StitchMainPipe)) {
            stAwbInfo.stAwbStat2.pau16ZoneAvgR = (((isp_stat *)pStatInfo)->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgR);
            stAwbInfo.stAwbStat2.pau16ZoneAvgG = (((isp_stat *)pStatInfo)->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgG);
            stAwbInfo.stAwbStat2.pau16ZoneAvgB = (((isp_stat *)pStatInfo)->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgB);
            stAwbInfo.stAwbStat2.pau16ZoneCount = (((isp_stat *)pStatInfo)->stitch_stat.stAwbStat2.au16MeteringMemArrayCountAll);

            if (pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_run != HI_NULL) {
                s32Ret = pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_run(pstLib->alg_lib.s32Id, &stAwbInfo, &stAwbResult, 0);

                if (s32Ret != HI_SUCCESS) {
                    ISP_ERR_TRACE("WARNING!! ISP[%d] run awb lib err!\n", ViPipe);
                }
            }

            memcpy(&pstIspCtx->awb_result, &stAwbResult, sizeof(ISP_AWB_RESULT_S));
        } else {
            memcpy(&stAwbResult, &g_astIspCtx[s8StitchMainPipe].awb_result, sizeof(ISP_AWB_RESULT_S));
        }
    } else {
        if (pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_run != HI_NULL) {
            s32Ret = pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_run(pstLib->alg_lib.s32Id, &stAwbInfo, &stAwbResult, 0);

            if (s32Ret != HI_SUCCESS) {
                ISP_ERR_TRACE("WARNING!! ISP[%d] run awb lib err!\n", ViPipe);
            }
        }
    }

    pstIspCtx->linkage.color_temp = stAwbResult.u32ColorTemp;
    pstIspCtx->linkage.saturation = stAwbResult.u8Saturation;
    pstIspCtx->linkage.white_balance_gain[0] = stAwbResult.au32WhiteBalanceGain[0];
    pstIspCtx->linkage.white_balance_gain[1] = stAwbResult.au32WhiteBalanceGain[1];
    pstIspCtx->linkage.white_balance_gain[2] = stAwbResult.au32WhiteBalanceGain[2];
    pstIspCtx->linkage.white_balance_gain[3] = stAwbResult.au32WhiteBalanceGain[3];

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        pstIspCtx->linkage.ccm[i] = stAwbResult.au16ColorMatrix[i];
    }

    {
        pstIspCtx->attach_info_ctrl.attach_info->isp_hdr.color_temp = pstIspCtx->linkage.color_temp;
        memcpy(pstIspCtx->attach_info_ctrl.attach_info->isp_hdr.ccm, pstIspCtx->linkage.ccm, CCM_MATRIX_SIZE * sizeof(HI_U16));
        pstIspCtx->attach_info_ctrl.attach_info->isp_hdr.saturation = pstIspCtx->linkage.saturation;
        pstIspCtx->attach_info_ctrl.attach_info->iso = pstIspCtx->linkage.iso;
        pstIspCtx->attach_info_ctrl.attach_info->sns_wdr_mode = pstIspCtx->sns_wdr_mode;
    }

    if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) {
        HI_S16 as16CalcCCM[CCM_MATRIX_SIZE] = { 0 };
        HI_S16 as16DiffCCM[CCM_MATRIX_SIZE] = { 0 };
        HI_S16 as16ResuCCM[CCM_MATRIX_SIZE] = { 0 };

        /* TODO: Multi-Pipe different config WBgain/CCM */
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            u32DiffGain = hi_ext_system_isp_pipe_diff_gain_read(ViPipe, i);
            stAwbResult.au32WhiteBalanceGain[i] = (stAwbResult.au32WhiteBalanceGain[i] * u32DiffGain) >> 8;
        }

        for (i = 0; i < CCM_MATRIX_SIZE; i++) {
            as16CalcCCM[i] = AwbDirectToComplement(stAwbResult.au16ColorMatrix[i]);
            as16DiffCCM[i] = hi_ext_system_isp_pipe_diff_ccm_read(ViPipe, i);
            as16DiffCCM[i] = AwbDirectToComplement(as16DiffCCM[i]);
        }

        AwbMatrixMultiply(as16CalcCCM, as16DiffCCM, as16ResuCCM, 3, 3, 3);

        for (i = 0; i < CCM_MATRIX_SIZE; i++) {
            stAwbResult.au16ColorMatrix[i] = AwbComplementToDirect(as16ResuCCM[i]);
        }
    }

    AwbCfgReg(ViPipe, &stAwbResult, (isp_reg_cfg *)pRegCfg, stAwbInfo.u8AwbGainSwitch);

    if (pstIspCtx->linkage.snap_state == HI_TRUE) {
        if (pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl != HI_NULL) {
            pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->alg_lib.s32Id,
                                                            AWB_CCM_CONFIG_SET, (HI_VOID *)&stCCMConfDef);
        }
    }

    return s32Ret;
}

HI_S32 ISP_AwbCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    HI_S32 i, s32Ret = HI_FAILURE;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_lib_node *pstLib = HI_NULL;
    isp_reg_cfg_attr *pstRegCfg = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);

    pstLib = &pstIspCtx->awb_lib_info.libs[pstIspCtx->awb_lib_info.active_lib];

    if (u32Cmd == ISP_PROC_WRITE) {
        if (pstLib->used) {
            if (pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl != HI_NULL) {
                s32Ret = pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->alg_lib.s32Id, u32Cmd, pValue);
            }
        }
    } else {
        for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
            if (pstIspCtx->awb_lib_info.libs[i].used) {
                pstLib = &pstIspCtx->awb_lib_info.libs[i];
                if (pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl != HI_NULL) {
                    s32Ret = pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->alg_lib.s32Id, u32Cmd, pValue);
                }
            }
        }
    }

    if (u32Cmd == ISP_CHANGE_IMAGE_MODE_SET) {
        AwbImageModeSet(ViPipe, &pstRegCfg->reg_cfg);
    }

    return s32Ret;
}

HI_S32 ISP_AwbExit(VI_PIPE ViPipe)
{
    HI_S32 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_lib_node *pstLib = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < MAX_REGISTER_ALG_LIB_NUM; i++) {
        if (pstIspCtx->awb_lib_info.libs[i].used) {
            pstLib = &pstIspCtx->awb_lib_info.libs[i];

            if (pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_exit != HI_NULL) {
                pstLib->awb_regsiter.stAwbExpFunc.pfn_awb_exit(pstLib->alg_lib.s32Id);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_awb(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_AWB;
    pstAlgs->alg_func.pfn_alg_init = ISP_AwbInit;
    pstAlgs->alg_func.pfn_alg_run = ISP_AwbRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_AwbCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_AwbExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
