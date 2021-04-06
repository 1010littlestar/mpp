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

static const HI_U16  g_au16MeshGainDef[8] = {512, 256, 128, 64, 0, 0, 0, 0};

typedef struct hiISP_LSC {
    HI_BOOL bLscEnable;
    HI_BOOL bLscCoefUpdata;
    HI_BOOL bLutUpdate;

    HI_U8  u8MeshScale;
    HI_U16 u16MeshStrength;
    HI_U16 u16MeshWeight;
    HI_U16 au16FirstPointPosX[ISP_STRIPING_MAX_NUM];
    HI_U16 au16CurWidth[ISP_STRIPING_MAX_NUM];
    HI_U16 u16BlkColStart;
    HI_U16 u16BlkColEnd;

    HI_U16 au16DeltaX[HI_ISP_LSC_GRID_COL - 1];
    HI_U16 au16DeltaY[(HI_ISP_LSC_GRID_ROW - 1) / 2];
    HI_U16 au16InvX[HI_ISP_LSC_GRID_COL - 1];
    HI_U16 au16InvY[(HI_ISP_LSC_GRID_ROW - 1) / 2];
} ISP_LSC_S;

ISP_LSC_S *g_pastLscCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define LSC_GET_CTX(dev, pstCtx)   (pstCtx = g_pastLscCtx[dev])
#define LSC_SET_CTX(dev, pstCtx)   (g_pastLscCtx[dev] = pstCtx)
#define LSC_RESET_CTX(dev)         (g_pastLscCtx[dev] = HI_NULL)

HI_S32 LscCtxInit(VI_PIPE ViPipe)
{
    ISP_LSC_S *pastLscCtx = HI_NULL;

    LSC_GET_CTX(ViPipe, pastLscCtx);

    if (pastLscCtx == HI_NULL) {
        pastLscCtx = (ISP_LSC_S *)ISP_MALLOC(sizeof(ISP_LSC_S));
        if (pastLscCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] LscCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastLscCtx, 0, sizeof(ISP_LSC_S));

    LSC_SET_CTX(ViPipe, pastLscCtx);

    return HI_SUCCESS;
}

HI_VOID LscCtxExit(VI_PIPE ViPipe)
{
    ISP_LSC_S *pastLscCtx = HI_NULL;

    LSC_GET_CTX(ViPipe, pastLscCtx);
    ISP_FREE(pastLscCtx);
    LSC_RESET_CTX(ViPipe);
}

static HI_VOID geometricInvSize(ISP_LSC_S *pstLsc)
{
    HI_S32 i;

    for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1); i++) {
        if (pstLsc->au16DeltaX[i] != 0) {
            pstLsc->au16InvX[i] = (4096 * 1024 / pstLsc->au16DeltaX[i] + 512) >> 10;
        } else {
            pstLsc->au16InvX[i] = 0;
        }
    }

    for (i = 0; i < ((HI_ISP_LSC_GRID_ROW - 1) / 2); i++) {
        if (pstLsc->au16DeltaY[i] != 0) {
            pstLsc->au16InvY[i] = (4096 * 1024 / pstLsc->au16DeltaY[i] + 512) >> 10;
        } else {
            pstLsc->au16InvY[i] = 0;
        }
    }

    return;
}

HI_VOID LscGetLutIndex(HI_U8 u8CurBlk, ISP_LSC_S *pstLsc, ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
    HI_U16 u16Width = pstLsc->au16CurWidth[u8CurBlk] >> 1;
    HI_U16 u16FirstPointPosX = pstLsc->au16FirstPointPosX[u8CurBlk] >> 1;
    HI_U16 u16LastPointPosX;
    HI_U16 u16WidthSumOfBlk;
    HI_U16 u16Dis2Right, u16Dis2Left;
    HI_U16 u16BlkColStart;
    HI_U16 u16BlkColEnd;

    u16BlkColStart      = 0;
    u16BlkColEnd        = 0;
    u16WidthSumOfBlk    = pstLsc->au16DeltaX[0];

    while ((u16FirstPointPosX >= u16WidthSumOfBlk) && (u16FirstPointPosX != 0)) {
        u16BlkColStart++;
        u16WidthSumOfBlk += pstLsc->au16DeltaX[u16BlkColStart];
    }

    u16Dis2Right = u16WidthSumOfBlk - u16FirstPointPosX;
    u16Dis2Left  = pstLsc->au16DeltaX[u16BlkColStart] - u16Dis2Right;

    pstUsrRegCfg->u16WidthOffset = u16Dis2Left;

    u16LastPointPosX = u16FirstPointPosX + u16Width;
    u16BlkColEnd     = u16BlkColStart;
    while (u16LastPointPosX >  u16WidthSumOfBlk) {
        u16BlkColEnd++;
        u16WidthSumOfBlk += pstLsc->au16DeltaX[u16BlkColEnd];
    }
    u16BlkColEnd += 1;

    pstLsc->u16BlkColStart  = u16BlkColStart;
    pstLsc->u16BlkColEnd    = u16BlkColEnd;

    return;
}

static HI_S32 LscGetGainLutInitialize(VI_PIPE ViPipe, ISP_LSC_S *pstLsc, ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
    HI_U16 i, j;
    HI_U16 u16BlkColEnd;
    HI_U16 u16BlkColStart;
    HI_U16 u16IndexOffset;
    HI_U16 u16SrcIndex;
    HI_U32 u32DefGain;
    hi_isp_cmos_default   *sns_dft  = HI_NULL;
    const hi_isp_cmos_lsc *cmos_lsc = HI_NULL;

    isp_sensor_get_default(ViPipe,&sns_dft);

    u16BlkColEnd   = pstLsc->u16BlkColEnd;
    u16BlkColStart = pstLsc->u16BlkColStart;

    if (sns_dft->key.bit1_lsc) {
        ISP_CHECK_POINTER(sns_dft->lsc);

        cmos_lsc = sns_dft->lsc;

        for (j = 0; j < HI_ISP_LSC_GRID_ROW; j++) {
            for (i = 0; i <= (u16BlkColEnd - u16BlkColStart); i++) {
                u16IndexOffset = j * HI_ISP_LSC_GRID_COL;
                u16SrcIndex    = u16IndexOffset  + u16BlkColStart + i;

                pstUsrRegCfg->au32RGain[u16IndexOffset + i]  = ((HI_U32)cmos_lsc->lsc_calib_table[1].r_gain[u16SrcIndex]  << 10) + \
                                                               cmos_lsc->lsc_calib_table[0].r_gain[u16SrcIndex];
                pstUsrRegCfg->au32GrGain[u16IndexOffset + i] = ((HI_U32)cmos_lsc->lsc_calib_table[1].gr_gain[u16SrcIndex] << 10) + \
                                                               cmos_lsc->lsc_calib_table[0].gr_gain[u16SrcIndex];
                pstUsrRegCfg->au32GbGain[u16IndexOffset + i] = ((HI_U32)cmos_lsc->lsc_calib_table[1].gb_gain[u16SrcIndex] << 10) + \
                                                               cmos_lsc->lsc_calib_table[0].gb_gain[u16SrcIndex];
                pstUsrRegCfg->au32BGain[u16IndexOffset + i]  = ((HI_U32)cmos_lsc->lsc_calib_table[1].b_gain[u16SrcIndex]  << 10) + \
                                                               cmos_lsc->lsc_calib_table[0].b_gain[u16SrcIndex];
            }
        }
    } else {
        u32DefGain = ((HI_U32)g_au16MeshGainDef[pstLsc->u8MeshScale] << 10) + g_au16MeshGainDef[pstLsc->u8MeshScale];

        for (j = 0; j < HI_ISP_LSC_GRID_ROW; j++) {
            for (i = 0; i <= (u16BlkColEnd - u16BlkColStart); i++) {
                u16IndexOffset = j * HI_ISP_LSC_GRID_COL;

                pstUsrRegCfg->au32RGain[u16IndexOffset + i]   = u32DefGain;
                pstUsrRegCfg->au32GrGain[u16IndexOffset + i]  = u32DefGain;
                pstUsrRegCfg->au32GbGain[u16IndexOffset + i]  = u32DefGain;
                pstUsrRegCfg->au32BGain[u16IndexOffset + i]   = u32DefGain;
            }
        }
    }

    for (i = 0; i < (u16BlkColEnd - u16BlkColStart); i++) {
        pstUsrRegCfg->au16DeltaX[i] = pstLsc->au16DeltaX[u16BlkColStart + i];
        pstUsrRegCfg->au16InvX[i]   = pstLsc->au16InvX[u16BlkColStart   + i];
    }

    for (i = 0; i < (HI_ISP_LSC_GRID_ROW - 1) / 2; i++) {
        pstUsrRegCfg->au16DeltaY[i] = pstLsc->au16DeltaY[i];
        pstUsrRegCfg->au16InvY[i]   = pstLsc->au16InvY[i];
    }

    return HI_SUCCESS;
}

static HI_S32 LscGetGainLut(VI_PIPE ViPipe, ISP_LSC_S *pstLsc, ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
    HI_U16 i, j;
    HI_U16 u16BlkColEnd;
    HI_U16 u16BlkColStart;
    HI_U16 u16IndexOffset;
    HI_U16 u16SrcIndex;
    HI_U16 r_gain0, r_gain1, gr_gain0, gr_gain1, gb_gain0, gb_gain1, b_gain0, b_gain1;

    u16BlkColEnd   = pstLsc->u16BlkColEnd;
    u16BlkColStart = pstLsc->u16BlkColStart;

    for (j = 0; j < HI_ISP_LSC_GRID_ROW; j++) {
        for (i = 0; i <= (u16BlkColEnd - u16BlkColStart); i++) {
            u16IndexOffset = j * HI_ISP_LSC_GRID_COL;
            u16SrcIndex    = u16IndexOffset  + u16BlkColStart + i;

            r_gain0  = hi_ext_system_isp_mesh_shading_r_gain0_read(ViPipe, u16SrcIndex);
            r_gain1  = hi_ext_system_isp_mesh_shading_r_gain1_read(ViPipe, u16SrcIndex);

            gr_gain0 = hi_ext_system_isp_mesh_shading_gr_gain0_read(ViPipe, u16SrcIndex);
            gr_gain1 = hi_ext_system_isp_mesh_shading_gr_gain1_read(ViPipe, u16SrcIndex);

            gb_gain0 = hi_ext_system_isp_mesh_shading_gb_gain0_read(ViPipe, u16SrcIndex);
            gb_gain1 = hi_ext_system_isp_mesh_shading_gb_gain1_read(ViPipe, u16SrcIndex);

            b_gain0  = hi_ext_system_isp_mesh_shading_b_gain0_read(ViPipe, u16SrcIndex);
            b_gain1  = hi_ext_system_isp_mesh_shading_b_gain1_read(ViPipe, u16SrcIndex);

            pstUsrRegCfg->au32RGain[u16IndexOffset + i]  = ((HI_U32)r_gain1  << 10) + r_gain0;
            pstUsrRegCfg->au32GrGain[u16IndexOffset + i] = ((HI_U32)gr_gain1 << 10) + gr_gain0;
            pstUsrRegCfg->au32GbGain[u16IndexOffset + i] = ((HI_U32)gb_gain1 << 10) + gb_gain0;
            pstUsrRegCfg->au32BGain[u16IndexOffset + i]  = ((HI_U32)b_gain1  << 10) + b_gain0;
        }
    }

    for (i = 0; i < (u16BlkColEnd - u16BlkColStart); i++) {
        pstUsrRegCfg->au16DeltaX[i] = pstLsc->au16DeltaX[u16BlkColStart + i];
        pstUsrRegCfg->au16InvX[i]   = pstLsc->au16InvX[u16BlkColStart   + i];
    }

    for (i = 0; i < (HI_ISP_LSC_GRID_ROW - 1) / 2; i++) {
        pstUsrRegCfg->au16DeltaY[i] = pstLsc->au16DeltaY[i];
        pstUsrRegCfg->au16InvY[i]   = pstLsc->au16InvY[i];
    }

    return HI_SUCCESS;
}


static HI_VOID LscStaticRegsInitialize(HI_U8 i, VI_PIPE ViPipe, ISP_LSC_STATIC_CFG_S *pstLscStaticRegCfg)
{
    pstLscStaticRegCfg->u8WinNumH   = HI_ISP_LSC_GRID_COL - 1;
    pstLscStaticRegCfg->u8WinNumV   = HI_ISP_LSC_GRID_ROW - 1;

    pstLscStaticRegCfg->bStaticResh = HI_TRUE;

    return;
}

static HI_VOID LscUsrRegsMultiInitialize(HI_U8 u8CurBlk, VI_PIPE ViPipe, ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
    ISP_LSC_S  *pstLsc    = HI_NULL;

    LSC_GET_CTX(ViPipe, pstLsc);
    ISP_CHECK_POINTER_VOID(pstLsc);

    pstUsrRegCfg->u16MeshStr  = pstLsc->u16MeshStrength;
    pstUsrRegCfg->u16Weight   = pstLsc->u16MeshWeight;
    pstUsrRegCfg->u8MeshScale = pstLsc->u8MeshScale;

    LscGetLutIndex(u8CurBlk, pstLsc, pstUsrRegCfg);

    LscGetGainLutInitialize(ViPipe, pstLsc, pstUsrRegCfg);

    pstUsrRegCfg->u32UpdateIndex = 1;

    pstUsrRegCfg->bLutUpdate     = HI_TRUE;
    pstUsrRegCfg->bCoefUpdate    = HI_TRUE;

    return;
}

static HI_VOID LscUsrRegsSingleInitialize(VI_PIPE ViPipe, ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
    HI_U16     i;
    HI_U32     u32DefGain;
    ISP_LSC_S  *pstLsc    = HI_NULL;

    hi_isp_cmos_default   *sns_dft  = HI_NULL;
    const hi_isp_cmos_lsc *cmos_lsc = HI_NULL;

    isp_sensor_get_default(ViPipe, &sns_dft);
    LSC_GET_CTX(ViPipe, pstLsc);
    ISP_CHECK_POINTER_VOID(pstLsc);

    pstUsrRegCfg->u16MeshStr     = pstLsc->u16MeshStrength;
    pstUsrRegCfg->u16Weight      = pstLsc->u16MeshWeight;
    pstUsrRegCfg->u8MeshScale    = pstLsc->u8MeshScale;
    pstUsrRegCfg->u16WidthOffset = 0;

    memcpy(pstUsrRegCfg->au16DeltaX, pstLsc->au16DeltaX, sizeof(HI_U16) * (HI_ISP_LSC_GRID_COL - 1));
    memcpy(pstUsrRegCfg->au16InvX,   pstLsc->au16InvX,   sizeof(HI_U16) * (HI_ISP_LSC_GRID_COL - 1));

    memcpy(pstUsrRegCfg->au16DeltaY, pstLsc->au16DeltaY, sizeof(HI_U16) * (HI_ISP_LSC_GRID_ROW - 1) / 2);
    memcpy(pstUsrRegCfg->au16InvY,   pstLsc->au16InvY,   sizeof(HI_U16) * (HI_ISP_LSC_GRID_ROW - 1) / 2);

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
        u32DefGain = ((HI_U32)g_au16MeshGainDef[pstLsc->u8MeshScale] << 10) + g_au16MeshGainDef[pstLsc->u8MeshScale];

        for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
            pstUsrRegCfg->au32RGain[i]  = u32DefGain;
            pstUsrRegCfg->au32GrGain[i] = u32DefGain;
            pstUsrRegCfg->au32GbGain[i] = u32DefGain;
            pstUsrRegCfg->au32BGain[i]  = u32DefGain;
        }
    }

    pstUsrRegCfg->u32UpdateIndex = 1;

    pstUsrRegCfg->bLutUpdate     = HI_TRUE;
    pstUsrRegCfg->bCoefUpdate    = HI_TRUE;
}

static HI_VOID LscRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_S32 i;
    ISP_LSC_S *pstLsc    = HI_NULL;

    LSC_GET_CTX(ViPipe, pstLsc);
    ISP_CHECK_POINTER_VOID(pstLsc);

    if (pstRegCfg->cfg_num == 1) {
        LscStaticRegsInitialize(0, ViPipe, &pstRegCfg->alg_reg_cfg[0].stLscRegCfg.stStaticRegCfg);
        LscUsrRegsSingleInitialize(ViPipe, &pstRegCfg->alg_reg_cfg[0].stLscRegCfg.stUsrRegCfg);

        pstRegCfg->alg_reg_cfg[0].stLscRegCfg.bLscEn     = pstLsc->bLscEnable;
    } else {
        for (i = 0; i < pstRegCfg->cfg_num; i++) {
            LscStaticRegsInitialize(i, ViPipe, &pstRegCfg->alg_reg_cfg[i].stLscRegCfg.stStaticRegCfg);
            LscUsrRegsMultiInitialize(i, ViPipe, &pstRegCfg->alg_reg_cfg[i].stLscRegCfg.stUsrRegCfg);

            pstRegCfg->alg_reg_cfg[i].stLscRegCfg.bLscEn     = pstLsc->bLscEnable;
        }
    }

    pstRegCfg->cfg_key.bit1LscCfg    = 1;

    return;
}

static HI_S32 LscExtRegsInitialize(VI_PIPE ViPipe)
{
    HI_U16 i;
    ISP_LSC_S            *pstLsc     = HI_NULL;
    hi_isp_cmos_default  *sns_dft  = HI_NULL;
    const hi_isp_cmos_lsc *cmos_lsc = HI_NULL;

    isp_sensor_get_default(ViPipe, &sns_dft);
    LSC_GET_CTX(ViPipe, pstLsc);
    ISP_CHECK_POINTER(pstLsc);

    hi_ext_system_isp_mesh_shading_enable_write(ViPipe, pstLsc->bLscEnable);
    hi_ext_system_isp_mesh_shading_attr_updata_write(ViPipe, HI_FALSE);
    hi_ext_system_isp_mesh_shading_lut_attr_updata_write(ViPipe, HI_FALSE);
    hi_ext_system_isp_mesh_shading_mesh_strength_write(ViPipe, pstLsc->u16MeshStrength);
    hi_ext_system_isp_mesh_shading_blendratio_write(ViPipe, pstLsc->u16MeshWeight);
    hi_ext_system_isp_mesh_shading_mesh_scale_write(ViPipe, pstLsc->u8MeshScale);

    for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1) / 2; i++) {
        hi_ext_system_isp_mesh_shading_xgrid_write(ViPipe, i, pstLsc->au16DeltaX[i]);
    }

    for (i = 0; i < (HI_ISP_LSC_GRID_ROW - 1) / 2; i++) {
        hi_ext_system_isp_mesh_shading_ygrid_write(ViPipe, i, pstLsc->au16DeltaY[i]);
    }

    if (sns_dft->key.bit1_lsc) {
        ISP_CHECK_POINTER(sns_dft->lsc);

        cmos_lsc = sns_dft->lsc;

        for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
            hi_ext_system_isp_mesh_shading_r_gain0_write(ViPipe,  i, cmos_lsc->lsc_calib_table[0].r_gain[i]);
            hi_ext_system_isp_mesh_shading_gr_gain0_write(ViPipe, i, cmos_lsc->lsc_calib_table[0].gr_gain[i]);
            hi_ext_system_isp_mesh_shading_gb_gain0_write(ViPipe, i, cmos_lsc->lsc_calib_table[0].gb_gain[i]);
            hi_ext_system_isp_mesh_shading_b_gain0_write(ViPipe,  i, cmos_lsc->lsc_calib_table[0].b_gain[i]);

            hi_ext_system_isp_mesh_shading_r_gain1_write(ViPipe,  i, cmos_lsc->lsc_calib_table[1].r_gain[i]);
            hi_ext_system_isp_mesh_shading_gr_gain1_write(ViPipe, i, cmos_lsc->lsc_calib_table[1].gr_gain[i]);
            hi_ext_system_isp_mesh_shading_gb_gain1_write(ViPipe, i, cmos_lsc->lsc_calib_table[1].gb_gain[i]);
            hi_ext_system_isp_mesh_shading_b_gain1_write(ViPipe,  i, cmos_lsc->lsc_calib_table[1].b_gain[i]);
        }
    } else {
        for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
            hi_ext_system_isp_mesh_shading_r_gain0_write(ViPipe,  i, g_au16MeshGainDef[pstLsc->u8MeshScale]);
            hi_ext_system_isp_mesh_shading_gr_gain0_write(ViPipe, i, g_au16MeshGainDef[pstLsc->u8MeshScale]);
            hi_ext_system_isp_mesh_shading_gb_gain0_write(ViPipe, i, g_au16MeshGainDef[pstLsc->u8MeshScale]);
            hi_ext_system_isp_mesh_shading_b_gain0_write(ViPipe,  i, g_au16MeshGainDef[pstLsc->u8MeshScale]);

            hi_ext_system_isp_mesh_shading_r_gain1_write(ViPipe,  i, g_au16MeshGainDef[pstLsc->u8MeshScale]);
            hi_ext_system_isp_mesh_shading_gr_gain1_write(ViPipe, i, g_au16MeshGainDef[pstLsc->u8MeshScale]);
            hi_ext_system_isp_mesh_shading_gb_gain1_write(ViPipe, i, g_au16MeshGainDef[pstLsc->u8MeshScale]);
            hi_ext_system_isp_mesh_shading_b_gain1_write(ViPipe,  i, g_au16MeshGainDef[pstLsc->u8MeshScale]);
        }
    }

    return HI_SUCCESS;
}

static HI_VOID BNRLscExtRegsInitialize(VI_PIPE ViPipe)
{
    HI_U16 i;
    for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
        hi_ext_system_isp_bnr_shading_r_gain_write(ViPipe, i, 4096);
        hi_ext_system_isp_bnr_shading_gr_gain_write(ViPipe, i, 4096);
        hi_ext_system_isp_bnr_shading_gb_gain_write(ViPipe, i, 4096);
        hi_ext_system_isp_bnr_shading_b_gain_write(ViPipe, i, 4096);
    }
    return;
}

static HI_S32 LscReadExtRegs(VI_PIPE ViPipe)
{
    HI_U16  i;
    ISP_LSC_S *pstLsc    = HI_NULL;

    LSC_GET_CTX(ViPipe, pstLsc);
    ISP_CHECK_POINTER(pstLsc);

    pstLsc->bLscCoefUpdata  = hi_ext_system_isp_mesh_shading_attr_updata_read(ViPipe);
    hi_ext_system_isp_mesh_shading_attr_updata_write(ViPipe, HI_FALSE);

    if (pstLsc->bLscCoefUpdata) {
        pstLsc->u16MeshStrength = hi_ext_system_isp_mesh_shading_mesh_strength_read(ViPipe);
        pstLsc->u16MeshWeight   = hi_ext_system_isp_mesh_shading_blendratio_read(ViPipe);
    }

    pstLsc->bLutUpdate      = hi_ext_system_isp_mesh_shading_lut_attr_updata_read(ViPipe);
    hi_ext_system_isp_mesh_shading_lut_attr_updata_write(ViPipe, HI_FALSE);

    if (pstLsc->bLutUpdate) {
        pstLsc->u8MeshScale = hi_ext_system_isp_mesh_shading_mesh_scale_read(ViPipe);

        for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1) / 2; i++) {
            pstLsc->au16DeltaX[i] = hi_ext_system_isp_mesh_shading_xgrid_read(ViPipe, i);
        }

        for (i = (HI_ISP_LSC_GRID_COL - 1) / 2; i < HI_ISP_LSC_GRID_COL - 1; i++) {
            pstLsc->au16DeltaX[i] = pstLsc->au16DeltaX[HI_ISP_LSC_GRID_COL - 2 - i];
        }

        for (i = 0; i < (HI_ISP_LSC_GRID_ROW - 1) / 2; i++) {
            pstLsc->au16DeltaY[i] = hi_ext_system_isp_mesh_shading_ygrid_read(ViPipe, i);
        }

        geometricInvSize(pstLsc);
    }

    return HI_SUCCESS;
}

static HI_VOID geometricGridSizeLsc(HI_U16 *pu16Delta, HI_U16 *pu16Inv, HI_U16 u16Length, HI_U16 u16GridSize)
{
    HI_U16 i, sum;
    HI_U16 u16HalfGridSize;
    HI_U16 diff;
    HI_U16 *pu16TmpStep;
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

static HI_VOID LscImageSize(VI_PIPE ViPipe, HI_U8 u8BlkNum, ISP_LSC_S  *pstLsc)
{
    HI_U16     i;
    isp_rect stBlockRect;
    isp_usr_ctx  *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    geometricGridSizeLsc(pstLsc->au16DeltaX, pstLsc->au16InvX, pstIspCtx->block_attr.frame_rect.width  / 2, HI_ISP_LSC_GRID_COL);
    geometricGridSizeLsc(pstLsc->au16DeltaY, pstLsc->au16InvY, pstIspCtx->block_attr.frame_rect.height / 2, HI_ISP_LSC_GRID_ROW);

    for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1) / 2; i++) {
        pstLsc->au16DeltaX[HI_ISP_LSC_GRID_COL - 2 - i] = pstLsc->au16DeltaX[i];
        pstLsc->au16InvX[HI_ISP_LSC_GRID_COL - 2 - i]   = pstLsc->au16InvX[i];
    }

    for (i = 0; i < u8BlkNum; i++) {
        ISP_GetBlockRect(&stBlockRect, &pstIspCtx->block_attr, i);

        pstLsc->au16CurWidth[i]       = stBlockRect.width;
        pstLsc->au16FirstPointPosX[i] = stBlockRect.x;
    }

    return;
}

static HI_S32 LscCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_lsc *cmos_lsc)
{
    HI_U16 i;

    if (cmos_lsc->mesh_scale > 7) {
        ISP_ERR_TRACE("Invalid u8MeshScale %d!\n", cmos_lsc->mesh_scale);
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

static HI_S32 LscInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_S32 s32Ret;
    ISP_LSC_S  *pstLsc    = HI_NULL;
    isp_usr_ctx  *pstIspCtx = HI_NULL;
    hi_isp_cmos_default *sns_dft  = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    LSC_GET_CTX(ViPipe, pstLsc);
    isp_sensor_get_default(ViPipe, &sns_dft);

    ISP_CHECK_POINTER(pstLsc);

    pstLsc->u16MeshStrength = HI_ISP_LSC_DEFAULT_MESH_STRENGTH;
    pstLsc->u16MeshWeight   = HI_ISP_LSC_DEFAULT_WEIGHT;

    if (sns_dft->key.bit1_lsc) {
        ISP_CHECK_POINTER(sns_dft->lsc);

        s32Ret = LscCheckCmosParam(ViPipe, sns_dft->lsc);
        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        pstLsc->u8MeshScale = sns_dft->lsc->mesh_scale;
    } else {
        pstLsc->u8MeshScale = HI_ISP_LSC_DEFAULT_MESH_SCALE;
    }

    LscImageSize(ViPipe, pstRegCfg->cfg_num, pstLsc);

    if (pstIspCtx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR) {
        pstLsc->bLscEnable = HI_FALSE;
    } else {
        pstLsc->bLscEnable = HI_TRUE;
    }

    return HI_SUCCESS;
}

static HI_VOID Lsc_Usr_Fw(VI_PIPE ViPipe, HI_U8 u8CurBlk, ISP_LSC_S *pstLsc, ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
    LscGetLutIndex(u8CurBlk, pstLsc, pstUsrRegCfg);

    LscGetGainLut(ViPipe, pstLsc, pstUsrRegCfg);

    pstUsrRegCfg->u8MeshScale     = pstLsc->u8MeshScale;
    pstUsrRegCfg->u32UpdateIndex += 1;
    pstUsrRegCfg->bLutUpdate      = HI_TRUE;

    return;
}

static HI_S32  Lsc_Usr_Fw_Normal(VI_PIPE ViPipe, ISP_LSC_S *pstLsc, ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
    HI_U16 i;
    HI_U16 r_gain0, r_gain1, gr_gain0, gr_gain1, gb_gain0, gb_gain1, b_gain0, b_gain1;

    pstUsrRegCfg->u16WidthOffset  = 0;
    pstUsrRegCfg->u8MeshScale     = pstLsc->u8MeshScale;
    pstUsrRegCfg->u32UpdateIndex += 1;
    pstUsrRegCfg->bLutUpdate      = HI_TRUE;

    memcpy(pstUsrRegCfg->au16DeltaX, pstLsc->au16DeltaX, sizeof(HI_U16) * (HI_ISP_LSC_GRID_COL - 1));
    memcpy(pstUsrRegCfg->au16InvX,   pstLsc->au16InvX,   sizeof(HI_U16) * (HI_ISP_LSC_GRID_COL - 1));

    memcpy(pstUsrRegCfg->au16DeltaY, pstLsc->au16DeltaY, sizeof(HI_U16) * (HI_ISP_LSC_GRID_ROW - 1) / 2);
    memcpy(pstUsrRegCfg->au16InvY,   pstLsc->au16InvY,   sizeof(HI_U16) * (HI_ISP_LSC_GRID_ROW - 1) / 2);

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

    return HI_SUCCESS;
}

static HI_BOOL __inline CheckLscOpen(ISP_LSC_S *pstLsc)
{
    return (pstLsc->bLscEnable == HI_TRUE);
}

static HI_S32 LscImageResWrite(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    ISP_LSC_S  *pstLsc    = HI_NULL;

    LSC_GET_CTX(ViPipe, pstLsc);
    ISP_CHECK_POINTER(pstLsc);

    LscImageSize(ViPipe, pstRegCfg->cfg_num, pstLsc);

    LscExtRegsInitialize(ViPipe);

    if (pstRegCfg->cfg_num == 1) {
        Lsc_Usr_Fw_Normal(ViPipe, pstLsc, &pstRegCfg->alg_reg_cfg[0].stLscRegCfg.stUsrRegCfg);
    } else {
        for (i = 0; i < pstRegCfg->cfg_num; i++) {
            Lsc_Usr_Fw(ViPipe, i, pstLsc, &pstRegCfg->alg_reg_cfg[i].stLscRegCfg.stUsrRegCfg);
        }
    }

    pstRegCfg->cfg_key.bit1LscCfg = 1;

    return HI_SUCCESS;
}

HI_S32 ISP_LscInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    s32Ret = LscCtxInit(ViPipe);

    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = LscInitialize(ViPipe, pstRegCfg);
    if (s32Ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    LscRegsInitialize(ViPipe, pstRegCfg);
    LscExtRegsInitialize(ViPipe);
    BNRLscExtRegsInitialize(ViPipe);

    return HI_SUCCESS;
}

static HI_VOID ISP_LscWdrModeSet(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    return;
}

HI_S32 ISP_LscRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                  HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    ISP_LSC_S *pstLsc    = HI_NULL;
    isp_reg_cfg *pstRegCfg  = (isp_reg_cfg *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    LSC_GET_CTX(ViPipe, pstLsc);
    ISP_CHECK_POINTER(pstLsc);

    if (pstIspCtx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    pstLsc->bLscEnable = hi_ext_system_isp_mesh_shading_enable_read(ViPipe);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stLscRegCfg.bLscEn = pstLsc->bLscEnable;
    }

    pstRegCfg->cfg_key.bit1LscCfg = 1;

    /* check hardware setting */
    if (!CheckLscOpen(pstLsc)) {
        return HI_SUCCESS;
    }

    LscReadExtRegs(ViPipe);

    if (pstLsc->bLscCoefUpdata) {
        for (i = 0; i < pstRegCfg->cfg_num; i++) {
            pstRegCfg->alg_reg_cfg[i].stLscRegCfg.stUsrRegCfg.bCoefUpdate   = HI_TRUE;
            pstRegCfg->alg_reg_cfg[i].stLscRegCfg.stUsrRegCfg.u16MeshStr    = pstLsc->u16MeshStrength;
            pstRegCfg->alg_reg_cfg[i].stLscRegCfg.stUsrRegCfg.u16Weight     = pstLsc->u16MeshWeight;
        }
    }

    if (pstLsc->bLutUpdate) {
        if (pstRegCfg->cfg_num == 1) {
            Lsc_Usr_Fw_Normal(ViPipe, pstLsc, &pstRegCfg->alg_reg_cfg[0].stLscRegCfg.stUsrRegCfg);
        } else {
            for (i = 0; i < pstRegCfg->cfg_num; i++) {
                Lsc_Usr_Fw(ViPipe, i, pstLsc, &pstRegCfg->alg_reg_cfg[i].stLscRegCfg.stUsrRegCfg);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_LscCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_LscWdrModeSet(ViPipe, (HI_VOID *)&pRegCfg->reg_cfg);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            LscImageResWrite(ViPipe, &pRegCfg->reg_cfg);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_LscExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stLscRegCfg.bLscEn = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1LscCfg = 1;

    LscCtxExit(ViPipe);

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_lsc(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_LSC;
    pstAlgs->alg_func.pfn_alg_init = ISP_LscInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_LscRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_LscCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_LscExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
