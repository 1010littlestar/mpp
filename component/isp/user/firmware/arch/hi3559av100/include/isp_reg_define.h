// ******************************************************************************
// Copyright     :  Copyright (C) 2016, Hisilicon Technologies Co., Ltd.
// File name     :  isp_reg_define.h
// Version       :  1.0
// Date          :  2016-08-27
// Description   :  Define all registers/tables
// History       :  2016-08-27 Create file
// ******************************************************************************

#ifndef __C_UNION_DEFINE_H__
#define __C_UNION_DEFINE_H__

#include "isp_lut_define.h"

/* Define the union U_ISP_FE_VERSION */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_patch          : 8   ; /* [7..0]  */
        unsigned int    isp_fe_build          : 8   ; /* [15..8]  */
        unsigned int    isp_fe_release        : 8   ; /* [23..16]  */
        unsigned int    isp_fe_version        : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_VERSION;

/* Define the union U_ISP_FE_DATE */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_date            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_DATE;
/* Define the union U_ISP_FE_FPGA_DATE */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_fpga_date       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_FPGA_DATE;
/* Define the union U_ISP_FE_MODULE_POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_MODULE_POS;

/* Define the union U_ISP_FE_USER_DEFINE0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_user_define0    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_USER_DEFINE0;
/* Define the union U_ISP_FE_USER_DEFINE1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_user_define1    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_USER_DEFINE1;
/* Define the union U_ISP_FE_STARTUP */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_fcnt            : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_STARTUP;
/* Define the union U_ISP_FE_FORMAT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_format          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_FORMAT;
/* Define the union U_ISP_FE_INT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_int_fstart     : 1   ; /* [0]  */
        unsigned int    isp_fe_int_update_cfg : 1   ; /* [1]  */
        unsigned int    isp_fe_int_cfg_loss   : 1   ; /* [2]  */
        unsigned int    isp_fe_int_int_delay  : 1   ; /* [3]  */
        unsigned int    isp_fe_int_ae1_int    : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    isp_fe_int_af1_int    : 1   ; /* [6]  */
        unsigned int    isp_fe_int_dis1_int   : 1   ; /* [7]  */
        unsigned int    int_res               : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_INT;

/* Define the union U_ISP_FE_INT_MASK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_int_mask_fstart : 1   ; /* [0]  */
        unsigned int    isp_fe_int_mask_update_cfg : 1   ; /* [1]  */
        unsigned int    isp_fe_int_mask_cfg_loss : 1   ; /* [2]  */
        unsigned int    isp_fe_int_mask_int_delay : 1   ; /* [3]  */
        unsigned int    isp_fe_int_mask_ae1_int : 1   ; /* [4]  */
        unsigned int    reserved_0            : 1   ; /* [5]  */
        unsigned int    isp_fe_int_mask_af1_int : 1   ; /* [6]  */
        unsigned int    isp_fe_int_mask_dis1_int : 1   ; /* [7]  */
        unsigned int    int_mask_res          : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_INT_MASK;

/* Define the union U_ISP_FE_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    en0_res               : 2   ; /* [1..0]  */
        unsigned int    isp_colorbar_en       : 1   ; /* [2]  */
        unsigned int    isp_crop_en           : 1   ; /* [3]  */
        unsigned int    isp_blc1_en           : 1   ; /* [4]  */
        unsigned int    isp_rc_en             : 1   ; /* [5]  */
        unsigned int    isp_hrs_en            : 1   ; /* [6]  */
        unsigned int    isp_lsc1_en           : 1   ; /* [7]  */
        unsigned int    isp_dg2_en            : 1   ; /* [8]  */
        unsigned int    isp_dis1_en           : 1   ; /* [9]  */
        unsigned int    isp_af1_en            : 1   ; /* [10]  */
        unsigned int    isp_wb1_en            : 1   ; /* [11]  */
        unsigned int    isp_drcs_en           : 1   ; /* [12]  */
        unsigned int    isp_ae1_en            : 1   ; /* [13]  */
        unsigned int    isp_loglut1_en        : 1   ; /* [14]  */
        unsigned int    en1_res               : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_CTRL;

/* Define the union U_ISP_FE_ADAPTER_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    fe_hsync_mode         : 1   ; /* [0]  */
        unsigned int    fe_vsync_mode         : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_ADAPTER_CFG;

/* Define the union U_ISP_FE_OUT_SIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    fe_out_width          : 16  ; /* [15..0]  */
        unsigned int    fe_out_height         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_OUT_SIZE;

/* Define the union U_ISP_FE_FSTART_DELAY */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_delay           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_FSTART_DELAY;
/* Define the union U_ISP_FE_CTRL_F */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_rggb_cfg       : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_CTRL_F;

/* Define the union U_ISP_FE_CTRL_I */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_update_mode    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_CTRL_I;

/* Define the union U_ISP_FE_TIMING_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_fe_fix_timing     : 13  ; /* [13..1]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_TIMING_CFG;

/* Define the union U_ISP_FE_REG_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_update         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_REG_UPDATE;

/* Define the union U_ISP_FE_SIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_width          : 16  ; /* [15..0]  */
        unsigned int    isp_fe_height         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_SIZE;

/* Define the union U_ISP_FE_IN_SUM_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_csc_sum_en     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_IN_SUM_CFG;

/* Define the union U_ISP_FE_OUT_SUM_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_yuv444_sum_en  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_OUT_SUM_CFG;

/* Define the union U_ISP_FE_BLK_SIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_blk_width      : 16  ; /* [15..0]  */
        unsigned int    isp_fe_blk_height     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_BLK_SIZE;

/* Define the union U_ISP_FE_BLK_HBLANK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_blk_b_hblank   : 16  ; /* [15..0]  */
        unsigned int    isp_fe_blk_f_hblank   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_BLK_HBLANK;

/* Define the union U_ISP_FE_BLK_VBLANK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fe_blk_b_vblank   : 16  ; /* [15..0]  */
        unsigned int    isp_fe_blk_f_vblank   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_BLK_VBLANK;

/* Define the union U_ISP_FE_IN_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_y_sum0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_IN_SUM0;
/* Define the union U_ISP_FE_IN_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_y_sum1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_IN_SUM1;
/* Define the union U_ISP_FE_OUT_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_yuv444_y_sum0   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_OUT_SUM0;
/* Define the union U_ISP_FE_OUT_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fe_yuv444_y_sum1   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FE_OUT_SUM1;
/* Define the union U_ISP_BLC1_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC1_VERSION;
/* Define the union U_ISP_BLC1_OFFSET1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc1_ofsgr        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_blc1_ofsr         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC1_OFFSET1;

/* Define the union U_ISP_BLC1_OFFSET2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc1_ofsgb        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_blc1_ofsb         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC1_OFFSET2;

/* Define the union U_ISP_DG2_BLC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_en_in         : 1   ; /* [0]  */
        unsigned int    isp_dg2_en_out        : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG2_BLC_CFG;

/* Define the union U_ISP_DG2_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG2_VERSION;
/* Define the union U_ISP_DG2_GAIN1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_grgain        : 16  ; /* [15..0]  */
        unsigned int    isp_dg2_rgain         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG2_GAIN1;

/* Define the union U_ISP_DG2_GAIN2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_gbgain        : 16  ; /* [15..0]  */
        unsigned int    isp_dg2_bgain         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG2_GAIN2;

/* Define the union U_ISP_DG2_BLC_OFFSET1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_ofsgr         : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_dg2_ofsr          : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG2_BLC_OFFSET1;

/* Define the union U_ISP_DG2_BLC_OFFSET2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_ofsgb         : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_dg2_ofsb          : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG2_BLC_OFFSET2;

/* Define the union U_ISP_DG2_CLIP_VALUE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg2_clip_value    : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG2_CLIP_VALUE;

/* Define the union U_ISP_HRS_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_hrs_ds_en         : 1   ; /* [1]  */
        unsigned int    reserved_1            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HRS_CFG;

/* Define the union U_ISP_HRS_FILTERLUT0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_hrs_filterlut0_0  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_hrs_filterlut0_1  : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HRS_FILTERLUT0;

/* Define the union U_ISP_HRS_FILTERLUT1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_hrs_filterlut1_0  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_hrs_filterlut1_1  : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HRS_FILTERLUT1;

/* Define the union U_ISP_HRS_FILTERLUT2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_hrs_filterlut2_0  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_hrs_filterlut2_1  : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HRS_FILTERLUT2;

/* Define the union U_ISP_HRS_FILTERLUT3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_hrs_filterlut3_0  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_hrs_filterlut3_1  : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HRS_FILTERLUT3;

/* Define the union U_ISP_HRS_FILTERLUT4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_hrs_filterlut4_0  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_hrs_filterlut4_1  : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HRS_FILTERLUT4;

/* Define the union U_ISP_HRS_FILTERLUT5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_hrs_filterlut5_0  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_hrs_filterlut5_1  : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HRS_FILTERLUT5;

/* Define the union U_ISP_WB1_BLC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_en_in         : 1   ; /* [0]  */
        unsigned int    isp_wb1_en_out        : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB1_BLC_CFG;

/* Define the union U_ISP_WB1_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB1_VERSION;
/* Define the union U_ISP_WB1_GAIN1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_grgain        : 16  ; /* [15..0]  */
        unsigned int    isp_wb1_rgain         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB1_GAIN1;

/* Define the union U_ISP_WB1_GAIN2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_gbgain        : 16  ; /* [15..0]  */
        unsigned int    isp_wb1_bgain         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB1_GAIN2;

/* Define the union U_ISP_WB1_BLC_OFFSET1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_ofsgr         : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wb1_ofsr          : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB1_BLC_OFFSET1;

/* Define the union U_ISP_WB1_BLC_OFFSET2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_ofsgb         : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wb1_ofsb          : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB1_BLC_OFFSET2;

/* Define the union U_ISP_WB1_CLIP_VALUE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb1_clip_value    : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB1_CLIP_VALUE;

/* Define the union U_ISP_AE1_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_version        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_VERSION;
/* Define the union U_ISP_AE1_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_hnum          : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_ae1_vnum          : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_ZONE;

/* Define the union U_ISP_AE1_SKIP_CRG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_skip_x        : 3   ; /* [2..0]  */
        unsigned int    isp_ae1_offset_x      : 1   ; /* [3]  */
        unsigned int    isp_ae1_skip_y        : 3   ; /* [6..4]  */
        unsigned int    isp_ae1_offset_y      : 1   ; /* [7]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_SKIP_CRG;

/* Define the union U_ISP_AE1_TOTAL_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_total_pixels  : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_TOTAL_STAT;

/* Define the union U_ISP_AE1_COUNT_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_count_pixels  : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_COUNT_STAT;

/* Define the union U_ISP_AE1_TOTAL_R_AVER */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_total_r_aver   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_TOTAL_R_AVER;
/* Define the union U_ISP_AE1_TOTAL_GR_AVER */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_total_gr_aver  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_TOTAL_GR_AVER;
/* Define the union U_ISP_AE1_TOTAL_GB_AVER */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_total_gb_aver  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_TOTAL_GB_AVER;
/* Define the union U_ISP_AE1_TOTAL_B_AVER */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_total_b_aver   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_TOTAL_B_AVER;
/* Define the union U_ISP_AE1_HIST_HIGH */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_hist_high      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_HIST_HIGH;
/* Define the union U_ISP_AE1_BITMOVE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_bitmove       : 5   ; /* [4..0]  */
        unsigned int    isp_ae1_hist_gamma_mode : 2   ; /* [6..5]  */
        unsigned int    isp_ae1_aver_gamma_mode : 2   ; /* [8..7]  */
        unsigned int    isp_ae1_blc_en        : 1   ; /* [9]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_BITMOVE;

/* Define the union U_ISP_AE1_OFFSET_R */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_offset_r      : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_OFFSET_R;

/* Define the union U_ISP_AE1_OFFSET_GR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_offset_gr     : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_OFFSET_GR;

/* Define the union U_ISP_AE1_OFFSET_GB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_offset_gb     : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_OFFSET_GB;

/* Define the union U_ISP_AE1_OFFSET_B */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_offset_b      : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_OFFSET_B;

/* Define the union U_ISP_AE1_GAMMA_LIMIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_gamma_limit   : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_GAMMA_LIMIT;

/* Define the union U_ISP_AE1_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_lut_update    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_LUT_UPDATE;

/* Define the union U_ISP_AE1_FOURPLANEMODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_fourplanemode : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_FOURPLANEMODE;

/* Define the union U_ISP_AE1_CROP_POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_crop_pos_x    : 16  ; /* [15..0]  */
        unsigned int    isp_ae1_crop_pos_y    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_CROP_POS;

/* Define the union U_ISP_AE1_CROP_OUTSIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae1_crop_out_width : 16  ; /* [15..0]  */
        unsigned int    isp_ae1_crop_out_height : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_CROP_OUTSIZE;

/* Define the union U_ISP_AE1_HIST_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_HIST_WADDR;
/* Define the union U_ISP_AE1_HIST_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_HIST_WDATA;
/* Define the union U_ISP_AE1_HIST_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_hist_raddr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_HIST_RADDR;
/* Define the union U_ISP_AE1_HIST_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_hist_rdata     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_HIST_RDATA;
/* Define the union U_ISP_AE1_AVER_R_GR_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_AVER_R_GR_WADDR;
/* Define the union U_ISP_AE1_AVER_R_GR_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_AVER_R_GR_WDATA;
/* Define the union U_ISP_AE1_AVER_R_GR_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_aver_r_gr_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_AVER_R_GR_RADDR;
/* Define the union U_ISP_AE1_AVER_R_GR_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_aver_r_gr_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_AVER_R_GR_RDATA;
/* Define the union U_ISP_AE1_AVER_GB_B_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_AVER_GB_B_WADDR;
/* Define the union U_ISP_AE1_AVER_GB_B_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_AVER_GB_B_WDATA;
/* Define the union U_ISP_AE1_AVER_GB_B_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_aver_gb_b_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_AVER_GB_B_RADDR;
/* Define the union U_ISP_AE1_AVER_GB_B_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_aver_gb_b_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_AVER_GB_B_RDATA;
/* Define the union U_ISP_AE1_WEIGHT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_wei_waddr      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_WEIGHT_WADDR;
/* Define the union U_ISP_AE1_WEIGHT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae1_wei_wdata      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_WEIGHT_WDATA;
/* Define the union U_ISP_AE1_WEIGHT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_WEIGHT_RADDR;
/* Define the union U_ISP_AE1_WEIGHT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE1_WEIGHT_RDATA;
/* Define the union U_ISP_AF1_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_af1_iir0_en0      : 1   ; /* [1]  */
        unsigned int    isp_af1_iir0_en1      : 1   ; /* [2]  */
        unsigned int    isp_af1_iir0_en2      : 1   ; /* [3]  */
        unsigned int    isp_af1_iir1_en0      : 1   ; /* [4]  */
        unsigned int    isp_af1_iir1_en1      : 1   ; /* [5]  */
        unsigned int    isp_af1_iir1_en2      : 1   ; /* [6]  */
        unsigned int    isp_af1_peak_mode     : 1   ; /* [7]  */
        unsigned int    isp_af1_squ_mode      : 1   ; /* [8]  */
        unsigned int    isp_af1_offset_en     : 1   ; /* [9]  */
        unsigned int    isp_af1_crop_en       : 1   ; /* [10]  */
        unsigned int    isp_af1_lpf_en        : 1   ; /* [11]  */
        unsigned int    isp_af1_mean_en       : 1   ; /* [12]  */
        unsigned int    reserved_1            : 1   ; /* [13]  */
        unsigned int    isp_af1_raw_mode      : 1   ; /* [14]  */
        unsigned int    isp_af1_bayer_mode    : 2   ; /* [16..15]  */
        unsigned int    isp_af1_iir0_ds_en    : 1   ; /* [17]  */
        unsigned int    isp_af1_iir1_ds_en    : 1   ; /* [18]  */
        unsigned int    isp_af1_fir0_lpf_en   : 1   ; /* [19]  */
        unsigned int    isp_af1_fir1_lpf_en   : 1   ; /* [20]  */
        unsigned int    isp_af1_iir0_ldg_en   : 1   ; /* [21]  */
        unsigned int    isp_af1_iir1_ldg_en   : 1   ; /* [22]  */
        unsigned int    isp_af1_fir0_ldg_en   : 1   ; /* [23]  */
        unsigned int    isp_af1_fir1_ldg_en   : 1   ; /* [24]  */
        unsigned int    isp_af1_gamma         : 3   ; /* [27..25]  */
        unsigned int    isp_af1_gain_lmt      : 3   ; /* [30..28]  */
        unsigned int    reserved_2            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_CFG;

/* Define the union U_ISP_AF1_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_version        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_VERSION;
/* Define the union U_ISP_AF1_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_hnum          : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_af1_vnum          : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_ZONE;

/* Define the union U_ISP_AF1_CROP_START */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_pos_x         : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_af1_pos_y         : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_CROP_START;

/* Define the union U_ISP_AF1_CROP_SIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_crop_hsize    : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_af1_crop_vsize    : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_CROP_SIZE;

/* Define the union U_ISP_AF1_MEAN_THRES */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_mean_thres    : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_MEAN_THRES;

/* Define the union U_ISP_AF1_IIRG0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iirg0_0       : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_af1_iirg0_1       : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRG0;

/* Define the union U_ISP_AF1_IIRG1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iirg1_0       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af1_iirg1_1       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRG1;

/* Define the union U_ISP_AF1_IIRG2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iirg2_0       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af1_iirg2_1       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRG2;

/* Define the union U_ISP_AF1_IIRG3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iirg3_0       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af1_iirg3_1       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRG3;

/* Define the union U_ISP_AF1_IIRG4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iirg4_0       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af1_iirg4_1       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRG4;

/* Define the union U_ISP_AF1_IIRG5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iirg5_0       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af1_iirg5_1       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRG5;

/* Define the union U_ISP_AF1_IIRG6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iirg6_0       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af1_iirg6_1       : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRG6;

/* Define the union U_ISP_AF1_IIRPL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iirplg_0      : 8   ; /* [7..0]  */
        unsigned int    isp_af1_iirpls_0      : 3   ; /* [10..8]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_af1_iirplg_1      : 8   ; /* [23..16]  */
        unsigned int    isp_af1_iirpls_1      : 3   ; /* [26..24]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRPL;

/* Define the union U_ISP_AF1_SHIFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iirshift0_0   : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    isp_af1_iirshift0_1   : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    isp_af1_iirshift0_2   : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    isp_af1_iirshift0_3   : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    isp_af1_iirshift1_0   : 3   ; /* [18..16]  */
        unsigned int    reserved_4            : 1   ; /* [19]  */
        unsigned int    isp_af1_iirshift1_1   : 3   ; /* [22..20]  */
        unsigned int    reserved_5            : 1   ; /* [23]  */
        unsigned int    isp_af1_iirshift1_2   : 3   ; /* [26..24]  */
        unsigned int    reserved_6            : 1   ; /* [27]  */
        unsigned int    isp_af1_iirshift1_3   : 3   ; /* [30..28]  */
        unsigned int    reserved_7            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_SHIFT;

/* Define the union U_ISP_AF1_FIRH0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_firh0_0       : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_af1_firh0_1       : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRH0;

/* Define the union U_ISP_AF1_FIRH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_firh1_0       : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_af1_firh1_1       : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRH1;

/* Define the union U_ISP_AF1_FIRH2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_firh2_0       : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_af1_firh2_1       : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRH2;

/* Define the union U_ISP_AF1_FIRH3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_firh3_0       : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_af1_firh3_1       : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRH3;

/* Define the union U_ISP_AF1_FIRH4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_firh4_0       : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_af1_firh4_1       : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRH4;

/* Define the union U_ISP_AF1_ACC_SHIFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_acc_shift0_h  : 4   ; /* [3..0]  */
        unsigned int    isp_af1_acc_shift1_h  : 4   ; /* [7..4]  */
        unsigned int    isp_af1_acc_shift0_v  : 4   ; /* [11..8]  */
        unsigned int    isp_af1_acc_shift1_v  : 4   ; /* [15..12]  */
        unsigned int    isp_af1_acc_shift_y   : 4   ; /* [19..16]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_ACC_SHIFT;

/* Define the union U_ISP_AF1_CNT_SHIFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_cnt_shift0_h  : 4   ; /* [3..0]  */
        unsigned int    isp_af1_cnt_shift1_h  : 4   ; /* [7..4]  */
        unsigned int    isp_af1_cnt_shift0_v  : 4   ; /* [11..8]  */
        unsigned int    isp_af1_cnt_shift1_v  : 4   ; /* [15..12]  */
        unsigned int    isp_af1_cnt_shift_y   : 4   ; /* [19..16]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_CNT_SHIFT;

/* Define the union U_ISP_AF1_STAT_H1_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_stat_h1_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_STAT_H1_RADDR;
/* Define the union U_ISP_AF1_STAT_H1_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_stat_h1_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_STAT_H1_RDATA;
/* Define the union U_ISP_AF1_STAT_H2_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_stat_h2_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_STAT_H2_RADDR;
/* Define the union U_ISP_AF1_STAT_H2_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_stat_h2_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_STAT_H2_RDATA;
/* Define the union U_ISP_AF1_STAT_V1_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_stat_v1_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_STAT_V1_RADDR;
/* Define the union U_ISP_AF1_STAT_V1_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_stat_v1_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_STAT_V1_RDATA;
/* Define the union U_ISP_AF1_STAT_V2_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_stat_v2_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_STAT_V2_RADDR;
/* Define the union U_ISP_AF1_STAT_V2_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_stat_v2_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_STAT_V2_RDATA;
/* Define the union U_ISP_AF1_STAT_Y_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_stat_y_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_STAT_Y_RADDR;
/* Define the union U_ISP_AF1_STAT_Y_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af1_stat_y_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_STAT_Y_RDATA;
/* Define the union U_ISP_AF1_IIRTHRE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iir_thre0_l   : 8   ; /* [7..0]  */
        unsigned int    isp_af1_iir_thre0_h   : 8   ; /* [15..8]  */
        unsigned int    isp_af1_iir_thre1_l   : 8   ; /* [23..16]  */
        unsigned int    isp_af1_iir_thre1_h   : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRTHRE;

/* Define the union U_ISP_AF1_IIRGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iir_gain0_l   : 8   ; /* [7..0]  */
        unsigned int    isp_af1_iir_gain0_h   : 8   ; /* [15..8]  */
        unsigned int    isp_af1_iir_gain1_l   : 8   ; /* [23..16]  */
        unsigned int    isp_af1_iir_gain1_h   : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRGAIN;

/* Define the union U_ISP_AF1_IIRSLOPE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iir_slope0_l  : 4   ; /* [3..0]  */
        unsigned int    isp_af1_iir_slope0_h  : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_af1_iir_slope1_l  : 4   ; /* [19..16]  */
        unsigned int    isp_af1_iir_slope1_h  : 4   ; /* [23..20]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRSLOPE;

/* Define the union U_ISP_AF1_IIRDELAY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iir_delay0    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_af1_iir_delay1    : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRDELAY;

/* Define the union U_ISP_AF1_FIRTHRE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_fir_thre0_l   : 8   ; /* [7..0]  */
        unsigned int    isp_af1_fir_thre0_h   : 8   ; /* [15..8]  */
        unsigned int    isp_af1_fir_thre1_l   : 8   ; /* [23..16]  */
        unsigned int    isp_af1_fir_thre1_h   : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRTHRE;

/* Define the union U_ISP_AF1_FIRGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_fir_gain0_l   : 8   ; /* [7..0]  */
        unsigned int    isp_af1_fir_gain0_h   : 8   ; /* [15..8]  */
        unsigned int    isp_af1_fir_gain1_l   : 8   ; /* [23..16]  */
        unsigned int    isp_af1_fir_gain1_h   : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRGAIN;

/* Define the union U_ISP_AF1_FIRSLOPE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_fir_slope0_l  : 4   ; /* [3..0]  */
        unsigned int    isp_af1_fir_slope0_h  : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_af1_fir_slope1_l  : 4   ; /* [19..16]  */
        unsigned int    isp_af1_fir_slope1_h  : 4   ; /* [23..20]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRSLOPE;

/* Define the union U_ISP_AF1_IIRTHRE_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iir_thre0_c   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_af1_iir_thre1_c   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRTHRE_CORING;

/* Define the union U_ISP_AF1_IIRPEAK_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iir_peak0_c   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_af1_iir_peak1_c   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRPEAK_CORING;

/* Define the union U_ISP_AF1_IIRSLOPE_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_iir_slope0_c  : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_af1_iir_slope1_c  : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_IIRSLOPE_CORING;

/* Define the union U_ISP_AF1_FIRTHRE_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_fir_thre0_c   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_af1_fir_thre1_c   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRTHRE_CORING;

/* Define the union U_ISP_AF1_FIRPEAK_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_fir_peak0_c   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_af1_fir_peak1_c   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRPEAK_CORING;

/* Define the union U_ISP_AF1_FIRSLOPE_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_fir_slope0_c  : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_af1_fir_slope1_c  : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_FIRSLOPE_CORING;

/* Define the union U_ISP_AF1_HILIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_hilight       : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_HILIGHT;

/* Define the union U_ISP_AF1_OFFSET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af1_offset_gr     : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_af1_offset_gb     : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF1_OFFSET;

/* Define the union U_ISP_DIS1_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_dis1_blc_en       : 1   ; /* [1]  */
        unsigned int    reserved_1            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_CFG;

/* Define the union U_ISP_DIS1_BLK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_blk_size_0   : 1   ; /* [0]  */
        unsigned int    isp_dis1_blk_size     : 8   ; /* [8..1]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_dis1_srch_range_0 : 1   ; /* [16]  */
        unsigned int    isp_dis1_srch_range   : 7   ; /* [23..17]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_BLK;

/* Define the union U_ISP_DIS1_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis1_version       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_VERSION;
/* Define the union U_ISP_DIS1_V0POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_v0pos_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_v0pos_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_V0POS;

/* Define the union U_ISP_DIS1_V4POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_v4pos_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_v4pos_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_V4POS;

/* Define the union U_ISP_DIS1_V8POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_v8pos_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_v8pos_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_V8POS;

/* Define the union U_ISP_DIS1_V0POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_v0pose_hor   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_v0pose_ver   : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_V0POSE;

/* Define the union U_ISP_DIS1_V4POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_v4pose_hor   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_v4pose_ver   : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_V4POSE;

/* Define the union U_ISP_DIS1_V8POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_v8pose_hor   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_v8pose_ver   : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_V8POSE;

/* Define the union U_ISP_DIS1_H0POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_h0pos_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_h0pos_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_H0POS;

/* Define the union U_ISP_DIS1_H4POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_h4pos_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_h4pos_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_H4POS;

/* Define the union U_ISP_DIS1_H8POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_h8pos_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_h8pos_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_H8POS;

/* Define the union U_ISP_DIS1_H0POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_h0pose_hor   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_h0pose_ver   : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_H0POSE;

/* Define the union U_ISP_DIS1_H4POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_h4pose_hor   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_h4pose_ver   : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_H4POSE;

/* Define the union U_ISP_DIS1_H8POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_h8pose_hor   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis1_h8pose_ver   : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_H8POSE;

/* Define the union U_ISP_DIS1_RAW_LUMA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_raw_luma     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_RAW_LUMA;

/* Define the union U_ISP_DIS1_GAMMA_EN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_gamma_en     : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_GAMMA_EN;

/* Define the union U_ISP_DIS1_GAIN_LMT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_gain_lmt     : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_GAIN_LMT;

/* Define the union U_ISP_DIS1_OFFSET_GR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_offset_gr    : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_OFFSET_GR;

/* Define the union U_ISP_DIS1_OFFSET_GB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_offset_gb    : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_OFFSET_GB;

/* Define the union U_ISP_DIS1_SCALE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis1_hor_scale    : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 6   ; /* [7..2]  */
        unsigned int    isp_dis1_ver_scale    : 2   ; /* [9..8]  */
        unsigned int    reserved_1            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_SCALE;

/* Define the union U_ISP_DIS1_H_STAT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_H_STAT_WADDR;
/* Define the union U_ISP_DIS1_H_STAT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_H_STAT_WDATA;
/* Define the union U_ISP_DIS1_H_STAT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis1_h_stat_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_H_STAT_RADDR;
/* Define the union U_ISP_DIS1_H_STAT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis1_h_stat_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_H_STAT_RDATA;
/* Define the union U_ISP_DIS1_V_STAT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_V_STAT_WADDR;
/* Define the union U_ISP_DIS1_V_STAT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_V_STAT_WDATA;
/* Define the union U_ISP_DIS1_V_STAT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis1_v_stat_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_V_STAT_RADDR;
/* Define the union U_ISP_DIS1_V_STAT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis1_v_stat_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS1_V_STAT_RDATA;
/* Define the union U_ISP_LSC1_WINNUM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_numh         : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_lsc1_numv         : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_WINNUM;

/* Define the union U_ISP_LSC1_WINX_0_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_deltax       : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_lsc1_invx         : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_WINX_0_23;

/* Define the union U_ISP_LSC1_WIDTH_OFFSET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_width_offset : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_WIDTH_OFFSET;

/* Define the union U_ISP_LSC1_MESH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_mesh_str     : 16  ; /* [15..0]  */
        unsigned int    isp_lsc1_mesh_scale   : 3   ; /* [18..16]  */
        unsigned int    reserved_0            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_MESH;

/* Define the union U_ISP_LSC1_WEIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_mesh_weight  : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_WEIGHT;

/* Define the union U_ISP_LSC1_RGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc1_rgain_waddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_RGAIN_WADDR;
/* Define the union U_ISP_LSC1_RGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_rgain0_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc1_rgain1_wdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_RGAIN_WDATA;

/* Define the union U_ISP_LSC1_RGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc1_rgain_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_RGAIN_RADDR;
/* Define the union U_ISP_LSC1_RGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_rgain0_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc1_rgain1_rdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_RGAIN_RDATA;

/* Define the union U_ISP_LSC1_GRGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc1_grgain_waddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_GRGAIN_WADDR;
/* Define the union U_ISP_LSC1_GRGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_grgain0_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc1_grgain1_wdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_GRGAIN_WDATA;

/* Define the union U_ISP_LSC1_GRGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc1_grgain_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_GRGAIN_RADDR;
/* Define the union U_ISP_LSC1_GRGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_grgain0_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc1_grgain1_rdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_GRGAIN_RDATA;

/* Define the union U_ISP_LSC1_BGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc1_bgain_waddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_BGAIN_WADDR;
/* Define the union U_ISP_LSC1_BGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_bgain0_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc1_bgain1_wdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_BGAIN_WDATA;

/* Define the union U_ISP_LSC1_BGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc1_bgain_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_BGAIN_RADDR;
/* Define the union U_ISP_LSC1_BGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_bgain0_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc1_bgain1_rdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_BGAIN_RDATA;

/* Define the union U_ISP_LSC1_GBGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc1_gbgain_waddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_GBGAIN_WADDR;
/* Define the union U_ISP_LSC1_GBGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_gbgain0_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc1_gbgain1_wdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_GBGAIN_WDATA;

/* Define the union U_ISP_LSC1_GBGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc1_gbgain_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_GBGAIN_RADDR;
/* Define the union U_ISP_LSC1_GBGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_gbgain0_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc1_gbgain1_rdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_GBGAIN_RDATA;

/* Define the union U_ISP_LSC1_WINX_24_31 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_deltax       : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_lsc1_invx         : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_WINX_24_31;

/* Define the union U_ISP_LSC1_WINY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_deltay       : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_lsc1_invy         : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_WINY;

/* Define the union U_ISP_LSC1_BLCEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_blc_in_en    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    isp_lsc1_blc_out_en   : 1   ; /* [8]  */
        unsigned int    reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_BLCEN;

/* Define the union U_ISP_LSC1_BLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_blc_r        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_lsc1_blc_gr       : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_BLC0;

/* Define the union U_ISP_LSC1_BLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_blc_b        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_lsc1_blc_gb       : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_BLC1;

/* Define the union U_ISP_LSC1_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc1_lut_update   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC1_LUT_UPDATE;

/* Define the union U_ISP_RC_BLCEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rc_blc_in_en      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    isp_rc_blc_out_en     : 1   ; /* [8]  */
        unsigned int    reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RC_BLCEN;

/* Define the union U_ISP_RC_BLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rc_blc_r          : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_rc_blc_gr         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RC_BLC0;

/* Define the union U_ISP_RC_BLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rc_blc_b          : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_rc_blc_gb         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RC_BLC1;

/* Define the union U_ISP_RC_SQRADIUS */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rc_sqradius        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RC_SQRADIUS;
/* Define the union U_ISP_RC_CENTER_COOR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rc_cenhor_coor    : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_rc_cenver_coor    : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RC_CENTER_COOR;

/* Define the union U_ISP_LOGLUT1_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_loglut1_version    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT1_VERSION;
/* Define the union U_ISP_LOGLUT1_BITW */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut1_bitw_out  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT1_BITW;

/* Define the union U_ISP_LOGLUT1_OFFSETR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut1_offset_r  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT1_OFFSETR;

/* Define the union U_ISP_LOGLUT1_OFFSETGR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut1_offset_gr : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT1_OFFSETGR;

/* Define the union U_ISP_LOGLUT1_OFFSETGB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut1_offset_gb : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT1_OFFSETGB;

/* Define the union U_ISP_LOGLUT1_OFFSETB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut1_offset_b  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT1_OFFSETB;

/* Define the union U_ISP_LOGLUT1_LUT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_loglut1_lut_waddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT1_LUT_WADDR;
/* Define the union U_ISP_LOGLUT1_LUT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_loglut1_lut_wdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT1_LUT_WDATA;
/* Define the union U_ISP_LOGLUT1_LUT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_loglut1_lut_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT1_LUT_RADDR;
/* Define the union U_ISP_LOGLUT1_LUT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_loglut1_lut_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT1_LUT_RDATA;
/* Define the union U_ISP_COLORBAR_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_colorbar_version   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_VERSION;
/* Define the union U_ISP_COLORBAR_PATTERN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_pattern  : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_PATTERN;

/* Define the union U_ISP_COLORBAR_RBACKGND */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_rbackgnd : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_RBACKGND;

/* Define the union U_ISP_COLORBAR_GBACKGND */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_gbackgnd : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_GBACKGND;

/* Define the union U_ISP_COLORBAR_BBACKGND */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_bbackgnd : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_BBACKGND;

/* Define the union U_ISP_COLORBAR_RFOREGND */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_rforegnd : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_RFOREGND;

/* Define the union U_ISP_COLORBAR_GFOREGND */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_gforegnd : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_GFOREGND;

/* Define the union U_ISP_COLORBAR_BFOREGND */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_bforegnd : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_BFOREGND;

/* Define the union U_ISP_COLORBAR_INIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_rgbinit  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_INIT;

/* Define the union U_ISP_COLORBAR_GRAD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_rgbgrad  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_GRAD;

/* Define the union U_ISP_COLORBAR_POS1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_pos_x1   : 16  ; /* [15..0]  */
        unsigned int    isp_colorbar_pos_y1   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_POS1;

/* Define the union U_ISP_COLORBAR_POS2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_colorbar_pos_x2   : 16  ; /* [15..0]  */
        unsigned int    isp_colorbar_pos_y2   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_COLORBAR_POS2;

/* Define the union U_ISP_CROP_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_crop_version       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP_VERSION;
/* Define the union U_ISP_CROP_SIZEOUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crop_width_out    : 16  ; /* [15..0]  */
        unsigned int    isp_crop_height_out   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP_SIZEOUT;

/* Define the union U_ISP_CROP_POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_crop_pos_x        : 16  ; /* [15..0]  */
        unsigned int    isp_crop_pos_y        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CROP_POS;

/* Define the union U_ISP_DRCS_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_drcs_wrstat_en    : 1   ; /* [1]  */
        unsigned int    reserved_1            : 1   ; /* [2]  */
        unsigned int    isp_drcs_vbiflt_en    : 1   ; /* [3]  */
        unsigned int    isp_drcs_color_corr_en : 1   ; /* [4]  */
        unsigned int    isp_drcs_pdw_sum_en   : 1   ; /* [5]  */
        unsigned int    isp_drcs_detail_boost_en : 1   ; /* [6]  */
        unsigned int    reserved_2            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CFG;

/* Define the union U_ISP_DRCS_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_version       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_VERSION;
/* Define the union U_ISP_DRCS_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_hnum         : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_drcs_vnum         : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_ZONE;

/* Define the union U_ISP_DRCS_ZONE_SIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_zone_hsize   : 9   ; /* [8..0]  */
        unsigned int    isp_drcs_chk_x        : 6   ; /* [14..9]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_drcs_zone_vsize   : 9   ; /* [24..16]  */
        unsigned int    isp_drcs_chk_y        : 6   ; /* [30..25]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_ZONE_SIZE;

/* Define the union U_ISP_DRCS_ZONE_DIV0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_div_x0       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_drcs_div_y0       : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_ZONE_DIV0;

/* Define the union U_ISP_DRCS_ZONE_DIV1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_div_x1       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_drcs_div_y1       : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_ZONE_DIV1;

/* Define the union U_ISP_DRCS_BIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_bin          : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BIN;

/* Define the union U_ISP_DRCS_ZONE_INIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_acc_x_init   : 16  ; /* [15..0]  */
        unsigned int    isp_drcs_cnt_x_init   : 9   ; /* [24..16]  */
        unsigned int    isp_drcs_idx_x_init   : 6   ; /* [30..25]  */
        unsigned int    isp_drcs_big_x_init   : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_ZONE_INIT;

/* Define the union U_ISP_DRCS_BIN_SCALE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_bin_scale    : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BIN_SCALE;

/* Define the union U_ISP_DRCS_OUTBITS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_outbits      : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_OUTBITS;

/* Define the union U_ISP_DRCS_LOCAL_EDGE_LMT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_local_edge_lmt : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_LOCAL_EDGE_LMT;

/* Define the union U_ISP_DRCS_STRENGTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_strength     : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_STRENGTH;

/* Define the union U_ISP_DRCS_STAT_WIDTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_stat_width   : 16  ; /* [15..0]  */
        unsigned int    isp_drcs_total_width  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_STAT_WIDTH;

/* Define the union U_ISP_DRCS_DARK_GAIN_LMT_Y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_val1_y       : 7   ; /* [6..0]  */
        unsigned int    isp_drcs_sft1_y       : 3   ; /* [9..7]  */
        unsigned int    isp_drcs_val2_y       : 4   ; /* [13..10]  */
        unsigned int    isp_drcs_sft2_y       : 2   ; /* [15..14]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DARK_GAIN_LMT_Y;

/* Define the union U_ISP_DRCS_DARK_GAIN_LMT_C */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_val1_c       : 7   ; /* [6..0]  */
        unsigned int    isp_drcs_sft1_c       : 3   ; /* [9..7]  */
        unsigned int    isp_drcs_val2_c       : 4   ; /* [13..10]  */
        unsigned int    isp_drcs_sft2_c       : 2   ; /* [15..14]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DARK_GAIN_LMT_C;

/* Define the union U_ISP_DRCS_BRIGHT_GAIN_LMT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_val          : 7   ; /* [6..0]  */
        unsigned int    isp_drcs_sft          : 3   ; /* [9..7]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BRIGHT_GAIN_LMT;

/* Define the union U_ISP_DRCS_RGB_WGT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_r_wgt        : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_drcs_g_wgt        : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_drcs_b_wgt        : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_RGB_WGT;

/* Define the union U_ISP_DRCS_LUT_MIX_CRTL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_lut_mix_ctrl : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_LUT_MIX_CRTL;

/* Define the union U_ISP_DRCS_GAIN_CLIP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_gain_clip_step : 4   ; /* [3..0]  */
        unsigned int    isp_drcs_gain_clip_knee : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_GAIN_CLIP;

/* Define the union U_ISP_DRCS_COLOR_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cc_lut_ctrl  : 1   ; /* [0]  */
        unsigned int    isp_drcs_cc_lin_pow   : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_COLOR_CTRL;

/* Define the union U_ISP_DRCS_GLOBAL_CORR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cc_global_corr : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_GLOBAL_CORR;

/* Define the union U_ISP_DRCS_MIXING_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_mixing_coring : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_MIXING_CORING;

/* Define the union U_ISP_DRCS_MIXING_DARK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_dark_min     : 8   ; /* [7..0]  */
        unsigned int    isp_drcs_dark_max     : 8   ; /* [15..8]  */
        unsigned int    isp_drcs_dark_thr     : 8   ; /* [23..16]  */
        unsigned int    isp_drcs_dark_slo     : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_MIXING_DARK;

/* Define the union U_ISP_DRCS_MIXING_BRIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_bright_min   : 8   ; /* [7..0]  */
        unsigned int    isp_drcs_bright_max   : 8   ; /* [15..8]  */
        unsigned int    isp_drcs_bright_thr   : 8   ; /* [23..16]  */
        unsigned int    isp_drcs_bright_slo   : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_MIXING_BRIGHT;

/* Define the union U_ISP_DRCS_RG_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_rg_ctr       : 8   ; /* [7..0]  */
        unsigned int    isp_drcs_rg_wid       : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_drcs_rg_slo       : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_RG_CTRL;

/* Define the union U_ISP_DRCS_BG_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_bg_thr       : 8   ; /* [7..0]  */
        unsigned int    isp_drcs_bg_slo       : 3   ; /* [10..8]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BG_CTRL;

/* Define the union U_ISP_DRCS_PDW */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_low_thr      : 8   ; /* [7..0]  */
        unsigned int    isp_drcs_low_slo      : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_drcs_high_thr     : 8   ; /* [23..16]  */
        unsigned int    isp_drcs_high_slo     : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_PDW;

/* Define the union U_ISP_DRCS_PPDTC_SUM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_ppdtc_sum    : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_PPDTC_SUM;

/* Define the union U_ISP_DRCS_TMLUT0_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_tmlut0_waddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_TMLUT0_WADDR;
/* Define the union U_ISP_DRCS_TMLUT0_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_tmlut0_wdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_TMLUT0_WDATA;
/* Define the union U_ISP_DRCS_TMLUT0_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_tmlut0_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_TMLUT0_RADDR;
/* Define the union U_ISP_DRCS_TMLUT0_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_tmlut0_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_TMLUT0_RDATA;
/* Define the union U_ISP_DRCS_TMLUT1_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_tmlut1_waddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_TMLUT1_WADDR;
/* Define the union U_ISP_DRCS_TMLUT1_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_tmlut1_wdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_TMLUT1_WDATA;
/* Define the union U_ISP_DRCS_TMLUT1_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_tmlut1_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_TMLUT1_RADDR;
/* Define the union U_ISP_DRCS_TMLUT1_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_tmlut1_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_TMLUT1_RDATA;
/* Define the union U_ISP_DRCS_CCLUT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_cclut_waddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CCLUT_WADDR;
/* Define the union U_ISP_DRCS_CCLUT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_cclut_wdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CCLUT_WDATA;
/* Define the union U_ISP_DRCS_CCLUT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_cclut_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CCLUT_RADDR;
/* Define the union U_ISP_DRCS_CCLUT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_cclut_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CCLUT_RDATA;
/* Define the union U_ISP_DRCS_GRAD_REV */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_grad_rev_thres : 8   ; /* [7..0]  */
        unsigned int    isp_drcs_grad_rev_max : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_drcs_grad_rev_slope : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 4   ; /* [23..20]  */
        unsigned int    isp_drcs_grad_rev_shift : 3   ; /* [26..24]  */
        unsigned int    reserved_2            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_GRAD_REV;

/* Define the union U_ISP_DRCS_VBI_STRENGTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_var_rng_fine : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    isp_drcs_var_rng_medium : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    isp_drcs_var_rng_coarse : 4   ; /* [11..8]  */
        unsigned int    isp_drcs_var_spa_fine : 2   ; /* [13..12]  */
        unsigned int    reserved_2            : 2   ; /* [15..14]  */
        unsigned int    isp_drcs_var_spa_medium : 3   ; /* [18..16]  */
        unsigned int    reserved_3            : 1   ; /* [19]  */
        unsigned int    isp_drcs_var_spa_coarse : 4   ; /* [23..20]  */
        unsigned int    reserved_4            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_VBI_STRENGTH;

/* Define the union U_ISP_DRCS_ADA_COEF */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_fr_ada_max   : 4   ; /* [3..0]  */
        unsigned int    isp_drcs_dis_offset_coef : 4   ; /* [7..4]  */
        unsigned int    isp_drcs_thr_coef_low : 3   ; /* [10..8]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    isp_drcs_thr_coef_high : 3   ; /* [14..12]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    reserved_2            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_ADA_COEF;

/* Define the union U_ISP_DRCS_VBI_STATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_vbi_state    : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_VBI_STATE;

/* Define the union U_ISP_DRCS_DETAIL_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_detail_coring : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DETAIL_CORING;

/* Define the union U_ISP_DRCS_DETAIL_DARK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_detail_dark_min : 8   ; /* [7..0]  */
        unsigned int    isp_drcs_detail_dark_max : 8   ; /* [15..8]  */
        unsigned int    isp_drcs_detail_dark_thr : 8   ; /* [23..16]  */
        unsigned int    isp_drcs_detail_dark_slo : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DETAIL_DARK;

/* Define the union U_ISP_DRCS_DETAIL_BRIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_detail_bright_min : 8   ; /* [7..0]  */
        unsigned int    isp_drcs_detail_bright_max : 8   ; /* [15..8]  */
        unsigned int    isp_drcs_detail_bright_thr : 8   ; /* [23..16]  */
        unsigned int    isp_drcs_detail_bright_slo : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DETAIL_BRIGHT;

/* Define the union U_ISP_DRCS_DARK_STEP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_dark_step    : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DARK_STEP;

/* Define the union U_ISP_DRCS_DARK_CURVE0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_detail_dark_curve0 : 4   ; /* [3..0]  */
        unsigned int    isp_drcs_detail_dark_curve1 : 4   ; /* [7..4]  */
        unsigned int    isp_drcs_detail_dark_curve2 : 4   ; /* [11..8]  */
        unsigned int    isp_drcs_detail_dark_curve3 : 4   ; /* [15..12]  */
        unsigned int    isp_drcs_detail_dark_curve4 : 4   ; /* [19..16]  */
        unsigned int    isp_drcs_detail_dark_curve5 : 4   ; /* [23..20]  */
        unsigned int    isp_drcs_detail_dark_curve6 : 4   ; /* [27..24]  */
        unsigned int    isp_drcs_detail_dark_curve7 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DARK_CURVE0;

/* Define the union U_ISP_DRCS_DARK_CURVE1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_detail_dark_curve8 : 4   ; /* [3..0]  */
        unsigned int    isp_drcs_detail_dark_curve9 : 4   ; /* [7..4]  */
        unsigned int    isp_drcs_detail_dark_curve10 : 4   ; /* [11..8]  */
        unsigned int    isp_drcs_detail_dark_curve11 : 4   ; /* [15..12]  */
        unsigned int    isp_drcs_detail_dark_curve12 : 4   ; /* [19..16]  */
        unsigned int    isp_drcs_detail_dark_curve13 : 4   ; /* [23..20]  */
        unsigned int    isp_drcs_detail_dark_curve14 : 4   ; /* [27..24]  */
        unsigned int    isp_drcs_detail_dark_curve15 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DARK_CURVE1;

/* Define the union U_ISP_DRCS_BRIGHT_STEP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_bright_step  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BRIGHT_STEP;

/* Define the union U_ISP_DRCS_BRIGHT_CURVE0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_detail_bright_curve0 : 4   ; /* [3..0]  */
        unsigned int    isp_drcs_detail_bright_curve1 : 4   ; /* [7..4]  */
        unsigned int    isp_drcs_detail_bright_curve2 : 4   ; /* [11..8]  */
        unsigned int    isp_drcs_detail_bright_curve3 : 4   ; /* [15..12]  */
        unsigned int    isp_drcs_detail_bright_curve4 : 4   ; /* [19..16]  */
        unsigned int    isp_drcs_detail_bright_curve5 : 4   ; /* [23..20]  */
        unsigned int    isp_drcs_detail_bright_curve6 : 4   ; /* [27..24]  */
        unsigned int    isp_drcs_detail_bright_curve7 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BRIGHT_CURVE0;

/* Define the union U_ISP_DRCS_BRIGHT_CURVE1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_detail_bright_curve8 : 4   ; /* [3..0]  */
        unsigned int    isp_drcs_detail_bright_curve9 : 4   ; /* [7..4]  */
        unsigned int    isp_drcs_detail_bright_curve10 : 4   ; /* [11..8]  */
        unsigned int    isp_drcs_detail_bright_curve11 : 4   ; /* [15..12]  */
        unsigned int    isp_drcs_detail_bright_curve12 : 4   ; /* [19..16]  */
        unsigned int    isp_drcs_detail_bright_curve13 : 4   ; /* [23..20]  */
        unsigned int    isp_drcs_detail_bright_curve14 : 4   ; /* [27..24]  */
        unsigned int    isp_drcs_detail_bright_curve15 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BRIGHT_CURVE1;

/* Define the union U_ISP_DRCS_PREV_LUMA_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_prev_luma_0  : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_PREV_LUMA_0;

/* Define the union U_ISP_DRCS_PREV_LUMA_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_prev_luma_1  : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_PREV_LUMA_1;

/* Define the union U_ISP_DRCS_PREV_LUMA_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_prev_luma_2  : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_PREV_LUMA_2;

/* Define the union U_ISP_DRCS_PREV_LUMA_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_prev_luma_3  : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_PREV_LUMA_3;

/* Define the union U_ISP_DRCS_PREV_LUMA_4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_prev_luma_4  : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_PREV_LUMA_4;

/* Define the union U_ISP_DRCS_PREV_LUMA_5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_prev_luma_5  : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_PREV_LUMA_5;

/* Define the union U_ISP_DRCS_PREV_LUMA_6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_prev_luma_6  : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_PREV_LUMA_6;

/* Define the union U_ISP_DRCS_PREV_LUMA_7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_prev_luma_7  : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_PREV_LUMA_7;

/* Define the union U_ISP_DRCS_CUBIC_THRESX01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_thresx01 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_THRESX01;

/* Define the union U_ISP_DRCS_CUBIC_THRESX10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_thresx10 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_THRESX10;

/* Define the union U_ISP_DRCS_CUBIC_THRESX11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_thresx11 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_THRESX11;

/* Define the union U_ISP_DRCS_CUBIC_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_sel    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_SEL;

/* Define the union U_ISP_DRCS_CUBIC_COEF00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef00_a : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 8   ; /* [23..16]  */
        unsigned int    isp_drcs_cubic_coef00_aexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF00;

/* Define the union U_ISP_DRCS_CUBIC_COEF01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef01_b : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 3   ; /* [23..21]  */
        unsigned int    isp_drcs_cubic_coef01_bexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF01;

/* Define the union U_ISP_DRCS_CUBIC_COEF02 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef02_c : 22  ; /* [21..0]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    isp_drcs_cubic_coef02_cexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF02;

/* Define the union U_ISP_DRCS_CUBIC_COEF03 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef03_d : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF03;

/* Define the union U_ISP_DRCS_CUBIC_COEF10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef10_a : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 8   ; /* [23..16]  */
        unsigned int    isp_drcs_cubic_coef10_aexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF10;

/* Define the union U_ISP_DRCS_CUBIC_COEF11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef11_b : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 3   ; /* [23..21]  */
        unsigned int    isp_drcs_cubic_coef11_bexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF11;

/* Define the union U_ISP_DRCS_CUBIC_COEF12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef12_c : 22  ; /* [21..0]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    isp_drcs_cubic_coef12_cexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF12;

/* Define the union U_ISP_DRCS_CUBIC_COEF13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef13_d : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF13;

/* Define the union U_ISP_DRCS_CUBIC_COEF20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef20_a : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 8   ; /* [23..16]  */
        unsigned int    isp_drcs_cubic_coef20_aexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF20;

/* Define the union U_ISP_DRCS_CUBIC_COEF21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef21_b : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 3   ; /* [23..21]  */
        unsigned int    isp_drcs_cubic_coef21_bexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF21;

/* Define the union U_ISP_DRCS_CUBIC_COEF22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef22_c : 22  ; /* [21..0]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    isp_drcs_cubic_coef22_cexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF22;

/* Define the union U_ISP_DRCS_CUBIC_COEF23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef23_d : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF23;

/* Define the union U_ISP_DRCS_CUBIC_COEF30 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef30_a : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 8   ; /* [23..16]  */
        unsigned int    isp_drcs_cubic_coef30_aexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF30;

/* Define the union U_ISP_DRCS_CUBIC_COEF31 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef31_b : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 3   ; /* [23..21]  */
        unsigned int    isp_drcs_cubic_coef31_bexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF31;

/* Define the union U_ISP_DRCS_CUBIC_COEF32 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef32_c : 22  ; /* [21..0]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    isp_drcs_cubic_coef32_cexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF32;

/* Define the union U_ISP_DRCS_CUBIC_COEF33 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_cubic_coef33_d : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_CUBIC_COEF33;

/* Define the union U_ISP_DRCS_FLT_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_flt_spa_fine : 4   ; /* [3..0]  */
        unsigned int    isp_drcs_flt_spa_medium : 4   ; /* [7..4]  */
        unsigned int    isp_drcs_flt_spa_coarse : 4   ; /* [11..8]  */
        unsigned int    isp_drcs_flt_rng_fine : 4   ; /* [15..12]  */
        unsigned int    isp_drcs_flt_rng_medium : 4   ; /* [19..16]  */
        unsigned int    isp_drcs_flt_rng_coarse : 4   ; /* [23..20]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_FLT_CFG;

/* Define the union U_ISP_DRCS_BIN_FACTOR_COARSE_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_bin_mix_factor_coarse_0 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_drcs_bin_mix_factor_coarse_1 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_drcs_bin_mix_factor_coarse_2 : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    isp_drcs_bin_mix_factor_coarse_3 : 5   ; /* [28..24]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BIN_FACTOR_COARSE_0;

/* Define the union U_ISP_DRCS_BIN_FACTOR_COARSE_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_bin_mix_factor_coarse_4 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_drcs_bin_mix_factor_coarse_5 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_drcs_bin_mix_factor_coarse_6 : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    isp_drcs_bin_mix_factor_coarse_7 : 5   ; /* [28..24]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BIN_FACTOR_COARSE_1;

/* Define the union U_ISP_DRCS_BIN_FACTOR_MEDIUM_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_bin_mix_factor_medium_0 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_drcs_bin_mix_factor_medium_1 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_drcs_bin_mix_factor_medium_2 : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    isp_drcs_bin_mix_factor_medium_3 : 5   ; /* [28..24]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BIN_FACTOR_MEDIUM_0;

/* Define the union U_ISP_DRCS_BIN_FACTOR_MEDIUM_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_bin_mix_factor_medium_4 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_drcs_bin_mix_factor_medium_5 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_drcs_bin_mix_factor_medium_6 : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    isp_drcs_bin_mix_factor_medium_7 : 5   ; /* [28..24]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BIN_FACTOR_MEDIUM_1;

/* Define the union U_ISP_DRCS_DETAIL_SUB_FACTOR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_detail_sub_factor : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DETAIL_SUB_FACTOR;

/* Define the union U_ISP_DRCS_WGT_BOX_TRI_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_wgt_box_tri_sel : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_WGT_BOX_TRI_SEL;

/* Define the union U_ISP_DRCS_BLK_WGT_INIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_blk_wgt_init : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_BLK_WGT_INIT;

/* Define the union U_ISP_DRCS_SHP_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_shp_log      : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_drcs_shp_exp      : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_SHP_CFG;

/* Define the union U_ISP_DRCS_DIV_DENOM_LOG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_div_denom_log : 18  ; /* [17..0]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DIV_DENOM_LOG;

/* Define the union U_ISP_DRCS_DENOM_EXP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drcs_denom_exp    : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DENOM_EXP;

/* Define the union U_ISP_DRCS_DEBUG_INFO */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drcs_debug_info    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRCS_DEBUG_INFO;

/* Define the global struct */
typedef struct {
    volatile unsigned int           reserved_73[32]                     ; /* 0x20000~0x2007c */
    volatile U_ISP_FE_VERSION       ISP_FE_VERSION                    ; /* 0x20080 */
    volatile U_ISP_FE_DATE          ISP_FE_DATE                       ; /* 0x20084 */
    volatile U_ISP_FE_FPGA_DATE     ISP_FE_FPGA_DATE                  ; /* 0x20088 */
    volatile unsigned int           reserved_74                     ; /* 0x2008c */
    volatile U_ISP_FE_MODULE_POS    ISP_FE_MODULE_POS                 ; /* 0x20090 */
    volatile unsigned int           reserved_75[3]                     ; /* 0x20094~0x2009c */
    volatile U_ISP_FE_USER_DEFINE0   ISP_FE_USER_DEFINE0              ; /* 0x200a0 */
    volatile U_ISP_FE_USER_DEFINE1   ISP_FE_USER_DEFINE1              ; /* 0x200a4 */
    volatile unsigned int           reserved_76[2]                     ; /* 0x200a8~0x200ac */
    volatile U_ISP_FE_STARTUP       ISP_FE_STARTUP                    ; /* 0x200b0 */
    volatile U_ISP_FE_FORMAT        ISP_FE_FORMAT                     ; /* 0x200b4 */
    volatile unsigned int           reserved_77[14]                     ; /* 0x200b8~0x200ec */
    volatile U_ISP_FE_INT           ISP_FE_INT                        ; /* 0x200f0 */
    volatile unsigned int           reserved_78                     ; /* 0x200f4 */
    volatile U_ISP_FE_INT_MASK      ISP_FE_INT_MASK                   ; /* 0x200f8 */
    volatile unsigned int           reserved_79                     ; /* 0x200fc */
    volatile U_ISP_FE_CTRL          ISP_FE_CTRL                       ; /* 0x20100 */
    volatile unsigned int           reserved_80[4]                     ; /* 0x20104~0x20110 */
    volatile U_ISP_FE_ADAPTER_CFG   ISP_FE_ADAPTER_CFG                ; /* 0x20114 */
    volatile U_ISP_FE_OUT_SIZE      ISP_FE_OUT_SIZE                   ; /* 0x20118 */
    volatile unsigned int           reserved_81[30]                     ; /* 0x2011c~0x20190 */
    volatile U_ISP_FE_FSTART_DELAY   ISP_FE_FSTART_DELAY              ; /* 0x20194 */
    volatile unsigned int           reserved_82[18]                     ; /* 0x20198~0x201dc */
    volatile U_ISP_FE_CTRL_F        ISP_FE_CTRL_F                     ; /* 0x201e0 */
    volatile U_ISP_FE_CTRL_I        ISP_FE_CTRL_I                     ; /* 0x201e4 */
    volatile U_ISP_FE_TIMING_CFG    ISP_FE_TIMING_CFG                 ; /* 0x201e8 */
    volatile U_ISP_FE_REG_UPDATE    ISP_FE_REG_UPDATE                 ; /* 0x201ec */
    volatile U_ISP_FE_SIZE          ISP_FE_SIZE                       ; /* 0x201f0 */
    volatile unsigned int           reserved_83[395]                     ; /* 0x201f4~0x2081c */
    volatile U_ISP_FE_IN_SUM_CFG    ISP_FE_IN_SUM_CFG                 ; /* 0x20820 */
    volatile U_ISP_FE_OUT_SUM_CFG   ISP_FE_OUT_SUM_CFG                ; /* 0x20824 */
    volatile unsigned int           reserved_84[19]                     ; /* 0x20828~0x20870 */
    volatile U_ISP_FE_BLK_SIZE      ISP_FE_BLK_SIZE                   ; /* 0x20874 */
    volatile U_ISP_FE_BLK_HBLANK    ISP_FE_BLK_HBLANK                 ; /* 0x20878 */
    volatile U_ISP_FE_BLK_VBLANK    ISP_FE_BLK_VBLANK                 ; /* 0x2087c */
    volatile unsigned int           reserved_85[4]                     ; /* 0x20880~0x2088c */
    volatile U_ISP_FE_IN_SUM0       ISP_FE_IN_SUM0                    ; /* 0x20890 */
    volatile U_ISP_FE_IN_SUM1       ISP_FE_IN_SUM1                    ; /* 0x20894 */
    volatile unsigned int           reserved_86[2]                     ; /* 0x20898~0x2089c */
    volatile U_ISP_FE_OUT_SUM0      ISP_FE_OUT_SUM0                   ; /* 0x208a0 */
    volatile U_ISP_FE_OUT_SUM1      ISP_FE_OUT_SUM1                   ; /* 0x208a4 */
    volatile unsigned int           reserved_87[537]                     ; /* 0x208a8~0x21108 */
    volatile U_ISP_BLC1_VERSION     ISP_BLC1_VERSION                  ; /* 0x2110c */
    volatile U_ISP_BLC1_OFFSET1     ISP_BLC1_OFFSET1                  ; /* 0x21110 */
    volatile U_ISP_BLC1_OFFSET2     ISP_BLC1_OFFSET2                  ; /* 0x21114 */
    volatile unsigned int           reserved_88[763]                     ; /* 0x21118~0x21d00 */
    volatile U_ISP_DG2_BLC_CFG      ISP_DG2_BLC_CFG                   ; /* 0x21d04 */
    volatile unsigned int           reserved_89                     ; /* 0x21d08 */
    volatile U_ISP_DG2_VERSION      ISP_DG2_VERSION                   ; /* 0x21d0c */
    volatile U_ISP_DG2_GAIN1        ISP_DG2_GAIN1                     ; /* 0x21d10 */
    volatile U_ISP_DG2_GAIN2        ISP_DG2_GAIN2                     ; /* 0x21d14 */
    volatile U_ISP_DG2_BLC_OFFSET1   ISP_DG2_BLC_OFFSET1              ; /* 0x21d18 */
    volatile U_ISP_DG2_BLC_OFFSET2   ISP_DG2_BLC_OFFSET2              ; /* 0x21d1c */
    volatile U_ISP_DG2_CLIP_VALUE   ISP_DG2_CLIP_VALUE                ; /* 0x21d20 */
    volatile unsigned int           reserved_90[55]                     ; /* 0x21d24~0x21dfc */
    volatile U_ISP_HRS_CFG          ISP_HRS_CFG                       ; /* 0x21e00 */
    volatile unsigned int           reserved_91[3]                     ; /* 0x21e04~0x21e0c */
    volatile U_ISP_HRS_FILTERLUT0   ISP_HRS_FILTERLUT0                ; /* 0x21e10 */
    volatile U_ISP_HRS_FILTERLUT1   ISP_HRS_FILTERLUT1                ; /* 0x21e14 */
    volatile U_ISP_HRS_FILTERLUT2   ISP_HRS_FILTERLUT2                ; /* 0x21e18 */
    volatile U_ISP_HRS_FILTERLUT3   ISP_HRS_FILTERLUT3                ; /* 0x21e1c */
    volatile U_ISP_HRS_FILTERLUT4   ISP_HRS_FILTERLUT4                ; /* 0x21e20 */
    volatile U_ISP_HRS_FILTERLUT5   ISP_HRS_FILTERLUT5                ; /* 0x21e24 */
    volatile unsigned int           reserved_92[55]                     ; /* 0x21e28~0x21f00 */
    volatile U_ISP_WB1_BLC_CFG      ISP_WB1_BLC_CFG                   ; /* 0x21f04 */
    volatile unsigned int           reserved_93                     ; /* 0x21f08 */
    volatile U_ISP_WB1_VERSION      ISP_WB1_VERSION                   ; /* 0x21f0c */
    volatile U_ISP_WB1_GAIN1        ISP_WB1_GAIN1                     ; /* 0x21f10 */
    volatile U_ISP_WB1_GAIN2        ISP_WB1_GAIN2                     ; /* 0x21f14 */
    volatile U_ISP_WB1_BLC_OFFSET1   ISP_WB1_BLC_OFFSET1              ; /* 0x21f18 */
    volatile U_ISP_WB1_BLC_OFFSET2   ISP_WB1_BLC_OFFSET2              ; /* 0x21f1c */
    volatile U_ISP_WB1_CLIP_VALUE   ISP_WB1_CLIP_VALUE                ; /* 0x21f20 */
    volatile unsigned int           reserved_94[570]                     ; /* 0x21f24~0x22808 */
    volatile U_ISP_AE1_VERSION      ISP_AE1_VERSION                   ; /* 0x2280c */
    volatile U_ISP_AE1_ZONE         ISP_AE1_ZONE                      ; /* 0x22810 */
    volatile U_ISP_AE1_SKIP_CRG     ISP_AE1_SKIP_CRG                  ; /* 0x22814 */
    volatile U_ISP_AE1_TOTAL_STAT   ISP_AE1_TOTAL_STAT                ; /* 0x22818 */
    volatile U_ISP_AE1_COUNT_STAT   ISP_AE1_COUNT_STAT                ; /* 0x2281c */
    volatile U_ISP_AE1_TOTAL_R_AVER   ISP_AE1_TOTAL_R_AVER            ; /* 0x22820 */
    volatile U_ISP_AE1_TOTAL_GR_AVER   ISP_AE1_TOTAL_GR_AVER          ; /* 0x22824 */
    volatile U_ISP_AE1_TOTAL_GB_AVER   ISP_AE1_TOTAL_GB_AVER          ; /* 0x22828 */
    volatile U_ISP_AE1_TOTAL_B_AVER   ISP_AE1_TOTAL_B_AVER            ; /* 0x2282c */
    volatile U_ISP_AE1_HIST_HIGH    ISP_AE1_HIST_HIGH                 ; /* 0x22830 */
    volatile unsigned int           reserved_95[3]                     ; /* 0x22834~0x2283c */
    volatile U_ISP_AE1_BITMOVE      ISP_AE1_BITMOVE                   ; /* 0x22840 */
    volatile U_ISP_AE1_OFFSET_R     ISP_AE1_OFFSET_R                  ; /* 0x22844 */
    volatile U_ISP_AE1_OFFSET_GR    ISP_AE1_OFFSET_GR                 ; /* 0x22848 */
    volatile U_ISP_AE1_OFFSET_GB    ISP_AE1_OFFSET_GB                 ; /* 0x2284c */
    volatile U_ISP_AE1_OFFSET_B     ISP_AE1_OFFSET_B                  ; /* 0x22850 */
    volatile unsigned int           reserved_96[3]                     ; /* 0x22854~0x2285c */
    volatile U_ISP_AE1_GAMMA_LIMIT   ISP_AE1_GAMMA_LIMIT              ; /* 0x22860 */
    volatile U_ISP_AE1_LUT_UPDATE   ISP_AE1_LUT_UPDATE                ; /* 0x22864 */
    volatile unsigned int           reserved_97[2]                     ; /* 0x22868~0x2286c */
    volatile U_ISP_AE1_FOURPLANEMODE   ISP_AE1_FOURPLANEMODE          ; /* 0x22870 */
    volatile U_ISP_AE1_CROP_POS     ISP_AE1_CROP_POS                  ; /* 0x22874 */
    volatile U_ISP_AE1_CROP_OUTSIZE   ISP_AE1_CROP_OUTSIZE            ; /* 0x22878 */
    volatile unsigned int           reserved_98                     ; /* 0x2287c */
    volatile U_ISP_AE1_HIST_WADDR   ISP_AE1_HIST_WADDR                ; /* 0x22880 */
    volatile U_ISP_AE1_HIST_WDATA   ISP_AE1_HIST_WDATA                ; /* 0x22884 */
    volatile U_ISP_AE1_HIST_RADDR   ISP_AE1_HIST_RADDR                ; /* 0x22888 */
    volatile U_ISP_AE1_HIST_RDATA   ISP_AE1_HIST_RDATA                ; /* 0x2288c */
    volatile U_ISP_AE1_AVER_R_GR_WADDR   ISP_AE1_AVER_R_GR_WADDR      ; /* 0x22890 */
    volatile U_ISP_AE1_AVER_R_GR_WDATA   ISP_AE1_AVER_R_GR_WDATA      ; /* 0x22894 */
    volatile U_ISP_AE1_AVER_R_GR_RADDR   ISP_AE1_AVER_R_GR_RADDR      ; /* 0x22898 */
    volatile U_ISP_AE1_AVER_R_GR_RDATA   ISP_AE1_AVER_R_GR_RDATA      ; /* 0x2289c */
    volatile U_ISP_AE1_AVER_GB_B_WADDR   ISP_AE1_AVER_GB_B_WADDR      ; /* 0x228a0 */
    volatile U_ISP_AE1_AVER_GB_B_WDATA   ISP_AE1_AVER_GB_B_WDATA      ; /* 0x228a4 */
    volatile U_ISP_AE1_AVER_GB_B_RADDR   ISP_AE1_AVER_GB_B_RADDR      ; /* 0x228a8 */
    volatile U_ISP_AE1_AVER_GB_B_RDATA   ISP_AE1_AVER_GB_B_RDATA      ; /* 0x228ac */
    volatile U_ISP_AE1_WEIGHT_WADDR   ISP_AE1_WEIGHT_WADDR            ; /* 0x228b0 */
    volatile U_ISP_AE1_WEIGHT_WDATA   ISP_AE1_WEIGHT_WDATA            ; /* 0x228b4 */
    volatile U_ISP_AE1_WEIGHT_RADDR   ISP_AE1_WEIGHT_RADDR            ; /* 0x228b8 */
    volatile U_ISP_AE1_WEIGHT_RDATA   ISP_AE1_WEIGHT_RDATA            ; /* 0x228bc */
    volatile unsigned int           reserved_99[80]                     ; /* 0x228c0~0x229fc */
    volatile U_ISP_AF1_CFG          ISP_AF1_CFG                       ; /* 0x22a00 */
    volatile unsigned int           reserved_100[2]                     ; /* 0x22a04~0x22a08 */
    volatile U_ISP_AF1_VERSION      ISP_AF1_VERSION                   ; /* 0x22a0c */
    volatile U_ISP_AF1_ZONE         ISP_AF1_ZONE                      ; /* 0x22a10 */
    volatile U_ISP_AF1_CROP_START   ISP_AF1_CROP_START                ; /* 0x22a14 */
    volatile U_ISP_AF1_CROP_SIZE    ISP_AF1_CROP_SIZE                 ; /* 0x22a18 */
    volatile U_ISP_AF1_MEAN_THRES   ISP_AF1_MEAN_THRES                ; /* 0x22a1c */
    volatile U_ISP_AF1_IIRG0        ISP_AF1_IIRG0                     ; /* 0x22a20 */
    volatile U_ISP_AF1_IIRG1        ISP_AF1_IIRG1                     ; /* 0x22a24 */
    volatile U_ISP_AF1_IIRG2        ISP_AF1_IIRG2                     ; /* 0x22a28 */
    volatile U_ISP_AF1_IIRG3        ISP_AF1_IIRG3                     ; /* 0x22a2c */
    volatile U_ISP_AF1_IIRG4        ISP_AF1_IIRG4                     ; /* 0x22a30 */
    volatile U_ISP_AF1_IIRG5        ISP_AF1_IIRG5                     ; /* 0x22a34 */
    volatile U_ISP_AF1_IIRG6        ISP_AF1_IIRG6                     ; /* 0x22a38 */
    volatile U_ISP_AF1_IIRPL        ISP_AF1_IIRPL                     ; /* 0x22a3c */
    volatile U_ISP_AF1_SHIFT        ISP_AF1_SHIFT                     ; /* 0x22a40 */
    volatile unsigned int           reserved_101[3]                     ; /* 0x22a44~0x22a4c */
    volatile U_ISP_AF1_FIRH0        ISP_AF1_FIRH0                     ; /* 0x22a50 */
    volatile U_ISP_AF1_FIRH1        ISP_AF1_FIRH1                     ; /* 0x22a54 */
    volatile U_ISP_AF1_FIRH2        ISP_AF1_FIRH2                     ; /* 0x22a58 */
    volatile U_ISP_AF1_FIRH3        ISP_AF1_FIRH3                     ; /* 0x22a5c */
    volatile U_ISP_AF1_FIRH4        ISP_AF1_FIRH4                     ; /* 0x22a60 */
    volatile unsigned int           reserved_102[5]                     ; /* 0x22a64~0x22a74 */
    volatile U_ISP_AF1_ACC_SHIFT    ISP_AF1_ACC_SHIFT                 ; /* 0x22a78 */
    volatile U_ISP_AF1_CNT_SHIFT    ISP_AF1_CNT_SHIFT                 ; /* 0x22a7c */
    volatile unsigned int           reserved_103[2]                     ; /* 0x22a80~0x22a84 */
    volatile U_ISP_AF1_STAT_H1_RADDR   ISP_AF1_STAT_H1_RADDR          ; /* 0x22a88 */
    volatile U_ISP_AF1_STAT_H1_RDATA   ISP_AF1_STAT_H1_RDATA          ; /* 0x22a8c */
    volatile unsigned int           reserved_104[2]                     ; /* 0x22a90~0x22a94 */
    volatile U_ISP_AF1_STAT_H2_RADDR   ISP_AF1_STAT_H2_RADDR          ; /* 0x22a98 */
    volatile U_ISP_AF1_STAT_H2_RDATA   ISP_AF1_STAT_H2_RDATA          ; /* 0x22a9c */
    volatile unsigned int           reserved_105[2]                     ; /* 0x22aa0~0x22aa4 */
    volatile U_ISP_AF1_STAT_V1_RADDR   ISP_AF1_STAT_V1_RADDR          ; /* 0x22aa8 */
    volatile U_ISP_AF1_STAT_V1_RDATA   ISP_AF1_STAT_V1_RDATA          ; /* 0x22aac */
    volatile unsigned int           reserved_106[2]                     ; /* 0x22ab0~0x22ab4 */
    volatile U_ISP_AF1_STAT_V2_RADDR   ISP_AF1_STAT_V2_RADDR          ; /* 0x22ab8 */
    volatile U_ISP_AF1_STAT_V2_RDATA   ISP_AF1_STAT_V2_RDATA          ; /* 0x22abc */
    volatile unsigned int           reserved_107[2]                     ; /* 0x22ac0~0x22ac4 */
    volatile U_ISP_AF1_STAT_Y_RADDR   ISP_AF1_STAT_Y_RADDR            ; /* 0x22ac8 */
    volatile U_ISP_AF1_STAT_Y_RDATA   ISP_AF1_STAT_Y_RDATA            ; /* 0x22acc */
    volatile unsigned int           reserved_108[12]                     ; /* 0x22ad0~0x22afc */
    volatile U_ISP_AF1_IIRTHRE      ISP_AF1_IIRTHRE                   ; /* 0x22b00 */
    volatile U_ISP_AF1_IIRGAIN      ISP_AF1_IIRGAIN                   ; /* 0x22b04 */
    volatile U_ISP_AF1_IIRSLOPE     ISP_AF1_IIRSLOPE                  ; /* 0x22b08 */
    volatile U_ISP_AF1_IIRDELAY     ISP_AF1_IIRDELAY                  ; /* 0x22b0c */
    volatile U_ISP_AF1_FIRTHRE      ISP_AF1_FIRTHRE                   ; /* 0x22b10 */
    volatile U_ISP_AF1_FIRGAIN      ISP_AF1_FIRGAIN                   ; /* 0x22b14 */
    volatile U_ISP_AF1_FIRSLOPE     ISP_AF1_FIRSLOPE                  ; /* 0x22b18 */
    volatile unsigned int           reserved_109                     ; /* 0x22b1c */
    volatile U_ISP_AF1_IIRTHRE_CORING   ISP_AF1_IIRTHRE_CORING        ; /* 0x22b20 */
    volatile U_ISP_AF1_IIRPEAK_CORING   ISP_AF1_IIRPEAK_CORING        ; /* 0x22b24 */
    volatile U_ISP_AF1_IIRSLOPE_CORING   ISP_AF1_IIRSLOPE_CORING      ; /* 0x22b28 */
    volatile unsigned int           reserved_110                     ; /* 0x22b2c */
    volatile U_ISP_AF1_FIRTHRE_CORING   ISP_AF1_FIRTHRE_CORING        ; /* 0x22b30 */
    volatile U_ISP_AF1_FIRPEAK_CORING   ISP_AF1_FIRPEAK_CORING        ; /* 0x22b34 */
    volatile U_ISP_AF1_FIRSLOPE_CORING   ISP_AF1_FIRSLOPE_CORING      ; /* 0x22b38 */
    volatile unsigned int           reserved_111                     ; /* 0x22b3c */
    volatile U_ISP_AF1_HILIGHT      ISP_AF1_HILIGHT                   ; /* 0x22b40 */
    volatile U_ISP_AF1_OFFSET       ISP_AF1_OFFSET                    ; /* 0x22b44 */
    volatile unsigned int           reserved_112[46]                     ; /* 0x22b48~0x22bfc */
    volatile U_ISP_DIS1_CFG         ISP_DIS1_CFG                      ; /* 0x22c00 */
    volatile U_ISP_DIS1_BLK         ISP_DIS1_BLK                      ; /* 0x22c04 */
    volatile unsigned int           reserved_113                     ; /* 0x22c08 */
    volatile U_ISP_DIS1_VERSION     ISP_DIS1_VERSION                  ; /* 0x22c0c */
    volatile U_ISP_DIS1_V0POS       ISP_DIS1_V0POS                    ; /* 0x22c10 */
    volatile U_ISP_DIS1_V4POS       ISP_DIS1_V4POS                    ; /* 0x22c14 */
    volatile U_ISP_DIS1_V8POS       ISP_DIS1_V8POS                    ; /* 0x22c18 */
    volatile unsigned int           reserved_114                     ; /* 0x22c1c */
    volatile U_ISP_DIS1_V0POSE      ISP_DIS1_V0POSE                   ; /* 0x22c20 */
    volatile U_ISP_DIS1_V4POSE      ISP_DIS1_V4POSE                   ; /* 0x22c24 */
    volatile U_ISP_DIS1_V8POSE      ISP_DIS1_V8POSE                   ; /* 0x22c28 */
    volatile unsigned int           reserved_115                     ; /* 0x22c2c */
    volatile U_ISP_DIS1_H0POS       ISP_DIS1_H0POS                    ; /* 0x22c30 */
    volatile U_ISP_DIS1_H4POS       ISP_DIS1_H4POS                    ; /* 0x22c34 */
    volatile U_ISP_DIS1_H8POS       ISP_DIS1_H8POS                    ; /* 0x22c38 */
    volatile unsigned int           reserved_116                     ; /* 0x22c3c */
    volatile U_ISP_DIS1_H0POSE      ISP_DIS1_H0POSE                   ; /* 0x22c40 */
    volatile U_ISP_DIS1_H4POSE      ISP_DIS1_H4POSE                   ; /* 0x22c44 */
    volatile U_ISP_DIS1_H8POSE      ISP_DIS1_H8POSE                   ; /* 0x22c48 */
    volatile unsigned int           reserved_117                     ; /* 0x22c4c */
    volatile U_ISP_DIS1_RAW_LUMA    ISP_DIS1_RAW_LUMA                 ; /* 0x22c50 */
    volatile U_ISP_DIS1_GAMMA_EN    ISP_DIS1_GAMMA_EN                 ; /* 0x22c54 */
    volatile U_ISP_DIS1_GAIN_LMT    ISP_DIS1_GAIN_LMT                 ; /* 0x22c58 */
    volatile U_ISP_DIS1_OFFSET_GR   ISP_DIS1_OFFSET_GR                ; /* 0x22c5c */
    volatile U_ISP_DIS1_OFFSET_GB   ISP_DIS1_OFFSET_GB                ; /* 0x22c60 */
    volatile U_ISP_DIS1_SCALE       ISP_DIS1_SCALE                    ; /* 0x22c64 */
    volatile unsigned int           reserved_118[6]                     ; /* 0x22c68~0x22c7c */
    volatile U_ISP_DIS1_H_STAT_WADDR   ISP_DIS1_H_STAT_WADDR          ; /* 0x22c80 */
    volatile U_ISP_DIS1_H_STAT_WDATA   ISP_DIS1_H_STAT_WDATA          ; /* 0x22c84 */
    volatile U_ISP_DIS1_H_STAT_RADDR   ISP_DIS1_H_STAT_RADDR          ; /* 0x22c88 */
    volatile U_ISP_DIS1_H_STAT_RDATA   ISP_DIS1_H_STAT_RDATA          ; /* 0x22c8c */
    volatile U_ISP_DIS1_V_STAT_WADDR   ISP_DIS1_V_STAT_WADDR          ; /* 0x22c90 */
    volatile U_ISP_DIS1_V_STAT_WDATA   ISP_DIS1_V_STAT_WDATA          ; /* 0x22c94 */
    volatile U_ISP_DIS1_V_STAT_RADDR   ISP_DIS1_V_STAT_RADDR          ; /* 0x22c98 */
    volatile U_ISP_DIS1_V_STAT_RDATA   ISP_DIS1_V_STAT_RDATA          ; /* 0x22c9c */
    volatile unsigned int           reserved_119[476]                     ; /* 0x22ca0~0x2340c */
    volatile U_ISP_LSC1_WINNUM      ISP_LSC1_WINNUM                   ; /* 0x23410 */
    volatile U_ISP_LSC1_WINX_0_23       ISP_LSC1_WINX_0_23[24]        ; /* 0x23414 */
    volatile U_ISP_LSC1_WIDTH_OFFSET   ISP_LSC1_WIDTH_OFFSET          ; /* 0x23474 */
    volatile U_ISP_LSC1_MESH        ISP_LSC1_MESH                     ; /* 0x23478 */
    volatile U_ISP_LSC1_WEIGHT      ISP_LSC1_WEIGHT                   ; /* 0x2347c */
    volatile U_ISP_LSC1_RGAIN_WADDR   ISP_LSC1_RGAIN_WADDR            ; /* 0x23480 */
    volatile U_ISP_LSC1_RGAIN_WDATA   ISP_LSC1_RGAIN_WDATA            ; /* 0x23484 */
    volatile U_ISP_LSC1_RGAIN_RADDR   ISP_LSC1_RGAIN_RADDR            ; /* 0x23488 */
    volatile U_ISP_LSC1_RGAIN_RDATA   ISP_LSC1_RGAIN_RDATA            ; /* 0x2348c */
    volatile U_ISP_LSC1_GRGAIN_WADDR   ISP_LSC1_GRGAIN_WADDR          ; /* 0x23490 */
    volatile U_ISP_LSC1_GRGAIN_WDATA   ISP_LSC1_GRGAIN_WDATA          ; /* 0x23494 */
    volatile U_ISP_LSC1_GRGAIN_RADDR   ISP_LSC1_GRGAIN_RADDR          ; /* 0x23498 */
    volatile U_ISP_LSC1_GRGAIN_RDATA   ISP_LSC1_GRGAIN_RDATA          ; /* 0x2349c */
    volatile U_ISP_LSC1_BGAIN_WADDR   ISP_LSC1_BGAIN_WADDR            ; /* 0x234a0 */
    volatile U_ISP_LSC1_BGAIN_WDATA   ISP_LSC1_BGAIN_WDATA            ; /* 0x234a4 */
    volatile U_ISP_LSC1_BGAIN_RADDR   ISP_LSC1_BGAIN_RADDR            ; /* 0x234a8 */
    volatile U_ISP_LSC1_BGAIN_RDATA   ISP_LSC1_BGAIN_RDATA            ; /* 0x234ac */
    volatile U_ISP_LSC1_GBGAIN_WADDR   ISP_LSC1_GBGAIN_WADDR          ; /* 0x234b0 */
    volatile U_ISP_LSC1_GBGAIN_WDATA   ISP_LSC1_GBGAIN_WDATA          ; /* 0x234b4 */
    volatile U_ISP_LSC1_GBGAIN_RADDR   ISP_LSC1_GBGAIN_RADDR          ; /* 0x234b8 */
    volatile U_ISP_LSC1_GBGAIN_RDATA   ISP_LSC1_GBGAIN_RDATA          ; /* 0x234bc */
    volatile unsigned int           reserved_120[20]                     ; /* 0x234c0~0x2350c */
    volatile U_ISP_LSC1_WINX_24_31      ISP_LSC1_WINX_24_31[8]        ; /* 0x23510 */
    volatile U_ISP_LSC1_WINY            ISP_LSC1_WINY[16]             ; /* 0x23530 */
    volatile U_ISP_LSC1_BLCEN       ISP_LSC1_BLCEN                    ; /* 0x23570 */
    volatile U_ISP_LSC1_BLC0        ISP_LSC1_BLC0                     ; /* 0x23574 */
    volatile U_ISP_LSC1_BLC1        ISP_LSC1_BLC1                     ; /* 0x23578 */
    volatile U_ISP_LSC1_LUT_UPDATE   ISP_LSC1_LUT_UPDATE              ; /* 0x2357c */
    volatile unsigned int           reserved_121[36]                     ; /* 0x23580~0x2360c */
    volatile U_ISP_RC_BLCEN         ISP_RC_BLCEN                      ; /* 0x23610 */
    volatile U_ISP_RC_BLC0          ISP_RC_BLC0                       ; /* 0x23614 */
    volatile U_ISP_RC_BLC1          ISP_RC_BLC1                       ; /* 0x23618 */
    volatile unsigned int           reserved_122                     ; /* 0x2361c */
    volatile U_ISP_RC_SQRADIUS      ISP_RC_SQRADIUS                   ; /* 0x23620 */
    volatile U_ISP_RC_CENTER_COOR   ISP_RC_CENTER_COOR                ; /* 0x23624 */
    volatile unsigned int           reserved_123[3897]                     ; /* 0x23628~0x27308 */
    volatile U_ISP_LOGLUT1_VERSION   ISP_LOGLUT1_VERSION              ; /* 0x2730c */
    volatile U_ISP_LOGLUT1_BITW     ISP_LOGLUT1_BITW                  ; /* 0x27310 */
    volatile U_ISP_LOGLUT1_OFFSETR   ISP_LOGLUT1_OFFSETR              ; /* 0x27314 */
    volatile U_ISP_LOGLUT1_OFFSETGR   ISP_LOGLUT1_OFFSETGR            ; /* 0x27318 */
    volatile U_ISP_LOGLUT1_OFFSETGB   ISP_LOGLUT1_OFFSETGB            ; /* 0x2731c */
    volatile U_ISP_LOGLUT1_OFFSETB   ISP_LOGLUT1_OFFSETB              ; /* 0x27320 */
    volatile unsigned int           reserved_124[23]                     ; /* 0x27324~0x2737c */
    volatile U_ISP_LOGLUT1_LUT_WADDR   ISP_LOGLUT1_LUT_WADDR          ; /* 0x27380 */
    volatile U_ISP_LOGLUT1_LUT_WDATA   ISP_LOGLUT1_LUT_WDATA          ; /* 0x27384 */
    volatile U_ISP_LOGLUT1_LUT_RADDR   ISP_LOGLUT1_LUT_RADDR          ; /* 0x27388 */
    volatile U_ISP_LOGLUT1_LUT_RDATA   ISP_LOGLUT1_LUT_RDATA          ; /* 0x2738c */
    volatile unsigned int           reserved_125[3871]                     ; /* 0x27390~0x2b008 */
    volatile U_ISP_COLORBAR_VERSION   ISP_COLORBAR_VERSION            ; /* 0x2b00c */
    volatile U_ISP_COLORBAR_PATTERN   ISP_COLORBAR_PATTERN            ; /* 0x2b010 */
    volatile U_ISP_COLORBAR_RBACKGND   ISP_COLORBAR_RBACKGND          ; /* 0x2b014 */
    volatile U_ISP_COLORBAR_GBACKGND   ISP_COLORBAR_GBACKGND          ; /* 0x2b018 */
    volatile U_ISP_COLORBAR_BBACKGND   ISP_COLORBAR_BBACKGND          ; /* 0x2b01c */
    volatile U_ISP_COLORBAR_RFOREGND   ISP_COLORBAR_RFOREGND          ; /* 0x2b020 */
    volatile U_ISP_COLORBAR_GFOREGND   ISP_COLORBAR_GFOREGND          ; /* 0x2b024 */
    volatile U_ISP_COLORBAR_BFOREGND   ISP_COLORBAR_BFOREGND          ; /* 0x2b028 */
    volatile U_ISP_COLORBAR_INIT    ISP_COLORBAR_INIT                 ; /* 0x2b02c */
    volatile U_ISP_COLORBAR_GRAD    ISP_COLORBAR_GRAD                 ; /* 0x2b030 */
    volatile U_ISP_COLORBAR_POS1    ISP_COLORBAR_POS1                 ; /* 0x2b034 */
    volatile U_ISP_COLORBAR_POS2    ISP_COLORBAR_POS2                 ; /* 0x2b038 */
    volatile unsigned int           reserved_126[52]                     ; /* 0x2b03c~0x2b108 */
    volatile U_ISP_CROP_VERSION     ISP_CROP_VERSION                  ; /* 0x2b10c */
    volatile U_ISP_CROP_SIZEOUT     ISP_CROP_SIZEOUT                  ; /* 0x2b110 */
    volatile U_ISP_CROP_POS         ISP_CROP_POS                      ; /* 0x2b114 */
    volatile unsigned int           reserved_127[4026]                     ; /* 0x2b118~0x2effc */
    volatile U_ISP_DRCS_CFG         ISP_DRCS_CFG                      ; /* 0x2f000 */
    volatile unsigned int           reserved_128[2]                     ; /* 0x2f004~0x2f008 */
    volatile U_ISP_DRCS_VERSION     ISP_DRCS_VERSION                  ; /* 0x2f00c */
    volatile U_ISP_DRCS_ZONE        ISP_DRCS_ZONE                     ; /* 0x2f010 */
    volatile U_ISP_DRCS_ZONE_SIZE   ISP_DRCS_ZONE_SIZE                ; /* 0x2f014 */
    volatile U_ISP_DRCS_ZONE_DIV0   ISP_DRCS_ZONE_DIV0                ; /* 0x2f018 */
    volatile U_ISP_DRCS_ZONE_DIV1   ISP_DRCS_ZONE_DIV1                ; /* 0x2f01c */
    volatile U_ISP_DRCS_BIN         ISP_DRCS_BIN                      ; /* 0x2f020 */
    volatile U_ISP_DRCS_ZONE_INIT   ISP_DRCS_ZONE_INIT                ; /* 0x2f024 */
    volatile U_ISP_DRCS_BIN_SCALE   ISP_DRCS_BIN_SCALE                ; /* 0x2f028 */
    volatile U_ISP_DRCS_OUTBITS     ISP_DRCS_OUTBITS                  ; /* 0x2f02c */
    volatile unsigned int           reserved_129                     ; /* 0x2f030 */
    volatile U_ISP_DRCS_LOCAL_EDGE_LMT   ISP_DRCS_LOCAL_EDGE_LMT      ; /* 0x2f034 */
    volatile U_ISP_DRCS_STRENGTH    ISP_DRCS_STRENGTH                 ; /* 0x2f038 */
    volatile U_ISP_DRCS_STAT_WIDTH   ISP_DRCS_STAT_WIDTH              ; /* 0x2f03c */
    volatile U_ISP_DRCS_DARK_GAIN_LMT_Y   ISP_DRCS_DARK_GAIN_LMT_Y    ; /* 0x2f040 */
    volatile U_ISP_DRCS_DARK_GAIN_LMT_C   ISP_DRCS_DARK_GAIN_LMT_C    ; /* 0x2f044 */
    volatile U_ISP_DRCS_BRIGHT_GAIN_LMT   ISP_DRCS_BRIGHT_GAIN_LMT    ; /* 0x2f048 */
    volatile U_ISP_DRCS_RGB_WGT     ISP_DRCS_RGB_WGT                  ; /* 0x2f04c */
    volatile unsigned int           reserved_130                     ; /* 0x2f050 */
    volatile U_ISP_DRCS_LUT_MIX_CRTL   ISP_DRCS_LUT_MIX_CRTL          ; /* 0x2f054 */
    volatile U_ISP_DRCS_GAIN_CLIP   ISP_DRCS_GAIN_CLIP                ; /* 0x2f058 */
    volatile U_ISP_DRCS_COLOR_CTRL   ISP_DRCS_COLOR_CTRL              ; /* 0x2f05c */
    volatile U_ISP_DRCS_GLOBAL_CORR   ISP_DRCS_GLOBAL_CORR            ; /* 0x2f060 */
    volatile U_ISP_DRCS_MIXING_CORING   ISP_DRCS_MIXING_CORING        ; /* 0x2f064 */
    volatile U_ISP_DRCS_MIXING_DARK   ISP_DRCS_MIXING_DARK            ; /* 0x2f068 */
    volatile U_ISP_DRCS_MIXING_BRIGHT   ISP_DRCS_MIXING_BRIGHT        ; /* 0x2f06c */
    volatile U_ISP_DRCS_RG_CTRL     ISP_DRCS_RG_CTRL                  ; /* 0x2f070 */
    volatile U_ISP_DRCS_BG_CTRL     ISP_DRCS_BG_CTRL                  ; /* 0x2f074 */
    volatile U_ISP_DRCS_PDW         ISP_DRCS_PDW                      ; /* 0x2f078 */
    volatile U_ISP_DRCS_PPDTC_SUM   ISP_DRCS_PPDTC_SUM                ; /* 0x2f07c */
    volatile U_ISP_DRCS_TMLUT0_WADDR   ISP_DRCS_TMLUT0_WADDR          ; /* 0x2f080 */
    volatile U_ISP_DRCS_TMLUT0_WDATA   ISP_DRCS_TMLUT0_WDATA          ; /* 0x2f084 */
    volatile U_ISP_DRCS_TMLUT0_RADDR   ISP_DRCS_TMLUT0_RADDR          ; /* 0x2f088 */
    volatile U_ISP_DRCS_TMLUT0_RDATA   ISP_DRCS_TMLUT0_RDATA          ; /* 0x2f08c */
    volatile U_ISP_DRCS_TMLUT1_WADDR   ISP_DRCS_TMLUT1_WADDR          ; /* 0x2f090 */
    volatile U_ISP_DRCS_TMLUT1_WDATA   ISP_DRCS_TMLUT1_WDATA          ; /* 0x2f094 */
    volatile U_ISP_DRCS_TMLUT1_RADDR   ISP_DRCS_TMLUT1_RADDR          ; /* 0x2f098 */
    volatile U_ISP_DRCS_TMLUT1_RDATA   ISP_DRCS_TMLUT1_RDATA          ; /* 0x2f09c */
    volatile U_ISP_DRCS_CCLUT_WADDR   ISP_DRCS_CCLUT_WADDR            ; /* 0x2f0a0 */
    volatile U_ISP_DRCS_CCLUT_WDATA   ISP_DRCS_CCLUT_WDATA            ; /* 0x2f0a4 */
    volatile U_ISP_DRCS_CCLUT_RADDR   ISP_DRCS_CCLUT_RADDR            ; /* 0x2f0a8 */
    volatile U_ISP_DRCS_CCLUT_RDATA   ISP_DRCS_CCLUT_RDATA            ; /* 0x2f0ac */
    volatile unsigned int           reserved_131[20]                     ; /* 0x2f0b0~0x2f0fc */
    volatile U_ISP_DRCS_GRAD_REV    ISP_DRCS_GRAD_REV                 ; /* 0x2f100 */
    volatile U_ISP_DRCS_VBI_STRENGTH   ISP_DRCS_VBI_STRENGTH          ; /* 0x2f104 */
    volatile U_ISP_DRCS_ADA_COEF    ISP_DRCS_ADA_COEF                 ; /* 0x2f108 */
    volatile unsigned int           reserved_132                     ; /* 0x2f10c */
    volatile U_ISP_DRCS_VBI_STATE   ISP_DRCS_VBI_STATE                ; /* 0x2f110 */
    volatile U_ISP_DRCS_DETAIL_CORING   ISP_DRCS_DETAIL_CORING        ; /* 0x2f114 */
    volatile U_ISP_DRCS_DETAIL_DARK   ISP_DRCS_DETAIL_DARK            ; /* 0x2f118 */
    volatile U_ISP_DRCS_DETAIL_BRIGHT   ISP_DRCS_DETAIL_BRIGHT        ; /* 0x2f11c */
    volatile U_ISP_DRCS_DARK_STEP   ISP_DRCS_DARK_STEP                ; /* 0x2f120 */
    volatile U_ISP_DRCS_DARK_CURVE0   ISP_DRCS_DARK_CURVE0            ; /* 0x2f124 */
    volatile U_ISP_DRCS_DARK_CURVE1   ISP_DRCS_DARK_CURVE1            ; /* 0x2f128 */
    volatile unsigned int           reserved_133                     ; /* 0x2f12c */
    volatile U_ISP_DRCS_BRIGHT_STEP   ISP_DRCS_BRIGHT_STEP            ; /* 0x2f130 */
    volatile U_ISP_DRCS_BRIGHT_CURVE0   ISP_DRCS_BRIGHT_CURVE0        ; /* 0x2f134 */
    volatile U_ISP_DRCS_BRIGHT_CURVE1   ISP_DRCS_BRIGHT_CURVE1        ; /* 0x2f138 */
    volatile U_ISP_DRCS_PREV_LUMA_0   ISP_DRCS_PREV_LUMA_0            ; /* 0x2f13c */
    volatile U_ISP_DRCS_PREV_LUMA_1   ISP_DRCS_PREV_LUMA_1            ; /* 0x2f140 */
    volatile U_ISP_DRCS_PREV_LUMA_2   ISP_DRCS_PREV_LUMA_2            ; /* 0x2f144 */
    volatile U_ISP_DRCS_PREV_LUMA_3   ISP_DRCS_PREV_LUMA_3            ; /* 0x2f148 */
    volatile U_ISP_DRCS_PREV_LUMA_4   ISP_DRCS_PREV_LUMA_4            ; /* 0x2f14c */
    volatile U_ISP_DRCS_PREV_LUMA_5   ISP_DRCS_PREV_LUMA_5            ; /* 0x2f150 */
    volatile U_ISP_DRCS_PREV_LUMA_6   ISP_DRCS_PREV_LUMA_6            ; /* 0x2f154 */
    volatile U_ISP_DRCS_PREV_LUMA_7   ISP_DRCS_PREV_LUMA_7            ; /* 0x2f158 */
    volatile unsigned int           reserved_134[41]                     ; /* 0x2f15c~0x2f1fc */
    volatile U_ISP_DRCS_CUBIC_THRESX01   ISP_DRCS_CUBIC_THRESX01      ; /* 0x2f200 */
    volatile U_ISP_DRCS_CUBIC_THRESX10   ISP_DRCS_CUBIC_THRESX10      ; /* 0x2f204 */
    volatile U_ISP_DRCS_CUBIC_THRESX11   ISP_DRCS_CUBIC_THRESX11      ; /* 0x2f208 */
    volatile U_ISP_DRCS_CUBIC_SEL   ISP_DRCS_CUBIC_SEL                ; /* 0x2f20c */
    volatile U_ISP_DRCS_CUBIC_COEF00   ISP_DRCS_CUBIC_COEF00          ; /* 0x2f210 */
    volatile U_ISP_DRCS_CUBIC_COEF01   ISP_DRCS_CUBIC_COEF01          ; /* 0x2f214 */
    volatile U_ISP_DRCS_CUBIC_COEF02   ISP_DRCS_CUBIC_COEF02          ; /* 0x2f218 */
    volatile U_ISP_DRCS_CUBIC_COEF03   ISP_DRCS_CUBIC_COEF03          ; /* 0x2f21c */
    volatile U_ISP_DRCS_CUBIC_COEF10   ISP_DRCS_CUBIC_COEF10          ; /* 0x2f220 */
    volatile U_ISP_DRCS_CUBIC_COEF11   ISP_DRCS_CUBIC_COEF11          ; /* 0x2f224 */
    volatile U_ISP_DRCS_CUBIC_COEF12   ISP_DRCS_CUBIC_COEF12          ; /* 0x2f228 */
    volatile U_ISP_DRCS_CUBIC_COEF13   ISP_DRCS_CUBIC_COEF13          ; /* 0x2f22c */
    volatile U_ISP_DRCS_CUBIC_COEF20   ISP_DRCS_CUBIC_COEF20          ; /* 0x2f230 */
    volatile U_ISP_DRCS_CUBIC_COEF21   ISP_DRCS_CUBIC_COEF21          ; /* 0x2f234 */
    volatile U_ISP_DRCS_CUBIC_COEF22   ISP_DRCS_CUBIC_COEF22          ; /* 0x2f238 */
    volatile U_ISP_DRCS_CUBIC_COEF23   ISP_DRCS_CUBIC_COEF23          ; /* 0x2f23c */
    volatile U_ISP_DRCS_CUBIC_COEF30   ISP_DRCS_CUBIC_COEF30          ; /* 0x2f240 */
    volatile U_ISP_DRCS_CUBIC_COEF31   ISP_DRCS_CUBIC_COEF31          ; /* 0x2f244 */
    volatile U_ISP_DRCS_CUBIC_COEF32   ISP_DRCS_CUBIC_COEF32          ; /* 0x2f248 */
    volatile U_ISP_DRCS_CUBIC_COEF33   ISP_DRCS_CUBIC_COEF33          ; /* 0x2f24c */
    volatile U_ISP_DRCS_FLT_CFG     ISP_DRCS_FLT_CFG                  ; /* 0x2f250 */
    volatile U_ISP_DRCS_BIN_FACTOR_COARSE_0   ISP_DRCS_BIN_FACTOR_COARSE_0 ; /* 0x2f254 */
    volatile U_ISP_DRCS_BIN_FACTOR_COARSE_1   ISP_DRCS_BIN_FACTOR_COARSE_1 ; /* 0x2f258 */
    volatile U_ISP_DRCS_BIN_FACTOR_MEDIUM_0   ISP_DRCS_BIN_FACTOR_MEDIUM_0 ; /* 0x2f25c */
    volatile U_ISP_DRCS_BIN_FACTOR_MEDIUM_1   ISP_DRCS_BIN_FACTOR_MEDIUM_1 ; /* 0x2f260 */
    volatile U_ISP_DRCS_DETAIL_SUB_FACTOR   ISP_DRCS_DETAIL_SUB_FACTOR ; /* 0x2f264 */
    volatile U_ISP_DRCS_WGT_BOX_TRI_SEL   ISP_DRCS_WGT_BOX_TRI_SEL    ; /* 0x2f268 */
    volatile U_ISP_DRCS_BLK_WGT_INIT   ISP_DRCS_BLK_WGT_INIT          ; /* 0x2f26c */
    volatile U_ISP_DRCS_SHP_CFG     ISP_DRCS_SHP_CFG                  ; /* 0x2f270 */
    volatile U_ISP_DRCS_DIV_DENOM_LOG   ISP_DRCS_DIV_DENOM_LOG        ; /* 0x2f274 */
    volatile U_ISP_DRCS_DENOM_EXP   ISP_DRCS_DENOM_EXP                ; /* 0x2f278 */
    volatile U_ISP_DRCS_DEBUG_INFO   ISP_DRCS_DEBUG_INFO              ; /* 0x2f27c */

} S_ISPFE_REGS_TYPE;

/* Define the union U_ISP_VIPROC_CTRL0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    out0_crop_en          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 12  ; /* [12..1]  */
        unsigned int    isp_be_en             : 1   ; /* [13]  */
        unsigned int    nr_en                 : 1   ; /* [14]  */
        unsigned int    sync_fifo_en          : 1   ; /* [15]  */
        unsigned int    img_pro_mode          : 2   ; /* [17..16]  */
        unsigned int    vcpin_sfifo_en        : 1   ; /* [18]  */
        unsigned int    dsf_en                : 1   ; /* [19]  */
        unsigned int    dsf_sel               : 1   ; /* [20]  */
        unsigned int    dcg_fpn_sel           : 1   ; /* [21]  */
        unsigned int    rfr_crop_en           : 1   ; /* [22]  */
        unsigned int    wmad_crop_en          : 1   ; /* [23]  */
        unsigned int    bnr_out_crop_en       : 1   ; /* [24]  */
        unsigned int    fpn_out_crop_en       : 1   ; /* [25]  */
        unsigned int    isp_be_bypass_mode    : 2   ; /* [27..26]  */
        unsigned int    wdr_mode              : 1   ; /* [28]  */
        unsigned int    reserved_1            : 2   ; /* [30..29]  */
        unsigned int    video_en              : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_CTRL0;

/* Define the union U_ISP_VIPROC_REGUP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reg_up                : 1   ; /* [0]  */
        unsigned int    reserved_0            : 4   ; /* [4..1]  */
        unsigned int    reg_up1               : 1   ; /* [5]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    manual_reg_up         : 1   ; /* [8]  */
        unsigned int    reserved_2            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_REGUP;

/* Define the union U_VIPROC_ISP_BE_CTRL0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdrsplit1_en      : 1   ; /* [0]  */
        unsigned int    isp_fpn_en            : 1   ; /* [1]  */
        unsigned int    isp_fpn_mode          : 1   ; /* [2]  */
        unsigned int    isp_dpc_en            : 1   ; /* [3]  */
        unsigned int    isp_dpc_stat_en       : 1   ; /* [4]  */
        unsigned int    isp_ge_en             : 1   ; /* [5]  */
        unsigned int    isp_4dg_en            : 1   ; /* [6]  */
        unsigned int    isp_flicker_en        : 1   ; /* [7]  */
        unsigned int    isp_wdr_en            : 1   ; /* [8]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    isp_bcom_en           : 1   ; /* [10]  */
        unsigned int    isp_bnr_en            : 1   ; /* [11]  */
        unsigned int    isp_bdec_en           : 1   ; /* [12]  */
        unsigned int    isp_lsc_en            : 1   ; /* [13]  */
        unsigned int    isp_dg_en             : 1   ; /* [14]  */
        unsigned int    isp_af_en             : 1   ; /* [15]  */
        unsigned int    isp_awb_en            : 1   ; /* [16]  */
        unsigned int    isp_ae_en             : 1   ; /* [17]  */
        unsigned int    isp_dis_en            : 1   ; /* [18]  */
        unsigned int    isp_wb_en             : 1   ; /* [19]  */
        unsigned int    isp_blc_en            : 1   ; /* [20]  */
        unsigned int    isp_pregamma_en       : 1   ; /* [21]  */
        unsigned int    isp_drc_en            : 1   ; /* [22]  */
        unsigned int    reserved_1            : 1   ; /* [23]  */
        unsigned int    isp_la_en             : 1   ; /* [24]  */
        unsigned int    isp_sqrt_en           : 1   ; /* [25]  */
        unsigned int    isp_dmnr_vhdm_en      : 1   ; /* [26]  */
        unsigned int    isp_dmnr_nddm_en      : 1   ; /* [27]  */
        unsigned int    isp_loglut_en         : 1   ; /* [28]  */
        unsigned int    isp_rlsc_en           : 1   ; /* [29]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VIPROC_ISP_BE_CTRL0;

/* Define the union U_VIPROC_ISP_BE_CTRL1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sq_en             : 1   ; /* [0]  */
        unsigned int    isp_cc_en             : 1   ; /* [1]  */
        unsigned int    isp_clut_en           : 1   ; /* [2]  */
        unsigned int    isp_sqrt1_en          : 1   ; /* [3]  */
        unsigned int    isp_gamma_en          : 1   ; /* [4]  */
        unsigned int    isp_dehaze_en         : 1   ; /* [5]  */
        unsigned int    isp_csc_en            : 1   ; /* [6]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    isp_ldci_en           : 1   ; /* [8]  */
        unsigned int    reserved_1            : 1   ; /* [9]  */
        unsigned int    reserved_2            : 1   ; /* [10]  */
        unsigned int    isp_ca_en             : 1   ; /* [11]  */
        unsigned int    isp_split_en          : 1   ; /* [12]  */
        unsigned int    isp_sharpen_en        : 1   ; /* [13]  */
        unsigned int    isp_mcds_en           : 1   ; /* [14]  */
        unsigned int    reserved_3            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VIPROC_ISP_BE_CTRL1;

/* Define the union U_VIPROC_ISP_BE_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_be_rggb_cfg       : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 14  ; /* [15..2]  */
        unsigned int    isp_be_timing_cfg     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VIPROC_ISP_BE_CFG;

/* Define the union U_ISP_VIPROC_INTMASK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    eof_mask              : 1   ; /* [0]  */
        unsigned int    eof_end_mask          : 1   ; /* [1]  */
        unsigned int    timeout_mask          : 1   ; /* [2]  */
        unsigned int    wbus_err_mask         : 1   ; /* [3]  */
        unsigned int    rbus_err_mask         : 1   ; /* [4]  */
        unsigned int    reserved_0            : 2   ; /* [6..5]  */
        unsigned int    dcmp_err_mask         : 1   ; /* [7]  */
        unsigned int    flow_mask             : 1   ; /* [8]  */
        unsigned int    frm_err_mask          : 1   ; /* [9]  */
        unsigned int    frm_start_mask        : 1   ; /* [10]  */
        unsigned int    frm_start_nr_mask     : 1   ; /* [11]  */
        unsigned int    cfg_loss_mask         : 1   ; /* [12]  */
        unsigned int    cfg_loss_nr_mask      : 1   ; /* [13]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    ispbe_fstart_mask     : 1   ; /* [16]  */
        unsigned int    ispbe_update_cfg_mask : 1   ; /* [17]  */
        unsigned int    ispbe_cfg_loss_mask   : 1   ; /* [18]  */
        unsigned int    reserved_2            : 1   ; /* [19]  */
        unsigned int    ispbe_ae_mask         : 1   ; /* [20]  */
        unsigned int    ispbe_awb_mask        : 1   ; /* [21]  */
        unsigned int    ispbe_af_mask         : 1   ; /* [22]  */
        unsigned int    ispbe_dis_mask        : 1   ; /* [23]  */
        unsigned int    ispbe_dci_mask        : 1   ; /* [24]  */
        unsigned int    ispbe_ldci_mask       : 1   ; /* [25]  */
        unsigned int    ispbe_dehaze_mask     : 1   ; /* [26]  */
        unsigned int    ispbe_flick_mask      : 1   ; /* [27]  */
        unsigned int    ispbe_mg_mask         : 1   ; /* [28]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_INTMASK;

/* Define the union U_ISP_VIPROC_PARA_DRC_ADDR_LOW */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int para_drc_addr_low      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_PARA_DRC_ADDR_LOW;
/* Define the union U_ISP_VIPROC_PARA_DRC_ADDR_HIGH */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int para_drc_addr_high     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_PARA_DRC_ADDR_HIGH;
/* Define the union U_ISP_VIPROC_PARA_DCI_ADDR_LOW */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int para_dci_addr_low      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_PARA_DCI_ADDR_LOW;
/* Define the union U_ISP_VIPROC_PARA_DCI_ADDR_HIGH */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int para_dci_addr_high     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_PARA_DCI_ADDR_HIGH;
/* Define the union U_ISP_VIPROC_OUT_PARA_DRC_ADDR_LOW */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int out_para_drc_addr_low  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_OUT_PARA_DRC_ADDR_LOW;
/* Define the union U_ISP_VIPROC_OUT_PARA_DRC_ADDR_HIGH */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int out_para_drc_addr_high : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_OUT_PARA_DRC_ADDR_HIGH;
/* Define the union U_ISP_VIPROC_OUT_PARA_DCI_ADDR_LOW */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int out_para_dci_addr_low  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_OUT_PARA_DCI_ADDR_LOW;
/* Define the union U_ISP_VIPROC_OUT_PARA_DCI_ADDR_HIGH */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int out_para_dci_addr_high : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VIPROC_OUT_PARA_DCI_ADDR_HIGH;

//#endif

/* Define the union U_ISP_BE_VERSION */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_patch             : 8   ; /* [7..0]  */
        unsigned int    isp_build             : 8   ; /* [15..8]  */
        unsigned int    isp_release           : 8   ; /* [23..16]  */
        unsigned int    isp_version           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_VERSION;

/* Define the union U_ISP_BE_DATE */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_date               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_DATE;
/* Define the union U_ISP_BE_FPGA_DATE */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpga_date          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_FPGA_DATE;
/* Define the union U_ISP_BE_MODULE_POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_sel            : 2   ; /* [1..0]  */
        unsigned int    isp_awb_sel           : 2   ; /* [3..2]  */
        unsigned int    isp_af_sel            : 2   ; /* [5..4]  */
        unsigned int    isp_dis_sel           : 1   ; /* [6]  */
        unsigned int    isp_clut_sel          : 1   ; /* [7]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_MODULE_POS;

/* Define the union U_ISP_BE_FSTART_DELAY */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_delay              : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_FSTART_DELAY;
/* Define the union U_ISP_BE_INPUT_MUX */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_input0_sel        : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    isp_input1_sel        : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    isp_input2_sel        : 2   ; /* [9..8]  */
        unsigned int    reserved_2            : 2   ; /* [11..10]  */
        unsigned int    isp_input3_sel        : 2   ; /* [13..12]  */
        unsigned int    reserved_3            : 2   ; /* [15..14]  */
        unsigned int    isp_input4_sel        : 1   ; /* [16]  */
        unsigned int    reserved_4            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_INPUT_MUX;

/* Define the union U_ISP_BE_SYS_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_stt_en            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_SYS_CTRL;

/* Define the union U_ISP_BE_USER_DEFINE0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_user_define0       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_USER_DEFINE0;
/* Define the union U_ISP_BE_USER_DEFINE1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_user_define1       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_USER_DEFINE1;
/* Define the union U_ISP_BE_STARTUP */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fcnt               : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_STARTUP;
/* Define the union U_ISP_BE_FORMAT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_format            : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BE_FORMAT;

/* Define the union U_ISP_CLIP_Y_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clip_y_min        : 16  ; /* [15..0]  */
        unsigned int    isp_clip_y_max        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLIP_Y_CFG;

/* Define the union U_ISP_CLIP_C_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clip_c_min        : 16  ; /* [15..0]  */
        unsigned int    isp_clip_c_max        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLIP_C_CFG;

/* Define the union U_ISP_CSC_SUM_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_csc_sum_en        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_SUM_CFG;

/* Define the union U_ISP_YUV444_SUM_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_yuv444_sum_en     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV444_SUM_CFG;

/* Define the union U_ISP_YUV422_SUM_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_yuv422_sum_en     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV422_SUM_CFG;

/* Define the union U_ISP_WDR_SUM_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_sum_en        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_SUM_CFG;

/* Define the union U_ISP_DEMOSAIC_SUM_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_sum_en   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_SUM_CFG;

/* Define the union U_ISP_DMNR_DITHER */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dmnr_dither_en    : 1   ; /* [0]  */
        unsigned int    isp_dmnr_dither_round : 2   ; /* [2..1]  */
        unsigned int    isp_dmnr_dither_spatial_mode : 1   ; /* [3]  */
        unsigned int    isp_dmnr_dither_out_bits : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DMNR_DITHER;

/* Define the union U_ISP_ACM_DITHER */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_acm_dither_en     : 1   ; /* [0]  */
        unsigned int    isp_acm_dither_round  : 2   ; /* [2..1]  */
        unsigned int    isp_acm_dither_spatial_mode : 1   ; /* [3]  */
        unsigned int    isp_acm_dither_out_bits : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_ACM_DITHER;

/* Define the union U_ISP_DRC_DITHER */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_dither_en     : 1   ; /* [0]  */
        unsigned int    isp_drc_dither_round  : 2   ; /* [2..1]  */
        unsigned int    isp_drc_dither_spatial_mode : 1   ; /* [3]  */
        unsigned int    isp_drc_dither_out_bits : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DITHER;

/* Define the union U_ISP_SQRT1_DITHER */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sqrt1_dither_en   : 1   ; /* [0]  */
        unsigned int    isp_sqrt1_dither_round : 2   ; /* [2..1]  */
        unsigned int    isp_sqrt1_dither_spatial_mode : 1   ; /* [3]  */
        unsigned int    isp_sqrt1_dither_out_bits : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SQRT1_DITHER;

/* Define the union U_ISP_BLK_HBLANK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blk_b_hblank      : 16  ; /* [15..0]  */
        unsigned int    isp_blk_f_hblank      : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLK_HBLANK;

/* Define the union U_ISP_BLK_VBLANK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blk_b_vblank      : 16  ; /* [15..0]  */
        unsigned int    isp_blk_f_vblank      : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLK_VBLANK;

/* Define the union U_ISP_Y_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_y_sum0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_Y_SUM0;
/* Define the union U_ISP_Y_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_y_sum1             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_Y_SUM1;
/* Define the union U_ISP_YUV444_Y_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_y_sum0      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV444_Y_SUM0;
/* Define the union U_ISP_YUV444_Y_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_y_sum1      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV444_Y_SUM1;
/* Define the union U_ISP_YUV444_U_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_u_sum0      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV444_U_SUM0;
/* Define the union U_ISP_YUV444_U_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_u_sum1      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV444_U_SUM1;
/* Define the union U_ISP_YUV444_V_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_v_sum0      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV444_V_SUM0;
/* Define the union U_ISP_YUV444_V_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv444_v_sum1      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV444_V_SUM1;
/* Define the union U_ISP_YUV422_Y_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_y_sum0      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV422_Y_SUM0;
/* Define the union U_ISP_YUV422_Y_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_y_sum1      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV422_Y_SUM1;
/* Define the union U_ISP_YUV422_C_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_c_sum0      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV422_C_SUM0;
/* Define the union U_ISP_YUV422_C_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_yuv422_c_sum1      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_YUV422_C_SUM1;
/* Define the union U_ISP_WDR0_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr0_sum0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR0_SUM0;
/* Define the union U_ISP_WDR0_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr0_sum1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR0_SUM1;
/* Define the union U_ISP_WDR1_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr1_sum0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR1_SUM0;
/* Define the union U_ISP_WDR1_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr1_sum1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR1_SUM1;
/* Define the union U_ISP_WDR2_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr2_sum0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR2_SUM0;
/* Define the union U_ISP_WDR2_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr2_sum1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR2_SUM1;
/* Define the union U_ISP_WDR3_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr3_sum0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR3_SUM0;
/* Define the union U_ISP_WDR3_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdr3_sum1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR3_SUM1;
/* Define the union U_ISP_DEMOSAIC_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_demosaic_sum0      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_SUM0;
/* Define the union U_ISP_DEMOSAIC_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_demosaic_sum1      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_SUM1;
/* Define the union U_ISP_BLC_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC_VERSION;
/* Define the union U_ISP_BLC_OFFSET1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc_ofsgr         : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_blc_ofsr          : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC_OFFSET1;

/* Define the union U_ISP_BLC_OFFSET2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_blc_ofsgb         : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_blc_ofsb          : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BLC_OFFSET2;

/* Define the union U_ISP_WB_BLC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_en_in          : 1   ; /* [0]  */
        unsigned int    isp_wb_en_out         : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_BLC_CFG;

/* Define the union U_ISP_WB_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_VERSION;
/* Define the union U_ISP_WB_GAIN1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_grgain         : 16  ; /* [15..0]  */
        unsigned int    isp_wb_rgain          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_GAIN1;

/* Define the union U_ISP_WB_GAIN2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_gbgain         : 16  ; /* [15..0]  */
        unsigned int    isp_wb_bgain          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_GAIN2;

/* Define the union U_ISP_WB_BLC_OFFSET1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_ofsgr          : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wb_ofsr           : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_BLC_OFFSET1;

/* Define the union U_ISP_WB_BLC_OFFSET2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_ofsgb          : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wb_ofsb           : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_BLC_OFFSET2;

/* Define the union U_ISP_WB_CLIP_VALUE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wb_clip_value     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WB_CLIP_VALUE;

/* Define the union U_ISP_DG_BLC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_en_in          : 1   ; /* [0]  */
        unsigned int    isp_dg_en_out         : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_BLC_CFG;

/* Define the union U_ISP_DG_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_VERSION;
/* Define the union U_ISP_DG_GAIN1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_grgain         : 16  ; /* [15..0]  */
        unsigned int    isp_dg_rgain          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_GAIN1;

/* Define the union U_ISP_DG_GAIN2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_gbgain         : 16  ; /* [15..0]  */
        unsigned int    isp_dg_bgain          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_GAIN2;

/* Define the union U_ISP_DG_BLC_OFFSET1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_ofsgr          : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_dg_ofsr           : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_BLC_OFFSET1;

/* Define the union U_ISP_DG_BLC_OFFSET2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_ofsgb          : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_dg_ofsb           : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_BLC_OFFSET2;

/* Define the union U_ISP_DG_CLIP_VALUE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dg_clip_value     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DG_CLIP_VALUE;

/* Define the union U_ISP_CC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_cc_prot_en        : 1   ; /* [1]  */
        unsigned int    isp_cc_colortone_en   : 1   ; /* [2]  */
        unsigned int    reserved_1            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_CFG;

/* Define the union U_ISP_CC_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_cc_version         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_VERSION;
/* Define the union U_ISP_CC_COEF0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_cc_coef00         : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    isp_cc_coef01         : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COEF0;

/* Define the union U_ISP_CC_COEF1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_cc_coef02         : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    isp_cc_coef10         : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COEF1;

/* Define the union U_ISP_CC_COEF2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_cc_coef11         : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    isp_cc_coef12         : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COEF2;

/* Define the union U_ISP_CC_COEF3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_cc_coef20         : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    isp_cc_coef21         : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COEF3;

/* Define the union U_ISP_CC_COEF4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_cc_coef22         : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COEF4;

/* Define the union U_ISP_CC_CC_THD0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_data_cc_thd0   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_CC_THD0;

/* Define the union U_ISP_CC_CC_THD1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_data_cc_thd1   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_CC_THD1;

/* Define the union U_ISP_CC_PROT_RATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_cc_prot_ratio  : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_PROT_RATIO;

/* Define the union U_ISP_CC_COLORTONE_RB_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_b_gain         : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_cc_r_gain         : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COLORTONE_RB_GAIN;

/* Define the union U_ISP_CC_COLORTONE_G_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_g_gain         : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_COLORTONE_G_GAIN;

/* Define the union U_ISP_CC_RR_THD0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_data_rr_thd0   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_RR_THD0;

/* Define the union U_ISP_CC_RR_THD1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_data_rr_thd1   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_RR_THD1;

/* Define the union U_ISP_CC_GG_THD0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_data_gg_thd0   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_GG_THD0;

/* Define the union U_ISP_CC_GG_THD1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_data_gg_thd1   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_GG_THD1;

/* Define the union U_ISP_CC_BB_THD0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_data_bb_thd0   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_BB_THD0;

/* Define the union U_ISP_CC_BB_THD1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_data_bb_thd1   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_BB_THD1;

/* Define the union U_ISP_CC_RGB_THD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_max_rgb_thd    : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_RGB_THD;

/* Define the union U_ISP_CC_RGB_RATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_rgb_prot_ratio : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_RGB_RATIO;

/* Define the union U_ISP_CC_IN_DC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_in_dc0         : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_IN_DC0;

/* Define the union U_ISP_CC_IN_DC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_in_dc1         : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_IN_DC1;

/* Define the union U_ISP_CC_IN_DC2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_in_dc2         : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_IN_DC2;

/* Define the union U_ISP_CC_OUT_DC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_out_dc0        : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_OUT_DC0;

/* Define the union U_ISP_CC_OUT_DC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_out_dc1        : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_OUT_DC1;

/* Define the union U_ISP_CC_OUT_DC2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_cc_out_dc2        : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CC_OUT_DC2;

/* Define the union U_ISP_CSC_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_csc_version        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_VERSION;
/* Define the union U_ISP_CSC_COEF0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_csc_coef00        : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    isp_csc_coef01        : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_COEF0;

/* Define the union U_ISP_CSC_COEF1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_csc_coef02        : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    isp_csc_coef10        : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_COEF1;

/* Define the union U_ISP_CSC_COEF2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_csc_coef11        : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    isp_csc_coef12        : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_COEF2;

/* Define the union U_ISP_CSC_COEF3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_csc_coef20        : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    isp_csc_coef21        : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_COEF3;

/* Define the union U_ISP_CSC_COEF4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_csc_coef22        : 15  ; /* [15..1]  */
        unsigned int    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_COEF4;

/* Define the union U_ISP_CSC_IN_DC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    isp_csc_in_dc0        : 11  ; /* [15..5]  */
        unsigned int    reserved_1            : 5   ; /* [20..16]  */
        unsigned int    isp_csc_in_dc1        : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_IN_DC0;

/* Define the union U_ISP_CSC_IN_DC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    isp_csc_in_dc2        : 11  ; /* [15..5]  */
        unsigned int    reserved_1            : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_IN_DC1;

/* Define the union U_ISP_CSC_OUT_DC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    isp_csc_out_dc0       : 11  ; /* [15..5]  */
        unsigned int    reserved_1            : 5   ; /* [20..16]  */
        unsigned int    isp_csc_out_dc1       : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_OUT_DC0;

/* Define the union U_ISP_CSC_OUT_DC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    isp_csc_out_dc2       : 11  ; /* [15..5]  */
        unsigned int    reserved_1            : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CSC_OUT_DC1;

/* Define the union U_ISP_MCDS_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_mcds_hcds_en      : 1   ; /* [1]  */
        unsigned int    isp_mcds_vcds_en      : 1   ; /* [2]  */
        unsigned int    reserved_1            : 1   ; /* [3]  */
        unsigned int    isp_mcds_midf_en      : 1   ; /* [4]  */
        unsigned int    reserved_2            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MCDS_CFG;

/* Define the union U_ISP_MCDS_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_mcds_version       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MCDS_VERSION;
/* Define the union U_ISP_HCDS_COEFH0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mcds_coefh1       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_mcds_coefh0       : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HCDS_COEFH0;

/* Define the union U_ISP_HCDS_COEFH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mcds_coefh3       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_mcds_coefh2       : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HCDS_COEFH1;

/* Define the union U_ISP_HCDS_COEFH2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mcds_coefh5       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_mcds_coefh4       : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HCDS_COEFH2;

/* Define the union U_ISP_HCDS_COEFH3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mcds_coefh7       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_mcds_coefh6       : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_HCDS_COEFH3;

/* Define the union U_ISP_VCDS_COEFV */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mcds_coefv0       : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    isp_mcds_coefv1       : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_VCDS_COEFV;

/* Define the union U_ISP_MCDS_PARA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_mcds_limit        : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 15  ; /* [23..9]  */
        unsigned int    isp_mcds_midf_bldr    : 5   ; /* [28..24]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_MCDS_PARA;

/* Define the union U_ISP_4DG_BLC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg_en_in         : 1   ; /* [0]  */
        unsigned int    isp_4dg_en_out        : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_BLC_CFG;

/* Define the union U_ISP_4DG_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_4dg_version        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_VERSION;
/* Define the union U_ISP_4DG_0_GAIN1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg0_grgain       : 16  ; /* [15..0]  */
        unsigned int    isp_4dg0_rgain        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_0_GAIN1;

/* Define the union U_ISP_4DG_0_GAIN2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg0_gbgain       : 16  ; /* [15..0]  */
        unsigned int    isp_4dg0_bgain        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_0_GAIN2;

/* Define the union U_ISP_4DG_0_BLC_OFFSET1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg0_ofsgr        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_4dg0_ofsr         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_0_BLC_OFFSET1;

/* Define the union U_ISP_4DG_0_BLC_OFFSET2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg0_ofsgb        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_4dg0_ofsb         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_0_BLC_OFFSET2;

/* Define the union U_ISP_4DG_1_GAIN1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg1_grgain       : 16  ; /* [15..0]  */
        unsigned int    isp_4dg1_rgain        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_1_GAIN1;

/* Define the union U_ISP_4DG_1_GAIN2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg1_gbgain       : 16  ; /* [15..0]  */
        unsigned int    isp_4dg1_bgain        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_1_GAIN2;

/* Define the union U_ISP_4DG_1_BLC_OFFSET1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg1_ofsgr        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_4dg1_ofsr         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_1_BLC_OFFSET1;

/* Define the union U_ISP_4DG_1_BLC_OFFSET2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg1_ofsgb        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_4dg1_ofsb         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_1_BLC_OFFSET2;

/* Define the union U_ISP_4DG_2_GAIN1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg2_grgain       : 16  ; /* [15..0]  */
        unsigned int    isp_4dg2_rgain        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_2_GAIN1;

/* Define the union U_ISP_4DG_2_GAIN2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg2_gbgain       : 16  ; /* [15..0]  */
        unsigned int    isp_4dg2_bgain        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_2_GAIN2;

/* Define the union U_ISP_4DG_2_BLC_OFFSET1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg2_ofsgr        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_4dg2_ofsr         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_2_BLC_OFFSET1;

/* Define the union U_ISP_4DG_2_BLC_OFFSET2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg2_ofsgb        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_4dg2_ofsb         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_2_BLC_OFFSET2;

/* Define the union U_ISP_4DG_3_GAIN1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg3_grgain       : 16  ; /* [15..0]  */
        unsigned int    isp_4dg3_rgain        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_3_GAIN1;

/* Define the union U_ISP_4DG_3_GAIN2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg3_gbgain       : 16  ; /* [15..0]  */
        unsigned int    isp_4dg3_bgain        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_3_GAIN2;

/* Define the union U_ISP_4DG_3_BLC_OFFSET1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg3_ofsgr        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_4dg3_ofsr         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_3_BLC_OFFSET1;

/* Define the union U_ISP_4DG_3_BLC_OFFSET2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg3_ofsgb        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_4dg3_ofsb         : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_3_BLC_OFFSET2;

/* Define the union U_ISP_4DG_0_CLIP_VALUE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg0_clip_value   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_0_CLIP_VALUE;

/* Define the union U_ISP_4DG_1_CLIP_VALUE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg1_clip_value   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_1_CLIP_VALUE;

/* Define the union U_ISP_4DG_2_CLIP_VALUE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg2_clip_value   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_2_CLIP_VALUE;

/* Define the union U_ISP_4DG_3_CLIP_VALUE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_4dg3_clip_value   : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_4DG_3_CLIP_VALUE;

/* Define the union U_ISP_AE_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_version         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_VERSION;
/* Define the union U_ISP_AE_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_hnum           : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_ae_vnum           : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_ZONE;

/* Define the union U_ISP_AE_SKIP_CRG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_skip_x         : 3   ; /* [2..0]  */
        unsigned int    isp_ae_offset_x       : 1   ; /* [3]  */
        unsigned int    isp_ae_skip_y         : 3   ; /* [6..4]  */
        unsigned int    isp_ae_offset_y       : 1   ; /* [7]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_SKIP_CRG;

/* Define the union U_ISP_AE_TOTAL_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_total_pixels   : 27  ; /* [26..0]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_STAT;

/* Define the union U_ISP_AE_COUNT_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_count_pixels   : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_COUNT_STAT;

/* Define the union U_ISP_AE_TOTAL_R_AVER */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_r_aver    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_R_AVER;
/* Define the union U_ISP_AE_TOTAL_GR_AVER */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_gr_aver   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_GR_AVER;
/* Define the union U_ISP_AE_TOTAL_GB_AVER */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_gb_aver   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_GB_AVER;
/* Define the union U_ISP_AE_TOTAL_B_AVER */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_total_b_aver    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_TOTAL_B_AVER;
/* Define the union U_ISP_AE_HIST_HIGH */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_high       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_HIST_HIGH;
/* Define the union U_ISP_AE_BITMOVE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_bitmove        : 5   ; /* [4..0]  */
        unsigned int    isp_ae_hist_gamma_mode : 2   ; /* [6..5]  */
        unsigned int    isp_ae_aver_gamma_mode : 2   ; /* [8..7]  */
        unsigned int    isp_ae_blc_en         : 1   ; /* [9]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_BITMOVE;

/* Define the union U_ISP_AE_OFFSET_R */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_offset_r       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_OFFSET_R;

/* Define the union U_ISP_AE_OFFSET_GR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_offset_gr      : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_OFFSET_GR;

/* Define the union U_ISP_AE_OFFSET_GB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_offset_gb      : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_OFFSET_GB;

/* Define the union U_ISP_AE_OFFSET_B */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_offset_b       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_OFFSET_B;

/* Define the union U_ISP_AE_GAMMA_LIMIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_gamma_limit    : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_GAMMA_LIMIT;

/* Define the union U_ISP_AE_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_lut_update     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_LUT_UPDATE;

/* Define the union U_ISP_AE_FOURPLANEMODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_fourplanemode  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_FOURPLANEMODE;

/* Define the union U_ISP_AE_CROP_POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_crop_pos_x     : 16  ; /* [15..0]  */
        unsigned int    isp_ae_crop_pos_y     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_CROP_POS;

/* Define the union U_ISP_AE_CROP_OUTSIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_crop_out_width : 16  ; /* [15..0]  */
        unsigned int    isp_ae_crop_out_height : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_CROP_OUTSIZE;

/* Define the union U_ISP_AE_HIST_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_HIST_WADDR;
/* Define the union U_ISP_AE_HIST_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_HIST_WDATA;
/* Define the union U_ISP_AE_HIST_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_raddr      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_HIST_RADDR;
/* Define the union U_ISP_AE_HIST_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_hist_rdata      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_HIST_RDATA;
/* Define the union U_ISP_AE_AVER_R_GR_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_AVER_R_GR_WADDR;
/* Define the union U_ISP_AE_AVER_R_GR_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_AVER_R_GR_WDATA;
/* Define the union U_ISP_AE_AVER_R_GR_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_r_gr_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_AVER_R_GR_RADDR;
/* Define the union U_ISP_AE_AVER_R_GR_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_r_gr_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_AVER_R_GR_RDATA;
/* Define the union U_ISP_AE_AVER_GB_B_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_AVER_GB_B_WADDR;
/* Define the union U_ISP_AE_AVER_GB_B_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_AVER_GB_B_WDATA;
/* Define the union U_ISP_AE_AVER_GB_B_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_gb_b_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_AVER_GB_B_RADDR;
/* Define the union U_ISP_AE_AVER_GB_B_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_aver_gb_b_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_AVER_GB_B_RDATA;
/* Define the union U_ISP_AE_WEIGHT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_wei_waddr       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_WEIGHT_WADDR;
/* Define the union U_ISP_AE_WEIGHT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ae_wei_wdata       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_WEIGHT_WDATA;
/* Define the union U_ISP_AE_WEIGHT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_WEIGHT_RADDR;
/* Define the union U_ISP_AE_WEIGHT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_WEIGHT_RDATA;
/* Define the union U_ISP_AWB_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_version        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_VERSION;
/* Define the union U_ISP_AWB_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_hnum          : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_awb_vnum          : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_ZONE;

/* Define the union U_ISP_AWB_BITMOVE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_bitmove       : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_BITMOVE;

/* Define the union U_ISP_AWB_THD_MIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_threshold_min : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_THD_MIN;

/* Define the union U_ISP_AWB_THD_MAX */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_threshold_max : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_THD_MAX;

/* Define the union U_ISP_AWB_CR_MM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_cr_ref_max    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_awb_cr_ref_min    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CR_MM;

/* Define the union U_ISP_AWB_CB_MM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_cb_ref_max    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_awb_cb_ref_min    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CB_MM;

/* Define the union U_ISP_AWB_OFFSET_COMP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_offset_comp   : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_OFFSET_COMP;

/* Define the union U_ISP_AWB_AVG_R */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_r         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_AVG_R;

/* Define the union U_ISP_AWB_AVG_G */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_g         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_AVG_G;

/* Define the union U_ISP_AWB_AVG_B */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_avg_b         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_AVG_B;

/* Define the union U_ISP_AWB_CNT_ALL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_count_all     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CNT_ALL;

/* Define the union U_ISP_AWB_HIST_BIN0_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_hist_bin0     : 16  ; /* [15..0]  */
        unsigned int    isp_awb_hist_bin1     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_HIST_BIN0_1;

/* Define the union U_ISP_AWB_HIST_BIN2_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_hist_bin2     : 16  ; /* [15..0]  */
        unsigned int    isp_awb_hist_bin3     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_HIST_BIN2_3;

/* Define the union U_ISP_AWB_ZONE_BIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_bin_num       : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_ZONE_BIN;

/* Define the union U_ISP_AWB_HIST_WEIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_hist_weight_b : 8   ; /* [7..0]  */
        unsigned int    isp_awb_hist_weight_g : 8   ; /* [15..8]  */
        unsigned int    isp_awb_hist_weight_r : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_HIST_WEIGHT;

/* Define the union U_ISP_AWB_CROP_POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_crop_pos_x    : 16  ; /* [15..0]  */
        unsigned int    isp_awb_crop_pos_y    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CROP_POS;

/* Define the union U_ISP_AWB_CROP_OUTSIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_crop_out_width : 16  ; /* [15..0]  */
        unsigned int    isp_awb_crop_out_height : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_CROP_OUTSIZE;

/* Define the union U_ISP_AWB_STAT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_STAT_WADDR;
/* Define the union U_ISP_AWB_STAT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_STAT_WDATA;
/* Define the union U_ISP_AWB_STAT_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_awb_stat_raddr    : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_STAT_RADDR;

/* Define the union U_ISP_AWB_STAT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_awb_stat_rdata     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AWB_STAT_RDATA;
/* Define the union U_ISP_AF_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_af_iir0_en0       : 1   ; /* [1]  */
        unsigned int    isp_af_iir0_en1       : 1   ; /* [2]  */
        unsigned int    isp_af_iir0_en2       : 1   ; /* [3]  */
        unsigned int    isp_af_iir1_en0       : 1   ; /* [4]  */
        unsigned int    isp_af_iir1_en1       : 1   ; /* [5]  */
        unsigned int    isp_af_iir1_en2       : 1   ; /* [6]  */
        unsigned int    isp_af_peak_mode      : 1   ; /* [7]  */
        unsigned int    isp_af_squ_mode       : 1   ; /* [8]  */
        unsigned int    isp_af_offset_en      : 1   ; /* [9]  */
        unsigned int    isp_af_crop_en        : 1   ; /* [10]  */
        unsigned int    isp_af_lpf_en         : 1   ; /* [11]  */
        unsigned int    isp_af_mean_en        : 1   ; /* [12]  */
        unsigned int    reserved_1            : 1   ; /* [13]  */
        unsigned int    isp_af_raw_mode       : 1   ; /* [14]  */
        unsigned int    isp_af_bayer_mode     : 2   ; /* [16..15]  */
        unsigned int    isp_af_iir0_ds_en     : 1   ; /* [17]  */
        unsigned int    isp_af_iir1_ds_en     : 1   ; /* [18]  */
        unsigned int    isp_af_fir0_lpf_en    : 1   ; /* [19]  */
        unsigned int    isp_af_fir1_lpf_en    : 1   ; /* [20]  */
        unsigned int    isp_af_iir0_ldg_en    : 1   ; /* [21]  */
        unsigned int    isp_af_iir1_ldg_en    : 1   ; /* [22]  */
        unsigned int    isp_af_fir0_ldg_en    : 1   ; /* [23]  */
        unsigned int    isp_af_fir1_ldg_en    : 1   ; /* [24]  */
        unsigned int    isp_af_gamma          : 3   ; /* [27..25]  */
        unsigned int    isp_af_gain_lmt       : 3   ; /* [30..28]  */
        unsigned int    reserved_2            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_CFG;

/* Define the union U_ISP_AF_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_version         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_VERSION;
/* Define the union U_ISP_AF_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_hnum           : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_af_vnum           : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_ZONE;

/* Define the union U_ISP_AF_CROP_START */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_pos_x          : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_af_pos_y          : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_CROP_START;

/* Define the union U_ISP_AF_CROP_SIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_crop_hsize     : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_af_crop_vsize     : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_CROP_SIZE;

/* Define the union U_ISP_AF_MEAN_THRES */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_mean_thres     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_MEAN_THRES;

/* Define the union U_ISP_AF_IIRG0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg0_0        : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_af_iirg0_1        : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRG0;

/* Define the union U_ISP_AF_IIRG1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg1_0        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af_iirg1_1        : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRG1;

/* Define the union U_ISP_AF_IIRG2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg2_0        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af_iirg2_1        : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRG2;

/* Define the union U_ISP_AF_IIRG3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg3_0        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af_iirg3_1        : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRG3;

/* Define the union U_ISP_AF_IIRG4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg4_0        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af_iirg4_1        : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRG4;

/* Define the union U_ISP_AF_IIRG5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg5_0        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af_iirg5_1        : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRG5;

/* Define the union U_ISP_AF_IIRG6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirg6_0        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_af_iirg6_1        : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRG6;

/* Define the union U_ISP_AF_IIRPL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirplg_0       : 8   ; /* [7..0]  */
        unsigned int    isp_af_iirpls_0       : 3   ; /* [10..8]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_af_iirplg_1       : 8   ; /* [23..16]  */
        unsigned int    isp_af_iirpls_1       : 3   ; /* [26..24]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRPL;

/* Define the union U_ISP_AF_SHIFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iirshift0_0    : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    isp_af_iirshift0_1    : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    isp_af_iirshift0_2    : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    isp_af_iirshift0_3    : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    isp_af_iirshift1_0    : 3   ; /* [18..16]  */
        unsigned int    reserved_4            : 1   ; /* [19]  */
        unsigned int    isp_af_iirshift1_1    : 3   ; /* [22..20]  */
        unsigned int    reserved_5            : 1   ; /* [23]  */
        unsigned int    isp_af_iirshift1_2    : 3   ; /* [26..24]  */
        unsigned int    reserved_6            : 1   ; /* [27]  */
        unsigned int    isp_af_iirshift1_3    : 3   ; /* [30..28]  */
        unsigned int    reserved_7            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_SHIFT;

/* Define the union U_ISP_AF_FIRH0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_firh0_0        : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_af_firh0_1        : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRH0;

/* Define the union U_ISP_AF_FIRH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_firh1_0        : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_af_firh1_1        : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRH1;

/* Define the union U_ISP_AF_FIRH2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_firh2_0        : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_af_firh2_1        : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRH2;

/* Define the union U_ISP_AF_FIRH3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_firh3_0        : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_af_firh3_1        : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRH3;

/* Define the union U_ISP_AF_FIRH4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_firh4_0        : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_af_firh4_1        : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRH4;

/* Define the union U_ISP_AF_ACC_SHIFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_acc_shift0_h   : 4   ; /* [3..0]  */
        unsigned int    isp_af_acc_shift1_h   : 4   ; /* [7..4]  */
        unsigned int    isp_af_acc_shift0_v   : 4   ; /* [11..8]  */
        unsigned int    isp_af_acc_shift1_v   : 4   ; /* [15..12]  */
        unsigned int    isp_af_acc_shift_y    : 4   ; /* [19..16]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_ACC_SHIFT;

/* Define the union U_ISP_AF_CNT_SHIFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_cnt_shift0_h   : 4   ; /* [3..0]  */
        unsigned int    isp_af_cnt_shift1_h   : 4   ; /* [7..4]  */
        unsigned int    isp_af_cnt_shift0_v   : 4   ; /* [11..8]  */
        unsigned int    isp_af_cnt_shift1_v   : 4   ; /* [15..12]  */
        unsigned int    isp_af_cnt_shift_y    : 4   ; /* [19..16]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_CNT_SHIFT;

/* Define the union U_ISP_AF_STAT_H1_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_h1_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT_H1_RADDR;
/* Define the union U_ISP_AF_STAT_H1_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_h1_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT_H1_RDATA;
/* Define the union U_ISP_AF_STAT_H2_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_h2_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT_H2_RADDR;
/* Define the union U_ISP_AF_STAT_H2_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_h2_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT_H2_RDATA;
/* Define the union U_ISP_AF_STAT_V1_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_v1_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT_V1_RADDR;
/* Define the union U_ISP_AF_STAT_V1_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_v1_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT_V1_RDATA;
/* Define the union U_ISP_AF_STAT_V2_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_v2_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT_V2_RADDR;
/* Define the union U_ISP_AF_STAT_V2_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_v2_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT_V2_RDATA;
/* Define the union U_ISP_AF_STAT_Y_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_y_raddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT_Y_RADDR;
/* Define the union U_ISP_AF_STAT_Y_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_af_stat_y_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_STAT_Y_RDATA;
/* Define the union U_ISP_AF_IIRTHRE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_thre0_l    : 8   ; /* [7..0]  */
        unsigned int    isp_af_iir_thre0_h    : 8   ; /* [15..8]  */
        unsigned int    isp_af_iir_thre1_l    : 8   ; /* [23..16]  */
        unsigned int    isp_af_iir_thre1_h    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRTHRE;

/* Define the union U_ISP_AF_IIRGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_gain0_l    : 8   ; /* [7..0]  */
        unsigned int    isp_af_iir_gain0_h    : 8   ; /* [15..8]  */
        unsigned int    isp_af_iir_gain1_l    : 8   ; /* [23..16]  */
        unsigned int    isp_af_iir_gain1_h    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRGAIN;

/* Define the union U_ISP_AF_IIRSLOPE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_slope0_l   : 4   ; /* [3..0]  */
        unsigned int    isp_af_iir_slope0_h   : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_af_iir_slope1_l   : 4   ; /* [19..16]  */
        unsigned int    isp_af_iir_slope1_h   : 4   ; /* [23..20]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRSLOPE;

/* Define the union U_ISP_AF_IIRDELAY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_delay0     : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_af_iir_delay1     : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRDELAY;

/* Define the union U_ISP_AF_FIRTHRE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_thre0_l    : 8   ; /* [7..0]  */
        unsigned int    isp_af_fir_thre0_h    : 8   ; /* [15..8]  */
        unsigned int    isp_af_fir_thre1_l    : 8   ; /* [23..16]  */
        unsigned int    isp_af_fir_thre1_h    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRTHRE;

/* Define the union U_ISP_AF_FIRGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_gain0_l    : 8   ; /* [7..0]  */
        unsigned int    isp_af_fir_gain0_h    : 8   ; /* [15..8]  */
        unsigned int    isp_af_fir_gain1_l    : 8   ; /* [23..16]  */
        unsigned int    isp_af_fir_gain1_h    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRGAIN;

/* Define the union U_ISP_AF_FIRSLOPE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_slope0_l   : 4   ; /* [3..0]  */
        unsigned int    isp_af_fir_slope0_h   : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_af_fir_slope1_l   : 4   ; /* [19..16]  */
        unsigned int    isp_af_fir_slope1_h   : 4   ; /* [23..20]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRSLOPE;

/* Define the union U_ISP_AF_IIRTHRE_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_thre0_c    : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_af_iir_thre1_c    : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRTHRE_CORING;

/* Define the union U_ISP_AF_IIRPEAK_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_peak0_c    : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_af_iir_peak1_c    : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRPEAK_CORING;

/* Define the union U_ISP_AF_IIRSLOPE_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_iir_slope0_c   : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_af_iir_slope1_c   : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_IIRSLOPE_CORING;

/* Define the union U_ISP_AF_FIRTHRE_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_thre0_c    : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_af_fir_thre1_c    : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRTHRE_CORING;

/* Define the union U_ISP_AF_FIRPEAK_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_peak0_c    : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_af_fir_peak1_c    : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRPEAK_CORING;

/* Define the union U_ISP_AF_FIRSLOPE_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_fir_slope0_c   : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_af_fir_slope1_c   : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_FIRSLOPE_CORING;

/* Define the union U_ISP_AF_HILIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_hilight        : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_HILIGHT;

/* Define the union U_ISP_AF_OFFSET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_af_offset_gr      : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_af_offset_gb      : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AF_OFFSET;

/* Define the union U_ISP_DIS_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_dis_blc_en        : 1   ; /* [1]  */
        unsigned int    reserved_1            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_CFG;

/* Define the union U_ISP_DIS_BLK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_blk_size_0    : 1   ; /* [0]  */
        unsigned int    isp_dis_blk_size      : 8   ; /* [8..1]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_dis_srch_range_0  : 1   ; /* [16]  */
        unsigned int    isp_dis_srch_range    : 7   ; /* [23..17]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_BLK;

/* Define the union U_ISP_DIS_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_version        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_VERSION;
/* Define the union U_ISP_DIS_V0POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_v0pos_hor     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_v0pos_ver     : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V0POS;

/* Define the union U_ISP_DIS_V4POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_v4pos_hor     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_v4pos_ver     : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V4POS;

/* Define the union U_ISP_DIS_V8POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_v8pos_hor     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_v8pos_ver     : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V8POS;

/* Define the union U_ISP_DIS_V0POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_v0pose_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_v0pose_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V0POSE;

/* Define the union U_ISP_DIS_V4POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_v4pose_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_v4pose_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V4POSE;

/* Define the union U_ISP_DIS_V8POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_v8pose_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_v8pose_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V8POSE;

/* Define the union U_ISP_DIS_H0POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_h0pos_hor     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_h0pos_ver     : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H0POS;

/* Define the union U_ISP_DIS_H4POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_h4pos_hor     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_h4pos_ver     : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H4POS;

/* Define the union U_ISP_DIS_H8POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_h8pos_hor     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_h8pos_ver     : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H8POS;

/* Define the union U_ISP_DIS_H0POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_h0pose_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_h0pose_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H0POSE;

/* Define the union U_ISP_DIS_H4POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_h4pose_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_h4pose_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H4POSE;

/* Define the union U_ISP_DIS_H8POSE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_h8pose_hor    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_dis_h8pose_ver    : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H8POSE;

/* Define the union U_ISP_DIS_RAW_LUMA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_raw_luma      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_RAW_LUMA;

/* Define the union U_ISP_DIS_GAMMA_EN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_gamma_en      : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_GAMMA_EN;

/* Define the union U_ISP_DIS_GAIN_LMT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_gain_lmt      : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_GAIN_LMT;

/* Define the union U_ISP_DIS_OFFSET_GR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_offset_gr     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_OFFSET_GR;

/* Define the union U_ISP_DIS_OFFSET_GB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_offset_gb     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_OFFSET_GB;

/* Define the union U_ISP_DIS_SCALE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dis_hor_scale     : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 6   ; /* [7..2]  */
        unsigned int    isp_dis_ver_scale     : 2   ; /* [9..8]  */
        unsigned int    reserved_1            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_SCALE;

/* Define the union U_ISP_DIS_H_STAT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H_STAT_WADDR;
/* Define the union U_ISP_DIS_H_STAT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H_STAT_WDATA;
/* Define the union U_ISP_DIS_H_STAT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_h_stat_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H_STAT_RADDR;
/* Define the union U_ISP_DIS_H_STAT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_h_stat_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_H_STAT_RDATA;
/* Define the union U_ISP_DIS_V_STAT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V_STAT_WADDR;
/* Define the union U_ISP_DIS_V_STAT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V_STAT_WDATA;
/* Define the union U_ISP_DIS_V_STAT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_v_stat_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V_STAT_RADDR;
/* Define the union U_ISP_DIS_V_STAT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dis_v_stat_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DIS_V_STAT_RDATA;
/* Define the union U_ISP_LA_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_version         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_VERSION;
/* Define the union U_ISP_LA_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_hnum           : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_la_vnum           : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_ZONE;

/* Define the union U_ISP_LA_BITMOVE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_bitmove        : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_la_gamma_en       : 1   ; /* [8]  */
        unsigned int    isp_la_blc_en         : 1   ; /* [9]  */
        unsigned int    reserved_1            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_BITMOVE;

/* Define the union U_ISP_LA_OFFSET_R */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_offset_r       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_OFFSET_R;

/* Define the union U_ISP_LA_OFFSET_GR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_offset_gr      : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_OFFSET_GR;

/* Define the union U_ISP_LA_OFFSET_GB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_offset_gb      : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_OFFSET_GB;

/* Define the union U_ISP_LA_OFFSET_B */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_offset_b       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_OFFSET_B;

/* Define the union U_ISP_LA_GAMMA_LIMIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_gamma_limit    : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_GAMMA_LIMIT;

/* Define the union U_ISP_LA_CROP_POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_crop_pos_x     : 16  ; /* [15..0]  */
        unsigned int    isp_la_crop_pos_y     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_CROP_POS;

/* Define the union U_ISP_LA_CROP_OUTSIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_la_crop_out_width : 16  ; /* [15..0]  */
        unsigned int    isp_la_crop_out_height : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_CROP_OUTSIZE;

/* Define the union U_ISP_LA_AVER_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_AVER_WADDR;
/* Define the union U_ISP_LA_AVER_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_AVER_WDATA;
/* Define the union U_ISP_LA_AVER_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_aver_raddr      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_AVER_RADDR;
/* Define the union U_ISP_LA_AVER_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_la_aver_rdata      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LA_AVER_RDATA;
/* Define the union U_ISP_FLICK_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 28  ; /* [27..0]  */
        unsigned int    isp_flick_mergeframe  : 3   ; /* [30..28]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_CTRL;

/* Define the union U_ISP_FLICK_F0_INBLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f0_inblc_gr : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_flick_f0_inblc_r  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_F0_INBLC0;

/* Define the union U_ISP_FLICK_F0_INBLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f0_inblc_b  : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_flick_f0_inblc_gb : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_F0_INBLC1;

/* Define the union U_ISP_FLICK_F1_INBLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f1_inblc_gr : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_flick_f1_inblc_r  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_F1_INBLC0;

/* Define the union U_ISP_FLICK_F1_INBLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f1_inblc_b  : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_flick_f1_inblc_gb : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_F1_INBLC1;

/* Define the union U_ISP_FLICK_F2_INBLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f2_inblc_gr : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_flick_f2_inblc_r  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_F2_INBLC0;

/* Define the union U_ISP_FLICK_F2_INBLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f2_inblc_b  : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_flick_f2_inblc_gb : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_F2_INBLC1;

/* Define the union U_ISP_FLICK_F3_INBLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f3_inblc_gr : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_flick_f3_inblc_r  : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_F3_INBLC0;

/* Define the union U_ISP_FLICK_F3_INBLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_f3_inblc_b  : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_flick_f3_inblc_gb : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_F3_INBLC1;

/* Define the union U_ISP_FLICK_EXPORATIO0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_exporatio1  : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_flick_exporatio0  : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_EXPORATIO0;

/* Define the union U_ISP_FLICK_EXPORATIO1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_exporatio2  : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_EXPORATIO1;

/* Define the union U_ISP_FLICK_OVERTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_overth      : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_OVERTH;

/* Define the union U_ISP_FLICK_OVERCOUNTTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_overcountth : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_OVERCOUNTTH;

/* Define the union U_ISP_FLICK_GAVG_PRE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gbavg_pre   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_flick_gravg_pre   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GAVG_PRE;

/* Define the union U_ISP_FLICK_COUNTOVER_PRE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_countover_pre : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_COUNTOVER_PRE;

/* Define the union U_ISP_FLICK_COUNTOVER_CUR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_countover_cur : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_COUNTOVER_CUR;

/* Define the union U_ISP_FLICK_GR_DIFF_CUR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gr_diff_cur : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GR_DIFF_CUR;

/* Define the union U_ISP_FLICK_GB_DIFF_CUR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gb_diff_cur : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GB_DIFF_CUR;

/* Define the union U_ISP_FLICK_GR_ABS_CUR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gr_abs_cur  : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GR_ABS_CUR;

/* Define the union U_ISP_FLICK_GB_ABS_CUR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gb_abs_cur  : 23  ; /* [22..0]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GB_ABS_CUR;

/* Define the union U_ISP_FLICK_GMEAN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GMEAN_WADDR;
/* Define the union U_ISP_FLICK_GMEAN_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GMEAN_WDATA;
/* Define the union U_ISP_FLICK_GMEAN_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gmean_raddr : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GMEAN_RADDR;

/* Define the union U_ISP_FLICK_GMEAN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_flick_gbmean_rdata : 11  ; /* [10..0]  */
        unsigned int    isp_flick_grmean_rdata : 11  ; /* [21..11]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FLICK_GMEAN_RDATA;

/* Define the union U_ISP_LSC_WINNUM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_numh          : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_lsc_numv          : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINNUM;

/* Define the union U_ISP_LSC_WINX_0_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_deltax          : 13  ; /* [12..0]  */
        unsigned int    reserved_0              : 3   ; /* [15..13]  */
        unsigned int    isp_lsc_invx            : 12  ; /* [27..16]  */
        unsigned int    reserved_1              : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX_0_23;

/* Define the union U_ISP_LSC_WIDTH_OFFSET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_width_offset  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WIDTH_OFFSET;

/* Define the union U_ISP_LSC_MESH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_mesh_str      : 16  ; /* [15..0]  */
        unsigned int    isp_lsc_mesh_scale    : 3   ; /* [18..16]  */
        unsigned int    reserved_0            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_MESH;

/* Define the union U_ISP_LSC_WEIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_mesh_weight   : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WEIGHT;

/* Define the union U_ISP_LSC_RGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc_rgain_waddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_RGAIN_WADDR;
/* Define the union U_ISP_LSC_RGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_rgain0_wdata  : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_rgain1_wdata  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_RGAIN_WDATA;

/* Define the union U_ISP_LSC_RGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc_rgain_raddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_RGAIN_RADDR;
/* Define the union U_ISP_LSC_RGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_rgain0_rdata  : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_rgain1_rdata  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_RGAIN_RDATA;

/* Define the union U_ISP_LSC_GRGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc_grgain_waddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GRGAIN_WADDR;
/* Define the union U_ISP_LSC_GRGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_grgain0_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_grgain1_wdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GRGAIN_WDATA;

/* Define the union U_ISP_LSC_GRGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc_grgain_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GRGAIN_RADDR;
/* Define the union U_ISP_LSC_GRGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_grgain0_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_grgain1_rdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GRGAIN_RDATA;

/* Define the union U_ISP_LSC_BGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc_bgain_waddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_BGAIN_WADDR;
/* Define the union U_ISP_LSC_BGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_bgain0_wdata  : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_bgain1_wdata  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_BGAIN_WDATA;

/* Define the union U_ISP_LSC_BGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc_bgain_raddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_BGAIN_RADDR;
/* Define the union U_ISP_LSC_BGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_bgain0_rdata  : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_bgain1_rdata  : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_BGAIN_RDATA;

/* Define the union U_ISP_LSC_GBGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc_gbgain_waddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GBGAIN_WADDR;
/* Define the union U_ISP_LSC_GBGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_gbgain0_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_gbgain1_wdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GBGAIN_WDATA;

/* Define the union U_ISP_LSC_GBGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_lsc_gbgain_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GBGAIN_RADDR;
/* Define the union U_ISP_LSC_GBGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_gbgain0_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_gbgain1_rdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GBGAIN_RDATA;

/* Define the union U_ISP_LSC_WINX_24_31 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_deltax          : 13  ; /* [12..0]  */
        unsigned int    reserved_0              : 3   ; /* [15..13]  */
        unsigned int    isp_lsc_invx            : 12  ; /* [27..16]  */
        unsigned int    reserved_1              : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINX_24_31;


/* Define the union U_ISP_LSC_WINY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_deltay       : 12  ; /* [11..0]  */
        unsigned int    reserved_0           : 4   ; /* [15..12]  */
        unsigned int    isp_lsc_invy         : 12  ; /* [27..16]  */
        unsigned int    reserved_1           : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_WINY;

/* Define the union U_ISP_LSC_BLCEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_blc_in_en     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    isp_lsc_blc_out_en    : 1   ; /* [8]  */
        unsigned int    reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_BLCEN;

/* Define the union U_ISP_LSC_BLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_blc_r         : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_lsc_blc_gr        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_BLC0;

/* Define the union U_ISP_LSC_BLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_blc_b         : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_lsc_blc_gb        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_BLC1;

/* Define the union U_ISP_LSC_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_lut_update    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_LUT_UPDATE;

/* Define the union U_ISP_GCAC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_en           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CFG;

/* Define the union U_ISP_GCAC_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gcac_version       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_VERSION;
/* Define the union U_ISP_GCAC_PARAMA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_red_a        : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_gcac_blue_a       : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_PARAMA;

/* Define the union U_ISP_GCAC_PARAMB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_red_b        : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_gcac_blue_b       : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_PARAMB;

/* Define the union U_ISP_GCAC_PARAMC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_red_c        : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_gcac_blue_c       : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_PARAMC;

/* Define the union U_ISP_GCAC_YNORM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_nf_y         : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    isp_gcac_ns_y         : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_YNORM;

/* Define the union U_ISP_GCAC_XNORM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_nf_x         : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    isp_gcac_ns_x         : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_XNORM;

/* Define the union U_ISP_GCAC_CNTSTART */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_cnt_start_h  : 16  ; /* [15..0]  */
        unsigned int    isp_gcac_cnt_start_v  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CNTSTART;

/* Define the union U_ISP_GCAC_CORSTART */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_cor_start_h  : 16  ; /* [15..0]  */
        unsigned int    isp_gcac_cor_start_v  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CORSTART;

/* Define the union U_ISP_GCAC_FILTEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_ver_filt_en  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_FILTEN;

/* Define the union U_ISP_GCAC_CHRVMODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_chrv_mode    : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CHRVMODE;

/* Define the union U_ISP_GCAC_CLIPMODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gcac_clip_mode_h  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 15  ; /* [15..1]  */
        unsigned int    isp_gcac_clip_mode_v  : 2   ; /* [17..16]  */
        unsigned int    reserved_1            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GCAC_CLIPMODE;

/* Define the union U_ISP_RLSC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_lsc_en       : 1   ; /* [0]  */
        unsigned int    isp_rlsc_crop_en      : 1   ; /* [1]  */
        unsigned int    isp_rlsc_blcoffsetout_en : 1   ; /* [2]  */
        unsigned int    isp_rlsc_blcoffsetin_en : 1   ; /* [3]  */
        unsigned int    isp_rlsc_nodenum      : 8   ; /* [11..4]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_CFG;

/* Define the union U_ISP_RLSC_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rlsc_version       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_VERSION;
/* Define the union U_ISP_RLSC_WIDTHOFFSET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_widthoffset  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_WIDTHOFFSET;

/* Define the union U_ISP_RLSC_POSR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_rx           : 16  ; /* [15..0]  */
        unsigned int    isp_rlsc_ry           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_POSR;

/* Define the union U_ISP_RLSC_POSGR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_grx          : 16  ; /* [15..0]  */
        unsigned int    isp_rlsc_gry          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_POSGR;

/* Define the union U_ISP_RLSC_POSGB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_gbx          : 16  ; /* [15..0]  */
        unsigned int    isp_rlsc_gby          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_POSGB;

/* Define the union U_ISP_RLSC_POSB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_bx           : 16  ; /* [15..0]  */
        unsigned int    isp_rlsc_by           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_POSB;

/* Define the union U_ISP_RLSC_OFFCENTERR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_offsetcenterr : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_OFFCENTERR;

/* Define the union U_ISP_RLSC_OFFCENTERGR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_offsetcentergr : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_OFFCENTERGR;

/* Define the union U_ISP_RLSC_OFFCENTERGB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_offsetcentergb : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_OFFCENTERGB;

/* Define the union U_ISP_RLSC_OFFCENTERB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_offsetcenterb : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_OFFCENTERB;

/* Define the union U_ISP_RLSC_VALIDRADIUS */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rlsc_validradius   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_VALIDRADIUS;
/* Define the union U_ISP_RLSC_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_gainstr      : 16  ; /* [15..0]  */
        unsigned int    isp_rlsc_gainscale    : 4   ; /* [19..16]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_GAIN;

/* Define the union U_ISP_RLSC_BLCOFFSETR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_blcoffsetr   : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_BLCOFFSETR;

/* Define the union U_ISP_RLSC_BLCOFFSETGR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_blcoffsetgr  : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_BLCOFFSETGR;

/* Define the union U_ISP_RLSC_BLCOFFSETGB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_blcoffsetgb  : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_BLCOFFSETGB;

/* Define the union U_ISP_RLSC_BLCOFFSETB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_blcoffsetb   : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_BLCOFFSETB;

/* Define the union U_ISP_RLSC_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_lut_update   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT_UPDATE;

/* Define the union U_ISP_RLSC_LUT0_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rlsc_lut0_waddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT0_WADDR;
/* Define the union U_ISP_RLSC_LUT0_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rlsc_lut0_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT0_WDATA;
/* Define the union U_ISP_RLSC_LUT0_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rlsc_lut0_raddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT0_RADDR;
/* Define the union U_ISP_RLSC_LUT0_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rlsc_lut0_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT0_RDATA;
/* Define the union U_ISP_RLSC_LUT1_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rlsc_lut1_waddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT1_WADDR;
/* Define the union U_ISP_RLSC_LUT1_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rlsc_lut1_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT1_WDATA;
/* Define the union U_ISP_RLSC_LUT1_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rlsc_lut1_raddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT1_RADDR;
/* Define the union U_ISP_RLSC_LUT1_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_rlsc_lut1_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT1_RDATA;
/* Define the union U_ISP_GE_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_ge_ge1_en         : 1   ; /* [1]  */
        unsigned int    isp_ge_ge2_en         : 1   ; /* [2]  */
        unsigned int    isp_ge_ge3_en         : 1   ; /* [3]  */
        unsigned int    reserved_1            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_CFG;

/* Define the union U_ISP_GE_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ge_version         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_VERSION;
/* Define the union U_ISP_GE0_CT_TH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge0_ct_th1     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE0_CT_TH1;

/* Define the union U_ISP_GE0_CT_TH2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge0_ct_th2     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE0_CT_TH2;

/* Define the union U_ISP_GE0_CT_TH3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge0_ct_th3     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE0_CT_TH3;

/* Define the union U_ISP_GE0_CT_SLOPE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge0_ct_slope1  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_ge_ge0_ct_slope2  : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE0_CT_SLOPE;

/* Define the union U_ISP_GE1_CT_TH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge1_ct_th1     : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE1_CT_TH1;

/* Define the union U_ISP_GE1_CT_TH2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge1_ct_th2     : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE1_CT_TH2;

/* Define the union U_ISP_GE1_CT_TH3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge1_ct_th3     : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE1_CT_TH3;

/* Define the union U_ISP_GE1_CT_SLOPE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge1_ct_slope1  : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_ge_ge1_ct_slope2  : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE1_CT_SLOPE;

/* Define the union U_ISP_GE2_CT_TH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge2_ct_th1     : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE2_CT_TH1;

/* Define the union U_ISP_GE2_CT_TH2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge2_ct_th2     : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE2_CT_TH2;

/* Define the union U_ISP_GE2_CT_TH3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge2_ct_th3     : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE2_CT_TH3;

/* Define the union U_ISP_GE2_CT_SLOPE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge2_ct_slope1  : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_ge_ge2_ct_slope2  : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE2_CT_SLOPE;

/* Define the union U_ISP_GE3_CT_TH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge3_ct_th1     : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE3_CT_TH1;

/* Define the union U_ISP_GE3_CT_TH2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge3_ct_th2     : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE3_CT_TH2;

/* Define the union U_ISP_GE3_CT_TH3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge3_ct_th3     : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE3_CT_TH3;

/* Define the union U_ISP_GE3_CT_SLOPE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_ge3_ct_slope1  : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_ge_ge3_ct_slope2  : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE3_CT_SLOPE;

/* Define the union U_ISP_GE_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_hnum           : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_ge_vnum           : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_ZONE;

/* Define the union U_ISP_GE_MODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_gr_en          : 1   ; /* [0]  */
        unsigned int    isp_ge_gb_en          : 1   ; /* [1]  */
        unsigned int    isp_ge_gr_gb_en       : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MODE;

/* Define the union U_ISP_GE_STRENGTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_strength       : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_STRENGTH;

/* Define the union U_ISP_GE_CROP_POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_crop_pos_x     : 16  ; /* [15..0]  */
        unsigned int    isp_ge_crop_pos_y     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_CROP_POS;

/* Define the union U_ISP_GE_CROP_OUTSIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_crop_out_width : 16  ; /* [15..0]  */
        unsigned int    isp_ge_crop_out_height : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_CROP_OUTSIZE;

/* Define the union U_ISP_GE_BIT_DEPTH_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_bit_depth_sel  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_BIT_DEPTH_SEL;

/* Define the union U_ISP_GE_MEM_AVER0_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER0_WADDR;
/* Define the union U_ISP_GE_MEM_AVER0_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER0_WDATA;
/* Define the union U_ISP_GE_MEM_AVER0_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ge_aver0_raddr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER0_RADDR;
/* Define the union U_ISP_GE_MEM_AVER0_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_mem_aver0_gb_rdata : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_ge_mem_aver0_gr_rdata : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER0_RDATA;

/* Define the union U_ISP_GE_MEM_AVER1_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER1_WADDR;
/* Define the union U_ISP_GE_MEM_AVER1_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER1_WDATA;
/* Define the union U_ISP_GE_MEM_AVER1_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ge_aver1_raddr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER1_RADDR;
/* Define the union U_ISP_GE_MEM_AVER1_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_mem_aver1_gb_rdata : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_ge_mem_aver1_gr_rdata : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER1_RDATA;

/* Define the union U_ISP_GE_MEM_AVER2_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER2_WADDR;
/* Define the union U_ISP_GE_MEM_AVER2_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER2_WDATA;
/* Define the union U_ISP_GE_MEM_AVER2_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ge_aver2_raddr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER2_RADDR;
/* Define the union U_ISP_GE_MEM_AVER2_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_mem_aver2_gb_rdata : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_ge_mem_aver2_gr_rdata : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER2_RDATA;

/* Define the union U_ISP_GE_MEM_AVER3_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER3_WADDR;
/* Define the union U_ISP_GE_MEM_AVER3_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER3_WDATA;
/* Define the union U_ISP_GE_MEM_AVER3_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ge_aver3_raddr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER3_RADDR;
/* Define the union U_ISP_GE_MEM_AVER3_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ge_mem_aver3_gb_rdata : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_ge_mem_aver3_gr_rdata : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GE_MEM_AVER3_RDATA;

/* Define the union U_ISP_FPN_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 9   ; /* [8..0]  */
        unsigned int    isp_fpn_line_frame    : 1   ; /* [9]  */
        unsigned int    isp_fpn_calib_offline : 1   ; /* [10]  */
        unsigned int    reserved_1            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CFG;

/* Define the union U_ISP_FPN_CALIB_START */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_calib_start   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CALIB_START;

/* Define the union U_ISP_FPN_CORR_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_correct0_en   : 1   ; /* [0]  */
        unsigned int    isp_fpn_correct1_en   : 1   ; /* [1]  */
        unsigned int    isp_fpn_correct2_en   : 1   ; /* [2]  */
        unsigned int    isp_fpn_correct3_en   : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CORR_CFG;

/* Define the union U_ISP_FPN_STAT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_busy          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    isp_fpn_vcnt          : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    isp_fpn_hcnt          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_STAT;

/* Define the union U_ISP_FPN_WHITE_LEVEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_white_level   : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_WHITE_LEVEL;

/* Define the union U_ISP_FPN_DIVCOEF */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_divcoef       : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_DIVCOEF;

/* Define the union U_ISP_FPN_FRAMELOG2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_framelog2     : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_FRAMELOG2;

/* Define the union U_ISP_FPN_SUM0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_sum0           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_SUM0;
/* Define the union U_ISP_FPN_SUM1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_sum1           : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_SUM1;
/* Define the union U_ISP_FPN_CORR0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_offset0       : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_fpn_strength0     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CORR0;

/* Define the union U_ISP_FPN_CORR1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_offset1       : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_fpn_strength1     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CORR1;

/* Define the union U_ISP_FPN_CORR2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_offset2       : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_fpn_strength2     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CORR2;

/* Define the union U_ISP_FPN_CORR3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_offset3       : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_fpn_strength3     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CORR3;

/* Define the union U_ISP_FPN_SHIFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_shift         : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_fpn_in_shift      : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    isp_fpn_out_shift     : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 4   ; /* [23..20]  */
        unsigned int    isp_fpn_frame_calib_shift : 4   ; /* [27..24]  */
        unsigned int    reserved_3            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_SHIFT;

/* Define the union U_ISP_FPN_MAX_O */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_max_o         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_MAX_O;

/* Define the union U_ISP_FPN_OVERFLOWTHR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_overflowthr   : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_OVERFLOWTHR;

/* Define the union U_ISP_FPN_BLACK_FRAME_OUT_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_data0_out_sel : 1   ; /* [0]  */
        unsigned int    isp_fpn_data1_out_sel : 1   ; /* [1]  */
        unsigned int    isp_fpn_data2_out_sel : 1   ; /* [2]  */
        unsigned int    isp_fpn_data3_out_sel : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_BLACK_FRAME_OUT_SEL;

/* Define the union U_ISP_FPN_OVERLAP_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_overlap_width_l : 16  ; /* [15..0]  */
        unsigned int    isp_fpn_overlap_width_r : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_OVERLAP_CFG;

/* Define the union U_ISP_FPN_CALIB_OFFLINE_SET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_calib_offline_set : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CALIB_OFFLINE_SET;

/* Define the union U_ISP_FPN_CALIB_STAT_CLEAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_calib_stat_clear : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_CALIB_STAT_CLEAR;

/* Define the union U_ISP_FPN_LINE_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_line_waddr    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_LINE_WADDR;

/* Define the union U_ISP_FPN_LINE_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_line_wdata     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_LINE_WDATA;
/* Define the union U_ISP_FPN_LINE_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_fpn_line_raddr    : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_LINE_RADDR;

/* Define the union U_ISP_FPN_LINE_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_line_rdata     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_LINE_RDATA;
/* Define the union U_ISP_DPC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_dpc_dpc_en1       : 1   ; /* [1]  */
        unsigned int    isp_dpc_dpc_en2       : 1   ; /* [2]  */
        unsigned int    isp_dpc_dpc_en3       : 1   ; /* [3]  */
        unsigned int    reserved_1            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_CFG;

/* Define the union U_ISP_DPC_ALPHA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_alpha1_g      : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_alpha1_rb     : 8   ; /* [15..8]  */
        unsigned int    isp_dpc_alpha0_g      : 8   ; /* [23..16]  */
        unsigned int    isp_dpc_alpha0_rb     : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_ALPHA;

/* Define the union U_ISP_DPC_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dpc_version        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_VERSION;
/* Define the union U_ISP_DPC_MODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_det_en        : 1   ; /* [0]  */
        unsigned int    isp_dpc_cor_en        : 1   ; /* [1]  */
        unsigned int    isp_dpc_six_det_en    : 1   ; /* [2]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    isp_dpc_grayscale_mode : 1   ; /* [4]  */
        unsigned int    isp_dpc_bpt_cor_en    : 1   ; /* [5]  */
        unsigned int    isp_dpc_dpt_det_sel   : 1   ; /* [6]  */
        unsigned int    isp_dpc_dp_hightlight_en : 1   ; /* [7]  */
        unsigned int    isp_dpc_ir_position   : 1   ; /* [8]  */
        unsigned int    isp_dpc_ir_channel    : 1   ; /* [9]  */
        unsigned int    reserved_1            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_MODE;

/* Define the union U_ISP_DPC_OUTPUT_MODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_stage1_incl_gr_center : 1   ; /* [0]  */
        unsigned int    isp_dpc_stage1_incl_rb_center : 1   ; /* [1]  */
        unsigned int    isp_dpc_stage1_incl_g_3x3 : 1   ; /* [2]  */
        unsigned int    isp_dpc_stage1_incl_rb_3x3 : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_OUTPUT_MODE;

/* Define the union U_ISP_DPC_SET_USE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_stage1_use_set1 : 1   ; /* [0]  */
        unsigned int    isp_dpc_stage1_use_set2 : 1   ; /* [1]  */
        unsigned int    isp_dpc_stage1_use_set3 : 1   ; /* [2]  */
        unsigned int    isp_dpc_stage1_use_fix_set : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_SET_USE;

/* Define the union U_ISP_DPC_METHODS_SET_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_pg_green1_enable : 1   ; /* [0]  */
        unsigned int    isp_dpc_lc_green1_enable : 1   ; /* [1]  */
        unsigned int    isp_dpc_ro_green1_enable : 1   ; /* [2]  */
        unsigned int    isp_dpc_rnd_green1_enable : 1   ; /* [3]  */
        unsigned int    isp_dpc_rg_green1_enable : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_dpc_pg_red_blue1_enable : 1   ; /* [8]  */
        unsigned int    isp_dpc_lc_red_blue1_enable : 1   ; /* [9]  */
        unsigned int    isp_dpc_ro_red_blue1_enable : 1   ; /* [10]  */
        unsigned int    isp_dpc_rnd_red_blue1_enable : 1   ; /* [11]  */
        unsigned int    isp_dpc_rg_red_blue1_enable : 1   ; /* [12]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_METHODS_SET_1;

/* Define the union U_ISP_DPC_METHODS_SET_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_pg_green2_enable : 1   ; /* [0]  */
        unsigned int    isp_dpc_lc_green2_enable : 1   ; /* [1]  */
        unsigned int    isp_dpc_ro_green2_enable : 1   ; /* [2]  */
        unsigned int    isp_dpc_rnd_green2_enable : 1   ; /* [3]  */
        unsigned int    isp_dpc_rg_green2_enable : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_dpc_pg_red_blue2_enable : 1   ; /* [8]  */
        unsigned int    isp_dpc_lc_red_blue2_enable : 1   ; /* [9]  */
        unsigned int    isp_dpc_ro_red_blue2_enable : 1   ; /* [10]  */
        unsigned int    isp_dpc_rnd_red_blue2_enable : 1   ; /* [11]  */
        unsigned int    isp_dpc_rg_red_blue2_enable : 1   ; /* [12]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_METHODS_SET_2;

/* Define the union U_ISP_DPC_METHODS_SET_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_pg_green3_enable : 1   ; /* [0]  */
        unsigned int    isp_dpc_lc_green3_enable : 1   ; /* [1]  */
        unsigned int    isp_dpc_ro_green3_enable : 1   ; /* [2]  */
        unsigned int    isp_dpc_rnd_green3_enable : 1   ; /* [3]  */
        unsigned int    isp_dpc_rg_green3_enable : 1   ; /* [4]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_dpc_pg_red_blue3_enable : 1   ; /* [8]  */
        unsigned int    isp_dpc_lc_red_blue3_enable : 1   ; /* [9]  */
        unsigned int    isp_dpc_ro_red_blue3_enable : 1   ; /* [10]  */
        unsigned int    isp_dpc_rnd_red_blue3_enable : 1   ; /* [11]  */
        unsigned int    isp_dpc_rg_red_blue3_enable : 1   ; /* [12]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_METHODS_SET_3;

/* Define the union U_ISP_DPC_LINE_THRESH_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_thr1_g   : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_line_thr1_rb  : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_THRESH_1;

/* Define the union U_ISP_DPC_LINE_MAD_FAC_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_mad_fac_1_g : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_line_mad_fac_1_rb : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_MAD_FAC_1;

/* Define the union U_ISP_DPC_PG_FAC_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_pg_fac_1_g    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_pg_fac_1_rb   : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_PG_FAC_1;

/* Define the union U_ISP_DPC_RND_THRESH_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_rnd_thr1_g    : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_rnd_thr1_rb   : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RND_THRESH_1;

/* Define the union U_ISP_DPC_RG_FAC_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_rg_fac_1_g    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_rg_fac_1_rb   : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RG_FAC_1;

/* Define the union U_ISP_DPC_LINE_THRESH_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_thr2_g   : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_line_thr2_rb  : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_THRESH_2;

/* Define the union U_ISP_DPC_LINE_MAD_FAC_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_mad_fac_2_g : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_line_mad_fac_2_rb : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_MAD_FAC_2;

/* Define the union U_ISP_DPC_PG_FAC_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_pg_fac_2_g    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_pg_fac_2_rb   : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_PG_FAC_2;

/* Define the union U_ISP_DPC_RND_THRESH_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_rnd_thr2_g    : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_rnd_thr2_rb   : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RND_THRESH_2;

/* Define the union U_ISP_DPC_RG_FAC_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_rg_fac_2_g    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_rg_fac_2_rb   : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RG_FAC_2;

/* Define the union U_ISP_DPC_LINE_THRESH_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_thr3_g   : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_line_thr3_rb  : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_THRESH_3;

/* Define the union U_ISP_DPC_LINE_MAD_FAC_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_mad_fac_3_g : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_line_mad_fac_3_rb : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_MAD_FAC_3;

/* Define the union U_ISP_DPC_PG_FAC_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_pg_fac_3_g    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_pg_fac_3_rb   : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_PG_FAC_3;

/* Define the union U_ISP_DPC_RND_THRESH_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_rnd_thr3_g    : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_rnd_thr3_rb   : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RND_THRESH_3;

/* Define the union U_ISP_DPC_RG_FAC_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_rg_fac_3_g    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_rg_fac_3_rb   : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RG_FAC_3;

/* Define the union U_ISP_DPC_RO_LIMITS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_ro_1_g        : 2   ; /* [1..0]  */
        unsigned int    isp_dpc_ro_1_rb       : 2   ; /* [3..2]  */
        unsigned int    isp_dpc_ro_2_g        : 2   ; /* [5..4]  */
        unsigned int    isp_dpc_ro_2_rb       : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_ro_3_g        : 2   ; /* [9..8]  */
        unsigned int    isp_dpc_ro_3_rb       : 2   ; /* [11..10]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RO_LIMITS;

/* Define the union U_ISP_DPC_RND_OFFS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_rnd_offs_1_g  : 2   ; /* [1..0]  */
        unsigned int    isp_dpc_rnd_offs_1_rb : 2   ; /* [3..2]  */
        unsigned int    isp_dpc_rnd_offs_2_g  : 2   ; /* [5..4]  */
        unsigned int    isp_dpc_rnd_offs_2_rb : 2   ; /* [7..6]  */
        unsigned int    isp_dpc_rnd_offs_3_g  : 2   ; /* [9..8]  */
        unsigned int    isp_dpc_rnd_offs_3_rb : 2   ; /* [11..10]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RND_OFFS;

/* Define the union U_ISP_DPC_BPT_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 4   ; /* [3..0]  */
        unsigned int    isp_dpc_bpt_use_set_1 : 1   ; /* [4]  */
        unsigned int    isp_dpc_bpt_use_set_2 : 1   ; /* [5]  */
        unsigned int    isp_dpc_bpt_use_set_3 : 1   ; /* [6]  */
        unsigned int    isp_dpc_bpt_use_fix_set : 1   ; /* [7]  */
        unsigned int    isp_dpc_bpt_incl_green_center : 1   ; /* [8]  */
        unsigned int    isp_dpc_bpt_incl_rb_center : 1   ; /* [9]  */
        unsigned int    isp_dpc_g_3x3         : 1   ; /* [10]  */
        unsigned int    isp_dpc_rb_3x3        : 1   ; /* [11]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    isp_dpc_bp_data       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_CTRL;

/* Define the union U_ISP_DPC_BPT_NUMBER */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_number    : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_NUMBER;

/* Define the union U_ISP_DPC_BPT_CALIB_NUMBER */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_calib_number : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_CALIB_NUMBER;

/* Define the union U_ISP_DPC_BPT_THRD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_dev_dead_thresh : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_abs_dead_thresh : 8   ; /* [15..8]  */
        unsigned int    isp_dpc_dev_hot_thresh : 8   ; /* [23..16]  */
        unsigned int    isp_dpc_abs_hot_thresh : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_THRD;

/* Define the union U_ISP_DPC_BPT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dpc_bpt_waddr      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_WADDR;
/* Define the union U_ISP_DPC_BPT_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_posx_wdata : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_dpc_bpt_posy_wdata : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_WDATA;

/* Define the union U_ISP_DPC_BPT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dpc_bpt_raddr      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_RADDR;
/* Define the union U_ISP_DPC_BPT_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_posx_rdata : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_dpc_bpt_posy_rdata : 13  ; /* [28..16]  */
        unsigned int    isp_dpc_bpt_rdata     : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_RDATA;

/* Define the union U_ISP_DPC_SOFT_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_ex_soft_thr_min : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_ex_soft_thr_max : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_SOFT_THR;

/* Define the union U_ISP_DPC_BHARDTHR_EN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_ex_hard_thr_en : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BHARDTHR_EN;

/* Define the union U_ISP_DPC_RAKERATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_ex_rake_ratio : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_RAKERATIO;

/* Define the union U_ISP_DPC_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_ex_lut_update : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LUT_UPDATE;

/* Define the union U_ISP_DPC_LCH_THR_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_aver_fac_1 : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_line_diff_thr_1 : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_dpc_line_std_thr_1 : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LCH_THR_1;

/* Define the union U_ISP_DPC_LCH_THR_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_aver_fac_2 : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_line_diff_thr_2 : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_dpc_line_std_thr_2 : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LCH_THR_2;

/* Define the union U_ISP_DPC_LCH_THR_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_aver_fac_3 : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_line_diff_thr_3 : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_dpc_line_std_thr_3 : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LCH_THR_3;

/* Define the union U_ISP_DPC_LCH_THR_4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_aver_fac_4 : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_line_diff_thr_4 : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_dpc_line_std_thr_4 : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LCH_THR_4;

/* Define the union U_ISP_DPC_LCH_THR_5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_aver_fac_5 : 8   ; /* [7..0]  */
        unsigned int    isp_dpc_line_diff_thr_5 : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_dpc_line_std_thr_5 : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LCH_THR_5;

/* Define the union U_ISP_DPC_LINE_KERDIFF_FAC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_line_kerdiff_fac : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_LINE_KERDIFF_FAC;

/* Define the union U_ISP_DPC_BLEND_MODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_blend_mode    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BLEND_MODE;

/* Define the union U_ISP_DPC_BIT_DEPTH_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bit_depth_sel : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BIT_DEPTH_SEL;

/* Define the union U_ISP_DEMOSAIC_CFG1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_ahd_en   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 15  ; /* [15..1]  */
        unsigned int    isp_demosaic_fcs_en   : 1   ; /* [16]  */
        unsigned int    isp_demosaic_local_cac_en : 1   ; /* [17]  */
        unsigned int    isp_demosaic_gcac_blend_select : 1   ; /* [18]  */
        unsigned int    isp_demosaic_fcr_en   : 1   ; /* [19]  */
        unsigned int    reserved_1            : 1   ; /* [20]  */
        unsigned int    isp_demosaic_de_fake_en : 1   ; /* [21]  */
        unsigned int    reserved_2            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CFG1;

/* Define the union U_ISP_DEMOSAIC_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_demosaic_version   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_VERSION;
/* Define the union U_ISP_DEMOSAIC_COEF0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_bld_limit2 : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_demosaic_bld_limit1 : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF0;

/* Define the union U_ISP_DEMOSAIC_COEF1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_bld_ratio2 : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_demosaic_bld_ratio1 : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF1;

/* Define the union U_ISP_DEMOSAIC_COEF2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_limit2 : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_demosaic_fcr_limit1 : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF2;

/* Define the union U_ISP_DEMOSAIC_COEF3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_ahd_par2 : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_demosaic_ahd_par1 : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF3;

/* Define the union U_ISP_DEMOSAIC_COEF4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_gain : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    isp_demosaic_fcr_thr  : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF4;

/* Define the union U_ISP_DEMOSAIC_COEF5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_hv_ratio : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF5;

/* Define the union U_ISP_DEMOSAIC_COEF6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_ratio : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_COEF6;

/* Define the union U_ISP_DEMOSAIC_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_hv_sel   : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_SEL;

/* Define the union U_ISP_DEMOSAIC_LCAC_CNT_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_r_counter_thr : 5   ; /* [4..0]  */
        unsigned int    isp_demosaic_b_counter_thr : 5   ; /* [9..5]  */
        unsigned int    isp_demosaic_g_counter_thr : 6   ; /* [15..10]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_LCAC_CNT_THR;

/* Define the union U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_r_luma_thr : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_demosaic_b_luma_thr : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR;

/* Define the union U_ISP_DEMOSAIC_LCAC_LUMA_G_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_g_luma_thr : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_LCAC_LUMA_G_THR;

/* Define the union U_ISP_DEMOSAIC_PURPLE_VAR_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_purple_var_thr : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_PURPLE_VAR_THR;

/* Define the union U_ISP_DEMOSAIC_FAKE_CR_VAR_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fake_cr_var_thr_low : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_demosaic_fake_cr_var_thr_high : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_FAKE_CR_VAR_THR;

/* Define the union U_ISP_DEMOSAIC_DEPURPLECTR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_depurplectrcb : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    isp_demosaic_depurplectrcr : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_DEPURPLECTR;

/* Define the union U_ISP_DEMOSAIC_LPF_COEF */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_lpf_f0   : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_demosaic_lpf_f1   : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_demosaic_lpf_f2   : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    isp_demosaic_lpf_f3   : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_LPF_COEF;

/* Define the union U_ISP_DEMOSAIC_G_CLIP_BIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_g_clip_sft_bit : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_G_CLIP_BIT;

/* Define the union U_ISP_DEMOSAIC_CBCRAVGTHLD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cbcr_avg_thld : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CBCRAVGTHLD;

/* Define the union U_ISP_DEMOSAIC_CC_HF_RATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cc_hf_min_ratio : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_demosaic_cc_hf_max_ratio : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CC_HF_RATIO;

/* Define the union U_ISP_DEMOSAIC_GVAR_BLEND_THLD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_varthrforblend : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_GVAR_BLEND_THLD;

/* Define the union U_ISP_DEMOSAIC_SATU_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_satu_thr : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_SATU_THR;

/* Define the union U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cbcr_ratio_low_limit : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_demosaic_cbcr_ratio_high_limit : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT;

/* Define the union U_ISP_DEMOSAIC_FCR_GRAY_RATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_gray_ratio : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_FCR_GRAY_RATIO;

/* Define the union U_ISP_DEMOSAIC_FCR_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_detg_sel : 4   ; /* [3..0]  */
        unsigned int    isp_demosaic_fcr_cmax_sel : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_FCR_SEL;

/* Define the union U_ISP_DEMOSAIC_CX_VAR_RATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cx_var_min_rate : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_demosaic_cx_var_max_rate : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CX_VAR_RATE;

/* Define the union U_ISP_DEMOSAIC_HF_INTP_TH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_hf_intp_th_low : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_demosaic_hf_intp_th_high : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HF_INTP_TH;

/* Define the union U_ISP_DEMOSAIC_HF_INTP_BLD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_hf_intp_bld_low : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_demosaic_hf_intp_bld_high : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HF_INTP_BLD;

/* Define the union U_ISP_DEMOSAIC_HF_INTP_TH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_hf_intp_th_low1 : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_demosaic_hf_intp_th_high1 : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HF_INTP_TH1;

/* Define the union U_ISP_DEMOSAIC_HF_INTP_RATIO0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_interp_ratio1 : 17  ; /* [16..0]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HF_INTP_RATIO0;

/* Define the union U_ISP_DEMOSAIC_HF_INTP_RATIO1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_interp_ratio2 : 17  ; /* [16..0]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_HF_INTP_RATIO1;

/* Define the union U_ISP_DEMOSAIC_CAC_CBCR_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cb_thr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_demosaic_luma_thr : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CAC_CBCR_THR;

/* Define the union U_ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cac_luma_high_cnt_thr : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR;

/* Define the union U_ISP_DEMOSAIC_CAC_CNT_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_cac_cb_cnt_low_thr : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_demosaic_cac_cb_cnt_high_thr : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CAC_CNT_CFG;

/* Define the union U_ISP_DEMOSAIC_CAC_BLD_AVG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaci_cac_bld_avg_cur : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_CAC_BLD_AVG;

/* Define the union U_ISP_DEMOSAIC_FCR_HF_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_demosaic_fcr_thresh1 : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_demosaic_fcr_thresh2 : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEMOSAIC_FCR_HF_THR;

/* Define the union U_ISP_SHARPEN_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_version    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_VERSION;
/* Define the union U_ISP_SHARPEN_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_benlumactrl : 1   ; /* [0]  */
        unsigned int    isp_sharpen_lumasrcsel : 1   ; /* [1]  */
        unsigned int    isp_sharpen_benshtvar_sel : 1   ; /* [2]  */
        unsigned int    isp_sharpen_benshtctrlbyvar : 1   ; /* [3]  */
        unsigned int    isp_sharpen_benskinctrl : 1   ; /* [4]  */
        unsigned int    isp_sharpen_skinsrcsel : 1   ; /* [5]  */
        unsigned int    isp_sharpen_benchrctrl : 1   ; /* [6]  */
        unsigned int    isp_sharpen_benchrsad : 1   ; /* [7]  */
        unsigned int    isp_sharpen_lpf_sel   : 1   ; /* [8]  */
        unsigned int    isp_sharpen_hsf_sel   : 2   ; /* [10..9]  */
        unsigned int    isp_sharpen_detailthd_sel : 2   ; /* [12..11]  */
        unsigned int    isp_sharpen_bendetailctrl : 1   ; /* [13]  */
        unsigned int    isp_sharpen_benshplowpow : 1   ; /* [14]  */
        unsigned int    isp_sharpen_benmarkedge : 1   ; /* [15]  */
        unsigned int    isp_sharpen_ben8dir_sel : 1   ; /* [16]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CTRL;

/* Define the union U_ISP_SHARPEN_MFTHD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_mfthdsftd : 4   ; /* [3..0]  */
        unsigned int    isp_sharpen_mfthdseld : 2   ; /* [5..4]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_sharpen_mfthdsftud : 4   ; /* [19..16]  */
        unsigned int    isp_sharpen_mfthdselud : 2   ; /* [21..20]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFTHD;

/* Define the union U_ISP_SHARPEN_HFTHD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_hfthdsftd : 4   ; /* [3..0]  */
        unsigned int    isp_sharpen_hfthdseld : 2   ; /* [5..4]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_sharpen_hfthdsftud : 4   ; /* [19..16]  */
        unsigned int    isp_sharpen_hfthdselud : 2   ; /* [21..20]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFTHD;

/* Define the union U_ISP_SHARPEN_DIRVAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dirvarsft : 4   ; /* [3..0]  */
        unsigned int    isp_sharpen_dirvarscale : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_dirrly0   : 7   ; /* [22..16]  */
        unsigned int    reserved_1            : 1   ; /* [23]  */
        unsigned int    isp_sharpen_dirrly1   : 7   ; /* [30..24]  */
        unsigned int    reserved_2            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_DIRVAR;

/* Define the union U_ISP_SHARPEN_DIRDIFF */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_dirdiffsft : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_sharpen_dirrt0    : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 3   ; /* [23..21]  */
        unsigned int    isp_sharpen_dirrt1    : 5   ; /* [28..24]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_DIRDIFF;

/* Define the union U_ISP_SHARPEN_LUMAWGT0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt0  : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_lumawgt1  : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_lumawgt2  : 8   ; /* [23..16]  */
        unsigned int    isp_sharpen_lumawgt3  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT0;

/* Define the union U_ISP_SHARPEN_LUMAWGT1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt4  : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_lumawgt5  : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_lumawgt6  : 8   ; /* [23..16]  */
        unsigned int    isp_sharpen_lumawgt7  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT1;

/* Define the union U_ISP_SHARPEN_LUMAWGT2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt8  : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_lumawgt9  : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_lumawgt10 : 8   ; /* [23..16]  */
        unsigned int    isp_sharpen_lumawgt11 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT2;

/* Define the union U_ISP_SHARPEN_LUMAWGT3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt12 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_lumawgt13 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_lumawgt14 : 8   ; /* [23..16]  */
        unsigned int    isp_sharpen_lumawgt15 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT3;

/* Define the union U_ISP_SHARPEN_LUMAWGT4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt16 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_lumawgt17 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_lumawgt18 : 8   ; /* [23..16]  */
        unsigned int    isp_sharpen_lumawgt19 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT4;

/* Define the union U_ISP_SHARPEN_LUMAWGT5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt20 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_lumawgt21 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_lumawgt22 : 8   ; /* [23..16]  */
        unsigned int    isp_sharpen_lumawgt23 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT5;

/* Define the union U_ISP_SHARPEN_LUMAWGT6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt24 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_lumawgt25 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_lumawgt26 : 8   ; /* [23..16]  */
        unsigned int    isp_sharpen_lumawgt27 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT6;

/* Define the union U_ISP_SHARPEN_LUMAWGT7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lumawgt28 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_lumawgt29 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_lumawgt30 : 8   ; /* [23..16]  */
        unsigned int    isp_sharpen_lumawgt31 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUMAWGT7;

/* Define the union U_ISP_SHARPEN_SHOOT_AMT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_selpixwgt : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_sharpen_oshtamt   : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    isp_sharpen_ushtamt   : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHOOT_AMT;

/* Define the union U_ISP_SHARPEN_SHOOT_MAXCHG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_omaxchg   : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_sharpen_umaxchg   : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHOOT_MAXCHG;

/* Define the union U_ISP_SHARPEN_SHTVAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_shtvarsft : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    isp_sharpen_shtvar5x5_sft : 4   ; /* [7..4]  */
        unsigned int    reserved_1            : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_shtbldrt  : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHTVAR;

/* Define the union U_ISP_SHARPEN_SHTNOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_shtnoisemin : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_shtnoisemax : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHTNOS;

/* Define the union U_ISP_SHARPEN_OSHTVAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_oshtvarthd0 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_oshtvarthd1 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_oshtvarwgt0 : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    isp_sharpen_oshtvarwgt1 : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_OSHTVAR;

/* Define the union U_ISP_SHARPEN_USHTVAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_ushtvarthd0 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_ushtvarthd1 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_ushtvarwgt0 : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    isp_sharpen_ushtvarwgt1 : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_USHTVAR;

/* Define the union U_ISP_SHARPEN_SHTVAR_MUL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_oshtvarmul : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_sharpen_ushtvarmul : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHTVAR_MUL;

/* Define the union U_ISP_SHARPEN_OSHTVARDIFF */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_oshtvardiffthd0 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_oshtvardiffthd1 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_oshtvardiffwgt0 : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    isp_sharpen_oshtvardiffwgt1 : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_OSHTVARDIFF;

/* Define the union U_ISP_SHARPEN_USHTVARDIFF */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_ushtvardiffthd0 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_ushtvardiffthd1 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_ushtvardiffwgt0 : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    isp_sharpen_ushtvardiffwgt1 : 7   ; /* [30..24]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_USHTVARDIFF;

/* Define the union U_ISP_SHARPEN_SHTVARDIFF_MUL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_oshtvardiffmul : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_sharpen_ushtvardiffmul : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHTVARDIFF_MUL;

/* Define the union U_ISP_SHARPEN_LMTMF0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lmtmf0    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_sharpen_lmtmf1    : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    isp_sharpen_lmtmf2    : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    isp_sharpen_lmtmf3    : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LMTMF0;

/* Define the union U_ISP_SHARPEN_LMTMF1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lmtmf4    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_sharpen_lmtmf5    : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    isp_sharpen_lmtmf6    : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    isp_sharpen_lmtmf7    : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LMTMF1;

/* Define the union U_ISP_SHARPEN_LMTHF0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lmthf0    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_sharpen_lmthf1    : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    isp_sharpen_lmthf2    : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    isp_sharpen_lmthf3    : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LMTHF0;

/* Define the union U_ISP_SHARPEN_LMTHF1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lmthf4    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_sharpen_lmthf5    : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    isp_sharpen_lmthf6    : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    isp_sharpen_lmthf7    : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LMTHF1;

/* Define the union U_ISP_SHARPEN_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_lut_update : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_LUT_UPDATE;

/* Define the union U_ISP_SHARPEN_MFGAIND_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mfgaind_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFGAIND_WADDR;
/* Define the union U_ISP_SHARPEN_MFGAIND_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mfgaind_wdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFGAIND_WDATA;
/* Define the union U_ISP_SHARPEN_MFGAIND_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mfgaind_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFGAIND_RADDR;
/* Define the union U_ISP_SHARPEN_MFGAIND_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mfgaind_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFGAIND_RDATA;
/* Define the union U_ISP_SHARPEN_MFGAINUD_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mfgainud_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFGAINUD_WADDR;
/* Define the union U_ISP_SHARPEN_MFGAINUD_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mfgainud_wdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFGAINUD_WDATA;
/* Define the union U_ISP_SHARPEN_MFGAINUD_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mfgainud_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFGAINUD_RADDR;
/* Define the union U_ISP_SHARPEN_MFGAINUD_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_mfgainud_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFGAINUD_RDATA;
/* Define the union U_ISP_SHARPEN_HFGAIND_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_hfgaind_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFGAIND_WADDR;
/* Define the union U_ISP_SHARPEN_HFGAIND_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_hfgaind_wdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFGAIND_WDATA;
/* Define the union U_ISP_SHARPEN_HFGAIND_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_hfgaind_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFGAIND_RADDR;
/* Define the union U_ISP_SHARPEN_HFGAIND_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_hfgaind_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFGAIND_RDATA;
/* Define the union U_ISP_SHARPEN_HFGAINUD_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_hfgainud_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFGAINUD_WADDR;
/* Define the union U_ISP_SHARPEN_HFGAINUD_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_hfgainud_wdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFGAINUD_WDATA;
/* Define the union U_ISP_SHARPEN_HFGAINUD_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_hfgainud_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFGAINUD_RADDR;
/* Define the union U_ISP_SHARPEN_HFGAINUD_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sharpen_hfgainud_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFGAINUD_RDATA;
/* Define the union U_ISP_SHARPEN_MARK_DATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_mark_udata : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_sharpen_mark_vdata : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MARK_DATA;

/* Define the union U_ISP_SHARPEN_SKIN_U */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skinminu  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_sharpen_skinmaxu  : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_U;

/* Define the union U_ISP_SHARPEN_SKIN_V */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skinminv  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_sharpen_skinmaxv  : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_V;

/* Define the union U_ISP_SHARPEN_SKIN_CNT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skincntthd0 : 4   ; /* [3..0]  */
        unsigned int    isp_sharpen_skincntthd1 : 4   ; /* [7..4]  */
        unsigned int    isp_sharpen_skincntmul : 5   ; /* [12..8]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_CNT;

/* Define the union U_ISP_SHARPEN_SKIN_EDGE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skinedgesft : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    isp_sharpen_skinedgemul : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_EDGE;

/* Define the union U_ISP_SHARPEN_SKIN_EDGETHD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skinedgethd0 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_skinedgethd1 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_skinedgewgt0 : 5   ; /* [20..16]  */
        unsigned int    reserved_0            : 3   ; /* [23..21]  */
        unsigned int    isp_sharpen_skinedgewgt1 : 5   ; /* [28..24]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_EDGETHD;

/* Define the union U_ISP_SHARPEN_SKIN_ACCUMTHD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skinaccumthd0 : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_sharpen_skinaccumthd1 : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_ACCUMTHD;

/* Define the union U_ISP_SHARPEN_SKIN_ACCUMWGT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_skinaccumwgt0 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_sharpen_skinaccumwgt1 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_sharpen_skinaccummul : 9   ; /* [24..16]  */
        unsigned int    reserved_2            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SKIN_ACCUMWGT;

/* Define the union U_ISP_SHARPEN_CHRR_VAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrrvarscale : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_sharpen_chrrvarsft : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    isp_sharpen_chrrscale : 9   ; /* [24..16]  */
        unsigned int    reserved_2            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRR_VAR;

/* Define the union U_ISP_SHARPEN_CHRR_ORI */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrrori0  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_chrrori1  : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRR_ORI;

/* Define the union U_ISP_SHARPEN_CHRR_SFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrrsft0  : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 5   ; /* [7..3]  */
        unsigned int    isp_sharpen_chrrsft1  : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 5   ; /* [15..11]  */
        unsigned int    isp_sharpen_chrrsft2  : 3   ; /* [18..16]  */
        unsigned int    reserved_2            : 5   ; /* [23..19]  */
        unsigned int    isp_sharpen_chrrsft3  : 3   ; /* [26..24]  */
        unsigned int    reserved_3            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRR_SFT;

/* Define the union U_ISP_SHARPEN_CHRR_THD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrrthd0  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_chrrthd1  : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRR_THD;

/* Define the union U_ISP_SHARPEN_CHRR_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrrgain0 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_sharpen_chrrgain1 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_sharpen_chrrmul   : 10  ; /* [25..16]  */
        unsigned int    reserved_2            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRR_GAIN;

/* Define the union U_ISP_SHARPEN_CHRG_VAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrgvarscale : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_sharpen_chrgvarsft : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    isp_sharpen_chrgscale : 9   ; /* [24..16]  */
        unsigned int    reserved_2            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRG_VAR;

/* Define the union U_ISP_SHARPEN_CHRG_ORI */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrgori0  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_chrgori1  : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRG_ORI;

/* Define the union U_ISP_SHARPEN_CHRG_SFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrgsft0  : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 5   ; /* [7..3]  */
        unsigned int    isp_sharpen_chrgsft1  : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 5   ; /* [15..11]  */
        unsigned int    isp_sharpen_chrgsft2  : 3   ; /* [18..16]  */
        unsigned int    reserved_2            : 5   ; /* [23..19]  */
        unsigned int    isp_sharpen_chrgsft3  : 3   ; /* [26..24]  */
        unsigned int    reserved_3            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRG_SFT;

/* Define the union U_ISP_SHARPEN_CHRG_THD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrgthd0  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_chrgthd1  : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRG_THD;

/* Define the union U_ISP_SHARPEN_CHRG_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrggain0 : 9   ; /* [8..0]  */
        unsigned int    isp_sharpen_chrggain1 : 9   ; /* [17..9]  */
        unsigned int    isp_sharpen_chrgmul   : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRG_GAIN;

/* Define the union U_ISP_SHARPEN_CHRB_VAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrbvarscale : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_sharpen_chrbvarsft : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    isp_sharpen_chrbscale : 9   ; /* [24..16]  */
        unsigned int    reserved_2            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRB_VAR;

/* Define the union U_ISP_SHARPEN_CHRB_ORI */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrbori0  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_chrbori1  : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRB_ORI;

/* Define the union U_ISP_SHARPEN_CHRB_SFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrbsft0  : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 5   ; /* [7..3]  */
        unsigned int    isp_sharpen_chrbsft1  : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 5   ; /* [15..11]  */
        unsigned int    isp_sharpen_chrbsft2  : 3   ; /* [18..16]  */
        unsigned int    reserved_2            : 5   ; /* [23..19]  */
        unsigned int    isp_sharpen_chrbsft3  : 3   ; /* [26..24]  */
        unsigned int    reserved_3            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRB_SFT;

/* Define the union U_ISP_SHARPEN_CHRB_THD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrbthd0  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_chrbthd1  : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRB_THD;

/* Define the union U_ISP_SHARPEN_CHRB_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_chrbgain0 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_sharpen_chrbgain1 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_sharpen_chrbmul   : 10  ; /* [25..16]  */
        unsigned int    reserved_2            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_CHRB_GAIN;

/* Define the union U_ISP_SHARPEN_GAIN_SFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_mfgain_sft : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 13  ; /* [15..3]  */
        unsigned int    isp_sharpen_hfgain_sft : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_GAIN_SFT;

/* Define the union U_ISP_SHARPEN_SHOOT_MAXGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_omaxgain  : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_sharpen_umaxgain  : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_SHOOT_MAXGAIN;

/* Define the union U_ISP_SHARPEN_DETAIL_MUL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_detl_oshtmul : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_sharpen_detl_ushtmul : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_DETAIL_MUL;

/* Define the union U_ISP_SHARPEN_OSHT_DETAIL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_osht_dtl_thd0 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_osht_dtl_thd1 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_osht_dtl_wgt : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_OSHT_DETAIL;

/* Define the union U_ISP_SHARPEN_USHT_DETAIL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_usht_dtl_thd0 : 8   ; /* [7..0]  */
        unsigned int    isp_sharpen_usht_dtl_thd1 : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_usht_dtl_wgt : 7   ; /* [22..16]  */
        unsigned int    reserved_0            : 1   ; /* [23]  */
        unsigned int    isp_sharpen_dtl_thdsft : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_USHT_DETAIL;

/* Define the union U_ISP_SHARPEN_MARK_SHIFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_mark_thd  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_sharpen_mark_thdsft : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MARK_SHIFT;

/* Define the union U_ISP_NDDM_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_cac_blend_en : 1   ; /* [0]  */
        unsigned int    isp_nddm_apt_interp_en : 1   ; /* [1]  */
        unsigned int    isp_nddm_alpha_filter_en : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_CFG;

/* Define the union U_ISP_NDDM_GF_TH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_th_low    : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_nddm_gf_th_high   : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_TH;

/* Define the union U_ISP_NDDM_GF_BLDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_bldr_gf_str  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    isp_nddm_bldr_cbcr    : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_BLDR;

/* Define the union U_ISP_NDDM_AWB_GF_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_awb_gf_gn_low : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    isp_nddm_awb_gf_gn_high : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    isp_nddm_awb_gf_gn_max : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_AWB_GF_CFG;

/* Define the union U_ISP_NDDM_USM_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_clip_usm     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_nddm_multi_mf     : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_CFG;

/* Define the union U_ISP_NDDM_USM_SATU */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_satu_th_low  : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    isp_nddm_satu_th_high : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    isp_nddm_satu_th_fix  : 9   ; /* [28..20]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_SATU;

/* Define the union U_ISP_NDDM_USM_GRAY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gray_th_low  : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    isp_nddm_gray_th_high : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    isp_nddm_gray_th_fix  : 9   ; /* [28..20]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_GRAY;

/* Define the union U_ISP_NDDM_APT_INTP_BLD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_bldr_gr_gb   : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_APT_INTP_BLD;

/* Define the union U_ISP_NDDM_DITHER_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_dith_mask    : 8   ; /* [7..0]  */
        unsigned int    isp_nddm_dith_ratio   : 8   ; /* [15..8]  */
        unsigned int    isp_nddm_dith_max     : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_DITHER_CFG;

/* Define the union U_ISP_NDDM_FCR_LIMIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_fcr_limit_low : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_nddm_fcr_limit_high : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_FCR_LIMIT;

/* Define the union U_ISP_NDDM_FCR_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_fcr_gf_gain  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    isp_nddm_fcr_det_low  : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_FCR_GAIN;

/* Define the union U_ISP_NDDM_FCR_SCALE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_scale        : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_FCR_SCALE;

/* Define the union U_ISP_NDDM_DM_BLDRATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_cac_blend_rate : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_DM_BLDRATE;

/* Define the union U_ISP_NDDM_GF_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_lut_update : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT_UPDATE;

/* Define the union U_ISP_NDDM_USM_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_usm_lut_update : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT_UPDATE;

/* Define the union U_ISP_NDDM_USM_GRAY_TH_FIX2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gray_th_fix2 : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_GRAY_TH_FIX2;

/* Define the union U_ISP_NDDM_APT_INTP_CLIP2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_clip_adjust_max : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_nddm_clip_delta_gain : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_APT_INTP_CLIP2;

/* Define the union U_ISP_NDDM_APT_AVG_CFG_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_clip_delta_intp_low : 12  ; /* [11..0]  */
        unsigned int    isp_nddm_clip_delta_intp_high : 12  ; /* [23..12]  */
        unsigned int    isp_nddm_filter_str_intp : 5   ; /* [28..24]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_APT_AVG_CFG_0;

/* Define the union U_ISP_NDDM_APT_AVG_CFG_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_clip_delta_filt_low : 12  ; /* [11..0]  */
        unsigned int    isp_nddm_clip_delta_filt_high : 12  ; /* [23..12]  */
        unsigned int    isp_nddm_filter_str_filt : 5   ; /* [28..24]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_APT_AVG_CFG_1;

/* Define the union U_ISP_NDDM_BLDR_ALPHA_GRAY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_bldr_gray    : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_BLDR_ALPHA_GRAY;

/* Define the union U_ISP_NDDM_SHT_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_sht_ctrl_gain : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 12  ; /* [15..4]  */
        unsigned int    isp_nddm_sht_ctrl_th  : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_SHT_CTRL;

/* Define the union U_ISP_NDDM_SHT_CLIP_R */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_clip_r_ud_sht : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_nddm_clip_r_ov_sht : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_SHT_CLIP_R;

/* Define the union U_ISP_NDDM_SHT_CLIP_B */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_clip_b_ud_sht : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_nddm_clip_b_ov_sht : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_SHT_CLIP_B;

/* Define the union U_ISP_NDDM_MULTI_MF_RB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_multi_mf_r   : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_nddm_multi_mf_b   : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_MULTI_MF_RB;

/* Define the union U_ISP_NDDM_USM_SATU_R */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_satu_r_th_low : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    isp_nddm_satu_r_th_high : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    isp_nddm_satu_r_th_fix : 9   ; /* [28..20]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_SATU_R;

/* Define the union U_ISP_NDDM_USM_SATU_B */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_satu_b_th_low : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 1   ; /* [9]  */
        unsigned int    isp_nddm_satu_b_th_high : 9   ; /* [18..10]  */
        unsigned int    reserved_1            : 1   ; /* [19]  */
        unsigned int    isp_nddm_satu_b_th_fix : 9   ; /* [28..20]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_SATU_B;

/* Define the union U_ISP_NDDM_SATU_FIX_EHCY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_satu_fix_ehcy : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_SATU_FIX_EHCY;

/* Define the union U_ISP_NDDM_GF_LUT_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_lut_waddr : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT_WADDR;

/* Define the union U_ISP_NDDM_GF_LUT_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_lut_wdata : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT_WDATA;

/* Define the union U_ISP_NDDM_GF_LUT_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_lut_raddr : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT_RADDR;

/* Define the union U_ISP_NDDM_GF_LUT_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gf_lut_rdata : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT_RDATA;

/* Define the union U_ISP_NDDM_USM_LUT_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_usm_lut_waddr : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT_WADDR;

/* Define the union U_ISP_NDDM_USM_LUT_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_usm_lut_wdata : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT_WDATA;

/* Define the union U_ISP_NDDM_USM_LUT_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_usm_lut_raddr : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT_RADDR;

/* Define the union U_ISP_NDDM_USM_LUT_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_usm_lut_rdata : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT_RDATA;

/* Define the union U_ISP_BNR_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_bnr_mono_sensor   : 1   ; /* [1]  */
        unsigned int    reserved_1            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_CFG;

/* Define the union U_ISP_BNR_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_version        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_VERSION;
/* Define the union U_ISP_BNR_CRATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_ratio_r       : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_bnr_ratio_gr      : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    isp_bnr_ratio_gb      : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    isp_bnr_ratio_b       : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_CRATIO;

/* Define the union U_ISP_BNR_AMED */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_amed_mode_r   : 1   ; /* [0]  */
        unsigned int    isp_bnr_amed_mode_gr  : 1   ; /* [1]  */
        unsigned int    isp_bnr_amed_mode_gb  : 1   ; /* [2]  */
        unsigned int    isp_bnr_amed_mode_b   : 1   ; /* [3]  */
        unsigned int    isp_bnr_amed_lev_r    : 2   ; /* [5..4]  */
        unsigned int    isp_bnr_amed_lev_gr   : 2   ; /* [7..6]  */
        unsigned int    isp_bnr_amed_lev_gb   : 2   ; /* [9..8]  */
        unsigned int    isp_bnr_amed_lev_b    : 2   ; /* [11..10]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_AMED;

/* Define the union U_ISP_BNR_JNLM0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlm_coringhig : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_bnr_jnlm_gain     : 8   ; /* [23..16]  */
        unsigned int    isp_bnr_jnlm_sel      : 2   ; /* [25..24]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_JNLM0;

/* Define the union U_ISP_BNR_JNLM2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlm_maxwtcoef : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_bnr_jnlm_symcoef  : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_JNLM2;

/* Define the union U_ISP_BNR_CHROMA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_rlmt_blc      : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_bnr_rlmt_rgain    : 10  ; /* [17..8]  */
        unsigned int    reserved_1            : 2   ; /* [19..18]  */
        unsigned int    isp_bnr_rlmt_bgain    : 10  ; /* [29..20]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_CHROMA;

/* Define the union U_ISP_BNR_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lut_update    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LUT_UPDATE;

/* Define the union U_ISP_BNR_DETAIL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_de_negclip    : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    isp_bnr_de_posclip    : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    isp_bnr_de_blcvalue   : 12  ; /* [27..16]  */
        unsigned int    isp_bnr_de_enable     : 1   ; /* [28]  */
        unsigned int    reserved_2            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_DETAIL;

/* Define the union U_ISP_BNR_LPCTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5   ; /* [4..0]  */
        unsigned int    isp_bnr_skip_lev4_enable : 1   ; /* [5]  */
        unsigned int    isp_bnr_skip_lev3_enable : 1   ; /* [6]  */
        unsigned int    isp_bnr_skip_lev2_enable : 1   ; /* [7]  */
        unsigned int    isp_bnr_skip_lev1_enable : 1   ; /* [8]  */
        unsigned int    isp_bnr_trisad_enable : 1   ; /* [9]  */
        unsigned int    isp_bnr_skip_enable   : 1   ; /* [10]  */
        unsigned int    isp_bnr_medc_enable   : 1   ; /* [11]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LPCTRL;

/* Define the union U_ISP_BNR_JNLMGAIN0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlmgain_r0   : 25  ; /* [24..0]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_JNLMGAIN0;

/* Define the union U_ISP_BNR_JNLMGAIN1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlmgain_gr0  : 25  ; /* [24..0]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_JNLMGAIN1;

/* Define the union U_ISP_BNR_JNLMGAIN2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlmgain_gb0  : 25  ; /* [24..0]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_JNLMGAIN2;

/* Define the union U_ISP_BNR_JNLMGAIN3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlmgain_b0   : 25  ; /* [24..0]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_JNLMGAIN3;

/* Define the union U_ISP_BNR_JNLMGAIN4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlmgain_r1   : 25  ; /* [24..0]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_JNLMGAIN4;

/* Define the union U_ISP_BNR_JNLMGAIN5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlmgain_gr1  : 25  ; /* [24..0]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_JNLMGAIN5;

/* Define the union U_ISP_BNR_JNLMGAIN6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlmgain_gb1  : 25  ; /* [24..0]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_JNLMGAIN6;

/* Define the union U_ISP_BNR_JNLMGAIN7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_jnlmgain_b1   : 25  ; /* [24..0]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_JNLMGAIN7;

/* Define the union U_ISP_BNR_WTI0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_wti_dval_th   : 8   ; /* [7..0]  */
        unsigned int    isp_bnr_wti_sval_th   : 8   ; /* [15..8]  */
        unsigned int    isp_bnr_wti_denom_offset : 9   ; /* [24..16]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_WTI0;

/* Define the union U_ISP_BNR_WTI1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_wti_midcoef   : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_bnr_wti_maxcoef   : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_WTI1;

/* Define the union U_ISP_BNR_NPTHRESH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lmt_npthresh  : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_NPTHRESH;

/* Define the union U_ISP_BNR_LMT_EVEN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lmt_even_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LMT_EVEN_WADDR;
/* Define the union U_ISP_BNR_LMT_EVEN_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lmt_even_wdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LMT_EVEN_WDATA;
/* Define the union U_ISP_BNR_LMT_EVEN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lmt_even_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LMT_EVEN_RADDR;
/* Define the union U_ISP_BNR_LMT_EVEN_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lmt_even_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LMT_EVEN_RDATA;
/* Define the union U_ISP_BNR_LMT_ODD_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lmt_odd_waddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LMT_ODD_WADDR;
/* Define the union U_ISP_BNR_LMT_ODD_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lmt_odd_wdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LMT_ODD_WDATA;
/* Define the union U_ISP_BNR_LMT_ODD_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lmt_odd_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LMT_ODD_RADDR;
/* Define the union U_ISP_BNR_LMT_ODD_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lmt_odd_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LMT_ODD_RDATA;
/* Define the union U_ISP_BNR_COR_EVEN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_cor_even_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_COR_EVEN_WADDR;
/* Define the union U_ISP_BNR_COR_EVEN_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_cor_even_wdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_COR_EVEN_WDATA;
/* Define the union U_ISP_BNR_COR_EVEN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_cor_even_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_COR_EVEN_RADDR;
/* Define the union U_ISP_BNR_COR_EVEN_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_cor_even_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_COR_EVEN_RDATA;
/* Define the union U_ISP_BNR_COR_ODD_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_cor_odd_waddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_COR_ODD_WADDR;
/* Define the union U_ISP_BNR_COR_ODD_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_cor_odd_wdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_COR_ODD_WDATA;
/* Define the union U_ISP_BNR_COR_ODD_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_cor_odd_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_COR_ODD_RADDR;
/* Define the union U_ISP_BNR_COR_ODD_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_cor_odd_rdata  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_COR_ODD_RDATA;
/* Define the union U_ISP_BNR_LSC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_en        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_CFG;

/* Define the union U_ISP_BNR_LSC_WINNUM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_numh      : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 10  ; /* [15..6]  */
        unsigned int    isp_bnr_lsc_numv      : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_WINNUM;

/* Define the union U_ISP_BNR_LSC_WINX_0_23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_deltax      : 13  ; /* [12..0]  */
        unsigned int    reserved_0              : 3   ; /* [15..13]  */
        unsigned int    isp_bnr_lsc_invx        : 12  ; /* [27..16]  */
        unsigned int    reserved_1              : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_WINX_0_23;

/* Define the union U_ISP_BNR_LSC_WIDTH_OFFSET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_width_offset : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_WIDTH_OFFSET;

/* Define the union U_ISP_BNR_LSC_MESH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_meshscale : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    isp_bnr_lsc_nrratio   : 8   ; /* [11..4]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_MESH;

/* Define the union U_ISP_BNR_LSC_WEIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_mesh_weight : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_WEIGHT;

/* Define the union U_ISP_BNR_LSC_RGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lsc_rgain_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_RGAIN_WADDR;
/* Define the union U_ISP_BNR_LSC_RGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_rgain0_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_rgain1_wdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_RGAIN_WDATA;

/* Define the union U_ISP_BNR_LSC_RGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lsc_rgain_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_RGAIN_RADDR;
/* Define the union U_ISP_BNR_LSC_RGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_rgain0_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_rgain1_rdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_RGAIN_RDATA;

/* Define the union U_ISP_BNR_LSC_GRGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lsc_grgain_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_GRGAIN_WADDR;
/* Define the union U_ISP_BNR_LSC_GRGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_grgain0_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_grgain1_wdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_GRGAIN_WDATA;

/* Define the union U_ISP_BNR_LSC_GRGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lsc_grgain_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_GRGAIN_RADDR;
/* Define the union U_ISP_BNR_LSC_GRGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_grgain0_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_grgain1_rdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_GRGAIN_RDATA;

/* Define the union U_ISP_BNR_LSC_BGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lsc_bgain_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_BGAIN_WADDR;
/* Define the union U_ISP_BNR_LSC_BGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_bgain0_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_bgain1_wdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_BGAIN_WDATA;

/* Define the union U_ISP_BNR_LSC_BGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lsc_bgain_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_BGAIN_RADDR;
/* Define the union U_ISP_BNR_LSC_BGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_bgain0_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_bgain1_rdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_BGAIN_RDATA;

/* Define the union U_ISP_BNR_LSC_GBGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lsc_gbgain_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_GBGAIN_WADDR;
/* Define the union U_ISP_BNR_LSC_GBGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_gbgain0_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_gbgain1_wdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_GBGAIN_WDATA;

/* Define the union U_ISP_BNR_LSC_GBGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bnr_lsc_gbgain_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_GBGAIN_RADDR;
/* Define the union U_ISP_BNR_LSC_GBGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_gbgain0_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_gbgain1_rdata : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_GBGAIN_RDATA;

/* Define the union U_ISP_BNR_LSC_WINX_24_31 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_deltax      : 13  ; /* [12..0]  */
        unsigned int    reserved_0              : 3   ; /* [15..13]  */
        unsigned int    isp_bnr_lsc_invx        : 12  ; /* [27..16]  */
        unsigned int    reserved_1              : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_WINX_24_31;


/* Define the union U_ISP_BNR_LSC_WINY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_deltay   : 12  ; /* [11..0]  */
        unsigned int    reserved_0           : 4   ; /* [15..12]  */
        unsigned int    isp_bnr_lsc_invy     : 12  ; /* [27..16]  */
        unsigned int    reserved_1           : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_WINY;

/* Define the union U_ISP_BNR_LSC_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_lut_update : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_LUT_UPDATE;

/* Define the union U_ISP_WDR_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_fusion_bnr    : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    isp_wdr_fusionmode    : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    isp_wdr_bnr_nosmode   : 1   ; /* [8]  */
        unsigned int    reserved_2            : 3   ; /* [11..9]  */
        unsigned int    isp_wdr_bnr           : 1   ; /* [12]  */
        unsigned int    reserved_3            : 3   ; /* [15..13]  */
        unsigned int    isp_wdr_erosion_en    : 1   ; /* [16]  */
        unsigned int    reserved_4            : 3   ; /* [19..17]  */
        unsigned int    isp_wdr_mdt_en        : 1   ; /* [20]  */
        unsigned int    reserved_5            : 3   ; /* [23..21]  */
        unsigned int    isp_wdr_grayscale_mode : 1   ; /* [24]  */
        unsigned int    reserved_6            : 3   ; /* [27..25]  */
        unsigned int    isp_wdr_mergeframe    : 3   ; /* [30..28]  */
        unsigned int    reserved_7            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_CTRL;

/* Define the union U_ISP_WDR_BIT_DEPTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_bitdepth_invalid : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_BIT_DEPTH;

/* Define the union U_ISP_WDR_F0_INBLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f0_inblc_gr   : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wdr_f0_inblc_r    : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_F0_INBLC0;

/* Define the union U_ISP_WDR_F0_INBLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f0_inblc_b    : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wdr_f0_inblc_gb   : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_F0_INBLC1;

/* Define the union U_ISP_WDR_F1_INBLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f1_inblc_gr   : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wdr_f1_inblc_r    : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_F1_INBLC0;

/* Define the union U_ISP_WDR_F1_INBLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f1_inblc_b    : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wdr_f1_inblc_gb   : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_F1_INBLC1;

/* Define the union U_ISP_WDR_F2_INBLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f2_inblc_gr   : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wdr_f2_inblc_r    : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_F2_INBLC0;

/* Define the union U_ISP_WDR_F2_INBLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f2_inblc_b    : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wdr_f2_inblc_gb   : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_F2_INBLC1;

/* Define the union U_ISP_WDR_F3_INBLC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f3_inblc_gr   : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wdr_f3_inblc_r    : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_F3_INBLC0;

/* Define the union U_ISP_WDR_F3_INBLC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f3_inblc_b    : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_wdr_f3_inblc_gb   : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_F3_INBLC1;

/* Define the union U_ISP_WDR_OUTBLC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_outblc        : 20  ; /* [19..0]  */
        unsigned int    isp_wdr_bsaveblc      : 1   ; /* [20]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_OUTBLC;

/* Define the union U_ISP_WDR_EXPOVALUE0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_expovalue1    : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_wdr_expovalue0    : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_EXPOVALUE0;

/* Define the union U_ISP_WDR_EXPOVALUE1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_expovalue3    : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_wdr_expovalue2    : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_EXPOVALUE1;

/* Define the union U_ISP_WDR_EXPORRATIO0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_exporratio1   : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_wdr_exporratio0   : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_EXPORRATIO0;

/* Define the union U_ISP_WDR_EXPORRATIO1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_exporratio2   : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_EXPORRATIO1;

/* Define the union U_ISP_WDR_BLC_COMP0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_blc_comp0     : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_BLC_COMP0;

/* Define the union U_ISP_WDR_BLC_COMP1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_blc_comp1     : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_BLC_COMP1;

/* Define the union U_ISP_WDR_BLC_COMP2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_blc_comp2     : 26  ; /* [25..0]  */
        unsigned int    reserved_0            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_BLC_COMP2;

/* Define the union U_ISP_WDR_MAXRATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_maxratio      : 17  ; /* [16..0]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_MAXRATIO;

/* Define the union U_ISP_WDR_SATURATE_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_saturate_thr  : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_SATURATE_THR;

/* Define the union U_ISP_WDR_STILL_THR0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f1_still_thr  : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_wdr_f0_still_thr  : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_STILL_THR0;

/* Define the union U_ISP_WDR_STILL_THR1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_f2_still_thr  : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_STILL_THR1;

/* Define the union U_ISP_WDR_MDTLBLD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mdtlbld       : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_MDTLBLD;

/* Define the union U_ISP_WDR_MDT_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mdt_full_thr  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_wdr_mdt_still_thr : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_MDT_THR;

/* Define the union U_ISP_WDR_BNR_FULLMDT_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_bnr_fullmdt_thr : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_BNR_FULLMDT_THR;

/* Define the union U_ISP_WDR_MDT_NOISEFLOOR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mdt_nosfloor  : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_MDT_NOISEFLOOR;

/* Define the union U_ISP_WDR_MDTHR_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mdthr_hig_gain : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    isp_wdr_mdthr_low_gain : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_MDTHR_GAIN;

/* Define the union U_ISP_WDR_SHORTEXPO_CHK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_tnosfloor     : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_wdr_shortexpo_chk : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_SHORTEXPO_CHK;

/* Define the union U_ISP_WDR_NOSLUT129X8_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_noslut129x8_waddr : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSLUT129X8_WADDR;

/* Define the union U_ISP_WDR_NOSLUT129X8_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_noslut129x8_wdata : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSLUT129X8_WDATA;

/* Define the union U_ISP_WDR_NOSLUT129X8_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_noslut129x8_raddr : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSLUT129X8_RADDR;

/* Define the union U_ISP_WDR_NOSLUT129X8_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_noslut129x8_rdata : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSLUT129X8_RDATA;

/* Define the union U_ISP_WDR_MASK_SIMILAR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_mask_similar_cnt : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 13  ; /* [15..3]  */
        unsigned int    isp_wdr_mask_similar_thr : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_MASK_SIMILAR;

/* Define the union U_ISP_WDR_WGTIDX_BLENDRATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_bldrlhfidx    : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_WGTIDX_BLENDRATIO;

/* Define the union U_ISP_WDR_WGTIDX_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_long_thr      : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_wdr_short_thr     : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_WGTIDX_THR;

/* Define the union U_ISP_WDR_DFTWGT_FL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_dftwgt_fl     : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_DFTWGT_FL;

/* Define the union U_ISP_WDR_SENSOR_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_sqrt_dgain_g  : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 13  ; /* [15..3]  */
        unsigned int    isp_wdr_sqrt_again_g  : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_SENSOR_GAIN;

/* Define the union U_ISP_WDR_NOISEFLOOR0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_nosfloor_b    : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_wdr_nosfloor_r    : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOISEFLOOR0;

/* Define the union U_ISP_WDR_NOISEFLOOR1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_nosfloor_g    : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOISEFLOOR1;

/* Define the union U_ISP_WDR_MODELCOEF_GAIN0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_modelcoef_bgain : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_wdr_modelcoef_rgain : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_MODELCOEF_GAIN0;

/* Define the union U_ISP_WDR_MODELCOEF_GAIN1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_modelcoef_ggain : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_MODELCOEF_GAIN1;

/* Define the union U_ISP_WDR_NOS_NP_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_nos_np_thr    : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOS_NP_THR;

/* Define the union U_ISP_WDR_FULLMDT_SIGMA_WGT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_fullmdt_sigwgt : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_FULLMDT_SIGMA_WGT;

/* Define the union U_ISP_WDR_GSIGMA_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_gsigma_gain3  : 8   ; /* [7..0]  */
        unsigned int    isp_wdr_gsigma_gain2  : 8   ; /* [15..8]  */
        unsigned int    isp_wdr_gsigma_gain1  : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_GSIGMA_GAIN;

/* Define the union U_ISP_WDR_CSIGMA_GAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_csigma_gain3  : 8   ; /* [7..0]  */
        unsigned int    isp_wdr_csigma_gain2  : 8   ; /* [15..8]  */
        unsigned int    isp_wdr_csigma_gain1  : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_CSIGMA_GAIN;

/* Define the union U_ISP_WDR_2DNR_WEIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_2dnr_weightc2 : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    isp_wdr_2dnr_weightc1 : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    isp_wdr_2dnr_weightc0 : 3   ; /* [10..8]  */
        unsigned int    reserved_2            : 1   ; /* [11]  */
        unsigned int    isp_wdr_2dnr_weightg2 : 3   ; /* [14..12]  */
        unsigned int    reserved_3            : 1   ; /* [15]  */
        unsigned int    isp_wdr_2dnr_weightg1 : 3   ; /* [18..16]  */
        unsigned int    reserved_4            : 1   ; /* [19]  */
        unsigned int    isp_wdr_2dnr_weightg0 : 3   ; /* [22..20]  */
        unsigned int    reserved_5            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_2DNR_WEIGHT;

/* Define the union U_ISP_WDR_FUSION_RATIO_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_fusion_rlow_thr : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_wdr_fusion_rhig_thr : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_FUSION_RATIO_THR;

/* Define the union U_ISP_WDR_FUSION_THR0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_fusion_f1_thr : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_wdr_fusion_f0_thr : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_FUSION_THR0;

/* Define the union U_ISP_WDR_FUSION_THR1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_fusion_f3_thr : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    isp_wdr_fusion_f2_thr : 14  ; /* [29..16]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_FUSION_THR1;

/* Define the union U_ISP_WDR_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_129x8_lut_update : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_LUT_UPDATE;

/* Define the union U_ISP_DEHAZE_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_dehaze_version     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_VERSION;
/* Define the union U_ISP_DEHAZE_PRE_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_pre_lut_update : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRE_UPDATE;

/* Define the union U_ISP_DEHAZE_BLK_SIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_block_sizev : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_dehaze_block_sizeh : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_BLK_SIZE;

/* Define the union U_ISP_DEHAZE_BLK_SUM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_block_sum  : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_BLK_SUM;

/* Define the union U_ISP_DEHAZE_DC_SIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_dc_numv    : 5   ; /* [4..0]  */
        unsigned int    isp_dehaze_dc_numh    : 5   ; /* [9..5]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_DC_SIZE;

/* Define the union U_ISP_DEHAZE_X */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_phase_x    : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_X;

/* Define the union U_ISP_DEHAZE_Y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_phase_y    : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_Y;

/* Define the union U_ISP_DEHAZE_STAT_MODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_max_mode   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_STAT_MODE;

/* Define the union U_ISP_DEHAZE_NEG_MODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_neg_mode   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_NEG_MODE;

/* Define the union U_ISP_DEHAZE_AIR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_air_b      : 10  ; /* [9..0]  */
        unsigned int    isp_dehaze_air_g      : 10  ; /* [19..10]  */
        unsigned int    isp_dehaze_air_r      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_AIR;

/* Define the union U_ISP_DEHAZE_THLD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_thld       : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_THLD;

/* Define the union U_ISP_DEHAZE_GSTRTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_gstrth     : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_GSTRTH;

/* Define the union U_ISP_DEHAZE_BLTHLD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_blthld     : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_BLTHLD;

/* Define the union U_ISP_DEHAZE_STR_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_str_lut_update : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_STR_LUT_UPDATE;

/* Define the union U_ISP_DEHAZE_MINSTAT_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_minstat_waddr : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MINSTAT_WADDR;

/* Define the union U_ISP_DEHAZE_MINSTAT_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_minstat_wdata_l : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_dehaze_minstat_wdata_h : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MINSTAT_WDATA;

/* Define the union U_ISP_DEHAZE_MINSTAT_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_minstat_raddr : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MINSTAT_RADDR;

/* Define the union U_ISP_DEHAZE_MINSTAT_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_minstat_rdata_l : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_dehaze_minstat_rdata_h : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MINSTAT_RDATA;

/* Define the union U_ISP_DEHAZE_MAXSTAT_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_maxstat_waddr : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MAXSTAT_WADDR;

/* Define the union U_ISP_DEHAZE_MAXSTAT_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_maxstat_wdata : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MAXSTAT_WDATA;

/* Define the union U_ISP_DEHAZE_MAXSTAT_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_maxstat_raddr : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MAXSTAT_RADDR;

/* Define the union U_ISP_DEHAZE_MAXSTAT_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_maxstat_rdata : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_MAXSTAT_RDATA;

/* Define the union U_ISP_DEHAZE_PRESTAT_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_prestat_waddr : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRESTAT_WADDR;

/* Define the union U_ISP_DEHAZE_PRESTAT_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_prestat_wdata_l : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_dehaze_prestat_wdata_h : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRESTAT_WDATA;

/* Define the union U_ISP_DEHAZE_PRESTAT_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_prestat_raddr : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRESTAT_RADDR;

/* Define the union U_ISP_DEHAZE_PRESTAT_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_prestat_rdata_l : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_dehaze_prestat_rdata_h : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRESTAT_RDATA;

/* Define the union U_ISP_DEHAZE_LUT_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_waddr  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_LUT_WADDR;

/* Define the union U_ISP_DEHAZE_LUT_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_wdata  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_LUT_WDATA;

/* Define the union U_ISP_DEHAZE_LUT_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_raddr  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_LUT_RADDR;

/* Define the union U_ISP_DEHAZE_LUT_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_lut_rdata  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_LUT_RDATA;

/* Define the union U_ISP_PREGAMMA_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    reserved_1            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_CFG;

/* Define the union U_ISP_PREGAMMA_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_pregamma_version   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_VERSION;
/* Define the union U_ISP_PREGAMMA_BITW */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_pregamma_bitw_out : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    isp_pregamma_bitw_in  : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_BITW;

/* Define the union U_ISP_PREGAMMA_OFFSETR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_pregamma_offset_r : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_OFFSETR;

/* Define the union U_ISP_PREGAMMA_OFFSETGR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_pregamma_offset_gr : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_OFFSETGR;

/* Define the union U_ISP_PREGAMMA_OFFSETGB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_pregamma_offset_gb : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_OFFSETGB;

/* Define the union U_ISP_PREGAMMA_OFFSETB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_pregamma_offset_b : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_OFFSETB;

/* Define the union U_ISP_PREGAMMA_LUT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_pregamma_lut_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_LUT_WADDR;
/* Define the union U_ISP_PREGAMMA_LUT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_pregamma_lut_wdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_LUT_WDATA;
/* Define the union U_ISP_PREGAMMA_LUT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_pregamma_lut_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_LUT_RADDR;
/* Define the union U_ISP_PREGAMMA_LUT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_pregamma_lut_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_LUT_RDATA;
/* Define the union U_ISP_GAMMA_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    reserved_1            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_CFG;

/* Define the union U_ISP_GAMMA_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_version      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_VERSION;
/* Define the union U_ISP_GAMMA_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_lut_update  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_LUT_UPDATE;

/* Define the union U_ISP_GAMMA_LUT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_waddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_LUT_WADDR;
/* Define the union U_ISP_GAMMA_LUT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_LUT_WDATA;
/* Define the union U_ISP_GAMMA_LUT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_raddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_LUT_RADDR;
/* Define the union U_ISP_GAMMA_LUT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_gamma_lut_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_LUT_RDATA;
/* Define the union U_ISP_BCOM_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    reserved_1            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BCOM_CFG;

/* Define the union U_ISP_BCOM_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bcom_version       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BCOM_VERSION;
/* Define the union U_ISP_BCOM_ALPHA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bcom_alpha        : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BCOM_ALPHA;

/* Define the union U_ISP_BDEC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    reserved_1            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BDEC_CFG;

/* Define the union U_ISP_BDEC_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_bdec_version       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BDEC_VERSION;
/* Define the union U_ISP_BDEC_ALPHA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bdec_alpha        : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BDEC_ALPHA;

/* Define the union U_ISP_WDRSPLIT_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    reserved_1            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_CFG;

/* Define the union U_ISP_WDRSPLIT_BITW */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdrsplit_bitw_out : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    isp_wdrsplit_bitw_in  : 5   ; /* [20..16]  */
        unsigned int    reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_BITW;

/* Define the union U_ISP_WDRSPLIT_MODE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdrsplit_mode_out : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    isp_wdrsplit_mode_in  : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_MODE;

/* Define the union U_ISP_WDRSPLIT_OFFSETR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdrsplit_offset_r : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_OFFSETR;

/* Define the union U_ISP_WDRSPLIT_OFFSETGR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdrsplit_offset_gr : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_OFFSETGR;

/* Define the union U_ISP_WDRSPLIT_OFFSETGB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdrsplit_offset_gb : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_OFFSETGB;

/* Define the union U_ISP_WDRSPLIT_OFFSETB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdrsplit_offset_b : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_OFFSETB;

/* Define the union U_ISP_WDRSPLIT_BLC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdrsplit_blc      : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_BLC;

/* Define the union U_ISP_WDRSPLIT_LUT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdrsplit_lut_waddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_LUT_WADDR;
/* Define the union U_ISP_WDRSPLIT_LUT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdrsplit_lut_wdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_LUT_WDATA;
/* Define the union U_ISP_WDRSPLIT_LUT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdrsplit_lut_raddr : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_LUT_RADDR;
/* Define the union U_ISP_WDRSPLIT_LUT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_wdrsplit_lut_rdata : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_LUT_RDATA;
/* Define the union U_ISP_CA_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_satadj_en      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    isp_ca_skinproc_en    : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    isp_ca_llhcproc_en    : 1   ; /* [8]  */
        unsigned int    reserved_2            : 3   ; /* [11..9]  */
        unsigned int    isp_ca_cp_en          : 1   ; /* [12]  */
        unsigned int    reserved_3            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_CTRL;

/* Define the union U_ISP_CA_LUMATH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_lumath_high    : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_lumath_low     : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LUMATH;

/* Define the union U_ISP_CA_DARKCHROMA_TH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_darkchromath_high : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_darkchromath_low : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_DARKCHROMA_TH;

/* Define the union U_ISP_CA_SDARKCHROMA_TH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_sdarkchromath_high : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_sdarkchromath_low : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SDARKCHROMA_TH;

/* Define the union U_ISP_CA_LLHC_RATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_lumaratio_high : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_ca_lumaratio_low  : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LLHC_RATIO;

/* Define the union U_ISP_CA_ISORATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_isoratio       : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_ISORATIO;

/* Define the union U_ISP_CA_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_lut_update     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LUT_UPDATE;

/* Define the union U_ISP_CA_YUV2RGB_COEF0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yuv2rgb_coef01 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_ca_yuv2rgb_coef00 : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_COEF0;

/* Define the union U_ISP_CA_YUV2RGB_COEF1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yuv2rgb_coef10 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_ca_yuv2rgb_coef02 : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_COEF1;

/* Define the union U_ISP_CA_YUV2RGB_COEF2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yuv2rgb_coef12 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_ca_yuv2rgb_coef11 : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_COEF2;

/* Define the union U_ISP_CA_YUV2RGB_COEF3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yuv2rgb_coef21 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_ca_yuv2rgb_coef20 : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_COEF3;

/* Define the union U_ISP_CA_YUV2RGB_COEF4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yuv2rgb_coef22 : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_COEF4;

/* Define the union U_ISP_CA_YUV2RGB_INDC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yuv2rgb_indc1  : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_ca_yuv2rgb_indc0  : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_INDC0;

/* Define the union U_ISP_CA_YUV2RGB_INDC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yuv2rgb_indc2  : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_INDC1;

/* Define the union U_ISP_CA_YUV2RGB_OUTDC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yuv2rgb_outdc1 : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    isp_ca_yuv2rgb_outdc0 : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_OUTDC0;

/* Define the union U_ISP_CA_YUV2RGB_OUTDC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_yuv2rgb_outdc2 : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_YUV2RGB_OUTDC1;

/* Define the union U_ISP_CA_SKINLLUMA_UTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skinlluma_umax : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_skinlluma_umin : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINLLUMA_UTH;

/* Define the union U_ISP_CA_SKINLLUMA_UYTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skinlluma_uymax : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_skinlluma_uymin : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINLLUMA_UYTH;

/* Define the union U_ISP_CA_SKINHLUMA_UTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skinhluma_umax : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_skinhluma_umin : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINHLUMA_UTH;

/* Define the union U_ISP_CA_SKINHLUMA_UYTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skinhluma_uymax : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_skinhluma_uymin : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINHLUMA_UYTH;

/* Define the union U_ISP_CA_SKINLLUMA_VTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skinlluma_vmax : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_skinlluma_vmin : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINLLUMA_VTH;

/* Define the union U_ISP_CA_SKINLLUMA_VYTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skinlluma_vymax : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_skinlluma_vymin : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINLLUMA_VYTH;

/* Define the union U_ISP_CA_SKINHLUMA_VTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skinhluma_vmax : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_skinhluma_vmin : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINHLUMA_VTH;

/* Define the union U_ISP_CA_SKINHLUMA_VYTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skinhluma_vymax : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ca_skinhluma_vymin : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKINHLUMA_VYTH;

/* Define the union U_ISP_CA_SKIN_UVDIFF */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skin_uvdiff    : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKIN_UVDIFF;

/* Define the union U_ISP_CA_SKIN_RATIOTH0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skinratioth_mid : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ca_skinratioth_low : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKIN_RATIOTH0;

/* Define the union U_ISP_CA_SKIN_RATIOTH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_skinratioth_high : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_SKIN_RATIOTH1;

/* Define the union U_ISP_CA_LUT_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_lut_waddr      : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LUT_WADDR;

/* Define the union U_ISP_CA_LUT_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_lut_wdata      : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LUT_WDATA;

/* Define the union U_ISP_CA_LUT_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_lut_raddr      : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LUT_RADDR;

/* Define the union U_ISP_CA_LUT_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_lut_rdata      : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LUT_RDATA;

/* Define the union U_ISP_LOGLUT_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_loglut_version     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_VERSION;
/* Define the union U_ISP_LOGLUT_BITW */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut_bitw_out   : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_BITW;

/* Define the union U_ISP_LOGLUT_OFFSETR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut_offset_r   : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_OFFSETR;

/* Define the union U_ISP_LOGLUT_OFFSETGR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut_offset_gr  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_OFFSETGR;

/* Define the union U_ISP_LOGLUT_OFFSETGB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut_offset_gb  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_OFFSETGB;

/* Define the union U_ISP_LOGLUT_OFFSETB */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut_offset_b   : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_OFFSETB;

/* Define the union U_ISP_LOGLUT_LUT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_loglut_lut_waddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_LUT_WADDR;
/* Define the union U_ISP_LOGLUT_LUT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_loglut_lut_wdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_LUT_WDATA;
/* Define the union U_ISP_LOGLUT_LUT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_loglut_lut_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_LUT_RADDR;
/* Define the union U_ISP_LOGLUT_LUT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_loglut_lut_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_LUT_RDATA;
/* Define the union U_ISP_CLUT_ADJ0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_gain0        : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_clut_gain1        : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_ADJ0;

/* Define the union U_ISP_CLUT_ADJ1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_gain2        : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_ADJ1;

/* Define the union U_ISP_CLUT_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut_update   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT_UPDATE;

/* Define the union U_ISP_CLUT_LUT0_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut0_waddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT0_WADDR;

/* Define the union U_ISP_CLUT_LUT0_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut0_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT0_WDATA;
/* Define the union U_ISP_CLUT_LUT0_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut0_raddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT0_RADDR;

/* Define the union U_ISP_CLUT_LUT0_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut0_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT0_RDATA;
/* Define the union U_ISP_CLUT_LUT1_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut1_waddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT1_WADDR;

/* Define the union U_ISP_CLUT_LUT1_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut1_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT1_WDATA;
/* Define the union U_ISP_CLUT_LUT1_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut1_raddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT1_RADDR;

/* Define the union U_ISP_CLUT_LUT1_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut1_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT1_RDATA;
/* Define the union U_ISP_CLUT_LUT2_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut2_waddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT2_WADDR;

/* Define the union U_ISP_CLUT_LUT2_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut2_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT2_WDATA;
/* Define the union U_ISP_CLUT_LUT2_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut2_raddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT2_RADDR;

/* Define the union U_ISP_CLUT_LUT2_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut2_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT2_RDATA;
/* Define the union U_ISP_CLUT_LUT3_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut3_waddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT3_WADDR;

/* Define the union U_ISP_CLUT_LUT3_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut3_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT3_WDATA;
/* Define the union U_ISP_CLUT_LUT3_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut3_raddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT3_RADDR;

/* Define the union U_ISP_CLUT_LUT3_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut3_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT3_RDATA;
/* Define the union U_ISP_CLUT_LUT4_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut4_waddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT4_WADDR;

/* Define the union U_ISP_CLUT_LUT4_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut4_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT4_WDATA;
/* Define the union U_ISP_CLUT_LUT4_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut4_raddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT4_RADDR;

/* Define the union U_ISP_CLUT_LUT4_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut4_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT4_RDATA;
/* Define the union U_ISP_CLUT_LUT5_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut5_waddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT5_WADDR;

/* Define the union U_ISP_CLUT_LUT5_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut5_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT5_WDATA;
/* Define the union U_ISP_CLUT_LUT5_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut5_raddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT5_RADDR;

/* Define the union U_ISP_CLUT_LUT5_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut5_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT5_RDATA;
/* Define the union U_ISP_CLUT_LUT6_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut6_waddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT6_WADDR;

/* Define the union U_ISP_CLUT_LUT6_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut6_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT6_WDATA;
/* Define the union U_ISP_CLUT_LUT6_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut6_raddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT6_RADDR;

/* Define the union U_ISP_CLUT_LUT6_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut6_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT6_RDATA;
/* Define the union U_ISP_CLUT_LUT7_WADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut7_waddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT7_WADDR;

/* Define the union U_ISP_CLUT_LUT7_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut7_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT7_WDATA;
/* Define the union U_ISP_CLUT_LUT7_RADDR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_clut_lut7_raddr   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT7_RADDR;

/* Define the union U_ISP_CLUT_LUT7_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut7_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT7_RDATA;
/* Define the union U_ISP_LDCI_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_ldci_calc_en      : 1   ; /* [1]  */
        unsigned int    isp_ldci_wrstat_en    : 1   ; /* [2]  */
        unsigned int    isp_ldci_rdstat_en    : 1   ; /* [3]  */
        unsigned int    reserved_1            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_CFG;

/* Define the union U_ISP_LDCI_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_version       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_VERSION;
/* Define the union U_ISP_LDCI_CALC_SMALL_OFFSET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_calc_map_offsetx : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_CALC_SMALL_OFFSET;

/* Define the union U_ISP_LDCI_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_smlmapstride : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_ldci_smlmapheight : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    isp_ldci_total_zone   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_ZONE;

/* Define the union U_ISP_LDCI_GLB_HEWGT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_glb_hewgt    : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_GLB_HEWGT;

/* Define the union U_ISP_LDCI_SCALE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_scalex       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_ldci_scaley       : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_SCALE;

/* Define the union U_ISP_LDCI_DRCEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_calc_drcen   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DRCEN;

/* Define the union U_ISP_LDCI_LUMA_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_luma_sel     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LUMA_SEL;

/* Define the union U_ISP_LDCI_BLC_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_blc_ctrl     : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_BLC_CTRL;

/* Define the union U_ISP_LDCI_PFLMT_EN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_pflmt_en     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_PFLMT_EN;

/* Define the union U_ISP_LDCI_PFORI */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_pfori0       : 8   ; /* [7..0]  */
        unsigned int    isp_ldci_pfori1       : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_PFORI;

/* Define the union U_ISP_LDCI_PFSFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_pfsft0       : 3   ; /* [2..0]  */
        unsigned int    isp_ldci_pfsft1       : 3   ; /* [5..3]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_ldci_pfsft2       : 3   ; /* [10..8]  */
        unsigned int    isp_ldci_pfsft3       : 3   ; /* [13..11]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_PFSFT;

/* Define the union U_ISP_LDCI_PFTHD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_pfthd0       : 8   ; /* [7..0]  */
        unsigned int    isp_ldci_pfthd1       : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_PFTHD;

/* Define the union U_ISP_LDCI_PFRLY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_pfrly0       : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_ldci_pfrly1       : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_PFRLY;

/* Define the union U_ISP_LDCI_PFMUL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_pfmul        : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_PFMUL;

/* Define the union U_ISP_LDCI_LUT_UPDATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_drc_lut_update : 1   ; /* [0]  */
        unsigned int    isp_ldci_calc_lut_update : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LUT_UPDATE;

/* Define the union U_ISP_LDCI_LPF_LPFCOEF0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfcoef0     : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_ldci_lpfcoef1     : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_LPFCOEF0;

/* Define the union U_ISP_LDCI_LPF_LPFCOEF1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfcoef2     : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_ldci_lpfcoef3     : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_LPFCOEF1;

/* Define the union U_ISP_LDCI_LPF_LPFCOEF2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfcoef4     : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_ldci_lpfcoef5     : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_LPFCOEF2;

/* Define the union U_ISP_LDCI_LPF_LPFCOEF3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfcoef6     : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    isp_ldci_lpfcoef7     : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_LPFCOEF3;

/* Define the union U_ISP_LDCI_LPF_LPFCOEF4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfcoef8     : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_LPFCOEF4;

/* Define the union U_ISP_LDCI_LPF_LPFSFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpfsft       : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_LPFSFT;

/* Define the union U_ISP_LDCI_DRC_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_drc_waddr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DRC_WADDR;
/* Define the union U_ISP_LDCI_DRC_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_calcdrc_wdata : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ldci_statdrc_wdata : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DRC_WDATA;

/* Define the union U_ISP_LDCI_DRC_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_drc_raddr     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DRC_RADDR;
/* Define the union U_ISP_LDCI_DRC_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_calcdrc_rdata : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ldci_statdrc_rdata : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DRC_RDATA;

/* Define the union U_ISP_LDCI_HE_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_he_waddr      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_HE_WADDR;
/* Define the union U_ISP_LDCI_HE_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_hepos_wdata  : 7   ; /* [6..0]  */
        unsigned int    isp_ldci_heneg_wdata  : 7   ; /* [13..7]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_HE_WDATA;

/* Define the union U_ISP_LDCI_HE_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_he_raddr      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_HE_RADDR;
/* Define the union U_ISP_LDCI_HE_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_hepos_rdata  : 7   ; /* [6..0]  */
        unsigned int    isp_ldci_heneg_rdata  : 7   ; /* [13..7]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_HE_RDATA;

/* Define the union U_ISP_LDCI_DE_USM_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_de_usm_waddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DE_USM_WADDR;
/* Define the union U_ISP_LDCI_DE_USM_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_usmpos_wdata : 9   ; /* [8..0]  */
        unsigned int    isp_ldci_usmneg_wdata : 9   ; /* [17..9]  */
        unsigned int    isp_ldci_delut_wdata  : 9   ; /* [26..18]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DE_USM_WDATA;

/* Define the union U_ISP_LDCI_DE_USM_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_de_usm_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DE_USM_RADDR;
/* Define the union U_ISP_LDCI_DE_USM_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_usmpos_rdata : 9   ; /* [8..0]  */
        unsigned int    isp_ldci_usmneg_rdata : 9   ; /* [17..9]  */
        unsigned int    isp_ldci_delut_rdata  : 9   ; /* [26..18]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DE_USM_RDATA;

/* Define the union U_ISP_LDCI_CGAIN_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_cgain_waddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_CGAIN_WADDR;
/* Define the union U_ISP_LDCI_CGAIN_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_cgain_wdata  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_CGAIN_WDATA;

/* Define the union U_ISP_LDCI_CGAIN_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_cgain_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_CGAIN_RADDR;
/* Define the union U_ISP_LDCI_CGAIN_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_cgain_rdata  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_CGAIN_RDATA;

/* Define the union U_ISP_LDCI_STAT_HPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_hstart       : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_ldci_hend         : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_STAT_HPOS;

/* Define the union U_ISP_LDCI_STAT_VPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_vstart       : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_ldci_vend         : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_STAT_VPOS;

/* Define the union U_ISP_LDCI_STAT_EVRATIO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_stat_evratio : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_STAT_EVRATIO;

/* Define the union U_ISP_LDCI_STAT_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_stat_smlmapwidth : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_ldci_stat_smlmapheight : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    isp_ldci_stat_total_zone : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_STAT_ZONE;

/* Define the union U_ISP_LDCI_BENDE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_bende        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_BENDE;

/* Define the union U_ISP_LDCI_DEREF */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_deref        : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DEREF;

/* Define the union U_ISP_LDCI_DEH_LPFSFT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_deh_lpfsft   : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DEH_LPFSFT;

/* Define the union U_ISP_LDCI_LPFCOEFH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_coefh0       : 9   ; /* [8..0]  */
        unsigned int    isp_ldci_coefh1       : 9   ; /* [17..9]  */
        unsigned int    isp_ldci_coefh2       : 9   ; /* [26..18]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPFCOEFH;

/* Define the union U_ISP_LDCI_LPFCOEFV */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_coefv0       : 9   ; /* [8..0]  */
        unsigned int    isp_ldci_coefv1       : 9   ; /* [17..9]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPFCOEFV;

/* Define the union U_ISP_LDCI_BLK_SMLMAPWIDTH0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_blk_smlmapwidth0 : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_BLK_SMLMAPWIDTH0;

/* Define the union U_ISP_LDCI_BLK_SMLMAPWIDTH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_blk_smlmapwidth1 : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_BLK_SMLMAPWIDTH1;

/* Define the union U_ISP_LDCI_BLK_SMLMAPWIDTH2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_blk_smlmapwidth2 : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_BLK_SMLMAPWIDTH2;

/* Define the union U_ISP_LDCI_STAT_DRCEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_stat_drcen   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_STAT_DRCEN;

/* Define the union U_ISP_LDCI_CHRDAMP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_chrposdamp   : 8   ; /* [7..0]  */
        unsigned int    isp_ldci_chrnegdamp   : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_CHRDAMP;

/* Define the union U_ISP_LDCI_GROBAL_MAP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_grobal_luma  : 10  ; /* [9..0]  */
        unsigned int    isp_ldci_grobal_poply2 : 10  ; /* [19..10]  */
        unsigned int    isp_ldci_grobal_poply3 : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_GROBAL_MAP;

/* Define the union U_ISP_LDCI_POLYP_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_poply_waddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYP_WADDR;
/* Define the union U_ISP_LDCI_POLYP_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_poply1_wdata : 10  ; /* [9..0]  */
        unsigned int    isp_ldci_poply2_wdata : 10  ; /* [19..10]  */
        unsigned int    isp_ldci_poply3_wdata : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYP_WDATA;

/* Define the union U_ISP_LDCI_POLYP_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_poply_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYP_RADDR;
/* Define the union U_ISP_LDCI_POLYP_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_poply1_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_ldci_poply2_rdata : 10  ; /* [19..10]  */
        unsigned int    isp_ldci_poply3_rdata : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYP_RDATA;

/* Define the union U_ISP_LDCI_POLYQ01_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_plyq01_waddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYQ01_WADDR;
/* Define the union U_ISP_LDCI_POLYQ01_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_plyq0_wdata  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ldci_plyq1_wdata  : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYQ01_WDATA;

/* Define the union U_ISP_LDCI_POLYQ01_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_plyq01_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYQ01_RADDR;
/* Define the union U_ISP_LDCI_POLYQ01_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_plyq0_rdata  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ldci_plyq1_rdata  : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYQ01_RDATA;

/* Define the union U_ISP_LDCI_POLYQ23_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_plyq23_waddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYQ23_WADDR;
/* Define the union U_ISP_LDCI_POLYQ23_WDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_plyq2_wdata  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ldci_plyq3_wdata  : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYQ23_WDATA;

/* Define the union U_ISP_LDCI_POLYQ23_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_plyq23_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYQ23_RADDR;
/* Define the union U_ISP_LDCI_POLYQ23_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_plyq2_rdata  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ldci_plyq3_rdata  : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYQ23_RDATA;

/* Define the union U_ISP_LDCI_LPF_MAP_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_MAP_WADDR;
/* Define the union U_ISP_LDCI_LPF_MAP_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reserved_0             : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_MAP_WDATA;
/* Define the union U_ISP_LDCI_LPF_MAP_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_ldci_lpfmap_raddr  : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_MAP_RADDR;
/* Define the union U_ISP_LDCI_LPF_MAP_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_lpflitmap_rdata : 10  ; /* [9..0]  */
        unsigned int    isp_ldci_lpfplyp2map_rdata : 10  ; /* [19..10]  */
        unsigned int    isp_ldci_lpfplyp3map_rdata : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_LPF_MAP_RDATA;

/* Define the union U_ISP_DRC_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    isp_drc_wrstat_en     : 1   ; /* [1]  */
        unsigned int    isp_drc_rdstat_en     : 1   ; /* [2]  */
        unsigned int    isp_drc_vbiflt_en     : 1   ; /* [3]  */
        unsigned int    isp_drc_color_corr_en : 1   ; /* [4]  */
        unsigned int    isp_drc_pdw_sum_en    : 1   ; /* [5]  */
        unsigned int    isp_drc_detail_boost_en : 1   ; /* [6]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CFG;

/* Define the union U_ISP_DRC_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_version        : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_VERSION;
/* Define the union U_ISP_DRC_ZONE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_hnum          : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    isp_drc_vnum          : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_ZONE;

/* Define the union U_ISP_DRC_ZONE_SIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_zone_hsize    : 9   ; /* [8..0]  */
        unsigned int    isp_drc_chk_x         : 6   ; /* [14..9]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_drc_zone_vsize    : 9   ; /* [24..16]  */
        unsigned int    isp_drc_chk_y         : 6   ; /* [30..25]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_ZONE_SIZE;

/* Define the union U_ISP_DRC_ZONE_DIV0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_div_x0        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_drc_div_y0        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_ZONE_DIV0;

/* Define the union U_ISP_DRC_ZONE_DIV1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_div_x1        : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_drc_div_y1        : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_ZONE_DIV1;

/* Define the union U_ISP_DRC_BIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_bin           : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BIN;

/* Define the union U_ISP_DRC_ZONE_INIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_acc_x_init    : 16  ; /* [15..0]  */
        unsigned int    isp_drc_cnt_x_init    : 9   ; /* [24..16]  */
        unsigned int    isp_drc_idx_x_init    : 6   ; /* [30..25]  */
        unsigned int    isp_drc_big_x_init    : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_ZONE_INIT;

/* Define the union U_ISP_DRC_BIN_SCALE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_bin_scale     : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BIN_SCALE;

/* Define the union U_ISP_DRC_OUTBITS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_outbits       : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_OUTBITS;

/* Define the union U_ISP_DRC_LOCAL_EDGE_LMT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_local_edge_lmt : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_LOCAL_EDGE_LMT;

/* Define the union U_ISP_DRC_STRENGTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_strength      : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_STRENGTH;

/* Define the union U_ISP_DRC_STAT_WIDTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_stat_width    : 16  ; /* [15..0]  */
        unsigned int    isp_drc_total_width   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_STAT_WIDTH;

/* Define the union U_ISP_DRC_DARK_GAIN_LMT_Y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_val1_y        : 7   ; /* [6..0]  */
        unsigned int    isp_drc_sft1_y        : 3   ; /* [9..7]  */
        unsigned int    isp_drc_val2_y        : 4   ; /* [13..10]  */
        unsigned int    isp_drc_sft2_y        : 2   ; /* [15..14]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DARK_GAIN_LMT_Y;

/* Define the union U_ISP_DRC_DARK_GAIN_LMT_C */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_val1_c        : 7   ; /* [6..0]  */
        unsigned int    isp_drc_sft1_c        : 3   ; /* [9..7]  */
        unsigned int    isp_drc_val2_c        : 4   ; /* [13..10]  */
        unsigned int    isp_drc_sft2_c        : 2   ; /* [15..14]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DARK_GAIN_LMT_C;

/* Define the union U_ISP_DRC_BRIGHT_GAIN_LMT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_val           : 7   ; /* [6..0]  */
        unsigned int    isp_drc_sft           : 3   ; /* [9..7]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BRIGHT_GAIN_LMT;

/* Define the union U_ISP_DRC_RGB_WGT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_r_wgt         : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_drc_g_wgt         : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_drc_b_wgt         : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_RGB_WGT;

/* Define the union U_ISP_DRC_LUT_MIX_CRTL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_lut_mix_ctrl  : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_LUT_MIX_CRTL;

/* Define the union U_ISP_DRC_GAIN_CLIP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_gain_clip_step : 4   ; /* [3..0]  */
        unsigned int    isp_drc_gain_clip_knee : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_GAIN_CLIP;

/* Define the union U_ISP_DRC_COLOR_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cc_lut_ctrl   : 1   ; /* [0]  */
        unsigned int    isp_drc_cc_lin_pow    : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_COLOR_CTRL;

/* Define the union U_ISP_DRC_GLOBAL_CORR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cc_global_corr : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_GLOBAL_CORR;

/* Define the union U_ISP_DRC_MIXING_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_mixing_coring : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_MIXING_CORING;

/* Define the union U_ISP_DRC_MIXING_DARK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_dark_min      : 8   ; /* [7..0]  */
        unsigned int    isp_drc_dark_max      : 8   ; /* [15..8]  */
        unsigned int    isp_drc_dark_thr      : 8   ; /* [23..16]  */
        unsigned int    isp_drc_dark_slo      : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_MIXING_DARK;

/* Define the union U_ISP_DRC_MIXING_BRIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_bright_min    : 8   ; /* [7..0]  */
        unsigned int    isp_drc_bright_max    : 8   ; /* [15..8]  */
        unsigned int    isp_drc_bright_thr    : 8   ; /* [23..16]  */
        unsigned int    isp_drc_bright_slo    : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_MIXING_BRIGHT;

/* Define the union U_ISP_DRC_RG_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_rg_ctr        : 8   ; /* [7..0]  */
        unsigned int    isp_drc_rg_wid        : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_drc_rg_slo        : 3   ; /* [18..16]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_RG_CTRL;

/* Define the union U_ISP_DRC_BG_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_bg_thr        : 8   ; /* [7..0]  */
        unsigned int    isp_drc_bg_slo        : 3   ; /* [10..8]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BG_CTRL;

/* Define the union U_ISP_DRC_PDW */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_low_thr       : 8   ; /* [7..0]  */
        unsigned int    isp_drc_low_slo       : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_drc_high_thr      : 8   ; /* [23..16]  */
        unsigned int    isp_drc_high_slo      : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PDW;

/* Define the union U_ISP_DRC_PPDTC_SUM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_ppdtc_sum     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PPDTC_SUM;

/* Define the union U_ISP_DRC_TMLUT0_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut0_waddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TMLUT0_WADDR;
/* Define the union U_ISP_DRC_TMLUT0_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut0_wdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TMLUT0_WDATA;
/* Define the union U_ISP_DRC_TMLUT0_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut0_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TMLUT0_RADDR;
/* Define the union U_ISP_DRC_TMLUT0_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut0_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TMLUT0_RDATA;
/* Define the union U_ISP_DRC_TMLUT1_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut1_waddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TMLUT1_WADDR;
/* Define the union U_ISP_DRC_TMLUT1_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut1_wdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TMLUT1_WDATA;
/* Define the union U_ISP_DRC_TMLUT1_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut1_raddr   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TMLUT1_RADDR;
/* Define the union U_ISP_DRC_TMLUT1_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_tmlut1_rdata   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TMLUT1_RDATA;
/* Define the union U_ISP_DRC_CCLUT_WADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_cclut_waddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CCLUT_WADDR;
/* Define the union U_ISP_DRC_CCLUT_WDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_cclut_wdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CCLUT_WDATA;
/* Define the union U_ISP_DRC_CCLUT_RADDR */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_cclut_raddr    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CCLUT_RADDR;
/* Define the union U_ISP_DRC_CCLUT_RDATA */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_cclut_rdata    : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CCLUT_RDATA;
/* Define the union U_ISP_DRC_GRAD_REV */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_grad_rev_thres : 8   ; /* [7..0]  */
        unsigned int    isp_drc_grad_rev_max  : 7   ; /* [14..8]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    isp_drc_grad_rev_slope : 4   ; /* [19..16]  */
        unsigned int    reserved_1            : 4   ; /* [23..20]  */
        unsigned int    isp_drc_grad_rev_shift : 3   ; /* [26..24]  */
        unsigned int    reserved_2            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_GRAD_REV;

/* Define the union U_ISP_DRC_VBI_STRENGTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_var_rng_fine  : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    isp_drc_var_rng_medium : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 1   ; /* [7]  */
        unsigned int    isp_drc_var_rng_coarse : 4   ; /* [11..8]  */
        unsigned int    isp_drc_var_spa_fine  : 2   ; /* [13..12]  */
        unsigned int    reserved_2            : 2   ; /* [15..14]  */
        unsigned int    isp_drc_var_spa_medium : 3   ; /* [18..16]  */
        unsigned int    reserved_3            : 1   ; /* [19]  */
        unsigned int    isp_drc_var_spa_coarse : 4   ; /* [23..20]  */
        unsigned int    reserved_4            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_VBI_STRENGTH;

/* Define the union U_ISP_DRC_ADA_COEF */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_fr_ada_max    : 4   ; /* [3..0]  */
        unsigned int    isp_drc_dis_offset_coef : 4   ; /* [7..4]  */
        unsigned int    isp_drc_thr_coef_low  : 3   ; /* [10..8]  */
        unsigned int    reserved_0            : 1   ; /* [11]  */
        unsigned int    isp_drc_thr_coef_high : 3   ; /* [14..12]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    reserved_2            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_ADA_COEF;

/* Define the union U_ISP_DRC_VBI_STATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_vbi_state     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_VBI_STATE;

/* Define the union U_ISP_DRC_DETAIL_CORING */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_detail_coring : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DETAIL_CORING;

/* Define the union U_ISP_DRC_DETAIL_DARK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_detail_dark_min : 8   ; /* [7..0]  */
        unsigned int    isp_drc_detail_dark_max : 8   ; /* [15..8]  */
        unsigned int    isp_drc_detail_dark_thr : 8   ; /* [23..16]  */
        unsigned int    isp_drc_detail_dark_slo : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DETAIL_DARK;

/* Define the union U_ISP_DRC_DETAIL_BRIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_detail_bright_min : 8   ; /* [7..0]  */
        unsigned int    isp_drc_detail_bright_max : 8   ; /* [15..8]  */
        unsigned int    isp_drc_detail_bright_thr : 8   ; /* [23..16]  */
        unsigned int    isp_drc_detail_bright_slo : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DETAIL_BRIGHT;

/* Define the union U_ISP_DRC_DARK_STEP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_dark_step     : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DARK_STEP;

/* Define the union U_ISP_DRC_DARK_CURVE0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_detail_dark_curve0 : 4   ; /* [3..0]  */
        unsigned int    isp_drc_detail_dark_curve1 : 4   ; /* [7..4]  */
        unsigned int    isp_drc_detail_dark_curve2 : 4   ; /* [11..8]  */
        unsigned int    isp_drc_detail_dark_curve3 : 4   ; /* [15..12]  */
        unsigned int    isp_drc_detail_dark_curve4 : 4   ; /* [19..16]  */
        unsigned int    isp_drc_detail_dark_curve5 : 4   ; /* [23..20]  */
        unsigned int    isp_drc_detail_dark_curve6 : 4   ; /* [27..24]  */
        unsigned int    isp_drc_detail_dark_curve7 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DARK_CURVE0;

/* Define the union U_ISP_DRC_DARK_CURVE1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_detail_dark_curve8 : 4   ; /* [3..0]  */
        unsigned int    isp_drc_detail_dark_curve9 : 4   ; /* [7..4]  */
        unsigned int    isp_drc_detail_dark_curve10 : 4   ; /* [11..8]  */
        unsigned int    isp_drc_detail_dark_curve11 : 4   ; /* [15..12]  */
        unsigned int    isp_drc_detail_dark_curve12 : 4   ; /* [19..16]  */
        unsigned int    isp_drc_detail_dark_curve13 : 4   ; /* [23..20]  */
        unsigned int    isp_drc_detail_dark_curve14 : 4   ; /* [27..24]  */
        unsigned int    isp_drc_detail_dark_curve15 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DARK_CURVE1;

/* Define the union U_ISP_DRC_BRIGHT_STEP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_bright_step   : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BRIGHT_STEP;

/* Define the union U_ISP_DRC_BRIGHT_CURVE0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_detail_bright_curve0 : 4   ; /* [3..0]  */
        unsigned int    isp_drc_detail_bright_curve1 : 4   ; /* [7..4]  */
        unsigned int    isp_drc_detail_bright_curve2 : 4   ; /* [11..8]  */
        unsigned int    isp_drc_detail_bright_curve3 : 4   ; /* [15..12]  */
        unsigned int    isp_drc_detail_bright_curve4 : 4   ; /* [19..16]  */
        unsigned int    isp_drc_detail_bright_curve5 : 4   ; /* [23..20]  */
        unsigned int    isp_drc_detail_bright_curve6 : 4   ; /* [27..24]  */
        unsigned int    isp_drc_detail_bright_curve7 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BRIGHT_CURVE0;

/* Define the union U_ISP_DRC_BRIGHT_CURVE1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_detail_bright_curve8 : 4   ; /* [3..0]  */
        unsigned int    isp_drc_detail_bright_curve9 : 4   ; /* [7..4]  */
        unsigned int    isp_drc_detail_bright_curve10 : 4   ; /* [11..8]  */
        unsigned int    isp_drc_detail_bright_curve11 : 4   ; /* [15..12]  */
        unsigned int    isp_drc_detail_bright_curve12 : 4   ; /* [19..16]  */
        unsigned int    isp_drc_detail_bright_curve13 : 4   ; /* [23..20]  */
        unsigned int    isp_drc_detail_bright_curve14 : 4   ; /* [27..24]  */
        unsigned int    isp_drc_detail_bright_curve15 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BRIGHT_CURVE1;

/* Define the union U_ISP_DRC_PREV_LUMA_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_prev_luma_0   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PREV_LUMA_0;

/* Define the union U_ISP_DRC_PREV_LUMA_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_prev_luma_1   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PREV_LUMA_1;

/* Define the union U_ISP_DRC_PREV_LUMA_2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_prev_luma_2   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PREV_LUMA_2;

/* Define the union U_ISP_DRC_PREV_LUMA_3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_prev_luma_3   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PREV_LUMA_3;

/* Define the union U_ISP_DRC_PREV_LUMA_4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_prev_luma_4   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PREV_LUMA_4;

/* Define the union U_ISP_DRC_PREV_LUMA_5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_prev_luma_5   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PREV_LUMA_5;

/* Define the union U_ISP_DRC_PREV_LUMA_6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_prev_luma_6   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PREV_LUMA_6;

/* Define the union U_ISP_DRC_PREV_LUMA_7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_prev_luma_7   : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_PREV_LUMA_7;

/* Define the union U_ISP_DRC_CUBIC_THRESX01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_thresx01 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_THRESX01;

/* Define the union U_ISP_DRC_CUBIC_THRESX10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_thresx10 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_THRESX10;

/* Define the union U_ISP_DRC_CUBIC_THRESX11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_thresx11 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_THRESX11;

/* Define the union U_ISP_DRC_CUBIC_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_sel     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_SEL;

/* Define the union U_ISP_DRC_CUBIC_COEF00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef00_a : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 8   ; /* [23..16]  */
        unsigned int    isp_drc_cubic_coef00_aexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF00;

/* Define the union U_ISP_DRC_CUBIC_COEF01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef01_b : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 3   ; /* [23..21]  */
        unsigned int    isp_drc_cubic_coef01_bexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF01;

/* Define the union U_ISP_DRC_CUBIC_COEF02 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef02_c : 22  ; /* [21..0]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    isp_drc_cubic_coef02_cexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF02;

/* Define the union U_ISP_DRC_CUBIC_COEF03 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef03_d : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF03;

/* Define the union U_ISP_DRC_CUBIC_COEF10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef10_a : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 8   ; /* [23..16]  */
        unsigned int    isp_drc_cubic_coef10_aexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF10;

/* Define the union U_ISP_DRC_CUBIC_COEF11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef11_b : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 3   ; /* [23..21]  */
        unsigned int    isp_drc_cubic_coef11_bexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF11;

/* Define the union U_ISP_DRC_CUBIC_COEF12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef12_c : 22  ; /* [21..0]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    isp_drc_cubic_coef12_cexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF12;

/* Define the union U_ISP_DRC_CUBIC_COEF13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef13_d : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF13;

/* Define the union U_ISP_DRC_CUBIC_COEF20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef20_a : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 8   ; /* [23..16]  */
        unsigned int    isp_drc_cubic_coef20_aexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF20;

/* Define the union U_ISP_DRC_CUBIC_COEF21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef21_b : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 3   ; /* [23..21]  */
        unsigned int    isp_drc_cubic_coef21_bexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF21;

/* Define the union U_ISP_DRC_CUBIC_COEF22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef22_c : 22  ; /* [21..0]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    isp_drc_cubic_coef22_cexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF22;

/* Define the union U_ISP_DRC_CUBIC_COEF23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef23_d : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF23;

/* Define the union U_ISP_DRC_CUBIC_COEF30 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef30_a : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 8   ; /* [23..16]  */
        unsigned int    isp_drc_cubic_coef30_aexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF30;

/* Define the union U_ISP_DRC_CUBIC_COEF31 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef31_b : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 3   ; /* [23..21]  */
        unsigned int    isp_drc_cubic_coef31_bexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF31;

/* Define the union U_ISP_DRC_CUBIC_COEF32 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef32_c : 22  ; /* [21..0]  */
        unsigned int    reserved_0            : 2   ; /* [23..22]  */
        unsigned int    isp_drc_cubic_coef32_cexp : 4   ; /* [27..24]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF32;

/* Define the union U_ISP_DRC_CUBIC_COEF33 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cubic_coef33_d : 30  ; /* [29..0]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CUBIC_COEF33;

/* Define the union U_ISP_DRC_FLT_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_flt_spa_fine  : 4   ; /* [3..0]  */
        unsigned int    isp_drc_flt_spa_medium : 4   ; /* [7..4]  */
        unsigned int    isp_drc_flt_spa_coarse : 4   ; /* [11..8]  */
        unsigned int    isp_drc_flt_rng_fine  : 4   ; /* [15..12]  */
        unsigned int    isp_drc_flt_rng_medium : 4   ; /* [19..16]  */
        unsigned int    isp_drc_flt_rng_coarse : 4   ; /* [23..20]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_FLT_CFG;

/* Define the union U_ISP_DRC_BIN_FACTOR_COARSE_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_bin_mix_factor_coarse_0 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_drc_bin_mix_factor_coarse_1 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_drc_bin_mix_factor_coarse_2 : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    isp_drc_bin_mix_factor_coarse_3 : 5   ; /* [28..24]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BIN_FACTOR_COARSE_0;

/* Define the union U_ISP_DRC_BIN_FACTOR_COARSE_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_bin_mix_factor_coarse_4 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_drc_bin_mix_factor_coarse_5 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_drc_bin_mix_factor_coarse_6 : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    isp_drc_bin_mix_factor_coarse_7 : 5   ; /* [28..24]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BIN_FACTOR_COARSE_1;

/* Define the union U_ISP_DRC_BIN_FACTOR_MEDIUM_0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_bin_mix_factor_medium_0 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_drc_bin_mix_factor_medium_1 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_drc_bin_mix_factor_medium_2 : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    isp_drc_bin_mix_factor_medium_3 : 5   ; /* [28..24]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BIN_FACTOR_MEDIUM_0;

/* Define the union U_ISP_DRC_BIN_FACTOR_MEDIUM_1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_bin_mix_factor_medium_4 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    isp_drc_bin_mix_factor_medium_5 : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    isp_drc_bin_mix_factor_medium_6 : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    isp_drc_bin_mix_factor_medium_7 : 5   ; /* [28..24]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BIN_FACTOR_MEDIUM_1;

/* Define the union U_ISP_DRC_DETAIL_SUB_FACTOR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_detail_sub_factor : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DETAIL_SUB_FACTOR;

/* Define the union U_ISP_DRC_WGT_BOX_TRI_SEL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_wgt_box_tri_sel : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_WGT_BOX_TRI_SEL;

/* Define the union U_ISP_DRC_BLK_WGT_INIT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_blk_wgt_init  : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_BLK_WGT_INIT;

/* Define the union U_ISP_DRC_SHP_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_shp_log       : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_drc_shp_exp       : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_SHP_CFG;

/* Define the union U_ISP_DRC_DIV_DENOM_LOG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_div_denom_log : 18  ; /* [17..0]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DIV_DENOM_LOG;

/* Define the union U_ISP_DRC_DENOM_EXP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_denom_exp     : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DENOM_EXP;

/* Define the union U_ISP_DRC_DEBUG_INFO */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_drc_debug_info     : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_DEBUG_INFO;
/* Define the union U_ISP_SQRT_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sqrt_version       : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SQRT_VERSION;
/* Define the union U_ISP_SQ_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sq_version         : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SQ_VERSION;
/* Define the union U_ISP_SQRT1_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_sqrt1_version      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SQRT1_VERSION;
/* Define the union U_ISP_SPLIT_VERSION */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_split_version      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_VERSION;
/* Define the union U_ISP_SPLIT_DATA_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_split_replace_y_en : 1   ; /* [0]  */
        unsigned int    isp_split_replace_u_en : 1   ; /* [1]  */
        unsigned int    isp_split_replace_v_en : 1   ; /* [2]  */
        unsigned int    isp_split_ex_y_en     : 1   ; /* [3]  */
        unsigned int    isp_split_ex_u_en     : 1   ; /* [4]  */
        unsigned int    isp_split_ex_v_en     : 1   ; /* [5]  */
        unsigned int    isp_split_ex_yuv_en   : 3   ; /* [8..6]  */
        unsigned int    isp_split_split_en    : 1   ; /* [9]  */
        unsigned int    isp_split_data_sel    : 2   ; /* [11..10]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_split_dc_data     : 7   ; /* [22..16]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_DATA_CFG;

/* Define the union U_ISP_SPLIT_DATA_VERSION */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_split_data_version : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_DATA_VERSION;

/* Define the union U_ISP_SPLIT_DATA_BITWIDTH */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_split_data_bitw   : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_DATA_BITWIDTH;

/* Define the union U_ISP_SPLIT_DATA_AGAIN */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_split_data_again   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_DATA_AGAIN;
/* Define the union U_ISP_SPLIT_DATA_DGAIN */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_split_data_dgain   : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_DATA_DGAIN;
/* Define the union U_ISP_SPLIT_DATA_SHUTTER */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_split_data_shutter : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SPLIT_DATA_SHUTTER;

typedef struct {
    volatile U_ISP_VIPROC_CTRL0     VIPROC_CTRL0;                                   /* 0x0 */
    volatile unsigned int           reserved_0[3];                                  /* 0x4~0xc */
    volatile U_ISP_VIPROC_REGUP     VIPROC_REGUP;                                   /* 0x10 */
    volatile unsigned int           reserved_1[3];                                  /* 0x14~0x1c */
    volatile U_VIPROC_ISP_BE_CTRL0  VIPROC_ISPBE_CTRL0;                             /* 0x20 */
    volatile U_VIPROC_ISP_BE_CTRL1  VIPROC_ISPBE_CTRL1;                             /* 0x24 */
    volatile unsigned int           reserved_2;                                     /* 0x28 */
    volatile U_VIPROC_ISP_BE_CFG    VIPROC_ISPBE_CFG;                               /* 0x2c */
    volatile unsigned int           reserved_3[48];                                 /* 0x30~0xec */
    volatile U_ISP_VIPROC_INTMASK   VIPROC_INTMASK;                                 /* 0xf0 */
    volatile unsigned int           reserved_4[360];                                /* 0xf4~0x690 */
    volatile U_ISP_VIPROC_PARA_DRC_ADDR_LOW       VIPROC_PARA_DRC_ADDR_LOW;         /* 0x694 */
    volatile U_ISP_VIPROC_PARA_DRC_ADDR_HIGH      VIPROC_PARA_DRC_ADDR_HIGH;        /* 0x698 */
    volatile U_ISP_VIPROC_PARA_DCI_ADDR_LOW       VIPROC_PARA_DCI_ADDR_LOW;         /* 0x69c */
    volatile U_ISP_VIPROC_PARA_DCI_ADDR_HIGH      VIPROC_PARA_DCI_ADDR_HIGH;        /* 0x6a0 */
    volatile U_ISP_VIPROC_OUT_PARA_DRC_ADDR_LOW   VIPROC_OUT_PARA_DRC_ADDR_LOW;     /* 0x6a4 */
    volatile U_ISP_VIPROC_OUT_PARA_DRC_ADDR_HIGH  VIPROC_OUT_PARA_DRC_ADDR_HIGH;    /* 0x6a8 */
    volatile U_ISP_VIPROC_OUT_PARA_DCI_ADDR_LOW   VIPROC_OUT_PARA_DCI_ADDR_LOW;     /* 0x6ac */
    volatile U_ISP_VIPROC_OUT_PARA_DCI_ADDR_HIGH  VIPROC_OUT_PARA_DCI_ADDR_HIGH;    /* 0x6b0 */
    volatile unsigned int           reserved_5[19];                                 /* 0x6b4~0x6fc */
} S_VIPROC_REGS_TYPE;

/* Define the global struct */
typedef struct {
    volatile unsigned int           reserved_23[32]                    ; /* 0x20000~0x2007c */
    volatile U_ISP_BE_VERSION       ISP_BE_VERSION                    ; /* 0x20080 */
    volatile U_ISP_BE_DATE          ISP_BE_DATE                       ; /* 0x20084 */
    volatile U_ISP_BE_FPGA_DATE     ISP_BE_FPGA_DATE                  ; /* 0x20088 */
    volatile unsigned int           reserved_26                     ; /* 0x2008c */
    volatile U_ISP_BE_MODULE_POS    ISP_BE_MODULE_POS                 ; /* 0x20090 */
    volatile U_ISP_BE_FSTART_DELAY   ISP_BE_FSTART_DELAY              ; /* 0x20094 */
    volatile U_ISP_BE_INPUT_MUX     ISP_BE_INPUT_MUX                  ; /* 0x20098 */
    volatile U_ISP_BE_SYS_CTRL      ISP_BE_SYS_CTRL                   ; /* 0x2009c */
    volatile U_ISP_BE_USER_DEFINE0   ISP_BE_USER_DEFINE0              ; /* 0x200a0 */
    volatile U_ISP_BE_USER_DEFINE1   ISP_BE_USER_DEFINE1              ; /* 0x200a4 */
    volatile unsigned int           reserved_27[2]                     ; /* 0x200a8~0x200ac */
    volatile U_ISP_BE_STARTUP       ISP_BE_STARTUP                    ; /* 0x200b0 */
    volatile U_ISP_BE_FORMAT        ISP_BE_FORMAT                     ; /* 0x200b4 */
    volatile unsigned int           reserved_28[466]                     ; /* 0x200b8~0x207fc */
    volatile U_ISP_CLIP_Y_CFG       ISP_CLIP_Y_CFG                    ; /* 0x20800 */
    volatile U_ISP_CLIP_C_CFG       ISP_CLIP_C_CFG                    ; /* 0x20804 */
    volatile unsigned int           reserved_29[6]                     ; /* 0x20808~0x2081c */
    volatile U_ISP_CSC_SUM_CFG      ISP_CSC_SUM_CFG                   ; /* 0x20820 */
    volatile U_ISP_YUV444_SUM_CFG   ISP_YUV444_SUM_CFG                ; /* 0x20824 */
    volatile U_ISP_YUV422_SUM_CFG   ISP_YUV422_SUM_CFG                ; /* 0x20828 */
    volatile unsigned int           reserved_30                     ; /* 0x2082c */
    volatile U_ISP_WDR_SUM_CFG      ISP_WDR_SUM_CFG                   ; /* 0x20830 */
    volatile U_ISP_DEMOSAIC_SUM_CFG   ISP_DEMOSAIC_SUM_CFG            ; /* 0x20834 */
    volatile unsigned int           reserved_31[2]                     ; /* 0x20838~0x2083c */
    volatile U_ISP_DMNR_DITHER      ISP_DMNR_DITHER                   ; /* 0x20840 */
    volatile unsigned int           reserved_32[3]                     ; /* 0x20844~0x2084c */
    volatile U_ISP_ACM_DITHER       ISP_ACM_DITHER                    ; /* 0x20850 */
    volatile unsigned int           reserved_33[3]                     ; /* 0x20854~0x2085c */
    volatile U_ISP_DRC_DITHER       ISP_DRC_DITHER                    ; /* 0x20860 */
    volatile unsigned int           reserved_34[3]                     ; /* 0x20864~0x2086c */
    volatile U_ISP_SQRT1_DITHER     ISP_SQRT1_DITHER                  ; /* 0x20870 */
    volatile unsigned int           reserved_35                     ; /* 0x20874 */
    volatile U_ISP_BLK_HBLANK       ISP_BLK_HBLANK                    ; /* 0x20878 */
    volatile U_ISP_BLK_VBLANK       ISP_BLK_VBLANK                    ; /* 0x2087c */
    volatile U_ISP_Y_SUM0           ISP_Y_SUM0                        ; /* 0x20880 */
    volatile U_ISP_Y_SUM1           ISP_Y_SUM1                        ; /* 0x20884 */
    volatile unsigned int           reserved_36[2]                     ; /* 0x20888~0x2088c */
    volatile U_ISP_YUV444_Y_SUM0    ISP_YUV444_Y_SUM0                 ; /* 0x20890 */
    volatile U_ISP_YUV444_Y_SUM1    ISP_YUV444_Y_SUM1                 ; /* 0x20894 */
    volatile U_ISP_YUV444_U_SUM0    ISP_YUV444_U_SUM0                 ; /* 0x20898 */
    volatile U_ISP_YUV444_U_SUM1    ISP_YUV444_U_SUM1                 ; /* 0x2089c */
    volatile U_ISP_YUV444_V_SUM0    ISP_YUV444_V_SUM0                 ; /* 0x208a0 */
    volatile U_ISP_YUV444_V_SUM1    ISP_YUV444_V_SUM1                 ; /* 0x208a4 */
    volatile unsigned int           reserved_37[2]                     ; /* 0x208a8~0x208ac */
    volatile U_ISP_YUV422_Y_SUM0    ISP_YUV422_Y_SUM0                 ; /* 0x208b0 */
    volatile U_ISP_YUV422_Y_SUM1    ISP_YUV422_Y_SUM1                 ; /* 0x208b4 */
    volatile U_ISP_YUV422_C_SUM0    ISP_YUV422_C_SUM0                 ; /* 0x208b8 */
    volatile U_ISP_YUV422_C_SUM1    ISP_YUV422_C_SUM1                 ; /* 0x208bc */
    volatile U_ISP_WDR0_SUM0        ISP_WDR0_SUM0                     ; /* 0x208c0 */
    volatile U_ISP_WDR0_SUM1        ISP_WDR0_SUM1                     ; /* 0x208c4 */
    volatile U_ISP_WDR1_SUM0        ISP_WDR1_SUM0                     ; /* 0x208c8 */
    volatile U_ISP_WDR1_SUM1        ISP_WDR1_SUM1                     ; /* 0x208cc */
    volatile U_ISP_WDR2_SUM0        ISP_WDR2_SUM0                     ; /* 0x208d0 */
    volatile U_ISP_WDR2_SUM1        ISP_WDR2_SUM1                     ; /* 0x208d4 */
    volatile U_ISP_WDR3_SUM0        ISP_WDR3_SUM0                     ; /* 0x208d8 */
    volatile U_ISP_WDR3_SUM1        ISP_WDR3_SUM1                     ; /* 0x208dc */
    volatile U_ISP_DEMOSAIC_SUM0    ISP_DEMOSAIC_SUM0                 ; /* 0x208e0 */
    volatile U_ISP_DEMOSAIC_SUM1    ISP_DEMOSAIC_SUM1                 ; /* 0x208e4 */
    volatile unsigned int           reserved_38[457]                     ; /* 0x208e8~0x21008 */
    volatile U_ISP_BLC_VERSION      ISP_BLC_VERSION                   ; /* 0x2100c */
    volatile U_ISP_BLC_OFFSET1      ISP_BLC_OFFSET1                   ; /* 0x21010 */
    volatile U_ISP_BLC_OFFSET2      ISP_BLC_OFFSET2                   ; /* 0x21014 */
    volatile unsigned int           reserved_39[123]                     ; /* 0x21018~0x21200 */
    volatile U_ISP_WB_BLC_CFG       ISP_WB_BLC_CFG                    ; /* 0x21204 */
    volatile unsigned int           reserved_40                     ; /* 0x21208 */
    volatile U_ISP_WB_VERSION       ISP_WB_VERSION                    ; /* 0x2120c */
    volatile U_ISP_WB_GAIN1         ISP_WB_GAIN1                      ; /* 0x21210 */
    volatile U_ISP_WB_GAIN2         ISP_WB_GAIN2                      ; /* 0x21214 */
    volatile U_ISP_WB_BLC_OFFSET1   ISP_WB_BLC_OFFSET1                ; /* 0x21218 */
    volatile U_ISP_WB_BLC_OFFSET2   ISP_WB_BLC_OFFSET2                ; /* 0x2121c */
    volatile U_ISP_WB_CLIP_VALUE    ISP_WB_CLIP_VALUE                 ; /* 0x21220 */
    volatile unsigned int           reserved_41[56]                     ; /* 0x21224~0x21300 */
    volatile U_ISP_DG_BLC_CFG       ISP_DG_BLC_CFG                    ; /* 0x21304 */
    volatile unsigned int           reserved_42                     ; /* 0x21308 */
    volatile U_ISP_DG_VERSION       ISP_DG_VERSION                    ; /* 0x2130c */
    volatile U_ISP_DG_GAIN1         ISP_DG_GAIN1                      ; /* 0x21310 */
    volatile U_ISP_DG_GAIN2         ISP_DG_GAIN2                      ; /* 0x21314 */
    volatile U_ISP_DG_BLC_OFFSET1   ISP_DG_BLC_OFFSET1                ; /* 0x21318 */
    volatile U_ISP_DG_BLC_OFFSET2   ISP_DG_BLC_OFFSET2                ; /* 0x2131c */
    volatile U_ISP_DG_CLIP_VALUE    ISP_DG_CLIP_VALUE                 ; /* 0x21320 */
    volatile unsigned int           reserved_43[55]                     ; /* 0x21324~0x213fc */
    volatile U_ISP_CC_CFG           ISP_CC_CFG                        ; /* 0x21400 */
    volatile unsigned int           reserved_44[2]                     ; /* 0x21404~0x21408 */
    volatile U_ISP_CC_VERSION       ISP_CC_VERSION                    ; /* 0x2140c */
    volatile U_ISP_CC_COEF0         ISP_CC_COEF0                      ; /* 0x21410 */
    volatile U_ISP_CC_COEF1         ISP_CC_COEF1                      ; /* 0x21414 */
    volatile U_ISP_CC_COEF2         ISP_CC_COEF2                      ; /* 0x21418 */
    volatile U_ISP_CC_COEF3         ISP_CC_COEF3                      ; /* 0x2141c */
    volatile U_ISP_CC_COEF4         ISP_CC_COEF4                      ; /* 0x21420 */
    volatile U_ISP_CC_CC_THD0       ISP_CC_CC_THD0                    ; /* 0x21424 */
    volatile U_ISP_CC_CC_THD1       ISP_CC_CC_THD1                    ; /* 0x21428 */
    volatile U_ISP_CC_PROT_RATIO    ISP_CC_PROT_RATIO                 ; /* 0x2142c */
    volatile unsigned int           reserved_45[3]                     ; /* 0x21430~0x21438 */
    volatile U_ISP_CC_COLORTONE_RB_GAIN   ISP_CC_COLORTONE_RB_GAIN    ; /* 0x2143c */
    volatile U_ISP_CC_COLORTONE_G_GAIN   ISP_CC_COLORTONE_G_GAIN      ; /* 0x21440 */
    volatile U_ISP_CC_RR_THD0       ISP_CC_RR_THD0                    ; /* 0x21444 */
    volatile U_ISP_CC_RR_THD1       ISP_CC_RR_THD1                    ; /* 0x21448 */
    volatile U_ISP_CC_GG_THD0       ISP_CC_GG_THD0                    ; /* 0x2144c */
    volatile U_ISP_CC_GG_THD1       ISP_CC_GG_THD1                    ; /* 0x21450 */
    volatile U_ISP_CC_BB_THD0       ISP_CC_BB_THD0                    ; /* 0x21454 */
    volatile U_ISP_CC_BB_THD1       ISP_CC_BB_THD1                    ; /* 0x21458 */
    volatile U_ISP_CC_RGB_THD       ISP_CC_RGB_THD                    ; /* 0x2145c */
    volatile U_ISP_CC_RGB_RATIO     ISP_CC_RGB_RATIO                  ; /* 0x21460 */
    volatile U_ISP_CC_IN_DC0        ISP_CC_IN_DC0                     ; /* 0x21464 */
    volatile U_ISP_CC_IN_DC1        ISP_CC_IN_DC1                     ; /* 0x21468 */
    volatile U_ISP_CC_IN_DC2        ISP_CC_IN_DC2                     ; /* 0x2146c */
    volatile U_ISP_CC_OUT_DC0       ISP_CC_OUT_DC0                    ; /* 0x21470 */
    volatile U_ISP_CC_OUT_DC1       ISP_CC_OUT_DC1                    ; /* 0x21474 */
    volatile U_ISP_CC_OUT_DC2       ISP_CC_OUT_DC2                    ; /* 0x21478 */
    volatile unsigned int           reserved_46[36]                     ; /* 0x2147c~0x21508 */
    volatile U_ISP_CSC_VERSION      ISP_CSC_VERSION                   ; /* 0x2150c */
    volatile U_ISP_CSC_COEF0        ISP_CSC_COEF0                     ; /* 0x21510 */
    volatile U_ISP_CSC_COEF1        ISP_CSC_COEF1                     ; /* 0x21514 */
    volatile U_ISP_CSC_COEF2        ISP_CSC_COEF2                     ; /* 0x21518 */
    volatile U_ISP_CSC_COEF3        ISP_CSC_COEF3                     ; /* 0x2151c */
    volatile U_ISP_CSC_COEF4        ISP_CSC_COEF4                     ; /* 0x21520 */
    volatile unsigned int           reserved_47[3]                     ; /* 0x21524~0x2152c */
    volatile U_ISP_CSC_IN_DC0       ISP_CSC_IN_DC0                    ; /* 0x21530 */
    volatile U_ISP_CSC_IN_DC1       ISP_CSC_IN_DC1                    ; /* 0x21534 */
    volatile unsigned int           reserved_48[2]                     ; /* 0x21538~0x2153c */
    volatile U_ISP_CSC_OUT_DC0      ISP_CSC_OUT_DC0                   ; /* 0x21540 */
    volatile U_ISP_CSC_OUT_DC1      ISP_CSC_OUT_DC1                   ; /* 0x21544 */
    volatile unsigned int           reserved_49[174]                     ; /* 0x21548~0x217fc */
    volatile U_ISP_MCDS_CFG         ISP_MCDS_CFG                      ; /* 0x21800 */
    volatile unsigned int           reserved_50[2]                     ; /* 0x21804~0x21808 */
    volatile U_ISP_MCDS_VERSION     ISP_MCDS_VERSION                  ; /* 0x2180c */
    volatile unsigned int           reserved_51[2]                     ; /* 0x21810~0x21814 */
    volatile U_ISP_HCDS_COEFH0      ISP_HCDS_COEFH0                   ; /* 0x21818 */
    volatile U_ISP_HCDS_COEFH1      ISP_HCDS_COEFH1                   ; /* 0x2181c */
    volatile U_ISP_HCDS_COEFH2      ISP_HCDS_COEFH2                   ; /* 0x21820 */
    volatile U_ISP_HCDS_COEFH3      ISP_HCDS_COEFH3                   ; /* 0x21824 */
    volatile U_ISP_VCDS_COEFV       ISP_VCDS_COEFV                    ; /* 0x21828 */
    volatile unsigned int           reserved_52[6]                     ; /* 0x2182c~0x21840 */
    volatile U_ISP_MCDS_PARA        ISP_MCDS_PARA                     ; /* 0x21844 */
    volatile unsigned int           reserved_53[239]                     ; /* 0x21848~0x21c00 */
    volatile U_ISP_4DG_BLC_CFG      ISP_4DG_BLC_CFG                   ; /* 0x21c04 */
    volatile unsigned int           reserved_54                     ; /* 0x21c08 */
    volatile U_ISP_4DG_VERSION      ISP_4DG_VERSION                   ; /* 0x21c0c */
    volatile U_ISP_4DG_0_GAIN1      ISP_4DG_0_GAIN1                   ; /* 0x21c10 */
    volatile U_ISP_4DG_0_GAIN2      ISP_4DG_0_GAIN2                   ; /* 0x21c14 */
    volatile U_ISP_4DG_0_BLC_OFFSET1   ISP_4DG_0_BLC_OFFSET1          ; /* 0x21c18 */
    volatile U_ISP_4DG_0_BLC_OFFSET2   ISP_4DG_0_BLC_OFFSET2          ; /* 0x21c1c */
    volatile U_ISP_4DG_1_GAIN1      ISP_4DG_1_GAIN1                   ; /* 0x21c20 */
    volatile U_ISP_4DG_1_GAIN2      ISP_4DG_1_GAIN2                   ; /* 0x21c24 */
    volatile U_ISP_4DG_1_BLC_OFFSET1   ISP_4DG_1_BLC_OFFSET1          ; /* 0x21c28 */
    volatile U_ISP_4DG_1_BLC_OFFSET2   ISP_4DG_1_BLC_OFFSET2          ; /* 0x21c2c */
    volatile U_ISP_4DG_2_GAIN1      ISP_4DG_2_GAIN1                   ; /* 0x21c30 */
    volatile U_ISP_4DG_2_GAIN2      ISP_4DG_2_GAIN2                   ; /* 0x21c34 */
    volatile U_ISP_4DG_2_BLC_OFFSET1   ISP_4DG_2_BLC_OFFSET1          ; /* 0x21c38 */
    volatile U_ISP_4DG_2_BLC_OFFSET2   ISP_4DG_2_BLC_OFFSET2          ; /* 0x21c3c */
    volatile U_ISP_4DG_3_GAIN1      ISP_4DG_3_GAIN1                   ; /* 0x21c40 */
    volatile U_ISP_4DG_3_GAIN2      ISP_4DG_3_GAIN2                   ; /* 0x21c44 */
    volatile U_ISP_4DG_3_BLC_OFFSET1   ISP_4DG_3_BLC_OFFSET1          ; /* 0x21c48 */
    volatile U_ISP_4DG_3_BLC_OFFSET2   ISP_4DG_3_BLC_OFFSET2          ; /* 0x21c4c */
    volatile U_ISP_4DG_0_CLIP_VALUE   ISP_4DG_0_CLIP_VALUE            ; /* 0x21c50 */
    volatile U_ISP_4DG_1_CLIP_VALUE   ISP_4DG_1_CLIP_VALUE            ; /* 0x21c54 */
    volatile U_ISP_4DG_2_CLIP_VALUE   ISP_4DG_2_CLIP_VALUE            ; /* 0x21c58 */
    volatile U_ISP_4DG_3_CLIP_VALUE   ISP_4DG_3_CLIP_VALUE            ; /* 0x21c5c */
    volatile unsigned int           reserved_55[235]                     ; /* 0x21c60~0x22008 */
    volatile U_ISP_AE_VERSION       ISP_AE_VERSION                    ; /* 0x2200c */
    volatile U_ISP_AE_ZONE          ISP_AE_ZONE                       ; /* 0x22010 */
    volatile U_ISP_AE_SKIP_CRG      ISP_AE_SKIP_CRG                   ; /* 0x22014 */
    volatile U_ISP_AE_TOTAL_STAT    ISP_AE_TOTAL_STAT                 ; /* 0x22018 */
    volatile U_ISP_AE_COUNT_STAT    ISP_AE_COUNT_STAT                 ; /* 0x2201c */
    volatile U_ISP_AE_TOTAL_R_AVER   ISP_AE_TOTAL_R_AVER              ; /* 0x22020 */
    volatile U_ISP_AE_TOTAL_GR_AVER   ISP_AE_TOTAL_GR_AVER            ; /* 0x22024 */
    volatile U_ISP_AE_TOTAL_GB_AVER   ISP_AE_TOTAL_GB_AVER            ; /* 0x22028 */
    volatile U_ISP_AE_TOTAL_B_AVER   ISP_AE_TOTAL_B_AVER              ; /* 0x2202c */
    volatile U_ISP_AE_HIST_HIGH     ISP_AE_HIST_HIGH                  ; /* 0x22030 */
    volatile unsigned int           reserved_56[3]                     ; /* 0x22034~0x2203c */
    volatile U_ISP_AE_BITMOVE       ISP_AE_BITMOVE                    ; /* 0x22040 */
    volatile U_ISP_AE_OFFSET_R      ISP_AE_OFFSET_R                   ; /* 0x22044 */
    volatile U_ISP_AE_OFFSET_GR     ISP_AE_OFFSET_GR                  ; /* 0x22048 */
    volatile U_ISP_AE_OFFSET_GB     ISP_AE_OFFSET_GB                  ; /* 0x2204c */
    volatile U_ISP_AE_OFFSET_B      ISP_AE_OFFSET_B                   ; /* 0x22050 */
    volatile unsigned int           reserved_57[3]                     ; /* 0x22054~0x2205c */
    volatile U_ISP_AE_GAMMA_LIMIT   ISP_AE_GAMMA_LIMIT                ; /* 0x22060 */
    volatile U_ISP_AE_LUT_UPDATE    ISP_AE_LUT_UPDATE                 ; /* 0x22064 */
    volatile unsigned int           reserved_58[2]                     ; /* 0x22068~0x2206c */
    volatile U_ISP_AE_FOURPLANEMODE   ISP_AE_FOURPLANEMODE            ; /* 0x22070 */
    volatile U_ISP_AE_CROP_POS      ISP_AE_CROP_POS                   ; /* 0x22074 */
    volatile U_ISP_AE_CROP_OUTSIZE   ISP_AE_CROP_OUTSIZE              ; /* 0x22078 */
    volatile unsigned int           reserved_59                     ; /* 0x2207c */
    volatile U_ISP_AE_HIST_WADDR    ISP_AE_HIST_WADDR                 ; /* 0x22080 */
    volatile U_ISP_AE_HIST_WDATA    ISP_AE_HIST_WDATA                 ; /* 0x22084 */
    volatile U_ISP_AE_HIST_RADDR    ISP_AE_HIST_RADDR                 ; /* 0x22088 */
    volatile U_ISP_AE_HIST_RDATA    ISP_AE_HIST_RDATA                 ; /* 0x2208c */
    volatile U_ISP_AE_AVER_R_GR_WADDR   ISP_AE_AVER_R_GR_WADDR        ; /* 0x22090 */
    volatile U_ISP_AE_AVER_R_GR_WDATA   ISP_AE_AVER_R_GR_WDATA        ; /* 0x22094 */
    volatile U_ISP_AE_AVER_R_GR_RADDR   ISP_AE_AVER_R_GR_RADDR        ; /* 0x22098 */
    volatile U_ISP_AE_AVER_R_GR_RDATA   ISP_AE_AVER_R_GR_RDATA        ; /* 0x2209c */
    volatile U_ISP_AE_AVER_GB_B_WADDR   ISP_AE_AVER_GB_B_WADDR        ; /* 0x220a0 */
    volatile U_ISP_AE_AVER_GB_B_WDATA   ISP_AE_AVER_GB_B_WDATA        ; /* 0x220a4 */
    volatile U_ISP_AE_AVER_GB_B_RADDR   ISP_AE_AVER_GB_B_RADDR        ; /* 0x220a8 */
    volatile U_ISP_AE_AVER_GB_B_RDATA   ISP_AE_AVER_GB_B_RDATA        ; /* 0x220ac */
    volatile U_ISP_AE_WEIGHT_WADDR   ISP_AE_WEIGHT_WADDR              ; /* 0x220b0 */
    volatile U_ISP_AE_WEIGHT_WDATA   ISP_AE_WEIGHT_WDATA              ; /* 0x220b4 */
    volatile U_ISP_AE_WEIGHT_RADDR   ISP_AE_WEIGHT_RADDR              ; /* 0x220b8 */
    volatile U_ISP_AE_WEIGHT_RDATA   ISP_AE_WEIGHT_RDATA              ; /* 0x220bc */
    volatile unsigned int           reserved_60[19]                     ; /* 0x220c0~0x22108 */
    volatile U_ISP_AWB_VERSION      ISP_AWB_VERSION                   ; /* 0x2210c */
    volatile U_ISP_AWB_ZONE         ISP_AWB_ZONE                      ; /* 0x22110 */
    volatile U_ISP_AWB_BITMOVE      ISP_AWB_BITMOVE                   ; /* 0x22114 */
    volatile U_ISP_AWB_THD_MIN      ISP_AWB_THD_MIN                   ; /* 0x22118 */
    volatile U_ISP_AWB_THD_MAX      ISP_AWB_THD_MAX                   ; /* 0x2211c */
    volatile U_ISP_AWB_CR_MM        ISP_AWB_CR_MM                     ; /* 0x22120 */
    volatile U_ISP_AWB_CB_MM        ISP_AWB_CB_MM                     ; /* 0x22124 */
    volatile U_ISP_AWB_OFFSET_COMP   ISP_AWB_OFFSET_COMP              ; /* 0x22128 */
    volatile unsigned int           reserved_61[5]                     ; /* 0x2212c~0x2213c */
    volatile U_ISP_AWB_AVG_R        ISP_AWB_AVG_R                     ; /* 0x22140 */
    volatile U_ISP_AWB_AVG_G        ISP_AWB_AVG_G                     ; /* 0x22144 */
    volatile U_ISP_AWB_AVG_B        ISP_AWB_AVG_B                     ; /* 0x22148 */
    volatile U_ISP_AWB_CNT_ALL      ISP_AWB_CNT_ALL                   ; /* 0x2214c */
    volatile U_ISP_AWB_HIST_BIN0_1   ISP_AWB_HIST_BIN0_1              ; /* 0x22150 */
    volatile U_ISP_AWB_HIST_BIN2_3   ISP_AWB_HIST_BIN2_3              ; /* 0x22154 */
    volatile U_ISP_AWB_ZONE_BIN     ISP_AWB_ZONE_BIN                  ; /* 0x22158 */
    volatile U_ISP_AWB_HIST_WEIGHT   ISP_AWB_HIST_WEIGHT              ; /* 0x2215c */
    volatile unsigned int           reserved_62[5]                     ; /* 0x22160~0x22170 */
    volatile U_ISP_AWB_CROP_POS     ISP_AWB_CROP_POS                  ; /* 0x22174 */
    volatile U_ISP_AWB_CROP_OUTSIZE   ISP_AWB_CROP_OUTSIZE            ; /* 0x22178 */
    volatile unsigned int           reserved_63                     ; /* 0x2217c */
    volatile U_ISP_AWB_STAT_WADDR   ISP_AWB_STAT_WADDR                ; /* 0x22180 */
    volatile U_ISP_AWB_STAT_WDATA   ISP_AWB_STAT_WDATA                ; /* 0x22184 */
    volatile U_ISP_AWB_STAT_RADDR   ISP_AWB_STAT_RADDR                ; /* 0x22188 */
    volatile U_ISP_AWB_STAT_RDATA   ISP_AWB_STAT_RDATA                ; /* 0x2218c */
    volatile unsigned int           reserved_64[28]                     ; /* 0x22190~0x221fc */
    volatile U_ISP_AF_CFG           ISP_AF_CFG                        ; /* 0x22200 */
    volatile unsigned int           reserved_65[2]                     ; /* 0x22204~0x22208 */
    volatile U_ISP_AF_VERSION       ISP_AF_VERSION                    ; /* 0x2220c */
    volatile U_ISP_AF_ZONE          ISP_AF_ZONE                       ; /* 0x22210 */
    volatile U_ISP_AF_CROP_START    ISP_AF_CROP_START                 ; /* 0x22214 */
    volatile U_ISP_AF_CROP_SIZE     ISP_AF_CROP_SIZE                  ; /* 0x22218 */
    volatile U_ISP_AF_MEAN_THRES    ISP_AF_MEAN_THRES                 ; /* 0x2221c */
    volatile U_ISP_AF_IIRG0         ISP_AF_IIRG0                      ; /* 0x22220 */
    volatile U_ISP_AF_IIRG1         ISP_AF_IIRG1                      ; /* 0x22224 */
    volatile U_ISP_AF_IIRG2         ISP_AF_IIRG2                      ; /* 0x22228 */
    volatile U_ISP_AF_IIRG3         ISP_AF_IIRG3                      ; /* 0x2222c */
    volatile U_ISP_AF_IIRG4         ISP_AF_IIRG4                      ; /* 0x22230 */
    volatile U_ISP_AF_IIRG5         ISP_AF_IIRG5                      ; /* 0x22234 */
    volatile U_ISP_AF_IIRG6         ISP_AF_IIRG6                      ; /* 0x22238 */
    volatile U_ISP_AF_IIRPL         ISP_AF_IIRPL                      ; /* 0x2223c */
    volatile U_ISP_AF_SHIFT         ISP_AF_SHIFT                      ; /* 0x22240 */
    volatile unsigned int           reserved_66[3]                     ; /* 0x22244~0x2224c */
    volatile U_ISP_AF_FIRH0         ISP_AF_FIRH0                      ; /* 0x22250 */
    volatile U_ISP_AF_FIRH1         ISP_AF_FIRH1                      ; /* 0x22254 */
    volatile U_ISP_AF_FIRH2         ISP_AF_FIRH2                      ; /* 0x22258 */
    volatile U_ISP_AF_FIRH3         ISP_AF_FIRH3                      ; /* 0x2225c */
    volatile U_ISP_AF_FIRH4         ISP_AF_FIRH4                      ; /* 0x22260 */
    volatile unsigned int           reserved_67[5]                     ; /* 0x22264~0x22274 */
    volatile U_ISP_AF_ACC_SHIFT     ISP_AF_ACC_SHIFT                  ; /* 0x22278 */
    volatile U_ISP_AF_CNT_SHIFT     ISP_AF_CNT_SHIFT                  ; /* 0x2227c */
    volatile unsigned int           reserved_68[2]                     ; /* 0x22280~0x22284 */
    volatile U_ISP_AF_STAT_H1_RADDR   ISP_AF_STAT_H1_RADDR            ; /* 0x22288 */
    volatile U_ISP_AF_STAT_H1_RDATA   ISP_AF_STAT_H1_RDATA            ; /* 0x2228c */
    volatile unsigned int           reserved_69[2]                     ; /* 0x22290~0x22294 */
    volatile U_ISP_AF_STAT_H2_RADDR   ISP_AF_STAT_H2_RADDR            ; /* 0x22298 */
    volatile U_ISP_AF_STAT_H2_RDATA   ISP_AF_STAT_H2_RDATA            ; /* 0x2229c */
    volatile unsigned int           reserved_70[2]                     ; /* 0x222a0~0x222a4 */
    volatile U_ISP_AF_STAT_V1_RADDR   ISP_AF_STAT_V1_RADDR            ; /* 0x222a8 */
    volatile U_ISP_AF_STAT_V1_RDATA   ISP_AF_STAT_V1_RDATA            ; /* 0x222ac */
    volatile unsigned int           reserved_71[2]                     ; /* 0x222b0~0x222b4 */
    volatile U_ISP_AF_STAT_V2_RADDR   ISP_AF_STAT_V2_RADDR            ; /* 0x222b8 */
    volatile U_ISP_AF_STAT_V2_RDATA   ISP_AF_STAT_V2_RDATA            ; /* 0x222bc */
    volatile unsigned int           reserved_72[2]                     ; /* 0x222c0~0x222c4 */
    volatile U_ISP_AF_STAT_Y_RADDR   ISP_AF_STAT_Y_RADDR              ; /* 0x222c8 */
    volatile U_ISP_AF_STAT_Y_RDATA   ISP_AF_STAT_Y_RDATA              ; /* 0x222cc */
    volatile unsigned int           reserved_73[12]                     ; /* 0x222d0~0x222fc */
    volatile U_ISP_AF_IIRTHRE       ISP_AF_IIRTHRE                    ; /* 0x22300 */
    volatile U_ISP_AF_IIRGAIN       ISP_AF_IIRGAIN                    ; /* 0x22304 */
    volatile U_ISP_AF_IIRSLOPE      ISP_AF_IIRSLOPE                   ; /* 0x22308 */
    volatile U_ISP_AF_IIRDELAY      ISP_AF_IIRDELAY                   ; /* 0x2230c */
    volatile U_ISP_AF_FIRTHRE       ISP_AF_FIRTHRE                    ; /* 0x22310 */
    volatile U_ISP_AF_FIRGAIN       ISP_AF_FIRGAIN                    ; /* 0x22314 */
    volatile U_ISP_AF_FIRSLOPE      ISP_AF_FIRSLOPE                   ; /* 0x22318 */
    volatile unsigned int           reserved_74                     ; /* 0x2231c */
    volatile U_ISP_AF_IIRTHRE_CORING   ISP_AF_IIRTHRE_CORING          ; /* 0x22320 */
    volatile U_ISP_AF_IIRPEAK_CORING   ISP_AF_IIRPEAK_CORING          ; /* 0x22324 */
    volatile U_ISP_AF_IIRSLOPE_CORING   ISP_AF_IIRSLOPE_CORING        ; /* 0x22328 */
    volatile unsigned int           reserved_75                     ; /* 0x2232c */
    volatile U_ISP_AF_FIRTHRE_CORING   ISP_AF_FIRTHRE_CORING          ; /* 0x22330 */
    volatile U_ISP_AF_FIRPEAK_CORING   ISP_AF_FIRPEAK_CORING          ; /* 0x22334 */
    volatile U_ISP_AF_FIRSLOPE_CORING   ISP_AF_FIRSLOPE_CORING        ; /* 0x22338 */
    volatile unsigned int           reserved_76                     ; /* 0x2233c */
    volatile U_ISP_AF_HILIGHT       ISP_AF_HILIGHT                    ; /* 0x22340 */
    volatile U_ISP_AF_OFFSET        ISP_AF_OFFSET                     ; /* 0x22344 */
    volatile unsigned int           reserved_77[46]                     ; /* 0x22348~0x223fc */
    volatile U_ISP_DIS_CFG          ISP_DIS_CFG                       ; /* 0x22400 */
    volatile U_ISP_DIS_BLK          ISP_DIS_BLK                       ; /* 0x22404 */
    volatile unsigned int           reserved_78                     ; /* 0x22408 */
    volatile U_ISP_DIS_VERSION      ISP_DIS_VERSION                   ; /* 0x2240c */
    volatile U_ISP_DIS_V0POS        ISP_DIS_V0POS                     ; /* 0x22410 */
    volatile U_ISP_DIS_V4POS        ISP_DIS_V4POS                     ; /* 0x22414 */
    volatile U_ISP_DIS_V8POS        ISP_DIS_V8POS                     ; /* 0x22418 */
    volatile unsigned int           reserved_79                     ; /* 0x2241c */
    volatile U_ISP_DIS_V0POSE       ISP_DIS_V0POSE                    ; /* 0x22420 */
    volatile U_ISP_DIS_V4POSE       ISP_DIS_V4POSE                    ; /* 0x22424 */
    volatile U_ISP_DIS_V8POSE       ISP_DIS_V8POSE                    ; /* 0x22428 */
    volatile unsigned int           reserved_80                     ; /* 0x2242c */
    volatile U_ISP_DIS_H0POS        ISP_DIS_H0POS                     ; /* 0x22430 */
    volatile U_ISP_DIS_H4POS        ISP_DIS_H4POS                     ; /* 0x22434 */
    volatile U_ISP_DIS_H8POS        ISP_DIS_H8POS                     ; /* 0x22438 */
    volatile unsigned int           reserved_81                     ; /* 0x2243c */
    volatile U_ISP_DIS_H0POSE       ISP_DIS_H0POSE                    ; /* 0x22440 */
    volatile U_ISP_DIS_H4POSE       ISP_DIS_H4POSE                    ; /* 0x22444 */
    volatile U_ISP_DIS_H8POSE       ISP_DIS_H8POSE                    ; /* 0x22448 */
    volatile unsigned int           reserved_82                     ; /* 0x2244c */
    volatile U_ISP_DIS_RAW_LUMA     ISP_DIS_RAW_LUMA                  ; /* 0x22450 */
    volatile U_ISP_DIS_GAMMA_EN     ISP_DIS_GAMMA_EN                  ; /* 0x22454 */
    volatile U_ISP_DIS_GAIN_LMT     ISP_DIS_GAIN_LMT                  ; /* 0x22458 */
    volatile U_ISP_DIS_OFFSET_GR    ISP_DIS_OFFSET_GR                 ; /* 0x2245c */
    volatile U_ISP_DIS_OFFSET_GB    ISP_DIS_OFFSET_GB                 ; /* 0x22460 */
    volatile U_ISP_DIS_SCALE        ISP_DIS_SCALE                     ; /* 0x22464 */
    volatile unsigned int           reserved_83[6]                     ; /* 0x22468~0x2247c */
    volatile U_ISP_DIS_H_STAT_WADDR   ISP_DIS_H_STAT_WADDR            ; /* 0x22480 */
    volatile U_ISP_DIS_H_STAT_WDATA   ISP_DIS_H_STAT_WDATA            ; /* 0x22484 */
    volatile U_ISP_DIS_H_STAT_RADDR   ISP_DIS_H_STAT_RADDR            ; /* 0x22488 */
    volatile U_ISP_DIS_H_STAT_RDATA   ISP_DIS_H_STAT_RDATA            ; /* 0x2248c */
    volatile U_ISP_DIS_V_STAT_WADDR   ISP_DIS_V_STAT_WADDR            ; /* 0x22490 */
    volatile U_ISP_DIS_V_STAT_WDATA   ISP_DIS_V_STAT_WDATA            ; /* 0x22494 */
    volatile U_ISP_DIS_V_STAT_RADDR   ISP_DIS_V_STAT_RADDR            ; /* 0x22498 */
    volatile U_ISP_DIS_V_STAT_RDATA   ISP_DIS_V_STAT_RDATA            ; /* 0x2249c */
    volatile unsigned int           reserved_84[27]                     ; /* 0x224a0~0x22508 */
    volatile U_ISP_LA_VERSION       ISP_LA_VERSION                    ; /* 0x2250c */
    volatile U_ISP_LA_ZONE          ISP_LA_ZONE                       ; /* 0x22510 */
    volatile unsigned int           reserved_85[11]                     ; /* 0x22514~0x2253c */
    volatile U_ISP_LA_BITMOVE       ISP_LA_BITMOVE                    ; /* 0x22540 */
    volatile U_ISP_LA_OFFSET_R      ISP_LA_OFFSET_R                   ; /* 0x22544 */
    volatile U_ISP_LA_OFFSET_GR     ISP_LA_OFFSET_GR                  ; /* 0x22548 */
    volatile U_ISP_LA_OFFSET_GB     ISP_LA_OFFSET_GB                  ; /* 0x2254c */
    volatile U_ISP_LA_OFFSET_B      ISP_LA_OFFSET_B                   ; /* 0x22550 */
    volatile unsigned int           reserved_86[3]                     ; /* 0x22554~0x2255c */
    volatile U_ISP_LA_GAMMA_LIMIT   ISP_LA_GAMMA_LIMIT                ; /* 0x22560 */
    volatile U_ISP_LA_CROP_POS      ISP_LA_CROP_POS                   ; /* 0x22564 */
    volatile U_ISP_LA_CROP_OUTSIZE   ISP_LA_CROP_OUTSIZE              ; /* 0x22568 */
    volatile unsigned int           reserved_87[9]                     ; /* 0x2256c~0x2258c */
    volatile U_ISP_LA_AVER_WADDR    ISP_LA_AVER_WADDR                 ; /* 0x22590 */
    volatile U_ISP_LA_AVER_WDATA    ISP_LA_AVER_WDATA                 ; /* 0x22594 */
    volatile U_ISP_LA_AVER_RADDR    ISP_LA_AVER_RADDR                 ; /* 0x22598 */
    volatile U_ISP_LA_AVER_RDATA    ISP_LA_AVER_RDATA                 ; /* 0x2259c */
    volatile unsigned int           reserved_88[28]                     ; /* 0x225a0~0x2260c */
    volatile U_ISP_FLICK_CTRL       ISP_FLICK_CTRL                    ; /* 0x22610 */
    volatile U_ISP_FLICK_F0_INBLC0   ISP_FLICK_F0_INBLC0              ; /* 0x22614 */
    volatile U_ISP_FLICK_F0_INBLC1   ISP_FLICK_F0_INBLC1              ; /* 0x22618 */
    volatile U_ISP_FLICK_F1_INBLC0   ISP_FLICK_F1_INBLC0              ; /* 0x2261c */
    volatile U_ISP_FLICK_F1_INBLC1   ISP_FLICK_F1_INBLC1              ; /* 0x22620 */
    volatile U_ISP_FLICK_F2_INBLC0   ISP_FLICK_F2_INBLC0              ; /* 0x22624 */
    volatile U_ISP_FLICK_F2_INBLC1   ISP_FLICK_F2_INBLC1              ; /* 0x22628 */
    volatile U_ISP_FLICK_F3_INBLC0   ISP_FLICK_F3_INBLC0              ; /* 0x2262c */
    volatile U_ISP_FLICK_F3_INBLC1   ISP_FLICK_F3_INBLC1              ; /* 0x22630 */
    volatile U_ISP_FLICK_EXPORATIO0   ISP_FLICK_EXPORATIO0            ; /* 0x22634 */
    volatile U_ISP_FLICK_EXPORATIO1   ISP_FLICK_EXPORATIO1            ; /* 0x22638 */
    volatile U_ISP_FLICK_OVERTH     ISP_FLICK_OVERTH                  ; /* 0x2263c */
    volatile U_ISP_FLICK_OVERCOUNTTH   ISP_FLICK_OVERCOUNTTH          ; /* 0x22640 */
    volatile U_ISP_FLICK_GAVG_PRE   ISP_FLICK_GAVG_PRE                ; /* 0x22644 */
    volatile U_ISP_FLICK_COUNTOVER_PRE   ISP_FLICK_COUNTOVER_PRE      ; /* 0x22648 */
    volatile U_ISP_FLICK_COUNTOVER_CUR   ISP_FLICK_COUNTOVER_CUR      ; /* 0x2264c */
    volatile U_ISP_FLICK_GR_DIFF_CUR   ISP_FLICK_GR_DIFF_CUR          ; /* 0x22650 */
    volatile U_ISP_FLICK_GB_DIFF_CUR   ISP_FLICK_GB_DIFF_CUR          ; /* 0x22654 */
    volatile U_ISP_FLICK_GR_ABS_CUR   ISP_FLICK_GR_ABS_CUR            ; /* 0x22658 */
    volatile U_ISP_FLICK_GB_ABS_CUR   ISP_FLICK_GB_ABS_CUR            ; /* 0x2265c */
    volatile unsigned int           reserved_89[8]                     ; /* 0x22660~0x2267c */
    volatile U_ISP_FLICK_GMEAN_WADDR   ISP_FLICK_GMEAN_WADDR          ; /* 0x22680 */
    volatile U_ISP_FLICK_GMEAN_WDATA   ISP_FLICK_GMEAN_WDATA          ; /* 0x22684 */
    volatile U_ISP_FLICK_GMEAN_RADDR   ISP_FLICK_GMEAN_RADDR          ; /* 0x22688 */
    volatile U_ISP_FLICK_GMEAN_RDATA   ISP_FLICK_GMEAN_RDATA          ; /* 0x2268c */
    volatile unsigned int           reserved_90[608]                     ; /* 0x22690~0x2300c */
    volatile U_ISP_LSC_WINNUM       ISP_LSC_WINNUM                    ; /* 0x23010 */
    volatile U_ISP_LSC_WINX_0_23      ISP_LSC_WINX_0_23[24]         ; /* 0x23014 */
    volatile U_ISP_LSC_WIDTH_OFFSET   ISP_LSC_WIDTH_OFFSET            ; /* 0x23074 */
    volatile U_ISP_LSC_MESH         ISP_LSC_MESH                      ; /* 0x23078 */
    volatile U_ISP_LSC_WEIGHT       ISP_LSC_WEIGHT                    ; /* 0x2307c */
    volatile U_ISP_LSC_RGAIN_WADDR   ISP_LSC_RGAIN_WADDR              ; /* 0x23080 */
    volatile U_ISP_LSC_RGAIN_WDATA   ISP_LSC_RGAIN_WDATA              ; /* 0x23084 */
    volatile U_ISP_LSC_RGAIN_RADDR   ISP_LSC_RGAIN_RADDR              ; /* 0x23088 */
    volatile U_ISP_LSC_RGAIN_RDATA   ISP_LSC_RGAIN_RDATA              ; /* 0x2308c */
    volatile U_ISP_LSC_GRGAIN_WADDR   ISP_LSC_GRGAIN_WADDR            ; /* 0x23090 */
    volatile U_ISP_LSC_GRGAIN_WDATA   ISP_LSC_GRGAIN_WDATA            ; /* 0x23094 */
    volatile U_ISP_LSC_GRGAIN_RADDR   ISP_LSC_GRGAIN_RADDR            ; /* 0x23098 */
    volatile U_ISP_LSC_GRGAIN_RDATA   ISP_LSC_GRGAIN_RDATA            ; /* 0x2309c */
    volatile U_ISP_LSC_BGAIN_WADDR   ISP_LSC_BGAIN_WADDR              ; /* 0x230a0 */
    volatile U_ISP_LSC_BGAIN_WDATA   ISP_LSC_BGAIN_WDATA              ; /* 0x230a4 */
    volatile U_ISP_LSC_BGAIN_RADDR   ISP_LSC_BGAIN_RADDR              ; /* 0x230a8 */
    volatile U_ISP_LSC_BGAIN_RDATA   ISP_LSC_BGAIN_RDATA              ; /* 0x230ac */
    volatile U_ISP_LSC_GBGAIN_WADDR   ISP_LSC_GBGAIN_WADDR            ; /* 0x230b0 */
    volatile U_ISP_LSC_GBGAIN_WDATA   ISP_LSC_GBGAIN_WDATA            ; /* 0x230b4 */
    volatile U_ISP_LSC_GBGAIN_RADDR   ISP_LSC_GBGAIN_RADDR            ; /* 0x230b8 */
    volatile U_ISP_LSC_GBGAIN_RDATA   ISP_LSC_GBGAIN_RDATA            ; /* 0x230bc */
    volatile unsigned int           reserved_91[20]                     ; /* 0x230c0~0x2310c */
    volatile U_ISP_LSC_WINX_24_31   ISP_LSC_WINX_24_31[8]             ; /* 0x23110 */
    volatile U_ISP_LSC_WINY             ISP_LSC_WINY[16]                 ; /* 0x23130 */
    volatile U_ISP_LSC_BLCEN        ISP_LSC_BLCEN                     ; /* 0x23170 */
    volatile U_ISP_LSC_BLC0         ISP_LSC_BLC0                      ; /* 0x23174 */
    volatile U_ISP_LSC_BLC1         ISP_LSC_BLC1                      ; /* 0x23178 */
    volatile U_ISP_LSC_LUT_UPDATE   ISP_LSC_LUT_UPDATE                ; /* 0x2317c */
    volatile unsigned int           reserved_92[32]                     ; /* 0x23180~0x231fc */
    volatile U_ISP_GCAC_CFG         ISP_GCAC_CFG                      ; /* 0x23200 */
    volatile unsigned int           reserved_93[2]                     ; /* 0x23204~0x23208 */
    volatile U_ISP_GCAC_VERSION     ISP_GCAC_VERSION                  ; /* 0x2320c */
    volatile U_ISP_GCAC_PARAMA      ISP_GCAC_PARAMA                   ; /* 0x23210 */
    volatile U_ISP_GCAC_PARAMB      ISP_GCAC_PARAMB                   ; /* 0x23214 */
    volatile U_ISP_GCAC_PARAMC      ISP_GCAC_PARAMC                   ; /* 0x23218 */
    volatile U_ISP_GCAC_YNORM       ISP_GCAC_YNORM                    ; /* 0x2321c */
    volatile U_ISP_GCAC_XNORM       ISP_GCAC_XNORM                    ; /* 0x23220 */
    volatile U_ISP_GCAC_CNTSTART    ISP_GCAC_CNTSTART                 ; /* 0x23224 */
    volatile U_ISP_GCAC_CORSTART    ISP_GCAC_CORSTART                 ; /* 0x23228 */
    volatile U_ISP_GCAC_FILTEN      ISP_GCAC_FILTEN                   ; /* 0x2322c */
    volatile U_ISP_GCAC_CHRVMODE    ISP_GCAC_CHRVMODE                 ; /* 0x23230 */
    volatile U_ISP_GCAC_CLIPMODE    ISP_GCAC_CLIPMODE                 ; /* 0x23234 */
    volatile unsigned int           reserved_94[306]                     ; /* 0x23238~0x236fc */
    volatile U_ISP_RLSC_CFG         ISP_RLSC_CFG                      ; /* 0x23700 */
    volatile unsigned int           reserved_95[2]                     ; /* 0x23704~0x23708 */
    volatile U_ISP_RLSC_VERSION     ISP_RLSC_VERSION                  ; /* 0x2370c */
    volatile U_ISP_RLSC_WIDTHOFFSET   ISP_RLSC_WIDTHOFFSET            ; /* 0x23710 */
    volatile U_ISP_RLSC_POSR        ISP_RLSC_POSR                     ; /* 0x23714 */
    volatile U_ISP_RLSC_POSGR       ISP_RLSC_POSGR                    ; /* 0x23718 */
    volatile U_ISP_RLSC_POSGB       ISP_RLSC_POSGB                    ; /* 0x2371c */
    volatile U_ISP_RLSC_POSB        ISP_RLSC_POSB                     ; /* 0x23720 */
    volatile U_ISP_RLSC_OFFCENTERR   ISP_RLSC_OFFCENTERR              ; /* 0x23724 */
    volatile U_ISP_RLSC_OFFCENTERGR   ISP_RLSC_OFFCENTERGR            ; /* 0x23728 */
    volatile U_ISP_RLSC_OFFCENTERGB   ISP_RLSC_OFFCENTERGB            ; /* 0x2372c */
    volatile U_ISP_RLSC_OFFCENTERB   ISP_RLSC_OFFCENTERB              ; /* 0x23730 */
    volatile U_ISP_RLSC_VALIDRADIUS   ISP_RLSC_VALIDRADIUS            ; /* 0x23734 */
    volatile U_ISP_RLSC_GAIN        ISP_RLSC_GAIN                     ; /* 0x23738 */
    volatile U_ISP_RLSC_BLCOFFSETR   ISP_RLSC_BLCOFFSETR              ; /* 0x2373c */
    volatile U_ISP_RLSC_BLCOFFSETGR   ISP_RLSC_BLCOFFSETGR            ; /* 0x23740 */
    volatile U_ISP_RLSC_BLCOFFSETGB   ISP_RLSC_BLCOFFSETGB            ; /* 0x23744 */
    volatile U_ISP_RLSC_BLCOFFSETB   ISP_RLSC_BLCOFFSETB              ; /* 0x23748 */
    volatile U_ISP_RLSC_LUT_UPDATE   ISP_RLSC_LUT_UPDATE              ; /* 0x2374c */
    volatile unsigned int           reserved_96[12]                     ; /* 0x23750~0x2377c */
    volatile U_ISP_RLSC_LUT0_WADDR   ISP_RLSC_LUT0_WADDR              ; /* 0x23780 */
    volatile U_ISP_RLSC_LUT0_WDATA   ISP_RLSC_LUT0_WDATA              ; /* 0x23784 */
    volatile U_ISP_RLSC_LUT0_RADDR   ISP_RLSC_LUT0_RADDR              ; /* 0x23788 */
    volatile U_ISP_RLSC_LUT0_RDATA   ISP_RLSC_LUT0_RDATA              ; /* 0x2378c */
    volatile U_ISP_RLSC_LUT1_WADDR   ISP_RLSC_LUT1_WADDR              ; /* 0x23790 */
    volatile U_ISP_RLSC_LUT1_WDATA   ISP_RLSC_LUT1_WDATA              ; /* 0x23794 */
    volatile U_ISP_RLSC_LUT1_RADDR   ISP_RLSC_LUT1_RADDR              ; /* 0x23798 */
    volatile U_ISP_RLSC_LUT1_RDATA   ISP_RLSC_LUT1_RDATA              ; /* 0x2379c */
    volatile unsigned int           reserved_97[88]                     ; /* 0x237a0~0x238fc */
    volatile U_ISP_GE_CFG           ISP_GE_CFG                        ; /* 0x23900 */
    volatile unsigned int           reserved_98[2]                     ; /* 0x23904~0x23908 */
    volatile U_ISP_GE_VERSION       ISP_GE_VERSION                    ; /* 0x2390c */
    volatile U_ISP_GE0_CT_TH1       ISP_GE0_CT_TH1                    ; /* 0x23910 */
    volatile U_ISP_GE0_CT_TH2       ISP_GE0_CT_TH2                    ; /* 0x23914 */
    volatile U_ISP_GE0_CT_TH3       ISP_GE0_CT_TH3                    ; /* 0x23918 */
    volatile U_ISP_GE0_CT_SLOPE     ISP_GE0_CT_SLOPE                  ; /* 0x2391c */
    volatile U_ISP_GE1_CT_TH1       ISP_GE1_CT_TH1                    ; /* 0x23920 */
    volatile U_ISP_GE1_CT_TH2       ISP_GE1_CT_TH2                    ; /* 0x23924 */
    volatile U_ISP_GE1_CT_TH3       ISP_GE1_CT_TH3                    ; /* 0x23928 */
    volatile U_ISP_GE1_CT_SLOPE     ISP_GE1_CT_SLOPE                  ; /* 0x2392c */
    volatile U_ISP_GE2_CT_TH1       ISP_GE2_CT_TH1                    ; /* 0x23930 */
    volatile U_ISP_GE2_CT_TH2       ISP_GE2_CT_TH2                    ; /* 0x23934 */
    volatile U_ISP_GE2_CT_TH3       ISP_GE2_CT_TH3                    ; /* 0x23938 */
    volatile U_ISP_GE2_CT_SLOPE     ISP_GE2_CT_SLOPE                  ; /* 0x2393c */
    volatile U_ISP_GE3_CT_TH1       ISP_GE3_CT_TH1                    ; /* 0x23940 */
    volatile U_ISP_GE3_CT_TH2       ISP_GE3_CT_TH2                    ; /* 0x23944 */
    volatile U_ISP_GE3_CT_TH3       ISP_GE3_CT_TH3                    ; /* 0x23948 */
    volatile U_ISP_GE3_CT_SLOPE     ISP_GE3_CT_SLOPE                  ; /* 0x2394c */
    volatile U_ISP_GE_ZONE          ISP_GE_ZONE                       ; /* 0x23950 */
    volatile U_ISP_GE_MODE          ISP_GE_MODE                       ; /* 0x23954 */
    volatile U_ISP_GE_STRENGTH      ISP_GE_STRENGTH                   ; /* 0x23958 */
    volatile U_ISP_GE_CROP_POS      ISP_GE_CROP_POS                   ; /* 0x2395c */
    volatile U_ISP_GE_CROP_OUTSIZE   ISP_GE_CROP_OUTSIZE              ; /* 0x23960 */
    volatile U_ISP_GE_BIT_DEPTH_SEL   ISP_GE_BIT_DEPTH_SEL            ; /* 0x23964 */
    volatile unsigned int           reserved_99[6]                     ; /* 0x23968~0x2397c */
    volatile U_ISP_GE_MEM_AVER0_WADDR   ISP_GE_MEM_AVER0_WADDR        ; /* 0x23980 */
    volatile U_ISP_GE_MEM_AVER0_WDATA   ISP_GE_MEM_AVER0_WDATA        ; /* 0x23984 */
    volatile U_ISP_GE_MEM_AVER0_RADDR   ISP_GE_MEM_AVER0_RADDR        ; /* 0x23988 */
    volatile U_ISP_GE_MEM_AVER0_RDATA   ISP_GE_MEM_AVER0_RDATA        ; /* 0x2398c */
    volatile U_ISP_GE_MEM_AVER1_WADDR   ISP_GE_MEM_AVER1_WADDR        ; /* 0x23990 */
    volatile U_ISP_GE_MEM_AVER1_WDATA   ISP_GE_MEM_AVER1_WDATA        ; /* 0x23994 */
    volatile U_ISP_GE_MEM_AVER1_RADDR   ISP_GE_MEM_AVER1_RADDR        ; /* 0x23998 */
    volatile U_ISP_GE_MEM_AVER1_RDATA   ISP_GE_MEM_AVER1_RDATA        ; /* 0x2399c */
    volatile U_ISP_GE_MEM_AVER2_WADDR   ISP_GE_MEM_AVER2_WADDR        ; /* 0x239a0 */
    volatile U_ISP_GE_MEM_AVER2_WDATA   ISP_GE_MEM_AVER2_WDATA        ; /* 0x239a4 */
    volatile U_ISP_GE_MEM_AVER2_RADDR   ISP_GE_MEM_AVER2_RADDR        ; /* 0x239a8 */
    volatile U_ISP_GE_MEM_AVER2_RDATA   ISP_GE_MEM_AVER2_RDATA        ; /* 0x239ac */
    volatile U_ISP_GE_MEM_AVER3_WADDR   ISP_GE_MEM_AVER3_WADDR        ; /* 0x239b0 */
    volatile U_ISP_GE_MEM_AVER3_WDATA   ISP_GE_MEM_AVER3_WDATA        ; /* 0x239b4 */
    volatile U_ISP_GE_MEM_AVER3_RADDR   ISP_GE_MEM_AVER3_RADDR        ; /* 0x239b8 */
    volatile U_ISP_GE_MEM_AVER3_RDATA   ISP_GE_MEM_AVER3_RDATA        ; /* 0x239bc */
    volatile unsigned int           reserved_100[16]                     ; /* 0x239c0~0x239fc */
    volatile U_ISP_FPN_CFG          ISP_FPN_CFG                       ; /* 0x23a00 */
    volatile U_ISP_FPN_CALIB_START   ISP_FPN_CALIB_START              ; /* 0x23a04 */
    volatile U_ISP_FPN_CORR_CFG     ISP_FPN_CORR_CFG                  ; /* 0x23a08 */
    volatile U_ISP_FPN_STAT         ISP_FPN_STAT                      ; /* 0x23a0c */
    volatile U_ISP_FPN_WHITE_LEVEL   ISP_FPN_WHITE_LEVEL              ; /* 0x23a10 */
    volatile unsigned int           reserved_101                     ; /* 0x23a14 */
    volatile U_ISP_FPN_DIVCOEF      ISP_FPN_DIVCOEF                   ; /* 0x23a18 */
    volatile U_ISP_FPN_FRAMELOG2    ISP_FPN_FRAMELOG2                 ; /* 0x23a1c */
    volatile U_ISP_FPN_SUM0         ISP_FPN_SUM0                      ; /* 0x23a20 */
    volatile U_ISP_FPN_SUM1         ISP_FPN_SUM1                      ; /* 0x23a24 */
    volatile unsigned int           reserved_102[2]                     ; /* 0x23a28~0x23a2c */
    volatile U_ISP_FPN_CORR0        ISP_FPN_CORR0                     ; /* 0x23a30 */
    volatile U_ISP_FPN_CORR1        ISP_FPN_CORR1                     ; /* 0x23a34 */
    volatile U_ISP_FPN_CORR2        ISP_FPN_CORR2                     ; /* 0x23a38 */
    volatile U_ISP_FPN_CORR3        ISP_FPN_CORR3                     ; /* 0x23a3c */
    volatile U_ISP_FPN_SHIFT        ISP_FPN_SHIFT                     ; /* 0x23a40 */
    volatile unsigned int           reserved_103[3]                     ; /* 0x23a44~0x23a4c */
    volatile U_ISP_FPN_MAX_O        ISP_FPN_MAX_O                     ; /* 0x23a50 */
    volatile U_ISP_FPN_OVERFLOWTHR   ISP_FPN_OVERFLOWTHR              ; /* 0x23a54 */
    volatile unsigned int           reserved_104[2]                     ; /* 0x23a58~0x23a5c */
    volatile U_ISP_FPN_BLACK_FRAME_OUT_SEL   ISP_FPN_BLACK_FRAME_OUT_SEL ; /* 0x23a60 */
    volatile U_ISP_FPN_OVERLAP_CFG   ISP_FPN_OVERLAP_CFG              ; /* 0x23a64 */
    volatile U_ISP_FPN_CALIB_OFFLINE_SET   ISP_FPN_CALIB_OFFLINE_SET  ; /* 0x23a68 */
    volatile U_ISP_FPN_CALIB_STAT_CLEAR   ISP_FPN_CALIB_STAT_CLEAR    ; /* 0x23a6c */
    volatile unsigned int           reserved_105[4]                     ; /* 0x23a70~0x23a7c */
    volatile U_ISP_FPN_LINE_WADDR   ISP_FPN_LINE_WADDR                ; /* 0x23a80 */
    volatile U_ISP_FPN_LINE_WDATA   ISP_FPN_LINE_WDATA                ; /* 0x23a84 */
    volatile U_ISP_FPN_LINE_RADDR   ISP_FPN_LINE_RADDR                ; /* 0x23a88 */
    volatile U_ISP_FPN_LINE_RDATA   ISP_FPN_LINE_RDATA                ; /* 0x23a8c */
    volatile unsigned int           reserved_106[92]                     ; /* 0x23a90~0x23bfc */
    volatile U_ISP_DPC_CFG          ISP_DPC_CFG                       ; /* 0x23c00 */
    volatile unsigned int           reserved_107                     ; /* 0x23c04 */
    volatile U_ISP_DPC_ALPHA        ISP_DPC_ALPHA                     ; /* 0x23c08 */
    volatile U_ISP_DPC_VERSION      ISP_DPC_VERSION                   ; /* 0x23c0c */
    volatile U_ISP_DPC_MODE         ISP_DPC_MODE                      ; /* 0x23c10 */
    volatile U_ISP_DPC_OUTPUT_MODE   ISP_DPC_OUTPUT_MODE              ; /* 0x23c14 */
    volatile U_ISP_DPC_SET_USE      ISP_DPC_SET_USE                   ; /* 0x23c18 */
    volatile U_ISP_DPC_METHODS_SET_1   ISP_DPC_METHODS_SET_1          ; /* 0x23c1c */
    volatile U_ISP_DPC_METHODS_SET_2   ISP_DPC_METHODS_SET_2          ; /* 0x23c20 */
    volatile U_ISP_DPC_METHODS_SET_3   ISP_DPC_METHODS_SET_3          ; /* 0x23c24 */
    volatile U_ISP_DPC_LINE_THRESH_1   ISP_DPC_LINE_THRESH_1          ; /* 0x23c28 */
    volatile U_ISP_DPC_LINE_MAD_FAC_1   ISP_DPC_LINE_MAD_FAC_1        ; /* 0x23c2c */
    volatile U_ISP_DPC_PG_FAC_1     ISP_DPC_PG_FAC_1                  ; /* 0x23c30 */
    volatile U_ISP_DPC_RND_THRESH_1   ISP_DPC_RND_THRESH_1            ; /* 0x23c34 */
    volatile U_ISP_DPC_RG_FAC_1     ISP_DPC_RG_FAC_1                  ; /* 0x23c38 */
    volatile U_ISP_DPC_LINE_THRESH_2   ISP_DPC_LINE_THRESH_2          ; /* 0x23c3c */
    volatile U_ISP_DPC_LINE_MAD_FAC_2   ISP_DPC_LINE_MAD_FAC_2        ; /* 0x23c40 */
    volatile U_ISP_DPC_PG_FAC_2     ISP_DPC_PG_FAC_2                  ; /* 0x23c44 */
    volatile U_ISP_DPC_RND_THRESH_2   ISP_DPC_RND_THRESH_2            ; /* 0x23c48 */
    volatile U_ISP_DPC_RG_FAC_2     ISP_DPC_RG_FAC_2                  ; /* 0x23c4c */
    volatile U_ISP_DPC_LINE_THRESH_3   ISP_DPC_LINE_THRESH_3          ; /* 0x23c50 */
    volatile U_ISP_DPC_LINE_MAD_FAC_3   ISP_DPC_LINE_MAD_FAC_3        ; /* 0x23c54 */
    volatile U_ISP_DPC_PG_FAC_3     ISP_DPC_PG_FAC_3                  ; /* 0x23c58 */
    volatile U_ISP_DPC_RND_THRESH_3   ISP_DPC_RND_THRESH_3            ; /* 0x23c5c */
    volatile U_ISP_DPC_RG_FAC_3     ISP_DPC_RG_FAC_3                  ; /* 0x23c60 */
    volatile U_ISP_DPC_RO_LIMITS    ISP_DPC_RO_LIMITS                 ; /* 0x23c64 */
    volatile U_ISP_DPC_RND_OFFS     ISP_DPC_RND_OFFS                  ; /* 0x23c68 */
    volatile U_ISP_DPC_BPT_CTRL     ISP_DPC_BPT_CTRL                  ; /* 0x23c6c */
    volatile U_ISP_DPC_BPT_NUMBER   ISP_DPC_BPT_NUMBER                ; /* 0x23c70 */
    volatile U_ISP_DPC_BPT_CALIB_NUMBER   ISP_DPC_BPT_CALIB_NUMBER    ; /* 0x23c74 */
    volatile unsigned int           reserved_108                     ; /* 0x23c78 */
    volatile U_ISP_DPC_BPT_THRD     ISP_DPC_BPT_THRD                  ; /* 0x23c7c */
    volatile U_ISP_DPC_BPT_WADDR    ISP_DPC_BPT_WADDR                 ; /* 0x23c80 */
    volatile U_ISP_DPC_BPT_WDATA    ISP_DPC_BPT_WDATA                 ; /* 0x23c84 */
    volatile U_ISP_DPC_BPT_RADDR    ISP_DPC_BPT_RADDR                 ; /* 0x23c88 */
    volatile U_ISP_DPC_BPT_RDATA    ISP_DPC_BPT_RDATA                 ; /* 0x23c8c */
    volatile unsigned int           reserved_109[32]                     ; /* 0x23c90~0x23d0c */
    volatile U_ISP_DPC_SOFT_THR     ISP_DPC_SOFT_THR                  ; /* 0x23d10 */
    volatile U_ISP_DPC_BHARDTHR_EN   ISP_DPC_BHARDTHR_EN              ; /* 0x23d14 */
    volatile U_ISP_DPC_RAKERATIO    ISP_DPC_RAKERATIO                 ; /* 0x23d18 */
    volatile U_ISP_DPC_LUT_UPDATE   ISP_DPC_LUT_UPDATE                ; /* 0x23d1c */
    volatile U_ISP_DPC_LCH_THR_1    ISP_DPC_LCH_THR_1                 ; /* 0x23d20 */
    volatile U_ISP_DPC_LCH_THR_2    ISP_DPC_LCH_THR_2                 ; /* 0x23d24 */
    volatile U_ISP_DPC_LCH_THR_3    ISP_DPC_LCH_THR_3                 ; /* 0x23d28 */
    volatile U_ISP_DPC_LCH_THR_4    ISP_DPC_LCH_THR_4                 ; /* 0x23d2c */
    volatile U_ISP_DPC_LCH_THR_5    ISP_DPC_LCH_THR_5                 ; /* 0x23d30 */
    volatile U_ISP_DPC_LINE_KERDIFF_FAC   ISP_DPC_LINE_KERDIFF_FAC    ; /* 0x23d34 */
    volatile U_ISP_DPC_BLEND_MODE   ISP_DPC_BLEND_MODE                ; /* 0x23d38 */
    volatile U_ISP_DPC_BIT_DEPTH_SEL   ISP_DPC_BIT_DEPTH_SEL          ; /* 0x23d3c */
    volatile unsigned int           reserved_110[177]                     ; /* 0x23d40~0x24000 */
    volatile U_ISP_DEMOSAIC_CFG1    ISP_DEMOSAIC_CFG1                 ; /* 0x24004 */
    volatile unsigned int           reserved_111                     ; /* 0x24008 */
    volatile U_ISP_DEMOSAIC_VERSION   ISP_DEMOSAIC_VERSION            ; /* 0x2400c */
    volatile U_ISP_DEMOSAIC_COEF0   ISP_DEMOSAIC_COEF0                ; /* 0x24010 */
    volatile U_ISP_DEMOSAIC_COEF1   ISP_DEMOSAIC_COEF1                ; /* 0x24014 */
    volatile U_ISP_DEMOSAIC_COEF2   ISP_DEMOSAIC_COEF2                ; /* 0x24018 */
    volatile U_ISP_DEMOSAIC_COEF3   ISP_DEMOSAIC_COEF3                ; /* 0x2401c */
    volatile U_ISP_DEMOSAIC_COEF4   ISP_DEMOSAIC_COEF4                ; /* 0x24020 */
    volatile U_ISP_DEMOSAIC_COEF5   ISP_DEMOSAIC_COEF5                ; /* 0x24024 */
    volatile U_ISP_DEMOSAIC_COEF6   ISP_DEMOSAIC_COEF6                ; /* 0x24028 */
    volatile unsigned int           reserved_112                     ; /* 0x2402c */
    volatile U_ISP_DEMOSAIC_SEL     ISP_DEMOSAIC_SEL                  ; /* 0x24030 */
    volatile unsigned int           reserved_113                     ; /* 0x24034 */
    volatile U_ISP_DEMOSAIC_LCAC_CNT_THR   ISP_DEMOSAIC_LCAC_CNT_THR  ; /* 0x24038 */
    volatile U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR   ISP_DEMOSAIC_LCAC_LUMA_RB_THR ; /* 0x2403c */
    volatile U_ISP_DEMOSAIC_LCAC_LUMA_G_THR   ISP_DEMOSAIC_LCAC_LUMA_G_THR ; /* 0x24040 */
    volatile U_ISP_DEMOSAIC_PURPLE_VAR_THR   ISP_DEMOSAIC_PURPLE_VAR_THR ; /* 0x24044 */
    volatile U_ISP_DEMOSAIC_FAKE_CR_VAR_THR   ISP_DEMOSAIC_FAKE_CR_VAR_THR ; /* 0x24048 */
    volatile U_ISP_DEMOSAIC_DEPURPLECTR   ISP_DEMOSAIC_DEPURPLECTR    ; /* 0x2404c */
    volatile U_ISP_DEMOSAIC_LPF_COEF   ISP_DEMOSAIC_LPF_COEF          ; /* 0x24050 */
    volatile U_ISP_DEMOSAIC_G_CLIP_BIT   ISP_DEMOSAIC_G_CLIP_BIT      ; /* 0x24054 */
    volatile U_ISP_DEMOSAIC_CBCRAVGTHLD   ISP_DEMOSAIC_CBCRAVGTHLD    ; /* 0x24058 */
    volatile unsigned int           reserved_114                     ; /* 0x2405c */
    volatile U_ISP_DEMOSAIC_CC_HF_RATIO   ISP_DEMOSAIC_CC_HF_RATIO    ; /* 0x24060 */
    volatile U_ISP_DEMOSAIC_GVAR_BLEND_THLD   ISP_DEMOSAIC_GVAR_BLEND_THLD ; /* 0x24064 */
    volatile U_ISP_DEMOSAIC_SATU_THR   ISP_DEMOSAIC_SATU_THR          ; /* 0x24068 */
    volatile U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT   ISP_DEMOSAIC_CBCR_RATIO_LIMIT ; /* 0x2406c */
    volatile U_ISP_DEMOSAIC_FCR_GRAY_RATIO   ISP_DEMOSAIC_FCR_GRAY_RATIO ; /* 0x24070 */
    volatile U_ISP_DEMOSAIC_FCR_SEL   ISP_DEMOSAIC_FCR_SEL            ; /* 0x24074 */
    volatile U_ISP_DEMOSAIC_CX_VAR_RATE   ISP_DEMOSAIC_CX_VAR_RATE    ; /* 0x24078 */
    volatile unsigned int           reserved_115[37]                     ; /* 0x2407c~0x2410c */
    volatile U_ISP_DEMOSAIC_HF_INTP_TH   ISP_DEMOSAIC_HF_INTP_TH      ; /* 0x24110 */
    volatile U_ISP_DEMOSAIC_HF_INTP_BLD   ISP_DEMOSAIC_HF_INTP_BLD    ; /* 0x24114 */
    volatile U_ISP_DEMOSAIC_HF_INTP_TH1   ISP_DEMOSAIC_HF_INTP_TH1    ; /* 0x24118 */
    volatile U_ISP_DEMOSAIC_HF_INTP_RATIO0   ISP_DEMOSAIC_HF_INTP_RATIO0 ; /* 0x2411c */
    volatile U_ISP_DEMOSAIC_HF_INTP_RATIO1   ISP_DEMOSAIC_HF_INTP_RATIO1 ; /* 0x24120 */
    volatile unsigned int           reserved_116[5]                     ; /* 0x24124~0x24134 */
    volatile U_ISP_DEMOSAIC_CAC_CBCR_THR   ISP_DEMOSAIC_CAC_CBCR_THR  ; /* 0x24138 */
    volatile U_ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR   ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR ; /* 0x2413c */
    volatile U_ISP_DEMOSAIC_CAC_CNT_CFG   ISP_DEMOSAIC_CAC_CNT_CFG    ; /* 0x24140 */
    volatile U_ISP_DEMOSAIC_CAC_BLD_AVG   ISP_DEMOSAIC_CAC_BLD_AVG    ; /* 0x24144 */
    volatile unsigned int           reserved_117                     ; /* 0x24148 */
    volatile U_ISP_DEMOSAIC_FCR_HF_THR   ISP_DEMOSAIC_FCR_HF_THR      ; /* 0x2414c */
    volatile unsigned int           reserved_118[1071]                     ; /* 0x24150~0x25208 */
    volatile U_ISP_SHARPEN_VERSION   ISP_SHARPEN_VERSION              ; /* 0x2520c */
    volatile U_ISP_SHARPEN_CTRL     ISP_SHARPEN_CTRL                  ; /* 0x25210 */
    volatile U_ISP_SHARPEN_MFTHD    ISP_SHARPEN_MFTHD                 ; /* 0x25214 */
    volatile U_ISP_SHARPEN_HFTHD    ISP_SHARPEN_HFTHD                 ; /* 0x25218 */
    volatile U_ISP_SHARPEN_DIRVAR   ISP_SHARPEN_DIRVAR                ; /* 0x2521c */
    volatile U_ISP_SHARPEN_DIRDIFF   ISP_SHARPEN_DIRDIFF              ; /* 0x25220 */
    volatile U_ISP_SHARPEN_LUMAWGT0   ISP_SHARPEN_LUMAWGT0            ; /* 0x25224 */
    volatile U_ISP_SHARPEN_LUMAWGT1   ISP_SHARPEN_LUMAWGT1            ; /* 0x25228 */
    volatile U_ISP_SHARPEN_LUMAWGT2   ISP_SHARPEN_LUMAWGT2            ; /* 0x2522c */
    volatile U_ISP_SHARPEN_LUMAWGT3   ISP_SHARPEN_LUMAWGT3            ; /* 0x25230 */
    volatile U_ISP_SHARPEN_LUMAWGT4   ISP_SHARPEN_LUMAWGT4            ; /* 0x25234 */
    volatile U_ISP_SHARPEN_LUMAWGT5   ISP_SHARPEN_LUMAWGT5            ; /* 0x25238 */
    volatile U_ISP_SHARPEN_LUMAWGT6   ISP_SHARPEN_LUMAWGT6            ; /* 0x2523c */
    volatile U_ISP_SHARPEN_LUMAWGT7   ISP_SHARPEN_LUMAWGT7            ; /* 0x25240 */
    volatile U_ISP_SHARPEN_SHOOT_AMT   ISP_SHARPEN_SHOOT_AMT          ; /* 0x25244 */
    volatile U_ISP_SHARPEN_SHOOT_MAXCHG   ISP_SHARPEN_SHOOT_MAXCHG    ; /* 0x25248 */
    volatile U_ISP_SHARPEN_SHTVAR   ISP_SHARPEN_SHTVAR                ; /* 0x2524c */
    volatile U_ISP_SHARPEN_SHTNOS   ISP_SHARPEN_SHTNOS                ; /* 0x25250 */
    volatile U_ISP_SHARPEN_OSHTVAR   ISP_SHARPEN_OSHTVAR              ; /* 0x25254 */
    volatile U_ISP_SHARPEN_USHTVAR   ISP_SHARPEN_USHTVAR              ; /* 0x25258 */
    volatile U_ISP_SHARPEN_SHTVAR_MUL   ISP_SHARPEN_SHTVAR_MUL        ; /* 0x2525c */
    volatile U_ISP_SHARPEN_OSHTVARDIFF   ISP_SHARPEN_OSHTVARDIFF      ; /* 0x25260 */
    volatile U_ISP_SHARPEN_USHTVARDIFF   ISP_SHARPEN_USHTVARDIFF      ; /* 0x25264 */
    volatile U_ISP_SHARPEN_SHTVARDIFF_MUL   ISP_SHARPEN_SHTVARDIFF_MUL ; /* 0x25268 */
    volatile U_ISP_SHARPEN_LMTMF0   ISP_SHARPEN_LMTMF0                ; /* 0x2526c */
    volatile U_ISP_SHARPEN_LMTMF1   ISP_SHARPEN_LMTMF1                ; /* 0x25270 */
    volatile U_ISP_SHARPEN_LMTHF0   ISP_SHARPEN_LMTHF0                ; /* 0x25274 */
    volatile U_ISP_SHARPEN_LMTHF1   ISP_SHARPEN_LMTHF1                ; /* 0x25278 */
    volatile U_ISP_SHARPEN_LUT_UPDATE   ISP_SHARPEN_LUT_UPDATE        ; /* 0x2527c */
    volatile U_ISP_SHARPEN_MFGAIND_WADDR   ISP_SHARPEN_MFGAIND_WADDR  ; /* 0x25280 */
    volatile U_ISP_SHARPEN_MFGAIND_WDATA   ISP_SHARPEN_MFGAIND_WDATA  ; /* 0x25284 */
    volatile U_ISP_SHARPEN_MFGAIND_RADDR   ISP_SHARPEN_MFGAIND_RADDR  ; /* 0x25288 */
    volatile U_ISP_SHARPEN_MFGAIND_RDATA   ISP_SHARPEN_MFGAIND_RDATA  ; /* 0x2528c */
    volatile U_ISP_SHARPEN_MFGAINUD_WADDR   ISP_SHARPEN_MFGAINUD_WADDR ; /* 0x25290 */
    volatile U_ISP_SHARPEN_MFGAINUD_WDATA   ISP_SHARPEN_MFGAINUD_WDATA ; /* 0x25294 */
    volatile U_ISP_SHARPEN_MFGAINUD_RADDR   ISP_SHARPEN_MFGAINUD_RADDR ; /* 0x25298 */
    volatile U_ISP_SHARPEN_MFGAINUD_RDATA   ISP_SHARPEN_MFGAINUD_RDATA ; /* 0x2529c */
    volatile U_ISP_SHARPEN_HFGAIND_WADDR   ISP_SHARPEN_HFGAIND_WADDR  ; /* 0x252a0 */
    volatile U_ISP_SHARPEN_HFGAIND_WDATA   ISP_SHARPEN_HFGAIND_WDATA  ; /* 0x252a4 */
    volatile U_ISP_SHARPEN_HFGAIND_RADDR   ISP_SHARPEN_HFGAIND_RADDR  ; /* 0x252a8 */
    volatile U_ISP_SHARPEN_HFGAIND_RDATA   ISP_SHARPEN_HFGAIND_RDATA  ; /* 0x252ac */
    volatile U_ISP_SHARPEN_HFGAINUD_WADDR   ISP_SHARPEN_HFGAINUD_WADDR ; /* 0x252b0 */
    volatile U_ISP_SHARPEN_HFGAINUD_WDATA   ISP_SHARPEN_HFGAINUD_WDATA ; /* 0x252b4 */
    volatile U_ISP_SHARPEN_HFGAINUD_RADDR   ISP_SHARPEN_HFGAINUD_RADDR ; /* 0x252b8 */
    volatile U_ISP_SHARPEN_HFGAINUD_RDATA   ISP_SHARPEN_HFGAINUD_RDATA ; /* 0x252bc */
    volatile unsigned int           reserved_119[18]                     ; /* 0x252c0~0x25304 */
    volatile U_ISP_SHARPEN_MARK_DATA   ISP_SHARPEN_MARK_DATA          ; /* 0x25308 */
    volatile unsigned int           reserved_120                     ; /* 0x2530c */
    volatile U_ISP_SHARPEN_SKIN_U   ISP_SHARPEN_SKIN_U                ; /* 0x25310 */
    volatile U_ISP_SHARPEN_SKIN_V   ISP_SHARPEN_SKIN_V                ; /* 0x25314 */
    volatile U_ISP_SHARPEN_SKIN_CNT   ISP_SHARPEN_SKIN_CNT            ; /* 0x25318 */
    volatile U_ISP_SHARPEN_SKIN_EDGE   ISP_SHARPEN_SKIN_EDGE          ; /* 0x2531c */
    volatile U_ISP_SHARPEN_SKIN_EDGETHD   ISP_SHARPEN_SKIN_EDGETHD    ; /* 0x25320 */
    volatile U_ISP_SHARPEN_SKIN_ACCUMTHD   ISP_SHARPEN_SKIN_ACCUMTHD  ; /* 0x25324 */
    volatile U_ISP_SHARPEN_SKIN_ACCUMWGT   ISP_SHARPEN_SKIN_ACCUMWGT  ; /* 0x25328 */
    volatile U_ISP_SHARPEN_CHRR_VAR   ISP_SHARPEN_CHRR_VAR            ; /* 0x2532c */
    volatile U_ISP_SHARPEN_CHRR_ORI   ISP_SHARPEN_CHRR_ORI            ; /* 0x25330 */
    volatile U_ISP_SHARPEN_CHRR_SFT   ISP_SHARPEN_CHRR_SFT            ; /* 0x25334 */
    volatile U_ISP_SHARPEN_CHRR_THD   ISP_SHARPEN_CHRR_THD            ; /* 0x25338 */
    volatile U_ISP_SHARPEN_CHRR_GAIN   ISP_SHARPEN_CHRR_GAIN          ; /* 0x2533c */
    volatile U_ISP_SHARPEN_CHRG_VAR   ISP_SHARPEN_CHRG_VAR            ; /* 0x25340 */
    volatile U_ISP_SHARPEN_CHRG_ORI   ISP_SHARPEN_CHRG_ORI            ; /* 0x25344 */
    volatile U_ISP_SHARPEN_CHRG_SFT   ISP_SHARPEN_CHRG_SFT            ; /* 0x25348 */
    volatile U_ISP_SHARPEN_CHRG_THD   ISP_SHARPEN_CHRG_THD            ; /* 0x2534c */
    volatile U_ISP_SHARPEN_CHRG_GAIN   ISP_SHARPEN_CHRG_GAIN          ; /* 0x25350 */
    volatile U_ISP_SHARPEN_CHRB_VAR   ISP_SHARPEN_CHRB_VAR            ; /* 0x25354 */
    volatile U_ISP_SHARPEN_CHRB_ORI   ISP_SHARPEN_CHRB_ORI            ; /* 0x25358 */
    volatile U_ISP_SHARPEN_CHRB_SFT   ISP_SHARPEN_CHRB_SFT            ; /* 0x2535c */
    volatile U_ISP_SHARPEN_CHRB_THD   ISP_SHARPEN_CHRB_THD            ; /* 0x25360 */
    volatile U_ISP_SHARPEN_CHRB_GAIN   ISP_SHARPEN_CHRB_GAIN          ; /* 0x25364 */
    volatile U_ISP_SHARPEN_GAIN_SFT   ISP_SHARPEN_GAIN_SFT            ; /* 0x25368 */
    volatile U_ISP_SHARPEN_SHOOT_MAXGAIN   ISP_SHARPEN_SHOOT_MAXGAIN  ; /* 0x2536c */
    volatile U_ISP_SHARPEN_DETAIL_MUL   ISP_SHARPEN_DETAIL_MUL        ; /* 0x25370 */
    volatile U_ISP_SHARPEN_OSHT_DETAIL   ISP_SHARPEN_OSHT_DETAIL      ; /* 0x25374 */
    volatile U_ISP_SHARPEN_USHT_DETAIL   ISP_SHARPEN_USHT_DETAIL      ; /* 0x25378 */
    volatile U_ISP_SHARPEN_MARK_SHIFT   ISP_SHARPEN_MARK_SHIFT        ; /* 0x2537c */
    volatile unsigned int           reserved_121[160]                     ; /* 0x25380~0x255fc */
    volatile U_ISP_NDDM_CFG         ISP_NDDM_CFG                      ; /* 0x25600 */
    volatile U_ISP_NDDM_GF_TH       ISP_NDDM_GF_TH                    ; /* 0x25604 */
    volatile U_ISP_NDDM_GF_BLDR     ISP_NDDM_GF_BLDR                  ; /* 0x25608 */
    volatile U_ISP_NDDM_AWB_GF_CFG   ISP_NDDM_AWB_GF_CFG              ; /* 0x2560c */
    volatile U_ISP_NDDM_USM_CFG     ISP_NDDM_USM_CFG                  ; /* 0x25610 */
    volatile U_ISP_NDDM_USM_SATU    ISP_NDDM_USM_SATU                 ; /* 0x25614 */
    volatile U_ISP_NDDM_USM_GRAY    ISP_NDDM_USM_GRAY                 ; /* 0x25618 */
    volatile U_ISP_NDDM_APT_INTP_BLD   ISP_NDDM_APT_INTP_BLD          ; /* 0x2561c */
    volatile unsigned int           reserved_122                     ; /* 0x25620 */
    volatile U_ISP_NDDM_DITHER_CFG   ISP_NDDM_DITHER_CFG              ; /* 0x25624 */
    volatile U_ISP_NDDM_FCR_LIMIT   ISP_NDDM_FCR_LIMIT                ; /* 0x25628 */
    volatile U_ISP_NDDM_FCR_GAIN    ISP_NDDM_FCR_GAIN                 ; /* 0x2562c */
    volatile U_ISP_NDDM_FCR_SCALE   ISP_NDDM_FCR_SCALE                ; /* 0x25630 */
    volatile U_ISP_NDDM_DM_BLDRATE   ISP_NDDM_DM_BLDRATE              ; /* 0x25634 */
    volatile U_ISP_NDDM_GF_LUT_UPDATE   ISP_NDDM_GF_LUT_UPDATE        ; /* 0x25638 */
    volatile unsigned int           reserved_123                     ; /* 0x2563c */
    volatile U_ISP_NDDM_USM_LUT_UPDATE   ISP_NDDM_USM_LUT_UPDATE      ; /* 0x25640 */
    volatile U_ISP_NDDM_USM_GRAY_TH_FIX2   ISP_NDDM_USM_GRAY_TH_FIX2  ; /* 0x25644 */
    volatile U_ISP_NDDM_APT_INTP_CLIP2   ISP_NDDM_APT_INTP_CLIP2      ; /* 0x25648 */
    volatile U_ISP_NDDM_APT_AVG_CFG_0   ISP_NDDM_APT_AVG_CFG_0        ; /* 0x2564c */
    volatile U_ISP_NDDM_APT_AVG_CFG_1   ISP_NDDM_APT_AVG_CFG_1        ; /* 0x25650 */
    volatile U_ISP_NDDM_BLDR_ALPHA_GRAY   ISP_NDDM_BLDR_ALPHA_GRAY    ; /* 0x25654 */
    volatile U_ISP_NDDM_SHT_CTRL    ISP_NDDM_SHT_CTRL                 ; /* 0x25658 */
    volatile U_ISP_NDDM_SHT_CLIP_R   ISP_NDDM_SHT_CLIP_R              ; /* 0x2565c */
    volatile U_ISP_NDDM_SHT_CLIP_B   ISP_NDDM_SHT_CLIP_B              ; /* 0x25660 */
    volatile U_ISP_NDDM_MULTI_MF_RB   ISP_NDDM_MULTI_MF_RB            ; /* 0x25664 */
    volatile U_ISP_NDDM_USM_SATU_R   ISP_NDDM_USM_SATU_R              ; /* 0x25668 */
    volatile U_ISP_NDDM_USM_SATU_B   ISP_NDDM_USM_SATU_B              ; /* 0x2566c */
    volatile U_ISP_NDDM_SATU_FIX_EHCY   ISP_NDDM_SATU_FIX_EHCY        ; /* 0x25670 */
    volatile unsigned int           reserved_124[3]                     ; /* 0x25674~0x2567c */
    volatile U_ISP_NDDM_GF_LUT_WADDR   ISP_NDDM_GF_LUT_WADDR          ; /* 0x25680 */
    volatile U_ISP_NDDM_GF_LUT_WDATA   ISP_NDDM_GF_LUT_WDATA          ; /* 0x25684 */
    volatile U_ISP_NDDM_GF_LUT_RADDR   ISP_NDDM_GF_LUT_RADDR          ; /* 0x25688 */
    volatile U_ISP_NDDM_GF_LUT_RDATA   ISP_NDDM_GF_LUT_RDATA          ; /* 0x2568c */
    volatile U_ISP_NDDM_USM_LUT_WADDR   ISP_NDDM_USM_LUT_WADDR        ; /* 0x25690 */
    volatile U_ISP_NDDM_USM_LUT_WDATA   ISP_NDDM_USM_LUT_WDATA        ; /* 0x25694 */
    volatile U_ISP_NDDM_USM_LUT_RADDR   ISP_NDDM_USM_LUT_RADDR        ; /* 0x25698 */
    volatile U_ISP_NDDM_USM_LUT_RDATA   ISP_NDDM_USM_LUT_RDATA        ; /* 0x2569c */
    volatile unsigned int           reserved_125[88]                     ; /* 0x256a0~0x257fc */
    volatile U_ISP_BNR_CFG          ISP_BNR_CFG                       ; /* 0x25800 */
    volatile unsigned int           reserved_126[2]                     ; /* 0x25804~0x25808 */
    volatile U_ISP_BNR_VERSION      ISP_BNR_VERSION                   ; /* 0x2580c */
    volatile U_ISP_BNR_CRATIO       ISP_BNR_CRATIO                    ; /* 0x25810 */
    volatile U_ISP_BNR_AMED         ISP_BNR_AMED                      ; /* 0x25814 */
    volatile unsigned int           reserved_127[3]                     ; /* 0x25818~0x25820 */
    volatile U_ISP_BNR_JNLM0        ISP_BNR_JNLM0                     ; /* 0x25824 */
    volatile unsigned int           reserved_128                     ; /* 0x25828 */
    volatile U_ISP_BNR_JNLM2        ISP_BNR_JNLM2                     ; /* 0x2582c */
    volatile U_ISP_BNR_CHROMA       ISP_BNR_CHROMA                    ; /* 0x25830 */
    volatile unsigned int           reserved_129[3]                     ; /* 0x25834~0x2583c */
    volatile U_ISP_BNR_LUT_UPDATE   ISP_BNR_LUT_UPDATE                ; /* 0x25840 */
    volatile U_ISP_BNR_DETAIL       ISP_BNR_DETAIL                    ; /* 0x25844 */
    volatile U_ISP_BNR_LPCTRL       ISP_BNR_LPCTRL                    ; /* 0x25848 */
    volatile unsigned int           reserved_130                     ; /* 0x2584c */
    volatile U_ISP_BNR_JNLMGAIN0    ISP_BNR_JNLMGAIN0                 ; /* 0x25850 */
    volatile U_ISP_BNR_JNLMGAIN1    ISP_BNR_JNLMGAIN1                 ; /* 0x25854 */
    volatile U_ISP_BNR_JNLMGAIN2    ISP_BNR_JNLMGAIN2                 ; /* 0x25858 */
    volatile U_ISP_BNR_JNLMGAIN3    ISP_BNR_JNLMGAIN3                 ; /* 0x2585c */
    volatile U_ISP_BNR_JNLMGAIN4    ISP_BNR_JNLMGAIN4                 ; /* 0x25860 */
    volatile U_ISP_BNR_JNLMGAIN5    ISP_BNR_JNLMGAIN5                 ; /* 0x25864 */
    volatile U_ISP_BNR_JNLMGAIN6    ISP_BNR_JNLMGAIN6                 ; /* 0x25868 */
    volatile U_ISP_BNR_JNLMGAIN7    ISP_BNR_JNLMGAIN7                 ; /* 0x2586c */
    volatile U_ISP_BNR_WTI0         ISP_BNR_WTI0                      ; /* 0x25870 */
    volatile U_ISP_BNR_WTI1         ISP_BNR_WTI1                      ; /* 0x25874 */
    volatile U_ISP_BNR_NPTHRESH     ISP_BNR_NPTHRESH                  ; /* 0x25878 */
    volatile unsigned int           reserved_131                     ; /* 0x2587c */
    volatile U_ISP_BNR_LMT_EVEN_WADDR   ISP_BNR_LMT_EVEN_WADDR        ; /* 0x25880 */
    volatile U_ISP_BNR_LMT_EVEN_WDATA   ISP_BNR_LMT_EVEN_WDATA        ; /* 0x25884 */
    volatile U_ISP_BNR_LMT_EVEN_RADDR   ISP_BNR_LMT_EVEN_RADDR        ; /* 0x25888 */
    volatile U_ISP_BNR_LMT_EVEN_RDATA   ISP_BNR_LMT_EVEN_RDATA        ; /* 0x2588c */
    volatile U_ISP_BNR_LMT_ODD_WADDR   ISP_BNR_LMT_ODD_WADDR          ; /* 0x25890 */
    volatile U_ISP_BNR_LMT_ODD_WDATA   ISP_BNR_LMT_ODD_WDATA          ; /* 0x25894 */
    volatile U_ISP_BNR_LMT_ODD_RADDR   ISP_BNR_LMT_ODD_RADDR          ; /* 0x25898 */
    volatile U_ISP_BNR_LMT_ODD_RDATA   ISP_BNR_LMT_ODD_RDATA          ; /* 0x2589c */
    volatile U_ISP_BNR_COR_EVEN_WADDR   ISP_BNR_COR_EVEN_WADDR        ; /* 0x258a0 */
    volatile U_ISP_BNR_COR_EVEN_WDATA   ISP_BNR_COR_EVEN_WDATA        ; /* 0x258a4 */
    volatile U_ISP_BNR_COR_EVEN_RADDR   ISP_BNR_COR_EVEN_RADDR        ; /* 0x258a8 */
    volatile U_ISP_BNR_COR_EVEN_RDATA   ISP_BNR_COR_EVEN_RDATA        ; /* 0x258ac */
    volatile U_ISP_BNR_COR_ODD_WADDR   ISP_BNR_COR_ODD_WADDR          ; /* 0x258b0 */
    volatile U_ISP_BNR_COR_ODD_WDATA   ISP_BNR_COR_ODD_WDATA          ; /* 0x258b4 */
    volatile U_ISP_BNR_COR_ODD_RADDR   ISP_BNR_COR_ODD_RADDR          ; /* 0x258b8 */
    volatile U_ISP_BNR_COR_ODD_RDATA   ISP_BNR_COR_ODD_RDATA          ; /* 0x258bc */
    volatile unsigned int           reserved_132[80]                     ; /* 0x258c0~0x259fc */
    volatile U_ISP_BNR_LSC_CFG      ISP_BNR_LSC_CFG                   ; /* 0x25a00 */
    volatile unsigned int           reserved_133[3]                     ; /* 0x25a04~0x25a0c */
    volatile U_ISP_BNR_LSC_WINNUM   ISP_BNR_LSC_WINNUM                ; /* 0x25a10 */
    volatile U_ISP_BNR_LSC_WINX_0_23    ISP_BNR_LSC_WINX_0_23[24]        ; /* 0x25a14 */
    volatile U_ISP_BNR_LSC_WIDTH_OFFSET   ISP_BNR_LSC_WIDTH_OFFSET    ; /* 0x25a74 */
    volatile U_ISP_BNR_LSC_MESH     ISP_BNR_LSC_MESH                  ; /* 0x25a78 */
    volatile U_ISP_BNR_LSC_WEIGHT   ISP_BNR_LSC_WEIGHT                ; /* 0x25a7c */
    volatile U_ISP_BNR_LSC_RGAIN_WADDR   ISP_BNR_LSC_RGAIN_WADDR      ; /* 0x25a80 */
    volatile U_ISP_BNR_LSC_RGAIN_WDATA   ISP_BNR_LSC_RGAIN_WDATA      ; /* 0x25a84 */
    volatile U_ISP_BNR_LSC_RGAIN_RADDR   ISP_BNR_LSC_RGAIN_RADDR      ; /* 0x25a88 */
    volatile U_ISP_BNR_LSC_RGAIN_RDATA   ISP_BNR_LSC_RGAIN_RDATA      ; /* 0x25a8c */
    volatile U_ISP_BNR_LSC_GRGAIN_WADDR   ISP_BNR_LSC_GRGAIN_WADDR    ; /* 0x25a90 */
    volatile U_ISP_BNR_LSC_GRGAIN_WDATA   ISP_BNR_LSC_GRGAIN_WDATA    ; /* 0x25a94 */
    volatile U_ISP_BNR_LSC_GRGAIN_RADDR   ISP_BNR_LSC_GRGAIN_RADDR    ; /* 0x25a98 */
    volatile U_ISP_BNR_LSC_GRGAIN_RDATA   ISP_BNR_LSC_GRGAIN_RDATA    ; /* 0x25a9c */
    volatile U_ISP_BNR_LSC_BGAIN_WADDR   ISP_BNR_LSC_BGAIN_WADDR      ; /* 0x25aa0 */
    volatile U_ISP_BNR_LSC_BGAIN_WDATA   ISP_BNR_LSC_BGAIN_WDATA      ; /* 0x25aa4 */
    volatile U_ISP_BNR_LSC_BGAIN_RADDR   ISP_BNR_LSC_BGAIN_RADDR      ; /* 0x25aa8 */
    volatile U_ISP_BNR_LSC_BGAIN_RDATA   ISP_BNR_LSC_BGAIN_RDATA      ; /* 0x25aac */
    volatile U_ISP_BNR_LSC_GBGAIN_WADDR   ISP_BNR_LSC_GBGAIN_WADDR    ; /* 0x25ab0 */
    volatile U_ISP_BNR_LSC_GBGAIN_WDATA   ISP_BNR_LSC_GBGAIN_WDATA    ; /* 0x25ab4 */
    volatile U_ISP_BNR_LSC_GBGAIN_RADDR   ISP_BNR_LSC_GBGAIN_RADDR    ; /* 0x25ab8 */
    volatile U_ISP_BNR_LSC_GBGAIN_RDATA   ISP_BNR_LSC_GBGAIN_RDATA    ; /* 0x25abc */
    volatile unsigned int           reserved_134[20]                     ; /* 0x25ac0~0x25b0c */
    volatile U_ISP_BNR_LSC_WINX_24_31       ISP_BNR_LSC_WINX_24_31[8]   ; /* 0x25b10 */
    volatile U_ISP_BNR_LSC_WINY         ISP_BNR_LSC_WINY[16]        ;  /* 0x25b30 */
    volatile unsigned int           reserved_135[3]                     ; /* 0x25b70~0x25b78 */
    volatile U_ISP_BNR_LSC_LUT_UPDATE   ISP_BNR_LSC_LUT_UPDATE        ; /* 0x25b7c */
    volatile unsigned int           reserved_136[292]                     ; /* 0x25b80~0x2600c */
    volatile U_ISP_WDR_CTRL         ISP_WDR_CTRL                      ; /* 0x26010 */
    volatile U_ISP_WDR_BIT_DEPTH    ISP_WDR_BIT_DEPTH                 ; /* 0x26014 */
    volatile U_ISP_WDR_F0_INBLC0    ISP_WDR_F0_INBLC0                 ; /* 0x26018 */
    volatile U_ISP_WDR_F0_INBLC1    ISP_WDR_F0_INBLC1                 ; /* 0x2601c */
    volatile U_ISP_WDR_F1_INBLC0    ISP_WDR_F1_INBLC0                 ; /* 0x26020 */
    volatile U_ISP_WDR_F1_INBLC1    ISP_WDR_F1_INBLC1                 ; /* 0x26024 */
    volatile U_ISP_WDR_F2_INBLC0    ISP_WDR_F2_INBLC0                 ; /* 0x26028 */
    volatile U_ISP_WDR_F2_INBLC1    ISP_WDR_F2_INBLC1                 ; /* 0x2602c */
    volatile U_ISP_WDR_F3_INBLC0    ISP_WDR_F3_INBLC0                 ; /* 0x26030 */
    volatile U_ISP_WDR_F3_INBLC1    ISP_WDR_F3_INBLC1                 ; /* 0x26034 */
    volatile U_ISP_WDR_OUTBLC       ISP_WDR_OUTBLC                    ; /* 0x26038 */
    volatile U_ISP_WDR_EXPOVALUE0   ISP_WDR_EXPOVALUE0                ; /* 0x2603c */
    volatile U_ISP_WDR_EXPOVALUE1   ISP_WDR_EXPOVALUE1                ; /* 0x26040 */
    volatile U_ISP_WDR_EXPORRATIO0   ISP_WDR_EXPORRATIO0              ; /* 0x26044 */
    volatile U_ISP_WDR_EXPORRATIO1   ISP_WDR_EXPORRATIO1              ; /* 0x26048 */
    volatile U_ISP_WDR_BLC_COMP0    ISP_WDR_BLC_COMP0                 ; /* 0x2604c */
    volatile U_ISP_WDR_BLC_COMP1    ISP_WDR_BLC_COMP1                 ; /* 0x26050 */
    volatile U_ISP_WDR_BLC_COMP2    ISP_WDR_BLC_COMP2                 ; /* 0x26054 */
    volatile U_ISP_WDR_MAXRATIO     ISP_WDR_MAXRATIO                  ; /* 0x26058 */
    volatile U_ISP_WDR_SATURATE_THR   ISP_WDR_SATURATE_THR            ; /* 0x2605c */
    volatile U_ISP_WDR_STILL_THR0   ISP_WDR_STILL_THR0                ; /* 0x26060 */
    volatile U_ISP_WDR_STILL_THR1   ISP_WDR_STILL_THR1                ; /* 0x26064 */
    volatile U_ISP_WDR_MDTLBLD      ISP_WDR_MDTLBLD                   ; /* 0x26068 */
    volatile U_ISP_WDR_MDT_THR      ISP_WDR_MDT_THR                   ; /* 0x2606c */
    volatile U_ISP_WDR_BNR_FULLMDT_THR   ISP_WDR_BNR_FULLMDT_THR      ; /* 0x26070 */
    volatile U_ISP_WDR_MDT_NOISEFLOOR   ISP_WDR_MDT_NOISEFLOOR        ; /* 0x26074 */
    volatile U_ISP_WDR_MDTHR_GAIN   ISP_WDR_MDTHR_GAIN                ; /* 0x26078 */
    volatile U_ISP_WDR_SHORTEXPO_CHK   ISP_WDR_SHORTEXPO_CHK          ; /* 0x2607c */
    volatile U_ISP_WDR_NOSLUT129X8_WADDR   ISP_WDR_NOSLUT129X8_WADDR  ; /* 0x26080 */
    volatile U_ISP_WDR_NOSLUT129X8_WDATA   ISP_WDR_NOSLUT129X8_WDATA  ; /* 0x26084 */
    volatile U_ISP_WDR_NOSLUT129X8_RADDR   ISP_WDR_NOSLUT129X8_RADDR  ; /* 0x26088 */
    volatile U_ISP_WDR_NOSLUT129X8_RDATA   ISP_WDR_NOSLUT129X8_RDATA  ; /* 0x2608c */
    volatile unsigned int           reserved_137[32]                     ; /* 0x26090~0x2610c */
    volatile U_ISP_WDR_MASK_SIMILAR   ISP_WDR_MASK_SIMILAR            ; /* 0x26110 */
    volatile U_ISP_WDR_WGTIDX_BLENDRATIO   ISP_WDR_WGTIDX_BLENDRATIO  ; /* 0x26114 */
    volatile U_ISP_WDR_WGTIDX_THR   ISP_WDR_WGTIDX_THR                ; /* 0x26118 */
    volatile U_ISP_WDR_DFTWGT_FL    ISP_WDR_DFTWGT_FL                 ; /* 0x2611c */
    volatile U_ISP_WDR_SENSOR_GAIN   ISP_WDR_SENSOR_GAIN              ; /* 0x26120 */
    volatile U_ISP_WDR_NOISEFLOOR0   ISP_WDR_NOISEFLOOR0              ; /* 0x26124 */
    volatile U_ISP_WDR_NOISEFLOOR1   ISP_WDR_NOISEFLOOR1              ; /* 0x26128 */
    volatile U_ISP_WDR_MODELCOEF_GAIN0   ISP_WDR_MODELCOEF_GAIN0      ; /* 0x2612c */
    volatile U_ISP_WDR_MODELCOEF_GAIN1   ISP_WDR_MODELCOEF_GAIN1      ; /* 0x26130 */
    volatile U_ISP_WDR_NOS_NP_THR   ISP_WDR_NOS_NP_THR                ; /* 0x26134 */
    volatile U_ISP_WDR_FULLMDT_SIGMA_WGT   ISP_WDR_FULLMDT_SIGMA_WGT  ; /* 0x26138 */
    volatile U_ISP_WDR_GSIGMA_GAIN   ISP_WDR_GSIGMA_GAIN              ; /* 0x2613c */
    volatile U_ISP_WDR_CSIGMA_GAIN   ISP_WDR_CSIGMA_GAIN              ; /* 0x26140 */
    volatile U_ISP_WDR_2DNR_WEIGHT   ISP_WDR_2DNR_WEIGHT              ; /* 0x26144 */
    volatile U_ISP_WDR_FUSION_RATIO_THR   ISP_WDR_FUSION_RATIO_THR    ; /* 0x26148 */
    volatile U_ISP_WDR_FUSION_THR0   ISP_WDR_FUSION_THR0              ; /* 0x2614c */
    volatile U_ISP_WDR_FUSION_THR1   ISP_WDR_FUSION_THR1              ; /* 0x26150 */
    volatile U_ISP_WDR_LUT_UPDATE   ISP_WDR_LUT_UPDATE                ; /* 0x26154 */
    volatile unsigned int           reserved_138[365]                     ; /* 0x26158~0x26708 */
    volatile U_ISP_DEHAZE_VERSION   ISP_DEHAZE_VERSION                ; /* 0x2670c */
    volatile U_ISP_DEHAZE_PRE_UPDATE   ISP_DEHAZE_PRE_UPDATE          ; /* 0x26710 */
    volatile U_ISP_DEHAZE_BLK_SIZE   ISP_DEHAZE_BLK_SIZE              ; /* 0x26714 */
    volatile U_ISP_DEHAZE_BLK_SUM   ISP_DEHAZE_BLK_SUM                ; /* 0x26718 */
    volatile U_ISP_DEHAZE_DC_SIZE   ISP_DEHAZE_DC_SIZE                ; /* 0x2671c */
    volatile U_ISP_DEHAZE_X         ISP_DEHAZE_X                      ; /* 0x26720 */
    volatile U_ISP_DEHAZE_Y         ISP_DEHAZE_Y                      ; /* 0x26724 */
    volatile U_ISP_DEHAZE_STAT_MODE   ISP_DEHAZE_STAT_MODE            ; /* 0x26728 */
    volatile U_ISP_DEHAZE_NEG_MODE   ISP_DEHAZE_NEG_MODE              ; /* 0x2672c */
    volatile U_ISP_DEHAZE_AIR       ISP_DEHAZE_AIR                    ; /* 0x26730 */
    volatile U_ISP_DEHAZE_THLD      ISP_DEHAZE_THLD                   ; /* 0x26734 */
    volatile U_ISP_DEHAZE_GSTRTH    ISP_DEHAZE_GSTRTH                 ; /* 0x26738 */
    volatile U_ISP_DEHAZE_BLTHLD    ISP_DEHAZE_BLTHLD                 ; /* 0x2673c */
    volatile U_ISP_DEHAZE_STR_LUT_UPDATE   ISP_DEHAZE_STR_LUT_UPDATE  ; /* 0x26740 */
    volatile unsigned int           reserved_139[15]                     ; /* 0x26744~0x2677c */
    volatile U_ISP_DEHAZE_MINSTAT_WADDR   ISP_DEHAZE_MINSTAT_WADDR    ; /* 0x26780 */
    volatile U_ISP_DEHAZE_MINSTAT_WDATA   ISP_DEHAZE_MINSTAT_WDATA    ; /* 0x26784 */
    volatile U_ISP_DEHAZE_MINSTAT_RADDR   ISP_DEHAZE_MINSTAT_RADDR    ; /* 0x26788 */
    volatile U_ISP_DEHAZE_MINSTAT_RDATA   ISP_DEHAZE_MINSTAT_RDATA    ; /* 0x2678c */
    volatile U_ISP_DEHAZE_MAXSTAT_WADDR   ISP_DEHAZE_MAXSTAT_WADDR    ; /* 0x26790 */
    volatile U_ISP_DEHAZE_MAXSTAT_WDATA   ISP_DEHAZE_MAXSTAT_WDATA    ; /* 0x26794 */
    volatile U_ISP_DEHAZE_MAXSTAT_RADDR   ISP_DEHAZE_MAXSTAT_RADDR    ; /* 0x26798 */
    volatile U_ISP_DEHAZE_MAXSTAT_RDATA   ISP_DEHAZE_MAXSTAT_RDATA    ; /* 0x2679c */
    volatile U_ISP_DEHAZE_PRESTAT_WADDR   ISP_DEHAZE_PRESTAT_WADDR    ; /* 0x267a0 */
    volatile U_ISP_DEHAZE_PRESTAT_WDATA   ISP_DEHAZE_PRESTAT_WDATA    ; /* 0x267a4 */
    volatile U_ISP_DEHAZE_PRESTAT_RADDR   ISP_DEHAZE_PRESTAT_RADDR    ; /* 0x267a8 */
    volatile U_ISP_DEHAZE_PRESTAT_RDATA   ISP_DEHAZE_PRESTAT_RDATA    ; /* 0x267ac */
    volatile U_ISP_DEHAZE_LUT_WADDR   ISP_DEHAZE_LUT_WADDR            ; /* 0x267b0 */
    volatile U_ISP_DEHAZE_LUT_WDATA   ISP_DEHAZE_LUT_WDATA            ; /* 0x267b4 */
    volatile U_ISP_DEHAZE_LUT_RADDR   ISP_DEHAZE_LUT_RADDR            ; /* 0x267b8 */
    volatile U_ISP_DEHAZE_LUT_RDATA   ISP_DEHAZE_LUT_RDATA            ; /* 0x267bc */
    volatile unsigned int           reserved_140[80]                     ; /* 0x267c0~0x268fc */
    volatile U_ISP_PREGAMMA_CFG     ISP_PREGAMMA_CFG                  ; /* 0x26900 */
    volatile unsigned int           reserved_141[2]                     ; /* 0x26904~0x26908 */
    volatile U_ISP_PREGAMMA_VERSION   ISP_PREGAMMA_VERSION            ; /* 0x2690c */
    volatile U_ISP_PREGAMMA_BITW    ISP_PREGAMMA_BITW                 ; /* 0x26910 */
    volatile U_ISP_PREGAMMA_OFFSETR   ISP_PREGAMMA_OFFSETR            ; /* 0x26914 */
    volatile U_ISP_PREGAMMA_OFFSETGR   ISP_PREGAMMA_OFFSETGR          ; /* 0x26918 */
    volatile U_ISP_PREGAMMA_OFFSETGB   ISP_PREGAMMA_OFFSETGB          ; /* 0x2691c */
    volatile U_ISP_PREGAMMA_OFFSETB   ISP_PREGAMMA_OFFSETB            ; /* 0x26920 */
    volatile unsigned int           reserved_142[23]                     ; /* 0x26924~0x2697c */
    volatile U_ISP_PREGAMMA_LUT_WADDR   ISP_PREGAMMA_LUT_WADDR        ; /* 0x26980 */
    volatile U_ISP_PREGAMMA_LUT_WDATA   ISP_PREGAMMA_LUT_WDATA        ; /* 0x26984 */
    volatile U_ISP_PREGAMMA_LUT_RADDR   ISP_PREGAMMA_LUT_RADDR        ; /* 0x26988 */
    volatile U_ISP_PREGAMMA_LUT_RDATA   ISP_PREGAMMA_LUT_RDATA        ; /* 0x2698c */
    volatile unsigned int           reserved_143[28]                  ; /* 0x26990~0x269fc */
    volatile U_ISP_GAMMA_CFG        ISP_GAMMA_CFG                     ; /* 0x26a00 */
    volatile unsigned int           reserved_144[2]                   ; /* 0x26a04~0x26a08 */
    volatile U_ISP_GAMMA_VERSION    ISP_GAMMA_VERSION                 ; /* 0x26a0c */
    volatile U_ISP_GAMMA_LUT_UPDATE   ISP_GAMMA_LUT_UPDATE            ; /* 0x26a10 */
    volatile unsigned int           reserved_145[27]                  ; /* 0x26a14~0x26a7c */
    volatile U_ISP_GAMMA_LUT_WADDR   ISP_GAMMA_LUT_WADDR              ; /* 0x26a80 */
    volatile U_ISP_GAMMA_LUT_WDATA   ISP_GAMMA_LUT_WDATA              ; /* 0x26a84 */
    volatile U_ISP_GAMMA_LUT_RADDR   ISP_GAMMA_LUT_RADDR              ; /* 0x26a88 */
    volatile U_ISP_GAMMA_LUT_RDATA   ISP_GAMMA_LUT_RDATA              ; /* 0x26a8c */
    volatile unsigned int           reserved_146[92]                  ; /* 0x26a90~0x26bfc */
    volatile U_ISP_BCOM_CFG         ISP_BCOM_CFG                      ; /* 0x26c00 */
    volatile unsigned int           reserved_147[2]                   ; /* 0x26c04~0x26c08 */
    volatile U_ISP_BCOM_VERSION     ISP_BCOM_VERSION                  ; /* 0x26c0c */
    volatile U_ISP_BCOM_ALPHA       ISP_BCOM_ALPHA                    ; /* 0x26c10 */
    volatile unsigned int           reserved_148[59]                  ; /* 0x26c14~0x26cfc */
    volatile U_ISP_BDEC_CFG         ISP_BDEC_CFG                      ; /* 0x26d00 */
    volatile unsigned int           reserved_149[2]                   ; /* 0x26d04~0x26d08 */
    volatile U_ISP_BDEC_VERSION     ISP_BDEC_VERSION                  ; /* 0x26d0c */
    volatile U_ISP_BDEC_ALPHA       ISP_BDEC_ALPHA                    ; /* 0x26d10 */
    volatile unsigned int           reserved_150[59]                  ; /* 0x26d14~0x26dfc */
    volatile U_ISP_WDRSPLIT_CFG     ISP_WDRSPLIT_CFG                  ; /* 0x26e00 */
    volatile unsigned int           reserved_151[3]                   ; /* 0x26e04~0x26e0c */
    volatile U_ISP_WDRSPLIT_BITW    ISP_WDRSPLIT_BITW                 ; /* 0x26e10 */
    volatile U_ISP_WDRSPLIT_MODE    ISP_WDRSPLIT_MODE                 ; /* 0x26e14 */
    volatile unsigned int           reserved_152[2]                   ; /* 0x26e18~0x26e1c */
    volatile U_ISP_WDRSPLIT_OFFSETR   ISP_WDRSPLIT_OFFSETR            ; /* 0x26e20 */
    volatile U_ISP_WDRSPLIT_OFFSETGR   ISP_WDRSPLIT_OFFSETGR          ; /* 0x26e24 */
    volatile U_ISP_WDRSPLIT_OFFSETGB   ISP_WDRSPLIT_OFFSETGB          ; /* 0x26e28 */
    volatile U_ISP_WDRSPLIT_OFFSETB   ISP_WDRSPLIT_OFFSETB            ; /* 0x26e2c */
    volatile U_ISP_WDRSPLIT_BLC     ISP_WDRSPLIT_BLC                  ; /* 0x26e30 */
    volatile unsigned int           reserved_153[19]                  ; /* 0x26e34~0x26e7c */
    volatile U_ISP_WDRSPLIT_LUT_WADDR   ISP_WDRSPLIT_LUT_WADDR        ; /* 0x26e80 */
    volatile U_ISP_WDRSPLIT_LUT_WDATA   ISP_WDRSPLIT_LUT_WDATA        ; /* 0x26e84 */
    volatile U_ISP_WDRSPLIT_LUT_RADDR   ISP_WDRSPLIT_LUT_RADDR        ; /* 0x26e88 */
    volatile U_ISP_WDRSPLIT_LUT_RDATA   ISP_WDRSPLIT_LUT_RDATA        ; /* 0x26e8c */
    volatile unsigned int           reserved_154[96]                  ; /* 0x26e90~0x2700c */
    volatile U_ISP_CA_CTRL          ISP_CA_CTRL                       ; /* 0x27010 */
    volatile U_ISP_CA_LUMATH        ISP_CA_LUMATH                     ; /* 0x27014 */
    volatile U_ISP_CA_DARKCHROMA_TH   ISP_CA_DARKCHROMA_TH            ; /* 0x27018 */
    volatile U_ISP_CA_SDARKCHROMA_TH   ISP_CA_SDARKCHROMA_TH          ; /* 0x2701c */
    volatile U_ISP_CA_LLHC_RATIO    ISP_CA_LLHC_RATIO                 ; /* 0x27020 */
    volatile U_ISP_CA_ISORATIO      ISP_CA_ISORATIO                   ; /* 0x27024 */
    volatile U_ISP_CA_LUT_UPDATE    ISP_CA_LUT_UPDATE                 ; /* 0x27028 */
    volatile U_ISP_CA_YUV2RGB_COEF0   ISP_CA_YUV2RGB_COEF0            ; /* 0x2702c */
    volatile U_ISP_CA_YUV2RGB_COEF1   ISP_CA_YUV2RGB_COEF1            ; /* 0x27030 */
    volatile U_ISP_CA_YUV2RGB_COEF2   ISP_CA_YUV2RGB_COEF2            ; /* 0x27034 */
    volatile U_ISP_CA_YUV2RGB_COEF3   ISP_CA_YUV2RGB_COEF3            ; /* 0x27038 */
    volatile U_ISP_CA_YUV2RGB_COEF4   ISP_CA_YUV2RGB_COEF4            ; /* 0x2703c */
    volatile U_ISP_CA_YUV2RGB_INDC0   ISP_CA_YUV2RGB_INDC0            ; /* 0x27040 */
    volatile U_ISP_CA_YUV2RGB_INDC1   ISP_CA_YUV2RGB_INDC1            ; /* 0x27044 */
    volatile U_ISP_CA_YUV2RGB_OUTDC0   ISP_CA_YUV2RGB_OUTDC0          ; /* 0x27048 */
    volatile U_ISP_CA_YUV2RGB_OUTDC1   ISP_CA_YUV2RGB_OUTDC1          ; /* 0x2704c */
    volatile U_ISP_CA_SKINLLUMA_UTH   ISP_CA_SKINLLUMA_UTH            ; /* 0x27050 */
    volatile U_ISP_CA_SKINLLUMA_UYTH   ISP_CA_SKINLLUMA_UYTH          ; /* 0x27054 */
    volatile U_ISP_CA_SKINHLUMA_UTH   ISP_CA_SKINHLUMA_UTH            ; /* 0x27058 */
    volatile U_ISP_CA_SKINHLUMA_UYTH   ISP_CA_SKINHLUMA_UYTH          ; /* 0x2705c */
    volatile U_ISP_CA_SKINLLUMA_VTH   ISP_CA_SKINLLUMA_VTH            ; /* 0x27060 */
    volatile U_ISP_CA_SKINLLUMA_VYTH   ISP_CA_SKINLLUMA_VYTH          ; /* 0x27064 */
    volatile U_ISP_CA_SKINHLUMA_VTH   ISP_CA_SKINHLUMA_VTH            ; /* 0x27068 */
    volatile U_ISP_CA_SKINHLUMA_VYTH   ISP_CA_SKINHLUMA_VYTH          ; /* 0x2706c */
    volatile U_ISP_CA_SKIN_UVDIFF   ISP_CA_SKIN_UVDIFF                ; /* 0x27070 */
    volatile U_ISP_CA_SKIN_RATIOTH0   ISP_CA_SKIN_RATIOTH0            ; /* 0x27074 */
    volatile U_ISP_CA_SKIN_RATIOTH1   ISP_CA_SKIN_RATIOTH1            ; /* 0x27078 */
    volatile unsigned int           reserved_155                      ; /* 0x2707c */
    volatile U_ISP_CA_LUT_WADDR     ISP_CA_LUT_WADDR                  ; /* 0x27080 */
    volatile U_ISP_CA_LUT_WDATA     ISP_CA_LUT_WDATA                  ; /* 0x27084 */
    volatile U_ISP_CA_LUT_RADDR     ISP_CA_LUT_RADDR                  ; /* 0x27088 */
    volatile U_ISP_CA_LUT_RDATA     ISP_CA_LUT_RDATA                  ; /* 0x2708c */
    volatile unsigned int           reserved_156[95]                  ; /* 0x27090~0x27208 */
    volatile U_ISP_LOGLUT_VERSION   ISP_LOGLUT_VERSION                ; /* 0x2720c */
    volatile U_ISP_LOGLUT_BITW      ISP_LOGLUT_BITW                   ; /* 0x27210 */
    volatile U_ISP_LOGLUT_OFFSETR   ISP_LOGLUT_OFFSETR                ; /* 0x27214 */
    volatile U_ISP_LOGLUT_OFFSETGR   ISP_LOGLUT_OFFSETGR              ; /* 0x27218 */
    volatile U_ISP_LOGLUT_OFFSETGB   ISP_LOGLUT_OFFSETGB              ; /* 0x2721c */
    volatile U_ISP_LOGLUT_OFFSETB   ISP_LOGLUT_OFFSETB                ; /* 0x27220 */
    volatile unsigned int           reserved_157[23]                  ; /* 0x27224~0x2727c */
    volatile U_ISP_LOGLUT_LUT_WADDR   ISP_LOGLUT_LUT_WADDR            ; /* 0x27280 */
    volatile U_ISP_LOGLUT_LUT_WDATA   ISP_LOGLUT_LUT_WDATA            ; /* 0x27284 */
    volatile U_ISP_LOGLUT_LUT_RADDR   ISP_LOGLUT_LUT_RADDR            ; /* 0x27288 */
    volatile U_ISP_LOGLUT_LUT_RDATA   ISP_LOGLUT_LUT_RDATA            ; /* 0x2728c */
    volatile unsigned int           reserved_158[94]                  ; /* 0x27290~0x27404 */
    volatile U_ISP_CLUT_ADJ0        ISP_CLUT_ADJ0                     ; /* 0x27408 */
    volatile U_ISP_CLUT_ADJ1        ISP_CLUT_ADJ1                     ; /* 0x2740c */
    volatile U_ISP_CLUT_LUT_UPDATE   ISP_CLUT_LUT_UPDATE              ; /* 0x27410 */
    volatile unsigned int           reserved_159[27]                  ; /* 0x27414~0x2747c */
    volatile U_ISP_CLUT_LUT0_WADDR   ISP_CLUT_LUT0_WADDR              ; /* 0x27480 */
    volatile U_ISP_CLUT_LUT0_WDATA   ISP_CLUT_LUT0_WDATA              ; /* 0x27484 */
    volatile U_ISP_CLUT_LUT0_RADDR   ISP_CLUT_LUT0_RADDR              ; /* 0x27488 */
    volatile U_ISP_CLUT_LUT0_RDATA   ISP_CLUT_LUT0_RDATA              ; /* 0x2748c */
    volatile U_ISP_CLUT_LUT1_WADDR   ISP_CLUT_LUT1_WADDR              ; /* 0x27490 */
    volatile U_ISP_CLUT_LUT1_WDATA   ISP_CLUT_LUT1_WDATA              ; /* 0x27494 */
    volatile U_ISP_CLUT_LUT1_RADDR   ISP_CLUT_LUT1_RADDR              ; /* 0x27498 */
    volatile U_ISP_CLUT_LUT1_RDATA   ISP_CLUT_LUT1_RDATA              ; /* 0x2749c */
    volatile U_ISP_CLUT_LUT2_WADDR   ISP_CLUT_LUT2_WADDR              ; /* 0x274a0 */
    volatile U_ISP_CLUT_LUT2_WDATA   ISP_CLUT_LUT2_WDATA              ; /* 0x274a4 */
    volatile U_ISP_CLUT_LUT2_RADDR   ISP_CLUT_LUT2_RADDR              ; /* 0x274a8 */
    volatile U_ISP_CLUT_LUT2_RDATA   ISP_CLUT_LUT2_RDATA              ; /* 0x274ac */
    volatile U_ISP_CLUT_LUT3_WADDR   ISP_CLUT_LUT3_WADDR              ; /* 0x274b0 */
    volatile U_ISP_CLUT_LUT3_WDATA   ISP_CLUT_LUT3_WDATA              ; /* 0x274b4 */
    volatile U_ISP_CLUT_LUT3_RADDR   ISP_CLUT_LUT3_RADDR              ; /* 0x274b8 */
    volatile U_ISP_CLUT_LUT3_RDATA   ISP_CLUT_LUT3_RDATA              ; /* 0x274bc */
    volatile unsigned int           reserved_160[48]                  ; /* 0x274c0~0x2757c */
    volatile U_ISP_CLUT_LUT4_WADDR   ISP_CLUT_LUT4_WADDR              ; /* 0x27580 */
    volatile U_ISP_CLUT_LUT4_WDATA   ISP_CLUT_LUT4_WDATA              ; /* 0x27584 */
    volatile U_ISP_CLUT_LUT4_RADDR   ISP_CLUT_LUT4_RADDR              ; /* 0x27588 */
    volatile U_ISP_CLUT_LUT4_RDATA   ISP_CLUT_LUT4_RDATA              ; /* 0x2758c */
    volatile U_ISP_CLUT_LUT5_WADDR   ISP_CLUT_LUT5_WADDR              ; /* 0x27590 */
    volatile U_ISP_CLUT_LUT5_WDATA   ISP_CLUT_LUT5_WDATA              ; /* 0x27594 */
    volatile U_ISP_CLUT_LUT5_RADDR   ISP_CLUT_LUT5_RADDR              ; /* 0x27598 */
    volatile U_ISP_CLUT_LUT5_RDATA   ISP_CLUT_LUT5_RDATA              ; /* 0x2759c */
    volatile U_ISP_CLUT_LUT6_WADDR   ISP_CLUT_LUT6_WADDR              ; /* 0x275a0 */
    volatile U_ISP_CLUT_LUT6_WDATA   ISP_CLUT_LUT6_WDATA              ; /* 0x275a4 */
    volatile U_ISP_CLUT_LUT6_RADDR   ISP_CLUT_LUT6_RADDR              ; /* 0x275a8 */
    volatile U_ISP_CLUT_LUT6_RDATA   ISP_CLUT_LUT6_RDATA              ; /* 0x275ac */
    volatile U_ISP_CLUT_LUT7_WADDR   ISP_CLUT_LUT7_WADDR              ; /* 0x275b0 */
    volatile U_ISP_CLUT_LUT7_WDATA   ISP_CLUT_LUT7_WDATA              ; /* 0x275b4 */
    volatile U_ISP_CLUT_LUT7_RADDR   ISP_CLUT_LUT7_RADDR              ; /* 0x275b8 */
    volatile U_ISP_CLUT_LUT7_RDATA   ISP_CLUT_LUT7_RDATA              ; /* 0x275bc */
    volatile unsigned int           reserved_161[144]                 ; /* 0x275c0~0x277fc */
    volatile U_ISP_LDCI_CFG         ISP_LDCI_CFG                      ; /* 0x27800 */
    volatile unsigned int           reserved_162[2]                   ; /* 0x27804~0x27808 */
    volatile U_ISP_LDCI_VERSION     ISP_LDCI_VERSION                  ; /* 0x2780c */
    volatile unsigned int           reserved_163                      ; /* 0x27810 */
    volatile U_ISP_LDCI_CALC_SMALL_OFFSET   ISP_LDCI_CALC_SMALL_OFFSET ; /* 0x27814 */
    volatile U_ISP_LDCI_ZONE        ISP_LDCI_ZONE                     ; /* 0x27818 */
    volatile U_ISP_LDCI_GLB_HEWGT   ISP_LDCI_GLB_HEWGT                ; /* 0x2781c */
    volatile unsigned int           reserved_164[2]                   ; /* 0x27820~0x27824 */
    volatile U_ISP_LDCI_SCALE       ISP_LDCI_SCALE                    ; /* 0x27828 */
    volatile unsigned int           reserved_165                      ; /* 0x2782c */
    volatile U_ISP_LDCI_DRCEN       ISP_LDCI_DRCEN                    ; /* 0x27830 */
    volatile U_ISP_LDCI_LUMA_SEL    ISP_LDCI_LUMA_SEL                 ; /* 0x27834 */
    volatile U_ISP_LDCI_BLC_CTRL    ISP_LDCI_BLC_CTRL                 ; /* 0x27838 */
    volatile unsigned int           reserved_166                      ; /* 0x2783c */
    volatile U_ISP_LDCI_PFLMT_EN    ISP_LDCI_PFLMT_EN                 ; /* 0x27840 */
    volatile U_ISP_LDCI_PFORI       ISP_LDCI_PFORI                    ; /* 0x27844 */
    volatile U_ISP_LDCI_PFSFT       ISP_LDCI_PFSFT                    ; /* 0x27848 */
    volatile U_ISP_LDCI_PFTHD       ISP_LDCI_PFTHD                    ; /* 0x2784c */
    volatile U_ISP_LDCI_PFRLY       ISP_LDCI_PFRLY                    ; /* 0x27850 */
    volatile U_ISP_LDCI_PFMUL       ISP_LDCI_PFMUL                    ; /* 0x27854 */
    volatile U_ISP_LDCI_LUT_UPDATE   ISP_LDCI_LUT_UPDATE              ; /* 0x27858 */
    volatile unsigned int           reserved_167[2]                   ; /* 0x2785c~0x27860 */
    volatile U_ISP_LDCI_LPF_LPFCOEF0   ISP_LDCI_LPF_LPFCOEF0          ; /* 0x27864 */
    volatile U_ISP_LDCI_LPF_LPFCOEF1   ISP_LDCI_LPF_LPFCOEF1          ; /* 0x27868 */
    volatile U_ISP_LDCI_LPF_LPFCOEF2   ISP_LDCI_LPF_LPFCOEF2          ; /* 0x2786c */
    volatile U_ISP_LDCI_LPF_LPFCOEF3   ISP_LDCI_LPF_LPFCOEF3          ; /* 0x27870 */
    volatile U_ISP_LDCI_LPF_LPFCOEF4   ISP_LDCI_LPF_LPFCOEF4          ; /* 0x27874 */
    volatile U_ISP_LDCI_LPF_LPFSFT   ISP_LDCI_LPF_LPFSFT              ; /* 0x27878 */
    volatile unsigned int           reserved_168                      ; /* 0x2787c */
    volatile U_ISP_LDCI_DRC_WADDR   ISP_LDCI_DRC_WADDR                ; /* 0x27880 */
    volatile U_ISP_LDCI_DRC_WDATA   ISP_LDCI_DRC_WDATA                ; /* 0x27884 */
    volatile U_ISP_LDCI_DRC_RADDR   ISP_LDCI_DRC_RADDR                ; /* 0x27888 */
    volatile U_ISP_LDCI_DRC_RDATA   ISP_LDCI_DRC_RDATA                ; /* 0x2788c */
    volatile U_ISP_LDCI_HE_WADDR    ISP_LDCI_HE_WADDR                 ; /* 0x27890 */
    volatile U_ISP_LDCI_HE_WDATA    ISP_LDCI_HE_WDATA                 ; /* 0x27894 */
    volatile U_ISP_LDCI_HE_RADDR    ISP_LDCI_HE_RADDR                 ; /* 0x27898 */
    volatile U_ISP_LDCI_HE_RDATA    ISP_LDCI_HE_RDATA                 ; /* 0x2789c */
    volatile U_ISP_LDCI_DE_USM_WADDR   ISP_LDCI_DE_USM_WADDR          ; /* 0x278a0 */
    volatile U_ISP_LDCI_DE_USM_WDATA   ISP_LDCI_DE_USM_WDATA          ; /* 0x278a4 */
    volatile U_ISP_LDCI_DE_USM_RADDR   ISP_LDCI_DE_USM_RADDR          ; /* 0x278a8 */
    volatile U_ISP_LDCI_DE_USM_RDATA   ISP_LDCI_DE_USM_RDATA          ; /* 0x278ac */
    volatile U_ISP_LDCI_CGAIN_WADDR   ISP_LDCI_CGAIN_WADDR            ; /* 0x278b0 */
    volatile U_ISP_LDCI_CGAIN_WDATA   ISP_LDCI_CGAIN_WDATA            ; /* 0x278b4 */
    volatile U_ISP_LDCI_CGAIN_RADDR   ISP_LDCI_CGAIN_RADDR            ; /* 0x278b8 */
    volatile U_ISP_LDCI_CGAIN_RDATA   ISP_LDCI_CGAIN_RDATA            ; /* 0x278bc */
    volatile unsigned int           reserved_169[24]                  ; /* 0x278c0~0x2791c */
    volatile U_ISP_LDCI_STAT_HPOS   ISP_LDCI_STAT_HPOS                ; /* 0x27920 */
    volatile U_ISP_LDCI_STAT_VPOS   ISP_LDCI_STAT_VPOS                ; /* 0x27924 */
    volatile U_ISP_LDCI_STAT_EVRATIO   ISP_LDCI_STAT_EVRATIO          ; /* 0x27928 */
    volatile unsigned int           reserved_170                      ; /* 0x2792c */
    volatile U_ISP_LDCI_STAT_ZONE   ISP_LDCI_STAT_ZONE                ; /* 0x27930 */
    volatile U_ISP_LDCI_BENDE       ISP_LDCI_BENDE                    ; /* 0x27934 */
    volatile U_ISP_LDCI_DEREF       ISP_LDCI_DEREF                    ; /* 0x27938 */
    volatile U_ISP_LDCI_DEH_LPFSFT   ISP_LDCI_DEH_LPFSFT              ; /* 0x2793c */
    volatile U_ISP_LDCI_LPFCOEFH    ISP_LDCI_LPFCOEFH                 ; /* 0x27940 */
    volatile U_ISP_LDCI_LPFCOEFV    ISP_LDCI_LPFCOEFV                 ; /* 0x27944 */
    volatile unsigned int           reserved_171[2]                   ; /* 0x27948~0x2794c */
    volatile U_ISP_LDCI_BLK_SMLMAPWIDTH0   ISP_LDCI_BLK_SMLMAPWIDTH0  ; /* 0x27950 */
    volatile U_ISP_LDCI_BLK_SMLMAPWIDTH1   ISP_LDCI_BLK_SMLMAPWIDTH1  ; /* 0x27954 */
    volatile U_ISP_LDCI_BLK_SMLMAPWIDTH2   ISP_LDCI_BLK_SMLMAPWIDTH2  ; /* 0x27958 */
    volatile unsigned int           reserved_172[2]                   ; /* 0x2795c~0x27960 */
    volatile U_ISP_LDCI_STAT_DRCEN   ISP_LDCI_STAT_DRCEN              ; /* 0x27964 */
    volatile U_ISP_LDCI_CHRDAMP     ISP_LDCI_CHRDAMP                  ; /* 0x27968 */
    volatile U_ISP_LDCI_GROBAL_MAP   ISP_LDCI_GROBAL_MAP              ; /* 0x2796c */
    volatile unsigned int           reserved_173[4]                   ; /* 0x27970~0x2797c */
    volatile U_ISP_LDCI_POLYP_WADDR   ISP_LDCI_POLYP_WADDR            ; /* 0x27980 */
    volatile U_ISP_LDCI_POLYP_WDATA   ISP_LDCI_POLYP_WDATA            ; /* 0x27984 */
    volatile U_ISP_LDCI_POLYP_RADDR   ISP_LDCI_POLYP_RADDR            ; /* 0x27988 */
    volatile U_ISP_LDCI_POLYP_RDATA   ISP_LDCI_POLYP_RDATA            ; /* 0x2798c */
    volatile U_ISP_LDCI_POLYQ01_WADDR   ISP_LDCI_POLYQ01_WADDR        ; /* 0x27990 */
    volatile U_ISP_LDCI_POLYQ01_WDATA   ISP_LDCI_POLYQ01_WDATA        ; /* 0x27994 */
    volatile U_ISP_LDCI_POLYQ01_RADDR   ISP_LDCI_POLYQ01_RADDR        ; /* 0x27998 */
    volatile U_ISP_LDCI_POLYQ01_RDATA   ISP_LDCI_POLYQ01_RDATA        ; /* 0x2799c */
    volatile U_ISP_LDCI_POLYQ23_WADDR   ISP_LDCI_POLYQ23_WADDR        ; /* 0x279a0 */
    volatile U_ISP_LDCI_POLYQ23_WDATA   ISP_LDCI_POLYQ23_WDATA        ; /* 0x279a4 */
    volatile U_ISP_LDCI_POLYQ23_RADDR   ISP_LDCI_POLYQ23_RADDR        ; /* 0x279a8 */
    volatile U_ISP_LDCI_POLYQ23_RDATA   ISP_LDCI_POLYQ23_RDATA        ; /* 0x279ac */
    volatile U_ISP_LDCI_LPF_MAP_WADDR   ISP_LDCI_LPF_MAP_WADDR        ; /* 0x279b0 */
    volatile U_ISP_LDCI_LPF_MAP_WDATA   ISP_LDCI_LPF_MAP_WDATA        ; /* 0x279b4 */
    volatile U_ISP_LDCI_LPF_MAP_RADDR   ISP_LDCI_LPF_MAP_RADDR        ; /* 0x279b8 */
    volatile U_ISP_LDCI_LPF_MAP_RDATA   ISP_LDCI_LPF_MAP_RDATA        ; /* 0x279bc */
    volatile unsigned int           reserved_174[144]                 ; /* 0x279c0~0x27bfc */
    volatile U_ISP_DRC_CFG          ISP_DRC_CFG                       ; /* 0x27c00 */
    volatile unsigned int           reserved_175[2]                   ; /* 0x27c04~0x27c08 */
    volatile U_ISP_DRC_VERSION      ISP_DRC_VERSION                   ; /* 0x27c0c */
    volatile U_ISP_DRC_ZONE         ISP_DRC_ZONE                      ; /* 0x27c10 */
    volatile U_ISP_DRC_ZONE_SIZE    ISP_DRC_ZONE_SIZE                 ; /* 0x27c14 */
    volatile U_ISP_DRC_ZONE_DIV0    ISP_DRC_ZONE_DIV0                 ; /* 0x27c18 */
    volatile U_ISP_DRC_ZONE_DIV1    ISP_DRC_ZONE_DIV1                 ; /* 0x27c1c */
    volatile U_ISP_DRC_BIN          ISP_DRC_BIN                       ; /* 0x27c20 */
    volatile U_ISP_DRC_ZONE_INIT    ISP_DRC_ZONE_INIT                 ; /* 0x27c24 */
    volatile U_ISP_DRC_BIN_SCALE    ISP_DRC_BIN_SCALE                 ; /* 0x27c28 */
    volatile U_ISP_DRC_OUTBITS      ISP_DRC_OUTBITS                   ; /* 0x27c2c */
    volatile unsigned int           reserved_176                      ; /* 0x27c30 */
    volatile U_ISP_DRC_LOCAL_EDGE_LMT   ISP_DRC_LOCAL_EDGE_LMT        ; /* 0x27c34 */
    volatile U_ISP_DRC_STRENGTH     ISP_DRC_STRENGTH                  ; /* 0x27c38 */
    volatile U_ISP_DRC_STAT_WIDTH   ISP_DRC_STAT_WIDTH                ; /* 0x27c3c */
    volatile U_ISP_DRC_DARK_GAIN_LMT_Y   ISP_DRC_DARK_GAIN_LMT_Y      ; /* 0x27c40 */
    volatile U_ISP_DRC_DARK_GAIN_LMT_C   ISP_DRC_DARK_GAIN_LMT_C      ; /* 0x27c44 */
    volatile U_ISP_DRC_BRIGHT_GAIN_LMT   ISP_DRC_BRIGHT_GAIN_LMT      ; /* 0x27c48 */
    volatile U_ISP_DRC_RGB_WGT      ISP_DRC_RGB_WGT                   ; /* 0x27c4c */
    volatile unsigned int           reserved_177                      ; /* 0x27c50 */
    volatile U_ISP_DRC_LUT_MIX_CRTL   ISP_DRC_LUT_MIX_CRTL            ; /* 0x27c54 */
    volatile U_ISP_DRC_GAIN_CLIP    ISP_DRC_GAIN_CLIP                 ; /* 0x27c58 */
    volatile U_ISP_DRC_COLOR_CTRL   ISP_DRC_COLOR_CTRL                ; /* 0x27c5c */
    volatile U_ISP_DRC_GLOBAL_CORR   ISP_DRC_GLOBAL_CORR              ; /* 0x27c60 */
    volatile U_ISP_DRC_MIXING_CORING   ISP_DRC_MIXING_CORING          ; /* 0x27c64 */
    volatile U_ISP_DRC_MIXING_DARK   ISP_DRC_MIXING_DARK              ; /* 0x27c68 */
    volatile U_ISP_DRC_MIXING_BRIGHT   ISP_DRC_MIXING_BRIGHT          ; /* 0x27c6c */
    volatile U_ISP_DRC_RG_CTRL      ISP_DRC_RG_CTRL                   ; /* 0x27c70 */
    volatile U_ISP_DRC_BG_CTRL      ISP_DRC_BG_CTRL                   ; /* 0x27c74 */
    volatile U_ISP_DRC_PDW          ISP_DRC_PDW                       ; /* 0x27c78 */
    volatile U_ISP_DRC_PPDTC_SUM    ISP_DRC_PPDTC_SUM                 ; /* 0x27c7c */
    volatile U_ISP_DRC_TMLUT0_WADDR   ISP_DRC_TMLUT0_WADDR            ; /* 0x27c80 */
    volatile U_ISP_DRC_TMLUT0_WDATA   ISP_DRC_TMLUT0_WDATA            ; /* 0x27c84 */
    volatile U_ISP_DRC_TMLUT0_RADDR   ISP_DRC_TMLUT0_RADDR            ; /* 0x27c88 */
    volatile U_ISP_DRC_TMLUT0_RDATA   ISP_DRC_TMLUT0_RDATA            ; /* 0x27c8c */
    volatile U_ISP_DRC_TMLUT1_WADDR   ISP_DRC_TMLUT1_WADDR            ; /* 0x27c90 */
    volatile U_ISP_DRC_TMLUT1_WDATA   ISP_DRC_TMLUT1_WDATA            ; /* 0x27c94 */
    volatile U_ISP_DRC_TMLUT1_RADDR   ISP_DRC_TMLUT1_RADDR            ; /* 0x27c98 */
    volatile U_ISP_DRC_TMLUT1_RDATA   ISP_DRC_TMLUT1_RDATA            ; /* 0x27c9c */
    volatile U_ISP_DRC_CCLUT_WADDR   ISP_DRC_CCLUT_WADDR              ; /* 0x27ca0 */
    volatile U_ISP_DRC_CCLUT_WDATA   ISP_DRC_CCLUT_WDATA              ; /* 0x27ca4 */
    volatile U_ISP_DRC_CCLUT_RADDR   ISP_DRC_CCLUT_RADDR              ; /* 0x27ca8 */
    volatile U_ISP_DRC_CCLUT_RDATA   ISP_DRC_CCLUT_RDATA              ; /* 0x27cac */
    volatile unsigned int           reserved_178[20]                  ; /* 0x27cb0~0x27cfc */
    volatile U_ISP_DRC_GRAD_REV     ISP_DRC_GRAD_REV                  ; /* 0x27d00 */
    volatile U_ISP_DRC_VBI_STRENGTH   ISP_DRC_VBI_STRENGTH            ; /* 0x27d04 */
    volatile U_ISP_DRC_ADA_COEF     ISP_DRC_ADA_COEF                  ; /* 0x27d08 */
    volatile unsigned int           reserved_179                      ; /* 0x27d0c */
    volatile U_ISP_DRC_VBI_STATE    ISP_DRC_VBI_STATE                 ; /* 0x27d10 */
    volatile U_ISP_DRC_DETAIL_CORING   ISP_DRC_DETAIL_CORING          ; /* 0x27d14 */
    volatile U_ISP_DRC_DETAIL_DARK   ISP_DRC_DETAIL_DARK              ; /* 0x27d18 */
    volatile U_ISP_DRC_DETAIL_BRIGHT   ISP_DRC_DETAIL_BRIGHT          ; /* 0x27d1c */
    volatile U_ISP_DRC_DARK_STEP    ISP_DRC_DARK_STEP                 ; /* 0x27d20 */
    volatile U_ISP_DRC_DARK_CURVE0   ISP_DRC_DARK_CURVE0              ; /* 0x27d24 */
    volatile U_ISP_DRC_DARK_CURVE1   ISP_DRC_DARK_CURVE1              ; /* 0x27d28 */
    volatile unsigned int           reserved_180                      ; /* 0x27d2c */
    volatile U_ISP_DRC_BRIGHT_STEP   ISP_DRC_BRIGHT_STEP              ; /* 0x27d30 */
    volatile U_ISP_DRC_BRIGHT_CURVE0   ISP_DRC_BRIGHT_CURVE0          ; /* 0x27d34 */
    volatile U_ISP_DRC_BRIGHT_CURVE1   ISP_DRC_BRIGHT_CURVE1          ; /* 0x27d38 */
    volatile U_ISP_DRC_PREV_LUMA_0   ISP_DRC_PREV_LUMA_0              ; /* 0x27d3c */
    volatile U_ISP_DRC_PREV_LUMA_1   ISP_DRC_PREV_LUMA_1              ; /* 0x27d40 */
    volatile U_ISP_DRC_PREV_LUMA_2   ISP_DRC_PREV_LUMA_2              ; /* 0x27d44 */
    volatile U_ISP_DRC_PREV_LUMA_3   ISP_DRC_PREV_LUMA_3              ; /* 0x27d48 */
    volatile U_ISP_DRC_PREV_LUMA_4   ISP_DRC_PREV_LUMA_4              ; /* 0x27d4c */
    volatile U_ISP_DRC_PREV_LUMA_5   ISP_DRC_PREV_LUMA_5              ; /* 0x27d50 */
    volatile U_ISP_DRC_PREV_LUMA_6   ISP_DRC_PREV_LUMA_6              ; /* 0x27d54 */
    volatile U_ISP_DRC_PREV_LUMA_7   ISP_DRC_PREV_LUMA_7              ; /* 0x27d58 */
    volatile unsigned int           reserved_181[41]                     ; /* 0x27d5c~0x27dfc */
    volatile U_ISP_DRC_CUBIC_THRESX01   ISP_DRC_CUBIC_THRESX01        ; /* 0x27e00 */
    volatile U_ISP_DRC_CUBIC_THRESX10   ISP_DRC_CUBIC_THRESX10        ; /* 0x27e04 */
    volatile U_ISP_DRC_CUBIC_THRESX11   ISP_DRC_CUBIC_THRESX11        ; /* 0x27e08 */
    volatile U_ISP_DRC_CUBIC_SEL    ISP_DRC_CUBIC_SEL                 ; /* 0x27e0c */
    volatile U_ISP_DRC_CUBIC_COEF00   ISP_DRC_CUBIC_COEF00            ; /* 0x27e10 */
    volatile U_ISP_DRC_CUBIC_COEF01   ISP_DRC_CUBIC_COEF01            ; /* 0x27e14 */
    volatile U_ISP_DRC_CUBIC_COEF02   ISP_DRC_CUBIC_COEF02            ; /* 0x27e18 */
    volatile U_ISP_DRC_CUBIC_COEF03   ISP_DRC_CUBIC_COEF03            ; /* 0x27e1c */
    volatile U_ISP_DRC_CUBIC_COEF10   ISP_DRC_CUBIC_COEF10            ; /* 0x27e20 */
    volatile U_ISP_DRC_CUBIC_COEF11   ISP_DRC_CUBIC_COEF11            ; /* 0x27e24 */
    volatile U_ISP_DRC_CUBIC_COEF12   ISP_DRC_CUBIC_COEF12            ; /* 0x27e28 */
    volatile U_ISP_DRC_CUBIC_COEF13   ISP_DRC_CUBIC_COEF13            ; /* 0x27e2c */
    volatile U_ISP_DRC_CUBIC_COEF20   ISP_DRC_CUBIC_COEF20            ; /* 0x27e30 */
    volatile U_ISP_DRC_CUBIC_COEF21   ISP_DRC_CUBIC_COEF21            ; /* 0x27e34 */
    volatile U_ISP_DRC_CUBIC_COEF22   ISP_DRC_CUBIC_COEF22            ; /* 0x27e38 */
    volatile U_ISP_DRC_CUBIC_COEF23   ISP_DRC_CUBIC_COEF23            ; /* 0x27e3c */
    volatile U_ISP_DRC_CUBIC_COEF30   ISP_DRC_CUBIC_COEF30            ; /* 0x27e40 */
    volatile U_ISP_DRC_CUBIC_COEF31   ISP_DRC_CUBIC_COEF31            ; /* 0x27e44 */
    volatile U_ISP_DRC_CUBIC_COEF32   ISP_DRC_CUBIC_COEF32            ; /* 0x27e48 */
    volatile U_ISP_DRC_CUBIC_COEF33   ISP_DRC_CUBIC_COEF33            ; /* 0x27e4c */
    volatile U_ISP_DRC_FLT_CFG      ISP_DRC_FLT_CFG                   ; /* 0x27e50 */
    volatile U_ISP_DRC_BIN_FACTOR_COARSE_0   ISP_DRC_BIN_FACTOR_COARSE_0 ; /* 0x27e54 */
    volatile U_ISP_DRC_BIN_FACTOR_COARSE_1   ISP_DRC_BIN_FACTOR_COARSE_1 ; /* 0x27e58 */
    volatile U_ISP_DRC_BIN_FACTOR_MEDIUM_0   ISP_DRC_BIN_FACTOR_MEDIUM_0 ; /* 0x27e5c */
    volatile U_ISP_DRC_BIN_FACTOR_MEDIUM_1   ISP_DRC_BIN_FACTOR_MEDIUM_1 ; /* 0x27e60 */
    volatile U_ISP_DRC_DETAIL_SUB_FACTOR   ISP_DRC_DETAIL_SUB_FACTOR  ; /* 0x27e64 */
    volatile U_ISP_DRC_WGT_BOX_TRI_SEL   ISP_DRC_WGT_BOX_TRI_SEL      ; /* 0x27e68 */
    volatile U_ISP_DRC_BLK_WGT_INIT   ISP_DRC_BLK_WGT_INIT            ; /* 0x27e6c */
    volatile U_ISP_DRC_SHP_CFG      ISP_DRC_SHP_CFG                   ; /* 0x27e70 */
    volatile U_ISP_DRC_DIV_DENOM_LOG   ISP_DRC_DIV_DENOM_LOG          ; /* 0x27e74 */
    volatile U_ISP_DRC_DENOM_EXP    ISP_DRC_DENOM_EXP                 ; /* 0x27e78 */
    volatile U_ISP_DRC_DEBUG_INFO   ISP_DRC_DEBUG_INFO                ; /* 0x27e7c */
    volatile unsigned int           reserved_182[611]                 ; /* 0x27e80~0x28808 */
    volatile U_ISP_SQRT_VERSION     ISP_SQRT_VERSION                  ; /* 0x2880c */
    volatile unsigned int           reserved_183[63]                  ; /* 0x28810~0x28908 */
    volatile U_ISP_SQ_VERSION       ISP_SQ_VERSION                    ; /* 0x2890c */
    volatile unsigned int           reserved_184[63]                  ; /* 0x28910~0x28a08 */
    volatile U_ISP_SQRT1_VERSION    ISP_SQRT1_VERSION                 ; /* 0x28a0c */
    volatile unsigned int           reserved_185[2559]                ; /* 0x28a10~0x2b208 */
    volatile U_ISP_SPLIT_VERSION    ISP_SPLIT_VERSION                 ; /* 0x2b20c */
    volatile U_ISP_SPLIT_DATA_CFG   ISP_SPLIT_DATA_CFG                ; /* 0x2b210 */
    volatile U_ISP_SPLIT_DATA_VERSION   ISP_SPLIT_DATA_VERSION        ; /* 0x2b214 */
    volatile U_ISP_SPLIT_DATA_BITWIDTH   ISP_SPLIT_DATA_BITWIDTH      ; /* 0x2b218 */
    volatile U_ISP_SPLIT_DATA_AGAIN   ISP_SPLIT_DATA_AGAIN            ; /* 0x2b21c */
    volatile U_ISP_SPLIT_DATA_DGAIN   ISP_SPLIT_DATA_DGAIN            ; /* 0x2b220 */
    volatile U_ISP_SPLIT_DATA_SHUTTER   ISP_SPLIT_DATA_SHUTTER        ; /* 0x2b224 */

    volatile unsigned int           reserved_345[2]                   ; /* 0xb228 ~ 0xb22c */
    S_VIPROC_REGS_TYPE              stViProcReg                       ; /* 0xb230 ~ 0xb32c */

    /* for align */
    volatile unsigned int           reserved_346[52]; /* 0xb930 ~ 0xb9fc */
    S_ISP_LUT_REGS_TYPE             stIspBeLut;      /* 0xba00 ~ 0x372d4 */
    volatile unsigned int           reserved_347[10]; /* 0x372d8 ~ 0x372fc */
} S_ISPBE_REGS_TYPE;


/* Declare the struct pointor of the module VICAP */

#endif /* __C_UNION_DEFINE_H__ */
