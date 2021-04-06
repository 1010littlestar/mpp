/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#ifndef __HIFB_COEF_ORG_H__
#define __HIFB_COEF_ORG_H__

#include "hifb_coef.h"

/* RGB->YUV601 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_RGB2YUV601_tv;
/* RGB->YUV601 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_RGB2YUV601_pc;
/* RGB->YUV709 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_RGB2YUV709_tv;
/* RGB->YUV709 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_RGB2YUV709_pc;
/* RGB->YUV2020 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_RGB2YUV2020_pc;
/* YUV601->RGB Constant coefficient matrix */
extern const CscCoef_S g_stCSC_YUV6012RGB_pc;
/* YUV709->RGB Constant coefficient matrix */
extern const CscCoef_S g_stCSC_YUV7092RGB_pc;
/* YUV2020->RGB Constant coefficient matrix */
extern const CscCoef_S g_stCSC_YUV20202RGB_pc;
/* YUV601->YUV709 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_YUV2YUV_601_709;
/* YUV709->YUV601 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_YUV2YUV_709_601;
/* YUV601->YUV709 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_Init;

#endif
