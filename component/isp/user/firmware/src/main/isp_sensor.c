/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <unistd.h>
#include "isp_sensor.h"
#include "yuv_cmos_ex.h"
#include "hi_comm_sns_adapt.h"
#include "hi_osal.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    ISP_SNS_ATTR_INFO_S     stSnsAttrInfo;
    ISP_SENSOR_REGISTER_S   stRegister;
    hi_isp_cmos_default     sns_dft;
    hi_isp_cmos_black_level sns_black_level;    /* some sensors's black level will be changed with iso */
    ISP_SNS_REGS_INFO_S     stSnsRegInfo;
    ISP_CMOS_SENSOR_IMAGE_MODE_S stSnsImageMode;
} isp_sensor_ctx;

isp_sensor_ctx *g_pastSensorCtx[ISP_MAX_PIPE_NUM] = { HI_NULL };

#define SENSOR_GET_CTX(dev, ctx) (ctx = g_pastSensorCtx[dev])
#define SENSOR_SET_CTX(dev, ctx) (g_pastSensorCtx[dev] = ctx)
#define SENSOR_RESET_CTX(dev)       (g_pastSensorCtx[dev] = HI_NULL)

HI_S32 ISP_SensorCtxInit(VI_PIPE vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);

    if (sensor_ctx == HI_NULL) {
        sensor_ctx = (isp_sensor_ctx *)ISP_MALLOC(sizeof(isp_sensor_ctx));
        if (sensor_ctx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] SensorCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(sensor_ctx, 0, sizeof(isp_sensor_ctx));

    SENSOR_SET_CTX(vi_pipe, sensor_ctx);

    return HI_SUCCESS;
}

HI_S32 ISP_SensorRegCallBack(VI_PIPE vi_pipe, ISP_SNS_ATTR_INFO_S *sns_attr_info, ISP_SENSOR_REGISTER_S *sns_register)
{
    hi_s32 ret = HI_SUCCESS;
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    ret = ISP_SensorCtxInit(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    memcpy(&sensor_ctx->stSnsAttrInfo, sns_attr_info, sizeof(ISP_SNS_ATTR_INFO_S));
    memcpy(&sensor_ctx->stRegister, sns_register, sizeof(ISP_SENSOR_REGISTER_S));

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_sensor_global_init != HI_NULL) {
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_sensor_global_init(vi_pipe);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorUnRegCallBack(VI_PIPE vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_FREE(sensor_ctx);
    SENSOR_RESET_CTX(vi_pipe);

    return HI_SUCCESS;
}

HI_S32 ISP_SensorUpdateAll(VI_PIPE vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    HI_ASSERT(sizeof(ISP_CMOS_DEFAULT_S) == sizeof(hi_isp_cmos_default));
    HI_ASSERT(sizeof(ISP_CMOS_DRC_S) == sizeof(hi_isp_cmos_drc));
    HI_ASSERT(sizeof(ISP_CMOS_DEMOSAIC_S) == sizeof(hi_isp_cmos_demosaic));
    HI_ASSERT(sizeof(ISP_CMOS_PREGAMMA_S) == sizeof(hi_isp_cmos_pregamma));
    HI_ASSERT(sizeof(ISP_CMOS_GAMMA_S) == sizeof(hi_isp_cmos_gamma));
    HI_ASSERT(sizeof(ISP_CMOS_SHARPEN_S) == sizeof(hi_isp_cmos_sharpen));
    HI_ASSERT(sizeof(ISP_CMOS_LDCI_S) == sizeof(hi_isp_cmos_ldci));
    HI_ASSERT(sizeof(ISP_CMOS_DPC_S) == sizeof(hi_isp_cmos_dpc));
    HI_ASSERT(sizeof(ISP_CMOS_LSC_S) == sizeof(hi_isp_cmos_lsc));
    HI_ASSERT(sizeof(ISP_CMOS_GE_S) == sizeof(hi_isp_cmos_ge));
    HI_ASSERT(sizeof(ISP_CMOS_ANTIFALSECOLOR_S) == sizeof(hi_isp_cmos_anti_false_color));
    HI_ASSERT(sizeof(ISP_CMOS_BAYERNR_S) == sizeof(hi_isp_cmos_bayernr));
    HI_ASSERT(sizeof(ISP_CMOS_CA_S) == sizeof(hi_isp_cmos_ca));
    HI_ASSERT(sizeof(ISP_CMOS_WDR_S) == sizeof(hi_isp_cmos_wdr));
    HI_ASSERT(sizeof(ISP_CMOS_BLACK_LEVEL_S) == sizeof(hi_isp_cmos_black_level));

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_isp_default != HI_NULL) {
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_isp_default(vi_pipe, (ISP_CMOS_DEFAULT_S *)(&sensor_ctx->sns_dft));
    } else {
        ISP_ERR_TRACE("Get isp[%d] default value error!\n", vi_pipe);
        return HI_FAILURE;
    }

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_isp_black_level != HI_NULL) {
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_isp_black_level(vi_pipe, (ISP_CMOS_BLACK_LEVEL_S *)(&sensor_ctx->sns_black_level));
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorUpdateAll_YUV(VI_PIPE vi_pipe)
{
    hi_s32 ret = HI_SUCCESS;
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    ret = ISP_GetYUVDefault((ISP_CMOS_DEFAULT_S *)(&sensor_ctx->sns_dft));

    sensor_ctx->sns_black_level.black_level[0] = 257;
    sensor_ctx->sns_black_level.black_level[1] = 257;
    sensor_ctx->sns_black_level.black_level[2] = 257;
    sensor_ctx->sns_black_level.black_level[3] = 257;
    sensor_ctx->sns_black_level.update = HI_TRUE;

    return ret;
}

HI_S32 ISP_SensorGetId(VI_PIPE vi_pipe, SENSOR_ID *sensor_id)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    *sensor_id = sensor_ctx->stSnsAttrInfo.eSensorId;

    return HI_SUCCESS;
}

HI_S32 isp_sensor_get_blc(VI_PIPE vi_pipe, hi_isp_cmos_black_level **sns_black_level)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    ISP_CHECK_PIPE(vi_pipe);
    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    *sns_black_level = &sensor_ctx->sns_black_level;

    return HI_SUCCESS;
}

HI_S32 isp_sensor_get_default(VI_PIPE vi_pipe, hi_isp_cmos_default **sns_dft)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    ISP_CHECK_PIPE(vi_pipe);
    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    *sns_dft = &sensor_ctx->sns_dft;

    return HI_SUCCESS;
}

HI_S32 ISP_SensorGetMaxResolution(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_MAX_RESOLUTION_S **sns_max_resolution)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    *sns_max_resolution = (ISP_CMOS_SENSOR_MAX_RESOLUTION_S *)(&sensor_ctx->sns_dft.sensor_max_resolution);

    return HI_SUCCESS;
}

HI_S32 ISP_SensorGetSnsReg(VI_PIPE vi_pipe, ISP_SNS_REGS_INFO_S **sns_reg_info)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    *sns_reg_info = &sensor_ctx->stSnsRegInfo;

    return HI_SUCCESS;
}

HI_S32 ISP_SensorInit(VI_PIPE vi_pipe)
{
    hi_s8 ssp_dev;
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    /* if I2C or SSP Dev is -1, don't init sensor */
    {
        ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;

        ISP_SensorUpdateSnsReg(vi_pipe);
        ISP_SensorGetSnsReg(vi_pipe, &pstSnsRegsInfo);
        ssp_dev = pstSnsRegsInfo->unComBus.s8SspDev.bit4SspDev;

        if ((pstSnsRegsInfo->enSnsType == ISP_SNS_I2C_TYPE) &&
            (pstSnsRegsInfo->unComBus.s8I2cDev == -1)) {
            return HI_SUCCESS;
        }

        if ((pstSnsRegsInfo->enSnsType == ISP_SNS_SSP_TYPE) &&
            (ssp_dev == -1)) {
            return HI_SUCCESS;
        }
    }

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_sensor_init != HI_NULL) {
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_sensor_init(vi_pipe);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorSwitch(VI_PIPE vi_pipe)
{
    hi_s8 ssp_dev;
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

  /* if I2C or SSP Dev is -1, don't init sensor */
    {
        ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;

        ISP_SensorUpdateSnsReg(vi_pipe);
        ISP_SensorGetSnsReg(vi_pipe, &pstSnsRegsInfo);
        ssp_dev = pstSnsRegsInfo->unComBus.s8SspDev.bit4SspDev;

        if ((pstSnsRegsInfo->enSnsType == ISP_SNS_I2C_TYPE) &&
            (pstSnsRegsInfo->unComBus.s8I2cDev == -1)) {
            return HI_SUCCESS;
        }

        if ((pstSnsRegsInfo->enSnsType == ISP_SNS_SSP_TYPE) &&
            (ssp_dev == -1)) {
            return HI_SUCCESS;
        }
    }

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_sensor_init != HI_NULL) {
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_sensor_init(vi_pipe);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorExit(VI_PIPE vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_sensor_exit != HI_NULL) {
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_sensor_exit(vi_pipe);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 isp_sensor_update_blc(VI_PIPE vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_isp_black_level != HI_NULL) {
        /* sensor should record the present iso, and calculate new black level. */
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_isp_black_level(vi_pipe, (ISP_CMOS_BLACK_LEVEL_S *)(&sensor_ctx->sns_black_level));
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorUpdateDefault(VI_PIPE vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_isp_default != HI_NULL) {
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_isp_default(vi_pipe, (ISP_CMOS_DEFAULT_S *)(&sensor_ctx->sns_dft));
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorSetWDRMode(VI_PIPE vi_pipe, hi_u8 mode)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_set_wdr_mode != HI_NULL) {
        if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_set_wdr_mode(vi_pipe, mode) != HI_SUCCESS) {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorSetImageMode(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *sns_image_mode)
{
    isp_sensor_ctx *sensor_ctx = NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_set_image_mode != HI_NULL) {
        return sensor_ctx->stRegister.stSnsExp.pfn_cmos_set_image_mode(vi_pipe, sns_image_mode);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorSetPixelDetect(VI_PIPE vi_pipe, hi_bool enable)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_set_pixel_detect != HI_NULL) {
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_set_pixel_detect(vi_pipe, enable);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 isp_sensor_get_awb_gain(VI_PIPE vi_pipe, hi_u32 *sensor_awb_gain)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_awb_gains != HI_NULL) {
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_awb_gains(vi_pipe, sensor_awb_gain);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
HI_S32 ISP_SensorUpdateSnsReg(VI_PIPE vi_pipe)
{
    isp_sensor_ctx *sensor_ctx = HI_NULL;

    SENSOR_GET_CTX(vi_pipe, sensor_ctx);
    ISP_CHECK_POINTER(sensor_ctx);

    if (sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_sns_reg_info != HI_NULL) {
        sensor_ctx->stRegister.stSnsExp.pfn_cmos_get_sns_reg_info(vi_pipe, &sensor_ctx->stSnsRegInfo);
    } else {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
