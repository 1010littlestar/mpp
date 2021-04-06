/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"
#include "isp_defaults.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_VOID ISP_RegsDefault(VI_PIPE ViPipe)
{
    return;
}

HI_VOID ISP_RegsInitialize(VI_PIPE ViPipe)
{
    return;
}

/* The ext regs is a block of memory, not real register, need a default value. */
HI_VOID ISP_ExtRegsDefault(VI_PIPE ViPipe)
{
    HI_U32 i;

    hi_ext_system_freeze_firmware_write(ViPipe, HI_EXT_SYSTEM_FREEZE_FIRMWARE_DEFAULT);

    hi_ext_system_sys_debug_enable_write(ViPipe, HI_EXT_SYSTEM_SYS_DEBUG_ENABLE_DEFAULT);
    hi_ext_system_sys_debug_high_addr_write(ViPipe, HI_EXT_SYSTEM_SYS_DEBUG_HIGH_ADDR_DEFAULT);
    hi_ext_system_sys_debug_low_addr_write(ViPipe, HI_EXT_SYSTEM_SYS_DEBUG_LOW_ADDR_DEFAULT);
    hi_ext_system_sys_debug_size_write(ViPipe, HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT);
    hi_ext_system_sys_debug_depth_write(ViPipe, HI_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT);
    hi_ext_system_statistics_ctrl_lowbit_write(ViPipe, HI_EXT_SYSTEM_STATISTICS_LOWBIT_DEFAULT);
    hi_ext_system_statistics_ctrl_highbit_write(ViPipe, HI_EXT_SYSTEM_STATISTICS_HIGHBIT_DEFAULT);
    hi_ext_system_be_free_buffer_high_addr_write(ViPipe, HI_EXT_SYSTEM_BE_FREE_BUFFER_HIGH_ADDR_DEFAULT);
    hi_ext_system_be_free_buffer_low_addr_write(ViPipe, HI_EXT_SYSTEM_BE_FREE_BUFFER_LOW_ADDR_DEFAULT);
    hi_ext_system_be_lut_stt_buffer_high_addr_write(ViPipe, HI_EXT_SYSTEM_BE_LUT_STT_BUFFER_HIGH_ADDR_DEFAULT);
    hi_ext_system_be_lut_stt_buffer_low_addr_write(ViPipe, HI_EXT_SYSTEM_BE_LUT_STT_BUFFER_LOW_ADDR_DEFAULT);
    hi_ext_system_ldci_read_stt_buffer_low_addr_write(ViPipe, HI_EXT_SYSTEM_LDCI_READ_STT_BUF_LOW_ADDR_DEFAULT);
    hi_ext_system_ldci_read_stt_buffer_high_addr_write(ViPipe, HI_EXT_SYSTEM_LDCI_READ_STT_BUF_HIGH_ADDR_DEFAULT);
    hi_ext_system_top_channel_select_write(ViPipe, HI_EXT_SYSTEM_TOP_CHANNEL_SELECT_WRITE_DEFAULT);
    hi_ext_system_top_channel_select_pre_write(ViPipe, HI_EXT_SYSTEM_TOP_CHANNEL_SELECT_PRE_WRITE_DEFAULT);
    hi_ext_system_isp_raw_position_enable_write(ViPipe, HI_EXT_SYSTEM_ISP_RAW_POSITION_ENABLE_WRITE_DEFAULT);

    /* isp pipe different configs for stitch mode */
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        hi_ext_system_isp_pipe_diff_offset_write(ViPipe, i, HI_EXT_SYSTEM_ISP_PIPE_DIFF_OFFSET_DEFAULT);
        hi_ext_system_isp_pipe_diff_gain_write(ViPipe, i, HI_EXT_SYSTEM_ISP_PIPE_DIFF_GAIN_DEFAULT);
    }

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        if (i % 4 == 0) {
            hi_ext_system_isp_pipe_diff_ccm_write(ViPipe, i, HI_EXT_SYSTEM_ISP_PIPE_DIFF_CCM_DEFAULT);
        } else {
            hi_ext_system_isp_pipe_diff_ccm_write(ViPipe, i, 0);
        }
    }

    return;
}

HI_VOID ISP_ExtRegsInitialize(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_U32 u32IspBindAttr = 0;
    ISP_CMOS_SENSOR_MAX_RESOLUTION_S *pstSnsMaxResolution = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_SensorGetMaxResolution(ViPipe, &pstSnsMaxResolution);
    hi_ext_system_sensor_max_resolution_width_write(ViPipe, pstSnsMaxResolution->u32MaxWidth);
    hi_ext_system_sensor_max_resolution_height_write(ViPipe, pstSnsMaxResolution->u32MaxHeight);

    /* update Isp bind attr to ext regs */
    u32IspBindAttr = SignedLeftShift(pstIspCtx->bind_attr.ae_lib.id, 8) + pstIspCtx->bind_attr.awb_lib.id;

    hi_ext_system_bind_attr_write(ViPipe, u32IspBindAttr);

    hi_ext_system_block_num_write(ViPipe, pstIspCtx->block_attr.block_num);

    return;
}

HI_VOID ISP_YUVExtRegsInitialize(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_U32 u32IspBindAttr = 0;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    /* update Isp bind attr to ext regs */
    u32IspBindAttr = (pstIspCtx->bind_attr.ae_lib.id << 8) | pstIspCtx->bind_attr.awb_lib.id;
    hi_ext_system_bind_attr_write(ViPipe, u32IspBindAttr);

    hi_ext_system_block_num_write(ViPipe, pstIspCtx->block_attr.block_num);

    return;
}

static HI_VOID ISP_ProShpParamInitialize(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_U8 i, j, k;
    ISP_GET_CTX(ViPipe, pstIspCtx);
    pstIspCtx->pro_shp_param_ctrl.pro_shp_param->enable = HI_FALSE;
    pstIspCtx->pro_shp_param_ctrl.pro_shp_param->param_num = 3;
    for (k = 0; k < PRO_MAX_FRAME_NUM; k++) {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
                pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].texture_str[j][i] = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT;
                pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].edge_str[j][i]    = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT;
            }

            for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
                pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].luma_wgt[j][i] = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT;
            }

            pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].texture_freq[i]  = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT;
            pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].edge_freq[i]     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT;
            pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].over_shoot[i]    = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT;
            pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].under_shoot[i]   = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT;
            pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].shoot_sup_str[i] = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT;
            pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].detail_ctrl[i]   = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT;
            pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].edge_filt_str[i] = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT;
            pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].r_gain[i]        = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT;
            pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].b_gain[i]        = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT;
            pstIspCtx->pro_shp_param_ctrl.pro_shp_param->shp_attr[k].skin_gain[i]     = HI_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT;
        }
    }
}

static HI_VOID ISP_ProNrParamInitialize(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_U8 i, j, k;
    ISP_GET_CTX(ViPipe, pstIspCtx);
    pstIspCtx->pro_nr_param_ctrl.pro_nr_param->enable = HI_FALSE;
    pstIspCtx->pro_nr_param_ctrl.pro_nr_param->param_num = 3;
    for (k = 0; k < PRO_MAX_FRAME_NUM; k++) {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[k].fine_str[i]   = HI_EXT_SYSTEM_BAYERNR_MANU_FINE_STRENGTH_DEFAULT;
            pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[k].coring_wgt[i] = HI_EXT_SYSTEM_BAYERNR_MANU_CORING_WEIGHT_DEFAULT;

            for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
                pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[k].chroma_str[j][i] = HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT;
                pstIspCtx->pro_nr_param_ctrl.pro_nr_param->nr_attr[k].coarse_str[j][i] = HI_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DEFAULT;
            }
        }
    }
}

HI_VOID ISP_AlgKeyInit(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstIspCtx->alg_key.key = 0xFFFFFFFFFFFFFFFF;
}

HI_VOID ISP_GlobalInitialize(VI_PIPE ViPipe)
{
    HI_U8  i;
    HI_U32 u32Value = 0;
    HI_VOID   *pValue    = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstIspCtx->sns_image_mode.width  = hi_ext_top_sensor_width_read(ViPipe);
    pstIspCtx->sns_image_mode.height = hi_ext_top_sensor_height_read(ViPipe);
    u32Value = hi_ext_system_fps_base_read(ViPipe);
    pValue = (HI_VOID *)&u32Value;
    pstIspCtx->sns_image_mode.fps = *(HI_FLOAT *)pValue;

    pstIspCtx->pre_sns_image_mode.width  = pstIspCtx->sns_image_mode.width;
    pstIspCtx->pre_sns_image_mode.height = pstIspCtx->sns_image_mode.height;
    pstIspCtx->pre_sns_image_mode.fps    = pstIspCtx->sns_image_mode.fps;

    pstIspCtx->change_wdr_mode   = HI_FALSE;
    pstIspCtx->change_image_mode = HI_FALSE;
    pstIspCtx->change_isp_res    = HI_FALSE;
    pstIspCtx->count             = 0;
    pstIspCtx->sns_wdr_mode      = hi_ext_system_sensor_wdr_mode_read(ViPipe);
    pstIspCtx->pre_sns_wdr_mode  = pstIspCtx->sns_wdr_mode;

    pstIspCtx->be_raw_info.enable_be_raw    = HI_FALSE;
    pstIspCtx->be_raw_info.pre_enabe_be_raw = HI_FALSE;
    pstIspCtx->frame_cnt            = 0;
    pstIspCtx->isp_image_mode_flag  = 0;

    pstIspCtx->linkage.defect_pixel = HI_FALSE;
    pstIspCtx->freeze_fw            = HI_FALSE;

    pstIspCtx->linkage.snap_state          = HI_FALSE;
    pstIspCtx->linkage.pro_trigger         = HI_FALSE;
    pstIspCtx->linkage.stat_ready          = HI_FALSE;
    pstIspCtx->linkage.run_once            = HI_FALSE;
    pstIspCtx->pro_frm_cnt                 = 0;
    pstIspCtx->linkage.iso_done_frm_cnt    = 0;
    pstIspCtx->linkage.pro_index           = 0;
    pstIspCtx->linkage.cfg2valid_delay_max = 2;
    pstIspCtx->linkage.fswdr_mode          = ISP_FSWDR_NORMAL_MODE;
    pstIspCtx->linkage.pre_fswdr_mode      = ISP_FSWDR_NORMAL_MODE;
    for (i = 0; i < ISP_SYNC_ISO_BUF_MAX; i++) {
        pstIspCtx->linkage.sync_iso_buf[i]  = 100;
        pstIspCtx->linkage.pro_index_buf[i] = 0;
    }
    for (i = 0; i < ISP_MAX_BE_NUM; i++) {
        pstIspCtx->special_opt.be_on_stt_update[i] = HI_TRUE;
    }

    memset(&pstIspCtx->pre_dng_ccm, 0, sizeof(isp_dng_ccm));
    memset(&pstIspCtx->pre_dng_color_param, 0, sizeof(hi_isp_dng_color_param));
    memset(&pstIspCtx->dcf_update_info, 0, sizeof(hi_isp_dcf_update_info));
    memset(&pstIspCtx->frame_info, 0, sizeof(hi_isp_frame_info));

    pstIspCtx->special_opt.fe_stt_update = HI_TRUE;
    ISP_ProShpParamInitialize(ViPipe);
    ISP_ProNrParamInitialize(ViPipe);
    ISP_AlgKeyInit(ViPipe);

    return;
}

HI_VOID ISP_DngExtRegsInitialize(VI_PIPE ViPipe, ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    if (pstDngColorParam->stWbGain1.u16Bgain > 0xFFF) {
        ISP_ERR_TRACE("stWbGain1.u16Bgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain1.u16Bgain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain1.u16Ggain > 0xFFF) {
        ISP_ERR_TRACE("stWbGain1.u16Ggain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain1.u16Ggain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain1.u16Rgain > 0xFFF) {
        ISP_ERR_TRACE("stWbGain1.u16Rgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain1.u16Rgain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain2.u16Bgain > 0xFFF) {
        ISP_ERR_TRACE("stWbGain2.u16Bgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain2.u16Bgain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain2.u16Ggain > 0xFFF) {
        ISP_ERR_TRACE("stWbGain2.u16Ggain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain2.u16Ggain = 0xFFF;
    }
    if (pstDngColorParam->stWbGain2.u16Rgain > 0xFFF) {
        ISP_ERR_TRACE("stWbGain2.u16Rgain can't bigger than 0xFFF in cmos!\n");
        pstDngColorParam->stWbGain2.u16Rgain = 0xFFF;
    }

    hi_ext_system_dng_high_wb_gain_r_write(ViPipe, pstDngColorParam->stWbGain1.u16Rgain);
    hi_ext_system_dng_high_wb_gain_g_write(ViPipe, pstDngColorParam->stWbGain1.u16Ggain);
    hi_ext_system_dng_high_wb_gain_b_write(ViPipe, pstDngColorParam->stWbGain1.u16Bgain);
    hi_ext_system_dng_low_wb_gain_r_write(ViPipe, pstDngColorParam->stWbGain2.u16Rgain);
    hi_ext_system_dng_low_wb_gain_g_write(ViPipe, pstDngColorParam->stWbGain2.u16Ggain);
    hi_ext_system_dng_low_wb_gain_b_write(ViPipe, pstDngColorParam->stWbGain2.u16Bgain);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
