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

#define HI_ISP_CA_CSC_DC_LEN       (3)
#define HI_ISP_CA_CSC_COEF_LEN     (9)
#define HI_ISP_CA_CSC_TYPE_DEFAULT (0)
#define HI_ISP_CA_RATIO_MAX        (2047)


static const  HI_S32 g_as32CAIsoLut  [ISP_AUTO_ISO_STRENGTH_NUM] = {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800};
static const  HI_U32 g_au32YRatioLut[HI_ISP_CA_YRATIO_LUT_LENGTH] = {
    36, 58, 81, 96, 111, 123, 136, 147, 158, 170, 182, 194, 207, 217, 228, 243, 259, 274, 290, 303,
    317, 331, 345, 357, 369, 382, 396, 408, 420, 432, 444, 456, 468, 480, 492, 503, 515, 524, 534, 545,
    556, 565, 574, 585, 597, 605, 614, 623, 632, 640, 648, 657, 666, 673, 681, 689, 697, 703, 709, 716,
    723, 728, 734, 741, 748, 753, 758, 764, 771, 775, 780, 784, 788, 794, 800, 804, 808, 811, 815, 818,
    822, 825, 829, 833, 837, 839, 841, 844, 848, 851, 854, 856, 858, 861, 864, 866, 868, 869, 871, 874,
    878, 879, 881, 883, 885, 887, 890, 891, 893, 895, 897, 898, 900, 901, 903, 904, 906, 907, 909, 910,
    912, 913, 915, 916, 918, 919, 921, 922, 924, 925, 926, 927, 929, 930, 931, 932, 934, 935, 936, 937,
    938, 939, 941, 942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 951, 952, 953, 954, 955, 956, 957,
    958, 959, 961, 961, 962, 963, 964, 965, 966, 967, 968, 968, 969, 969, 970, 970, 971, 972, 973, 973,
    974, 975, 976, 976, 977, 978, 979, 979, 980, 980, 981, 982, 983, 983, 984, 984, 985, 985, 986, 987,
    988, 988, 989, 989, 990, 990, 991, 991, 992, 992, 993, 994, 995, 995, 996, 996, 997, 997, 998, 998,
    999, 999, 1000, 1000, 1001, 1002, 1004, 1004, 1005, 1005, 1006, 1006, 1007, 1008, 1009, 1009, 1010, 1010, 1011, 1011,
    1012, 1013, 1014, 1015, 1016, 1016, 1017, 1018, 1019, 1019, 1020, 1021, 1022, 1023, 1024, 1024
};
static const  HI_S16 g_as16ISORatio[ISP_AUTO_ISO_STRENGTH_NUM] = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
static const  HI_U32 g_au32CaCpLut[HI_ISP_CA_YRATIO_LUT_LENGTH] = {98432, 98432, 163968, 163968, 229504, 229504, 295040, 295040, 360576, 360576, 426112, 426112,
        491648, 491648, 557184, 557184, 622720, 622720, 688256, 688256, 753792, 753792, 819328, 819328, 884864, 884864, 950400, 950400, 1015936, 1015940, 1081481,
        1081481, 1147018, 1081481, 1081481, 1081481, 1081482, 1081482, 1081483, 1147020, 1212558, 1212560, 1278098, 1278100, 1343639, 1409178, 1474718, 1540256,
        1605794, 1671333, 1736872, 1736876, 1802416, 1867957, 1999034, 2064575, 2130116, 2261193, 2392270, 2457810, 2523350, 2588889, 2719964, 2785502, 2851041,
        2916578, 3047652, 3113188, 3178724, 3309797, 3440870, 3506405, 3571940, 3768547, 3965155, 4030687, 4161755, 4292824, 4489429, 4751569, 5079246, 5144775,
        5210305, 5472446, 5734587, 5669044, 5669038, 5931179, 6258856, 6652071, 7045287, 6914208, 6783130, 7110807, 7504021, 7438480, 7438475, 7766154, 8159370,
        8355976, 8552583, 8618118, 8683653, 8945798, 9273479, 9273478, 9339013, 9666694, 10059911, 10256518, 10518662, 10649732, 10846339, 11174019, 11501699,
        11501699, 11567235, 11764355, 11961731, 12027779, 12159619, 12160130, 12160898, 12292482, 12424067, 12424579, 12425348, 12360324, 12361092, 12296323,
        12231554, 12231810, 12232323, 11970691, 11774595, 11774596, 11774853, 11513476, 11317635, 11317635, 11317892, 11121796, 10991492, 10729860, 10533764,
        10533764, 10534020, 10337924, 10207364, 10142084, 10076804, 9946244, 9815684, 9553796, 9357700, 9357700, 9423493, 9096325, 8834949, 8900742, 9032328,
        8770950, 8509573, 8247685, 8051589, 7986309, 7986822, 7856006, 7725446, 7594630, 7529606, 7464327, 7399048, 7333513, 7268235, 7268236, 7268238, 7268239,
        7268241, 7268242, 7333780, 7398808, 7464092, 7463838, 7529377, 7725476, 7921831, 8182957, 8509875, 8444086, 8443833, 8836032, 9228487, 9096905, 8965579,
        9358033, 9816279, 9946586, 10077150, 10207969, 10338788, 10534887, 10730986, 10664682, 10664171, 10925805, 11253232, 11514610, 11775989, 11710195,
        11644402, 11971317, 12298488, 12298231, 12297975, 12494071, 12690168, 13016823, 13343735, 13408758, 13474038, 13669878, 13865718, 13799668, 13799154,
        14126324, 14519287, 14453237, 14452980, 14583541, 14779894, 14910966, 15042038, 15041782, 15107063, 15107062, 15107062, 15107062, 15107062, 15172342,
        15303158, 15303158, 15303158, 15368438, 15433974, 15499510, 15565046, 15630582, 15696118, 15761654, 15827190, 15892726, 15958262, 16023798, 16089334, 16089334,
                                                                  };


typedef enum hiHI_ISP_CA_CS_E {
    CA_CS_BT_709 = 0,
    CA_CS_BT_601 = 1,
    CA_CS_BUTT
} HI_ISP_CA_CS_E;

typedef struct tagHI_ISP_CA_CSC_TABLE_S {
    HI_S32 s32CSCIdc[HI_ISP_CA_CSC_DC_LEN];
    HI_S32 s32CSCOdc[HI_ISP_CA_CSC_DC_LEN];
    HI_S32 s32CSCCoef[HI_ISP_CA_CSC_COEF_LEN];
} HI_ISP_CA_CSC_TABLE_S;

static HI_ISP_CA_CSC_TABLE_S  g_stCSCTable_HDYCbCr_to_RGB = {
    {0,    -512, -512},
    {0,    0,    0},
    {1000, 0,    1575, 1000, -187, -468, 1000, 1856, 0},
}; // range[0,255]  X1000

static HI_ISP_CA_CSC_TABLE_S  g_stCSCTable_SDYCbCr_to_RGB = {
    {0,    -512, -512},
    {0,    0,    0},
    {1000, 0,    1402, 1000, -344, -714, 1000, 1772, 0},
}; // range[0,255]  X1000

typedef struct hiISP_CA_S {
    HI_BOOL bCaEn;      // u1.0
    HI_BOOL bCaCpEn;    // u1.0
    HI_BOOL bCaCoefUpdateEn;
    HI_U16 u16LumaThdHigh;
    HI_S16 s16SaturationRatio;
    HI_U32 au32YRatioLut[HI_ISP_CA_YRATIO_LUT_LENGTH];
    HI_S16 as16CaIsoRatio[ISP_AUTO_ISO_STRENGTH_NUM]; // 16
    HI_U32 au32CaCpLut[HI_ISP_CA_YRATIO_LUT_LENGTH];
} ISP_CA_S;

ISP_CA_S *g_pastCaCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define CA_GET_CTX(dev, pstCtx)   (pstCtx = g_pastCaCtx[dev])
#define CA_SET_CTX(dev, pstCtx)   (g_pastCaCtx[dev] = pstCtx)
#define CA_RESET_CTX(dev)         (g_pastCaCtx[dev] = HI_NULL)

HI_S32 CaCtxInit(VI_PIPE ViPipe)
{
    ISP_CA_S *pastCaCtx = HI_NULL;

    CA_GET_CTX(ViPipe, pastCaCtx);

    if (pastCaCtx == HI_NULL) {
        pastCaCtx = (ISP_CA_S *)ISP_MALLOC(sizeof(ISP_CA_S));
        if (pastCaCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] CaCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastCaCtx, 0, sizeof(ISP_CA_S));

    CA_SET_CTX(ViPipe, pastCaCtx);

    return HI_SUCCESS;
}

HI_VOID CaCtxExit(VI_PIPE ViPipe)
{
    ISP_CA_S *pastCaCtx = HI_NULL;

    CA_GET_CTX(ViPipe, pastCaCtx);
    ISP_FREE(pastCaCtx);
    CA_RESET_CTX(ViPipe);
}

static HI_VOID CaExtRegsInitialize(VI_PIPE ViPipe)
{
    HI_U16 i;
    ISP_CA_S  *pstCA     = HI_NULL;

    CA_GET_CTX(ViPipe, pstCA);
    ISP_CHECK_POINTER_VOID(pstCA);

    hi_ext_system_ca_en_write(ViPipe, pstCA->bCaEn);
    hi_ext_system_ca_cp_en_write(ViPipe, pstCA->bCaCpEn);

    hi_ext_system_ca_luma_thd_high_write(ViPipe, HI_ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT);
    hi_ext_system_ca_saturation_ratio_write(ViPipe, HI_ISP_EXT_CA_SATURATION_RATIO_DEFAULT);
    hi_ext_system_ca_coef_update_en_write(ViPipe, HI_TRUE);

    for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
        hi_ext_system_ca_y_ratio_lut_write(ViPipe, i, pstCA->au32YRatioLut[i]);

        hi_ext_system_ca_cp_lut_write(ViPipe, i, pstCA->au32CaCpLut[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        hi_ext_system_ca_iso_ratio_lut_write(ViPipe, i, pstCA->as16CaIsoRatio[i]);
    }

    return;
}

static HI_VOID GetCscTable(HI_ISP_CA_CS_E enCsc, HI_ISP_CA_CSC_TABLE_S **pstYuv2Rgb)
{
    switch (enCsc) {
        case CA_CS_BT_709:
            *pstYuv2Rgb = &g_stCSCTable_HDYCbCr_to_RGB;
            break;
        case CA_CS_BT_601:
            *pstYuv2Rgb = &g_stCSCTable_SDYCbCr_to_RGB;
            break;
        default:
            *pstYuv2Rgb = HI_NULL;
            break;
    }
}

static HI_VOID SetColorSpaceConvertParamsDef(ISP_CA_STATIC_CFG_S *pstStaticRegCfg, HI_ISP_CA_CS_E enCsc)
{
    HI_ISP_CA_CSC_TABLE_S *pstYuv2Rgb;

    GetCscTable(enCsc, &pstYuv2Rgb);

    if (pstYuv2Rgb == HI_NULL) {
        ISP_ERR_TRACE("Unable to handle null point in ca moudle!\n");
        return;
    }

    pstStaticRegCfg->s16CaYuv2RgbCoef00 = pstYuv2Rgb->s32CSCCoef[0] * 1024 / 1000;
    pstStaticRegCfg->s16CaYuv2RgbCoef01 = pstYuv2Rgb->s32CSCCoef[1] * 1024 / 1000;
    pstStaticRegCfg->s16CaYuv2RgbCoef02 = pstYuv2Rgb->s32CSCCoef[2] * 1024 / 1000;
    pstStaticRegCfg->s16CaYuv2RgbCoef10 = pstYuv2Rgb->s32CSCCoef[3] * 1024 / 1000;
    pstStaticRegCfg->s16CaYuv2RgbCoef11 = pstYuv2Rgb->s32CSCCoef[4] * 1024 / 1000;
    pstStaticRegCfg->s16CaYuv2RgbCoef12 = pstYuv2Rgb->s32CSCCoef[5] * 1024 / 1000;
    pstStaticRegCfg->s16CaYuv2RgbCoef20 = pstYuv2Rgb->s32CSCCoef[6] * 1024 / 1000;
    pstStaticRegCfg->s16CaYuv2RgbCoef21 = pstYuv2Rgb->s32CSCCoef[7] * 1024 / 1000;
    pstStaticRegCfg->s16CaYuv2RgbCoef22 = pstYuv2Rgb->s32CSCCoef[8] * 1024 / 1000;
    pstStaticRegCfg->s16CaYuv2RgbInDc0  = pstYuv2Rgb->s32CSCIdc[0];
    pstStaticRegCfg->s16CaYuv2RgbInDc1  = pstYuv2Rgb->s32CSCIdc[1];
    pstStaticRegCfg->s16CaYuv2RgbInDc2  = pstYuv2Rgb->s32CSCIdc[2];
    pstStaticRegCfg->s16CaYuv2RgbOutDc0 = pstYuv2Rgb->s32CSCOdc[0];
    pstStaticRegCfg->s16CaYuv2RgbOutDc1 = pstYuv2Rgb->s32CSCOdc[1];
    pstStaticRegCfg->s16CaYuv2RgbOutDc2 = pstYuv2Rgb->s32CSCOdc[2];

}

static HI_VOID CaStaticRegsInitialize(HI_U8 i, ISP_CA_STATIC_CFG_S *pstStaticRegCfg)
{
    pstStaticRegCfg->bCaLlhcProcEn  = HI_TRUE;
    pstStaticRegCfg->bCaSkinProcEn  = HI_TRUE;
    pstStaticRegCfg->bCaSatuAdjEn   = HI_TRUE;

    pstStaticRegCfg->u16CaLumaThrLow         = HI_ISP_CA_LUMA_THD_LOW_DEFAULT;
    pstStaticRegCfg->u16CaDarkChromaThrLow   = HI_ISP_CA_DARKCHROMA_THD_LOW_DEFAULT;
    pstStaticRegCfg->u16CaDarkChromaThrHigh  = HI_ISP_CA_DARKCHROMA_THD_HIGH_DEFAULT;
    pstStaticRegCfg->u16CaSDarkChromaThrLow  = HI_ISP_CA_SDARKCHROMA_THD_LOW_DEFAULT;
    pstStaticRegCfg->u16CaSDarkChromaThrHigh = HI_ISP_CA_SDARKCHROMA_THD_HIGH_DEFAULT;
    pstStaticRegCfg->u16CaLumaRatioLow       = HI_ISP_CA_LUMA_RATIO_LOW_DEFAULT;

    pstStaticRegCfg->u16CaSkinLowLumaMinU    = HI_ISP_CA_SKINLOWLUAM_UMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMaxU    = HI_ISP_CA_SKINLOWLUAM_UMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMinUy   = HI_ISP_CA_SKINLOWLUAM_UYMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMaxUy   = HI_ISP_CA_SKINLOWLUAM_UYMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMinU   = HI_ISP_CA_SKINHIGHLUAM_UMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMaxU   = HI_ISP_CA_SKINHIGHLUAM_UMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMinUy  = HI_ISP_CA_SKINHIGHLUAM_UYMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMaxUy  = HI_ISP_CA_SKINHIGHLUAM_UYMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMinV    = HI_ISP_CA_SKINLOWLUAM_VMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMaxV    = HI_ISP_CA_SKINLOWLUAM_VMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMinVy   = HI_ISP_CA_SKINLOWLUAM_VYMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinLowLumaMaxVy   = HI_ISP_CA_SKINLOWLUAM_VYMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMinV   = HI_ISP_CA_SKINHIGHLUAM_VMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMaxV   = HI_ISP_CA_SKINHIGHLUAM_VMAX_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMinVy  = HI_ISP_CA_SKINHIGHLUAM_VYMIN_DEFAULT;
    pstStaticRegCfg->u16CaSkinHighLumaMaxVy  = HI_ISP_CA_SKINHIGHLUAM_VYMAX_DEFAULT;
    pstStaticRegCfg->s16CaSkinUvDiff         = HI_ISP_CA_SKINUVDIFF_DEFAULT;
    pstStaticRegCfg->u16CaSkinRatioThrLow    = HI_ISP_CA_SKINRATIOTHD_LOW_DEFAULT;
    pstStaticRegCfg->u16CaSkinRatioThrMid    = HI_ISP_CA_SKINRATIOTHD_MID_DEFAULT;
    pstStaticRegCfg->u16CaSkinRatioThrHigh   = HI_ISP_CA_SKINRATIOTHD_HIGH_DEFAULT;

    SetColorSpaceConvertParamsDef(pstStaticRegCfg, HI_ISP_CA_CSC_TYPE_DEFAULT);

    pstStaticRegCfg->bStaticResh = HI_TRUE;

    return;
}

static HI_VOID CaUsrRegsInitialize(ISP_CA_USR_CFG_S *pstUsrRegCfg, ISP_CA_S  *pstCA)
{
    HI_U16 u16Index;

    pstUsrRegCfg->bCaCpEn           = pstCA->bCaCpEn;
    pstUsrRegCfg->u16CaLumaThrHigh  = HI_ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT;
    u16Index = (pstUsrRegCfg->u16CaLumaThrHigh >> 3);
    u16Index = (u16Index >= HI_ISP_CA_YRATIO_LUT_LENGTH) ? (HI_ISP_CA_YRATIO_LUT_LENGTH - 1) : u16Index;
    pstUsrRegCfg->u16CaLumaRatioHigh = pstCA->au32YRatioLut[u16Index];

    if (!pstCA->bCaCpEn) {
        memcpy(pstUsrRegCfg->au32YRatioLUT, pstCA->au32YRatioLut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(HI_U32));
    } else {
        memcpy(pstUsrRegCfg->au32YRatioLUT, pstCA->au32CaCpLut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(HI_U32));
    }
    pstUsrRegCfg->bCaLutUpdateEn = HI_TRUE;
    pstUsrRegCfg->bResh          = HI_TRUE;
    pstUsrRegCfg->u32UpdateIndex = 1;

    return;
}

static HI_VOID CaDynaRegsInitialize(ISP_CA_DYNA_CFG_S *pstDynaRegCfg)
{
    pstDynaRegCfg->u16CaISORatio  = 1024;
    pstDynaRegCfg->bResh          = HI_TRUE;

    return;
}

static HI_VOID CaRegsInitialize(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfg)
{
    HI_U8 i;
    ISP_CA_S  *pstCA     = HI_NULL;

    CA_GET_CTX(ViPipe, pstCA);
    ISP_CHECK_POINTER_VOID(pstCA);

    for (i = 0; i < pstRegCfg->cfg_num; i++) {
        pstRegCfg->alg_reg_cfg[i].stCaRegCfg.bCaEn = pstCA->bCaEn;

        CaStaticRegsInitialize(i, &pstRegCfg->alg_reg_cfg[i].stCaRegCfg.stStaticRegCfg);
        CaUsrRegsInitialize(&pstRegCfg->alg_reg_cfg[i].stCaRegCfg.stUsrRegCfg, pstCA);
        CaDynaRegsInitialize(&pstRegCfg->alg_reg_cfg[i].stCaRegCfg.stDynaRegCfg);
    }

    pstRegCfg->cfg_key.bit1CaCfg = 1;

    return;
}

static HI_S32 CaCheckCmosParam(VI_PIPE ViPipe, const hi_isp_cmos_ca *cmos_ca)
{
    HI_U16 i;

    ISP_CHECK_BOOL(cmos_ca->enable);
    ISP_CHECK_BOOL(cmos_ca->cp_enable);

    for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
        if (cmos_ca->y_ratio_lut[i] > HI_ISP_CA_RATIO_MAX) {
            ISP_ERR_TRACE("Invalid au16YRatioLut[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_ca->y_ratio_luty[i] > 255) {
            ISP_ERR_TRACE("Invalid au32YRatioLUTY[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_ca->y_ratio_lutu[i] > 255) {
            ISP_ERR_TRACE("Invalid au32YRatioLUTU[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_ca->y_ratio_lutv[i] > 255) {
            ISP_ERR_TRACE("Invalid au32YRatioLUTV[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (cmos_ca->iso_ratio[i] > HI_ISP_CA_RATIO_MAX) {
            ISP_ERR_TRACE("Invalid as16ISORatio[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 CaInInitialize(VI_PIPE ViPipe)
{
    HI_U16             i;
    HI_S32             s32Ret;
    HI_U32 u32LutY, u32LutU, u32LutV;
    ISP_CA_S           *pstCA     = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    CA_GET_CTX(ViPipe, pstCA);
    ISP_CHECK_POINTER(pstCA);
    isp_sensor_get_default(ViPipe, &sns_dft);

    if (sns_dft->key.bit1_ca) {
        ISP_CHECK_POINTER(sns_dft->ca);

        s32Ret = CaCheckCmosParam(ViPipe, sns_dft->ca);
        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }
        pstCA->bCaEn  = sns_dft->ca->enable;
        pstCA->bCaCpEn = sns_dft->ca->cp_enable;
        memcpy(pstCA->au32YRatioLut, sns_dft->ca->y_ratio_lut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(HI_U32));
        memcpy(pstCA->as16CaIsoRatio, sns_dft->ca->iso_ratio, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U16));

        for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
            u32LutY = sns_dft->ca->y_ratio_luty[i];
            u32LutU = sns_dft->ca->y_ratio_lutu[i];
            u32LutV = sns_dft->ca->y_ratio_lutv[i];
            pstCA->au32CaCpLut[i] = (u32LutY << 16) + (u32LutU << 8) + u32LutV;
        }
    } else {
        pstCA->bCaEn  = HI_TRUE;
        pstCA->bCaCpEn = HI_FALSE;
        memcpy(pstCA->au32YRatioLut, g_au32YRatioLut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(HI_U32));
        memcpy(pstCA->as16CaIsoRatio, g_as16ISORatio, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(HI_U16));
        memcpy(pstCA->au32CaCpLut,   g_au32CaCpLut, HI_ISP_CA_YRATIO_LUT_LENGTH * sizeof(HI_U32));
    }

    return HI_SUCCESS;
}

static HI_S32 CaReadExtregs(VI_PIPE ViPipe)
{
    HI_U16 i;
    ISP_CA_S *pstCA = HI_NULL;

    CA_GET_CTX(ViPipe, pstCA);
    ISP_CHECK_POINTER(pstCA);

    pstCA->bCaCoefUpdateEn = hi_ext_system_ca_coef_update_en_read(ViPipe);
    hi_ext_system_ca_coef_update_en_write(ViPipe, HI_FALSE);

    if (pstCA->bCaCoefUpdateEn) {
        pstCA->bCaCpEn = hi_ext_system_ca_cp_en_read(ViPipe);
        if (pstCA->bCaCpEn) {
            for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
                pstCA->au32YRatioLut[i] = hi_ext_system_ca_cp_lut_read(ViPipe, i);
            }
        } else {
            for (i = 0; i < HI_ISP_CA_YRATIO_LUT_LENGTH; i++) {
                pstCA->au32YRatioLut[i] = hi_ext_system_ca_y_ratio_lut_read(ViPipe, i);
            }
        }

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            pstCA->as16CaIsoRatio[i] = hi_ext_system_ca_iso_ratio_lut_read(ViPipe, i);
        }

        pstCA->u16LumaThdHigh     = hi_ext_system_ca_luma_thd_high_read(ViPipe);
        pstCA->s16SaturationRatio = (HI_S16)hi_ext_system_ca_saturation_ratio_read(ViPipe);
    }

    return HI_SUCCESS;
}

static HI_S32 CaGetValueFromLut(HI_S32 x, HI_S32 const *pLutX, HI_S16 *pLutY, HI_S32 length)
{
    HI_S32 n = 0;

    if (x <= pLutX[0]) {
        return pLutY[0];
    }

    for (n = 1; n < length; n++) {
        if (x <= pLutX[n]) {
            return (pLutY[n - 1] + (HI_S64)(pLutY[n] - pLutY[n - 1]) * (HI_S64)(x - pLutX[n - 1]) / DIV_0_TO_1(pLutX[n] - pLutX[n - 1]));
        }
    }

    return pLutY[length - 1];
}

static HI_BOOL __inline CheckCaOpen(ISP_CA_S *pstCA)
{
    return (pstCA->bCaEn == HI_TRUE);
}

static HI_VOID Isp_Ca_Usr_Fw(ISP_CA_S *pstCA, ISP_CA_USR_CFG_S *pstUsrRegCfg)
{
    HI_U16 j, u16Index;

    pstUsrRegCfg->bCaCpEn = pstCA->bCaCpEn;

    if (pstCA->bCaCpEn == 0) {
        for (j = 0; j < HI_ISP_CA_YRATIO_LUT_LENGTH; j++) {
            pstUsrRegCfg->au32YRatioLUT[j] = MIN2(pstCA->au32YRatioLut[j] * pstCA->s16SaturationRatio / 1000, HI_ISP_CA_RATIO_MAX); // CLIP3(,0,2047);
        }
    } else {
        for (j = 0; j < HI_ISP_CA_YRATIO_LUT_LENGTH; j++) {
            pstUsrRegCfg->au32YRatioLUT[j] = pstCA->au32YRatioLut[j];
        }
    }

    u16Index = (pstCA->u16LumaThdHigh >> 2);
    u16Index = (u16Index >= HI_ISP_CA_YRATIO_LUT_LENGTH) ? (HI_ISP_CA_YRATIO_LUT_LENGTH - 1) : u16Index;

    pstUsrRegCfg->u16CaLumaThrHigh   = pstCA->u16LumaThdHigh;
    pstUsrRegCfg->u16CaLumaRatioHigh = pstCA->au32YRatioLut[u16Index];

    pstUsrRegCfg->bCaLutUpdateEn     = HI_TRUE;

    pstUsrRegCfg->bResh              = HI_TRUE;
    pstUsrRegCfg->u32UpdateIndex    += 1;

    return;
}

static HI_VOID Isp_Ca_Dyna_Fw(HI_S32 s32Iso, ISP_CA_DYNA_CFG_S *pstDynaRegCfg, ISP_CA_S *pstCA)
{
    HI_S32 s32IsoRatio;

    s32IsoRatio = CaGetValueFromLut(s32Iso, g_as32CAIsoLut, pstCA->as16CaIsoRatio, ISP_AUTO_ISO_STRENGTH_NUM);

    pstDynaRegCfg->u16CaISORatio = CLIP3(s32IsoRatio, 0, HI_ISP_CA_RATIO_MAX);
    pstDynaRegCfg->bResh         = HI_TRUE;

    return;
}

static HI_S32 ISP_CaInit(VI_PIPE ViPipe, HI_VOID *pRegCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    isp_reg_cfg *pstRegCfg = (isp_reg_cfg *)pRegCfg;

    s32Ret = CaCtxInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = CaInInitialize(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    CaRegsInitialize(ViPipe, pstRegCfg);
    CaExtRegsInitialize(ViPipe);

    return HI_SUCCESS;
}

static HI_S32 ISP_CaRun(VI_PIPE ViPipe, const HI_VOID *pStatInfo,
                        HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_U8  i;
    ISP_CA_S  *pstCA = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_reg_cfg *pstReg = (isp_reg_cfg *)pRegCfg;

    CA_GET_CTX(ViPipe, pstCA);
    ISP_CHECK_POINTER(pstCA);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    /* calculate every two interrupts */
    if ((pstIspCtx->frame_cnt % 2 != 0) && (pstIspCtx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    pstCA->bCaEn = hi_ext_system_ca_en_read(ViPipe);

    for (i = 0; i < pstReg->cfg_num; i++) {
        pstReg->alg_reg_cfg[i].stCaRegCfg.bCaEn = pstCA->bCaEn;
    }

    pstReg->cfg_key.bit1CaCfg = 1;

    /* check hardware setting */
    if (!CheckCaOpen(pstCA)) {
        return HI_SUCCESS;
    }

    CaReadExtregs(ViPipe);

    if (pstCA->bCaCoefUpdateEn) {
        for (i = 0; i < pstReg->cfg_num; i++) {
            Isp_Ca_Usr_Fw(pstCA, &pstReg->alg_reg_cfg[i].stCaRegCfg.stUsrRegCfg);
        }
    }

    for (i = 0; i < pstReg->cfg_num; i++) {
        Isp_Ca_Dyna_Fw((HI_S32)pstIspCtx->linkage.iso, &pstReg->alg_reg_cfg[i].stCaRegCfg.stDynaRegCfg, pstCA);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_CaCtrl(VI_PIPE ViPipe, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd) {
        default :
            break;
    }
    return HI_SUCCESS;
}

static HI_S32 ISP_CaExit(VI_PIPE ViPipe)
{
    HI_U8 i;

    isp_reg_cfg_attr  *pRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->reg_cfg.cfg_num; i++) {
        pRegCfg->reg_cfg.alg_reg_cfg[i].stCaRegCfg.bCaEn = HI_FALSE;
    }

    pRegCfg->reg_cfg.cfg_key.bit1CaCfg = 1;

    CaCtxExit(ViPipe);

    return HI_SUCCESS;
}

HI_S32 isp_alg_register_ca(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_alg_node *pstAlgs = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->algs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->alg_type = ISP_ALG_CA;
    pstAlgs->alg_func.pfn_alg_init = ISP_CaInit;
    pstAlgs->alg_func.pfn_alg_run  = ISP_CaRun;
    pstAlgs->alg_func.pfn_alg_ctrl = ISP_CaCtrl;
    pstAlgs->alg_func.pfn_alg_exit = ISP_CaExit;
    pstAlgs->used = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
