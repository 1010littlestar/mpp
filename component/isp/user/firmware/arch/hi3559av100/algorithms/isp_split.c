/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */
/***************************************************************************
ModeIn:       0=linear; 1=2~3mux;2=16LOG;3=sensor-build-in
ModeOut:     0=1 channel;1=2 channel;2=3 channel;3=4 chnnel
Inputwidthselect:  0=12bit;  1=14bit;  2=16bit; 3=20bit
****************************************************************************/
static HI_U8 SplitGetBitInWidth(HI_U8 u8InputWidthSel)
{
    switch (u8InputWidthSel) {
        case 0:
            return 12;
        case 1:
            return 14;
        case 2:
            return 16;
        case 3:
            return 20;
        default :
            return 12;
    }
}

static HI_VOID SplitStaticRegsInitialize(VI_PIPE ViPipe, ISP_SPLIT_STATIC_CFG_S *pstStaticRegCfg)
{
    HI_U32 _i, _v;
    HI_U32 X0, Y0, X1, Y1, X2, Y2, X3, Y3, X_max, Y_max;
    hi_isp_cmos_default     *sns_dft   = HI_NULL;
    const hi_isp_cmos_split *sns_split = HI_NULL;

    isp_sensor_get_default(ViPipe, &sns_dft);

    if (sns_dft->key.bit1_split) {
        sns_split = sns_dft->split;

        ISP_CHECK_POINTER_VOID(sns_split);

        pstStaticRegCfg->u8BitDepthIn  = SplitGetBitInWidth(sns_split->input_width_sel);
        pstStaticRegCfg->u8BitDepthOut = sns_split->bit_depth_out;
        pstStaticRegCfg->u8ModeIn      = sns_split->mode_in;
        pstStaticRegCfg->u8ModeOut     = sns_split->mode_out;

        X0       = sns_split->split_point[0].x;
        Y0       = sns_split->split_point[0].y;
        X1       = sns_split->split_point[1].x;
        Y1       = sns_split->split_point[1].y;
        X2       = sns_split->split_point[2].x;
        Y2       = sns_split->split_point[2].y;
        X3       = sns_split->split_point[3].x;
        Y3       = sns_split->split_point[3].y;
        X_max    = sns_split->split_point[4].x;
        Y_max    = sns_split->split_point[4].y;

        for (_i = 0; _i < X0; _i++) {
            _v = ((_i * Y0) / DIV_0_TO_1(X0));
            pstStaticRegCfg->au16WdrSplitLut[_i] = _v;
        }

        for (; _i < X1; _i++) {
            _v = (((_i - X0) * (Y1 - Y0)) / DIV_0_TO_1(X1 - X0) + Y0);
            pstStaticRegCfg->au16WdrSplitLut[_i] = _v;
        }

        for (; _i < X2; _i++) {
            _v = (((_i - X1) * (Y2 - Y1)) / DIV_0_TO_1(X2 - X1) + Y1);
            pstStaticRegCfg->au16WdrSplitLut[_i] = _v;
        }

        for (; _i < X3; _i++) {
            _v = (((_i - X2) * (Y3 - Y2)) / DIV_0_TO_1(X3 - X2) + Y2);
            pstStaticRegCfg->au16WdrSplitLut[_i] = _v;
        }

        for (; _i < X_max; _i++) {
            _v = Y_max;
            pstStaticRegCfg->au16WdrSplitLut[_i] = _v;
        }
    } else {
        pstStaticRegCfg->u8BitDepthIn  = 16;
        pstStaticRegCfg->u8BitDepthOut = 16;
        pstStaticRegCfg->u8ModeIn      = 0;
        pstStaticRegCfg->u8ModeOut     = 0;
        memset(pstStaticRegCfg->au16WdrSplitLut, 0, 129 * sizeof(HI_U16));
    }

    pstStaticRegCfg->bResh     = HI_TRUE;
}

static HI_S32 SplitCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_split *split)
{
    HI_U8 i;

    ISP_CHECK_BOOL(split->enable);

    if (split->input_width_sel > 0x3) {
        ISP_ERR_TRACE("Invalid u8InputWidthSel!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (split->mode_in > 0x3) {
        ISP_ERR_TRACE("Invalid u8ModeIn!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (split->mode_out > 0x3) {
        ISP_ERR_TRACE("Invalid u8ModeOut!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((split->bit_depth_out > 0x14) || (split->bit_depth_out < 0xC)) {
        ISP_ERR_TRACE("Invalid u32BitDepthOut!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < ISP_SPLIT_POINT_NUM; i++) {
        if (split->split_point[i].x > 0x81) {
            ISP_ERR_TRACE("Invalid astSplitPoint[%d].u8X!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (split->split_point[i].y > 0x8000) {
            ISP_ERR_TRACE("Invalid astSplitPoint[%d].u16Y!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 SplitRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8     i;
    HI_S32    s32Ret;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    isp_sensor_get_default(ViPipe, &sns_dft);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (sns_dft->key.bit1_split) {
        ISP_CHECK_POINTER(sns_dft->split);

        s32Ret = SplitCheckCmosParam(ViPipe, sns_dft->split);
        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }
    }

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        SplitStaticRegsInitialize(ViPipe, &pstRegCfg->alg_reg_cfg[i].stSplitCfg.stStaticRegCfg);

        if (pstIspCtx->sns_wdr_mode == WDR_MODE_BUILT_IN) {
            if (sns_dft->key.bit1_split) {
                ISP_CHECK_POINTER(sns_dft->split);
                pstRegCfg->alg_reg_cfg[i].stSplitCfg.bEnable = sns_dft->split->enable;
            } else {
                pstRegCfg->alg_reg_cfg[i].stSplitCfg.bEnable = HI_FALSE;
            }
        } else {
            pstRegCfg->alg_reg_cfg[i].stSplitCfg.bEnable = HI_FALSE;
        }
    }

    pstRegCfg->cfg_key.bit1SplitCfg = 1;

    return HI_SUCCESS;
}

HI_S32 ISP_SplitInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32    s32Ret;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    s32Ret = SplitRegsInitialize(ViPipe, pstRegCfg);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SplitRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    return HI_SUCCESS;
}

HI_S32 ISP_SplitCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_SplitInit(ViPipe, (HI_VOID *)&pRegCfg->reg_cfg);
            break;
        default :
            break;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SplitExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stSplitCfg.bEnable = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1SplitCfg = 1;

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_split(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_SPLIT;
    pstAlgs->alg_func.pfn_alg_init = ISP_SplitInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_SplitRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_SplitCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_SplitExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
