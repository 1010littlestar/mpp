/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP frame info header file
 * Author: svp group
 * Create: 2017-10-17
 */
#ifndef __SVP_DSP_H__
#define __SVP_DSP_H__

#include <stdint.h>
#include <stddef.h>
#include "hi_type.h"
/*
 * Pixel pack format
 */
typedef enum hiSVP_DSP_PIXEL_PACK_FORMAT_E {
    SVP_DSP_PIXEL_PACK_FORMAT_ONE = 0x1,   /* SINGLE_COMPONENT_PLANAR */
    SVP_DSP_PIXEL_PACK_FORMAT_TWO = 0x2,   /* TWO_COMPONENT_PACKED */
    SVP_DSP_PIXEL_PACK_FORMAT_THREE = 0x3, /* THREE_COMPONENT_PACKED */
    SVP_DSP_PIXEL_PACK_FORMAT_FOUR = 0x4,  /* FOUR_COMPONENT_PACKED */

    SVP_DSP_PIXEL_PACK_FORMAT_BUTT
} SVP_DSP_PIXEL_PACK_FORMAT_E;

/*
 * Padding Type
 */
typedef enum hiSVP_DSP_PADDING_TYPE_E {
    SVP_DSP_PADDING_TYPE_ZERO = 0x0,
    SVP_DSP_PADDING_TYPE_EDGE = 0x1,

    SVP_DSP_PADDING_TYPE_BUTT
} SVP_DSP_PADDING_TYPE_E;

/**
 * Frame
 */
typedef struct hiSVP_DSP_FRAME_S {
    void *pvFrameBuff;
    HI_U32 u32FrameBuffSize;
    void *pvFrameData;
    HI_S32 s32FrameWidth;
    HI_S32 s32FrameHeight;
    HI_S32 s32FramePitch;
    HI_U8 u8PixelRes;
    HI_U8 u8PixelPackFormat;
    HI_U8 u8LeftEdgePadWidth;
    HI_U8 u8TopEdgePadHeight;
    HI_U8 u8RightEdgePadWidth;
    HI_U8 u8BottomEdgePadHeight;
    HI_U8 u8PaddingType;
} SVP_DSP_FRAME_S;

typedef SVP_DSP_FRAME_S SVP_DSP_SRC_FRAME_S;
typedef SVP_DSP_FRAME_S SVP_DSP_DST_FRAME_S;
typedef SVP_DSP_PIXEL_PACK_FORMAT_E pixelPackFormat_t;
typedef SVP_DSP_FRAME_S xvFrame;
typedef SVP_DSP_FRAME_S *xvpFrame;

/*
 * XV_ARRAY_FIELDS,the same to cadence
 */
#define XV_ARRAY_FIELDS  \
    void *pBuffer;       \
    uint32_t bufferSize; \
    void *pData;         \
    int32_t width;       \
    int32_t pitch;       \
    uint32_t status;     \
    uint16_t type;       \
    uint16_t height;

/*
 * xvArrayStruct,the same to cadence
 */
typedef struct xvArrayStruct {
    XV_ARRAY_FIELDS
} xvArray, *xvpArray;

/*
 * xvTileStruct,the same to cadence
 */
typedef struct xvTileStruct {
    XV_ARRAY_FIELDS
    xvFrame *pFrame;
    int32_t x;
    int32_t y;
    uint16_t tileEdgeLeft;
    uint16_t tileEdgeTop;
    uint16_t tileEdgeRight;
    uint16_t tileEdgeBottom;
    int32_t reuseCount;
    struct xvTileStruct *pPrevTile;
} xvTile, *xvpTile;

typedef xvArray SVP_DSP_ARRAY_S;
typedef xvTile SVP_DSP_TILE_S;
typedef SVP_DSP_ARRAY_S SVP_DSP_SRC_ARRAY_S;
typedef SVP_DSP_ARRAY_S SVP_DSP_DST_ARRAY_S;
typedef SVP_DSP_TILE_S SVP_DSP_SRC_TILE_S;
typedef SVP_DSP_TILE_S SVP_DSP_DST_TILE_S;

#endif /* __SVP_DSP_H__ */

