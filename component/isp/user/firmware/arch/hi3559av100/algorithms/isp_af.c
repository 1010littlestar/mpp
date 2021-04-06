/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include <stdio.h>
#include "isp_alg.h"
#include "hi_comm_isp_adapt.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include <math.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define  AF_MIN_WIDTH  256
#define  AF_MIN_HEIGHT 120

hi_isp_focus_statistics_cfg  g_focus_cfg[ISP_MAX_PIPE_NUM] = {
    [0 ...(ISP_MAX_PIPE_NUM - 1)] = {
        {1, 17, 15, 1920, 1080, 1, 0, {0, 0, 0, 1920, 1080}, 0, {0x2, 0x4, 0}, {1, 0x9bff}, 0xf0},
        {1, {1, 1, 1}, 15, {188, 414, -330, 486, -461, 400, -328}, {7, 0, 3, 1}, {1, 0, 255, 0, 220, 8, 14}, {127, 12, 2047} },
        {0, {1, 1, 0}, 2, {200, 200, -110, 461, -415, 0, 0}, {6, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 0 }, {15, 12, 2047} },
        {{20, 16, 0, -16, -20}, {1, 0, 255, 0, 220, 8, 14}, {38, 12, 1800} },
        {{ -12, -24, 0, 24, 12}, {1, 0, 255, 0, 220, 8, 14}, {15, 12, 2047} },
        {4, {0, 0}, {1, 1}, 0}
    }
};

hi_s32 isp_af_ext_regs_default(VI_PIPE vi_pipe)
{
    hi_u8 i, shift0;
    hi_s16 g0, g1, g2;
    hi_isp_af_h_param *iir = HI_NULL;
    hi_u32 pls, plg;
    float pl, temp;
    hi_u8 af_enable = 0;
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_isp_bayer_format cfa;
    ISP_GET_CTX(vi_pipe, isp_ctx);

    /* AF STATISTICS CONIFG START */
    af_enable = (g_focus_cfg[vi_pipe].config.enable== HI_TRUE) ? 0x3 : 0x0;
    hi_ext_system_af_enable_write(vi_pipe, af_enable);
    hi_ext_af_iir0_enable0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_en[0]);
    hi_ext_af_iir0_enable1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_en[1]);
    hi_ext_af_iir0_enable2_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_en[2]);
    hi_ext_af_iir1_enable0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_en[0]);
    hi_ext_af_iir1_enable1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_en[1]);
    hi_ext_af_iir1_enable2_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_en[2]);
    hi_ext_af_iir0_shift_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_shift);
    hi_ext_af_iir1_shift_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_shift);
    hi_ext_af_peakmode_write(vi_pipe, g_focus_cfg[vi_pipe].config.peak_mode);
    hi_ext_af_squmode_write(vi_pipe, g_focus_cfg[vi_pipe].config.squ_mode);
    hi_ext_af_window_hnum_write(vi_pipe, g_focus_cfg[vi_pipe].config.h_wnd);
    hi_ext_af_window_vnum_write(vi_pipe, g_focus_cfg[vi_pipe].config.v_wnd);
    hi_ext_af_iir_gain0_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[0]);
    hi_ext_af_iir_gain0_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[0]);
    hi_ext_af_iir_gain1_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[1]);
    hi_ext_af_iir_gain1_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[1]);
    hi_ext_af_iir_gain2_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[2]);
    hi_ext_af_iir_gain2_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[2]);
    hi_ext_af_iir_gain3_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[3]);
    hi_ext_af_iir_gain3_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[3]);
    hi_ext_af_iir_gain4_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[4]);
    hi_ext_af_iir_gain4_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[4]);
    hi_ext_af_iir_gain5_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[5]);
    hi_ext_af_iir_gain5_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[5]);
    hi_ext_af_iir_gain6_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[6]);
    hi_ext_af_iir_gain6_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[6]);
    hi_ext_af_iir0_shift_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[0]);
    hi_ext_af_iir1_shift_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[1]);
    hi_ext_af_iir2_shift_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[2]);
    hi_ext_af_iir3_shift_group0_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[3]);
    hi_ext_af_iir0_shift_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[0]);
    hi_ext_af_iir1_shift_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[1]);
    hi_ext_af_iir2_shift_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[2]);
    hi_ext_af_iir3_shift_group1_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[3]);
    hi_ext_af_fir_h_gain0_group0_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.firh[0]);
    hi_ext_af_fir_h_gain0_group1_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.firh[0]);
    hi_ext_af_fir_h_gain1_group0_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.firh[1]);
    hi_ext_af_fir_h_gain1_group1_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.firh[1]);
    hi_ext_af_fir_h_gain2_group0_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.firh[2]);
    hi_ext_af_fir_h_gain2_group1_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.firh[2]);
    hi_ext_af_fir_h_gain3_group0_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.firh[3]);
    hi_ext_af_fir_h_gain3_group1_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.firh[3]);
    hi_ext_af_fir_h_gain4_group0_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.firh[4]);
    hi_ext_af_fir_h_gain4_group1_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.firh[4]);

    /* ds */
    hi_ext_af_iir0_ds_enable_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.narrow_band);
    hi_ext_af_iir1_ds_enable_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.narrow_band);

    /* PLG and PLS */
    for (i = 0; i < 2; i++) {
        iir = i ? &(g_focus_cfg[vi_pipe].h_param_iir1) : &(g_focus_cfg[vi_pipe].h_param_iir0);

        shift0 = (hi_u8)iir->iir_shift_lut[0];
        g0 = iir->iir_gain[0];
        g1 = iir->iir_gain[1];
        g2 = iir->iir_gain[2];

        pl = (512.f / DIV_0_TO_1(512 - 2 * g1 - g2) * g0) / (1 << shift0);
        temp = pl;
        temp = MIN2(7 - floor(log(temp) / log(2)), 7);

        pls = (hi_u32)temp;
        plg = (hi_u32)((pl * (1 << pls)) + 0.5);

        if (i == 0) {
            hi_ext_af_iir_pls_group0_write(vi_pipe, pls);
            hi_ext_af_iir_plg_group0_write(vi_pipe, plg);
        } else {
            hi_ext_af_iir_pls_group1_write(vi_pipe, pls);
            hi_ext_af_iir_plg_group1_write(vi_pipe, plg);
        }
    }

    /* AF crop */
    hi_ext_af_crop_enable_write(vi_pipe, g_focus_cfg[vi_pipe].config.crop.enable);
    hi_ext_af_crop_pos_x_write(vi_pipe, 0);
    hi_ext_af_crop_pos_y_write(vi_pipe, 0);
    hi_ext_af_crop_hsize_write(vi_pipe, isp_ctx->block_attr.frame_rect.width / 8 * 8);
    hi_ext_af_crop_vsize_write(vi_pipe, isp_ctx->block_attr.frame_rect.height / 2 * 2);

    /* AF raw cfg */
    hi_ext_af_pos_sel_write(vi_pipe, g_focus_cfg[vi_pipe].config.statistics_pos);

    hi_ext_af_rawmode_write(vi_pipe, ~(((g_focus_cfg[vi_pipe].config.statistics_pos) >> 0x1) & 0x1));
    hi_ext_af_gain_limit_write(vi_pipe, g_focus_cfg[vi_pipe].config.raw_cfg.gamma_gain_limit);
    hi_ext_af_gamma_write(vi_pipe, g_focus_cfg[vi_pipe].config.raw_cfg.gamma_value);
    cfa = hi_ext_system_rggb_cfg_read(vi_pipe);
    hi_ext_af_bayermode_write(vi_pipe, (hi_u8)cfa);


    /* AF pre median filter */
    hi_ext_af_mean_enable_write(vi_pipe, g_focus_cfg[vi_pipe].config.pre_flt_cfg.en);
    hi_ext_af_mean_thres_write(vi_pipe, g_focus_cfg[vi_pipe].config.pre_flt_cfg.strength);

    /* level depend gain */
    hi_ext_af_iir0_ldg_enable_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.ld.ld_en);
    hi_ext_af_iir_thre0_low_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.ld.th_low);
    hi_ext_af_iir_thre0_high_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.ld.th_high);
    hi_ext_af_iir_slope0_low_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.ld.slp_low);
    hi_ext_af_iir_slope0_high_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.ld.slp_high);
    hi_ext_af_iir_gain0_low_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.ld.gain_low);
    hi_ext_af_iir_gain0_high_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.ld.gain_high);

    hi_ext_af_iir1_ldg_enable_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.ld.ld_en);
    hi_ext_af_iir_thre1_low_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.ld.th_low);
    hi_ext_af_iir_thre1_high_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.ld.th_high);
    hi_ext_af_iir_slope1_low_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.ld.slp_low);
    hi_ext_af_iir_slope1_high_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.ld.slp_high);
    hi_ext_af_iir_gain1_low_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.ld.gain_low);
    hi_ext_af_iir_gain1_high_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.ld.gain_high);

    hi_ext_af_fir0_ldg_enable_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.ld.ld_en);
    hi_ext_af_fir_thre0_low_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.ld.th_low);
    hi_ext_af_fir_thre0_high_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.ld.th_high);
    hi_ext_af_fir_slope0_low_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.ld.slp_low);
    hi_ext_af_fir_slope0_high_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.ld.slp_high);
    hi_ext_af_fir_gain0_low_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.ld.gain_low);
    hi_ext_af_fir_gain0_high_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.ld.gain_high);

    hi_ext_af_fir1_ldg_enable_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.ld.ld_en);
    hi_ext_af_fir_thre1_low_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.ld.th_low);
    hi_ext_af_fir_thre1_high_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.ld.th_high);
    hi_ext_af_fir_slope1_low_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.ld.slp_low);
    hi_ext_af_fir_slope1_high_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.ld.slp_high);
    hi_ext_af_fir_gain1_low_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.ld.gain_low);
    hi_ext_af_fir_gain1_high_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.ld.gain_high);

    /* AF coring */
    hi_ext_af_iir_thre0_coring_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.coring.th);
    hi_ext_af_iir_slope0_coring_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.coring.slp);
    hi_ext_af_iir_peak0_coring_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir0.coring.lmt);

    hi_ext_af_iir_thre1_coring_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.coring.th);
    hi_ext_af_iir_slope1_coring_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.coring.slp);
    hi_ext_af_iir_peak1_coring_write(vi_pipe, g_focus_cfg[vi_pipe].h_param_iir1.coring.lmt);

    hi_ext_af_fir_thre0_coring_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.coring.th);
    hi_ext_af_fir_slope0_coring_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.coring.slp);
    hi_ext_af_fir_peak0_coring_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir0.coring.lmt);

    hi_ext_af_fir_thre1_coring_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.coring.th);
    hi_ext_af_fir_slope1_coring_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.coring.slp);
    hi_ext_af_fir_peak1_coring_write(vi_pipe, g_focus_cfg[vi_pipe].v_param_fir1.coring.lmt);

    /* high luma counter */
    hi_ext_af_hiligh_thre_write(vi_pipe, (hi_u8)g_focus_cfg[vi_pipe].config.high_luma_th);

    /* AF output shift */
    hi_ext_af_acc_shift0_h_write(vi_pipe, g_focus_cfg[vi_pipe].fv_param.acc_shift_h[0]);
    hi_ext_af_acc_shift1_h_write(vi_pipe, g_focus_cfg[vi_pipe].fv_param.acc_shift_h[1]);
    hi_ext_af_acc_shift0_v_write(vi_pipe, g_focus_cfg[vi_pipe].fv_param.acc_shift_v[0]);
    hi_ext_af_acc_shift1_v_write(vi_pipe, g_focus_cfg[vi_pipe].fv_param.acc_shift_v[1]);
    hi_ext_af_acc_shift_y_write(vi_pipe, g_focus_cfg[vi_pipe].fv_param.acc_shift_y);
    hi_ext_af_shift_count_y_write(vi_pipe, g_focus_cfg[vi_pipe].fv_param.hl_cnt_shift);

    g_focus_cfg[vi_pipe].config.h_size = isp_ctx->block_attr.frame_rect.width;
    g_focus_cfg[vi_pipe].config.v_size = isp_ctx->block_attr.frame_rect.height;

    hi_ext_af_input_hsize_write(vi_pipe, g_focus_cfg[vi_pipe].config.h_size);
    hi_ext_af_input_vsize_write(vi_pipe, g_focus_cfg[vi_pipe].config.v_size);

    hi_ext_af_set_flag_write(vi_pipe, HI_EXT_AF_SET_FLAG_DISABLE);

    return HI_SUCCESS;
}

static hi_void isp_af_blk_cfg(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg, isp_usr_ctx *isp_ctx)
{
    hi_u8  i;
    hi_u8  block_num   = reg_cfg->cfg_num;
    hi_u8  window_hnum = hi_ext_af_window_hnum_read(vi_pipe);
    hi_u8  window_vnum = hi_ext_af_window_vnum_read(vi_pipe);
    hi_u16 overlap;
    isp_rect        block_rect;
    isp_af_reg_cfg  *af_default_cfg = HI_NULL;

    overlap = isp_ctx->block_attr.over_lap;

    for (i = 0; i < block_num; i++) {
        af_default_cfg   = &reg_cfg->alg_reg_cfg[i].be_af_reg_cfg;

        if (i < window_hnum % DIV_0_TO_1(block_num)) {
            af_default_cfg->window_hnum = window_hnum / DIV_0_TO_1(block_num) + 1;
        } else {
            af_default_cfg->window_hnum = window_hnum / DIV_0_TO_1(block_num);
        }

        af_default_cfg->window_vnum     = window_vnum;

        reg_cfg->kernel_reg_cfg.alg_kernel_cfg[i].be_af_zone_cfg.colum =  af_default_cfg->window_hnum;
        reg_cfg->kernel_reg_cfg.alg_kernel_cfg[i].be_af_zone_cfg.row   =  af_default_cfg->window_vnum;

        if (block_num == 1) {
            af_default_cfg->crop_enable  = hi_ext_af_crop_enable_read(vi_pipe);
            af_default_cfg->crop_pos_y  = hi_ext_af_crop_pos_y_read(vi_pipe);
            af_default_cfg->crop_pos_x  = hi_ext_af_crop_pos_x_read(vi_pipe);
            af_default_cfg->crop_h_size = hi_ext_af_crop_hsize_read(vi_pipe);
            af_default_cfg->crop_v_size = hi_ext_af_crop_vsize_read(vi_pipe);
        } else {
            ISP_GetBlockRect(&block_rect, &isp_ctx->block_attr, i);

            af_default_cfg->crop_enable        = HI_TRUE;
            af_default_cfg->crop_pos_y        = block_rect.y;
            af_default_cfg->crop_v_size       = block_rect.height;

            if (i == 0) {
                if (block_num > 1) {
                    af_default_cfg->crop_pos_x   = 0;
                    af_default_cfg->crop_h_size  = block_rect.width - overlap;
                } else {
                    af_default_cfg->crop_pos_x  = 0;
                    af_default_cfg->crop_h_size = block_rect.width;
                }
            } else if (i == (block_num - 1)) {
                af_default_cfg->crop_pos_x   = overlap;
                af_default_cfg->crop_h_size  = block_rect.width - overlap;
            } else {
                af_default_cfg->crop_pos_x   = overlap;
                af_default_cfg->crop_h_size  = block_rect.width - (overlap << 1);
            }
        }
    }
}

hi_s32 isp_set_af_fe_default(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i, shift0;
    hi_s16 g0, g1, g2;
    hi_isp_af_h_param *iir = HI_NULL;;
    hi_u32 pls, plg;
    float pl, temp;
    hi_isp_bayer_format cfa;
    isp_af_reg_cfg    *af_default_cfg = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;

    af_default_cfg   = &reg_cfg->alg_reg_cfg[0].fe_af_reg_cfg;

    cfa = hi_ext_system_rggb_cfg_read(vi_pipe);
    af_default_cfg->bayer_mode         = (hi_u8)cfa;
    af_default_cfg->lpf_enable         = HI_ISP_FE_AF_LPF_EN_DEFAULT;
    af_default_cfg->fir0_lpf_enable    = HI_ISP_FE_AF_FIR0_LPF_EN_DEFAULT;
    af_default_cfg->fir1_lpf_enable    = HI_ISP_FE_AF_FIR1_LPF_EN_DEFAULT;
    af_default_cfg->iir_plg_group0     = HI_ISP_FE_AF_IIR0_PLG_DEFAULT;
    af_default_cfg->iir_pls_group0     = HI_ISP_FE_AF_IIR0_PLS_DEFAULT;
    af_default_cfg->iir_plg_group1     = HI_ISP_FE_AF_IIR1_PLG_DEFAULT;
    af_default_cfg->iir_pls_group1     = HI_ISP_FE_AF_IIR1_PLS_DEFAULT;

    /* PLG and PLS */
    for (i = 0; i < 2; i++) {
        iir = i ? &(g_focus_cfg[vi_pipe].h_param_iir1) : &(g_focus_cfg[vi_pipe].h_param_iir0);

        shift0 = (hi_u8)iir->iir_shift_lut[0];
        g0 = iir->iir_gain[0];
        g1 = iir->iir_gain[1];
        g2 = iir->iir_gain[2];

        pl = (512.f / DIV_0_TO_1(512 - 2 * g1 - g2) * g0) / (1 << shift0);
        temp = pl;
        temp = MIN2(7 - floor(log(temp) / log(2)), 7);

        pls = (hi_u32)temp;
        plg = (hi_u32)((pl * (1 << pls)) + 0.5);

        if (i == 0) {
            af_default_cfg->iir_pls_group0 = pls;
            af_default_cfg->iir_plg_group0 = plg;
        } else {
            af_default_cfg->iir_pls_group1 = pls;
            af_default_cfg->iir_plg_group1 = plg;
        }
    }

    /* AF FE STATISTICS CONIFG START */
    af_default_cfg->af_enable          = g_focus_cfg[vi_pipe].config.enable;
    af_default_cfg->iir0_ds_enable     = g_focus_cfg[vi_pipe].h_param_iir0.narrow_band;
    af_default_cfg->iir1_ds_enable     = g_focus_cfg[vi_pipe].h_param_iir1.narrow_band;
    af_default_cfg->iir0_enable0       = g_focus_cfg[vi_pipe].h_param_iir0.iir_en[0];
    af_default_cfg->iir0_enable1       = g_focus_cfg[vi_pipe].h_param_iir0.iir_en[1];
    af_default_cfg->iir0_enable2       = g_focus_cfg[vi_pipe].h_param_iir0.iir_en[2];
    af_default_cfg->iir1_enable0       = g_focus_cfg[vi_pipe].h_param_iir1.iir_en[0];
    af_default_cfg->iir1_enable1       = g_focus_cfg[vi_pipe].h_param_iir1.iir_en[1];
    af_default_cfg->iir1_enable2       = g_focus_cfg[vi_pipe].h_param_iir1.iir_en[2];
    af_default_cfg->iir0_shift         = g_focus_cfg[vi_pipe].h_param_iir0.iir_shift;
    af_default_cfg->iir1_shift         = g_focus_cfg[vi_pipe].h_param_iir1.iir_shift;
    af_default_cfg->peak_mode          = g_focus_cfg[vi_pipe].config.peak_mode;
    af_default_cfg->squ_mode           = g_focus_cfg[vi_pipe].config.squ_mode;
    af_default_cfg->window_hnum        = g_focus_cfg[vi_pipe].config.h_wnd;
    af_default_cfg->window_vnum        = g_focus_cfg[vi_pipe].config.v_wnd;
    af_default_cfg->iir_gain0_group0   = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[0];
    af_default_cfg->iir_gain0_group1   = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[0];
    af_default_cfg->iir_gain1_group0   = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[1];
    af_default_cfg->iir_gain1_group1   = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[1];
    af_default_cfg->iir_gain2_group0   = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[2];
    af_default_cfg->iir_gain2_group1   = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[2];
    af_default_cfg->iir_gain3_group0   = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[3];
    af_default_cfg->iir_gain3_group1   = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[3];
    af_default_cfg->iir_gain4_group0   = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[4];
    af_default_cfg->iir_gain4_group1   = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[4];
    af_default_cfg->iir_gain5_group0   = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[5];
    af_default_cfg->iir_gain5_group1   = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[5];
    af_default_cfg->iir_gain6_group0   = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[6];
    af_default_cfg->iir_gain6_group1   = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[6];

    af_default_cfg->iir0_shift_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[0];
    af_default_cfg->iir1_shift_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[1];
    af_default_cfg->iir2_shift_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[2];
    af_default_cfg->iir3_shift_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[3];
    af_default_cfg->iir0_shift_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[0];
    af_default_cfg->iir1_shift_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[1];
    af_default_cfg->iir2_shift_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[2];
    af_default_cfg->iir3_shift_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[3];

    af_default_cfg->fir_h_gain0_group0 = g_focus_cfg[vi_pipe].v_param_fir0.firh[0];
    af_default_cfg->fir_h_gain0_group1 = g_focus_cfg[vi_pipe].v_param_fir1.firh[0];
    af_default_cfg->fir_h_gain1_group0 = g_focus_cfg[vi_pipe].v_param_fir0.firh[1];
    af_default_cfg->fir_h_gain1_group1 = g_focus_cfg[vi_pipe].v_param_fir1.firh[1];
    af_default_cfg->fir_h_gain2_group0 = g_focus_cfg[vi_pipe].v_param_fir0.firh[2];
    af_default_cfg->fir_h_gain2_group1 = g_focus_cfg[vi_pipe].v_param_fir1.firh[2];
    af_default_cfg->fir_h_gain3_group0 = g_focus_cfg[vi_pipe].v_param_fir0.firh[3];
    af_default_cfg->fir_h_gain3_group1 = g_focus_cfg[vi_pipe].v_param_fir1.firh[3];
    af_default_cfg->fir_h_gain4_group0 = g_focus_cfg[vi_pipe].v_param_fir0.firh[4];
    af_default_cfg->fir_h_gain4_group1 = g_focus_cfg[vi_pipe].v_param_fir1.firh[4];

    /* AF FE crop */
    af_default_cfg->crop_enable        = hi_ext_af_crop_enable_read(vi_pipe);
    af_default_cfg->crop_pos_y         = hi_ext_af_crop_pos_y_read(vi_pipe);
    af_default_cfg->crop_pos_x         = hi_ext_af_crop_pos_x_read(vi_pipe);
    af_default_cfg->crop_h_size        = hi_ext_af_crop_hsize_read(vi_pipe);
    af_default_cfg->crop_v_size        = hi_ext_af_crop_vsize_read(vi_pipe);

    /* AF FE raw cfg */
    af_default_cfg->af_pos_sel         = 0;
    af_default_cfg->raw_mode           = 0x1; /* FE,raw mode only */
    af_default_cfg->gain_limit         = g_focus_cfg[vi_pipe].config.raw_cfg.gamma_gain_limit;
    af_default_cfg->gamma              = g_focus_cfg[vi_pipe].config.raw_cfg.gamma_value;
    af_default_cfg->offset_enable      = 0x1;

    // BE has subtracted Blacklevel
    isp_sensor_get_blc(vi_pipe, &sns_black_level);
    af_default_cfg->offset_gr           = sns_black_level->black_level[2] << 2;
    af_default_cfg->offset_gb           = sns_black_level->black_level[1] << 2;

    /* AF FE pre median filter */
    af_default_cfg->mean_enable        = g_focus_cfg[vi_pipe].config.pre_flt_cfg.en;
    af_default_cfg->mean_thres         = g_focus_cfg[vi_pipe].config.pre_flt_cfg.strength;

    /* level depend gain */
    af_default_cfg->iir0_ldg_enable   = g_focus_cfg[vi_pipe].h_param_iir0.ld.ld_en;
    af_default_cfg->iir_thre0_low     = g_focus_cfg[vi_pipe].h_param_iir0.ld.th_low;
    af_default_cfg->iir_thre0_high    = g_focus_cfg[vi_pipe].h_param_iir0.ld.th_high;
    af_default_cfg->iir_slope0_low    = g_focus_cfg[vi_pipe].h_param_iir0.ld.slp_low;
    af_default_cfg->iir_slope0_high   = g_focus_cfg[vi_pipe].h_param_iir0.ld.slp_high;
    af_default_cfg->iir_gain0_low     = g_focus_cfg[vi_pipe].h_param_iir0.ld.gain_low;
    af_default_cfg->iir_gain0_high    = g_focus_cfg[vi_pipe].h_param_iir0.ld.gain_high;

    af_default_cfg->iir1_ldg_enable     = g_focus_cfg[vi_pipe].h_param_iir1.ld.ld_en;
    af_default_cfg->iir_thre1_low     = g_focus_cfg[vi_pipe].h_param_iir1.ld.th_low;
    af_default_cfg->iir_thre1_high    = g_focus_cfg[vi_pipe].h_param_iir1.ld.th_high;
    af_default_cfg->iir_slope1_low    = g_focus_cfg[vi_pipe].h_param_iir1.ld.slp_low;
    af_default_cfg->iir_slope1_high   = g_focus_cfg[vi_pipe].h_param_iir1.ld.slp_high;
    af_default_cfg->iir_gain1_low     = g_focus_cfg[vi_pipe].h_param_iir1.ld.gain_low;
    af_default_cfg->iir_gain1_high    = g_focus_cfg[vi_pipe].h_param_iir1.ld.gain_high;

    af_default_cfg->fir0_ldg_enable     = g_focus_cfg[vi_pipe].v_param_fir0.ld.ld_en;
    af_default_cfg->fir_thre0_low     = g_focus_cfg[vi_pipe].v_param_fir0.ld.th_low;
    af_default_cfg->fir_thre0_high    = g_focus_cfg[vi_pipe].v_param_fir0.ld.th_high;
    af_default_cfg->fir_slope0_low    = g_focus_cfg[vi_pipe].v_param_fir0.ld.slp_low;
    af_default_cfg->fir_slope0_high   = g_focus_cfg[vi_pipe].v_param_fir0.ld.slp_high;
    af_default_cfg->fir_gain0_low     = g_focus_cfg[vi_pipe].v_param_fir0.ld.gain_low;
    af_default_cfg->fir_gain0_high    = g_focus_cfg[vi_pipe].v_param_fir0.ld.gain_high;

    af_default_cfg->fir1_ldg_enable     = g_focus_cfg[vi_pipe].v_param_fir1.ld.ld_en;
    af_default_cfg->fir_thre1_low     = g_focus_cfg[vi_pipe].v_param_fir1.ld.th_low;
    af_default_cfg->fir_thre1_high    = g_focus_cfg[vi_pipe].v_param_fir1.ld.th_high;
    af_default_cfg->fir_slope1_low    = g_focus_cfg[vi_pipe].v_param_fir1.ld.slp_low;
    af_default_cfg->fir_slope1_high   = g_focus_cfg[vi_pipe].v_param_fir1.ld.slp_high;
    af_default_cfg->fir_gain1_low     = g_focus_cfg[vi_pipe].v_param_fir1.ld.gain_low;
    af_default_cfg->fir_gain1_high    = g_focus_cfg[vi_pipe].v_param_fir1.ld.gain_high;

    /* AF FE coring */
    af_default_cfg->iir_thre0_coring  = g_focus_cfg[vi_pipe].h_param_iir0.coring.th;
    af_default_cfg->iir_slope0_coring = g_focus_cfg[vi_pipe].h_param_iir0.coring.slp;
    af_default_cfg->iir_peak0_coring  = g_focus_cfg[vi_pipe].h_param_iir0.coring.lmt;

    af_default_cfg->iir_thre1_coring  = g_focus_cfg[vi_pipe].h_param_iir1.coring.th;
    af_default_cfg->iir_slope1_coring = g_focus_cfg[vi_pipe].h_param_iir1.coring.slp;
    af_default_cfg->iir_peak1_coring  = g_focus_cfg[vi_pipe].h_param_iir1.coring.lmt;

    af_default_cfg->fir_thre0_coring  = g_focus_cfg[vi_pipe].v_param_fir0.coring.th;
    af_default_cfg->fir_slope0_coring = g_focus_cfg[vi_pipe].v_param_fir0.coring.slp;
    af_default_cfg->fir_peak0_coring  = g_focus_cfg[vi_pipe].v_param_fir0.coring.lmt;

    af_default_cfg->fir_thre1_coring  = g_focus_cfg[vi_pipe].v_param_fir1.coring.th;
    af_default_cfg->fir_slope1_coring = g_focus_cfg[vi_pipe].v_param_fir1.coring.slp;
    af_default_cfg->fir_peak1_coring  = g_focus_cfg[vi_pipe].v_param_fir1.coring.lmt;

    /* high luma counter */
    af_default_cfg->hiligh_thre       = (hi_u8)g_focus_cfg[vi_pipe].config.high_luma_th;

    /* AF output shift */
    af_default_cfg->acc_shift0_h      = g_focus_cfg[vi_pipe].fv_param.acc_shift_h[0];
    af_default_cfg->acc_shift1_h      = g_focus_cfg[vi_pipe].fv_param.acc_shift_h[1];
    af_default_cfg->acc_shift0_v      = g_focus_cfg[vi_pipe].fv_param.acc_shift_v[0];
    af_default_cfg->acc_shift1_v      = g_focus_cfg[vi_pipe].fv_param.acc_shift_v[1];
    af_default_cfg->acc_shift_y       = g_focus_cfg[vi_pipe].fv_param.acc_shift_y;
    af_default_cfg->shift_count_y     = g_focus_cfg[vi_pipe].fv_param.hl_cnt_shift;

    af_default_cfg->input_h_size      = g_focus_cfg[vi_pipe].config.h_size;
    af_default_cfg->input_v_size      = g_focus_cfg[vi_pipe].config.v_size;

    reg_cfg->cfg_key.bit1_af_fe_cfg = 1;

    return HI_SUCCESS;
}

hi_s32 isp_set_af_be_default(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 block_num = 0;
    hi_u8 i;
    hi_u8 j, shift0;
    hi_s16 g0, g1, g2;
    hi_isp_af_h_param *iir = HI_NULL;
    hi_u32 pls, plg;
    float pl, temp;

    hi_isp_bayer_format cfa;
    isp_af_reg_cfg    *af_default_cfg = HI_NULL;
    isp_usr_ctx        *isp_ctx = HI_NULL;
    ISP_GET_CTX(vi_pipe, isp_ctx);

    block_num = isp_ctx->block_attr.block_num;

    for (i = 0; i < block_num; i++) {
        af_default_cfg   = &reg_cfg->alg_reg_cfg[i].be_af_reg_cfg;

        cfa = hi_ext_system_rggb_cfg_read(vi_pipe);

        af_default_cfg->bayer_mode        = (hi_u8)cfa;
        af_default_cfg->lpf_enable         = HI_ISP_BE_AF_LPF_EN_DEFAULT;
        af_default_cfg->fir0_lpf_enable     = HI_ISP_BE_AF_FIR0_LPF_EN_DEFAULT;
        af_default_cfg->fir1_lpf_enable     = HI_ISP_BE_AF_FIR1_LPF_EN_DEFAULT;
        af_default_cfg->iir_plg_group0     = HI_ISP_BE_AF_IIR0_PLG_DEFAULT;
        af_default_cfg->iir_pls_group0     = HI_ISP_BE_AF_IIR0_PLS_DEFAULT;
        af_default_cfg->iir_plg_group1     = HI_ISP_BE_AF_IIR1_PLG_DEFAULT;
        af_default_cfg->iir_pls_group1     = HI_ISP_BE_AF_IIR1_PLS_DEFAULT;

        /* PLG and PLS */
        for (j = 0; j < 2; j++) {
            iir = j ? &(g_focus_cfg[vi_pipe].h_param_iir1) : &(g_focus_cfg[vi_pipe].h_param_iir0);

            shift0 = (hi_u8)iir->iir_shift_lut[0];
            g0 = iir->iir_gain[0];
            g1 = iir->iir_gain[1];
            g2 = iir->iir_gain[2];

            pl = (512.f / DIV_0_TO_1(512 - 2 * g1 - g2) * g0) / (1 << shift0);
            temp = pl;
            temp = MIN2(7 - floor(log(temp) / log(2)), 7);

            pls = (hi_u32)temp;
            plg = (hi_u32)((pl * (1 << pls)) + 0.5);

            if (j == 0) {
                af_default_cfg->iir_pls_group0 = pls;
                af_default_cfg->iir_plg_group0 = plg;
            } else {
                af_default_cfg->iir_pls_group1 = pls;
                af_default_cfg->iir_plg_group1 = plg;
            }
        }

        /* AF BE STATISTICS CONIFG START */
        if ((isp_ctx->block_attr.frame_rect.height < AF_MIN_HEIGHT ) ||
            (isp_ctx->block_attr.frame_rect.width < AF_MIN_WIDTH)) {
            af_default_cfg->af_enable      = HI_FALSE;
        } else {
            af_default_cfg->af_enable      = g_focus_cfg[vi_pipe].config.enable;
        }

        af_default_cfg->iir0_ds_enable    = g_focus_cfg[vi_pipe].h_param_iir0.narrow_band;
        af_default_cfg->iir1_ds_enable    = g_focus_cfg[vi_pipe].h_param_iir1.narrow_band;
        af_default_cfg->iir0_enable0      = g_focus_cfg[vi_pipe].h_param_iir0.iir_en[0];
        af_default_cfg->iir0_enable1      = g_focus_cfg[vi_pipe].h_param_iir0.iir_en[1];
        af_default_cfg->iir0_enable2      = g_focus_cfg[vi_pipe].h_param_iir0.iir_en[2];
        af_default_cfg->iir1_enable0      = g_focus_cfg[vi_pipe].h_param_iir1.iir_en[0];
        af_default_cfg->iir1_enable1      = g_focus_cfg[vi_pipe].h_param_iir1.iir_en[1];
        af_default_cfg->iir1_enable2      = g_focus_cfg[vi_pipe].h_param_iir1.iir_en[2];
        af_default_cfg->iir0_shift        = g_focus_cfg[vi_pipe].h_param_iir0.iir_shift;
        af_default_cfg->iir1_shift        = g_focus_cfg[vi_pipe].h_param_iir1.iir_shift;
        af_default_cfg->peak_mode         = g_focus_cfg[vi_pipe].config.peak_mode;
        af_default_cfg->squ_mode          = g_focus_cfg[vi_pipe].config.squ_mode;

        af_default_cfg->iir_gain0_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[0];
        af_default_cfg->iir_gain0_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[0];
        af_default_cfg->iir_gain1_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[1];
        af_default_cfg->iir_gain1_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[1];
        af_default_cfg->iir_gain2_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[2];
        af_default_cfg->iir_gain2_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[2];
        af_default_cfg->iir_gain3_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[3];
        af_default_cfg->iir_gain3_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[3];
        af_default_cfg->iir_gain4_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[4];
        af_default_cfg->iir_gain4_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[4];
        af_default_cfg->iir_gain5_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[5];
        af_default_cfg->iir_gain5_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[5];
        af_default_cfg->iir_gain6_group0  = g_focus_cfg[vi_pipe].h_param_iir0.iir_gain[6];
        af_default_cfg->iir_gain6_group1  = g_focus_cfg[vi_pipe].h_param_iir1.iir_gain[6];

        af_default_cfg->iir0_shift_group0 = g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[0];
        af_default_cfg->iir1_shift_group0 = g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[1];
        af_default_cfg->iir2_shift_group0 = g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[2];
        af_default_cfg->iir3_shift_group0 = g_focus_cfg[vi_pipe].h_param_iir0.iir_shift_lut[3];
        af_default_cfg->iir0_shift_group1 = g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[0];
        af_default_cfg->iir1_shift_group1 = g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[1];
        af_default_cfg->iir2_shift_group1 = g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[2];
        af_default_cfg->iir3_shift_group1 = g_focus_cfg[vi_pipe].h_param_iir1.iir_shift_lut[3];

        af_default_cfg->fir_h_gain0_group0 = g_focus_cfg[vi_pipe].v_param_fir0.firh[0];
        af_default_cfg->fir_h_gain0_group1 = g_focus_cfg[vi_pipe].v_param_fir1.firh[0];
        af_default_cfg->fir_h_gain1_group0 = g_focus_cfg[vi_pipe].v_param_fir0.firh[1];
        af_default_cfg->fir_h_gain1_group1 = g_focus_cfg[vi_pipe].v_param_fir1.firh[1];
        af_default_cfg->fir_h_gain2_group0 = g_focus_cfg[vi_pipe].v_param_fir0.firh[2];
        af_default_cfg->fir_h_gain2_group1 = g_focus_cfg[vi_pipe].v_param_fir1.firh[2];
        af_default_cfg->fir_h_gain3_group0 = g_focus_cfg[vi_pipe].v_param_fir0.firh[3];
        af_default_cfg->fir_h_gain3_group1 = g_focus_cfg[vi_pipe].v_param_fir1.firh[3];
        af_default_cfg->fir_h_gain4_group0 = g_focus_cfg[vi_pipe].v_param_fir0.firh[4];
        af_default_cfg->fir_h_gain4_group1 = g_focus_cfg[vi_pipe].v_param_fir1.firh[4];

        /* AF BE raw cfg */
        af_default_cfg->af_pos_sel         = g_focus_cfg[vi_pipe].config.statistics_pos;
        af_default_cfg->raw_mode           = ~(((g_focus_cfg[vi_pipe].config.statistics_pos) >> 0x1) & 0x1);
        if (isp_ctx->isp_yuv_mode == HI_TRUE) {
            af_default_cfg->af_pos_sel = 2;
            af_default_cfg->raw_mode = 0;
        }
        af_default_cfg->gain_limit        = g_focus_cfg[vi_pipe].config.raw_cfg.gamma_gain_limit;
        af_default_cfg->gamma            = g_focus_cfg[vi_pipe].config.raw_cfg.gamma_value;
        af_default_cfg->offset_enable      = 0x0;
        af_default_cfg->offset_gr        = 0x0;
        af_default_cfg->offset_gb        = 0x0;

        /* AF BE pre median filter */
        af_default_cfg->mean_enable        = g_focus_cfg[vi_pipe].config.pre_flt_cfg.en;
        af_default_cfg->mean_thres         = g_focus_cfg[vi_pipe].config.pre_flt_cfg.strength;

        /* level depend gain */
        af_default_cfg->iir0_ldg_enable     = g_focus_cfg[vi_pipe].h_param_iir0.ld.ld_en;
        af_default_cfg->iir_thre0_low     = g_focus_cfg[vi_pipe].h_param_iir0.ld.th_low;
        af_default_cfg->iir_thre0_high    = g_focus_cfg[vi_pipe].h_param_iir0.ld.th_high;
        af_default_cfg->iir_slope0_low    = g_focus_cfg[vi_pipe].h_param_iir0.ld.slp_low;
        af_default_cfg->iir_slope0_high   = g_focus_cfg[vi_pipe].h_param_iir0.ld.slp_high;
        af_default_cfg->iir_gain0_low     = g_focus_cfg[vi_pipe].h_param_iir0.ld.gain_low;
        af_default_cfg->iir_gain0_high    = g_focus_cfg[vi_pipe].h_param_iir0.ld.gain_high;

        af_default_cfg->iir1_ldg_enable     = g_focus_cfg[vi_pipe].h_param_iir1.ld.ld_en;
        af_default_cfg->iir_thre1_low     = g_focus_cfg[vi_pipe].h_param_iir1.ld.th_low;
        af_default_cfg->iir_thre1_high    = g_focus_cfg[vi_pipe].h_param_iir1.ld.th_high;
        af_default_cfg->iir_slope1_low    = g_focus_cfg[vi_pipe].h_param_iir1.ld.slp_low;
        af_default_cfg->iir_slope1_high   = g_focus_cfg[vi_pipe].h_param_iir1.ld.slp_high;
        af_default_cfg->iir_gain1_low     = g_focus_cfg[vi_pipe].h_param_iir1.ld.gain_low;
        af_default_cfg->iir_gain1_high    = g_focus_cfg[vi_pipe].h_param_iir1.ld.gain_high;

        af_default_cfg->fir0_ldg_enable     = g_focus_cfg[vi_pipe].v_param_fir0.ld.ld_en;
        af_default_cfg->fir_thre0_low     = g_focus_cfg[vi_pipe].v_param_fir0.ld.th_low;
        af_default_cfg->fir_thre0_high    = g_focus_cfg[vi_pipe].v_param_fir0.ld.th_high;
        af_default_cfg->fir_slope0_low    = g_focus_cfg[vi_pipe].v_param_fir0.ld.slp_low;
        af_default_cfg->fir_slope0_high   = g_focus_cfg[vi_pipe].v_param_fir0.ld.slp_high;
        af_default_cfg->fir_gain0_low     = g_focus_cfg[vi_pipe].v_param_fir0.ld.gain_low;
        af_default_cfg->fir_gain0_high    = g_focus_cfg[vi_pipe].v_param_fir0.ld.gain_high;

        af_default_cfg->fir1_ldg_enable     = g_focus_cfg[vi_pipe].v_param_fir1.ld.ld_en;
        af_default_cfg->fir_thre1_low     = g_focus_cfg[vi_pipe].v_param_fir1.ld.th_low;
        af_default_cfg->fir_thre1_high    = g_focus_cfg[vi_pipe].v_param_fir1.ld.th_high;
        af_default_cfg->fir_slope1_low    = g_focus_cfg[vi_pipe].v_param_fir1.ld.slp_low;
        af_default_cfg->fir_slope1_high   = g_focus_cfg[vi_pipe].v_param_fir1.ld.slp_high;
        af_default_cfg->fir_gain1_low     = g_focus_cfg[vi_pipe].v_param_fir1.ld.gain_low;
        af_default_cfg->fir_gain1_high    = g_focus_cfg[vi_pipe].v_param_fir1.ld.gain_high;

        /* AF BE coring */
        af_default_cfg->iir_thre0_coring  = g_focus_cfg[vi_pipe].h_param_iir0.coring.th;
        af_default_cfg->iir_slope0_coring = g_focus_cfg[vi_pipe].h_param_iir0.coring.slp;
        af_default_cfg->iir_peak0_coring  = g_focus_cfg[vi_pipe].h_param_iir0.coring.lmt;

        af_default_cfg->iir_thre1_coring  = g_focus_cfg[vi_pipe].h_param_iir1.coring.th;
        af_default_cfg->iir_slope1_coring = g_focus_cfg[vi_pipe].h_param_iir1.coring.slp;
        af_default_cfg->iir_peak1_coring  = g_focus_cfg[vi_pipe].h_param_iir1.coring.lmt;

        af_default_cfg->fir_thre0_coring  = g_focus_cfg[vi_pipe].v_param_fir0.coring.th;
        af_default_cfg->fir_slope0_coring = g_focus_cfg[vi_pipe].v_param_fir0.coring.slp;
        af_default_cfg->fir_peak0_coring  = g_focus_cfg[vi_pipe].v_param_fir0.coring.lmt;

        af_default_cfg->fir_thre1_coring  = g_focus_cfg[vi_pipe].v_param_fir1.coring.th;
        af_default_cfg->fir_slope1_coring = g_focus_cfg[vi_pipe].v_param_fir1.coring.slp;
        af_default_cfg->fir_peak1_coring  = g_focus_cfg[vi_pipe].v_param_fir1.coring.lmt;

        /* high luma counter */
        af_default_cfg->hiligh_thre       = (hi_u8)g_focus_cfg[vi_pipe].config.high_luma_th;

        /* AF output shift */
        af_default_cfg->acc_shift0_h      = g_focus_cfg[vi_pipe].fv_param.acc_shift_h[0];
        af_default_cfg->acc_shift1_h      = g_focus_cfg[vi_pipe].fv_param.acc_shift_h[1];
        af_default_cfg->acc_shift0_v      = g_focus_cfg[vi_pipe].fv_param.acc_shift_v[0];
        af_default_cfg->acc_shift1_v      = g_focus_cfg[vi_pipe].fv_param.acc_shift_v[1];
        af_default_cfg->acc_shift_y       = g_focus_cfg[vi_pipe].fv_param.acc_shift_y;
        af_default_cfg->shift_count_y     = g_focus_cfg[vi_pipe].fv_param.hl_cnt_shift;

        af_default_cfg->update_index     = 1;
        reg_cfg->cfg_key.bit1_af_be_cfg  = 1;
        reg_cfg->kernel_reg_cfg.cfg_key.bit1_be_af_cfg = 1;
    }

    isp_af_blk_cfg(vi_pipe, reg_cfg, isp_ctx);

    return HI_SUCCESS;
}

hi_s32 isp_af_fe_cfg_regs(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg)
{
    isp_af_reg_cfg    *af_default_cfg = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    af_default_cfg   = &reg_cfg->alg_reg_cfg[0].fe_af_reg_cfg;

    af_default_cfg->bayer_mode         = hi_ext_af_bayermode_read(vi_pipe);
    af_default_cfg->iir0_ds_enable     = hi_ext_af_iir0_ds_enable_read(vi_pipe);
    af_default_cfg->iir1_ds_enable     = hi_ext_af_iir1_ds_enable_read(vi_pipe);
    af_default_cfg->iir_plg_group0     = hi_ext_af_iir_plg_group0_read(vi_pipe);
    af_default_cfg->iir_pls_group0     = hi_ext_af_iir_pls_group0_read(vi_pipe);
    af_default_cfg->iir_plg_group1     = hi_ext_af_iir_plg_group1_read(vi_pipe);
    af_default_cfg->iir_pls_group1     = hi_ext_af_iir_pls_group1_read(vi_pipe);

    /* AF FE STATISTICS CONIFG START */
    af_default_cfg->af_enable         = (hi_ext_system_af_enable_read(vi_pipe) & 0x1);
    af_default_cfg->iir0_enable0      = hi_ext_af_iir0_enable0_read(vi_pipe);
    af_default_cfg->iir0_enable1      = hi_ext_af_iir0_enable1_read(vi_pipe);
    af_default_cfg->iir0_enable2      = hi_ext_af_iir0_enable2_read(vi_pipe);
    af_default_cfg->iir1_enable0      = hi_ext_af_iir1_enable0_read(vi_pipe);
    af_default_cfg->iir1_enable1      = hi_ext_af_iir1_enable1_read(vi_pipe);
    af_default_cfg->iir1_enable2      = hi_ext_af_iir1_enable2_read(vi_pipe);
    af_default_cfg->iir0_shift        = hi_ext_af_iir0_shift_read(vi_pipe);
    af_default_cfg->iir1_shift        = hi_ext_af_iir1_shift_read(vi_pipe);
    af_default_cfg->peak_mode         = hi_ext_af_peakmode_read(vi_pipe);
    af_default_cfg->squ_mode          = hi_ext_af_squmode_read(vi_pipe);
    af_default_cfg->window_hnum       = hi_ext_af_window_hnum_read(vi_pipe);
    af_default_cfg->window_vnum       = hi_ext_af_window_vnum_read(vi_pipe);
    af_default_cfg->iir_gain0_group0  = hi_ext_af_iir_gain0_group0_read(vi_pipe);
    af_default_cfg->iir_gain0_group1  = hi_ext_af_iir_gain0_group1_read(vi_pipe);
    af_default_cfg->iir_gain1_group0  = hi_ext_af_iir_gain1_group0_read(vi_pipe);
    af_default_cfg->iir_gain1_group1  = hi_ext_af_iir_gain1_group1_read(vi_pipe);
    af_default_cfg->iir_gain2_group0  = hi_ext_af_iir_gain2_group0_read(vi_pipe);
    af_default_cfg->iir_gain2_group1  = hi_ext_af_iir_gain2_group1_read(vi_pipe);
    af_default_cfg->iir_gain3_group0  = hi_ext_af_iir_gain3_group0_read(vi_pipe);
    af_default_cfg->iir_gain3_group1  = hi_ext_af_iir_gain3_group1_read(vi_pipe);
    af_default_cfg->iir_gain4_group0  = hi_ext_af_iir_gain4_group0_read(vi_pipe);
    af_default_cfg->iir_gain4_group1  = hi_ext_af_iir_gain4_group1_read(vi_pipe);
    af_default_cfg->iir_gain5_group0  = hi_ext_af_iir_gain5_group0_read(vi_pipe);
    af_default_cfg->iir_gain5_group1  = hi_ext_af_iir_gain5_group1_read(vi_pipe);
    af_default_cfg->iir_gain6_group0  = hi_ext_af_iir_gain6_group0_read(vi_pipe);
    af_default_cfg->iir_gain6_group1  = hi_ext_af_iir_gain6_group1_read(vi_pipe);

    af_default_cfg->iir0_shift_group0 = hi_ext_af_iir0_shift_group0_read(vi_pipe);
    af_default_cfg->iir1_shift_group0 = hi_ext_af_iir1_shift_group0_read(vi_pipe);
    af_default_cfg->iir2_shift_group0 = hi_ext_af_iir2_shift_group0_read(vi_pipe);
    af_default_cfg->iir3_shift_group0 = hi_ext_af_iir3_shift_group0_read(vi_pipe);
    af_default_cfg->iir0_shift_group1 = hi_ext_af_iir0_shift_group1_read(vi_pipe);
    af_default_cfg->iir1_shift_group1 = hi_ext_af_iir1_shift_group1_read(vi_pipe);
    af_default_cfg->iir2_shift_group1 = hi_ext_af_iir2_shift_group1_read(vi_pipe);
    af_default_cfg->iir3_shift_group1 = hi_ext_af_iir3_shift_group1_read(vi_pipe);

    af_default_cfg->fir_h_gain0_group0 = hi_ext_af_fir_h_gain0_group0_read(vi_pipe);
    af_default_cfg->fir_h_gain0_group1 = hi_ext_af_fir_h_gain0_group1_read(vi_pipe);
    af_default_cfg->fir_h_gain1_group0 = hi_ext_af_fir_h_gain1_group0_read(vi_pipe);
    af_default_cfg->fir_h_gain1_group1 = hi_ext_af_fir_h_gain1_group1_read(vi_pipe);
    af_default_cfg->fir_h_gain2_group0 = hi_ext_af_fir_h_gain2_group0_read(vi_pipe);
    af_default_cfg->fir_h_gain2_group1 = hi_ext_af_fir_h_gain2_group1_read(vi_pipe);
    af_default_cfg->fir_h_gain3_group0 = hi_ext_af_fir_h_gain3_group0_read(vi_pipe);
    af_default_cfg->fir_h_gain3_group1 = hi_ext_af_fir_h_gain3_group1_read(vi_pipe);
    af_default_cfg->fir_h_gain4_group0 = hi_ext_af_fir_h_gain4_group0_read(vi_pipe);
    af_default_cfg->fir_h_gain4_group1 = hi_ext_af_fir_h_gain4_group1_read(vi_pipe);

    /* AF FE crop */
    af_default_cfg->crop_enable       = hi_ext_af_crop_enable_read(vi_pipe);
    af_default_cfg->crop_pos_y        = hi_ext_af_crop_pos_y_read(vi_pipe);
    af_default_cfg->crop_pos_x        = hi_ext_af_crop_pos_x_read(vi_pipe);
    af_default_cfg->crop_h_size       = hi_ext_af_crop_hsize_read(vi_pipe);
    af_default_cfg->crop_v_size       = hi_ext_af_crop_vsize_read(vi_pipe);

    /* AF FE raw cfg */
    af_default_cfg->af_pos_sel         = 0;
    af_default_cfg->raw_mode           = 0x1; /* FE,raw mode only */
    af_default_cfg->gain_limit         = hi_ext_af_gain_limit_read(vi_pipe);
    af_default_cfg->gamma              = hi_ext_af_gamma_read(vi_pipe);
    af_default_cfg->offset_enable      = 0x1;

    // FE has subtracted Blacklevel
    isp_sensor_get_blc(vi_pipe, &sns_black_level);
    af_default_cfg->offset_gr        = sns_black_level->black_level[2] << 2;
    af_default_cfg->offset_gb        = sns_black_level->black_level[1] << 2;

    /* AF FE pre median filter */
    af_default_cfg->mean_enable        = hi_ext_af_mean_enable_read(vi_pipe);
    af_default_cfg->mean_thres         = hi_ext_af_mean_thres_read(vi_pipe);

    /* level depend gain */
    af_default_cfg->iir0_ldg_enable   = hi_ext_af_iir0_ldg_enable_read(vi_pipe);
    af_default_cfg->iir_thre0_low     = hi_ext_af_iir_thre0_low_read(vi_pipe);
    af_default_cfg->iir_thre0_high    = hi_ext_af_iir_thre0_high_read(vi_pipe);
    af_default_cfg->iir_slope0_low    = hi_ext_af_iir_slope0_low_read(vi_pipe);
    af_default_cfg->iir_slope0_high   = hi_ext_af_iir_slope0_high_read(vi_pipe);
    af_default_cfg->iir_gain0_low     = hi_ext_af_iir_gain0_low_read(vi_pipe);
    af_default_cfg->iir_gain0_high    = hi_ext_af_iir_gain0_high_read(vi_pipe);

    af_default_cfg->iir1_ldg_enable     = hi_ext_af_iir1_ldg_enable_read(vi_pipe);
    af_default_cfg->iir_thre1_low     = hi_ext_af_iir_thre1_low_read(vi_pipe);
    af_default_cfg->iir_thre1_high    = hi_ext_af_iir_thre1_high_read(vi_pipe);
    af_default_cfg->iir_slope1_low    = hi_ext_af_iir_slope1_low_read(vi_pipe);
    af_default_cfg->iir_slope1_high   = hi_ext_af_iir_slope1_high_read(vi_pipe);
    af_default_cfg->iir_gain1_low     = hi_ext_af_iir_gain1_low_read(vi_pipe);
    af_default_cfg->iir_gain1_high    = hi_ext_af_iir_gain1_high_read(vi_pipe);

    af_default_cfg->fir0_ldg_enable     = hi_ext_af_fir0_ldg_enable_read(vi_pipe);
    af_default_cfg->fir_thre0_low     = hi_ext_af_fir_thre0_low_read(vi_pipe);
    af_default_cfg->fir_thre0_high    = hi_ext_af_fir_thre0_high_read(vi_pipe);
    af_default_cfg->fir_slope0_low    = hi_ext_af_fir_slope0_low_read(vi_pipe);
    af_default_cfg->fir_slope0_high   = hi_ext_af_fir_slope0_high_read(vi_pipe);
    af_default_cfg->fir_gain0_low     = hi_ext_af_fir_gain0_low_read(vi_pipe);
    af_default_cfg->fir_gain0_high    = hi_ext_af_fir_gain0_high_read(vi_pipe);

    af_default_cfg->fir1_ldg_enable     = hi_ext_af_fir1_ldg_enable_read(vi_pipe);
    af_default_cfg->fir_thre1_low     = hi_ext_af_fir_thre1_low_read(vi_pipe);
    af_default_cfg->fir_thre1_high    = hi_ext_af_fir_thre1_high_read(vi_pipe);
    af_default_cfg->fir_slope1_low    = hi_ext_af_fir_slope1_low_read(vi_pipe);
    af_default_cfg->fir_slope1_high   = hi_ext_af_fir_slope1_high_read(vi_pipe);
    af_default_cfg->fir_gain1_low     = hi_ext_af_fir_gain1_low_read(vi_pipe);
    af_default_cfg->fir_gain1_high    = hi_ext_af_fir_gain1_high_read(vi_pipe);

    /* AF FE coring */
    af_default_cfg->iir_thre0_coring  = hi_ext_af_iir_thre0_coring_read(vi_pipe);
    af_default_cfg->iir_slope0_coring = hi_ext_af_iir_slope0_coring_read(vi_pipe);
    af_default_cfg->iir_peak0_coring  = hi_ext_af_iir_peak0_coring_read(vi_pipe);

    af_default_cfg->iir_thre1_coring  = hi_ext_af_iir_thre1_coring_read(vi_pipe);
    af_default_cfg->iir_slope1_coring = hi_ext_af_iir_slope1_coring_read(vi_pipe);
    af_default_cfg->iir_peak1_coring  = hi_ext_af_iir_peak1_coring_read(vi_pipe);

    af_default_cfg->fir_thre0_coring  = hi_ext_af_fir_thre0_coring_read(vi_pipe);
    af_default_cfg->fir_slope0_coring = hi_ext_af_fir_slope0_coring_read(vi_pipe);
    af_default_cfg->fir_peak0_coring  = hi_ext_af_fir_peak0_coring_read(vi_pipe);

    af_default_cfg->fir_thre1_coring  = hi_ext_af_fir_thre1_coring_read(vi_pipe);
    af_default_cfg->fir_slope1_coring = hi_ext_af_fir_slope1_coring_read(vi_pipe);
    af_default_cfg->fir_peak1_coring  = hi_ext_af_fir_peak1_coring_read(vi_pipe);

    /* high luma counter */
    af_default_cfg->hiligh_thre       = hi_ext_af_hiligh_thre_read(vi_pipe);

    /* AF output shift */
    af_default_cfg->acc_shift0_h      = hi_ext_af_acc_shift0_h_read(vi_pipe);
    af_default_cfg->acc_shift1_h      = hi_ext_af_acc_shift1_h_read(vi_pipe);
    af_default_cfg->acc_shift0_v      = hi_ext_af_acc_shift0_v_read(vi_pipe);
    af_default_cfg->acc_shift1_v      = hi_ext_af_acc_shift1_v_read(vi_pipe);
    af_default_cfg->acc_shift_y       = hi_ext_af_acc_shift_y_read(vi_pipe);
    af_default_cfg->shift_count_y     = hi_ext_af_shift_count_y_read(vi_pipe);

    af_default_cfg->input_h_size      = hi_ext_af_input_hsize_read(vi_pipe);
    af_default_cfg->input_v_size      = hi_ext_af_input_vsize_read(vi_pipe);

    reg_cfg->cfg_key.bit1_af_fe_cfg   = 1;

    return HI_SUCCESS;
}

hi_s32 isp_af_be_cfg_regs(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 block_num = 0;
    hi_u8 window_hnum;
    hi_u8 i;
    isp_af_reg_cfg    *af_default_cfg = HI_NULL;
    isp_usr_ctx       *isp_ctx        = HI_NULL;
    ISP_GET_CTX(vi_pipe, isp_ctx);

    block_num = isp_ctx->block_attr.block_num;

    for (i = 0; i < block_num; i++) {
        af_default_cfg   = &reg_cfg->alg_reg_cfg[i].be_af_reg_cfg;

        af_default_cfg->bayer_mode        = hi_ext_af_bayermode_read(vi_pipe);
        af_default_cfg->iir0_ds_enable      = hi_ext_af_iir0_ds_enable_read(vi_pipe);
        af_default_cfg->iir1_ds_enable      = hi_ext_af_iir1_ds_enable_read(vi_pipe);

        af_default_cfg->iir_plg_group0     = hi_ext_af_iir_plg_group0_read(vi_pipe);
        af_default_cfg->iir_pls_group0     = hi_ext_af_iir_pls_group0_read(vi_pipe);
        af_default_cfg->iir_plg_group1     = hi_ext_af_iir_plg_group1_read(vi_pipe);
        af_default_cfg->iir_pls_group1     = hi_ext_af_iir_pls_group1_read(vi_pipe);

        /* AF BE STATISTICS CONIFG START */
        af_default_cfg->af_enable          = ((hi_ext_system_af_enable_read(vi_pipe) >> 1) & 0x1);
        af_default_cfg->iir0_enable0       = hi_ext_af_iir0_enable0_read(vi_pipe);
        af_default_cfg->iir0_enable1       = hi_ext_af_iir0_enable1_read(vi_pipe);
        af_default_cfg->iir0_enable2       = hi_ext_af_iir0_enable2_read(vi_pipe);
        af_default_cfg->iir1_enable0       = hi_ext_af_iir1_enable0_read(vi_pipe);
        af_default_cfg->iir1_enable1       = hi_ext_af_iir1_enable1_read(vi_pipe);
        af_default_cfg->iir1_enable2       = hi_ext_af_iir1_enable2_read(vi_pipe);
        af_default_cfg->iir0_shift        = hi_ext_af_iir0_shift_read(vi_pipe);
        af_default_cfg->iir1_shift        = hi_ext_af_iir1_shift_read(vi_pipe);
        af_default_cfg->peak_mode         = hi_ext_af_peakmode_read(vi_pipe);
        af_default_cfg->squ_mode          = hi_ext_af_squmode_read(vi_pipe);
        af_default_cfg->iir_gain0_group0  = hi_ext_af_iir_gain0_group0_read(vi_pipe);
        af_default_cfg->iir_gain0_group1  = hi_ext_af_iir_gain0_group1_read(vi_pipe);
        af_default_cfg->iir_gain1_group0  = hi_ext_af_iir_gain1_group0_read(vi_pipe);
        af_default_cfg->iir_gain1_group1  = hi_ext_af_iir_gain1_group1_read(vi_pipe);
        af_default_cfg->iir_gain2_group0  = hi_ext_af_iir_gain2_group0_read(vi_pipe);
        af_default_cfg->iir_gain2_group1  = hi_ext_af_iir_gain2_group1_read(vi_pipe);
        af_default_cfg->iir_gain3_group0  = hi_ext_af_iir_gain3_group0_read(vi_pipe);
        af_default_cfg->iir_gain3_group1  = hi_ext_af_iir_gain3_group1_read(vi_pipe);
        af_default_cfg->iir_gain4_group0  = hi_ext_af_iir_gain4_group0_read(vi_pipe);
        af_default_cfg->iir_gain4_group1  = hi_ext_af_iir_gain4_group1_read(vi_pipe);
        af_default_cfg->iir_gain5_group0  = hi_ext_af_iir_gain5_group0_read(vi_pipe);
        af_default_cfg->iir_gain5_group1  = hi_ext_af_iir_gain5_group1_read(vi_pipe);
        af_default_cfg->iir_gain6_group0  = hi_ext_af_iir_gain6_group0_read(vi_pipe);
        af_default_cfg->iir_gain6_group1  = hi_ext_af_iir_gain6_group1_read(vi_pipe);

        af_default_cfg->iir0_shift_group0 = hi_ext_af_iir0_shift_group0_read(vi_pipe);
        af_default_cfg->iir1_shift_group0 = hi_ext_af_iir1_shift_group0_read(vi_pipe);
        af_default_cfg->iir2_shift_group0 = hi_ext_af_iir2_shift_group0_read(vi_pipe);
        af_default_cfg->iir3_shift_group0 = hi_ext_af_iir3_shift_group0_read(vi_pipe);
        af_default_cfg->iir0_shift_group1 = hi_ext_af_iir0_shift_group1_read(vi_pipe);
        af_default_cfg->iir1_shift_group1 = hi_ext_af_iir1_shift_group1_read(vi_pipe);
        af_default_cfg->iir2_shift_group1 = hi_ext_af_iir2_shift_group1_read(vi_pipe);
        af_default_cfg->iir3_shift_group1 = hi_ext_af_iir3_shift_group1_read(vi_pipe);

        af_default_cfg->fir_h_gain0_group0 = hi_ext_af_fir_h_gain0_group0_read(vi_pipe);
        af_default_cfg->fir_h_gain0_group1 = hi_ext_af_fir_h_gain0_group1_read(vi_pipe);
        af_default_cfg->fir_h_gain1_group0 = hi_ext_af_fir_h_gain1_group0_read(vi_pipe);
        af_default_cfg->fir_h_gain1_group1 = hi_ext_af_fir_h_gain1_group1_read(vi_pipe);
        af_default_cfg->fir_h_gain2_group0 = hi_ext_af_fir_h_gain2_group0_read(vi_pipe);
        af_default_cfg->fir_h_gain2_group1 = hi_ext_af_fir_h_gain2_group1_read(vi_pipe);
        af_default_cfg->fir_h_gain3_group0 = hi_ext_af_fir_h_gain3_group0_read(vi_pipe);
        af_default_cfg->fir_h_gain3_group1 = hi_ext_af_fir_h_gain3_group1_read(vi_pipe);
        af_default_cfg->fir_h_gain4_group0 = hi_ext_af_fir_h_gain4_group0_read(vi_pipe);
        af_default_cfg->fir_h_gain4_group1 = hi_ext_af_fir_h_gain4_group1_read(vi_pipe);

        window_hnum      = hi_ext_af_window_hnum_read(vi_pipe);

        if (i < window_hnum % DIV_0_TO_1(block_num)) {
            af_default_cfg->window_hnum = window_hnum / DIV_0_TO_1(block_num) + 1;
        } else {
            af_default_cfg->window_hnum = window_hnum / DIV_0_TO_1(block_num);
        }

        af_default_cfg->window_vnum     = hi_ext_af_window_vnum_read(vi_pipe);
        reg_cfg->kernel_reg_cfg.alg_kernel_cfg[i].be_af_zone_cfg.colum =  af_default_cfg->window_hnum;
        reg_cfg->kernel_reg_cfg.alg_kernel_cfg[i].be_af_zone_cfg.row   =  af_default_cfg->window_vnum;

        /* AF BE crop */
        if (block_num == 1) {
            af_default_cfg->crop_enable       = hi_ext_af_crop_enable_read(vi_pipe);
            af_default_cfg->crop_pos_y        = hi_ext_af_crop_pos_y_read(vi_pipe);
            af_default_cfg->crop_pos_x        = hi_ext_af_crop_pos_x_read(vi_pipe);
            af_default_cfg->crop_h_size       = hi_ext_af_crop_hsize_read(vi_pipe);
            af_default_cfg->crop_v_size       = hi_ext_af_crop_vsize_read(vi_pipe);
        }

        /* AF BE raw cfg */
        af_default_cfg->af_pos_sel         = hi_ext_af_pos_sel_read(vi_pipe);
        af_default_cfg->raw_mode           = hi_ext_af_rawmode_read(vi_pipe);

        if (isp_ctx->isp_yuv_mode == HI_TRUE) {
            af_default_cfg->af_pos_sel = 2;
            af_default_cfg->raw_mode = 0;
        }

        af_default_cfg->gain_limit       = hi_ext_af_gain_limit_read(vi_pipe);
        af_default_cfg->gamma            = hi_ext_af_gamma_read(vi_pipe);
        af_default_cfg->offset_enable    = 0x0;
        af_default_cfg->offset_gr        = 0x0;
        af_default_cfg->offset_gb        = 0x0;

        /* AF BE pre median filter */
        af_default_cfg->mean_enable      = hi_ext_af_mean_enable_read(vi_pipe);
        af_default_cfg->mean_thres       = hi_ext_af_mean_thres_read(vi_pipe);

        /* level depend gain */
        af_default_cfg->iir0_ldg_enable   = hi_ext_af_iir0_ldg_enable_read(vi_pipe);
        af_default_cfg->iir_thre0_low     = hi_ext_af_iir_thre0_low_read(vi_pipe);
        af_default_cfg->iir_thre0_high    = hi_ext_af_iir_thre0_high_read(vi_pipe);
        af_default_cfg->iir_slope0_low    = hi_ext_af_iir_slope0_low_read(vi_pipe);
        af_default_cfg->iir_slope0_high   = hi_ext_af_iir_slope0_high_read(vi_pipe);
        af_default_cfg->iir_gain0_low     = hi_ext_af_iir_gain0_low_read(vi_pipe);
        af_default_cfg->iir_gain0_high    = hi_ext_af_iir_gain0_high_read(vi_pipe);

        af_default_cfg->iir1_ldg_enable   = hi_ext_af_iir1_ldg_enable_read(vi_pipe);
        af_default_cfg->iir_thre1_low     = hi_ext_af_iir_thre1_low_read(vi_pipe);
        af_default_cfg->iir_thre1_high    = hi_ext_af_iir_thre1_high_read(vi_pipe);
        af_default_cfg->iir_slope1_low    = hi_ext_af_iir_slope1_low_read(vi_pipe);
        af_default_cfg->iir_slope1_high   = hi_ext_af_iir_slope1_high_read(vi_pipe);
        af_default_cfg->iir_gain1_low     = hi_ext_af_iir_gain1_low_read(vi_pipe);
        af_default_cfg->iir_gain1_high    = hi_ext_af_iir_gain1_high_read(vi_pipe);

        af_default_cfg->fir0_ldg_enable   = hi_ext_af_fir0_ldg_enable_read(vi_pipe);
        af_default_cfg->fir_thre0_low     = hi_ext_af_fir_thre0_low_read(vi_pipe);
        af_default_cfg->fir_thre0_high    = hi_ext_af_fir_thre0_high_read(vi_pipe);
        af_default_cfg->fir_slope0_low    = hi_ext_af_fir_slope0_low_read(vi_pipe);
        af_default_cfg->fir_slope0_high   = hi_ext_af_fir_slope0_high_read(vi_pipe);
        af_default_cfg->fir_gain0_low     = hi_ext_af_fir_gain0_low_read(vi_pipe);
        af_default_cfg->fir_gain0_high    = hi_ext_af_fir_gain0_high_read(vi_pipe);

        af_default_cfg->fir1_ldg_enable   = hi_ext_af_fir1_ldg_enable_read(vi_pipe);
        af_default_cfg->fir_thre1_low     = hi_ext_af_fir_thre1_low_read(vi_pipe);
        af_default_cfg->fir_thre1_high    = hi_ext_af_fir_thre1_high_read(vi_pipe);
        af_default_cfg->fir_slope1_low    = hi_ext_af_fir_slope1_low_read(vi_pipe);
        af_default_cfg->fir_slope1_high   = hi_ext_af_fir_slope1_high_read(vi_pipe);
        af_default_cfg->fir_gain1_low     = hi_ext_af_fir_gain1_low_read(vi_pipe);
        af_default_cfg->fir_gain1_high    = hi_ext_af_fir_gain1_high_read(vi_pipe);

        /* AF BE coring */
        af_default_cfg->iir_thre0_coring  = hi_ext_af_iir_thre0_coring_read(vi_pipe);
        af_default_cfg->iir_slope0_coring = hi_ext_af_iir_slope0_coring_read(vi_pipe);
        af_default_cfg->iir_peak0_coring  = hi_ext_af_iir_peak0_coring_read(vi_pipe);

        af_default_cfg->iir_thre1_coring  = hi_ext_af_iir_thre1_coring_read(vi_pipe);
        af_default_cfg->iir_slope1_coring = hi_ext_af_iir_slope1_coring_read(vi_pipe);
        af_default_cfg->iir_peak1_coring  = hi_ext_af_iir_peak1_coring_read(vi_pipe);

        af_default_cfg->fir_thre0_coring  = hi_ext_af_fir_thre0_coring_read(vi_pipe);
        af_default_cfg->fir_slope0_coring = hi_ext_af_fir_slope0_coring_read(vi_pipe);
        af_default_cfg->fir_peak0_coring  = hi_ext_af_fir_peak0_coring_read(vi_pipe);

        af_default_cfg->fir_thre1_coring  = hi_ext_af_fir_thre1_coring_read(vi_pipe);
        af_default_cfg->fir_slope1_coring = hi_ext_af_fir_slope1_coring_read(vi_pipe);
        af_default_cfg->fir_peak1_coring  = hi_ext_af_fir_peak1_coring_read(vi_pipe);

        /* high luma counter */
        af_default_cfg->hiligh_thre       = hi_ext_af_hiligh_thre_read(vi_pipe);

        /* AF output shift */
        af_default_cfg->acc_shift0_h      = hi_ext_af_acc_shift0_h_read(vi_pipe);
        af_default_cfg->acc_shift1_h      = hi_ext_af_acc_shift1_h_read(vi_pipe);
        af_default_cfg->acc_shift0_v      = hi_ext_af_acc_shift0_v_read(vi_pipe);
        af_default_cfg->acc_shift1_v      = hi_ext_af_acc_shift1_v_read(vi_pipe);
        af_default_cfg->acc_shift_y       = hi_ext_af_acc_shift_y_read(vi_pipe);
        af_default_cfg->shift_count_y     = hi_ext_af_shift_count_y_read(vi_pipe);

        af_default_cfg->update_index    += 1;
        reg_cfg->cfg_key.bit1_af_be_cfg  = 1;

        reg_cfg->kernel_reg_cfg.cfg_key.bit1_be_af_cfg = 1;
    }

    return HI_SUCCESS;
}

static hi_void isp_af_image_res_write(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    ISP_GET_CTX(vi_pipe, isp_ctx);

    hi_ext_af_crop_pos_x_write(vi_pipe, 0);
    hi_ext_af_crop_pos_y_write(vi_pipe, 0);
    hi_ext_af_crop_hsize_write(vi_pipe, isp_ctx->block_attr.frame_rect.width / 8 * 8);
    hi_ext_af_crop_vsize_write(vi_pipe, isp_ctx->block_attr.frame_rect.height / 2 * 2);

    isp_af_blk_cfg(vi_pipe, reg_cfg, isp_ctx);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].be_af_reg_cfg.update_index++;
    }

    reg_cfg->cfg_key.bit1_af_fe_cfg = 1;
}

static hi_void isp_af_wdr_mode_set(VI_PIPE vi_pipe, hi_void *reg_cfg_info)
{
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_info;

    reg_cfg->cfg_key.bit1_af_fe_cfg = 1;
}

hi_s32 isp_af_init(VI_PIPE vi_pipe, hi_void *reg_cfg)
{
    /* some registers about AF module shound be initial */
    isp_af_ext_regs_default(vi_pipe);
    isp_set_af_fe_default(vi_pipe, (isp_reg_cfg *)reg_cfg);
    isp_set_af_be_default(vi_pipe, (isp_reg_cfg *)reg_cfg);

    return HI_SUCCESS;
}

hi_s32 isp_af_run(VI_PIPE vi_pipe, const hi_void *stat_info,
                  hi_void *reg_cfg_info, hi_s32 rsv)
{
    isp_usr_ctx          *isp_ctx   = HI_NULL;

    ISP_GET_CTX(vi_pipe, isp_ctx);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    if (isp_ctx->linkage.snap_state == HI_TRUE) {
        return HI_SUCCESS;
    }

    if (hi_ext_af_set_flag_read(vi_pipe) == HI_EXT_AF_SET_FLAG_ENABLE) {
        hi_ext_af_set_flag_write(vi_pipe, HI_EXT_AF_SET_FLAG_DISABLE);
        isp_af_fe_cfg_regs(vi_pipe, (isp_reg_cfg *)reg_cfg_info);
        isp_af_be_cfg_regs(vi_pipe, (isp_reg_cfg *)reg_cfg_info);
    }

    return HI_SUCCESS;
}

hi_s32 isp_af_ctrl(VI_PIPE vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr  *reg_cfg_attr    = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            ISP_REGCFG_GET_CTX(vi_pipe, reg_cfg_attr);
            ISP_CHECK_POINTER(reg_cfg_attr);
            isp_af_wdr_mode_set(vi_pipe, (HI_VOID *)&reg_cfg_attr->reg_cfg);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            ISP_REGCFG_GET_CTX(vi_pipe, reg_cfg_attr);
            ISP_CHECK_POINTER(reg_cfg_attr);
            isp_af_image_res_write(vi_pipe, &reg_cfg_attr->reg_cfg);
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

hi_s32 isp_af_exit(VI_PIPE vi_pipe)
{
    return HI_SUCCESS;
}

hi_s32 isp_alg_register_af(VI_PIPE vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    ISP_GET_CTX(vi_pipe, isp_ctx);

    algs = ISP_SearchAlg(isp_ctx->algs);
    ISP_CHECK_POINTER(algs);

    algs->alg_type = ISP_ALG_AF;
    algs->alg_func.pfn_alg_init = isp_af_init;
    algs->alg_func.pfn_alg_run  = isp_af_run;
    algs->alg_func.pfn_alg_ctrl = isp_af_ctrl;
    algs->alg_func.pfn_alg_exit = isp_af_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
