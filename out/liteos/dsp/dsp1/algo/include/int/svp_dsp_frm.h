/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP algorithm frame header file
 * Author: svp group
 * Create: 2017-10-17
 */

#ifndef __SVP_DSP_FRM_H__
#define __SVP_DSP_FRM_H__

#include "svp_dsp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*****************************************************************************
*   Prototype    : SVP_DSP_FRM_Init
*   Description  : DSP frame and tile manager init.
*   Parameters   : HI_VOID
*
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
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
HI_S32 SVP_DSP_FRM_Init(HI_VOID);

/*****************************************************************************
*   Prototype    : SVP_DSP_FRM_Exit
*   Description  : DSP  frame and tile manager exit.
*   Parameters   : HI_VOID
*
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
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
HI_S32 SVP_DSP_FRM_Exit(HI_VOID);

/*****************************************************************************
*   Prototype    : SVP_DSP_CopyData
*   Description  : Transfer data between external and local memories.
*   Parameters   : HI_VOID *pvSrc          Input source data address.
*                  HI_VOID *pvDst          Output destination address.
*                  HI_S32 s32Size         Data size
*
*   Return Value : HI_SUCCESS: Success;Error codes: .
*   Spec         :
*   History:
*
*       1.  Date         : 2017-03-31
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
HI_S32 SVP_DSP_CopyData(HI_VOID *pvDst, HI_VOID *pvSrc, HI_S32 s32Size);

/*****************************************************************************
*   Prototype    : SVP_DSP_Dilate_3x3_U8_U8_Frm
*   Description  : Dilates a frame taking maximum of pixel neighborhood with 3x3 filter.
*   Parameters   : SVP_DSP_SRC_FRAME_S *pstSrc          Input source data. Only the U8C1 input format is supported.
*                  SVP_DSP_DST_FRAME_S *pstDst          Output result.
*
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
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
HI_S32 SVP_DSP_Dilate_3x3_U8_U8_Frm(SVP_DSP_SRC_FRAME_S *pstSrc, SVP_DSP_DST_FRAME_S *pstDst);

/*****************************************************************************
*   Prototype    : SVP_DSP_Erode_3x3_U8_U8_Frm
*   Description  : Erodes a frame taking maximum of pixel neighborhood with 3x3 filter.
*   Parameters   : SVP_DSP_SRC_FRAME_S *pstSrc          Input source data. Only the U8C1 input format is supported.
*                  SVP_DSP_DST_FRAME_S *pstDst          Output result.

*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
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
HI_S32 SVP_DSP_Erode_3x3_U8_U8_Frm(SVP_DSP_SRC_FRAME_S *pstSrc, SVP_DSP_DST_FRAME_S *pstDst);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __SVP_DSP_FRM_H__ */

