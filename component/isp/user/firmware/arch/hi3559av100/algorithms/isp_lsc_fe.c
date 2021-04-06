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
#include "isp_sensor.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define HI_ISP_FE_LSC_DEFAULT_WIDTH_OFFSET 0

static const  HI_U16  g_au16MeshGainDef[8] = {512, 256, 128, 64, 0, 0, 0, 0};

typedef struct hiISP_FeLSC {
    HI_BOOL bFeLscEn;
    HI_BOOL bLscCoefUpdata;
    HI_BOOL bLutUpdate;

    HI_U8  u8MeshScale;
    HI_U16 u16MeshStrength;
    HI_U16 u16MeshWeight;

    HI_U16 au16DeltaX[HI_ISP_LSC_GRID_COL - 1];
    HI_U16 au16DeltaY[(HI_ISP_LSC_GRID_ROW - 1) / 2];
    HI_U16 au16InvX[HI_ISP_LSC_GRID_COL - 1];
    HI_U16 au16InvY[(HI_ISP_LSC_GRID_ROW - 1) / 2];

    HI_U32 u32Width;
} ISP_FeLSC_S;

ISP_FeLSC_S *g_astFeLscCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define FeLSC_GET_CTX(dev, pstCtx)   (pstCtx = g_astFeLscCtx[dev])
#define FeLSC_SET_CTX(dev, pstCtx)   (g_astFeLscCtx[dev] = pstCtx)
#define FeLSC_RESET_CTX(dev)         (g_astFeLscCtx[dev] = HI_NULL)

HI_S32 FeLscCtxInit(VI_PIPE ViPipe)
{
    ISP_FeLSC_S *pastFeLscCtx = HI_NULL;

    FeLSC_GET_CTX(ViPipe, pastFeLscCtx);

    if (pastFeLscCtx == HI_NULL) {
        pastFeLscCtx = (ISP_FeLSC_S *)ISP_MALLOC(sizeof(ISP_FeLSC_S));
        if (pastFeLscCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] LscCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastFeLscCtx, 0, sizeof(ISP_FeLSC_S));

    FeLSC_SET_CTX(ViPipe, pastFeLscCtx);

    return HI_SUCCESS;
}

HI_VOID FeLscCtxExit(VI_PIPE ViPipe)
{
    ISP_FeLSC_S *pastFeLscCtx = HI_NULL;

    FeLSC_GET_CTX(ViPipe, pastFeLscCtx);
    ISP_FREE(pastFeLscCtx);
    FeLSC_RESET_CTX(ViPipe);
}

static HI_VOID geometricInvSizeFeLsc(ISP_FeLSC_S *pstFeLsc)
{
    HI_S32 i;

    for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1); i++) {
        if (pstFeLsc->au16DeltaX[i] != 0) {
            pstFeLsc->au16InvX[i] = (4096 * 1024 / pstFeLsc->au16DeltaX[i] + 512) >> 10;
        } else {
            pstFeLsc->au16InvX[i] = 0;
        }
    }


    for (i = 0; i < ((HI_ISP_LSC_GRID_ROW - 1) / 2); i++) {
        if (pstFeLsc->au16DeltaY[i] != 0) {
            pstFeLsc->au16InvY[i] = (4096 * 1024 / pstFeLsc->au16DeltaY[i] + 512) >> 10;
        } else {
            pstFeLsc->au16InvY[i] = 0;
        }
    }

    return;
}

static HI_VOID FeLscStaticRegInit(VI_PIPE ViPipe, ISP_FE_LSC_STATIC_CFG_S *pstStaticRegCfg)
{
    pstStaticRegCfg->bResh          = HI_TRUE;
    pstStaticRegCfg->u8WinNumH      = HI_ISP_LSC_GRID_COL - 1;
    pstStaticRegCfg->u8WinNumV      = HI_ISP_LSC_GRID_ROW - 1;
    pstStaticRegCfg->u16WidthOffset = HI_ISP_FE_LSC_DEFAULT_WIDTH_OFFSET;

    return;
}

static HI_VOID FeLscUsrRegInit(VI_PIPE ViPipe, ISP_FE_LSC_USR_CFG_S *pstUsrRegCfg)
{
    HI_U16 i;
    HI_U32 u32DefGain;
    ISP_FeLSC_S            *pstFeLsc   = HI_NULL;
    hi_isp_cmos_default    *sns_dft  = HI_NULL;
    const hi_isp_cmos_lsc  *cmos_lsc = HI_NULL;

    isp_sensor_get_default(ViPipe, &sns_dft);
    FeLSC_GET_CTX(ViPipe, pstFeLsc);
    ISP_CHECK_POINTER_VOID(pstFeLsc);

    pstUsrRegCfg->bResh       = HI_TRUE;
    pstUsrRegCfg->bLutUpdate  = HI_TRUE;
    pstUsrRegCfg->u8MeshScale = pstFeLsc->u8MeshScale;
    pstUsrRegCfg->u16MeshStr  = pstFeLsc->u16MeshStrength;
    pstUsrRegCfg->u16Weight   = pstFeLsc->u16MeshWeight;

    memcpy(pstUsrRegCfg->au16DeltaX, pstFeLsc->au16DeltaX, sizeof(HI_U16) * (HI_ISP_LSC_GRID_COL - 1));
    memcpy(pstUsrRegCfg->au16InvX, pstFeLsc->au16InvX, sizeof(HI_U16) * (HI_ISP_LSC_GRID_COL - 1));

    memcpy(pstUsrRegCfg->au16DeltaY, pstFeLsc->au16DeltaY, sizeof(HI_U16) * (HI_ISP_LSC_GRID_ROW - 1) / 2);
    memcpy(pstUsrRegCfg->au16InvY, pstFeLsc->au16InvY, sizeof(HI_U16) * (HI_ISP_LSC_GRID_ROW - 1) / 2);

    if (sns_dft->key.bit1_lsc) {
        ISP_CHECK_POINTER_VOID(sns_dft->lsc);

        cmos_lsc = sns_dft->lsc;

        for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
            pstUsrRegCfg->au32RGain[i]  = ((HI_U32)cmos_lsc->lsc_calib_table[1].r_gain[i]  << 10) + cmos_lsc->lsc_calib_table[0].r_gain[i];
            pstUsrRegCfg->au32GrGain[i] = ((HI_U32)cmos_lsc->lsc_calib_table[1].gr_gain[i] << 10) + cmos_lsc->lsc_calib_table[0].gr_gain[i];
            pstUsrRegCfg->au32GbGain[i] = ((HI_U32)cmos_lsc->lsc_calib_table[1].gb_gain[i] << 10) + cmos_lsc->lsc_calib_table[0].gb_gain[i];
            pstUsrRegCfg->au32BGain[i]  = ((HI_U32)cmos_lsc->lsc_calib_table[1].b_gain[i]  << 10) + cmos_lsc->lsc_calib_table[0].b_gain[i];
        }
    } else {
        u32DefGain = ((HI_U32)g_au16MeshGainDef[pstFeLsc->u8MeshScale] << 10) + g_au16MeshGainDef[pstFeLsc->u8MeshScale];

        for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
            pstUsrRegCfg->au32RGain[i]  = u32DefGain;
            pstUsrRegCfg->au32GrGain[i] = u32DefGain;
            pstUsrRegCfg->au32GbGain[i] = u32DefGain;
            pstUsrRegCfg->au32BGain[i]  = u32DefGain;
        }
    }

    return;
}

static HI_VOID FeLscRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    ISP_FeLSC_S *pstFeLsc = HI_NULL;

    FeLSC_GET_CTX(ViPipe, pstFeLsc);
    ISP_CHECK_POINTER_VOID(pstFeLsc);

    FeLscStaticRegInit(ViPipe, &pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.stStaticRegCfg);
    FeLscUsrRegInit(ViPipe, &pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.stUsrRegCfg);

    pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.bLscEn = pstFeLsc->bFeLscEn;

    pstRegCfg->cfg_key.bit1FeLscCfg = 1;

    return;
}

static HI_VOID FeLscExtRegsInitialize(VI_PIPE ViPipe)
{
    ISP_FeLSC_S *pstFeLsc = HI_NULL;
    FeLSC_GET_CTX(ViPipe, pstFeLsc);
    ISP_CHECK_POINTER_VOID(pstFeLsc);

    hi_ext_system_isp_fe_lsc_enable_write(ViPipe, pstFeLsc->bFeLscEn);
    hi_ext_system_isp_mesh_shading_fe_lut_attr_updata_write(ViPipe, HI_FALSE);
    hi_ext_system_isp_mesh_shading_fe_attr_updata_write(ViPipe, HI_FALSE);

    return;
}

static HI_VOID geometricGridSizeFeLsc(HI_U16 *pu16Delta, HI_U16 *pu16Inv, HI_U16 u16Length, HI_U16 u16GridSize)
{
    HI_U16 i, sum;
    HI_U16 u16HalfGridSize;
    HI_U16 diff;
    HI_U16 *pu16TmpStep = HI_NULL;
    HI_U16 u16SumR;

    u16HalfGridSize = (u16GridSize - 1) >> 1;

    if (u16HalfGridSize == 0) {
        return;
    }

    pu16TmpStep = (HI_U16 *)ISP_MALLOC(sizeof(HI_U16) * u16HalfGridSize);

    if (pu16TmpStep == NULL) {
        return;
    }

    memset(pu16TmpStep, 0, sizeof(HI_U16)*u16HalfGridSize);

    u16SumR = u16HalfGridSize;

    for (i = 0; i < u16HalfGridSize; i++) {
        pu16TmpStep[i] = (HI_U16)((((u16Length >> 1) * 1024 / DIV_0_TO_1(u16SumR)) + 512) >> 10);
    }

    sum = 0;
    for (i = 0; i < u16HalfGridSize; i++) {
        sum = sum + pu16TmpStep[i];
    }

    if (sum != (u16Length >> 1)) {
        if (sum > (u16Length >> 1)) {
            diff = sum - (u16Length >> 1);
            for (i = 1; i <= diff; i++) {
                pu16TmpStep[u16HalfGridSize - i] = pu16TmpStep[u16HalfGridSize - i] - 1;
            }
        } else {
            diff = (u16Length >> 1) - sum;
            for (i = 1; i <= diff; i++) {
                pu16TmpStep[i - 1] = pu16TmpStep[i - 1] + 1;
            }
        }
    }

    for (i = 0; i < u16HalfGridSize; i++) {
        pu16Delta[i] = pu16TmpStep[i];
        pu16Inv[i]   = (pu16Delta[i] == 0) ? (0) : ((4096 * 1024 / DIV_0_TO_1(pu16Delta[i]) + 512) >> 10);
    }

    ISP_FREE(pu16TmpStep);

    return;
}

static HI_VOID FeLscImageSize(VI_PIPE ViPipe, ISP_FeLSC_S *pstFeLsc)
{
    HI_U8      i;
    HI_U32     u32Width, u32Height;
    isp_usr_ctx  *pstIspCtx  = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u32Width  = pstIspCtx->sys_rect.width;
    u32Height = pstIspCtx->sys_rect.height;

    geometricGridSizeFeLsc(pstFeLsc->au16DeltaX, pstFeLsc->au16InvX, u32Width / 2, HI_ISP_LSC_GRID_COL);
    geometricGridSizeFeLsc(pstFeLsc->au16DeltaY, pstFeLsc->au16InvY, u32Height / 2, HI_ISP_LSC_GRID_ROW);

    for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1) / 2; i++) {
        pstFeLsc->au16DeltaX[HI_ISP_LSC_GRID_COL - 2 - i] = pstFeLsc->au16DeltaX[i];
        pstFeLsc->au16InvX[HI_ISP_LSC_GRID_COL - 2 - i]   = pstFeLsc->au16InvX[i];
    }

    return;
}

static HI_S32 FeLscCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_lsc *cmos_lsc)
{
    HI_U16 i;

    if (cmos_lsc->mesh_scale > 7) {
        ISP_ERR_TRACE("Invalid u8MeshScale!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
        if ((cmos_lsc->lsc_calib_table[0].r_gain[i]  > 1023) || (cmos_lsc->lsc_calib_table[0].gr_gain[i] > 1023) || \
            (cmos_lsc->lsc_calib_table[0].gb_gain[i] > 1023) || (cmos_lsc->lsc_calib_table[0].b_gain[i]  > 1023) || \
            (cmos_lsc->lsc_calib_table[1].r_gain[i]  > 1023) || (cmos_lsc->lsc_calib_table[1].gr_gain[i] > 1023) || \
            (cmos_lsc->lsc_calib_table[1].gb_gain[i] > 1023) || (cmos_lsc->lsc_calib_table[1].b_gain[i]  > 1023)) {
            ISP_ERR_TRACE("Invalid Gain!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    return HI_SUCCESS;
}

static HI_S32 FeLscInitialize(VI_PIPE ViPipe)
{
    HI_S32     s32Ret;
    ISP_FeLSC_S *pstFeLsc          = HI_NULL;
    hi_isp_cmos_default *sns_dft  = HI_NULL;
    isp_usr_ctx          *pstIspCtx  = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    FeLSC_GET_CTX(ViPipe, pstFeLsc);
    isp_sensor_get_default(ViPipe, &sns_dft);
    ISP_CHECK_POINTER(pstFeLsc);

    pstFeLsc->u16MeshStrength = HI_ISP_LSC_DEFAULT_MESH_STRENGTH;
    pstFeLsc->u16MeshWeight   = HI_ISP_LSC_DEFAULT_WEIGHT;

    if (sns_dft->key.bit1_lsc) {
        ISP_CHECK_POINTER(sns_dft->lsc);

        s32Ret = FeLscCheckCmosParam(ViPipe, sns_dft->lsc);
        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        pstFeLsc->u8MeshScale     = sns_dft->lsc->mesh_scale;
    } else {
        pstFeLsc->u8MeshScale = HI_ISP_LSC_DEFAULT_MESH_SCALE;
    }

    FeLscImageSize(ViPipe, pstFeLsc);

    if (pstIspCtx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR) {
        pstFeLsc->bFeLscEn = HI_FALSE;
    } else {
        pstFeLsc->bFeLscEn = HI_TRUE;
    }

    return HI_SUCCESS;
}

static HI_VOID FeLscReadExtRegs(VI_PIPE ViPipe)
{
    HI_U16  i;
    HI_U16  au16DeltaX[(HI_ISP_LSC_GRID_COL - 1) / 2];
    ISP_FeLSC_S *pstFeLsc   = HI_NULL;
    isp_usr_ctx   *pstIspCtx  = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    FeLSC_GET_CTX(ViPipe, pstFeLsc);
    ISP_CHECK_POINTER_VOID(pstFeLsc);

    pstFeLsc->bLscCoefUpdata  = hi_ext_system_isp_mesh_shading_fe_attr_updata_read(ViPipe);
    hi_ext_system_isp_mesh_shading_fe_attr_updata_write(ViPipe, HI_FALSE);

    if (pstFeLsc->bLscCoefUpdata) {
        pstFeLsc->u16MeshStrength = hi_ext_system_isp_mesh_shading_mesh_strength_read(ViPipe);
        pstFeLsc->u16MeshWeight   = hi_ext_system_isp_mesh_shading_blendratio_read(ViPipe);
    }

    pstFeLsc->bLutUpdate      = hi_ext_system_isp_mesh_shading_fe_lut_attr_updata_read(ViPipe);
    hi_ext_system_isp_mesh_shading_fe_lut_attr_updata_write(ViPipe, HI_FALSE);

    if (pstFeLsc->bLutUpdate) {
        pstFeLsc->u8MeshScale = hi_ext_system_isp_mesh_shading_mesh_scale_read(ViPipe);

        if (pstIspCtx->sys_rect.width == pstIspCtx->block_attr.frame_rect.width) {
            for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1) / 2; i++) {
                au16DeltaX[i] = hi_ext_system_isp_mesh_shading_xgrid_read(ViPipe, i);
            }

            for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1) / 2; i++) {
                pstFeLsc->au16DeltaX[i] = au16DeltaX[i];
            }

            for (i = (HI_ISP_LSC_GRID_COL - 1) / 2; i < HI_ISP_LSC_GRID_COL - 1; i++) {
                pstFeLsc->au16DeltaX[i] = au16DeltaX[HI_ISP_LSC_GRID_COL - 2 - i];
            }
        }

        if (pstIspCtx->sys_rect.height == pstIspCtx->block_attr.frame_rect.height) {
            for (i = 0; i < (HI_ISP_LSC_GRID_ROW - 1) / 2; i++) {
                pstFeLsc->au16DeltaY[i] = hi_ext_system_isp_mesh_shading_ygrid_read(ViPipe, i);
            }
        }

        geometricInvSizeFeLsc(pstFeLsc);
    }

    return;
}

static HI_VOID FeLsc_Usr_Fw(VI_PIPE ViPipe, ISP_FeLSC_S *pstFeLsc, ISP_FE_LSC_USR_CFG_S *pstUsrRegCfg)
{
    HI_U16  i;
    HI_U16  r_gain0, r_gain1, gr_gain0, gr_gain1, gb_gain0, gb_gain1, b_gain0, b_gain1;

    pstUsrRegCfg->bResh       = HI_TRUE;
    pstUsrRegCfg->bLutUpdate  = HI_TRUE;
    pstUsrRegCfg->u8MeshScale = pstFeLsc->u8MeshScale;

    memcpy(pstUsrRegCfg->au16DeltaX, pstFeLsc->au16DeltaX, sizeof(HI_U16) * (HI_ISP_LSC_GRID_COL - 1));
    memcpy(pstUsrRegCfg->au16InvX,   pstFeLsc->au16InvX,   sizeof(HI_U16) * (HI_ISP_LSC_GRID_COL - 1));

    memcpy(pstUsrRegCfg->au16DeltaY, pstFeLsc->au16DeltaY, sizeof(HI_U16) * (HI_ISP_LSC_GRID_ROW - 1) / 2);
    memcpy(pstUsrRegCfg->au16InvY,   pstFeLsc->au16InvY,   sizeof(HI_U16) * (HI_ISP_LSC_GRID_ROW - 1) / 2);

    for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
        r_gain0  = hi_ext_system_isp_mesh_shading_r_gain0_read(ViPipe, i);
        r_gain1  = hi_ext_system_isp_mesh_shading_r_gain1_read(ViPipe, i);

        gr_gain0 = hi_ext_system_isp_mesh_shading_gr_gain0_read(ViPipe, i);
        gr_gain1 = hi_ext_system_isp_mesh_shading_gr_gain1_read(ViPipe, i);

        gb_gain0 = hi_ext_system_isp_mesh_shading_gb_gain0_read(ViPipe, i);
        gb_gain1 = hi_ext_system_isp_mesh_shading_gb_gain1_read(ViPipe, i);

        b_gain0  = hi_ext_system_isp_mesh_shading_b_gain0_read(ViPipe, i);
        b_gain1  = hi_ext_system_isp_mesh_shading_b_gain1_read(ViPipe, i);

        pstUsrRegCfg->au32RGain[i]  = ((HI_U32)r_gain1  << 10) + r_gain0;
        pstUsrRegCfg->au32GrGain[i] = ((HI_U32)gr_gain1 << 10) + gr_gain0;
        pstUsrRegCfg->au32GbGain[i] = ((HI_U32)gb_gain1 << 10) + gb_gain0;
        pstUsrRegCfg->au32BGain[i]  = ((HI_U32)b_gain1  << 10) + b_gain0;
    }

    return;
}

static HI_S32 FeLscImageResWrite(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    ISP_FeLSC_S *pstFeLsc   = HI_NULL;

    FeLSC_GET_CTX(ViPipe, pstFeLsc);
    ISP_CHECK_POINTER(pstFeLsc);

    FeLscImageSize(ViPipe, pstFeLsc);

    FeLsc_Usr_Fw(ViPipe, pstFeLsc, &pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.stUsrRegCfg);

    pstRegCfg->cfg_key.bit1FeLscCfg = 1;

    return HI_SUCCESS;
}

static HI_VOID ISP_FeLscWdrModeSet(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    pstRegCfg->cfg_key.bit1FeLscCfg = 1;
    pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.stStaticRegCfg.bResh   = HI_TRUE;
    pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.stUsrRegCfg.bResh      = HI_TRUE;
    pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.stUsrRegCfg.bLutUpdate = HI_TRUE;

    return;
}

HI_S32 ISP_FeLscInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    s32Ret = FeLscCtxInit(ViPipe);

    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = FeLscInitialize(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    FeLscRegsInitialize(ViPipe, pstRegCfg);
    FeLscExtRegsInitialize(ViPipe);

    return HI_SUCCESS;
}

static HI_BOOL __inline CheckFeLscOpen(ISP_FeLSC_S *pstFeLsc)
{
    return (pstFeLsc->bFeLscEn == HI_TRUE);
}

HI_S32 ISP_FeLscRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ISP_FeLSC_S *pstFeLsc = HI_NULL;
    isp_usr_ctx *pstIspCtx  = HI_NULL;
    isp_reg_cfg *pstRegCfg  = (isp_reg_cfg *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    FeLSC_GET_CTX(ViPipe, pstFeLsc);
    ISP_CHECK_POINTER(pstFeLsc);

    /* calculate every two interrupts */
    if ((pstIspCtx->frame_cnt % 2 != 0) && (pstIspCtx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    pstFeLsc->bFeLscEn = hi_ext_system_isp_mesh_shading_enable_read(ViPipe);

    pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.bLscEn = pstFeLsc->bFeLscEn;
    pstRegCfg->cfg_key.bit1FeLscCfg = 1;

    if (!CheckFeLscOpen(pstFeLsc)) {
        return HI_SUCCESS;
    }

    FeLscReadExtRegs(ViPipe);

    if (pstFeLsc->bLscCoefUpdata) {
        pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.stUsrRegCfg.bResh      = HI_TRUE;
        pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.stUsrRegCfg.u16MeshStr = pstFeLsc->u16MeshStrength;
        pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.stUsrRegCfg.u16Weight  = pstFeLsc->u16MeshWeight;
    }

    if (pstFeLsc->bLutUpdate) {
        FeLsc_Usr_Fw(ViPipe, pstFeLsc, &pstRegCfg->alg_reg_cfg[0].stFeLscRegCfg.stUsrRegCfg);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_FeLscCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_FeLscWdrModeSet(ViPipe, (HI_VOID *)&pRegCfg->reg_cfg);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            FeLscImageResWrite(ViPipe, &pRegCfg->reg_cfg);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_FeLscExit(VI_PIPE ViPipe)
{
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    pRegCfg->reg_cfg.alg_reg_cfg[0].stFeLscRegCfg.bLscEn = HI_FALSE;
    pRegCfg->reg_cfg.cfg_key.bit1FeLscCfg                  = 1;

    FeLscCtxExit(ViPipe);

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_fe_lsc(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_FeLSC;
    pstAlgs->alg_func.pfn_alg_init = ISP_FeLscInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_FeLscRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_FeLscCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_FeLscExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
