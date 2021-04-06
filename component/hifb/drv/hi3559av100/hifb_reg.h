/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#ifndef __HIFB_REG_H__
#define __HIFB_REG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/****************************************************************************
*
* VDP Architecture IP
*
****************************************************************************/
/* Define the union U_VOCTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 25; /* [24..0]  */
        unsigned int wbc_dhd_ck_gt_en : 1; /* [25]  */
        unsigned int g1_ck_gt_en : 1; /* [26]  */
        unsigned int g0_ck_gt_en : 1; /* [27]  */
        unsigned int v1_ck_gt_en : 1; /* [28]  */
        unsigned int v0_ck_gt_en : 1; /* [29]  */
        unsigned int chk_sum_en : 1; /* [30]  */
        unsigned int vo_ck_gt_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOCTRL;

/* Define the union U_VOINTSTA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_int : 1; /* [0]  */
        unsigned int dhd0vtthd2_int : 1; /* [1]  */
        unsigned int dhd0vtthd3_int : 1; /* [2]  */
        unsigned int dhd0uf_int : 1; /* [3]  */
        unsigned int dhd1vtthd1_int : 1; /* [4]  */
        unsigned int dhd1vtthd2_int : 1; /* [5]  */
        unsigned int dhd1vtthd3_int : 1; /* [6]  */
        unsigned int dhd1uf_int : 1; /* [7]  */
        unsigned int dsdvtthd1_int : 1; /* [8]  */
        unsigned int dsdvtthd2_int : 1; /* [9]  */
        unsigned int dsdvtthd3_int : 1; /* [10]  */
        unsigned int dsduf_int : 1; /* [11]  */
        unsigned int b0_err_int : 1; /* [12]  */
        unsigned int b1_err_int : 1; /* [13]  */
        unsigned int b2_err_int : 1; /* [14]  */
        unsigned int wbc_dhd_over_int : 1; /* [15]  */
        unsigned int vdac0_int : 1; /* [16]  */
        unsigned int vdac1_int : 1; /* [17]  */
        unsigned int vdac2_int : 1; /* [18]  */
        unsigned int vdac3_int : 1; /* [19]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOINTSTA;

/* Define the union U_VOMSKINTSTA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_clr : 1; /* [0]  */
        unsigned int dhd0vtthd2_clr : 1; /* [1]  */
        unsigned int dhd0vtthd3_clr : 1; /* [2]  */
        unsigned int dhd0uf_clr : 1; /* [3]  */
        unsigned int dhd1vtthd1_clr : 1; /* [4]  */
        unsigned int dhd1vtthd2_clr : 1; /* [5]  */
        unsigned int dhd1vtthd3_clr : 1; /* [6]  */
        unsigned int dhd1uf_clr : 1; /* [7]  */
        unsigned int dsdvtthd1_clr : 1; /* [8]  */
        unsigned int dsdvtthd2_clr : 1; /* [9]  */
        unsigned int dsdvtthd3_clr : 1; /* [10]  */
        unsigned int dsduf_clr : 1; /* [11]  */
        unsigned int b0_err_clr : 1; /* [12]  */
        unsigned int b1_err_clr : 1; /* [13]  */
        unsigned int b2_err_clr : 1; /* [14]  */
        unsigned int wbc_dhd_over_clr : 1; /* [15]  */
        unsigned int vdac0_clr : 1; /* [16]  */
        unsigned int vdac1_clr : 1; /* [17]  */
        unsigned int vdac2_clr : 1; /* [18]  */
        unsigned int vdac3_clr : 1; /* [19]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOMSKINTSTA;

/* Define the union U_VOINTMSK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_intmask : 1; /* [0]  */
        unsigned int dhd0vtthd2_intmask : 1; /* [1]  */
        unsigned int dhd0vtthd3_intmask : 1; /* [2]  */
        unsigned int dhd0uf_intmask : 1; /* [3]  */
        unsigned int dhd1vtthd1_intmask : 1; /* [4]  */
        unsigned int dhd1vtthd2_intmask : 1; /* [5]  */
        unsigned int dhd1vtthd3_intmask : 1; /* [6]  */
        unsigned int dhd1uf_intmask : 1; /* [7]  */
        unsigned int dsdvtthd1_intmask : 1; /* [8]  */
        unsigned int dsdvtthd2_intmask : 1; /* [9]  */
        unsigned int dsdvtthd3_intmask : 1; /* [10]  */
        unsigned int dsduf_intmask : 1; /* [11]  */
        unsigned int b0_err_intmask : 1; /* [12]  */
        unsigned int b1_err_intmask : 1; /* [13]  */
        unsigned int b2_err_intmask : 1; /* [14]  */
        unsigned int wbc_dhd_over_intmask : 1; /* [15]  */
        unsigned int vdac0_intmask : 1; /* [16]  */
        unsigned int vdac1_intmask : 1; /* [17]  */
        unsigned int vdac2_intmask : 1; /* [18]  */
        unsigned int vdac3_intmask : 1; /* [19]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOINTMSK;

/* Define the union U_VODEBUG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rm_en_chn : 4; /* [3..0]  */
        unsigned int dhd0_ff_info : 2; /* [5..4]  */
        unsigned int dhd1_ff_info : 2; /* [7..6]  */
        unsigned int dsd0_ff_info : 2; /* [9..8]  */
        unsigned int bfm_vga_en : 1; /* [10]  */
        unsigned int bfm_cvbs_en : 1; /* [11]  */
        unsigned int bfm_lcd_en : 1; /* [12]  */
        unsigned int bfm_bt1120_en : 1; /* [13]  */
        unsigned int wbc2_ff_info : 2; /* [15..14]  */
        unsigned int wbc_mode : 4; /* [19..16]  */
        unsigned int node_num : 4; /* [23..20]  */
        unsigned int wbc_cmp_mode : 2; /* [25..24]  */
        unsigned int bfm_mode : 3; /* [28..26]  */
        unsigned int bfm_clk_sel : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VODEBUG;

/* Define the union U_VOINTSTA1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_int : 1; /* [0]  */
        unsigned int dhd0vtthd2_int : 1; /* [1]  */
        unsigned int dhd0vtthd3_int : 1; /* [2]  */
        unsigned int dhd0uf_int : 1; /* [3]  */
        unsigned int dhd1vtthd1_int : 1; /* [4]  */
        unsigned int dhd1vtthd2_int : 1; /* [5]  */
        unsigned int dhd1vtthd3_int : 1; /* [6]  */
        unsigned int dhd1uf_int : 1; /* [7]  */
        unsigned int dsdvtthd1_int : 1; /* [8]  */
        unsigned int dsdvtthd2_int : 1; /* [9]  */
        unsigned int dsdvtthd3_int : 1; /* [10]  */
        unsigned int dsduf_int : 1; /* [11]  */
        unsigned int b0_err_int : 1; /* [12]  */
        unsigned int b1_err_int : 1; /* [13]  */
        unsigned int b2_err_int : 1; /* [14]  */
        unsigned int wbc_dhd_over_int : 1; /* [15]  */
        unsigned int vdac0_int : 1; /* [16]  */
        unsigned int vdac1_int : 1; /* [17]  */
        unsigned int vdac2_int : 1; /* [18]  */
        unsigned int vdac3_int : 1; /* [19]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOINTSTA1;

/* Define the union U_VOMSKINTSTA1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_clr : 1; /* [0]  */
        unsigned int dhd0vtthd2_clr : 1; /* [1]  */
        unsigned int dhd0vtthd3_clr : 1; /* [2]  */
        unsigned int dhd0uf_clr : 1; /* [3]  */
        unsigned int dhd1vtthd1_clr : 1; /* [4]  */
        unsigned int dhd1vtthd2_clr : 1; /* [5]  */
        unsigned int dhd1vtthd3_clr : 1; /* [6]  */
        unsigned int dhd1uf_clr : 1; /* [7]  */
        unsigned int dsdvtthd1_clr : 1; /* [8]  */
        unsigned int dsdvtthd2_clr : 1; /* [9]  */
        unsigned int dsdvtthd3_clr : 1; /* [10]  */
        unsigned int dsduf_clr : 1; /* [11]  */
        unsigned int b0_err_clr : 1; /* [12]  */
        unsigned int b1_err_clr : 1; /* [13]  */
        unsigned int b2_err_clr : 1; /* [14]  */
        unsigned int wbc_dhd_over_clr : 1; /* [15]  */
        unsigned int vdac0_clr : 1; /* [16]  */
        unsigned int vdac1_clr : 1; /* [17]  */
        unsigned int vdac2_clr : 1; /* [18]  */
        unsigned int vdac3_clr : 1; /* [19]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOMSKINTSTA1;

/* Define the union U_VOINTMSK1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dhd0vtthd1_intmask : 1; /* [0]  */
        unsigned int dhd0vtthd2_intmask : 1; /* [1]  */
        unsigned int dhd0vtthd3_intmask : 1; /* [2]  */
        unsigned int dhd0uf_intmask : 1; /* [3]  */
        unsigned int dhd1vtthd1_intmask : 1; /* [4]  */
        unsigned int dhd1vtthd2_intmask : 1; /* [5]  */
        unsigned int dhd1vtthd3_intmask : 1; /* [6]  */
        unsigned int dhd1uf_intmask : 1; /* [7]  */
        unsigned int dsdvtthd1_intmask : 1; /* [8]  */
        unsigned int dsdvtthd2_intmask : 1; /* [9]  */
        unsigned int dsdvtthd3_intmask : 1; /* [10]  */
        unsigned int dsduf_intmask : 1; /* [11]  */
        unsigned int b0_err_intmask : 1; /* [12]  */
        unsigned int b1_err_intmask : 1; /* [13]  */
        unsigned int b2_err_intmask : 1; /* [14]  */
        unsigned int wbc_dhd_over_intmask : 1; /* [15]  */
        unsigned int vdac0_intmask : 1; /* [16]  */
        unsigned int vdac1_intmask : 1; /* [17]  */
        unsigned int vdac2_intmask : 1; /* [18]  */
        unsigned int vdac3_intmask : 1; /* [19]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOINTMSK1;

/* Define the union U_VOLOWPOWER_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rfs_ema : 3; /* [2..0]  */
        unsigned int rfs_emaw : 2; /* [4..3]  */
        unsigned int ret1n : 1; /* [5]  */
        unsigned int rft_emaa : 3; /* [8..6]  */
        unsigned int rft_emab : 3; /* [11..9]  */
        unsigned int rfs_colldisn : 1; /* [12]  */
        unsigned int rft_emasa : 1; /* [13]  */
        unsigned int rfsuhd_wtsel : 2; /* [15..14]  */
        unsigned int rfsuhd_rtsel : 2; /* [17..16]  */
        unsigned int rfs_wtsel : 2; /* [19..18]  */
        unsigned int rfs_rtsel : 2; /* [21..20]  */
        unsigned int rfts_wct : 2; /* [23..22]  */
        unsigned int rfts_rct : 2; /* [25..24]  */
        unsigned int rfts_kp : 3; /* [28..26]  */
        unsigned int rftf_wct : 2; /* [30..29]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOLOWPOWER_CTRL;

/* Define the union U_VOUFSTA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int v0_uf_sta : 1; /* [0]  */
        unsigned int v1_uf_sta : 1; /* [1]  */
        unsigned int reserved_0 : 1; /* [2]  */
        unsigned int v3_uf_sta : 1; /* [3]  */
        unsigned int reserved_1 : 4; /* [7..4]  */
        unsigned int g0_uf_sta : 1; /* [8]  */
        unsigned int g1_uf_sta : 1; /* [9]  */
        unsigned int g2_uf_sta : 1; /* [10]  */
        unsigned int g3_uf_sta : 1; /* [11]  */
        unsigned int g4_uf_sta : 1; /* [12]  */
        unsigned int reserved_2 : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOUFSTA;

/* Define the union U_VOUFCLR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int v0_uf_clr : 1; /* [0]  */
        unsigned int v1_uf_clr : 1; /* [1]  */
        unsigned int reserved_0 : 1; /* [2]  */
        unsigned int v3_uf_clr : 1; /* [3]  */
        unsigned int reserved_1 : 4; /* [7..4]  */
        unsigned int g0_uf_clr : 1; /* [8]  */
        unsigned int g1_uf_clr : 1; /* [9]  */
        unsigned int g2_uf_clr : 1; /* [10]  */
        unsigned int g3_uf_clr : 1; /* [11]  */
        unsigned int g4_uf_clr : 1; /* [12]  */
        unsigned int reserved_2 : 19; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOUFCLR;

/* Define the union U_VOINTPROC_TIM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vointproc_time : 24; /* [23..0]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOINTPROC_TIM;

/* Define the union U_VOLOWPOWER_CTRL1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rftf_rct : 2; /* [1..0]  */
        unsigned int rftf_kp : 3; /* [4..2]  */
        unsigned int rft_wtsel : 2; /* [6..5]  */
        unsigned int rft_rtsel : 2; /* [8..7]  */
        unsigned int rft_mtsel : 2; /* [10..9]  */
        unsigned int rasshds_wtsel : 2; /* [12..11]  */
        unsigned int rasshds_rtsel : 2; /* [14..13]  */
        unsigned int rasshdm_wtsel : 2; /* [16..15]  */
        unsigned int rasshdm_rtsel : 2; /* [18..17]  */
        unsigned int rashds_wtsel : 2; /* [20..19]  */
        unsigned int rashds_rtsel : 2; /* [22..21]  */
        unsigned int rashdm_wtsel : 2; /* [24..23]  */
        unsigned int rashdm_rtsel : 2; /* [26..25]  */
        unsigned int ras_wtsel : 2; /* [28..27]  */
        unsigned int ras_rtsel : 2; /* [30..29]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOLOWPOWER_CTRL1;

/* Define the union U_VOFPGADEF */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_v_def : 1; /* [0]  */
        unsigned int hihdr_g_def : 1; /* [1]  */
        unsigned int hihdr_wd_def : 1; /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VOFPGADEF;

/* Define the union U_CBM_BKG1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cbm_bkgcr1 : 10; /* [9..0]  */
        unsigned int cbm_bkgcb1 : 10; /* [19..10]  */
        unsigned int cbm_bkgy1 : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_CBM_BKG1;

/* Define the union U_CBM_MIX1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_prio0 : 4; /* [3..0]  */
        unsigned int mixer_prio1 : 4; /* [7..4]  */
        unsigned int mixer_prio2 : 4; /* [11..8]  */
        unsigned int mixer_prio3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_CBM_MIX1;

/* Define the union U_WBC_BMP_THD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbc_bmp_thd : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_WBC_BMP_THD;

/* Define the union U_CBM_BKG2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cbm_bkgcr2 : 10; /* [9..0]  */
        unsigned int cbm_bkgcb2 : 10; /* [19..10]  */
        unsigned int cbm_bkgy2 : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_CBM_BKG2;

/* Define the union U_CBM_MIX2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_prio0 : 4; /* [3..0]  */
        unsigned int mixer_prio1 : 4; /* [7..4]  */
        unsigned int mixer_prio2 : 4; /* [11..8]  */
        unsigned int reserved_0 : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_CBM_MIX2;

/* Define the union U_HC_BMP_THD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hc_bmp_thd : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HC_BMP_THD;

/* Define the union U_CBM_BKG3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cbm_bkgcr3 : 10; /* [9..0]  */
        unsigned int cbm_bkgcb3 : 10; /* [19..10]  */
        unsigned int cbm_bkgy3 : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_CBM_BKG3;

/* Define the union U_CBM_MIX3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_prio0 : 4; /* [3..0]  */
        unsigned int mixer_prio1 : 4; /* [7..4]  */
        unsigned int mixer_prio2 : 4; /* [11..8]  */
        unsigned int reserved_0 : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_CBM_MIX3;

/* Define the union U_MIXV0_BKG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_bkgcr : 10; /* [9..0]  */
        unsigned int mixer_bkgcb : 10; /* [19..10]  */
        unsigned int mixer_bkgy : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_MIXV0_BKG;

/* Define the union U_MIXV0_MIX */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_prio0 : 4; /* [3..0]  */
        unsigned int mixer_prio1 : 4; /* [7..4]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_MIXV0_MIX;

/* Define the union U_MIXG0_BKG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_bkgcr : 10; /* [9..0]  */
        unsigned int mixer_bkgcb : 10; /* [19..10]  */
        unsigned int mixer_bkgy : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_MIXG0_BKG;

/* Define the union U_MIXG0_BKALPHA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_MIXG0_BKALPHA;

/* Define the union U_MIXG0_MIX */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_prio0 : 4; /* [3..0]  */
        unsigned int mixer_prio1 : 4; /* [7..4]  */
        unsigned int mixer_prio2 : 4; /* [11..8]  */
        unsigned int mixer_prio3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_MIXG0_MIX;

/* Define the union U_LINK_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cbm0_sel : 1; /* [0]  */
        unsigned int cbm1_sel : 1; /* [1]  */
        unsigned int hc_link : 1; /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_LINK_CTRL;

/* Define the union U_PARA_UP_VHD */
typedef union {
    /* Define the struct bits */
    struct {
        /* The 0 members below is corresponding to the member "para_up_vhd_chn00   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn01   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn02   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn03   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn04   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn05   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn06   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn07   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn08   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn09   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn10   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn11   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn12   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn13   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn14   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn15   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn16   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn17   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn18   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn19   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn20   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn21   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn22   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn23   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn24   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn25   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn26   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn27   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn28   " of the struct "" */
        /* The 0 members below is corresponding to the member "para_up_vhd_chn29   " of the struct "" */
        unsigned int para_up_vhd_chn30 : 1; /* [0]  */
        unsigned int para_up_vhd_chn31 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_PARA_UP_VHD;

/* Define the union U_G0_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 19; /* [26..8]  */
        unsigned int g0_depremult : 1; /* [27]  */
        unsigned int rupd_field : 1; /* [28]  */
        unsigned int rgup_mode : 1; /* [29]  */
        unsigned int nosec_flag : 1; /* [30]  */
        unsigned int surface_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_CTRL;

/* Define the union U_G0_UPD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_UPD;

/* Define the union U_G0_0RESO_READ */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_0RESO_READ;

/* Define the union U_G0_DFPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_DFPOS;

/* Define the union U_G0_DLPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_DLPOS;

/* Define the union U_G0_VFPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_VFPOS;

/* Define the union U_G0_VLPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_VLPOS;

/* Define the union U_G0_BK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_cr : 10; /* [9..0]  */
        unsigned int vbk_cb : 10; /* [19..10]  */
        unsigned int vbk_y : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_BK;

/* Define the union U_G0_ALPHA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_ALPHA;

/* Define the union U_G0_MUTE_BK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_cr : 10; /* [9..0]  */
        unsigned int mute_cb : 10; /* [19..10]  */
        unsigned int mute_y : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_MUTE_BK;

/* Define the union U_G0_LBOX_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_LBOX_CTRL;

/* Define the union U_G0_DOF_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0]  */
        unsigned int dof_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_DOF_CTRL;

/* Define the union U_G0_DOF_STEP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int left_step : 8; /* [7..0]  */
        unsigned int right_step : 8; /* [15..8]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_DOF_STEP;

/* Define the union U_G0_DOF_BKG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dof_bk_cr : 10; /* [9..0]  */
        unsigned int dof_bk_cb : 10; /* [19..10]  */
        unsigned int dof_bk_y : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_DOF_BKG;

/* Define the union U_G0_DOF_ALPHA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dof_bk_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_DOF_ALPHA;

/* Define the union U_G0_ZME_HINFO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out_width : 16; /* [15..0]  */
        unsigned int ck_gt_en : 1; /* [16]  */
        unsigned int reserved_0 : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_ZME_HINFO;

/* Define the union U_G0_ZME_HSP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hratio : 24; /* [23..0]  */
        unsigned int hfir_order : 1; /* [24]  */
        unsigned int ahfir_mode : 1; /* [25]  */
        unsigned int lhfir_mode : 1; /* [26]  */
        unsigned int reserved_0 : 1; /* [27]  */
        unsigned int chfir_mid_en : 1; /* [28]  */
        unsigned int lhfir_mid_en : 1; /* [29]  */
        unsigned int ahfir_mid_en : 1; /* [30]  */
        unsigned int hfir_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_ZME_HSP;

/* Define the union U_G0_ZME_HLOFFSET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lhfir_offset : 24; /* [23..0]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_ZME_HLOFFSET;

/* Define the union U_G0_ZME_HCOFFSET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chfir_offset : 24; /* [23..0]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_ZME_HCOFFSET;

/* Define the union U_G0_ZME_COEF_REN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1; /* [0]  */
        unsigned int apb_g0_vf_lren : 1; /* [1]  */
        unsigned int reserved_1 : 1; /* [2]  */
        unsigned int apb_g0_hf_lren : 1; /* [3]  */
        unsigned int reserved_2 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_ZME_COEF_REN;

/* Define the union U_G0_ZME_COEF_RDATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_coef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_ZME_COEF_RDATA;

/* Define the union U_G0_ZME_VINFO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out_height : 16; /* [15..0]  */
        unsigned int reserved_0 : 2; /* [17..16]  */
        unsigned int out_pro : 1; /* [18]  */
        unsigned int reserved_1 : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_ZME_VINFO;

/* Define the union U_G0_ZME_VSP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vratio : 16; /* [15..0]  */
        unsigned int reserved_0 : 9; /* [24..16]  */
        unsigned int vafir_mode : 1; /* [25]  */
        unsigned int lvfir_mode : 1; /* [26]  */
        unsigned int reserved_1 : 1; /* [27]  */
        unsigned int cvfir_mid_en : 1; /* [28]  */
        unsigned int lvfir_mid_en : 1; /* [29]  */
        unsigned int avfir_mid_en : 1; /* [30]  */
        unsigned int vfir_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_ZME_VSP;

/* Define the union U_G0_ZME_VOFFSET */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbtm_offset : 16; /* [15..0]  */
        unsigned int vtp_offset : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G0_ZME_VOFFSET;

/* Define the union U_G1_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 19; /* [26..8]  */
        unsigned int g1_depremult : 1; /* [27]  */
        unsigned int rupd_field : 1; /* [28]  */
        unsigned int rgup_mode : 1; /* [29]  */
        unsigned int nosec_flag : 1; /* [30]  */
        unsigned int surface_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CTRL;

/* Define the union U_G1_UPD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_UPD;

/* Define the union U_G1_0RESO_READ */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_0RESO_READ;

/* Define the union U_G1_DFPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_DFPOS;

/* Define the union U_G1_DLPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_DLPOS;

/* Define the union U_G1_VFPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_VFPOS;

/* Define the union U_G1_VLPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_VLPOS;

/* Define the union U_G1_BK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_cr : 10; /* [9..0]  */
        unsigned int vbk_cb : 10; /* [19..10]  */
        unsigned int vbk_y : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_BK;

/* Define the union U_G1_ALPHA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_ALPHA;

/* Define the union U_G1_MUTE_BK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_cr : 10; /* [9..0]  */
        unsigned int mute_cb : 10; /* [19..10]  */
        unsigned int mute_y : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_MUTE_BK;

/* Define the union U_G1_LBOX_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_LBOX_CTRL;

/* Define the union U_G1_CSC_IDC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc0 : 11; /* [10..0]  */
        unsigned int cscidc1 : 11; /* [21..11]  */
        unsigned int csc_en : 1; /* [22]  */
        unsigned int csc_mode : 3; /* [25..23]  */
        unsigned int csc_ck_gt_en : 1; /* [26]  */
        unsigned int reserved_0 : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC_IDC;

/* Define the union U_G1_CSC_ODC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscodc0 : 11; /* [10..0]  */
        unsigned int cscodc1 : 11; /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC_ODC;

/* Define the union U_G1_CSC_IODC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11; /* [10..0]  */
        unsigned int cscodc2 : 11; /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC_IODC;

/* Define the union U_G1_CSC_P0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp00 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC_P0;

/* Define the union U_G1_CSC_P1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp02 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC_P1;

/* Define the union U_G1_CSC_P2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp11 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp12 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC_P2;

/* Define the union U_G1_CSC_P3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp20 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp21 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC_P3;

/* Define the union U_G1_CSC_P4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15; /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC_P4;

/* Define the union U_G1_CSC1_IDC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc0 : 11; /* [10..0]  */
        unsigned int cscidc1 : 11; /* [21..11]  */
        unsigned int csc_en : 1; /* [22]  */
        unsigned int csc_mode : 3; /* [25..23]  */
        unsigned int reserved_0 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC1_IDC;

/* Define the union U_G1_CSC1_ODC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscodc0 : 11; /* [10..0]  */
        unsigned int cscodc1 : 11; /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC1_ODC;

/* Define the union U_G1_CSC1_IODC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11; /* [10..0]  */
        unsigned int cscodc2 : 11; /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC1_IODC;

/* Define the union U_G1_CSC1_P0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp00 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC1_P0;

/* Define the union U_G1_CSC1_P1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp02 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC1_P1;

/* Define the union U_G1_CSC1_P2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp11 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp12 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC1_P2;

/* Define the union U_G1_CSC1_P3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp20 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp21 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC1_P3;

/* Define the union U_G1_CSC1_P4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15; /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G1_CSC1_P4;

/* Define the union U_G3_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 19; /* [26..8]  */
        unsigned int g1_depremult : 1; /* [27]  */
        unsigned int rupd_field : 1; /* [28]  */
        unsigned int rgup_mode : 1; /* [29]  */
        unsigned int nosec_flag : 1; /* [30]  */
        unsigned int surface_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CTRL;

/* Define the union U_G3_UPD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_UPD;

/* Define the union U_G3_0RESO_READ */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_0RESO_READ;

/* Define the union U_G3_DFPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_DFPOS;

/* Define the union U_G3_DLPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_DLPOS;

/* Define the union U_G3_VFPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_VFPOS;

/* Define the union U_G3_VLPOS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_VLPOS;

/* Define the union U_G3_BK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_cr : 10; /* [9..0]  */
        unsigned int vbk_cb : 10; /* [19..10]  */
        unsigned int vbk_y : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_BK;

/* Define the union U_G3_ALPHA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_ALPHA;

/* Define the union U_G3_MUTE_BK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_cr : 10; /* [9..0]  */
        unsigned int mute_cb : 10; /* [19..10]  */
        unsigned int mute_y : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_MUTE_BK;

/* Define the union U_G3_LBOX_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_LBOX_CTRL;

/* Define the union U_G3_CSC_IDC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc0 : 11; /* [10..0]  */
        unsigned int cscidc1 : 11; /* [21..11]  */
        unsigned int csc_en : 1; /* [22]  */
        unsigned int csc_mode : 3; /* [25..23]  */
        unsigned int csc_ck_gt_en : 1; /* [26]  */
        unsigned int reserved_0 : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC_IDC;

/* Define the union U_G3_CSC_ODC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscodc0 : 11; /* [10..0]  */
        unsigned int cscodc1 : 11; /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC_ODC;

/* Define the union U_G3_CSC_IODC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11; /* [10..0]  */
        unsigned int cscodc2 : 11; /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC_IODC;

/* Define the union U_G3_CSC_P0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp00 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC_P0;

/* Define the union U_G3_CSC_P1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp02 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC_P1;

/* Define the union U_G3_CSC_P2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp11 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp12 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC_P2;

/* Define the union U_G3_CSC_P3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp20 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp21 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC_P3;

/* Define the union U_G3_CSC_P4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15; /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC_P4;

/* Define the union U_G3_CSC1_IDC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc0 : 11; /* [10..0]  */
        unsigned int cscidc1 : 11; /* [21..11]  */
        unsigned int csc_en : 1; /* [22]  */
        unsigned int csc_mode : 3; /* [25..23]  */
        unsigned int reserved_0 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC1_IDC;

/* Define the union U_G3_CSC1_ODC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscodc0 : 11; /* [10..0]  */
        unsigned int cscodc1 : 11; /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC1_ODC;

/* Define the union U_G3_CSC1_IODC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11; /* [10..0]  */
        unsigned int cscodc2 : 11; /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC1_IODC;

/* Define the union U_G3_CSC1_P0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp00 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC1_P0;

/* Define the union U_G3_CSC1_P1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp02 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC1_P1;

/* Define the union U_G3_CSC1_P2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp11 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp12 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC1_P2;

/* Define the union U_G3_CSC1_P3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp20 : 15; /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp21 : 15; /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC1_P3;

/* Define the union U_G3_CSC1_P4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15; /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_G3_CSC1_P4;

/* Define the union U_DHD0_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1; /* [0]  */
        unsigned int disp_mode : 3; /* [3..1]  */
        unsigned int iop : 1; /* [4]  */
        unsigned int intf_ivs : 1; /* [5]  */
        unsigned int intf_ihs : 1; /* [6]  */
        unsigned int intf_idv : 1; /* [7]  */
        unsigned int reserved_0 : 1; /* [8]  */
        unsigned int hdmi420c_sel : 1; /* [9]  */
        unsigned int hdmi420_en : 1; /* [10]  */
        unsigned int uf_offline_en : 1; /* [11]  */
        unsigned int reserved_1 : 2; /* [13..12]  */
        unsigned int hdmi_mode : 1; /* [14]  */
        unsigned int twochn_debug : 1; /* [15]  */
        unsigned int twochn_en : 1; /* [16]  */
        unsigned int reserved_2 : 1; /* [17]  */
        unsigned int cbar_mode : 1; /* [18]  */
        unsigned int sin_en : 1; /* [19]  */
        unsigned int fpga_lmt_width : 7; /* [26..20]  */
        unsigned int fpga_lmt_en : 1; /* [27]  */
        unsigned int p2i_en : 1; /* [28]  */
        unsigned int cbar_sel : 1; /* [29]  */
        unsigned int cbar_en : 1; /* [30]  */
        unsigned int intf_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_CTRL;

/* Define the union U_DHD0_VSYNC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vact : 16; /* [15..0]  */
        unsigned int vbb : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_VSYNC1;

/* Define the union U_DHD0_VSYNC2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vfb : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_VSYNC2;

/* Define the union U_DHD0_HSYNC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hact : 16; /* [15..0]  */
        unsigned int hbb : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_HSYNC1;

/* Define the union U_DHD0_HSYNC2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hfb : 16; /* [15..0]  */
        unsigned int hmid : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_HSYNC2;

/* Define the union U_DHD0_VPLUS1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bvact : 16; /* [15..0]  */
        unsigned int bvbb : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_VPLUS1;

/* Define the union U_DHD0_VPLUS2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bvfb : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_VPLUS2;

/* Define the union U_DHD0_PWR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hpw : 16; /* [15..0]  */
        unsigned int vpw : 8; /* [23..16]  */
        unsigned int reserved_0 : 3; /* [26..24]  */
        unsigned int multichn_en : 2; /* [28..27]  */
        unsigned int reserved_1 : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_PWR;

/* Define the union U_DHD0_VTTHD3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vtmgthd3 : 13; /* [12..0]  */
        unsigned int reserved_0 : 2; /* [14..13]  */
        unsigned int thd3_mode : 1; /* [15]  */
        unsigned int vtmgthd4 : 13; /* [28..16]  */
        unsigned int reserved_1 : 2; /* [30..29]  */
        unsigned int thd4_mode : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_VTTHD3;

/* Define the union U_DHD0_VTTHD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vtmgthd1 : 13; /* [12..0]  */
        unsigned int reserved_0 : 2; /* [14..13]  */
        unsigned int thd1_mode : 1; /* [15]  */
        unsigned int vtmgthd2 : 13; /* [28..16]  */
        unsigned int reserved_1 : 2; /* [30..29]  */
        unsigned int thd2_mode : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_VTTHD;

/* Define the union U_DHD0_PARATHD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int para_thd : 8; /* [7..0]  */
        unsigned int reserved_0 : 23; /* [30..8]  */
        unsigned int dfs_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_PARATHD;

/* Define the union U_DHD0_PRECHARGE_THD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tcon_precharge_thd : 17; /* [16..0]  */
        unsigned int reserved_0 : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_PRECHARGE_THD;

/* Define the union U_DHD0_START_POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int start_pos : 8; /* [7..0]  */
        unsigned int timing_start_pos : 8; /* [15..8]  */
        unsigned int fi_start_pos : 4; /* [19..16]  */
        unsigned int req_start_pos : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_START_POS;

/* Define the union U_DHD0_START_POS1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_start_pos1 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_START_POS1;

/* Define the union U_DHD0_PARAUP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0]  */
        unsigned int paraup_mode : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_PARAUP;

/* Define the union U_DHD0_SYNC_INV */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lcd_dv_inv : 1; /* [0]  */
        unsigned int lcd_hs_inv : 1; /* [1]  */
        unsigned int lcd_vs_inv : 1; /* [2]  */
        unsigned int reserved_0 : 1; /* [3]  */
        unsigned int vga_dv_inv : 1; /* [4]  */
        unsigned int vga_hs_inv : 1; /* [5]  */
        unsigned int vga_vs_inv : 1; /* [6]  */
        unsigned int reserved_1 : 1; /* [7]  */
        unsigned int hdmi_dv_inv : 1; /* [8]  */
        unsigned int hdmi_hs_inv : 1; /* [9]  */
        unsigned int hdmi_vs_inv : 1; /* [10]  */
        unsigned int hdmi_f_inv : 1; /* [11]  */
        unsigned int date_dv_inv : 1; /* [12]  */
        unsigned int date_hs_inv : 1; /* [13]  */
        unsigned int date_vs_inv : 1; /* [14]  */
        unsigned int date_f_inv : 1; /* [15]  */
        unsigned int reserved_2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_SYNC_INV;

/* Define the union U_DHD0_CLK_DV_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int intf_clk_mux : 1; /* [0]  */
        unsigned int intf_dv_mux : 1; /* [1]  */
        unsigned int no_active_area_pos : 16; /* [17..2]  */
        unsigned int reserved_0 : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_CLK_DV_CTRL;

/* Define the union U_DHD0_RGB_FIX_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fix_b : 10; /* [9..0]  */
        unsigned int fix_g : 10; /* [19..10]  */
        unsigned int fix_r : 10; /* [29..20]  */
        unsigned int rgb_fix_mux : 1; /* [30]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_RGB_FIX_CTRL;

/* Define the union U_DHD0_LOCKCFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int measure_en : 1; /* [0]  */
        unsigned int lock_cnt_en : 1; /* [1]  */
        unsigned int vdp_measure_en : 1; /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_LOCKCFG;

/* Define the union U_DHD0_INTF_CHKSUM_HIGH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int y0_sum_high : 8; /* [7..0]  */
        unsigned int g0_sum_high : 8; /* [15..8]  */
        unsigned int b0_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_INTF_CHKSUM_HIGH1;

/* Define the union U_DHD0_INTF_CHKSUM_HIGH2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int y1_sum_high : 8; /* [7..0]  */
        unsigned int g1_sum_high : 8; /* [15..8]  */
        unsigned int b1_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_INTF_CHKSUM_HIGH2;

/* Define the union U_DHD0_STATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vback_blank : 1; /* [0]  */
        unsigned int vblank : 1; /* [1]  */
        unsigned int bottom_field : 1; /* [2]  */
        unsigned int vcnt : 13; /* [15..3]  */
        unsigned int count_int : 8; /* [23..16]  */
        unsigned int dhd_even : 1; /* [24]  */
        unsigned int reserved_0 : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_STATE;

/* Define the union U_DHD0_UF_STATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ud_first_cnt : 13; /* [12..0]  */
        unsigned int reserved_0 : 3; /* [15..13]  */
        unsigned int start_pos : 8; /* [23..16]  */
        unsigned int reserved_1 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD0_UF_STATE;

/* Define the union U_VO_MUX */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mipi_sel : 4; /* [3..0]  */
        unsigned int lcd_sel : 4; /* [7..4]  */
        unsigned int bt_sel : 4; /* [11..8]  */
        unsigned int reserved_0 : 16; /* [27..12]  */
        unsigned int digital_sel : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VO_MUX;

/* Define the union U_VO_MUX_SYNC */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sync_dv : 1; /* [0]  */
        unsigned int sync_hsync : 1; /* [1]  */
        unsigned int sync_vsync : 1; /* [2]  */
        unsigned int sync_field : 1; /* [3]  */
        unsigned int reserved_0 : 27; /* [30..4]  */
        unsigned int sync_test_mode : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VO_MUX_SYNC;

/* Define the union U_VO_MUX_DATA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vomux_data : 30; /* [29..0]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_VO_MUX_DATA;

/* Define the union U_DHD1_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1; /* [0]  */
        unsigned int disp_mode : 3; /* [3..1]  */
        unsigned int iop : 1; /* [4]  */
        unsigned int intf_ivs : 1; /* [5]  */
        unsigned int intf_ihs : 1; /* [6]  */
        unsigned int intf_idv : 1; /* [7]  */
        unsigned int reserved_0 : 1; /* [8]  */
        unsigned int hdmi420c_sel : 1; /* [9]  */
        unsigned int hdmi420_en : 1; /* [10]  */
        unsigned int uf_offline_en : 1; /* [11]  */
        unsigned int reserved_1 : 2; /* [13..12]  */
        unsigned int hdmi_mode : 1; /* [14]  */
        unsigned int twochn_debug : 1; /* [15]  */
        unsigned int twochn_en : 1; /* [16]  */
        unsigned int reserved_2 : 1; /* [17]  */
        unsigned int cbar_mode : 1; /* [18]  */
        unsigned int sin_en : 1; /* [19]  */
        unsigned int fpga_lmt_width : 7; /* [26..20]  */
        unsigned int fpga_lmt_en : 1; /* [27]  */
        unsigned int p2i_en : 1; /* [28]  */
        unsigned int cbar_sel : 1; /* [29]  */
        unsigned int cbar_en : 1; /* [30]  */
        unsigned int intf_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_CTRL;

/* Define the union U_DHD1_VSYNC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vact : 16; /* [15..0]  */
        unsigned int vbb : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_VSYNC1;

/* Define the union U_DHD1_VSYNC2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vfb : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_VSYNC2;

/* Define the union U_DHD1_HSYNC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hact : 16; /* [15..0]  */
        unsigned int hbb : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_HSYNC1;

/* Define the union U_DHD1_HSYNC2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hfb : 16; /* [15..0]  */
        unsigned int hmid : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_HSYNC2;

/* Define the union U_DHD1_VPLUS1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bvact : 16; /* [15..0]  */
        unsigned int bvbb : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_VPLUS1;

/* Define the union U_DHD1_VPLUS2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bvfb : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_VPLUS2;

/* Define the union U_DHD1_PWR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hpw : 16; /* [15..0]  */
        unsigned int vpw : 8; /* [23..16]  */
        unsigned int reserved_0 : 3; /* [26..24]  */
        unsigned int multichn_en : 2; /* [28..27]  */
        unsigned int reserved_1 : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_PWR;

/* Define the union U_DHD1_VTTHD3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vtmgthd3 : 13; /* [12..0]  */
        unsigned int reserved_0 : 2; /* [14..13]  */
        unsigned int thd3_mode : 1; /* [15]  */
        unsigned int vtmgthd4 : 13; /* [28..16]  */
        unsigned int reserved_1 : 2; /* [30..29]  */
        unsigned int thd4_mode : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_VTTHD3;

/* Define the union U_DHD1_VTTHD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vtmgthd1 : 13; /* [12..0]  */
        unsigned int reserved_0 : 2; /* [14..13]  */
        unsigned int thd1_mode : 1; /* [15]  */
        unsigned int vtmgthd2 : 13; /* [28..16]  */
        unsigned int reserved_1 : 2; /* [30..29]  */
        unsigned int thd2_mode : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_VTTHD;

/* Define the union U_DHD1_PARATHD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int para_thd : 8; /* [7..0]  */
        unsigned int reserved_0 : 23; /* [30..8]  */
        unsigned int dfs_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_PARATHD;

/* Define the union U_DHD1_PRECHARGE_THD */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tcon_precharge_thd : 17; /* [16..0]  */
        unsigned int reserved_0 : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_PRECHARGE_THD;

/* Define the union U_DHD1_START_POS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int start_pos : 8; /* [7..0]  */
        unsigned int timing_start_pos : 8; /* [15..8]  */
        unsigned int fi_start_pos : 4; /* [19..16]  */
        unsigned int req_start_pos : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_START_POS;

/* Define the union U_DHD1_START_POS1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_start_pos1 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_START_POS1;

/* Define the union U_DHD1_PARAUP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0]  */
        unsigned int paraup_mode : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_PARAUP;

/* Define the union U_DHD1_SYNC_INV */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lcd_dv_inv : 1; /* [0]  */
        unsigned int lcd_hs_inv : 1; /* [1]  */
        unsigned int lcd_vs_inv : 1; /* [2]  */
        unsigned int reserved_0 : 1; /* [3]  */
        unsigned int vga_dv_inv : 1; /* [4]  */
        unsigned int vga_hs_inv : 1; /* [5]  */
        unsigned int vga_vs_inv : 1; /* [6]  */
        unsigned int reserved_1 : 1; /* [7]  */
        unsigned int hdmi_dv_inv : 1; /* [8]  */
        unsigned int hdmi_hs_inv : 1; /* [9]  */
        unsigned int hdmi_vs_inv : 1; /* [10]  */
        unsigned int hdmi_f_inv : 1; /* [11]  */
        unsigned int date_dv_inv : 1; /* [12]  */
        unsigned int date_hs_inv : 1; /* [13]  */
        unsigned int date_vs_inv : 1; /* [14]  */
        unsigned int date_f_inv : 1; /* [15]  */
        unsigned int reserved_2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_SYNC_INV;

/* Define the union U_DHD1_CLK_DV_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int intf_clk_mux : 1; /* [0]  */
        unsigned int intf_dv_mux : 1; /* [1]  */
        unsigned int no_active_area_pos : 16; /* [17..2]  */
        unsigned int reserved_0 : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_CLK_DV_CTRL;

/* Define the union U_DHD1_RGB_FIX_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fix_b : 10; /* [9..0]  */
        unsigned int fix_g : 10; /* [19..10]  */
        unsigned int fix_r : 10; /* [29..20]  */
        unsigned int rgb_fix_mux : 1; /* [30]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_RGB_FIX_CTRL;

/* Define the union U_DHD1_LOCKCFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int measure_en : 1; /* [0]  */
        unsigned int lock_cnt_en : 1; /* [1]  */
        unsigned int vdp_measure_en : 1; /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_LOCKCFG;

/* Define the union U_DHD1_INTF_CHKSUM_HIGH1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int y0_sum_high : 8; /* [7..0]  */
        unsigned int g0_sum_high : 8; /* [15..8]  */
        unsigned int b0_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_INTF_CHKSUM_HIGH1;

/* Define the union U_DHD1_INTF_CHKSUM_HIGH2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int y1_sum_high : 8; /* [7..0]  */
        unsigned int g1_sum_high : 8; /* [15..8]  */
        unsigned int b1_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_INTF_CHKSUM_HIGH2;

/* Define the union U_DHD1_STATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vback_blank : 1; /* [0]  */
        unsigned int vblank : 1; /* [1]  */
        unsigned int bottom_field : 1; /* [2]  */
        unsigned int vcnt : 13; /* [15..3]  */
        unsigned int count_int : 8; /* [23..16]  */
        unsigned int dhd_even : 1; /* [24]  */
        unsigned int reserved_0 : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_STATE;

/* Define the union U_DHD1_UF_STATE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ud_first_cnt : 13; /* [12..0]  */
        unsigned int reserved_0 : 3; /* [15..13]  */
        unsigned int start_pos : 8; /* [23..16]  */
        unsigned int reserved_1 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_DHD1_UF_STATE;

/* Define the union U_GFX_READ_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int read_mode : 2; /* [1..0]  */
        unsigned int reserved_0 : 2; /* [3..2]  */
        unsigned int draw_mode : 2; /* [5..4]  */
        unsigned int reserved_1 : 2; /* [7..6]  */
        unsigned int flip_en : 1; /* [8]  */
        unsigned int reserved_2 : 1; /* [9]  */
        unsigned int mute_en : 1; /* [10]  */
        unsigned int mute_req_en : 1; /* [11]  */
        unsigned int reserved_3 : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_READ_CTRL;

/* Define the union U_GFX_MAC_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_ctrl : 2; /* [1..0]  */
        unsigned int req_len : 2; /* [3..2]  */
        unsigned int reserved_0 : 4; /* [7..4]  */
        unsigned int ofl_master : 1; /* [8]  */
        unsigned int reserved_1 : 23; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_MAC_CTRL;

/* Define the union U_GFX_OUT_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int palpha_range : 1; /* [0]  */
        unsigned int palpha_en : 1; /* [1]  */
        unsigned int reserved_0 : 2; /* [3..2]  */
        unsigned int key_mode : 1; /* [4]  */
        unsigned int key_en : 1; /* [5]  */
        unsigned int reserved_1 : 2; /* [7..6]  */
        unsigned int bitext : 2; /* [9..8]  */
        unsigned int premulti_en : 1; /* [10]  */
        unsigned int testpattern_en : 1; /* [11]  */
        unsigned int reserved_2 : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_OUT_CTRL;

/* Define the union U_GFX_MUTE_ALPHA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_MUTE_ALPHA;

/* Define the union U_GFX_MUTE_BK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_cr : 10; /* [9..0]  */
        unsigned int mute_cb : 10; /* [19..10]  */
        unsigned int mute_y : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_MUTE_BK;

/* Define the union U_GFX_SMMU_BYPASS */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smmu_bypass_2d : 1; /* [0]  */
        unsigned int smmu_bypass_3d : 1; /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_SMMU_BYPASS;

/* Define the union U_GFX_1555_ALPHA */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int alpha_0 : 8; /* [7..0]  */
        unsigned int alpha_1 : 8; /* [15..8]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_1555_ALPHA;

/* Define the union U_GFX_SRC_INFO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ifmt : 8; /* [7..0]  */
        unsigned int reserved_0 : 8; /* [15..8]  */
        unsigned int disp_mode : 4; /* [19..16]  */
        unsigned int dcmp_en : 1; /* [20]  */
        unsigned int reserved_1 : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_SRC_INFO;

/* Define the union U_GFX_SRC_RESO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int src_w : 16; /* [15..0]  */
        unsigned int src_h : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_SRC_RESO;

/* Define the union U_GFX_SRC_CROP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int src_crop_x : 16; /* [15..0]  */
        unsigned int src_crop_y : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_SRC_CROP;

/* Define the union U_GFX_IRESO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ireso_w : 16; /* [15..0]  */
        unsigned int ireso_h : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_IRESO;

/* Define the union U_GFX_STRIDE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int surface_stride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_STRIDE;

/* Define the union U_GFX_CKEY_MAX */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_b_max : 8; /* [7..0]  */
        unsigned int key_g_max : 8; /* [15..8]  */
        unsigned int key_r_max : 8; /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_CKEY_MAX;

/* Define the union U_GFX_CKEY_MIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_b_min : 8; /* [7..0]  */
        unsigned int key_g_min : 8; /* [15..8]  */
        unsigned int key_r_min : 8; /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_CKEY_MIN;

/* Define the union U_GFX_CKEY_MASK */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_b_msk : 8; /* [7..0]  */
        unsigned int key_g_msk : 8; /* [15..8]  */
        unsigned int key_r_msk : 8; /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_CKEY_MASK;

/* Define the union U_GFX_TESTPAT_CFG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tp_speed : 10; /* [9..0]  */
        unsigned int reserved_0 : 2; /* [11..10]  */
        unsigned int tp_line_w : 1; /* [12]  */
        unsigned int tp_color_mode : 1; /* [13]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int tp_mode : 2; /* [17..16]  */
        unsigned int reserved_2 : 14; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_TESTPAT_CFG;

/* Define the union U_GFX_TESTPAT_SEED */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tp_seed : 30; /* [29..0]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_TESTPAT_SEED;

/* Define the union U_GFX_LATENCY_CLR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int latency_clr : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_LATENCY_CLR;

/* Define the union U_GFX_LINE_LATENCY */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int axi_line_num : 16; /* [15..0]  */
        unsigned int axi_line_max : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_LINE_LATENCY;

/* Define the union U_GFX_DEBUG_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chk_bit8_sel : 4; /* [3..0]  */
        unsigned int check_sum_en : 1; /* [4]  */
        unsigned int reserved_0 : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_DEBUG_CTRL;

/* Define the union U_GFX_STA_CLR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_end_clr : 1; /* [0]  */
        unsigned int out_end_clr : 1; /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_STA_CLR;

/* Define the union U_GFX_DCMPERRCLR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dcmp_errclr : 1; /* [0]  */
        unsigned int dcmp_frm_end_clr : 1; /* [1]  */
        unsigned int frame_size_errclr : 1; /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_DCMPERRCLR;

/* Define the union U_GFX_DCMP_ERR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dcmp_wrong : 1; /* [0]  */
        unsigned int dcmp_frame_end : 1; /* [1]  */
        unsigned int frame_size_wrong : 1; /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_DCMP_ERR;

/* Define the union U_GFX_OSD_GLB_INFO */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dcmp_en : 1; /* [0]  */
        unsigned int is_lossless : 1; /* [1]  */
        unsigned int is_lossless_a : 1; /* [2]  */
        unsigned int cmp_mode : 1; /* [3]  */
        unsigned int source_mode : 3; /* [6..4]  */
        unsigned int tpred_en : 1; /* [7]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_OSD_GLB_INFO;

/* Define the union U_GFX_OSD_FRAME_SIZE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_width : 13; /* [12..0]  */
        unsigned int reserved_0 : 3; /* [15..13]  */
        unsigned int frame_height : 13; /* [28..16]  */
        unsigned int reserved_1 : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_OSD_FRAME_SIZE;

/* Define the union U_GFX_OSD_DBG_REG */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 30; /* [29..0]  */
        unsigned int dcmp_err0 : 1; /* [30]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_OSD_DBG_REG;

/* Define the union U_GFX_OSD_DBG_REG1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 30; /* [29..0]  */
        unsigned int dcmp_err1 : 1; /* [30]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GFX_OSD_DBG_REG1;

// ==============================================================================
/* Define the global struct */
typedef struct {
    volatile U_VOCTRL VOCTRL; /* 0x0 */
    volatile U_VOINTSTA VOINTSTA; /* 0x4 */
    volatile U_VOMSKINTSTA VOMSKINTSTA; /* 0x8 */
    volatile U_VOINTMSK VOINTMSK; /* 0xc */
    volatile U_VODEBUG VODEBUG; /* 0x10 */
    volatile U_VOINTSTA1 VOINTSTA1; /* 0x14 */
    volatile U_VOMSKINTSTA1 VOMSKINTSTA1; /* 0x18 */
    volatile U_VOINTMSK1 VOINTMSK1; /* 0x1c */
    volatile unsigned int VDPVERSION1; /* 0x20 */
    volatile unsigned int VDPVERSION2; /* 0x24 */
    volatile U_VOLOWPOWER_CTRL VOLOWPOWER_CTRL; /* 0x28 */
    volatile U_VOUFSTA VOUFSTA; /* 0x2c */
    volatile U_VOUFCLR VOUFCLR; /* 0x30 */
    volatile U_VOINTPROC_TIM VOINTPROC_TIM; /* 0x34 */
    volatile unsigned int VOFPGATEST; /* 0x38 */
    volatile unsigned int reserved_0[3]; /* 0x3c~0x44 */
    volatile U_VOLOWPOWER_CTRL1 VOLOWPOWER_CTRL1; /* 0x48 */
    volatile U_VOFPGADEF VOFPGADEF; /* 0x4c */
    volatile unsigned int reserved_1[172]; /* 0x50~0x2fc */
    volatile U_CBM_BKG1 CBM_BKG1; /* 0x300 */
    volatile unsigned int reserved_2; /* 0x304 */
    volatile U_CBM_MIX1 CBM_MIX1; /* 0x308 */
    volatile unsigned int reserved_3[14]; /* 0x30c~0x340 */
    volatile U_WBC_BMP_THD WBC_BMP_THD; /* 0x344 */
    volatile unsigned int reserved_4[14]; /* 0x348~0x37c */
    volatile U_CBM_BKG2 CBM_BKG2; /* 0x380 */
    volatile unsigned int reserved_5; /* 0x384 */
    volatile U_CBM_MIX2 CBM_MIX2; /* 0x388 */
    volatile unsigned int reserved_6[14]; /* 0x38c~0x3c0 */
    volatile U_HC_BMP_THD HC_BMP_THD; /* 0x3c4 */
    volatile unsigned int reserved_7[14]; /* 0x3c8~0x3fc */
    volatile U_CBM_BKG3 CBM_BKG3; /* 0x400 */
    volatile unsigned int reserved_8; /* 0x404 */
    volatile U_CBM_MIX3 CBM_MIX3; /* 0x408 */
    volatile unsigned int reserved_9[125]; /* 0x40c~0x5fc */
    volatile U_MIXV0_BKG MIXV0_BKG; /* 0x600 */
    volatile unsigned int reserved_10; /* 0x604 */
    volatile U_MIXV0_MIX MIXV0_MIX; /* 0x608 */
    volatile unsigned int reserved_11[189]; /* 0x60c~0x8fc */
    volatile U_MIXG0_BKG MIXG0_BKG; /* 0x900 */
    volatile U_MIXG0_BKALPHA MIXG0_BKALPHA; /* 0x904 */
    volatile U_MIXG0_MIX MIXG0_MIX; /* 0x908 */
    volatile unsigned int reserved_12[189]; /* 0x90c~0xbfc */
    volatile U_LINK_CTRL LINK_CTRL; /* 0xc00 */
    volatile unsigned int reserved_13[135]; /* 0xc04~0xe1c */
    volatile unsigned int PARA_HADDR_VHD_CHN04; /* 0xe20 */
    volatile unsigned int PARA_ADDR_VHD_CHN04; /* 0xe24 */
    volatile unsigned int PARA_HADDR_VHD_CHN05; /* 0xe28 */
    volatile unsigned int PARA_ADDR_VHD_CHN05; /* 0xe2c */
    volatile unsigned int PARA_HADDR_VHD_CHN06; /* 0xe30 */
    volatile unsigned int PARA_ADDR_VHD_CHN06; /* 0xe34 */
    volatile unsigned int reserved_14[50]; /* 0xe38~0xefc */
    volatile U_PARA_UP_VHD PARA_UP_VHD; /* 0xf00 */
    volatile unsigned int reserved_15[6207]; /* 0xf04~0x6ffc */
    volatile U_G0_CTRL G0_CTRL; /* 0x7000 */
    volatile U_G0_UPD G0_UPD; /* 0x7004 */
    volatile unsigned int G0_GALPHA_SUM; /* 0x7008 */
    volatile U_G0_0RESO_READ G0_0RESO_READ; /* 0x700c */
    volatile unsigned int reserved_16[28]; /* 0x7010~0x707c */
    volatile U_G0_DFPOS G0_DFPOS; /* 0x7080 */
    volatile U_G0_DLPOS G0_DLPOS; /* 0x7084 */
    volatile U_G0_VFPOS G0_VFPOS; /* 0x7088 */
    volatile U_G0_VLPOS G0_VLPOS; /* 0x708c */
    volatile U_G0_BK G0_BK; /* 0x7090 */
    volatile U_G0_ALPHA G0_ALPHA; /* 0x7094 */
    volatile U_G0_MUTE_BK G0_MUTE_BK; /* 0x7098 */
    volatile U_G0_LBOX_CTRL G0_LBOX_CTRL; /* 0x709c */
    volatile unsigned int reserved_17[88]; /* 0x70a0~0x71fc */
    volatile U_G0_DOF_CTRL G0_DOF_CTRL; /* 0x7200 */
    volatile U_G0_DOF_STEP G0_DOF_STEP; /* 0x7204 */
    volatile U_G0_DOF_BKG G0_DOF_BKG; /* 0x7208 */
    volatile U_G0_DOF_ALPHA G0_DOF_ALPHA; /* 0x720c */
    volatile unsigned int reserved_18[60]; /* 0x7210~0x72fc */
    volatile U_G0_ZME_HINFO G0_ZME_HINFO; /* 0x7300 */
    volatile U_G0_ZME_HSP G0_ZME_HSP; /* 0x7304 */
    volatile U_G0_ZME_HLOFFSET G0_ZME_HLOFFSET; /* 0x7308 */
    volatile U_G0_ZME_HCOFFSET G0_ZME_HCOFFSET; /* 0x730c */
    volatile unsigned int reserved_19[5]; /* 0x7310~0x7320 */
    volatile U_G0_ZME_COEF_REN G0_ZME_COEF_REN; /* 0x7324 */
    volatile U_G0_ZME_COEF_RDATA G0_ZME_COEF_RDATA; /* 0x7328 */
    volatile unsigned int reserved_20[21]; /* 0x732c~0x737c */
    volatile U_G0_ZME_VINFO G0_ZME_VINFO; /* 0x7380 */
    volatile U_G0_ZME_VSP G0_ZME_VSP; /* 0x7384 */
    volatile U_G0_ZME_VOFFSET G0_ZME_VOFFSET; /* 0x7388 */
    volatile unsigned int reserved_21[285]; /* 0x738c~0x77fc */
    volatile U_G1_CTRL G1_CTRL; /* 0x7800 */
    volatile U_G1_UPD G1_UPD; /* 0x7804 */
    volatile unsigned int G1_GALPHA_SUM; /* 0x7808 */
    volatile U_G1_0RESO_READ G1_0RESO_READ; /* 0x780c */
    volatile unsigned int reserved_22[28]; /* 0x7810~0x787c */
    volatile U_G1_DFPOS G1_DFPOS; /* 0x7880 */
    volatile U_G1_DLPOS G1_DLPOS; /* 0x7884 */
    volatile U_G1_VFPOS G1_VFPOS; /* 0x7888 */
    volatile U_G1_VLPOS G1_VLPOS; /* 0x788c */
    volatile U_G1_BK G1_BK; /* 0x7890 */
    volatile U_G1_ALPHA G1_ALPHA; /* 0x7894 */
    volatile U_G1_MUTE_BK G1_MUTE_BK; /* 0x7898 */
    volatile U_G1_LBOX_CTRL G1_LBOX_CTRL; /* 0x789c */
    volatile unsigned int reserved_23[24]; /* 0x78a0~0x78fc */
    volatile U_G1_CSC_IDC G1_CSC_IDC; /* 0x7900 */
    volatile U_G1_CSC_ODC G1_CSC_ODC; /* 0x7904 */
    volatile U_G1_CSC_IODC G1_CSC_IODC; /* 0x7908 */
    volatile U_G1_CSC_P0 G1_CSC_P0; /* 0x790c */
    volatile U_G1_CSC_P1 G1_CSC_P1; /* 0x7910 */
    volatile U_G1_CSC_P2 G1_CSC_P2; /* 0x7914 */
    volatile U_G1_CSC_P3 G1_CSC_P3; /* 0x7918 */
    volatile U_G1_CSC_P4 G1_CSC_P4; /* 0x791c */
    volatile U_G1_CSC1_IDC G1_CSC1_IDC; /* 0x7920 */
    volatile U_G1_CSC1_ODC G1_CSC1_ODC; /* 0x7924 */
    volatile U_G1_CSC1_IODC G1_CSC1_IODC; /* 0x7928 */
    volatile U_G1_CSC1_P0 G1_CSC1_P0; /* 0x792c */
    volatile U_G1_CSC1_P1 G1_CSC1_P1; /* 0x7930 */
    volatile U_G1_CSC1_P2 G1_CSC1_P2; /* 0x7934 */
    volatile U_G1_CSC1_P3 G1_CSC1_P3; /* 0x7938 */
    volatile U_G1_CSC1_P4 G1_CSC1_P4; /* 0x793c */
    volatile unsigned int reserved_24[944]; /* 0x7940~0x87fc */
    volatile U_G3_CTRL G3_CTRL; /* 0x8800 */
    volatile U_G3_UPD G3_UPD; /* 0x8804 */
    volatile unsigned int G3_GALPHA_SUM; /* 0x8808 */
    volatile U_G3_0RESO_READ G3_0RESO_READ; /* 0x880c */
    volatile unsigned int reserved_25[28]; /* 0x8810~0x887c */
    volatile U_G3_DFPOS G3_DFPOS; /* 0x8880 */
    volatile U_G3_DLPOS G3_DLPOS; /* 0x8884 */
    volatile U_G3_VFPOS G3_VFPOS; /* 0x8888 */
    volatile U_G3_VLPOS G3_VLPOS; /* 0x888c */
    volatile U_G3_BK G3_BK; /* 0x8890 */
    volatile U_G3_ALPHA G3_ALPHA; /* 0x8894 */
    volatile U_G3_MUTE_BK G3_MUTE_BK; /* 0x8898 */
    volatile U_G3_LBOX_CTRL G3_LBOX_CTRL; /* 0x889c */
    volatile unsigned int reserved_26[24]; /* 0x88a0~0x88fc */
    volatile U_G3_CSC_IDC G3_CSC_IDC; /* 0x8900 */
    volatile U_G3_CSC_ODC G3_CSC_ODC; /* 0x8904 */
    volatile U_G3_CSC_IODC G3_CSC_IODC; /* 0x8908 */
    volatile U_G3_CSC_P0 G3_CSC_P0; /* 0x890c */
    volatile U_G3_CSC_P1 G3_CSC_P1; /* 0x8910 */
    volatile U_G3_CSC_P2 G3_CSC_P2; /* 0x8914 */
    volatile U_G3_CSC_P3 G3_CSC_P3; /* 0x8918 */
    volatile U_G3_CSC_P4 G3_CSC_P4; /* 0x891c */
    volatile U_G3_CSC1_IDC G3_CSC1_IDC; /* 0x8920 */
    volatile U_G3_CSC1_ODC G3_CSC1_ODC; /* 0x8924 */
    volatile U_G3_CSC1_IODC G3_CSC1_IODC; /* 0x8928 */
    volatile U_G3_CSC1_P0 G3_CSC1_P0; /* 0x892c */
    volatile U_G3_CSC1_P1 G3_CSC1_P1; /* 0x8930 */
    volatile U_G3_CSC1_P2 G3_CSC1_P2; /* 0x8934 */
    volatile U_G3_CSC1_P3 G3_CSC1_P3; /* 0x8938 */
    volatile U_G3_CSC1_P4 G3_CSC1_P4; /* 0x893c */
    volatile unsigned int reserved_27[4528]; /* 0x8940~0xcffc */
    volatile U_DHD0_CTRL DHD0_CTRL; /* 0xd000 */
    volatile U_DHD0_VSYNC1 DHD0_VSYNC1; /* 0xd004 */
    volatile U_DHD0_VSYNC2 DHD0_VSYNC2; /* 0xd008 */
    volatile U_DHD0_HSYNC1 DHD0_HSYNC1; /* 0xd00c */
    volatile U_DHD0_HSYNC2 DHD0_HSYNC2; /* 0xd010 */
    volatile U_DHD0_VPLUS1 DHD0_VPLUS1; /* 0xd014 */
    volatile U_DHD0_VPLUS2 DHD0_VPLUS2; /* 0xd018 */
    volatile U_DHD0_PWR DHD0_PWR; /* 0xd01c */
    volatile U_DHD0_VTTHD3 DHD0_VTTHD3; /* 0xd020 */
    volatile U_DHD0_VTTHD DHD0_VTTHD; /* 0xd024 */
    volatile U_DHD0_PARATHD DHD0_PARATHD; /* 0xd028 */
    volatile U_DHD0_PRECHARGE_THD DHD0_PRECHARGE_THD; /* 0xd02c */
    volatile U_DHD0_START_POS DHD0_START_POS; /* 0xd030 */
    volatile U_DHD0_START_POS1 DHD0_START_POS1; /* 0xd034 */
    volatile U_DHD0_PARAUP DHD0_PARAUP; /* 0xd038 */
    volatile U_DHD0_SYNC_INV DHD0_SYNC_INV; /* 0xd03c */
    volatile U_DHD0_CLK_DV_CTRL DHD0_CLK_DV_CTRL; /* 0xd040 */
    volatile U_DHD0_RGB_FIX_CTRL DHD0_RGB_FIX_CTRL; /* 0xd044 */
    volatile U_DHD0_LOCKCFG DHD0_LOCKCFG; /* 0xd048 */
    volatile unsigned int DHD0_CAP_FRM_CNT; /* 0xd04c */
    volatile unsigned int DHD0_VDP_FRM_CNT; /* 0xd050 */
    volatile unsigned int DHD0_VSYNC_CAP_VDP_CNT; /* 0xd054 */
    volatile unsigned int DHD0_INTF_CHKSUM_Y; /* 0xd058 */
    volatile unsigned int DHD0_INTF_CHKSUM_U; /* 0xd05c */
    volatile unsigned int DHD0_INTF_CHKSUM_V; /* 0xd060 */
    volatile unsigned int DHD0_INTF1_CHKSUM_Y; /* 0xd064 */
    volatile unsigned int DHD0_INTF1_CHKSUM_U; /* 0xd068 */
    volatile unsigned int DHD0_INTF1_CHKSUM_V; /* 0xd06c */
    volatile U_DHD0_INTF_CHKSUM_HIGH1 DHD0_INTF_CHKSUM_HIGH1; /* 0xd070 */
    volatile U_DHD0_INTF_CHKSUM_HIGH2 DHD0_INTF_CHKSUM_HIGH2; /* 0xd074 */
    volatile unsigned int reserved_28[3]; /* 0xd078~0xd080 */
    volatile unsigned int DHD0_AFIFO_PRE_THD; /* 0xd084 */
    volatile U_DHD0_STATE DHD0_STATE; /* 0xd088 */
    volatile U_DHD0_UF_STATE DHD0_UF_STATE; /* 0xd08c */
    volatile U_VO_MUX VO_MUX; /* 0xd090 */
    volatile U_VO_MUX_SYNC VO_MUX_SYNC; /* 0xd094 */
    volatile U_VO_MUX_DATA VO_MUX_DATA; /* 0xd098 */
    volatile unsigned int reserved_29[985]; /* 0xd09c~0xdffc */
    volatile U_DHD1_CTRL DHD1_CTRL; /* 0xe000 */
    volatile U_DHD1_VSYNC1 DHD1_VSYNC1; /* 0xe004 */
    volatile U_DHD1_VSYNC2 DHD1_VSYNC2; /* 0xe008 */
    volatile U_DHD1_HSYNC1 DHD1_HSYNC1; /* 0xe00c */
    volatile U_DHD1_HSYNC2 DHD1_HSYNC2; /* 0xe010 */
    volatile U_DHD1_VPLUS1 DHD1_VPLUS1; /* 0xe014 */
    volatile U_DHD1_VPLUS2 DHD1_VPLUS2; /* 0xe018 */
    volatile U_DHD1_PWR DHD1_PWR; /* 0xe01c */
    volatile U_DHD1_VTTHD3 DHD1_VTTHD3; /* 0xe020 */
    volatile U_DHD1_VTTHD DHD1_VTTHD; /* 0xe024 */
    volatile U_DHD1_PARATHD DHD1_PARATHD; /* 0xe028 */
    volatile U_DHD1_PRECHARGE_THD DHD1_PRECHARGE_THD; /* 0xe02c */
    volatile U_DHD1_START_POS DHD1_START_POS; /* 0xe030 */
    volatile U_DHD1_START_POS1 DHD1_START_POS1; /* 0xe034 */
    volatile U_DHD1_PARAUP DHD1_PARAUP; /* 0xe038 */
    volatile U_DHD1_SYNC_INV DHD1_SYNC_INV; /* 0xe03c */
    volatile U_DHD1_CLK_DV_CTRL DHD1_CLK_DV_CTRL; /* 0xe040 */
    volatile U_DHD1_RGB_FIX_CTRL DHD1_RGB_FIX_CTRL; /* 0xe044 */
    volatile U_DHD1_LOCKCFG DHD1_LOCKCFG; /* 0xe048 */
    volatile unsigned int DHD1_CAP_FRM_CNT; /* 0xe04c */
    volatile unsigned int DHD1_VDP_FRM_CNT; /* 0xe050 */
    volatile unsigned int DHD1_VSYNC_CAP_VDP_CNT; /* 0xe054 */
    volatile unsigned int DHD1_INTF_CHKSUM_Y; /* 0xe058 */
    volatile unsigned int DHD1_INTF_CHKSUM_U; /* 0xe05c */
    volatile unsigned int DHD1_INTF_CHKSUM_V; /* 0xe060 */
    volatile unsigned int DHD1_INTF1_CHKSUM_Y; /* 0xe064 */
    volatile unsigned int DHD1_INTF1_CHKSUM_U; /* 0xe068 */
    volatile unsigned int DHD1_INTF1_CHKSUM_V; /* 0xe06c */
    volatile U_DHD1_INTF_CHKSUM_HIGH1 DHD1_INTF_CHKSUM_HIGH1; /* 0xe070 */
    volatile U_DHD1_INTF_CHKSUM_HIGH2 DHD1_INTF_CHKSUM_HIGH2; /* 0xe074 */
    volatile unsigned int reserved_30[3]; /* 0xe078~0xe080 */
    volatile unsigned int DHD1_AFIFO_PRE_THD; /* 0xe084 */
    volatile U_DHD1_STATE DHD1_STATE; /* 0xe088 */
    volatile U_DHD1_UF_STATE DHD1_UF_STATE; /* 0xe08c */
    volatile unsigned int reserved_31[2908]; /* 0xe090~0x10dfc */
    volatile U_GFX_READ_CTRL GFX_READ_CTRL; /* 0x10e00 */
    volatile U_GFX_MAC_CTRL GFX_MAC_CTRL; /* 0x10e04 */
    volatile U_GFX_OUT_CTRL GFX_OUT_CTRL; /* 0x10e08 */
    volatile unsigned int reserved_32; /* 0x10e0c */
    volatile U_GFX_MUTE_ALPHA GFX_MUTE_ALPHA; /* 0x10e10 */
    volatile U_GFX_MUTE_BK GFX_MUTE_BK; /* 0x10e14 */
    volatile unsigned int reserved_33[2]; /* 0x10e18~0x10e1c */
    volatile U_GFX_SMMU_BYPASS GFX_SMMU_BYPASS; /* 0x10e20 */
    volatile unsigned int reserved_34; /* 0x10e24 */
    volatile U_GFX_1555_ALPHA GFX_1555_ALPHA; /* 0x10e28 */
    volatile unsigned int reserved_35[5]; /* 0x10e2c~0x10e3c */
    volatile U_GFX_SRC_INFO GFX_SRC_INFO; /* 0x10e40 */
    volatile U_GFX_SRC_RESO GFX_SRC_RESO; /* 0x10e44 */
    volatile U_GFX_SRC_CROP GFX_SRC_CROP; /* 0x10e48 */
    volatile U_GFX_IRESO GFX_IRESO; /* 0x10e4c */
    volatile unsigned int GFX_ADDR_H; /* 0x10e50 */
    volatile unsigned int GFX_ADDR_L; /* 0x10e54 */
    volatile unsigned int GFX_NADDR_H; /* 0x10e58 */
    volatile unsigned int GFX_NADDR_L; /* 0x10e5c */
    volatile U_GFX_STRIDE GFX_STRIDE; /* 0x10e60 */
    volatile unsigned int reserved_36[3]; /* 0x10e64~0x10e6c */
    volatile unsigned int GFX_DCMP_ADDR_H; /* 0x10e70 */
    volatile unsigned int GFX_DCMP_ADDR_L; /* 0x10e74 */
    volatile unsigned int GFX_DCMP_NADDR_H; /* 0x10e78 */
    volatile unsigned int GFX_DCMP_NADDR_L; /* 0x10e7c */
    volatile unsigned int reserved_37[32]; /* 0x10e80~0x10efc */
    volatile U_GFX_CKEY_MAX GFX_CKEY_MAX; /* 0x10f00 */
    volatile U_GFX_CKEY_MIN GFX_CKEY_MIN; /* 0x10f04 */
    volatile U_GFX_CKEY_MASK GFX_CKEY_MASK; /* 0x10f08 */
    volatile unsigned int reserved_38; /* 0x10f0c */
    volatile U_GFX_TESTPAT_CFG GFX_TESTPAT_CFG; /* 0x10f10 */
    volatile U_GFX_TESTPAT_SEED GFX_TESTPAT_SEED; /* 0x10f14 */
    volatile unsigned int reserved_39[2]; /* 0x10f18~0x10f1c */
    volatile U_GFX_LATENCY_CLR GFX_LATENCY_CLR; /* 0x10f20 */
    volatile U_GFX_LINE_LATENCY GFX_LINE_LATENCY; /* 0x10f24 */
    volatile unsigned int GFX_FRAM_LATENCY; /* 0x10f28 */
    volatile unsigned int reserved_40; /* 0x10f2c */
    volatile U_GFX_DEBUG_CTRL GFX_DEBUG_CTRL; /* 0x10f30 */
    volatile U_GFX_STA_CLR GFX_STA_CLR; /* 0x10f34 */
    volatile unsigned int GFX_STATE0; /* 0x10f38 */
    volatile unsigned int GFX_STATE1; /* 0x10f3c */
    volatile unsigned int GFX_STATE2; /* 0x10f40 */
    volatile unsigned int GFX_STATE3; /* 0x10f44 */
    volatile unsigned int reserved_41[3]; /* 0x10f48~0x10f50 */
    volatile unsigned int GFX_DCMP_FRAMESIZE0; /* 0x10f54 */
    volatile unsigned int GFX_DCMP_FRAMESIZE1; /* 0x10f58 */
    volatile U_GFX_DCMPERRCLR GFX_DCMPERRCLR; /* 0x10f5c */
    volatile U_GFX_DCMP_ERR GFX_DCMP_ERR; /* 0x10f60 */
    volatile unsigned int GFX_DCMP_STATE; /* 0x10f64 */
    volatile unsigned int reserved_42[6]; /* 0x10f68~0x10f7c */
    volatile U_GFX_OSD_GLB_INFO GFX_OSD_GLB_INFO; /* 0x10f80 */
    volatile U_GFX_OSD_FRAME_SIZE GFX_OSD_FRAME_SIZE; /* 0x10f84 */
    volatile unsigned int reserved_43[2]; /* 0x10f88~0x10f8c */
    volatile U_GFX_OSD_DBG_REG GFX_OSD_DBG_REG; /* 0x10f90 */
    volatile U_GFX_OSD_DBG_REG1 GFX_OSD_DBG_REG1; /* 0x10f94 */

} S_VDP_REGS_TYPE;

/* Declare the struct pointor of the module VDP */
/****************************************************************************
*
* Algorithm IP
*
****************************************************************************/
/* Define the union U_HIPPCTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 28; /* [27..0]  */
        unsigned int int_test_mode : 1; /* [28]  */
        unsigned int int_test : 1; /* [29]  */
        unsigned int chk_sum_en : 1; /* [30]  */
        unsigned int vo_ck_gt_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIPPCTRL;

/* Define the union U_GHDR_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_en : 1; /* [0]  */
        unsigned int ghdr_ck_gt_en : 1; /* [1]  */
        unsigned int reserved_0 : 10; /* [11..2]  */
        unsigned int ghdr_demo_en : 1; /* [12]  */
        unsigned int ghdr_demo_mode : 1; /* [13]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int ghdr_demo_pos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_CTRL;

/* Define the union U_GHDR_DEGAMMA_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_degmm_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DEGAMMA_CTRL;

/* Define the union U_GHDR_DEGAMMA_STEP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_degmm_x1_step : 4; /* [3..0]  */
        unsigned int g_degmm_x2_step : 4; /* [7..4]  */
        unsigned int g_degmm_x3_step : 4; /* [11..8]  */
        unsigned int g_degmm_x4_step : 4; /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DEGAMMA_STEP;

/* Define the union U_GHDR_DEGAMMA_POS1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_degmm_x1_pos : 10; /* [9..0]  */
        unsigned int g_degmm_x2_pos : 10; /* [19..10]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DEGAMMA_POS1;

/* Define the union U_GHDR_DEGAMMA_POS2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_degmm_x3_pos : 10; /* [9..0]  */
        unsigned int g_degmm_x4_pos : 10; /* [19..10]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DEGAMMA_POS2;

/* Define the union U_GHDR_DEGAMMA_NUM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_degmm_x1_num : 6; /* [5..0]  */
        unsigned int reserved_0 : 2; /* [7..6]  */
        unsigned int g_degmm_x2_num : 6; /* [13..8]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int g_degmm_x3_num : 6; /* [21..16]  */
        unsigned int reserved_2 : 2; /* [23..22]  */
        unsigned int g_degmm_x4_num : 6; /* [29..24]  */
        unsigned int reserved_3 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DEGAMMA_NUM;

/* Define the union U_GHDR_GAMUT_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_CTRL;

/* Define the union U_GHDR_GAMUT_COEF00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_coef00 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_COEF00;

/* Define the union U_GHDR_GAMUT_COEF01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_coef01 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_COEF01;

/* Define the union U_GHDR_GAMUT_COEF02 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_coef02 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_COEF02;

/* Define the union U_GHDR_GAMUT_COEF10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_coef10 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_COEF10;

/* Define the union U_GHDR_GAMUT_COEF11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_coef11 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_COEF11;

/* Define the union U_GHDR_GAMUT_COEF12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_coef12 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_COEF12;

/* Define the union U_GHDR_GAMUT_COEF20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_coef20 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_COEF20;

/* Define the union U_GHDR_GAMUT_COEF21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_coef21 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_COEF21;

/* Define the union U_GHDR_GAMUT_COEF22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_coef22 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_COEF22;

/* Define the union U_GHDR_GAMUT_SCALE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_scale : 4; /* [3..0]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_SCALE;

/* Define the union U_GHDR_GAMUT_CLIP_MIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_clip_min : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_CLIP_MIN;

/* Define the union U_GHDR_GAMUT_CLIP_MAX */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamut_clip_max : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMUT_CLIP_MAX;

/* Define the union U_GHDR_TONEMAP_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_tmap_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_CTRL;

/* Define the union U_GHDR_TONEMAP_REN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_tmap_rd_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_REN;

/* Define the union U_GHDR_TONEMAP_STEP */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_tmap_x1_step : 4; /* [3..0]  */
        unsigned int reserved_0 : 4; /* [7..4]  */
        unsigned int g_tmap_x2_step : 4; /* [11..8]  */
        unsigned int reserved_1 : 4; /* [15..12]  */
        unsigned int g_tmap_x3_step : 4; /* [19..16]  */
        unsigned int reserved_2 : 4; /* [23..20]  */
        unsigned int g_tmap_x4_step : 4; /* [27..24]  */
        unsigned int reserved_3 : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_STEP;

/* Define the union U_GHDR_TONEMAP_POS1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_tmap_x1_pos : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_POS1;

/* Define the union U_GHDR_TONEMAP_POS2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_tmap_x2_pos : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_POS2;

/* Define the union U_GHDR_TONEMAP_POS3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_tmap_x3_pos : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_POS3;

/* Define the union U_GHDR_TONEMAP_POS4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_tmap_x4_pos : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_POS4;

/* Define the union U_GHDR_TONEMAP_NUM */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_tmap_x1_num : 5; /* [4..0]  */
        unsigned int reserved_0 : 3; /* [7..5]  */
        unsigned int g_tmap_x2_num : 5; /* [12..8]  */
        unsigned int reserved_1 : 3; /* [15..13]  */
        unsigned int g_tmap_x3_num : 5; /* [20..16]  */
        unsigned int reserved_2 : 3; /* [23..21]  */
        unsigned int g_tmap_x4_num : 5; /* [28..24]  */
        unsigned int reserved_3 : 3; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_NUM;

/* Define the union U_GHDR_TONEMAP_LUMA_COEF0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_tmap_luma_coef0 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_LUMA_COEF0;

/* Define the union U_GHDR_TONEMAP_LUMA_COEF1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_tmap_luma_coef1 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_LUMA_COEF1;

/* Define the union U_GHDR_TONEMAP_LUMA_COEF2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_tmap_luma_coef2 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_LUMA_COEF2;

/* Define the union U_GHDR_TONEMAP_LUMA_SCALE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_tmap_luma_scale : 4; /* [3..0]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_LUMA_SCALE;

/* Define the union U_GHDR_TONEMAP_COEF_SCALE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_tmap_coef_scale : 4; /* [3..0]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_COEF_SCALE;

/* Define the union U_GHDR_TONEMAP_OUT_CLIP_MIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_tmap_out_clip_min : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_OUT_CLIP_MIN;

/* Define the union U_GHDR_TONEMAP_OUT_CLIP_MAX */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_tmap_out_clip_max : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_TONEMAP_OUT_CLIP_MAX;

/* Define the union U_GHDR_GAMMA_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gmm_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMMA_CTRL;

/* Define the union U_GHDR_GAMMA_REN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_gamma_rd_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMMA_REN;

/* Define the union U_GHDR_GAMMA_STEP1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_gmm_x1_step : 4; /* [3..0]  */
        unsigned int reserved_0 : 4; /* [7..4]  */
        unsigned int g_gmm_x2_step : 4; /* [11..8]  */
        unsigned int reserved_1 : 4; /* [15..12]  */
        unsigned int g_gmm_x3_step : 4; /* [19..16]  */
        unsigned int reserved_2 : 4; /* [23..20]  */
        unsigned int g_gmm_x4_step : 4; /* [27..24]  */
        unsigned int reserved_3 : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMMA_STEP1;

/* Define the union U_GHDR_GAMMA_STEP2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_gmm_x5_step : 4; /* [3..0]  */
        unsigned int reserved_0 : 4; /* [7..4]  */
        unsigned int g_gmm_x6_step : 4; /* [11..8]  */
        unsigned int reserved_1 : 4; /* [15..12]  */
        unsigned int g_gmm_x7_step : 4; /* [19..16]  */
        unsigned int reserved_2 : 4; /* [23..20]  */
        unsigned int g_gmm_x8_step : 4; /* [27..24]  */
        unsigned int reserved_3 : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMMA_STEP2;

/* Define the union U_GHDR_GAMMA_POS1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_gmm_x1_pos : 16; /* [15..0]  */
        unsigned int g_gmm_x2_pos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMMA_POS1;

/* Define the union U_GHDR_GAMMA_POS2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_gmm_x3_pos : 16; /* [15..0]  */
        unsigned int g_gmm_x4_pos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMMA_POS2;

/* Define the union U_GHDR_GAMMA_POS3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_gmm_x5_pos : 16; /* [15..0]  */
        unsigned int g_gmm_x6_pos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMMA_POS3;

/* Define the union U_GHDR_GAMMA_POS4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_gmm_x7_pos : 16; /* [15..0]  */
        unsigned int g_gmm_x8_pos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMMA_POS4;

/* Define the union U_GHDR_GAMMA_NUM1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_gmm_x1_num : 6; /* [5..0]  */
        unsigned int reserved_0 : 2; /* [7..6]  */
        unsigned int g_gmm_x2_num : 6; /* [13..8]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int g_gmm_x3_num : 6; /* [21..16]  */
        unsigned int reserved_2 : 2; /* [23..22]  */
        unsigned int g_gmm_x4_num : 6; /* [29..24]  */
        unsigned int reserved_3 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMMA_NUM1;

/* Define the union U_GHDR_GAMMA_NUM2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_gmm_x5_num : 6; /* [5..0]  */
        unsigned int reserved_0 : 2; /* [7..6]  */
        unsigned int g_gmm_x6_num : 6; /* [13..8]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int g_gmm_x7_num : 6; /* [21..16]  */
        unsigned int reserved_2 : 2; /* [23..22]  */
        unsigned int g_gmm_x8_num : 6; /* [29..24]  */
        unsigned int reserved_3 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_GAMMA_NUM2;

/* Define the union U_GHDR_DITHER_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_dither_tap_mode : 2; /* [1..0]  */
        unsigned int ghdr_dither_domain_mode : 1; /* [2]  */
        unsigned int ghdr_dither_round : 1; /* [3]  */
        unsigned int ghdr_dither_mode : 1; /* [4]  */
        unsigned int ghdr_dither_en : 1; /* [5]  */
        unsigned int ghdr_dither_round_unlim : 1; /* [6]  */
        unsigned int reserved_0 : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DITHER_CTRL;

/* Define the union U_GHDR_DITHER_THR */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_dither_thr_min : 16; /* [15..0]  */
        unsigned int ghdr_dither_thr_max : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DITHER_THR;

/* Define the union U_GHDR_DITHER_SED_Y0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_dither_sed_y0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DITHER_SED_Y0;

/* Define the union U_GHDR_DITHER_SED_U0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_dither_sed_u0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DITHER_SED_U0;

/* Define the union U_GHDR_DITHER_SED_V0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_dither_sed_v0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DITHER_SED_V0;

/* Define the union U_GHDR_DITHER_SED_W0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_dither_sed_w0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DITHER_SED_W0;

/* Define the union U_GHDR_DITHER_SED_Y1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_dither_sed_y1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DITHER_SED_Y1;

/* Define the union U_GHDR_DITHER_SED_U1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_dither_sed_u1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DITHER_SED_U1;

/* Define the union U_GHDR_DITHER_SED_V1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_dither_sed_v1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DITHER_SED_V1;

/* Define the union U_GHDR_DITHER_SED_W1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ghdr_dither_sed_w1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_GHDR_DITHER_SED_W1;

/* Define the union U_HIHDR_G_RGB2YUV_CTRL */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_CTRL;

/* Define the union U_HIHDR_G_RGB2YUV_COEF0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_coef00 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_COEF0;

/* Define the union U_HIHDR_G_RGB2YUV_COEF1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_coef01 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_COEF1;

/* Define the union U_HIHDR_G_RGB2YUV_COEF2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_coef02 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_COEF2;

/* Define the union U_HIHDR_G_RGB2YUV_COEF3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_coef10 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_COEF3;

/* Define the union U_HIHDR_G_RGB2YUV_COEF4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_coef11 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_COEF4;

/* Define the union U_HIHDR_G_RGB2YUV_COEF5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_coef12 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_COEF5;

/* Define the union U_HIHDR_G_RGB2YUV_COEF6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_coef20 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_COEF6;

/* Define the union U_HIHDR_G_RGB2YUV_COEF7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_coef21 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_COEF7;

/* Define the union U_HIHDR_G_RGB2YUV_COEF8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_coef22 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_COEF8;

/* Define the union U_HIHDR_G_RGB2YUV_SCALE2P */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_scale2p : 4; /* [3..0]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_SCALE2P;

/* Define the union U_HIHDR_G_RGB2YUV_IDC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_idc0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_IDC0;

/* Define the union U_HIHDR_G_RGB2YUV_IDC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_idc1 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_IDC1;

/* Define the union U_HIHDR_G_RGB2YUV_IDC2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_idc2 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_IDC2;

/* Define the union U_HIHDR_G_RGB2YUV_ODC0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_odc0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_ODC0;

/* Define the union U_HIHDR_G_RGB2YUV_ODC1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_odc1 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_ODC1;

/* Define the union U_HIHDR_G_RGB2YUV_ODC2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_odc2 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_ODC2;

/* Define the union U_HIHDR_G_RGB2YUV_MIN */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_clip_min : 10; /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_MIN;

/* Define the union U_HIHDR_G_RGB2YUV_MAX */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hihdr_r2y_clip_max : 10; /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;

} U_HIHDR_G_RGB2YUV_MAX;

// ==============================================================================
/* Define the global struct */
typedef struct {
    volatile U_HIPPCTRL HIPPCTRL; /* 0x0 */
    volatile unsigned int reserved_0[24575]; /* 0x4~0x17ffc */
    volatile U_GHDR_CTRL GHDR_CTRL; /* 0x18000 */
    volatile unsigned int reserved_1[63]; /* 0x18004~0x180fc */
    volatile U_GHDR_DEGAMMA_CTRL GHDR_DEGAMMA_CTRL; /* 0x18100 */
    volatile U_GHDR_DEGAMMA_STEP GHDR_DEGAMMA_STEP; /* 0x18104 */
    volatile U_GHDR_DEGAMMA_POS1 GHDR_DEGAMMA_POS1; /* 0x18108 */
    volatile U_GHDR_DEGAMMA_POS2 GHDR_DEGAMMA_POS2; /* 0x1810c */
    volatile U_GHDR_DEGAMMA_NUM GHDR_DEGAMMA_NUM; /* 0x18110 */
    volatile unsigned int reserved_2[59]; /* 0x18114~0x181fc */
    volatile U_GHDR_GAMUT_CTRL GHDR_GAMUT_CTRL; /* 0x18200 */
    volatile U_GHDR_GAMUT_COEF00 GHDR_GAMUT_COEF00; /* 0x18204 */
    volatile U_GHDR_GAMUT_COEF01 GHDR_GAMUT_COEF01; /* 0x18208 */
    volatile U_GHDR_GAMUT_COEF02 GHDR_GAMUT_COEF02; /* 0x1820c */
    volatile U_GHDR_GAMUT_COEF10 GHDR_GAMUT_COEF10; /* 0x18210 */
    volatile U_GHDR_GAMUT_COEF11 GHDR_GAMUT_COEF11; /* 0x18214 */
    volatile U_GHDR_GAMUT_COEF12 GHDR_GAMUT_COEF12; /* 0x18218 */
    volatile U_GHDR_GAMUT_COEF20 GHDR_GAMUT_COEF20; /* 0x1821c */
    volatile U_GHDR_GAMUT_COEF21 GHDR_GAMUT_COEF21; /* 0x18220 */
    volatile U_GHDR_GAMUT_COEF22 GHDR_GAMUT_COEF22; /* 0x18224 */
    volatile U_GHDR_GAMUT_SCALE GHDR_GAMUT_SCALE; /* 0x18228 */
    volatile U_GHDR_GAMUT_CLIP_MIN GHDR_GAMUT_CLIP_MIN; /* 0x1822c */
    volatile U_GHDR_GAMUT_CLIP_MAX GHDR_GAMUT_CLIP_MAX; /* 0x18230 */
    volatile unsigned int reserved_3[51]; /* 0x18234~0x182fc */
    volatile U_GHDR_TONEMAP_CTRL GHDR_TONEMAP_CTRL; /* 0x18300 */
    volatile U_GHDR_TONEMAP_REN GHDR_TONEMAP_REN; /* 0x18304 */
    volatile unsigned int GHDR_TONEMAP_DATA; /* 0x18308 */
    volatile U_GHDR_TONEMAP_STEP GHDR_TONEMAP_STEP; /* 0x1830c */
    volatile U_GHDR_TONEMAP_POS1 GHDR_TONEMAP_POS1; /* 0x18310 */
    volatile U_GHDR_TONEMAP_POS2 GHDR_TONEMAP_POS2; /* 0x18314 */
    volatile U_GHDR_TONEMAP_POS3 GHDR_TONEMAP_POS3; /* 0x18318 */
    volatile U_GHDR_TONEMAP_POS4 GHDR_TONEMAP_POS4; /* 0x1831c */
    volatile U_GHDR_TONEMAP_NUM GHDR_TONEMAP_NUM; /* 0x18320 */
    volatile U_GHDR_TONEMAP_LUMA_COEF0 GHDR_TONEMAP_LUMA_COEF0; /* 0x18324 */
    volatile U_GHDR_TONEMAP_LUMA_COEF1 GHDR_TONEMAP_LUMA_COEF1; /* 0x18328 */
    volatile U_GHDR_TONEMAP_LUMA_COEF2 GHDR_TONEMAP_LUMA_COEF2; /* 0x1832c */
    volatile U_GHDR_TONEMAP_LUMA_SCALE GHDR_TONEMAP_LUMA_SCALE; /* 0x18330 */
    volatile U_GHDR_TONEMAP_COEF_SCALE GHDR_TONEMAP_COEF_SCALE; /* 0x18334 */
    volatile U_GHDR_TONEMAP_OUT_CLIP_MIN GHDR_TONEMAP_OUT_CLIP_MIN; /* 0x18338 */
    volatile U_GHDR_TONEMAP_OUT_CLIP_MAX GHDR_TONEMAP_OUT_CLIP_MAX; /* 0x1833c */
    volatile unsigned int reserved_4[48]; /* 0x18340~0x183fc */
    volatile U_GHDR_GAMMA_CTRL GHDR_GAMMA_CTRL; /* 0x18400 */
    volatile U_GHDR_GAMMA_REN GHDR_GAMMA_REN; /* 0x18404 */
    volatile unsigned int GHDR_GAMMMA_DATA; /* 0x18408 */
    volatile U_GHDR_GAMMA_STEP1 GHDR_GAMMA_STEP1; /* 0x1840c */
    volatile U_GHDR_GAMMA_STEP2 GHDR_GAMMA_STEP2; /* 0x18410 */
    volatile U_GHDR_GAMMA_POS1 GHDR_GAMMA_POS1; /* 0x18414 */
    volatile U_GHDR_GAMMA_POS2 GHDR_GAMMA_POS2; /* 0x18418 */
    volatile U_GHDR_GAMMA_POS3 GHDR_GAMMA_POS3; /* 0x1841c */
    volatile U_GHDR_GAMMA_POS4 GHDR_GAMMA_POS4; /* 0x18420 */
    volatile U_GHDR_GAMMA_NUM1 GHDR_GAMMA_NUM1; /* 0x18424 */
    volatile U_GHDR_GAMMA_NUM2 GHDR_GAMMA_NUM2; /* 0x18428 */
    volatile unsigned int reserved_5[53]; /* 0x1842c~0x184fc */
    volatile U_GHDR_DITHER_CTRL GHDR_DITHER_CTRL; /* 0x18500 */
    volatile U_GHDR_DITHER_THR GHDR_DITHER_THR; /* 0x18504 */
    volatile U_GHDR_DITHER_SED_Y0 GHDR_DITHER_SED_Y0; /* 0x18508 */
    volatile U_GHDR_DITHER_SED_U0 GHDR_DITHER_SED_U0; /* 0x1850c */
    volatile U_GHDR_DITHER_SED_V0 GHDR_DITHER_SED_V0; /* 0x18510 */
    volatile U_GHDR_DITHER_SED_W0 GHDR_DITHER_SED_W0; /* 0x18514 */
    volatile U_GHDR_DITHER_SED_Y1 GHDR_DITHER_SED_Y1; /* 0x18518 */
    volatile U_GHDR_DITHER_SED_U1 GHDR_DITHER_SED_U1; /* 0x1851c */
    volatile U_GHDR_DITHER_SED_V1 GHDR_DITHER_SED_V1; /* 0x18520 */
    volatile U_GHDR_DITHER_SED_W1 GHDR_DITHER_SED_W1; /* 0x18524 */
    volatile unsigned int reserved_6[54]; /* 0x18528~0x185fc */
    volatile U_HIHDR_G_RGB2YUV_CTRL HIHDR_G_RGB2YUV_CTRL; /* 0x18600 */
    volatile U_HIHDR_G_RGB2YUV_COEF0 HIHDR_G_RGB2YUV_COEF0; /* 0x18604 */
    volatile U_HIHDR_G_RGB2YUV_COEF1 HIHDR_G_RGB2YUV_COEF1; /* 0x18608 */
    volatile U_HIHDR_G_RGB2YUV_COEF2 HIHDR_G_RGB2YUV_COEF2; /* 0x1860c */
    volatile U_HIHDR_G_RGB2YUV_COEF3 HIHDR_G_RGB2YUV_COEF3; /* 0x18610 */
    volatile U_HIHDR_G_RGB2YUV_COEF4 HIHDR_G_RGB2YUV_COEF4; /* 0x18614 */
    volatile U_HIHDR_G_RGB2YUV_COEF5 HIHDR_G_RGB2YUV_COEF5; /* 0x18618 */
    volatile U_HIHDR_G_RGB2YUV_COEF6 HIHDR_G_RGB2YUV_COEF6; /* 0x1861c */
    volatile U_HIHDR_G_RGB2YUV_COEF7 HIHDR_G_RGB2YUV_COEF7; /* 0x18620 */
    volatile U_HIHDR_G_RGB2YUV_COEF8 HIHDR_G_RGB2YUV_COEF8; /* 0x18624 */
    volatile U_HIHDR_G_RGB2YUV_SCALE2P HIHDR_G_RGB2YUV_SCALE2P; /* 0x18628 */
    volatile U_HIHDR_G_RGB2YUV_IDC0 HIHDR_G_RGB2YUV_IDC0; /* 0x1862c */
    volatile U_HIHDR_G_RGB2YUV_IDC1 HIHDR_G_RGB2YUV_IDC1; /* 0x18630 */
    volatile U_HIHDR_G_RGB2YUV_IDC2 HIHDR_G_RGB2YUV_IDC2; /* 0x18634 */
    volatile U_HIHDR_G_RGB2YUV_ODC0 HIHDR_G_RGB2YUV_ODC0; /* 0x18638 */
    volatile U_HIHDR_G_RGB2YUV_ODC1 HIHDR_G_RGB2YUV_ODC1; /* 0x1863c */
    volatile U_HIHDR_G_RGB2YUV_ODC2 HIHDR_G_RGB2YUV_ODC2; /* 0x18640 */
    volatile U_HIHDR_G_RGB2YUV_MIN HIHDR_G_RGB2YUV_MIN; /* 0x18644 */
    volatile U_HIHDR_G_RGB2YUV_MAX HIHDR_G_RGB2YUV_MAX; /* 0x18648 */

} S_HIPP_REGS_TYPE;

/* Declare the struct pointor of the module HIPP */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __VOU_REG_H__ */
