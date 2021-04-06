/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP tile header file
 * Author: svp group
 * Create: 2017-10-17
 */

#ifndef __SVP_DSP_TILE_H__
#define __SVP_DSP_TILE_H__
#include "svp_dsp.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* __cplusplus */

/*****************************************************************************
*   Prototype    : SVP_DSP_Erode_3x3_U8_U8_Const
*   Description  : 3x3 const template erode.
*   Parameters   : SVP_DSP_SRC_TILE_S         *pstSrc            Input tile,U8C1.
*                  SVP_DSP_DST_TILE_S         *pstDst            Output tile,U8C1.
*  
*          
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         : 
*   History:
* 
*       1.  Date         : 2017-03-30
*           Author       : 
*           Modification : Created function
*
*****************************************************************************/
HI_S32  SVP_DSP_Erode_3x3_U8_U8_Const(SVP_DSP_SRC_TILE_S  *pstSrc, SVP_DSP_DST_TILE_S *pstDst);

/*****************************************************************************
*   Prototype    : SVP_DSP_Dilate_3x3_U8_U8_Const
*   Description  : 3x3 const template dilate.
*   Parameters   : SVP_DSP_SRC_TILE_S         *pstSrc           Input tile,U8C1.
*                  SVP_DSP_DST_TILE_S        *pstDst            Output Tile,U8C1.
*    
*          
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         : 
*   History:
* 
*       1.  Date         : 2017-03-30
*           Author       : 
*           Modification : Created function
*
*****************************************************************************/
HI_S32  SVP_DSP_Dilate_3x3_U8_U8_Const(SVP_DSP_SRC_TILE_S *pstSrc, SVP_DSP_DST_TILE_S *pstDst);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SVP_DSP_TILE_H__ */

