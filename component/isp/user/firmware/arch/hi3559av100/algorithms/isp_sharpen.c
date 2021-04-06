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

#define     SHRP_SHT_VAR_MUL_PRECS              (4)
#define     SHRP_SKIN_EDGE_MUL_PRECS            (4)
#define     SHRP_SKIN_ACCUM_MUL_PRECS           (3)
#define     SHRP_CHR_MUL_SFT                    (4)
#define     SHRP_DETAIL_SHT_MUL_PRECS           (4)
#define     SHRP_DETAIL_CTRL_THR_DELTA          (16)

#define     SHARPEN_CLIP3(min,max,x)         ( (x)<= (min) ? (min) : ((x)>(max)?(max):(x)) )

typedef struct hiISP_SHARPEN_S {
    HI_BOOL bSharpenEn;
    HI_BOOL bSharpenMpiUpdateEn;
    HI_U32  u32IsoLast;
    /* Sharpening Yuv */
    // tmp registers
    HI_U8   u8ManualSharpenYuvEnabled;
    HI_U8   u8mfThdSftD;
    HI_U8   u8dirVarSft;
    HI_U8   u8selPixWgt;
    HI_U8   u8mfThdSelUD;
    HI_U8   u8mfThdSftUD;

    HI_U8   u8oshtVarWgt0;
    HI_U8   u8ushtVarWgt0;
    HI_U8   u8oshtVarDiffThd0;
    HI_U8   u8oshtVarDiffThd1;
    HI_U8   u8oshtVarDiffWgt1;
    HI_U8   u8ushtVarDiffWgt1;

    // MPI
    HI_U8 u8SkinUmin;
    HI_U8 u8SkinVmin;
    HI_U8 u8SkinUmax;
    HI_U8 u8SkinVmax;
    HI_U16  au16TextureStr[ISP_SHARPEN_GAIN_NUM];
    HI_U16  au16EdgeStr[ISP_SHARPEN_GAIN_NUM];
    HI_U8   au8LumaWgt[ISP_SHARPEN_LUMA_NUM];
    HI_U16  u16TextureFreq;
    HI_U16  u16EdgeFreq;
    HI_U8   u8OverShoot;
    HI_U8   u8UnderShoot;
    HI_U8   u8ShootSupStr;
    HI_U8  u8ShootSupAdj;
    HI_U8   u8DetailCtrl;
    HI_U8  u8DetailCtrlThr;
    HI_U8   u8EdgeFiltStr;
    HI_U8   u8EdgeFiltMaxCap;                            //u8EdgeFiltMaxCap    U6.0  [0, 47]
    HI_U8   u8RGain;
    HI_U8   u8GGain;
    HI_U8   u8BGain;
    HI_U8   u8SkinGain;
    HI_U8  u16MaxSharpGain;

    HI_U16 au16AutoTextureStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U16 au16AutoEdgeStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8AutoLumaWgt[ISP_SHARPEN_LUMA_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U16 au16TextureFreq[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U16 au16EdgeFreq[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8OverShoot[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8UnderShoot[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8ShootSupStr[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8ShootSupAdj[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8DetailCtrl[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8DetailCtrlThr[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8EdgeFiltStr[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8EdgeFiltMaxCap[ISP_AUTO_ISO_STRENGTH_NUM];                            //u8EdgeFiltStr    U6.0  [0, 63]
    HI_U8  au8RGain[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8GGain[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8BGain[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au8SkinGain[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8  au16MaxSharpGain[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_SHARPEN_S;

ISP_SHARPEN_S *g_pastSharpenCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define SHARPEN_GET_CTX(dev, pstCtx)   (pstCtx = g_pastSharpenCtx[dev])
#define SHARPEN_SET_CTX(dev, pstCtx)   (g_pastSharpenCtx[dev] = pstCtx)
#define SHARPEN_RESET_CTX(dev)         (g_pastSharpenCtx[dev] = HI_NULL)

HI_S32 shrp_blend(HI_U8 sft, HI_S32 wgt1, HI_S32 v1, HI_S32 wgt2, HI_S32 v2)
{
    HI_S32 res;
    res = SignedRightShift(((HI_S64)v1 * wgt1) + ((HI_S64)v2 * wgt2), sft);
    return res;
}

HI_S32 SharpenCtxInit(VI_PIPE ViPipe)
{
    ISP_SHARPEN_S *pastSharpenCtx = HI_NULL;

    SHARPEN_GET_CTX(ViPipe, pastSharpenCtx);

    if (pastSharpenCtx == HI_NULL) {
        pastSharpenCtx = (ISP_SHARPEN_S *)ISP_MALLOC(sizeof(ISP_SHARPEN_S));
        if (pastSharpenCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] SharpenCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastSharpenCtx, 0, sizeof(ISP_SHARPEN_S));

    SHARPEN_SET_CTX(ViPipe, pastSharpenCtx);

    return HI_SUCCESS;
}

HI_VOID SharpenCtxExit(VI_PIPE ViPipe)
{
    ISP_SHARPEN_S *pastSharpenCtx = HI_NULL;

    SHARPEN_GET_CTX(ViPipe, pastSharpenCtx);
    ISP_FREE(pastSharpenCtx);
    SHARPEN_RESET_CTX(ViPipe);
}

static HI_S32 SharpenCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_sharpen *sharpen)
{
    hi_u8 i, j;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            if (sharpen->auto_attr.texture_str[j][i] > 4095) {
                ISP_ERR_TRACE("Invalid stAuto.au16TextureStr[%d][%d]! Value range:[0, 4095]\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            if (sharpen->auto_attr.edge_str[j][i] > 4095) {
                ISP_ERR_TRACE("Invalid stAuto.au16EdgeStr[%d][%d]! Value range:[0, 4095]\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            if (sharpen->auto_attr.luma_wgt[j][i] > 127) {
                ISP_ERR_TRACE("Invalid stAuto.au8LumaWgt[%d][%d]! Value range:[0, 127]\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
        if (sharpen->auto_attr.texture_freq[i] > 4095) {
            ISP_ERR_TRACE("Invalid stAuto.au16TextureFreq[%d]! Value range:[0, 4095]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (sharpen->auto_attr.edge_freq[i] > 4095) {
            ISP_ERR_TRACE("Invalid stAuto.au16EdgeFreq[%d]! Value range:[0, 4095]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (sharpen->auto_attr.over_shoot[i] > 127) {
            ISP_ERR_TRACE("Invalid stAuto.au8OverShoot[%d]! Value range:[0, 127]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (sharpen->auto_attr.under_shoot[i] > 127) {
            ISP_ERR_TRACE("Invalid stAuto.au8UnderShoot[%d]! Value range:[0, 127]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.shoot_sup_adj[i] > 15) {
            ISP_ERR_TRACE("Invalid stAuto.au8ShootSupAdj[%d]! Value range:[0, 15]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.edge_filt_str[i] > 63) {
            ISP_ERR_TRACE("Invalid stAuto.au8EdgeFiltStr[%d]! Value range:[0, 63]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.edge_filt_max_cap[i] > HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX ) {
            ISP_ERR_TRACE("Invalid stAuto.au8EdgeFiltMaxCap[%d]! Value range:[0, %d]\n", i, HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.r_gain[i] > HI_ISP_SHARPEN_RGAIN_MAX ) {
            ISP_ERR_TRACE("Invalid stAuto.au8RGain[%d]! Value range:[0, %d]\n", i, HI_ISP_SHARPEN_RGAIN_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.g_gain[i] > HI_ISP_SHARPEN_GGAIN_MAX) {
            ISP_ERR_TRACE("Invalid stAuto.au8GGain[%d]! Value range:[0, %d]\n", i, HI_ISP_SHARPEN_GGAIN_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.b_gain[i] > HI_ISP_SHARPEN_BGAIN_MAX) {
            ISP_ERR_TRACE("Invalid stAuto.au8BGain[%d]! Value range:[0, %d]\n", i, HI_ISP_SHARPEN_BGAIN_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.skin_gain[i] > 31) {
            ISP_ERR_TRACE("Invalid stAuto.au8SkinGain[%d]! Value range:[0, 31]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.max_sharp_gain[i] > 0x7FF) {
            ISP_ERR_TRACE("Invalid stAuto.au16MaxSharpGain[%d]! Value range:[0, 0x7FF]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
        if (sharpen->manual_attr.texture_str[j] > 4095) {
            ISP_ERR_TRACE("Invalid stManual.au16TextureStr[%d]! Value range:[0, 4095]\n", j);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (sharpen->manual_attr.edge_str[j] > 4095) {
            ISP_ERR_TRACE("Invalid stManual.au16EdgeStr[%d]! Value range:[0, 4095]\n", j);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
        if (sharpen->manual_attr.luma_wgt[j] > 127) {
            ISP_ERR_TRACE("Invalid stManual.au8LumaWgt[%d]! Value range:[0, 127]\n", j);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    if (sharpen->manual_attr.texture_freq > 4095) {
        ISP_ERR_TRACE("Invalid stManual.u16TextureFreq! Value range:[0, 4095]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.edge_freq > 4095) {
        ISP_ERR_TRACE("Invalid stManual.u16EdgeFreq! Value range:[0, 4095]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.over_shoot > 127) {
        ISP_ERR_TRACE("Invalid stManual.u8OverShoot! Value range:[0, 127]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.under_shoot > 127) {
        ISP_ERR_TRACE("Invalid stManual.u8UnderShoot! Value range:[0, 127]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.shoot_sup_adj > 15) {
        ISP_ERR_TRACE("Invalid stManual.u8ShootSupAdj! Value range:[0, 15]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.edge_filt_str > 63) {
        ISP_ERR_TRACE("Invalid stManual.u8EdgeFiltStr! Value range:[0, 63]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.edge_filt_max_cap > HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX) {
        ISP_ERR_TRACE("Invalid stManual.u8EdgeFiltMaxCap! Value range:[0, %d]\n", HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.r_gain > HI_ISP_SHARPEN_RGAIN_MAX) {
        ISP_ERR_TRACE("Invalid stManual.u8RGain! Value range:[0, %d]\n", HI_ISP_SHARPEN_RGAIN_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.g_gain > HI_ISP_SHARPEN_GGAIN_MAX) {
        ISP_ERR_TRACE("Invalid stManual.u8GGain! Value range:[0, %d]\n", HI_ISP_SHARPEN_GGAIN_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.b_gain > HI_ISP_SHARPEN_BGAIN_MAX) {
        ISP_ERR_TRACE("Invalid stManual.u8BGain! Value range:[0, %d]\n", HI_ISP_SHARPEN_BGAIN_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.skin_gain > 31) {
        ISP_ERR_TRACE("Invalid stManual.u8SkinGain! Value range:[0, 31]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (sharpen->manual_attr.max_sharp_gain > 0x7FF) {
        ISP_ERR_TRACE("Invalid stManual.u16MaxSharpGain! Value range:[0, 0x7FF]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static HI_S32 SharpenExtRegsInitialize(VI_PIPE ViPipe)
{

    HI_U16 i, j;
    HI_S32 s32Ret;

    hi_isp_cmos_default       *sns_dft      = HI_NULL;
    const hi_isp_cmos_sharpen *cmos_sharpen = HI_NULL;

    isp_sensor_get_default(ViPipe, &sns_dft);

    hi_ext_system_isp_sharpen_manu_mode_write(ViPipe, OP_TYPE_AUTO);
    hi_ext_system_manual_isp_sharpen_en_write(ViPipe, HI_TRUE);
    hi_ext_system_sharpen_mpi_update_en_write(ViPipe, HI_TRUE);

    // auto ExtRegs initial
    if (sns_dft->key.bit1_sharpen) {
        ISP_CHECK_POINTER(sns_dft->sharpen);
        s32Ret = SharpenCheckCmosParam(ViPipe, sns_dft->sharpen);
        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }
        cmos_sharpen = sns_dft->sharpen;

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                hi_ext_system_Isp_sharpen_TextureStr_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, cmos_sharpen->auto_attr.texture_str[j][i]);
                hi_ext_system_Isp_sharpen_EdgeStr_write   (ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, cmos_sharpen->auto_attr.edge_str[j][i]);
            }

            for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                hi_ext_system_Isp_sharpen_LumaWgt_write    (ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, cmos_sharpen->auto_attr.luma_wgt[j][i]);
            }

            hi_ext_system_Isp_sharpen_TextureFreq_write    (ViPipe, i, cmos_sharpen->auto_attr.texture_freq[i]);
            hi_ext_system_Isp_sharpen_EdgeFreq_write       (ViPipe, i, cmos_sharpen->auto_attr.edge_freq[i]);
            hi_ext_system_Isp_sharpen_OverShoot_write      (ViPipe, i, cmos_sharpen->auto_attr.over_shoot[i]);
            hi_ext_system_Isp_sharpen_UnderShoot_write     (ViPipe, i, cmos_sharpen->auto_attr.under_shoot[i]);
            hi_ext_system_Isp_sharpen_shootSupStr_write    (ViPipe, i, cmos_sharpen->auto_attr.shoot_sup_str[i]);
            hi_ext_system_Isp_sharpen_detailctrl_write     (ViPipe, i, cmos_sharpen->auto_attr.detail_ctrl[i]);
            hi_ext_system_Isp_sharpen_EdgeFiltStr_write    (ViPipe, i, cmos_sharpen->auto_attr.edge_filt_str[i]);
            hi_ext_system_Isp_sharpen_EdgeFiltMaxCap_write    (ViPipe, i, cmos_sharpen->auto_attr.edge_filt_max_cap[i]);
            hi_ext_system_Isp_sharpen_RGain_write          (ViPipe, i, cmos_sharpen->auto_attr.r_gain[i]);
            hi_ext_system_Isp_sharpen_GGain_write          (ViPipe, i, cmos_sharpen->auto_attr.g_gain[i]);
            hi_ext_system_Isp_sharpen_BGain_write          (ViPipe, i, cmos_sharpen->auto_attr.b_gain[i]);
            hi_ext_system_Isp_sharpen_SkinGain_write       (ViPipe, i, cmos_sharpen->auto_attr.skin_gain[i]);
            hi_ext_system_Isp_sharpen_ShootSupAdj_write(ViPipe, i, cmos_sharpen->auto_attr.shoot_sup_adj[i]);
            hi_ext_system_Isp_sharpen_MaxSharpGain_write(ViPipe, i, cmos_sharpen->auto_attr.max_sharp_gain[i]);
            hi_ext_system_Isp_sharpen_detailctrlThr_write(ViPipe, i, cmos_sharpen->auto_attr.detail_ctrl_thr[i]);
        }

        // manual ExtRegs initial
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            hi_ext_system_manual_Isp_sharpen_TextureStr_write(ViPipe, i, cmos_sharpen->manual_attr.texture_str[i]);
            hi_ext_system_manual_Isp_sharpen_EdgeStr_write(ViPipe, i, cmos_sharpen->manual_attr.edge_str[i]);
        }
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            hi_ext_system_manual_Isp_sharpen_LumaWgt_write(ViPipe, i, cmos_sharpen->manual_attr.luma_wgt[i]);
        }

        hi_ext_system_manual_Isp_sharpen_TextureFreq_write   (ViPipe, cmos_sharpen->manual_attr.texture_freq);
        hi_ext_system_manual_Isp_sharpen_EdgeFreq_write      (ViPipe, cmos_sharpen->manual_attr.edge_freq);
        hi_ext_system_manual_Isp_sharpen_OverShoot_write     (ViPipe, cmos_sharpen->manual_attr.over_shoot);
        hi_ext_system_manual_Isp_sharpen_UnderShoot_write    (ViPipe, cmos_sharpen->manual_attr.under_shoot);
        hi_ext_system_manual_Isp_sharpen_shootSupStr_write   (ViPipe, cmos_sharpen->manual_attr.shoot_sup_str);
        hi_ext_system_manual_Isp_sharpen_detailctrl_write    (ViPipe, cmos_sharpen->manual_attr.detail_ctrl);
        hi_ext_system_manual_Isp_sharpen_EdgeFiltStr_write   (ViPipe, cmos_sharpen->manual_attr.edge_filt_str);
        hi_ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_write(ViPipe, cmos_sharpen->manual_attr.edge_filt_max_cap);
        hi_ext_system_manual_Isp_sharpen_RGain_write         (ViPipe, cmos_sharpen->manual_attr.r_gain);
        hi_ext_system_manual_Isp_sharpen_GGain_write         (ViPipe, cmos_sharpen->manual_attr.g_gain);
        hi_ext_system_manual_Isp_sharpen_BGain_write         (ViPipe, cmos_sharpen->manual_attr.b_gain);
        hi_ext_system_manual_Isp_sharpen_SkinGain_write      (ViPipe, cmos_sharpen->manual_attr.skin_gain);
        hi_ext_system_manual_Isp_sharpen_ShootSupAdj_write   (ViPipe, cmos_sharpen->manual_attr.shoot_sup_adj);
        hi_ext_system_manual_Isp_sharpen_MaxSharpGain_write  (ViPipe, cmos_sharpen->manual_attr.max_sharp_gain);
        hi_ext_system_manual_Isp_sharpen_detailctrlThr_write (ViPipe, cmos_sharpen->manual_attr.detail_ctrl_thr);
        hi_ext_system_manual_Isp_sharpen_SkinUmax_write(ViPipe, cmos_sharpen->skin_umax);
        hi_ext_system_manual_Isp_sharpen_SkinUmin_write(ViPipe, cmos_sharpen->skin_umin);
        hi_ext_system_manual_Isp_sharpen_SkinVmax_write(ViPipe, cmos_sharpen->skin_vmax);
        hi_ext_system_manual_Isp_sharpen_SkinVmin_write(ViPipe, cmos_sharpen->skin_vmin);

        {
            hi_ext_system_actual_sharpen_overshootAmt_write     (ViPipe, cmos_sharpen->manual_attr.over_shoot);
            hi_ext_system_actual_sharpen_undershootAmt_write    (ViPipe, cmos_sharpen->manual_attr.under_shoot);
            hi_ext_system_actual_sharpen_shootSupSt_write       (ViPipe, cmos_sharpen->manual_attr.shoot_sup_str);
            hi_ext_system_actual_sharpen_edge_frequence_write   (ViPipe, cmos_sharpen->manual_attr.edge_freq);
            hi_ext_system_actual_sharpen_texture_frequence_write(ViPipe, cmos_sharpen->manual_attr.texture_freq);
            for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
                hi_ext_system_actual_sharpen_edge_str_write   (ViPipe, i, cmos_sharpen->manual_attr.edge_str[i]);
                hi_ext_system_actual_sharpen_texture_str_write(ViPipe, i, cmos_sharpen->manual_attr.texture_str[i]);
            }
        }
    } else {
        // auto ExtRegs initial
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                hi_ext_system_Isp_sharpen_TextureStr_write(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT);
                hi_ext_system_Isp_sharpen_EdgeStr_write   (ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT);
            }
            for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                hi_ext_system_Isp_sharpen_LumaWgt_write    (ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT);
            }

            hi_ext_system_Isp_sharpen_TextureFreq_write  (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT);
            hi_ext_system_Isp_sharpen_EdgeFreq_write     (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT);
            hi_ext_system_Isp_sharpen_OverShoot_write    (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT);
            hi_ext_system_Isp_sharpen_UnderShoot_write   (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT);
            hi_ext_system_Isp_sharpen_shootSupStr_write  (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT);
            hi_ext_system_Isp_sharpen_detailctrl_write   (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT);
            hi_ext_system_Isp_sharpen_EdgeFiltStr_write  (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT);
            hi_ext_system_Isp_sharpen_EdgeFiltMaxCap_write (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTMAXCAP_DEFAULT);
            hi_ext_system_Isp_sharpen_RGain_write        (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT);
            hi_ext_system_Isp_sharpen_GGain_write        (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_GGAIN_DEFAULT);
            hi_ext_system_Isp_sharpen_BGain_write        (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT);
            hi_ext_system_Isp_sharpen_SkinGain_write     (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT);
            hi_ext_system_Isp_sharpen_ShootSupAdj_write  (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPADJ_DEFAULT);
            hi_ext_system_Isp_sharpen_MaxSharpGain_write (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_MAXSHARPGAIN_DEFAULT);
            hi_ext_system_Isp_sharpen_detailctrlThr_write(ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRLTHR_DEFAULT);
        }

        // manual ExtRegs initial
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            hi_ext_system_manual_Isp_sharpen_TextureStr_write(ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT);
            hi_ext_system_manual_Isp_sharpen_EdgeStr_write(ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT);
        }
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            hi_ext_system_manual_Isp_sharpen_LumaWgt_write(ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT);
        }

        hi_ext_system_manual_Isp_sharpen_TextureFreq_write (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_EdgeFreq_write    (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_OverShoot_write   (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_UnderShoot_write  (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_shootSupStr_write (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_detailctrl_write  (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_EdgeFiltStr_write (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_write(ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTMAXCAP_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_RGain_write       (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_GGain_write       (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_GGAIN_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_BGain_write       (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_SkinGain_write    (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_ShootSupAdj_write (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPADJ_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_MaxSharpGain_write(ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_MAXSHARPGAIN_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_detailctrlThr_write(ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRLTHR_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_SkinUmax_write(ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINUMAX_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_SkinUmin_write(ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINUMIN_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_SkinVmax_write(ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINVMAX_DEFAULT);
        hi_ext_system_manual_Isp_sharpen_SkinVmin_write(ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINVMIN_DEFAULT);

        hi_ext_system_actual_sharpen_overshootAmt_write      (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT);
        hi_ext_system_actual_sharpen_undershootAmt_write     (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT);
        hi_ext_system_actual_sharpen_shootSupSt_write        (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT);
        hi_ext_system_actual_sharpen_edge_frequence_write    (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT);
        hi_ext_system_actual_sharpen_texture_frequence_write (ViPipe, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT);

        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            hi_ext_system_actual_sharpen_edge_str_write      (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT);
            hi_ext_system_actual_sharpen_texture_str_write   (ViPipe, i, HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT);
        }
    }

    return HI_SUCCESS;
}

static void SharpenCheckStaticReg(ISP_SHARPEN_STATIC_REG_CFG_S *pstShrpReg)
{

    HI_U32 i;

    // Direction judgement stbEnChrSad
    pstShrpReg->u8bEnChrSad       = SHARPEN_CLIP3(0, 0x1,  pstShrpReg->u8bEnChrSad);       // U1.0
    pstShrpReg->u8dirVarScale     = SHARPEN_CLIP3(0, 0xC,  pstShrpReg->u8dirVarScale);     // U4.0
    pstShrpReg->u8mfThdSelD       = SHARPEN_CLIP3(0, 0x2, pstShrpReg->u8mfThdSelD);        // U2.0
    pstShrpReg->u8hfThdSelD       = SHARPEN_CLIP3(0, 0x2, pstShrpReg->u8hfThdSelD);        // U2.0

    for (i = 0; i < 2; i++) {
        pstShrpReg->u8dirRly[i]   = SHARPEN_CLIP3(0, 0x7F,  pstShrpReg->u8dirRly[i]);      // U0.7
    }

    // Control sharpen based on luma
    pstShrpReg->u8lumaSrcSel      = SHARPEN_CLIP3(0, 0x1, pstShrpReg->u8lumaSrcSel);       // U1.0

    // Shoot control
    pstShrpReg->u16oMaxChg        = SHARPEN_CLIP3(0, 0x3FF, pstShrpReg->u16oMaxChg);       // U10.0
    pstShrpReg->u16uMaxChg        = SHARPEN_CLIP3(0, 0x3FF, pstShrpReg->u16uMaxChg);       // U10.0

    // Control shoot based on variance
    pstShrpReg->u8shtVarSft       = SHARPEN_CLIP3(0, 0x7,  pstShrpReg->u8shtVarSft);         // U3.0
    pstShrpReg->u8ushtVarDiffWgt0 = SHARPEN_CLIP3(0, 0x7F, pstShrpReg->u8ushtVarDiffWgt0);   // U0.7
    pstShrpReg->u8oshtVarDiffWgt0 = SHARPEN_CLIP3(0, 0x7F, pstShrpReg->u8oshtVarDiffWgt0);   // U0.7
    pstShrpReg->u8oshtVarWgt1     = SHARPEN_CLIP3(0, 0x7F, pstShrpReg->u8oshtVarWgt1);       // U0.7
    pstShrpReg->u8ushtVarWgt1     = SHARPEN_CLIP3(0, 0x7F, pstShrpReg->u8ushtVarWgt1);       // U0.7

    // Coring
    for (i = 0; i < ISP_SHARPEN_FREQ_CORING_LENGTH; i++) {
        pstShrpReg->u8lmtMF[i]    = SHARPEN_CLIP3(0, 0x3F, pstShrpReg->u8lmtMF[i]); // U6.0
        pstShrpReg->u8lmtHF[i]    = SHARPEN_CLIP3(0, 0x3F, pstShrpReg->u8lmtHF[i]); // U6.0
    }

    // Skin
    pstShrpReg->u8skinSrcSel      = SHARPEN_CLIP3(0, 0x1, pstShrpReg->u8skinSrcSel);        // U1.0
    pstShrpReg->u8skinCntThd[0]   = SHARPEN_CLIP3(0, 0x9, pstShrpReg->u8skinCntThd[0]);   // U4.0, [0,9]
    pstShrpReg->u8skinCntThd[1]   = SHARPEN_CLIP3(0, 0x9, pstShrpReg->u8skinCntThd[1]);   // U4.0, [0,9]
    pstShrpReg->u8skinEdgeSft     = SHARPEN_CLIP3(0, 0xC, pstShrpReg->u8skinEdgeSft);     // U4.0

    for (i = 0; i < 2; i++) {
        pstShrpReg->u16skinAccumThd[i] = SHARPEN_CLIP3(0, 0x1FF, pstShrpReg->u16skinAccumThd[i]);  // U9.0
        pstShrpReg->u8skinAccumWgt[i]  = SHARPEN_CLIP3(0, 0x1F, pstShrpReg->u8skinAccumWgt[i]);    // U0.5
    }

    // Chroma modification
    pstShrpReg->u8chrRVarSft   = SHARPEN_CLIP3(0, 0xA, pstShrpReg->u8chrRVarSft);       // U4.0, [0,10]
    pstShrpReg->u8chrRVarScale = SHARPEN_CLIP3(0, 0xA, pstShrpReg->u8chrRVarScale);     // U4.0, [0,10]
    pstShrpReg->u16chrRScale   = SHARPEN_CLIP3(0, 0x1FF, pstShrpReg->u16chrRScale);     // U4.5
    pstShrpReg->u8chrGVarSft   = SHARPEN_CLIP3(0, 0xA, pstShrpReg->u8chrGVarSft);       // U4.0, [0,10]
    pstShrpReg->u8chrGVarScale = SHARPEN_CLIP3(0, 0xA, pstShrpReg->u8chrGVarScale);     // U4.0, [0,10]
    pstShrpReg->u16chrGScale   = SHARPEN_CLIP3(0, 0x1FF, pstShrpReg->u16chrGScale);     // U4.5
    pstShrpReg->u8chrBVarSft   = SHARPEN_CLIP3(0, 0xA, pstShrpReg->u8chrBVarSft);       // U4.0, [0,10]
    pstShrpReg->u8chrBVarScale = SHARPEN_CLIP3(0, 0xA, pstShrpReg->u8chrBVarScale);     // U4.0, [0,10]
    pstShrpReg->u16chrBScale   = SHARPEN_CLIP3(0, 0x1FF, pstShrpReg->u16chrBScale);     // U4.5
    pstShrpReg->u16chrGGain1   =  SHARPEN_CLIP3(0, 0x1FF, pstShrpReg->u16chrGGain1);    // U4.5
    pstShrpReg->u8chrRGain1    =  SHARPEN_CLIP3(0, 0x1F, pstShrpReg->u8chrRGain1);      // U0.5
    pstShrpReg->u8chrBGain1    =  SHARPEN_CLIP3(0, 0x1F, pstShrpReg->u8chrBGain1);      // U0.5

    for (i = 0; i < 4; i++) {
        pstShrpReg->u8chrRSft[i]  =  SHARPEN_CLIP3(0, 0x7, pstShrpReg->u8chrRSft[i]);   // U0.3
        pstShrpReg->u8chrGSft[i]  =  SHARPEN_CLIP3(0, 0x7, pstShrpReg->u8chrGSft[i]);   // U0.3
        pstShrpReg->u8chrBSft[i]  =  SHARPEN_CLIP3(0, 0x7, pstShrpReg->u8chrBSft[i]);   // U0.3
    }
    for (i = 0; i < 2; i++) {
        // pstShrpReg->u8dirRt[i]  = SHARPEN_CLIP3(0, 0x1F,  pstShrpReg->u8dirRt[i]);      // U0.5
    }
    pstShrpReg->bEnShp8Dir      = SHARPEN_CLIP3(0, 0x1, pstShrpReg->bEnShp8Dir);
    pstShrpReg->bEnShpLowPow    = SHARPEN_CLIP3(0, 0x1, pstShrpReg->bEnShpLowPow);
    pstShrpReg->u8hfGainSft     = SHARPEN_CLIP3(0, 0x7, pstShrpReg->u8hfGainSft);
    pstShrpReg->u8mfGainSft     = SHARPEN_CLIP3(0, 0x7, pstShrpReg->u8mfGainSft);
    pstShrpReg->u8lpfSel        = SHARPEN_CLIP3(0, 0x1, pstShrpReg->u8lpfSel);
    pstShrpReg->u8hsfSel        = SHARPEN_CLIP3(0, 0x2, pstShrpReg->u8hsfSel);
    pstShrpReg->u8shtVarSel     = SHARPEN_CLIP3(0, 0x1, pstShrpReg->u8shtVarSel);
    pstShrpReg->u8shtVar5x5Sft  = SHARPEN_CLIP3(0, 12, pstShrpReg->u8shtVar5x5Sft);
    pstShrpReg->u8detailThdSel  = SHARPEN_CLIP3(0, 3, pstShrpReg->u8detailThdSel);
    pstShrpReg->u8detailThdSft  = SHARPEN_CLIP3(0, 12, pstShrpReg->u8detailThdSft);
}

static void SharpenCheckDefDynaReg(ISP_SHARPEN_DYNA_REG_CFG_S *pstShrpReg)
{
    ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S *pstSharpDefaultDynaRegCfg = HI_NULL;

    pstSharpDefaultDynaRegCfg = &(pstShrpReg->stDefaultDynaRegCfg);

    pstSharpDefaultDynaRegCfg->u8mfThdSftD   = SHARPEN_CLIP3(0, 0xf, pstSharpDefaultDynaRegCfg->u8mfThdSftD);   // U4.0
    pstSharpDefaultDynaRegCfg->u8mfThdSelUD  = SHARPEN_CLIP3(0, 0x2, pstSharpDefaultDynaRegCfg->u8mfThdSelUD);  // U2.0
    pstSharpDefaultDynaRegCfg->u8mfThdSftUD  = SHARPEN_CLIP3(0, 0xf, pstSharpDefaultDynaRegCfg->u8mfThdSftUD);  // U4.0
    pstSharpDefaultDynaRegCfg->u8hfThdSftD   = SHARPEN_CLIP3(0, 0xf, pstSharpDefaultDynaRegCfg->u8hfThdSftD);   // U4.0
    pstSharpDefaultDynaRegCfg->u8hfThdSelUD  = SHARPEN_CLIP3(0, 0x2, pstSharpDefaultDynaRegCfg->u8hfThdSelUD);  // U2.0
    pstSharpDefaultDynaRegCfg->u8hfThdSftUD  = SHARPEN_CLIP3(0, 0xf, pstSharpDefaultDynaRegCfg->u8hfThdSftUD);  // U4.0
    pstSharpDefaultDynaRegCfg->u8dirVarSft   = SHARPEN_CLIP3(0, 0xC,  pstSharpDefaultDynaRegCfg->u8dirVarSft);  // U4.0

    pstSharpDefaultDynaRegCfg->u8ushtVarDiffWgt1 = SHARPEN_CLIP3(0, 0x7F, pstSharpDefaultDynaRegCfg->u8ushtVarDiffWgt1);   // U0.7
    pstSharpDefaultDynaRegCfg->u8oshtVarDiffWgt1 = SHARPEN_CLIP3(0, 0x7F, pstSharpDefaultDynaRegCfg->u8oshtVarDiffWgt1);   // U0.7
    pstSharpDefaultDynaRegCfg->u8oshtVarWgt0     = SHARPEN_CLIP3(0, 0x7F, pstSharpDefaultDynaRegCfg->u8oshtVarWgt0);       // U0.7
    pstSharpDefaultDynaRegCfg->u8ushtVarWgt0     = SHARPEN_CLIP3(0, 0x7F, pstSharpDefaultDynaRegCfg->u8ushtVarWgt0);       // U0.7
    pstSharpDefaultDynaRegCfg->u8selPixWgt       = SHARPEN_CLIP3(0, 0x1F,  pstSharpDefaultDynaRegCfg->u8selPixWgt);        // U0.5
}

static void SharpenCheckMpiDynaReg(ISP_SHARPEN_DYNA_REG_CFG_S *pstShrpReg)
{
    HI_U32 i;

    ISP_SHARPEN_MPI_DYNA_REG_CFG_S *pstSharpMpiDynaRegCfg = HI_NULL;

    pstSharpMpiDynaRegCfg = &(pstShrpReg->stMpiDynaRegCfg);

    // MPI
    pstSharpMpiDynaRegCfg->u8bEnShtCtrlByVar = SHARPEN_CLIP3(0, 0x1,  pstSharpMpiDynaRegCfg->u8bEnShtCtrlByVar);   // U1.0
    pstSharpMpiDynaRegCfg->u8shtBldRt        = SHARPEN_CLIP3(0, 0xF,  pstSharpMpiDynaRegCfg->u8shtBldRt);          // U4.0
    pstSharpMpiDynaRegCfg->u8oshtAmt         = SHARPEN_CLIP3(0, 0x7F,  pstSharpMpiDynaRegCfg->u8oshtAmt);          // U0.7
    pstSharpMpiDynaRegCfg->u8ushtAmt         = SHARPEN_CLIP3(0, 0x7F,  pstSharpMpiDynaRegCfg->u8ushtAmt);          // U0.7
    pstSharpMpiDynaRegCfg->u8bEnChrCtrl      = SHARPEN_CLIP3(0, 0x1, pstSharpMpiDynaRegCfg->u8bEnChrCtrl);         // U1.0
    pstSharpMpiDynaRegCfg->u8chrBGain0       = SHARPEN_CLIP3(0, 0x1F, pstSharpMpiDynaRegCfg->u8chrBGain0);         // U0.5
    pstSharpMpiDynaRegCfg->u8chrRGain0       = SHARPEN_CLIP3(0, 0x1F, pstSharpMpiDynaRegCfg->u8chrRGain0);         // U0.5
    pstSharpMpiDynaRegCfg->u16chrGGain0      = SHARPEN_CLIP3(0, 0xFF, pstSharpMpiDynaRegCfg->u16chrGGain0);         // U0.5
    pstSharpMpiDynaRegCfg->u8bEnSkinCtrl     = SHARPEN_CLIP3(0, 0x1, pstSharpMpiDynaRegCfg->u8bEnSkinCtrl);        // U1.0
    pstSharpMpiDynaRegCfg->u8skinEdgeWgt[0]  = SHARPEN_CLIP3(0, 0x1F, pstSharpMpiDynaRegCfg->u8skinEdgeWgt[0]);    // U0.5
    pstSharpMpiDynaRegCfg->u8skinEdgeWgt[1]  = SHARPEN_CLIP3(0, 0x1F, pstSharpMpiDynaRegCfg->u8skinEdgeWgt[1]);    // U0.5
    pstSharpMpiDynaRegCfg->u8bEnLumaCtrl     = SHARPEN_CLIP3(0, 0x1, pstSharpMpiDynaRegCfg->u8bEnLumaCtrl);        // U1.0
    for (i = 0; i < SHRP_GAIN_LUT_SIZE; i++) {
        pstSharpMpiDynaRegCfg->u16mfGainD[i]  = SHARPEN_CLIP3(0, 0xFFF, pstSharpMpiDynaRegCfg->u16mfGainD[i]);     // U7.5
        pstSharpMpiDynaRegCfg->u16mfGainUD[i] = SHARPEN_CLIP3(0, 0xFFF, pstSharpMpiDynaRegCfg->u16mfGainUD[i]);    // U7.5
        pstSharpMpiDynaRegCfg->u16hfGainD[i]  = SHARPEN_CLIP3(0, 0xFFF, pstSharpMpiDynaRegCfg->u16hfGainD[i]);     // U7.5
        pstSharpMpiDynaRegCfg->u16hfGainUD[i] = SHARPEN_CLIP3(0, 0xFFF, pstSharpMpiDynaRegCfg->u16hfGainUD[i]);    // U7.5
    }
    for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
        pstSharpMpiDynaRegCfg->au8LumaWgt[i] = SHARPEN_CLIP3(0, 0x7F,  pstSharpMpiDynaRegCfg->au8LumaWgt[i]);      // U0.7
    }
    pstSharpMpiDynaRegCfg->bEnDetailCtrl     = SHARPEN_CLIP3(0, 0x1,  pstSharpMpiDynaRegCfg->bEnDetailCtrl);
    pstSharpMpiDynaRegCfg->u8detailOshtAmt   = SHARPEN_CLIP3(0, 0x7F,  pstSharpMpiDynaRegCfg->u8detailOshtAmt);
    pstSharpMpiDynaRegCfg->u8detailUshtAmt   = SHARPEN_CLIP3(0, 0x7F,  pstSharpMpiDynaRegCfg->u8detailUshtAmt);
    pstSharpMpiDynaRegCfg->u8dirDiffSft      = SHARPEN_CLIP3(0, 0x3F, pstSharpMpiDynaRegCfg->u8dirDiffSft);        // U6.0
    for (i = 0; i < 2; i++) {
        pstSharpMpiDynaRegCfg->u8dirRt[i]  = SHARPEN_CLIP3(0, 47,  pstSharpMpiDynaRegCfg->u8dirRt[i]);      // U0.5
    }

    pstSharpMpiDynaRegCfg->u16oMaxGain       = SHARPEN_CLIP3(0, 0x7FF, pstSharpMpiDynaRegCfg->u16oMaxGain);      // U11.0
    pstSharpMpiDynaRegCfg->u16uMaxGain       = SHARPEN_CLIP3(0, 0x7FF, pstSharpMpiDynaRegCfg->u16uMaxGain);      // U11.0
    pstSharpMpiDynaRegCfg->u16skinMaxU       = SHARPEN_CLIP3(0, 0x3FF, pstSharpMpiDynaRegCfg->u16skinMaxU);       // U10.0
    pstSharpMpiDynaRegCfg->u16skinMinU       = SHARPEN_CLIP3(0, 0x3FF, pstSharpMpiDynaRegCfg->u16skinMinU);       // U10.0
    pstSharpMpiDynaRegCfg->u16skinMaxV       = SHARPEN_CLIP3(0, 0x3FF, pstSharpMpiDynaRegCfg->u16skinMaxV);       // U10.0
    pstSharpMpiDynaRegCfg->u16skinMinV       = SHARPEN_CLIP3(0, 0x3FF, pstSharpMpiDynaRegCfg->u16skinMinV);       // U10.0

}

// ****Sharpen hardware Regs that will not change****//
static HI_VOID SharpenStaticRegInit(VI_PIPE ViPipe, ISP_SHARPEN_STATIC_REG_CFG_S *pstSharpenStaticRegCfg)
{
    HI_U8 i;
    pstSharpenStaticRegCfg->bStaticResh         = HI_TRUE;
    pstSharpenStaticRegCfg->u8hfThdSelD         = 1;
    pstSharpenStaticRegCfg->u8mfThdSelD         = 1;
    pstSharpenStaticRegCfg->u8dirVarScale       = 0;
    pstSharpenStaticRegCfg->u8bEnChrSad         = 1;
    pstSharpenStaticRegCfg->u8dirRly[0]         = 127;
    pstSharpenStaticRegCfg->u8dirRly[1]         = 0;
    pstSharpenStaticRegCfg->u16oMaxChg          = 1000;
    pstSharpenStaticRegCfg->u16uMaxChg          = 1000;
    pstSharpenStaticRegCfg->u8shtVarSft         = 2;
    for (i = 0; i < ISP_SHARPEN_FREQ_CORING_LENGTH; i++) {
        pstSharpenStaticRegCfg->u8lmtMF[i]      = 0;
        pstSharpenStaticRegCfg->u8lmtHF[i]      = 0;
    }
    pstSharpenStaticRegCfg->u8skinSrcSel        = 0;
    pstSharpenStaticRegCfg->u8skinCntThd[0]     = 5;
    pstSharpenStaticRegCfg->u8skinEdgeThd[0]    = 10;
    pstSharpenStaticRegCfg->u16skinAccumThd[0]  = 0;
    pstSharpenStaticRegCfg->u8skinAccumWgt[0]   = 0;
    pstSharpenStaticRegCfg->u8skinCntThd[1]     = 8;
    pstSharpenStaticRegCfg->u8skinEdgeThd[1]    = 30;
    pstSharpenStaticRegCfg->u16skinAccumThd[1]  = 10;
    pstSharpenStaticRegCfg->u8skinAccumWgt[1]   = 20;
    pstSharpenStaticRegCfg->u8skinEdgeSft       = 3;
    pstSharpenStaticRegCfg->u8chrROri[0]        = 120;
    pstSharpenStaticRegCfg->u8chrRThd[0]        = 40;
    pstSharpenStaticRegCfg->u8chrGOri[0]        = 95;
    pstSharpenStaticRegCfg->u8chrGThd[0]        = 20;
    pstSharpenStaticRegCfg->u8chrBThd[0]        = 50;
    pstSharpenStaticRegCfg->u8chrBOri[0]        = 200;
    pstSharpenStaticRegCfg->u8chrROri[1]        = 192;
    pstSharpenStaticRegCfg->u8chrRThd[1]        = 60;
    pstSharpenStaticRegCfg->u8chrRGain1         = 31;
    pstSharpenStaticRegCfg->u8chrGOri[1]        = 95;
    pstSharpenStaticRegCfg->u8chrGThd[1]        = 40;
    pstSharpenStaticRegCfg->u16chrGGain1        = 32;
    pstSharpenStaticRegCfg->u8chrBThd[1]        = 100;
    pstSharpenStaticRegCfg->u8chrBGain1         = 31;
    pstSharpenStaticRegCfg->u8chrBOri[1]        = 64;
    pstSharpenStaticRegCfg->u8chrRVarSft        = 5;
    pstSharpenStaticRegCfg->u8chrRVarScale      = 3;
    pstSharpenStaticRegCfg->u16chrRScale        = 32;
    pstSharpenStaticRegCfg->u8chrGSft[0]        = 4;
    pstSharpenStaticRegCfg->u8chrGSft[1]        = 7;
    pstSharpenStaticRegCfg->u8chrGSft[2]        = 4;
    pstSharpenStaticRegCfg->u8chrGSft[3]        = 7;
    pstSharpenStaticRegCfg->u8chrRSft[0]        = 5;
    pstSharpenStaticRegCfg->u8chrRSft[1]        = 7;
    pstSharpenStaticRegCfg->u8chrRSft[2]        = 7;
    pstSharpenStaticRegCfg->u8chrRSft[3]        = 7;
    pstSharpenStaticRegCfg->u8chrBSft[0]        = 7;
    pstSharpenStaticRegCfg->u8chrBSft[1]        = 7;
    pstSharpenStaticRegCfg->u8chrBSft[2]        = 7;
    pstSharpenStaticRegCfg->u8chrBSft[3]        = 7;
    pstSharpenStaticRegCfg->u8chrGVarSft        = 10;
    pstSharpenStaticRegCfg->u8chrGVarScale      = 0;
    pstSharpenStaticRegCfg->u16chrGScale        = 32;
    pstSharpenStaticRegCfg->u8chrBVarSft        = 4;
    pstSharpenStaticRegCfg->u8chrBVarScale      = 3;
    pstSharpenStaticRegCfg->u16chrBScale        = 32;
    pstSharpenStaticRegCfg->u8oshtVarWgt1       = 127;
    pstSharpenStaticRegCfg->u8ushtVarWgt1       = 127;
    pstSharpenStaticRegCfg->u8ushtVarDiffWgt0   = 127;
    pstSharpenStaticRegCfg->u8oshtVarDiffWgt0   = 127;
    pstSharpenStaticRegCfg->u8oshtVarThd0       = 0;
    pstSharpenStaticRegCfg->u8ushtVarThd0       = 0;
    pstSharpenStaticRegCfg->u8lumaSrcSel        = 0;
    //pstSharpenStaticRegCfg->u8dirRt[0]          = 8;
    //pstSharpenStaticRegCfg->u8dirRt[1]          = 16;
    pstSharpenStaticRegCfg->u8shtNoiseMin       = 0;
    pstSharpenStaticRegCfg->u8shtNoiseMax       = 0;
    pstSharpenStaticRegCfg->bEnShp8Dir          = 1;
    pstSharpenStaticRegCfg->bEnShpLowPow        = 1;
    pstSharpenStaticRegCfg->u8hfGainSft         = 5;
    pstSharpenStaticRegCfg->u8mfGainSft         = 5;
    pstSharpenStaticRegCfg->u8lpfSel            = 1;
    pstSharpenStaticRegCfg->u8hsfSel            = 2;
    pstSharpenStaticRegCfg->u8shtVarSel         = 1;
    pstSharpenStaticRegCfg->u8shtVar5x5Sft      = 3;
    pstSharpenStaticRegCfg->u8detailThdSel      = 0;
    pstSharpenStaticRegCfg->u8detailThdSft      = 0;


    SharpenCheckStaticReg(pstSharpenStaticRegCfg);

    // Skin detection
    pstSharpenStaticRegCfg->u8skinCntMul = CalcMulCoef(pstSharpenStaticRegCfg->u8skinCntThd[0], 0, pstSharpenStaticRegCfg->u8skinCntThd[1], 31, 0);
    pstSharpenStaticRegCfg->s16skinAccumMul =  CalcMulCoef(pstSharpenStaticRegCfg->u16skinAccumThd[0], pstSharpenStaticRegCfg->u8skinAccumWgt[0],
                                                           pstSharpenStaticRegCfg->u16skinAccumThd[1], pstSharpenStaticRegCfg->u8skinAccumWgt[1],
                                                           SHRP_SKIN_ACCUM_MUL_PRECS);
}

// ****Sharpen hardware Regs that will change with MPI and ISO****//
static HI_VOID SharpenMpiDynaRegInit(ISP_SHARPEN_MPI_DYNA_REG_CFG_S *pstSharpenMpiDynaRegCfg)
{
    HI_U8 i;

    for (i = 0; i < SHRP_GAIN_LUT_SIZE; i++) {
        pstSharpenMpiDynaRegCfg->u16mfGainD[i]  = 300;
        pstSharpenMpiDynaRegCfg->u16mfGainUD[i] = 200;
        pstSharpenMpiDynaRegCfg->u16hfGainD[i]  = 450;
        pstSharpenMpiDynaRegCfg->u16hfGainUD[i] = 400;
    }

    pstSharpenMpiDynaRegCfg->u8oshtAmt          = 100;
    pstSharpenMpiDynaRegCfg->u8ushtAmt          = 127;
    pstSharpenMpiDynaRegCfg->u8bEnShtCtrlByVar  = 1;
    pstSharpenMpiDynaRegCfg->u8shtBldRt         = 9;
    pstSharpenMpiDynaRegCfg->u8oshtVarThd1      = 5;
    pstSharpenMpiDynaRegCfg->u8ushtVarThd1      = 5;
    pstSharpenMpiDynaRegCfg->u8bEnSkinCtrl      = 0;
    pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[1]   = 31;
    pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[0]   = 31;
    pstSharpenMpiDynaRegCfg->u8bEnChrCtrl       = 1;
    pstSharpenMpiDynaRegCfg->u8chrBGain0        = 31;
    pstSharpenMpiDynaRegCfg->u8chrRGain0        = 31;
    pstSharpenMpiDynaRegCfg->u16chrGGain0       = 31;

    pstSharpenMpiDynaRegCfg->u8bEnLumaCtrl      = 0;
    pstSharpenMpiDynaRegCfg->bEnDetailCtrl      = 0;
    pstSharpenMpiDynaRegCfg->u8detailOshtAmt    = 100;
    pstSharpenMpiDynaRegCfg->u8detailUshtAmt    = 127;
    pstSharpenMpiDynaRegCfg->u8dirDiffSft = 10;
    pstSharpenMpiDynaRegCfg->u8dirRt[0]         = 6;
    pstSharpenMpiDynaRegCfg->u8dirRt[1]         = 18;
    pstSharpenMpiDynaRegCfg->u16skinMaxU         = 511;
    pstSharpenMpiDynaRegCfg->u16skinMinU         = 400;
    pstSharpenMpiDynaRegCfg->u16skinMaxV         = 600;
    pstSharpenMpiDynaRegCfg->u16skinMinV         = 540;
    pstSharpenMpiDynaRegCfg->u16oMaxGain         = 160;
    pstSharpenMpiDynaRegCfg->u16uMaxGain         = 160;
    pstSharpenMpiDynaRegCfg->u8detailOshtThr[0]  = 65;
    pstSharpenMpiDynaRegCfg->u8detailOshtThr[1]  = 90;
    pstSharpenMpiDynaRegCfg->u8detailUshtThr[0]  = 65;
    pstSharpenMpiDynaRegCfg->u8detailUshtThr[1]  = 90;

    for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
        pstSharpenMpiDynaRegCfg->au8LumaWgt[i]  = 127;
    }
    pstSharpenMpiDynaRegCfg->u32UpdateIndex     = 1;
    pstSharpenMpiDynaRegCfg->bResh              = HI_TRUE;
}

// ****Sharpen hardware Regs that will change only with ISO****//
static HI_VOID SharpenDefaultDynaRegInit(ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S *pstSharpenDefaultDynaRegCfg)
{
    pstSharpenDefaultDynaRegCfg->bResh                  = HI_TRUE;
    pstSharpenDefaultDynaRegCfg->u8mfThdSftD            = 0;
    pstSharpenDefaultDynaRegCfg->u8mfThdSelUD           = 2;
    pstSharpenDefaultDynaRegCfg->u8mfThdSftUD           = 0;
    pstSharpenDefaultDynaRegCfg->u8hfThdSftD            = 0;
    pstSharpenDefaultDynaRegCfg->u8hfThdSelUD           = 2;
    pstSharpenDefaultDynaRegCfg->u8hfThdSftUD           = 0;
    pstSharpenDefaultDynaRegCfg->u8dirVarSft            = 12;
    pstSharpenDefaultDynaRegCfg->u8selPixWgt            = 31;
    pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[0]    = 20;
    pstSharpenDefaultDynaRegCfg->u8ushtVarDiffThd[0]    = 20;
    pstSharpenDefaultDynaRegCfg->u8oshtVarWgt0          = 0;
    pstSharpenDefaultDynaRegCfg->u8ushtVarWgt0          = 0;
    pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[1]    = 35;
    pstSharpenDefaultDynaRegCfg->u8oshtVarDiffWgt1      = 5;
    pstSharpenDefaultDynaRegCfg->u8ushtVarDiffThd[1]    = 35;
    pstSharpenDefaultDynaRegCfg->u8ushtVarDiffWgt1      = 10;
}

static HI_VOID SharpenDynaRegInit(ISP_SHARPEN_REG_CFG_S *pstSharpenRegCfg)
{
    ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S *pstSharpenDefaultDynaRegCfg = HI_NULL;
    ISP_SHARPEN_MPI_DYNA_REG_CFG_S *pstSharpenMpiDynaRegCfg = HI_NULL;
    ISP_SHARPEN_STATIC_REG_CFG_S *pstSharpenStaticRegCfg = HI_NULL;

    pstSharpenDefaultDynaRegCfg = &(pstSharpenRegCfg->stDynaRegCfg.stDefaultDynaRegCfg);
    pstSharpenMpiDynaRegCfg     = &(pstSharpenRegCfg->stDynaRegCfg.stMpiDynaRegCfg);
    pstSharpenStaticRegCfg      = &(pstSharpenRegCfg->stStaticRegCfg);

    SharpenDefaultDynaRegInit(pstSharpenDefaultDynaRegCfg);
    SharpenMpiDynaRegInit(pstSharpenMpiDynaRegCfg);

    SharpenCheckDefDynaReg(&(pstSharpenRegCfg->stDynaRegCfg));
    SharpenCheckMpiDynaReg(&(pstSharpenRegCfg->stDynaRegCfg));

    /* Calc all MulCoef */
    // Control shoot based on variance
    pstSharpenMpiDynaRegCfg->u16oshtVarMul         = CalcMulCoef(pstSharpenStaticRegCfg->u8oshtVarThd0,  pstSharpenDefaultDynaRegCfg->u8oshtVarWgt0,
                                                     pstSharpenMpiDynaRegCfg->u8oshtVarThd1, pstSharpenStaticRegCfg->u8oshtVarWgt1,
                                                     SHRP_SHT_VAR_MUL_PRECS);
    pstSharpenMpiDynaRegCfg->u16ushtVarMul         = CalcMulCoef(pstSharpenStaticRegCfg->u8ushtVarThd0,  pstSharpenDefaultDynaRegCfg->u8ushtVarWgt0,
                                                     pstSharpenMpiDynaRegCfg->u8ushtVarThd1, pstSharpenStaticRegCfg->u8ushtVarWgt1,
                                                     SHRP_SHT_VAR_MUL_PRECS);
    pstSharpenDefaultDynaRegCfg->s16oshtVarDiffMul = CalcMulCoef(pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[0], pstSharpenStaticRegCfg->u8oshtVarDiffWgt0,
                                                     pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[1], pstSharpenDefaultDynaRegCfg->u8oshtVarDiffWgt1,
                                                     SHRP_SHT_VAR_MUL_PRECS);
    pstSharpenDefaultDynaRegCfg->s16ushtVarDiffMul = CalcMulCoef(pstSharpenDefaultDynaRegCfg->u8ushtVarDiffThd[0], pstSharpenStaticRegCfg->u8ushtVarDiffWgt0,
                                                     pstSharpenDefaultDynaRegCfg->u8ushtVarDiffThd[1], pstSharpenDefaultDynaRegCfg->u8ushtVarDiffWgt1,
                                                     SHRP_SHT_VAR_MUL_PRECS);
    pstSharpenMpiDynaRegCfg->s16detailOshtMul      = CalcMulCoef(pstSharpenMpiDynaRegCfg->u8detailOshtThr[0], pstSharpenMpiDynaRegCfg->u8detailOshtAmt,
                                                     pstSharpenMpiDynaRegCfg->u8detailOshtThr[1], pstSharpenMpiDynaRegCfg->u8oshtAmt,
                                                     SHRP_DETAIL_SHT_MUL_PRECS);
    pstSharpenMpiDynaRegCfg->s16detailUshtMul      = CalcMulCoef(pstSharpenMpiDynaRegCfg->u8detailUshtThr[0], pstSharpenMpiDynaRegCfg->u8detailUshtAmt,
                                                     pstSharpenMpiDynaRegCfg->u8detailUshtThr[1], pstSharpenMpiDynaRegCfg->u8ushtAmt,
                                                     SHRP_DETAIL_SHT_MUL_PRECS);

    // Chroma modification
    pstSharpenMpiDynaRegCfg->s16chrRMul            = CalcMulCoef(pstSharpenStaticRegCfg->u8chrRThd[0], pstSharpenMpiDynaRegCfg->u8chrRGain0,
                                                     pstSharpenStaticRegCfg->u8chrRThd[1], pstSharpenStaticRegCfg->u8chrRGain1,
                                                     SHRP_CHR_MUL_SFT);
    pstSharpenMpiDynaRegCfg->s16chrGMul            = CalcMulCoef(pstSharpenStaticRegCfg->u8chrGThd[0], pstSharpenMpiDynaRegCfg->u16chrGGain0,
                                                     pstSharpenStaticRegCfg->u8chrGThd[1], pstSharpenStaticRegCfg->u16chrGGain1,
                                                     SHRP_CHR_MUL_SFT);
    pstSharpenMpiDynaRegCfg->s16chrBMul            = CalcMulCoef(pstSharpenStaticRegCfg->u8chrBThd[0], pstSharpenMpiDynaRegCfg->u8chrBGain0,
                                                     pstSharpenStaticRegCfg->u8chrBThd[1], pstSharpenStaticRegCfg->u8chrBGain1,
                                                     SHRP_CHR_MUL_SFT);
    pstSharpenMpiDynaRegCfg->s16skinEdgeMul        = CalcMulCoef(pstSharpenStaticRegCfg->u8skinEdgeThd[0], pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[0],
                                                     pstSharpenStaticRegCfg->u8skinEdgeThd[1], pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[1],
                                                     SHRP_SKIN_EDGE_MUL_PRECS);
}

static HI_VOID SharpenRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pRegCfg)
{
    HI_U32 i;

    for (i = 0; i < pRegCfg->cfg_num; i++) {
        pRegCfg->alg_reg_cfg[i].stSharpenRegCfg.bEnable = HI_TRUE;
        SharpenStaticRegInit(ViPipe, &(pRegCfg->alg_reg_cfg[i].stSharpenRegCfg.stStaticRegCfg));
        SharpenDynaRegInit(&(pRegCfg->alg_reg_cfg[i].stSharpenRegCfg));
    }
    pRegCfg->cfg_key.bit1SharpenCfg = 1;
    return;
}

static HI_S32 SharpenReadExtregs(VI_PIPE ViPipe)
{
    HI_U8 i, j;
    ISP_SHARPEN_S *pstSharpen = HI_NULL;

    SHARPEN_GET_CTX(ViPipe, pstSharpen);
    ISP_CHECK_POINTER(pstSharpen);

    pstSharpen->bSharpenMpiUpdateEn = hi_ext_system_sharpen_mpi_update_en_read(ViPipe);
    pstSharpen->u8SkinUmax   = hi_ext_system_manual_Isp_sharpen_SkinUmax_read(ViPipe);
    pstSharpen->u8SkinUmin   = hi_ext_system_manual_Isp_sharpen_SkinUmin_read(ViPipe);
    pstSharpen->u8SkinVmax   = hi_ext_system_manual_Isp_sharpen_SkinVmax_read(ViPipe);
    pstSharpen->u8SkinVmin   = hi_ext_system_manual_Isp_sharpen_SkinVmin_read(ViPipe);

    hi_ext_system_sharpen_mpi_update_en_write(ViPipe, HI_FALSE);

    if (pstSharpen->bSharpenMpiUpdateEn) {
        pstSharpen->u8ManualSharpenYuvEnabled = hi_ext_system_isp_sharpen_manu_mode_read(ViPipe);

        if (pstSharpen->u8ManualSharpenYuvEnabled == OP_TYPE_MANUAL) {
            for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                pstSharpen->au16TextureStr[j] = hi_ext_system_manual_Isp_sharpen_TextureStr_read(ViPipe, j);
                pstSharpen->au16EdgeStr[j]    = hi_ext_system_manual_Isp_sharpen_EdgeStr_read(ViPipe, j);
            }
            for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                pstSharpen->au8LumaWgt[j]     = hi_ext_system_manual_Isp_sharpen_LumaWgt_read(ViPipe, j);
            }
            pstSharpen->u16TextureFreq    = hi_ext_system_manual_Isp_sharpen_TextureFreq_read(ViPipe);
            pstSharpen->u16EdgeFreq       = hi_ext_system_manual_Isp_sharpen_EdgeFreq_read(ViPipe);
            pstSharpen->u8OverShoot       = hi_ext_system_manual_Isp_sharpen_OverShoot_read(ViPipe);
            pstSharpen->u8UnderShoot      = hi_ext_system_manual_Isp_sharpen_UnderShoot_read(ViPipe);
            pstSharpen->u8ShootSupStr     = hi_ext_system_manual_Isp_sharpen_shootSupStr_read(ViPipe);
            pstSharpen->u8DetailCtrl      = hi_ext_system_manual_Isp_sharpen_detailctrl_read(ViPipe);
            pstSharpen->u8EdgeFiltStr     = hi_ext_system_manual_Isp_sharpen_EdgeFiltStr_read(ViPipe);
            pstSharpen->u8EdgeFiltMaxCap  = hi_ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_read(ViPipe);
            pstSharpen->u8RGain           = hi_ext_system_manual_Isp_sharpen_RGain_read(ViPipe);
            pstSharpen->u8GGain           = hi_ext_system_manual_Isp_sharpen_GGain_read(ViPipe);
            pstSharpen->u8BGain           = hi_ext_system_manual_Isp_sharpen_BGain_read(ViPipe);
            pstSharpen->u8SkinGain        = hi_ext_system_manual_Isp_sharpen_SkinGain_read(ViPipe);
            pstSharpen->u8ShootSupAdj     = hi_ext_system_manual_Isp_sharpen_ShootSupAdj_read(ViPipe);
            pstSharpen->u8DetailCtrlThr   = hi_ext_system_manual_Isp_sharpen_detailctrlThr_read(ViPipe);
            pstSharpen->u16MaxSharpGain   = hi_ext_system_manual_Isp_sharpen_MaxSharpGain_read(ViPipe);
        } else {
            for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
                for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                    pstSharpen->au16AutoTextureStr[j][i] = hi_ext_system_Isp_sharpen_TextureStr_read(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
                    pstSharpen->au16AutoEdgeStr[j][i]    = hi_ext_system_Isp_sharpen_EdgeStr_read(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
                }
                for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                    pstSharpen->au8AutoLumaWgt[j][i]     = hi_ext_system_Isp_sharpen_LumaWgt_read(ViPipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
                }
                pstSharpen->au16TextureFreq[i]  = hi_ext_system_Isp_sharpen_TextureFreq_read(ViPipe, i);
                pstSharpen->au16EdgeFreq[i]     = hi_ext_system_Isp_sharpen_EdgeFreq_read(ViPipe, i);
                pstSharpen->au8OverShoot[i]     = hi_ext_system_Isp_sharpen_OverShoot_read(ViPipe, i);
                pstSharpen->au8UnderShoot[i]    = hi_ext_system_Isp_sharpen_UnderShoot_read(ViPipe, i);
                pstSharpen->au8ShootSupStr[i]   = hi_ext_system_Isp_sharpen_shootSupStr_read(ViPipe, i);
                pstSharpen->au8DetailCtrl[i]    = hi_ext_system_Isp_sharpen_detailctrl_read(ViPipe, i);
                pstSharpen->au8EdgeFiltStr[i]   = hi_ext_system_Isp_sharpen_EdgeFiltStr_read(ViPipe, i);
                pstSharpen->au8EdgeFiltMaxCap[i] = hi_ext_system_Isp_sharpen_EdgeFiltMaxCap_read(ViPipe, i);
                pstSharpen->au8RGain[i]         = hi_ext_system_Isp_sharpen_RGain_read(ViPipe, i);
                pstSharpen->au8GGain[i]         = hi_ext_system_Isp_sharpen_GGain_read(ViPipe, i);
                pstSharpen->au8BGain[i]         = hi_ext_system_Isp_sharpen_BGain_read(ViPipe, i);
                pstSharpen->au8SkinGain[i]      = hi_ext_system_Isp_sharpen_SkinGain_read(ViPipe, i);
                pstSharpen->au8ShootSupAdj[i]   = hi_ext_system_Isp_sharpen_ShootSupAdj_read(ViPipe, i);
                pstSharpen->au16MaxSharpGain[i] = hi_ext_system_Isp_sharpen_MaxSharpGain_read(ViPipe, i);
                pstSharpen->au8DetailCtrlThr[i] = hi_ext_system_Isp_sharpen_detailctrlThr_read(ViPipe, i);
            }
        }
    }
    return 0;
}

static HI_S32 SharpenReadProMode(VI_PIPE ViPipe)
{
    HI_U8 i, j;
    isp_usr_ctx     *pstIspCtx       = HI_NULL;
    ISP_SHARPEN_S *pstSharpen      = HI_NULL;
    HI_U8          u8Index         = 0;
    HI_U8          u8IndexMaxValue = 0;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    SHARPEN_GET_CTX(ViPipe, pstSharpen);
    ISP_CHECK_POINTER(pstSharpen);

    if (pstIspCtx->pro_shp_param_ctrl.pro_shp_param->enable == HI_TRUE) {
        u8Index = pstIspCtx->linkage.pro_index;
        u8IndexMaxValue = MIN2(pstIspCtx->pro_shp_param_ctrl.pro_shp_param->param_num, PRO_MAX_FRAME_NUM);
        if (u8Index > u8IndexMaxValue) {
            u8Index =  u8IndexMaxValue;
        }
        if (u8Index < 1) {
            return HI_SUCCESS;
        }
        u8Index -= 1;
    } else {
        return HI_SUCCESS;
    }
    pstSharpen->u8ManualSharpenYuvEnabled = OP_TYPE_AUTO;
    pstSharpen->bSharpenMpiUpdateEn = HI_TRUE;
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            pstSharpen->au16AutoTextureStr[j][i] = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].texture_str[j][i];
            pstSharpen->au16AutoEdgeStr[j][i]    = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].edge_str[j][i];
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            pstSharpen->au8AutoLumaWgt[j][i]     = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].luma_wgt[j][i];
        }
        pstSharpen->au16TextureFreq[i]  = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].texture_freq[i];
        pstSharpen->au16EdgeFreq[i]     = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].edge_freq[i];
        pstSharpen->au8OverShoot[i]     = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].over_shoot[i];
        pstSharpen->au8UnderShoot[i]    = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].under_shoot[i];
        pstSharpen->au8ShootSupStr[i]   = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].shoot_sup_str[i];
        pstSharpen->au8DetailCtrl[i]    = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].detail_ctrl[i];
        pstSharpen->au8EdgeFiltStr[i]   = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].edge_filt_str[i];
        pstSharpen->au8EdgeFiltMaxCap[i] = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].edge_filt_max_cap[i];
        pstSharpen->au8RGain[i]         = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].r_gain[i];
        pstSharpen->au8GGain[i]         = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].g_gain[i];
        pstSharpen->au8BGain[i]         = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].b_gain[i];
        pstSharpen->au8SkinGain[i]      = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].skin_gain[i];
        pstSharpen->au8ShootSupAdj[i]   = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].shoot_sup_adj[i];
        pstSharpen->au16MaxSharpGain[i] = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].max_sharp_gain[i];
        pstSharpen->au8DetailCtrlThr[i] = pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[u8Index].detail_ctrl_thr[i];
    }
    return HI_SUCCESS;
}
static HI_S32 ISP_SharpenCtxInit(ISP_SHARPEN_S *pstSharpen)
{
    HI_U8 i, j;
    pstSharpen->bSharpenEn = 1;
    pstSharpen->bSharpenMpiUpdateEn = 1;
    pstSharpen->u32IsoLast = 0;
    /* Sharpening Yuv */
    // tmp registers
    pstSharpen->u8ManualSharpenYuvEnabled = 1;
    pstSharpen->u8mfThdSftD       = 0;
    pstSharpen->u8dirVarSft       = 12;
    pstSharpen->u8selPixWgt       = 31;
    pstSharpen->u8mfThdSelUD      = 2;
    pstSharpen->u8mfThdSftUD      = 0;
    pstSharpen->u8oshtVarWgt0     = 10;
    pstSharpen->u8ushtVarWgt0     = 20;
    pstSharpen->u8oshtVarDiffThd0 = 20;
    pstSharpen->u8oshtVarDiffThd1 = 35;
    pstSharpen->u8oshtVarDiffWgt1 = 20;
    pstSharpen->u8ushtVarDiffWgt1 = 35;
    // MPI
    pstSharpen->u8SkinUmax      = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINUMAX_DEFAULT;
    pstSharpen->u8SkinUmin      = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINUMIN_DEFAULT;
    pstSharpen->u8SkinVmin      = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINVMIN_DEFAULT;
    pstSharpen->u8SkinVmax      = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINVMAX_DEFAULT;
    for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
        pstSharpen->au16TextureStr[i] = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT;
        pstSharpen->au16EdgeStr[i]    = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT;
    }
    for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
        pstSharpen->au8LumaWgt[i]     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT;
    }
    pstSharpen->u16TextureFreq  = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT;
    pstSharpen->u16EdgeFreq     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT;
    pstSharpen->u8OverShoot     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT;
    pstSharpen->u8UnderShoot    = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT;
    pstSharpen->u8ShootSupStr   = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT;
    pstSharpen->u8DetailCtrl    = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT;
    pstSharpen->u8EdgeFiltStr   = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT;
    pstSharpen->u8EdgeFiltMaxCap = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTMAXCAP_DEFAULT;
    pstSharpen->u8RGain         = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT;
    pstSharpen->u8GGain         = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_GGAIN_DEFAULT;
    pstSharpen->u8BGain         = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT;
    pstSharpen->u8SkinGain      = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT;
    pstSharpen->u8ShootSupAdj     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPADJ_DEFAULT;
    pstSharpen->u16MaxSharpGain    = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_MAXSHARPGAIN_DEFAULT;
    pstSharpen->u8DetailCtrlThr    = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRLTHR_DEFAULT;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            pstSharpen->au16AutoTextureStr[j][i] = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT;
            pstSharpen->au16AutoEdgeStr[j][i]    = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT;
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            pstSharpen->au8AutoLumaWgt[j][i]     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT;
        }
        pstSharpen->au16TextureFreq[i] = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT;
        pstSharpen->au16EdgeFreq[i]    = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT;
        pstSharpen->au8OverShoot[i]    = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT;
        pstSharpen->au8UnderShoot[i]   = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT;
        pstSharpen->au8ShootSupStr[i]  = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT;
        pstSharpen->au8DetailCtrl[i]   = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT;
        pstSharpen->au8EdgeFiltStr[i]  = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT;
        pstSharpen->au8EdgeFiltMaxCap[i]  = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTMAXCAP_DEFAULT;
        pstSharpen->au8RGain[i]        = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT;
        pstSharpen->au8GGain[i]        = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_GGAIN_DEFAULT;
        pstSharpen->au8BGain[i]        = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT;
        pstSharpen->au8SkinGain[i]     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT;
        pstSharpen->au8ShootSupAdj[i]     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPADJ_DEFAULT;
        pstSharpen->au16MaxSharpGain[i]     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_MAXSHARPGAIN_DEFAULT;
        pstSharpen->au8DetailCtrlThr[i]     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRLTHR_DEFAULT;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_SharpenInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_SHARPEN_S *pstSharpen = HI_NULL;

    s32Ret = SharpenCtxInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    SHARPEN_GET_CTX(ViPipe, pstSharpen);
    ISP_CHECK_POINTER(pstSharpen);

    pstSharpen->u8ManualSharpenYuvEnabled = 1;
    pstSharpen->bSharpenEn                = HI_TRUE;
    pstSharpen->u32IsoLast                = 0;

    ISP_SharpenCtxInit(pstSharpen);
    SharpenRegsInitialize(ViPipe, (isp_reg_cfg *)pRegCfg);
    s32Ret = SharpenExtRegsInitialize(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_VOID ISP_SharpenWdrModeSet(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_U8  i;
    HI_U32 au32UpdateIdx[ISP_STRIPING_MAX_NUM] = {0};
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        au32UpdateIdx[i] = pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.u32UpdateIndex;
    }

    ISP_SharpenInit(ViPipe, pRegCfg);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.u32UpdateIndex = au32UpdateIdx[i] + 1;
    }
}

static HI_S32 ISP_Sharpen_GetLinearDefaultRegCfg(ISP_SHARPEN_S *pstSharpenPara, HI_U32 u32ISO, HI_U32  idxCur, HI_U32 idxPre, HI_U32  isoLvlCur, HI_U32 isoLvlPre)
{
    const HI_U8  u8mfThdSelUDLinear[ISP_AUTO_ISO_STRENGTH_NUM]      = {2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    const HI_U8  u8mfThdSftUDLinear[ISP_AUTO_ISO_STRENGTH_NUM]      = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
    const HI_U8  u8oshtVarWgt0Linear[ISP_AUTO_ISO_STRENGTH_NUM]     = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
    const HI_U8  u8ushtVarWgt0Linear[ISP_AUTO_ISO_STRENGTH_NUM]     = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30};
    const HI_U8  u8oshtVarDiffThd0Linear[ISP_AUTO_ISO_STRENGTH_NUM] = {20, 22, 23, 25, 28, 30, 32, 36, 37, 38, 39, 40, 40, 40, 40, 40};
    const HI_U8  u8oshtVarDiffThd1Linear[ISP_AUTO_ISO_STRENGTH_NUM] = {35, 37, 38, 40, 40, 43, 43, 46, 47, 48, 49, 50, 50, 50, 50, 50};
    const HI_U8  u8oshtVarDiffWgt1Linear[ISP_AUTO_ISO_STRENGTH_NUM] = {0,  0,  0,  0,  5,  10, 15, 18, 20, 20, 20, 20, 20, 20, 20, 20};
    const HI_U8  u8ushtVarDiffWgt1Linear[ISP_AUTO_ISO_STRENGTH_NUM] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 20, 10, 10, 10, 10, 10 };

    HI_U16 WgtPre = 0;
    HI_U16 WgtCur = 0;   // U0.8
    HI_U8  sft = 8;

    if (u32ISO <= isoLvlPre) {
        pstSharpenPara->u8mfThdSelUD      =  u8mfThdSelUDLinear[idxPre];
        pstSharpenPara->u8mfThdSftUD      =  u8mfThdSftUDLinear[idxPre];
        pstSharpenPara->u8oshtVarWgt0     =  u8oshtVarWgt0Linear[idxPre];
        pstSharpenPara->u8ushtVarWgt0     =  u8ushtVarWgt0Linear[idxPre];
        pstSharpenPara->u8oshtVarDiffThd0 =  u8oshtVarDiffThd0Linear[idxPre];
        pstSharpenPara->u8oshtVarDiffThd1 =  u8oshtVarDiffThd1Linear[idxPre];
        pstSharpenPara->u8oshtVarDiffWgt1 =  u8oshtVarDiffWgt1Linear[idxPre];
        pstSharpenPara->u8ushtVarDiffWgt1 =  u8ushtVarDiffWgt1Linear[idxPre];
    } else if (u32ISO >= isoLvlCur) {
        pstSharpenPara->u8mfThdSelUD      =  u8mfThdSelUDLinear[idxCur];
        pstSharpenPara->u8mfThdSftUD      =  u8mfThdSftUDLinear[idxCur];
        pstSharpenPara->u8oshtVarWgt0     =  u8oshtVarWgt0Linear[idxCur];
        pstSharpenPara->u8ushtVarWgt0     =  u8ushtVarWgt0Linear[idxCur];
        pstSharpenPara->u8oshtVarDiffThd0 =  u8oshtVarDiffThd0Linear[idxCur];
        pstSharpenPara->u8oshtVarDiffThd1 =  u8oshtVarDiffThd1Linear[idxCur];
        pstSharpenPara->u8oshtVarDiffWgt1 =  u8oshtVarDiffWgt1Linear[idxCur];
        pstSharpenPara->u8ushtVarDiffWgt1 =  u8ushtVarDiffWgt1Linear[idxCur];

    } else {
        WgtPre = SignedLeftShift((isoLvlCur - u32ISO), sft) / (isoLvlCur - isoLvlPre);
        WgtCur = SignedLeftShift(1, sft) - WgtPre;

        pstSharpenPara->u8mfThdSelUD      =  shrp_blend(sft, WgtPre, u8mfThdSelUDLinear[idxPre],      WgtCur, u8mfThdSelUDLinear[idxCur]);
        pstSharpenPara->u8mfThdSftUD      =  shrp_blend(sft, WgtPre, u8mfThdSftUDLinear[idxPre],      WgtCur, u8mfThdSftUDLinear[idxCur]);
        pstSharpenPara->u8oshtVarWgt0     =  shrp_blend(sft, WgtPre, u8oshtVarWgt0Linear[idxPre],     WgtCur, u8oshtVarWgt0Linear[idxCur]);
        pstSharpenPara->u8ushtVarWgt0     =  shrp_blend(sft, WgtPre, u8ushtVarWgt0Linear[idxPre],     WgtCur, u8ushtVarWgt0Linear[idxCur]);
        pstSharpenPara->u8oshtVarDiffThd0 =  shrp_blend(sft, WgtPre, u8oshtVarDiffThd0Linear[idxPre], WgtCur, u8oshtVarDiffThd0Linear[idxCur]);
        pstSharpenPara->u8oshtVarDiffThd1 =  shrp_blend(sft, WgtPre, u8oshtVarDiffThd1Linear[idxPre], WgtCur, u8oshtVarDiffThd1Linear[idxCur]);
        pstSharpenPara->u8oshtVarDiffWgt1 =  shrp_blend(sft, WgtPre, u8oshtVarDiffWgt1Linear[idxPre], WgtCur, u8oshtVarDiffWgt1Linear[idxCur]);
        pstSharpenPara->u8ushtVarDiffWgt1 =  shrp_blend(sft, WgtPre, u8ushtVarDiffWgt1Linear[idxPre], WgtCur, u8ushtVarDiffWgt1Linear[idxCur]);

    }

    return HI_SUCCESS;
}

static HI_S32 ISP_Sharpen_GetWdrDefaultRegCfg(ISP_SHARPEN_S *pstSharpenPara, HI_U32 u32ISO, HI_U32  idxCur, HI_U32 idxPre, HI_U32  isoLvlCur, HI_U32 isoLvlPre)
{
    const HI_U8  u8mfThdSelUDWdr[ISP_AUTO_ISO_STRENGTH_NUM]      = {2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    const HI_U8  u8mfThdSftUDWdr[ISP_AUTO_ISO_STRENGTH_NUM]      = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
    const HI_U8  u8oshtVarWgt0Wdr[ISP_AUTO_ISO_STRENGTH_NUM]     = {60, 60, 60, 60, 60, 50, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
    const HI_U8  u8ushtVarWgt0Wdr[ISP_AUTO_ISO_STRENGTH_NUM]     = {70, 70, 70, 70, 70, 60, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30};
    const HI_U8  u8oshtVarDiffThd0Wdr[ISP_AUTO_ISO_STRENGTH_NUM] = {20, 22, 23, 25, 28, 30, 32, 36, 37, 38, 39, 40, 40, 40, 40, 40};
    const HI_U8  u8oshtVarDiffThd1Wdr[ISP_AUTO_ISO_STRENGTH_NUM] = {35, 37, 38, 40, 40, 43, 43, 46, 47, 48, 49, 50, 50, 50, 50, 50};
    const HI_U8  u8oshtVarDiffWgt1Wdr[ISP_AUTO_ISO_STRENGTH_NUM] = {5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5 };
    const HI_U8  u8ushtVarDiffWgt1Wdr[ISP_AUTO_ISO_STRENGTH_NUM] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

    HI_U16 WgtPre = 0;   // linerinter
    HI_U16 WgtCur = 0;   // linerinter
    HI_U8  sft = 8;      // linerinter

    if (u32ISO <= isoLvlPre) {
        pstSharpenPara->u8mfThdSelUD   =  u8mfThdSelUDWdr[idxPre];
        pstSharpenPara->u8mfThdSftUD   =  u8mfThdSftUDWdr[idxPre];
        pstSharpenPara->u8oshtVarWgt0  =  u8oshtVarWgt0Wdr[idxPre];
        pstSharpenPara->u8ushtVarWgt0  =  u8ushtVarWgt0Wdr[idxPre];
        pstSharpenPara->u8oshtVarDiffThd0 =  u8oshtVarDiffThd0Wdr[idxPre];
        pstSharpenPara->u8oshtVarDiffThd1 =  u8oshtVarDiffThd1Wdr[idxPre];
        pstSharpenPara->u8oshtVarDiffWgt1 =  u8oshtVarDiffWgt1Wdr[idxPre];
        pstSharpenPara->u8ushtVarDiffWgt1 =  u8ushtVarDiffWgt1Wdr[idxPre];
    } else if (u32ISO >= isoLvlCur) {
        pstSharpenPara->u8mfThdSelUD   =  u8mfThdSelUDWdr[idxCur];
        pstSharpenPara->u8mfThdSftUD   =  u8mfThdSftUDWdr[idxCur];
        pstSharpenPara->u8oshtVarWgt0  =  u8oshtVarWgt0Wdr[idxCur];
        pstSharpenPara->u8ushtVarWgt0  =  u8ushtVarWgt0Wdr[idxCur];
        pstSharpenPara->u8oshtVarDiffThd0 =  u8oshtVarDiffThd0Wdr[idxCur];
        pstSharpenPara->u8oshtVarDiffThd1 =  u8oshtVarDiffThd1Wdr[idxCur];
        pstSharpenPara->u8oshtVarDiffWgt1 =  u8oshtVarDiffWgt1Wdr[idxCur];
        pstSharpenPara->u8ushtVarDiffWgt1 =  u8ushtVarDiffWgt1Wdr[idxCur];

    } else {
        WgtPre = SignedLeftShift((isoLvlCur - u32ISO), sft) / (isoLvlCur - isoLvlPre);
        WgtCur = SignedLeftShift(1, sft) - WgtPre;

        pstSharpenPara->u8mfThdSelUD      =  shrp_blend(sft, WgtPre, u8mfThdSelUDWdr[idxPre],      WgtCur, u8mfThdSelUDWdr[idxCur]);
        pstSharpenPara->u8mfThdSftUD      =  shrp_blend(sft, WgtPre, u8mfThdSftUDWdr[idxPre],      WgtCur, u8mfThdSftUDWdr[idxCur]);
        pstSharpenPara->u8oshtVarWgt0     =  shrp_blend(sft, WgtPre, u8oshtVarWgt0Wdr[idxPre],     WgtCur, u8oshtVarWgt0Wdr[idxCur]);
        pstSharpenPara->u8ushtVarWgt0     =  shrp_blend(sft, WgtPre, u8ushtVarWgt0Wdr[idxPre],     WgtCur, u8ushtVarWgt0Wdr[idxCur]);
        pstSharpenPara->u8oshtVarDiffThd0 =  shrp_blend(sft, WgtPre, u8oshtVarDiffThd0Wdr[idxPre], WgtCur, u8oshtVarDiffThd0Wdr[idxCur]);
        pstSharpenPara->u8oshtVarDiffThd1 =  shrp_blend(sft, WgtPre, u8oshtVarDiffThd1Wdr[idxPre], WgtCur, u8oshtVarDiffThd1Wdr[idxCur]);
        pstSharpenPara->u8oshtVarDiffWgt1 =  shrp_blend(sft, WgtPre, u8oshtVarDiffWgt1Wdr[idxPre], WgtCur, u8oshtVarDiffWgt1Wdr[idxCur]);
        pstSharpenPara->u8ushtVarDiffWgt1 =  shrp_blend(sft, WgtPre, u8ushtVarDiffWgt1Wdr[idxPre], WgtCur, u8ushtVarDiffWgt1Wdr[idxCur]);
    }
    return HI_SUCCESS;
}

static HI_S32 ISP_Sharpen_GetHdrDefaultRegCfg(ISP_SHARPEN_S *pstSharpenPara, HI_U32 u32ISO, HI_U32  idxCur, HI_U32 idxPre, HI_U32  isoLvlCur, HI_U32 isoLvlPre)
{
    const HI_U8  u8mfThdSelUDHdr[ISP_AUTO_ISO_STRENGTH_NUM]      = {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    const HI_U8  u8mfThdSftUDHdr[ISP_AUTO_ISO_STRENGTH_NUM]      = {2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2};
    const HI_U8  u8oshtVarWgt0Hdr[ISP_AUTO_ISO_STRENGTH_NUM]     = {80, 70, 60, 50, 40, 30, 20, 10, 0,  0,  0,  0,  0,  0,  0,  0};
    const HI_U8  u8ushtVarWgt0Hdr[ISP_AUTO_ISO_STRENGTH_NUM]     = {80, 70, 60, 50, 40, 30, 20, 10, 0,  0,  0,  0,  0,  0,  0,  0};
    const HI_U8  u8oshtVarDiffThd0Hdr[ISP_AUTO_ISO_STRENGTH_NUM] = {20, 22, 23, 25, 28, 30, 32, 36, 37, 38, 39, 40, 40, 40, 40, 40};
    const HI_U8  u8oshtVarDiffThd1Hdr[ISP_AUTO_ISO_STRENGTH_NUM] = {35, 37, 38, 40, 40, 43, 43, 46, 47, 48, 49, 50, 50, 50, 50, 50};
    const HI_U8  u8oshtVarDiffWgt1Hdr[ISP_AUTO_ISO_STRENGTH_NUM] = {0,  0,  0,  0,  0,  0,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5};
    const HI_U8  u8ushtVarDiffWgt1Hdr[ISP_AUTO_ISO_STRENGTH_NUM] = {0,  0,  0,  0,  0,  0,  5,  10, 10, 10, 10, 10, 10, 10, 10, 10 };

    HI_U16 WgtPre = 0;   // linerinter
    HI_U16 WgtCur = 0;   // linerinter
    HI_U8  sft = 8;      // linerinter

    if (u32ISO <= isoLvlPre) {
        pstSharpenPara->u8mfThdSelUD   =  u8mfThdSelUDHdr[idxPre];
        pstSharpenPara->u8mfThdSftUD   =  u8mfThdSftUDHdr[idxPre];
        pstSharpenPara->u8oshtVarWgt0  =  u8oshtVarWgt0Hdr[idxPre];
        pstSharpenPara->u8ushtVarWgt0  =  u8ushtVarWgt0Hdr[idxPre];
        pstSharpenPara->u8oshtVarDiffThd0 =  u8oshtVarDiffThd0Hdr[idxPre];
        pstSharpenPara->u8oshtVarDiffThd1 =  u8oshtVarDiffThd1Hdr[idxPre];
        pstSharpenPara->u8oshtVarDiffWgt1 =  u8oshtVarDiffWgt1Hdr[idxPre];
        pstSharpenPara->u8ushtVarDiffWgt1 =  u8ushtVarDiffWgt1Hdr[idxPre];
    } else if (u32ISO >= isoLvlCur) {
        pstSharpenPara->u8mfThdSelUD   =  u8mfThdSelUDHdr[idxCur];
        pstSharpenPara->u8mfThdSftUD   =  u8mfThdSftUDHdr[idxCur];
        pstSharpenPara->u8oshtVarWgt0  =  u8oshtVarWgt0Hdr[idxCur];
        pstSharpenPara->u8ushtVarWgt0  =  u8ushtVarWgt0Hdr[idxCur];
        pstSharpenPara->u8oshtVarDiffThd0 =  u8oshtVarDiffThd0Hdr[idxCur];
        pstSharpenPara->u8oshtVarDiffThd1 =  u8oshtVarDiffThd1Hdr[idxCur];
        pstSharpenPara->u8oshtVarDiffWgt1 =  u8oshtVarDiffWgt1Hdr[idxCur];
        pstSharpenPara->u8ushtVarDiffWgt1 =  u8ushtVarDiffWgt1Hdr[idxCur];

    } else {
        WgtPre = SignedLeftShift((isoLvlCur - u32ISO), sft) / (isoLvlCur - isoLvlPre);
        WgtCur = SignedLeftShift(1, sft) - WgtPre;

        pstSharpenPara->u8mfThdSelUD      =  shrp_blend(sft, WgtPre, u8mfThdSelUDHdr[idxPre],      WgtCur, u8mfThdSelUDHdr[idxCur]);
        pstSharpenPara->u8mfThdSftUD      =  shrp_blend(sft, WgtPre, u8mfThdSftUDHdr[idxPre],      WgtCur, u8mfThdSftUDHdr[idxCur]);
        pstSharpenPara->u8oshtVarWgt0     =  shrp_blend(sft, WgtPre, u8oshtVarWgt0Hdr[idxPre],     WgtCur, u8oshtVarWgt0Hdr[idxCur]);
        pstSharpenPara->u8ushtVarWgt0     =  shrp_blend(sft, WgtPre, u8ushtVarWgt0Hdr[idxPre],     WgtCur, u8ushtVarWgt0Hdr[idxCur]);
        pstSharpenPara->u8oshtVarDiffThd0 =  shrp_blend(sft, WgtPre, u8oshtVarDiffThd0Hdr[idxPre], WgtCur, u8oshtVarDiffThd0Hdr[idxCur]);
        pstSharpenPara->u8oshtVarDiffThd1 =  shrp_blend(sft, WgtPre, u8oshtVarDiffThd1Hdr[idxPre], WgtCur, u8oshtVarDiffThd1Hdr[idxCur]);
        pstSharpenPara->u8oshtVarDiffWgt1 =  shrp_blend(sft, WgtPre, u8oshtVarDiffWgt1Hdr[idxPre], WgtCur, u8oshtVarDiffWgt1Hdr[idxCur]);
        pstSharpenPara->u8ushtVarDiffWgt1 =  shrp_blend(sft, WgtPre, u8ushtVarDiffWgt1Hdr[idxPre], WgtCur, u8ushtVarDiffWgt1Hdr[idxCur]);
    }

    return HI_SUCCESS;
}


static HI_S32 ISP_Sharpen_GetDefaultRegCfg(VI_PIPE ViPipe, HI_U32 u32ISO)
{
    const HI_U32 SharpenLutIso[ISP_AUTO_ISO_STRENGTH_NUM] = {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800};
    // Common Regs
    const HI_U8  u8mfThdSftD[ISP_AUTO_ISO_STRENGTH_NUM]   = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
    const HI_U8  u8dirVarSft[ISP_AUTO_ISO_STRENGTH_NUM]   = {12, 12, 12, 12, 12, 12, 12, 10, 9,  8,  7,  6,  5,  5,  5,  5};
    const HI_U8  u8selPixWgt[ISP_AUTO_ISO_STRENGTH_NUM]   = {31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31};

    HI_U32 i;
    HI_U32 idxCur, idxPre;
    HI_U32 isoLvlCur, isoLvlPre;
    HI_U8  u8WDRMode;

    HI_U16 WgtPre = 0;   // linerinter
    HI_U16 WgtCur = 0;   // linerinter
    HI_U8  sft = 8;      // linerinter

    isp_usr_ctx     *pstIspCtx      = HI_NULL;
    ISP_SHARPEN_S *pstSharpenPara = HI_NULL;

    SHARPEN_GET_CTX(ViPipe, pstSharpenPara);
    ISP_CHECK_POINTER(pstSharpenPara);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8WDRMode = pstIspCtx->sns_wdr_mode;

    // Get ISO category index
    // idxCur : current index
    // idxPre : previous level index
    idxCur = ISP_AUTO_ISO_STRENGTH_NUM - 1;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (u32ISO <= SharpenLutIso[i]) {
            idxCur = i;
            break;
        }
    }

    idxPre = (idxCur == 0) ? 0 : MAX2(idxCur - 1, 0);

    isoLvlCur   =  SharpenLutIso[idxCur];
    isoLvlPre   =  SharpenLutIso[idxPre];

    /* Common default regs */
    if (u32ISO <= isoLvlPre) {
        pstSharpenPara->u8mfThdSftD    =  u8mfThdSftD[idxPre];
        pstSharpenPara->u8dirVarSft    =  u8dirVarSft[idxPre];
        pstSharpenPara->u8selPixWgt    =  u8selPixWgt[idxPre];
    } else if (u32ISO >= isoLvlCur) {
        pstSharpenPara->u8mfThdSftD    =  u8mfThdSftD[idxCur];
        pstSharpenPara->u8dirVarSft    =  u8dirVarSft[idxCur];
        pstSharpenPara->u8selPixWgt    =  u8selPixWgt[idxCur];
    } else {
        WgtPre = SignedLeftShift((isoLvlCur - u32ISO), sft) / (isoLvlCur - isoLvlPre);
        WgtCur = SignedLeftShift(1, sft) - WgtPre;

        pstSharpenPara->u8mfThdSftD    =  shrp_blend(sft, WgtPre, u8mfThdSftD[idxPre],    WgtCur, u8mfThdSftD[idxCur]);
        pstSharpenPara->u8dirVarSft    =  shrp_blend(sft, WgtPre, u8dirVarSft[idxPre],    WgtCur, u8dirVarSft[idxCur]);
        pstSharpenPara->u8selPixWgt    =  shrp_blend(sft, WgtPre, u8selPixWgt[idxPre],    WgtCur, u8selPixWgt[idxCur]);
    }
    /* Linear mode default regs */
    if (IS_LINEAR_MODE(u8WDRMode) || (pstIspCtx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (pstIspCtx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        ISP_Sharpen_GetLinearDefaultRegCfg(pstSharpenPara, u32ISO, idxCur, idxPre, isoLvlCur, isoLvlPre);
    } else if (IS_2to1_WDR_MODE(u8WDRMode) || IS_3to1_WDR_MODE(u8WDRMode) || IS_4to1_WDR_MODE(u8WDRMode) || IS_BUILT_IN_WDR_MODE(u8WDRMode)) { /* WDR mode default regs */
        ISP_Sharpen_GetWdrDefaultRegCfg(pstSharpenPara, u32ISO, idxCur, idxPre, isoLvlCur, isoLvlPre);
    } else {
        ISP_Sharpen_GetHdrDefaultRegCfg(pstSharpenPara, u32ISO, idxCur, idxPre, isoLvlCur, isoLvlPre);
    }

    return HI_SUCCESS;

}

static HI_S32 ISP_Sharpen_GetMpiRegCfg(VI_PIPE ViPipe, HI_U32 u32ISO)
{
    const HI_S32 SharpenLutIso[ISP_AUTO_ISO_STRENGTH_NUM] = {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800};

    HI_U32  i;
    HI_S32  idxCur, idxPre;
    HI_S32  isoLvlCur, isoLvlPre;

    ISP_SHARPEN_S *pstSharpenPara = HI_NULL;

    HI_U16 WgtPre = 0;   // linerinter
    HI_U16 WgtCur = 0;   // linerinter
    HI_U8  sft = 8;      // linerinter prec

    SHARPEN_GET_CTX(ViPipe, pstSharpenPara);
    ISP_CHECK_POINTER(pstSharpenPara);

    // Get ISO category index
    // idxCur : current index
    // idxPre : previous level index
    idxCur = ISP_AUTO_ISO_STRENGTH_NUM - 1;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (u32ISO <= SharpenLutIso[i]) {
            idxCur = i;
            break;
        }
    }
    idxPre    = MAX2(idxCur - 1, 0);
    isoLvlCur =  SharpenLutIso[idxCur];
    isoLvlPre =  SharpenLutIso[idxPre];

    // linerinter begain
    if (u32ISO <= isoLvlPre) {
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            pstSharpenPara->au16TextureStr[i] =  pstSharpenPara->au16AutoTextureStr[i][idxPre];
            pstSharpenPara->au16EdgeStr[i]    =  pstSharpenPara->au16AutoEdgeStr[i][idxPre];
        }
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            pstSharpenPara->au8LumaWgt[i]     =  pstSharpenPara->au8AutoLumaWgt[i][idxPre];
        }
        pstSharpenPara->u16TextureFreq        =  pstSharpenPara->au16TextureFreq[idxPre];
        pstSharpenPara->u16EdgeFreq           =  pstSharpenPara->au16EdgeFreq[idxPre];
        pstSharpenPara->u8OverShoot           =  pstSharpenPara->au8OverShoot[idxPre];
        pstSharpenPara->u8UnderShoot          =  pstSharpenPara->au8UnderShoot[idxPre];
        pstSharpenPara->u8ShootSupStr         =  pstSharpenPara->au8ShootSupStr[idxPre];
        pstSharpenPara->u8DetailCtrl          =  pstSharpenPara->au8DetailCtrl[idxPre];
        pstSharpenPara->u8EdgeFiltStr         =  pstSharpenPara->au8EdgeFiltStr[idxPre];
        pstSharpenPara->u8EdgeFiltMaxCap      =  pstSharpenPara->au8EdgeFiltMaxCap[idxPre];
        pstSharpenPara->u8RGain               =  pstSharpenPara->au8RGain[idxPre];
        pstSharpenPara->u8GGain               =  pstSharpenPara->au8GGain[idxPre];
        pstSharpenPara->u8BGain               =  pstSharpenPara->au8BGain[idxPre];
        pstSharpenPara->u8SkinGain            =  pstSharpenPara->au8SkinGain[idxPre];
        pstSharpenPara->u8ShootSupAdj         =  pstSharpenPara->au8ShootSupAdj[idxPre];
        pstSharpenPara->u8DetailCtrlThr       =  pstSharpenPara->au8DetailCtrlThr[idxPre];
        pstSharpenPara->u16MaxSharpGain       =  pstSharpenPara->au16MaxSharpGain[idxPre];

    } else if (u32ISO >= isoLvlCur) {
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            pstSharpenPara->au16TextureStr[i] =  pstSharpenPara->au16AutoTextureStr[i][idxCur];
            pstSharpenPara->au16EdgeStr[i]    =  pstSharpenPara->au16AutoEdgeStr[i][idxCur];
        }
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            pstSharpenPara->au8LumaWgt[i]     =  pstSharpenPara->au8AutoLumaWgt[i][idxCur];
        }
        pstSharpenPara->u16TextureFreq        =  pstSharpenPara->au16TextureFreq[idxCur];
        pstSharpenPara->u16EdgeFreq           =  pstSharpenPara->au16EdgeFreq[idxCur];
        pstSharpenPara->u8OverShoot           =  pstSharpenPara->au8OverShoot[idxCur];
        pstSharpenPara->u8UnderShoot          =  pstSharpenPara->au8UnderShoot[idxCur];
        pstSharpenPara->u8ShootSupStr         =  pstSharpenPara->au8ShootSupStr[idxCur];
        pstSharpenPara->u8DetailCtrl          =  pstSharpenPara->au8DetailCtrl[idxCur];
        pstSharpenPara->u8EdgeFiltStr         =  pstSharpenPara->au8EdgeFiltStr[idxCur];
        pstSharpenPara->u8EdgeFiltMaxCap      =  pstSharpenPara->au8EdgeFiltMaxCap[idxCur];
        pstSharpenPara->u8RGain               =  pstSharpenPara->au8RGain[idxCur];
        pstSharpenPara->u8GGain               =  pstSharpenPara->au8GGain[idxCur];
        pstSharpenPara->u8BGain               =  pstSharpenPara->au8BGain[idxCur];
        pstSharpenPara->u8SkinGain            =  pstSharpenPara->au8SkinGain[idxCur];
        pstSharpenPara->u8ShootSupAdj         =  pstSharpenPara->au8ShootSupAdj[idxCur];
        pstSharpenPara->u8DetailCtrlThr       =  pstSharpenPara->au8DetailCtrlThr[idxCur];
        pstSharpenPara->u16MaxSharpGain       =  pstSharpenPara->au16MaxSharpGain[idxCur];

    } else {
        WgtPre = SignedLeftShift((isoLvlCur - u32ISO), sft) / (isoLvlCur - isoLvlPre);
        WgtCur = SignedLeftShift(1, sft) - WgtPre;

        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            pstSharpenPara->au16TextureStr[i] =  shrp_blend(sft, WgtPre, pstSharpenPara->au16AutoTextureStr[i][idxPre], WgtCur, pstSharpenPara->au16AutoTextureStr[i][idxCur]);
            pstSharpenPara->au16EdgeStr[i]    =  shrp_blend(sft, WgtPre, pstSharpenPara->au16AutoEdgeStr[i][idxPre], WgtCur, pstSharpenPara->au16AutoEdgeStr[i][idxCur]);
        }
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            pstSharpenPara->au8LumaWgt[i]     =  shrp_blend(sft, WgtPre, pstSharpenPara->au8AutoLumaWgt[i][idxPre], WgtCur, pstSharpenPara->au8AutoLumaWgt[i][idxCur]);
        }
        pstSharpenPara->u16TextureFreq        =  shrp_blend(sft, WgtPre, pstSharpenPara->au16TextureFreq[idxPre], WgtCur, pstSharpenPara->au16TextureFreq[idxCur]);
        pstSharpenPara->u16EdgeFreq           =  shrp_blend(sft, WgtPre, pstSharpenPara->au16EdgeFreq[idxPre], WgtCur, pstSharpenPara->au16EdgeFreq[idxCur]);
        pstSharpenPara->u8OverShoot           =  shrp_blend(sft, WgtPre, pstSharpenPara->au8OverShoot[idxPre], WgtCur, pstSharpenPara->au8OverShoot[idxCur]);
        pstSharpenPara->u8UnderShoot          =  shrp_blend(sft, WgtPre, pstSharpenPara->au8UnderShoot[idxPre], WgtCur, pstSharpenPara->au8UnderShoot[idxCur]);
        pstSharpenPara->u8ShootSupStr         =  shrp_blend(sft, WgtPre, pstSharpenPara->au8ShootSupStr[idxPre], WgtCur, pstSharpenPara->au8ShootSupStr[idxCur]);
        pstSharpenPara->u8DetailCtrl          =  shrp_blend(sft, WgtPre, pstSharpenPara->au8DetailCtrl[idxPre], WgtCur, pstSharpenPara->au8DetailCtrl[idxCur]);
        pstSharpenPara->u8EdgeFiltStr         =  shrp_blend(sft, WgtPre, pstSharpenPara->au8EdgeFiltStr[idxPre], WgtCur, pstSharpenPara->au8EdgeFiltStr[idxCur]);
        pstSharpenPara->u8EdgeFiltMaxCap      =  shrp_blend(sft, WgtPre, pstSharpenPara->au8EdgeFiltMaxCap[idxPre], WgtCur, pstSharpenPara->au8EdgeFiltMaxCap[idxCur]);
        pstSharpenPara->u8RGain               =  shrp_blend(sft, WgtPre, pstSharpenPara->au8RGain[idxPre], WgtCur, pstSharpenPara->au8RGain[idxCur]);
        pstSharpenPara->u8GGain               =  shrp_blend(sft, WgtPre, pstSharpenPara->au8GGain[idxPre], WgtCur, pstSharpenPara->au8GGain[idxCur]);
        pstSharpenPara->u8BGain               =  shrp_blend(sft, WgtPre, pstSharpenPara->au8BGain[idxPre], WgtCur, pstSharpenPara->au8BGain[idxCur]);
        pstSharpenPara->u8SkinGain            =  shrp_blend(sft, WgtPre, pstSharpenPara->au8SkinGain[idxPre], WgtCur, pstSharpenPara->au8SkinGain[idxCur]);
        pstSharpenPara->u8ShootSupAdj         =  shrp_blend(sft, WgtPre, pstSharpenPara->au8ShootSupAdj[idxPre], WgtCur, pstSharpenPara->au8ShootSupAdj[idxCur]);
        pstSharpenPara->u8DetailCtrlThr       =  shrp_blend(sft, WgtPre, pstSharpenPara->au8DetailCtrlThr[idxPre], WgtCur, pstSharpenPara->au8DetailCtrlThr[idxCur]);
        pstSharpenPara->u16MaxSharpGain       =  shrp_blend(sft, WgtPre, pstSharpenPara->au16MaxSharpGain[idxPre], WgtCur, pstSharpenPara->au16MaxSharpGain[idxCur]);
    }

    // linerinter end

    {
        hi_ext_system_actual_sharpen_overshootAmt_write(ViPipe, pstSharpenPara->u8OverShoot);
        hi_ext_system_actual_sharpen_undershootAmt_write(ViPipe, pstSharpenPara->u8UnderShoot);
        hi_ext_system_actual_sharpen_shootSupSt_write(ViPipe, pstSharpenPara->u8ShootSupStr);
        hi_ext_system_actual_sharpen_edge_frequence_write(ViPipe, pstSharpenPara->u16EdgeFreq);
        hi_ext_system_actual_sharpen_texture_frequence_write(ViPipe, pstSharpenPara->u16TextureFreq);

        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            hi_ext_system_actual_sharpen_edge_str_write(ViPipe, i, pstSharpenPara->au16EdgeStr[i]);
            hi_ext_system_actual_sharpen_texture_str_write(ViPipe, i, pstSharpenPara->au16TextureStr[i]);
        }
    }

    return HI_SUCCESS;

}

static HI_S32 SharpenActualUpdate(VI_PIPE ViPipe)
{
    HI_U32 i;
    ISP_SHARPEN_S *pstSharpenPara = HI_NULL;
    SHARPEN_GET_CTX(ViPipe, pstSharpenPara);
    ISP_CHECK_POINTER(pstSharpenPara);

    {
        hi_ext_system_actual_sharpen_overshootAmt_write(ViPipe, pstSharpenPara->u8OverShoot);
        hi_ext_system_actual_sharpen_undershootAmt_write(ViPipe, pstSharpenPara->u8UnderShoot);
        hi_ext_system_actual_sharpen_shootSupSt_write(ViPipe, pstSharpenPara->u8ShootSupStr);
        hi_ext_system_actual_sharpen_edge_frequence_write(ViPipe, pstSharpenPara->u16EdgeFreq);
        hi_ext_system_actual_sharpen_texture_frequence_write(ViPipe, pstSharpenPara->u16TextureFreq);

        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            hi_ext_system_actual_sharpen_edge_str_write(ViPipe, i, pstSharpenPara->au16EdgeStr[i]);
            hi_ext_system_actual_sharpen_texture_str_write(ViPipe, i, pstSharpenPara->au16TextureStr[i]);
        }
    }

    return HI_SUCCESS;

}

static void SharpenMPI2Reg(ISP_SHARPEN_REG_CFG_S *pstSharpenRegCfg, ISP_SHARPEN_S *pstSharpen)
{
    HI_U8 i, j;

    ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S *pstSharpenDefaultDynaRegCfg = HI_NULL;
    ISP_SHARPEN_MPI_DYNA_REG_CFG_S     *pstSharpenMpiDynaRegCfg     = HI_NULL;
    ISP_SHARPEN_STATIC_REG_CFG_S       *pstSharpenStaticRegCfg      = HI_NULL;
    ISP_SHARPEN_DYNA_REG_CFG_S         *pstSharpenDynaRegCfg        = HI_NULL;

    pstSharpenDynaRegCfg = &(pstSharpenRegCfg->stDynaRegCfg);
    pstSharpenDefaultDynaRegCfg = &(pstSharpenDynaRegCfg->stDefaultDynaRegCfg);
    pstSharpenMpiDynaRegCfg = &(pstSharpenDynaRegCfg->stMpiDynaRegCfg);
    pstSharpenStaticRegCfg = &(pstSharpenRegCfg->stStaticRegCfg);

    if (pstSharpenDefaultDynaRegCfg->bResh) {
        pstSharpenDefaultDynaRegCfg->u8mfThdSftD            =   pstSharpen->u8mfThdSftD;
        pstSharpenDefaultDynaRegCfg->u8mfThdSelUD           =   pstSharpen->u8mfThdSelUD;
        pstSharpenDefaultDynaRegCfg->u8mfThdSftUD           =   pstSharpen->u8mfThdSftUD;
        pstSharpenDefaultDynaRegCfg->u8hfThdSftD            =   pstSharpenDefaultDynaRegCfg->u8mfThdSftD;
        pstSharpenDefaultDynaRegCfg->u8hfThdSelUD           =   pstSharpenDefaultDynaRegCfg->u8mfThdSelUD;
        pstSharpenDefaultDynaRegCfg->u8hfThdSftUD           =   pstSharpenDefaultDynaRegCfg->u8mfThdSftUD;
        pstSharpenDefaultDynaRegCfg->u8dirVarSft            =   pstSharpen->u8dirVarSft;
        pstSharpenDefaultDynaRegCfg->u8selPixWgt            =   pstSharpen->u8selPixWgt;
        pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[0]    =   pstSharpen->u8oshtVarDiffThd0;
        pstSharpenDefaultDynaRegCfg->u8ushtVarDiffThd[0]    =   pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[0];
        pstSharpenDefaultDynaRegCfg->u8oshtVarWgt0          =   pstSharpen->u8oshtVarWgt0;
        pstSharpenDefaultDynaRegCfg->u8ushtVarWgt0          =   pstSharpen->u8ushtVarWgt0;
        pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[1]    =   pstSharpen->u8oshtVarDiffThd1;
        pstSharpenDefaultDynaRegCfg->u8oshtVarDiffWgt1      =   pstSharpen->u8oshtVarDiffWgt1;
        pstSharpenDefaultDynaRegCfg->u8ushtVarDiffThd[1]    =   pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[1];
        pstSharpenDefaultDynaRegCfg->u8ushtVarDiffWgt1      =   pstSharpen->u8ushtVarDiffWgt1;
    }

    if (pstSharpenMpiDynaRegCfg->bResh) {
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            j = i << 1;
            if (i < ISP_SHARPEN_GAIN_NUM - 1) {
                pstSharpenMpiDynaRegCfg->u16mfGainD[j]      = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16EdgeStr[i]));
                pstSharpenMpiDynaRegCfg->u16mfGainUD[j]     = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
                pstSharpenMpiDynaRegCfg->u16mfGainD[j + 1]  = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16EdgeStr[i] + pstSharpen->au16EdgeStr[i + 1]) >> 1)));
                pstSharpenMpiDynaRegCfg->u16mfGainUD[j + 1] = SHARPEN_CLIP3(0, 0xFFF, (32 + ((pstSharpen->au16TextureStr[i] + pstSharpen->au16TextureStr[i + 1]) >> 1)));
            } else { // 31
                pstSharpenMpiDynaRegCfg->u16mfGainD[j]      = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16EdgeStr[i]));
                pstSharpenMpiDynaRegCfg->u16mfGainUD[j]     = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
                pstSharpenMpiDynaRegCfg->u16mfGainD[j + 1]  = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16EdgeStr[i]));
                pstSharpenMpiDynaRegCfg->u16mfGainUD[j + 1] = SHARPEN_CLIP3(0, 0xFFF, (32 + pstSharpen->au16TextureStr[i]));
            }
            pstSharpenMpiDynaRegCfg->u16hfGainD[j]      = (HI_U16)SHARPEN_CLIP3(0, 0xFFF, ((((HI_U32)(pstSharpenMpiDynaRegCfg->u16mfGainD[j])) * (pstSharpen->u16EdgeFreq)) >> 6));
            pstSharpenMpiDynaRegCfg->u16hfGainUD[j]     = (HI_U16)SHARPEN_CLIP3(0, 0xFFF, ((((HI_U32)(pstSharpenMpiDynaRegCfg->u16mfGainUD[j])) * (pstSharpen->u16TextureFreq)) >> 6));
            pstSharpenMpiDynaRegCfg->u16hfGainD[j + 1]  = (HI_U16)SHARPEN_CLIP3(0, 0xFFF, ((((HI_U32)(pstSharpenMpiDynaRegCfg->u16mfGainD[j + 1])) * (pstSharpen->u16EdgeFreq)) >> 6));
            pstSharpenMpiDynaRegCfg->u16hfGainUD[j + 1] = (HI_U16)SHARPEN_CLIP3(0, 0xFFF, ((((HI_U32)(pstSharpenMpiDynaRegCfg->u16mfGainUD[j + 1])) * (pstSharpen->u16TextureFreq)) >> 6));
        }
        pstSharpenMpiDynaRegCfg->u8oshtAmt              = pstSharpen->u8OverShoot;
        pstSharpenMpiDynaRegCfg->u8ushtAmt              = pstSharpen->u8UnderShoot;

        // skin Ctrl
        if (pstSharpen->u8SkinGain == 31) {
            pstSharpenMpiDynaRegCfg->u8bEnSkinCtrl      =    0;
        } else {
            pstSharpenMpiDynaRegCfg->u8bEnSkinCtrl      =    1;
            pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[1]   = SHARPEN_CLIP3(0, 0x1F, (31 - pstSharpen->u8SkinGain));
            pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[0]   = SHARPEN_CLIP3(0, 0x1F, (pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[1] << 1));

        }

        // Chr Ctrl
        if ((pstSharpen->u8RGain == 31) && (pstSharpen->u8GGain == 32) && (pstSharpen->u8BGain == 31)) {
            pstSharpenMpiDynaRegCfg->u8bEnChrCtrl    = 0;
            pstSharpenMpiDynaRegCfg->u8chrRGain0     = pstSharpen->u8RGain;
            pstSharpenMpiDynaRegCfg->u16chrGGain0    = pstSharpen->u8GGain;
            pstSharpenMpiDynaRegCfg->u8chrBGain0     = pstSharpen->u8BGain;
        } else {
            pstSharpenMpiDynaRegCfg->u8bEnChrCtrl    = 1;
            pstSharpenMpiDynaRegCfg->u8chrRGain0     = pstSharpen->u8RGain;
            pstSharpenMpiDynaRegCfg->u16chrGGain0    = pstSharpen->u8GGain;
            pstSharpenMpiDynaRegCfg->u8chrBGain0     = pstSharpen->u8BGain;
        }

        if (pstSharpen->u8DetailCtrl == 128) {
            pstSharpenMpiDynaRegCfg->bEnDetailCtrl = 0;
        } else {
            pstSharpenMpiDynaRegCfg->bEnDetailCtrl = 1;
        }
        pstSharpenMpiDynaRegCfg->u8detailOshtAmt   = SHARPEN_CLIP3(0, 127, (pstSharpenMpiDynaRegCfg->u8oshtAmt) + (pstSharpen->u8DetailCtrl) - 128);
        pstSharpenMpiDynaRegCfg->u8detailUshtAmt   = SHARPEN_CLIP3(0, 127, (pstSharpenMpiDynaRegCfg->u8ushtAmt) + (pstSharpen->u8DetailCtrl) - 128);

        pstSharpenMpiDynaRegCfg->u8dirDiffSft      = 63 - pstSharpen->u8EdgeFiltStr;

        if (pstSharpen->u8EdgeFiltMaxCap <= 12) {
            pstSharpenMpiDynaRegCfg->u8dirRt[1] = pstSharpen->u8EdgeFiltMaxCap;
            pstSharpenMpiDynaRegCfg->u8dirRt[0] = (pstSharpen->u8EdgeFiltMaxCap) >> 1;
        } else if (pstSharpen->u8EdgeFiltMaxCap <= 30) {
            pstSharpenMpiDynaRegCfg->u8dirRt[1] = pstSharpen->u8EdgeFiltMaxCap;
            pstSharpenMpiDynaRegCfg->u8dirRt[0] = 6;
        } else {
            pstSharpenMpiDynaRegCfg->u8dirRt[1] = 30;
            pstSharpenMpiDynaRegCfg->u8dirRt[0] = pstSharpen->u8EdgeFiltMaxCap - 24;
        }

        pstSharpenMpiDynaRegCfg->u8bEnShtCtrlByVar = 1;
        pstSharpenMpiDynaRegCfg->u8shtBldRt        = pstSharpen->u8ShootSupAdj;
        pstSharpenMpiDynaRegCfg->u8oshtVarThd1     = pstSharpen->u8ShootSupStr;

        pstSharpenMpiDynaRegCfg->u8ushtVarThd1 = pstSharpenMpiDynaRegCfg->u8oshtVarThd1;

        pstSharpenMpiDynaRegCfg->u16oMaxGain = pstSharpen->u16MaxSharpGain;
        pstSharpenMpiDynaRegCfg->u16uMaxGain = pstSharpen->u16MaxSharpGain;
        pstSharpenMpiDynaRegCfg->u16skinMaxU = pstSharpen->u8SkinUmax << 2;
        pstSharpenMpiDynaRegCfg->u16skinMinU = pstSharpen->u8SkinUmin << 2;
        pstSharpenMpiDynaRegCfg->u16skinMaxV = pstSharpen->u8SkinVmax << 2;
        pstSharpenMpiDynaRegCfg->u16skinMinV = pstSharpen->u8SkinVmin << 2;
        pstSharpenMpiDynaRegCfg->u8detailOshtThr[0]  = pstSharpen->u8DetailCtrlThr;
        pstSharpenMpiDynaRegCfg->u8detailOshtThr[1]  = SHARPEN_CLIP3(0, 255, (pstSharpen->u8DetailCtrlThr + SHRP_DETAIL_CTRL_THR_DELTA));
        pstSharpenMpiDynaRegCfg->u8detailUshtThr[0]  = pstSharpen->u8DetailCtrlThr;
        pstSharpenMpiDynaRegCfg->u8detailUshtThr[1]  = SHARPEN_CLIP3(0, 255, (pstSharpen->u8DetailCtrlThr + SHRP_DETAIL_CTRL_THR_DELTA));

        pstSharpenMpiDynaRegCfg->u8bEnLumaCtrl     =  0;
        for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
            pstSharpenMpiDynaRegCfg->au8LumaWgt[i] = pstSharpen->au8LumaWgt[i];
            if (pstSharpenMpiDynaRegCfg->au8LumaWgt[i] < 127) {
                pstSharpenMpiDynaRegCfg->u8bEnLumaCtrl = 1;
            }
        }
    }

    /* Calc all MulCoef */
    // Control shoot based on variance
    pstSharpenDefaultDynaRegCfg->s16oshtVarDiffMul = CalcMulCoef(pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[0], pstSharpenStaticRegCfg->u8oshtVarDiffWgt0,
                                                     pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[1], pstSharpenDefaultDynaRegCfg->u8oshtVarDiffWgt1,
                                                     SHRP_SHT_VAR_MUL_PRECS);
    pstSharpenDefaultDynaRegCfg->s16ushtVarDiffMul = CalcMulCoef(pstSharpenDefaultDynaRegCfg->u8ushtVarDiffThd[0], pstSharpenStaticRegCfg->u8ushtVarDiffWgt0,
                                                     pstSharpenDefaultDynaRegCfg->u8ushtVarDiffThd[1], pstSharpenDefaultDynaRegCfg->u8ushtVarDiffWgt1,
                                                     SHRP_SHT_VAR_MUL_PRECS);

    pstSharpenMpiDynaRegCfg->u16oshtVarMul         = CalcMulCoef(pstSharpenStaticRegCfg->u8oshtVarThd0, pstSharpenDefaultDynaRegCfg->u8oshtVarWgt0,
                                                     pstSharpenMpiDynaRegCfg->u8oshtVarThd1, pstSharpenStaticRegCfg->u8oshtVarWgt1,
                                                     SHRP_SHT_VAR_MUL_PRECS);
    pstSharpenMpiDynaRegCfg->u16ushtVarMul         = CalcMulCoef(pstSharpenStaticRegCfg->u8ushtVarThd0,  pstSharpenDefaultDynaRegCfg->u8ushtVarWgt0,
                                                     pstSharpenMpiDynaRegCfg->u8ushtVarThd1, pstSharpenStaticRegCfg->u8ushtVarWgt1,
                                                     SHRP_SHT_VAR_MUL_PRECS);
    pstSharpenMpiDynaRegCfg->s16detailOshtMul      = CalcMulCoef(pstSharpenMpiDynaRegCfg->u8detailOshtThr[0], pstSharpenMpiDynaRegCfg->u8detailOshtAmt,
                                                     pstSharpenMpiDynaRegCfg->u8detailOshtThr[1], pstSharpenMpiDynaRegCfg->u8oshtAmt,
                                                     SHRP_DETAIL_SHT_MUL_PRECS);
    pstSharpenMpiDynaRegCfg->s16detailUshtMul      = CalcMulCoef(pstSharpenMpiDynaRegCfg->u8detailUshtThr[0], pstSharpenMpiDynaRegCfg->u8detailUshtAmt,
                                                     pstSharpenMpiDynaRegCfg->u8detailUshtThr[1], pstSharpenMpiDynaRegCfg->u8ushtAmt,
                                                     SHRP_DETAIL_SHT_MUL_PRECS);
    pstSharpenMpiDynaRegCfg->s16chrRMul            = CalcMulCoef(pstSharpenStaticRegCfg->u8chrRThd[0], pstSharpenMpiDynaRegCfg->u8chrRGain0,
                                                     pstSharpenStaticRegCfg->u8chrRThd[1], pstSharpenStaticRegCfg->u8chrRGain1,
                                                     SHRP_CHR_MUL_SFT);
    pstSharpenMpiDynaRegCfg->s16chrGMul            = CalcMulCoef(pstSharpenStaticRegCfg->u8chrGThd[0], pstSharpenMpiDynaRegCfg->u16chrGGain0,
                                                     pstSharpenStaticRegCfg->u8chrGThd[1], pstSharpenStaticRegCfg->u16chrGGain1,
                                                     SHRP_CHR_MUL_SFT);
    pstSharpenMpiDynaRegCfg->s16chrBMul            = CalcMulCoef(pstSharpenStaticRegCfg->u8chrBThd[0], pstSharpenMpiDynaRegCfg->u8chrBGain0,
                                                     pstSharpenStaticRegCfg->u8chrBThd[1], pstSharpenStaticRegCfg->u8chrBGain1,
                                                     SHRP_CHR_MUL_SFT);
    pstSharpenMpiDynaRegCfg->s16skinEdgeMul        = CalcMulCoef(pstSharpenStaticRegCfg->u8skinEdgeThd[0], pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[0],
                                                     pstSharpenStaticRegCfg->u8skinEdgeThd[1], pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[1],
                                                     SHRP_SKIN_EDGE_MUL_PRECS);

}

static HI_BOOL __inline CheckSharpenOpen(ISP_SHARPEN_S *pstSharpen)
{
    return (pstSharpen->bSharpenEn == HI_TRUE);
}

HI_S32 SharpenProcWrite(VI_PIPE ViPipe, hi_isp_ctrl_proc_write *pstProc)
{
    hi_isp_ctrl_proc_write stProcTmp;

    ISP_SHARPEN_S *pstSharpen = HI_NULL;

    SHARPEN_GET_CTX(ViPipe, pstSharpen);
    ISP_CHECK_POINTER(pstSharpen);

    if ((pstProc->proc_buff == HI_NULL) || (pstProc->buff_len == 0)) {
        return HI_FAILURE;
    }

    stProcTmp.proc_buff = pstProc->proc_buff;
    stProcTmp.buff_len = pstProc->buff_len;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "-----SHARPEN INFO--------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "bSharpenEn");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16u\n",
                    (HI_U16)pstSharpen->bSharpenEn);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "LumaWgt 0--7:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au8LumaWgt[0],
                    (HI_U16)pstSharpen->au8LumaWgt[1],
                    (HI_U16)pstSharpen->au8LumaWgt[2],
                    (HI_U16)pstSharpen->au8LumaWgt[3],
                    (HI_U16)pstSharpen->au8LumaWgt[4],
                    (HI_U16)pstSharpen->au8LumaWgt[5],
                    (HI_U16)pstSharpen->au8LumaWgt[6],
                    (HI_U16)pstSharpen->au8LumaWgt[7]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "LumaWgt 8--15:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au8LumaWgt[8],
                    (HI_U16)pstSharpen->au8LumaWgt[9],
                    (HI_U16)pstSharpen->au8LumaWgt[10],
                    (HI_U16)pstSharpen->au8LumaWgt[11],
                    (HI_U16)pstSharpen->au8LumaWgt[12],
                    (HI_U16)pstSharpen->au8LumaWgt[13],
                    (HI_U16)pstSharpen->au8LumaWgt[14],
                    (HI_U16)pstSharpen->au8LumaWgt[15]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "LumaWgt 16--23:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au8LumaWgt[16],
                    (HI_U16)pstSharpen->au8LumaWgt[17],
                    (HI_U16)pstSharpen->au8LumaWgt[18],
                    (HI_U16)pstSharpen->au8LumaWgt[19],
                    (HI_U16)pstSharpen->au8LumaWgt[20],
                    (HI_U16)pstSharpen->au8LumaWgt[21],
                    (HI_U16)pstSharpen->au8LumaWgt[22],
                    (HI_U16)pstSharpen->au8LumaWgt[23]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "LumaWgt 24--31:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au8LumaWgt[24],
                    (HI_U16)pstSharpen->au8LumaWgt[25],
                    (HI_U16)pstSharpen->au8LumaWgt[26],
                    (HI_U16)pstSharpen->au8LumaWgt[27],
                    (HI_U16)pstSharpen->au8LumaWgt[28],
                    (HI_U16)pstSharpen->au8LumaWgt[29],
                    (HI_U16)pstSharpen->au8LumaWgt[30],
                    (HI_U16)pstSharpen->au8LumaWgt[31]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "TextureStr 0--7:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au16TextureStr[0],
                    (HI_U16)pstSharpen->au16TextureStr[1],
                    (HI_U16)pstSharpen->au16TextureStr[2],
                    (HI_U16)pstSharpen->au16TextureStr[3],
                    (HI_U16)pstSharpen->au16TextureStr[4],
                    (HI_U16)pstSharpen->au16TextureStr[5],
                    (HI_U16)pstSharpen->au16TextureStr[6],
                    (HI_U16)pstSharpen->au16TextureStr[7]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "TextureStr 8--15:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au16TextureStr[8],
                    (HI_U16)pstSharpen->au16TextureStr[9],
                    (HI_U16)pstSharpen->au16TextureStr[10],
                    (HI_U16)pstSharpen->au16TextureStr[11],
                    (HI_U16)pstSharpen->au16TextureStr[12],
                    (HI_U16)pstSharpen->au16TextureStr[13],
                    (HI_U16)pstSharpen->au16TextureStr[14],
                    (HI_U16)pstSharpen->au16TextureStr[15]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "TextureStr 16--23:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au16TextureStr[16],
                    (HI_U16)pstSharpen->au16TextureStr[17],
                    (HI_U16)pstSharpen->au16TextureStr[18],
                    (HI_U16)pstSharpen->au16TextureStr[19],
                    (HI_U16)pstSharpen->au16TextureStr[20],
                    (HI_U16)pstSharpen->au16TextureStr[21],
                    (HI_U16)pstSharpen->au16TextureStr[22],
                    (HI_U16)pstSharpen->au16TextureStr[23]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "TextureStr 24--31:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au16TextureStr[24],
                    (HI_U16)pstSharpen->au16TextureStr[25],
                    (HI_U16)pstSharpen->au16TextureStr[26],
                    (HI_U16)pstSharpen->au16TextureStr[27],
                    (HI_U16)pstSharpen->au16TextureStr[28],
                    (HI_U16)pstSharpen->au16TextureStr[29],
                    (HI_U16)pstSharpen->au16TextureStr[30],
                    (HI_U16)pstSharpen->au16TextureStr[31]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "EdgeStr 0--7:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au16EdgeStr[0],
                    (HI_U16)pstSharpen->au16EdgeStr[1],
                    (HI_U16)pstSharpen->au16EdgeStr[2],
                    (HI_U16)pstSharpen->au16EdgeStr[3],
                    (HI_U16)pstSharpen->au16EdgeStr[4],
                    (HI_U16)pstSharpen->au16EdgeStr[5],
                    (HI_U16)pstSharpen->au16EdgeStr[6],
                    (HI_U16)pstSharpen->au16EdgeStr[7]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "EdgeStr 8--15:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au16EdgeStr[8],
                    (HI_U16)pstSharpen->au16EdgeStr[9],
                    (HI_U16)pstSharpen->au16EdgeStr[10],
                    (HI_U16)pstSharpen->au16EdgeStr[11],
                    (HI_U16)pstSharpen->au16EdgeStr[12],
                    (HI_U16)pstSharpen->au16EdgeStr[13],
                    (HI_U16)pstSharpen->au16EdgeStr[14],
                    (HI_U16)pstSharpen->au16EdgeStr[15]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "EdgeStr 16--23:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au16EdgeStr[16],
                    (HI_U16)pstSharpen->au16EdgeStr[17],
                    (HI_U16)pstSharpen->au16EdgeStr[18],
                    (HI_U16)pstSharpen->au16EdgeStr[19],
                    (HI_U16)pstSharpen->au16EdgeStr[20],
                    (HI_U16)pstSharpen->au16EdgeStr[21],
                    (HI_U16)pstSharpen->au16EdgeStr[22],
                    (HI_U16)pstSharpen->au16EdgeStr[23]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%16s\n",
                    "EdgeStr 24--31:");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                    (HI_U16)pstSharpen->au16EdgeStr[24],
                    (HI_U16)pstSharpen->au16EdgeStr[25],
                    (HI_U16)pstSharpen->au16EdgeStr[26],
                    (HI_U16)pstSharpen->au16EdgeStr[27],
                    (HI_U16)pstSharpen->au16EdgeStr[28],
                    (HI_U16)pstSharpen->au16EdgeStr[29],
                    (HI_U16)pstSharpen->au16EdgeStr[30],
                    (HI_U16)pstSharpen->au16EdgeStr[31]);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s \n",
                    "TextureFreq", "EdgeFreq", "OverShoot", "UnderShoot", "ShootSupStr", "DetailCtrl", "EdgeFiltStr", "EdgeFiltMaxCap", "RGain", "GGain", "BGain", "SkinGain");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%12u"  "%12u"  "%12u"  "%12u"  "%12u"  "%12u"  "%12u"  "%12u"  "%12u" "%12u"  "%12u"   "%12u\n\n",
                    (HI_U16)pstSharpen->u16TextureFreq,
                    (HI_U16)pstSharpen->u16EdgeFreq,
                    (HI_U16)pstSharpen->u8OverShoot,
                    (HI_U16)pstSharpen->u8UnderShoot,
                    (HI_U16)pstSharpen->u8ShootSupStr,
                    (HI_U16)pstSharpen->u8DetailCtrl,
                    (HI_U16)pstSharpen->u8EdgeFiltStr,
                    (HI_U16)pstSharpen->u8EdgeFiltMaxCap,
                    (HI_U16)pstSharpen->u8RGain,
                    (HI_U16)pstSharpen->u8GGain,
                    (HI_U16)pstSharpen->u8BGain,
                    (HI_U16)pstSharpen->u8SkinGain);

    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%12s" "%14s" "%14s" "%12s" "%12s" "%12s"  "%12s\n",
                    "ShootSupAdj", "DetailCtrlThr", "MaxSharpGain", "SkinUmax", "SkinUmin", "SkinVmax", "SkinVmin");


    ISP_PROC_PRINTF(&stProcTmp, pstProc->write_len,
                    "%12u"  "%14u"  "%14u"  "%12u"  "%12u"  "%12u"   "%12u\n\n",
                    (HI_U16)pstSharpen->u8ShootSupAdj,
                    (HI_U16)pstSharpen->u8DetailCtrlThr,
                    (HI_U16)pstSharpen->u16MaxSharpGain,
                    (HI_U16)pstSharpen->u8SkinUmax,
                    (HI_U16)pstSharpen->u8SkinUmin,
                    (HI_U16)pstSharpen->u8SkinVmax,
                    (HI_U16)pstSharpen->u8SkinVmin);


    pstProc->write_len += 1;

    return HI_SUCCESS;
}

HI_S32 ISP_SharpenRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo, HI_VOID *pRegCfg, HI_S32 s32Rsv)
{

    HI_U8  i;
    HI_U32 u32Iso = 0;

    isp_usr_ctx     *pstIspCtx  = HI_NULL;
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    isp_reg_cfg *pstRegCfg  = (isp_reg_cfg *)pRegCfg;

    SHARPEN_GET_CTX(ViPipe, pstSharpen);
    ISP_CHECK_POINTER(pstSharpen);

    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    pstSharpen->bSharpenEn = hi_ext_system_manual_isp_sharpen_en_read(ViPipe);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg.bEnable = pstSharpen->bSharpenEn;
    }

    pstRegCfg->cfg_key.bit1SharpenCfg = 1;

    /* check hardware setting */
    if (!CheckSharpenOpen(pstSharpen)) {
        return HI_SUCCESS;
    }

    /* sharpen strength linkage with the u32ISO calculated by ae */
    u32Iso = pstIspCtx->linkage.iso;

    SharpenReadExtregs(ViPipe);
    SharpenReadProMode(ViPipe);
    if ((u32Iso != pstSharpen->u32IsoLast) ||
        (pstIspCtx->linkage.fswdr_mode != pstIspCtx->linkage.pre_fswdr_mode)) { // will not work if ISO is the same
        ISP_Sharpen_GetDefaultRegCfg(ViPipe, u32Iso);
        for (i = 0; i < pstRegCfg->cfg_num; i++) {
            pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg.stDynaRegCfg.stDefaultDynaRegCfg.bResh = HI_TRUE;
        }
    }

    if (pstSharpen->bSharpenMpiUpdateEn) {
        if (pstSharpen->u8ManualSharpenYuvEnabled == OP_TYPE_AUTO) { // auto mode
            ISP_Sharpen_GetMpiRegCfg(ViPipe, u32Iso);
        }
        for (i = 0; i < pstRegCfg->cfg_num; i++) {
            pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.bResh            = HI_TRUE;
            pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.u32UpdateIndex  += 1;
            SharpenMPI2Reg(&(pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg), pstSharpen);
        }
    } else {
        if (u32Iso != pstSharpen->u32IsoLast) {
            if (pstSharpen->u8ManualSharpenYuvEnabled == OP_TYPE_AUTO) { // auto mode
                ISP_Sharpen_GetMpiRegCfg(ViPipe, u32Iso);
                for (i = 0; i < pstRegCfg->cfg_num; i++) {
                    pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.bResh = HI_TRUE;
                    pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.u32UpdateIndex += 1;
                    SharpenMPI2Reg(&(pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg), pstSharpen);
                }
            } else {
                for (i = 0; i < pstRegCfg->cfg_num; i++) {
                    SharpenMPI2Reg(&(pstRegCfg->alg_reg_cfg[i].stSharpenRegCfg), pstSharpen);
                }
            }
        } else {
        }
    }

    SharpenActualUpdate(ViPipe);

    pstSharpen->u32IsoLast = u32Iso;    // will not work if ISO is the same

    return HI_SUCCESS;
}

HI_S32 ISP_SharpenCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_SharpenWdrModeSet(ViPipe, (HI_VOID *)&pRegCfg->reg_cfg);
            break;
        case ISP_PROC_WRITE:
            SharpenProcWrite(ViPipe, (hi_isp_ctrl_proc_write *)pValue);
            break;
        default :
            break;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SharpenExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_reg_cfg_attr *pRegCfg   = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stSharpenRegCfg.bEnable = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1SharpenCfg = 1;

    hi_ext_system_isp_sharpen_manu_mode_write(ViPipe, HI_FALSE);

    SharpenCtxExit(ViPipe);

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_sharpen(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_SHARPEN;
    pstAlgs->alg_func.pfn_alg_init = ISP_SharpenInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_SharpenRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_SharpenCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_SharpenExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
