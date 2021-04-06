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

typedef struct hiISP_GCAC_S {
    HI_BOOL bGlobalCacEn;
    HI_BOOL bCoefUpdateEn;
    HI_S16  s16ParamRedA; // [-256,255]
    HI_S16  s16ParamRedB; // [-256,255]
    HI_S16  s16ParamRedC; // [-256,255]
    HI_S16  s16ParamBlueA; // [-256,255]
    HI_S16  s16ParamBlueB; // [-256,255]
    HI_S16  s16ParamBlueC; // [-256,255]

    HI_U8   u8VerNormShift; // [0,7]
    HI_U8   u8VerNormFactor; // [0,31]
    HI_U8   u8HorNormShift; // [0,7]
    HI_U8   u8HorNormFactor; // [0,31]
    HI_U16  u16CenterCoorHor;
    HI_U16  u16CenterCoorVer;
    HI_U16 u16CorVarThr; // [0,4095]   // edge
} ISP_GCAC_S;

ISP_GCAC_S *g_pastGlobalCacCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define GlobalCAC_GET_CTX(dev, pstCtx)   (pstCtx = g_pastGlobalCacCtx[dev])
#define GlobalCAC_SET_CTX(dev, pstCtx)   (g_pastGlobalCacCtx[dev] = pstCtx)
#define GlobalCAC_RESET_CTX(dev)         (g_pastGlobalCacCtx[dev] = HI_NULL)

HI_S32 GlobalCacCtxInit(VI_PIPE ViPipe)
{
    ISP_GCAC_S *pastGlobalCacCtx = HI_NULL;

    GlobalCAC_GET_CTX(ViPipe, pastGlobalCacCtx);

    if (pastGlobalCacCtx == HI_NULL) {
        pastGlobalCacCtx = (ISP_GCAC_S *)ISP_MALLOC(sizeof(ISP_GCAC_S));
        if (pastGlobalCacCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] GlobalCacCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastGlobalCacCtx, 0, sizeof(ISP_GCAC_S));

    GlobalCAC_SET_CTX(ViPipe, pastGlobalCacCtx);

    return HI_SUCCESS;
}

HI_VOID GlobalCacCtxExit(VI_PIPE ViPipe)
{
    ISP_GCAC_S *pastGlobalCacCtx = HI_NULL;

    GlobalCAC_GET_CTX(ViPipe, pastGlobalCacCtx);
    ISP_FREE(pastGlobalCacCtx);
    GlobalCAC_RESET_CTX(ViPipe);
}

static HI_VOID GCacExtRegsInitialize(VI_PIPE ViPipe)
{
    HI_U16 u16FullFrameWidth  = 0, u16FullFrameHeight = 0;
    isp_usr_ctx *pstIspCtx;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u16FullFrameWidth  = pstIspCtx->block_attr.frame_rect.width;
    u16FullFrameHeight = pstIspCtx->block_attr.frame_rect.height;

    /* global CAC */
    hi_ext_system_GlobalCAC_enable_write(ViPipe, HI_FALSE);
    hi_ext_system_GlobalCAC_coef_update_en_write(ViPipe, HI_TRUE);
    hi_ext_system_GlobalCAC_coor_center_hor_write(ViPipe, (u16FullFrameWidth - 1) >> 1);
    hi_ext_system_GlobalCAC_coor_center_ver_write(ViPipe, (u16FullFrameHeight - 1) >> 1);
    hi_ext_system_GlobalCAC_param_red_A_write(ViPipe, HI_ISP_GCAC_PARAM_RED_A_DEFAULT);
    hi_ext_system_GlobalCAC_param_red_B_write(ViPipe, HI_ISP_GCAC_PARAM_RED_B_DEFAULT);
    hi_ext_system_GlobalCAC_param_red_C_write(ViPipe, HI_ISP_GCAC_PARAM_RED_C_DEFAULT);
    hi_ext_system_GlobalCAC_param_blue_A_write(ViPipe, HI_ISP_GCAC_PARAM_BLUE_A_DEFAULT);
    hi_ext_system_GlobalCAC_param_blue_B_write(ViPipe, HI_ISP_GCAC_PARAM_BLUE_B_DEFAULT);
    hi_ext_system_GlobalCAC_param_blue_C_write(ViPipe, HI_ISP_GCAC_PARAM_BLUE_C_DEFAULT);
    hi_ext_system_GlobalCAC_y_ns_norm_write(ViPipe, HI_ISP_GCAC_Y_NORM_SHIFT_DEFAULT);
    hi_ext_system_GlobalCAC_y_nf_norm_write(ViPipe, HI_ISP_GCAC_Y_NORM_FACTOR_DEFAULT);
    hi_ext_system_GlobalCAC_x_ns_norm_write(ViPipe, HI_ISP_GCAC_X_NORM_SHIFT_DEFAULT);
    hi_ext_system_GlobalCAC_x_nf_norm_write(ViPipe, HI_ISP_GCAC_X_NORM_FACTOR_DEFAULT);

    hi_ext_system_GlobalCAC_cor_thr_write(ViPipe, HI_ISP_DEMOSAIC_GCAC_BLEND_THR_DEFAULT);
}

static HI_VOID GCacStaticRegsInitialize(VI_PIPE ViPipe, ISP_GLOBAL_CAC_STATIC_CFG_S *pstGCacStaticRegCfg, isp_usr_ctx *pstIspCtx)
{
    pstGCacStaticRegCfg->bGCacVerFilEn      = HI_ISP_GCAC_VER_FILT_EN_DEFAULT;
    pstGCacStaticRegCfg->u8GcacClipModeHor  = HI_ISP_GCAC_CLIP_MODE_HOR_DEFAULT;
    pstGCacStaticRegCfg->u8GcacClipModeVer  = HI_ISP_GCAC_CLIP_MODE_VER_DEFAULT;
    pstGCacStaticRegCfg->u8GCacBlendSel     = HI_ISP_GCAC_BLEND_SELECTION_DEFAULT;
    pstGCacStaticRegCfg->u8GcacChrVerMode   = HI_ISP_GCAC_CHR_VER_MODE_DEFAULT;
    pstGCacStaticRegCfg->bStaticResh        = HI_TRUE;
}

static HI_S32 GCacImageSize(HI_U8 i, ISP_GLOBAL_CAC_USR_CFG_S *pstGCacUsrRegCfg, isp_block_attr *pstBlockAttr)
{
    isp_rect stBlockRect;

    ISP_GetBlockRect(&stBlockRect, pstBlockAttr, i);
    pstGCacUsrRegCfg->u16StartCoorHor = stBlockRect.x;
    pstGCacUsrRegCfg->u16StartCoorVer = stBlockRect.y;

    return HI_SUCCESS;
}

static HI_VOID GCacUsrRegsInitialize(HI_U8 i, ISP_GLOBAL_CAC_USR_CFG_S *pstGCacUsrRegCfg, isp_usr_ctx *pstIspCtx)
{
    HI_U16 u16FullFrameWidth  = 0, u16FullFrameHeight = 0;

    u16FullFrameWidth  = pstIspCtx->block_attr.frame_rect.width;
    u16FullFrameHeight = pstIspCtx->block_attr.frame_rect.height;

    pstGCacUsrRegCfg->u32UpdateIndex   = 1;
    pstGCacUsrRegCfg->bResh            = HI_TRUE;
    pstGCacUsrRegCfg->s16ParamRedA     = HI_ISP_GCAC_PARAM_RED_A_DEFAULT;
    pstGCacUsrRegCfg->s16ParamRedB     = HI_ISP_GCAC_PARAM_RED_B_DEFAULT;
    pstGCacUsrRegCfg->s16ParamRedC     = HI_ISP_GCAC_PARAM_RED_C_DEFAULT;

    pstGCacUsrRegCfg->s16ParamBlueA    = HI_ISP_GCAC_PARAM_BLUE_A_DEFAULT;
    pstGCacUsrRegCfg->s16ParamBlueB    = HI_ISP_GCAC_PARAM_BLUE_B_DEFAULT;
    pstGCacUsrRegCfg->s16ParamBlueC    = HI_ISP_GCAC_PARAM_BLUE_C_DEFAULT;

    pstGCacUsrRegCfg->u8VerNormShift   = HI_ISP_GCAC_Y_NORM_SHIFT_DEFAULT;
    pstGCacUsrRegCfg->u8VerNormFactor  = HI_ISP_GCAC_Y_NORM_FACTOR_DEFAULT;
    pstGCacUsrRegCfg->u8HorNormShift   = HI_ISP_GCAC_X_NORM_SHIFT_DEFAULT;
    pstGCacUsrRegCfg->u8HorNormFactor  = HI_ISP_GCAC_X_NORM_FACTOR_DEFAULT;
    pstGCacUsrRegCfg->u16CorVarThr     = HI_ISP_DEMOSAIC_GCAC_BLEND_THR_DEFAULT;
    pstGCacUsrRegCfg->u16CenterCoorVer = (u16FullFrameHeight - 1) >> 1;
    pstGCacUsrRegCfg->u16CenterCoorHor = (u16FullFrameWidth - 1) >> 1;

    GCacImageSize(i, pstGCacUsrRegCfg, &pstIspCtx->block_attr);
}

static HI_VOID GCacRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    isp_usr_ctx  *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < pstIspCtx->block_attr.block_num; i++) {
        GCacStaticRegsInitialize(ViPipe, &pstRegCfg->alg_reg_cfg[i].stGCacRegCfg.stStaticRegCfg, pstIspCtx);

        GCacUsrRegsInitialize(i, &pstRegCfg->alg_reg_cfg[i].stGCacRegCfg.stUsrRegCfg, pstIspCtx);

        pstRegCfg->alg_reg_cfg[i].stGCacRegCfg.bGlobalCacEn = HI_FALSE;
    }

    pstRegCfg->cfg_key.bit1GlobalCacCfg = 1;
}

static HI_VOID GCacInitialize(VI_PIPE ViPipe)
{
    ISP_GCAC_S *pstGlobalCAC    = HI_NULL;

    GlobalCAC_GET_CTX(ViPipe, pstGlobalCAC);
    ISP_CHECK_POINTER_VOID(pstGlobalCAC);

    pstGlobalCAC->bGlobalCacEn    = HI_FALSE;
}

static HI_S32 GCacReadExtregs(VI_PIPE ViPipe)
{
    ISP_GCAC_S  *pstGlobalCac = HI_NULL;

    GlobalCAC_GET_CTX(ViPipe, pstGlobalCac);
    ISP_CHECK_POINTER(pstGlobalCac);

    pstGlobalCac->bCoefUpdateEn = hi_ext_system_GlobalCAC_coef_update_en_read(ViPipe);
    hi_ext_system_GlobalCAC_coef_update_en_write(ViPipe, HI_FALSE);

    if (pstGlobalCac->bCoefUpdateEn) {
        pstGlobalCac->s16ParamRedA      = hi_ext_system_GlobalCAC_param_red_A_read(ViPipe);
        pstGlobalCac->s16ParamRedB      = hi_ext_system_GlobalCAC_param_red_B_read(ViPipe);
        pstGlobalCac->s16ParamRedC      = hi_ext_system_GlobalCAC_param_red_C_read(ViPipe);
        pstGlobalCac->s16ParamBlueA     = hi_ext_system_GlobalCAC_param_blue_A_read(ViPipe);
        pstGlobalCac->s16ParamBlueB     = hi_ext_system_GlobalCAC_param_blue_B_read(ViPipe);
        pstGlobalCac->s16ParamBlueC     = hi_ext_system_GlobalCAC_param_blue_C_read(ViPipe);
        pstGlobalCac->u8VerNormShift    = hi_ext_system_GlobalCAC_y_ns_norm_read(ViPipe);
        pstGlobalCac->u8VerNormFactor   = hi_ext_system_GlobalCAC_y_nf_norm_read(ViPipe);
        pstGlobalCac->u8HorNormShift    = hi_ext_system_GlobalCAC_x_ns_norm_read(ViPipe);
        pstGlobalCac->u8HorNormFactor   = hi_ext_system_GlobalCAC_x_nf_norm_read(ViPipe);
        pstGlobalCac->u16CenterCoorHor  = hi_ext_system_GlobalCAC_coor_center_hor_read(ViPipe);
        pstGlobalCac->u16CenterCoorVer  = hi_ext_system_GlobalCAC_coor_center_ver_read(ViPipe);
        pstGlobalCac->u16CorVarThr      = hi_ext_system_GlobalCAC_cor_thr_read(ViPipe);
    }

    return HI_SUCCESS;
}

HI_S32 global_cac_usr_fw(HI_U8 u8CurBlk, ISP_GCAC_S *pstGlobalCAC, ISP_GLOBAL_CAC_USR_CFG_S *pstGCacUsrRegCfg)
{
    pstGCacUsrRegCfg->s16ParamRedA     = pstGlobalCAC->s16ParamRedA;
    pstGCacUsrRegCfg->s16ParamRedB     = pstGlobalCAC->s16ParamRedB;
    pstGCacUsrRegCfg->s16ParamRedC     = pstGlobalCAC->s16ParamRedC;
    pstGCacUsrRegCfg->s16ParamBlueA    = pstGlobalCAC->s16ParamBlueA;
    pstGCacUsrRegCfg->s16ParamBlueB    = pstGlobalCAC->s16ParamBlueB;
    pstGCacUsrRegCfg->s16ParamBlueC    = pstGlobalCAC->s16ParamRedC;
    pstGCacUsrRegCfg->u8VerNormShift   = pstGlobalCAC->u8VerNormShift;
    pstGCacUsrRegCfg->u8VerNormFactor  = pstGlobalCAC->u8VerNormFactor;
    pstGCacUsrRegCfg->u8HorNormShift   = pstGlobalCAC->u8HorNormShift;
    pstGCacUsrRegCfg->u8HorNormFactor  = pstGlobalCAC->u8HorNormFactor;
    pstGCacUsrRegCfg->u16CorVarThr     = pstGlobalCAC->u16CorVarThr;
    pstGCacUsrRegCfg->u16CenterCoorVer = pstGlobalCAC->u16CenterCoorVer;
    pstGCacUsrRegCfg->u16CenterCoorHor = pstGlobalCAC->u16CenterCoorHor;
    pstGCacUsrRegCfg->bResh            = HI_TRUE;
    pstGCacUsrRegCfg->u32UpdateIndex  += 1;

    return HI_SUCCESS;
}

static HI_BOOL __inline CheckGCacOpen(ISP_GCAC_S *pstGlobalCAC)
{
    return (pstGlobalCAC->bGlobalCacEn == HI_TRUE);
}

HI_S32 ISP_GCacInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    s32Ret = GlobalCacCtxInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    GCacRegsInitialize(ViPipe, pstRegCfg);
    GCacExtRegsInitialize(ViPipe);
    GCacInitialize(ViPipe);

    return HI_SUCCESS;
}

HI_S32 ISP_GCacRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                   HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_U8         i;
    isp_usr_ctx     *pstIspCtx    = HI_NULL;
    ISP_GCAC_S    *pstGlobalCAC = HI_NULL;
    isp_reg_cfg *pstRegCfg    = (isp_reg_cfg *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    GlobalCAC_GET_CTX(ViPipe, pstGlobalCAC);
    ISP_CHECK_POINTER(pstGlobalCAC);

    if (pstIspCtx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    pstGlobalCAC->bGlobalCacEn = hi_ext_system_GlobalCAC_enable_read(ViPipe);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stGCacRegCfg.bGlobalCacEn = pstGlobalCAC->bGlobalCacEn;
    }

    pstRegCfg->cfg_key.bit1GlobalCacCfg = 1;

    /* check hardware setting */
    if (!CheckGCacOpen(pstGlobalCAC)) {
        return HI_SUCCESS;
    }

    GCacReadExtregs(ViPipe);

    if (pstGlobalCAC->bCoefUpdateEn) {
        for (i = 0; i < pstIspCtx->block_attr.block_num; i++) {
            global_cac_usr_fw(i, pstGlobalCAC, &pstRegCfg->alg_reg_cfg[i].stGCacRegCfg.stUsrRegCfg);
        }
    }

    return HI_SUCCESS;
}

static __inline HI_S32 GCACImageResWrite(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < pstIspCtx->block_attr.block_num; i++) {
        GCacImageSize(i, &pstRegCfg->alg_reg_cfg[i].stGCacRegCfg.stUsrRegCfg, &pstIspCtx->block_attr);
        pstRegCfg->alg_reg_cfg[i].stGCacRegCfg.stUsrRegCfg.u32UpdateIndex += 1;
        pstRegCfg->alg_reg_cfg[i].stGCacRegCfg.stUsrRegCfg.bResh           = HI_TRUE;
    }

    pstRegCfg->cfg_key.bit1GlobalCacCfg = 1;

    return HI_SUCCESS;
}

HI_S32 ISP_GCacCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    switch (u32Cmd) {
        case ISP_CHANGE_IMAGE_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            GCACImageResWrite(ViPipe, &pRegCfg->reg_cfg);
            break;
        default :
            break;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_GCacExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stGCacRegCfg.bGlobalCacEn = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1GlobalCacCfg = 1;

    GlobalCacCtxExit(ViPipe);

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_gcac(VI_PIPE ViPipe)
{
    isp_usr_ctx      *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs   = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_GCAC;
    pstAlgs->alg_func.pfn_alg_init = ISP_GCacInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_GCacRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_GCacCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_GCacExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
