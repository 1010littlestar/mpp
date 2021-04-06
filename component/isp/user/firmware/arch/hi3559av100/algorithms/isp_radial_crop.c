/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <math.h>
#include <stdio.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_sensor.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    HI_BOOL bRcEn;
    HI_BOOL bCoefUpdateEn;
    HI_U16  u16CenterVerCoor;
    HI_U16  u16CenterHorCoor;
    HI_U32  u32Radius;
} ISP_RC_S;

ISP_RC_S g_astRcCtx[ISP_MAX_PIPE_NUM] = {{0}};
#define RC_GET_CTX(dev, pstCtx)   pstCtx = &g_astRcCtx[dev]

static HI_VOID RcUsrRegsInitialize(ISP_RC_USR_CFG_S *pstUsrRegCfg, ISP_RC_S *pstRc)
{
    pstUsrRegCfg->u16CenterHorCoor = pstRc->u16CenterHorCoor;
    pstUsrRegCfg->u16CenterVerCoor = pstRc->u16CenterVerCoor;
    pstUsrRegCfg->u32SquareRadius  = pstRc->u32Radius * pstRc->u32Radius;
    pstUsrRegCfg->bResh            = HI_TRUE;
}

static HI_VOID RcRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    ISP_RC_S   *pstRc = HI_NULL;

    RC_GET_CTX(ViPipe, pstRc);

    RcUsrRegsInitialize(&pstRegCfg->alg_reg_cfg[0].stRcRegCfg.stUsrRegCfg, pstRc);

    pstRegCfg->alg_reg_cfg[0].stRcRegCfg.bRcEn = pstRc->bRcEn;

    pstRegCfg->cfg_key.bit1RcCfg = 1;

    return;
}

static HI_VOID RcExtRegsInitialize(VI_PIPE ViPipe)
{
    ISP_RC_S   *pstRc = HI_NULL;

    RC_GET_CTX(ViPipe, pstRc);

    hi_ext_system_rc_en_write(ViPipe, pstRc->bRcEn);
    hi_ext_system_rc_center_hor_coor_write(ViPipe, pstRc->u16CenterHorCoor);
    hi_ext_system_rc_center_ver_coor_write(ViPipe, pstRc->u16CenterVerCoor);
    hi_ext_system_rc_radius_write(ViPipe, pstRc->u32Radius);
    hi_ext_system_rc_coef_update_en_write(ViPipe, HI_FALSE);

    return;
}

static HI_VOID RcInitialize(VI_PIPE ViPipe)
{
    HI_U16     u16HorCoor, u16VerCoor;
    ISP_RC_S   *pstRc = HI_NULL;
    isp_usr_ctx  *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    RC_GET_CTX(ViPipe, pstRc);

    u16HorCoor  = (HI_U16)pstIspCtx->sys_rect.width >> 1;
    u16VerCoor  = (HI_U16)pstIspCtx->sys_rect.height >> 1;

    pstRc->u16CenterHorCoor = u16HorCoor;
    pstRc->u16CenterVerCoor = u16VerCoor;
    pstRc->u32Radius        = (HI_U32)sqrt((HI_DOUBLE)(u16HorCoor * u16HorCoor + u16VerCoor * u16VerCoor)) + 1;

    pstRc->bRcEn   = HI_FALSE;
}

static HI_BOOL __inline CheckRcOpen(ISP_RC_S  *pstRc)
{
    return (pstRc->bRcEn == HI_TRUE);
}

static HI_VOID RcReadExtRegs(VI_PIPE ViPipe)
{
    ISP_RC_S *pstRc      = HI_NULL;
    RC_GET_CTX(ViPipe, pstRc);

    pstRc->bCoefUpdateEn    = hi_ext_system_rc_coef_update_en_read(ViPipe);

    hi_ext_system_rc_coef_update_en_write(ViPipe, HI_FALSE);

    if (pstRc->bCoefUpdateEn) {
        pstRc->u16CenterHorCoor = hi_ext_system_rc_center_hor_coor_read(ViPipe);
        pstRc->u16CenterVerCoor = hi_ext_system_rc_center_ver_coor_read(ViPipe);
        pstRc->u32Radius        = hi_ext_system_rc_radius_read(ViPipe);
    }
}

HI_VOID Rc_Usr_Fw(ISP_RC_USR_CFG_S *pstUsrRegCfg, ISP_RC_S *pstRc)
{
    pstUsrRegCfg->u16CenterHorCoor = pstRc->u16CenterHorCoor;
    pstUsrRegCfg->u16CenterVerCoor = pstRc->u16CenterVerCoor;
    pstUsrRegCfg->u32SquareRadius  = pstRc->u32Radius * pstRc->u32Radius;
    pstUsrRegCfg->bResh            = HI_TRUE;
}

static HI_VOID ISP_RcWdrModeSet(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    pstRegCfg->cfg_key.bit1RcCfg = 1;
    pstRegCfg->alg_reg_cfg[0].stRcRegCfg.stUsrRegCfg.bResh = HI_TRUE;
}

HI_S32 ISP_RcInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    RcInitialize(ViPipe);
    RcRegsInitialize(ViPipe, pstRegCfg);
    RcExtRegsInitialize(ViPipe);

    return HI_SUCCESS;
}

HI_S32 ISP_RcRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                 HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ISP_RC_S  *pstRc       = HI_NULL;
    isp_usr_ctx *pstIspCtx   = HI_NULL;
    isp_reg_cfg *pstRegCfg  = (isp_reg_cfg *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    RC_GET_CTX(ViPipe, pstRc);

    /* calculate every two interrupts */
    if ((pstIspCtx->frame_cnt % 2 != 0) && (pstIspCtx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    pstRc->bRcEn = hi_ext_system_rc_en_read(ViPipe);
    pstRegCfg->alg_reg_cfg[0].stRcRegCfg.bRcEn = pstRc->bRcEn;
    pstRegCfg->cfg_key.bit1RcCfg = 1;

    /* check hardware setting */
    if (!CheckRcOpen(pstRc)) {
        return HI_SUCCESS;
    }

    RcReadExtRegs(ViPipe);

    if (pstRc->bCoefUpdateEn) {
        Rc_Usr_Fw(&pstRegCfg->alg_reg_cfg[0].stRcRegCfg.stUsrRegCfg, pstRc);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RcCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_RcWdrModeSet(ViPipe, (HI_VOID *)&pRegCfg->reg_cfg);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_RcExit(VI_PIPE ViPipe)
{
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    pRegCfg->reg_cfg.alg_reg_cfg[0].stRcRegCfg.bRcEn = HI_FALSE;
    pRegCfg->reg_cfg.cfg_key.bit1RcCfg                 = 1;

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_rc(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_RC;
    pstAlgs->alg_func.pfn_alg_init = ISP_RcInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_RcRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_RcCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_RcExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
