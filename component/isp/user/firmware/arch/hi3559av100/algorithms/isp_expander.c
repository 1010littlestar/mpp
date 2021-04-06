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

HI_S32 ISP_ExpanderInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    return HI_SUCCESS;
}

HI_S32 ISP_ExpanderRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                       HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    return HI_SUCCESS;
}

HI_S32 ISP_ExpanderCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd) {
        default :
            break;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ExpanderExit(VI_PIPE ViPipe)
{
    return HI_SUCCESS;
}

HI_S32 isp_alg_register_expander(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_EXPANDER;
    pstAlgs->alg_func.pfn_alg_init = ISP_ExpanderInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_ExpanderRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_ExpanderCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_ExpanderExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
