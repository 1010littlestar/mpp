/*
*  Copyright (C),2013-2019, Hisilicon Tech. Co., Ltd.
*  Version       : Initial Draft
*  Author        : ISP_SW
*  Created       : 2013/11/07
*  Description   :
*  History       :
*  1.Date        : 2013/11/07
*  Modification: Created file
*/

#ifndef __MPI_AE_ADAPT_H__
#define __MPI_AE_ADAPT_H__

#include "hi_comm_isp_adapt.h"
#include "hi_comm_3a_adapt.h"
#include "hi_ae_comm_adapt.h"
#include "hi_comm_sns_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#if 0
/* The interface of ae lib register to isp. */
MPI_STATIC hi_s32 hi_mpi_ae_register(VI_PIPE vi_pipe, hi_isp_alg_lib *ae_lib);
MPI_STATIC hi_s32 hi_mpi_ae_unregister(VI_PIPE vi_pipe, hi_isp_alg_lib *ae_lib);

/* The callback function of sensor register to ae lib. */
MPI_STATIC hi_s32 hi_mpi_ae_sensor_reg_callback(VI_PIPE vi_pipe, hi_isp_alg_lib *ae_lib, hi_isp_sns_attr_info *sns_attr_info,
                                                hi_isp_ae_sensor_register *sns_register);
MPI_STATIC hi_s32 hi_mpi_ae_sensor_unreg_callback(VI_PIPE vi_pipe, hi_isp_alg_lib *ae_lib, SENSOR_ID sensor_id);
#endif
MPI_STATIC hi_s32 hi_mpi_isp_set_exposure_attr(VI_PIPE vi_pipe, const hi_isp_exposure_attr *exp_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_exposure_attr(VI_PIPE vi_pipe, hi_isp_exposure_attr *exp_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_wdr_exposure_attr(VI_PIPE vi_pipe, const hi_isp_wdr_exposure_attr *wdr_exp_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_wdr_exposure_attr(VI_PIPE vi_pipe, hi_isp_wdr_exposure_attr *wdr_exp_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_hdr_exposure_attr(VI_PIPE vi_pipe, const hi_isp_hdr_exposure_attr *hdr_exp_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_hdr_exposure_attr(VI_PIPE vi_pipe, hi_isp_hdr_exposure_attr *hdr_exp_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_ae_route_attr(VI_PIPE vi_pipe, const hi_isp_ae_route *ae_route_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_ae_route_attr(VI_PIPE vi_pipe, hi_isp_ae_route *ae_route_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_ae_route_sf_attr(VI_PIPE vi_pipe, const hi_isp_ae_route *ae_route_sf_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_ae_route_sf_attr(VI_PIPE vi_pipe, hi_isp_ae_route *ae_route_sf_attr);

MPI_STATIC hi_s32 hi_mpi_isp_query_exposure_info(VI_PIPE vi_pipe, hi_isp_exp_info *exp_info);

MPI_STATIC hi_s32 hi_mpi_isp_set_iris_attr(VI_PIPE vi_pipe, const hi_isp_iris_attr *iris_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_iris_attr(VI_PIPE vi_pipe, hi_isp_iris_attr *iris_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_dciris_attr(VI_PIPE vi_pipe, const hi_isp_dciris_attr *dciris_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_dciris_attr(VI_PIPE vi_pipe, hi_isp_dciris_attr *dciris_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_piris_attr(VI_PIPE vi_pipe, const hi_isp_piris_attr *piris_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_piris_attr(VI_PIPE vi_pipe, hi_isp_piris_attr *piris_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_ae_route_attr_ex(VI_PIPE vi_pipe, const hi_isp_ae_route_ex *ae_route_attr_ex);
MPI_STATIC hi_s32 hi_mpi_isp_get_ae_route_attr_ex(VI_PIPE vi_pipe, hi_isp_ae_route_ex *ae_route_attr_ex);

MPI_STATIC hi_s32 hi_mpi_isp_set_ae_route_sf_attr_ex(VI_PIPE vi_pipe, const hi_isp_ae_route_ex *ae_route_sf_attr_ex);
MPI_STATIC hi_s32 hi_mpi_isp_get_ae_route_sf_attr_ex(VI_PIPE vi_pipe, hi_isp_ae_route_ex *ae_route_sf_attr_ex);

MPI_STATIC hi_s32 hi_mpi_isp_set_smart_exposure_attr(VI_PIPE vi_pipe, const hi_isp_smart_exposure_attr *smart_exp_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_smart_exposure_attr(VI_PIPE vi_pipe, hi_isp_smart_exposure_attr *smart_exp_attr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
