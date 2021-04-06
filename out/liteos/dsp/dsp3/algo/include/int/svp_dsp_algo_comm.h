/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP algorithm comm header file
 * Author: svp group
 * Create: 2017-10-17
 */

#ifndef __SVP_DSP_ALGO_COMM_H__
#define __SVP_DSP_ALGO_COMM_H__
#include "svp_dsp_trace.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* 4GB physical address range */
#define SVP_DSP_PHYADDR_4GB 0xFFFFFFFF
/* 1k byte */
#define SVP_DSP_1_K (1024)

/*****************************************
*                   Frame Access Macros
*****************************************/

#define SVP_DSP_FRAME_GET_BUFF_PTR(frame)        ((frame)->pvFrameBuff)
#define SVP_DSP_FRAME_SET_BUFF_PTR(frame, pBuff) (frame)->pvFrameBuff = ((void *)(pBuff))

#define SVP_DSP_FRAME_GET_BUFF_SIZE(frame)           ((frame)->u32FrameBuffSize)
#define SVP_DSP_FRAME_SET_BUFF_SIZE(frame, buffSize) (frame)->u32FrameBuffSize = ((HI_U32)(buffSize))

#define SVP_DSP_FRAME_GET_DATA_PTR(frame)        ((frame)->pvFrameData)
#define SVP_DSP_FRAME_SET_DATA_PTR(frame, pData) (frame)->pvFrameData = ((void *)(pData))

#define SVP_DSP_FRAME_GET_WIDTH(frame)        ((frame)->s32FrameWidth)
#define SVP_DSP_FRAME_SET_WIDTH(frame, width) (frame)->s32FrameWidth = ((HI_S32)(width))

#define SVP_DSP_FRAME_GET_HEIGHT(frame)         ((frame)->s32FrameHeight)
#define SVP_DSP_FRAME_SET_HEIGHT(frame, height) (frame)->s32FrameHeight = ((HI_S32)(height))

#define SVP_DSP_FRAME_GET_PITCH(frame)          ((frame)->s32FramePitch)
#define SVP_DSP_FRAME_SET_PITCH(frame, pitch)   (frame)->s32FramePitch = ((HI_S32)(pitch))
#define SVP_DSP_FRAME_GET_PITCH_IN_BYTES(frame) ((frame)->s32FramePitch * (frame)->pixRes)

#define SVP_DSP_FRAME_GET_PIXEL_RES(frame)         ((frame)->u8PixelRes)
#define SVP_DSP_FRAME_SET_PIXEL_RES(frame, pixRes) (frame)->u8PixelRes = ((HI_U8)(pixRes))

#define SVP_DSP_FRAME_GET_PIXEL_FORMAT(frame)              ((frame)->u8PixelPackFormat)
#define SVP_DSP_FRAME_SET_PIXEL_FORMAT(frame, pixelFormat) (frame)->u8PixelPackFormat = ((HI_U8)(pixelFormat))

#define SVP_DSP_FRAME_GET_EDGE_WIDTH(frame)           ((frame)->u8LeftEdgePadWidth < (frame)->u8RightEdgePadWidth\
    ? (frame)->u8LeftEdgePadWidth : (frame)->u8RightEdgePadWidth)
#define SVP_DSP_FRAME_SET_EDGE_WIDTH(frame, padWidth) (frame)->u8LeftEdgePadWidth = (frame)->u8RightEdgePadWidth =\
    ((HI_U8)(padWidth))

#define SVP_DSP_FRAME_GET_EDGE_HEIGHT(frame)            ((frame)->u8TopEdgePadWidth < (frame)->u8BottomEdgePadWidth\
    ? (frame)->u8TopEdgePadWidth : (frame)->u8BottomEdgePadWidth)
#define SVP_DSP_FRAME_SET_EDGE_HEIGHT(frame, padHeight) (frame)->u8TopEdgePadHeight = (frame)->u8BottomEdgePadHeight =\
    ((HI_U8)(padHeight))

#define SVP_DSP_FRAME_GET_EDGE_LEFT(frame)           ((frame)->u8LeftEdgePadWidth)
#define SVP_DSP_FRAME_SET_EDGE_LEFT(frame, padWidth) (frame)->u8LeftEdgePadWidth = ((HI_U8)(padWidth))

#define SVP_DSP_FRAME_GET_EDGE_RIGHT(frame)           ((frame)->u8RightEdgePadWidth)
#define SVP_DSP_FRAME_SET_EDGE_RIGHT(frame, padWidth) (frame)->u8RightEdgePadWidth = ((HI_U8)(padWidth))

#define SVP_DSP_FRAME_GET_EDGE_TOP(frame)            ((frame)->u8TopEdgePadWidth)
#define SVP_DSP_FRAME_SET_EDGE_TOP(frame, padHeight) (frame)->u8TopEdgePadHeight = ((HI_U8)(padHeight))

#define SVP_DSP_FRAME_GET_EDGE_BOTTOM(frame)            ((frame)->u8BottomEdgePadWidth)
#define SVP_DSP_FRAME_SET_EDGE_BOTTOM(frame, padHeight) (frame)->u8BottomEdgePadHeight = ((HI_U8)(padHeight))

#define SVP_DSP_FRAME_GET_PADDING_TYPE(frame)          ((frame)->u8PaddingType)
#define SVP_DSP_FRAME_SET_PADDING_TYPE(frame, padType) (frame)->u8PaddingType = (padType)

#define SVP_DSP_SETUP_FRAME(frame, pFrameBuffer, width, height, padWidth, padHeight, stride, pixRes, ppf, paddingType)\
    {                                                                                                                 \
        SVP_DSP_FRAME_SET_BUFF_PTR((frame), (pFrameBuffer));                                                         \
        SVP_DSP_FRAME_SET_BUFF_SIZE((frame), (stride) * ((height) + 2 * (padHeight)));                              \
        SVP_DSP_FRAME_SET_WIDTH((frame), (width));                                                                   \
        SVP_DSP_FRAME_SET_HEIGHT((frame), (height));                                                                 \
        SVP_DSP_FRAME_SET_PITCH((frame), (stride));                                                                 \
        SVP_DSP_FRAME_SET_PIXEL_RES((frame), (pixRes));                                                              \
        SVP_DSP_FRAME_SET_DATA_PTR((frame), (pFrameBuffer) +\
        (SVP_DSP_FRAME_GET_PITCH(frame) * (padHeight) + (padWidth)) * (pixRes)); \
        SVP_DSP_FRAME_SET_EDGE_WIDTH((frame), (padWidth));                                                           \
        SVP_DSP_FRAME_SET_EDGE_HEIGHT((frame), (padHeight));                                                        \
        SVP_DSP_FRAME_SET_PIXEL_FORMAT((frame), (ppf));                                                             \
        SVP_DSP_FRAME_SET_PADDING_TYPE((frame), (paddingType));                                                      \
    }

/*****************************************************************************
*   Prototype    : SVP_DSP_GetImgPhyAddr
*   Description  : Get Image physical address
*   Input        :  HI_U64         u64IdmaOffset   IDMA offset
*                   SVP_IMAGE_S    *pstImg         Image.
*
*   Output       : HI_U32         au32PhyAddr[3]  Physical address
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   Calls        :
*   Called By    :
*   History:
*
*       1.  Date         : 2017-11-01
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
HI_S32 SVP_DSP_GetImgPhyAddr(HI_U64 u64IdmaOffset, SVP_IMAGE_S *pstImg, HI_U32 au32PhyAddr[3]);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SVP_DSP_ALGO_COMM_H__ */

