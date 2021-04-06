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

typedef struct hiISP_EDGEMARK_S {

    HI_BOOL bEdgeMarkMpiUpdateEn;
    HI_BOOL bEnable;
    HI_U8   u8Threshold;
    HI_U32  u32Color;
} ISP_EDGEMARK_S;

ISP_EDGEMARK_S g_astEdgeMarkCtx[ISP_MAX_PIPE_NUM] = {{0}};
#define EDGEMARK_GET_CTX(dev, pstCtx)   pstCtx = &g_astEdgeMarkCtx[dev]

static HI_VOID EdgeMarkExtRegsInitialize(VI_PIPE ViPipe)
{
    ISP_EDGEMARK_S *pstEdgeMark = HI_NULL;

    EDGEMARK_GET_CTX(ViPipe, pstEdgeMark);

    hi_ext_system_edgemark_mpi_update_en_write(ViPipe, HI_TRUE);

    hi_ext_system_manual_isp_edgemark_en_write(ViPipe, pstEdgeMark->bEnable);
    hi_ext_system_manual_isp_edgemark_color_write(ViPipe, pstEdgeMark->u32Color);
    hi_ext_system_manual_isp_edgemark_threshold_write(ViPipe, pstEdgeMark->u8Threshold);

    return;
}

static void EdgeMarkCheckReg(ISP_EDGEMARK_REG_CFG_S *pstEdgeMarkReg)
{

    pstEdgeMarkReg->bEnable = MIN2(pstEdgeMarkReg->bEnable, 0x1);
    pstEdgeMarkReg->u8markEdgeSft = MIN2(pstEdgeMarkReg->u8markEdgeSft, 12);
    pstEdgeMarkReg->u16uMarkValue = MIN2(pstEdgeMarkReg->u16uMarkValue, 1023);
    pstEdgeMarkReg->u16vMarkValue = MIN2(pstEdgeMarkReg->u16vMarkValue, 1023);

}

// ****Sharpen hardware Regs that will change with MPI and ISO****//
static HI_VOID EdgeMarkRegInit(ISP_EDGEMARK_REG_CFG_S *pstEdgeMarkReg)
{

    pstEdgeMarkReg->bEnable        = 0;
    pstEdgeMarkReg->u8markEdgeSft  = 0;
    pstEdgeMarkReg->u8MarkEdgeThd  = 100;
    pstEdgeMarkReg->u16uMarkValue  = 120;
    pstEdgeMarkReg->u16vMarkValue  = 220;
    pstEdgeMarkReg->u32UpdateIndex = 1;
    EdgeMarkCheckReg(pstEdgeMarkReg);
}

static HI_VOID EdgeMarkRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pRegCfg)
{
    HI_U32 i;

    for (i = 0; i < pRegCfg->cfg_num; i++) {
        pRegCfg->alg_reg_cfg[i].stEdgeMarkRegCfg.bEnable = HI_FALSE;
        EdgeMarkRegInit(&(pRegCfg->alg_reg_cfg[i].stEdgeMarkRegCfg));
    }

    pRegCfg->cfg_key.bit1EdgeMarkCfg = 1;

    return;
}


static HI_S32 EdgeMarkReadExtregs(VI_PIPE ViPipe)
{
    ISP_EDGEMARK_S *pstEdgeMark = HI_NULL;

    EDGEMARK_GET_CTX(ViPipe, pstEdgeMark);
    pstEdgeMark->bEdgeMarkMpiUpdateEn = hi_ext_system_edgemark_mpi_update_en_read(ViPipe);

    hi_ext_system_edgemark_mpi_update_en_write(ViPipe, HI_FALSE);

    if (pstEdgeMark->bEdgeMarkMpiUpdateEn) {
        pstEdgeMark->u32Color = hi_ext_system_manual_isp_edgemark_color_read(ViPipe);
        pstEdgeMark->u8Threshold = hi_ext_system_manual_isp_edgemark_threshold_read(ViPipe);

    }

    return HI_SUCCESS;
}

static HI_S32 EdgeMarkCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_edgemark *edge_mark)
{
    ISP_CHECK_BOOL(edge_mark->enable);

    if (edge_mark->color > 0xFFFFFF) {
        ISP_ERR_TRACE("Invalid u32Color! Value range:[0, 0xFFFFFF]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static HI_S32 EdgeMarkInitialize(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    ISP_EDGEMARK_S     *pstEdgeMark = HI_NULL;
    hi_isp_cmos_default *sns_dft   = HI_NULL;

    EDGEMARK_GET_CTX(ViPipe, pstEdgeMark);
    isp_sensor_get_default(ViPipe, &sns_dft);

    if (sns_dft->key.bit1_edge_mark) {
        ISP_CHECK_POINTER(sns_dft->edge_mark);

        s32Ret = EdgeMarkCheckCmosParam(ViPipe, sns_dft->edge_mark);
        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        pstEdgeMark->bEnable     = sns_dft->edge_mark->enable;
        pstEdgeMark->u8Threshold = sns_dft->edge_mark->threshold;
        pstEdgeMark->u32Color    = sns_dft->edge_mark->color;
    } else {
        pstEdgeMark->bEnable     = HI_FALSE;
        pstEdgeMark->u8Threshold = 100;
        pstEdgeMark->u32Color    = 0xFF0000;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_EdgeMarkInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret;

    s32Ret = EdgeMarkInitialize(ViPipe);

    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    EdgeMarkRegsInitialize(ViPipe, (isp_reg_cfg *)pRegCfg);
    EdgeMarkExtRegsInitialize(ViPipe);

    return HI_SUCCESS;
}

static void EdgeMarkRgb2Uv(HI_U8 u8R, HI_U8 u8G, HI_U8 u8B,  HI_U16 *u,  HI_U16 *v)
{
    HI_U8 i;
    /* RGB -> BT.709 (i.e. HD) */
    const HI_S16 cscMat[6] = {
        // R    G   B
        // 183, 614, 62,       // Y
        -101, -338, 439, // U
        439, -399, -40      // V
    }; // range[16,235]

    HI_S16 cscCoef[6];

    for (i = 0; i < 6; i++) {
        cscCoef[i] = cscMat[i] * 1024 / 1000;
    }

    *u = (HI_U16)(CLIP3(SignedRightShift((cscCoef[0] * u8R + cscCoef[1] * u8G + cscCoef[2] * u8B + (128 << 10)), 8), 0, 1023));
    *v = (HI_U16)(CLIP3(SignedRightShift((cscCoef[3] * u8R + cscCoef[4] * u8G + cscCoef[5] * u8B + (128 << 10)), 8), 0, 1023));

}

static void EdgeMarkMPI2Reg(ISP_EDGEMARK_REG_CFG_S *pstEdgeMarkRegCfg, ISP_EDGEMARK_S *pstEdgeMark)
{

    HI_U8   u8RValue, u8GValue, u8BValue;

    pstEdgeMarkRegCfg->bEnable       = pstEdgeMark->bEnable;
    pstEdgeMarkRegCfg->u8MarkEdgeThd = pstEdgeMark->u8Threshold;
    pstEdgeMarkRegCfg->u8markEdgeSft = 2;

    u8BValue = (HI_U8)((pstEdgeMark->u32Color) & 0xFF);
    u8GValue = (HI_U8)((pstEdgeMark->u32Color >> 8) & 0xFF);
    u8RValue = (HI_U8)((pstEdgeMark->u32Color >> 16) & 0xFF);

    EdgeMarkRgb2Uv(u8RValue, u8GValue, u8BValue, &(pstEdgeMarkRegCfg->u16uMarkValue), &(pstEdgeMarkRegCfg->u16vMarkValue));

    EdgeMarkCheckReg(pstEdgeMarkRegCfg);
}

static HI_BOOL __inline CheckEdgeMarkOpen(ISP_EDGEMARK_S *pstEdgeMark)
{
    return (pstEdgeMark->bEnable == HI_TRUE);
}

HI_S32 EdgeMarkProcWrite(VI_PIPE ViPipe, hi_isp_ctrl_proc_write *pstProc)
{
    hi_isp_ctrl_proc_write stProcTmp;

    ISP_EDGEMARK_S *pstEdgeMark = HI_NULL;

    EDGEMARK_GET_CTX(ViPipe, pstEdgeMark);

    if ((pstProc->proc_buff == HI_NULL) || (pstProc->buff_len == 0)) {
        return HI_FAILURE;
    }


    stProcTmp.proc_buff = pstProc->proc_buff;
    stProcTmp.buff_len = pstProc->buff_len;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "-----EDGEMARK INFO--------------------------------------------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s"    "%16s"    "%16s\n",
                    "bEnable", "Threshold", "Color");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16u"    "%16u"    "%16u\n",
                    (HI_U16)pstEdgeMark->bEnable,
                    (HI_U16)pstEdgeMark->u8Threshold,
                    (HI_U32)pstEdgeMark->u32Color);


    pstProc->write_len += 1;

    return HI_SUCCESS;
}

HI_S32 ISP_EdgeMarkRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo, HI_VOID *pRegCfg, HI_S32 s32Rsv)
{

    HI_U8  i;

    isp_usr_ctx *pstIspCtx = HI_NULL;
    ISP_EDGEMARK_S *pstEdgeMark = HI_NULL;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    EDGEMARK_GET_CTX(ViPipe, pstEdgeMark);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts */
    if ((pstIspCtx->frame_cnt % 2 != 0) && (pstIspCtx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    pstEdgeMark->bEnable = hi_ext_system_manual_isp_edgemark_en_read(ViPipe);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stEdgeMarkRegCfg.bEnable = pstEdgeMark->bEnable;
    }

    pstRegCfg->cfg_key.bit1EdgeMarkCfg = 1;

    /* check hardware setting */
    if (!CheckEdgeMarkOpen(pstEdgeMark)) {
        return HI_SUCCESS;
    }

    EdgeMarkReadExtregs(ViPipe);

    if (pstEdgeMark->bEdgeMarkMpiUpdateEn) {
        for (i = 0; i < pstRegCfg->cfg_num; i++) {
            pstRegCfg->alg_reg_cfg[i].stEdgeMarkRegCfg.u32UpdateIndex      += 1;
            EdgeMarkMPI2Reg(&(pstRegCfg->alg_reg_cfg[i].stEdgeMarkRegCfg), pstEdgeMark);
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_EdgeMarkCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd) {
        case ISP_PROC_WRITE:
            EdgeMarkProcWrite(ViPipe, (hi_isp_ctrl_proc_write *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_EdgeMarkExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr  *pRegCfg   = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stEdgeMarkRegCfg.bEnable = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1EdgeMarkCfg = 1;

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_edge_mark(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type =  ISP_ALG_EDGEAMRK;
    pstAlgs->alg_func.pfn_alg_init = ISP_EdgeMarkInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_EdgeMarkRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_EdgeMarkCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_EdgeMarkExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
