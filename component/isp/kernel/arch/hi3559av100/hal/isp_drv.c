/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include "isp_drv_defines.h"
#include "isp_reg_define.h"
#include "isp_stt_define.h"
#include "hi_common.h"
#include "hi_osal.h"
#include "hi_math.h"
#include "mkp_isp.h"
#include "isp_drv.h"
#include "isp.h"
#include "mm_ext.h"
#include "sys_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

extern osal_spinlock_t g_stIspLock[ISP_MAX_PIPE_NUM];
extern HI_BOOL         g_IntBothalf;
extern HI_BOOL         g_UseBothalf;
extern HI_BOOL         g_LdciTprFltEn[ISP_MAX_PIPE_NUM];

void  *reg_vicap_base_va = HI_NULL;
void  *reg_ispfe_base_va[ISP_MAX_PIPE_NUM]    = {HI_NULL};
void  *reg_vicap_ch_base_va[ISP_MAX_PIPE_NUM] = {HI_NULL};
void  *reg_ispbe_base_va[ISP_MAX_BE_NUM]      = {HI_NULL};
void  *reg_viproc_base_va[ISP_MAX_BE_NUM]     = {HI_NULL};

HI_U32 g_DrcCurLumaLut[HI_ISP_DRC_SHP_LOG_CONFIG_NUM][HI_ISP_DRC_EXP_COMP_SAMPLE_NUM - 1] = {
    {1,     1,      5,      31,     180,    1023,   32767},
    {1,     3,      8,      52,     277,    1446,   38966},
    {2,     5,      15,     87,     427,    2044,   46337},
    {4,     9,      27,     144,    656,    2888,   55101},
    {7,     16,     48,     240,    1008,   4080,   65521},
    {12,    29,     85,     399,    1547,   5761,   77906},
    {23,    53,     151,    660,    2372,   8128,   92622},
    {42,    97,     267,    1090,   3628,   11458,  110100},
    {76,    175,    468,    1792,   5537,   16130,  130840},
    {138,   313,    816,    2933,   8423,   22664,  155417},
    {258,   555,    1412,   4770,   12758,  31760,  184476},
    {441,   977,    2420,   7699,   19215,  44338,  218711},
    {776,   1698,   4100,   12304,  28720,  61568,  258816},
    {1344,  2907,   6847,   19416,  42491,  84851,  305376},
    {2283,  4884,   11224,  30137,  62006,  115708, 358680},
    {3783,  8004,   17962,  45770,  88821,  155470, 418391},
};

static HI_U16 Sqrt32(HI_U32 u32Arg)
{
    HI_U32 u32Mask = (HI_U32)1 << 15;
    HI_U16 u16Res = 0;
    HI_U32 i = 0;

    for (i = 0; i < 16; i++) {
        if ((u16Res + (u32Mask >> i)) * (u16Res + (u32Mask >> i)) <= u32Arg) {
            u16Res = u16Res + (u32Mask >> i);
        }
    }

    /* rounding */
    if (u16Res * u16Res + u16Res < u32Arg) {
        ++u16Res;
    }

    return u16Res;
}


/* --------------------------------- isp drv FHY regs define ------------------------------------------- */
HI_S32 ISP_DRV_SetInputSel(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 *pu32InputSel)
{
    U_ISP_BE_INPUT_MUX o_isp_be_input_mux;
    ISP_CHECK_POINTER(pstIspBeRegs);
    ISP_CHECK_POINTER(pu32InputSel);

    o_isp_be_input_mux.u32 = pstIspBeRegs->ISP_BE_INPUT_MUX.u32;
    o_isp_be_input_mux.bits.isp_input0_sel = pu32InputSel[0];
    o_isp_be_input_mux.bits.isp_input1_sel = pu32InputSel[1];
    o_isp_be_input_mux.bits.isp_input2_sel = pu32InputSel[2];
    o_isp_be_input_mux.bits.isp_input3_sel = pu32InputSel[3];
    o_isp_be_input_mux.bits.isp_input4_sel = pu32InputSel[4];
    pstIspBeRegs->ISP_BE_INPUT_MUX.u32 = o_isp_be_input_mux.u32;
    return 1;
}
static __inline HI_S32 ISP_DRV_SetIspDgain(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 u32IspDgain)
{
    U_ISP_DG_GAIN1 o_isp_dg_gain1;
    U_ISP_DG_GAIN2 o_isp_dg_gain2;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_dg_gain1.u32 = pstIspBeRegs->ISP_DG_GAIN1.u32;
    o_isp_dg_gain1.bits.isp_dg_rgain  = u32IspDgain;
    o_isp_dg_gain1.bits.isp_dg_grgain = u32IspDgain;
    pstIspBeRegs->ISP_DG_GAIN1.u32 = o_isp_dg_gain1.u32;

    o_isp_dg_gain2.u32 = pstIspBeRegs->ISP_DG_GAIN2.u32;
    o_isp_dg_gain2.bits.isp_dg_bgain  = u32IspDgain;
    o_isp_dg_gain2.bits.isp_dg_gbgain = u32IspDgain;
    pstIspBeRegs->ISP_DG_GAIN2.u32 = o_isp_dg_gain2.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_AwbEnable(S_VIPROC_REGS_TYPE *pstViProcReg, HI_U32 uisp_awb_en)
{
    U_VIPROC_ISP_BE_CTRL0 o_viproc_ispbe_ctrl0;
    o_viproc_ispbe_ctrl0.u32 = pstViProcReg->VIPROC_ISPBE_CTRL0.u32;
    o_viproc_ispbe_ctrl0.bits.isp_awb_en = uisp_awb_en;
    pstViProcReg->VIPROC_ISPBE_CTRL0.u32 = o_viproc_ispbe_ctrl0.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetIsp4Dgain0(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 *au32Isp4Dgain0)
{
    U_ISP_4DG_0_GAIN1 o_isp_4dg_0_gain1;
    U_ISP_4DG_0_GAIN2 o_isp_4dg_0_gain2;
    ISP_CHECK_POINTER(pstIspBeRegs);
    ISP_CHECK_POINTER(au32Isp4Dgain0);

    o_isp_4dg_0_gain1.u32 = pstIspBeRegs->ISP_4DG_0_GAIN1.u32;
    o_isp_4dg_0_gain1.bits.isp_4dg0_rgain  = au32Isp4Dgain0[0];
    o_isp_4dg_0_gain1.bits.isp_4dg0_grgain = au32Isp4Dgain0[1];
    pstIspBeRegs->ISP_4DG_0_GAIN1.u32 = o_isp_4dg_0_gain1.u32;

    o_isp_4dg_0_gain2.u32 = pstIspBeRegs->ISP_4DG_0_GAIN2.u32;
    o_isp_4dg_0_gain2.bits.isp_4dg0_bgain  = au32Isp4Dgain0[3];
    o_isp_4dg_0_gain2.bits.isp_4dg0_gbgain = au32Isp4Dgain0[2];
    pstIspBeRegs->ISP_4DG_0_GAIN2.u32 = o_isp_4dg_0_gain2.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetIsp4Dgain1(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 *au32Isp4Dgain1)
{
    U_ISP_4DG_1_GAIN1 o_isp_4dg_1_gain1;
    U_ISP_4DG_1_GAIN2 o_isp_4dg_1_gain2;
    ISP_CHECK_POINTER(pstIspBeRegs);
    ISP_CHECK_POINTER(au32Isp4Dgain1);

    o_isp_4dg_1_gain1.u32 = pstIspBeRegs->ISP_4DG_1_GAIN1.u32;
    o_isp_4dg_1_gain1.bits.isp_4dg1_rgain  = au32Isp4Dgain1[0];
    o_isp_4dg_1_gain1.bits.isp_4dg1_grgain = au32Isp4Dgain1[1];
    pstIspBeRegs->ISP_4DG_1_GAIN1.u32 = o_isp_4dg_1_gain1.u32;

    o_isp_4dg_1_gain2.u32 = pstIspBeRegs->ISP_4DG_1_GAIN2.u32;
    o_isp_4dg_1_gain2.bits.isp_4dg1_bgain  = au32Isp4Dgain1[3];
    o_isp_4dg_1_gain2.bits.isp_4dg1_gbgain = au32Isp4Dgain1[2];
    pstIspBeRegs->ISP_4DG_1_GAIN2.u32 = o_isp_4dg_1_gain2.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetIsp4Dgain2(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 *au32Isp4Dgain2)
{
    U_ISP_4DG_2_GAIN1 o_isp_4dg_2_gain1;
    U_ISP_4DG_2_GAIN2 o_isp_4dg_2_gain2;
    ISP_CHECK_POINTER(pstIspBeRegs);
    ISP_CHECK_POINTER(au32Isp4Dgain2);

    o_isp_4dg_2_gain1.u32 = pstIspBeRegs->ISP_4DG_2_GAIN1.u32;
    o_isp_4dg_2_gain1.bits.isp_4dg2_rgain  = au32Isp4Dgain2[0];
    o_isp_4dg_2_gain1.bits.isp_4dg2_grgain = au32Isp4Dgain2[1];
    pstIspBeRegs->ISP_4DG_2_GAIN1.u32 = o_isp_4dg_2_gain1.u32;

    o_isp_4dg_2_gain2.u32 = pstIspBeRegs->ISP_4DG_2_GAIN2.u32;
    o_isp_4dg_2_gain2.bits.isp_4dg2_bgain  = au32Isp4Dgain2[3];
    o_isp_4dg_2_gain2.bits.isp_4dg2_gbgain = au32Isp4Dgain2[2];
    pstIspBeRegs->ISP_4DG_2_GAIN2.u32 = o_isp_4dg_2_gain2.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetIsp4Dgain3(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 *au32Isp4Dgain3)
{
    U_ISP_4DG_3_GAIN1 o_isp_4dg_3_gain1;
    U_ISP_4DG_3_GAIN2 o_isp_4dg_3_gain2;
    ISP_CHECK_POINTER(pstIspBeRegs);
    ISP_CHECK_POINTER(au32Isp4Dgain3);

    o_isp_4dg_3_gain1.u32 = pstIspBeRegs->ISP_4DG_3_GAIN1.u32;
    o_isp_4dg_3_gain1.bits.isp_4dg3_rgain  = au32Isp4Dgain3[0];
    o_isp_4dg_3_gain1.bits.isp_4dg3_grgain = au32Isp4Dgain3[1];
    pstIspBeRegs->ISP_4DG_3_GAIN1.u32 = o_isp_4dg_3_gain1.u32;

    o_isp_4dg_3_gain2.u32 = pstIspBeRegs->ISP_4DG_3_GAIN2.u32;
    o_isp_4dg_3_gain2.bits.isp_4dg3_bgain  = au32Isp4Dgain3[3];
    o_isp_4dg_3_gain2.bits.isp_4dg3_gbgain = au32Isp4Dgain3[2];
    pstIspBeRegs->ISP_4DG_3_GAIN2.u32 = o_isp_4dg_3_gain2.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrExporratio0(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_exporatio0)
{
    U_ISP_WDR_EXPORRATIO0 o_isp_wdr_exporatio0;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_exporatio0.u32 = pstIspBeRegs->ISP_WDR_EXPORRATIO0.u32;
    o_isp_wdr_exporatio0.bits.isp_wdr_exporratio0 = uisp_wdr_exporatio0;
    pstIspBeRegs->ISP_WDR_EXPORRATIO0.u32 = o_isp_wdr_exporatio0.u32;
    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrExporratio1(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_exporatio1)
{
    U_ISP_WDR_EXPORRATIO0 o_isp_wdr_exporatio0;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_exporatio0.u32 = pstIspBeRegs->ISP_WDR_EXPORRATIO0.u32;
    o_isp_wdr_exporatio0.bits.isp_wdr_exporratio1 = uisp_wdr_exporatio1;
    pstIspBeRegs->ISP_WDR_EXPORRATIO0.u32 = o_isp_wdr_exporatio0.u32;
    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrExporratio2(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_exporatio2)
{
    U_ISP_WDR_EXPORRATIO1 o_isp_wdr_exporatio1;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_exporatio1.u32 = pstIspBeRegs->ISP_WDR_EXPORRATIO1.u32;
    o_isp_wdr_exporatio1.bits.isp_wdr_exporratio2 = uisp_wdr_exporatio2;
    pstIspBeRegs->ISP_WDR_EXPORRATIO1.u32 = o_isp_wdr_exporatio1.u32;
    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrExpoValue0(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_expovalue0)
{
    U_ISP_WDR_EXPOVALUE0 o_isp_wdr_expovalue0;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_expovalue0.u32 = pstIspBeRegs->ISP_WDR_EXPOVALUE0.u32;
    o_isp_wdr_expovalue0.bits.isp_wdr_expovalue0 = uisp_wdr_expovalue0;
    pstIspBeRegs->ISP_WDR_EXPOVALUE0.u32 = o_isp_wdr_expovalue0.u32;

    return 1;
}
static __inline HI_S32 ISP_DRV_SetWdrExpoValue1(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_expovalue1)
{
    U_ISP_WDR_EXPOVALUE0 o_isp_wdr_expovalue0;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_expovalue0.u32 = pstIspBeRegs->ISP_WDR_EXPOVALUE0.u32;
    o_isp_wdr_expovalue0.bits.isp_wdr_expovalue1 = uisp_wdr_expovalue1;
    pstIspBeRegs->ISP_WDR_EXPOVALUE0.u32 = o_isp_wdr_expovalue0.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrExpoValue2(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_expovalue2)
{
    U_ISP_WDR_EXPOVALUE1 o_isp_wdr_expovalue1;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_expovalue1.u32 = pstIspBeRegs->ISP_WDR_EXPOVALUE1.u32;
    o_isp_wdr_expovalue1.bits.isp_wdr_expovalue2 = uisp_wdr_expovalue2;
    pstIspBeRegs->ISP_WDR_EXPOVALUE1.u32 = o_isp_wdr_expovalue1.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrExpoValue3(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_expovalue3)
{
    U_ISP_WDR_EXPOVALUE1 o_isp_wdr_expovalue1;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_expovalue1.u32 = pstIspBeRegs->ISP_WDR_EXPOVALUE1.u32;
    o_isp_wdr_expovalue1.bits.isp_wdr_expovalue3 = uisp_wdr_expovalue3;
    pstIspBeRegs->ISP_WDR_EXPOVALUE1.u32 = o_isp_wdr_expovalue1.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetFlickExporatio0(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_flick_exporatio0)
{
    U_ISP_FLICK_EXPORATIO0 o_isp_flick_exporatio0;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_flick_exporatio0.u32 = pstIspBeRegs->ISP_FLICK_EXPORATIO0.u32;
    o_isp_flick_exporatio0.bits.isp_flick_exporatio0 = uisp_flick_exporatio0;
    pstIspBeRegs->ISP_FLICK_EXPORATIO0.u32 = o_isp_flick_exporatio0.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetFlickExporatio1(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_flick_exporatio1)
{
    U_ISP_FLICK_EXPORATIO0 o_isp_flick_exporatio0;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_flick_exporatio0.u32 = pstIspBeRegs->ISP_FLICK_EXPORATIO0.u32;
    o_isp_flick_exporatio0.bits.isp_flick_exporatio1 = uisp_flick_exporatio1;
    pstIspBeRegs->ISP_FLICK_EXPORATIO0.u32 = o_isp_flick_exporatio0.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetFlickExporatio2(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_flick_exporatio2)
{
    U_ISP_FLICK_EXPORATIO1 o_isp_flick_exporatio1;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_flick_exporatio1.u32 = pstIspBeRegs->ISP_FLICK_EXPORATIO1.u32;
    o_isp_flick_exporatio1.bits.isp_flick_exporatio2 = uisp_flick_exporatio2;
    pstIspBeRegs->ISP_FLICK_EXPORATIO1.u32 = o_isp_flick_exporatio1.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrBlcComp0(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_blc_comp0)
{
    U_ISP_WDR_BLC_COMP0 o_isp_wdr_blc_comp0;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_blc_comp0.u32 = pstIspBeRegs->ISP_WDR_BLC_COMP0.u32;
    o_isp_wdr_blc_comp0.bits.isp_wdr_blc_comp0 = uisp_wdr_blc_comp0;
    pstIspBeRegs->ISP_WDR_BLC_COMP0.u32 = o_isp_wdr_blc_comp0.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrBlcComp1(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_blc_comp1)
{
    U_ISP_WDR_BLC_COMP1 o_isp_wdr_blc_comp1;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_blc_comp1.u32 = pstIspBeRegs->ISP_WDR_BLC_COMP1.u32;
    o_isp_wdr_blc_comp1.bits.isp_wdr_blc_comp1 = uisp_wdr_blc_comp1;
    pstIspBeRegs->ISP_WDR_BLC_COMP1.u32 = o_isp_wdr_blc_comp1.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrBlcComp2(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_blc_comp2)
{
    U_ISP_WDR_BLC_COMP2 o_isp_wdr_blc_comp2;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_blc_comp2.u32 = pstIspBeRegs->ISP_WDR_BLC_COMP2.u32;
    o_isp_wdr_blc_comp2.bits.isp_wdr_blc_comp2 = uisp_wdr_blc_comp2;
    pstIspBeRegs->ISP_WDR_BLC_COMP2.u32 = o_isp_wdr_blc_comp2.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrMaxRatio(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_maxratio)
{
    U_ISP_WDR_MAXRATIO o_isp_wdr_maxratio;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_maxratio.u32 = pstIspBeRegs->ISP_WDR_MAXRATIO.u32;
    o_isp_wdr_maxratio.bits.isp_wdr_maxratio = uisp_wdr_maxratio;
    pstIspBeRegs->ISP_WDR_MAXRATIO.u32 = o_isp_wdr_maxratio.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrFusionMode(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_fusion_mode)
{
    U_ISP_WDR_CTRL o_isp_wdr_ctrl;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_ctrl.u32 = pstIspBeRegs->ISP_WDR_CTRL.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_fusionmode = uisp_wdr_fusion_mode;
    pstIspBeRegs->ISP_WDR_CTRL.u32     = o_isp_wdr_ctrl.u32;

    return 1;
}


static __inline HI_S32 ISP_DRV_SetWdrLongThr(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_long_thr)
{
    U_ISP_WDR_WGTIDX_THR o_isp_wdr_wgtidx_thr;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_wgtidx_thr.u32 = pstIspBeRegs->ISP_WDR_WGTIDX_THR.u32;
    o_isp_wdr_wgtidx_thr.bits.isp_wdr_long_thr  = uisp_wdr_long_thr;
    pstIspBeRegs->ISP_WDR_WGTIDX_THR.u32 = o_isp_wdr_wgtidx_thr.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrShortThr(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_short_thr)
{
    U_ISP_WDR_WGTIDX_THR o_isp_wdr_wgtidx_thr;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_wgtidx_thr.u32 = pstIspBeRegs->ISP_WDR_WGTIDX_THR.u32;
    o_isp_wdr_wgtidx_thr.bits.isp_wdr_short_thr  = uisp_wdr_short_thr;
    pstIspBeRegs->ISP_WDR_WGTIDX_THR.u32 = o_isp_wdr_wgtidx_thr.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetWdrMdtEn(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_wdr_mdt_en)
{
    U_ISP_WDR_CTRL o_isp_wdr_ctrl;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_wdr_ctrl.u32 = pstIspBeRegs->ISP_WDR_CTRL.u32;
    o_isp_wdr_ctrl.bits.isp_wdr_mdt_en = uisp_wdr_mdt_en;
    pstIspBeRegs->ISP_WDR_CTRL.u32     = o_isp_wdr_ctrl.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetLdciStatEvratio(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_ldci_stat_evratio)
{
    U_ISP_LDCI_STAT_EVRATIO o_isp_ldci_stat_evratio;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_ldci_stat_evratio.u32 = pstIspBeRegs->ISP_LDCI_STAT_EVRATIO.u32;
    o_isp_ldci_stat_evratio.bits.isp_ldci_stat_evratio = uisp_ldci_stat_evratio;
    pstIspBeRegs->ISP_LDCI_STAT_EVRATIO.u32 = o_isp_ldci_stat_evratio.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcPrevLuma0(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_prev_luma_0)
{
    U_ISP_DRC_PREV_LUMA_0 o_isp_drc_prev_luma_0;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_prev_luma_0.u32 = pstIspBeRegs->ISP_DRC_PREV_LUMA_0.u32;
    o_isp_drc_prev_luma_0.bits.isp_drc_prev_luma_0 = uisp_drc_prev_luma_0;
    pstIspBeRegs->ISP_DRC_PREV_LUMA_0.u32 = o_isp_drc_prev_luma_0.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcPrevLuma1(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_prev_luma_1)
{
    U_ISP_DRC_PREV_LUMA_1 o_isp_drc_prev_luma_1;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_prev_luma_1.u32 = pstIspBeRegs->ISP_DRC_PREV_LUMA_1.u32;
    o_isp_drc_prev_luma_1.bits.isp_drc_prev_luma_1 = uisp_drc_prev_luma_1;
    pstIspBeRegs->ISP_DRC_PREV_LUMA_1.u32 = o_isp_drc_prev_luma_1.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcPrevLuma2(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_prev_luma_2)
{
    U_ISP_DRC_PREV_LUMA_2 o_isp_drc_prev_luma_2;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_prev_luma_2.u32 = pstIspBeRegs->ISP_DRC_PREV_LUMA_2.u32;
    o_isp_drc_prev_luma_2.bits.isp_drc_prev_luma_2 = uisp_drc_prev_luma_2;
    pstIspBeRegs->ISP_DRC_PREV_LUMA_2.u32 = o_isp_drc_prev_luma_2.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcPrevLuma3(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_prev_luma_3)
{
    U_ISP_DRC_PREV_LUMA_3 o_isp_drc_prev_luma_3;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_prev_luma_3.u32 = pstIspBeRegs->ISP_DRC_PREV_LUMA_3.u32;
    o_isp_drc_prev_luma_3.bits.isp_drc_prev_luma_3 = uisp_drc_prev_luma_3;
    pstIspBeRegs->ISP_DRC_PREV_LUMA_3.u32 = o_isp_drc_prev_luma_3.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcPrevLuma4(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_prev_luma_4)
{
    U_ISP_DRC_PREV_LUMA_4 o_isp_drc_prev_luma_4;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_prev_luma_4.u32 = pstIspBeRegs->ISP_DRC_PREV_LUMA_4.u32;
    o_isp_drc_prev_luma_4.bits.isp_drc_prev_luma_4 = uisp_drc_prev_luma_4;
    pstIspBeRegs->ISP_DRC_PREV_LUMA_4.u32 = o_isp_drc_prev_luma_4.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcPrevLuma5(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_prev_luma_5)
{
    U_ISP_DRC_PREV_LUMA_5 o_isp_drc_prev_luma_5;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_prev_luma_5.u32 = pstIspBeRegs->ISP_DRC_PREV_LUMA_5.u32;
    o_isp_drc_prev_luma_5.bits.isp_drc_prev_luma_5 = uisp_drc_prev_luma_5;
    pstIspBeRegs->ISP_DRC_PREV_LUMA_5.u32 = o_isp_drc_prev_luma_5.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcPrevLuma6(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_prev_luma_6)
{
    U_ISP_DRC_PREV_LUMA_6 o_isp_drc_prev_luma_6;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_prev_luma_6.u32 = pstIspBeRegs->ISP_DRC_PREV_LUMA_6.u32;
    o_isp_drc_prev_luma_6.bits.isp_drc_prev_luma_6 = uisp_drc_prev_luma_6;
    pstIspBeRegs->ISP_DRC_PREV_LUMA_6.u32 = o_isp_drc_prev_luma_6.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcPrevLuma7(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_prev_luma_7)
{
    U_ISP_DRC_PREV_LUMA_7 o_isp_drc_prev_luma_7;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_prev_luma_7.u32 = pstIspBeRegs->ISP_DRC_PREV_LUMA_7.u32;
    o_isp_drc_prev_luma_7.bits.isp_drc_prev_luma_7 = uisp_drc_prev_luma_7;
    pstIspBeRegs->ISP_DRC_PREV_LUMA_7.u32 = o_isp_drc_prev_luma_7.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcShpCfg(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_shp_log, HI_U32 uisp_drc_shp_exp)
{
    U_ISP_DRC_SHP_CFG o_isp_drc_shp_cfg;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_shp_cfg.u32 = pstIspBeRegs->ISP_DRC_SHP_CFG.u32;
    o_isp_drc_shp_cfg.bits.isp_drc_shp_log = uisp_drc_shp_log;
    o_isp_drc_shp_cfg.bits.isp_drc_shp_exp = uisp_drc_shp_exp;
    pstIspBeRegs->ISP_DRC_SHP_CFG.u32 = o_isp_drc_shp_cfg.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcDivDenomLog(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_div_denom_log)
{
    U_ISP_DRC_DIV_DENOM_LOG o_isp_drc_div_denom_log;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_div_denom_log.u32 = pstIspBeRegs->ISP_DRC_DIV_DENOM_LOG.u32;
    o_isp_drc_div_denom_log.bits.isp_drc_div_denom_log = uisp_drc_div_denom_log;
    pstIspBeRegs->ISP_DRC_DIV_DENOM_LOG.u32 = o_isp_drc_div_denom_log.u32;

    return 1;
}

static __inline HI_S32 ISP_DRV_SetDrcDenomExp(S_ISPBE_REGS_TYPE *pstIspBeRegs, HI_U32 uisp_drc_denom_exp)
{
    U_ISP_DRC_DENOM_EXP o_isp_drc_denom_exp;
    ISP_CHECK_POINTER(pstIspBeRegs);

    o_isp_drc_denom_exp.u32 = pstIspBeRegs->ISP_DRC_DENOM_EXP.u32;
    o_isp_drc_denom_exp.bits.isp_drc_denom_exp = uisp_drc_denom_exp;
    pstIspBeRegs->ISP_DRC_DENOM_EXP.u32 = o_isp_drc_denom_exp.u32;

    return 1;
}

HI_S32 ISP_DRV_SetLdciWriteSttAddr(S_VIPROC_REGS_TYPE *apstViprocReg, HI_U64 u64WriteAddr)
{
    ISP_CHECK_POINTER(apstViprocReg);
    apstViprocReg->VIPROC_OUT_PARA_DCI_ADDR_LOW.u32  = GetLowAddr(u64WriteAddr);
    apstViprocReg->VIPROC_OUT_PARA_DCI_ADDR_HIGH.u32 = GetHighAddr(u64WriteAddr);
    return 1;
}

HI_S32 ISP_DRV_SetLdciReadSttAddr(S_VIPROC_REGS_TYPE *apstViprocReg, HI_U64 u64ReadAddr)
{
    ISP_CHECK_POINTER(apstViprocReg);
    apstViprocReg->VIPROC_PARA_DCI_ADDR_LOW.u32  = GetLowAddr(u64ReadAddr);
    apstViprocReg->VIPROC_PARA_DCI_ADDR_HIGH.u32 = GetHighAddr(u64ReadAddr);
    return 1;
}

HI_S32 ISP_DRV_BeRemap(void)
{
    HI_U32 IspBePhyPipe = 0;

    for (IspBePhyPipe = 0; IspBePhyPipe < ISP_MAX_BE_NUM; IspBePhyPipe++) {
        reg_ispbe_base_va[IspBePhyPipe] = (void *)osal_ioremap(ISP_BE_REG_BASE(IspBePhyPipe), (HI_U32)VI_ISP_BE_REG_SIZE);

        if (reg_ispbe_base_va[IspBePhyPipe] == HI_NULL) {
            osal_printk("Remap ISP BE[%d] failed!\n", IspBePhyPipe);
            return HI_FAILURE;
        }

        reg_viproc_base_va[IspBePhyPipe] = (void *)osal_ioremap(ISP_VIPROC_REG_BASE(IspBePhyPipe), (HI_U32)VIPROC_REG_SIZE);

        if (reg_viproc_base_va[IspBePhyPipe] == HI_NULL) {
            osal_printk("Remap isp viproc[%d] failed!\n", IspBePhyPipe);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_VOID ISP_DRV_BeUnmap(void)
{
    HI_U32 IspBePhyPipe = 0;

    for (IspBePhyPipe = 0; IspBePhyPipe < ISP_MAX_BE_NUM; IspBePhyPipe++) {
        if (reg_ispbe_base_va[IspBePhyPipe] != NULL) {
            osal_iounmap(reg_ispbe_base_va[IspBePhyPipe]);
            reg_ispbe_base_va[IspBePhyPipe] = NULL;
        }

        if (reg_viproc_base_va[IspBePhyPipe] != NULL) {
            osal_iounmap(reg_viproc_base_va[IspBePhyPipe]);
            reg_viproc_base_va[IspBePhyPipe] = NULL;
        }
    }
}

HI_S32 ISP_DRV_VicapRemap(void)
{
    reg_vicap_base_va = (void *)osal_ioremap(CAP_REG_BASE, (HI_U32)CAP_REG_SIZE_ALIGN);

    if (reg_vicap_base_va == HI_NULL) {
        osal_printk("Remap ISP PT failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID ISP_DRV_VicapUnmap(void)
{
    if (reg_vicap_base_va != NULL) {
        osal_iounmap(reg_vicap_base_va);
        reg_vicap_base_va = NULL;
    }
}

HI_S32 ISP_DRV_FeRemap(void)
{
    HI_U8   i;
    VI_PIPE ViPipe;
    for (ViPipe = 0; ViPipe < ISP_MAX_PHY_PIPE_NUM; ViPipe++) {
        reg_ispfe_base_va[ViPipe] = (void *)osal_ioremap(ISP_FE_REG_BASE(ViPipe), (HI_U32)VI_ISP_FE_REG_SIZE);

        if (reg_ispfe_base_va[ViPipe] == HI_NULL) {
            osal_printk("Remap ISP[%d] FE failed!\n", ViPipe);
            return HI_FAILURE;
        }

        reg_vicap_ch_base_va[ViPipe] = (void *)osal_ioremap(ISP_VICAP_CH_REG_BASE(ViPipe), (HI_U32)VICAP_CH_REG_SIZE_ALIGN);

        if (reg_vicap_ch_base_va[ViPipe] == HI_NULL) {
            osal_printk("Remap ISP[%d] FE STT failed!\n", ViPipe);
            return HI_FAILURE;
        }

        /* enable port int & isp int */
        IO_RW_PT_ADDRESS(VICAP_HD_MASK) |= VICAP_INT_MASK_PT(ViPipe);
        IO_RW_PT_ADDRESS(VICAP_HD_MASK) |= VICAP_INT_MASK_ISP(ViPipe);

        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) = (0x0);
    }

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        osal_memset(&g_astIspDrvCtx[ViPipe], 0, sizeof(ISP_DRV_CTX_S));
        osal_snprintf(g_astIspDrvCtx[ViPipe].acName, sizeof(g_astIspDrvCtx[ViPipe].acName), "ISP[%d]", ViPipe);

        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.wdr_wb_gain[0] = 0x1EB;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.wdr_wb_gain[1] = 0x100;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.wdr_wb_gain[2] = 0x100;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.wdr_wb_gain[3] = 0x1CA;

        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.ccm[0] = 0x01A7;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.ccm[1] = 0x809D;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.ccm[2] = 0x800A;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.ccm[3] = 0x804F;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.ccm[4] = 0x018E;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.ccm[5] = 0x803F;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.ccm[6] = 0x000B;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.ccm[7] = 0x8098;
        g_astIspDrvCtx[ViPipe].stIspBeSyncPara.ccm[8] = 0x018D;

        g_astIspDrvCtx[ViPipe].stWorkMode.running_mode      = ISP_MODE_RUNNING_OFFLINE;
        g_astIspDrvCtx[ViPipe].stWorkMode.block_num             = 1;

        g_astIspDrvCtx[ViPipe].stStitchAttr.stitch_enable = HI_FALSE;
        g_astIspDrvCtx[ViPipe].stStitchAttr.stitch_pipe_num = 2;
        g_astIspDrvCtx[ViPipe].stStitchAttr.stitch_bind_id[0] = 0;
        g_astIspDrvCtx[ViPipe].stStitchAttr.stitch_bind_id[1] = 3;
        g_astIspDrvCtx[ViPipe].stStitchAttr.stitch_bind_id[2] = -1;
        g_astIspDrvCtx[ViPipe].stStitchAttr.stitch_bind_id[3] = -1;
        g_astIspDrvCtx[ViPipe].stStitchAttr.stitch_bind_id[4] = -1;
        g_astIspDrvCtx[ViPipe].stStitchAttr.stitch_bind_id[5] = -1;
        g_astIspDrvCtx[ViPipe].astChnSelAttr[0].channel_sel = 0;
        g_astIspDrvCtx[ViPipe].astChnSelAttr[1].channel_sel = 0;
        g_astIspDrvCtx[ViPipe].enIspRunningState = ISP_BE_BUF_STATE_INIT;
        g_astIspDrvCtx[ViPipe].enIspExitState = ISP_BE_BUF_EXIT;

        /* snap attr init */
        g_astIspDrvCtx[ViPipe].stSnapAttr.snap_type = SNAP_TYPE_NORMAL;
        g_astIspDrvCtx[ViPipe].stSnapAttr.picture_pipe_id = -1;
        g_astIspDrvCtx[ViPipe].stSnapAttr.preview_pipe_id = -1;
        g_astIspDrvCtx[ViPipe].stSnapAttr.load_ccm = HI_TRUE;
        g_astIspDrvCtx[ViPipe].stSnapAttr.pro_param.operation_mode = OPERATION_MODE_AUTO;

        g_astIspDrvCtx[ViPipe].bRunOnceOk = HI_FALSE;
        g_astIspDrvCtx[ViPipe].bRunOnceFlag = HI_FALSE;
        g_astIspDrvCtx[ViPipe].bYUVRunOnceOk = HI_FALSE;
        g_astIspDrvCtx[ViPipe].bYUVRunOnceFlag = HI_FALSE;
        g_astIspDrvCtx[ViPipe].pstUseNode = HI_NULL;

        for (i = 0; i < PRO_MAX_FRAME_NUM; i++) {
            g_astIspDrvCtx[ViPipe].stSnapAttr.pro_param.auto_param.pro_exp_step[i] = 256;
            g_astIspDrvCtx[ViPipe].stSnapAttr.pro_param.manual_param.man_exp_time[i] = 10000;
            g_astIspDrvCtx[ViPipe].stSnapAttr.pro_param.manual_param.man_sysgain[i] = 1024;
        }

        g_astIspDrvCtx[ViPipe].stSnapAttr.pro_param.pro_frame_num= 3;

        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            g_astIspDrvCtx[ViPipe].stIspBeSyncPara.isp_dgain[i] = 0x100;
        }

        osal_wait_init(&g_astIspDrvCtx[ViPipe].stIspWait);
        osal_wait_init(&g_astIspDrvCtx[ViPipe].stIspWaitVdStart);
        osal_wait_init(&g_astIspDrvCtx[ViPipe].stIspWaitVdEnd);
        osal_wait_init(&g_astIspDrvCtx[ViPipe].stIspWaitVdBeEnd);
        osal_wait_init(&g_astIspDrvCtx[ViPipe].stIspExitWait);
        g_astIspDrvCtx[ViPipe].bEdge = HI_FALSE;
        g_astIspDrvCtx[ViPipe].bVdStart = HI_FALSE;
        g_astIspDrvCtx[ViPipe].bVdEnd = HI_FALSE;
        g_astIspDrvCtx[ViPipe].bVdBeEnd = HI_FALSE;
        g_astIspDrvCtx[ViPipe].bMemInit = HI_FALSE;
        g_astIspDrvCtx[ViPipe].bIspInit = HI_FALSE;
        g_astIspDrvCtx[ViPipe].pub_attr_ok = HI_FALSE;
        g_astIspDrvCtx[ViPipe].bIspRunFlag = HI_FALSE;
        g_astIspDrvCtx[ViPipe].u32ProTrigFlag = 0;
        osal_sema_init(&g_astIspDrvCtx[ViPipe].stIspSem, 1);
        osal_sema_init(&g_astIspDrvCtx[ViPipe].stIspSemVd, 1);
        osal_sema_init(&g_astIspDrvCtx[ViPipe].stIspSemBeVd, 1);
        osal_sema_init(&g_astIspDrvCtx[ViPipe].stProcSem, 1);
        osal_sema_init(&g_astIspDrvCtx[ViPipe].stProInfoSem, 1);
        SyncTaskInit(ViPipe);
    }
    return HI_SUCCESS;
}

HI_VOID ISP_DRV_FeUnmap(void)
{
    VI_PIPE ViPipe;

    for (ViPipe = 0; ViPipe < ISP_MAX_PHY_PIPE_NUM; ViPipe++) {
        if (reg_ispfe_base_va[ViPipe] != NULL) {
            osal_iounmap(reg_ispfe_base_va[ViPipe]);
            reg_ispfe_base_va[ViPipe] = NULL;
        }

        if (reg_vicap_ch_base_va[ViPipe] != NULL) {
            osal_iounmap(reg_vicap_ch_base_va[ViPipe]);
            reg_vicap_ch_base_va[ViPipe] = NULL;
        }
    }

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        osal_sema_destory(&g_astIspDrvCtx[ViPipe].stIspSem);
        osal_sema_destory(&g_astIspDrvCtx[ViPipe].stIspSemVd);
        osal_sema_destory(&g_astIspDrvCtx[ViPipe].stIspSemBeVd);
        osal_sema_destory(&g_astIspDrvCtx[ViPipe].stProcSem);
        osal_sema_destory(&g_astIspDrvCtx[ViPipe].stProInfoSem);

        osal_wait_destory(&g_astIspDrvCtx[ViPipe].stIspWait);
        osal_wait_destory(&g_astIspDrvCtx[ViPipe].stIspWaitVdStart);
        osal_wait_destory(&g_astIspDrvCtx[ViPipe].stIspWaitVdEnd);
        osal_wait_destory(&g_astIspDrvCtx[ViPipe].stIspWaitVdBeEnd);
        osal_wait_destory(&g_astIspDrvCtx[ViPipe].stIspExitWait);

        SyncTaskExit(ViPipe);
    }
}

HI_S32 ISP_DRV_GetViprocRegsAttr(VI_PIPE ViPipe, S_VIPROC_REGS_TYPE *apstViprocReg[], isp_be_regs_attr *pstBlkAttr)
{
    HI_U8  k, u8BlkDev, u8BlkNum;
    HI_S8  s8BlockId = 0;
    isp_running_mode  enIspRunningMode;
    ISP_DRV_CTX_S       *pstDrvCtx      = HI_NULL;
    isp_be_wo_reg_cfg *pstIspBeRegCfg = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBlkAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    enIspRunningMode = pstDrvCtx->stWorkMode.running_mode;

    u8BlkNum = ISP_DRV_GetBlockNum(ViPipe);
    u8BlkNum = DIV_0_TO_1(u8BlkNum);
    s8BlockId = ISP_DRV_GetBlockId(ViPipe, enIspRunningMode);

    if (-1 == s8BlockId) {
        ISP_ERR_TRACE("ISP[%d] Online Mode Pipe Err!\n", ViPipe);
        return HI_FAILURE;
    }

    u8BlkDev = (HI_U8)s8BlockId;
    pstBlkAttr->block_dev = u8BlkDev;
    pstBlkAttr->block_num = u8BlkNum;

    switch (enIspRunningMode) {
        case ISP_MODE_RUNNING_ONLINE:
            ISP_DRV_VIPROCREG_CTX(u8BlkDev, apstViprocReg[u8BlkDev]);
            break;

        case ISP_MODE_RUNNING_SIDEBYSIDE:
            for (k = 0; k < ISP_MAX_BE_NUM; k++) {
                ISP_DRV_VIPROCREG_CTX(k, apstViprocReg[k]);
            }
            break;
        case ISP_MODE_RUNNING_OFFLINE:
            ISP_CHECK_BEBUF_INIT(ViPipe);
            if (pstDrvCtx->pstUseNode == HI_NULL) {
                ISP_WARN_TRACE("ISP[%d] UseNode is Null!\n", ViPipe);
                return HI_FAILURE;
            }
            pstIspBeRegCfg = (isp_be_wo_reg_cfg *)pstDrvCtx->pstUseNode->stBeCfgBuf.vir_addr;
            apstViprocReg[u8BlkDev] = &pstIspBeRegCfg->be_reg_cfg[u8BlkDev].stViProcReg;

            break;

        case ISP_MODE_RUNNING_STRIPING:
            ISP_CHECK_BEBUF_INIT(ViPipe);
            if (pstDrvCtx->pstUseNode == HI_NULL) {
                ISP_WARN_TRACE("ISP[%d] UseNode is Null!\n", ViPipe);
                return HI_FAILURE;
            }
            pstIspBeRegCfg = (isp_be_wo_reg_cfg *)pstDrvCtx->pstUseNode->stBeCfgBuf.vir_addr;
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                apstViprocReg[k] = &pstIspBeRegCfg->be_reg_cfg[k].stViProcReg;
            }
            break;

        default:
            ISP_ERR_TRACE("ISP[%d] Running Mode Err!\n", ViPipe);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_RegConfigChnSel(S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, HI_U8 u8BlkNum, HI_U8 u8BlkDev)
{
    HI_U8   k;
    HI_U32  au32ChnSwitch[5]  = {0};
    HI_U32  u32ChannelSel     = 0;

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);
    }

    ISP_CHECK_POINTER(pstDrvCtx);

    u32ChannelSel = pstDrvCtx->astChnSelAttr[0].channel_sel;

    if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
        switch (u32ChannelSel & 0x3) {
            case 0:
                au32ChnSwitch[0] = (pstDrvCtx->stSyncCfg.u8VCNumMax - pstDrvCtx->stSyncCfg.u8VCNum) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[1] = (au32ChnSwitch[0] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[2] = (au32ChnSwitch[0] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[3] = (au32ChnSwitch[0] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                break;

            case 1:
                au32ChnSwitch[1] = (pstDrvCtx->stSyncCfg.u8VCNumMax - pstDrvCtx->stSyncCfg.u8VCNum) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[0] = (au32ChnSwitch[1] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[2] = (au32ChnSwitch[1] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[3] = (au32ChnSwitch[1] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                break;

            case 2:
                au32ChnSwitch[2] = (pstDrvCtx->stSyncCfg.u8VCNumMax - pstDrvCtx->stSyncCfg.u8VCNum) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[1] = (au32ChnSwitch[2] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[0] = (au32ChnSwitch[2] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[3] = (au32ChnSwitch[2] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                break;

            case 3:
                au32ChnSwitch[3] = (pstDrvCtx->stSyncCfg.u8VCNumMax - pstDrvCtx->stSyncCfg.u8VCNum) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[2] = (au32ChnSwitch[3] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[1] = (au32ChnSwitch[3] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                au32ChnSwitch[0] = (au32ChnSwitch[3] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
                break;
        }

        for (k = 0; k < u8BlkNum; k++) {
            ISP_DRV_SetInputSel(apstBeReg[k + u8BlkDev], &au32ChnSwitch[0]);
        }

    } else if ((IS_LINE_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) ||
               (IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))) {
        switch (u32ChannelSel & 0x3) {
            case 0:
                au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[0];
                au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[1];
                au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[2];
                au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[3];
                break;

            case 1:
                au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[0];
                au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[1];
                au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[2];
                au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[3];
                break;

            case 2:
                au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[0];
                au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[1];
                au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[2];
                au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[3];
                break;

            case 3:
                au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[0];
                au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[1];
                au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[2];
                au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[3];
                break;
        }

        /* offline mode: isp BE buffer poll, so chn switch need each frame refres */
        if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
            (IS_STRIPING_MODE(pstDrvCtx->stWorkMode.running_mode))) {
            for (k = 0; k < u8BlkNum; k++) {
                ISP_DRV_SetInputSel(apstBeReg[k + u8BlkDev], &au32ChnSwitch[0]);
            }
        }
    } else {
    }

    return HI_SUCCESS;
}

static hi_void isp_drv_update_wdr_blc_offset(ISP_DRV_CTX_S *drv_ctx, hi_u16 *blc_offset)
{
    ISP_CHECK_POINTER_VOID(drv_ctx);

    if (IS_ONLINE_MODE(drv_ctx->stWorkMode.running_mode) ||
        IS_SIDEBYSIDE_MODE(drv_ctx->stWorkMode.running_mode)) {
        return;
    }

    if (drv_ctx->stFrameIntAttr.int_type == FRAME_INTERRUPT_START) {
        if (drv_ctx->stSyncCfg.apstNode[1] != HI_NULL) {
            blc_offset[0] = drv_ctx->stSyncCfg.apstNode[1]->stWDRRegCfg.u16Offset0;
            blc_offset[1] = drv_ctx->stSyncCfg.apstNode[1]->stWDRRegCfg.u16Offset1;
            blc_offset[2] = drv_ctx->stSyncCfg.apstNode[1]->stWDRRegCfg.u16Offset2;
        }
    } else {
        if (drv_ctx->stSyncCfg.apstNode[0] != HI_NULL) {
            blc_offset[0] = drv_ctx->stSyncCfg.apstNode[0]->stWDRRegCfg.u16Offset0;
            blc_offset[1] = drv_ctx->stSyncCfg.apstNode[0]->stWDRRegCfg.u16Offset1;
            blc_offset[2] = drv_ctx->stSyncCfg.apstNode[0]->stWDRRegCfg.u16Offset2;
        }
    }
}

HI_S32 ISP_DRV_RegConfigWdr(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx,
                            ISP_FSWDR_SYNC_CFG_S *pstWDRRegCfg, HI_U8 u8BlkNum, HI_U8 u8BlkDev, HI_U32 *u32Ratio)
{
    HI_U8   i;
    HI_U8   u8LFMode          = 0;
    HI_U8   u8BitDepthInValid = 0;
    HI_U8   u8BitShift        = 0;
    HI_U32  au32ExpoValue[4]  = {0};
    HI_U32  au32BlcComp[3]    = {0};
    HI_S32  u32MaxRatio       = 0;
    HI_U16  u16LongThr        = 0x3FFF;
    HI_U16  u16ShortThr       = 0x3FFF;
    HI_BOOL bWDRMdtEn         = 0;
    hi_u16  offset[3]         = { 0 };
    HI_U8   u8CfgDlyMax;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDrvCtx);
    ISP_CHECK_POINTER(apstBeReg[u8BlkDev]);
    ISP_CHECK_POINTER(pstWDRRegCfg);

    u8BitDepthInValid = apstBeReg[u8BlkDev]->ISP_WDR_BIT_DEPTH.bits.isp_wdr_bitdepth_invalid;
    u8BitShift = 14 - u8BitDepthInValid;

    offset[0] = pstWDRRegCfg->u16Offset0;
    offset[1] = pstWDRRegCfg->u16Offset1;
    offset[2] = pstWDRRegCfg->u16Offset2;

    isp_drv_update_wdr_blc_offset(pstDrvCtx, offset);

    if (IS_2to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
        ISP_DRV_SetWdrExporratio0(apstBeReg[u8BlkDev], MIN2((ISP_BITMASK(10) * 64 / DIV_0_TO_1(u32Ratio[0])), 0x3FF));
        ISP_DRV_SetFlickExporatio0(apstBeReg[u8BlkDev], MIN2(u32Ratio[0], 0X3FFF));

        if (pstWDRRegCfg->bFusionMode == 0) {
            au32ExpoValue[0] = MIN2(u32Ratio[0], ISP_BITMASK(14));
            au32ExpoValue[1] = MIN2(64, ISP_BITMASK(14));
        } else {
            au32ExpoValue[0] = MIN2((u32Ratio[0] + 64), ISP_BITMASK(14));
            au32ExpoValue[1] = MIN2(64, ISP_BITMASK(14));
        }

        ISP_DRV_SetWdrExpoValue0(apstBeReg[u8BlkDev], au32ExpoValue[0]);
        ISP_DRV_SetWdrExpoValue1(apstBeReg[u8BlkDev], au32ExpoValue[1]);

        au32BlcComp[0] = (au32ExpoValue[0] - au32ExpoValue[1]) * offset[0] >> u8BitShift;
        ISP_DRV_SetWdrBlcComp0(apstBeReg[u8BlkDev], au32BlcComp[0]);
    } else if (IS_3to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
        ISP_DRV_SetWdrExporratio0(apstBeReg[u8BlkDev], MIN2((ISP_BITMASK(10) * 64 / DIV_0_TO_1(u32Ratio[0])), 0x3FF));
        ISP_DRV_SetWdrExporratio1(apstBeReg[u8BlkDev], MIN2((ISP_BITMASK(10) * 64 / DIV_0_TO_1(u32Ratio[1])), 0x3FF));

        ISP_DRV_SetFlickExporatio0(apstBeReg[u8BlkDev], MIN2(u32Ratio[0], 0X3FFF));
        ISP_DRV_SetFlickExporatio1(apstBeReg[u8BlkDev], MIN2(u32Ratio[1], 0X3FFF));

        if (pstWDRRegCfg->bFusionMode == 0) {
            au32ExpoValue[2] = MIN2(64, ISP_BITMASK(14));
            au32ExpoValue[1] = MIN2(u32Ratio[1], ISP_BITMASK(14));
            au32ExpoValue[0] = MIN2((u32Ratio[1] * u32Ratio[0]) >> 6, ISP_BITMASK(14));
        } else {
            au32ExpoValue[2] = MIN2(64, ISP_BITMASK(14));
            au32ExpoValue[1] = MIN2(((u32Ratio[1] * u32Ratio[0] / DIV_0_TO_1(u32Ratio[0] + 64)) + 64), ISP_BITMASK(14));
            au32ExpoValue[0] = MIN2((64 + (u32Ratio[1] * u32Ratio[0] >> 6) + u32Ratio[0]), ISP_BITMASK(14));
        }

        ISP_DRV_SetWdrExpoValue0(apstBeReg[u8BlkDev], au32ExpoValue[0]);
        ISP_DRV_SetWdrExpoValue1(apstBeReg[u8BlkDev], au32ExpoValue[1]);
        ISP_DRV_SetWdrExpoValue2(apstBeReg[u8BlkDev], au32ExpoValue[2]);

        au32BlcComp[0] = (au32ExpoValue[0] - au32ExpoValue[1]) * offset[0] >> u8BitShift;
        au32BlcComp[1] = (au32ExpoValue[1] - au32ExpoValue[2]) * offset[1] >> u8BitShift;

        ISP_DRV_SetWdrBlcComp0(apstBeReg[u8BlkDev], au32BlcComp[0]);
        ISP_DRV_SetWdrBlcComp1(apstBeReg[u8BlkDev], au32BlcComp[1]);
    } else if (IS_4to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
        ISP_DRV_SetWdrExporratio0(apstBeReg[u8BlkDev], MIN2((ISP_BITMASK(10) * 64 / DIV_0_TO_1(u32Ratio[0])), 0x3FF));
        ISP_DRV_SetWdrExporratio1(apstBeReg[u8BlkDev], MIN2((ISP_BITMASK(10) * 64 / DIV_0_TO_1(u32Ratio[1])), 0x3FF));
        ISP_DRV_SetWdrExporratio2(apstBeReg[u8BlkDev], MIN2((ISP_BITMASK(10) * 64 / DIV_0_TO_1(u32Ratio[2])), 0x3FF));

        ISP_DRV_SetFlickExporatio0(apstBeReg[u8BlkDev], MIN2(u32Ratio[0], 0X3FFF));
        ISP_DRV_SetFlickExporatio1(apstBeReg[u8BlkDev], MIN2(u32Ratio[1], 0X3FFF));
        ISP_DRV_SetFlickExporatio2(apstBeReg[u8BlkDev], MIN2(u32Ratio[2], 0X3FFF));

        if (pstWDRRegCfg->bFusionMode == 0) {
            au32ExpoValue[3] = MIN2(64, ISP_BITMASK(14));
            au32ExpoValue[2] = MIN2(u32Ratio[2], ISP_BITMASK(14));
            au32ExpoValue[1] = MIN2((u32Ratio[2] * u32Ratio[1]) >> 6, ISP_BITMASK(14));
            au32ExpoValue[0] = MIN2((u32Ratio[2] * u32Ratio[1] * u32Ratio[0]) >> 12, ISP_BITMASK(14));
        } else {
            au32ExpoValue[3] = MIN2(64, ISP_BITMASK(14));
            au32ExpoValue[2] = MIN2((64 + (u32Ratio[2] * u32Ratio[1] * u32Ratio[0] >> 6) / DIV_0_TO_1(64 + (u32Ratio[1] * u32Ratio[0] >> 6) + u32Ratio[0])), ISP_BITMASK(14));
            au32ExpoValue[1] = MIN2((64 + ((u32Ratio[2] * u32Ratio[1] * u32Ratio[0] >> 6) + u32Ratio[1] * u32Ratio[0]) / DIV_0_TO_1(64 + u32Ratio[0])), ISP_BITMASK(14));
            au32ExpoValue[0] = MIN2((64 + (u32Ratio[2] * u32Ratio[1] * u32Ratio[0] >> 12) + (u32Ratio[1] * u32Ratio[0] >> 6) + u32Ratio[0]), ISP_BITMASK(14));
        }

        ISP_DRV_SetWdrExpoValue0(apstBeReg[u8BlkDev], au32ExpoValue[0]);
        ISP_DRV_SetWdrExpoValue1(apstBeReg[u8BlkDev], au32ExpoValue[1]);
        ISP_DRV_SetWdrExpoValue2(apstBeReg[u8BlkDev], au32ExpoValue[2]);
        ISP_DRV_SetWdrExpoValue3(apstBeReg[u8BlkDev], au32ExpoValue[3]);

        au32BlcComp[0] = (au32ExpoValue[0] - au32ExpoValue[1]) * offset[0] >> u8BitShift;
        au32BlcComp[1] = (au32ExpoValue[1] - au32ExpoValue[2]) * offset[1] >> u8BitShift;
        au32BlcComp[2] = (au32ExpoValue[2] - au32ExpoValue[3]) * offset[2] >> u8BitShift;
        ISP_DRV_SetWdrBlcComp0(apstBeReg[u8BlkDev], au32BlcComp[0]);
        ISP_DRV_SetWdrBlcComp1(apstBeReg[u8BlkDev], au32BlcComp[1]);
        ISP_DRV_SetWdrBlcComp2(apstBeReg[u8BlkDev], au32BlcComp[2]);
    }

    for (i = 0; i < SYNC_EXP_RATIO_NUM; i++) {
        pstDrvCtx->stIspBeSyncPara.wdr_exp_ratio[i]   = MIN2((ISP_BITMASK(10) * 64 / DIV_0_TO_1(u32Ratio[i])), 0x3FF);
        pstDrvCtx->stIspBeSyncPara.flick_exp_ratio[i] = MIN2(u32Ratio[i], 0X3FFF);
    }

    for (i = 0; i < SYNC_WDR_EXP_VAL_NUM; i++) {
        pstDrvCtx->stIspBeSyncPara.wdr_exp_val[i] = au32ExpoValue[i];
    }

    for (i = 0; i < SYNC_WDR_BLC_COMP_NUM; i++) {
        pstDrvCtx->stIspBeSyncPara.wdr_blc_comp[i] = au32BlcComp[i];
    }

    if ((IS_2to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) || (IS_3to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) || (IS_4to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))) {

        u32MaxRatio = ((1 << 22) - 1) / DIV_0_TO_1(au32ExpoValue[0]);

        ISP_DRV_SetWdrMaxRatio(apstBeReg[u8BlkDev], u32MaxRatio);

        pstDrvCtx->stIspBeSyncPara.wdr_max_ratio = u32MaxRatio;

        ISP_DRV_SetWdrFusionMode(apstBeReg[u8BlkDev], pstWDRRegCfg->bFusionMode);

        pstDrvCtx->stIspBeSyncPara.fusion_mode = pstWDRRegCfg->bFusionMode;
    }


    u8CfgDlyMax = ISP_DRV_GetBeSyncIndex(pstDrvCtx);
    u8CfgDlyMax = MIN2(u8CfgDlyMax, CFG2VLD_DLY_LIMIT - 1);
    u8LFMode = pstDrvCtx->stSyncCfg.u8LFMode[u8CfgDlyMax];

    u16LongThr  = pstWDRRegCfg->u16LongThr;
    u16ShortThr = pstWDRRegCfg->u16ShortThr;
    bWDRMdtEn   = pstWDRRegCfg->bWDRMdtEn;

    if ((u8LFMode != 0) && (au32ExpoValue[0] < 0x44)) {
        u16LongThr  = 0x3FFF;
        u16ShortThr = 0x3FFF;
        bWDRMdtEn   = 0;
    }

    ISP_DRV_SetWdrLongThr(apstBeReg[u8BlkDev], u16LongThr);
    ISP_DRV_SetWdrShortThr(apstBeReg[u8BlkDev], u16ShortThr);
    ISP_DRV_SetWdrMdtEn(apstBeReg[u8BlkDev], bWDRMdtEn);
    pstDrvCtx->stIspBeSyncPara.long_thr  = u16LongThr;
    pstDrvCtx->stIspBeSyncPara.short_thr = u16ShortThr;
    pstDrvCtx->stIspBeSyncPara.wdr_mdt_en   = bWDRMdtEn;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfigDe(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], isp_sync_cfg_buf_node *pstCfgNode)
{
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfigLdci(S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, HI_U8 u8BlkNum, HI_U8 u8BlkDev)
{
    HI_U8   k;
    HI_U32  u32LdciComp = 0x1000;
    HI_U32  u32LdciCompIndex = 2;

    ISP_CHECK_POINTER(pstDrvCtx);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);
    }

    u32LdciCompIndex = ISP_DRV_GetBeSyncIndex(pstDrvCtx);
    if (u32LdciCompIndex >= 1) {
        u32LdciCompIndex = u32LdciCompIndex - 1; // ldci compensate is earlier tham drc one frame
    } else {
        u32LdciCompIndex = 0;
    }
    u32LdciCompIndex = MIN2(u32LdciCompIndex, CFG2VLD_DLY_LIMIT - 1);

    u32LdciComp = pstDrvCtx->stSyncCfg.u32DRCComp[u32LdciCompIndex];
    u32LdciComp = Sqrt32(u32LdciComp << DRC_COMP_SHIFT);
    u32LdciComp = MIN2(u32LdciComp, 0xFFFF);
    if (pstDrvCtx->stSnapInfoLoad.snap_state == SNAP_STATE_CFG) {
        u32LdciComp = 0x1000;
    }

    for (k = 0; k < u8BlkNum; k++) {
        ISP_DRV_SetLdciStatEvratio(apstBeReg[k + u8BlkDev], u32LdciComp);
    }

    pstDrvCtx->stIspBeSyncPara.ldci_comp = u32LdciComp;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfigDrc(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, ISP_DRC_REG_CFG_2_S *pstDRCRegCfg, HI_U8 u8BlkNum, HI_U8 u8BlkDev, HI_U32 *u32Ratio)
{
    HI_U8  k;
    HI_U8  i;
    HI_U8  u8CfgDlyMax;
    HI_U32 u32DrcDivDenomLog;
    HI_U32 u32DrcDenomExp;
    HI_U32 u32DrcExpRatio = 0x1000;
    HI_U32 au32DrcPrevLuma[SYNC_DRC_PRELUMA_NUM] = {0};
    HI_BOOL bUpdateLogParam = HI_FALSE;

    HI_U32 au32DrcDivDenomLog[16] = {
        52429, 55188,  58254,  61681,  65536,  69905,  74898, 80659, \
        87379, 95319, 104843, 116472, 130980, 149557, 174114, 207870
    };

    HI_U32 au32DrcDenomExp[16] = {
        1310720, 1245184, 1179648, 1114113, 1048577, 983043, 917510, 851980, \
        786455,  720942,  655452,  590008,  524657, 459488, 394682, 330589
    };

    static HI_U8 u8DrcShpLog[ISP_MAX_PIPE_NUM][ISP_STRIPING_MAX_NUM] = \
    {[0 ... ISP_MAX_PIPE_NUM - 1] = {8, 8}};
    static HI_U8 u8DrcShpExp[ISP_MAX_PIPE_NUM][ISP_STRIPING_MAX_NUM] = \
    {[0 ... ISP_MAX_PIPE_NUM - 1] = {8, 8}};

    ISP_CHECK_POINTER(pstDrvCtx);
    ISP_CHECK_POINTER(pstDRCRegCfg);
    ISP_CHECK_POINTER(u32Ratio);
    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);
    }

    /* config drc compensation */
    u8CfgDlyMax = ISP_DRV_GetBeSyncIndex(pstDrvCtx);
    u8CfgDlyMax = MIN2(u8CfgDlyMax, CFG2VLD_DLY_LIMIT - 1);

    u32DrcExpRatio = pstDrvCtx->stSyncCfg.u32DRCComp[u8CfgDlyMax];

    if (u32DrcExpRatio != 0x1000) { // do division only when u32DrcExpRatio != 4096
        u32DrcExpRatio = DIV_0_TO_1(u32DrcExpRatio);
        u32DrcExpRatio = osal_div_u64((1 << (DRC_COMP_SHIFT + DRC_COMP_SHIFT)), u32DrcExpRatio);
        u32DrcExpRatio = MIN2(u32DrcExpRatio, (15 << DRC_COMP_SHIFT)); // Maximum supported ratio is 15
    }

    if (pstDrvCtx->stSnapInfoLoad.snap_state == SNAP_STATE_CFG) {
        u32DrcExpRatio = 0x1000;
    }

    if (u8DrcShpLog[ViPipe][u8BlkDev] != pstDRCRegCfg->u8ShpLog \
        || u8DrcShpExp[ViPipe][u8BlkDev] != pstDRCRegCfg->u8ShpExp) {
        u8DrcShpLog[ViPipe][u8BlkDev] = pstDRCRegCfg->u8ShpLog;
        u8DrcShpExp[ViPipe][u8BlkDev] = pstDRCRegCfg->u8ShpExp;
        bUpdateLogParam = HI_TRUE;
    } else {
        bUpdateLogParam = HI_FALSE;
    }

    // Compensate on PrevLuma when ShpLog/ShpExp is modified, but no compensation under offline repeat mode
    if (bUpdateLogParam && (!pstDRCRegCfg->bIsOfflineRepeatMode)) {
        for (i = 0; i < SYNC_DRC_PRELUMA_NUM - 1; i++) {
            au32DrcPrevLuma[i] = (HI_U32)((HI_S32)g_DrcCurLumaLut[u8DrcShpLog[ViPipe][u8BlkDev]][i] + pstDRCRegCfg->as32PrevLumaDelta[i]);
        }
    } else {
        for (i = 0; i < SYNC_DRC_PRELUMA_NUM - 1; i++) {
            au32DrcPrevLuma[i] = g_DrcCurLumaLut[u8DrcShpLog[ViPipe][u8BlkDev]][i];
        }
    }
    au32DrcPrevLuma[SYNC_DRC_PRELUMA_NUM - 1] = (1 << 20);

    if ((u32DrcExpRatio != 0x1000) && (!pstDRCRegCfg->bIsOfflineRepeatMode)) {
        for (i = 0; i < SYNC_DRC_PRELUMA_NUM; i++) {
            au32DrcPrevLuma[i] = (HI_U32)(((HI_U64)u32DrcExpRatio * au32DrcPrevLuma[i]) >> DRC_COMP_SHIFT);
        }
    }

    u32DrcDivDenomLog = au32DrcDivDenomLog[u8DrcShpLog[ViPipe][u8BlkDev]];
    u32DrcDenomExp = au32DrcDenomExp[u8DrcShpExp[ViPipe][u8BlkDev]];
    for (k = 0; k < u8BlkNum; k++) {
        ISP_DRV_SetDrcShpCfg(apstBeReg[k + u8BlkDev], u8DrcShpLog[ViPipe][u8BlkDev], u8DrcShpExp[ViPipe][u8BlkDev]);
        ISP_DRV_SetDrcDivDenomLog(apstBeReg[k + u8BlkDev], u32DrcDivDenomLog);
        ISP_DRV_SetDrcDenomExp(apstBeReg[k + u8BlkDev], u32DrcDenomExp);

        ISP_DRV_SetDrcPrevLuma0(apstBeReg[k + u8BlkDev], au32DrcPrevLuma[0]);
        ISP_DRV_SetDrcPrevLuma1(apstBeReg[k + u8BlkDev], au32DrcPrevLuma[1]);
        ISP_DRV_SetDrcPrevLuma2(apstBeReg[k + u8BlkDev], au32DrcPrevLuma[2]);
        ISP_DRV_SetDrcPrevLuma3(apstBeReg[k + u8BlkDev], au32DrcPrevLuma[3]);
        ISP_DRV_SetDrcPrevLuma4(apstBeReg[k + u8BlkDev], au32DrcPrevLuma[4]);
        ISP_DRV_SetDrcPrevLuma5(apstBeReg[k + u8BlkDev], au32DrcPrevLuma[5]);
        ISP_DRV_SetDrcPrevLuma6(apstBeReg[k + u8BlkDev], au32DrcPrevLuma[6]);
        ISP_DRV_SetDrcPrevLuma7(apstBeReg[k + u8BlkDev], au32DrcPrevLuma[7]);
    }

    for (i = 0; i < SYNC_DRC_PRELUMA_NUM; i++) {
        pstDrvCtx->stIspBeSyncPara.drc_prev_luma[i] = au32DrcPrevLuma[i];
    }
    pstDrvCtx->stIspBeSyncPara.drc_shp_log = u8DrcShpLog[ViPipe][u8BlkDev];
    pstDrvCtx->stIspBeSyncPara.drc_div_denom_log = u32DrcDivDenomLog;
    pstDrvCtx->stIspBeSyncPara.drc_denom_exp = u32DrcDenomExp;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfigDgain(S_ISPBE_REGS_TYPE *apstBeReg[], HI_U32  u32IspDgain, HI_U8 u8BlkNum, HI_U8 u8BlkDev)
{
    HI_U8 k;

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);
        ISP_DRV_SetIspDgain(apstBeReg[k + u8BlkDev], u32IspDgain);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ConfigCCM(ISP_DRV_CTX_S *pstDrvCtx, HI_U16 *au16CCM)
{
    HI_U8 i;

    ISP_CHECK_POINTER(pstDrvCtx);

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        pstDrvCtx->stIspBeSyncPara.ccm[i] = au16CCM[i];
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfig4Dgain(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, HI_U32 *au32WDRGain, HI_U8 u8BlkNum, HI_U8 u8BlkDev, HI_U32 *au32WDRWBGain, HI_U32 *au32WBViGain)
{
    HI_U8 k;
    HI_U32 au32WDRGainAWB[4][4] = {0};

    for (k = 0; k < 4; k++) {
        au32WDRGainAWB[k][0] = (au32WDRGain[k] * au32WDRWBGain[0] + 0x80) >> 8;
        au32WDRGainAWB[k][1] = (au32WDRGain[k] * au32WDRWBGain[1] + 0x80) >> 8;
        au32WDRGainAWB[k][2] = (au32WDRGain[k] * au32WDRWBGain[2] + 0x80) >> 8;
        au32WDRGainAWB[k][3] = (au32WDRGain[k] * au32WDRWBGain[3] + 0x80) >> 8;
    }

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);
        ISP_DRV_SetIsp4Dgain0(apstBeReg[k + u8BlkDev], au32WDRGainAWB[0]);
        ISP_DRV_SetIsp4Dgain1(apstBeReg[k + u8BlkDev], au32WDRGainAWB[1]);
        ISP_DRV_SetIsp4Dgain2(apstBeReg[k + u8BlkDev], au32WDRGainAWB[2]);
        ISP_DRV_SetIsp4Dgain3(apstBeReg[k + u8BlkDev], au32WDRGainAWB[3]);
    }

    for (k = 0; k < ISP_BAYER_CHN_NUM; k++) {
        pstDrvCtx->stIspBeSyncPara.wdr_gain[0][k] = au32WDRGainAWB[0][k];
        pstDrvCtx->stIspBeSyncPara.wdr_gain[1][k] = au32WDRGainAWB[1][k];
        pstDrvCtx->stIspBeSyncPara.wdr_gain[2][k] = au32WDRGainAWB[2][k];
        pstDrvCtx->stIspBeSyncPara.wdr_gain[3][k] = au32WDRGainAWB[3][k];
        pstDrvCtx->stIspBeSyncPara.wdr_wb_gain[k] = au32WBViGain[k];
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfigAwbEn(VI_PIPE ViPipe, HI_BOOL bAwbEn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 k;
    isp_be_regs_attr  stIspBeRegsAttr = {0};
    S_VIPROC_REGS_TYPE *apstViprocReg[ISP_STRIPING_MAX_NUM] = {HI_NULL};

    s32Ret = ISP_DRV_GetViprocRegsAttr(ViPipe, apstViprocReg, &stIspBeRegsAttr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Get ViprocRegs Attr Err!\n", ViPipe);
        return s32Ret;
    }

    for (k = 0; k < stIspBeRegsAttr.block_num; k++) {
        ISP_CHECK_POINTER(apstViprocReg[k + stIspBeRegsAttr.block_dev]);
        ISP_DRV_AwbEnable(apstViprocReg[k + stIspBeRegsAttr.block_dev], bAwbEn);
    }

    return HI_SUCCESS;
}

/* read FE statistics information */
HI_S32 ISP_DRV_FE_AE_StatisticsRead(isp_stat *pstStat, S_ISPFE_REGS_TYPE *pstFeReg, HI_U32 k)
{
    HI_S32 i, j;
    HI_U32 u32AveMem = 0;

    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_POINTER(pstFeReg);

    pstFeReg->ISP_AE1_HIST_RADDR.u32 = 0x0;

    for (i = 0; i < 1024; i++) {
        pstStat->stFEAeStat1.au32HistogramMemArray[k][i] = pstFeReg->ISP_AE1_HIST_RDATA.u32;
    }

    pstStat->stFEAeStat1.u32PixelWeight[k] = pstFeReg->ISP_AE1_COUNT_STAT.u32;
    pstStat->stFEAeStat1.u32PixelCount[k]  = pstFeReg->ISP_AE1_TOTAL_STAT.u32;

    pstStat->stFEAeStat2.u16GlobalAvgR[k]  = pstFeReg->ISP_AE1_TOTAL_R_AVER.u32;
    pstStat->stFEAeStat2.u16GlobalAvgGr[k] = pstFeReg->ISP_AE1_TOTAL_GR_AVER.u32;
    pstStat->stFEAeStat2.u16GlobalAvgGb[k] = pstFeReg->ISP_AE1_TOTAL_GB_AVER.u32;
    pstStat->stFEAeStat2.u16GlobalAvgB[k]  = pstFeReg->ISP_AE1_TOTAL_B_AVER.u32;

    pstFeReg->ISP_AE1_AVER_R_GR_RADDR.u32 = 0x0;
    pstFeReg->ISP_AE1_AVER_GB_B_RADDR.u32 = 0x0;

    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            u32AveMem = pstFeReg->ISP_AE1_AVER_R_GR_RDATA.u32;
            pstStat->stFEAeStat3.au16ZoneAvg[k][i][j][0] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
            pstStat->stFEAeStat3.au16ZoneAvg[k][i][j][1] = (HI_U16)((u32AveMem & 0xFFFF));

            u32AveMem = pstFeReg->ISP_AE1_AVER_GB_B_RDATA.u32;
            pstStat->stFEAeStat3.au16ZoneAvg[k][i][j][2] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
            pstStat->stFEAeStat3.au16ZoneAvg[k][i][j][3] = (HI_U16)((u32AveMem & 0xFFFF));
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_FE_AE_Global_StatisticsRead(isp_stat *pstStat, S_ISPFE_REGS_TYPE *pstFeReg, HI_U32 k)
{
    HI_S32 i;

    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_POINTER(pstFeReg);

    pstFeReg->ISP_AE1_HIST_RADDR.u32 = 0x0;

    for (i = 0; i < 1024; i++) {
        pstStat->stFEAeStat1.au32HistogramMemArray[k][i] = pstFeReg->ISP_AE1_HIST_RDATA.u32;
    }

    pstStat->stFEAeStat1.u32PixelWeight[k] = pstFeReg->ISP_AE1_COUNT_STAT.u32;
    pstStat->stFEAeStat1.u32PixelCount[k]  = pstFeReg->ISP_AE1_TOTAL_STAT.u32;

    pstStat->stFEAeStat2.u16GlobalAvgR[k]  = pstFeReg->ISP_AE1_TOTAL_R_AVER.u32;
    pstStat->stFEAeStat2.u16GlobalAvgGr[k] = pstFeReg->ISP_AE1_TOTAL_GR_AVER.u32;
    pstStat->stFEAeStat2.u16GlobalAvgGb[k] = pstFeReg->ISP_AE1_TOTAL_GB_AVER.u32;
    pstStat->stFEAeStat2.u16GlobalAvgB[k]  = pstFeReg->ISP_AE1_TOTAL_B_AVER.u32;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_FE_AE_Local_StatisticsRead(isp_stat *pstStat, S_ISPFE_REGS_TYPE *pstFeReg, HI_U32 k)
{
    HI_S32 i, j;
    HI_U32 u32AveMem = 0;

    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_POINTER(pstFeReg);

    pstFeReg->ISP_AE1_AVER_R_GR_RADDR.u32 = 0x0;
    pstFeReg->ISP_AE1_AVER_GB_B_RADDR.u32 = 0x0;

    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            u32AveMem = pstFeReg->ISP_AE1_AVER_R_GR_RDATA.u32;
            pstStat->stFEAeStat3.au16ZoneAvg[k][i][j][0] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
            pstStat->stFEAeStat3.au16ZoneAvg[k][i][j][1] = (HI_U16)((u32AveMem & 0xFFFF));

            u32AveMem = pstFeReg->ISP_AE1_AVER_GB_B_RDATA.u32;
            pstStat->stFEAeStat3.au16ZoneAvg[k][i][j][2] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
            pstStat->stFEAeStat3.au16ZoneAvg[k][i][j][3] = (HI_U16)((u32AveMem & 0xFFFF));
        }
    }

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_FE_AF_StatisticsRead(isp_stat *pstStat, S_ISPFE_REGS_TYPE *pstFeReg, VI_PIPE ViPipeBind, HI_U32 k)
{
    HI_S32 i, j;
    HI_U8  u8AfEnbale[ISP_MAX_PIPE_NUM] = {ISP_PIPE_FEAF_MODULE_ENABLE};
    HI_U32 u32FEAfStatData = 0;
    HI_U32 u32Zone;
    HI_U32 u32Col;
    HI_U32 u32Row;

    ISP_CHECK_PIPE(ViPipeBind);
    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_POINTER(pstFeReg);

    u32Zone = pstFeReg->ISP_AF1_ZONE.u32;
    u32Col  = MIN2((u32Zone & 0x1F), AF_ZONE_COLUMN);
    u32Row  = MIN2(((u32Zone & 0x1F00) >> 8), AF_ZONE_ROW);

    if (!u8AfEnbale[ViPipeBind]) {
        for (i = 0; i < u32Row; i++) {
            for (j = 0; j < u32Col; j++) {
                pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16h1 = 0;
                pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16h2 = 0;
                pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16v1 = 0;
                pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16v2 = 0;
                pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16HlCnt = 0;
                pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16y  = 0;
            }
        }

        return HI_SUCCESS;
    }

    pstFeReg->ISP_AF1_STAT_H1_RADDR.u32 = 0;
    pstFeReg->ISP_AF1_STAT_H2_RADDR.u32 = 0;
    pstFeReg->ISP_AF1_STAT_V1_RADDR.u32 = 0;
    pstFeReg->ISP_AF1_STAT_V2_RADDR.u32 = 0;
    pstFeReg->ISP_AF1_STAT_Y_RADDR.u32  = 0;

    for (i = 0; i < u32Row; i++) {
        for (j = 0; j < u32Col; j++) {
            u32FEAfStatData = pstFeReg->ISP_AF1_STAT_H1_RDATA.u32;
            pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16h1 = (HI_U16)(0xFFFF & u32FEAfStatData);
            u32FEAfStatData = pstFeReg->ISP_AF1_STAT_H2_RDATA.u32;
            pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16h2 = (HI_U16)(0xFFFF & u32FEAfStatData);
            u32FEAfStatData = pstFeReg->ISP_AF1_STAT_V1_RDATA.u32;
            pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16v1 = (HI_U16)(0xFFFF & u32FEAfStatData);
            u32FEAfStatData = pstFeReg->ISP_AF1_STAT_V2_RDATA.u32;
            pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16v2 = (HI_U16)(0xFFFF & u32FEAfStatData);
            u32FEAfStatData = pstFeReg->ISP_AF1_STAT_Y_RDATA.u32;
            pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16HlCnt = (HI_U16)((0xFFFF0000 & u32FEAfStatData) >> 16);
            pstStat->stFEAfStat.stZoneMetrics[k][i][j].u16y     = (HI_U16)(0xFFFF & u32FEAfStatData);
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_FE_APB_StatisticsRead(isp_stat *pstStat, S_ISPFE_REGS_TYPE *pstFeReg, VI_PIPE ViPipeBind, HI_U32 k, isp_stat_key unStatkey)
{
    if (unStatkey.bit1FEAeGloStat) {
        ISP_DRV_FE_AE_Global_StatisticsRead(pstStat, pstFeReg, k);
    }

    if (unStatkey.bit1FEAeLocStat) {
        ISP_DRV_FE_AE_Local_StatisticsRead(pstStat, pstFeReg, k);
    }

    if (unStatkey.bit1FEAfStat) {
        ISP_DRV_FE_AF_StatisticsRead(pstStat, pstFeReg, ViPipeBind, k);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_FE_STT_StatisticsRead(isp_stat *pstStat, S_ISPFE_REGS_TYPE *pstFeReg, ISP_DRV_CTX_S *pstDrvCtx, VI_PIPE ViPipeBind, HI_U32 k, isp_stat_key unStatkey)
{
    return HI_SUCCESS;
}

HI_S32  ISP_DRV_FE_StitchStatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo)
{
    HI_U8   u8StitchNum;
    HI_U32  i, j, k, l;
    HI_U32  u32AveMem = 0;
    HI_U64  au64GlobalAvgSumR[ISP_WDR_CHN_MAX]  = {0};
    HI_U64  au64GlobalAvgSumGr[ISP_WDR_CHN_MAX] = {0};
    HI_U64  au64GlobalAvgSumGb[ISP_WDR_CHN_MAX] = {0};
    HI_U64  au64GlobalAvgSumB[ISP_WDR_CHN_MAX]  = {0};
    HI_U32  u32PixelWeight = 0;
    HI_U32  u32PixelWeightTmp = 0;
    VI_PIPE StitPipeBind, WdrPipeBind;

    ISP_DRV_CTX_S *pstDrvCtx     = HI_NULL;
    ISP_DRV_CTX_S *pstDrvBindCtx = HI_NULL;
    isp_stat    *pstStat       = HI_NULL;
    S_ISPFE_REGS_TYPE *pstFeReg  = HI_NULL;
    isp_stat_key unStatkey;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    pstStat = (isp_stat *)pstStatInfo->virt_addr;

    if (pstStat == HI_NULL) {
        return HI_FAILURE;
    }

    unStatkey.key = pstStatInfo->stat_key.bit32IsrAccess;

    if ((pstDrvCtx->stStitchAttr.stitch_enable != HI_TRUE) || (pstDrvCtx->stStitchAttr.main_pipe != HI_TRUE)) {
        return HI_FAILURE;
    }

    u8StitchNum = pstDrvCtx->stStitchAttr.stitch_pipe_num;

    if (unStatkey.bit1FEAeStiGloStat) {
        for (k = 0; k < u8StitchNum; k++) {
            StitPipeBind = pstDrvCtx->stStitchAttr.stitch_bind_id[k];

            ISP_CHECK_PIPE(StitPipeBind);

            pstDrvBindCtx = ISP_DRV_GET_CTX(StitPipeBind);

            for (j = 0; j < pstDrvCtx->stWDRAttr.dev_bind_pipe.num; j++) {
                WdrPipeBind = pstDrvBindCtx->stWDRAttr.dev_bind_pipe.pipe_id[j];
                ISP_CHECK_PIPE(WdrPipeBind);

                ISP_DRV_FEREG_CTX(WdrPipeBind, pstFeReg);

                pstFeReg->ISP_AE1_HIST_RADDR.u32 = 0x0;

                if (k == 0) {
                    for (i = 0; i < 1024; i++) {
                        pstStat->stitch_stat.stFEAeStat1.au32HistogramMemArray[j][i] = pstFeReg->ISP_AE1_HIST_RDATA.u32;
                    }

                    u32PixelWeightTmp = pstFeReg->ISP_AE1_COUNT_STAT.u32;
                    pstStat->stitch_stat.stFEAeStat1.u32PixelCount[j]  = u32PixelWeightTmp;
                    pstStat->stitch_stat.stFEAeStat1.u32PixelWeight[j] = pstFeReg->ISP_AE1_TOTAL_STAT.u32;
                } else {
                    for (i = 0; i < 1024; i++) {
                        pstStat->stitch_stat.stFEAeStat1.au32HistogramMemArray[j][i] += pstFeReg->ISP_AE1_HIST_RDATA.u32;
                    }

                    u32PixelWeightTmp = pstFeReg->ISP_AE1_COUNT_STAT.u32;
                    pstStat->stitch_stat.stFEAeStat1.u32PixelCount[j]  += u32PixelWeightTmp;
                    pstStat->stitch_stat.stFEAeStat1.u32PixelWeight[j] += pstFeReg->ISP_AE1_TOTAL_STAT.u32;
                }

                au64GlobalAvgSumR[j]  += (HI_U64)pstFeReg->ISP_AE1_TOTAL_R_AVER.u32  * u32PixelWeightTmp;
                au64GlobalAvgSumGr[j] += (HI_U64)pstFeReg->ISP_AE1_TOTAL_GR_AVER.u32 * u32PixelWeightTmp;
                au64GlobalAvgSumGb[j] += (HI_U64)pstFeReg->ISP_AE1_TOTAL_GB_AVER.u32 * u32PixelWeightTmp;
                au64GlobalAvgSumB[j]  += (HI_U64)pstFeReg->ISP_AE1_TOTAL_B_AVER.u32  * u32PixelWeightTmp;
            }
        }

        for (j = 0; j < pstDrvCtx->stWDRAttr.dev_bind_pipe.num; j++) {
            u32PixelWeight = DIV_0_TO_1(pstStat->stitch_stat.stFEAeStat1.u32PixelCount[j]);

            pstStat->stitch_stat.stFEAeStat2.u16GlobalAvgR[j]  = osal_div_u64(au64GlobalAvgSumR[j],  u32PixelWeight);
            pstStat->stitch_stat.stFEAeStat2.u16GlobalAvgGr[j] = osal_div_u64(au64GlobalAvgSumGr[j], u32PixelWeight);
            pstStat->stitch_stat.stFEAeStat2.u16GlobalAvgGb[j] = osal_div_u64(au64GlobalAvgSumGb[j], u32PixelWeight);
            pstStat->stitch_stat.stFEAeStat2.u16GlobalAvgB[j]  = osal_div_u64(au64GlobalAvgSumB[j],  u32PixelWeight);
        }
    }

    if (unStatkey.bit1FEAeStiLocStat) {
        for (k = 0; k < u8StitchNum; k++) {
            StitPipeBind = pstDrvCtx->stStitchAttr.stitch_bind_id[k];

            ISP_CHECK_PIPE(StitPipeBind);

            pstDrvBindCtx = ISP_DRV_GET_CTX(StitPipeBind);

            for (j = 0; j < pstDrvCtx->stWDRAttr.dev_bind_pipe.num; j++) {
                WdrPipeBind = pstDrvBindCtx->stWDRAttr.dev_bind_pipe.pipe_id[j];
                ISP_CHECK_PIPE(WdrPipeBind);

                ISP_DRV_FEREG_CTX(WdrPipeBind, pstFeReg);

                pstFeReg->ISP_AE1_AVER_R_GR_RADDR.u32 = 0x0;
                pstFeReg->ISP_AE1_AVER_GB_B_RADDR.u32 = 0x0;

                for (i = 0; i < AE_ZONE_ROW; i++) {
                    for (l = 0; l < AE_ZONE_COLUMN; l++) {
                        u32AveMem = pstFeReg->ISP_AE1_AVER_R_GR_RDATA.u32;
                        pstStat->stitch_stat.stFEAeStat3.au16ZoneAvg[StitPipeBind][j][i][l][0] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                        pstStat->stitch_stat.stFEAeStat3.au16ZoneAvg[StitPipeBind][j][i][l][1] = (HI_U16)((u32AveMem & 0xFFFF));

                        u32AveMem = pstFeReg->ISP_AE1_AVER_GB_B_RDATA.u32;
                        pstStat->stitch_stat.stFEAeStat3.au16ZoneAvg[StitPipeBind][j][i][l][2] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                        pstStat->stitch_stat.stFEAeStat3.au16ZoneAvg[StitPipeBind][j][i][l][3] = (HI_U16)((u32AveMem & 0xFFFF));
                    }
                }
            }
        }
    }

    return HI_SUCCESS;
}

/* read BE statistics information from phy:online */
HI_S32 ISP_DRV_BE_COMM_StatisticsRead(isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkDev)
{
    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_POINTER(apstBeReg[u8BlkDev]);

    pstStat->stCommStat.au16WhiteBalanceGain[0] = apstBeReg[u8BlkDev]->ISP_WB_GAIN1.bits.isp_wb_rgain;
    pstStat->stCommStat.au16WhiteBalanceGain[1] = apstBeReg[u8BlkDev]->ISP_WB_GAIN1.bits.isp_wb_grgain;
    pstStat->stCommStat.au16WhiteBalanceGain[2] = apstBeReg[u8BlkDev]->ISP_WB_GAIN2.bits.isp_wb_gbgain;
    pstStat->stCommStat.au16WhiteBalanceGain[3] = apstBeReg[u8BlkDev]->ISP_WB_GAIN2.bits.isp_wb_bgain;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_AE_StatisticsRead(isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev)
{
    HI_U8  k;
    HI_U8  u8BlockOffset    = 0;
    HI_U8  u8BlockZoneWidth = 0;
    HI_U32 i, j;
    HI_U32 u32AveMem;
    HI_U64 u64GlobalAvgSumR  = 0;
    HI_U64 u64GlobalAvgSumGr = 0;
    HI_U64 u64GlobalAvgSumGb = 0;
    HI_U64 u64GlobalAvgSumB  = 0;
    HI_U32 u32PixelWeightTmp = 0;
    HI_U32 u32PixelWeight    = 0;

    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);

        apstBeReg[k + u8BlkDev]->ISP_AE_HIST_RADDR.u32 = 0x0;

        for (i = 0; i < 1024; i++) {
            if (k == 0) {
                pstStat->stBEAeStat1.au32HistogramMemArray[i]  = apstBeReg[k + u8BlkDev]->ISP_AE_HIST_RDATA.u32;
            } else {
                pstStat->stBEAeStat1.au32HistogramMemArray[i] += apstBeReg[k + u8BlkDev]->ISP_AE_HIST_RDATA.u32;
            }
        }

        if (k == 0) {
            u32PixelWeightTmp = apstBeReg[k + u8BlkDev]->ISP_AE_COUNT_STAT.bits.isp_ae_count_pixels;
            pstStat->stBEAeStat1.u32PixelWeight = u32PixelWeightTmp;
            pstStat->stBEAeStat1.u32PixelCount  = apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_STAT.bits.isp_ae_total_pixels;
        } else {
            u32PixelWeightTmp = apstBeReg[k + u8BlkDev]->ISP_AE_COUNT_STAT.bits.isp_ae_count_pixels;
            pstStat->stBEAeStat1.u32PixelWeight += u32PixelWeightTmp;
            pstStat->stBEAeStat1.u32PixelCount  += apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_STAT.bits.isp_ae_total_pixels;
        }

        u64GlobalAvgSumR  += ((HI_U64)apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_R_AVER.u32)  * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumGr += ((HI_U64)apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_GR_AVER.u32) * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumGb += ((HI_U64)apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_GB_AVER.u32) * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumB  += ((HI_U64)apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_B_AVER.u32)  * ((HI_U64)u32PixelWeightTmp);

        apstBeReg[k + u8BlkDev]->ISP_AE_AVER_R_GR_RADDR.u32 = 0x0;
        apstBeReg[k + u8BlkDev]->ISP_AE_AVER_GB_B_RADDR.u32 = 0x0;

        for (i = 0; i < AE_ZONE_ROW; i++) {
            if (k < (AE_ZONE_COLUMN % DIV_0_TO_1(u8BlkNum))) {
                u8BlockZoneWidth = (AE_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum)) + 1;
            } else {
                u8BlockZoneWidth = AE_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum);
            }

            for (j = 0; j < u8BlockZoneWidth; j++) {
                u32AveMem = apstBeReg[k + u8BlkDev]->ISP_AE_AVER_R_GR_RDATA.u32;
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][0] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][1] = (HI_U16)((u32AveMem & 0xFFFF));

                u32AveMem = apstBeReg[k + u8BlkDev]->ISP_AE_AVER_GB_B_RDATA.u32;
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][2] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][3] = (HI_U16)((u32AveMem & 0xFFFF));
            }
        }

        u8BlockOffset += u8BlockZoneWidth;
    }

    u32PixelWeight = DIV_0_TO_1(pstStat->stBEAeStat1.u32PixelWeight);

    pstStat->stBEAeStat2.u16GlobalAvgR  = osal_div_u64(u64GlobalAvgSumR,  u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgGr = osal_div_u64(u64GlobalAvgSumGr, u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgGb = osal_div_u64(u64GlobalAvgSumGb, u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgB  = osal_div_u64(u64GlobalAvgSumB,  u32PixelWeight);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_AE_Global_StatisticsRead(isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev)
{
    HI_U32 i, k;
    HI_U64 u64GlobalAvgSumR  = 0;
    HI_U64 u64GlobalAvgSumGr = 0;
    HI_U64 u64GlobalAvgSumGb = 0;
    HI_U64 u64GlobalAvgSumB  = 0;
    HI_U32 u32PixelWeightTmp = 0;
    HI_U32 u32PixelWeight    = 0;

    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);

        apstBeReg[k + u8BlkDev]->ISP_AE_HIST_RADDR.u32 = 0x0;

        for (i = 0; i < 1024; i++) {
            if (k == 0) {
                pstStat->stBEAeStat1.au32HistogramMemArray[i]  = apstBeReg[k + u8BlkDev]->ISP_AE_HIST_RDATA.u32;
            } else {
                pstStat->stBEAeStat1.au32HistogramMemArray[i] += apstBeReg[k + u8BlkDev]->ISP_AE_HIST_RDATA.u32;
            }
        }

        if (k == 0) {
            u32PixelWeightTmp = apstBeReg[k + u8BlkDev]->ISP_AE_COUNT_STAT.bits.isp_ae_count_pixels;
            pstStat->stBEAeStat1.u32PixelWeight = u32PixelWeightTmp;
            pstStat->stBEAeStat1.u32PixelCount  = apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_STAT.bits.isp_ae_total_pixels;
        } else {
            u32PixelWeightTmp = apstBeReg[k + u8BlkDev]->ISP_AE_COUNT_STAT.bits.isp_ae_count_pixels;
            pstStat->stBEAeStat1.u32PixelWeight += u32PixelWeightTmp;
            pstStat->stBEAeStat1.u32PixelCount  += apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_STAT.bits.isp_ae_total_pixels;
        }

        u64GlobalAvgSumR  += ((HI_U64)apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_R_AVER.u32)  * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumGr += ((HI_U64)apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_GR_AVER.u32) * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumGb += ((HI_U64)apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_GB_AVER.u32) * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumB  += ((HI_U64)apstBeReg[k + u8BlkDev]->ISP_AE_TOTAL_B_AVER.u32)  * ((HI_U64)u32PixelWeightTmp);
    }

    u32PixelWeight = DIV_0_TO_1(pstStat->stBEAeStat1.u32PixelWeight);

    pstStat->stBEAeStat2.u16GlobalAvgR  = osal_div_u64(u64GlobalAvgSumR,  u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgGr = osal_div_u64(u64GlobalAvgSumGr, u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgGb = osal_div_u64(u64GlobalAvgSumGb, u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgB  = osal_div_u64(u64GlobalAvgSumB,  u32PixelWeight);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_AE_Local_StatisticsRead(isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev)
{
    HI_U32 i, j, k;
    HI_U8  u8BlockOffset    = 0;
    HI_U8  u8BlockZoneWidth = 0;
    HI_U32 u32AveMem;

    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);

        apstBeReg[k + u8BlkDev]->ISP_AE_AVER_R_GR_RADDR.u32 = 0x0;
        apstBeReg[k + u8BlkDev]->ISP_AE_AVER_GB_B_RADDR.u32 = 0x0;

        for (i = 0; i < AE_ZONE_ROW; i++) {
            if (k < (AE_ZONE_COLUMN % DIV_0_TO_1(u8BlkNum))) {
                u8BlockZoneWidth = (AE_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum)) + 1;
            } else {
                u8BlockZoneWidth = AE_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum);
            }

            for (j = 0; j < u8BlockZoneWidth; j++) {
                u32AveMem = apstBeReg[k + u8BlkDev]->ISP_AE_AVER_R_GR_RDATA.u32;
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][0] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][1] = (HI_U16)((u32AveMem & 0xFFFF));

                u32AveMem = apstBeReg[k + u8BlkDev]->ISP_AE_AVER_GB_B_RDATA.u32;
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][2] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][3] = (HI_U16)((u32AveMem & 0xFFFF));
            }
        }

        u8BlockOffset += u8BlockZoneWidth;
    }

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_BE_MG_StatisticsRead(isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev)
{
    HI_U8  k;
    HI_U8  u8BlockOffset     = 0;
    HI_U8  u8BlockZoneWidth  = 0;
    HI_U32 i, j;
    HI_U32 u32AveMem;

    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);

        apstBeReg[k + u8BlkDev]->ISP_LA_AVER_RADDR.u32 = 0x0;

        for (i = 0; i < MG_ZONE_ROW; i++) {
            if (k < (MG_ZONE_COLUMN % DIV_0_TO_1(u8BlkNum))) {
                u8BlockZoneWidth = (MG_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum)) + 1;
            } else {
                u8BlockZoneWidth = MG_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum);
            }

            for (j = 0; j < u8BlockZoneWidth; j++) {
                u32AveMem = apstBeReg[k + u8BlkDev]->ISP_LA_AVER_RDATA.u32;
                pstStat->stMgStat.au16ZoneAvg[i][j + u8BlockOffset][0] = (HI_U16)((u32AveMem & 0xFF000000) >> 24);
                pstStat->stMgStat.au16ZoneAvg[i][j + u8BlockOffset][1] = (HI_U16)((u32AveMem & 0xFF0000) >> 16);
                pstStat->stMgStat.au16ZoneAvg[i][j + u8BlockOffset][2] = (HI_U16)((u32AveMem & 0xFF00) >> 8);
                pstStat->stMgStat.au16ZoneAvg[i][j + u8BlockOffset][3] = (HI_U16)((u32AveMem & 0xFF));
            }
        }

        u8BlockOffset += u8BlockZoneWidth;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_AWB_StatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev, isp_stat_key unStatkey)
{
    HI_S32 i, j, k;
    HI_U8  u8BlockOffset = 0;
    HI_U32 u32Value;
    HI_U64 u64MeteringAwbAvgR = 0;
    HI_U64 u64MeteringAwbAvgG = 0;
    HI_U64 u64MeteringAwbAvgB = 0;
    HI_U32 u32MeteringAwbCountAll = 0;
    HI_U32 u32Zone;
    HI_U32 u32Col, u32Row;
    HI_U32 u32ZoneBin;
    HI_U32 u32WholeCol = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_sync_cfg_buf_node *pstNode = HI_NULL;
    HI_U32 WbGainBfStat[ISP_BAYER_CHN_NUM] = {0x100, 0x100, 0x100, 0x100};
    HI_U32 WDRWbGain[ISP_BAYER_CHN_NUM] = {0x100, 0x100, 0x100, 0x100};
    HI_U32 SensorWbGain[ISP_BAYER_CHN_NUM] = {0x100, 0x100, 0x100, 0x100};
    HI_U16 RgainConv = 0x100;
    HI_U16 GgainConv = 0x100;
    HI_U16 BgainConv = 0x100;
    HI_U32 MinGain, NormGain;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStat);

    u8BlockOffset = 0;

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);
        u32Zone = apstBeReg[k + u8BlkDev]->ISP_AWB_ZONE.u32;
        u32WholeCol  += MIN2((u32Zone & 0x3F), AWB_ZONE_ORIG_COLUMN);
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    pstNode = &(pstDrvCtx->stSyncCfg.stSyncCfgBuf.stSyncCfgBufNode[0]);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        WDRWbGain[i] = pstNode->stAWBRegCfg.au32WDRWBGain[i];
		SensorWbGain[i] = pstNode->stAWBRegCfg.au32SensorWBGain[i];
    }

    if (pstNode->stAWBRegCfg.enBEAWBSwitch == ISP_AWB_AFTER_DRC) {
         for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            WbGainBfStat[i] = pstNode->stAWBRegCfg.au32BEWhiteBalanceGain[i];
        }
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        WbGainBfStat[i] = (WbGainBfStat[i] * WDRWbGain[i]) >> 8;
        WbGainBfStat[i] = (WbGainBfStat[i] * SensorWbGain[i]) >> 8;
    }

    MinGain = 0xFFFF;
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        MinGain = MIN2(MinGain, WbGainBfStat[i]);
    }
    NormGain = 0x10000 / DIV_0_TO_1(MinGain);
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        WbGainBfStat[i] = (WbGainBfStat[i] * NormGain + 0x80) >> 8;
    }

    RgainConv = (0x10000 + (WbGainBfStat[0] >> 1)) / DIV_0_TO_1(WbGainBfStat[0]);
    GgainConv = (0x10000 + (WbGainBfStat[1] >> 1)) / DIV_0_TO_1(WbGainBfStat[1]);
    BgainConv = (0x10000 + (WbGainBfStat[3] >> 1)) / DIV_0_TO_1(WbGainBfStat[3]);


    for (k = 0; k < u8BlkNum; k++)
    {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);
        u32Zone = apstBeReg[k + u8BlkDev]->ISP_AWB_ZONE.u32;
        u32Col  = MIN2((u32Zone & 0x3F), AWB_ZONE_ORIG_COLUMN);
        u32Row  = MIN2(((u32Zone & 0x3F00) >> 8), AWB_ZONE_ORIG_ROW);
        u32ZoneBin = apstBeReg[k + u8BlkDev]->ISP_AWB_ZONE_BIN.u32;
        if (unStatkey.bit1AwbStat1) {
            u64MeteringAwbAvgR += (HI_U64)apstBeReg[k + u8BlkDev]->ISP_AWB_AVG_R.bits.isp_awb_avg_r * apstBeReg[k + u8BlkDev]->ISP_AWB_CNT_ALL.bits.isp_awb_count_all;
            u64MeteringAwbAvgG += (HI_U64)apstBeReg[k + u8BlkDev]->ISP_AWB_AVG_G.bits.isp_awb_avg_g * apstBeReg[k + u8BlkDev]->ISP_AWB_CNT_ALL.bits.isp_awb_count_all;
            u64MeteringAwbAvgB += (HI_U64)apstBeReg[k + u8BlkDev]->ISP_AWB_AVG_B.bits.isp_awb_avg_b * apstBeReg[k + u8BlkDev]->ISP_AWB_CNT_ALL.bits.isp_awb_count_all;
            u32MeteringAwbCountAll += apstBeReg[k + u8BlkDev]->ISP_AWB_CNT_ALL.bits.isp_awb_count_all;
        }

        if (unStatkey.bit1AwbStat2) {
            apstBeReg[k + u8BlkDev]->ISP_AWB_STAT_RADDR.u32 = 0x0;

            for (i = 0; i < u32Row; i++) {

                for (j = 0; j < u32Col; j++) {
                    HI_U16 u16WStartAddr = (i * u32WholeCol + j + u8BlockOffset) * u32ZoneBin;
                    HI_U16 m;
                    HI_U16 ReadRAvg, ReadGAvg, ReadBAvg;

                    for (m = 0; m < u32ZoneBin; m++) {
                        u32Value = apstBeReg[k + u8BlkDev]->ISP_AWB_STAT_RDATA.u32;
                        ReadRAvg = (u32Value & 0xFFFF);
                        ReadGAvg = ((u32Value >> 16) & 0xFFFF);
                        u32Value = apstBeReg[k + u8BlkDev]->ISP_AWB_STAT_RDATA.u32;
                        ReadBAvg = (u32Value & 0xFFFF);
                        pstStat->stAwbStat2.au16MeteringMemArrayCountAll[u16WStartAddr + m] = ((u32Value >> 16) & 0xFFFF);

                        pstStat->stAwbStat2.au16MeteringMemArrayAvgR[u16WStartAddr + m] = (ReadRAvg * RgainConv + 0x80) >> 8;
                        pstStat->stAwbStat2.au16MeteringMemArrayAvgG[u16WStartAddr + m] = (ReadGAvg * GgainConv + 0x80) >> 8;
                        pstStat->stAwbStat2.au16MeteringMemArrayAvgB[u16WStartAddr + m] = (ReadBAvg * BgainConv + 0x80) >> 8;
                    }
                }
            }
        }

        u8BlockOffset += u32Col;
    }

    pstStat->stAwbStat1.u16MeteringAwbAvgR = (HI_U16)(osal_div_u64(u64MeteringAwbAvgR, DIV_0_TO_1(u32MeteringAwbCountAll)));
    pstStat->stAwbStat1.u16MeteringAwbAvgG = (HI_U16)(osal_div_u64(u64MeteringAwbAvgG, DIV_0_TO_1(u32MeteringAwbCountAll)));
    pstStat->stAwbStat1.u16MeteringAwbAvgB = (HI_U16)(osal_div_u64(u64MeteringAwbAvgB, DIV_0_TO_1(u32MeteringAwbCountAll)));
    pstStat->stAwbStat1.u16MeteringAwbCountAll = (HI_U16)(u32MeteringAwbCountAll / DIV_0_TO_1(u8BlkNum));

    pstStat->stAwbStat1.u16MeteringAwbAvgR = (pstStat->stAwbStat1.u16MeteringAwbAvgR *  RgainConv + 0x80) >> 8;
    pstStat->stAwbStat1.u16MeteringAwbAvgG = (pstStat->stAwbStat1.u16MeteringAwbAvgG *  GgainConv + 0x80) >> 8;
    pstStat->stAwbStat1.u16MeteringAwbAvgB = (pstStat->stAwbStat1.u16MeteringAwbAvgB *  BgainConv + 0x80) >> 8;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_AF_StatisticsRead(isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev)
{
    HI_U8  u8ColIndex    = 0;
    HI_U8  u8BlockOffset = 0;
    HI_U32 i, j, k;
    HI_U32 u32BEAfStatData = 0;
    HI_U32 u32Zone, u32Col, u32Row;

    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);

        u32Zone = apstBeReg[k + u8BlkDev]->ISP_AF_ZONE.u32;
        u32Col  = MIN2((u32Zone & 0x1F), AF_ZONE_COLUMN);
        u32Row  = MIN2(((u32Zone & 0x1F00) >> 8), AF_ZONE_ROW);

        apstBeReg[k + u8BlkDev]->ISP_AF_STAT_H1_RADDR.u32 = 0;
        apstBeReg[k + u8BlkDev]->ISP_AF_STAT_H2_RADDR.u32 = 0;
        apstBeReg[k + u8BlkDev]->ISP_AF_STAT_V1_RADDR.u32 = 0;
        apstBeReg[k + u8BlkDev]->ISP_AF_STAT_V2_RADDR.u32 = 0;
        apstBeReg[k + u8BlkDev]->ISP_AF_STAT_Y_RADDR.u32  = 0;

        for (i = 0; i < u32Row; i++) {
            for (j = 0; j < u32Col; j++) {
                u8ColIndex = MIN2(j + u8BlockOffset, AF_ZONE_COLUMN);

                u32BEAfStatData = apstBeReg[k + u8BlkDev]->ISP_AF_STAT_H1_RDATA.u32;
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16h1 = (HI_U16)(0xFFFF & u32BEAfStatData);
                u32BEAfStatData = apstBeReg[k + u8BlkDev]->ISP_AF_STAT_H2_RDATA.u32;
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16h2 = (HI_U16)(0xFFFF & u32BEAfStatData);
                u32BEAfStatData = apstBeReg[k + u8BlkDev]->ISP_AF_STAT_V1_RDATA.u32;
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16v1 = (HI_U16)(0xFFFF & u32BEAfStatData);
                u32BEAfStatData = apstBeReg[k + u8BlkDev]->ISP_AF_STAT_V2_RDATA.u32;
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16v2 = (HI_U16)(0xFFFF & u32BEAfStatData);
                u32BEAfStatData = apstBeReg[k + u8BlkDev]->ISP_AF_STAT_Y_RDATA.u32;
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16HlCnt = (HI_U16)((0xFFFF0000 & u32BEAfStatData) >> 16);
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16y  = (HI_U16)(0xFFFF & u32BEAfStatData);
            }
        }

        u8BlockOffset += u32Col;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_DEHAZE_StatisticsRead(isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev)
{
    HI_U32 i, j, m;

    ISP_CHECK_POINTER(pstStat);

    j = DEFOG_ZONE_NUM / 2;

    for (i = 0; i < u8BlkNum; i++) {
        ISP_CHECK_POINTER(apstBeReg[i + u8BlkDev]);

        apstBeReg[i + u8BlkDev]->ISP_DEHAZE_MINSTAT_RADDR.u32 = 0x0;

        for (m = 0; m < j; m++) {
            pstStat->dehaze_stat.min_dout[i][m] = apstBeReg[i + u8BlkDev]->ISP_DEHAZE_MINSTAT_RDATA.u32;
        }

        apstBeReg[i + u8BlkDev]->ISP_DEHAZE_MAXSTAT_RADDR.u32 = 0x0;

        for (m = 0; m < DEFOG_ZONE_NUM; m++) {
            pstStat->dehaze_stat.max_stat_dout[i][m] = apstBeReg[i + u8BlkDev]->ISP_DEHAZE_MAXSTAT_RDATA.u32;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_FLICKER_StatisticsRead(isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev)
{
    HI_U8  k;
    HI_U16 j;

    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeReg[k + u8BlkDev]);

        pstStat->flicker_stat.cur_cnt_over[k] = apstBeReg[k + u8BlkDev]->ISP_FLICK_COUNTOVER_CUR.bits.isp_flick_countover_cur;
        pstStat->flicker_stat.cur_diff_gr[k]  = apstBeReg[k + u8BlkDev]->ISP_FLICK_GR_DIFF_CUR.bits.isp_flick_gr_diff_cur;
        pstStat->flicker_stat.cur_diff_gb[k]  = apstBeReg[k + u8BlkDev]->ISP_FLICK_GB_DIFF_CUR.bits.isp_flick_gb_diff_cur;
        pstStat->flicker_stat.cur_abs_gr[k]   = apstBeReg[k + u8BlkDev]->ISP_FLICK_GR_ABS_CUR.bits.isp_flick_gr_abs_cur;
        pstStat->flicker_stat.cur_abs_gb[k]   = apstBeReg[k + u8BlkDev]->ISP_FLICK_GB_ABS_CUR.bits.isp_flick_gb_abs_cur;

        apstBeReg[k + u8BlkDev]->ISP_FLICK_GMEAN_RADDR.u32 = 0;

        for (j = 0; j < ISP_FLICKER_G_MEAN_LENGTH; j++) {
            pstStat->flicker_stat.g_mean[k][j] = apstBeReg[k + u8BlkDev]->ISP_FLICK_GMEAN_RDATA.u32;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_APB_StatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev, isp_stat_key unStatkey)
{
    // BE comm statistics
    if (unStatkey.bit1CommStat) {
        ISP_DRV_BE_COMM_StatisticsRead(pstStat, apstBeReg, u8BlkDev);
    }

    // BE AE statistics
    if (unStatkey.bit1BEAeGloStat) {
        ISP_DRV_BE_AE_Global_StatisticsRead(pstStat, apstBeReg, u8BlkNum, u8BlkDev);
    }

    if (unStatkey.bit1BEAeLocStat) {
        ISP_DRV_BE_AE_Local_StatisticsRead(pstStat, apstBeReg, u8BlkNum, u8BlkDev);
    }

    if (unStatkey.bit1MgStat) {
        ISP_DRV_BE_MG_StatisticsRead(pstStat, apstBeReg, u8BlkNum, u8BlkDev);
    }

    /* BE AWB statistics */
    ISP_DRV_BE_AWB_StatisticsRead(ViPipe, pstStat, apstBeReg, u8BlkNum, u8BlkDev, unStatkey);

    /* BE AF statistics */
    if (unStatkey.bit1BEAfStat) {
        ISP_DRV_BE_AF_StatisticsRead(pstStat, apstBeReg, u8BlkNum, u8BlkDev);
    }
    if (unStatkey.bit1Dehaze) {
        ISP_DRV_BE_DEHAZE_StatisticsRead(pstStat, apstBeReg, u8BlkNum, u8BlkDev);
    }

    if (unStatkey.bit1Flicker) {
        ISP_DRV_BE_FLICKER_StatisticsRead(pstStat, apstBeReg, u8BlkNum, u8BlkDev);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_STT_StatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], HI_U8 u8BlkNum, HI_U8 u8BlkDev, isp_stat_key unStatkey)
{
    ISP_DRV_LdciOnlineAttrUpdate(ViPipe, pstStat);

    return HI_SUCCESS;
}

/* read BE statistics information:offline */
HI_S32 ISP_DRV_BE_AE_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum)
{
    HI_U8  k;
    HI_U8  u8BlockOffset    = 0;
    HI_U8  u8BlockZoneWidth = 0;
    HI_U32 i, j;
    HI_U32 u32AveMem;
    HI_U64 u64GlobalAvgSumR  = 0;
    HI_U64 u64GlobalAvgSumGr = 0;
    HI_U64 u64GlobalAvgSumGb = 0;
    HI_U64 u64GlobalAvgSumB  = 0;
    HI_U32 u32PixelWeightTmp = 0;
    HI_U32 u32PixelWeight    = 0;

    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeStt[k]);

        for (i = 0; i < 1024; i++) {
            if (k == 0) {
                pstStat->stBEAeStat1.au32HistogramMemArray[i] = apstBeStt[k]->ISP_AE_HIST[i].u32;
            } else {
                pstStat->stBEAeStat1.au32HistogramMemArray[i] += apstBeStt[k]->ISP_AE_HIST[i].u32;
            }
        }

        if (k == 0) {
            u32PixelWeightTmp = apstBeStt[k]->ISP_AE_COUNT_STAT_RSTT.bits.isp_ae_count_pixels_stt;
            pstStat->stBEAeStat1.u32PixelWeight = u32PixelWeightTmp;
            pstStat->stBEAeStat1.u32PixelCount  = apstBeStt[k]->ISP_AE_TOTAL_STAT_RSTT.bits.isp_ae_total_pixels_stt;
        } else {
            u32PixelWeightTmp = apstBeStt[k]->ISP_AE_COUNT_STAT_RSTT.bits.isp_ae_count_pixels_stt;
            pstStat->stBEAeStat1.u32PixelWeight += u32PixelWeightTmp;
            pstStat->stBEAeStat1.u32PixelCount  += apstBeStt[k]->ISP_AE_TOTAL_STAT_RSTT.bits.isp_ae_total_pixels_stt;
        }

        u64GlobalAvgSumR  += ((HI_U64)apstBeStt[k]->ISP_AE_TOTAL_R_AVER_RSTT.u32)  * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumGr += ((HI_U64)apstBeStt[k]->ISP_AE_TOTAL_GR_AVER_RSTT.u32) * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumGb += ((HI_U64)apstBeStt[k]->ISP_AE_TOTAL_GB_AVER_RSTT.u32) * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumB  += ((HI_U64)apstBeStt[k]->ISP_AE_TOTAL_B_AVER_RSTT.u32)  * ((HI_U64)u32PixelWeightTmp);

        for (i = 0; i < AE_ZONE_ROW; i++) {
            if (k < (AE_ZONE_COLUMN % DIV_0_TO_1(u8BlkNum))) {
                u8BlockZoneWidth = (AE_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum)) + 1;
            } else {
                u8BlockZoneWidth = AE_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum);
            }

            for (j = 0; j < u8BlockZoneWidth; j++) {
                u32AveMem = apstBeStt[k]->ISP_AE_AVER_R_GR[i * u8BlockZoneWidth + j].u32;
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][0] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][1] = (HI_U16)((u32AveMem & 0xFFFF));

                u32AveMem = apstBeStt[k]->ISP_AE_AVER_GB_B[i * u8BlockZoneWidth + j].u32;
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][2] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][3] = (HI_U16)((u32AveMem & 0xFFFF));
            }
        }

        u8BlockOffset += u8BlockZoneWidth;
    }

    u32PixelWeight = DIV_0_TO_1(pstStat->stBEAeStat1.u32PixelWeight);

    pstStat->stBEAeStat2.u16GlobalAvgR  = osal_div_u64(u64GlobalAvgSumR,  u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgGr = osal_div_u64(u64GlobalAvgSumGr, u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgGb = osal_div_u64(u64GlobalAvgSumGb, u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgB  = osal_div_u64(u64GlobalAvgSumB,  u32PixelWeight);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_AE_Global_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum)
{
    HI_U32 i, k;
    HI_U64 u64GlobalAvgSumR  = 0;
    HI_U64 u64GlobalAvgSumGr = 0;
    HI_U64 u64GlobalAvgSumGb = 0;
    HI_U64 u64GlobalAvgSumB  = 0;
    HI_U32 u32PixelWeightTmp = 0;
    HI_U32 u32PixelWeight    = 0;

    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeStt[k]);

        for (i = 0; i < 1024; i++) {
            if (k == 0) {
                pstStat->stBEAeStat1.au32HistogramMemArray[i] = apstBeStt[k]->ISP_AE_HIST[i].u32;
            } else {
                pstStat->stBEAeStat1.au32HistogramMemArray[i] += apstBeStt[k]->ISP_AE_HIST[i].u32;
            }
        }

        if (k == 0) {
            u32PixelWeightTmp = apstBeStt[k]->ISP_AE_COUNT_STAT_RSTT.bits.isp_ae_count_pixels_stt;
            pstStat->stBEAeStat1.u32PixelWeight = u32PixelWeightTmp;
            pstStat->stBEAeStat1.u32PixelCount  = apstBeStt[k]->ISP_AE_TOTAL_STAT_RSTT.bits.isp_ae_total_pixels_stt;
        } else {
            u32PixelWeightTmp = apstBeStt[k]->ISP_AE_COUNT_STAT_RSTT.bits.isp_ae_count_pixels_stt;
            pstStat->stBEAeStat1.u32PixelWeight += u32PixelWeightTmp;
            pstStat->stBEAeStat1.u32PixelCount  += apstBeStt[k]->ISP_AE_TOTAL_STAT_RSTT.bits.isp_ae_total_pixels_stt;
        }

        u64GlobalAvgSumR  += ((HI_U64)apstBeStt[k]->ISP_AE_TOTAL_R_AVER_RSTT.u32)  * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumGr += ((HI_U64)apstBeStt[k]->ISP_AE_TOTAL_GR_AVER_RSTT.u32) * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumGb += ((HI_U64)apstBeStt[k]->ISP_AE_TOTAL_GB_AVER_RSTT.u32) * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumB  += ((HI_U64)apstBeStt[k]->ISP_AE_TOTAL_B_AVER_RSTT.u32)  * ((HI_U64)u32PixelWeightTmp);
    }

    u32PixelWeight = DIV_0_TO_1(pstStat->stBEAeStat1.u32PixelWeight);

    pstStat->stBEAeStat2.u16GlobalAvgR  = osal_div_u64(u64GlobalAvgSumR,  u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgGr = osal_div_u64(u64GlobalAvgSumGr, u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgGb = osal_div_u64(u64GlobalAvgSumGb, u32PixelWeight);
    pstStat->stBEAeStat2.u16GlobalAvgB  = osal_div_u64(u64GlobalAvgSumB,  u32PixelWeight);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_AE_Local_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum)
{
    HI_U32 i, j, k;
    HI_U8  u8BlockOffset    = 0;
    HI_U8  u8BlockZoneWidth = 0;
    HI_U32 u32AveMem;;

    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeStt[k]);

        for (i = 0; i < AE_ZONE_ROW; i++) {
            if (k < (AE_ZONE_COLUMN % DIV_0_TO_1(u8BlkNum))) {
                u8BlockZoneWidth = (AE_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum)) + 1;
            } else {
                u8BlockZoneWidth = AE_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum);
            }

            for (j = 0; j < u8BlockZoneWidth; j++) {
                u32AveMem = apstBeStt[k]->ISP_AE_AVER_R_GR[i * u8BlockZoneWidth + j].u32;
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][0] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][1] = (HI_U16)((u32AveMem & 0xFFFF));

                u32AveMem = apstBeStt[k]->ISP_AE_AVER_GB_B[i * u8BlockZoneWidth + j].u32;
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][2] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                pstStat->stBEAeStat3.au16ZoneAvg[i][j + u8BlockOffset][3] = (HI_U16)((u32AveMem & 0xFFFF));
            }
        }

        u8BlockOffset += u8BlockZoneWidth;
    }

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_BE_MG_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum)
{
    HI_U8  k;
    HI_U8  u8BlockOffset    = 0;
    HI_U8  u8BlockZoneWidth = 0;
    HI_U32 i, j;
    HI_U32 u32AveMem;

    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeStt[k]);

        for (i = 0; i < MG_ZONE_ROW; i++) {
            if (k < (MG_ZONE_COLUMN % DIV_0_TO_1(u8BlkNum))) {
                u8BlockZoneWidth = (MG_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum)) + 1;
            } else {
                u8BlockZoneWidth = MG_ZONE_COLUMN / DIV_0_TO_1(u8BlkNum);
            }

            for (j = 0; j < u8BlockZoneWidth; j++) {
                u32AveMem = apstBeStt[k]->ISP_LA_AVER[i * u8BlockZoneWidth + j].u32;
                pstStat->stMgStat.au16ZoneAvg[i][j + u8BlockOffset][0] = (HI_U16)((u32AveMem & 0xFF000000) >> 24);
                pstStat->stMgStat.au16ZoneAvg[i][j + u8BlockOffset][1] = (HI_U16)((u32AveMem & 0xFF0000) >> 16);
                pstStat->stMgStat.au16ZoneAvg[i][j + u8BlockOffset][2] = (HI_U16)((u32AveMem & 0xFF00) >> 8);
                pstStat->stMgStat.au16ZoneAvg[i][j + u8BlockOffset][3] = (HI_U16)((u32AveMem & 0xFF));
            }
        }

        u8BlockOffset += u8BlockZoneWidth;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_AWB_OfflineStatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat,  S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum, HI_U8 u8BlkDev, isp_stat_key unStatkey)
{
    HI_S32 i, j, k, m;
    HI_U8  u8BlockOffset    = 0;
    HI_U8  u8BlockZoneWidth = 0;
    HI_U32 u32Value;
    HI_U64 u64MeteringAwbAvgR = 0;
    HI_U64 u64MeteringAwbAvgG = 0;
    HI_U64 u64MeteringAwbAvgB = 0;
    HI_U32 u32MeteringAwbCountAll = 0;
    HI_U32 u32Col, u32Row;
    HI_U32 u32ZoneBin;
    HI_U16 u16WStartAddr = 0;
    HI_U16 u16RStartAddr = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_sync_cfg_buf_node *pstNode = HI_NULL;
    HI_U32 WbGainBfStat[ISP_BAYER_CHN_NUM] = {0x100, 0x100, 0x100, 0x100};
    HI_U32 WDRWbGain[ISP_BAYER_CHN_NUM] = {0x100, 0x100, 0x100, 0x100};
    HI_U32 SensorWbGain[ISP_BAYER_CHN_NUM] = {0x100, 0x100, 0x100, 0x100};
    HI_U16 RgainConv = 0x100;
    HI_U16 GgainConv = 0x100;
    HI_U16 BgainConv = 0x100;
    HI_U32 MinGain, NormGain;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStat);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    pstNode = &(pstDrvCtx->stSyncCfg.stSyncCfgBuf.stSyncCfgBufNode[0]);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        WDRWbGain[i] = pstNode->stAWBRegCfg.au32WDRWBGain[i];
        SensorWbGain[i] = pstNode->stAWBRegCfg.au32SensorWBGain[i];
    }

    if (pstNode->stAWBRegCfg.enBEAWBSwitch == ISP_AWB_AFTER_DRC) {
         for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            WbGainBfStat[i] = pstNode->stAWBRegCfg.au32BEWhiteBalanceGain[i];
        }
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        WbGainBfStat[i] = (WbGainBfStat[i] * WDRWbGain[i]) >> 8;
        WbGainBfStat[i] = (WbGainBfStat[i] * SensorWbGain[i]) >> 8;
    }

    MinGain = 0xFFFF;
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        MinGain = MIN2(MinGain, WbGainBfStat[i]);
    }
    NormGain = 0x10000 / DIV_0_TO_1(MinGain);
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        WbGainBfStat[i] = (WbGainBfStat[i] * NormGain + 0x80) >> 8;
    }

    RgainConv = (0x10000 + (WbGainBfStat[0] >> 1)) / DIV_0_TO_1(WbGainBfStat[0]);
    GgainConv = (0x10000 + (WbGainBfStat[1] >> 1)) / DIV_0_TO_1(WbGainBfStat[1]);
    BgainConv = (0x10000 + (WbGainBfStat[3] >> 1)) / DIV_0_TO_1(WbGainBfStat[3]);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeStt[k]);

        u32Col  = pstDrvCtx->astKernelCfg[pstDrvCtx->u32RegCfgInfoFlag].alg_kernel_cfg[k].stAWBZoneCfg.u32Colum;
        u32Row  = pstDrvCtx->astKernelCfg[pstDrvCtx->u32RegCfgInfoFlag].alg_kernel_cfg[k].stAWBZoneCfg.u32Row;
        u32Col  = MIN2(u32Col, AWB_ZONE_ORIG_COLUMN);
        u32Row  = MIN2(u32Row, AWB_ZONE_ORIG_ROW);

        u32ZoneBin = pstDrvCtx->astKernelCfg[pstDrvCtx->u32RegCfgInfoFlag].alg_kernel_cfg[k].stAWBZoneCfg.u32ZoneBin;
        u32ZoneBin = MIN2(u32ZoneBin, AWB_ZONE_BIN);

        if (unStatkey.bit1AwbStat1) {
            u64MeteringAwbAvgR += (HI_U64)apstBeStt[k]->ISP_AWB_AVG_R_RSTT.bits.isp_awb_avg_r_stt *
                                  apstBeStt[k]->ISP_AWB_CNT_ALL_RSTT.bits.isp_awb_count_all_stt;
            u64MeteringAwbAvgG += (HI_U64)apstBeStt[k]->ISP_AWB_AVG_G_RSTT.bits.isp_awb_avg_g_stt *
                                  apstBeStt[k]->ISP_AWB_CNT_ALL_RSTT.bits.isp_awb_count_all_stt;
            u64MeteringAwbAvgB += (HI_U64)apstBeStt[k]->ISP_AWB_AVG_B_RSTT.bits.isp_awb_avg_b_stt *
                                  apstBeStt[k]->ISP_AWB_CNT_ALL_RSTT.bits.isp_awb_count_all_stt;
            u32MeteringAwbCountAll += apstBeStt[k]->ISP_AWB_CNT_ALL_RSTT.bits.isp_awb_count_all_stt;
        }

        if (unStatkey.bit1AwbStat2) {
            for (i = 0; i < u32Row; i++) {
                if (k < (u32Col % DIV_0_TO_1(u8BlkNum))) {
                    u8BlockZoneWidth = (u32Col / DIV_0_TO_1(u8BlkNum)) + 1;
                } else {
                    u8BlockZoneWidth = u32Col / DIV_0_TO_1(u8BlkNum);
                }

                for (j = 0; j < u8BlockZoneWidth; j++) {
                    u16WStartAddr = (i * u32Col + j + u8BlockOffset) * u32ZoneBin;
                    u16RStartAddr = (i * u8BlockZoneWidth + j) * u32ZoneBin * 2;

                    for (m = 0; m < u32ZoneBin; m++) {
                        HI_U16 ReadRAvg, ReadGAvg, ReadBAvg;

                        u32Value = apstBeStt[k]->ISP_AWB_STAT[u16RStartAddr + m * 2 + 0].u32;
                        ReadRAvg = (u32Value & 0xFFFF);
                        ReadGAvg = ((u32Value >> 16) & 0xFFFF);
                        u32Value = apstBeStt[k]->ISP_AWB_STAT[u16RStartAddr + m * 2 + 1].u32;
                        ReadBAvg = (u32Value & 0xFFFF);
                        pstStat->stAwbStat2.au16MeteringMemArrayCountAll[u16WStartAddr + m] = ((u32Value >> 16) & 0xFFFF);

                        pstStat->stAwbStat2.au16MeteringMemArrayAvgR[u16WStartAddr + m] = (ReadRAvg * RgainConv + 0x80) >> 8;
                        pstStat->stAwbStat2.au16MeteringMemArrayAvgG[u16WStartAddr + m] = (ReadGAvg * GgainConv + 0x80) >> 8;
                        pstStat->stAwbStat2.au16MeteringMemArrayAvgB[u16WStartAddr + m] = (ReadBAvg * BgainConv + 0x80) >> 8;
                    }
                }
            }
        }

        u8BlockOffset += u8BlockZoneWidth;
    }

    pstStat->stAwbStat1.u16MeteringAwbAvgR = (HI_U16)(osal_div_u64(u64MeteringAwbAvgR, DIV_0_TO_1(u32MeteringAwbCountAll)));
    pstStat->stAwbStat1.u16MeteringAwbAvgG = (HI_U16)(osal_div_u64(u64MeteringAwbAvgG, DIV_0_TO_1(u32MeteringAwbCountAll)));
    pstStat->stAwbStat1.u16MeteringAwbAvgB = (HI_U16)(osal_div_u64(u64MeteringAwbAvgB, DIV_0_TO_1(u32MeteringAwbCountAll)));
    pstStat->stAwbStat1.u16MeteringAwbCountAll = (HI_U16)(u32MeteringAwbCountAll / DIV_0_TO_1(u8BlkNum));

    pstStat->stAwbStat1.u16MeteringAwbAvgR = (pstStat->stAwbStat1.u16MeteringAwbAvgR * RgainConv + 0x80) >> 8;
    pstStat->stAwbStat1.u16MeteringAwbAvgG = (pstStat->stAwbStat1.u16MeteringAwbAvgG * GgainConv + 0x80) >> 8;
    pstStat->stAwbStat1.u16MeteringAwbAvgB = (pstStat->stAwbStat1.u16MeteringAwbAvgB * BgainConv + 0x80) >> 8;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_AF_OfflineStatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum)
{
    HI_U8  k;
    HI_U8  u8ColIndex    = 0;
    HI_U8  u8BlockOffset = 0;
    HI_U32 i, j;
    HI_U32 u32Col;
    HI_U32 u32Row;
    HI_U32 u32BEAfStatData   = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_POINTER(pstStat);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeStt[k]);

        u32Col     = pstDrvCtx->astKernelCfg[pstDrvCtx->u32RegCfgInfoFlag].alg_kernel_cfg[k].be_af_zone_cfg.colum;
        u32Row     = pstDrvCtx->astKernelCfg[pstDrvCtx->u32RegCfgInfoFlag].alg_kernel_cfg[k].be_af_zone_cfg.row;
        u32Col     = MIN2(u32Col, AF_ZONE_COLUMN);
        u32Row     = MIN2(u32Row, AF_ZONE_ROW);

        for (i = 0; i < u32Row; i++) {
            for (j = 0; j < u32Col; j++) {
                u8ColIndex = MIN2(j + u8BlockOffset, AF_ZONE_COLUMN);

                u32BEAfStatData = apstBeStt[k]->ISP_AF_STAT_H1[i * u32Col + j].u32;
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16h1 = (HI_U16)(0xFFFF & u32BEAfStatData);
                u32BEAfStatData = apstBeStt[k]->ISP_AF_STAT_H2[i * u32Col + j].u32;
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16h2 = (HI_U16)(0xFFFF & u32BEAfStatData);
                u32BEAfStatData = apstBeStt[k]->ISP_AF_STAT_V1[i * u32Col + j].u32;
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16v1 = (HI_U16)(0xFFFF & u32BEAfStatData);
                u32BEAfStatData = apstBeStt[k]->ISP_AF_STAT_V2[i * u32Col + j].u32;
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16v2 = (HI_U16)(0xFFFF & u32BEAfStatData);
                u32BEAfStatData = apstBeStt[k]->ISP_AF_STAT_Y[i * u32Col + j].u32;
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16HlCnt = (HI_U16)((0xFFFF0000 & u32BEAfStatData) >> 16);
                pstStat->stBEAfStat.stZoneMetrics[i][u8ColIndex].u16y  = (HI_U16)(0xFFFF & u32BEAfStatData);
            }
        }

        u8BlockOffset += u32Col;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_Dehaze_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum)
{
    HI_U32 i, j, m;

    j = DEFOG_ZONE_NUM / 2;

    ISP_CHECK_POINTER(pstStat);

    for (i = 0; i < u8BlkNum; i++) {
        ISP_CHECK_POINTER(apstBeStt[i]);

        for (m = 0; m < j; m++) {
            pstStat->dehaze_stat.min_dout[i][m] = apstBeStt[i]->ISP_DEHAZE_MINSTAT[m].u32;
        }

        for (m = 0; m < DEFOG_ZONE_NUM; m++) {
            pstStat->dehaze_stat.max_stat_dout[i][m] = apstBeStt[i]->ISP_DEHAZE_MAXSTAT[m].u32;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_DPC_OfflineCalibInfoRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum)
{
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_Flicker_OfflineStatisticsRead(isp_stat *pstStat, S_ISP_STT_REGS_TYPE *apstBeStt[], HI_U8 u8BlkNum)
{
    HI_U8  k;
    HI_U16 j;
    ISP_CHECK_POINTER(pstStat);

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(apstBeStt[k]);

        pstStat->flicker_stat.cur_diff_gr[k]  = apstBeStt[k]->ISP_FLICK_GR_DIFF_CUR_RSTT.bits.isp_flick_gr_diff_cur_rstt;
        pstStat->flicker_stat.cur_diff_gb[k]  = apstBeStt[k]->ISP_FLICK_GB_DIFF_CUR_RSTT.bits.isp_flick_gb_diff_cur_rstt;
        pstStat->flicker_stat.cur_abs_gr[k]   = apstBeStt[k]->ISP_FLICK_GR_ABS_CUR_RSTT.bits.isp_flick_gr_abs_cur_rstt;
        pstStat->flicker_stat.cur_abs_gb[k]   = apstBeStt[k]->ISP_FLICK_GB_ABS_CUR_RSTT.bits.isp_flick_gb_abs_cur_rstt;
        pstStat->flicker_stat.cur_cnt_over[k] = apstBeStt[k]->ISP_FLICK_COUNTOVER_CUR_RSTT.bits.isp_flick_countover_cur_rstt;

        for (j = 0; j < ISP_FLICKER_G_MEAN_LENGTH; j++) {
            pstStat->flicker_stat.g_mean[k][j] = apstBeStt[k]->ISP_FLICK_GMEAN[j].u32;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_LDCI_OfflineStatisticsRead(VI_PIPE ViPipe, isp_stat *pstStat)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStat);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_memcpy(&pstStat->ldci_stat, &pstDrvCtx->stLdciSttAddr.ldci_stat, sizeof(isp_ldci_stat));

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeOfflineAEStitchGlobalStatisticsRead(isp_stat *pstStat, ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i, k = 0;
    HI_U64 u64GlobalAvgSumR = 0;
    HI_U64 u64GlobalAvgSumGr = 0;
    HI_U64 u64GlobalAvgSumGb = 0;
    HI_U64 u64GlobalAvgSumB = 0;
    HI_U32 u32PixelWeightTmp = 0;
    HI_U32 u32PixelWeight = 0;

    S_ISP_STITCH_STT_REGS_TYPE *ptmp;

    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_POINTER(pstDrvCtx);

    for (k = 0; k < pstDrvCtx->stStitchAttr.stitch_pipe_num; k++) {
        ptmp = (S_ISP_STITCH_STT_REGS_TYPE *)pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[k].vir_addr;

        if (k == 0) {
            for (i = 0; i < 1024; i++) {
                pstStat->stitch_stat.stBEAeStat1.au32HistogramMemArray[i] = ptmp->ISP_AE_HIST[i].u32;
            }

            u32PixelWeightTmp = ptmp->ISP_AE_COUNT_STAT_RSTT.bits.isp_ae_count_pixels_stt;
            pstStat->stitch_stat.stBEAeStat1.u32PixelWeight = u32PixelWeightTmp;
            pstStat->stitch_stat.stBEAeStat1.u32PixelCount  = ptmp->ISP_AE_TOTAL_STAT_RSTT.bits.isp_ae_total_pixels_stt;
        } else {
            for (i = 0; i < 1024; i++) {
                pstStat->stitch_stat.stBEAeStat1.au32HistogramMemArray[i] += ptmp->ISP_AE_HIST[i].u32;
            }

            u32PixelWeightTmp = ptmp->ISP_AE_COUNT_STAT_RSTT.bits.isp_ae_count_pixels_stt;
            pstStat->stitch_stat.stBEAeStat1.u32PixelWeight += u32PixelWeightTmp;
            pstStat->stitch_stat.stBEAeStat1.u32PixelCount  += ptmp->ISP_AE_TOTAL_STAT_RSTT.bits.isp_ae_total_pixels_stt;
        }

        u64GlobalAvgSumR  += ((HI_U64)ptmp->ISP_AE_TOTAL_R_AVER_RSTT.u32)  * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumGr += ((HI_U64)ptmp->ISP_AE_TOTAL_GR_AVER_RSTT.u32) * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumGb += ((HI_U64)ptmp->ISP_AE_TOTAL_GB_AVER_RSTT.u32) * ((HI_U64)u32PixelWeightTmp);
        u64GlobalAvgSumB  += ((HI_U64)ptmp->ISP_AE_TOTAL_B_AVER_RSTT.u32)  * ((HI_U64)u32PixelWeightTmp);
    }

    u32PixelWeight = DIV_0_TO_1(pstStat->stitch_stat.stBEAeStat1.u32PixelWeight);

    pstStat->stitch_stat.stBEAeStat2.u16GlobalAvgR  = osal_div_u64(u64GlobalAvgSumR,  u32PixelWeight);
    pstStat->stitch_stat.stBEAeStat2.u16GlobalAvgGr = osal_div_u64(u64GlobalAvgSumGr, u32PixelWeight);
    pstStat->stitch_stat.stBEAeStat2.u16GlobalAvgGb = osal_div_u64(u64GlobalAvgSumGb, u32PixelWeight);
    pstStat->stitch_stat.stBEAeStat2.u16GlobalAvgB  = osal_div_u64(u64GlobalAvgSumB,  u32PixelWeight);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeOfflineAEStitchLocalStatisticsRead(isp_stat *pstStat, ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i, j, k = 0;
    HI_U32 u32AveMem;
    VI_PIPE ViPipeBind = 0;

    S_ISP_STITCH_STT_REGS_TYPE *ptmp;

    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_POINTER(pstDrvCtx);

    for (k = 0; k < pstDrvCtx->stStitchAttr.stitch_pipe_num; k++) {
        ptmp = (S_ISP_STITCH_STT_REGS_TYPE *)pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[k].vir_addr;
        ViPipeBind = pstDrvCtx->stStitchAttr.stitch_bind_id[k];

        if (ViPipeBind < VI_MAX_PIPE_NUM) {
            for (i = 0; i < AE_ZONE_ROW; i++) {
                for (j = 0; j < AE_ZONE_COLUMN; j++) {
                    u32AveMem = ptmp->ISP_AE_AVER_R_GR[i * AE_ZONE_COLUMN + j].u32;
                    pstStat->stitch_stat.stBEAeStat3.au16ZoneAvg[ViPipeBind][i][j][0] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                    pstStat->stitch_stat.stBEAeStat3.au16ZoneAvg[ViPipeBind][i][j][1] = (HI_U16)((u32AveMem & 0xFFFF));

                    u32AveMem = ptmp->ISP_AE_AVER_GB_B[i * AE_ZONE_COLUMN + j].u32;
                    pstStat->stitch_stat.stBEAeStat3.au16ZoneAvg[ViPipeBind][i][j][2] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                    pstStat->stitch_stat.stBEAeStat3.au16ZoneAvg[ViPipeBind][i][j][3] = (HI_U16)((u32AveMem & 0xFFFF));
                }
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeOfflineAWBStitchStatisticsRead(isp_stat *pstStat, S_ISPBE_REGS_TYPE *apstBeReg[], ISP_DRV_CTX_S *pstDrvCtx, HI_U8 u8BlkDev, isp_stat_key unStatkey)
{
    HI_U8  u8StitchNum       = 0;
    HI_U16 u16BlockZoneWidth = 0;
    HI_U16 u16StitchWidth    = 0;
    HI_S32 i, j, k = 0;
    HI_U32 u32Col, u32Row;
    HI_U32 u32ZoneBin;

    S_ISP_STITCH_STT_REGS_TYPE *ptmp = HI_NULL;

    ISP_CHECK_POINTER(pstStat);
    ISP_CHECK_POINTER(pstDrvCtx);

    u8StitchNum = pstDrvCtx->stStitchAttr.stitch_pipe_num;
    u32Col  = pstDrvCtx->astKernelCfg[pstDrvCtx->u32RegCfgInfoFlag].alg_kernel_cfg[0].stAWBZoneCfg.u32Colum;
    u32Row  = pstDrvCtx->astKernelCfg[pstDrvCtx->u32RegCfgInfoFlag].alg_kernel_cfg[0].stAWBZoneCfg.u32Row;
    u32Col  = MIN2(u32Col, AWB_ZONE_ORIG_COLUMN);
    u32Row  = MIN2(u32Row, AWB_ZONE_ORIG_ROW);
    u32ZoneBin = pstDrvCtx->astKernelCfg[pstDrvCtx->u32RegCfgInfoFlag].alg_kernel_cfg[0].stAWBZoneCfg.u32ZoneBin;

    if (unStatkey.bit1AwbStat2) {
        u16BlockZoneWidth = u32Col;
        u16StitchWidth = u16BlockZoneWidth * u8StitchNum;

        pstStat->stitch_stat.stAwbStat2.u16ZoneRow = u32Row;
        pstStat->stitch_stat.stAwbStat2.u16ZoneCol = u16StitchWidth;


        for (k = 0; k < u8StitchNum; k++) {
            ptmp = (S_ISP_STITCH_STT_REGS_TYPE *)pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[k].vir_addr;

            for (i = 0; i < u32Row; i++) {
                for (j = 0; j < u16BlockZoneWidth; j++) {
                    HI_U16 u16WStartAddr = (u16StitchWidth * i + u16BlockZoneWidth * k + j) * u32ZoneBin;
                    HI_U16 u16RStartAddr = (u16BlockZoneWidth * i + j) * 2 * u32ZoneBin;
                    HI_U16 m;

                    for (m = 0; m < u32ZoneBin; m++) {
                        HI_U32 u32StatGR, u32StatCountB;

                        u32StatGR       = ptmp->ISP_AWB_STAT[u16RStartAddr + (m * 2) + 0].u32;
                        u32StatCountB = ptmp->ISP_AWB_STAT[u16RStartAddr + (m * 2) + 1].u32;

                        pstStat->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgR[u16WStartAddr + m]     = (u32StatGR & 0xFFFF);
                        pstStat->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgG[u16WStartAddr + m]     = ((u32StatGR >> 16) & 0xFFFF);
                        pstStat->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgB[u16WStartAddr + m]     = (u32StatCountB & 0xFFFF);
                        pstStat->stitch_stat.stAwbStat2.au16MeteringMemArrayCountAll[u16WStartAddr + m] = ((u32StatCountB >> 16) & 0xFFFF);
                    }
                }
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BE_StitchStatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo)
{
    HI_S32 i, j, k = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U64 u64GlobalAvgSumR = 0;
    HI_U64 u64GlobalAvgSumGr = 0;
    HI_U64 u64GlobalAvgSumGb = 0;
    HI_U64 u64GlobalAvgSumB = 0;
    HI_U32 u32PixelWeightTmp = 0;
    HI_U32 u32PixelWeight = 0;
    HI_U8  u8StitchNum = 0;
    HI_U16 u16BlockZoneWidth = 0;
    HI_U16 u16StitchWidth = 0;
    VI_PIPE ViPipeBind = 0;
    HI_U32 u32Zone;
    HI_U32 u32Col, u32Row;
    HI_U32 u32ZoneBin;
    HI_U32 u32AveMem;

    isp_stat_key unStatkey;
    isp_stat *pstStat = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_be_regs_attr stIspBeRegsAttr = {0};
    S_ISPBE_REGS_TYPE *apstBeReg[ISP_STRIPING_MAX_NUM] = {HI_NULL};

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);
    pstStat = (isp_stat *)pstStatInfo->virt_addr;

    if (pstStat == HI_NULL) {
        return HI_FAILURE;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if ((pstDrvCtx->stStitchAttr.stitch_enable != HI_TRUE) || (pstDrvCtx->stStitchAttr.main_pipe != HI_TRUE)) {
        return HI_SUCCESS;
    }

    u8StitchNum = pstDrvCtx->stStitchAttr.stitch_pipe_num;
    u8StitchNum = (u8StitchNum >= ISP_MAX_BE_NUM) ? ISP_MAX_BE_NUM : u8StitchNum;

    pstStat->be_update = HI_TRUE;
    unStatkey.key = pstStatInfo->stat_key.bit32IsrAccess;

    for (k = 0; k < pstDrvCtx->stStitchAttr.stitch_pipe_num; k++) {
        ViPipeBind = pstDrvCtx->stStitchAttr.stitch_bind_id[k];

        ISP_CHECK_PIPE(ViPipeBind);
        s32Ret = ISP_DRV_GetBeRegsAttr(ViPipeBind, apstBeReg, &stIspBeRegsAttr);

        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        // BE AE statistics
        if (unStatkey.bit1BEAeStiGloStat) {
            apstBeReg[k]->ISP_AE_HIST_RADDR.u32 = 0x0;

            if (k == 0) {
                for (i = 0; i < 1024; i++) {
                    pstStat->stitch_stat.stBEAeStat1.au32HistogramMemArray[i] = apstBeReg[k]->ISP_AE_HIST_RDATA.u32;
                }

                u32PixelWeightTmp = apstBeReg[k]->ISP_AE_COUNT_STAT.bits.isp_ae_count_pixels;
                pstStat->stitch_stat.stBEAeStat1.u32PixelWeight = u32PixelWeightTmp;
                pstStat->stitch_stat.stBEAeStat1.u32PixelCount  = apstBeReg[k]->ISP_AE_TOTAL_STAT.bits.isp_ae_total_pixels;
            } else {
                for (i = 0; i < 1024; i++) {
                    pstStat->stitch_stat.stBEAeStat1.au32HistogramMemArray[i] += apstBeReg[k]->ISP_AE_HIST_RDATA.u32;
                }

                u32PixelWeightTmp = apstBeReg[k]->ISP_AE_COUNT_STAT.bits.isp_ae_count_pixels;
                pstStat->stitch_stat.stBEAeStat1.u32PixelWeight += u32PixelWeightTmp;
                pstStat->stitch_stat.stBEAeStat1.u32PixelCount  += apstBeReg[k]->ISP_AE_TOTAL_STAT.bits.isp_ae_total_pixels;
            }

            u64GlobalAvgSumR  += ((HI_U64)apstBeReg[k]->ISP_AE_TOTAL_R_AVER.u32)  * ((HI_U64)u32PixelWeightTmp);
            u64GlobalAvgSumGr += ((HI_U64)apstBeReg[k]->ISP_AE_TOTAL_GR_AVER.u32) * ((HI_U64)u32PixelWeightTmp);
            u64GlobalAvgSumGb += ((HI_U64)apstBeReg[k]->ISP_AE_TOTAL_GB_AVER.u32) * ((HI_U64)u32PixelWeightTmp);
            u64GlobalAvgSumB  += ((HI_U64)apstBeReg[k]->ISP_AE_TOTAL_B_AVER.u32)  * ((HI_U64)u32PixelWeightTmp);
        }

        if (unStatkey.bit1BEAeStiLocStat) {
            apstBeReg[k]->ISP_AE_AVER_R_GR_RADDR.u32 = 0x0;
            apstBeReg[k]->ISP_AE_AVER_GB_B_RADDR.u32 = 0x0;

            if (ViPipeBind < VI_MAX_PIPE_NUM) {
                for (i = 0; i < AE_ZONE_ROW; i++) {
                    for (j = 0; j < AE_ZONE_COLUMN; j++) {
                        u32AveMem = apstBeReg[k]->ISP_AE_AVER_R_GR_RDATA.u32;
                        pstStat->stitch_stat.stBEAeStat3.au16ZoneAvg[ViPipeBind][i][j][0] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                        pstStat->stitch_stat.stBEAeStat3.au16ZoneAvg[ViPipeBind][i][j][1] = (HI_U16)((u32AveMem & 0xFFFF));

                        u32AveMem = apstBeReg[k]->ISP_AE_AVER_GB_B_RDATA.u32;
                        pstStat->stitch_stat.stBEAeStat3.au16ZoneAvg[ViPipeBind][i][j][2] = (HI_U16)((u32AveMem & 0xFFFF0000) >> 16);
                        pstStat->stitch_stat.stBEAeStat3.au16ZoneAvg[ViPipeBind][i][j][3] = (HI_U16)((u32AveMem & 0xFFFF));
                    }
                }
            }
        }

        if (unStatkey.bit1AwbStat2) {
            u32Zone = apstBeReg[k]->ISP_AWB_ZONE.u32;
            u32Col  = MIN2((u32Zone & 0x3F), AWB_ZONE_ORIG_COLUMN);
            u32Row  = MIN2(((u32Zone & 0x3F00) >> 8), AWB_ZONE_ORIG_ROW);
            u32ZoneBin = apstBeReg[k]->ISP_AWB_ZONE_BIN.u32;
            u16BlockZoneWidth = u32Col;
            u16StitchWidth = u16BlockZoneWidth * u8StitchNum;
            pstStat->stitch_stat.stAwbStat2.u16ZoneRow = u32Row;
            pstStat->stitch_stat.stAwbStat2.u16ZoneCol = u16StitchWidth;
            pstStat->stitch_stat.stAwbStat2.u16ZoneBin = u32ZoneBin;
            apstBeReg[k]->ISP_AWB_STAT_RADDR.u32 = 0x0;

            for (i = 0; i < u32Row; i++) {
                for (j = 0; j < u16BlockZoneWidth; j++) {
                    HI_U16 u16WStartAddr = (u16StitchWidth * i + u16BlockZoneWidth * k + j) * u32ZoneBin;
                    HI_U32 u32StatGR, u32StatCountB, m;

                    for (m = 0; m < u32ZoneBin; m++) {
                        u32StatGR     = apstBeReg[k]->ISP_AWB_STAT_RDATA.u32;
                        pstStat->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgR[u16WStartAddr + m] = (u32StatGR & 0xFFFF);
                        pstStat->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgG[u16WStartAddr + m] = ((u32StatGR >> 16) & 0xFFFF);

                        u32StatCountB = apstBeReg[k]->ISP_AWB_STAT_RDATA.u32;
                        pstStat->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgB[u16WStartAddr + m] = (u32StatCountB & 0xFFFF);
                        pstStat->stitch_stat.stAwbStat2.au16MeteringMemArrayCountAll[u16WStartAddr + m] = ((u32StatCountB >> 16) & 0xFFFF);
                    }
                }
            }
        }
    }

    if (unStatkey.bit1BEAeStiGloStat) {
        u32PixelWeight = DIV_0_TO_1(pstStat->stitch_stat.stBEAeStat1.u32PixelWeight);

        pstStat->stitch_stat.stBEAeStat2.u16GlobalAvgR  = osal_div_u64(u64GlobalAvgSumR,  u32PixelWeight);
        pstStat->stitch_stat.stBEAeStat2.u16GlobalAvgGr = osal_div_u64(u64GlobalAvgSumGr, u32PixelWeight);
        pstStat->stitch_stat.stBEAeStat2.u16GlobalAvgGb = osal_div_u64(u64GlobalAvgSumGb, u32PixelWeight);
        pstStat->stitch_stat.stBEAeStat2.u16GlobalAvgB  = osal_div_u64(u64GlobalAvgSumB,  u32PixelWeight);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_FE_StitchNonStatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo)
{
    HI_U8  u8ChnNumMax = 1;
    HI_U32 k;
    VI_PIPE ViPipeBind = 0;
    isp_stat_key unStatkey;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_stat *pstStat = HI_NULL;
    S_ISPFE_REGS_TYPE *pstFeReg = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    if (pstDrvCtx->stWDRAttr.mast_pipe == HI_FALSE) {
        return HI_SUCCESS;
    }

    pstStat = (isp_stat *)pstStatInfo->virt_addr;
    if (pstStat == HI_NULL) {
        return HI_FAILURE;
    }

    unStatkey.key = pstStatInfo->stat_key.bit32IsrAccess;

    u8ChnNumMax = MIN2(pstDrvCtx->stWDRAttr.dev_bind_pipe.num, ISP_WDR_CHN_MAX);

    for (k = 0; k < u8ChnNumMax; k++) {
        /* get side statistics */
        ViPipeBind = pstDrvCtx->stWDRAttr.dev_bind_pipe.pipe_id[k];
        ISP_CHECK_PIPE(ViPipeBind);
        ISP_DRV_FEREG_CTX(ViPipeBind, pstFeReg);

        ISP_DRV_FE_APB_StatisticsRead(pstStat, pstFeReg, ViPipeBind, k, unStatkey);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeOfflineSttBufInit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_U32 i;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U64 u64PhyAddr, u64Size;
    HI_U8  *pu8VirAddr;
    unsigned long u32Flags;
    HI_U64  u64StitchPhyAddr, u64StitchSize;
    HI_U8   *pu8StitchVirAddr;
    HI_CHAR acName[MAX_MMZ_NAMELEN]       = {0};
    HI_CHAR acStitchName[MAX_MMZ_NAMELEN] = {0};

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stBeOffSttAttr.init);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].BeStt", ViPipe);

    u64Size = sizeof(S_ISP_STT_REGS_TYPE);

    u64Size = (u64Size + 255) / 256 * 256; // 256bytes align

    s32Ret = cmpi_mmz_malloc_cached(HI_NULL, acName, &u64PhyAddr, (HI_VOID **)&pu8VirAddr, u64Size * ISP_STRIPING_MAX_NUM);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("alloc ISP BeSttBuf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size * ISP_STRIPING_MAX_NUM);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->stBeOffSttAttr.init= HI_TRUE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        pstDrvCtx->stBeOffSttAttr.be_stt_buf[i].phy_addr = u64PhyAddr + i * u64Size;
        pstDrvCtx->stBeOffSttAttr.be_stt_buf[i].vir_addr = (HI_VOID *)(pu8VirAddr + i * u64Size);
        pstDrvCtx->stBeOffSttAttr.be_stt_buf[i].size     = u64Size;
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if ((pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) && (pstDrvCtx->stStitchAttr.main_pipe == HI_TRUE)) {
        ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stBeOffStitSttAttr.init);

        osal_snprintf(acStitchName, sizeof(acStitchName), "ISP[%d].StitStat", ViPipe);

        u64StitchSize = sizeof(S_ISP_STITCH_STT_REGS_TYPE) * pstDrvCtx->stStitchAttr.stitch_pipe_num;

        s32Ret = cmpi_mmz_malloc_cached(HI_NULL, acStitchName, &u64StitchPhyAddr, (HI_VOID **)&pu8StitchVirAddr, u64StitchSize);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("alloc ISP stitch statistics buf err\n");
            goto fail0;
        }

        osal_memset(pu8StitchVirAddr, 0, u64StitchSize);

        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

        pstDrvCtx->stBeOffStitSttAttr.init = HI_TRUE;

        for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
            pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[i].phy_addr = u64StitchPhyAddr + i * sizeof(S_ISP_STITCH_STT_REGS_TYPE);
            pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[i].vir_addr = (HI_VOID *)(pu8StitchVirAddr + i * sizeof(S_ISP_STITCH_STT_REGS_TYPE));
        }

        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    }

    return HI_SUCCESS;

fail0:
    pstDrvCtx->stBeOffSttAttr.init = HI_FALSE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        pstDrvCtx->stBeOffSttAttr.be_stt_buf[i].phy_addr = 0;
        pstDrvCtx->stBeOffSttAttr.be_stt_buf[i].vir_addr = HI_NULL;
        pstDrvCtx->stBeOffSttAttr.be_stt_buf[i].size     = 0;
    }

    if (u64PhyAddr != 0) {
        hil_mmb_invalid_cache_byaddr(pu8VirAddr, u64PhyAddr, u64Size * ISP_STRIPING_MAX_NUM);
        cmpi_mmz_free(u64PhyAddr, (HI_VOID *)pu8VirAddr);
    }

    return HI_FAILURE;
}

HI_S32 ISP_DRV_BeOfflineSttBufExit(VI_PIPE ViPipe)
{
    HI_U32 i;
    HI_U64 u64Size, u64StitchSize;
    HI_U64 u64PhyAddr;
    HI_VOID *pVirAddr;
    HI_U64 u64PhyStitchAddr;
    HI_VOID *pStitchVirAddr;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stBeOffSttAttr.init);

    u64PhyAddr = pstDrvCtx->stBeOffSttAttr.be_stt_buf[0].phy_addr;
    pVirAddr   = pstDrvCtx->stBeOffSttAttr.be_stt_buf[0].vir_addr;
    u64Size    = pstDrvCtx->stBeOffSttAttr.be_stt_buf[0].size * ISP_STRIPING_MAX_NUM;

    u64PhyStitchAddr = pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[0].phy_addr;
    pStitchVirAddr   = pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[0].vir_addr;
    u64StitchSize    = sizeof(S_ISP_STITCH_STT_REGS_TYPE) * pstDrvCtx->stStitchAttr.stitch_pipe_num;

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->stBeOffSttAttr.init = HI_FALSE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        pstDrvCtx->stBeOffSttAttr.be_stt_buf[i].phy_addr = 0;
        pstDrvCtx->stBeOffSttAttr.be_stt_buf[i].size     = 0;
        pstDrvCtx->stBeOffSttAttr.be_stt_buf[i].vir_addr = HI_NULL;
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        hil_mmb_invalid_cache_byaddr(pVirAddr, u64PhyAddr, u64Size);
        cmpi_mmz_free(u64PhyAddr, pVirAddr);
    }

    if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
        if (pstDrvCtx->stStitchAttr.main_pipe == HI_TRUE) {
            ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stBeOffStitSttAttr.init);

            for (i = 0; i < ISP_STITCH_MAX_NUM; i++) {
                pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[0].phy_addr = 0;
                pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[0].size     = 0;
                pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[0].vir_addr = HI_NULL;
            }

            pstDrvCtx->stBeOffStitSttAttr.init = HI_FALSE;

            if (u64PhyStitchAddr != 0) {
                hil_mmb_invalid_cache_byaddr(pStitchVirAddr, u64PhyStitchAddr, u64StitchSize);
                cmpi_mmz_free(u64PhyStitchAddr, pStitchVirAddr);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_FeSttAddrInit(VI_PIPE ViPipe)
{
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_FeStitchSttAddrInit(VI_PIPE ViPipe)
{
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeSttBufInit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.running_mode)\
        || IS_STRIPING_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        if (ISP_DRV_BeOfflineSttBufInit(ViPipe) != HI_SUCCESS) {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeSttBufExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.running_mode)\
        || IS_STRIPING_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        if (ISP_DRV_BeOfflineSttBufExit(ViPipe) != HI_SUCCESS) {
            ISP_ERR_TRACE("pipe[%d] ISP_DRV_BeOfflineSttBufExit failed!\n", ViPipe);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_SttBufInit(VI_PIPE ViPipe)
{
    ISP_CHECK_PIPE(ViPipe);

    return ISP_DRV_BeSttBufInit(ViPipe);
}

HI_S32 ISP_DRV_SttBufExit(VI_PIPE ViPipe)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    return ISP_DRV_BeSttBufExit(ViPipe);
}

HI_S32 ISP_DRV_GetP2EnInfo(VI_PIPE ViPipe, HI_BOOL *pbP2En)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pbP2En);

    if (ViPipe == 0) {
        *pbP2En = (HI_BOOL)((IO_RW_PT_ADDRESS(VI_ISP_MODE) & (1 << VI_ISP0_P2_EN_BIT)) >> VI_ISP0_P2_EN_BIT);
    } else {
        *pbP2En = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StaKernelGet(VI_PIPE ViPipe, ISP_DRV_AF_STATISTICS_S *pstFocusStat)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_stat *pstStat = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstFocusStat == HI_NULL) {
        ISP_ERR_TRACE("get statistic active buffer err, pstFocusStat is NULL!\n");
        return HI_FAILURE;
    }

    if (!pstDrvCtx->stStatisticsBuf.pstActStat) {
        ISP_INFO_TRACE("get statistic active buffer err, stat not ready!\n");
        return HI_FAILURE;
    }

    if (!pstDrvCtx->stStatisticsBuf.pstActStat->virt_addr) {
        ISP_ERR_TRACE("get statistic active buffer err, pVirtAddr is NULL!\n");
        return HI_FAILURE;
    }

    pstStat = (isp_stat *)pstDrvCtx->stStatisticsBuf.pstActStat->virt_addr;

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_memcpy(&(pstFocusStat->stFEAFStat), &pstStat->stFEAfStat, sizeof(ISP_DRV_FE_FOCUS_STATISTICS_S));
    osal_memcpy(&(pstFocusStat->stBEAFStat), &pstStat->stBEAfStat, sizeof(ISP_DRV_BE_FOCUS_STATISTICS_S));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StitchSyncCtrlInit(VI_PIPE ViPipe)
{
    HI_U8 i;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;
    VI_PIPE ViPipeS;
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {

        for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
            ViPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
            pstDrvCtxS->stIspSyncCtrl.s8StitchFlag = 0;
            pstDrvCtxS->stIspSyncCtrl.u8ReadAll = 0;
            pstDrvCtxS->stIspSyncCtrl.u8FirstOrder = 0;
            pstDrvCtxS->stIspSyncCtrl.u8FirstReadAll = 0;
            pstDrvCtxS->stIspSyncCtrl.u8AllInit = 0;
            pstDrvCtxS->stIspSyncCtrl.u32ViCallbackCount = 0;
        }
    }

    return HI_SUCCESS;
}

HI_VOID ISP_DRV_BeSyncParaStatusRest(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;
    VI_PIPE ViPipeS;
    HI_U32 i = 0;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
        ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
        pstDrvCtxS->stIspSyncCtrl.u8FirstOrder = 0;
        pstDrvCtxS->stIspSyncCtrl.u8ReadAll = 0;
        pstDrvCtxS->stIspSyncCtrl.u32ViCallbackCount = 0;
        pstDrvCtxS->stIspSyncCtrl.s8StitchFlag = 0;
    }

    pstDrvCtx->stIspSyncCtrl.u8FirstOrder = 1;
    pstDrvCtx->stIspSyncCtrl.s8StitchFlag = 1;
    pstDrvCtx->stIspSyncCtrl.u32ViCallbackCount = 1;
}

HI_U32 ISP_DRV_ViGetBeSyncParaCrossFrame(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_DRV_CTX_S    *pstDrvCtxS  = HI_NULL;
    VI_PIPE ViPipeS;
    HI_U64 MaxGap;
    HI_U32 i, j = 0;
    HI_U32 k = 0;
    HI_BOOL  bSameCount = HI_FALSE;
    HI_U32 u32CrossFrameNum = 0;
    HI_U32 u32TimeCrossFrame = 100;
    HI_BOOL  bAllInit = 0;
    HI_U32 x, y;
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    x = pstDrvCtx->stIspSyncCtrl.u32ViCallbackCount;

    if (pstDrvCtx->stIspSyncCtrl.u8AllInit == 0) {
        for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
            ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

            if (pstDrvCtxS->bIspRunFlag == HI_FALSE) {
                bAllInit = HI_FALSE;
                break;
            }

            if (i  == (pstDrvCtx->stStitchAttr.stitch_pipe_num - 1)) {
                bAllInit = HI_TRUE;
            }
        }

        if (bAllInit == HI_TRUE) {
            for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
                ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
                pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
                pstDrvCtxS->stIspSyncCtrl.u8AllInit = 1;
            }
        }
    }

    if (pstDrvCtx->bIspRunFlag == HI_FALSE) {
        for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
            ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
            pstDrvCtxS->stIspSyncCtrl.u8AllInit = 0;
        }
    }

    for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
        ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

        if ((pstDrvCtxS->stIspSyncCtrl.u8FirstOrder == 1) && (ViPipeS != ViPipe)) {
            MaxGap = (pstDrvCtx->stIspSyncCtrl.u64PTS > pstDrvCtxS->stIspSyncCtrl.u64PTS) ? (pstDrvCtx->stIspSyncCtrl.u64PTS - pstDrvCtxS->stIspSyncCtrl.u64PTS) : (pstDrvCtxS->stIspSyncCtrl.u64PTS - pstDrvCtx->stIspSyncCtrl.u64PTS);

            if (MaxGap >= 6000) {
                ISP_DRV_BeSyncParaStatusRest(ViPipe);
                return u32TimeCrossFrame;
            }
        }
    }

    for (j = 0; j < pstDrvCtx->stStitchAttr.stitch_pipe_num; j++) {
        ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[j];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

        if ((pstDrvCtxS->stIspSyncCtrl.u8FirstReadAll == 1) && pstDrvCtxS->stIspSyncCtrl.u8AllInit) {
            break;
        }

        if (j == pstDrvCtx->stStitchAttr.stitch_pipe_num - 1) {
            for (k = 0; k < pstDrvCtx->stStitchAttr.stitch_pipe_num; k++) {
                ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[k];
                pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
                pstDrvCtxS->stIspSyncCtrl.u32ViCallbackCount = 0;
            }

            return u32TimeCrossFrame;
        }
    }

    for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
        ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
        y = pstDrvCtxS->stIspSyncCtrl.u32ViCallbackCount;

        if (y != x) {
            u32CrossFrameNum =   x > y ? (x - y) : (y - x);
        } else {
            j++;
        }

        if ((u32CrossFrameNum >= 2) && (pstDrvCtxS->stIspSyncCtrl.u8AllInit)) {
            ISP_DRV_BeSyncParaStatusRest(ViPipe);
            return u32TimeCrossFrame;
        }

        if ((i == (pstDrvCtx->stStitchAttr.stitch_pipe_num - 1)) && (i == (j - 1)) && pstDrvCtxS->stIspSyncCtrl.u8AllInit) {
            bSameCount = HI_TRUE;
            u32CrossFrameNum = 0;
        }

    }

    if (bSameCount == HI_TRUE) {
        for (j = 0; j < pstDrvCtx->stStitchAttr.stitch_pipe_num; j++) {

            ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[j];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
            pstDrvCtxS->stIspSyncCtrl.u32ViCallbackCount = 0;
        }
    }

    return u32CrossFrameNum;
}

HI_S32 ISP_DRV_GetStitchBeSyncPara(VI_PIPE ViPipe, isp_be_sync_para *pstBeSyncPara)
{
    HI_U8   i, j;
    HI_U32  u32CrossFrameNum;
    VI_PIPE ViPipeS;
    VI_PIPE MainPipe;
    ISP_DRV_CTX_S *pstDrvCtx   = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS  = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeSyncPara);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pstDrvCtx->stIspSyncCtrl.u32ViCallbackCount  += 1;
    u32CrossFrameNum = ISP_DRV_ViGetBeSyncParaCrossFrame(ViPipe);


    for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
        ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

        if ((pstDrvCtxS->stIspSyncCtrl.s8StitchFlag == 0) && (pstDrvCtxS->stIspSyncCtrl.u8ReadAll == 0)) {

        } else {
            pstDrvCtx->stIspSyncCtrl.u8FirstOrder = 0;
            break;
        }

        if (i == (pstDrvCtx->stStitchAttr.stitch_pipe_num - 1) && (pstDrvCtx->stIspSyncCtrl.u8AllInit == 1)) {
            pstDrvCtx->stIspSyncCtrl.u8FirstOrder = 1;
        }
    }

    for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
        if (ViPipe == pstDrvCtx->stStitchAttr.stitch_bind_id[i]) {
            pstDrvCtx->stIspSyncCtrl.s8StitchFlag = 1;
            break;
        }
    }

    for (j = 0; j < pstDrvCtx->stStitchAttr.stitch_pipe_num; j++) {
        ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[j];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

        if (pstDrvCtxS->stIspSyncCtrl.s8StitchFlag != 1) {
            pstDrvCtxS->stIspSyncCtrl.u8ReadAll = 0;
            break;
        }

        if (j == pstDrvCtx->stStitchAttr.stitch_pipe_num - 1) {
            pstDrvCtx->stIspSyncCtrl.u8ReadAll = 1;
            pstDrvCtx->stIspSyncCtrl.u8FirstReadAll = 1;
        }
    }

    if (pstDrvCtx->stIspSyncCtrl.u8ReadAll == 1) {
        for (j = 0; j < pstDrvCtx->stStitchAttr.stitch_pipe_num; j++) {
            ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[j];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
            pstDrvCtxS->stIspSyncCtrl.s8StitchFlag  = 0;
            pstDrvCtxS->stIspSyncCtrl.u8ReadAll  = 0;
        }
    }

    if (pstDrvCtx->stIspSyncCtrl.u8FirstOrder == 1) {
        for (j = 0; j < pstDrvCtx->stStitchAttr.stitch_pipe_num; j++) {
            ViPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[j];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

            if (ViPipeS != ViPipe) {
                pstDrvCtxS->stIspSyncCtrl.u8FirstOrder = 0;
            }
        }
    }

    if (u32CrossFrameNum >= 2 ||  pstDrvCtx->stIspSyncCtrl.u8AllInit == 0) {
        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
        osal_memcpy(pstBeSyncPara, &pstDrvCtx->stIspBeSyncPara, sizeof(isp_be_sync_para));
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    } else if ((pstDrvCtx->stIspSyncCtrl.u8FirstOrder == 1) && (pstDrvCtx->stIspSyncCtrl.u8AllInit == 1)) {

        MainPipe  = pstDrvCtx->stStitchAttr.stitch_bind_id[0];
        pstDrvCtxS = ISP_DRV_GET_CTX(MainPipe);

        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
        osal_memcpy(&pstDrvCtxS->stIspBePreSyncPara, &pstDrvCtx->stIspBeSyncPara, sizeof(isp_be_sync_para));
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);


        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
        osal_memcpy(pstBeSyncPara, &pstDrvCtx->stIspBeSyncPara, sizeof(isp_be_sync_para));
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    } else {
        MainPipe  = pstDrvCtx->stStitchAttr.stitch_bind_id[0];
        pstDrvCtxS = ISP_DRV_GET_CTX(MainPipe);

        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
        osal_memcpy(pstBeSyncPara, &pstDrvCtxS->stIspBePreSyncPara, sizeof(isp_be_sync_para));
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    }


    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ResetFeCfg(VI_PIPE ViPipe)
{
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_UpdateLdciTprOfflineStat(VI_PIPE ViPipe, isp_be_wo_reg_cfg *pstBeCfg)
{
    HI_U8  i;
    HI_U64 u64PhyAddr, u64PhyAddrHigh;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeCfg);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u64PhyAddrHigh = pstBeCfg->be_reg_cfg[0].stViProcReg.VIPROC_OUT_PARA_DCI_ADDR_HIGH.u32;
    u64PhyAddr     = pstBeCfg->be_reg_cfg[0].stViProcReg.VIPROC_OUT_PARA_DCI_ADDR_LOW.u32;
    u64PhyAddr    |= (u64PhyAddrHigh << 32);

    for (i = 0; i < pstDrvCtx->stLdciReadBufAttr.buf_num; i++) {
        if (u64PhyAddr == pstDrvCtx->stLdciWriteBufAttr.ldci_buf[i].phy_addr) {
            osal_memcpy(&pstDrvCtx->stLdciSttAddr.ldci_stat, pstDrvCtx->stLdciWriteBufAttr.ldci_buf[i].vir_addr,
                        sizeof(isp_ldci_stat));
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_UpdateLdciTprSttAddr(VI_PIPE ViPipe, isp_be_wo_reg_cfg *pstBeCfg)
{
    HI_U8  i;
    HI_U64 u64PhyAddrHigh, u64PhyAddrLow;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeCfg);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (g_LdciTprFltEn[ViPipe] == HI_FALSE) {
        return HI_SUCCESS;
    }

    for (i = 0; i < pstDrvCtx->stWorkMode.block_num; i++) {
        u64PhyAddrHigh = pstBeCfg->be_reg_cfg[i].stViProcReg.VIPROC_OUT_PARA_DCI_ADDR_HIGH.u32;
        u64PhyAddrLow  = pstBeCfg->be_reg_cfg[i].stViProcReg.VIPROC_OUT_PARA_DCI_ADDR_LOW.u32;
        pstDrvCtx->stLdciSttAddr.cur_write_stt_addr[i] = (u64PhyAddrHigh << 32) + u64PhyAddrLow;

        u64PhyAddrHigh = pstBeCfg->be_reg_cfg[i].stViProcReg.VIPROC_PARA_DCI_ADDR_HIGH.u32;
        u64PhyAddrLow  = pstBeCfg->be_reg_cfg[i].stViProcReg.VIPROC_PARA_DCI_ADDR_LOW.u32;
        pstDrvCtx->stLdciSttAddr.read_stt_addr[i]     = (u64PhyAddrHigh << 32) + u64PhyAddrLow;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
