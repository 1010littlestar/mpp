/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP algorithm performance header file
 * Author: svp group
 * Create: 2017-10-17
 */

#ifndef __SVP_DSP_PERFORMANCE_H__
#define __SVP_DSP_PERFORMANCE_H__
#include "hi_type.h"

typedef enum hiSVP_DSP_TASK_TYPE_E {
    SVP_DSP_TASK_ERODE = 0x00,
    SVP_DSP_TASK_DILATE = 0x01,

    SVP_DSP_TASK_BUTT
} SVP_DSP_TASK_TYPE_E;
/*
 * Performance
 */
typedef struct hiSVP_DSP_PERF_S {
    HI_U32 u32TileWidth;         /* Tile Width */
    HI_U32 u32TileHeight;        /* Tile Height */
    HI_U32 u32ImageWidth;        /* Image Width */
    HI_U32 u32ImageHeight;       /* Image Height */
    HI_U32 u32TileCount;         /* Tile Count */
    HI_U32 u32TileTotalCycle;    /* Tile Total cycle */
    HI_U32 u32InTotalBandWidth;  /* Read Total BandWidth */
    HI_U32 u32OutTotalBandWidth; /* Write Total BandWidth */
} SVP_DSP_PERF_S;

/*****************************************************************************
*   Prototype    : SVP_DSP_GetPerf
*   Description  : Get DSP performance.
*   Parameters   : SVP_DSP_TASK_TYPE_E  enType      Input task id. The algorithm need to get performance
*                  SVP_DSP_PERF_S      *pstPerf     Output performance structure. To store performance data
*
*   Return Value :
*   Spec         :
*
*
*   History:
*
*       1.  Date         : 2017-03-31
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
HI_VOID SVP_DSP_GetPerf(SVP_DSP_TASK_TYPE_E enType, SVP_DSP_PERF_S *pstPerf);

#endif /* __SVP_DSP_PERFORMANCE_H__ */

