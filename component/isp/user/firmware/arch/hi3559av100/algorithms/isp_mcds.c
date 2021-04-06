/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include "isp_config.h"
#include "hi_isp_debug.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MCDS_EN          (1)
#define MCDS_FILTER_MODE (1)     // 1: filter mode; 0: discard mode

static HI_VOID McdsStaticRegInit(VI_PIPE ViPipe, ISP_MCDS_STATIC_REG_CFG_S *pstStaticRegCfg, isp_usr_ctx *pstIspCtx)
{

    static const HI_S32 Coeff_Filter_8tap_H[2][8] = {{ -16, 0, 145, 254, 145, 0, -16, 0}, {0, 0, 0, 256, 256, 0, 0, 0}};
    static const HI_S32 Coeff_Discard_8pixel_H[8] = {0, 0, 0, 512, 0, 0, 0, 0};

    static const HI_S32 Coeff_Filer_4tap_V[2][4] = {{4, 4, 6, 6}, {3, 3, 3, 3}};
    static const HI_S32 Coeff_Discard_4tap_V[4]  = {5, 6, 6, 6};

    HI_S32 s32HCoef[8];
    HI_S32 s32VCoef[2];
    HI_U32  i;

    if (MCDS_FILTER_MODE) {         // Filter Mode
        if (pstIspCtx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR) {     // SDR mode
            for (i = 0; i < 8; i++) {

                s32HCoef[i] = Coeff_Filter_8tap_H[1][i];
            }
        } else {
            // HDR mode
            for (i = 0; i < 8; i++) {
                s32HCoef[i] = Coeff_Filter_8tap_H[0][i];
            }
        }

        for (i = 0; i < 2; i++) {
            s32VCoef[i] = Coeff_Filer_4tap_V[0][i];
        }
    } else {    // discard Mode
        for (i = 0; i < 8; i++) {
            s32HCoef[i] = Coeff_Discard_8pixel_H[i];
        }

        for (i = 0; i < 2; i++) {
            s32VCoef[i] = Coeff_Discard_4tap_V[i];
        }
    }

    pstStaticRegCfg->bHcdsEn    = 1;
    pstStaticRegCfg->s32HCoef0  = s32HCoef[0];
    pstStaticRegCfg->s32HCoef1  = s32HCoef[1];
    pstStaticRegCfg->s32HCoef2  = s32HCoef[2];
    pstStaticRegCfg->s32HCoef3  = s32HCoef[3];
    pstStaticRegCfg->s32HCoef4  = s32HCoef[4];
    pstStaticRegCfg->s32HCoef5  = s32HCoef[5];
    pstStaticRegCfg->s32HCoef6  = s32HCoef[6];
    pstStaticRegCfg->s32HCoef7  = s32HCoef[7];
    pstStaticRegCfg->s32VCoef0  = s32VCoef[0];
    pstStaticRegCfg->s32VCoef1  = s32VCoef[1];
    pstStaticRegCfg->u16CoringLimit = 0;
    pstStaticRegCfg->u8MidfBldr     = 8;
    pstStaticRegCfg->bStaticResh    = HI_TRUE;
}

static HI_VOID McdsDynaRegInit(ISP_MCDS_DYNA_REG_CFG_S *pstDynaRegCfg, isp_usr_ctx *pstIspCtx)
{
    pstDynaRegCfg->bMidfEn   = 1;
    pstDynaRegCfg->bDynaResh = 1;
    if (pstIspCtx->hdr_attr.format == PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
        pstDynaRegCfg->bVcdsEn    = HI_FALSE;
    } else if (pstIspCtx->hdr_attr.format == PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
        pstDynaRegCfg->bVcdsEn    = HI_TRUE;      // 422: 0; 420: 1
    } else { // 400 is the same with 420
        pstDynaRegCfg->bVcdsEn    = HI_TRUE;
    }

}

static HI_VOID McdsRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pRegCfg)
{
    HI_U32 i;

    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < pRegCfg->cfg_num; i++) {
        pRegCfg->alg_reg_cfg[i].stMcdsRegCfg.bMCDSen = HI_TRUE;
        McdsStaticRegInit(ViPipe, &(pRegCfg->alg_reg_cfg[i].stMcdsRegCfg.stStaticRegCfg), pstIspCtx);
        McdsDynaRegInit(&(pRegCfg->alg_reg_cfg[i].stMcdsRegCfg.stDynaRegCfg), pstIspCtx);
    }

    pRegCfg->cfg_key.bit1McdsCfg = 1;

    return;
}

HI_S32 McdsProcWrite(VI_PIPE ViPipe, hi_isp_ctrl_proc_write *pstProc)
{
    return HI_SUCCESS;
}

HI_S32 ISP_McdsInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    McdsRegsInitialize(ViPipe, pstRegCfg);

    return HI_SUCCESS;
}

HI_S32 ISP_McdsRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo, HI_VOID *pRegCfg, HI_S32 s32Rsv)
{

    HI_U8  i;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    HI_BOOL bEnEdgeMarkRead;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts */
    if ((pstIspCtx->frame_cnt % 2 != 0) && (pstIspCtx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    bEnEdgeMarkRead = hi_ext_system_manual_isp_edgemark_en_read(ViPipe);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stMcdsRegCfg.bMCDSen = HI_TRUE;

        pstRegCfg->alg_reg_cfg[i].stMcdsRegCfg.stDynaRegCfg.bDynaResh = 1;

        if (pstIspCtx->hdr_attr.format == PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
            pstRegCfg->alg_reg_cfg[i].stMcdsRegCfg.stDynaRegCfg.bVcdsEn = HI_FALSE;
        } else if (pstIspCtx->hdr_attr.format == PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
            pstRegCfg->alg_reg_cfg[i].stMcdsRegCfg.stDynaRegCfg.bVcdsEn    = HI_TRUE;      // 422: 0; 420: 1
        } else { // 400 is the same with 420
            pstRegCfg->alg_reg_cfg[i].stMcdsRegCfg.stDynaRegCfg.bVcdsEn    = HI_TRUE;
        }


        if (bEnEdgeMarkRead) { // To close Median filter when edgemark is open
            pstRegCfg->alg_reg_cfg[i].stMcdsRegCfg.stDynaRegCfg.bMidfEn = 0;
        } else {
            pstRegCfg->alg_reg_cfg[i].stMcdsRegCfg.stDynaRegCfg.bMidfEn = 1;
        }
    }

    pstRegCfg->cfg_key.bit1McdsCfg = 1;

    return HI_SUCCESS;
}

HI_S32 ISP_McdsCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    switch (u32Cmd) {
        case ISP_PROC_WRITE:
            McdsProcWrite(ViPipe, (hi_isp_ctrl_proc_write *)pValue);
            break;
        case  ISP_CHANGE_IMAGE_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_McdsInit(ViPipe, &pRegCfg->reg_cfg);
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_McdsExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stMcdsRegCfg.bMCDSen = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1McdsCfg = 1;
    return HI_SUCCESS;
}

HI_S32 isp_alg_register_mcds(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_MCDS;
    pstAlgs->alg_func.pfn_alg_init = ISP_McdsInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_McdsRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_McdsCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_McdsExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
