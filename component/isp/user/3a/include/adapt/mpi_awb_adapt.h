/*  copyright (C),2013-2019, hisilicon tech. co., ltd.
*  version       : initial draft
*  author        : ISP_SW
*  created       : 2013/11/07
*  description   :
*  history       :
*  1.date        : 2013/11/07
*  modification: created file
*/

#ifndef __MPI_AWB_ADAPT_H__
#define __MPI_AWB_ADAPT_H__

#include "hi_comm_isp_adapt.h"
#include "hi_comm_3a_adapt.h"
#include "hi_awb_comm_adapt.h"
#include "hi_comm_sns_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if 0
/* the interface of awb lib register to isp. */
MPI_STATIC hi_s32 hi_mpi_awb_register(VI_PIPE vi_pipe, hi_isp_alg_lib *awb_lib);
MPI_STATIC hi_s32 hi_mpi_awb_unregister(VI_PIPE vi_pipe, hi_isp_alg_lib *awb_lib);

/* the callback function of sensor register to awb lib. */
MPI_STATIC hi_s32 hi_mpi_awb_sensor_reg_callback(VI_PIPE vi_pipe, hi_isp_alg_lib *awb_lib, hi_isp_sns_attr_info *sns_attr_info,
                                                  hi_isp_awb_sensor_register *sns_register);
MPI_STATIC hi_s32 hi_mpi_awb_sensor_unreg_callback(VI_PIPE vi_pipe, hi_isp_alg_lib *awb_lib, SENSOR_ID sensor_id);
#endif
MPI_STATIC hi_s32 hi_mpi_isp_set_wb_attr(VI_PIPE vi_pipe, const hi_isp_wb_attr *wb_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_wb_attr(VI_PIPE vi_pipe, hi_isp_wb_attr *wb_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_awb_attr_ex(VI_PIPE vi_pipe, hi_isp_awb_attr_ex *awb_attr_ex);
MPI_STATIC hi_s32 hi_mpi_isp_get_awb_attr_ex(VI_PIPE vi_pipe, hi_isp_awb_attr_ex *awb_attr_ex);

MPI_STATIC hi_s32 hi_mpi_isp_set_ccm_attr(VI_PIPE vi_pipe, const hi_isp_color_matrix_attr *ccm_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_ccm_attr(VI_PIPE vi_pipe, hi_isp_color_matrix_attr *ccm_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_saturation_attr(VI_PIPE vi_pipe, const hi_isp_saturation_attr *sat_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_saturation_attr(VI_PIPE vi_pipe, hi_isp_saturation_attr *sat_attr);

MPI_STATIC hi_s32 hi_mpi_isp_query_wb_info(VI_PIPE vi_pipe, hi_isp_wb_info *wb_info);
MPI_STATIC hi_s32 hi_mpi_isp_cal_gain_by_temp(VI_PIPE vi_pipe, const hi_isp_wb_attr *wb_attr, hi_u16 color_temp, hi_s16 shift, hi_u16 *awb_gain);
MPI_STATIC hi_s32 hi_mpi_isp_set_spec_awb_attr(VI_PIPE vi_pipe, const hi_isp_spec_awb_attr *spec_awb_attr);
MPI_STATIC hi_s32 hi_mpi_isp_set_caa_control_attr(VI_PIPE vi_pipe, const hi_isp_spec_awb_caa_control *spec_awb_caa_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_caa_control_attr(VI_PIPE vi_pipe, hi_isp_spec_awb_caa_control *spec_awb_caa_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_spec_awb_attr(VI_PIPE vi_pipe, hi_isp_spec_awb_attr *spec_awb_attr);
MPI_STATIC HI_S32 hi_mpi_isp_set_spec_awb_control_attr(VI_PIPE vi_pipe, hi_isp_spec_awb_control *spec_awb_control);
MPI_STATIC HI_S32 hi_mpi_isp_get_spec_awb_control_attr(VI_PIPE vi_pipe, hi_isp_spec_awb_control *spec_awb_control);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif
