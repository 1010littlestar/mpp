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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_S16 as16HRSFilterLut[2][6] = {
    { -14, 23, 232, 23, -14, 6},
    {10, -39, 157, 157, -39, 10}
};

static HI_VOID HrsRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 j;
    isp_usr_ctx    *pstIspCtx = HI_NULL;
    ISP_HRS_STATIC_CFG_S   *pstHrsStaticRegCfg  = &pstRegCfg->alg_reg_cfg[0].stHrsRegCfg.stStaticRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstHrsStaticRegCfg->u8RSEnable = (IS_HRS_ON(ViPipe) ? HI_TRUE : HI_FALSE);
    pstHrsStaticRegCfg->u8Enable   = HI_TRUE;
    pstHrsStaticRegCfg->u16Height  = pstIspCtx->sys_rect.height;
    pstHrsStaticRegCfg->u16Width   = pstIspCtx->sys_rect.width;

    for (j = 0; j < 6; j++) {
        pstHrsStaticRegCfg->as16HRSFilterLut0[j] = as16HRSFilterLut[0][j];
        pstHrsStaticRegCfg->as16HRSFilterLut1[j] = as16HRSFilterLut[1][j];
    }

    pstHrsStaticRegCfg->bResh    = HI_TRUE;

    pstRegCfg->cfg_key.bit1HrsCfg = 1;

    return;
}

HI_S32 ISP_HrsInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    HrsRegsInitialize(ViPipe, pstRegCfg);

    return HI_SUCCESS;
}

HI_S32 ISP_HrsRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                  HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    return HI_SUCCESS;
}

HI_S32 ISP_HrsCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :

            break;
        case ISP_PROC_WRITE:
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:

            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_HrsExit(VI_PIPE ViPipe)
{
    return HI_SUCCESS;
}

HI_S32 isp_alg_register_hrs(VI_PIPE ViPipe)
{
    isp_usr_ctx      *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs   = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_HRS;
    pstAlgs->alg_func.pfn_alg_init = ISP_HrsInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_HrsRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_HrsCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_HrsExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
