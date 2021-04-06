/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include "isp_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_ext_config.h"


#include "hi_comm_vi.h"
#include "mpi_vi.h"
#include "vi_ext.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define FPN_OVERFLOWTHR         0x7C0
#define ISP_FPN_MAX_O           0xFFF
#define FPN_OVERFLOWTHR_OFF     0x3FFF
#define FPN_CHN_NUM             4
#define ISP_FPN_MODE_CORRECTION 0x0
#define ISP_FPN_MODE_CALIBRATE  0x1
#define ISP_FPN_CLIP(min,max,x)         ( (x)<= (min) ? (min) : ((x)>(max)?(max):(x)) )

typedef struct hiISP_FPN_S {
    HI_S32  s32Iso[ISP_MAX_PIPE_NUM];
    HI_U32  u32IspIntCnt;
} ISP_FPN_S;

typedef enum hiISP_SENSOR_BIT_WIDTH_E {
    ISP_SENSOR_8BIT  = 8,
    ISP_SENSOR_10BIT = 10,
    ISP_SENSOR_12BIT = 12,
    ISP_SENSOR_14BIT = 14,
    ISP_SENSOR_16BIT = 16,
    ISP_SENSOR_32BIT = 32,
    ISP_SENSOR_BUTT
} ISP_SENSOR_BIT_WIDTH_E;

ISP_FPN_S g_astFpnCtx[ISP_MAX_PIPE_NUM] = {0};
#define FPN_GET_CTX(dev, pstFpnCtx)   pstFpnCtx = &g_astFpnCtx[dev]

HI_S32 isp_set_calibrate_attr(VI_PIPE ViPipe, hi_isp_fpn_calibrate_attr *pstCalibrate)
{
    vi_fpn_attr stFpnAttr;
    HI_S32 s32Ret;

    stFpnAttr.fpn_work_mode = FPN_MODE_CALIBRATE;
    memcpy(&stFpnAttr.calibrate_attr, pstCalibrate, sizeof(hi_isp_fpn_calibrate_attr));

    hi_ext_system_fpn_cablibrate_enable_write(ViPipe, 1);

    s32Ret = mpi_vi_set_fpn_attr(ViPipe, &stFpnAttr);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    memcpy(pstCalibrate, &stFpnAttr.calibrate_attr, sizeof(hi_isp_fpn_calibrate_attr));
    pstCalibrate->fpn_cali_frame.iso = hi_ext_system_fpn_sensor_iso_read(ViPipe);
    hi_ext_system_fpn_cablibrate_enable_write(ViPipe, 0);

    return HI_SUCCESS;
}

HI_U32 ISP_Fpn_GetStrength(HI_U32 u32Iso, HI_U32 u32CalibrateIso)
{
    HI_U32 u32Strength;

    u32Strength = 256 * u32Iso / DIV_0_TO_1(u32CalibrateIso);

    return u32Strength;
}

HI_S32 isp_set_correction_attr(VI_PIPE ViPipe, const hi_isp_fpn_attr *pstCorrection)
{
    HI_S32 s32Ret;
    vi_fpn_attr stFpnAttr;

    stFpnAttr.fpn_work_mode = FPN_MODE_CORRECTION;
    memcpy(&stFpnAttr.correction_attr, pstCorrection, sizeof(hi_isp_fpn_attr));

    s32Ret = mpi_vi_set_fpn_attr(ViPipe, &stFpnAttr);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    hi_ext_system_manual_fpn_opmode_write(ViPipe, pstCorrection->op_type);
    hi_ext_system_manual_fpn_Enable_write(ViPipe, pstCorrection->enable);
    hi_ext_system_manual_fpn_iso_write(ViPipe, pstCorrection->fpn_frm_info.iso);
    hi_ext_system_manual_fpn_Gain_write(ViPipe, pstCorrection->manual_attr.strength);

    return s32Ret;
}

HI_S32 isp_get_correction_attr(VI_PIPE ViPipe, hi_isp_fpn_attr *pstCorrection)
{
    HI_U8 u8BlkDev = 0;
    HI_U8 index = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    vi_fpn_attr stTempViFpnAttr;

    s32Ret = mpi_vi_get_fpn_attr(ViPipe, &stTempViFpnAttr);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    memcpy(pstCorrection, &stTempViFpnAttr.correction_attr, sizeof(hi_isp_fpn_attr));
    pstCorrection->auto_attr.strength = (isp_fpn_corr_gainoffset_read(ViPipe, u8BlkDev, index) >> 16);

    return HI_SUCCESS;
}

static HI_VOID FPNExtRegsDefault(VI_PIPE ViPipe)
{
    hi_ext_system_fpn_sensor_iso_write(ViPipe, HI_EXT_SYSTEM_FPN_SENSOR_ISO_DEFAULT);
    hi_ext_system_manual_fpn_iso_write(ViPipe, HI_EXT_SYSTEM_FPN_MANU_ISO_DEFAULT);

    hi_ext_system_manual_fpn_CorrCfg_write(ViPipe, HI_EXT_SYSTEM_FPN_MANU_CORRCFG_DEFAULT);
    hi_ext_system_manual_fpn_Gain_write(ViPipe, HI_EXT_SYSTEM_FPN_STRENGTH_DEFAULT);
    hi_ext_system_manual_fpn_opmode_write(ViPipe, HI_EXT_SYSTEM_FPN_OPMODE_DEFAULT);
    hi_ext_system_manual_fpn_update_write(ViPipe, HI_EXT_SYSTEM_FPN_MANU_UPDATE_DEFAULT);

    hi_ext_system_manual_fpn_Type_write(ViPipe, 0);
    hi_ext_system_manual_fpn_Offset_write(ViPipe, 0);
    hi_ext_system_manual_fpn_Enable_write(ViPipe, 0);
    hi_ext_system_manual_fpn_Pixelformat_write(ViPipe, 0);
    hi_ext_system_fpn_cablibrate_enable_write(ViPipe, 0);

    return;
}

static HI_VOID FPNRegsDefault(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    ISP_FPN_S        *pstFpnCtx    = HI_NULL;

    FPN_GET_CTX(ViPipe, pstFpnCtx);
    for (i = 0; i < 4; i++) {
        pstFpnCtx->s32Iso[i] = 0;
    }

    pstFpnCtx->u32IspIntCnt = 1;

    pstRegCfg->cfg_key.bit1FpnCfg = 1;

    return;
}

static HI_VOID FPNExtRegsInitialize(VI_PIPE ViPipe)
{
    return;
}

static HI_VOID FPNRegsInitialize(VI_PIPE ViPipe)
{
    return;
}

static HI_S32 FPNReadExtregs(VI_PIPE ViPipe)
{
    return 0;
}

HI_S32 FPNUpdateExtRegs(VI_PIPE ViPipe)
{
    HI_U8 i = 0;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_reg_cfg_attr *pRegCfg = HI_NULL;
    HI_U8 u8FpnOpMode;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    hi_ext_system_fpn_sensor_iso_write(ViPipe, pstIspCtx->linkage.sensor_iso);
    u8FpnOpMode = hi_ext_system_manual_fpn_opmode_read(ViPipe);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        if (hi_ext_system_fpn_cablibrate_enable_read(ViPipe)) {
            pRegCfg->reg_cfg.alg_reg_cfg[i].stFpnRegCfg.stDynaRegCfg.u32IspFpnCalibCorr = ISP_FPN_MODE_CALIBRATE;
        } else {
            pRegCfg->reg_cfg.alg_reg_cfg[i].stFpnRegCfg.stDynaRegCfg.u32IspFpnCalibCorr = ISP_FPN_MODE_CORRECTION;
        }

        if (IS_WDR_MODE(pstIspCtx->pre_sns_wdr_mode) && (u8FpnOpMode == OP_TYPE_AUTO)) {
            pRegCfg->reg_cfg.alg_reg_cfg[i].stFpnRegCfg.stDynaRegCfg.u32IspFpnOverflowThr = FPN_OVERFLOWTHR;
        } else {
            pRegCfg->reg_cfg.alg_reg_cfg[i].stFpnRegCfg.stDynaRegCfg.u32IspFpnOverflowThr = FPN_OVERFLOWTHR_OFF;
        }
    }

    return HI_SUCCESS;
}


HI_VOID IspSetStrength(VI_PIPE ViPipe)
{
    isp_usr_ctx        *pstIspCtx                 = HI_NULL;
    HI_U32           u32Iso, u32Gain, i;
    HI_U32           u32CalibrateIso;
    HI_U8            u8FpnOpMode, u8FpnEn;
    ISP_FPN_S        *pstFpnCtx                 = HI_NULL;
    isp_reg_cfg_attr     *pRegCfg                   = HI_NULL;
    HI_U8            u8ViSBSModeEn              = 0; /* SBS mode,or not  */

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    FPN_GET_CTX(ViPipe, pstFpnCtx);

    if (ISP_MODE_RUNNING_SIDEBYSIDE == pstIspCtx->block_attr.running_mode ||
        ISP_MODE_RUNNING_STRIPING == pstIspCtx->block_attr.running_mode) {
        u8ViSBSModeEn = 1;
    }

    if (pstIspCtx->frame_cnt % pstFpnCtx->u32IspIntCnt != 0) {
        return;
    }

    u8FpnEn = isp_ext_system_manual_fpn_Enable_read(ViPipe);

    if (!u8FpnEn) {
        return;
    }

    u8FpnOpMode = hi_ext_system_manual_fpn_opmode_read(ViPipe);

    if (u8FpnOpMode == OP_TYPE_MANUAL) {
        for (i = 0; i < FPN_CHN_NUM; i++) {
            pRegCfg->reg_cfg.alg_reg_cfg[0].stFpnRegCfg.stDynaRegCfg.u32IspFpnStrength[i] = isp_ext_system_manual_fpn_Gain_read(ViPipe);

            if (u8ViSBSModeEn) {
                pRegCfg->reg_cfg.alg_reg_cfg[1].stFpnRegCfg.stDynaRegCfg.u32IspFpnStrength[i] = isp_ext_system_manual_fpn_Gain_read(ViPipe);
            }
        }

        pRegCfg->reg_cfg.cfg_key.bit1FpnCfg = 1;

        return;
    }

    u32Iso = hi_ext_system_fpn_sensor_iso_read(ViPipe);
    u32CalibrateIso = hi_ext_system_manual_fpn_iso_read(ViPipe);
    u32Gain = ISP_Fpn_GetStrength(u32Iso, u32CalibrateIso);
    u32Gain = ISP_FPN_CLIP(0, 0x3FF, u32Gain);

    for (i = 0; i < FPN_CHN_NUM; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[0].stFpnRegCfg.stDynaRegCfg.u32IspFpnStrength[i] = u32Gain;

        if (u8ViSBSModeEn) {
            pRegCfg->reg_cfg.alg_reg_cfg[1].stFpnRegCfg.stDynaRegCfg.u32IspFpnStrength[i] = u32Gain;
        }
    }

    pRegCfg->reg_cfg.cfg_key.bit1FpnCfg = 1;
}

HI_S32 ISP_FPNInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    FPNRegsDefault(ViPipe, (isp_reg_cfg *)pRegCfg);
    FPNExtRegsDefault(ViPipe);
    FPNReadExtregs(ViPipe);
    FPNRegsInitialize(ViPipe);
    FPNExtRegsInitialize(ViPipe);

    return HI_SUCCESS;
}

HI_S32 ISP_FPNRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                  HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    FPNUpdateExtRegs(ViPipe);
    IspSetStrength(ViPipe);

    return HI_SUCCESS;
}

HI_S32 FpnProcWrite(VI_PIPE ViPipe, hi_isp_ctrl_proc_write *pstProc)
{
    HI_U8 u8BlkDev = 0; /* BE Num, for SBS mode, BE0 & BE1 is the same FPN config, so just 0 is ok */
    hi_isp_ctrl_proc_write stProcTmp;
    HI_U32 u32Offset = 0;
    HI_U32 u32Strength = 0;

    if ((pstProc->proc_buff == HI_NULL)
        || (pstProc->buff_len == 0)) {
        return HI_FAILURE;
    }

    stProcTmp.proc_buff = pstProc->proc_buff;
    stProcTmp.buff_len = pstProc->buff_len;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "-----FPN CORRECT INFO------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8s" "%7s"     "%9s"       "%8s",
                    "En", "OpType", "Strength", "Offset\n");

    u32Offset = isp_fpn_corr_gainoffset_read(ViPipe, u8BlkDev, 0) & 0xfff;
    u32Strength = (isp_fpn_corr_gainoffset_read(ViPipe, u8BlkDev, 0) >> 16) & 0xffff;

    if (isp_ext_system_manual_fpn_Enable_read(ViPipe)) {
        ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                        "%8d" "%4d"  "%9d"  "%8d",
                        isp_ext_system_manual_fpn_Enable_read(ViPipe),
                        hi_ext_system_manual_fpn_opmode_read(ViPipe),
                        u32Strength,
                        u32Offset);
    } else {
        ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                        "%8d" "%4s"  "%9s"  "%8s\n",
                        isp_fpn_en_read(ViPipe, u8BlkDev),
                        "--",
                        "--",
                        "--");
    }

    pstProc->write_len += 1;

    return HI_SUCCESS;
}

HI_S32 ISP_FPNCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd) {
        case ISP_PROC_WRITE:
            FpnProcWrite(ViPipe, (hi_isp_ctrl_proc_write *)pValue);
            break;

        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_FPNExit(VI_PIPE ViPipe)
{
    return HI_SUCCESS;
}

HI_S32 isp_alg_register_fpn(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_FPN;
    pstAlgs->alg_func.pfn_alg_init = ISP_FPNInit;
    pstAlgs->alg_func.pfn_alg_run = ISP_FPNRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_FPNCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_FPNExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
