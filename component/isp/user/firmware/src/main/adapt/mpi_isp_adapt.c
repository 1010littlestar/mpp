/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>

#include "mpi_sys.h"
#include "mkp_isp.h"
#include "hi_comm_isp_adapt.h"
#include "hi_comm_3a_adapt.h"
#include "hi_ae_comm_adapt.h"
#include "hi_awb_comm.h"
#include "hi_comm_vi.h"
#include "isp_inner.h"
#include "isp_ext_config.h"
#include "isp_debug.h"
#include "isp_main.h"
#include "isp_proc.h"
#include "isp_math_utils.h"
#include "isp_vreg.h"
#include "hi_math.h"
#include "hi_comm_sns_adapt.h"
#include "mpi_isp_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

MPI_STATIC hi_s32 hi_mpi_isp_set_pub_attr(VI_PIPE vi_pipe, const hi_isp_pub_attr *pub_attr)
{
    hi_s32  ret;
    hi_void *value    = HI_NULL;
    isp_usr_ctx *isp_ctx_info = HI_NULL;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_GET_CTX(vi_pipe, isp_ctx_info);
    ISP_CHECK_POINTER(isp_ctx_info);
    ISP_CHECK_POINTER(pub_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if ((pub_attr->wnd_rect.width < RES_WIDTH_MIN) ||
        (pub_attr->wnd_rect.width > RES_WIDTH_MAX(vi_pipe)) ||
        (pub_attr->wnd_rect.width % 4) != 0) {
        ISP_ERR_TRACE("Invalid image width:%d!\n", pub_attr->wnd_rect.width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->wnd_rect.height < RES_HEIGHT_MIN) ||
        (pub_attr->wnd_rect.height > RES_HEIGHT_MAX(vi_pipe)) ||
        (pub_attr->wnd_rect.height % 4) != 0) {
        ISP_ERR_TRACE("Invalid image height:%d!\n", pub_attr->wnd_rect.height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->sns_size.width < RES_WIDTH_MIN) ||
        (pub_attr->sns_size.width > SENSOR_RES_WIDTH_MAX)) {
        ISP_ERR_TRACE("Invalid sensor image width:%d!\n", pub_attr->sns_size.width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->sns_size.height < RES_HEIGHT_MIN) ||
        (pub_attr->sns_size.height > SENSOR_RES_HEIGHT_MAX)) {
        ISP_ERR_TRACE("Invalid sensor image height:%d!\n", pub_attr->sns_size.height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->wnd_rect.x < 0) || (pub_attr->wnd_rect.x > RES_WIDTH_MAX(vi_pipe) - RES_WIDTH_MIN)) {
        ISP_ERR_TRACE("Invalid image X:%d!\n", pub_attr->wnd_rect.x);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->wnd_rect.y < 0) || (pub_attr->wnd_rect.y > RES_HEIGHT_MAX(vi_pipe) - RES_HEIGHT_MIN)) {
        ISP_ERR_TRACE("Invalid image Y:%d!\n", pub_attr->wnd_rect.y);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pub_attr->f32_frame_rate <= 0.0) || (pub_attr->f32_frame_rate > FRAME_RATE_MAX)) {
        ISP_ERR_TRACE("Invalid frame_rate:%f!\n", pub_attr->f32_frame_rate);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pub_attr->bayer >= BAYER_BUTT) {
        ISP_ERR_TRACE("Invalid bayer pattern:%d!\n", pub_attr->bayer);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pub_attr->wdr_mode >= WDR_MODE_BUTT) {
        ISP_ERR_TRACE("Invalid WDR mode %d!\n", pub_attr->wdr_mode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    ret = ioctl(g_as32IspFd[vi_pipe], ISP_PUB_ATTR_INFO, pub_attr);
    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set ISP PUB attr failed\n", vi_pipe);
        return ret;
    }

    /* set WDR mode */
    hi_ext_top_wdr_switch_write(vi_pipe, HI_FALSE);

    isp_ctx_info->para_rec.wdr_cfg = HI_TRUE;
    hi_ext_top_wdr_cfg_write(vi_pipe, isp_ctx_info->para_rec.wdr_cfg);

    if ((hi_u8)pub_attr->wdr_mode == hi_ext_system_sensor_wdr_mode_read(vi_pipe)) {
        hi_ext_top_wdr_switch_write(vi_pipe, HI_TRUE);
    } else {
        hi_ext_system_sensor_wdr_mode_write(vi_pipe, (hi_u8)pub_attr->wdr_mode);
    }

    /* set othes cfgs */
    hi_ext_top_res_switch_write(vi_pipe, HI_FALSE);

    hi_ext_system_corp_pos_x_write(vi_pipe, pub_attr->wnd_rect.x);
    hi_ext_system_corp_pos_y_write(vi_pipe, pub_attr->wnd_rect.y);

    hi_ext_sync_total_width_write(vi_pipe, pub_attr->wnd_rect.width);
    hi_ext_sync_total_height_write(vi_pipe, pub_attr->wnd_rect.height);

    hi_ext_top_sensor_width_write(vi_pipe, pub_attr->sns_size.width);
    hi_ext_top_sensor_height_write(vi_pipe, pub_attr->sns_size.height);

    hi_ext_system_rggb_cfg_write(vi_pipe, (hi_u8)pub_attr->bayer);

    value = (hi_void *)(&pub_attr->f32_frame_rate);
    hi_ext_system_fps_base_write(vi_pipe, *(hi_u32 *)value);
    hi_ext_system_sensor_mode_write(vi_pipe, pub_attr->sns_mode);

    isp_ctx_info->para_rec.pub_cfg = HI_TRUE;
    hi_ext_top_pub_attr_cfg_write(vi_pipe, isp_ctx_info->para_rec.pub_cfg);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_pub_attr(VI_PIPE vi_pipe, hi_isp_pub_attr *pub_attr)
{
    hi_u8 bayer;
    hi_u8 wd_rmode;
    hi_u32 fps_value = 0;
    hi_void *value = HI_NULL;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(pub_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    bayer = hi_ext_system_rggb_cfg_read(vi_pipe);
    pub_attr->bayer = (bayer >= BAYER_BUTT) ? BAYER_BUTT : bayer;

    wd_rmode = hi_ext_system_sensor_wdr_mode_read(vi_pipe);
    pub_attr->wdr_mode = (wd_rmode >= WDR_MODE_BUTT) ? WDR_MODE_BUTT : wd_rmode;

    fps_value = hi_ext_system_fps_base_read(vi_pipe);
    value   = (hi_void *)&fps_value;
    pub_attr->f32_frame_rate = *(hi_float *)value;

    pub_attr->sns_mode = hi_ext_system_sensor_mode_read(vi_pipe);

    pub_attr->wnd_rect.x      = hi_ext_system_corp_pos_x_read(vi_pipe);
    pub_attr->wnd_rect.y      = hi_ext_system_corp_pos_y_read(vi_pipe);
    pub_attr->wnd_rect.width  = hi_ext_sync_total_width_read(vi_pipe);
    pub_attr->wnd_rect.height = hi_ext_sync_total_height_read(vi_pipe);
    pub_attr->sns_size.width  = hi_ext_top_sensor_width_read(vi_pipe);
    pub_attr->sns_size.height = hi_ext_top_sensor_height_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_pipe_differ_attr(VI_PIPE vi_pipe, const hi_isp_pipe_diff_attr *pipe_differ)
{
    return isp_set_pipe_differ_attr(vi_pipe, pipe_differ);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_pipe_differ_attr(VI_PIPE vi_pipe, hi_isp_pipe_diff_attr *pipe_differ)
{
    return isp_get_pipe_differ_attr(vi_pipe, pipe_differ);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_module_control(VI_PIPE vi_pipe, const hi_isp_module_ctrl *mod_ctrl)
{
    hi_s32 ret = HI_SUCCESS;
    hi_u32 chn_sel_cur = 0;
    hi_u32 chn_sel_pre = 0;
    hi_u64 info = 0;
    hi_bool bEnale;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(mod_ctrl);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    info = mod_ctrl->key;

    hi_ext_system_isp_bit_bypass_write(vi_pipe, info);

    hi_ext_system_isp_dgain_enable_write(vi_pipe, !((info >> 0) & 0x1));
    hi_ext_system_antifalsecolor_enable_write(vi_pipe, !((info >> 1) & 0x1));
    hi_ext_system_ge_enable_write(vi_pipe, !((info >> 2) & 0x1));
    hi_ext_system_dpc_dynamic_cor_enable_write(vi_pipe, !((info >> 3) & 0x1));
    hi_ext_system_bayernr_enable_write(vi_pipe, !((info >> 4) & 0x1));
    hi_ext_system_dehaze_enable_write(vi_pipe, !((info >> 5) & 0x1));
    hi_ext_system_awb_gain_enable_write(vi_pipe, !((info >> 6) & 0x1));
    hi_ext_system_isp_mesh_shading_enable_write(vi_pipe, !((info >> 7) & 0x1));
    hi_ext_system_drc_enable_write(vi_pipe, !((info >> 8) & 0x1));
    hi_ext_system_demosaic_enable_write(vi_pipe, !((info >> 9) & 0x1));
    hi_ext_system_cc_enable_write(vi_pipe, !((info >> 10) & 0x1));
    hi_ext_system_gamma_en_write(vi_pipe, !((info >> 11) & 0x1));
    hi_ext_system_wdr_en_write(vi_pipe, !((info >> 12) & 0x1));
    hi_ext_system_ca_en_write(vi_pipe, !((info >> 13) & 0x1));
    hi_ext_system_csc_enable_write(vi_pipe, !((info >> 14) & 0x1));
    hi_ext_system_rc_en_write(vi_pipe, !((info >> 15) & 0x1));
    hi_ext_system_manual_isp_sharpen_en_write(vi_pipe, !((info >> 16) & 0x1));
    hi_ext_system_localCAC_enable_write(vi_pipe, !((info >> 17) & 0x1));
    hi_ext_system_GlobalCAC_enable_write(vi_pipe, !((info >> 18) & 0x1));
    hi_ext_system_top_channel_select_write(vi_pipe, (info >> 19) & 0x3);
    hi_ext_system_ldci_enable_write(vi_pipe, !((info >> 21) & 0x1));
    hi_ext_system_pregamma_en_write(vi_pipe, !((info >> 22) & 0x1));
    hi_ext_system_isp_radial_shading_enable_write(vi_pipe, !((info >> 23) & 0x1));
    hi_ext_system_ae_fe_en_write(vi_pipe, !((info >> 24) & 0x1));
    hi_ext_system_ae_be_en_write(vi_pipe, !((info >> 25) & 0x1));
    hi_ext_system_mg_en_write(vi_pipe, !((info >> 26) & 0x1));
    hi_ext_system_detail_enable_write(vi_pipe, !((info >> 27) & 0x1));
    hi_ext_system_af_enable_write(vi_pipe, ~((info >> 28) & 0x3));
    hi_ext_system_awb_sta_enable_write(vi_pipe, !((info >> 30) & 0x1));
    hi_ext_system_clut_en_write(vi_pipe, !((info >> 31) & 0x1));
    hi_ext_system_manual_isp_hlc_en_write(vi_pipe, !((info >> 32) & 0x1));
    hi_ext_system_manual_isp_edgemark_en_write(vi_pipe, !((info >> 33) & 0x1));
    hi_ext_system_rgbir_enable_write(vi_pipe, !((info >> 34) & 0x1));
    chn_sel_cur = mod_ctrl->bit2_chn_select;
    chn_sel_pre = hi_ext_system_top_channel_select_pre_read(vi_pipe);

    if (chn_sel_pre != chn_sel_cur) {
        ret = ioctl(g_as32IspFd[vi_pipe], ISP_CHN_SELECT_CFG, &chn_sel_cur);

        if (ret != HI_SUCCESS) {
            ISP_ERR_TRACE("set isp[%d] chn select failed with ec %#x!\n", vi_pipe, ret);
            return ret;
        }

        hi_ext_system_top_channel_select_pre_write(vi_pipe, chn_sel_cur);
    }

    hi_ext_af_set_flag_write(vi_pipe, HI_EXT_AF_SET_FLAG_ENABLE);

    bEnale = !((info >> 30) & 0x1);
    ret = ioctl(g_as32IspFd[vi_pipe], ISP_AWB_EN_SET, &bEnale);
    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Set AWB enable error!\n");
        return ret;
    }

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_module_control(VI_PIPE vi_pipe, hi_isp_module_ctrl *mod_ctrl)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(mod_ctrl);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    mod_ctrl->bit_bypass_isp_d_gain        = !(hi_ext_system_isp_dgain_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_anti_false_color  = !(hi_ext_system_antifalsecolor_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_crosstalk_removal = !(hi_ext_system_ge_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_dpc               = !(hi_ext_system_dpc_dynamic_cor_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_nr                = !(hi_ext_system_bayernr_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_dehaze            = !(hi_ext_system_dehaze_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_wb_gain           = !(hi_ext_system_awb_gain_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_mesh_shading      = !(hi_ext_system_isp_mesh_shading_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_drc               = !(hi_ext_system_drc_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_demosaic          = !(hi_ext_system_demosaic_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_color_matrix      = !(hi_ext_system_cc_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_gamma             = !(hi_ext_system_gamma_en_read(vi_pipe));
    mod_ctrl->bit_bypass_fswdr             = !(hi_ext_system_wdr_en_read(vi_pipe));
    mod_ctrl->bit_bypass_ca                = !(hi_ext_system_ca_en_read(vi_pipe));
    mod_ctrl->bit_bypass_csc               = !(hi_ext_system_csc_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_radial_crop       = !(hi_ext_system_rc_en_read(vi_pipe));
    mod_ctrl->bit_bypass_sharpen           = !(hi_ext_system_manual_isp_sharpen_en_read(vi_pipe));
    mod_ctrl->bit_bypass_local_cac         = !(hi_ext_system_localCAC_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_global_cac        = !(hi_ext_system_GlobalCAC_enable_read(vi_pipe));
    mod_ctrl->bit2_chn_select              = hi_ext_system_top_channel_select_read(vi_pipe);
    mod_ctrl->bit_bypass_ldci              = !(hi_ext_system_ldci_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_pregamma          = !(hi_ext_system_pregamma_en_read(vi_pipe));
    mod_ctrl->bit_bypass_radial_shading    = !(hi_ext_system_isp_radial_shading_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_ae_stat_fe        = !(hi_ext_system_ae_fe_en_read(vi_pipe));
    mod_ctrl->bit_bypass_ae_stat_be        = !(hi_ext_system_ae_be_en_read(vi_pipe));
    mod_ctrl->bit_bypass_mg_stat           = !(hi_ext_system_mg_en_read(vi_pipe));
    mod_ctrl->bit_bypass_de                = !(hi_ext_system_detail_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_af_stat_fe        = !(hi_ext_system_af_enable_read(vi_pipe) & 0x1);
    mod_ctrl->bit_bypass_af_stat_be        = !((hi_ext_system_af_enable_read(vi_pipe) >> 1) & 0x1);
    mod_ctrl->bit_bypass_awb_stat          = !(hi_ext_system_awb_sta_enable_read(vi_pipe));
    mod_ctrl->bit_bypass_clut              = !(hi_ext_system_clut_en_read(vi_pipe));
    mod_ctrl->bit_bypass_hlc               = !(hi_ext_system_manual_isp_hlc_en_read(vi_pipe));
    mod_ctrl->bit_bypass_edge_mark         = !(hi_ext_system_manual_isp_edgemark_en_read(vi_pipe));
    mod_ctrl->bit_bypass_rgbir             = !(hi_ext_system_rgbir_enable_read(vi_pipe));

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_raw_pos(VI_PIPE vi_pipe, const hi_isp_raw_pos_attr *raw_pos_attr)
{
    return isp_set_raw_pos(vi_pipe, raw_pos_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_raw_pos(VI_PIPE vi_pipe, hi_isp_raw_pos_attr *raw_pos_attr)
{
    return isp_get_raw_pos(vi_pipe, raw_pos_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_fmw_state(VI_PIPE vi_pipe, const hi_isp_fmw_state state)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (state >= ISP_FMW_STATE_BUTT) {
        ISP_ERR_TRACE("Invalid firmware state %d!\n", state);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_freeze_firmware_write(vi_pipe, (hi_u8)state);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_fmw_state(VI_PIPE vi_pipe, hi_isp_fmw_state *state)
{
    hi_u8 fmw_state;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(state);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    fmw_state = hi_ext_system_freeze_firmware_read(vi_pipe);
    *state  = (fmw_state >= ISP_FMW_STATE_BUTT) ? ISP_FMW_STATE_BUTT : fmw_state;

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_ldci_attr(VI_PIPE vi_pipe, const hi_isp_ldci_attr *ldci_attr)
{
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(ldci_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ISP_CHECK_BOOL(ldci_attr->enable);

    if (ldci_attr->gauss_lpf_sigma < 0x1) {
        ISP_ERR_TRACE("Invalid gauss_lpf_sigma!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (ldci_attr->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid op_type!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_ldci_enable_write(vi_pipe, ldci_attr->enable);
    hi_ext_system_ldci_gaussLPFSigma_write(vi_pipe, ldci_attr->gauss_lpf_sigma);
    hi_ext_system_ldci_manu_mode_write(vi_pipe, ldci_attr->op_type);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (ldci_attr->auto_attr.he_wgt[i].he_pos_wgt.wgt > HI_ISP_LDCI_HEPOSWGT_MAX) {
            ISP_ERR_TRACE("Invalid he_wgt.he_pos_wgt.wgt[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (ldci_attr->auto_attr.he_wgt[i].he_pos_wgt.sigma < 0x1) {
            ISP_ERR_TRACE("Invalid he_wgt.he_pos_wgt.sigma[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (ldci_attr->auto_attr.he_wgt[i].he_neg_wgt.wgt > HI_ISP_LDCI_HENEGWGT_MAX) {
            ISP_ERR_TRACE("Invalid he_wgt.he_neg_wgt.wgt[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (ldci_attr->auto_attr.he_wgt[i].he_neg_wgt.sigma < 0x1) {
            ISP_ERR_TRACE("Invalid he_wgt.he_neg_wgt.sigma[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (ldci_attr->auto_attr.blc_ctrl[i] > 0x1ff) {
            ISP_ERR_TRACE("Invalid blc_ctrl[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_ldci_hePosWgt_write(vi_pipe, i, ldci_attr->auto_attr.he_wgt[i].he_pos_wgt.wgt);
        hi_ext_system_ldci_hePosSigma_write(vi_pipe, i, ldci_attr->auto_attr.he_wgt[i].he_pos_wgt.sigma);
        hi_ext_system_ldci_hePosMean_write(vi_pipe, i, ldci_attr->auto_attr.he_wgt[i].he_pos_wgt.mean);
        hi_ext_system_ldci_heNegWgt_write(vi_pipe, i, ldci_attr->auto_attr.he_wgt[i].he_neg_wgt.wgt);
        hi_ext_system_ldci_heNegSigma_write(vi_pipe, i, ldci_attr->auto_attr.he_wgt[i].he_neg_wgt.sigma);
        hi_ext_system_ldci_heNegMean_write(vi_pipe, i, ldci_attr->auto_attr.he_wgt[i].he_neg_wgt.mean);
        hi_ext_system_ldci_blcCtrl_write(vi_pipe, i, ldci_attr->auto_attr.blc_ctrl[i]);
    }

    if (ldci_attr->manual_attr.he_wgt.he_pos_wgt.wgt > HI_ISP_LDCI_HEPOSWGT_MAX) {
        ISP_ERR_TRACE("Invalid he_wgt.he_pos_wgt.wgt!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (ldci_attr->manual_attr.he_wgt.he_pos_wgt.sigma < 0x1) {
        ISP_ERR_TRACE("Invalid he_wgt.he_pos_wgt.sigma!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (ldci_attr->manual_attr.he_wgt.he_neg_wgt.wgt > HI_ISP_LDCI_HENEGWGT_MAX) {
        ISP_ERR_TRACE("Invalid he_wgt.he_neg_wgt.wgt!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (ldci_attr->manual_attr.he_wgt.he_neg_wgt.sigma < 0x1) {
        ISP_ERR_TRACE("Invalid he_wgt.he_neg_wgt.sigma!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (ldci_attr->manual_attr.blc_ctrl > 0x1ff) {
        ISP_ERR_TRACE("Invalid blc_ctrl!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (ldci_attr->tpr_incr_coef > 0x100) {
        ISP_ERR_TRACE("Invalid tpr_incr_coef!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (ldci_attr->tpr_decr_coef > 0x100) {
        ISP_ERR_TRACE("Invalid tpr_decr_coef!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_ldci_TprIncrCoef_write(vi_pipe, ldci_attr->tpr_incr_coef);
    hi_ext_system_ldci_TprDecrCoef_write(vi_pipe, ldci_attr->tpr_decr_coef);
    hi_ext_system_ldci_manu_hePosWgt_write(vi_pipe, ldci_attr->manual_attr.he_wgt.he_pos_wgt.wgt);
    hi_ext_system_ldci_manu_hePosSigma_write(vi_pipe, ldci_attr->manual_attr.he_wgt.he_pos_wgt.sigma);
    hi_ext_system_ldci_manu_hePosMean_write(vi_pipe, ldci_attr->manual_attr.he_wgt.he_pos_wgt.mean);
    hi_ext_system_ldci_manu_heNegWgt_write(vi_pipe, ldci_attr->manual_attr.he_wgt.he_neg_wgt.wgt);
    hi_ext_system_ldci_manu_heNegSigma_write(vi_pipe, ldci_attr->manual_attr.he_wgt.he_neg_wgt.sigma);
    hi_ext_system_ldci_manu_heNegMean_write(vi_pipe, ldci_attr->manual_attr.he_wgt.he_neg_wgt.mean);
    hi_ext_system_ldci_manu_blcCtrl_write(vi_pipe, ldci_attr->manual_attr.blc_ctrl);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_ldci_attr(VI_PIPE vi_pipe, hi_isp_ldci_attr *ldci_attr)
{
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(ldci_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ldci_attr->enable          = hi_ext_system_ldci_enable_read(vi_pipe);
    ldci_attr->gauss_lpf_sigma = hi_ext_system_ldci_gaussLPFSigma_read(vi_pipe);
    ldci_attr->op_type         = (hi_isp_op_type)hi_ext_system_ldci_manu_mode_read(vi_pipe);
    ldci_attr->tpr_incr_coef   = hi_ext_system_ldci_TprIncrCoef_read(vi_pipe);
    ldci_attr->tpr_decr_coef   = hi_ext_system_ldci_TprDecrCoef_read(vi_pipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        ldci_attr->auto_attr.he_wgt[i].he_pos_wgt.wgt   = hi_ext_system_ldci_hePosWgt_read(vi_pipe, i);
        ldci_attr->auto_attr.he_wgt[i].he_pos_wgt.sigma = hi_ext_system_ldci_hePosSigma_read(vi_pipe, i);
        ldci_attr->auto_attr.he_wgt[i].he_pos_wgt.mean  = hi_ext_system_ldci_hePosMean_read(vi_pipe, i);
        ldci_attr->auto_attr.he_wgt[i].he_neg_wgt.wgt   = hi_ext_system_ldci_heNegWgt_read(vi_pipe, i);
        ldci_attr->auto_attr.he_wgt[i].he_neg_wgt.sigma = hi_ext_system_ldci_heNegSigma_read(vi_pipe, i);
        ldci_attr->auto_attr.he_wgt[i].he_neg_wgt.mean  = hi_ext_system_ldci_heNegMean_read(vi_pipe, i);
        ldci_attr->auto_attr.blc_ctrl[i] = hi_ext_system_ldci_blcCtrl_read(vi_pipe, i);
    }

    ldci_attr->manual_attr.he_wgt.he_pos_wgt.wgt   = hi_ext_system_ldci_manu_hePosWgt_read(vi_pipe);
    ldci_attr->manual_attr.he_wgt.he_pos_wgt.sigma = hi_ext_system_ldci_manu_hePosSigma_read(vi_pipe);
    ldci_attr->manual_attr.he_wgt.he_pos_wgt.mean  = hi_ext_system_ldci_manu_hePosMean_read(vi_pipe);
    ldci_attr->manual_attr.he_wgt.he_neg_wgt.wgt   = hi_ext_system_ldci_manu_heNegWgt_read(vi_pipe);
    ldci_attr->manual_attr.he_wgt.he_neg_wgt.sigma = hi_ext_system_ldci_manu_heNegSigma_read(vi_pipe);
    ldci_attr->manual_attr.he_wgt.he_neg_wgt.mean  = hi_ext_system_ldci_manu_heNegMean_read(vi_pipe);
    ldci_attr->manual_attr.blc_ctrl = hi_ext_system_ldci_manu_blcCtrl_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_drc_attr(VI_PIPE vi_pipe, const hi_isp_drc_attr *drc)
{
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(drc);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ISP_CHECK_BOOL(drc->enable);
    if (drc->dp_detect_range_ratio > 0x1F) {
        ISP_ERR_TRACE("Invalid dp_detect_range_ratio!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->dp_detect_thr_slo > 0x1F) {
        ISP_ERR_TRACE("Invalid dp_detect_thr_slo!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((drc->asymmetry_curve.asymmetry > 0x1E) || (drc->asymmetry_curve.asymmetry < 0x1)) {
        ISP_ERR_TRACE("Invalid asymmetry!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((drc->asymmetry_curve.second_pole > 0xD2) || (drc->asymmetry_curve.second_pole < 0x96)) {
        ISP_ERR_TRACE("Invalid second_pole!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((drc->asymmetry_curve.stretch > 0x3C) || (drc->asymmetry_curve.stretch < 0x1E)) {
        ISP_ERR_TRACE("Invalid stretch!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((drc->asymmetry_curve.compress > 0xc8) || (drc->asymmetry_curve.compress < 0x64)) {
        ISP_ERR_TRACE("Invalid compress!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((drc->curve_select >= DRC_CURVE_BUTT)) {
        ISP_ERR_TRACE("Invalid curve_select!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid op_type!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->auto_attr.strength > HI_ISP_DRC_STRENGTH_MAX) {
        ISP_ERR_TRACE("Invalid auto_attr.strength!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->auto_attr.strength_max > HI_ISP_DRC_STRENGTH_MAX) {
        ISP_ERR_TRACE("Invalid auto_attr.strength_max!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->auto_attr.strength_min > HI_ISP_DRC_STRENGTH_MAX) {
        ISP_ERR_TRACE("Invalid auto_attr.strength_min!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->auto_attr.strength_max < drc->auto_attr.strength_min) {
        ISP_ERR_TRACE("Invalid auto_attr.strength_min and auto_attr.strength_max!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->manual_attr.strength > HI_ISP_DRC_STRENGTH_MAX) {
        ISP_ERR_TRACE("Invalid manual_attr.strength!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->local_mixing_dark_max > 0x80) {
        ISP_ERR_TRACE("Invalid local_mixing_dark_max!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->local_mixing_dark_min > 0x40) {
        ISP_ERR_TRACE("Invalid local_mixing_dark_min!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->local_mixing_dark_slo > 7 || drc->local_mixing_dark_slo < -7) {
        ISP_ERR_TRACE("Invalid local_mixing_dark_slo!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->local_mixing_bright_max > 0x80) {
        ISP_ERR_TRACE("Invalid local_mixing_bright_max!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->local_mixing_bright_min > 0x40) {
        ISP_ERR_TRACE("Invalid local_mixing_bright_min!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->local_mixing_bright_slo > 7 || drc->local_mixing_bright_slo < -7) {
        ISP_ERR_TRACE("Invalid local_mixing_bright_slo!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->bright_gain_lmt > 0xF) {
        ISP_ERR_TRACE("Invalid bright_gain_lmt!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->bright_gain_lmt_step > 0xF) {
        ISP_ERR_TRACE("Invalid bright_gain_lmt_step!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (drc->dark_gain_lmt_y > 0x85) {
        ISP_ERR_TRACE("Invalid dark_gain_lmt_y!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->dark_gain_lmt_c > 0x85) {
        ISP_ERR_TRACE("Invalid dark_gain_lmt_c!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->pd_strength > 0x80) {
        ISP_ERR_TRACE("Invalid pd_strength!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->detail_dark_str > 0x80) {
        ISP_ERR_TRACE("Invalid detail_dark_str!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->detail_bright_str > 0x80) {
        ISP_ERR_TRACE("Invalid detail_bright_str!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->detail_dark_step > 0x80) {
        ISP_ERR_TRACE("Invalid detail_dark_step!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->detail_bright_step > 0x80) {
        ISP_ERR_TRACE("Invalid detail_bright_step!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->flt_scale_coarse > 0xf) {
        ISP_ERR_TRACE("Invalid flt_scale_coarse!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->flt_scale_fine > 0xf) {
        ISP_ERR_TRACE("Invalid flt_scale_fine!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->contrast_control > 0xF) {
        ISP_ERR_TRACE("Invalid contrast_control!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->spatial_flt_coef > HI_ISP_DRC_SPA_FLT_COEF_MAX) {
        ISP_ERR_TRACE("Invalid spatial_filter_coef!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->range_flt_coef > 0xA) {
        ISP_ERR_TRACE("Invalid range_flt_coef!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->range_ada_max > 0x8) {
        ISP_ERR_TRACE("Invalid range_ada_max!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (drc->grad_rev_max > 0x40) {
        ISP_ERR_TRACE("Invalid grad_rev_max!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->grad_rev_thr > 0x80) {
        ISP_ERR_TRACE("Invalid grad_rev_thr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (drc->detail_adjust_factor > 15 || drc->detail_adjust_factor < -15) {
        ISP_ERR_TRACE("Invalid detail_adjust_factor!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_drc_manual_mode_write(vi_pipe, drc->op_type);
    hi_ext_system_drc_auto_strength_write(vi_pipe, drc->auto_attr.strength);
    hi_ext_system_drc_auto_strength_max_write(vi_pipe, drc->auto_attr.strength_max);
    hi_ext_system_drc_auto_strength_min_write(vi_pipe, drc->auto_attr.strength_min);
    hi_ext_system_drc_manual_strength_write(vi_pipe, drc->manual_attr.strength);

    hi_ext_system_drc_enable_write(vi_pipe, drc->enable);
    hi_ext_system_drc_asymmetry_write(vi_pipe, drc->asymmetry_curve.asymmetry);
    hi_ext_system_drc_secondpole_write(vi_pipe, drc->asymmetry_curve.second_pole);
    hi_ext_system_drc_stretch_write(vi_pipe, drc->asymmetry_curve.stretch);
    hi_ext_system_drc_compress_write(vi_pipe, drc->asymmetry_curve.compress);

    hi_ext_system_drc_mixing_dark_max_write(vi_pipe, drc->local_mixing_dark_max);
    hi_ext_system_drc_mixing_dark_min_write(vi_pipe, drc->local_mixing_dark_min);
    hi_ext_system_drc_mixing_dark_thr_write(vi_pipe, drc->local_mixing_dark_thr);
    hi_ext_system_drc_mixing_dark_slo_write(vi_pipe, drc->local_mixing_dark_slo);
    hi_ext_system_drc_mixing_bright_max_write(vi_pipe, drc->local_mixing_bright_max);
    hi_ext_system_drc_mixing_bright_min_write(vi_pipe, drc->local_mixing_bright_min);
    hi_ext_system_drc_mixing_bright_thr_write(vi_pipe, drc->local_mixing_bright_thr);
    hi_ext_system_drc_mixing_bright_slo_write(vi_pipe, drc->local_mixing_bright_slo);

    hi_ext_system_drc_gain_clip_knee_write(vi_pipe, drc->bright_gain_lmt);
    hi_ext_system_drc_gain_clip_step_write(vi_pipe, drc->bright_gain_lmt_step);
    hi_ext_system_drc_dark_gain_lmt_y_write(vi_pipe, drc->dark_gain_lmt_y);
    hi_ext_system_drc_dark_gain_lmt_c_write(vi_pipe, drc->dark_gain_lmt_c);
    hi_ext_system_drc_purple_high_thr_write(vi_pipe, drc->pd_strength);

    // shp_exp and shp_log are both bound to contrast_control
    hi_ext_system_drc_shp_log_write(vi_pipe, drc->contrast_control);
    hi_ext_system_drc_shp_exp_write(vi_pipe, drc->contrast_control);

    hi_ext_system_drc_flt_scale_coarse_write(vi_pipe, drc->flt_scale_coarse);
    hi_ext_system_drc_flt_scale_fine_write(vi_pipe, drc->flt_scale_fine);

    hi_ext_system_drc_flt_rng_ada_max_write(vi_pipe, drc->range_ada_max);

    hi_ext_system_drc_flt_spa_fine_write(vi_pipe, drc->spatial_flt_coef);
    hi_ext_system_drc_flt_rng_fine_write(vi_pipe, drc->range_flt_coef);
    hi_ext_system_drc_grad_max_write(vi_pipe, drc->grad_rev_max);
    hi_ext_system_drc_grad_thr_write(vi_pipe, drc->grad_rev_thr);
    hi_ext_system_drc_detail_sub_factor_write(vi_pipe, drc->detail_adjust_factor);

    hi_ext_system_drc_detail_dark_max_write(vi_pipe, drc->detail_dark_str);
    hi_ext_system_drc_detail_bright_min_write(vi_pipe, drc->detail_bright_str);
    hi_ext_system_drc_detail_darkstep_write(vi_pipe, drc->detail_dark_step);
    hi_ext_system_drc_detail_brightstep_write(vi_pipe, drc->detail_bright_step);

    for (i = 0; i < HI_ISP_DRC_TM_NODE_NUM; i++) {
        hi_ext_system_drc_tm_lut_write(vi_pipe, i, drc->tone_mapping_value[i]);
    }

    for (i = 0; i < HI_ISP_DRC_CC_NODE_NUM; i++) {
        if ((drc->color_correction_lut[i] > 0x400)) {
            ISP_ERR_TRACE("Invalid color_correction_lut!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_drc_colorcc_lut_write(vi_pipe, i, drc->color_correction_lut[i]);
    }

    hi_ext_system_drc_curve_sel_write(vi_pipe, drc->curve_select);

    for (i = 0; i < HI_ISP_DRC_CUBIC_POINT_NUM; i++) {
        if ((drc->cubic_point[i].x > 0x3E8)) {
            ISP_ERR_TRACE("Invalid cubic_point[%d].x!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if ((drc->cubic_point[i].y > 0x3E8)) {
            ISP_ERR_TRACE("Invalid cubic_point[%d].y!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if ((drc->cubic_point[i].slope > 0x2710)) {
            ISP_ERR_TRACE("Invalid cubic_point[%d].slope!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    hi_ext_system_drc_cubic_ltmx0_write(vi_pipe, drc->cubic_point[0].x);
    hi_ext_system_drc_cubic_ltmy0_write(vi_pipe, drc->cubic_point[0].y);
    hi_ext_system_drc_cubic_slp0_write(vi_pipe, drc->cubic_point[0].slope);

    hi_ext_system_drc_cubic_ltmx1_write(vi_pipe, drc->cubic_point[1].x);
    hi_ext_system_drc_cubic_ltmy1_write(vi_pipe, drc->cubic_point[1].y);
    hi_ext_system_drc_cubic_slp1_write(vi_pipe, drc->cubic_point[1].slope);

    hi_ext_system_drc_cubic_ltmx2_write(vi_pipe, drc->cubic_point[2].x);
    hi_ext_system_drc_cubic_ltmy2_write(vi_pipe, drc->cubic_point[2].y);
    hi_ext_system_drc_cubic_slp2_write(vi_pipe, drc->cubic_point[2].slope);

    hi_ext_system_drc_cubic_ltmx3_write(vi_pipe, drc->cubic_point[3].x);
    hi_ext_system_drc_cubic_ltmy3_write(vi_pipe, drc->cubic_point[3].y);
    hi_ext_system_drc_cubic_slp3_write(vi_pipe, drc->cubic_point[3].slope);

    hi_ext_system_drc_cubic_ltmx4_write(vi_pipe, drc->cubic_point[4].x);
    hi_ext_system_drc_cubic_ltmy4_write(vi_pipe, drc->cubic_point[4].y);
    hi_ext_system_drc_cubic_slp4_write(vi_pipe, drc->cubic_point[4].slope);

    hi_ext_system_drc_coef_update_en_write(vi_pipe, HI_TRUE);
    hi_ext_system_drc_manual_mode_write(vi_pipe, drc->op_type);

    hi_ext_system_drc_dp_detect_rng_ratio_write(vi_pipe, drc->dp_detect_range_ratio);
    hi_ext_system_drc_dp_detect_thr_slo_write(vi_pipe, drc->dp_detect_thr_slo);
    hi_ext_system_drc_dp_detect_thr_min_write(vi_pipe, drc->dp_detect_thr_min);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_drc_attr(VI_PIPE vi_pipe, hi_isp_drc_attr *drc)
{
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(drc);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    drc->op_type                = (hi_isp_op_type)hi_ext_system_drc_manual_mode_read(vi_pipe);
    drc->auto_attr.strength     = hi_ext_system_drc_auto_strength_read(vi_pipe);
    drc->auto_attr.strength_max = hi_ext_system_drc_auto_strength_max_read(vi_pipe);
    drc->auto_attr.strength_min = hi_ext_system_drc_auto_strength_min_read(vi_pipe);
    drc->manual_attr.strength   = hi_ext_system_drc_manual_strength_read(vi_pipe);
    drc->enable                 = hi_ext_system_drc_enable_read(vi_pipe);

    drc->asymmetry_curve.asymmetry  = hi_ext_system_drc_asymmetry_read(vi_pipe);
    drc->asymmetry_curve.second_pole = hi_ext_system_drc_secondpole_read(vi_pipe);
    drc->asymmetry_curve.stretch    = hi_ext_system_drc_stretch_read(vi_pipe);
    drc->asymmetry_curve.compress   = hi_ext_system_drc_compress_read(vi_pipe);

    drc->local_mixing_dark_max   = hi_ext_system_drc_mixing_dark_max_read(vi_pipe);
    drc->local_mixing_dark_min   = hi_ext_system_drc_mixing_dark_min_read(vi_pipe);
    drc->local_mixing_dark_thr   = hi_ext_system_drc_mixing_dark_thr_read(vi_pipe);
    drc->local_mixing_dark_slo   = hi_ext_system_drc_mixing_dark_slo_read(vi_pipe);
    drc->local_mixing_bright_max = hi_ext_system_drc_mixing_bright_max_read(vi_pipe);
    drc->local_mixing_bright_min = hi_ext_system_drc_mixing_bright_min_read(vi_pipe);
    drc->local_mixing_bright_thr = hi_ext_system_drc_mixing_bright_thr_read(vi_pipe);
    drc->local_mixing_bright_slo = hi_ext_system_drc_mixing_bright_slo_read(vi_pipe);

    drc->bright_gain_lmt      = hi_ext_system_drc_gain_clip_knee_read(vi_pipe);
    drc->bright_gain_lmt_step = hi_ext_system_drc_gain_clip_step_read(vi_pipe);
    drc->dark_gain_lmt_y      = hi_ext_system_drc_dark_gain_lmt_y_read(vi_pipe);
    drc->dark_gain_lmt_c      = hi_ext_system_drc_dark_gain_lmt_c_read(vi_pipe);
    drc->pd_strength          = hi_ext_system_drc_purple_high_thr_read(vi_pipe);

    drc->flt_scale_coarse = hi_ext_system_drc_flt_scale_coarse_read(vi_pipe);
    drc->flt_scale_fine   = hi_ext_system_drc_flt_scale_fine_read(vi_pipe);

    drc->contrast_control     = hi_ext_system_drc_shp_log_read(vi_pipe);
    drc->detail_adjust_factor = hi_ext_system_drc_detail_sub_factor_read(vi_pipe);

    drc->spatial_flt_coef = hi_ext_system_drc_flt_spa_fine_read(vi_pipe);
    drc->range_flt_coef   = hi_ext_system_drc_flt_rng_fine_read(vi_pipe);
    drc->range_ada_max    = hi_ext_system_drc_flt_rng_ada_max_read(vi_pipe);

    drc->grad_rev_max = hi_ext_system_drc_grad_max_read(vi_pipe);
    drc->grad_rev_thr = hi_ext_system_drc_grad_thr_read(vi_pipe);

    drc->detail_dark_str    = hi_ext_system_drc_detail_dark_max_read(vi_pipe);
    drc->detail_bright_str  = hi_ext_system_drc_detail_bright_min_read(vi_pipe);
    drc->detail_dark_step   = hi_ext_system_drc_detail_darkstep_read(vi_pipe);
    drc->detail_bright_step = hi_ext_system_drc_detail_brightstep_read(vi_pipe);

    for (i = 0; i < HI_ISP_DRC_CC_NODE_NUM; i++) {
        drc->color_correction_lut[i] = hi_ext_system_drc_colorcc_lut_read(vi_pipe, i);
    }

    for (i = 0; i < HI_ISP_DRC_TM_NODE_NUM; i++) {
        drc->tone_mapping_value[i] = hi_ext_system_drc_tm_lut_read(vi_pipe, i);
    }

    drc->curve_select = hi_ext_system_drc_curve_sel_read(vi_pipe);

    drc->cubic_point[0].x = hi_ext_system_drc_cubic_ltmx0_read(vi_pipe);
    drc->cubic_point[1].x = hi_ext_system_drc_cubic_ltmx1_read(vi_pipe);
    drc->cubic_point[2].x = hi_ext_system_drc_cubic_ltmx2_read(vi_pipe);
    drc->cubic_point[3].x = hi_ext_system_drc_cubic_ltmx3_read(vi_pipe);
    drc->cubic_point[4].x = hi_ext_system_drc_cubic_ltmx4_read(vi_pipe);

    drc->cubic_point[0].y = hi_ext_system_drc_cubic_ltmy0_read(vi_pipe);
    drc->cubic_point[1].y = hi_ext_system_drc_cubic_ltmy1_read(vi_pipe);
    drc->cubic_point[2].y = hi_ext_system_drc_cubic_ltmy2_read(vi_pipe);
    drc->cubic_point[3].y = hi_ext_system_drc_cubic_ltmy3_read(vi_pipe);
    drc->cubic_point[4].y = hi_ext_system_drc_cubic_ltmy4_read(vi_pipe);

    drc->cubic_point[0].slope = hi_ext_system_drc_cubic_slp0_read(vi_pipe);
    drc->cubic_point[1].slope = hi_ext_system_drc_cubic_slp1_read(vi_pipe);
    drc->cubic_point[2].slope = hi_ext_system_drc_cubic_slp2_read(vi_pipe);
    drc->cubic_point[3].slope = hi_ext_system_drc_cubic_slp3_read(vi_pipe);
    drc->cubic_point[4].slope = hi_ext_system_drc_cubic_slp4_read(vi_pipe);

    drc->dp_detect_range_ratio = hi_ext_system_drc_dp_detect_rng_ratio_read(vi_pipe);
    drc->dp_detect_thr_slo     = hi_ext_system_drc_dp_detect_thr_slo_read(vi_pipe);
    drc->dp_detect_thr_min     = hi_ext_system_drc_dp_detect_thr_min_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_dehaze_attr(VI_PIPE vi_pipe, const hi_isp_dehaze_attr *dehaze_attr)
{
    hi_s32 i;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dehaze_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ISP_CHECK_BOOL(dehaze_attr->enable);
    ISP_CHECK_BOOL(dehaze_attr->user_lut_enable);

    if (dehaze_attr->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid op_type!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (dehaze_attr->tmprflt_decr_coef > 0x80) {
        ISP_ERR_TRACE("Invalid tmprflt_decr_coef\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (dehaze_attr->tmprflt_incr_coef > 0x80) {
        ISP_ERR_TRACE("Invalid tmprflt_incr_coef\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_user_dehaze_lut_enable_write(vi_pipe, dehaze_attr->user_lut_enable);

    for (i = 0; i < 256; i++) {
        hi_ext_system_dehaze_lut_write(vi_pipe, i, dehaze_attr->dehaze_lut[i]);
    }

    if (dehaze_attr->user_lut_enable) {
        // 1:update the defog lut,FW will change it to 0 when the lut updating is finished.
        hi_ext_system_user_dehaze_lut_update_write(vi_pipe, HI_TRUE);
    }

    hi_ext_system_dehaze_enable_write(vi_pipe, dehaze_attr->enable);
    hi_ext_system_dehaze_manu_mode_write(vi_pipe, dehaze_attr->op_type);
    hi_ext_system_manual_dehaze_strength_write(vi_pipe, dehaze_attr->manual_attr.strength);
    hi_ext_system_manual_dehaze_autostrength_write(vi_pipe, dehaze_attr->auto_attr.strength);
    hi_ext_system_dehaze_tfic_write(vi_pipe, dehaze_attr->tmprflt_incr_coef);
    hi_ext_system_dehaze_tfdc_write(vi_pipe, dehaze_attr->tmprflt_decr_coef);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_dehaze_attr(VI_PIPE vi_pipe, hi_isp_dehaze_attr *dehaze_attr)
{
    hi_s32 i;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dehaze_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    dehaze_attr->enable               = hi_ext_system_dehaze_enable_read(vi_pipe);
    dehaze_attr->op_type              = (hi_isp_op_type)hi_ext_system_dehaze_manu_mode_read(vi_pipe);
    dehaze_attr->manual_attr.strength = hi_ext_system_manual_dehaze_strength_read(vi_pipe);
    dehaze_attr->auto_attr.strength   = hi_ext_system_manual_dehaze_autostrength_read(vi_pipe);
    dehaze_attr->user_lut_enable      = hi_ext_system_user_dehaze_lut_enable_read(vi_pipe);

    for (i = 0; i < 256; i++) {
        dehaze_attr->dehaze_lut[i] = hi_ext_system_dehaze_lut_read(vi_pipe, i);
    }

    dehaze_attr->tmprflt_incr_coef = hi_ext_system_dehaze_tfic_read(vi_pipe);
    dehaze_attr->tmprflt_decr_coef = hi_ext_system_dehaze_tfdc_read(vi_pipe);

    return HI_SUCCESS;
}

extern hi_s32 mpi_vi_set_isp_dis_attr(VI_PIPE vi_pipe, hi_bool dis_enable);
extern hi_s32 mpi_vi_get_isp_dis_attr(VI_PIPE vi_pipe, hi_bool *dis_enable);

MPI_STATIC hi_s32 hi_mpi_isp_set_dis_attr(VI_PIPE vi_pipe, const hi_isp_dis_attr *dis_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dis_attr);
    ISP_CHECK_BOOL(dis_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    return mpi_vi_set_isp_dis_attr(vi_pipe, dis_attr->enable);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_dis_attr(VI_PIPE vi_pipe, hi_isp_dis_attr *dis_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dis_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    return mpi_vi_get_isp_dis_attr(vi_pipe, &dis_attr->enable);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_fswdr_attr(VI_PIPE vi_pipe, const hi_isp_wdr_fs_attr *fswdr_attr)
{
    hi_u8 j;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(fswdr_attr);
    ISP_CHECK_BOOL(fswdr_attr->wdr_combine.motion_comp);
    ISP_CHECK_BOOL(fswdr_attr->wdr_combine.wdr_mdt.md_ref_flicker);
    ISP_CHECK_BOOL(fswdr_attr->wdr_combine.force_long);
    ISP_CHECK_BOOL(fswdr_attr->bnr_attr.short_frame_nr);
    ISP_CHECK_BOOL(fswdr_attr->wdr_combine.wdr_mdt.short_expo_chk);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (fswdr_attr->wdr_merge_mode >= MERGE_BUTT) {
        ISP_ERR_TRACE("Invalid wdr_merge_mode !\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->bnr_attr.bnr_mode >= BNR_BUTT) {
        ISP_ERR_TRACE("Invalid bnr_mode !\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->wdr_combine.short_thr > 0xFFF) {
        ISP_ERR_TRACE("Invalid short_thr !\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (fswdr_attr->wdr_combine.long_thr > 0xFFF) {
        ISP_ERR_TRACE("Invalid long_thr !\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (fswdr_attr->wdr_combine.long_thr > fswdr_attr->wdr_combine.short_thr) {
        ISP_ERR_TRACE("long_thresh should NOT be larger than short_thresh !\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->wdr_combine.wdr_mdt.op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid op_type!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->wdr_combine.wdr_mdt.manual_attr.md_thr_low_gain >
        fswdr_attr->wdr_combine.wdr_mdt.manual_attr.md_thr_hig_gain) {
        ISP_ERR_TRACE("md_thr_low_gain should NOT be larger than md_thr_hig_gain!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (fswdr_attr->wdr_combine.force_long_hig_thr > 0XFFF) {
        ISP_ERR_TRACE("Invalid force_long_hig_thr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->wdr_combine.force_long_low_thr > fswdr_attr->wdr_combine.force_long_hig_thr) {
        ISP_ERR_TRACE("force_long_low_thr should NOT be larger than force_long_hig_thr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->bnr_attr.short_frame_nr_str > 0X3F) {
        ISP_ERR_TRACE("Invalid mot2_sig_cwgt_high!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->bnr_attr.fusion_bnr_str > 0X3F) {
        ISP_ERR_TRACE("Invalid mot2_sig_cwgt_high!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (j = 0; j < 4; j++) {
        if (fswdr_attr->fusion_attr.fusion_thr[j] > 0x3FFF) {
            ISP_ERR_TRACE("Invalid fusion_thr!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (j = 0; j < ISP_AUTO_ISO_STRENGTH_NUM; j++) {
        if (fswdr_attr->wdr_combine.wdr_mdt.auto_attr.md_thr_low_gain[j] >
            fswdr_attr->wdr_combine.wdr_mdt.auto_attr.md_thr_hig_gain[j]) {
            ISP_ERR_TRACE("md_thr_low_gain[%d] should NOT be larger than md_thr_hig_gain[%d]!\n", j, j);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_wdr_auto_mdthr_low_gain_write(vi_pipe, j, fswdr_attr->wdr_combine.wdr_mdt.auto_attr.md_thr_low_gain[j]);
        hi_ext_system_wdr_auto_mdthr_hig_gain_write(vi_pipe, j, fswdr_attr->wdr_combine.wdr_mdt.auto_attr.md_thr_hig_gain[j]);
    }

    if (fswdr_attr->wdr_combine.wdr_mdt.mdt_full_thd > 254) {
        ISP_ERR_TRACE("Invalid mdt_full_thd!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->wdr_combine.wdr_mdt.mdt_still_thd > 254) {
        ISP_ERR_TRACE("Invalid mdt_still_thd!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->wdr_combine.wdr_mdt.mdt_long_blend > 254) {
        ISP_ERR_TRACE("Invalid mdt_long_blend!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->bnr_attr.full_mdt_sig_wgt > 31) {
        ISP_ERR_TRACE("Invalid full_mdt_sig_wgt!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->bnr_attr.full_mdt_sig_g_wgt > 31) {
        ISP_ERR_TRACE("Invalid full_mdt_sig_g_wgt!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->bnr_attr.full_mdt_sig_rb_wgt > 31) {
        ISP_ERR_TRACE("Invalid full_mdt_sig_rb_wgt!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->wdr_combine.wdr_mdt.short_check_thd > 4095) {
        ISP_ERR_TRACE("Invalid short_check_thd!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (fswdr_attr->wdr_wb_gain_position >= WDR_WBGAIN_POSITION_BUTT) {
        ISP_ERR_TRACE("Invalid wdr_wb_gain_position %d!\n", fswdr_attr->wdr_wb_gain_position);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_wdr_wbgain_position_write(vi_pipe, fswdr_attr->wdr_wb_gain_position);

    hi_ext_system_fusion_mode_write(vi_pipe, fswdr_attr->wdr_merge_mode);
    hi_ext_system_mdt_en_write(vi_pipe, fswdr_attr->wdr_combine.motion_comp);
    hi_ext_system_wdr_longthr_write(vi_pipe, fswdr_attr->wdr_combine.long_thr);
    hi_ext_system_wdr_shortthr_write(vi_pipe, fswdr_attr->wdr_combine.short_thr);

    hi_ext_system_wdr_manual_mode_write(vi_pipe, fswdr_attr->wdr_combine.wdr_mdt.op_type);
    hi_ext_system_wdr_mdref_flicker_write(vi_pipe, fswdr_attr->wdr_combine.wdr_mdt.md_ref_flicker);
    hi_ext_system_wdr_manual_mdthr_low_gain_write(vi_pipe, fswdr_attr->wdr_combine.wdr_mdt.manual_attr.md_thr_low_gain);
    hi_ext_system_wdr_manual_mdthr_hig_gain_write(vi_pipe, fswdr_attr->wdr_combine.wdr_mdt.manual_attr.md_thr_hig_gain);

    for (j = 0; j < 4; j++) {
        hi_ext_system_fusion_thr_write(vi_pipe, j, fswdr_attr->fusion_attr.fusion_thr[j]);
    }

    hi_ext_system_bnr_mode_write(vi_pipe, fswdr_attr->bnr_attr.bnr_mode);
    hi_ext_system_wdr_coef_update_en_write(vi_pipe, HI_TRUE);

    hi_ext_system_wdr_forcelong_en_write(vi_pipe, fswdr_attr->wdr_combine.force_long);
    hi_ext_system_wdr_forcelong_high_thd_write(vi_pipe, fswdr_attr->wdr_combine.force_long_hig_thr);
    hi_ext_system_wdr_forcelong_low_thd_write(vi_pipe, fswdr_attr->wdr_combine.force_long_low_thr);

    hi_ext_system_wdr_sfnr_en_write(vi_pipe, fswdr_attr->bnr_attr.short_frame_nr);
    hi_ext_system_wdr_shortframe_nrstr_write(vi_pipe, fswdr_attr->bnr_attr.short_frame_nr_str);

    hi_ext_system_wdr_fusionbnrstr_write(vi_pipe, fswdr_attr->bnr_attr.fusion_bnr_str);
    hi_ext_system_wdr_shortexpo_chk_write(vi_pipe, fswdr_attr->wdr_combine.wdr_mdt.short_expo_chk);

    hi_ext_system_wdr_shortcheck_thd_write(vi_pipe, fswdr_attr->wdr_combine.wdr_mdt.short_check_thd);
    hi_ext_system_wdr_mdt_full_thr_write(vi_pipe, fswdr_attr->wdr_combine.wdr_mdt.mdt_full_thd);
    hi_ext_system_wdr_mdt_still_thr_write(vi_pipe, fswdr_attr->wdr_combine.wdr_mdt.mdt_still_thd);
    hi_ext_system_wdr_mdt_long_blend_write(vi_pipe, fswdr_attr->wdr_combine.wdr_mdt.mdt_long_blend);

    for (j = 0; j < NoiseSet_EleNum; j++) {
        hi_ext_system_wdr_floorset_write(vi_pipe, j, fswdr_attr->bnr_attr.noise_floor[j]);
    }

    hi_ext_system_wdr_mot2sig_gwgt_high_write(vi_pipe, fswdr_attr->bnr_attr.full_mdt_sig_g_wgt);
    hi_ext_system_wdr_mot2sig_cwgt_high_write(vi_pipe, fswdr_attr->bnr_attr.full_mdt_sig_rb_wgt);
    hi_ext_system_wdr_full_mot_sigma_weight_write(vi_pipe, fswdr_attr->bnr_attr.full_mdt_sig_wgt);
    hi_ext_system_wdr_g_sigma_gain1_write(vi_pipe, fswdr_attr->bnr_attr.g_sigma_gain[0]);
    hi_ext_system_wdr_g_sigma_gain2_write(vi_pipe, fswdr_attr->bnr_attr.g_sigma_gain[1]);
    hi_ext_system_wdr_g_sigma_gain3_write(vi_pipe, fswdr_attr->bnr_attr.g_sigma_gain[2]);
    hi_ext_system_wdr_c_sigma_gain1_write(vi_pipe, fswdr_attr->bnr_attr.rb_sigma_gain[0]);
    hi_ext_system_wdr_c_sigma_gain2_write(vi_pipe, fswdr_attr->bnr_attr.rb_sigma_gain[1]);
    hi_ext_system_wdr_c_sigma_gain3_write(vi_pipe, fswdr_attr->bnr_attr.rb_sigma_gain[2]);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_fswdr_attr(VI_PIPE vi_pipe, hi_isp_wdr_fs_attr *fswdr_attr)
{
    hi_u8 j;
    hi_u8 tmp;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(fswdr_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    fswdr_attr->wdr_merge_mode          = hi_ext_system_fusion_mode_read(vi_pipe);
    fswdr_attr->wdr_combine.motion_comp = hi_ext_system_mdt_en_read(vi_pipe);
    fswdr_attr->wdr_combine.long_thr  = hi_ext_system_wdr_longthr_read(vi_pipe);
    fswdr_attr->wdr_combine.short_thr = hi_ext_system_wdr_shortthr_read(vi_pipe);

    fswdr_attr->wdr_combine.wdr_mdt.op_type        = hi_ext_system_wdr_manual_mode_read(vi_pipe);
    fswdr_attr->wdr_combine.wdr_mdt.md_ref_flicker = hi_ext_system_wdr_mdref_flicker_read(vi_pipe);

    fswdr_attr->wdr_combine.wdr_mdt.manual_attr.md_thr_low_gain = hi_ext_system_wdr_manual_mdthr_low_gain_read(vi_pipe);
    fswdr_attr->wdr_combine.wdr_mdt.manual_attr.md_thr_hig_gain = hi_ext_system_wdr_manual_mdthr_hig_gain_read(vi_pipe);

    for (j = 0; j < ISP_AUTO_ISO_STRENGTH_NUM; j++) {
        fswdr_attr->wdr_combine.wdr_mdt.auto_attr.md_thr_low_gain[j] = hi_ext_system_wdr_auto_mdthr_low_gain_read(vi_pipe, j);
        fswdr_attr->wdr_combine.wdr_mdt.auto_attr.md_thr_hig_gain[j] = hi_ext_system_wdr_auto_mdthr_hig_gain_read(vi_pipe, j);
    }
    for (j = 0; j < 4; j++) {
        fswdr_attr->fusion_attr.fusion_thr[j] = hi_ext_system_fusion_thr_read(vi_pipe, j);
    }
    /*************************WDR DGAIN CONIFG ***************************/
    tmp = hi_ext_system_wdr_wbgain_position_read(vi_pipe);
    if (tmp == HI_EXT_SYSTEM_WDR_WBGAIN_IN_DG1) {
        fswdr_attr->wdr_wb_gain_position = WDR_WBGAIN_IN_DG1;
    } else if (tmp == HI_EXT_SYSTEM_WDR_WBGAIN_IN_WB) {
        fswdr_attr->wdr_wb_gain_position = WDR_WBGAIN_IN_WB;
    } else {
        fswdr_attr->wdr_wb_gain_position = WDR_WBGAIN_POSITION_BUTT;
    }
    fswdr_attr->bnr_attr.bnr_mode              = hi_ext_system_bnr_mode_read(vi_pipe);
    fswdr_attr->wdr_combine.force_long         = hi_ext_system_wdr_forcelong_en_read(vi_pipe);
    fswdr_attr->wdr_combine.force_long_hig_thr = hi_ext_system_wdr_forcelong_high_thd_read(vi_pipe);
    fswdr_attr->wdr_combine.force_long_low_thr = hi_ext_system_wdr_forcelong_low_thd_read(vi_pipe);
    fswdr_attr->bnr_attr.short_frame_nr        = hi_ext_system_wdr_sfnr_en_read(vi_pipe);
    fswdr_attr->bnr_attr.short_frame_nr_str    = hi_ext_system_wdr_shortframe_nrstr_read(vi_pipe);

    fswdr_attr->bnr_attr.fusion_bnr_str            = hi_ext_system_wdr_fusionbnrstr_read(vi_pipe);
    fswdr_attr->wdr_combine.wdr_mdt.short_expo_chk = hi_ext_system_wdr_shortexpo_chk_read(vi_pipe);

    fswdr_attr->wdr_combine.wdr_mdt.short_check_thd = hi_ext_system_wdr_shortcheck_thd_read(vi_pipe);
    fswdr_attr->wdr_combine.wdr_mdt.mdt_full_thd    = hi_ext_system_wdr_mdt_full_thr_read(vi_pipe);
    fswdr_attr->wdr_combine.wdr_mdt.mdt_still_thd   = hi_ext_system_wdr_mdt_still_thr_read(vi_pipe);
    fswdr_attr->wdr_combine.wdr_mdt.mdt_long_blend  = hi_ext_system_wdr_mdt_long_blend_read(vi_pipe);

    for (j = 0; j < NoiseSet_EleNum; j++) {
        fswdr_attr->bnr_attr.noise_floor[j] = hi_ext_system_wdr_floorset_read(vi_pipe, j);
    }

    fswdr_attr->bnr_attr.full_mdt_sig_g_wgt  = hi_ext_system_wdr_mot2sig_gwgt_high_read(vi_pipe);
    fswdr_attr->bnr_attr.full_mdt_sig_rb_wgt = hi_ext_system_wdr_mot2sig_cwgt_high_read(vi_pipe);
    fswdr_attr->bnr_attr.full_mdt_sig_wgt   = hi_ext_system_wdr_full_mot_sigma_weight_read(vi_pipe);
    fswdr_attr->bnr_attr.g_sigma_gain[0]  = hi_ext_system_wdr_g_sigma_gain1_read(vi_pipe);
    fswdr_attr->bnr_attr.g_sigma_gain[1]  = hi_ext_system_wdr_g_sigma_gain2_read(vi_pipe);
    fswdr_attr->bnr_attr.g_sigma_gain[2]  = hi_ext_system_wdr_g_sigma_gain3_read(vi_pipe);
    fswdr_attr->bnr_attr.rb_sigma_gain[0] = hi_ext_system_wdr_c_sigma_gain1_read(vi_pipe);
    fswdr_attr->bnr_attr.rb_sigma_gain[1] = hi_ext_system_wdr_c_sigma_gain2_read(vi_pipe);
    fswdr_attr->bnr_attr.rb_sigma_gain[2] = hi_ext_system_wdr_c_sigma_gain3_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_focus_statistics(VI_PIPE vi_pipe, hi_isp_af_statistics *af_stat)
{
#ifdef CONFIG_HI_ISP_AF_SUPPORT
    hi_u8 col, row, wdr_chn;
    hi_s32 i, j;
    vi_pipe_wdr_attr wdr_attr;

    hi_isp_statistics_ctrl stat_key;
    isp_stat_info act_stat_info;
    isp_stat *isp_act_stat = HI_NULL;
    hi_s32 ret;
    hi_u32 key_lowbit, key_highbit;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(af_stat);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    key_lowbit  = hi_ext_system_statistics_ctrl_lowbit_read(vi_pipe);
    key_highbit = hi_ext_system_statistics_ctrl_highbit_read(vi_pipe);
    stat_key.key = ((hi_u64)key_highbit << 32) + key_lowbit;

    col = MIN2(hi_ext_af_window_hnum_read(vi_pipe), AF_ZONE_COLUMN);
    row = MIN2(hi_ext_af_window_vnum_read(vi_pipe), AF_ZONE_ROW);

    ret = ioctl(g_as32IspFd[vi_pipe], ISP_STAT_ACT_GET, &act_stat_info);
    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("get active stat buffer err\n");
        return HI_ERR_ISP_NO_INT;
    }

    act_stat_info.virt_addr = HI_MPI_SYS_MmapCache(act_stat_info.phy_addr, sizeof(isp_stat));
    if (act_stat_info.virt_addr == HI_NULL) {
        return HI_ERR_ISP_NULL_PTR;
    }

    isp_act_stat = (isp_stat *)act_stat_info.virt_addr;

    ret = ioctl(g_as32IspFd[vi_pipe], ISP_GET_WDR_ATTR, &wdr_attr);
    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get WDR attr failed\n", vi_pipe);
        HI_MPI_SYS_Munmap(act_stat_info.virt_addr, sizeof(isp_stat));
        return ret;
    }

    wdr_chn = MIN2(wdr_attr.dev_bind_pipe.num, WDR_CHN_MAX);

    /* AF FE stat */
    if (stat_key.bit1_fe_af_stat && wdr_attr.mast_pipe) {
        isp_get_fe_focus_statistics(vi_pipe, &af_stat->fe_af_stat, isp_act_stat, wdr_chn);
    }

    /* BE */
    if (stat_key.bit1_be_af_stat) {
        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                af_stat->be_af_stat.zone_metrics[i][j].v1 = isp_act_stat->stBEAfStat.stZoneMetrics[i][j].u16v1;
                af_stat->be_af_stat.zone_metrics[i][j].h1 = isp_act_stat->stBEAfStat.stZoneMetrics[i][j].u16h1;
                af_stat->be_af_stat.zone_metrics[i][j].v2 = isp_act_stat->stBEAfStat.stZoneMetrics[i][j].u16v2;
                af_stat->be_af_stat.zone_metrics[i][j].h2 = isp_act_stat->stBEAfStat.stZoneMetrics[i][j].u16h2;
                af_stat->be_af_stat.zone_metrics[i][j].y  = isp_act_stat->stBEAfStat.stZoneMetrics[i][j].u16y;
                af_stat->be_af_stat.zone_metrics[i][j].hl_cnt = isp_act_stat->stBEAfStat.stZoneMetrics[i][j].u16HlCnt;
            }
        }
    }

    HI_MPI_SYS_Munmap((hi_void *)act_stat_info.virt_addr, sizeof(isp_stat));

    isp_get_af_grid_info(vi_pipe, &af_stat->fe_af_grid_info, &af_stat->be_af_grid_info);

    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_get_ae_statistics(VI_PIPE vi_pipe, hi_isp_ae_statistics *ae_stat)
{
    hi_s32 i, j, k;
    hi_u32 pipe_num;
    hi_s32 ret;
    hi_u32 key_lowbit, key_highbit;

    vi_pipe_wdr_attr wdr_attr;
    hi_isp_statistics_ctrl stat_key;
    isp_stat_info stat_info;
    isp_stat *isp_act_stat;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(ae_stat);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ret = ioctl(g_as32IspFd[vi_pipe], ISP_STAT_ACT_GET, &stat_info);
    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("get active stat buffer err\n");
        return HI_ERR_ISP_NOMEM;
    }

    stat_info.virt_addr = HI_MPI_SYS_MmapCache(stat_info.phy_addr, sizeof(isp_stat));
    if (stat_info.virt_addr == HI_NULL) {
        return HI_ERR_ISP_NULL_PTR;
    }
    isp_act_stat = (isp_stat *)stat_info.virt_addr;

    ret = ioctl(g_as32IspFd[vi_pipe], ISP_GET_WDR_ATTR, &wdr_attr);
    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get WDR attr failed\n", vi_pipe);
        HI_MPI_SYS_Munmap(stat_info.virt_addr, sizeof(isp_stat));
        return ret;
    }

    key_lowbit  = hi_ext_system_statistics_ctrl_lowbit_read(vi_pipe);
    key_highbit = hi_ext_system_statistics_ctrl_highbit_read(vi_pipe);
    stat_key.key = ((hi_u64)key_highbit << 32) + key_lowbit;

    /* AE FE stat */
    pipe_num = MIN2(wdr_attr.dev_bind_pipe.num, ISP_CHN_MAX_NUM);
    k = 0;
    if (stat_key.bit1_fe_ae_glo_stat && wdr_attr.mast_pipe) {
        for (; k < pipe_num; k++) {
            for (i = 0; i < HIST_NUM; i++) {
                ae_stat->fe_hist1024_value[k][i] = isp_act_stat->stFEAeStat1.au32HistogramMemArray[k][i];
            }

            ae_stat->fe_global_avg[k][0] = isp_act_stat->stFEAeStat2.u16GlobalAvgR[k];
            ae_stat->fe_global_avg[k][1] = isp_act_stat->stFEAeStat2.u16GlobalAvgGr[k];
            ae_stat->fe_global_avg[k][2] = isp_act_stat->stFEAeStat2.u16GlobalAvgGb[k];
            ae_stat->fe_global_avg[k][3] = isp_act_stat->stFEAeStat2.u16GlobalAvgB[k];
        }

        for (; k < ISP_CHN_MAX_NUM; k++) {
            for (i = 0; i < HIST_NUM; i++) {
                ae_stat->fe_hist1024_value[k][i] = 0;
            }

            ae_stat->fe_global_avg[k][0] = 0;
            ae_stat->fe_global_avg[k][1] = 0;
            ae_stat->fe_global_avg[k][2] = 0;
            ae_stat->fe_global_avg[k][3] = 0;
        }
    }

    k = 0;
    if (stat_key.bit1_fe_ae_loc_stat && wdr_attr.mast_pipe) {
        for (; k < pipe_num; k++) {
            for (i = 0; i < AE_ZONE_ROW; i++) {
                for (j = 0; j < AE_ZONE_COLUMN; j++) {
                    ae_stat->fe_zone_avg[k][i][j][0] = isp_act_stat->stFEAeStat3.au16ZoneAvg[k][i][j][0]; /* R */
                    ae_stat->fe_zone_avg[k][i][j][1] = isp_act_stat->stFEAeStat3.au16ZoneAvg[k][i][j][1]; /* Gr */
                    ae_stat->fe_zone_avg[k][i][j][2] = isp_act_stat->stFEAeStat3.au16ZoneAvg[k][i][j][2]; /* Gb */
                    ae_stat->fe_zone_avg[k][i][j][3] = isp_act_stat->stFEAeStat3.au16ZoneAvg[k][i][j][3]; /* B */
                }
            }
        }

        for (; k < ISP_CHN_MAX_NUM; k++) {
            for (i = 0; i < AE_ZONE_ROW; i++) {
                for (j = 0; j < AE_ZONE_COLUMN; j++) {
                    ae_stat->fe_zone_avg[k][i][j][0] = 0; /* R */
                    ae_stat->fe_zone_avg[k][i][j][1] = 0; /* Gr */
                    ae_stat->fe_zone_avg[k][i][j][2] = 0; /* Gb */
                    ae_stat->fe_zone_avg[k][i][j][3] = 0; /* B */
                }
            }
        }
    }

    /* AE BE stat */
    if (stat_key.bit1_be_ae_glo_stat) {
        for (i = 0; i < HIST_NUM; i++) {
            ae_stat->be_hist1024_value[i] = isp_act_stat->stBEAeStat1.au32HistogramMemArray[i];
        }

        ae_stat->be_global_avg[0] = isp_act_stat->stBEAeStat2.u16GlobalAvgR;
        ae_stat->be_global_avg[1] = isp_act_stat->stBEAeStat2.u16GlobalAvgGr;
        ae_stat->be_global_avg[2] = isp_act_stat->stBEAeStat2.u16GlobalAvgGb;
        ae_stat->be_global_avg[3] = isp_act_stat->stBEAeStat2.u16GlobalAvgB;
    }

    if (stat_key.bit1_be_ae_loc_stat) {
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                ae_stat->be_zone_avg[i][j][0] = isp_act_stat->stBEAeStat3.au16ZoneAvg[i][j][0]; /* R */
                ae_stat->be_zone_avg[i][j][1] = isp_act_stat->stBEAeStat3.au16ZoneAvg[i][j][1]; /* Gr */
                ae_stat->be_zone_avg[i][j][2] = isp_act_stat->stBEAeStat3.au16ZoneAvg[i][j][2]; /* Gb */
                ae_stat->be_zone_avg[i][j][3] = isp_act_stat->stBEAeStat3.au16ZoneAvg[i][j][3]; /* B */
            }
        }
    }

    HI_MPI_SYS_Munmap((hi_void *)isp_act_stat, sizeof(isp_stat));
    isp_get_ae_grid_info(vi_pipe, &ae_stat->fe_grid_info, &ae_stat->be_grid_info);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_ae_stitch_statistics(VI_PIPE vi_pipe, hi_isp_ae_stitch_statistics *ae_stitch_stat)
{
    return isp_get_ae_stitch_statistics(vi_pipe, ae_stitch_stat);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_mg_statistics(VI_PIPE vi_pipe, hi_isp_mg_statistics *mg_stat)
{
    hi_s32 i, j;
    hi_s32 ret;
    hi_u32 key_lowbit, key_highbit;

    hi_isp_statistics_ctrl stat_key;
    isp_stat_info stat_info;
    isp_stat *isp_act_stat;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(mg_stat);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ret = ioctl(g_as32IspFd[vi_pipe], ISP_STAT_ACT_GET, &stat_info);
    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("get active stat buffer err\n");
        return HI_ERR_ISP_NOMEM;
    }

    stat_info.virt_addr = HI_MPI_SYS_MmapCache(stat_info.phy_addr, sizeof(isp_stat));
    if (stat_info.virt_addr == HI_NULL) {
        return HI_ERR_ISP_NULL_PTR;
    }
    isp_act_stat = (isp_stat *)stat_info.virt_addr;

    key_lowbit  = hi_ext_system_statistics_ctrl_lowbit_read(vi_pipe);
    key_highbit = hi_ext_system_statistics_ctrl_highbit_read(vi_pipe);
    stat_key.key = ((hi_u64)key_highbit << 32) + key_lowbit;

    /* AE FE stat */
    if (stat_key.bit1_mg_stat) {
        for (i = 0; i < MG_ZONE_ROW; i++) {
            for (j = 0; j < MG_ZONE_COLUMN; j++) {
                mg_stat->au16_zone_avg[i][j][0] = isp_act_stat->stMgStat.au16ZoneAvg[i][j][0]; /* R */
                mg_stat->au16_zone_avg[i][j][1] = isp_act_stat->stMgStat.au16ZoneAvg[i][j][1]; /* Gr */
                mg_stat->au16_zone_avg[i][j][2] = isp_act_stat->stMgStat.au16ZoneAvg[i][j][2]; /* Gb */
                mg_stat->au16_zone_avg[i][j][3] = isp_act_stat->stMgStat.au16ZoneAvg[i][j][3]; /* B */
            }
        }
    }

    HI_MPI_SYS_Munmap((hi_void *)isp_act_stat, sizeof(isp_stat));
    isp_get_mg_grid_info(vi_pipe, &mg_stat->grid_info);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_wb_stitch_statistics(VI_PIPE vi_pipe, hi_isp_wb_stitch_statistics *stitch_wb_stat)
{
    return isp_get_wb_stitch_statistics(vi_pipe, stitch_wb_stat);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_wb_statistics(VI_PIPE vi_pipe, hi_isp_wb_statistics *wb_stat)
{
    hi_s32 i;
    hi_isp_statistics_ctrl stat_key;
    isp_stat_info act_stat_info;
    isp_stat *isp_act_stat;
    hi_s32 ret;
    hi_u32 key_lowbit, key_highbit;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(wb_stat);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    key_lowbit  = hi_ext_system_statistics_ctrl_lowbit_read(vi_pipe);
    key_highbit = hi_ext_system_statistics_ctrl_highbit_read(vi_pipe);
    stat_key.key = ((hi_u64)key_highbit << 32) + key_lowbit;

    ret = ioctl(g_as32IspFd[vi_pipe], ISP_STAT_ACT_GET, &act_stat_info);

    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("get active stat buffer err\n");
        return HI_ERR_ISP_NOMEM;
    }

    act_stat_info.virt_addr = HI_MPI_SYS_MmapCache(act_stat_info.phy_addr, sizeof(isp_stat));

    if (act_stat_info.virt_addr == HI_NULL) {
        return HI_ERR_ISP_NULL_PTR;
    }

    isp_act_stat = (isp_stat *)act_stat_info.virt_addr;

    if (stat_key.bit1_awb_stat1) {
        wb_stat->global_r = isp_act_stat->stAwbStat1.u16MeteringAwbAvgR;
        wb_stat->global_g = isp_act_stat->stAwbStat1.u16MeteringAwbAvgG;
        wb_stat->global_b = isp_act_stat->stAwbStat1.u16MeteringAwbAvgB;
        wb_stat->count_all = isp_act_stat->stAwbStat1.u16MeteringAwbCountAll;
    }

    if (stat_key.bit1_awb_stat2) {
        for (i = 0; i < AWB_ZONE_NUM; i++) {
            wb_stat->zone_avg_r[i] = isp_act_stat->stAwbStat2.au16MeteringMemArrayAvgR[i];
            wb_stat->zone_avg_g[i] = isp_act_stat->stAwbStat2.au16MeteringMemArrayAvgG[i];
            wb_stat->zone_avg_b[i] = isp_act_stat->stAwbStat2.au16MeteringMemArrayAvgB[i];
            wb_stat->zone_count_all[i] = isp_act_stat->stAwbStat2.au16MeteringMemArrayCountAll[i];
        }
    }

    HI_MPI_SYS_Munmap(act_stat_info.virt_addr, sizeof(isp_stat));

    isp_get_wb_grid_info(vi_pipe, &wb_stat->grid_info);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_statistics_config(VI_PIPE vi_pipe, const hi_isp_statistics_cfg *stat_cfg)
{
    hi_u8  max_awb_col, max_awb_row;
    hi_u16 width, height;
    hi_s32 i, j;
    hi_u32 key_lowbit, key_highbit;
    isp_working_mode isp_work_mode;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(stat_cfg);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (ioctl(g_as32IspFd[vi_pipe], ISP_WORK_MODE_GET, &isp_work_mode) != HI_SUCCESS) {
        ISP_ERR_TRACE("get work mode error!\n");
        return HI_FAILURE;
    }

    width  = hi_ext_system_be_total_width_read(vi_pipe);
    height = hi_ext_system_be_total_height_read(vi_pipe);

    if (isp_work_mode.data_input_mode == ISP_MODE_RAW) {
        // AWB
        if (stat_cfg->wb_cfg.awb_switch >= ISP_AWB_SWITCH_BUTT) {
            ISP_ERR_TRACE("Invalid WB awb_switch %d !\n", stat_cfg->wb_cfg.awb_switch);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->wb_cfg.cr_max > 0xFFF) {
            ISP_ERR_TRACE("max value WB cr_max is 0xFFF!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->wb_cfg.cb_max > 0xFFF) {
            ISP_ERR_TRACE("max value of WB cb_max is 0xFFF!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->wb_cfg.black_level > stat_cfg->wb_cfg.white_level) {
            ISP_ERR_TRACE("WB black_level should not larger than white_level!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((stat_cfg->wb_cfg.cr_min) > (stat_cfg->wb_cfg.cr_max)) {
            ISP_ERR_TRACE("WB cr_min should not larger than cr_max!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->wb_cfg.cb_min > stat_cfg->wb_cfg.cb_max) {
            ISP_ERR_TRACE("WB cb_min should not larger than cb_max!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        max_awb_col = MIN2(AWB_ZONE_ORIG_COLUMN, width / AWB_MIN_WIDTH);
        max_awb_row = MIN2(AWB_ZONE_ORIG_ROW, height / AWB_MIN_HEIGHT);

        if ((stat_cfg->wb_cfg.zone_row < 1) || (stat_cfg->wb_cfg.zone_row > max_awb_row)) {
            ISP_ERR_TRACE("Invalid zone_row! value range:[1, %d]!\n", max_awb_row);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((stat_cfg->wb_cfg.zone_col < isp_work_mode.block_num) || (stat_cfg->wb_cfg.zone_col > max_awb_col)) {
            ISP_ERR_TRACE("Invalid zone_col! value range:[%d, %d]!\n", isp_work_mode.block_num, max_awb_col);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->wb_cfg.zone_bin == 0x0) {
            ISP_ERR_TRACE("the value of WB zone_bin should be larger than 0x0!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->wb_cfg.zone_bin > AWB_ZONE_BIN) {
            ISP_ERR_TRACE("max value of WB zone_bin is %d!\n", AWB_ZONE_BIN);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        for (i = 1; i < 4; i++) {
            if (stat_cfg->wb_cfg.hist_bin_thresh[i] < stat_cfg->wb_cfg.hist_bin_thresh[i - 1]) {
                ISP_ERR_TRACE("max value of WB hist_bin_thresh[%d] should not be smaller than hist_bin_thresh[%d]!\n",
                          i, i - 1);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }

        if (stat_cfg->wb_cfg.awb_switch == ISP_AWB_AFTER_DG) {
            hi_ext_system_awb_switch_write(vi_pipe, HI_EXT_SYSTEM_AWB_SWITCH_AFTER_DG);
        } else if (stat_cfg->wb_cfg.awb_switch == ISP_AWB_AFTER_DRC) {
            hi_ext_system_awb_switch_write(vi_pipe, HI_EXT_SYSTEM_AWB_SWITCH_AFTER_DRC);
        } else if (stat_cfg->wb_cfg.awb_switch == ISP_AWB_AFTER_Expander) {
            hi_ext_system_awb_switch_write(vi_pipe, HI_EXT_SYSTEM_AWB_SWITCH_AFTER_EXPANDER);
        } else {
            ISP_ERR_TRACE("not support!");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        ISP_CHECK_BOOL(stat_cfg->wb_cfg.crop.enable);

        if (stat_cfg->wb_cfg.crop.w < stat_cfg->wb_cfg.zone_col * AWB_MIN_WIDTH) {
            ISP_ERR_TRACE("w should NOT be less than %d(zone_col * 60)!\n",
                      stat_cfg->wb_cfg.zone_col * AWB_MIN_WIDTH);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->wb_cfg.crop.h < stat_cfg->wb_cfg.zone_row * AWB_MIN_HEIGHT) {
            ISP_ERR_TRACE("h should NOT be less than %d(zone_row *14)!\n",
                      stat_cfg->wb_cfg.zone_row * AWB_MIN_HEIGHT);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((stat_cfg->wb_cfg.crop.x + stat_cfg->wb_cfg.crop.w) > width) {
            ISP_ERR_TRACE("x + w should NOT be larger than %d!\n", width);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((stat_cfg->wb_cfg.crop.y + stat_cfg->wb_cfg.crop.h) > height) {
            ISP_ERR_TRACE("y + h should NOT be larger than %d!\n", height);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_awb_white_level_write(vi_pipe, stat_cfg->wb_cfg.white_level);
        hi_ext_system_awb_black_level_write(vi_pipe, stat_cfg->wb_cfg.black_level);
        hi_ext_system_awb_cr_ref_max_write(vi_pipe, stat_cfg->wb_cfg.cr_max);
        hi_ext_system_awb_cr_ref_min_write(vi_pipe, stat_cfg->wb_cfg.cr_min);
        hi_ext_system_awb_cb_ref_max_write(vi_pipe, stat_cfg->wb_cfg.cb_max);
        hi_ext_system_awb_cb_ref_min_write(vi_pipe, stat_cfg->wb_cfg.cb_min);

        hi_ext_system_awb_vnum_write(vi_pipe, stat_cfg->wb_cfg.zone_row);
        hi_ext_system_awb_hnum_write(vi_pipe, stat_cfg->wb_cfg.zone_col);
        hi_ext_system_awb_zone_bin_write(vi_pipe, stat_cfg->wb_cfg.zone_bin);
        hi_ext_system_awb_hist_bin_thresh0_write(vi_pipe, stat_cfg->wb_cfg.hist_bin_thresh[0]);
        hi_ext_system_awb_hist_bin_thresh1_write(vi_pipe, stat_cfg->wb_cfg.hist_bin_thresh[1]);
        hi_ext_system_awb_hist_bin_thresh2_write(vi_pipe, stat_cfg->wb_cfg.hist_bin_thresh[2]);
        hi_ext_system_awb_hist_bin_thresh3_write(vi_pipe, stat_cfg->wb_cfg.hist_bin_thresh[3]);

        hi_ext_system_awb_crop_en_write(vi_pipe, stat_cfg->wb_cfg.crop.enable);
        hi_ext_system_awb_crop_x_write(vi_pipe, stat_cfg->wb_cfg.crop.x);
        hi_ext_system_awb_crop_y_write(vi_pipe, stat_cfg->wb_cfg.crop.y);
        hi_ext_system_awb_crop_height_write(vi_pipe, stat_cfg->wb_cfg.crop.h);
        hi_ext_system_awb_crop_width_write(vi_pipe, stat_cfg->wb_cfg.crop.w);

        hi_ext_system_wb_statistics_mpi_update_en_write(vi_pipe, HI_TRUE);

        // AE
        if (stat_cfg->ae_cfg.ae_switch >= ISP_AE_SWITCH_BUTT) {
            ISP_ERR_TRACE("Invalid AE switch %d!\n", stat_cfg->ae_cfg.ae_switch);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->ae_cfg.hist_config.hist_skip_x >= ISP_AE_HIST_SKIP_BUTT) {
            ISP_ERR_TRACE("hist_skip_x should not be larger than 6\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->ae_cfg.hist_config.hist_skip_y >= ISP_AE_HIST_SKIP_BUTT) {
            ISP_ERR_TRACE("hist_skip_y should not be larger than 6\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->ae_cfg.hist_config.hist_offset_x >= ISP_AE_HIST_OFFSET_X_BUTT) {
            ISP_ERR_TRACE("hist_offset_x should not be larger than 1\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->ae_cfg.hist_config.hist_offset_y >= ISP_AE_HIST_OFFSET_Y_BUTT) {
            ISP_ERR_TRACE("hist_offset_y should not larger than 1\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->ae_cfg.four_plane_mode >= ISP_AE_FOUR_PLANE_MODE_BUTT) {
            ISP_ERR_TRACE("four_plane_mode should not be larger than 1\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((stat_cfg->ae_cfg.four_plane_mode == ISP_AE_FOUR_PLANE_MODE_DISABLE) &&
            (stat_cfg->ae_cfg.hist_config.hist_skip_x == ISP_AE_HIST_SKIP_EVERY_PIXEL)) {
            ISP_ERR_TRACE("hist_skip_x should not be 0 when not in four_plane_mode\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->ae_cfg.hist_mode >= ISP_AE_MODE_BUTT) {
            ISP_ERR_TRACE("hist_mode should not be larger than 1\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->ae_cfg.aver_mode >= ISP_AE_MODE_BUTT) {
            ISP_ERR_TRACE("aver_mode should not be larger than 1\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->ae_cfg.max_gain_mode >= ISP_AE_MODE_BUTT) {
            ISP_ERR_TRACE("max_gain_mode should not be larger than 1\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                if (stat_cfg->ae_cfg.weight[i][j] > 0xF) {
                    ISP_ERR_TRACE("weight[%d][%d] should not be larger than 0xF!\n", i, j);
                    return HI_ERR_ISP_ILLEGAL_PARAM;
                }
            }
        }

        if (stat_cfg->ae_cfg.ae_switch == ISP_AE_AFTER_DG) {
            hi_ext_system_ae_be_sel_write(vi_pipe, HI_ISP_TOP_AE_SELECT_AFTER_DG);
        } else if (stat_cfg->ae_cfg.ae_switch == ISP_AE_AFTER_WB) {
            hi_ext_system_ae_be_sel_write(vi_pipe, HI_ISP_TOP_AE_SELECT_AFTER_WB);
        } else if (stat_cfg->ae_cfg.ae_switch == ISP_AE_AFTER_DRC) {
            hi_ext_system_ae_be_sel_write(vi_pipe, HI_ISP_TOP_AE_SELECT_AFTER_DRC);
        } else {
            ISP_ERR_TRACE("vi_pipe:%d not support this!\n", vi_pipe);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        ISP_CHECK_BOOL(stat_cfg->ae_cfg.crop.enable);

        if (stat_cfg->ae_cfg.crop.w < AE_MIN_WIDTH) {
            ISP_ERR_TRACE("crop_width should be lager than 256!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (stat_cfg->ae_cfg.crop.h < AE_MIN_HEIGHT) {
            ISP_ERR_TRACE("crop_height should be lager than 120!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((stat_cfg->ae_cfg.crop.x + stat_cfg->ae_cfg.crop.w) > width) {
            ISP_ERR_TRACE("x + w should not be lager than %d!\n", width);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((stat_cfg->ae_cfg.crop.y + stat_cfg->ae_cfg.crop.h) > height) {
            ISP_ERR_TRACE("y + h should not be lager than %d!\n", height);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_ae_fourplanemode_write(vi_pipe, stat_cfg->ae_cfg.four_plane_mode);
        hi_ext_system_ae_hist_skip_x_write(vi_pipe, stat_cfg->ae_cfg.hist_config.hist_skip_x);
        hi_ext_system_ae_hist_skip_y_write(vi_pipe, stat_cfg->ae_cfg.hist_config.hist_skip_y);
        hi_ext_system_ae_hist_offset_x_write(vi_pipe, stat_cfg->ae_cfg.hist_config.hist_offset_x);
        hi_ext_system_ae_hist_offset_y_write(vi_pipe, stat_cfg->ae_cfg.hist_config.hist_offset_y);
        hi_ext_system_ae_histmode_write(vi_pipe, stat_cfg->ae_cfg.hist_mode);
        hi_ext_system_ae_avermode_write(vi_pipe, stat_cfg->ae_cfg.aver_mode);
        hi_ext_system_ae_maxgainmode_write(vi_pipe, stat_cfg->ae_cfg.max_gain_mode);

        hi_ext_system_ae_crop_en_write(vi_pipe, stat_cfg->ae_cfg.crop.enable);
        hi_ext_system_ae_crop_x_write(vi_pipe, stat_cfg->ae_cfg.crop.x);
        hi_ext_system_ae_crop_y_write(vi_pipe, stat_cfg->ae_cfg.crop.y);
        hi_ext_system_ae_crop_height_write(vi_pipe, stat_cfg->ae_cfg.crop.h);
        hi_ext_system_ae_crop_width_write(vi_pipe, stat_cfg->ae_cfg.crop.w);

        /* set 15*17 weight table */
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                hi_ext_system_ae_weight_table_write(vi_pipe, (i * AE_ZONE_COLUMN + j), stat_cfg->ae_cfg.weight[i][j]);
            }
        }
    }

#ifdef CONFIG_HI_ISP_AF_SUPPORT
    hi_u8 shift0;
    hi_s16 g0, g1, g2;
    hi_float f32_temp, f32_pl;
    hi_u32 plg, pls;
    const hi_isp_af_h_param *iir;

    /* AF paramters check */
    /* confg */
    if (stat_cfg->focus_cfg.config.h_wnd < isp_work_mode.block_num) {
        ISP_ERR_TRACE("the value of AF h_wnd should be >= u8BlockNum:%d!\n", isp_work_mode.block_num);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.enable,         0x1,            "Invalid AF enable!\n");
    MPI_ISP_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.h_wnd,               1,        17, "Invalid AF h_wnd!\n");
    MPI_ISP_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.v_wnd,               1,        15, "Invalid AF v_wnd!\n");
    MPI_ISP_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.h_size,            256,  width, "Invalid AF h_size!\n");
    MPI_ISP_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.v_size, RES_HEIGHT_MIN, height, "Invalid AF v_size!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.peak_mode,      0x1,            "Invalid AF peak_mode!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.squ_mode,       0x1,            "Invalid AF squ_mode!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.crop.enable,  0x1,            "Invalid AF crop.enable!\n");

    if (stat_cfg->focus_cfg.config.crop.w < 256) {
        ISP_ERR_TRACE("w should be larger than 256!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (stat_cfg->focus_cfg.config.crop.h < 120) {
        ISP_ERR_TRACE("h should be larger than 256!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((stat_cfg->focus_cfg.config.crop.x + stat_cfg->focus_cfg.config.crop.w) > width) {
        ISP_ERR_TRACE("x + w should NOT be larger than %d!\n", width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((stat_cfg->focus_cfg.config.crop.y + stat_cfg->focus_cfg.config.crop.h) > height) {
        ISP_ERR_TRACE("y + h should NOT be larger than %d!\n", height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (stat_cfg->focus_cfg.config.crop.w % 8 != 0) {
        ISP_ERR_TRACE("Invalid crop w:%d!\n", stat_cfg->focus_cfg.config.crop.w);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (stat_cfg->focus_cfg.config.crop.h % 2 != 0) {
        ISP_ERR_TRACE("Invalid crop h:%d!\n", stat_cfg->focus_cfg.config.crop.h);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.statistics_pos,                       0x2, "Invalid AF statistics_pos!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.raw_cfg.gamma_value,    AF_GAMMA_VALUE_MAX, "Invalid AF raw_cfg.gamma_value!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.raw_cfg.gamma_gain_limit,               0x5, "Invalid AF raw_cfg.gamma_gain_limit!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.raw_cfg.pattern,                    0x3, "Invalid AF raw_cfg.pattern!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.pre_flt_cfg.en,                       0x1, "Invalid AF pre_flt_cfg.en!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.pre_flt_cfg.strength,            0xFFFF, "Invalid AF pre_flt_cfg.u16strength!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.config.high_luma_th,                        0xFF, "Invalid AF high_luma_th!\n");

    /* IIR0 */
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.narrow_band,           0x1,               "Invalid AF h_param_iir0.narrow_band!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.iir_en,     hi_bool, 3,    0x1,    "Invalid AF h_param_iir0.iir_en!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.iir_shift,           0x3F,               "Invalid AF h_param_iir0.iir_delay!\n");
    MPI_ISP_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.iir_gain[0],              0,     255,      "Invalid AF h_param_iir0.iir_gain[0]\n!");
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(&stat_cfg->focus_cfg.h_param_iir0.iir_gain[1],  hi_s16, 6, -511, 511, "Invalid AF h_param_iir0.iir_gain!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.iir_shift_lut, hi_u16, 4,  0x7,      "Invalid AF h_param_iir0.iir_shift!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.ld.ld_en,            0x1,               "Invalid AF h_param_iir0.ld.ld_en!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.ld.th_low,        0xFF,               "Invalid AF h_param_iir0.ld.th_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.ld.gain_low,      0xFF,               "Invalid AF h_param_iir0.ld.gain_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.ld.slp_low,        0xF,               "Invalid AF h_param_iir0.ld.slp_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.ld.th_high,       0xFF,               "Invalid AF h_param_iir0.ld.th_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.ld.gain_high,     0xFF,               "Invalid AF h_param_iir0.ld.gain_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.ld.slp_high,       0xF,               "Invalid AF h_param_iir0.ld.slp_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.coring.th,      0x7FF,               "Invalid AF h_param_iir0.coring.th!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.coring.slp,       0xF,               "Invalid AF h_param_iir0.coring.slp!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir0.coring.lmt,     0x7FF,               "Invalid AF h_param_iir0.coring.lmt!\n");
    /* IIR1 */
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.narrow_band,           0x1,               "Invalid AF h_param_iir1.narrow_band!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.iir_en,     hi_bool, 3,  0x1,      "Invalid AF h_param_iir1.iir_en!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.iir_shift,           0x3F,               "Invalid AF h_param_iir1.iir_delay\n!");
    MPI_ISP_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.iir_gain[0],              0,    0xFF,      "Invalid AF h_param_iir1.iir_gain[0]!\n");
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(&stat_cfg->focus_cfg.h_param_iir1.iir_gain[1],  hi_s16, 6, -511, 511, "Invalid AF h_param_iir1.iir_gain!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.iir_shift_lut, hi_u16, 4,  0x7,      "Invalid AF h_param_iir1.iir_shift!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.ld.ld_en,            0x1,               "Invalid AF h_param_iir1.ld.ld_en!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.ld.th_low,        0xFF,               "Invalid AF h_param_iir1.ld.th_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.ld.gain_low,      0xFF,               "Invalid AF h_param_iir1.ld.gain_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.ld.slp_low,        0xF,               "Invalid AF h_param_iir1.ld.slp_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.ld.th_high,       0xFF,               "Invalid AF h_param_iir1.ld.th_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.ld.gain_high,     0xFF,               "Invalid AF h_param_iir1.ld.gain_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.ld.slp_high,       0xF,               "Invalid AF h_param_iir1.ld.slp_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.coring.th,      0x7FF,               "Invalid AF h_param_iir1.coring.th!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.coring.slp,       0xF,               "Invalid AF h_param_iir1.coring.slp!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.h_param_iir1.coring.lmt,     0x7FF,               "Invalid AF h_param_iir1.coring.lmt!\n");
    /* FIR0 */
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.firh,      hi_s16, 5, -31,  31,   "Invalid AF v_param_fir0.firh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.ld.ld_en,         0x1,                "Invalid AF v_param_fir0.ld.ld_en!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.ld.th_low,     0xFF,                "Invalid AF v_param_fir0.ld.th_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.ld.gain_low,   0xFF,                "Invalid AF v_param_fir0.ld.gain_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.ld.slp_low,     0xF,                "Invalid AF v_param_fir0.ld.slp_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.ld.th_high,    0xFF,                "Invalid AF v_param_fir0.ld.th_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.ld.gain_high,  0xFF,                "Invalid AF v_param_fir0.ld.gain_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.ld.slp_high,    0xF,                "Invalid AF v_param_fir0.ld.slp_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.coring.th,   0x7FF,                "Invalid AF v_param_fir0.coring.th!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.coring.slp,    0xF,                "Invalid AF v_param_fir0.coring.slp!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir0.coring.lmt,  0x7FF,                "Invalid AF v_param_fir0.coring.lmt!\n");
    /* FIR1 */
    MPI_ISP_ARRAY_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.firh,     hi_s16, 5, -31,  31,  "Invalid AF v_param_fir1.firh!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.ld.ld_en,        0x1,               "Invalid AF v_param_fir1.ld.ld_en!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.ld.th_low,    0xFF,               "Invalid AF v_param_fir1.ld.th_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.ld.gain_low,  0xFF,               "Invalid AF v_param_fir1.ld.gain_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.ld.slp_low,    0xF,               "Invalid AF v_param_fir1.ld.slp_low!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.ld.th_high,   0xFF,               "Invalid AF v_param_fir1.ld.th_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.ld.gain_high, 0xFF,               "Invalid AF v_param_fir1.ld.gain_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.ld.slp_high,   0xF,               "Invalid AF v_param_fir1.ld.slp_high!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.coring.th,  0x7FF,               "Invalid AF v_param_fir1.coring.th!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.coring.slp,   0xF,               "Invalid AF v_param_fir1.coring.slp!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.v_param_fir1.coring.lmt, 0x7FF,               "Invalid AF v_param_fir1.coring.lmt!\n");
    /* FVPARAM */
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.fv_param.acc_shift_y,          0xF,                  "Invalid AF fv_param.acc_shift_y!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.fv_param.acc_shift_h, hi_u16,       2,   0xF, "Invalid AF fv_param.acc_shift_h!\n");
    MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.fv_param.acc_shift_v, hi_u16,       2,   0xF, "Invalid AF fv_param.acc_shift_h!\n");
    MPI_ISP_MAX_PARAM_CHECK_RETURN(stat_cfg->focus_cfg.fv_param.hl_cnt_shift,         0xF,                  "Invalid AF fv_param.hl_cnt_shift!\n");

    /* AF STATISTICS CONIFG */
    if (stat_cfg->focus_cfg.config.enable) {
        hi_ext_system_af_enable_write(vi_pipe, 0x3);
    } else {
        hi_ext_system_af_enable_write(vi_pipe, 0x0);
    }
    hi_ext_af_iir0_enable0_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.iir_en[0]);
    hi_ext_af_iir0_enable1_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.iir_en[1]);
    hi_ext_af_iir0_enable2_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.iir_en[2]);
    hi_ext_af_iir1_enable0_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.iir_en[0]);
    hi_ext_af_iir1_enable1_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.iir_en[1]);
    hi_ext_af_iir1_enable2_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.iir_en[2]);
    hi_ext_af_iir0_shift_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.iir_shift);
    hi_ext_af_iir1_shift_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.iir_shift);
    hi_ext_af_peakmode_write(vi_pipe, stat_cfg->focus_cfg.config.peak_mode);
    hi_ext_af_squmode_write(vi_pipe, stat_cfg->focus_cfg.config.squ_mode);
    hi_ext_af_window_hnum_write(vi_pipe, stat_cfg->focus_cfg.config.h_wnd);
    hi_ext_af_window_vnum_write(vi_pipe, stat_cfg->focus_cfg.config.v_wnd);
    hi_ext_af_iir_gain0_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir0.iir_gain[0]);
    hi_ext_af_iir_gain0_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir1.iir_gain[0]);
    hi_ext_af_iir_gain1_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir0.iir_gain[1]);
    hi_ext_af_iir_gain1_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir1.iir_gain[1]);
    hi_ext_af_iir_gain2_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir0.iir_gain[2]);
    hi_ext_af_iir_gain2_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir1.iir_gain[2]);
    hi_ext_af_iir_gain3_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir0.iir_gain[3]);
    hi_ext_af_iir_gain3_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir1.iir_gain[3]);
    hi_ext_af_iir_gain4_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir0.iir_gain[4]);
    hi_ext_af_iir_gain4_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir1.iir_gain[4]);
    hi_ext_af_iir_gain5_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir0.iir_gain[5]);
    hi_ext_af_iir_gain5_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir1.iir_gain[5]);
    hi_ext_af_iir_gain6_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir0.iir_gain[6]);
    hi_ext_af_iir_gain6_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.h_param_iir1.iir_gain[6]);
    hi_ext_af_iir0_shift_group0_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.iir_shift_lut[0]);
    hi_ext_af_iir1_shift_group0_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.iir_shift_lut[1]);
    hi_ext_af_iir2_shift_group0_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.iir_shift_lut[2]);
    hi_ext_af_iir3_shift_group0_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.iir_shift_lut[3]);
    hi_ext_af_iir0_shift_group1_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.iir_shift_lut[0]);
    hi_ext_af_iir1_shift_group1_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.iir_shift_lut[1]);
    hi_ext_af_iir2_shift_group1_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.iir_shift_lut[2]);
    hi_ext_af_iir3_shift_group1_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.iir_shift_lut[3]);
    hi_ext_af_fir_h_gain0_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.v_param_fir0.firh[0]);
    hi_ext_af_fir_h_gain0_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.v_param_fir1.firh[0]);
    hi_ext_af_fir_h_gain1_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.v_param_fir0.firh[1]);
    hi_ext_af_fir_h_gain1_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.v_param_fir1.firh[1]);
    hi_ext_af_fir_h_gain2_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.v_param_fir0.firh[2]);
    hi_ext_af_fir_h_gain2_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.v_param_fir1.firh[2]);
    hi_ext_af_fir_h_gain3_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.v_param_fir0.firh[3]);
    hi_ext_af_fir_h_gain3_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.v_param_fir1.firh[3]);
    hi_ext_af_fir_h_gain4_group0_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.v_param_fir0.firh[4]);
    hi_ext_af_fir_h_gain4_group1_write(vi_pipe, (hi_u32)stat_cfg->focus_cfg.v_param_fir1.firh[4]);

    /* ds */
    hi_ext_af_iir0_ds_enable_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.narrow_band);
    hi_ext_af_iir1_ds_enable_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.narrow_band);

    /* PLG and PLS */
    for (i = 0; i < 2; i++) {
        iir = i ? &(stat_cfg->focus_cfg.h_param_iir1) : &(stat_cfg->focus_cfg.h_param_iir0);

        shift0 = iir->iir_shift_lut[0];
        g0 = iir->iir_gain[0];
        g1 = iir->iir_gain[1];
        g2 = iir->iir_gain[2];

        f32_pl = (512.f / DIV_0_TO_1(512 - 2 * g1 - g2) * g0) / (1 << shift0);
        f32_temp = f32_pl;
        f32_temp = MIN2(7 - floor(log(f32_temp) / log(2)), 7);

        pls = (hi_u32)f32_temp;
        plg = (hi_u32)((f32_pl * (1 << pls)) + 0.5);

        if (i == 0) {
            hi_ext_af_iir_pls_group0_write(vi_pipe, pls);
            hi_ext_af_iir_plg_group0_write(vi_pipe, plg);
        } else {
            hi_ext_af_iir_pls_group1_write(vi_pipe, pls);
            hi_ext_af_iir_plg_group1_write(vi_pipe, plg);
        }
    }

    /* AF crop */
    hi_ext_af_crop_enable_write(vi_pipe, stat_cfg->focus_cfg.config.crop.enable);
    hi_ext_af_crop_pos_x_write(vi_pipe, stat_cfg->focus_cfg.config.crop.x);
    hi_ext_af_crop_pos_y_write(vi_pipe, stat_cfg->focus_cfg.config.crop.y);
    hi_ext_af_crop_hsize_write(vi_pipe, stat_cfg->focus_cfg.config.crop.w);
    hi_ext_af_crop_vsize_write(vi_pipe, stat_cfg->focus_cfg.config.crop.h);

    /* AF raw cfg */
    hi_ext_af_pos_sel_write(vi_pipe, stat_cfg->focus_cfg.config.statistics_pos);
    hi_ext_af_rawmode_write(vi_pipe, ~(((stat_cfg->focus_cfg.config.statistics_pos) >> 0x1) & 0x1));
    hi_ext_af_gain_limit_write(vi_pipe, stat_cfg->focus_cfg.config.raw_cfg.gamma_gain_limit);
    hi_ext_af_gamma_write(vi_pipe, stat_cfg->focus_cfg.config.raw_cfg.gamma_value);
    hi_ext_af_bayermode_write(vi_pipe, stat_cfg->focus_cfg.config.raw_cfg.pattern);

    /* AF pre median filter */
    hi_ext_af_mean_enable_write(vi_pipe, stat_cfg->focus_cfg.config.pre_flt_cfg.en);
    hi_ext_af_mean_thres_write(vi_pipe, stat_cfg->focus_cfg.config.pre_flt_cfg.strength);

    /* level depend gain */
    hi_ext_af_iir0_ldg_enable_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.ld.ld_en);

    hi_ext_af_iir_thre0_low_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.ld.th_low);
    hi_ext_af_iir_thre0_high_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.ld.th_high);
    hi_ext_af_iir_slope0_low_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.ld.slp_low);
    hi_ext_af_iir_slope0_high_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.ld.slp_high);
    hi_ext_af_iir_gain0_low_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.ld.gain_low);
    hi_ext_af_iir_gain0_high_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.ld.gain_high);

    hi_ext_af_iir1_ldg_enable_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.ld.ld_en);

    hi_ext_af_iir_thre1_low_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.ld.th_low);
    hi_ext_af_iir_thre1_high_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.ld.th_high);
    hi_ext_af_iir_slope1_low_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.ld.slp_low);
    hi_ext_af_iir_slope1_high_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.ld.slp_high);
    hi_ext_af_iir_gain1_low_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.ld.gain_low);
    hi_ext_af_iir_gain1_high_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.ld.gain_high);

    hi_ext_af_fir0_ldg_enable_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir0.ld.ld_en);

    hi_ext_af_fir_thre0_low_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir0.ld.th_low);
    hi_ext_af_fir_thre0_high_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir0.ld.th_high);
    hi_ext_af_fir_slope0_low_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir0.ld.slp_low);
    hi_ext_af_fir_slope0_high_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir0.ld.slp_high);
    hi_ext_af_fir_gain0_low_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir0.ld.gain_low);
    hi_ext_af_fir_gain0_high_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir0.ld.gain_high);

    hi_ext_af_fir1_ldg_enable_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir1.ld.ld_en);

    hi_ext_af_fir_thre1_low_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir1.ld.th_low);
    hi_ext_af_fir_thre1_high_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir1.ld.th_high);
    hi_ext_af_fir_slope1_low_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir1.ld.slp_low);
    hi_ext_af_fir_slope1_high_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir1.ld.slp_high);
    hi_ext_af_fir_gain1_low_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir1.ld.gain_low);
    hi_ext_af_fir_gain1_high_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir1.ld.gain_high);

    /* AF coring */
    hi_ext_af_iir_thre0_coring_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.coring.th);
    hi_ext_af_iir_slope0_coring_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.coring.slp);
    hi_ext_af_iir_peak0_coring_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir0.coring.lmt);

    hi_ext_af_iir_thre1_coring_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.coring.th);
    hi_ext_af_iir_slope1_coring_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.coring.slp);
    hi_ext_af_iir_peak1_coring_write(vi_pipe, stat_cfg->focus_cfg.h_param_iir1.coring.lmt);

    hi_ext_af_fir_thre0_coring_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir0.coring.th);
    hi_ext_af_fir_slope0_coring_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir0.coring.slp);
    hi_ext_af_fir_peak0_coring_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir0.coring.lmt);

    hi_ext_af_fir_thre1_coring_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir1.coring.th);
    hi_ext_af_fir_slope1_coring_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir1.coring.slp);
    hi_ext_af_fir_peak1_coring_write(vi_pipe, stat_cfg->focus_cfg.v_param_fir1.coring.lmt);

    /* high luma counter */
    hi_ext_af_hiligh_thre_write(vi_pipe, stat_cfg->focus_cfg.config.high_luma_th);

    /* AF output shift */
    hi_ext_af_acc_shift0_h_write(vi_pipe, stat_cfg->focus_cfg.fv_param.acc_shift_h[0]);
    hi_ext_af_acc_shift1_h_write(vi_pipe, stat_cfg->focus_cfg.fv_param.acc_shift_h[1]);
    hi_ext_af_acc_shift0_v_write(vi_pipe, stat_cfg->focus_cfg.fv_param.acc_shift_v[0]);
    hi_ext_af_acc_shift1_v_write(vi_pipe, stat_cfg->focus_cfg.fv_param.acc_shift_v[1]);
    hi_ext_af_acc_shift_y_write(vi_pipe, stat_cfg->focus_cfg.fv_param.acc_shift_y);
    hi_ext_af_shift_count_y_write(vi_pipe, stat_cfg->focus_cfg.fv_param.hl_cnt_shift);

    hi_ext_af_input_hsize_write(vi_pipe, stat_cfg->focus_cfg.config.h_size);
    hi_ext_af_input_vsize_write(vi_pipe, stat_cfg->focus_cfg.config.v_size);

    hi_ext_af_set_flag_write(vi_pipe, HI_EXT_AF_SET_FLAG_ENABLE);
#endif

    key_lowbit = (hi_u32)(stat_cfg->key.key & 0xFFFFFFFF);
    key_highbit = (hi_u32)((stat_cfg->key.key & 0xFFFFFFFF00000000) >> 32);
    hi_ext_system_statistics_ctrl_lowbit_write(vi_pipe, key_lowbit);
    hi_ext_system_statistics_ctrl_highbit_write(vi_pipe, key_highbit);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_statistics_config(VI_PIPE vi_pipe, hi_isp_statistics_cfg *stat_cfg)
{
    hi_s32 i, j;
    hi_u8  tmp;
    hi_u32 key_lowbit, key_highbit;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(stat_cfg);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    key_lowbit  = hi_ext_system_statistics_ctrl_lowbit_read(vi_pipe);
    key_highbit = hi_ext_system_statistics_ctrl_highbit_read(vi_pipe);
    stat_cfg->key.key = ((hi_u64)key_highbit << 32) + key_lowbit;

    /*************************AWB STATISTICS CONIFG ***************************/
    tmp = hi_ext_system_awb_switch_read(vi_pipe);
    if (tmp == HI_EXT_SYSTEM_AWB_SWITCH_AFTER_DG) {
        stat_cfg->wb_cfg.awb_switch = ISP_AWB_AFTER_DG;
    } else if (tmp == HI_EXT_SYSTEM_AWB_SWITCH_AFTER_EXPANDER) {
        stat_cfg->wb_cfg.awb_switch = ISP_AWB_AFTER_Expander;
    } else if (tmp == HI_EXT_SYSTEM_AWB_SWITCH_AFTER_DRC) {
        stat_cfg->wb_cfg.awb_switch = ISP_AWB_AFTER_DRC;
    } else {
        stat_cfg->wb_cfg.awb_switch = ISP_AWB_SWITCH_BUTT;
    }

    stat_cfg->wb_cfg.zone_row = hi_ext_system_awb_vnum_read(vi_pipe);
    stat_cfg->wb_cfg.zone_col = hi_ext_system_awb_hnum_read(vi_pipe);
    stat_cfg->wb_cfg.zone_bin = hi_ext_system_awb_zone_bin_read(vi_pipe);
    stat_cfg->wb_cfg.hist_bin_thresh[0] = hi_ext_system_awb_hist_bin_thresh0_read(vi_pipe);
    stat_cfg->wb_cfg.hist_bin_thresh[1] = hi_ext_system_awb_hist_bin_thresh1_read(vi_pipe);
    stat_cfg->wb_cfg.hist_bin_thresh[2] = hi_ext_system_awb_hist_bin_thresh2_read(vi_pipe);
    stat_cfg->wb_cfg.hist_bin_thresh[3] = hi_ext_system_awb_hist_bin_thresh3_read(vi_pipe);

    stat_cfg->wb_cfg.white_level = hi_ext_system_awb_white_level_read(vi_pipe);
    stat_cfg->wb_cfg.black_level = hi_ext_system_awb_black_level_read(vi_pipe);
    stat_cfg->wb_cfg.cr_max = hi_ext_system_awb_cr_ref_max_read(vi_pipe);
    stat_cfg->wb_cfg.cr_min = hi_ext_system_awb_cr_ref_min_read(vi_pipe);
    stat_cfg->wb_cfg.cb_max = hi_ext_system_awb_cb_ref_max_read(vi_pipe);
    stat_cfg->wb_cfg.cb_min = hi_ext_system_awb_cb_ref_min_read(vi_pipe);
    stat_cfg->wb_cfg.crop.enable = hi_ext_system_awb_crop_en_read(vi_pipe);
    stat_cfg->wb_cfg.crop.x = hi_ext_system_awb_crop_x_read(vi_pipe);
    stat_cfg->wb_cfg.crop.y = hi_ext_system_awb_crop_y_read(vi_pipe);
    stat_cfg->wb_cfg.crop.h = hi_ext_system_awb_crop_height_read(vi_pipe);
    stat_cfg->wb_cfg.crop.w = hi_ext_system_awb_crop_width_read(vi_pipe);

    /*************************AE STATISTICS CONIFG ***************************/
    tmp = hi_ext_system_ae_be_sel_read(vi_pipe);
    if (tmp == HI_ISP_TOP_AE_SELECT_AFTER_DG) {
        stat_cfg->ae_cfg.ae_switch = ISP_AE_AFTER_DG;
    } else if (tmp == HI_ISP_TOP_AE_SELECT_AFTER_WB) {
        stat_cfg->ae_cfg.ae_switch = ISP_AE_AFTER_WB;
    } else if (tmp == HI_ISP_TOP_AE_SELECT_AFTER_DRC) {
        stat_cfg->ae_cfg.ae_switch = ISP_AE_AFTER_DRC;
    } else {
        stat_cfg->ae_cfg.ae_switch = ISP_AE_SWITCH_BUTT;
    }

    tmp = hi_ext_system_ae_fourplanemode_read(vi_pipe);
    if (tmp == HI_ISP_AE_FOUR_PLANE_MODE_DISABLE) {
        stat_cfg->ae_cfg.four_plane_mode = ISP_AE_FOUR_PLANE_MODE_DISABLE;
    } else if (tmp == HI_ISP_AE_FOUR_PLANE_MODE_ENABLE) {
        stat_cfg->ae_cfg.four_plane_mode = ISP_AE_FOUR_PLANE_MODE_ENABLE;
    } else {
        stat_cfg->ae_cfg.four_plane_mode = ISP_AE_FOUR_PLANE_MODE_BUTT;
    }

    stat_cfg->ae_cfg.hist_config.hist_skip_x = hi_ext_system_ae_hist_skip_x_read(vi_pipe);
    stat_cfg->ae_cfg.hist_config.hist_skip_y = hi_ext_system_ae_hist_skip_y_read(vi_pipe);
    stat_cfg->ae_cfg.hist_config.hist_offset_x = hi_ext_system_ae_hist_offset_x_read(vi_pipe);
    stat_cfg->ae_cfg.hist_config.hist_offset_y = hi_ext_system_ae_hist_offset_y_read(vi_pipe);
    stat_cfg->ae_cfg.hist_mode = hi_ext_system_ae_histmode_read(vi_pipe);
    stat_cfg->ae_cfg.aver_mode = hi_ext_system_ae_avermode_read(vi_pipe);
    stat_cfg->ae_cfg.max_gain_mode = hi_ext_system_ae_maxgainmode_read(vi_pipe);

    stat_cfg->ae_cfg.crop.enable = hi_ext_system_ae_crop_en_read(vi_pipe);
    stat_cfg->ae_cfg.crop.x = hi_ext_system_ae_crop_x_read(vi_pipe);
    stat_cfg->ae_cfg.crop.y = hi_ext_system_ae_crop_y_read(vi_pipe);
    stat_cfg->ae_cfg.crop.h = hi_ext_system_ae_crop_height_read(vi_pipe);
    stat_cfg->ae_cfg.crop.w = hi_ext_system_ae_crop_width_read(vi_pipe);

    /* set 15*17 weight table */
    for (i = 0; i < AE_ZONE_ROW; i++) {
        for (j = 0; j < AE_ZONE_COLUMN; j++) {
            stat_cfg->ae_cfg.weight[i][j] = hi_ext_system_ae_weight_table_read(vi_pipe, (i * AE_ZONE_COLUMN + j));
        }
    }

#ifdef CONFIG_HI_ISP_AF_SUPPORT
    /* AF FE STATISTICS CONIFG START */
    stat_cfg->focus_cfg.config.enable = (hi_ext_system_af_enable_read(vi_pipe) != 0x0) ? 1 : 0;
    stat_cfg->focus_cfg.h_param_iir0.iir_en[0] = hi_ext_af_iir0_enable0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_en[1] = hi_ext_af_iir0_enable1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_en[2] = hi_ext_af_iir0_enable2_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_en[0] = hi_ext_af_iir1_enable0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_en[1] = hi_ext_af_iir1_enable1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_en[2] = hi_ext_af_iir1_enable2_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_shift = hi_ext_af_iir0_shift_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_shift = hi_ext_af_iir1_shift_read(vi_pipe);
    stat_cfg->focus_cfg.config.peak_mode = hi_ext_af_peakmode_read(vi_pipe);
    stat_cfg->focus_cfg.config.squ_mode = hi_ext_af_squmode_read(vi_pipe);
    stat_cfg->focus_cfg.config.h_wnd = hi_ext_af_window_hnum_read(vi_pipe);
    stat_cfg->focus_cfg.config.v_wnd = hi_ext_af_window_vnum_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_gain[0] = hi_ext_af_iir_gain0_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_gain[0] = hi_ext_af_iir_gain0_group1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_gain[1] = hi_ext_af_iir_gain1_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_gain[1] = hi_ext_af_iir_gain1_group1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_gain[2] = hi_ext_af_iir_gain2_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_gain[2] = hi_ext_af_iir_gain2_group1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_gain[3] = hi_ext_af_iir_gain3_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_gain[3] = hi_ext_af_iir_gain3_group1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_gain[4] = hi_ext_af_iir_gain4_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_gain[4] = hi_ext_af_iir_gain4_group1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_gain[5] = hi_ext_af_iir_gain5_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_gain[5] = hi_ext_af_iir_gain5_group1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_gain[6] = hi_ext_af_iir_gain6_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_gain[6] = hi_ext_af_iir_gain6_group1_read(vi_pipe);

    stat_cfg->focus_cfg.h_param_iir0.iir_shift_lut[0] = hi_ext_af_iir0_shift_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_shift_lut[1] = hi_ext_af_iir1_shift_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_shift_lut[2] = hi_ext_af_iir2_shift_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.iir_shift_lut[3] = hi_ext_af_iir3_shift_group0_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_shift_lut[0] = hi_ext_af_iir0_shift_group1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_shift_lut[1] = hi_ext_af_iir1_shift_group1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_shift_lut[2] = hi_ext_af_iir2_shift_group1_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.iir_shift_lut[3] = hi_ext_af_iir3_shift_group1_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.firh[0] = hi_ext_af_fir_h_gain0_group0_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.firh[0] = hi_ext_af_fir_h_gain0_group1_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.firh[1] = hi_ext_af_fir_h_gain1_group0_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.firh[1] = hi_ext_af_fir_h_gain1_group1_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.firh[2] = hi_ext_af_fir_h_gain2_group0_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.firh[2] = hi_ext_af_fir_h_gain2_group1_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.firh[3] = hi_ext_af_fir_h_gain3_group0_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.firh[3] = hi_ext_af_fir_h_gain3_group1_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.firh[4] = hi_ext_af_fir_h_gain4_group0_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.firh[4] = hi_ext_af_fir_h_gain4_group1_read(vi_pipe);

    /* ds */
    stat_cfg->focus_cfg.h_param_iir0.narrow_band = hi_ext_af_iir0_ds_enable_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.narrow_band = hi_ext_af_iir1_ds_enable_read(vi_pipe);

    /* AF crop */
    stat_cfg->focus_cfg.config.crop.enable = hi_ext_af_crop_enable_read(vi_pipe);
    stat_cfg->focus_cfg.config.crop.x = hi_ext_af_crop_pos_x_read(vi_pipe);
    stat_cfg->focus_cfg.config.crop.y = hi_ext_af_crop_pos_y_read(vi_pipe);
    stat_cfg->focus_cfg.config.crop.w = hi_ext_af_crop_hsize_read(vi_pipe);
    stat_cfg->focus_cfg.config.crop.h = hi_ext_af_crop_vsize_read(vi_pipe);

    /* AF raw cfg */
    stat_cfg->focus_cfg.config.statistics_pos = hi_ext_af_pos_sel_read(vi_pipe);
    stat_cfg->focus_cfg.config.raw_cfg.gamma_gain_limit = hi_ext_af_gain_limit_read(vi_pipe);
    stat_cfg->focus_cfg.config.raw_cfg.gamma_value = hi_ext_af_gamma_read(vi_pipe);
    stat_cfg->focus_cfg.config.raw_cfg.pattern = hi_ext_af_bayermode_read(vi_pipe);

    /* AF pre median filter */
    stat_cfg->focus_cfg.config.pre_flt_cfg.en = hi_ext_af_mean_enable_read(vi_pipe);
    stat_cfg->focus_cfg.config.pre_flt_cfg.strength = hi_ext_af_mean_thres_read(vi_pipe);

    stat_cfg->focus_cfg.h_param_iir0.ld.ld_en       = hi_ext_af_iir0_ldg_enable_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.ld.th_low    = hi_ext_af_iir_thre0_low_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.ld.th_high   = hi_ext_af_iir_thre0_high_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.ld.slp_low   = hi_ext_af_iir_slope0_low_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.ld.slp_high  = hi_ext_af_iir_slope0_high_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.ld.gain_low  = hi_ext_af_iir_gain0_low_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.ld.gain_high = hi_ext_af_iir_gain0_high_read(vi_pipe);

    stat_cfg->focus_cfg.h_param_iir1.ld.ld_en       = hi_ext_af_iir1_ldg_enable_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.ld.th_low    = hi_ext_af_iir_thre1_low_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.ld.th_high   = hi_ext_af_iir_thre1_high_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.ld.slp_low   = hi_ext_af_iir_slope1_low_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.ld.slp_high  = hi_ext_af_iir_slope1_high_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.ld.gain_low  = hi_ext_af_iir_gain1_low_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.ld.gain_high = hi_ext_af_iir_gain1_high_read(vi_pipe);

    /* level depend gain */
    stat_cfg->focus_cfg.v_param_fir0.ld.ld_en       = hi_ext_af_fir0_ldg_enable_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.ld.th_low    = hi_ext_af_fir_thre0_low_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.ld.th_high   = hi_ext_af_fir_thre0_high_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.ld.slp_low   = hi_ext_af_fir_slope0_low_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.ld.slp_high  = hi_ext_af_fir_slope0_high_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.ld.gain_low  = hi_ext_af_fir_gain0_low_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.ld.gain_high = hi_ext_af_fir_gain0_high_read(vi_pipe);

    stat_cfg->focus_cfg.v_param_fir1.ld.ld_en       = hi_ext_af_fir1_ldg_enable_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.ld.th_low    = hi_ext_af_fir_thre1_low_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.ld.th_high   = hi_ext_af_fir_thre1_high_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.ld.slp_low   = hi_ext_af_fir_slope1_low_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.ld.slp_high  = hi_ext_af_fir_slope1_high_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.ld.gain_low  = hi_ext_af_fir_gain1_low_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.ld.gain_high = hi_ext_af_fir_gain1_high_read(vi_pipe);

    /* AF coring */
    stat_cfg->focus_cfg.h_param_iir0.coring.th  = hi_ext_af_iir_thre0_coring_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.coring.slp = hi_ext_af_iir_slope0_coring_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir0.coring.lmt = hi_ext_af_iir_peak0_coring_read(vi_pipe);

    stat_cfg->focus_cfg.h_param_iir1.coring.th  = hi_ext_af_iir_thre1_coring_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.coring.slp = hi_ext_af_iir_slope1_coring_read(vi_pipe);
    stat_cfg->focus_cfg.h_param_iir1.coring.lmt = hi_ext_af_iir_peak1_coring_read(vi_pipe);

    stat_cfg->focus_cfg.v_param_fir0.coring.th  = hi_ext_af_fir_thre0_coring_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.coring.slp = hi_ext_af_fir_slope0_coring_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir0.coring.lmt = hi_ext_af_fir_peak0_coring_read(vi_pipe);

    stat_cfg->focus_cfg.v_param_fir1.coring.th  = hi_ext_af_fir_thre1_coring_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.coring.slp = hi_ext_af_fir_slope1_coring_read(vi_pipe);
    stat_cfg->focus_cfg.v_param_fir1.coring.lmt = hi_ext_af_fir_peak1_coring_read(vi_pipe);

    /* high luma counter */
    stat_cfg->focus_cfg.config.high_luma_th = hi_ext_af_hiligh_thre_read(vi_pipe);

    stat_cfg->focus_cfg.fv_param.acc_shift_h[0] = hi_ext_af_acc_shift0_h_read(vi_pipe);
    stat_cfg->focus_cfg.fv_param.acc_shift_h[1] = hi_ext_af_acc_shift1_h_read(vi_pipe);
    stat_cfg->focus_cfg.fv_param.acc_shift_v[0] = hi_ext_af_acc_shift0_v_read(vi_pipe);
    stat_cfg->focus_cfg.fv_param.acc_shift_v[1] = hi_ext_af_acc_shift1_v_read(vi_pipe);
    stat_cfg->focus_cfg.fv_param.acc_shift_y = hi_ext_af_acc_shift_y_read(vi_pipe);
    stat_cfg->focus_cfg.fv_param.hl_cnt_shift = hi_ext_af_shift_count_y_read(vi_pipe);
    stat_cfg->focus_cfg.config.h_size = hi_ext_af_input_hsize_read(vi_pipe);
    stat_cfg->focus_cfg.config.v_size = hi_ext_af_input_vsize_read(vi_pipe);
#endif

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_gamma_attr(VI_PIPE vi_pipe, const hi_isp_gamma_attr *gamma_attr)
{
    hi_u32 i = 0;
    hi_u32 wdr_mode;

    const hi_u16 *gamma_lut;
    static hi_u16 gamma033[GAMMA_NODE_NUM] = {
        0, 21, 42, 64, 85, 106, 128, 149, 171, 192, 214, 235, 257, 279, 300, 322, 344, 366, 388, 409, 431, 453, 475, 496, 518, 540, 562, 583, 605, 627, 648, 670,
        691, 712, 734, 755, 776, 797, 818, 839, 860, 881, 902, 922, 943, 963, 984, 1004, 1024, 1044, 1064, 1084, 1104, 1124, 1144, 1164, 1182, 1198, 1213, 1227, 1241, 1256, 1270, 1285,
        1299, 1313, 1328, 1342, 1356, 1370, 1384, 1398, 1412, 1426, 1440, 1453, 1467, 1481, 1494, 1508, 1521, 1535, 1548, 1562, 1575, 1588, 1602, 1615, 1628, 1641, 1654, 1667, 1680, 1693, 1706, 1719,
        1732, 1745, 1758, 1771, 1784, 1797, 1810, 1823, 1836, 1849, 1862, 1875, 1888, 1901, 1914, 1926, 1939, 1952, 1965, 1978, 1991, 2004, 2017, 2030, 2043, 2056, 2069, 2082, 2095, 2109, 2123, 2137,
        2148, 2157, 2164, 2171, 2178, 2185, 2193, 2200, 2208, 2216, 2223, 2231, 2239, 2247, 2255, 2262, 2270, 2278, 2286, 2293, 2301, 2309, 2316, 2324, 2332, 2340, 2348, 2356, 2364, 2372, 2380, 2388,
        2396, 2404, 2412, 2419, 2427, 2435, 2443, 2451, 2459, 2467, 2475, 2483, 2491, 2499, 2507, 2515, 2523, 2531, 2539, 2546, 2554, 2562, 2570, 2578, 2586, 2594, 2602, 2609, 2617, 2625, 2633, 2640,
        2648, 2656, 2664, 2671, 2679, 2687, 2695, 2702, 2710, 2718, 2725, 2733, 2740, 2748, 2755, 2763, 2770, 2778, 2785, 2793, 2800, 2807, 2815, 2822, 2829, 2836, 2844, 2851, 2858, 2865, 2872, 2879,
        2886, 2893, 2900, 2906, 2913, 2920, 2927, 2934, 2941, 2948, 2954, 2961, 2967, 2974, 2980, 2987, 2993, 2999, 3006, 3012, 3018, 3024, 3030, 3036, 3042, 3048, 3054, 3059, 3065, 3071, 3077, 3082,
        3088, 3094, 3099, 3105, 3110, 3115, 3121, 3126, 3131, 3136, 3142, 3147, 3152, 3157, 3162, 3167, 3172, 3177, 3182, 3187, 3192, 3197, 3202, 3206, 3211, 3216, 3220, 3225, 3229, 3234, 3238, 3243,
        3247, 3252, 3256, 3261, 3265, 3269, 3274, 3278, 3282, 3286, 3291, 3295, 3299, 3303, 3307, 3311, 3315, 3319, 3323, 3327, 3331, 3335, 3339, 3342, 3346, 3350, 3354, 3357, 3361, 3365, 3368, 3371,
        3375, 3379, 3383, 3386, 3390, 3394, 3397, 3401, 3404, 3407, 3411, 3414, 3417, 3420, 3424, 3427, 3430, 3433, 3437, 3440, 3443, 3446, 3450, 3453, 3456, 3459, 3462, 3465, 3468, 3471, 3474, 3477,
        3480, 3483, 3486, 3489, 3492, 3495, 3498, 3501, 3504, 3507, 3510, 3512, 3515, 3518, 3521, 3523, 3526, 3529, 3532, 3534, 3537, 3540, 3543, 3545, 3548, 3551, 3554, 3556, 3559, 3562, 3564, 3567,
        3569, 3572, 3574, 3577, 3579, 3582, 3584, 3587, 3589, 3591, 3594, 3596, 3598, 3600, 3603, 3605, 3607, 3609, 3612, 3614, 3616, 3618, 3620, 3622, 3624, 3626, 3628, 3629, 3631, 3633, 3635, 3637,
        3639, 3641, 3643, 3644, 3646, 3648, 3649, 3650, 3652, 3654, 3656, 3657, 3659, 3661, 3662, 3664, 3665, 3667, 3668, 3670, 3671, 3672, 3674, 3675, 3676, 3677, 3678, 3680, 3681, 3682, 3684, 3686,
        3687, 3688, 3690, 3691, 3692, 3693, 3694, 3695, 3696, 3697, 3698, 3700, 3701, 3702, 3704, 3705, 3706, 3707, 3708, 3709, 3710, 3711, 3713, 3714, 3715, 3716, 3717, 3718, 3719, 3720, 3721, 3722,
        3723, 3724, 3726, 3727, 3728, 3729, 3730, 3731, 3732, 3733, 3734, 3735, 3736, 3737, 3739, 3740, 3741, 3742, 3743, 3744, 3745, 3746, 3748, 3749, 3750, 3751, 3752, 3753, 3754, 3755, 3756, 3758,
        3759, 3760, 3762, 3763, 3764, 3765, 3767, 3768, 3769, 3770, 3771, 3772, 3773, 3774, 3776, 3777, 3778, 3779, 3780, 3781, 3782, 3783, 3785, 3786, 3787, 3788, 3789, 3790, 3791, 3792, 3793, 3794,
        3795, 3796, 3797, 3798, 3799, 3800, 3801, 3802, 3803, 3804, 3805, 3806, 3807, 3808, 3809, 3810, 3811, 3812, 3813, 3814, 3815, 3816, 3817, 3818, 3819, 3820, 3821, 3822, 3823, 3824, 3825, 3825,
        3826, 3827, 3828, 3829, 3830, 3831, 3832, 3833, 3834, 3835, 3836, 3836, 3837, 3838, 3839, 3840, 3841, 3842, 3843, 3843, 3844, 3845, 3845, 3846, 3847, 3848, 3849, 3850, 3851, 3852, 3853, 3853,
        3854, 3855, 3855, 3856, 3857, 3858, 3859, 3860, 3861, 3862, 3863, 3863, 3864, 3865, 3866, 3866, 3867, 3868, 3868, 3869, 3870, 3871, 3872, 3873, 3874, 3875, 3876, 3876, 3877, 3878, 3879, 3879,
        3880, 3881, 3882, 3882, 3883, 3884, 3885, 3885, 3886, 3887, 3888, 3888, 3889, 3890, 3891, 3891, 3892, 3893, 3894, 3894, 3895, 3896, 3897, 3897, 3898, 3899, 3899, 3899, 3900, 3901, 3901, 3902,
        3903, 3904, 3905, 3905, 3906, 3907, 3907, 3907, 3908, 3909, 3910, 3910, 3911, 3912, 3912, 3912, 3913, 3914, 3915, 3915, 3916, 3917, 3917, 3917, 3918, 3919, 3920, 3920, 3921, 3922, 3922, 3923,
        3923, 3923, 3924, 3924, 3925, 3926, 3927, 3927, 3928, 3929, 3929, 3930, 3930, 3930, 3931, 3931, 3932, 3933, 3934, 3934, 3935, 3936, 3936, 3937, 3937, 3937, 3938, 3938, 3939, 3940, 3941, 3941,
        3942, 3943, 3943, 3944, 3944, 3944, 3945, 3945, 3946, 3947, 3948, 3948, 3949, 3950, 3950, 3950, 3951, 3952, 3953, 3953, 3954, 3955, 3955, 3955, 3956, 3957, 3958, 3958, 3959, 3960, 3960, 3960,
        3961, 3962, 3963, 3963, 3964, 3965, 3965, 3965, 3966, 3967, 3968, 3968, 3969, 3970, 3970, 3970, 3971, 3972, 3973, 3973, 3974, 3975, 3975, 3975, 3976, 3977, 3977, 3978, 3979, 3980, 3981, 3981,
        3982, 3983, 3983, 3983, 3984, 3985, 3985, 3986, 3987, 3988, 3989, 3989, 3990, 3991, 3991, 3991, 3992, 3993, 3994, 3994, 3995, 3996, 3996, 3996, 3997, 3998, 3998, 3999, 4000, 4001, 4002, 4002,
        4003, 4004, 4004, 4004, 4005, 4006, 4007, 4007, 4008, 4009, 4009, 4009, 4010, 4011, 4012, 4012, 4013, 4014, 4014, 4014, 4015, 4016, 4017, 4017, 4018, 4019, 4019, 4019, 4020, 4021, 4022, 4022,
        4023, 4024, 4024, 4024, 4025, 4026, 4027, 4027, 4028, 4029, 4029, 4030, 4030, 4030, 4031, 4031, 4032, 4033, 4034, 4034, 4035, 4036, 4036, 4037, 4037, 4038, 4038, 4039, 4039, 4040, 4040, 4041,
        4041, 4042, 4042, 4043, 4043, 4044, 4044, 4045, 4045, 4046, 4046, 4047, 4047, 4048, 4048, 4049, 4049, 4050, 4050, 4051, 4051, 4052, 4052, 4053, 4053, 4054, 4054, 4055, 4055, 4055, 4056, 4056,
        4056, 4056, 4057, 4057, 4058, 4059, 4059, 4060, 4060, 4060, 4061, 4061, 4061, 4061, 4062, 4062, 4063, 4064, 4064, 4065, 4065, 4065, 4066, 4066, 4066, 4066, 4067, 4067, 4068, 4069, 4069, 4070,
        4070, 4070, 4071, 4071, 4071, 4071, 4072, 4073, 4073, 4073, 4074, 4074, 4074, 4074, 4075, 4076, 4076, 4076, 4077, 4077, 4077, 4077, 4078, 4078, 4079, 4080, 4080, 4081, 4081, 4081, 4082, 4082,
        4082, 4082, 4083, 4084, 4084, 4084, 4085, 4085, 4085, 4085, 4086, 4087, 4087, 4087, 4088, 4088, 4088, 4088, 4089, 4089, 4090, 4091, 4091, 4092, 4092, 4092, 4093, 4093, 4093, 4093, 4094, 4094,
        4095

    };

    static hi_u16 gamma_def[GAMMA_NODE_NUM] = {
        0,   30,  60,  90,  120, 145, 170, 195, 220, 243, 265, 288, 310, 330, 350, 370, 390, 410, 430, 450, 470, 488, 505, 523, 540, 558, 575, 593, 610, 625, 640, 655, 670, 685, 700,
        715, 730, 744, 758, 772, 786, 800, 814, 828, 842, 855, 868, 881, 894, 907, 919, 932, 944, 957, 969, 982, 994, 1008,    1022,    1036,    1050,   1062,    1073,    1085,
        1096,    1107,    1117,    1128,    1138,    1148,    1158,    1168,    1178,    1188,    1198,    1208,    1218,    1227,    1236,    1245,    1254,    1261,
        1267,    1274,    1280,    1289,    1297,    1306,    1314,    1322,    1330,    1338,    1346,    1354,    1362,    1370,    1378,    1386,    1393,    1401,
        1408,    1416,    1423,    1431,    1438,    1445,    1453,    1460,    1467,    1474,    1480,    1487,    1493,    1500,    1506,    1513,    1519,    1525,
        1531,    1537,    1543,    1549,    1556,    1562,    1568,    1574,    1580,    1586,    1592,    1598,    1604,    1609,    1615,    1621,    1627,    1632,
        1638,    1644,    1650,    1655,    1661,    1667,    1672,    1678,    1683,    1689,    1694,    1700,    1705,    1710,    1716,    1721,    1726,    1732,
        1737,    1743,    1748,    1753,    1759,    1764,    1769,    1774,    1779,    1784,    1789,    1794,    1800,    1805,    1810,    1815,    1820,    1825,
        1830,    1835,    1840,    1844,    1849,    1854,    1859,    1864,    1869,    1874,    1879,    1883,    1888,    1893,    1898,    1902,    1907,    1912,
        1917,    1921,    1926,    1931,    1936,    1940,    1945,    1950,    1954,    1959,    1963,    1968,    1972,    1977,    1981,    1986,    1990,    1995,
        1999,    2004,    2008,    2013,    2017,    2021,    2026,    2030,    2034,    2039,    2043,    2048,    2052,    2056,    2061,    2065,    2069,    2073,
        2078,    2082,    2086,    2090,    2094,    2098,    2102,    2106,    2111,    2115,    2119,    2123,    2128,    2132,    2136,    2140,    2144,    2148,
        2152,    2156,    2160,    2164,    2168,    2172,    2176,    2180,    2184,    2188,    2192,    2196,    2200,    2204,    2208,    2212,    2216,    2220,
        2224,    2227,    2231,    2235,    2239,    2243,    2247,    2251,    2255,    2258,    2262,    2266,    2270,    2273,    2277,    2281,    2285,    2288,
        2292,    2296,    2300,    2303,    2307,    2311,    2315,    2318,    2322,    2326,    2330,    2333,    2337,    2341,    2344,    2348,    2351,    2355,
        2359,    2362,    2366,    2370,    2373,    2377,    2380,    2384,    2387,    2391,    2394,    2398,    2401,    2405,    2408,    2412,    2415,    2419,
        2422,    2426,    2429,    2433,    2436,    2440,    2443,    2447,    2450,    2454,    2457,    2461,    2464,    2467,    2471,    2474,    2477,    2481,
        2484,    2488,    2491,    2494,    2498,    2501,    2504,    2508,    2511,    2515,    2518,    2521,    2525,    2528,    2531,    2534,    2538,    2541,
        2544,    2547,    2551,    2554,    2557,    2560,    2564,    2567,    2570,    2573,    2577,    2580,    2583,    2586,    2590,    2593,    2596,    2599,
        2603,    2606,    2609,    2612,    2615,    2618,    2621,    2624,    2628,    2631,    2634,    2637,    2640,    2643,    2646,    2649,    2653,    2656,
        2659,    2662,    2665,    2668,    2671,    2674,    2677,    2680,    2683,    2686,    2690,    2693,    2696,    2699,    2702,    2705,    2708,    2711,
        2714,    2717,    2720,    2723,    2726,    2729,    2732,    2735,    2738,    2741,    2744,    2747,    2750,    2753,    2756,    2759,    2762,    2764,
        2767,    2770,    2773,    2776,    2779,    2782,    2785,    2788,    2791,    2794,    2797,    2799,    2802,    2805,    2808,    2811,    2814,    2817,
        2820,    2822,    2825,    2828,    2831,    2834,    2837,    2840,    2843,    2845,    2848,    2851,    2854,    2856,    2859,    2862,    2865,    2868,
        2871,    2874,    2877,    2879,    2882,    2885,    2888,    2890,    2893,    2896,    2899,    2901,    2904,    2907,    2910,    2912,    2915,    2918,
        2921,    2923,    2926,    2929,    2932,    2934,    2937,    2940,    2943,    2945,    2948,    2951,    2954,    2956,    2959,    2962,    2964,    2967,
        2969,    2972,    2975,    2977,    2980,    2983,    2986,    2988,    2991,    2994,    2996,    2999,    3001,    3004,    3007,    3009,    3012,    3015,
        3018,    3020,    3023,    3026,    3028,    3031,    3033,    3036,    3038,    3041,    3043,    3046,    3049,    3051,    3054,    3057,    3059,    3062,
        3064,    3067,    3069,    3072,    3074,    3077,    3080,    3082,    3085,    3088,    3090,    3093,    3095,    3098,    3100,    3103,    3105,    3108,
        3110,    3113,    3115,    3118,    3120,    3123,    3125,    3128,    3130,    3133,    3135,    3138,    3140,    3143,    3145,    3148,    3150,    3153,
        3155,    3158,    3160,    3163,    3165,    3168,    3170,    3173,    3175,    3178,    3180,    3183,    3185,    3187,    3190,    3192,    3194,    3197,
        3199,    3202,    3204,    3207,    3209,    3212,    3214,    3217,    3219,    3222,    3224,    3226,    3229,    3231,    3233,    3236,    3238,    3241,
        3243,    3245,    3248,    3250,    3252,    3255,    3257,    3260,    3262,    3264,    3267,    3269,    3271,    3274,    3276,    3279,    3281,    3283,
        3286,    3288,    3290,    3293,    3295,    3298,    3300,    3302,    3305,    3307,    3309,    3311,    3314,    3316,    3318,    3320,    3323,    3325,
        3327,    3330,    3332,    3335,    3337,    3339,    3342,    3344,    3346,    3348,    3351,    3353,    3355,    3357,    3360,    3362,    3364,    3366,
        3369,    3371,    3373,    3375,    3378,    3380,    3382,    3384,    3387,    3389,    3391,    3393,    3396,    3398,    3400,    3402,    3405,    3407,
        3409,    3411,    3414,    3416,    3418,    3420,    3423,    3425,    3427,    3429,    3432,    3434,    3436,    3438,    3441,    3443,    3445,    3447,
        3450,    3452,    3454,    3456,    3459,    3461,    3463,    3465,    3467,    3469,    3471,    3473,    3476,    3478,    3480,    3482,    3485,    3487,
        3489,    3491,    3494,    3496,    3498,    3500,    3502,    3504,    3506,    3508,    3511,    3513,    3515,    3517,    3519,    3521,    3523,    3525,
        3528,    3530,    3532,    3534,    3536,    3538,    3540,    3542,    3545,    3547,    3549,    3551,    3553,    3555,    3557,    3559,    3562,    3564,
        3566,    3568,    3570,    3572,    3574,    3576,    3579,    3581,    3583,    3585,    3587,    3589,    3591,    3593,    3596,    3598,    3600,    3602,
        3604,    3606,    3608,    3610,    3612,    3614,    3616,    3618,    3620,    3622,    3624,    3626,    3629,    3631,    3633,    3635,    3637,    3639,
        3641,    3643,    3645,    3647,    3649,    3651,    3653,    3655,    3657,    3659,    3661,    3663,    3665,    3667,    3670,    3672,    3674,    3676,
        3678,    3680,    3682,    3684,    3686,    3688,    3690,    3692,    3694,    3696,    3698,    3700,    3702,    3704,    3706,    3708,    3710,    3712,
        3714,    3716,    3718,    3720,    3722,    3724,    3726,    3728,    3730,    3732,    3734,    3736,    3738,    3740,    3742,    3744,    3746,    3748,
        3750,    3752,    3754,    3756,    3758,    3760,    3762,    3764,    3766,    3767,    3769,    3771,    3773,    3775,    3777,    3779,    3781,    3783,
        3785,    3787,    3789,    3791,    3793,    3795,    3797,    3799,    3801,    3803,    3805,    3806,    3808,    3810,    3812,    3814,    3816,    3818,
        3820,    3822,    3824,    3826,    3828,    3830,    3832,    3834,    3836,    3837,    3839,    3841,    3843,    3845,    3847,    3849,    3851,    3853,
        3855,    3857,    3859,    3860,    3862,    3864,    3866,    3868,    3870,    3872,    3874,    3875,    3877,    3879,    3881,    3883,    3885,    3887,
        3889,    3890,    3892,    3894,    3896,    3898,    3900,    3902,    3904,    3905,    3907,    3909,    3911,    3913,    3915,    3917,    3919,    3920,
        3922,    3924,    3926,    3928,    3930,    3932,    3934,    3935,    3937,    3939,    3941,    3943,    3945,    3947,    3949,    3950,    3952,    3954,
        3956,    3957,    3959,    3961,    3963,    3965,    3967,    3969,    3971,    3972,    3974,    3976,    3978,    3979,    3981,    3983,    3985,    3987,
        3989,    3991,    3993,    3994,    3996,    3998,    4000,    4001,    4003,    4005,    4007,    4008,    4010,    4012,    4014,    4016,    4018,    4020,
        4022,    4023,    4025,    4027,    4029,    4030,    4032,    4034,    4036,    4037,    4039,    4041,    4043,    4044,    4046,    4048,    4050,    4052,
        4054,    4056,    4058,    4059,    4061,    4063,    4065,    4066,    4068,    4070,    4072,    4073,    4075,    4077,    4079,    4080,    4082,    4084,
        4086,    4087,    4089,    4091,    4092,    4094,    4095

    };
    static hi_u16 gammas_rgb[GAMMA_NODE_NUM] = {
        0,   18,  36,  54,  72,  90,  108, 126, 144, 162, 180, 198, 216, 234, 252, 270, 288, 306, 324, 343, 360, 377, 394, 410, 426, 441, 456, 471, 486, 500, 514, 527,
        541, 554, 567, 580, 592, 605, 617, 629, 641, 652, 664, 675, 686, 698, 709, 719, 730, 741, 751, 761, 772, 782, 792, 802, 812, 821, 831, 841, 850, 859, 869, 878,
        887, 896, 905, 914, 923, 931, 940, 949, 957, 966, 974, 983, 991, 999, 1007, 1015, 1024, 1032, 1039, 1047, 1055, 1063, 1071, 1078, 1086, 1094, 1101, 1109, 1116, 1124, 1131, 1138,
        1146, 1153, 1160, 1167, 1174, 1182, 1189, 1196, 1203, 1210, 1216, 1223, 1230, 1237, 1244, 1250, 1257, 1264, 1270, 1277, 1284, 1290, 1297, 1303, 1310, 1316, 1322, 1329, 1335, 1341, 1348, 1354,
        1360, 1366, 1372, 1379, 1385, 1391, 1397, 1403, 1409, 1415, 1421, 1427, 1433, 1439, 1444, 1450, 1456, 1462, 1468, 1474, 1479, 1485, 1491, 1496, 1502, 1508, 1513, 1519, 1524, 1530, 1536, 1541,
        1547, 1552, 1557, 1563, 1568, 1574, 1579, 1585, 1590, 1595, 1601, 1606, 1611, 1616, 1622, 1627, 1632, 1637, 1642, 1648, 1653, 1658, 1663, 1668, 1673, 1678, 1683, 1688, 1693, 1698, 1703, 1708,
        1713, 1718, 1723, 1728, 1733, 1738, 1743, 1748, 1753, 1758, 1762, 1767, 1772, 1777, 1782, 1786, 1791, 1796, 1801, 1805, 1810, 1815, 1819, 1824, 1829, 1833, 1838, 1843, 1847, 1852, 1857, 1861,
        1866, 1870, 1875, 1879, 1884, 1888, 1893, 1897, 1902, 1906, 1911, 1915, 1920, 1924, 1928, 1933, 1937, 1942, 1946, 1950, 1955, 1959, 1963, 1968, 1972, 1976, 1981, 1985, 1989, 1994, 1998, 2002,
        2006, 2011, 2015, 2019, 2023, 2027, 2032, 2036, 2040, 2044, 2048, 2052, 2057, 2061, 2065, 2069, 2073, 2077, 2081, 2085, 2089, 2093, 2097, 2102, 2106, 2110, 2114, 2118, 2122, 2126, 2130, 2134,
        2138, 2142, 2146, 2149, 2153, 2157, 2161, 2165, 2169, 2173, 2177, 2181, 2185, 2189, 2192, 2196, 2200, 2204, 2208, 2212, 2216, 2219, 2223, 2227, 2231, 2235, 2238, 2242, 2246, 2250, 2254, 2257,
        2261, 2265, 2269, 2272, 2276, 2280, 2283, 2287, 2291, 2295, 2298, 2302, 2306, 2309, 2313, 2317, 2320, 2324, 2328, 2331, 2335, 2338, 2342, 2346, 2349, 2353, 2356, 2360, 2364, 2367, 2371, 2374,
        2378, 2381, 2385, 2389, 2392, 2396, 2399, 2403, 2406, 2410, 2413, 2417, 2420, 2424, 2427, 2431, 2434, 2438, 2441, 2445, 2448, 2451, 2455, 2458, 2462, 2465, 2469, 2472, 2475, 2479, 2482, 2486,
        2489, 2492, 2496, 2499, 2503, 2506, 2509, 2513, 2516, 2519, 2523, 2526, 2529, 2533, 2536, 2539, 2543, 2546, 2549, 2553, 2556, 2559, 2563, 2566, 2569, 2572, 2576, 2579, 2582, 2585, 2589, 2592,
        2595, 2598, 2602, 2605, 2608, 2611, 2615, 2618, 2621, 2624, 2627, 2631, 2634, 2637, 2640, 2643, 2647, 2650, 2653, 2656, 2659, 2662, 2666, 2669, 2672, 2675, 2678, 2681, 2684, 2688, 2691, 2694,
        2697, 2700, 2703, 2706, 2709, 2712, 2716, 2719, 2722, 2725, 2728, 2731, 2734, 2737, 2740, 2743, 2746, 2749, 2752, 2755, 2759, 2762, 2765, 2768, 2771, 2774, 2777, 2780, 2783, 2786, 2789, 2792,
        2795, 2798, 2801, 2804, 2807, 2810, 2813, 2816, 2819, 2822, 2825, 2828, 2831, 2833, 2836, 2839, 2842, 2845, 2848, 2851, 2854, 2857, 2860, 2863, 2866, 2869, 2872, 2875, 2877, 2880, 2883, 2886,
        2889, 2892, 2895, 2898, 2901, 2904, 2906, 2909, 2912, 2915, 2918, 2921, 2924, 2926, 2929, 2932, 2935, 2938, 2941, 2944, 2946, 2949, 2952, 2955, 2958, 2961, 2963, 2966, 2969, 2972, 2975, 2977,
        2980, 2983, 2986, 2989, 2991, 2994, 2997, 3000, 3003, 3005, 3008, 3011, 3014, 3016, 3019, 3022, 3025, 3027, 3030, 3033, 3036, 3038, 3041, 3044, 3047, 3049, 3052, 3055, 3058, 3060, 3063, 3066,
        3068, 3071, 3074, 3077, 3079, 3082, 3085, 3087, 3090, 3093, 3095, 3098, 3101, 3103, 3106, 3109, 3112, 3114, 3117, 3120, 3122, 3125, 3128, 3130, 3133, 3135, 3138, 3141, 3143, 3146, 3149, 3151,
        3154, 3157, 3159, 3162, 3164, 3167, 3170, 3172, 3175, 3178, 3180, 3183, 3185, 3188, 3191, 3193, 3196, 3198, 3201, 3204, 3206, 3209, 3211, 3214, 3217, 3219, 3222, 3224, 3227, 3229, 3232, 3235,
        3237, 3240, 3242, 3245, 3247, 3250, 3252, 3255, 3257, 3260, 3263, 3265, 3268, 3270, 3273, 3275, 3278, 3280, 3283, 3285, 3288, 3290, 3293, 3295, 3298, 3300, 3303, 3305, 3308, 3310, 3313, 3315,
        3318, 3320, 3323, 3325, 3328, 3330, 3333, 3335, 3338, 3340, 3343, 3345, 3348, 3350, 3353, 3355, 3358, 3360, 3362, 3365, 3367, 3370, 3372, 3375, 3377, 3380, 3382, 3385, 3387, 3389, 3392, 3394,
        3397, 3399, 3402, 3404, 3406, 3409, 3411, 3414, 3416, 3418, 3421, 3423, 3426, 3428, 3431, 3433, 3435, 3438, 3440, 3443, 3445, 3447, 3450, 3452, 3454, 3457, 3459, 3462, 3464, 3466, 3469, 3471,
        3474, 3476, 3478, 3481, 3483, 3485, 3488, 3490, 3492, 3495, 3497, 3500, 3502, 3504, 3507, 3509, 3511, 3514, 3516, 3518, 3521, 3523, 3525, 3528, 3530, 3532, 3535, 3537, 3539, 3542, 3544, 3546,
        3549, 3551, 3553, 3555, 3558, 3560, 3562, 3565, 3567, 3569, 3572, 3574, 3576, 3579, 3581, 3583, 3585, 3588, 3590, 3592, 3595, 3597, 3599, 3601, 3604, 3606, 3608, 3610, 3613, 3615, 3617, 3620,
        3622, 3624, 3626, 3629, 3631, 3633, 3635, 3638, 3640, 3642, 3644, 3647, 3649, 3651, 3653, 3656, 3658, 3660, 3662, 3665, 3667, 3669, 3671, 3674, 3676, 3678, 3680, 3682, 3685, 3687, 3689, 3691,
        3694, 3696, 3698, 3700, 3702, 3705, 3707, 3709, 3711, 3713, 3716, 3718, 3720, 3722, 3724, 3727, 3729, 3731, 3733, 3735, 3738, 3740, 3742, 3744, 3746, 3749, 3751, 3753, 3755, 3757, 3759, 3762,
        3764, 3766, 3768, 3770, 3772, 3775, 3777, 3779, 3781, 3783, 3785, 3788, 3790, 3792, 3794, 3796, 3798, 3800, 3803, 3805, 3807, 3809, 3811, 3813, 3815, 3818, 3820, 3822, 3824, 3826, 3828, 3830,
        3833, 3835, 3837, 3839, 3841, 3843, 3845, 3847, 3850, 3852, 3854, 3856, 3858, 3860, 3862, 3864, 3866, 3869, 3871, 3873, 3875, 3877, 3879, 3881, 3883, 3885, 3887, 3890, 3892, 3894, 3896, 3898,
        3900, 3902, 3904, 3906, 3908, 3910, 3912, 3915, 3917, 3919, 3921, 3923, 3925, 3927, 3929, 3931, 3933, 3935, 3937, 3939, 3942, 3944, 3946, 3948, 3950, 3952, 3954, 3956, 3958, 3960, 3962, 3964,
        3966, 3968, 3970, 3972, 3974, 3976, 3978, 3980, 3983, 3985, 3987, 3989, 3991, 3993, 3995, 3997, 3999, 4001, 4003, 4005, 4007, 4009, 4011, 4013, 4015, 4017, 4019, 4021, 4023, 4025, 4027, 4029,
        4031, 4033, 4035, 4037, 4039, 4041, 4043, 4045, 4047, 4049, 4051, 4053, 4055, 4057, 4059, 4061, 4063, 4065, 4067, 4069, 4071, 4073, 4075, 4077, 4079, 4081, 4083, 4085, 4087, 4089, 4091, 4093,
        4095
    };

    static hi_u16 gamma_def_wdr[GAMMA_NODE_NUM] = {
        0,      3,      6,      8,      11,     14,     17,     20,     23,     26,     29,     32,     35,     38,     41,     44,     47,     50,     53,     56,     59,     62,     65,     68,
        71,     74,     77,     79,     82,     85,     88,     91,     94,     97,     100,    103,    106,    109,    112,    114,    117,    120,    123,    126,    129,    132,    135,    138,
        141,    144,    147,    149,    152,    155,    158,    161,    164,    167,    170,    172,    175,    178,    181,    184,    187,    190,    193,    195,    198,    201,    204,    207,
        210,    213,    216,    218,    221,    224,    227,    230,    233,    236,    239,    241,    244,    247,    250,    253,    256,    259,    262,    264,    267,    270,    273,    276,
        279,    282,    285,    287,    290,    293,    296,    299,    302,    305,    308,    310,    313,    316,    319,    322,    325,    328,    331,    333,    336,    339,    342,    345,
        348,    351,    354,    356,    359,    362,    365,    368,    371,    374,    377,    380,    383,    386,    389,    391,    394,    397,    400,    403,    406,    409,    412,    415,
        418,    421,    424,    426,    429,    432,    435,    438,    441,    444,    447,    450,    453,    456,    459,    462,    465,    468,    471,    474,    477,    480,    483,    486,
        489,    492,    495,    498,    501,    504,    507,    510,    513,    516,    519,    522,    525,    528,    531,    534,    537,    540,    543,    546,    549,    552,    555,    558,
        561,    564,    568,    571,    574,    577,    580,    583,    586,    589,    592,    595,    598,    601,    605,    608,    611,    614,    617,    620,    623,    626,    630,    633,
        636,    639,    643,    646,    649,    652,    655,    658,    661,    664,    668,    671,    674,    677,    681,    684,    687,    690,    694,    697,    700,    703,    707,    710,
        713,    716,    720,    723,    726,    730,    733,    737,    740,    743,    747,    750,    753,    757,    760,    764,    767,    770,    774,    777,    780,    784,    787,    791,
        794,    797,    801,    804,    807,    811,    814,    818,    821,    825,    828,    832,    835,    838,    842,    845,    848,    852,    855,    859,    862,    866,     869,   873,
        876,    880,    883,    887,    890,    894,    897,    901,    904,    908,    911,    915,    918,    922,    925,    929,    932,    936,    939,    943,    946,    950,    954,    957,
        961,    965,    968,    972,    975,    979,    982,    986,    989,    993,    997,    1000,   1004,   1008,   1011,   1015,   1018,   1022,   1026,   1029,   1033,   1037,   1040,   1044,
        1047,   1051,   1055,   1058,   1062,   1066,   1069,   1073,   1076,   1080,   1084,   1087,   1091,   1095,   1099,   1102,   1106,   1110,   1113,   1117,   1120,   1124,   1128,   1131,
        1135,   1139,   1143,   1146,   1150,   1154,   1158,   1161,   1165,   1169,   1173,   1176,   1180,   1184,   1188,   1191,   1195,   1199,   1203,   1206,   1210,   1214,   1218,   1221,
        1225,   1229,   1233,   1236,   1240,   1244,   1248,   1251,   1255,   1259,   1263,   1266,   1270,   1274,   1278,   1281,   1285,   1289,   1293,   1297,   1301,   1305,   1309,   1312,
        1316,   1320,   1324,   1327,   1331,   1335,   1339,   1343,   1347,   1351,   1355,   1358,   1362,   1366,   1370,   1373,   1377,   1381,   1385,   1389,   1393,   1397,   1401,   1404,
        1408,   1412,   1416,   1420,   1424,   1428,   1432,   1435,   1439,   1443,   1447,   1451,   1455,   1459,   1463,   1467,   1471,   1475,   1479,   1482,   1486,   1490,   1494,   1498,
        1502,   1506,   1510,   1514,   1518,   1522,   1526,   1529,   1533,   1537,   1541,   1545,   1549,   1553,   1557,   1561,   1565,   1569,   1573,   1577,   1581,   1585,   1589,   1593,
        1597,   1601,   1605,   1609,   1613,   1617,   1621,   1624,   1628,   1632,   1636,   1640,   1644,   1648,   1652,   1656,   1660,   1664,   1668,   1672,   1676,   1680,   1684,   1688,
        1692,   1696,   1700,   1704,   1708,   1712,   1717,   1721,   1725,   1729,   1733,   1737,   1741,   1745,   1749,   1753,   1757,   1761,   1765,   1769,   1773,   1777,   1781,   1785,
        1789,   1793,   1797,   1801,   1805,   1809,   1813,   1817,   1821,   1825,   1830,   1834,   1838,   1842,   1846,   1850,   1854,   1858,   1862,   1866,   1870,   1874,   1879,   1883,
        1887,   1891,   1895,   1899,   1903,   1907,   1911,   1915,   1919,   1923,   1928,   1932,   1936,   1940,   1944,   1948,   1952,   1956,   1961,   1965,   1969,   1973,   1977,   1981,
        1985,   1989,   1994,   1998,   2002,   2006,   2010,   2014,   2018,   2022,   2027,   2031,   2035,   2039,   2044,   2048,   2052,   2056,   2060,   2064,   2068,   2072,   2077,   2081,
        2085,   2089,   2094,   2098,   2102,   2106,   2111,   2115,   2119,   2123,   2128,   2132,   2136,   2140,   2144,   2148,   2152,   2156,   2161,   2165,   2169,   2173,   2178,   2182,
        2186,   2190,   2195,   2199,   2203,   2207,   2212,   2216,   2220,   2224,   2229,   2233,   2237,   2241,   2246,   2250,   2254,   2259,   2263,   2268,   2272,   2276,   2281,   2285,
        2289,   2293,   2298,   2302,   2306,   2310,   2315,   2319,   2323,   2327,   2332,   2336,   2340,   2345,   2349,   2354,   2358,   2362,   2367,   2371,   2375,   2380,   2384,   2389,
        2393,   2397,   2402,   2406,   2410,   2415,   2419,   2424,   2428,   2432,   2437,   2441,   2445,   2450,   2454,   2459,   2463,   2467,   2472,   2476,   2480,   2485,   2489,   2494,
        2498,   2503,   2507,   2512,   2516,   2520,   2525,   2529,   2533,   2538,   2542,   2547,   2551,   2556,   2560,   2565,   2569,   2574,   2578,   2583,   2587,   2592,   2596,   2601,
        2605,   2610,   2614,   2619,   2623,   2628,   2632,   2637,   2641,   2646,   2650,   2655,   2659,   2664,   2668,   2673,   2677,   2682,   2686,   2691,   2695,   2700,   2704,   2709,
        2713,   2718,   2723,   2727,   2732,   2737,   2741,   2746,   2750,   2755,   2759,   2764,   2768,   2773,   2778,   2782,   2787,   2792,   2796,   2801,   2805,   2810,   2815,   2819,
        2824,   2829,   2833,   2838,   2842,   2847,   2852,   2856,   2861,   2866,   2870,   2875,   2879,   2884,   2889,   2893,   2898,   2903,   2908,   2912,   2917,   2922,   2927,   2931,
        2936,   2941,   2946,   2950,   2955,   2960,   2965,   2969,   2974,   2979,   2984,   2988,   2993,   2998,   3003,   3008,   3013,   3018,   3023,   3027,   3032,   3037,   3042,   3046,
        3051,   3056,   3061,   3066,   3071,   3076,   3081,   3085,   3090,   3095,   3100,   3105,   3110,   3115,   3120,   3124,   3129,   3134,   3139,   3144,   3149,   3154,   3159,   3163,
        3168,   3173,   3178,   3183,   3188,   3193,   3198,   3203,   3208,   3213,   3218,   3223,   3228,   3233,   3238,   3243,   3248,   3253,   3258,   3263,   3268,   3273,   3278,   3283,
        3288,   3293,   3298,   3303,   3308,   3313,   3318,   3323,   3328,   3333,   3338,   3343,   3348,   3353,   3358,   3363,   3368,   3373,   3378,   3383,   3388,   3393,   3398,   3403,
        3408,   3413,   3418,   3423,   3428,   3433,   3438,   3443,   3448,   3453,   3458,   3463,   3468,   3473,   3479,   3484,   3489,   3494,   3499,   3504,   3509,   3514,   3519,   3524,
        3529,   3534,   3539,   3544,   3549,   3554,   3560,   3565,   3570,   3575,   3580,   3585,   3590,   3595,   3600,   3605,   3610,   3615,   3621,   3626,   3631,   3636,   3641,   3646,
        3651,   3656,   3661,   3666,   3671,   3676,   3682,   3687,   3692,   3697,   3702,   3707,   3712,   3717,   3722,   3727,   3732,   3737,   3742,   3747,   3752,   3757,   3763,   3768,
        3773,   3778,   3783,   3788,   3793,   3798,   3803,   3808,   3813,   3818,   3824,   3829,   3834,   3839,   3844,   3849,   3854,   3859,   3864,   3869,   3874,   3879,   3884,   3889,
        3894,   3899,   3904,   3909,   3914,   3919,   3924,   3929,   3934,   3939,   3945,   3950,   3955,   3960,   3965,   3970,   3975,   3980,   3985,   3990,   3995,   4000,   4005,   4010,
        4015,   4020,   4025,   4030,   4035,   4040,   4045,   4050,   4055,   4060,   4065,   4070,   4075,   4080,   4085,   4090,   4095

    };
    // HDR mode is only supported in hi3559_av100
    static hi_u16 gamma_hdr[GAMMA_NODE_NUM] = {
        0,   42,   67,     87,     105,    122,    138,    152,    167,    180,    193,    205,    218,    229,    241,    252,    263,    274,    284,    295,    305,    315,    325,    334,   344,
        353,   363,   372,    381,    390,    398,    407,    416,    424,    433,    441,    449,    458,    466,    474,    482,    490,    498,    505,    513,    521,    528,   536,   543,   551,
        558,   566,   573,    580,    587,    594,    602,    609,    616,    623,    630,    636,    643,    650,    657,    664,    670,    677,    684,    690,    697,    704,   710,   717,   723,
        729,   736,   742,    749,    755,    761,    767,    774,    780,    786,    792,    798,    805,    811,    817,    823,    829,    835,    841,    847,    853,    859,   864,   870,   876,
        882,   888,   894,    899,    905,    911,    917,    922,    928,    934,    939,    945,    950,    956,    962,    967,    973,    978,    984,    989,    995,    1000,   1006,   1011,   1017,
        1022,   1027,   1033,   1038,   1043,   1049,   1054,   1059,   1065,   1070,   1075,   1080,   1086,   1091,   1096,   1101,   1106,   1112,   1117,   1122,   1127,   1132,   1137,   1142,   1148,
        1153,   1158,   1163,   1168,   1173,   1178,   1183,   1188,   1193,   1198,   1203,   1208,   1213,   1218,   1223,   1227,   1232,   1237,   1242,   1247,   1252,   1257,   1262,   1266,   1271,
        1276,   1281,   1286,   1290,   1295,   1300,   1305,   1309,   1314,   1319,   1324,   1328,   1333,   1338,   1343,   1347,   1352,   1357,   1361,   1366,   1370,   1375,   1380,   1384,   1389,
        1394,   1398,   1403,   1407,   1412,   1416,   1421,   1426,   1430,   1435,   1439,   1444,   1448,   1453,   1457,   1462,   1466,   1471,   1475,   1480,   1484,   1489,   1493,   1497,   1502,
        1506,   1511,   1515,   1519,   1524,   1528,   1533,   1537,   1541,   1546,   1550,   1554,   1559,   1563,   1567,   1572,   1576,   1580,   1585,   1589,   1593,   1598,   1602,   1606,   1610,
        1615,   1619,   1623,   1627,   1632,   1636,   1640,   1644,   1649,   1653,   1657,   1661,   1665,   1670,   1674,   1678,   1682,   1686,   1691,   1695,   1699,   1703,   1707,   1711,   1715,
        1720,   1724,   1728,   1732,   1736,   1740,   1744,   1748,   1752,   1756,   1761,   1765,   1769,   1773,   1777,   1781,   1785,   1789,   1793,   1797,   1801,   1805,   1809,   1813,   1817,
        1821,   1825,   1829,   1833,   1837,   1841,   1845,   1849,   1853,   1857,   1861,   1865,   1869,   1873,   1877,   1881,   1885,   1889,   1893,   1897,   1900,   1904,   1908,   1912,   1916,
        1920,   1924,   1928,   1932,   1936,   1939,   1943,   1947,   1951,   1955,   1959,   1963,   1966,   1970,   1974,   1978,   1982,   1986,   1990,   1993,   1997,   2001,   2005,   2009,   2012,
        2016,   2020,   2024,   2028,   2031,   2035,   2039,   2043,   2047,   2050,   2054,   2058,   2062,   2065,   2069,   2073,   2077,   2080,   2084,   2088,   2092,   2095,   2099,   2103,   2106,
        2110,   2114,   2118,   2121,   2125,   2129,   2132,   2136,   2140,   2143,   2147,   2151,   2154,   2158,   2162,   2166,   2169,   2173,   2176,   2180,   2184,   2187,   2191,   2195,   2198,
        2202,   2206,   2209,   2213,   2216,   2220,   2224,   2227,   2231,   2235,   2238,   2242,   2245,   2249,   2253,   2256,   2260,   2263,   2267,   2270,   2274,   2278,   2281,   2285,   2288,
        2292,   2295,   2299,   2303,   2306,   2310,   2313,   2317,   2320,   2324,   2327,   2331,   2334,   2338,   2341,   2345,   2348,   2352,   2355,   2359,   2363,   2366,   2370,   2373,   2377,
        2380,   2383,   2387,   2390,   2394,   2397,   2401,   2404,   2408,   2411,   2415,   2418,   2422,   2425,   2429,   2432,   2436,   2439,   2442,   2446,   2449,   2453,   2456,   2460,   2463,
        2466,   2470,   2473,   2477,   2480,   2484,   2487,   2490,   2494,   2497,   2501,   2504,   2507,   2511,   2514,   2518,   2521,   2524,   2528,   2531,   2535,   2538,   2541,   2545,   2548,
        2551,   2555,   2558,   2561,   2565,   2568,   2572,   2575,   2578,   2582,   2585,   2588,   2592,   2595,   2598,   2602,   2605,   2608,   2612,   2615,   2618,   2622,   2625,   2628,   2632,
        2635,   2638,   2642,   2645,   2648,   2651,   2655,   2658,   2661,   2665,   2668,   2671,   2674,   2678,   2681,   2684,   2688,   2691,   2694,   2697,   2701,   2704,   2707,   2711,   2714,
        2717,   2720,   2724,   2727,   2730,   2733,   2737,   2740,   2743,   2746,   2750,   2753,   2756,   2759,   2762,   2766,   2769,   2772,   2775,   2779,   2782,   2785,   2788,   2792,   2795,
        2798,   2801,   2804,   2808,   2811,   2814,   2817,   2820,   2824,   2827,   2830,   2833,   2836,   2840,   2843,   2846,   2849,   2852,   2855,   2859,   2862,   2865,   2868,   2871,   2874,
        2878,   2881,   2884,   2887,   2890,   2893,   2897,   2900,   2903,   2906,   2909,   2912,   2915,   2919,   2922,   2925,   2928,   2931,   2934,   2937,   2941,   2944,   2947,   2950,   2953,
        2956,   2959,   2962,   2966,   2969,   2972,   2975,   2978,   2981,   2984,   2987,   2990,   2994,   2997,   3000,   3003,   3006,   3009,   3012,   3015,   3018,   3021,   3024,   3028,   3031,
        3034,   3037,   3040,   3043,   3046,   3049,   3052,   3055,   3058,   3061,   3064,   3068,   3071,   3074,   3077,   3080,   3083,   3086,   3089,   3092,   3095,   3098,   3101,   3104,   3107,
        3110,   3113,   3116,   3119,   3122,   3125,   3128,   3132,   3135,   3138,   3141,   3144,   3147,   3150,   3153,   3156,   3159,   3162,   3165,   3168,   3171,   3174,   3177,   3180,   3183,
        3186,   3189,   3192,   3195,   3198,   3201,   3204,   3207,   3210,   3213,   3216,   3219,   3222,   3225,   3228,   3231,   3234,   3237,   3240,   3243,   3246,   3249,   3252,   3255,   3257,
        3260,   3263,   3266,   3269,   3272,   3275,   3278,   3281,   3284,   3287,   3290,   3293,   3296,   3299,   3302,   3305,   3308,   3311,   3314,   3317,   3320,   3322,   3325,   3328,   3331,
        3334,   3337,   3340,   3343,   3346,   3349,   3352,   3355,   3358,   3361,   3364,   3366,   3369,   3372,   3375,   3378,   3381,   3384,   3387,   3390,   3393,   3396,   3398,   3401,   3404,
        3407,   3410,   3413,   3416,   3419,   3422,   3425,   3427,   3430,   3433,   3436,   3439,   3442,   3445,   3448,   3451,   3453,   3456,   3459,   3462,   3465,   3468,   3471,   3474,   3476,
        3479,   3482,   3485,   3488,   3491,   3494,   3497,   3499,   3502,   3505,   3508,   3511,   3514,   3517,   3519,   3522,   3525,   3528,   3531,   3534,   3536,   3539,   3542,   3545,   3548,
        3551,   3554,   3556,   3559,   3562,   3565,   3568,   3571,   3573,   3576,   3579,   3582,   3585,   3588,   3590,   3593,   3596,   3599,   3602,   3604,   3607,   3610,   3613,   3616,   3619,
        3621,   3624,   3627,   3630,   3633,   3635,   3638,   3641,   3644,   3647,   3649,   3652,   3655,   3658,   3661,   3663,   3666,   3669,   3672,   3675,   3677,   3680,   3683,   3686,   3689,
        3691,   3694,   3697,   3700,   3702,   3705,   3708,   3711,   3714,   3716,   3719,   3722,   3725,   3727,   3730,   3733,   3736,   3738,   3741,   3744,   3747,   3750,   3752,   3755,   3758,
        3761,   3763,   3766,   3769,   3772,   3774,   3777,   3780,   3783,   3785,   3788,   3791,   3794,   3796,   3799,   3802,   3805,   3807,   3810,   3813,   3816,   3818,   3821,   3824,   3826,
        3829,   3832,   3835,   3837,   3840,   3843,   3846,   3848,   3851,   3854,   3856,   3859,   3862,   3865,   3867,   3870,   3873,   3876,   3878,   3881,   3884,   3886,   3889,   3892,   3894,
        3897,   3900,   3903,   3905,   3908,   3911,   3913,   3916,   3919,   3922,   3924,   3927,   3930,   3932,   3935,   3938,   3940,   3943,   3946,   3948,   3951,   3954,   3957,   3959,   3962,
        3965,   3967,   3970,   3973,   3975,   3978,   3981,   3983,   3986,   3989,   3991,   3994,   3997,   3999,   4002,   4005,   4007,   4010,   4013,   4015,   4018,   4021,   4023,   4026,   4029,
        4031,   4034,   4037,   4039,   4042,   4045,   4047,   4050,   4053,   4055,   4058,   4061,   4063,   4066,   4069,   4071,   4074,   4077,   4079,   4082,   4084,   4087,   4090,   4092,   4095

    };

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(gamma_attr);
    ISP_CHECK_BOOL(gamma_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    wdr_mode = hi_ext_system_sensor_wdr_mode_read(vi_pipe);

    switch (gamma_attr->curve_type) {
        case ISP_GAMMA_CURVE_DEFAULT:
            if (wdr_mode == 0) {
                gamma_lut = gamma_def;
            } else {
                gamma_lut = gamma_def_wdr;
            }
            break;
        case ISP_GAMMA_CURVE_SRGB:
            if (wdr_mode == 0) {
                gamma_lut = gammas_rgb;
            } else {
                gamma_lut = gamma033;
            }
            break;
        case ISP_GAMMA_CURVE_HDR:
            ISP_WARN_TRACE("ISP_GAMMA_CURVE_HDR is only used for hi3559_av100!\n");
            gamma_lut = gamma_hdr;
            break;
        case ISP_GAMMA_CURVE_USER_DEFINE:
            gamma_lut = gamma_attr->table;
            break;
        default:
            ISP_ERR_TRACE("Invalid  gamma curve type %d!\n", gamma_attr->curve_type);
            return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < GAMMA_NODE_NUM; i++) {
        if (gamma_lut[i] > 0xFFF) {
            ISP_ERR_TRACE("Invalid gamma table[%d] %d!\n", i, gamma_lut[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_gamma_lut_write(vi_pipe, i, gamma_lut[i]);
    }

    hi_ext_system_gamma_lut_update_write(vi_pipe, HI_TRUE);
    hi_ext_system_gamma_en_write(vi_pipe, gamma_attr->enable);
    hi_ext_system_gamma_curve_type_write(vi_pipe, gamma_attr->curve_type);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_gamma_attr(VI_PIPE vi_pipe, hi_isp_gamma_attr *gamma_attr)
{
    hi_u32 i = 0;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(gamma_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    gamma_attr->enable = hi_ext_system_gamma_en_read(vi_pipe);
    gamma_attr->curve_type = hi_ext_system_gamma_curve_type_read(vi_pipe);

    for (i = 0; i < GAMMA_NODE_NUM; i++) {
        gamma_attr->table[i] = hi_ext_system_gamma_lut_read(vi_pipe, i);
    }

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_pre_gamma_attr(VI_PIPE vi_pipe, const hi_isp_pregamma_attr *pre_gamma_attr)
{
#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    hi_u32 i = 0;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(pre_gamma_attr);
    ISP_CHECK_BOOL(pre_gamma_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
        if (pre_gamma_attr->table[i] > HI_ISP_PREGAMMA_LUT_MAX) {
            ISP_ERR_TRACE("Invalid table[%d] %d!\n", i, pre_gamma_attr->table[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_pregamma_lut_write(vi_pipe, i, pre_gamma_attr->table[i]);
    }

    hi_ext_system_pregamma_lut_update_write(vi_pipe, HI_TRUE);
    hi_ext_system_pregamma_en_write(vi_pipe, pre_gamma_attr->enable);

    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_get_pre_gamma_attr(VI_PIPE vi_pipe, hi_isp_pregamma_attr *pre_gamma_attr)
{
#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    hi_u32 i = 0;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(pre_gamma_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    pre_gamma_attr->enable = hi_ext_system_pregamma_en_read(vi_pipe);

    for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
        pre_gamma_attr->table[i] = hi_ext_system_pregamma_lut_read(vi_pipe, i);
    }

    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_set_pre_log_lut_attr(VI_PIPE vi_pipe, const hi_isp_preloglut_attr *pre_log_lut_attr)
{
    return isp_set_pre_log_lut_attr(vi_pipe, pre_log_lut_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_pre_log_lut_attr(VI_PIPE vi_pipe, hi_isp_preloglut_attr *pre_log_lut_attr)
{
    return isp_get_pre_log_lut_attr(vi_pipe, pre_log_lut_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_log_lut_attr(VI_PIPE vi_pipe, const hi_isp_loglut_attr *log_lut_attr)
{
    return isp_set_log_lut_attr(vi_pipe, log_lut_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_log_lut_attr(VI_PIPE vi_pipe, hi_isp_loglut_attr *log_lut_attr)
{
    return isp_get_log_lut_attr(vi_pipe, log_lut_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_csc_attr(VI_PIPE vi_pipe, const hi_isp_csc_attr *csc_attr)
{
    hi_u8 i;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(csc_attr);
    ISP_CHECK_BOOL(csc_attr->enable);
    ISP_CHECK_BOOL(csc_attr->limited_range_en);
    ISP_CHECK_BOOL(csc_attr->ct_mode_en);
    ISP_CHECK_BOOL(csc_attr->ext_csc_en);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if ((csc_attr->luma > 100) || (csc_attr->contr > 100) ||
        (csc_attr->satu > 100) || (csc_attr->hue > 100)) {
        ISP_ERR_TRACE("Invalid csc parameter!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (csc_attr->color_gamut >= COLOR_GAMUT_BUTT) {
        ISP_ERR_TRACE("Invalid color gamut!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (csc_attr->color_gamut == COLOR_GAMUT_BT2020) {
        ISP_WARN_TRACE("BT.2020 gamut is only used in hi3559_av100!\n");
    }

    hi_ext_system_csc_enable_write(vi_pipe, csc_attr->enable);
    hi_ext_system_csc_gamuttype_write(vi_pipe, csc_attr->color_gamut);
    hi_ext_system_csc_luma_write(vi_pipe, csc_attr->luma);
    hi_ext_system_csc_contrast_write(vi_pipe, csc_attr->contr);
    hi_ext_system_csc_hue_write(vi_pipe, csc_attr->hue);
    hi_ext_system_csc_sat_write(vi_pipe, csc_attr->satu);
    hi_ext_system_csc_limitrange_en_write(vi_pipe, csc_attr->limited_range_en);
    hi_ext_system_csc_ext_en_write(vi_pipe, csc_attr->ext_csc_en);
    hi_ext_system_csc_ctmode_en_write(vi_pipe, csc_attr->ct_mode_en);

    for (i = 0; i < 3; i++) {
        if ((csc_attr->csc_magtrx.csc_in_dc[i] > 1023) || (csc_attr->csc_magtrx.csc_in_dc[i] < -1024)) {
            ISP_ERR_TRACE("Invalid csc_in_dc[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((csc_attr->csc_magtrx.csc_out_dc[i] > 1023) || (csc_attr->csc_magtrx.csc_out_dc[i] < -1024)) {
            ISP_ERR_TRACE("Invalid csc_out_dc[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_csc_dcin_write(vi_pipe, i,  csc_attr->csc_magtrx.csc_in_dc[i]);
        hi_ext_system_csc_dcout_write(vi_pipe, i, csc_attr->csc_magtrx.csc_out_dc[i]);
    }

    for (i = 0; i < 9; i++) {
        hi_ext_system_csc_coef_write(vi_pipe, i, csc_attr->csc_magtrx.csc_coef[i]);
    }

    hi_ext_system_csc_attr_update_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_csc_attr(VI_PIPE vi_pipe, hi_isp_csc_attr *csc_attr)
{
    hi_u8 i;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(csc_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    csc_attr->enable        = hi_ext_system_csc_enable_read(vi_pipe);
    csc_attr->color_gamut   = hi_ext_system_csc_gamuttype_read(vi_pipe);
    csc_attr->hue           = hi_ext_system_csc_hue_read(vi_pipe);
    csc_attr->luma          = hi_ext_system_csc_luma_read(vi_pipe);
    csc_attr->contr         = hi_ext_system_csc_contrast_read(vi_pipe);
    csc_attr->satu          = hi_ext_system_csc_sat_read(vi_pipe);
    csc_attr->limited_range_en = hi_ext_system_csc_limitrange_en_read(vi_pipe);
    csc_attr->ext_csc_en       = hi_ext_system_csc_ext_en_read(vi_pipe);
    csc_attr->ct_mode_en       = hi_ext_system_csc_ctmode_en_read(vi_pipe);

    for (i = 0; i < 3; i++) {
        csc_attr->csc_magtrx.csc_in_dc[i]  = hi_ext_system_csc_dcin_read(vi_pipe, i);
        csc_attr->csc_magtrx.csc_out_dc[i] = hi_ext_system_csc_dcout_read(vi_pipe, i);
    }

    for (i = 0; i < 9; i++) {
        csc_attr->csc_magtrx.csc_coef[i] = hi_ext_system_csc_coef_read(vi_pipe, i);
    }

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_dp_calibrate(VI_PIPE vi_pipe, const hi_isp_dp_static_calibrate *dp_calibrate)
{
#ifdef CONFIG_HI_ISP_DPC_STATIC_TABLE_SUPPORT
    hi_u16 static_count_max = STATIC_DP_COUNT_NORMAL;
    isp_working_mode isp_work_mode;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dp_calibrate);
    ISP_CHECK_BOOL(dp_calibrate->enable_detect);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (ioctl(g_as32IspFd[vi_pipe], ISP_WORK_MODE_GET, &isp_work_mode) != HI_SUCCESS) {
        ISP_ERR_TRACE("get work mode error!\n");
        return HI_FAILURE;
    }

    if (((isp_work_mode.running_mode == ISP_MODE_RUNNING_OFFLINE) ||
         (isp_work_mode.running_mode == ISP_MODE_RUNNING_STRIPING)) &&
        (dp_calibrate->enable_detect == HI_TRUE) &&
        (ISP_SUPPORT_OFFLINE_DPC_CALIBRATION) != 1) {
        ISP_ERR_TRACE("only support dpc calibration under online mode!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    static_count_max = static_count_max * isp_work_mode.block_num;

    static_count_max = MIN2(static_count_max, STATIC_DP_COUNT_MAX);

    if (dp_calibrate->count_max > static_count_max) {
        ISP_ERR_TRACE("Invalid count_max!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (dp_calibrate->count_min > dp_calibrate->count_max) {
        ISP_ERR_TRACE("Invalid count_min!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (dp_calibrate->time_limit > 0x640) {
        ISP_ERR_TRACE("Invalid time_limit!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (dp_calibrate->static_dp_type >= ISP_STATIC_DP_BUTT) {
        ISP_ERR_TRACE("Invalid static_dp_type!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (dp_calibrate->start_thresh == 0) {
        ISP_ERR_TRACE("Invalid start_thresh!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_dpc_static_calib_enable_write(vi_pipe, dp_calibrate->enable_detect);

    hi_ext_system_dpc_static_defect_type_write(vi_pipe, dp_calibrate->static_dp_type);
    hi_ext_system_dpc_start_thresh_write(vi_pipe, dp_calibrate->start_thresh);
    hi_ext_system_dpc_count_max_write(vi_pipe, dp_calibrate->count_max);
    hi_ext_system_dpc_count_min_write(vi_pipe, dp_calibrate->count_min);
    hi_ext_system_dpc_trigger_time_write(vi_pipe, dp_calibrate->time_limit);
    hi_ext_system_dpc_trigger_status_write(vi_pipe, ISP_STATE_INIT);

    return HI_SUCCESS;
#else
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_get_dp_calibrate(VI_PIPE vi_pipe, hi_isp_dp_static_calibrate *dp_calibrate)
{
#ifdef CONFIG_HI_ISP_DPC_STATIC_TABLE_SUPPORT
    hi_u16 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dp_calibrate);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    dp_calibrate->enable_detect   = hi_ext_system_dpc_static_calib_enable_read(vi_pipe);

    dp_calibrate->static_dp_type  = hi_ext_system_dpc_static_defect_type_read(vi_pipe);
    dp_calibrate->start_thresh   = hi_ext_system_dpc_start_thresh_read(vi_pipe);
    dp_calibrate->count_max     = hi_ext_system_dpc_count_max_read(vi_pipe);
    dp_calibrate->count_min     = hi_ext_system_dpc_count_min_read(vi_pipe);
    dp_calibrate->time_limit    = hi_ext_system_dpc_trigger_time_read(vi_pipe);

    dp_calibrate->status        = hi_ext_system_dpc_trigger_status_read(vi_pipe);
    dp_calibrate->finish_thresh  = hi_ext_system_dpc_finish_thresh_read(vi_pipe);
    dp_calibrate->count        = hi_ext_system_dpc_bpt_calib_number_read(vi_pipe);

    if (dp_calibrate->status == ISP_STATE_INIT) {
        for (i = 0; i < STATIC_DP_COUNT_MAX; i++) {
            dp_calibrate->table[i] = 0;
        }
    } else {
        for (i = 0; i < STATIC_DP_COUNT_MAX; i++) {
            if (i < dp_calibrate->count) {
                dp_calibrate->table[i] = hi_ext_system_dpc_calib_bpt_read(vi_pipe, i);
            } else {
                dp_calibrate->table[i] = 0;
            }
        }
    }
    return HI_SUCCESS;
#else
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_set_dp_static_attr(VI_PIPE vi_pipe, const hi_isp_dp_static_attr *dp_static_attr)
{
#ifdef CONFIG_HI_ISP_DPC_STATIC_TABLE_SUPPORT
    hi_u16 i = 0, j = 0, m = 0, count_in = 0;
    hi_s32 ret = HI_SUCCESS;
    hi_u16 static_count_max = STATIC_DP_COUNT_NORMAL;
    hi_u32 *defect_pixel_table = HI_NULL;
    isp_working_mode isp_work_mode;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dp_static_attr);
    ISP_CHECK_BOOL(dp_static_attr->enable);
    ISP_CHECK_BOOL(dp_static_attr->show);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (ioctl(g_as32IspFd[vi_pipe], ISP_WORK_MODE_GET, &isp_work_mode)) {
        ISP_ERR_TRACE("get work mode error!\n");
        return HI_FAILURE;
    }

    if ((isp_work_mode.running_mode == ISP_MODE_RUNNING_SIDEBYSIDE)) {
        static_count_max = static_count_max * ISP_SBS_BLOCK_NUM;
    } else if (isp_work_mode.running_mode == ISP_MODE_RUNNING_STRIPING) {
        if (ISP_SUPPORT_OFFLINE_DPC_CALIBRATION == 1) {
            static_count_max = static_count_max * isp_work_mode.block_num;
        } else {
            static_count_max = static_count_max * ISP_SBS_BLOCK_NUM;
        }
    }

    if (dp_static_attr->bright_count > static_count_max) {
        ISP_ERR_TRACE("Invalid bright_count!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (dp_static_attr->dark_count > static_count_max) {
        ISP_ERR_TRACE("Invalid dark_count!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < static_count_max; i++) {
        if (dp_static_attr->bright_table[i] > 0x1FFF1FFF || dp_static_attr->dark_table[i] > 0x1FFF1FFF) {
            ISP_ERR_TRACE("dark_table and bright_table should be less than 0x1FFF1FFF!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    defect_pixel_table = (hi_u32 *)ISP_MALLOC(sizeof(hi_u32) * static_count_max);

    if (defect_pixel_table == HI_NULL) {
        ISP_ERR_TRACE("malloc defect_pixel_table memory failure!\n");
        return HI_ERR_ISP_NOMEM;
    }

    i = 0;
    /* merging dark talbe and bright table */
    while ((i < dp_static_attr->bright_count) && (j < dp_static_attr->dark_count)) {
        if (m >= static_count_max) {
            ISP_ERR_TRACE("the size of merging DP table(bright_table and dark_table) is larger than %d!\n",
                      static_count_max);
            ret = HI_ERR_ISP_ILLEGAL_PARAM;
            goto EXIT_SET_DP_ATTR;
        }
        if (dp_static_attr->bright_table[i] > dp_static_attr->dark_table[j]) {
            defect_pixel_table[m++] = dp_static_attr->dark_table[j++];
        } else if (dp_static_attr->bright_table[i] < dp_static_attr->dark_table[j]) {
            defect_pixel_table[m++] = dp_static_attr->bright_table[i++];
        } else {
            defect_pixel_table[m++] = dp_static_attr->bright_table[i];
            i++;
            j++;
        }
    }

    if (i >= dp_static_attr->bright_count) {
        while (j < dp_static_attr->dark_count) {
            if (m >= static_count_max) {
                ISP_ERR_TRACE("the size of merging DP table(bright_table and dark_table) is larger than %d!\n",
                          static_count_max);
                ret = HI_ERR_ISP_ILLEGAL_PARAM;
                goto EXIT_SET_DP_ATTR;
            }
            defect_pixel_table[m++] = dp_static_attr->dark_table[j++];
        }
    }
    if (j >= dp_static_attr->dark_count) {
        while (i < dp_static_attr->bright_count) {
            if (m >= static_count_max) {
                ISP_ERR_TRACE("the size of merging DP table(bright_table and dark_table) is larger than %d!\n",
                          static_count_max);
                ret = HI_ERR_ISP_ILLEGAL_PARAM;
                goto EXIT_SET_DP_ATTR;
            }
            defect_pixel_table[m++] = dp_static_attr->bright_table[i++];
        }
    }

    count_in = m;

    hi_ext_system_dpc_static_cor_enable_write(vi_pipe, dp_static_attr->enable);
    hi_ext_system_dpc_static_dp_show_write(vi_pipe, dp_static_attr->show);
    hi_ext_system_dpc_bpt_cor_number_write(vi_pipe, count_in);

    for (i = 0; i < STATIC_DP_COUNT_MAX; i++) {
        if (i < count_in) {
            hi_ext_system_dpc_cor_bpt_write(vi_pipe, i, defect_pixel_table[i]);
        } else {
            hi_ext_system_dpc_cor_bpt_write(vi_pipe, i, 0);
        }
    }
    hi_ext_system_dpc_static_attr_update_write(vi_pipe, HI_TRUE);

EXIT_SET_DP_ATTR:

    ISP_FREE(defect_pixel_table);

    return ret;
#else
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_get_dp_static_attr(VI_PIPE vi_pipe,  hi_isp_dp_static_attr *dp_static_attr)
{
#ifdef CONFIG_HI_ISP_DPC_STATIC_TABLE_SUPPORT
    hi_u32 i = 0;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dp_static_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    dp_static_attr->enable        = hi_ext_system_dpc_static_cor_enable_read(vi_pipe);
    dp_static_attr->show          = hi_ext_system_dpc_static_dp_show_read(vi_pipe);
    dp_static_attr->bright_count = hi_ext_system_dpc_bpt_cor_number_read(vi_pipe);
    dp_static_attr->dark_count   = 0;

    for (i = 0; i < STATIC_DP_COUNT_MAX; i++) {
        if (i < dp_static_attr->bright_count) {
            dp_static_attr->bright_table[i] = hi_ext_system_dpc_cor_bpt_read(vi_pipe, i);
        } else {
            dp_static_attr->bright_table[i] = 0;
        }
        dp_static_attr->dark_table[i] = 0;
    }
    return HI_SUCCESS;
#else
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_set_dp_dynamic_attr(VI_PIPE vi_pipe, const hi_isp_dp_dynamic_attr *dp_dynamic_attr)
{
    hi_u8 i;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dp_dynamic_attr);
    ISP_CHECK_BOOL(dp_dynamic_attr->enable);
    ISP_CHECK_BOOL(dp_dynamic_attr->sup_twinkle_en);
    ISP_CHECK_OPEN(vi_pipe);

    ISP_CHECK_MEM_INIT(vi_pipe);

    if (dp_dynamic_attr->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid op_type!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_dpc_dynamic_cor_enable_write(vi_pipe, dp_dynamic_attr->enable);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (dp_dynamic_attr->auto_attr.strength[i] > 255) {
            ISP_ERR_TRACE("Invalid strength[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (dp_dynamic_attr->auto_attr.blend_ratio[i] > 0x80) {
            ISP_ERR_TRACE("Invalid blend_ratio[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_dpc_dynamic_strength_table_write(vi_pipe, i, dp_dynamic_attr->auto_attr.strength[i]);
        hi_ext_system_dpc_dynamic_blend_ratio_table_write(vi_pipe, i, dp_dynamic_attr->auto_attr.blend_ratio[i]);
    }

    if (dp_dynamic_attr->manual_attr.strength > 255) {
        ISP_ERR_TRACE("Invalid strength!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (dp_dynamic_attr->manual_attr.blend_ratio > 0x80) {
        ISP_ERR_TRACE("Invalid blend_ratio!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (dp_dynamic_attr->soft_thr > 0x80 || dp_dynamic_attr->soft_thr < 0x0) {
        ISP_ERR_TRACE("Invalid soft_thr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_dpc_dynamic_manual_enable_write(vi_pipe, dp_dynamic_attr->op_type);
    hi_ext_system_dpc_dynamic_strength_write(vi_pipe, dp_dynamic_attr->manual_attr.strength);
    hi_ext_system_dpc_dynamic_blend_ratio_write(vi_pipe, dp_dynamic_attr->manual_attr.blend_ratio);

    hi_ext_system_dpc_suppress_twinkle_enable_write(vi_pipe, dp_dynamic_attr->sup_twinkle_en);
    hi_ext_system_dpc_suppress_twinkle_thr_write(vi_pipe, dp_dynamic_attr->soft_thr);
    hi_ext_system_dpc_suppress_twinkle_slope_write(vi_pipe, dp_dynamic_attr->soft_slope);
    hi_ext_system_dpc_dynamic_attr_update_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_dp_dynamic_attr(VI_PIPE vi_pipe, hi_isp_dp_dynamic_attr *dp_dynamic_attr)
{
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dp_dynamic_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    dp_dynamic_attr->enable       = hi_ext_system_dpc_dynamic_cor_enable_read(vi_pipe);
    dp_dynamic_attr->op_type      = (hi_isp_op_type)hi_ext_system_dpc_dynamic_manual_enable_read(vi_pipe);
    dp_dynamic_attr->sup_twinkle_en = hi_ext_system_dpc_suppress_twinkle_enable_read(vi_pipe);
    dp_dynamic_attr->soft_thr     = hi_ext_system_dpc_suppress_twinkle_thr_read(vi_pipe);
    dp_dynamic_attr->soft_slope   = hi_ext_system_dpc_suppress_twinkle_slope_read(vi_pipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        dp_dynamic_attr->auto_attr.strength[i]    = hi_ext_system_dpc_dynamic_strength_table_read(vi_pipe, i);
        dp_dynamic_attr->auto_attr.blend_ratio[i] = hi_ext_system_dpc_dynamic_blend_ratio_table_read(vi_pipe, i);
    }

    dp_dynamic_attr->manual_attr.strength    = hi_ext_system_dpc_dynamic_strength_read(vi_pipe);
    dp_dynamic_attr->manual_attr.blend_ratio = hi_ext_system_dpc_dynamic_blend_ratio_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_mesh_shading_attr(VI_PIPE vi_pipe, const hi_isp_shading_attr *shading_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(shading_attr);
    ISP_CHECK_BOOL(shading_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    hi_ext_system_isp_mesh_shading_enable_write(vi_pipe, shading_attr->enable);
    if (shading_attr->mesh_str > HI_ISP_LSC_MESHSTR_MAX) {
        ISP_ERR_TRACE("Invalid mesh_str!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_isp_mesh_shading_mesh_strength_write(vi_pipe, shading_attr->mesh_str);

    if (shading_attr->blend_ratio > 256) {
        ISP_ERR_TRACE("Invalid blend_ratio!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_isp_mesh_shading_blendratio_write(vi_pipe, shading_attr->blend_ratio);

    hi_ext_system_isp_mesh_shading_attr_updata_write(vi_pipe, HI_TRUE);
    hi_ext_system_isp_mesh_shading_fe_attr_updata_write(vi_pipe, HI_TRUE);
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_mesh_shading_attr(VI_PIPE vi_pipe, hi_isp_shading_attr *shading_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(shading_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    shading_attr->enable       = hi_ext_system_isp_mesh_shading_enable_read(vi_pipe);
    shading_attr->mesh_str    = hi_ext_system_isp_mesh_shading_mesh_strength_read(vi_pipe);
    shading_attr->blend_ratio = hi_ext_system_isp_mesh_shading_blendratio_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_mesh_shading_gain_lut_attr(VI_PIPE vi_pipe, const hi_isp_shading_lut_attr *shading_lut_attr)
{
    hi_u16 i;
    hi_u32 width, height;
    hi_u32 x_sum = 0;
    hi_u32 y_sum = 0;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(shading_lut_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (shading_lut_attr->mesh_scale > 7) {
        ISP_ERR_TRACE("Invalid mesh_scale!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_isp_mesh_shading_mesh_scale_write(vi_pipe, shading_lut_attr->mesh_scale);

    for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
        if ((shading_lut_attr->lsc_gain_lut[0].r_gain[i] > 1023) ||
            (shading_lut_attr->lsc_gain_lut[0].gr_gain[i] > 1023) ||
            (shading_lut_attr->lsc_gain_lut[0].gb_gain[i] > 1023) ||
            (shading_lut_attr->lsc_gain_lut[0].b_gain[i] > 1023) ||
            (shading_lut_attr->lsc_gain_lut[1].r_gain[i] > 1023) ||
            (shading_lut_attr->lsc_gain_lut[1].gr_gain[i] > 1023) ||
            (shading_lut_attr->lsc_gain_lut[1].gb_gain[i] > 1023) ||
            (shading_lut_attr->lsc_gain_lut[1].b_gain[i] > 1023)) {
            ISP_ERR_TRACE("Invalid gain!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        hi_ext_system_isp_mesh_shading_r_gain0_write(vi_pipe, i, shading_lut_attr->lsc_gain_lut[0].r_gain[i]);
        hi_ext_system_isp_mesh_shading_gr_gain0_write(vi_pipe, i, shading_lut_attr->lsc_gain_lut[0].gr_gain[i]);
        hi_ext_system_isp_mesh_shading_gb_gain0_write(vi_pipe, i, shading_lut_attr->lsc_gain_lut[0].gb_gain[i]);
        hi_ext_system_isp_mesh_shading_b_gain0_write(vi_pipe, i, shading_lut_attr->lsc_gain_lut[0].b_gain[i]);

        hi_ext_system_isp_mesh_shading_r_gain1_write(vi_pipe, i, shading_lut_attr->lsc_gain_lut[1].r_gain[i]);
        hi_ext_system_isp_mesh_shading_gr_gain1_write(vi_pipe, i, shading_lut_attr->lsc_gain_lut[1].gr_gain[i]);
        hi_ext_system_isp_mesh_shading_gb_gain1_write(vi_pipe, i, shading_lut_attr->lsc_gain_lut[1].gb_gain[i]);
        hi_ext_system_isp_mesh_shading_b_gain1_write(vi_pipe, i, shading_lut_attr->lsc_gain_lut[1].b_gain[i]);
    }

    for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
        hi_ext_system_isp_bnr_shading_r_gain_write(vi_pipe, i, shading_lut_attr->bnr_lsc_gain_lut.r_gain[i]);
        hi_ext_system_isp_bnr_shading_gr_gain_write(vi_pipe, i, shading_lut_attr->bnr_lsc_gain_lut.gr_gain[i]);
        hi_ext_system_isp_bnr_shading_gb_gain_write(vi_pipe, i, shading_lut_attr->bnr_lsc_gain_lut.gb_gain[i]);
        hi_ext_system_isp_bnr_shading_b_gain_write(vi_pipe, i, shading_lut_attr->bnr_lsc_gain_lut.b_gain[i]);
    }
    width  = hi_ext_system_be_total_width_read(vi_pipe);
    height = hi_ext_system_be_total_height_read(vi_pipe);

    for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1) / 2; i++) {
        x_sum += shading_lut_attr->x_grid_width[i];
    }

    for (i = 0; i < (HI_ISP_LSC_GRID_ROW - 1) / 2; i++) {
        y_sum += shading_lut_attr->y_grid_width[i];
    }

    if ((x_sum != (width / 4)) || (y_sum != (height / 4))) {
        ISP_ERR_TRACE("Invalid block size x_sum = %d, y_sum = %d!\n", x_sum, y_sum);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1) / 2; i++) {
        if ((shading_lut_attr->x_grid_width[i] > HI_ISP_LSC_XGRID_WIDTH_MAX(width)) ||
            (shading_lut_attr->x_grid_width[i] < 4)) {
            ISP_ERR_TRACE("Invalid x_grid_width[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_isp_mesh_shading_xgrid_write(vi_pipe, i, shading_lut_attr->x_grid_width[i]);
    }

    for (i = 0; i < (HI_ISP_LSC_GRID_ROW - 1) / 2; i++) {
        if ((shading_lut_attr->y_grid_width[i] > HI_ISP_LSC_YGRID_WIDTH_MAX(height)) ||
            (shading_lut_attr->y_grid_width[i] < 4)) {
            ISP_ERR_TRACE("Invalid y_grid_width[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_isp_mesh_shading_ygrid_write(vi_pipe, i, shading_lut_attr->y_grid_width[i]);
    }

    hi_ext_system_isp_mesh_shading_lut_attr_updata_write(vi_pipe, HI_TRUE);
    hi_ext_system_isp_mesh_shading_fe_lut_attr_updata_write(vi_pipe, HI_TRUE);
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_mesh_shading_gain_lut_attr(VI_PIPE vi_pipe, hi_isp_shading_lut_attr *shading_lut_attr)
{
    hi_u16 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(shading_lut_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    shading_lut_attr->mesh_scale = hi_ext_system_isp_mesh_shading_mesh_scale_read(vi_pipe);

    for (i = 0; i < HI_ISP_LSC_GRID_POINTS; i++) {
        shading_lut_attr->lsc_gain_lut[0].r_gain[i]  = hi_ext_system_isp_mesh_shading_r_gain0_read(vi_pipe, i);
        shading_lut_attr->lsc_gain_lut[0].gr_gain[i] = hi_ext_system_isp_mesh_shading_gr_gain0_read(vi_pipe, i);
        shading_lut_attr->lsc_gain_lut[0].gb_gain[i] = hi_ext_system_isp_mesh_shading_gb_gain0_read(vi_pipe, i);
        shading_lut_attr->lsc_gain_lut[0].b_gain[i]  = hi_ext_system_isp_mesh_shading_b_gain0_read(vi_pipe, i);
        shading_lut_attr->lsc_gain_lut[1].r_gain[i]  = hi_ext_system_isp_mesh_shading_r_gain1_read(vi_pipe, i);
        shading_lut_attr->lsc_gain_lut[1].gr_gain[i] = hi_ext_system_isp_mesh_shading_gr_gain1_read(vi_pipe, i);
        shading_lut_attr->lsc_gain_lut[1].gb_gain[i] = hi_ext_system_isp_mesh_shading_gb_gain1_read(vi_pipe, i);
        shading_lut_attr->lsc_gain_lut[1].b_gain[i]  = hi_ext_system_isp_mesh_shading_b_gain1_read(vi_pipe, i);
    }

    for (i = 0; i < (HI_ISP_LSC_GRID_COL - 1) / 2; i++) {
        shading_lut_attr->x_grid_width[i] = hi_ext_system_isp_mesh_shading_xgrid_read(vi_pipe, i);
    }

    for (i = 0; i < (HI_ISP_LSC_GRID_ROW - 1) / 2; i++) {
        shading_lut_attr->y_grid_width[i] = hi_ext_system_isp_mesh_shading_ygrid_read(vi_pipe, i);
    }

    for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
        shading_lut_attr->bnr_lsc_gain_lut.r_gain[i]  = hi_ext_system_isp_bnr_shading_r_gain_read(vi_pipe, i);
        shading_lut_attr->bnr_lsc_gain_lut.gr_gain[i] = hi_ext_system_isp_bnr_shading_gr_gain_read(vi_pipe, i);
        shading_lut_attr->bnr_lsc_gain_lut.gb_gain[i] = hi_ext_system_isp_bnr_shading_gb_gain_read(vi_pipe, i);
        shading_lut_attr->bnr_lsc_gain_lut.b_gain[i]  = hi_ext_system_isp_bnr_shading_b_gain_read(vi_pipe, i);
    }

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_radial_shading_attr(VI_PIPE vi_pipe, const hi_isp_radial_shading_attr *radial_shading_attr)
{
    return isp_set_radial_shading_attr(vi_pipe, radial_shading_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_radial_shading_attr(VI_PIPE vi_pipe, hi_isp_radial_shading_attr *radial_shading_attr)
{
    return isp_get_radial_shading_attr(vi_pipe, radial_shading_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_radial_shading_lut(VI_PIPE vi_pipe, const hi_isp_radial_shading_lut_attr *radial_shading_lut_attr)
{
    return isp_set_radial_shading_lut(vi_pipe, radial_shading_lut_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_radial_shading_lut(VI_PIPE vi_pipe, hi_isp_radial_shading_lut_attr *radial_shading_lut_attr)
{
    return isp_get_radial_shading_lut(vi_pipe, radial_shading_lut_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_lightbox_gain(VI_PIPE vi_pipe, hi_isp_awb_calibration_gain *awb_calibration_gain)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(awb_calibration_gain);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    return isp_get_lightbox_gain(vi_pipe, awb_calibration_gain);
}

MPI_STATIC hi_s32 hi_mpi_isp_mesh_shading_calibration(VI_PIPE vi_pipe, hi_u16 *src_raw,
                                                      hi_isp_mlsc_calibration_cfg *mlsc_cali_cfg,
                                                      hi_isp_mesh_shading_table *mlsc_table)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(src_raw);
    ISP_CHECK_POINTER(mlsc_cali_cfg);
    ISP_CHECK_POINTER(mlsc_table);

    if (mlsc_cali_cfg->img_width % 4 != 0) {
        ISP_ERR_TRACE("Invalid img_width:%d, width must be a muliple of 4!\n", mlsc_cali_cfg->img_width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->img_height % 4 != 0) {
        ISP_ERR_TRACE("Invalid img_height:%d, height must be a muliple of 4!\n", mlsc_cali_cfg->img_height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->dst_img_width % 4 != 0) {
        ISP_ERR_TRACE("Invalid dst_img_width:%d, width must be a muliple of 4!\n", mlsc_cali_cfg->dst_img_width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->dst_img_height % 4 != 0) {
        ISP_ERR_TRACE("Invalid dst_img_height:%d, height must be a muliple of 4!\n", mlsc_cali_cfg->dst_img_height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((mlsc_cali_cfg->offset_x % 2 != 0) || (mlsc_cali_cfg->offset_y % 2) != 0) {
        ISP_ERR_TRACE("Invalid offset_x:%d/offset_y:%d, bayer pattern of the cropped image does not match to the input image!\n",
                  mlsc_cali_cfg->offset_x, mlsc_cali_cfg->offset_y);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->dst_img_width + mlsc_cali_cfg->offset_x > mlsc_cali_cfg->img_width) {
        ISP_ERR_TRACE("Invalid dst_img_width:%d/offset_x:%d, dst_img_width + offset_x > img_width!\n",
                  mlsc_cali_cfg->dst_img_width, mlsc_cali_cfg->offset_x);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->dst_img_height + mlsc_cali_cfg->offset_y > mlsc_cali_cfg->img_height) {
        ISP_ERR_TRACE("Invalid dst_img_height:%d/offset_y:%d, dst_img_height + offset_y > img_height!\n",
                  mlsc_cali_cfg->dst_img_height, mlsc_cali_cfg->offset_y);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->mesh_scale > 7) {
        ISP_ERR_TRACE("Invalid mesh_scale:%d, mesh_scale must be smaller than 8!\n", mlsc_cali_cfg->mesh_scale);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((mlsc_cali_cfg->raw_bit >= BAYER_RAWBIT_BUTT) || (mlsc_cali_cfg->raw_bit < BAYER_RAWBIT_8BIT) ||
        ((mlsc_cali_cfg->raw_bit % 2) == 1)) {
        ISP_ERR_TRACE("Invalid raw_bit type:%d!\n", mlsc_cali_cfg->raw_bit);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->bayer >= BAYER_BUTT) {
        ISP_ERR_TRACE("Invalid bayer type:%d!\n", mlsc_cali_cfg->bayer);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->blc_offset_b > 0xFFF) {
        ISP_ERR_TRACE("Invalid blc(chn B) offset:%d!\n", mlsc_cali_cfg->blc_offset_b);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->blc_offset_gb > 0xFFF) {
        ISP_ERR_TRACE("Invalid blc(chn Gb) offset:%d!\n", mlsc_cali_cfg->blc_offset_gb);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->blc_offset_gr > 0xFFF) {
        ISP_ERR_TRACE("Invalid blc(chn Gr) offset:%d!\n", mlsc_cali_cfg->blc_offset_gr);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (mlsc_cali_cfg->blc_offset_r > 0xFFF) {
        ISP_ERR_TRACE("Invalid blc(chn R) offset:%d!\n", mlsc_cali_cfg->blc_offset_r);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return ISP_MeshShadingCalibration(src_raw, (ISP_MLSC_CALIBRATION_CFG_S *)mlsc_cali_cfg, (ISP_MESH_SHADING_TABLE_S *)mlsc_table);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_local_cac_attr(VI_PIPE vi_pipe, const hi_isp_local_cac_attr *local_cac_attr)
{
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(local_cac_attr);
    ISP_CHECK_BOOL(local_cac_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (local_cac_attr->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid op_type!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (local_cac_attr->var_thr > 4095) {
        ISP_ERR_TRACE("Invalid var_thr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (local_cac_attr->purple_det_range > 410) {
        ISP_ERR_TRACE("Invalid purple_det_range!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
        if (local_cac_attr->auto_attr.de_purple_cr_str[i] > 8) {
            ISP_ERR_TRACE("Invalid de_purple_cr_str[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (local_cac_attr->auto_attr.de_purple_cb_str[i] > 8) {
            ISP_ERR_TRACE("Invalid de_purple_cb_str[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_localCAC_auto_cb_str_table_write(vi_pipe, i, local_cac_attr->auto_attr.de_purple_cb_str[i]);
        hi_ext_system_localCAC_auto_cr_str_table_write(vi_pipe, i, local_cac_attr->auto_attr.de_purple_cr_str[i]);
    }

    if (local_cac_attr->manual_attr.de_purple_cr_str > 8) {
        ISP_ERR_TRACE("Invalid de_purple_cr_str!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (local_cac_attr->manual_attr.de_purple_cb_str > 8) {
        ISP_ERR_TRACE("Invalid de_purple_cr_str!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_localCAC_manual_mode_enable_write(vi_pipe, local_cac_attr->op_type);
    hi_ext_system_localCAC_manual_cb_str_write(vi_pipe, local_cac_attr->manual_attr.de_purple_cb_str);
    hi_ext_system_localCAC_manual_cr_str_write(vi_pipe, local_cac_attr->manual_attr.de_purple_cr_str);

    hi_ext_system_localCAC_enable_write(vi_pipe, local_cac_attr->enable);

    hi_ext_system_localCAC_purple_var_thr_write(vi_pipe, local_cac_attr->var_thr);
    hi_ext_system_localCAC_purple_det_range_write(vi_pipe, local_cac_attr->purple_det_range);

    hi_ext_system_LocalCAC_coef_update_en_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_local_cac_attr(VI_PIPE vi_pipe, hi_isp_local_cac_attr *local_cac_attr)
{
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(local_cac_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    local_cac_attr->enable            = hi_ext_system_localCAC_enable_read(vi_pipe);
    local_cac_attr->purple_det_range  = hi_ext_system_localCAC_purple_det_range_read(vi_pipe);
    local_cac_attr->var_thr           = hi_ext_system_localCAC_purple_var_thr_read(vi_pipe);
    local_cac_attr->op_type           = hi_ext_system_localCAC_manual_mode_enable_read(vi_pipe);
    local_cac_attr->manual_attr.de_purple_cb_str = hi_ext_system_localCAC_manual_cb_str_read(vi_pipe);
    local_cac_attr->manual_attr.de_purple_cr_str = hi_ext_system_localCAC_manual_cr_str_read(vi_pipe);

    for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
        local_cac_attr->auto_attr.de_purple_cb_str[i] = hi_ext_system_localCAC_auto_cb_str_table_read(vi_pipe, i);
        local_cac_attr->auto_attr.de_purple_cr_str[i] = hi_ext_system_localCAC_auto_cr_str_table_read(vi_pipe, i);
    }
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_global_cac_attr(VI_PIPE vi_pipe, const hi_isp_global_cac_attr *global_cac_attr)
{
#ifdef CONFIG_HI_ISP_GCAC_SUPPORT
    hi_u16 width = 0, height = 0;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(global_cac_attr);
    ISP_CHECK_BOOL(global_cac_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    width  = hi_ext_system_be_total_width_read(vi_pipe);
    height = hi_ext_system_be_total_height_read(vi_pipe);

    if (global_cac_attr->hor_coordinate >= width) {
        ISP_ERR_TRACE("Invalid hor_coordinate!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (global_cac_attr->ver_coordinate >= height) {
        ISP_ERR_TRACE("Invalid ver_coordinate!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((global_cac_attr->param_red_a > 255) || (global_cac_attr->param_red_a < -256)) {
        ISP_ERR_TRACE("Invalid param_red_a!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((global_cac_attr->param_red_b > 255) || (global_cac_attr->param_red_b < -256)) {
        ISP_ERR_TRACE("Invalid param_red_b!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((global_cac_attr->param_red_c > 255) || (global_cac_attr->param_red_c < -256)) {
        ISP_ERR_TRACE("Invalid param_red_c!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((global_cac_attr->param_blue_a > 255) || (global_cac_attr->param_blue_a < -256)) {
        ISP_ERR_TRACE("Invalid param_blue_a!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((global_cac_attr->param_blue_b > 255) || (global_cac_attr->param_blue_b < -256)) {
        ISP_ERR_TRACE("Invalid param_blue_b!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((global_cac_attr->param_blue_c > 255) || (global_cac_attr->param_blue_c < -256)) {
        ISP_ERR_TRACE("Invalid param_blue_c!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (global_cac_attr->ver_norm_shift > 7) {
        ISP_ERR_TRACE("Invalid ver_norm_shift!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (global_cac_attr->ver_norm_factor > 31) {
        ISP_ERR_TRACE("Invalid ver_norm_factor!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (global_cac_attr->hor_norm_shift > 7) {
        ISP_ERR_TRACE("Invalid hor_norm_shift!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (global_cac_attr->hor_norm_factor > 31) {
        ISP_ERR_TRACE("Invalid hor_norm_factor!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (global_cac_attr->cor_var_thr > 4095) {
        ISP_ERR_TRACE("Invalid cor_var_thr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_GlobalCAC_enable_write(vi_pipe, global_cac_attr->enable);
    hi_ext_system_GlobalCAC_coor_center_hor_write(vi_pipe, global_cac_attr->hor_coordinate);
    hi_ext_system_GlobalCAC_coor_center_ver_write(vi_pipe, global_cac_attr->ver_coordinate);
    hi_ext_system_GlobalCAC_param_red_A_write(vi_pipe, global_cac_attr->param_red_a);
    hi_ext_system_GlobalCAC_param_red_B_write(vi_pipe, global_cac_attr->param_red_b);
    hi_ext_system_GlobalCAC_param_red_C_write(vi_pipe, global_cac_attr->param_red_c);
    hi_ext_system_GlobalCAC_param_blue_A_write(vi_pipe, global_cac_attr->param_blue_a);
    hi_ext_system_GlobalCAC_param_blue_B_write(vi_pipe, global_cac_attr->param_blue_b);
    hi_ext_system_GlobalCAC_param_blue_C_write(vi_pipe, global_cac_attr->param_blue_c);

    hi_ext_system_GlobalCAC_y_ns_norm_write(vi_pipe, global_cac_attr->ver_norm_shift);
    hi_ext_system_GlobalCAC_y_nf_norm_write(vi_pipe, global_cac_attr->ver_norm_factor);
    hi_ext_system_GlobalCAC_x_ns_norm_write(vi_pipe, global_cac_attr->hor_norm_shift);
    hi_ext_system_GlobalCAC_x_nf_norm_write(vi_pipe, global_cac_attr->hor_norm_factor);

    hi_ext_system_GlobalCAC_cor_thr_write(vi_pipe, global_cac_attr->cor_var_thr);
    hi_ext_system_GlobalCAC_coef_update_en_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_get_global_cac_attr(VI_PIPE vi_pipe, hi_isp_global_cac_attr *global_cac_attr)
{
#ifdef CONFIG_HI_ISP_GCAC_SUPPORT
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(global_cac_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    global_cac_attr->enable           = hi_ext_system_GlobalCAC_enable_read(vi_pipe);
    global_cac_attr->hor_coordinate   = hi_ext_system_GlobalCAC_coor_center_hor_read(vi_pipe);
    global_cac_attr->ver_coordinate   = hi_ext_system_GlobalCAC_coor_center_ver_read(vi_pipe);
    global_cac_attr->param_red_a      = hi_ext_system_GlobalCAC_param_red_A_read(vi_pipe);
    global_cac_attr->param_red_b      = hi_ext_system_GlobalCAC_param_red_B_read(vi_pipe);
    global_cac_attr->param_red_c      = hi_ext_system_GlobalCAC_param_red_C_read(vi_pipe);
    global_cac_attr->param_blue_a     = hi_ext_system_GlobalCAC_param_blue_A_read(vi_pipe);
    global_cac_attr->param_blue_b     = hi_ext_system_GlobalCAC_param_blue_B_read(vi_pipe);
    global_cac_attr->param_blue_c     = hi_ext_system_GlobalCAC_param_blue_C_read(vi_pipe);
    global_cac_attr->ver_norm_shift   = hi_ext_system_GlobalCAC_y_ns_norm_read(vi_pipe);
    global_cac_attr->ver_norm_factor  = hi_ext_system_GlobalCAC_y_nf_norm_read(vi_pipe);
    global_cac_attr->hor_norm_shift   = hi_ext_system_GlobalCAC_x_ns_norm_read(vi_pipe);
    global_cac_attr->hor_norm_factor  = hi_ext_system_GlobalCAC_x_nf_norm_read(vi_pipe);
    global_cac_attr->cor_var_thr      = hi_ext_system_GlobalCAC_cor_thr_read(vi_pipe);
    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_set_rc_attr(VI_PIPE vi_pipe, const hi_isp_rc_attr *rc_attr)
{
    return isp_set_rc_attr(vi_pipe, rc_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_rc_attr(VI_PIPE vi_pipe, hi_isp_rc_attr *rc_attr)
{
    return isp_get_rc_attr(vi_pipe, rc_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_nr_attr(VI_PIPE vi_pipe, const hi_isp_nr_attr *nr_attr)
{
    hi_u8 i, j;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(nr_attr);
    ISP_CHECK_BOOL(nr_attr->enable);
    ISP_CHECK_BOOL(nr_attr->low_power_enable);
    ISP_CHECK_BOOL(nr_attr->nr_lsc_enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (nr_attr->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid NR type %d!\n", nr_attr->op_type);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_bayernr_enable_write(vi_pipe, nr_attr->enable);
    hi_ext_system_bayernr_manual_mode_write(vi_pipe, nr_attr->op_type);
    hi_ext_system_bayernr_low_power_enable_write(vi_pipe, nr_attr->low_power_enable);
    hi_ext_system_bayernr_lsc_enable_write(vi_pipe, nr_attr->nr_lsc_enable);

    hi_ext_system_bayernr_lsc_nr_ratio_write(vi_pipe, nr_attr->nr_lsc_ratio);

    if (nr_attr->bnr_lsc_max_gain > 0xbf) {
        ISP_ERR_TRACE("Invalid bnr_lsc_max_gain %d!\n", nr_attr->bnr_lsc_max_gain);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_bayernr_lsc_max_gain_write(vi_pipe, nr_attr->bnr_lsc_max_gain);

    if (nr_attr->bnr_lsc_cmp_strength > 0x100) {
        ISP_ERR_TRACE("Invalid bnr_lsc_cmp_strength %d!\n", nr_attr->bnr_lsc_cmp_strength);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_bayernr_lsc_cmp_strength_write(vi_pipe, nr_attr->bnr_lsc_cmp_strength);

    for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
        if (nr_attr->coring_ratio[i] > 0x3ff) {
            ISP_ERR_TRACE("Invalid coring_ratio %d!\n", nr_attr->coring_ratio[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        hi_ext_system_bayernr_coring_ratio_write(vi_pipe, i, nr_attr->coring_ratio[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
            if (nr_attr->auto_attr.chroma_str[j][i] > 3) {
                ISP_ERR_TRACE("Invalid chroma_str[%d][%d] %d!\n", j, i, nr_attr->auto_attr.chroma_str[j][i]);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }

            if (nr_attr->auto_attr.coarse_str[j][i] > 0x360) {
                ISP_ERR_TRACE("Invalid coarse_str[%d][%d] %d!\n", j, i, nr_attr->auto_attr.coarse_str[j][i]);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }

        if (nr_attr->auto_attr.fine_str[i] > 128) {
            ISP_ERR_TRACE("Invalid fine_str %d!\n", nr_attr->auto_attr.fine_str[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (nr_attr->auto_attr.coring_wgt[i] > 3200) {
            ISP_ERR_TRACE("Invalid coring_wgt %d!\n", nr_attr->auto_attr.coring_wgt[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_bayernr_auto_chroma_strength_r_write(vi_pipe, i, nr_attr->auto_attr.chroma_str[0][i]);
        hi_ext_system_bayernr_auto_chroma_strength_gr_write(vi_pipe, i, nr_attr->auto_attr.chroma_str[1][i]);
        hi_ext_system_bayernr_auto_chroma_strength_gb_write(vi_pipe, i, nr_attr->auto_attr.chroma_str[2][i]);
        hi_ext_system_bayernr_auto_chroma_strength_b_write(vi_pipe, i, nr_attr->auto_attr.chroma_str[3][i]);
        hi_ext_system_bayernr_auto_coarse_strength_r_write(vi_pipe, i, nr_attr->auto_attr.coarse_str[0][i]);
        hi_ext_system_bayernr_auto_coarse_strength_gr_write(vi_pipe, i, nr_attr->auto_attr.coarse_str[1][i]);
        hi_ext_system_bayernr_auto_coarse_strength_gb_write(vi_pipe, i, nr_attr->auto_attr.coarse_str[2][i]);
        hi_ext_system_bayernr_auto_coarse_strength_b_write(vi_pipe, i, nr_attr->auto_attr.coarse_str[3][i]);
        hi_ext_system_bayernr_auto_fine_strength_write(vi_pipe, i, nr_attr->auto_attr.fine_str[i]);
        hi_ext_system_bayernr_auto_coring_weight_write(vi_pipe, i, nr_attr->auto_attr.coring_wgt[i]);
    }

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        if (nr_attr->manual_attr.chroma_str[j] > 3) {
            ISP_ERR_TRACE("Invalid chroma_str[%d] %d!\n", j, nr_attr->manual_attr.chroma_str[j]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (nr_attr->manual_attr.coarse_str[j] > 0x360) {
            ISP_ERR_TRACE("Invalid coarse_str[%d] %d!\n", j, nr_attr->manual_attr.coarse_str[j]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_bayernr_manual_chroma_strength_write(vi_pipe, j, nr_attr->manual_attr.chroma_str[j]);
        hi_ext_system_bayernr_manual_coarse_strength_write(vi_pipe, j, nr_attr->manual_attr.coarse_str[j]);
    }

    if (nr_attr->manual_attr.fine_str > 128) {
        ISP_ERR_TRACE("Invalid fine_str %d!\n", nr_attr->manual_attr.fine_str);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (nr_attr->manual_attr.coring_wgt > 3200) {
        ISP_ERR_TRACE("Invalid coring_wgt %d!\n", nr_attr->manual_attr.coring_wgt);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_bayernr_manual_fine_strength_write(vi_pipe, nr_attr->manual_attr.fine_str);
    hi_ext_system_bayernr_manual_coring_weight_write(vi_pipe, nr_attr->manual_attr.coring_wgt);

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        if (nr_attr->wdr_attr.wdr_frame_str[i] > 80) {
            ISP_ERR_TRACE("Invalid wdr_frame_str[%d] %d!\n", i, nr_attr->wdr_attr.wdr_frame_str[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        hi_ext_system_bayernr_wdr_frame_strength_write(vi_pipe, i, nr_attr->wdr_attr.wdr_frame_str[i]);
    }
    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        if (nr_attr->wdr_attr.fusion_frame_str[i] > 80) {
            ISP_ERR_TRACE("Invalid fusion_frame_str[%d] %d!\n", i, nr_attr->wdr_attr.fusion_frame_str[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        hi_ext_system_bayernr_fusion_frame_strength_write(vi_pipe, i, nr_attr->wdr_attr.fusion_frame_str[i]);
    }

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_nr_attr(VI_PIPE vi_pipe, hi_isp_nr_attr *nr_attr)
{
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(nr_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    nr_attr->enable              = hi_ext_system_bayernr_enable_read(vi_pipe);
    nr_attr->op_type             = hi_ext_system_bayernr_manual_mode_read(vi_pipe);
    nr_attr->low_power_enable      = hi_ext_system_bayernr_low_power_enable_read(vi_pipe);
    nr_attr->nr_lsc_enable         = hi_ext_system_bayernr_lsc_enable_read(vi_pipe);
    nr_attr->nr_lsc_ratio         = hi_ext_system_bayernr_lsc_nr_ratio_read(vi_pipe);
    nr_attr->bnr_lsc_max_gain      = hi_ext_system_bayernr_lsc_max_gain_read(vi_pipe);
    nr_attr->bnr_lsc_cmp_strength = hi_ext_system_bayernr_lsc_cmp_strength_read(vi_pipe);

    for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
        nr_attr->coring_ratio[i] = hi_ext_system_bayernr_coring_ratio_read(vi_pipe, i);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        nr_attr->auto_attr.chroma_str[0][i]  = hi_ext_system_bayernr_auto_chroma_strength_r_read(vi_pipe, i);
        nr_attr->auto_attr.chroma_str[1][i]  = hi_ext_system_bayernr_auto_chroma_strength_gr_read(vi_pipe, i);
        nr_attr->auto_attr.chroma_str[2][i]  = hi_ext_system_bayernr_auto_chroma_strength_gb_read(vi_pipe, i);
        nr_attr->auto_attr.chroma_str[3][i]  = hi_ext_system_bayernr_auto_chroma_strength_b_read(vi_pipe, i);
        nr_attr->auto_attr.coarse_str[0][i] = hi_ext_system_bayernr_auto_coarse_strength_r_read(vi_pipe, i);
        nr_attr->auto_attr.coarse_str[1][i] = hi_ext_system_bayernr_auto_coarse_strength_gr_read(vi_pipe, i);
        nr_attr->auto_attr.coarse_str[2][i] = hi_ext_system_bayernr_auto_coarse_strength_gb_read(vi_pipe, i);
        nr_attr->auto_attr.coarse_str[3][i] = hi_ext_system_bayernr_auto_coarse_strength_b_read(vi_pipe, i);
        nr_attr->auto_attr.fine_str[i]       = hi_ext_system_bayernr_auto_fine_strength_read(vi_pipe, i);
        nr_attr->auto_attr.coring_wgt[i]    = hi_ext_system_bayernr_auto_coring_weight_read(vi_pipe, i);
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        nr_attr->manual_attr.chroma_str[i]  = hi_ext_system_bayernr_manual_chroma_strength_read(vi_pipe, i);
        nr_attr->manual_attr.coarse_str[i] = hi_ext_system_bayernr_manual_coarse_strength_read(vi_pipe, i);
    }

    nr_attr->manual_attr.fine_str        = hi_ext_system_bayernr_manual_fine_strength_read(vi_pipe);
    nr_attr->manual_attr.coring_wgt     = hi_ext_system_bayernr_manual_coring_weight_read(vi_pipe);

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        nr_attr->wdr_attr.wdr_frame_str[i]     =  hi_ext_system_bayernr_wdr_frame_strength_read(vi_pipe, i);
        nr_attr->wdr_attr.fusion_frame_str[i]  =  hi_ext_system_bayernr_fusion_frame_strength_read(vi_pipe, i);
    }
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_de_attr(VI_PIPE vi_pipe, const hi_isp_de_attr *de_attr)
{
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(de_attr);
    ISP_CHECK_BOOL(de_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (de_attr->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid DE type %d!\n", de_attr->op_type);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_detail_manual_mode_write(vi_pipe, de_attr->op_type);

    hi_ext_system_detail_enable_write(vi_pipe, de_attr->enable);

    for (i = 0; i < HI_ISP_DE_LUMA_GAIN_LUT_N; i++) {
        if (de_attr->luma_gain_lut[i] > 0x100) {
            ISP_ERR_TRACE("Invalid luma_gain_lut[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        hi_ext_system_detail_luma_gain_lut_write(vi_pipe, i, de_attr->luma_gain_lut[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (de_attr->auto_attr.global_gain[i] > 0x100) {
            ISP_ERR_TRACE("Invalid global_gain[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_detail_auto_global_gain_write(vi_pipe, i, de_attr->auto_attr.global_gain[i]);

        if (de_attr->auto_attr.gain_lf[i] > 0x20) {
            ISP_ERR_TRACE("Invalid gain_lf[%d] input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_detail_auto_gain_lf_write(vi_pipe, i, de_attr->auto_attr.gain_lf[i]);

        if (de_attr->auto_attr.gain_hf[i] > 0x20) {
            ISP_ERR_TRACE("Invalid gain_hf[%d] input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_detail_auto_gain_hf_write(vi_pipe, i, de_attr->auto_attr.gain_hf[i]);
    }

    // de manual mode
    if (de_attr->manual_attr.global_gain > 0x100) {
        ISP_ERR_TRACE("Invalid global_gain!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_detail_manual_global_gain_write(vi_pipe, de_attr->manual_attr.global_gain);

    if (de_attr->manual_attr.gain_lf > 0x20) {
        ISP_ERR_TRACE("Invalid gain_lf!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_detail_manual_gain_lf_write(vi_pipe, de_attr->manual_attr.gain_lf);

    if (de_attr->manual_attr.gain_hf > 0x20) {
        ISP_ERR_TRACE("Invalid gain_hf!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_detail_manual_gain_hf_write(vi_pipe, de_attr->manual_attr.gain_hf);

    hi_ext_system_detail_attr_update_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_de_attr(VI_PIPE vi_pipe, hi_isp_de_attr *de_attr)
{
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(de_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    de_attr->enable  = hi_ext_system_detail_enable_read(vi_pipe);
    de_attr->op_type = hi_ext_system_detail_manual_mode_read(vi_pipe);

    for (i = 0; i < HI_ISP_DE_LUMA_GAIN_LUT_N; i++) {
        de_attr->luma_gain_lut[i] = hi_ext_system_detail_luma_gain_lut_read(vi_pipe, i);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        de_attr->auto_attr.global_gain[i] = hi_ext_system_detail_auto_global_gain_read(vi_pipe, i);
        de_attr->auto_attr.gain_lf[i]     = hi_ext_system_detail_auto_gain_lf_read(vi_pipe, i);
        de_attr->auto_attr.gain_hf[i]     = hi_ext_system_detail_auto_gain_hf_read(vi_pipe, i);
    }

    de_attr->manual_attr.global_gain = hi_ext_system_detail_manual_global_gain_read(vi_pipe);
    de_attr->manual_attr.gain_lf     = hi_ext_system_detail_manual_gain_lf_read(vi_pipe);
    de_attr->manual_attr.gain_hf     = hi_ext_system_detail_manual_gain_hf_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_rgbir_attr(VI_PIPE vi_pipe, const hi_isp_rgbir_attr *rgbir_attr)
{
    return isp_set_rgbir_attr(vi_pipe, rgbir_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_rgbir_attr(VI_PIPE vi_pipe, hi_isp_rgbir_attr *rgbir_attr)
{
    return isp_get_rgbir_attr(vi_pipe, rgbir_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_color_tone_attr(VI_PIPE vi_pipe, const hi_isp_color_tone_attr *ct_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(ct_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if ((ct_attr->red_cast_gain < 0x100) || (ct_attr->red_cast_gain > 0x180)) {
        ISP_ERR_TRACE("Invalid red_cast_gain! should in range of [0x100, 0x180]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((ct_attr->green_cast_gain < 0x100) || (ct_attr->green_cast_gain > 0x180)) {
        ISP_ERR_TRACE("Invalid green_cast_gain! should in range of [0x100, 0x180]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((ct_attr->blue_cast_gain < 0x100) || (ct_attr->blue_cast_gain > 0x180)) {
        ISP_ERR_TRACE("Invalid blue_cast_gain! should in range of [0x100, 0x180]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_cc_colortone_rgain_write(vi_pipe, ct_attr->red_cast_gain);
    hi_ext_system_cc_colortone_ggain_write(vi_pipe, ct_attr->green_cast_gain);
    hi_ext_system_cc_colortone_bgain_write(vi_pipe, ct_attr->blue_cast_gain);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_color_tone_attr(VI_PIPE vi_pipe, hi_isp_color_tone_attr *ct_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(ct_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ct_attr->red_cast_gain   = hi_ext_system_cc_colortone_rgain_read(vi_pipe);
    ct_attr->green_cast_gain = hi_ext_system_cc_colortone_ggain_read(vi_pipe);
    ct_attr->blue_cast_gain  = hi_ext_system_cc_colortone_bgain_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_sharpen_attr(VI_PIPE vi_pipe, const hi_isp_sharpen_attr *isp_shp_attr)
{
    hi_u8 i, j;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(isp_shp_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ISP_CHECK_BOOL(isp_shp_attr->enable);
    if (isp_shp_attr->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid sharpen type %d!\n", isp_shp_attr->op_type);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_manual_isp_sharpen_en_write(vi_pipe, isp_shp_attr->enable);
    hi_ext_system_isp_sharpen_manu_mode_write(vi_pipe, isp_shp_attr->op_type);
    hi_ext_system_manual_Isp_sharpen_SkinUmax_write(vi_pipe, isp_shp_attr->skin_umax);
    hi_ext_system_manual_Isp_sharpen_SkinVmax_write(vi_pipe, isp_shp_attr->skin_vmax);
    hi_ext_system_manual_Isp_sharpen_SkinUmin_write(vi_pipe, isp_shp_attr->skin_umin);
    hi_ext_system_manual_Isp_sharpen_SkinVmin_write(vi_pipe, isp_shp_attr->skin_vmin);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            if (isp_shp_attr->auto_attr.texture_str[j][i] > 4095) {
                ISP_ERR_TRACE("Invalid auto_attr.texture_str:%d! value range:[0, 4095]\n",
                          isp_shp_attr->auto_attr.texture_str[j][i]);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            if (isp_shp_attr->auto_attr.edge_str[j][i] > 4095) {
                ISP_ERR_TRACE("Invalid auto_attr.edge_str:%d! value range:[0, 4095]\n",
                          isp_shp_attr->auto_attr.edge_str[j][i]);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            if (isp_shp_attr->auto_attr.luma_wgt[j][i] > HI_ISP_SHARPEN_LUMAWGT_MAX) {
                ISP_ERR_TRACE("Invalid auto_attr.luma_wgt:%d! value range:[0, %d]\n", isp_shp_attr->auto_attr.luma_wgt[j][i],
                          HI_ISP_SHARPEN_LUMAWGT_MAX);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
        if (isp_shp_attr->auto_attr.texture_freq[i] > 4095) {
            ISP_ERR_TRACE("Invalid auto_attr.texture_freq:%d! value range:[0, 4095]\n",
                      isp_shp_attr->auto_attr.texture_freq[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (isp_shp_attr->auto_attr.edge_freq[i] > 4095) {
            ISP_ERR_TRACE("Invalid auto_attr.edge_freq:%d! value range:[0, 4095]\n", isp_shp_attr->auto_attr.edge_freq[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (isp_shp_attr->auto_attr.over_shoot[i] > 127) {
            ISP_ERR_TRACE("Invalid auto_attr.over_shoot:%d! value range:[0, 127]\n", isp_shp_attr->auto_attr.over_shoot[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (isp_shp_attr->auto_attr.under_shoot[i] > 127) {
            ISP_ERR_TRACE("Invalid auto_attr.under_shoot:%d! value range:[0, 127]\n",
                      isp_shp_attr->auto_attr.under_shoot[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (isp_shp_attr->auto_attr.edge_filt_str[i] > 63) {
            ISP_ERR_TRACE("Invalid auto_attr.edge_filt_str:%d! value range:[0, 63]\n",
                      isp_shp_attr->auto_attr.edge_filt_str[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (isp_shp_attr->auto_attr.edge_filt_max_cap[i] > HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX) {
            ISP_ERR_TRACE("Invalid auto_attr.edge_filt_max_cap:%d! value range:[0, %d]\n", isp_shp_attr->auto_attr.edge_filt_max_cap[i],
                      HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (isp_shp_attr->auto_attr.r_gain[i] > HI_ISP_SHARPEN_RGAIN_MAX) {
            ISP_ERR_TRACE("Invalid auto_attr.r_gain:%d! value range:[0, %d]\n", isp_shp_attr->auto_attr.r_gain[i],
                      HI_ISP_SHARPEN_RGAIN_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (isp_shp_attr->auto_attr.g_gain[i] > HI_ISP_SHARPEN_GGAIN_MAX) {
            ISP_ERR_TRACE("Invalid auto_attr.g_gain:%d! value range:[0, %d]\n", isp_shp_attr->auto_attr.g_gain[i],
                      HI_ISP_SHARPEN_GGAIN_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (isp_shp_attr->auto_attr.b_gain[i] > HI_ISP_SHARPEN_BGAIN_MAX) {
            ISP_ERR_TRACE("Invalid auto_attr.b_gain:%d! value range:0, %d]\n", isp_shp_attr->auto_attr.b_gain[i],
                      HI_ISP_SHARPEN_BGAIN_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (isp_shp_attr->auto_attr.skin_gain[i] > 31) {
            ISP_ERR_TRACE("Invalid auto_attr.skin_gain:%d! value range:[0, 31]\n", isp_shp_attr->auto_attr.skin_gain[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (isp_shp_attr->auto_attr.shoot_sup_adj[i] > 15) {
            ISP_ERR_TRACE("Invalid auto_attr.shoot_sup_adj:%d! value range:[0, 15]\n",
                      isp_shp_attr->auto_attr.shoot_sup_adj[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (isp_shp_attr->auto_attr.max_sharp_gain[i] > 0x7FF) {
            ISP_ERR_TRACE("Invalid auto_attr.max_sharp_gain:%d! value range:[0, 0x7FF]\n",
                      isp_shp_attr->auto_attr.max_sharp_gain[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (isp_shp_attr->auto_attr.weak_detail_gain[i] > 0x7F) {
            ISP_ERR_TRACE("Invalid auto_attr.weak_detail_gain:%d! value range:[0, 0x7F]\n",
                      isp_shp_attr->auto_attr.weak_detail_gain[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            hi_ext_system_Isp_sharpen_TextureStr_write(vi_pipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, isp_shp_attr->auto_attr.texture_str[j][i]);
            hi_ext_system_Isp_sharpen_EdgeStr_write(vi_pipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, isp_shp_attr->auto_attr.edge_str[j][i]);
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            hi_ext_system_Isp_sharpen_LumaWgt_write(vi_pipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM, isp_shp_attr->auto_attr.luma_wgt[j][i]);
        }

        hi_ext_system_Isp_sharpen_TextureFreq_write(vi_pipe, i, isp_shp_attr->auto_attr.texture_freq[i]);
        hi_ext_system_Isp_sharpen_EdgeFreq_write(vi_pipe, i, isp_shp_attr->auto_attr.edge_freq[i]);
        hi_ext_system_Isp_sharpen_OverShoot_write(vi_pipe, i, isp_shp_attr->auto_attr.over_shoot[i]);
        hi_ext_system_Isp_sharpen_UnderShoot_write(vi_pipe, i, isp_shp_attr->auto_attr.under_shoot[i]);
        hi_ext_system_Isp_sharpen_shootSupStr_write(vi_pipe, i, isp_shp_attr->auto_attr.shoot_sup_str[i]);
        hi_ext_system_Isp_sharpen_detailctrl_write(vi_pipe, i, isp_shp_attr->auto_attr.detail_ctrl[i]);
        hi_ext_system_Isp_sharpen_EdgeFiltStr_write(vi_pipe, i, isp_shp_attr->auto_attr.edge_filt_str[i]);
        hi_ext_system_Isp_sharpen_EdgeFiltMaxCap_write(vi_pipe, i, isp_shp_attr->auto_attr.edge_filt_max_cap[i]);
        hi_ext_system_Isp_sharpen_RGain_write(vi_pipe, i, isp_shp_attr->auto_attr.r_gain[i]);
        hi_ext_system_Isp_sharpen_GGain_write(vi_pipe, i, isp_shp_attr->auto_attr.g_gain[i]);
        hi_ext_system_Isp_sharpen_BGain_write(vi_pipe, i, isp_shp_attr->auto_attr.b_gain[i]);
        hi_ext_system_Isp_sharpen_SkinGain_write(vi_pipe, i, isp_shp_attr->auto_attr.skin_gain[i]);
        hi_ext_system_Isp_sharpen_ShootSupAdj_write(vi_pipe, i, isp_shp_attr->auto_attr.shoot_sup_adj[i]);
        hi_ext_system_Isp_sharpen_detailctrlThr_write(vi_pipe, i, isp_shp_attr->auto_attr.detail_ctrl_thr[i]);
        hi_ext_system_Isp_sharpen_MaxSharpGain_write(vi_pipe, i, isp_shp_attr->auto_attr.max_sharp_gain[i]);
        hi_ext_system_Isp_sharpen_WeakDetailGain_write(vi_pipe, i, isp_shp_attr->auto_attr.weak_detail_gain[i]);
    }
    for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
        if (isp_shp_attr->manual_attr.texture_str[j] > 4095) {
            ISP_ERR_TRACE("Invalid manual_attr.texture_str:%d! value range:[0, 4095]\n",
                      isp_shp_attr->manual_attr.texture_str[j]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (isp_shp_attr->manual_attr.edge_str[j] > 4095) {
            ISP_ERR_TRACE("Invalid manual_attr.edge_str:%d! value range:[0, 4095]\n",
                      isp_shp_attr->manual_attr.edge_str[j]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
        if (isp_shp_attr->manual_attr.luma_wgt[j] > HI_ISP_SHARPEN_LUMAWGT_MAX) {
            ISP_ERR_TRACE("Invalid manual_attr.luma_wgt:%d! value range:[0, %d]\n", isp_shp_attr->manual_attr.luma_wgt[j],
                      HI_ISP_SHARPEN_LUMAWGT_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    if (isp_shp_attr->manual_attr.texture_freq > 4095) {
        ISP_ERR_TRACE("Invalid manual_attr.texture_freq:%d! value range:[0, 4095]\n",
                  isp_shp_attr->manual_attr.texture_freq);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.edge_freq > 4095) {
        ISP_ERR_TRACE("Invalid manual_attr.edge_freq:%d! value range:[0, 4095]\n", isp_shp_attr->manual_attr.edge_freq);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.over_shoot > 127) {
        ISP_ERR_TRACE("Invalid manual_attr.over_shoot:%d! value range:[0, 127]\n", isp_shp_attr->manual_attr.over_shoot);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.under_shoot > 127) {
        ISP_ERR_TRACE("Invalid manual_attr.under_shoot:%d! value range:[0, 127]\n", isp_shp_attr->manual_attr.under_shoot);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.edge_filt_str > 63) {
        ISP_ERR_TRACE("Invalid manual_attr.edge_filt_str:%d! value range:[0, 63]\n",
                  isp_shp_attr->manual_attr.edge_filt_str);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.edge_filt_max_cap > HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX) {
        ISP_ERR_TRACE("Invalid manual_attr.edge_filt_max_cap:%d! value range:[0, %d]\n", isp_shp_attr->manual_attr.edge_filt_max_cap,
                  HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.r_gain > HI_ISP_SHARPEN_RGAIN_MAX) {
        ISP_ERR_TRACE("Invalid manual_attr.r_gain:%d! value range:[0, %d]\n", isp_shp_attr->manual_attr.r_gain,
                  HI_ISP_SHARPEN_RGAIN_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.g_gain > HI_ISP_SHARPEN_GGAIN_MAX) {
        ISP_ERR_TRACE("Invalid manual_attr.g_gain:%d! value range:[0, %d]\n", isp_shp_attr->manual_attr.g_gain,
                  HI_ISP_SHARPEN_GGAIN_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.b_gain > HI_ISP_SHARPEN_BGAIN_MAX) {
        ISP_ERR_TRACE("Invalid manual_attr.b_gain:%d! value range:[0, %d]\n", isp_shp_attr->manual_attr.b_gain,
                  HI_ISP_SHARPEN_BGAIN_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.skin_gain > 31) {
        ISP_ERR_TRACE("Invalid manual_attr.skin_gain:%d! value range:[0, 31]\n", isp_shp_attr->manual_attr.skin_gain);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.shoot_sup_adj > 15) {
        ISP_ERR_TRACE("Invalid manual_attr.shoot_sup_adj:%d! value range:[0, 15]\n",
                  isp_shp_attr->manual_attr.shoot_sup_adj);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (isp_shp_attr->manual_attr.max_sharp_gain > 0x7FF) {
        ISP_ERR_TRACE("Invalid manual_attr.max_sharp_gain:%d! value range:[0, 0x7FF]\n",
                  isp_shp_attr->manual_attr.max_sharp_gain);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (isp_shp_attr->manual_attr.weak_detail_gain > 0x7F) {
        ISP_ERR_TRACE("Invalid manual_attr.weak_detail_gain:%d! value range:[0, 0x7F]\n",
                  isp_shp_attr->manual_attr.weak_detail_gain);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
        hi_ext_system_manual_Isp_sharpen_TextureStr_write(vi_pipe, j, isp_shp_attr->manual_attr.texture_str[j]);
        hi_ext_system_manual_Isp_sharpen_EdgeStr_write(vi_pipe, j, isp_shp_attr->manual_attr.edge_str[j]);
    }
    for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
        hi_ext_system_manual_Isp_sharpen_LumaWgt_write(vi_pipe, j, isp_shp_attr->manual_attr.luma_wgt[j]);
    }
    hi_ext_system_manual_Isp_sharpen_TextureFreq_write(vi_pipe, isp_shp_attr->manual_attr.texture_freq);
    hi_ext_system_manual_Isp_sharpen_EdgeFreq_write(vi_pipe, isp_shp_attr->manual_attr.edge_freq);
    hi_ext_system_manual_Isp_sharpen_OverShoot_write(vi_pipe, isp_shp_attr->manual_attr.over_shoot);
    hi_ext_system_manual_Isp_sharpen_UnderShoot_write(vi_pipe, isp_shp_attr->manual_attr.under_shoot);
    hi_ext_system_manual_Isp_sharpen_shootSupStr_write(vi_pipe, isp_shp_attr->manual_attr.shoot_sup_str);
    hi_ext_system_manual_Isp_sharpen_detailctrl_write(vi_pipe, isp_shp_attr->manual_attr.detail_ctrl);
    hi_ext_system_manual_Isp_sharpen_EdgeFiltStr_write(vi_pipe, isp_shp_attr->manual_attr.edge_filt_str);
    hi_ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_write(vi_pipe, isp_shp_attr->manual_attr.edge_filt_max_cap);
    hi_ext_system_manual_Isp_sharpen_RGain_write(vi_pipe, isp_shp_attr->manual_attr.r_gain);
    hi_ext_system_manual_Isp_sharpen_GGain_write(vi_pipe, isp_shp_attr->manual_attr.g_gain);
    hi_ext_system_manual_Isp_sharpen_BGain_write(vi_pipe, isp_shp_attr->manual_attr.b_gain);
    hi_ext_system_manual_Isp_sharpen_SkinGain_write(vi_pipe, isp_shp_attr->manual_attr.skin_gain);
    hi_ext_system_manual_Isp_sharpen_ShootSupAdj_write(vi_pipe, isp_shp_attr->manual_attr.shoot_sup_adj);
    hi_ext_system_manual_Isp_sharpen_MaxSharpGain_write(vi_pipe, isp_shp_attr->manual_attr.max_sharp_gain);
    hi_ext_system_manual_Isp_sharpen_detailctrlThr_write(vi_pipe, isp_shp_attr->manual_attr.detail_ctrl_thr);
    hi_ext_system_manual_Isp_sharpen_WeakDetailGain_write(vi_pipe, isp_shp_attr->manual_attr.weak_detail_gain);

    hi_ext_system_sharpen_mpi_update_en_write(vi_pipe, HI_TRUE);
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_sharpen_attr(VI_PIPE vi_pipe, hi_isp_sharpen_attr *isp_shp_attr)
{
    hi_u8 i, j;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(isp_shp_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    isp_shp_attr->enable = hi_ext_system_manual_isp_sharpen_en_read(vi_pipe);
    isp_shp_attr->op_type = (hi_isp_op_type)hi_ext_system_isp_sharpen_manu_mode_read(vi_pipe);

    isp_shp_attr->skin_umax = hi_ext_system_manual_Isp_sharpen_SkinUmax_read(vi_pipe);
    isp_shp_attr->skin_vmax = hi_ext_system_manual_Isp_sharpen_SkinVmax_read(vi_pipe);
    isp_shp_attr->skin_umin = hi_ext_system_manual_Isp_sharpen_SkinUmin_read(vi_pipe);
    isp_shp_attr->skin_vmin = hi_ext_system_manual_Isp_sharpen_SkinVmin_read(vi_pipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            isp_shp_attr->auto_attr.texture_str[j][i] = hi_ext_system_Isp_sharpen_TextureStr_read(vi_pipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
            isp_shp_attr->auto_attr.edge_str[j][i]    = hi_ext_system_Isp_sharpen_EdgeStr_read(vi_pipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            isp_shp_attr->auto_attr.luma_wgt[j][i]     = hi_ext_system_Isp_sharpen_LumaWgt_read(vi_pipe, i + j * ISP_AUTO_ISO_STRENGTH_NUM);
        }
        isp_shp_attr->auto_attr.texture_freq[i]  = hi_ext_system_Isp_sharpen_TextureFreq_read(vi_pipe, i);
        isp_shp_attr->auto_attr.edge_freq[i]     = hi_ext_system_Isp_sharpen_EdgeFreq_read(vi_pipe, i);
        isp_shp_attr->auto_attr.over_shoot[i]     = hi_ext_system_Isp_sharpen_OverShoot_read(vi_pipe, i);
        isp_shp_attr->auto_attr.under_shoot[i]    = hi_ext_system_Isp_sharpen_UnderShoot_read(vi_pipe, i);
        isp_shp_attr->auto_attr.shoot_sup_str[i]   = hi_ext_system_Isp_sharpen_shootSupStr_read(vi_pipe, i);
        isp_shp_attr->auto_attr.detail_ctrl[i]    = hi_ext_system_Isp_sharpen_detailctrl_read(vi_pipe, i);
        isp_shp_attr->auto_attr.edge_filt_str[i]   = hi_ext_system_Isp_sharpen_EdgeFiltStr_read(vi_pipe, i);
        isp_shp_attr->auto_attr.edge_filt_max_cap[i] = hi_ext_system_Isp_sharpen_EdgeFiltMaxCap_read(vi_pipe, i);
        isp_shp_attr->auto_attr.r_gain[i]         = hi_ext_system_Isp_sharpen_RGain_read(vi_pipe, i);
        isp_shp_attr->auto_attr.g_gain[i]         = hi_ext_system_Isp_sharpen_GGain_read(vi_pipe, i);
        isp_shp_attr->auto_attr.b_gain[i]         = hi_ext_system_Isp_sharpen_BGain_read(vi_pipe, i);
        isp_shp_attr->auto_attr.skin_gain[i]      = hi_ext_system_Isp_sharpen_SkinGain_read(vi_pipe, i);
        isp_shp_attr->auto_attr.shoot_sup_adj[i]   = hi_ext_system_Isp_sharpen_ShootSupAdj_read(vi_pipe, i);
        isp_shp_attr->auto_attr.max_sharp_gain[i] = hi_ext_system_Isp_sharpen_MaxSharpGain_read(vi_pipe, i);
        isp_shp_attr->auto_attr.detail_ctrl_thr[i] = hi_ext_system_Isp_sharpen_detailctrlThr_read(vi_pipe, i);
        isp_shp_attr->auto_attr.weak_detail_gain[i] = hi_ext_system_Isp_sharpen_WeakDetailGain_read(vi_pipe, i);
    }

    for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
        isp_shp_attr->manual_attr.texture_str[j] = hi_ext_system_manual_Isp_sharpen_TextureStr_read(vi_pipe, j);
        isp_shp_attr->manual_attr.edge_str[j]    = hi_ext_system_manual_Isp_sharpen_EdgeStr_read(vi_pipe, j);
    }
    for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
        isp_shp_attr->manual_attr.luma_wgt[j] = hi_ext_system_manual_Isp_sharpen_LumaWgt_read(vi_pipe, j);
    }
    isp_shp_attr->manual_attr.texture_freq = hi_ext_system_manual_Isp_sharpen_TextureFreq_read(vi_pipe);
    isp_shp_attr->manual_attr.edge_freq    = hi_ext_system_manual_Isp_sharpen_EdgeFreq_read(vi_pipe);
    isp_shp_attr->manual_attr.over_shoot    = hi_ext_system_manual_Isp_sharpen_OverShoot_read(vi_pipe);
    isp_shp_attr->manual_attr.under_shoot   = hi_ext_system_manual_Isp_sharpen_UnderShoot_read(vi_pipe);
    isp_shp_attr->manual_attr.shoot_sup_str  = hi_ext_system_manual_Isp_sharpen_shootSupStr_read(vi_pipe);
    isp_shp_attr->manual_attr.detail_ctrl   = hi_ext_system_manual_Isp_sharpen_detailctrl_read(vi_pipe);
    isp_shp_attr->manual_attr.edge_filt_str  = hi_ext_system_manual_Isp_sharpen_EdgeFiltStr_read(vi_pipe);
    isp_shp_attr->manual_attr.edge_filt_max_cap = hi_ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_read(vi_pipe);
    isp_shp_attr->manual_attr.r_gain        = hi_ext_system_manual_Isp_sharpen_RGain_read(vi_pipe);
    isp_shp_attr->manual_attr.g_gain        = hi_ext_system_manual_Isp_sharpen_GGain_read(vi_pipe);
    isp_shp_attr->manual_attr.b_gain        = hi_ext_system_manual_Isp_sharpen_BGain_read(vi_pipe);
    isp_shp_attr->manual_attr.skin_gain     = hi_ext_system_manual_Isp_sharpen_SkinGain_read(vi_pipe);
    isp_shp_attr->manual_attr.shoot_sup_adj   = hi_ext_system_manual_Isp_sharpen_ShootSupAdj_read(vi_pipe);
    isp_shp_attr->manual_attr.max_sharp_gain = hi_ext_system_manual_Isp_sharpen_MaxSharpGain_read(vi_pipe);
    isp_shp_attr->manual_attr.detail_ctrl_thr = hi_ext_system_manual_Isp_sharpen_detailctrlThr_read(vi_pipe);
    isp_shp_attr->manual_attr.weak_detail_gain = hi_ext_system_manual_Isp_sharpen_WeakDetailGain_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_edge_mark_attr(VI_PIPE vi_pipe, const hi_isp_edge_mark_attr *isp_edge_mark_attr)
{
#ifdef CONFIG_HI_ISP_EDGEMARK_SUPPORT
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(isp_edge_mark_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ISP_CHECK_BOOL(isp_edge_mark_attr->enable);

    if (isp_edge_mark_attr->color > 0xFFFFFF) {
        ISP_ERR_TRACE("Invalid color:%d! value range:[0, 0xFFFFFF]\n", isp_edge_mark_attr->color);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_manual_isp_edgemark_en_write(vi_pipe, isp_edge_mark_attr->enable);
    hi_ext_system_manual_isp_edgemark_color_write(vi_pipe, isp_edge_mark_attr->color);
    hi_ext_system_manual_isp_edgemark_threshold_write(vi_pipe, isp_edge_mark_attr->threshold);

    hi_ext_system_edgemark_mpi_update_en_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_get_edge_mark_attr(VI_PIPE vi_pipe, hi_isp_edge_mark_attr *isp_edge_mark_attr)
{
#ifdef CONFIG_HI_ISP_EDGEMARK_SUPPORT
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(isp_edge_mark_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    isp_edge_mark_attr->enable     = hi_ext_system_manual_isp_edgemark_en_read(vi_pipe);
    isp_edge_mark_attr->color    = hi_ext_system_manual_isp_edgemark_color_read(vi_pipe);
    isp_edge_mark_attr->threshold = hi_ext_system_manual_isp_edgemark_threshold_read(vi_pipe);

    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_set_hlc_attr(VI_PIPE vi_pipe, const hi_isp_hlc_attr *isp_hlc_attr)
{
#ifdef CONFIG_HI_ISP_HLC_SUPPORT
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(isp_hlc_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ISP_CHECK_BOOL(isp_hlc_attr->enable);

    hi_ext_system_manual_isp_hlc_en_write(vi_pipe, isp_hlc_attr->enable);
    hi_ext_system_manual_isp_hlc_lumathr_write(vi_pipe, isp_hlc_attr->luma_thr);
    hi_ext_system_manual_isp_hlc_lumatarget_write(vi_pipe, isp_hlc_attr->luma_target);

    hi_ext_system_hlc_mpi_update_en_write(vi_pipe, HI_TRUE);
    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_get_hlc_attr(VI_PIPE vi_pipe, hi_isp_hlc_attr *isp_hlc_attr)
{
#ifdef CONFIG_HI_ISP_HLC_SUPPORT
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(isp_hlc_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    isp_hlc_attr->enable      = hi_ext_system_manual_isp_hlc_en_read(vi_pipe);
    isp_hlc_attr->luma_thr    = hi_ext_system_manual_isp_hlc_lumathr_read(vi_pipe);
    isp_hlc_attr->luma_target = hi_ext_system_manual_isp_hlc_lumatarget_read(vi_pipe);

    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_set_crosstalk_attr(VI_PIPE vi_pipe, const hi_isp_cr_attr *cr_attr)
{
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(cr_attr);
    ISP_CHECK_BOOL(cr_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (cr_attr->slope > HI_ISP_CR_SLOPE_MAX) {
        ISP_ERR_TRACE("Invalid slope %d!\n", cr_attr->slope);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (cr_attr->sensi_slope > HI_ISP_CR_SLOPE_MAX) {
        ISP_ERR_TRACE("Invalid sensitivity %d!\n", cr_attr->sensi_slope);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (cr_attr->sensi_thr > HI_ISP_CR_THR_MAX) {
        ISP_ERR_TRACE("Invalid sensi_threshold %d!\n", cr_attr->sensi_thr);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_ge_enable_write(vi_pipe, cr_attr->enable);
    hi_ext_system_ge_slope_write(vi_pipe, cr_attr->slope);
    hi_ext_system_ge_sensitivity_write(vi_pipe, cr_attr->sensi_slope);
    hi_ext_system_ge_sensithreshold_write(vi_pipe, cr_attr->sensi_thr);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (cr_attr->strength[i] > 0x100) {
            ISP_ERR_TRACE("Invalid strength %d!\n", cr_attr->strength[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_ge_strength_write(vi_pipe, i, cr_attr->strength[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (cr_attr->np_offset[i] > HI_ISP_CR_NPOFFSET_MAX || cr_attr->np_offset[i] < HI_ISP_CR_NPOFFSET_MIN) {
            ISP_ERR_TRACE("Invalid np_offset %d!\n", cr_attr->np_offset[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_ge_npoffset_write(vi_pipe, i, cr_attr->np_offset[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (cr_attr->threshold[i] > HI_ISP_CR_THR_MAX) {
            ISP_ERR_TRACE("Invalid threshold %d!\n", cr_attr->threshold[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_ge_threshold_write(vi_pipe, i, cr_attr->threshold[i]);
    }

    hi_ext_system_ge_coef_update_en_write(vi_pipe, HI_TRUE);
    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_get_crosstalk_attr(VI_PIPE vi_pipe, hi_isp_cr_attr *cr_attr)
{
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    hi_u8 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(cr_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    cr_attr->enable      = hi_ext_system_ge_enable_read(vi_pipe);
    cr_attr->slope       = hi_ext_system_ge_slope_read(vi_pipe);
    cr_attr->sensi_slope = hi_ext_system_ge_sensitivity_read(vi_pipe);
    cr_attr->sensi_thr   = hi_ext_system_ge_sensithreshold_read(vi_pipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        cr_attr->strength[i]  = hi_ext_system_ge_strength_read(vi_pipe, i);
        cr_attr->np_offset[i] = hi_ext_system_ge_npoffset_read(vi_pipe, i);
        cr_attr->threshold[i] = hi_ext_system_ge_threshold_read(vi_pipe, i);
    }

    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_set_anti_false_color_attr(VI_PIPE vi_pipe, const hi_isp_antifalsecolor_attr *anti_false_color)
{
    hi_u32 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(anti_false_color);
    ISP_CHECK_BOOL(anti_false_color->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (anti_false_color->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid op_type %d!\n", anti_false_color->op_type);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_antifalsecolor_enable_write(vi_pipe, anti_false_color->enable);
    hi_ext_system_antifalsecolor_manual_mode_write(vi_pipe, anti_false_color->op_type);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (anti_false_color->auto_attr.threshold[i] > 0x20) {
            ISP_ERR_TRACE("Invalid threshold %d!\n",
                      anti_false_color->auto_attr.threshold[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (anti_false_color->auto_attr.strength[i] > 0x1f) {
            ISP_ERR_TRACE("Invalid strength %d!\n",
                      anti_false_color->auto_attr.strength[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        hi_ext_system_antifalsecolor_auto_threshold_write(vi_pipe, i, anti_false_color->auto_attr.threshold[i]);
        hi_ext_system_antifalsecolor_auto_strenght_write(vi_pipe, i, anti_false_color->auto_attr.strength[i]);
    }

    if (anti_false_color->manual_attr.threshold > 0x20) {
        ISP_ERR_TRACE("Invalid threshold %d!\n", anti_false_color->manual_attr.threshold);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (anti_false_color->manual_attr.strength > 0x1f) {
        ISP_ERR_TRACE("Invalid strength %d!\n", anti_false_color->manual_attr.strength);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_antifalsecolor_manual_threshold_write(vi_pipe, anti_false_color->manual_attr.threshold);
    hi_ext_system_antifalsecolor_manual_strenght_write(vi_pipe, anti_false_color->manual_attr.strength);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_anti_false_color_attr(VI_PIPE vi_pipe, hi_isp_antifalsecolor_attr *anti_false_color)
{
    hi_u32 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(anti_false_color);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    anti_false_color->enable  = hi_ext_system_antifalsecolor_enable_read(vi_pipe);
    anti_false_color->op_type = hi_ext_system_antifalsecolor_manual_mode_read(vi_pipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        anti_false_color->auto_attr.threshold[i] = hi_ext_system_antifalsecolor_auto_threshold_read(vi_pipe, i);
        anti_false_color->auto_attr.strength[i]  = hi_ext_system_antifalsecolor_auto_strenght_read(vi_pipe, i);
    }

    anti_false_color->manual_attr.threshold = hi_ext_system_antifalsecolor_manual_threshold_read(vi_pipe);
    anti_false_color->manual_attr.strength  = hi_ext_system_antifalsecolor_manual_strenght_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_demosaic_attr(VI_PIPE vi_pipe, const hi_isp_demosaic_attr *demosaic_attr)
{
    hi_u32 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(demosaic_attr);
    ISP_CHECK_BOOL(demosaic_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (demosaic_attr->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid op_type %d!\n", demosaic_attr->op_type);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_demosaic_enable_write(vi_pipe, demosaic_attr->enable);
    hi_ext_system_demosaic_manual_mode_write(vi_pipe, demosaic_attr->op_type);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (demosaic_attr->auto_attr.non_dir_mf_detail_ehc_str[i] > HI_ISP_DEMOSAIC_NONDIR_MFDETALEHC_STR_MAX) {
            ISP_ERR_TRACE("Invalid non_dir_mf_detail_ehc_str %d!\n", demosaic_attr->auto_attr.non_dir_mf_detail_ehc_str[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (demosaic_attr->auto_attr.non_dir_hf_detail_ehc_str[i] > 0x10) {
            ISP_ERR_TRACE("Invalid non_dir_hf_detail_ehc_str %d!\n", demosaic_attr->auto_attr.non_dir_hf_detail_ehc_str[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if ((demosaic_attr->auto_attr.detail_smooth_range[i] > HI_ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MAX) ||
            (demosaic_attr->auto_attr.detail_smooth_range[i] < HI_ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MIN)) {
            ISP_ERR_TRACE("Invalid detail_smooth_range %d!\n", demosaic_attr->auto_attr.detail_smooth_range[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (demosaic_attr->auto_attr.detail_smooth_str[i] > 0x100) {
            ISP_ERR_TRACE("Invalid detail_smooth_str %d!\n", demosaic_attr->auto_attr.detail_smooth_str[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (demosaic_attr->auto_attr.color_noise_ctrl_strf[i] > 8) {
            ISP_ERR_TRACE("Invalid color_noise_ctrl_strf %d!\n", demosaic_attr->auto_attr.color_noise_ctrl_strf[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (demosaic_attr->auto_attr.color_noise_ctrl_thdy[i] > HI_ISP_DEMOSAIC_COLOR_NOISE_THDY_MAX) {
            ISP_ERR_TRACE("Invalid color_noise_ctrl_thdy %d!\n", demosaic_attr->auto_attr.color_noise_ctrl_thdy[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (demosaic_attr->auto_attr.color_noise_ctrl_stry[i] > HI_ISP_DEMOSAIC_COLOR_NOISE_STRY_MAX) {
            ISP_ERR_TRACE("Invalid color_noise_ctrl_stry %d!\n", demosaic_attr->auto_attr.color_noise_ctrl_stry[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_demosaic_auto_nondirection_strength_write(vi_pipe, i, demosaic_attr->auto_attr.non_dir_str[i]);
        hi_ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_write(vi_pipe, i, demosaic_attr->auto_attr.non_dir_mf_detail_ehc_str[i]);
        hi_ext_system_demosaic_auto_nondirection_higfreq_detailenhance_strength_write(vi_pipe, i, demosaic_attr->auto_attr.non_dir_hf_detail_ehc_str[i]);
        hi_ext_system_demosaic_auto_detail_smooth_range_write(vi_pipe, i, demosaic_attr->auto_attr.detail_smooth_range[i]);
        hi_ext_system_demosaic_auto_detail_smooth_strength_write(vi_pipe, i, demosaic_attr->auto_attr.detail_smooth_str[i]);
        hi_ext_system_demosaic_auto_colornoise_thdf_write(vi_pipe, i, demosaic_attr->auto_attr.color_noise_ctrl_thdf[i]);
        hi_ext_system_demosaic_auto_colornoise_strf_write(vi_pipe, i, demosaic_attr->auto_attr.color_noise_ctrl_strf[i]);
        hi_ext_system_demosaic_auto_desat_dark_range_write(vi_pipe, i, demosaic_attr->auto_attr.color_noise_ctrl_thdy[i]);
        hi_ext_system_demosaic_auto_desat_dark_strength_write(vi_pipe, i, demosaic_attr->auto_attr.color_noise_ctrl_stry[i]);
    }

    if (demosaic_attr->manual_attr.non_dir_mf_detail_ehc_str > HI_ISP_DEMOSAIC_NONDIR_MFDETALEHC_STR_MAX) {
        ISP_ERR_TRACE("Invalid non_dir_mf_detail_ehc_str %d!\n", demosaic_attr->manual_attr.non_dir_mf_detail_ehc_str);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (demosaic_attr->manual_attr.non_dir_hf_detail_ehc_str > 0x10) {
        ISP_ERR_TRACE("Invalid non_dir_hf_detail_ehc_str %d!\n", demosaic_attr->manual_attr.non_dir_hf_detail_ehc_str);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((demosaic_attr->manual_attr.detail_smooth_range > HI_ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MAX) ||
        (demosaic_attr->manual_attr.detail_smooth_range < HI_ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MIN)) {
        ISP_ERR_TRACE("Invalid detail_smooth_range %d!\n", demosaic_attr->manual_attr.detail_smooth_range);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (demosaic_attr->manual_attr.detail_smooth_str > 0x100) {
        ISP_ERR_TRACE("Invalid detail_smooth_str %d!\n", demosaic_attr->manual_attr.detail_smooth_str);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (demosaic_attr->manual_attr.color_noise_ctrl_strf > 8) {
        ISP_ERR_TRACE("Invalid color_noise_ctrl_strf %d!\n", demosaic_attr->manual_attr.color_noise_ctrl_strf);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (demosaic_attr->manual_attr.color_noise_ctrl_thdy > HI_ISP_DEMOSAIC_COLOR_NOISE_THDY_MAX) {
        ISP_ERR_TRACE("Invalid color_noise_ctrl_thdy %d!\n", demosaic_attr->manual_attr.color_noise_ctrl_thdy);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (demosaic_attr->manual_attr.color_noise_ctrl_stry > HI_ISP_DEMOSAIC_COLOR_NOISE_STRY_MAX) {
        ISP_ERR_TRACE("Invalid color_noise_ctrl_stry %d!\n", demosaic_attr->manual_attr.color_noise_ctrl_stry);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_demosaic_manual_nondirection_strength_write(vi_pipe, demosaic_attr->manual_attr.non_dir_str);
    hi_ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_write(vi_pipe, demosaic_attr->manual_attr.non_dir_mf_detail_ehc_str);
    hi_ext_system_demosaic_manual_nondirection_higfreq_detailenhance_strength_write(vi_pipe, demosaic_attr->manual_attr.non_dir_hf_detail_ehc_str);
    hi_ext_system_demosaic_manual_detail_smooth_range_write(vi_pipe, demosaic_attr->manual_attr.detail_smooth_range);
    hi_ext_system_demosaic_manual_detail_smooth_strength_write(vi_pipe, demosaic_attr->manual_attr.detail_smooth_str);
    hi_ext_system_demosaic_manual_colornoise_thdf_write(vi_pipe, demosaic_attr->manual_attr.color_noise_ctrl_thdf);
    hi_ext_system_demosaic_manual_colornoise_strf_write(vi_pipe, demosaic_attr->manual_attr.color_noise_ctrl_strf);
    hi_ext_system_demosaic_manual_desat_dark_range_write(vi_pipe, demosaic_attr->manual_attr.color_noise_ctrl_thdy);
    hi_ext_system_demosaic_manual_desat_dark_strength_write(vi_pipe, demosaic_attr->manual_attr.color_noise_ctrl_stry);

    hi_ext_system_demosaic_attr_update_en_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_demosaic_attr(VI_PIPE vi_pipe, hi_isp_demosaic_attr *demosaic_attr)
{
    hi_u32 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(demosaic_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    demosaic_attr->enable  = hi_ext_system_demosaic_enable_read(vi_pipe);
    demosaic_attr->op_type = hi_ext_system_demosaic_manual_mode_read(vi_pipe);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        demosaic_attr->auto_attr.non_dir_str[i]               = hi_ext_system_demosaic_auto_nondirection_strength_read(vi_pipe, i);
        demosaic_attr->auto_attr.non_dir_mf_detail_ehc_str[i] = hi_ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_read(vi_pipe, i);
        demosaic_attr->auto_attr.non_dir_hf_detail_ehc_str[i] = hi_ext_system_demosaic_auto_nondirection_higfreq_detailenhance_strength_read(vi_pipe, i);
        demosaic_attr->auto_attr.detail_smooth_range[i]       = hi_ext_system_demosaic_auto_detail_smooth_range_read(vi_pipe, i);
        demosaic_attr->auto_attr.detail_smooth_str[i]         = hi_ext_system_demosaic_auto_detail_smooth_strength_read(vi_pipe, i);
        demosaic_attr->auto_attr.color_noise_ctrl_thdf[i]     = hi_ext_system_demosaic_auto_colornoise_thdf_read(vi_pipe, i);
        demosaic_attr->auto_attr.color_noise_ctrl_strf[i]     = hi_ext_system_demosaic_auto_colornoise_strf_read(vi_pipe, i);
        demosaic_attr->auto_attr.color_noise_ctrl_thdy[i]     = hi_ext_system_demosaic_auto_desat_dark_range_read(vi_pipe, i);
        demosaic_attr->auto_attr.color_noise_ctrl_stry[i]     = hi_ext_system_demosaic_auto_desat_dark_strength_read(vi_pipe, i);
    }

    demosaic_attr->manual_attr.non_dir_str               = hi_ext_system_demosaic_manual_nondirection_strength_read(vi_pipe);
    demosaic_attr->manual_attr.non_dir_mf_detail_ehc_str = hi_ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_read(vi_pipe);
    demosaic_attr->manual_attr.non_dir_hf_detail_ehc_str = hi_ext_system_demosaic_manual_nondirection_higfreq_detailenhance_strength_read(vi_pipe);
    demosaic_attr->manual_attr.detail_smooth_range       = hi_ext_system_demosaic_manual_detail_smooth_range_read(vi_pipe);
    demosaic_attr->manual_attr.detail_smooth_str         = hi_ext_system_demosaic_manual_detail_smooth_strength_read(vi_pipe);
    demosaic_attr->manual_attr.color_noise_ctrl_thdf     = hi_ext_system_demosaic_manual_colornoise_thdf_read(vi_pipe);
    demosaic_attr->manual_attr.color_noise_ctrl_strf     = hi_ext_system_demosaic_manual_colornoise_strf_read(vi_pipe);
    demosaic_attr->manual_attr.color_noise_ctrl_thdy     = hi_ext_system_demosaic_manual_desat_dark_range_read(vi_pipe);
    demosaic_attr->manual_attr.color_noise_ctrl_stry     = hi_ext_system_demosaic_manual_desat_dark_strength_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_ca_attr(VI_PIPE vi_pipe, const hi_isp_ca_attr *ca_attr)
{
#ifdef CONFIG_HI_ISP_CA_SUPPORT
    hi_u16 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(ca_attr);
    ISP_CHECK_BOOL(ca_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    hi_ext_system_ca_en_write(vi_pipe, ca_attr->enable);

    if (ca_attr->ca_cp_en >= ISP_CA_BUTT) {
        ISP_ERR_TRACE("Invalid ca_cp_en!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_ca_cp_en_write(vi_pipe, ca_attr->ca_cp_en);

    for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
        hi_ext_system_ca_cp_lut_write(vi_pipe, i, ((ca_attr->cp.cp_lut_y[i] << 16) + (ca_attr->cp.cp_lut_u[i] << 8) + (ca_attr->cp.cp_lut_v[i])));

        if (ca_attr->ca.y_ratio_lut[i] > 2047) {
            ISP_ERR_TRACE("Invalid y_ratio_lut[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_ca_y_ratio_lut_write(vi_pipe, i, ca_attr->ca.y_ratio_lut[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (ca_attr->ca.iso_ratio[i] > 2047) {
            ISP_ERR_TRACE("Invalid iso_ratio[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        hi_ext_system_ca_iso_ratio_lut_write(vi_pipe, i, ca_attr->ca.iso_ratio[i]);
    }

    hi_ext_system_ca_coef_update_en_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_get_ca_attr(VI_PIPE vi_pipe, hi_isp_ca_attr *ca_attr)
{
#ifdef CONFIG_HI_ISP_CA_SUPPORT
    hi_u16 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(ca_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ca_attr->enable   = hi_ext_system_ca_en_read(vi_pipe);
    ca_attr->ca_cp_en = hi_ext_system_ca_cp_en_read(vi_pipe);

    for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
        ca_attr->cp.cp_lut_y[i] = (hi_ext_system_ca_cp_lut_read(vi_pipe, i) >> 16) & 0x000000FF;
        ca_attr->cp.cp_lut_u[i] = (hi_ext_system_ca_cp_lut_read(vi_pipe, i) >> 8) & 0x000000FF;
        ca_attr->cp.cp_lut_v[i] = hi_ext_system_ca_cp_lut_read(vi_pipe, i) & 0x000000FF;
    }

    for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
        ca_attr->ca.y_ratio_lut[i] = hi_ext_system_ca_y_ratio_lut_read(vi_pipe, i);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        ca_attr->ca.iso_ratio[i] = hi_ext_system_ca_iso_ratio_lut_read(vi_pipe, i);
    }

    return HI_SUCCESS;
#else
    ISP_ERR_TRACE("not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
#endif
}

MPI_STATIC hi_s32 hi_mpi_isp_set_clut_coeff(VI_PIPE vi_pipe, const hi_isp_clut_lut *clut_lut)
{
    return isp_set_clut_coeff(vi_pipe, clut_lut);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_clut_coeff(VI_PIPE vi_pipe, hi_isp_clut_lut *clut_lut)
{
    return isp_get_clut_coeff(vi_pipe, clut_lut);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_clut_attr(VI_PIPE vi_pipe, const hi_isp_clut_attr *clut_attr)
{
    return isp_set_clut_attr(vi_pipe, clut_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_clut_attr(VI_PIPE vi_pipe, hi_isp_clut_attr *clut_attr)
{
    return isp_get_clut_attr(vi_pipe, clut_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_black_level_attr(VI_PIPE vi_pipe, const hi_isp_black_level *black_level)
{
    hi_s32 i = 0;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(black_level);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (black_level->op_type >= OP_TYPE_BUTT) {
        ISP_ERR_TRACE("Invalid op_type %d!\n", black_level->op_type);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_black_level_manual_mode_write(vi_pipe, black_level->op_type);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        if (black_level->black_level[i] > 0xFFF) {
            ISP_ERR_TRACE("Invalid black_level[%d]:%d!\n", i, black_level->black_level[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    hi_ext_system_black_level_00_write(vi_pipe, black_level->black_level[0]);
    hi_ext_system_black_level_01_write(vi_pipe, black_level->black_level[1]);
    hi_ext_system_black_level_10_write(vi_pipe, black_level->black_level[2]);
    hi_ext_system_black_level_11_write(vi_pipe, black_level->black_level[3]);
    hi_ext_system_black_level_change_write(vi_pipe, (hi_u8)HI_TRUE);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_black_level_attr(VI_PIPE vi_pipe, hi_isp_black_level *black_level)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(black_level);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    black_level->op_type = hi_ext_system_black_level_manual_mode_read(vi_pipe);
    black_level->black_level[0] = hi_ext_system_black_level_00_read(vi_pipe);
    black_level->black_level[1] = hi_ext_system_black_level_01_read(vi_pipe);
    black_level->black_level[2] = hi_ext_system_black_level_10_read(vi_pipe);
    black_level->black_level[3] = hi_ext_system_black_level_11_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_fpn_calibrate(VI_PIPE vi_pipe, hi_isp_fpn_calibrate_attr *calibrate_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(calibrate_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    return isp_set_calibrate_attr(vi_pipe, calibrate_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_fpn_attr(VI_PIPE vi_pipe, const hi_isp_fpn_attr *fpn_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(fpn_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);
    ISP_CHECK_BOOL(fpn_attr->enable);

    return isp_set_correction_attr(vi_pipe, fpn_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_fpn_attr(VI_PIPE vi_pipe, hi_isp_fpn_attr *fpn_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(fpn_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    return isp_get_correction_attr(vi_pipe, fpn_attr);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_isp_reg_attr(VI_PIPE vi_pipe, hi_isp_reg_attr *isp_reg_attr)
{
    hi_u32 isp_bind_attr;
    hi_isp_alg_lib ae_lib;
    hi_isp_alg_lib awb_lib;
    isp_usr_ctx *isp_ctx_info = HI_NULL;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_GET_CTX(vi_pipe, isp_ctx_info);
    ISP_CHECK_POINTER(isp_ctx_info);
    ISP_CHECK_POINTER(isp_reg_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    isp_bind_attr = hi_ext_system_bind_attr_read(vi_pipe);
    ae_lib.id  = (isp_bind_attr >> 8) & 0xFF;
    awb_lib.id = isp_bind_attr & 0xFF;

    MUTEX_LOCK(isp_ctx_info->lock);

    isp_reg_attr->isp_ext_reg_addr = VReg_GetVirtAddrBase(ISP_VIR_REG_BASE(vi_pipe));
    isp_reg_attr->isp_ext_reg_size = ISP_VREG_SIZE_BIN;
    isp_reg_attr->ae_ext_reg_addr  = VReg_GetVirtAddrBase(AE_LIB_VREG_BASE(ae_lib.id));
    isp_reg_attr->ae_ext_reg_size  = AE_VREG_SIZE;
    isp_reg_attr->awb_ext_reg_addr = VReg_GetVirtAddrBase(AWB_LIB_VREG_BASE(awb_lib.id));
    isp_reg_attr->awb_ext_reg_size = ALG_LIB_VREG_SIZE_BIN;
    MUTEX_UNLOCK(isp_ctx_info->lock);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_debug(VI_PIPE vi_pipe, const hi_isp_debug_info *isp_debug)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(isp_debug);
    ISP_CHECK_BOOL(isp_debug->debug_en);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    return isp_dbg_set(vi_pipe, isp_debug);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_debug(VI_PIPE vi_pipe, hi_isp_debug_info *isp_debug)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(isp_debug);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    return isp_dbg_get(vi_pipe, isp_debug);
}

MPI_STATIC hi_s32 hi_mpi_isp_set_ctrl_param(VI_PIPE vi_pipe, const hi_isp_ctrl_param *isp_ctrl_param)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(isp_ctrl_param);

    ISP_CHECK_OPEN(vi_pipe);

    return ioctl(g_as32IspFd[vi_pipe], ISP_SET_CTRL_PARAM, isp_ctrl_param);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_ctrl_param(VI_PIPE vi_pipe, hi_isp_ctrl_param *isp_ctrl_param)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(isp_ctrl_param);
    ISP_CHECK_OPEN(vi_pipe);

    return ioctl(g_as32IspFd[vi_pipe], ISP_GET_CTRL_PARAM, isp_ctrl_param);
}

MPI_STATIC hi_s32 hi_mpi_isp_query_inner_state_info(VI_PIPE vi_pipe, hi_isp_inner_state_info *inner_state_info)
{
    hi_s32 i;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(inner_state_info);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    inner_state_info->over_shoot    = hi_ext_system_actual_sharpen_overshootAmt_read(vi_pipe);
    inner_state_info->under_shoot   = hi_ext_system_actual_sharpen_undershootAmt_read(vi_pipe);
    inner_state_info->shoot_sup_str = hi_ext_system_actual_sharpen_shootSupSt_read(vi_pipe);
    inner_state_info->texture_freq  = hi_ext_system_actual_sharpen_texture_frequence_read(vi_pipe);
    inner_state_info->edge_freq     = hi_ext_system_actual_sharpen_edge_frequence_read(vi_pipe);
    for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
        inner_state_info->edge_str[i]    = hi_ext_system_actual_sharpen_edge_str_read(vi_pipe, i);
        inner_state_info->texture_str[i] = hi_ext_system_actual_sharpen_texture_str_read(vi_pipe, i);
    }

    inner_state_info->nr_lsc_ratio = hi_ext_system_bayernr_actual_nr_lsc_ratio_read(vi_pipe);
    inner_state_info->coring_wgt   = hi_ext_system_bayernr_actual_coring_weight_read(vi_pipe);
    inner_state_info->fine_str     = hi_ext_system_bayernr_actual_fine_strength_read(vi_pipe);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        inner_state_info->coarse_str[i] = hi_ext_system_bayernr_actual_coarse_strength_read(vi_pipe, i);
        inner_state_info->chroma_str[i] = hi_ext_system_bayernr_actual_chroma_strength_read(vi_pipe, i);
    }

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        inner_state_info->wdr_frame_str[i] = hi_ext_system_bayernr_actual_wdr_frame_strength_read(vi_pipe, i);
    }

    for (i = 0; i < 3; i++) {
        inner_state_info->wdr_exp_ratio_actual[i] = hi_ext_system_actual_wdr_exposure_ratio_read(vi_pipe, i);
    }

    inner_state_info->drc_strength_actual    = hi_ext_system_drc_actual_strength_read(vi_pipe);
    inner_state_info->dehaze_strength_actual = hi_ext_system_dehaze_actual_strength_read(vi_pipe);
    inner_state_info->wdr_switch_finish      = hi_ext_top_wdr_switch_read(vi_pipe);
    inner_state_info->res_switch_finish      = hi_ext_top_res_switch_read(vi_pipe);

    inner_state_info->bl_actual[0] = hi_ext_system_black_level_query_00_read(vi_pipe);
    inner_state_info->bl_actual[1] = hi_ext_system_black_level_query_01_read(vi_pipe);
    inner_state_info->bl_actual[2] = hi_ext_system_black_level_query_10_read(vi_pipe);
    inner_state_info->bl_actual[3] = hi_ext_system_black_level_query_11_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_dng_image_static_info(VI_PIPE vi_pipe, hi_isp_dng_image_static_info *dng_image_static_info)
{
    hi_bool valid_dng_image_static_info;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dng_image_static_info);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    valid_dng_image_static_info = hi_ext_system_dng_static_info_valid_read(vi_pipe);

    if (valid_dng_image_static_info == HI_FALSE) {
        ISP_ERR_TRACE("dng_image_static_info have not been set in xxx_cmos.x!\n");
        return HI_SUCCESS;
    }

    if (ioctl(g_as32IspFd[vi_pipe], ISP_DNG_INFO_GET, dng_image_static_info) != HI_SUCCESS) {
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_dng_color_param(VI_PIPE vi_pipe, const hi_isp_dng_color_param *dng_color_param)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dng_color_param);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (dng_color_param->wb_gain1.b_gain > 0xFFF) {
        ISP_ERR_TRACE("wb_gain1.b_gain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (dng_color_param->wb_gain1.g_gain > 0xFFF) {
        ISP_ERR_TRACE("wb_gain1.g_gain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (dng_color_param->wb_gain1.r_gain > 0xFFF) {
        ISP_ERR_TRACE("wb_gain1.r_gain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (dng_color_param->wb_gain2.b_gain > 0xFFF) {
        ISP_ERR_TRACE("wb_gain2.b_gain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (dng_color_param->wb_gain2.g_gain > 0xFFF) {
        ISP_ERR_TRACE("wb_gain2.g_gain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (dng_color_param->wb_gain2.r_gain > 0xFFF) {
        ISP_ERR_TRACE("wb_gain2.r_gain can't bigger than 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_dng_high_wb_gain_r_write(vi_pipe, dng_color_param->wb_gain1.r_gain);
    hi_ext_system_dng_high_wb_gain_g_write(vi_pipe, dng_color_param->wb_gain1.g_gain);
    hi_ext_system_dng_high_wb_gain_b_write(vi_pipe, dng_color_param->wb_gain1.b_gain);
    hi_ext_system_dng_low_wb_gain_r_write(vi_pipe, dng_color_param->wb_gain2.r_gain);
    hi_ext_system_dng_low_wb_gain_g_write(vi_pipe, dng_color_param->wb_gain2.g_gain);
    hi_ext_system_dng_low_wb_gain_b_write(vi_pipe, dng_color_param->wb_gain2.b_gain);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_dng_color_param(VI_PIPE vi_pipe, hi_isp_dng_color_param *dng_color_param)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(dng_color_param);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    dng_color_param->wb_gain1.r_gain = hi_ext_system_dng_high_wb_gain_r_read(vi_pipe);
    dng_color_param->wb_gain1.g_gain = hi_ext_system_dng_high_wb_gain_g_read(vi_pipe);
    dng_color_param->wb_gain1.b_gain = hi_ext_system_dng_high_wb_gain_b_read(vi_pipe);

    dng_color_param->wb_gain2.r_gain = hi_ext_system_dng_low_wb_gain_r_read(vi_pipe);
    dng_color_param->wb_gain2.g_gain = hi_ext_system_dng_low_wb_gain_g_read(vi_pipe);
    dng_color_param->wb_gain2.b_gain = hi_ext_system_dng_low_wb_gain_b_read(vi_pipe);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_smart_info(VI_PIPE vi_pipe, const hi_isp_smart_info *smart_info)
{
    hi_u32 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(smart_info);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        ISP_CHECK_BOOL(smart_info->roi[i].enable);
        ISP_CHECK_BOOL(smart_info->roi[i].available);
        hi_ext_system_smart_enable_write(vi_pipe, i, smart_info->roi[i].enable);
        hi_ext_system_smart_available_write(vi_pipe, i, smart_info->roi[i].available);
        hi_ext_system_smart_luma_write(vi_pipe, i, smart_info->roi[i].luma);
    }

    hi_ext_system_smart_update_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_smart_info(VI_PIPE vi_pipe, hi_isp_smart_info *smart_info)
{
    hi_u32 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(smart_info);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        smart_info->roi[i].enable    = hi_ext_system_smart_enable_read(vi_pipe, i);
        smart_info->roi[i].available = hi_ext_system_smart_available_read(vi_pipe, i);
        smart_info->roi[i].luma     = hi_ext_system_smart_luma_read(vi_pipe, i);
    }

    return HI_SUCCESS;
}
MPI_STATIC hi_s32 hi_mpi_isp_calc_flicker_type(VI_PIPE vi_pipe, hi_isp_calc_flicker_input *input_param,
                                               hi_isp_calc_flicker_output *output_param,
                                               hi_video_frame_info frame[], hi_u32 array_size)
{
    return HI_SUCCESS;
}


MPI_STATIC hi_s32 hi_mpi_isp_set_mod_param(const hi_isp_mod_param *mod_param)
{
    ISP_CHECK_PIPE(0);
    ISP_CHECK_POINTER(mod_param);
    ISP_CHECK_OPEN(0);

    return  ioctl(g_as32IspFd[0], ISP_SET_MOD_PARAM, mod_param);
}

MPI_STATIC hi_s32 hi_mpi_isp_get_mod_param(hi_isp_mod_param *mod_param)
{
    ISP_CHECK_PIPE(0);
    ISP_CHECK_POINTER(mod_param);
    ISP_CHECK_OPEN(0);

    return ioctl(g_as32IspFd[0], ISP_GET_MOD_PARAM, mod_param);
}

MPI_STATIC hi_s32 hi_mpi_isp_mem_init(VI_PIPE vi_pipe)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_init(VI_PIPE vi_pipe)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_run_once(VI_PIPE vi_pipe)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_run(VI_PIPE vi_pipe)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_exit(VI_PIPE vi_pipe)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_register(VI_PIPE vi_pipe, hi_u32 addr, hi_u32 value)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_register(VI_PIPE vi_pipe, hi_u32 addr, hi_u32 *value)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_sns_slave_attr(SLAVE_DEV slavedev, const hi_isp_slave_sns_sync *sns_sync)
{
    return HI_SUCCESS;
}
MPI_STATIC hi_s32 hi_mpi_isp_get_sns_slave_attr(SLAVE_DEV slavedev, hi_isp_slave_sns_sync *sns_sync)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_sensor_reg_callback(VI_PIPE vi_pipe, hi_isp_sns_attr_info *sns_attr_info, hi_isp_sensor_register *sns_register)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_ae_lib_reg_callback(VI_PIPE vi_pipe, hi_isp_alg_lib *ae_lib, hi_isp_ae_register *ae_register)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_awb_lib_reg_callback(VI_PIPE vi_pipe, hi_isp_alg_lib *awb_lib, hi_isp_awb_register *awb_register)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_sensor_unreg_callback(VI_PIPE vi_pipe, SENSOR_ID sensor_id)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_ae_lib_unreg_callback(VI_PIPE vi_pipe, hi_isp_alg_lib *ae_lib)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_awb_lib_unreg_callback(VI_PIPE vi_pipe, hi_isp_alg_lib *awb_lib)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_bind_attr(VI_PIPE vi_pipe, const hi_isp_bind_attr *bind_attr)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_bind_attr(VI_PIPE vi_pipe, hi_isp_bind_attr *bind_attr)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_vd_time_out(VI_PIPE vi_pipe, hi_isp_vd_type isp_vd_type, hi_u32 milli_sec)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_dcf_info(VI_PIPE vi_pipe, const hi_isp_dcf_info *isp_dcf)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_dcf_info(VI_PIPE vi_pipe, hi_isp_dcf_info *isp_dcf)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_set_frame_info(VI_PIPE vi_pipe, const hi_isp_frame_info *pstIspFrame)
{
    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_isp_get_frame_info(VI_PIPE vi_pipe, const hi_isp_frame_info *pstIspFrame)
{
    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
