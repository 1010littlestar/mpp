/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#include "isp_sensor.h"
#include "isp_main.h"
#include "isp_alg.h"
#include "isp_dnginfo.h"
#include "isp_ext_config.h"
#include "hi_isp_defines.h"

extern HI_S32 g_as32IspFd[ISP_MAX_PIPE_NUM];

HI_S32 ISP_DngInfoInit(VI_PIPE ViPipe)
{
    HI_U64 u64PhyAddr;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->isp_trans_info.dng_info.phy_addr;

    pstIspCtx->dng_info_ctrl.isp_dng = HI_MPI_SYS_Mmap(u64PhyAddr, sizeof(hi_isp_dng_image_static_info));

    if (pstIspCtx->dng_info_ctrl.isp_dng == HI_NULL) {
        ISP_ERR_TRACE("isp[%d] mmap Dng info buf failed!\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DngInfoExit(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->dng_info_ctrl.isp_dng != HI_NULL) {
        HI_MPI_SYS_Munmap(pstIspCtx->dng_info_ctrl.isp_dng, sizeof(hi_isp_dng_image_static_info));
        pstIspCtx->dng_info_ctrl.isp_dng = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_BOOL ISP_DngColorParamCheck(isp_dng_ccm *pstDngCcm, isp_dng_ccm *pstPreDngCcm)
{
    HI_BOOL bChanged = HI_FALSE;
    HI_U8 i;

    for (i = 0; i < 9; i++) {
        if (pstDngCcm->low_ccm[i] != pstPreDngCcm->low_ccm[i]) {
            bChanged = HI_TRUE;
            return bChanged;
        }
    }

    for (i = 0; i < 9; i++) {
        if (pstDngCcm->high_ccm[i] != pstPreDngCcm->high_ccm[i]) {
            bChanged = HI_TRUE;
            return bChanged;
        }
    }

    if (pstDngCcm->high_color_temp!= pstPreDngCcm->high_color_temp) {
        bChanged = HI_TRUE;
        return bChanged;
    }

    if (pstDngCcm->low_color_temp!= pstPreDngCcm->low_color_temp) {
        bChanged = HI_TRUE;
        return bChanged;
    }

    return bChanged;
}

void ISP_CcmDataFormat(HI_U16 *pu16CcmIn, HI_DOUBLE *pdCcmOut)
{
    HI_U8 i;
    HI_S16 tmp;
    for (i = 0; i < 9; i++) {
        tmp = (HI_S16)pu16CcmIn[i];
        pdCcmOut[i] = (HI_DOUBLE)tmp / 256;
    }
}

const double kNearZero = 1.0E-10;

static const double SRGB_to_XYZD50[9] = { 0.4361, 0.3851, 0.1431, 0.2225, 0.7169, 0.0606, 0.0139, 0.0971, 0.7141 };
static const double SRGB_to_XYZA[9] = { 0.4969, 0.4388, 0.1630, 0.2225, 0.7169, 0.0606, 0.0060, 0.0419, 0.3080 };

static const double XYZD50_to_sRGB[9] = { 3.1340, -1.6169, -0.4907, -0.9784, 1.9159, 0.0334, 0.0720, -0.2290, 1.4049 };

double Abs_double(double x)
{
    return (x < 0.0 ? -x : x);
}

void Invert3by3(const double *MatrixA, double *InvMatrix)
{
    double a00 = MatrixA[0];
    double a01 = MatrixA[1];
    double a02 = MatrixA[2];
    double a10 = MatrixA[3];
    double a11 = MatrixA[4];
    double a12 = MatrixA[5];
    double a20 = MatrixA[6];
    double a21 = MatrixA[7];
    double a22 = MatrixA[8];
    double temp[9];
    double det;
    int i;

    temp[0] = a11 * a22 - a21 * a12;
    temp[1] = a21 * a02 - a01 * a22;
    temp[2] = a01 * a12 - a11 * a02;
    temp[3] = a20 * a12 - a10 * a22;
    temp[4] = a00 * a22 - a20 * a02;
    temp[5] = a10 * a02 - a00 * a12;
    temp[6] = a10 * a21 - a20 * a11;
    temp[7] = a20 * a01 - a00 * a21;
    temp[8] = a00 * a11 - a10 * a01;

    det = (a00 * temp[0] +
           a01 * temp[3] +
           a02 * temp[6]);

    if (Abs_double(det) < kNearZero) {
        return;
    }

    for (i = 0; i < 3 * 3; i++) {
        InvMatrix[i] = temp[i] / det;
    }

    return;
}

void MultiMatrix3x3(const double *MatrixA, const double *MatrixB, double *MutMatrix)
{
    int i, j, k;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            double temp = 0;

            for (k = 0; k < 3; ++k) {
                temp += MatrixA[i * 3 + k] * MatrixB[k * 3 + j];
            }
            MutMatrix[i * 3 + j] = temp;
        }
    }
}

void ISP_Dng_LightSource_Checker(HI_U16 u16ColorTmep, HI_U8 *pu8LightSource)
{
    /* Light source, actually this means white balance setting. '0' means unknown, '1' daylight, '2'
           fluorescent, '3' tungsten, '10' flash, '17' standard light A, '18' standard light B, '19' standard light
           C, '20' D55, '21' D65, '22' D75, '255' other */
    if (u16ColorTmep >= (7500 - 500)) { /* D75 7500 */
        *pu8LightSource = 22;
    } else if ((u16ColorTmep < (6500 + 500)) && (u16ColorTmep >= (6500 - 500))) { /* D65 6500 */
        *pu8LightSource = 21;
    } else if ((u16ColorTmep < (5500 + 500)) && (u16ColorTmep >= (5500 - 250))) { /* D55 5500 */
        *pu8LightSource = 20;
    } else if ((u16ColorTmep < (5000 + 250)) && (u16ColorTmep >= (5000 - 100))) { /* daylight 5000 */
        *pu8LightSource = 23;
    } else if ((u16ColorTmep < (4800 + 100)) && (u16ColorTmep >= (4800 - 550))) { /* B 4800 */
        *pu8LightSource = 18;
    } else if ((u16ColorTmep < (4000 + 250)) && (u16ColorTmep >= (4000 - 800))) { /* fluorescent 4000 */
        *pu8LightSource = 2;
    } else if (u16ColorTmep < (2800 + 400)) { /* A 2800 */
        *pu8LightSource = 17;
    }
}

HI_VOID ISP_DngExtRead(VI_PIPE ViPipe, hi_isp_dng_color_param *pstDngColorParam)
{
    pstDngColorParam->wb_gain1.r_gain = hi_ext_system_dng_high_wb_gain_r_read(ViPipe);
    pstDngColorParam->wb_gain1.g_gain = hi_ext_system_dng_high_wb_gain_g_read(ViPipe);
    pstDngColorParam->wb_gain1.b_gain = hi_ext_system_dng_high_wb_gain_b_read(ViPipe);

    pstDngColorParam->wb_gain2.r_gain = hi_ext_system_dng_low_wb_gain_r_read(ViPipe);
    pstDngColorParam->wb_gain2.g_gain = hi_ext_system_dng_low_wb_gain_g_read(ViPipe);
    pstDngColorParam->wb_gain2.b_gain = hi_ext_system_dng_low_wb_gain_b_read(ViPipe);
}

HI_S32 ISP_DngColorParamUpdate(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    AWB_CCM_CONFIG_S stAwbCcmConfig = { 0 };
    isp_dng_ccm stDngCcm = { 0 };
    HI_DOUBLE adHiD50Ccm[9] = { 0 }, adHiACcm[9] = { 0 };
    HI_DOUBLE adHiD50Wbgain[9] = { 0 }, adHiAWbgain[9] = { 0 };
    HI_U16 au16HiD50Wbgain[9] = { 0 }, au16HiAWbgain[9] = { 0 };
    HI_DOUBLE adAMultMatrix[9] = { 0 }, adD50MultMatrix[9] = { 0 };
    HI_DOUBLE adInvAColorMatrix[9] = { 0 }, adInvD50ColorMatrix[9] = { 0 };
    HI_DOUBLE adAColorMatrix[9] = { 0 }, adD50ColorMatrix[9] = { 0 };
    HI_DOUBLE adAForwardMatrix[9] = { 0 }, adD50ForwardMatrix[9] = { 0 };
    hi_isp_dng_color_param stDngColorParam = { 0 };
    HI_U8 i;
    VI_PIPE AWB_DEV = (ViPipe ? 0 : 1);

    HI_BOOL bChanged = HI_FALSE;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_DngExtRead(ViPipe, &stDngColorParam);

    /* DNG color param have not set */
    if (stDngColorParam.wb_gain1.g_gain == 0) {
        return HI_SUCCESS;
    }

    memset(au16HiAWbgain, 0, sizeof(HI_U16) * 9);
    memset(au16HiD50Wbgain, 0, sizeof(HI_U16) * 9);

    /* get CCM from AWB libiray */
    switch (pstIspCtx->linkage.snap_type) {
        default:

        case SNAP_TYPE_NORMAL:
        case SNAP_TYPE_PRO:
            if ((pstIspCtx->linkage.snap_pipe_mode == ISP_SNAP_PICTURE) && (pstIspCtx->linkage.load_ccm == HI_TRUE)) {
                ISP_AlgsCtrl(pstIspCtx->algs, AWB_DEV, AWB_CCM_CONFIG_GET, (HI_VOID *)&stAwbCcmConfig);
            } else {
                ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, AWB_CCM_CONFIG_GET, (HI_VOID *)&stAwbCcmConfig);
            }
            break;
    }

    memcpy(&stDngCcm.low_ccm, &stAwbCcmConfig.au16LowCCM, sizeof(HI_U16) * 9);
    memcpy(&stDngCcm.high_ccm, &stAwbCcmConfig.au16HighCCM, sizeof(HI_U16) * 9);
    stDngCcm.high_color_temp = stAwbCcmConfig.u16HighColorTemp;
    stDngCcm.low_color_temp  = stAwbCcmConfig.u16LowColorTemp;

    /* if CCM or WB gain changed, recaculate color parameters */
    bChanged = ISP_DngColorParamCheck(&stDngCcm, &pstIspCtx->pre_dng_ccm);
    if (!bChanged) {
        if ((pstIspCtx->pre_dng_color_param.wb_gain1.r_gain != stDngColorParam.wb_gain1.r_gain) ||
            (pstIspCtx->pre_dng_color_param.wb_gain1.g_gain != stDngColorParam.wb_gain1.g_gain) ||
            (pstIspCtx->pre_dng_color_param.wb_gain1.b_gain != stDngColorParam.wb_gain1.b_gain) ||
            (pstIspCtx->pre_dng_color_param.wb_gain2.r_gain != stDngColorParam.wb_gain2.r_gain) ||
            (pstIspCtx->pre_dng_color_param.wb_gain2.g_gain != stDngColorParam.wb_gain2.g_gain) ||
            (pstIspCtx->pre_dng_color_param.wb_gain2.b_gain != stDngColorParam.wb_gain2.b_gain)) {
            bChanged = HI_TRUE;
        }
    }
    /* save last CCM and WB gain */
    memcpy(&pstIspCtx->pre_dng_ccm, &stDngCcm, sizeof(isp_dng_ccm));
    memcpy(&pstIspCtx->pre_dng_color_param, &stDngColorParam, sizeof(hi_isp_dng_color_param));

    if (bChanged == HI_TRUE) {
        /* data format */
        au16HiAWbgain[0] = stDngColorParam.wb_gain1.r_gain;
        au16HiAWbgain[4] = stDngColorParam.wb_gain1.g_gain;
        au16HiAWbgain[8] = stDngColorParam.wb_gain1.b_gain;
        au16HiD50Wbgain[0] = stDngColorParam.wb_gain2.r_gain;
        au16HiD50Wbgain[4] = stDngColorParam.wb_gain2.g_gain;
        au16HiD50Wbgain[8] = stDngColorParam.wb_gain2.b_gain;
        ISP_CcmDataFormat(stDngCcm.low_ccm, adHiACcm);
        ISP_CcmDataFormat(stDngCcm.high_ccm, adHiD50Ccm);
        ISP_CcmDataFormat(au16HiAWbgain, adHiAWbgain);
        ISP_CcmDataFormat(au16HiD50Wbgain, adHiD50Wbgain);

        /* calculate ColorMatrix1 */
        MultiMatrix3x3(adHiACcm, adHiAWbgain, adAMultMatrix);
        MultiMatrix3x3(SRGB_to_XYZA, adAMultMatrix, adInvAColorMatrix);
        Invert3by3(adInvAColorMatrix, adAColorMatrix);

        /* calculate ColorMatrix2 */
        MultiMatrix3x3(adHiD50Ccm, adHiD50Wbgain, adD50MultMatrix);
        MultiMatrix3x3(SRGB_to_XYZD50, adD50MultMatrix, adInvD50ColorMatrix);
        Invert3by3(adInvD50ColorMatrix, adD50ColorMatrix);

        /* calculate ForwardMatrix1 */
        Invert3by3(XYZD50_to_sRGB, adInvAColorMatrix);
        MultiMatrix3x3(adInvAColorMatrix, adHiACcm, adAForwardMatrix);

        /* calculate ForwardMatrix2 */
        Invert3by3(XYZD50_to_sRGB, adInvD50ColorMatrix);
        MultiMatrix3x3(adInvD50ColorMatrix, adHiD50Ccm, adD50ForwardMatrix);
        for (i = 0; i < 9; i++) {
            pstIspCtx->dng_info_ctrl.isp_dng->color_matrix1[i].numerator = (HI_S32)(adAColorMatrix[i] * 1000000);
            pstIspCtx->dng_info_ctrl.isp_dng->color_matrix1[i].denominator = 1000000;

            pstIspCtx->dng_info_ctrl.isp_dng->color_matrix2[i].numerator = (HI_S32)(adD50ColorMatrix[i] * 1000000);
            pstIspCtx->dng_info_ctrl.isp_dng->color_matrix2[i].denominator = 1000000;

            pstIspCtx->dng_info_ctrl.isp_dng->forwad_matrix1[i].numerator = (HI_S32)(adAForwardMatrix[i] * 1000000);
            pstIspCtx->dng_info_ctrl.isp_dng->forwad_matrix1[i].denominator = 1000000;

            pstIspCtx->dng_info_ctrl.isp_dng->forwad_matrix2[i].numerator= (HI_S32)(adD50ForwardMatrix[i] * 1000000);
            pstIspCtx->dng_info_ctrl.isp_dng->forwad_matrix2[i].denominator= 1000000;

            pstIspCtx->dng_info_ctrl.isp_dng->camera_calibration1[i].numerator = 0;
            pstIspCtx->dng_info_ctrl.isp_dng->camera_calibration1[i].denominator = 1000000;

            pstIspCtx->dng_info_ctrl.isp_dng->camera_calibration2[i].numerator = 0;
            pstIspCtx->dng_info_ctrl.isp_dng->camera_calibration2[i].denominator = 1000000;
        }
        pstIspCtx->dng_info_ctrl.isp_dng->camera_calibration1[0].numerator = 1000000;
        pstIspCtx->dng_info_ctrl.isp_dng->camera_calibration1[4].numerator = 1000000;
        pstIspCtx->dng_info_ctrl.isp_dng->camera_calibration1[8].numerator = 1000000;

        pstIspCtx->dng_info_ctrl.isp_dng->camera_calibration2[0].numerator = 1000000;
        pstIspCtx->dng_info_ctrl.isp_dng->camera_calibration2[4].numerator = 1000000;
        pstIspCtx->dng_info_ctrl.isp_dng->camera_calibration2[8].numerator = 1000000;

        ISP_Dng_LightSource_Checker(stDngCcm.low_color_temp, &pstIspCtx->dng_info_ctrl.isp_dng->calibration_illuminant1);
        ISP_Dng_LightSource_Checker(stDngCcm.high_color_temp, &pstIspCtx->dng_info_ctrl.isp_dng->calibration_illuminant2);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_UpdateDngImageDynamicInfo(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    hi_dng_image_dynamic_info stDngImageDynamicInfo;
    HI_U8 i;
    HI_S32 s32Ret;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    isp_sensor_get_blc(ViPipe, &sns_black_level);

    if (sns_black_level->update == HI_TRUE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            stDngImageDynamicInfo.black_level[i] = sns_black_level->black_level[i];
        }
    } else {
        stDngImageDynamicInfo.black_level[0] = hi_ext_system_black_level_query_00_read(ViPipe);
        stDngImageDynamicInfo.black_level[1] = hi_ext_system_black_level_query_01_read(ViPipe);
        stDngImageDynamicInfo.black_level[2] = hi_ext_system_black_level_query_10_read(ViPipe);
        stDngImageDynamicInfo.black_level[3] = hi_ext_system_black_level_query_11_read(ViPipe);
    }

    stDngImageDynamicInfo.as_shot_neutral[0].denominator = MAX2(pstIspCtx->linkage.white_balance_gain[0], 1);
    stDngImageDynamicInfo.as_shot_neutral[0].numerator = pstIspCtx->linkage.white_balance_gain[1];
    stDngImageDynamicInfo.as_shot_neutral[1].denominator = MAX2(pstIspCtx->linkage.white_balance_gain[1], 1);
    stDngImageDynamicInfo.as_shot_neutral[1].numerator = pstIspCtx->linkage.white_balance_gain[1];
    stDngImageDynamicInfo.as_shot_neutral[2].denominator = MAX2(pstIspCtx->linkage.white_balance_gain[3], 1);
    stDngImageDynamicInfo.as_shot_neutral[2].numerator = pstIspCtx->linkage.white_balance_gain[1];

    stDngImageDynamicInfo.ad_noise_profile[0] = 2.0E-5;
    stDngImageDynamicInfo.ad_noise_profile[1] = 4.5E-7;
    stDngImageDynamicInfo.ad_noise_profile[2] = 2.0E-5;
    stDngImageDynamicInfo.ad_noise_profile[3] = 4.5E-7;
    stDngImageDynamicInfo.ad_noise_profile[4] = 2.0E-5;
    stDngImageDynamicInfo.ad_noise_profile[5] = 4.5E-7;

    ISP_DngColorParamUpdate(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_DNG_INFO_SET, &stDngImageDynamicInfo);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}
