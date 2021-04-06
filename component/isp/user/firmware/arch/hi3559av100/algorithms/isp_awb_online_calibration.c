/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hi_comm_isp_adapt.h"
#include "mpi_isp.h"
#include "isp_math_utils.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

hi_s32 isp_get_lightbox_gain(VI_PIPE vi_pipe, hi_isp_awb_calibration_gain *awb_calibration_gain)
{
    HI_U32 u32Rsum = 0;
    HI_U32 u32Bsum = 0;
    HI_U32 u32Gsum = 0;
    HI_S32 i, j;
    HI_S32 s32ZoneCol;
    HI_S32 s32ZoneRow;
    HI_S32 s32ZoneColStart;
    HI_S32 s32ZoneColEnd;
    HI_S32 s32ZoneRowStart;
    HI_S32 s32ZoneRowEnd;
    HI_S32 s32Ret;
    ISP_WB_STATISTICS_S  *pstWBStat  = NULL;
    ISP_STATISTICS_CFG_S *pstStatCfg = NULL;

    pstWBStat = (ISP_WB_STATISTICS_S *)ISP_MALLOC(sizeof(ISP_WB_STATISTICS_S));
    if (pstWBStat == HI_NULL) {
        ISP_ERR_TRACE("pstWBStat malloc failure !\n");
        return HI_ERR_ISP_NOMEM;
    }
    pstStatCfg = (ISP_STATISTICS_CFG_S *)ISP_MALLOC(sizeof(ISP_STATISTICS_CFG_S));
    if (pstStatCfg == HI_NULL) {
        ISP_ERR_TRACE("pstWBStat malloc failure !\n");
        ISP_FREE(pstWBStat);

        return HI_ERR_ISP_NOMEM;
    }
    s32Ret = HI_MPI_ISP_GetStatisticsConfig(vi_pipe, pstStatCfg);
    if (s32Ret != HI_SUCCESS) {
        ISP_FREE(pstWBStat);
        ISP_FREE(pstStatCfg);

        return s32Ret;
    }
    if (pstStatCfg->stWBCfg.u16ZoneCol * pstStatCfg->stWBCfg.u16ZoneRow < 16) {
        ISP_ERR_TRACE("Not support zone number less than 16 !\n");
        ISP_FREE(pstWBStat);
        ISP_FREE(pstStatCfg);

        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    s32Ret = HI_MPI_ISP_GetWBStatistics(vi_pipe, pstWBStat);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Get WB statics failed!\n");
        ISP_FREE(pstWBStat);
        ISP_FREE(pstStatCfg);

        return s32Ret;
    }

    s32ZoneCol = pstStatCfg->stWBCfg.u16ZoneCol;
    s32ZoneRow = pstStatCfg->stWBCfg.u16ZoneRow;
    s32ZoneColStart = s32ZoneCol / 2 - 2;
    s32ZoneColEnd   = s32ZoneCol / 2 + 2;
    s32ZoneRowStart = s32ZoneRow / 2 - 2;
    s32ZoneRowEnd   = s32ZoneRow / 2 + 2;

    /* Get_statistics */
    for (j = s32ZoneRowStart; j < s32ZoneRowEnd; j++) {
        for (i = j * s32ZoneCol + s32ZoneColStart; i < j * s32ZoneCol + s32ZoneColEnd; i++) {
            u32Rsum += pstWBStat->au16ZoneAvgR[i];
            u32Bsum += pstWBStat->au16ZoneAvgB[i];
            u32Gsum += pstWBStat->au16ZoneAvgG[i];
        }
    }

    awb_calibration_gain->avg_r_gain = (u32Gsum << 8) / DIV_0_TO_1(u32Rsum); /* G/R*256 */
    awb_calibration_gain->avg_b_gain = (u32Gsum << 8) / DIV_0_TO_1(u32Bsum); /* avarage for 16 zones */

    ISP_FREE(pstWBStat);
    ISP_FREE(pstStatCfg);

    return HI_SUCCESS;
}
