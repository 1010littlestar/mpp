/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <string.h>
#include <stdio.h>

#include "hi_comm_isp.h"
#include "hi_comm_3a.h"
#include "hi_ae_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 SAMPLE_HI_MPI_AE_Register(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib);
HI_S32 SAMPLE_HI_MPI_AE_UnRegister(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib);

/* The callback function of sensor register to ae lib. */
HI_S32 SAMPLE_HI_MPI_AE_SensorRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
                                          AE_SENSOR_REGISTER_S *pstRegister);
HI_S32 SAMPLE_HI_MPI_AE_SensorUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, SENSOR_ID SensorId);

HI_S32 SAMPLE_HI_MPI_ISP_SetExposureAttr(VI_PIPE ViPipe, const ISP_EXPOSURE_ATTR_S *pstExpAttr);
HI_S32 SAMPLE_HI_MPI_ISP_GetExposureAttr(VI_PIPE ViPipe, ISP_EXPOSURE_ATTR_S *pstExpAttr);

HI_S32 SAMPLE_HI_MPI_ISP_SetWDRExposureAttr(VI_PIPE ViPipe, const ISP_WDR_EXPOSURE_ATTR_S *pstWDRExpAttr);
HI_S32 SAMPLE_HI_MPI_ISP_GetWDRExposureAttr(VI_PIPE ViPipe, ISP_WDR_EXPOSURE_ATTR_S *pstWDRExpAttr);

HI_S32 SAMPLE_HI_MPI_ISP_SetAERouteAttr(VI_PIPE ViPipe, const ISP_AE_ROUTE_S *pstAERouteAttr);
HI_S32 SAMPLE_HI_MPI_ISP_GetAERouteAttr(VI_PIPE ViPipe, ISP_AE_ROUTE_S *pstAERouteAttr);

HI_S32 SAMPLE_HI_MPI_ISP_QueryExposureInfo(VI_PIPE ViPipe, ISP_EXP_INFO_S *pstExpInfo);

HI_S32 SAMPLE_HI_MPI_ISP_SetIrisAttr(VI_PIPE ViPipe, const ISP_IRIS_ATTR_S *pstIrisAttr);
HI_S32 SAMPLE_HI_MPI_ISP_GetIrisAttr(VI_PIPE ViPipe, ISP_IRIS_ATTR_S *pstIrisAttr);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
