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

#define LUT_FACTOR (7)

typedef struct {
    hi_bool pre_defect_pixel;
    hi_u8   black_level_change;
    hi_u8   wdr_mode_state;
    hi_u16  black_level[ISP_BAYER_CHN_NUM];
    hi_u16  rm_diff_black_level[ISP_BAYER_CHN_NUM];
    hi_u16  actual_black_level[ISP_BAYER_CHN_NUM];
    hi_isp_op_type op_type;
} isp_blacklevel_ctx;

isp_blacklevel_ctx g_black_level_ctx[ISP_MAX_PIPE_NUM] = {{0}};
#define BLACKLEVEL_GET_CTX(dev, black_level_ctx)   black_level_ctx = &g_black_level_ctx[dev]

static hi_void blc_initialize(VI_PIPE vi_pipe)
{
    hi_u8  i;
    hi_u8  wdr_mode;
    isp_blacklevel_ctx      *black_level_ctx = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    isp_usr_ctx             *isp_ctx         = HI_NULL;

    BLACKLEVEL_GET_CTX(vi_pipe, black_level_ctx);
    isp_sensor_get_blc(vi_pipe, &sns_black_level);
    ISP_GET_CTX(vi_pipe, isp_ctx);

    wdr_mode = isp_ctx->sns_wdr_mode;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        black_level_ctx->black_level[i] = sns_black_level->black_level[i];
    }

    if (IS_LINEAR_MODE(wdr_mode) || IS_BUILT_IN_WDR_MODE(wdr_mode)) {
        black_level_ctx->wdr_mode_state = HI_FALSE;
    } else {
        black_level_ctx->wdr_mode_state = HI_TRUE;
    }

    black_level_ctx->pre_defect_pixel = HI_FALSE;
    black_level_ctx->op_type          = OP_TYPE_AUTO;
}

static hi_void blc_ext_regs_initialize(VI_PIPE vi_pipe)
{
    isp_blacklevel_ctx        *black_level_ctx    = HI_NULL;

    BLACKLEVEL_GET_CTX(vi_pipe, black_level_ctx);

    hi_ext_system_black_level_manual_mode_write(vi_pipe, HI_EXT_SYSTEM_BLACK_LEVEL_MANUAL_MODE_DEFAULT);
    hi_ext_system_black_level_change_write(vi_pipe, HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DEFAULT);
    hi_ext_system_black_level_00_write(vi_pipe, black_level_ctx->black_level[0]);
    hi_ext_system_black_level_01_write(vi_pipe, black_level_ctx->black_level[1]);
    hi_ext_system_black_level_10_write(vi_pipe, black_level_ctx->black_level[2]);
    hi_ext_system_black_level_11_write(vi_pipe, black_level_ctx->black_level[3]);

    hi_ext_system_black_level_query_00_write(vi_pipe, black_level_ctx->actual_black_level[0]);
    hi_ext_system_black_level_query_01_write(vi_pipe, black_level_ctx->actual_black_level[1]);
    hi_ext_system_black_level_query_10_write(vi_pipe, black_level_ctx->actual_black_level[2]);
    hi_ext_system_black_level_query_11_write(vi_pipe, black_level_ctx->actual_black_level[3]);
}

static hi_void balance_black_level(isp_blacklevel_ctx *black_level_ctx)
{
    hi_u8  i;
    hi_u16 offset_min;

    /* calulate Min blacklevel of RGrGbG channel */
    offset_min = black_level_ctx->black_level[0];
    for (i = 1; i < ISP_BAYER_CHN_NUM; i++) {
        if (offset_min > black_level_ctx->black_level[i]) {
            offset_min = black_level_ctx->black_level[i];
        }
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        black_level_ctx->rm_diff_black_level[i] = offset_min;
    }
}

static hi_s32 blc_linear_interpol(hi_u16 xm, hi_u16 x0, hi_s32 y0, hi_u16 x1, hi_s32 y1, hi_u8 shift)
{
    hi_s32 ym;

    if (xm <= (x0 << shift)) {
        return y0;
    }
    if (xm >= (x1 << shift)) {
        return y1;
    }

    ym = (y1 - y0) * (xm - (x0 << shift)) / DIV_0_TO_1(x1 - x0) + y0;

    return ym;
}

static hi_void get_built_in_expander_blc(const hi_isp_cmos_split *sns_split, hi_u16 *sensor_blc, hi_u16 *expander_blc)
{
    hi_u8  i, j;
    hi_u8  shift    = 12 - LUT_FACTOR;
    hi_u8  index_up  = ISP_SPLIT_POINT_NUM - 1;
    hi_u8  index_low = 0;
    hi_u8  x[ISP_SPLIT_POINT_NUM + 1]  = {0};
    hi_u16 y[ISP_SPLIT_POINT_NUM + 1] = {0};

    for (i = 1; i < ISP_SPLIT_POINT_NUM + 1; i++) {
        x[i]  = sns_split->split_point[i - 1].x;
        y[i] = sns_split->split_point[i - 1].y;
    }

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        index_up  = ISP_SPLIT_POINT_NUM - 1;

        for (i = 0; i < ISP_SPLIT_POINT_NUM; i++) {
            if ((sensor_blc[j] >> shift) < x[i]) {
                index_up = i;
                break;
            }
        }

        index_low = (hi_u8)MAX2((hi_s8)index_up - 1, 0);

        expander_blc[j] = ((hi_u16)blc_linear_interpol(sensor_blc[j], x[index_low], y[index_low], x[index_up], y[index_up], shift)) >> (1 + shift);
    }
}

static hi_void be_blc_dyna_regs_linear(VI_PIPE vi_pipe, isp_be_blc_cfg *be_blc_cfg, isp_blacklevel_ctx *black_level_ctx)
{
    hi_u8 i, j;

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
            be_blc_cfg->wdr_dg_blc[i].usr_reg_cfg.blc[j]  = black_level_ctx->rm_diff_black_level[j] << 2; /* 4DG */
            be_blc_cfg->wdr_blc[i].usr_reg_cfg.blc[j]     = black_level_ctx->rm_diff_black_level[j];     /* WDR */
            be_blc_cfg->wdr_blc[i].usr_reg_cfg.out_blc    = 0;                                          /* WDR */
            be_blc_cfg->flicker_blc[i].usr_reg_cfg.blc[j] = 0;                                          /* flicker */
        }

        be_blc_cfg->split_blc.usr_reg_cfg.blc[j]    = 0;                                           /* split */
        be_blc_cfg->log_lut_blc.usr_reg_cfg.blc[j]  = black_level_ctx->rm_diff_black_level[j] << 4; /* LogLUT */
        be_blc_cfg->rlsc_blc.usr_reg_cfg.blc[j]     = black_level_ctx->rm_diff_black_level[j] << 2; /* rlsc */
        be_blc_cfg->lsc_blc.usr_reg_cfg.blc[j]      = black_level_ctx->rm_diff_black_level[j] << 2; /* lsc */
        be_blc_cfg->dg_blc.usr_reg_cfg.blc[j]       = black_level_ctx->rm_diff_black_level[j] << 2; /* Dg */
        be_blc_cfg->ae_blc.usr_reg_cfg.blc[j]       = black_level_ctx->rm_diff_black_level[j] << 2; /* AE */
        be_blc_cfg->mg_blc.usr_reg_cfg.blc[j]       = black_level_ctx->rm_diff_black_level[j] << 2; /* MG */
        be_blc_cfg->wb_blc.usr_reg_cfg.blc[j]       = black_level_ctx->rm_diff_black_level[j] << 2; /* WB */
        be_blc_cfg->pre_gamma_blc.usr_reg_cfg.blc[j] = 0;                                           /* pregamma */
    }

    be_blc_cfg->split_blc.usr_reg_cfg.out_blc = 0;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        black_level_ctx->actual_black_level[i] = black_level_ctx->black_level[i];
    }
}

static hi_void be_blc_dyna_regs_wdr(VI_PIPE vi_pipe, isp_be_blc_cfg  *be_blc_cfg, isp_blacklevel_ctx *black_level_ctx)
{
    hi_u8  i, j;
    hi_u8  wdr_mode_state = black_level_ctx->wdr_mode_state;
    hi_u16 wdr_out_blc = 0;

    if (wdr_mode_state == HI_FALSE) { // reg value same as linear mode
        be_blc_dyna_regs_linear(vi_pipe, be_blc_cfg, black_level_ctx);
    } else if (wdr_mode_state == HI_TRUE) {
        wdr_out_blc = ((black_level_ctx->rm_diff_black_level[0] >> 4) << 6); /* WDR outblc */
        for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
            for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
                be_blc_cfg->wdr_dg_blc[i].usr_reg_cfg.blc[j]  = black_level_ctx->rm_diff_black_level[j] << 2; /* 4DG */
                be_blc_cfg->wdr_blc[i].usr_reg_cfg.blc[j]     = black_level_ctx->rm_diff_black_level[j] << 2; /* WDR */
                be_blc_cfg->wdr_blc[i].usr_reg_cfg.out_blc    = wdr_out_blc;                                   /* WDR */
                be_blc_cfg->flicker_blc[i].usr_reg_cfg.blc[j] = black_level_ctx->rm_diff_black_level[j] << 2; /* flicker */
            }

            be_blc_cfg->split_blc.usr_reg_cfg.blc[j]    = 0;              /* split */
            be_blc_cfg->log_lut_blc.usr_reg_cfg.blc[j]  = 0;              /* LogLUT */
            be_blc_cfg->rlsc_blc.usr_reg_cfg.blc[j]     = wdr_out_blc >> 6; /* rlsc */
            be_blc_cfg->lsc_blc.usr_reg_cfg.blc[j]      = wdr_out_blc >> 6; /* lsc */
            be_blc_cfg->dg_blc.usr_reg_cfg.blc[j]       = wdr_out_blc >> 6; /* Dg */
            be_blc_cfg->ae_blc.usr_reg_cfg.blc[j]       = wdr_out_blc >> 6; /* AE */
            be_blc_cfg->mg_blc.usr_reg_cfg.blc[j]       = wdr_out_blc >> 6; /* MG */
            be_blc_cfg->wb_blc.usr_reg_cfg.blc[j]       = wdr_out_blc >> 6; /* WB */
            be_blc_cfg->pre_gamma_blc.usr_reg_cfg.blc[j] = 0;              /* pregamma */
        }

        be_blc_cfg->split_blc.usr_reg_cfg.out_blc = 0;

        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            black_level_ctx->actual_black_level[i] = black_level_ctx->black_level[i];
        }
    }
}

static hi_s32 split_check_cmos_param(VI_PIPE vi_pipe, const hi_isp_cmos_split *split)
{
    hi_u8 i;

    for (i = 0; i < ISP_SPLIT_POINT_NUM; i++) {
        if (split->split_point[i].x > 0x81) {
            ISP_ERR_TRACE("Invalid split_point[%d].x:%d!\n", i, split->split_point[i].x);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (split->split_point[i].y > 0x8000) {
            ISP_ERR_TRACE("Invalid split_point[%d].y:%d!\n", i, split->split_point[i].y);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 be_blc_dyna_regs_built_in(VI_PIPE vi_pipe, isp_be_blc_cfg *be_blc_cfg, isp_blacklevel_ctx *black_level_ctx)
{
    hi_u8  i, j;
    hi_u16 black_level[ISP_BAYER_CHN_NUM] = {0};
    hi_u16 actual_black_level[ISP_BAYER_CHN_NUM] = {0};
    hi_s32 ret;
    hi_isp_cmos_default *sns_dft     = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (sns_dft->key.bit1_split) {
        ISP_CHECK_POINTER(sns_dft->split);
        ret = split_check_cmos_param(vi_pipe, sns_dft->split);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        get_built_in_expander_blc(sns_dft->split, black_level_ctx->rm_diff_black_level, black_level);
        get_built_in_expander_blc(sns_dft->split, black_level_ctx->black_level, actual_black_level);
    } else {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            black_level[i]       = black_level_ctx->rm_diff_black_level[i] << 2; // 14bits
            actual_black_level[i] = black_level_ctx->black_level[i] << 2;      // 14bits
        }
    }

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
            be_blc_cfg->wdr_dg_blc[i].usr_reg_cfg.blc[j]  = 0; /* 4DG */
            be_blc_cfg->wdr_blc[i].usr_reg_cfg.blc[j]     = 0; /* WDR */
            be_blc_cfg->wdr_blc[i].usr_reg_cfg.out_blc    = 0; /* WDR */
            be_blc_cfg->flicker_blc[i].usr_reg_cfg.blc[j] = 0; /* flicker */
        }

        be_blc_cfg->split_blc.usr_reg_cfg.blc[j]    = 0;              /* split */
        be_blc_cfg->log_lut_blc.usr_reg_cfg.blc[j]  = 0;              /* LogLUT */
        be_blc_cfg->rlsc_blc.usr_reg_cfg.blc[j]     = black_level[j]; /* rlsc */
        be_blc_cfg->lsc_blc.usr_reg_cfg.blc[j]      = black_level[j]; /* lsc */
        be_blc_cfg->dg_blc.usr_reg_cfg.blc[j]       = black_level[j]; /* Dg */
        be_blc_cfg->ae_blc.usr_reg_cfg.blc[j]       = black_level[j]; /* AE */
        be_blc_cfg->mg_blc.usr_reg_cfg.blc[j]       = black_level[j]; /* MG */
        be_blc_cfg->wb_blc.usr_reg_cfg.blc[j]       = black_level[j]; /* WB */
        be_blc_cfg->pre_gamma_blc.usr_reg_cfg.blc[j] = 0;             /* pregamma */
    }

    be_blc_cfg->split_blc.usr_reg_cfg.out_blc = 0;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        black_level_ctx->actual_black_level[i] = actual_black_level[i]; /* Notice: Actual Blc is 14bits */
    }

    return HI_SUCCESS;
}

static hi_void be_blc_dyna_regs(VI_PIPE vi_pipe, hi_u8  wdr_mode, isp_be_blc_cfg *be_blc_cfg, isp_blacklevel_ctx *black_level_ctx)
{
    black_level_ctx->wdr_mode_state = hi_ext_system_wdr_en_read(vi_pipe);

    if (IS_LINEAR_MODE(wdr_mode)) {
        be_blc_dyna_regs_linear(vi_pipe, be_blc_cfg, black_level_ctx);
    } else if (IS_2to1_WDR_MODE(wdr_mode) || IS_3to1_WDR_MODE(wdr_mode) || IS_4to1_WDR_MODE(wdr_mode)) {
        be_blc_dyna_regs_wdr(vi_pipe, be_blc_cfg, black_level_ctx);
    } else if (IS_BUILT_IN_WDR_MODE(wdr_mode)) {
        be_blc_dyna_regs_built_in(vi_pipe, be_blc_cfg, black_level_ctx);
    }

    be_blc_cfg->resh_dyna = HI_TRUE;
}

static hi_void be_blc_static_regs(hi_u8  wdr_mode, isp_be_blc_cfg  *be_blc_cfg)
{
    hi_u8 i;

    /* 4DG */
    for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
        be_blc_cfg->wdr_dg_blc[i].static_reg_cfg.blc_in  = HI_TRUE;
        be_blc_cfg->wdr_dg_blc[i].static_reg_cfg.blc_out = HI_TRUE;
    }

    /* WDR */
    be_blc_cfg->wdr_blc[0].static_reg_cfg.blc_out = HI_TRUE;
    /* rlsc */
    be_blc_cfg->rlsc_blc.static_reg_cfg.blc_in  = HI_TRUE;
    be_blc_cfg->rlsc_blc.static_reg_cfg.blc_out = HI_TRUE;
    /* lsc */
    be_blc_cfg->lsc_blc.static_reg_cfg.blc_in   = HI_TRUE;
    be_blc_cfg->lsc_blc.static_reg_cfg.blc_out  = HI_TRUE;
    /* Dg */
    be_blc_cfg->dg_blc.static_reg_cfg.blc_in    = HI_TRUE;
    be_blc_cfg->dg_blc.static_reg_cfg.blc_out   = HI_FALSE;
    /* AE */
    be_blc_cfg->ae_blc.static_reg_cfg.blc_in    = HI_FALSE;
    /* MG */
    be_blc_cfg->mg_blc.static_reg_cfg.blc_in    = HI_FALSE;
    /* WB */
    be_blc_cfg->wb_blc.static_reg_cfg.blc_in    = HI_FALSE;
    be_blc_cfg->wb_blc.static_reg_cfg.blc_out   = HI_FALSE;

    be_blc_cfg->resh_static = HI_TRUE;
}

static hi_void fe_blc_dyna_regs(isp_fe_blc_cfg *fe_blc_cfg, isp_blacklevel_ctx *black_level_ctx)
{
    hi_u8 j;

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        fe_blc_cfg->fe_blc.usr_reg_cfg.blc[j]        = (black_level_ctx->black_level[j] - black_level_ctx->rm_diff_black_level[j]) << 2;
        fe_blc_cfg->fe_dg_blc.usr_reg_cfg.blc[j]     = black_level_ctx->rm_diff_black_level[j] << 2; /* Fe Dg */
        fe_blc_cfg->fe_wb_blc.usr_reg_cfg.blc[j]     = black_level_ctx->rm_diff_black_level[j] << 2; /* Fe WB */
        fe_blc_cfg->fe_ae_blc.usr_reg_cfg.blc[j]     = black_level_ctx->rm_diff_black_level[j] << 2; /* Fe AE */
        fe_blc_cfg->fe_lsc_blc.usr_reg_cfg.blc[j]    = black_level_ctx->rm_diff_black_level[j] << 2; /* Fe LSC */
        fe_blc_cfg->rc_blc.usr_reg_cfg.blc[j]        = black_level_ctx->rm_diff_black_level[j] << 2; /* RC */
        fe_blc_cfg->fe_log_lut_blc.usr_reg_cfg.blc[j] = black_level_ctx->rm_diff_black_level[j] << 4; /* Fe LogLUT */
    }

    fe_blc_cfg->resh_dyna          = HI_TRUE;
    fe_blc_cfg->fe_blc_cfg_changed = HI_FALSE;
    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        if (fe_blc_cfg->fe_blc_cfg_save[j] != fe_blc_cfg->fe_blc.usr_reg_cfg.blc[j]) {
            fe_blc_cfg->fe_blc_cfg_changed = HI_TRUE;
        }
        fe_blc_cfg->fe_blc_cfg_save[j] = fe_blc_cfg->fe_blc.usr_reg_cfg.blc[j];
    }
}

static hi_void fe_blc_static_regs(isp_fe_blc_cfg  *fe_blc_cfg)
{
    /* Fe Dg */
    fe_blc_cfg->fe_dg_blc.static_reg_cfg.blc_in  = HI_TRUE;
    fe_blc_cfg->fe_dg_blc.static_reg_cfg.blc_out = HI_TRUE;
    /* Fe WB */
    fe_blc_cfg->fe_wb_blc.static_reg_cfg.blc_in  = HI_TRUE;
    fe_blc_cfg->fe_wb_blc.static_reg_cfg.blc_out = HI_TRUE;
    /* Fe AE */
    fe_blc_cfg->fe_ae_blc.static_reg_cfg.blc_in  = HI_FALSE;
    /* Fe LSC */
    fe_blc_cfg->fe_lsc_blc.static_reg_cfg.blc_in  = HI_TRUE;
    fe_blc_cfg->fe_lsc_blc.static_reg_cfg.blc_out = HI_TRUE;
    /* RC */
    fe_blc_cfg->rc_blc.static_reg_cfg.blc_in     = HI_FALSE;
    fe_blc_cfg->rc_blc.static_reg_cfg.blc_out    = HI_FALSE;
    /* FE BLC */
    fe_blc_cfg->fe_blc.static_reg_cfg.blc_in     = HI_TRUE;

    fe_blc_cfg->resh_static= HI_TRUE;
}

static hi_void blc_regs_initialize(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8  i, wdr_mode;
    isp_blacklevel_ctx *black_level_ctx = HI_NULL;
    isp_usr_ctx        *isp_ctx     = HI_NULL;

    ISP_GET_CTX(vi_pipe, isp_ctx);
    BLACKLEVEL_GET_CTX(vi_pipe, black_level_ctx);

    wdr_mode = isp_ctx->sns_wdr_mode;

    balance_black_level(black_level_ctx);

    /* BE */
    for (i = 0; i < reg_cfg->cfg_num; i++) {
        be_blc_dyna_regs(vi_pipe, wdr_mode, &reg_cfg->alg_reg_cfg[i].be_blc_cfg, black_level_ctx);
        be_blc_static_regs(wdr_mode, &reg_cfg->alg_reg_cfg[i].be_blc_cfg);
        reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset0 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[0].usr_reg_cfg.blc[1];
        reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset1 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[1].usr_reg_cfg.blc[1];
        reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset2 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[2].usr_reg_cfg.blc[1];
        memcpy_s(&reg_cfg->alg_reg_cfg[i].be_last_blc_cfg, sizeof(isp_be_blc_cfg),
                 &reg_cfg->alg_reg_cfg[i].be_blc_cfg, sizeof(isp_be_blc_cfg));
    }
    reg_cfg->cfg_key.bit1_be_blc_cfg = 1;

    /* FE */
    fe_blc_dyna_regs(&reg_cfg->alg_reg_cfg[0].fe_blc_cfg, black_level_ctx);
    fe_blc_static_regs(&reg_cfg->alg_reg_cfg[0].fe_blc_cfg);
    reg_cfg->cfg_key.bit1_fe_blc_cfg = 1;
}

static hi_s32 blc_read_extregs(VI_PIPE vi_pipe)
{
    isp_blacklevel_ctx *black_level_ctx = HI_NULL;
    BLACKLEVEL_GET_CTX(vi_pipe, black_level_ctx);

    black_level_ctx->black_level_change = hi_ext_system_black_level_change_read(vi_pipe);
    hi_ext_system_black_level_change_write(vi_pipe, HI_FALSE);

    if (black_level_ctx->black_level_change) {
        black_level_ctx->op_type         = hi_ext_system_black_level_manual_mode_read(vi_pipe);
        black_level_ctx->black_level[0]  = hi_ext_system_black_level_00_read(vi_pipe);
        black_level_ctx->black_level[1]  = hi_ext_system_black_level_01_read(vi_pipe);
        black_level_ctx->black_level[2]  = hi_ext_system_black_level_10_read(vi_pipe);
        black_level_ctx->black_level[3]  = hi_ext_system_black_level_11_read(vi_pipe);
    }

    return HI_SUCCESS;
}

static hi_void blc_write_actual_value(VI_PIPE vi_pipe, hi_u16 *black_level)
{
    hi_ext_system_black_level_query_00_write(vi_pipe, black_level[0]);
    hi_ext_system_black_level_query_01_write(vi_pipe, black_level[1]);
    hi_ext_system_black_level_query_10_write(vi_pipe, black_level[2]);
    hi_ext_system_black_level_query_11_write(vi_pipe, black_level[3]);
}

static hi_s32 isp_blc_proc_write(VI_PIPE vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write proc_tmp;
    isp_blacklevel_ctx    *black_level_ctx = HI_NULL;

    BLACKLEVEL_GET_CTX(vi_pipe, black_level_ctx);

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len  = proc->buff_len;

    ISP_PROC_PRINTF(&proc_tmp, proc->write_len,
                    "-----Black Level Actual INFO--------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&proc_tmp, proc->write_len,
                    "%16s"     "%16s"     "%16s"     "%16s\n",
                    "BlcR", "BlcGr", "BlcGb", "BlcB");

    ISP_PROC_PRINTF(&proc_tmp, proc->write_len,
                    "%16u"     "%16u"     "%16u"     "%16u\n",
                    black_level_ctx->actual_black_level[0],
                    black_level_ctx->actual_black_level[1],
                    black_level_ctx->actual_black_level[2],
                    black_level_ctx->actual_black_level[3]);

    proc->write_len += 1;

    return HI_SUCCESS;
}

static hi_bool check_wdr_state(VI_PIPE vi_pipe, isp_usr_ctx *isp_ctx, isp_blacklevel_ctx *black_level_ctx)
{
    hi_u8   wdr_en;
    hi_bool wdr_state_change;

    wdr_en = hi_ext_system_wdr_en_read(vi_pipe);

    if (IS_2to1_WDR_MODE(isp_ctx->sns_wdr_mode) || IS_3to1_WDR_MODE(isp_ctx->sns_wdr_mode) || IS_4to1_WDR_MODE(isp_ctx->sns_wdr_mode)) {
        wdr_state_change = (black_level_ctx->wdr_mode_state == wdr_en) ? HI_FALSE : HI_TRUE;
    } else {
        wdr_state_change = HI_FALSE;
    }

    black_level_ctx->wdr_mode_state = wdr_en;

    return wdr_state_change;
}

static hi_s32 isp_blc_wdr_mode_set(VI_PIPE vi_pipe, hi_void *reg_cfg_info)
{
    hi_u8 i;
    isp_usr_ctx             *isp_ctx         = HI_NULL;
    isp_blacklevel_ctx      *black_level_ctx = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    isp_reg_cfg           *reg_cfg         = (isp_reg_cfg *)reg_cfg_info;

    ISP_GET_CTX(vi_pipe, isp_ctx);
    BLACKLEVEL_GET_CTX(vi_pipe, black_level_ctx);
    isp_sensor_get_blc(vi_pipe, &sns_black_level);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        black_level_ctx->black_level[i] = sns_black_level->black_level[i];
    }

    balance_black_level(black_level_ctx);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        be_blc_dyna_regs(vi_pipe, isp_ctx->sns_wdr_mode, &reg_cfg->alg_reg_cfg[i].be_blc_cfg, black_level_ctx);
        reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset0 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[0].usr_reg_cfg.blc[1];
        reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset1 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[1].usr_reg_cfg.blc[1];
        reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset2 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[2].usr_reg_cfg.blc[1];

        memcpy_s(&reg_cfg->alg_reg_cfg[i].be_last_blc_cfg, sizeof(isp_be_blc_cfg),
                 &reg_cfg->alg_reg_cfg[i].be_blc_cfg, sizeof(isp_be_blc_cfg));
    }

    fe_blc_dyna_regs(&reg_cfg->alg_reg_cfg[0].fe_blc_cfg, black_level_ctx);

    blc_ext_regs_initialize(vi_pipe);

    reg_cfg->cfg_key.bit1_be_blc_cfg = 1;
    reg_cfg->cfg_key.bit1_fe_blc_cfg = 1;
    reg_cfg->alg_reg_cfg[0].fe_blc_cfg.resh_dyna   = HI_TRUE;
    reg_cfg->alg_reg_cfg[0].fe_blc_cfg.resh_static = HI_TRUE;

    return HI_SUCCESS;
}

hi_s32 isp_blc_init(VI_PIPE vi_pipe, hi_void *reg_cfg_info)
{
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_info;

    blc_initialize(vi_pipe);
    blc_regs_initialize(vi_pipe, reg_cfg);
    blc_ext_regs_initialize(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_blc_run(VI_PIPE vi_pipe, const hi_void *stat_info, hi_void *reg_cfg_info, hi_s32 rsv)
{
    hi_u8   i;
    hi_bool wdr_state_change;
    VI_PIPE main_pipe     = 0;
    hi_s16  blk_offset = 0;
    hi_s32  gain_k     = 0x100;
    hi_s32  blc_tmp    = 0;
    isp_usr_ctx              *isp_ctx         = HI_NULL;
    hi_isp_cmos_black_level  *sns_black_level = HI_NULL;
    isp_blacklevel_ctx       *black_level_ctx = HI_NULL;
    isp_reg_cfg              *reg_cfg         = (isp_reg_cfg *)reg_cfg_info;

    ISP_GET_CTX(vi_pipe, isp_ctx);
    BLACKLEVEL_GET_CTX(vi_pipe, black_level_ctx);

    wdr_state_change = check_wdr_state(vi_pipe, isp_ctx, black_level_ctx);

    if (hi_ext_system_dpc_static_defect_type_read(vi_pipe) == 0) { // hot pixel
        if (isp_ctx->linkage.defect_pixel) {
            if (black_level_ctx->pre_defect_pixel == HI_FALSE) {
                for (i = 0; i < reg_cfg->cfg_num; i++) {
                    reg_cfg->alg_reg_cfg[i].be_blc_cfg.wb_blc.static_reg_cfg.blc_in = HI_TRUE;
                    reg_cfg->alg_reg_cfg[i].be_blc_cfg.resh_static                  = HI_TRUE;
                }
            }
        } else if (black_level_ctx->pre_defect_pixel) {
            for (i = 0; i < reg_cfg->cfg_num; i++) {
                reg_cfg->alg_reg_cfg[i].be_blc_cfg.wb_blc.static_reg_cfg.blc_in = HI_FALSE;
                reg_cfg->alg_reg_cfg[i].be_blc_cfg.resh_static                  = HI_TRUE;
            }
        }

        black_level_ctx->pre_defect_pixel = isp_ctx->linkage.defect_pixel;
    }

    blc_read_extregs(vi_pipe);

    reg_cfg->cfg_key.bit1_fe_blc_cfg = 1;
    reg_cfg->cfg_key.bit1_be_blc_cfg = 1;
    reg_cfg->alg_reg_cfg[0].fe_blc_cfg.fe_blc_cfg_changed = HI_FALSE;

    /*  mannual mode update */
    if (black_level_ctx->op_type == OP_TYPE_MANUAL) {
        balance_black_level(black_level_ctx);
        fe_blc_dyna_regs(&reg_cfg->alg_reg_cfg[0].fe_blc_cfg, black_level_ctx);

        for (i = 0; i < reg_cfg->cfg_num; i++) {
            be_blc_dyna_regs(vi_pipe, isp_ctx->sns_wdr_mode, &reg_cfg->alg_reg_cfg[i].be_blc_cfg, black_level_ctx);
            reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset0 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[0].usr_reg_cfg.blc[1];
            reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset1 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[1].usr_reg_cfg.blc[1];
            reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset2 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[2].usr_reg_cfg.blc[1];
        }

        blc_write_actual_value(vi_pipe, black_level_ctx->actual_black_level);

        return HI_SUCCESS;
    }

    /* some sensors's blacklevel is changed with iso. */
    isp_sensor_get_blc(vi_pipe, &sns_black_level);

    /* sensors's blacklevel is changed by cmos. */
    if (sns_black_level->update == HI_TRUE) {
        if (isp_ctx->linkage.snap_pipe_mode != ISP_SNAP_NONE) {
            if (vi_pipe == isp_ctx->linkage.picture_pipe_id) {
                main_pipe = isp_ctx->linkage.preview_pipe_id;
                ISP_CHECK_PIPE(main_pipe);
                isp_sensor_get_blc(main_pipe, &sns_black_level);
            } else {
                isp_sensor_get_blc(vi_pipe, &sns_black_level);
            }

            memcpy(&black_level_ctx->black_level[0], &sns_black_level->black_level[0], ISP_BAYER_CHN_NUM * sizeof(hi_u16));
        }

        /* ISP Multi-pipe blackLevel different configs */
        if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
            if (isp_ctx->stitch_attr.main_pipe != HI_TRUE) {
                main_pipe = isp_ctx->stitch_attr.stitch_bind_id[0];
                ISP_CHECK_PIPE(main_pipe);
                isp_sensor_get_blc(main_pipe, &sns_black_level);
            } else {
                isp_sensor_update_blc(vi_pipe);
            }

            for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
                blk_offset = hi_ext_system_isp_pipe_diff_offset_read(vi_pipe, i);
                gain_k  = hi_ext_system_isp_pipe_diff_gain_read(vi_pipe, i);
                blc_tmp = ((hi_s32)black_level_ctx->black_level[i] * 0x100 / DIV_0_TO_1(gain_k)) - blk_offset;
                black_level_ctx->black_level[i] = CLIP3(blc_tmp, 0, 0xFFF);
            }
        } else {
            isp_sensor_update_blc(vi_pipe);
            memcpy(&black_level_ctx->black_level[0], &sns_black_level->black_level[0], ISP_BAYER_CHN_NUM * sizeof(hi_u16));
        }

        balance_black_level(black_level_ctx);

        fe_blc_dyna_regs(&reg_cfg->alg_reg_cfg[0].fe_blc_cfg, black_level_ctx);

        for (i = 0; i < reg_cfg->cfg_num; i++) {
            be_blc_dyna_regs(vi_pipe, isp_ctx->sns_wdr_mode, &reg_cfg->alg_reg_cfg[i].be_blc_cfg, black_level_ctx);
            reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset0 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[0].usr_reg_cfg.blc[1];
            reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset1 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[1].usr_reg_cfg.blc[1];
            reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset2 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[2].usr_reg_cfg.blc[1];
        }

        blc_write_actual_value(vi_pipe, black_level_ctx->actual_black_level);

        return HI_SUCCESS;
    }

    /* sensors's blacklevel is changed by mpi. */
    if (((black_level_ctx->black_level_change == HI_TRUE) && \
         (black_level_ctx->op_type == OP_TYPE_AUTO)) || \
        (wdr_state_change == HI_TRUE)) {
        isp_sensor_update_blc(vi_pipe);
        memcpy(&black_level_ctx->black_level[0], &sns_black_level->black_level[0], ISP_BAYER_CHN_NUM * sizeof(hi_u16));

        /* ISP Multi-pipe blackLevel different configs */
        if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
            for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
                blk_offset = (hi_s16)hi_ext_system_isp_pipe_diff_offset_read(vi_pipe, i);
                gain_k  = hi_ext_system_isp_pipe_diff_gain_read(vi_pipe, i);
                blc_tmp = ((hi_s32)black_level_ctx->black_level[i] * 0x100 / DIV_0_TO_1(gain_k)) - blk_offset;
                black_level_ctx->black_level[i] = CLIP3(blc_tmp, 0, 0xFFF);
            }
        }

        balance_black_level(black_level_ctx);

        fe_blc_dyna_regs(&reg_cfg->alg_reg_cfg[0].fe_blc_cfg, black_level_ctx);

        for (i = 0; i < reg_cfg->cfg_num; i++) {
            be_blc_dyna_regs(vi_pipe, isp_ctx->sns_wdr_mode, &reg_cfg->alg_reg_cfg[i].be_blc_cfg, black_level_ctx);
            reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset0 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[0].usr_reg_cfg.blc[1];
            reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset1 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[1].usr_reg_cfg.blc[1];
            reg_cfg->alg_reg_cfg[i].stWdrRegCfg.stSyncRegCfg.u16Offset2 = reg_cfg->alg_reg_cfg[i].be_blc_cfg.wdr_blc[2].usr_reg_cfg.blc[1];
        }

        blc_write_actual_value(vi_pipe, black_level_ctx->actual_black_level);
    }

    return HI_SUCCESS;
}

hi_s32 isp_blc_ctrl(VI_PIPE vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr  *reg_cfg_attr   = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(vi_pipe, reg_cfg_attr);
            ISP_CHECK_POINTER(reg_cfg_attr);
            isp_blc_wdr_mode_set(vi_pipe, (hi_void *)&reg_cfg_attr->reg_cfg);
            break;
        case ISP_PROC_WRITE :
            isp_blc_proc_write(vi_pipe, (hi_isp_ctrl_proc_write  *)value);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

hi_s32 isp_blc_exit(VI_PIPE vi_pipe)
{
    return HI_SUCCESS;
}

hi_s32 isp_alg_register_blc(VI_PIPE vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    ISP_GET_CTX(vi_pipe, isp_ctx);

    algs = ISP_SearchAlg(isp_ctx->algs);
    ISP_CHECK_POINTER(algs);

    algs->alg_type = ISP_ALG_BLC;
    algs->alg_func.pfn_alg_init = isp_blc_init;
    algs->alg_func.pfn_alg_run  = isp_blc_run;
    algs->alg_func.pfn_alg_ctrl = isp_blc_ctrl;
    algs->alg_func.pfn_alg_exit = isp_blc_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
