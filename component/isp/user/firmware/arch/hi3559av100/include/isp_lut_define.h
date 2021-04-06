// ******************************************************************************
// Copyright     :  Copyright (C) 2017, Hisilicon Technologies Co., Ltd.
// File name     :  isp_lut_define.h
// Author        :
// Version       :  1.0
// Date          :  2017-02-23
// Description   :  Define all registers/tables for Hi3559AV100
// History       :  2017-02-23 Create file
// ******************************************************************************

#ifndef __ISP_LUT_DEFINE_H__
#define __ISP_LUT_DEFINE_H__

/* Define the union U_ISP_AE_WEIGHT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ae_weight0        : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    isp_ae_weight1        : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    isp_ae_weight2        : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 4   ; /* [23..20]  */
        unsigned int    isp_ae_weight3        : 4   ; /* [27..24]  */
        unsigned int    reserved_3            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_AE_WEIGHT;

/* Define the union U_ISP_LSC_RGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_rgain0        : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_rgain1        : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_RGAIN;

/* Define the union U_ISP_LSC_GRGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_grgain0       : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_grgain1       : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GRGAIN;

/* Define the union U_ISP_LSC_BGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_bgain0        : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_bgain1        : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_BGAIN;

/* Define the union U_ISP_LSC_GBGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_lsc_gbgain0       : 10  ; /* [9..0]  */
        unsigned int    isp_lsc_gbgain1       : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LSC_GBGAIN;

/* Define the union U_ISP_FPN_LINE_WLUT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_fpn_line_wlut      : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_FPN_LINE_WLUT;
/* Define the union U_ISP_DPC_BPT_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dpc_bpt_wlut_x    : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    isp_dpc_bpt_wlut_y    : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DPC_BPT_WLUT;

/* Define the union U_ISP_SHARPEN_MFGAIND */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_mfgaind   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFGAIND;

/* Define the union U_ISP_SHARPEN_MFGAINUD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_mfgainud  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_MFGAINUD;

/* Define the union U_ISP_SHARPEN_HFGAIND */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_hfgaind   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFGAIND;

/* Define the union U_ISP_SHARPEN_HFGAINUD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_sharpen_hfgainud  : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_SHARPEN_HFGAINUD;

/* Define the union U_ISP_NDDM_GF_LUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_gflut        : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_GF_LUT;

/* Define the union U_ISP_NDDM_USM_LUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_nddm_usmlut       : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_NDDM_USM_LUT;

/* Define the union U_ISP_BNR_LMT_EVEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lmt_even      : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LMT_EVEN;

/* Define the union U_ISP_BNR_LMT_ODD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lmt_odd       : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LMT_ODD;

/* Define the union U_ISP_BNR_COR_EVEN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_cor_even      : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_COR_EVEN;

/* Define the union U_ISP_BNR_COR_ODD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_cor_odd       : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_COR_ODD;

/* Define the union U_ISP_BNR_LSC_RGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_rgain0    : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_rgain1    : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_RGAIN;

/* Define the union U_ISP_BNR_LSC_GRGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_grgain0   : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_grgain1   : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_GRGAIN;

/* Define the union U_ISP_BNR_LSC_BGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_bgain0    : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_bgain1    : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_BGAIN;

/* Define the union U_ISP_BNR_LSC_GBGAIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_bnr_lsc_gbgain0   : 10  ; /* [9..0]  */
        unsigned int    isp_bnr_lsc_gbgain1   : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_BNR_LSC_GBGAIN;

/* Define the union U_ISP_WDR_NOSLUT129X8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdr_noslut129x8   : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDR_NOSLUT129X8;

/* Define the union U_ISP_DEHAZE_PRESTAT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_prestat_l  : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_dehaze_prestat_h  : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_PRESTAT;

/* Define the union U_ISP_DEHAZE_LUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_dehaze_dehaze_lut : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DEHAZE_LUT;

/* Define the union U_ISP_PREGAMMA_LUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_pregamma_lut      : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_PREGAMMA_LUT;

/* Define the union U_ISP_GAMMA_LUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_gamma_lut         : 14  ; /* [13..0]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_GAMMA_LUT;

/* Define the union U_ISP_CA_LUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ca_lut            : 24  ; /* [23..0]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CA_LUT;

/* Define the union U_ISP_CLUT_LUT0_WLUT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut0          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT0_WLUT;
/* Define the union U_ISP_CLUT_LUT1_WLUT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut1          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT1_WLUT;
/* Define the union U_ISP_CLUT_LUT2_WLUT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut2          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT2_WLUT;
/* Define the union U_ISP_CLUT_LUT3_WLUT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut3          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT3_WLUT;
/* Define the union U_ISP_CLUT_LUT4_WLUT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut4          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT4_WLUT;
/* Define the union U_ISP_CLUT_LUT5_WLUT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut5          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT5_WLUT;
/* Define the union U_ISP_CLUT_LUT6_WLUT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut6          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT6_WLUT;
/* Define the union U_ISP_CLUT_LUT7_WLUT */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int isp_clut_lut7          : 32  ; /* [31..0]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_CLUT_LUT7_WLUT;
/* Define the union U_ISP_LDCI_DRC_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_calcdrc_wlut : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    isp_ldci_statdrc_wlut : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DRC_WLUT;

/* Define the union U_ISP_LDCI_HE_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_hepos_wlut   : 7   ; /* [6..0]  */
        unsigned int    isp_ldci_heneg_wlut   : 7   ; /* [13..7]  */
        unsigned int    reserved_0            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_HE_WLUT;

/* Define the union U_ISP_LDCI_DE_USM_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_usmpos_wlut  : 9   ; /* [8..0]  */
        unsigned int    isp_ldci_usmneg_wlut  : 9   ; /* [17..9]  */
        unsigned int    isp_ldci_delut_wlut   : 9   ; /* [26..18]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_DE_USM_WLUT;

/* Define the union U_ISP_LDCI_CGAIN_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_cgain_wlut   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_CGAIN_WLUT;

/* Define the union U_ISP_LDCI_POLYP_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_poply1_wlut  : 10  ; /* [9..0]  */
        unsigned int    isp_ldci_poply2_wlut  : 10  ; /* [19..10]  */
        unsigned int    isp_ldci_poply3_wlut  : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYP_WLUT;

/* Define the union U_ISP_LDCI_POLYQ01_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_plyq0_wlut   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ldci_plyq1_wlut   : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYQ01_WLUT;

/* Define the union U_ISP_LDCI_POLYQ23_WLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_ldci_plyq2_wlut   : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    isp_ldci_plyq3_wlut   : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LDCI_POLYQ23_WLUT;

/* Define the union U_ISP_DRC_TMLUT0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_tmlut0_diff   : 12  ; /* [11..0]  */
        unsigned int    isp_drc_tmlut0_value  : 16  ; /* [27..12]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TMLUT0;

/* Define the union U_ISP_DRC_TMLUT1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_tmlut1_diff   : 12  ; /* [11..0]  */
        unsigned int    isp_drc_tmlut1_value  : 16  ; /* [27..12]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_TMLUT1;

/* Define the union U_ISP_DRC_CCLUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_drc_cclut         : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_DRC_CCLUT;

/* Define the union U_ISP_WDRSPLIT_LUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_wdrsplit_lut      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_WDRSPLIT_LUT;

/* Define the union U_ISP_LOGLUT_LUT */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_loglut_lut        : 21  ; /* [20..0]  */
        unsigned int    reserved_0            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_LOGLUT_LUT;

/* Define the union U_ISP_RLSC_LUT0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_lut0_ch0     : 16  ; /* [15..0]  */
        unsigned int    isp_rlsc_lut0_ch1     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT0;

/* Define the union U_ISP_RLSC_LUT1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    isp_rlsc_lut1_ch2     : 16  ; /* [15..0]  */
        unsigned int    isp_rlsc_lut1_ch3     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ISP_RLSC_LUT1;

/* Define the global struct */
typedef struct {
    volatile U_ISP_AE_WEIGHT        ISP_AE_WEIGHT[64]                 ; /* 0x0~0xfc */
    volatile U_ISP_LSC_RGAIN        ISP_LSC_RGAIN[1089]               ; /* 0x100~0x1200 */
    volatile unsigned int           reserved_0[3]                     ; /* 0x1204~0x120c */
    volatile U_ISP_LSC_GRGAIN       ISP_LSC_GRGAIN[1089]              ; /* 0x1210~0x2310 */
    volatile unsigned int           reserved_1[3]                     ; /* 0x2314~0x231c */
    volatile U_ISP_LSC_BGAIN        ISP_LSC_BGAIN[1089]               ; /* 0x2320~0x3420 */
    volatile unsigned int           reserved_2[3]                     ; /* 0x3424~0x342c */
    volatile U_ISP_LSC_GBGAIN       ISP_LSC_GBGAIN[1089]              ; /* 0x3430~0x4530 */
    volatile unsigned int           reserved_3[3]                     ; /* 0x4534~0x453c */
    volatile U_ISP_FPN_LINE_WLUT    ISP_FPN_LINE_WLUT[4352]           ; /* 0x4540~0x893c */
    volatile U_ISP_DPC_BPT_WLUT     ISP_DPC_BPT_WLUT[4096]            ; /* 0x8940~0xc93c */
    volatile U_ISP_SHARPEN_MFGAIND   ISP_SHARPEN_MFGAIND[64]          ; /* 0xc940~0xca3c */
    volatile U_ISP_SHARPEN_MFGAINUD   ISP_SHARPEN_MFGAINUD[64]        ; /* 0xca40~0xcb3c */
    volatile U_ISP_SHARPEN_HFGAIND   ISP_SHARPEN_HFGAIND[64]          ; /* 0xcb40~0xcc3c */
    volatile U_ISP_SHARPEN_HFGAINUD   ISP_SHARPEN_HFGAINUD[64]        ; /* 0xcc40~0xcd3c */
    volatile U_ISP_NDDM_GF_LUT      ISP_NDDM_GF_LUT[17]               ; /* 0xcd40~0xcd80 */
    volatile unsigned int           reserved_4[3]                     ; /* 0xcd84~0xcd8c */
    volatile U_ISP_NDDM_USM_LUT     ISP_NDDM_USM_LUT[17]              ; /* 0xcd90~0xcdd0 */
    volatile unsigned int           reserved_5[3]                     ; /* 0xcdd4~0xcddc */
    volatile U_ISP_BNR_LMT_EVEN     ISP_BNR_LMT_EVEN[65]              ; /* 0xcde0~0xcee0 */
    volatile unsigned int           reserved_6[3]                     ; /* 0xcee4~0xceec */
    volatile U_ISP_BNR_LMT_ODD      ISP_BNR_LMT_ODD[64]               ; /* 0xcef0~0xcfec */
    volatile U_ISP_BNR_COR_EVEN     ISP_BNR_COR_EVEN[17]              ; /* 0xcff0~0xd030 */
    volatile unsigned int           reserved_7[3]                     ; /* 0xd034~0xd03c */
    volatile U_ISP_BNR_COR_ODD      ISP_BNR_COR_ODD[16]               ; /* 0xd040~0xd07c */
    volatile U_ISP_BNR_LSC_RGAIN    ISP_BNR_LSC_RGAIN[1089]           ; /* 0xd080~0xe180 */
    volatile unsigned int           reserved_8[3]                     ; /* 0xe184~0xe18c */
    volatile U_ISP_BNR_LSC_GRGAIN   ISP_BNR_LSC_GRGAIN[1089]          ; /* 0xe190~0xf290 */
    volatile unsigned int           reserved_9[3]                     ; /* 0xf294~0xf29c */
    volatile U_ISP_BNR_LSC_BGAIN    ISP_BNR_LSC_BGAIN[1089]           ; /* 0xf2a0~0x103a0 */
    volatile unsigned int           reserved_10[3]                     ; /* 0x103a4~0x103ac */
    volatile U_ISP_BNR_LSC_GBGAIN   ISP_BNR_LSC_GBGAIN[1089]          ; /* 0x103b0~0x114b0 */
    volatile unsigned int           reserved_11[3]                     ; /* 0x114b4~0x114bc */
    volatile U_ISP_WDR_NOSLUT129X8   ISP_WDR_NOSLUT129X8[129]         ; /* 0x114c0~0x116c0 */
    volatile unsigned int           reserved_12[3]                     ; /* 0x116c4~0x116cc */
    volatile U_ISP_DEHAZE_PRESTAT   ISP_DEHAZE_PRESTAT[512]           ; /* 0x116d0~0x11ecc */
    volatile U_ISP_DEHAZE_LUT       ISP_DEHAZE_LUT[256]               ; /* 0x11ed0~0x122cc */
    volatile U_ISP_PREGAMMA_LUT     ISP_PREGAMMA_LUT[257]             ; /* 0x122d0~0x126d0 */
    volatile unsigned int           reserved_13[3]                     ; /* 0x126d4~0x126dc */
    volatile U_ISP_GAMMA_LUT        ISP_GAMMA_LUT[1025]               ; /* 0x126e0~0x136e0 */
    volatile unsigned int           reserved_14[3]                     ; /* 0x136e4~0x136ec */
    volatile U_ISP_CA_LUT           ISP_CA_LUT[256]                   ; /* 0x136f0~0x13aec */
    volatile U_ISP_CLUT_LUT0_WLUT   ISP_CLUT_LUT0_WLUT[3087]          ; /* 0x13af0~0x16b28 */
    volatile unsigned int           reserved_15                     ; /* 0x16b2c */
    volatile U_ISP_CLUT_LUT1_WLUT   ISP_CLUT_LUT1_WLUT[2871]          ; /* 0x16b30~0x19808 */
    volatile unsigned int           reserved_16                     ; /* 0x1980c */
    volatile U_ISP_CLUT_LUT2_WLUT   ISP_CLUT_LUT2_WLUT[2871]          ; /* 0x19810~0x1c4e8 */
    volatile unsigned int           reserved_17                     ; /* 0x1c4ec */
    volatile U_ISP_CLUT_LUT3_WLUT   ISP_CLUT_LUT3_WLUT[2664]          ; /* 0x1c4f0~0x1ee8c */
    volatile U_ISP_CLUT_LUT4_WLUT   ISP_CLUT_LUT4_WLUT[2871]          ; /* 0x1ee90~0x21b68 */
    volatile unsigned int           reserved_18                     ; /* 0x21b6c */
    volatile U_ISP_CLUT_LUT5_WLUT   ISP_CLUT_LUT5_WLUT[2664]          ; /* 0x21b70~0x2450c */
    volatile U_ISP_CLUT_LUT6_WLUT   ISP_CLUT_LUT6_WLUT[2664]          ; /* 0x24510~0x26eac */
    volatile U_ISP_CLUT_LUT7_WLUT   ISP_CLUT_LUT7_WLUT[2475]          ; /* 0x26eb0~0x29558 */
    volatile unsigned int           reserved_19                     ; /* 0x2955c */
    volatile U_ISP_LDCI_DRC_WLUT    ISP_LDCI_DRC_WLUT[65]             ; /* 0x29560~0x29660 */
    volatile unsigned int           reserved_20[3]                     ; /* 0x29664~0x2966c */
    volatile U_ISP_LDCI_HE_WLUT     ISP_LDCI_HE_WLUT[33]              ; /* 0x29670~0x296f0 */
    volatile unsigned int           reserved_21[3]                     ; /* 0x296f4~0x296fc */
    volatile U_ISP_LDCI_DE_USM_WLUT   ISP_LDCI_DE_USM_WLUT[33]        ; /* 0x29700~0x29780 */
    volatile unsigned int           reserved_22[3]                     ; /* 0x29784~0x2978c */
    volatile U_ISP_LDCI_CGAIN_WLUT   ISP_LDCI_CGAIN_WLUT[65]          ; /* 0x29790~0x29890 */
    volatile unsigned int           reserved_23[3]                     ; /* 0x29894~0x2989c */
    volatile U_ISP_LDCI_POLYP_WLUT   ISP_LDCI_POLYP_WLUT[65]          ; /* 0x298a0~0x299a0 */
    volatile unsigned int           reserved_24[3]                     ; /* 0x299a4~0x299ac */
    volatile U_ISP_LDCI_POLYQ01_WLUT   ISP_LDCI_POLYQ01_WLUT[65]      ; /* 0x299b0~0x29ab0 */
    volatile unsigned int           reserved_25[3]                     ; /* 0x29ab4~0x29abc */
    volatile U_ISP_LDCI_POLYQ23_WLUT   ISP_LDCI_POLYQ23_WLUT[65]      ; /* 0x29ac0~0x29bc0 */
    volatile unsigned int           reserved_26[3]                     ; /* 0x29bc4~0x29bcc */
    volatile U_ISP_DRC_TMLUT0       ISP_DRC_TMLUT0[200]               ; /* 0x29bd0~0x29eec */
    volatile U_ISP_DRC_TMLUT1       ISP_DRC_TMLUT1[200]               ; /* 0x29ef0~0x2a20c */
    volatile U_ISP_DRC_CCLUT        ISP_DRC_CCLUT[33]                 ; /* 0x2a210~0x2a290 */
    volatile unsigned int           reserved_27[3]                     ; /* 0x2a294~0x2a29c */
    volatile U_ISP_WDRSPLIT_LUT     ISP_WDRSPLIT_LUT[129]             ; /* 0x2a2a0~0x2a4a0 */
    volatile unsigned int           reserved_28[3]                     ; /* 0x2a4a4~0x2a4ac */
    volatile U_ISP_LOGLUT_LUT       ISP_LOGLUT_LUT[1025]              ; /* 0x2a4b0~0x2b4b0 */
    volatile unsigned int           reserved_29[3]                     ; /* 0x2b4b4~0x2b4bc */
    volatile U_ISP_RLSC_LUT0        ISP_RLSC_LUT0[130]                ; /* 0x2b4c0~0x2b6c4 */
    volatile unsigned int           reserved_30[2]                     ; /* 0x2b6c8~0x2b6cc */
    volatile U_ISP_RLSC_LUT1        ISP_RLSC_LUT1[130]                ; /* 0x2b6d0~0x2b8d4 */
} S_ISP_LUT_REGS_TYPE;


#endif /* __ISP_LUT_DEFINE_H__ */
