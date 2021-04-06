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
#include "mpi_sys.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static const  HI_U16 g_au16GainR = 128;
static const  HI_U16 g_au16GainG = 128;
static const  HI_U16 g_au16GainB = 128;

typedef struct hi_ISP_CLUT_CTX_S {
    HI_BOOL         bClutLutUpdateEn;
    HI_BOOL         bClutCtrlUpdateEn;
    HI_U32          *pu32VirAddr;
    ISP_CLUT_ATTR_S stClutCtrl;
} ISP_CLUT_CTX_S;

ISP_CLUT_CTX_S g_astClutCtx[ISP_MAX_PIPE_NUM] = {{0}};

#define CLUT_GET_CTX(dev, pstCtx)   pstCtx = &g_astClutCtx[dev]

static HI_VOID ClutExtRegsInitialize(VI_PIPE ViPipe)
{
    ISP_CLUT_CTX_S *pstClutCtx = HI_NULL;
    CLUT_GET_CTX(ViPipe, pstClutCtx);

    hi_ext_system_clut_en_write(ViPipe, pstClutCtx->stClutCtrl.bEnable);
    hi_ext_system_clut_gainR_write(ViPipe, pstClutCtx->stClutCtrl.u32GainR);
    hi_ext_system_clut_gainG_write(ViPipe, pstClutCtx->stClutCtrl.u32GainG);
    hi_ext_system_clut_gainB_write(ViPipe, pstClutCtx->stClutCtrl.u32GainB);
    hi_ext_system_clut_ctrl_update_en_write(ViPipe, HI_FALSE);
    hi_ext_system_clut_lut_update_en_write(ViPipe, HI_FALSE);

    return;
}

static HI_VOID ClutUsrCoefRegsInitialize(VI_PIPE ViPipe, ISP_CLUT_USR_COEF_CFG_S *pstUsrCoefRegCfg)
{
    HI_U32 u32Offset = 0;
    ISP_CLUT_CTX_S  *pstClutCtx  = HI_NULL;
    CLUT_GET_CTX(ViPipe, pstClutCtx);

    memcpy(pstUsrCoefRegCfg->au32lut0, pstClutCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT0 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT0;
    memcpy(pstUsrCoefRegCfg->au32lut1, pstClutCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT1 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT1;
    memcpy(pstUsrCoefRegCfg->au32lut2, pstClutCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT2 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT2;
    memcpy(pstUsrCoefRegCfg->au32lut3, pstClutCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT3 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT3;
    memcpy(pstUsrCoefRegCfg->au32lut4, pstClutCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT4 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT4;
    memcpy(pstUsrCoefRegCfg->au32lut5, pstClutCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT5 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT5;
    memcpy(pstUsrCoefRegCfg->au32lut6, pstClutCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT6 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT6;
    memcpy(pstUsrCoefRegCfg->au32lut7, pstClutCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT7 * sizeof(HI_U32));

    pstUsrCoefRegCfg->bResh          = HI_TRUE;
    pstUsrCoefRegCfg->u32UpdateIndex = 1;
}

static HI_VOID ClutUsrCtrlRegsInitialize(VI_PIPE ViPipe, ISP_CLUT_USR_CTRL_CFG_S *pstUsrCtrlRegCfg)
{
    pstUsrCtrlRegCfg->bDemoMode        = HI_FALSE;
    pstUsrCtrlRegCfg->u32GainR         = 128;
    pstUsrCtrlRegCfg->u32GainB         = 128;
    pstUsrCtrlRegCfg->u32GainG         = 128;
    pstUsrCtrlRegCfg->bDemoEnable      = HI_FALSE;
    pstUsrCtrlRegCfg->bResh            = HI_TRUE;

    return;
}
static HI_VOID ClutUsrRegsInitialize(VI_PIPE ViPipe, ISP_CLUT_USR_CFG_S *pstUsrRegCfg)
{
    ClutUsrCoefRegsInitialize(ViPipe, &pstUsrRegCfg->stClutUsrCoefCfg);
    ClutUsrCtrlRegsInitialize(ViPipe, &pstUsrRegCfg->stClutUsrCtrlCfg);

    return;
}

HI_S32 ClutRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    ISP_CLUT_CTX_S *pstClutCtx = HI_NULL;
    CLUT_GET_CTX(ViPipe, pstClutCtx);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        ClutUsrRegsInitialize(ViPipe, &pstRegCfg->alg_reg_cfg[i].stClutCfg.stUsrRegCfg);
        pstRegCfg->alg_reg_cfg[i].stClutCfg.bEnable = pstClutCtx->stClutCtrl.bEnable;
    }

    pstRegCfg->cfg_key.bit1ClutCfg   = 1;

    return HI_SUCCESS;
}

static HI_S32 ClutReadExtregs(VI_PIPE ViPipe)
{
    ISP_CLUT_CTX_S *pstCLUTCtx = HI_NULL;

    CLUT_GET_CTX(ViPipe, pstCLUTCtx);

    pstCLUTCtx->bClutCtrlUpdateEn = hi_ext_system_clut_ctrl_update_en_read(ViPipe);
    hi_ext_system_clut_ctrl_update_en_write(ViPipe, HI_FALSE);

    if (pstCLUTCtx->bClutCtrlUpdateEn) {
        pstCLUTCtx->stClutCtrl.bEnable    = hi_ext_system_clut_en_read(ViPipe);
        pstCLUTCtx->stClutCtrl.u32GainR   = hi_ext_system_clut_gainR_read(ViPipe);
        pstCLUTCtx->stClutCtrl.u32GainG   = hi_ext_system_clut_gainG_read(ViPipe);
        pstCLUTCtx->stClutCtrl.u32GainB   = hi_ext_system_clut_gainB_read(ViPipe);
    }

    pstCLUTCtx->bClutLutUpdateEn = hi_ext_system_clut_lut_update_en_read(ViPipe);
    hi_ext_system_clut_lut_update_en_write(ViPipe, HI_FALSE);

    return 0;
}

static HI_S32 ClutCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_clut *cmos_clut)
{
    ISP_CHECK_BOOL(cmos_clut->enable);

    if (cmos_clut->gain_r > 4095) {
        ISP_ERR_TRACE("Invalid gain_r!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (cmos_clut->gain_g > 4095) {
        ISP_ERR_TRACE("Invalid gain_g!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (cmos_clut->gain_b > 4095) {
        ISP_ERR_TRACE("Invalid gain_b!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static HI_S32 ClutInitialize(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_mmz_buf_ex   stClutBuf;
    ISP_CLUT_CTX_S     *pstClutCtx = HI_NULL;
    hi_isp_cmos_default *sns_dft  = HI_NULL;

    CLUT_GET_CTX(ViPipe, pstClutCtx);
    isp_sensor_get_default(ViPipe, &sns_dft);

    pstClutCtx->pu32VirAddr = HI_NULL;

    if (ioctl(g_as32IspFd[ViPipe], ISP_CLUT_BUF_GET, &stClutBuf.phy_addr) != HI_SUCCESS) {
        ISP_ERR_TRACE("Get Clut Buffer Err\n");
        return HI_FAILURE;
    }

    stClutBuf.vir_addr = HI_MPI_SYS_Mmap(stClutBuf.phy_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(HI_U32));

    if (!stClutBuf.vir_addr) {
        return HI_FAILURE;
    }

    pstClutCtx->pu32VirAddr = (HI_U32 *)stClutBuf.vir_addr;

    if (sns_dft->key.bit1_clut) {
        ISP_CHECK_POINTER(sns_dft->clut);

        s32Ret = ClutCheckCmosParam(ViPipe, sns_dft->clut);

        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        pstClutCtx->stClutCtrl.bEnable  = sns_dft->clut->enable;
        pstClutCtx->stClutCtrl.u32GainR = sns_dft->clut->gain_r;
        pstClutCtx->stClutCtrl.u32GainG = sns_dft->clut->gain_g;
        pstClutCtx->stClutCtrl.u32GainB = sns_dft->clut->gain_b;

        memcpy(pstClutCtx->pu32VirAddr, sns_dft->clut->clut_lut.lut, HI_ISP_CLUT_LUT_LENGTH * sizeof(HI_U32));
    } else {
        pstClutCtx->stClutCtrl.bEnable  = HI_FALSE;
        pstClutCtx->stClutCtrl.u32GainR = g_au16GainR;
        pstClutCtx->stClutCtrl.u32GainG = g_au16GainG;
        pstClutCtx->stClutCtrl.u32GainB = g_au16GainB;

        memset(pstClutCtx->pu32VirAddr, 0, HI_ISP_CLUT_LUT_LENGTH * sizeof(HI_U32));
    }

    return HI_SUCCESS;
}

HI_VOID Isp_Clut_Usr_Coef_Fw(ISP_CLUT_CTX_S *pstCLUTCtx, ISP_CLUT_USR_COEF_CFG_S *pstClutUsrCoefCfg)
{
    HI_U32 u32Offset = 0;

    memcpy(pstClutUsrCoefCfg->au32lut0, pstCLUTCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT0 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT0;
    memcpy(pstClutUsrCoefCfg->au32lut1, pstCLUTCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT1 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT1;
    memcpy(pstClutUsrCoefCfg->au32lut2, pstCLUTCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT2 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT2;
    memcpy(pstClutUsrCoefCfg->au32lut3, pstCLUTCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT3 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT3;
    memcpy(pstClutUsrCoefCfg->au32lut4, pstCLUTCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT4 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT4;
    memcpy(pstClutUsrCoefCfg->au32lut5, pstCLUTCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT5 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT5;
    memcpy(pstClutUsrCoefCfg->au32lut6, pstCLUTCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT6 * sizeof(HI_U32));

    u32Offset += HI_ISP_CLUT_LUT6;
    memcpy(pstClutUsrCoefCfg->au32lut7, pstCLUTCtx->pu32VirAddr + u32Offset, HI_ISP_CLUT_LUT7 * sizeof(HI_U32));

    pstClutUsrCoefCfg->bResh           = HI_TRUE;
    pstClutUsrCoefCfg->u32UpdateIndex += 1;
}

HI_VOID Isp_Clut_Usr_Ctrl_Fw(ISP_CLUT_ATTR_S *pstClutCtrl, ISP_CLUT_USR_CTRL_CFG_S *pstClutUsrCtrlCfg)
{
    pstClutUsrCtrlCfg->u32GainR       = pstClutCtrl->u32GainR;
    pstClutUsrCtrlCfg->u32GainG       = pstClutCtrl->u32GainG;
    pstClutUsrCtrlCfg->u32GainB       = pstClutCtrl->u32GainB;
    pstClutUsrCtrlCfg->bResh          = HI_TRUE;
}

static HI_BOOL __inline CheckClutOpen(ISP_CLUT_CTX_S *pstClutCtx)
{
    return (pstClutCtx->stClutCtrl.bEnable == HI_TRUE);
}

static HI_S32 ISP_ClutInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    s32Ret = ClutInitialize(ViPipe);

    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    ClutRegsInitialize(ViPipe, pstRegCfg);
    ClutExtRegsInitialize(ViPipe);

    return HI_SUCCESS;
}

static HI_S32 ISP_ClutRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                          HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_U8 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    ISP_CLUT_CTX_S *pstCLUTCtx = HI_NULL;
    isp_reg_cfg *pstReg = (isp_reg_cfg *)pRegCfg;

    CLUT_GET_CTX(ViPipe, pstCLUTCtx);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    /* calculate every two interrupts */
    if ((pstIspCtx->frame_cnt % 2 != 0) && (pstIspCtx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    pstCLUTCtx->stClutCtrl.bEnable = hi_ext_system_clut_en_read(ViPipe);

    for (i = 0; i < pstReg->cfg_num; i++) {
        pstReg->alg_reg_cfg[i].stClutCfg.bEnable = pstCLUTCtx->stClutCtrl.bEnable;
    }

    pstReg->cfg_key.bit1ClutCfg = 1;

    /* check hardware setting */
    if (!CheckClutOpen(pstCLUTCtx)) {
        return HI_SUCCESS;
    }

    ClutReadExtregs(ViPipe);

    if (pstCLUTCtx->bClutCtrlUpdateEn) {
        for (i = 0; i < pstReg->cfg_num; i++) {
            Isp_Clut_Usr_Ctrl_Fw(&pstCLUTCtx->stClutCtrl, &pstReg->alg_reg_cfg[i].stClutCfg.stUsrRegCfg.stClutUsrCtrlCfg);
        }
    }

    if (pstCLUTCtx->bClutLutUpdateEn) {
        for (i = 0; i < pstReg->cfg_num; i++) {
            if (!pstCLUTCtx->pu32VirAddr) {
                return HI_FAILURE;
            }

            Isp_Clut_Usr_Coef_Fw(pstCLUTCtx, &pstReg->alg_reg_cfg[i].stClutCfg.stUsrRegCfg.stClutUsrCoefCfg);
        }
    }

    return HI_SUCCESS;
}
static HI_S32 ISP_ClutCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd) {
        default :
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_ClutExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr   *pRegCfg    = HI_NULL;
    ISP_CLUT_CTX_S *pstClutCtx = HI_NULL;

    CLUT_GET_CTX(ViPipe, pstClutCtx);

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stClutCfg.bEnable = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1ClutCfg = 1;

    if (pstClutCtx->pu32VirAddr != HI_NULL) {
        HI_MPI_SYS_Munmap((HI_VOID *)pstClutCtx->pu32VirAddr, HI_ISP_CLUT_LUT_LENGTH * sizeof(HI_U32));
    }

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_clut(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_CLUT;
    pstAlgs->alg_func.pfn_alg_init = ISP_ClutInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_ClutRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_ClutCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_ClutExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
