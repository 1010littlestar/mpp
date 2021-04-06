/*
 * Copyright (c) 2015 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of
 * Cadence Design Systems Inc.  They may be adapted and modified by bona fide
 * purchasers for internal use, but neither the original nor any adapted
 * or modified version may be disclosed or distributed to third parties
 * in any manner, medium, or form, in whole or in part, without the prior
 * written consent of Cadence Design Systems Inc.  This software and its
 * derivatives are to be executed solely on products incorporating a Cadence
 * Design Systems processor.
 */

#ifndef __TILE_MANAGER_H__
#define __TILE_MANAGER_H__

#include <stdint.h>
#include <stddef.h>

#ifndef XV_EMULATE_DMA
#include "xmem.h"
#include <xtensa/idma.h>
#endif //XV_EMULATE_DMA

#include "svp_dsp.h"

//typedef SVP_DSP_PIXEL_PACK_FORMAT_E  pixelPackFormat_t;
//typedef SVP_DSP_FRAME_S xvFrame;
//typedef SVP_DSP_FRAME_S *xvpFrame;
// MAX limits for number of tiles, frames memory banks and dma queue length
#define MAX_NUM_MEM_BANKS         8
#define MAX_NUM_TILES             32
#define MAX_NUM_FRAMES            8
#define MAX_NUM_DMA_QUEUE_LENGTH  32 // Optimization, multiple of 2

// Bank colors. XV_MEM_BANK_COLOR_ANY is an unlikely enum value
#define XV_MEM_BANK_COLOR_0    0x0
#define XV_MEM_BANK_COLOR_1    0x1
#define XV_MEM_BANK_COLOR_2    0x2
#define XV_MEM_BANK_COLOR_3    0x3
#define XV_MEM_BANK_COLOR_4    0x4
#define XV_MEM_BANK_COLOR_5    0x5
#define XV_MEM_BANK_COLOR_6    0x6
#define XV_MEM_BANK_COLOR_7    0x7
#define XV_MEM_BANK_COLOR_ANY  0xBEEDDEAF

// Edge padding format
//#define SVP_DSP_PADDING_TYPE_ZERO  0
//#define SVP_DSP_PADDING_TYPE_EDGE  1

#define IVP_SIMD_WIDTH      XCHAL_IVPN_SIMD_WIDTH
#define IVP_ALIGNMENT       0x1F
#define XVTM_MIN(a, b)  (((a) < (b)) ? (a) : (b))

#define XVTM_DUMMY_DMA_INDEX  -2
#define XVTM_ERROR            -1
#define XVTM_SUCCESS          0


#ifdef XV_EMULATE_DMA

typedef enum
{
  IDMA_1D_DESC = 1,
  IDMA_2D_DESC = 2
} idma_type_t;


#define MAX_BLOCK_2    2
#define TICK_CYCLES_1  1
#define TICK_CYCLES_2  2
#define IDMA_BUFFER_DEFINE(name, n, type) \
  void* name;

typedef int  idma_status_t;
typedef int  idma_max_block_t;
typedef int  idma_max_pif_t;
typedef int  idma_ticks_cyc_t;
typedef int  idma_error_details_t;
typedef int  idma_buffer_t;
typedef void (*idma_callback_fn)(void*);
typedef void (*idma_err_callback_fn)(idma_error_details_t*);
typedef int  xmem_mgr_t;
typedef int  xmem_status_t;
typedef int  _idma_buffer_t;

idma_error_details_t* idma_buffer_error_details_emulate();
#endif
/*
typedef enum
{
  SVP_DSP_PIXEL_PACK_FORMAT_ONE = 1,
  SVP_DSP_PIXEL_PACK_FORMAT_TWO    = 2,
  SVP_DSP_PIXEL_PACK_FORMAT_THREE  = 3,
  SVP_DSP_PIXEL_PACK_FORMAT_FOUR   = 4
} pixelPackFormat_t;
*/
typedef enum
{
  XV_ERROR_SUCCESS            = 0,
  XV_ERROR_TILE_MANAGER_NULL  = 1,
  XV_ERROR_POINTER_NULL       = 2,
  XV_ERROR_FRAME_NULL         = 3,
  XV_ERROR_TILE_NULL          = 4,
  XV_ERROR_BUFFER_NULL        = 5,
  XV_ERROR_ALLOC_FAILED       = 6,
  XV_ERROR_FRAME_BUFFER_FULL  = 7,
  XV_ERROR_TILE_BUFFER_FULL   = 8,
  XV_ERROR_DIMENSION_MISMATCH = 9,
  XV_ERROR_BUFFER_OVERFLOW    = 10,
  XV_ERROR_BAD_ARG            = 11
}xvError_t;
/*
#define XV_ARRAY_FIELDS \
  void     *pBuffer;    \
  uint32_t bufferSize;  \
  void *pData;          \
  int32_t width;        \
  int32_t pitch;        \
  uint32_t status;      \
  uint16_t type;        \
  uint16_t height;


typedef struct xvArrayStruct
{
  XV_ARRAY_FIELDS
} xvArray, *xvpArray;


typedef struct xvTileStruct
{
  XV_ARRAY_FIELDS
  xvFrame             *pFrame;
  int32_t             x;
  int32_t             y;
  uint16_t            tileEdgeLeft;
  uint16_t            tileEdgeTop;
  uint16_t            tileEdgeRight;
  uint16_t            tileEdgeBottom;
  int32_t             reuseCount;
  struct xvTileStruct *pPrevTile;
} xvTile, *xvpTile;

*/
typedef struct xvTileManagerStruct
{
  // iDMA related
  void    *pdmaObj;
  int32_t tileDMApendingCount;       // Incremented when new request is added. Decremented when request is completed.
  int32_t tileDMAstartIndex;         // Incremented when request is completed
  xvTile  *tileProcQueue[MAX_NUM_DMA_QUEUE_LENGTH];
  int32_t tileDMAindex[MAX_NUM_DMA_QUEUE_LENGTH];

  // Mem Banks
  int32_t    numMemBanks;       // Number of memory banks/pools
#ifndef XV_EMULATE_DMA
  xmem_mgr_t memBankMgr[MAX_NUM_MEM_BANKS];       // xmem memory manager, one for each bank
#endif
  void       *pMemBankStart[MAX_NUM_MEM_BANKS];    // Start address of bank
  int32_t    memBankSize[MAX_NUM_MEM_BANKS];       // size of each bank

  // Tiles and frame allocation
  xvTile    tilePtrArr[MAX_NUM_TILES];
  xvFrame   framePtrArr[MAX_NUM_FRAMES];
  int32_t   tileAllocFlags[(MAX_NUM_TILES + 31) / 32];      // Each bit of tileAllocFlags and frameAllocFlags
  int32_t   frameAllocFlags[(MAX_NUM_FRAMES + 31) / 32];    // indicates if a particular tile/frame is allocated
  int32_t   tileCount;
  int32_t   frameCount;
  xvError_t errFlag;
} xvTileManager;

/*****************************************
*   Individual status flags definitions
*****************************************/

#define XV_TILE_STATUS_DMA_ONGOING                 (0x01 << 0)
#define XV_TILE_STATUS_LEFT_EDGE_PADDING_NEEDED    (0x01 << 1)
#define XV_TILE_STATUS_RIGHT_EDGE_PADDING_NEEDED   (0x01 << 2)
#define XV_TILE_STATUS_TOP_EDGE_PADDING_NEEDED     (0x01 << 3)
#define XV_TILE_STATUS_BOTTOM_EDGE_PADDING_NEEDED  (0x01 << 4)
#define XV_TILE_STATUS_EDGE_PADDING_NEEDED         (XV_TILE_STATUS_LEFT_EDGE_PADDING_NEEDED |  \
                                                    XV_TILE_STATUS_RIGHT_EDGE_PADDING_NEEDED | \
                                                    XV_TILE_STATUS_TOP_EDGE_PADDING_NEEDED |   \
                                                    XV_TILE_STATUS_BOTTOM_EDGE_PADDING_NEEDED)


/*****************************************
*   Data type definitions
*****************************************/

#define XV_TYPE_SIGNED_BIT         (1 << 15)
#define XV_TYPE_TILE_BIT           (1 << 14)

#define XV_TYPE_ELEMENT_SIZE_BITS  10
#define XV_TYPE_ELEMENT_SIZE_MASK  ((1 << XV_TYPE_ELEMENT_SIZE_BITS) - 1)
#define XV_TYPE_CHANNELS_BITS      2
#define XV_TYPE_CHANNELS_MASK      (((1 << XV_TYPE_CHANNELS_BITS) - 1) << XV_TYPE_ELEMENT_SIZE_BITS)

#define XV_MAKETYPE(flags, depth, channels)  (((depth) * (channels)) | (((channels) - 1) << XV_TYPE_ELEMENT_SIZE_BITS) | (flags))
#define XV_CUSTOMTYPE(type)                  XV_MAKETYPE(0, sizeof(type), 1)

#define XV_TYPE_ELEMENT_SIZE(type)           ((type) & (XV_TYPE_ELEMENT_SIZE_MASK))
#define XV_TYPE_ELEMENT_TYPE(type)           ((type) & (XV_TYPE_SIGNED_BIT | XV_TYPE_CHANNELS_MASK | XV_TYPE_ELEMENT_SIZE_MASK))
#define XV_TYPE_IS_TILE(type)                ((type) & (XV_TYPE_TILE_BIT))
#define XV_TYPE_IS_SIGNED(type)              ((type) & (XV_TYPE_SIGNED_BIT))

#define XV_U8         XV_MAKETYPE(0, 1, 1)
#define XV_U16        XV_MAKETYPE(0, 2, 1)
#define XV_U32        XV_MAKETYPE(0, 4, 1)

#define XV_S8         XV_MAKETYPE(XV_TYPE_SIGNED_BIT, 1, 1)
#define XV_S16        XV_MAKETYPE(XV_TYPE_SIGNED_BIT, 2, 1)
#define XV_S32        XV_MAKETYPE(XV_TYPE_SIGNED_BIT, 4, 1)

#define XV_ARRAY_U8   XV_U8
#define XV_ARRAY_S8   XV_S8
#define XV_ARRAY_U16  XV_U16
#define XV_ARRAY_S16  XV_S16
#define XV_ARRAY_U32  XV_U32
#define XV_ARRAY_S32  XV_S32

#define XV_TILE_U8    (XV_U8 | XV_TYPE_TILE_BIT)
#define XV_TILE_S8    (XV_S8 | XV_TYPE_TILE_BIT)
#define XV_TILE_U16   (XV_U16 | XV_TYPE_TILE_BIT)
#define XV_TILE_S16   (XV_S16 | XV_TYPE_TILE_BIT)
#define XV_TILE_U32   (XV_U32 | XV_TYPE_TILE_BIT)
#define XV_TILE_S32   (XV_S32 | XV_TYPE_TILE_BIT)

/*****************************************
*                   Frame Access Macros
*****************************************/

#define XV_FRAME_GET_BUFF_PTR(pFrame)                   ((pFrame)->pvFrameBuff)
#define XV_FRAME_SET_BUFF_PTR(pFrame, pBuff)            (pFrame)->pvFrameBuff = ((void *) (pBuff))

#define XV_FRAME_GET_BUFF_SIZE(pFrame)                  ((pFrame)->u32FrameBuffSize)
#define XV_FRAME_SET_BUFF_SIZE(pFrame, buffSize)        (pFrame)->u32FrameBuffSize = ((uint32_t) (buffSize))

#define XV_FRAME_GET_DATA_PTR(pFrame)                   ((pFrame)->pvFrameData)
#define XV_FRAME_SET_DATA_PTR(pFrame, pData)            (pFrame)->pvFrameData = ((void *) (pData))

#define XV_FRAME_GET_WIDTH(pFrame)                      ((pFrame)->s32FrameWidth)
#define XV_FRAME_SET_WIDTH(pFrame, width)               (pFrame)->s32FrameWidth = ((uint16_t) (width))

#define XV_FRAME_GET_HEIGHT(pFrame)                     ((pFrame)->s32FrameHeight)
#define XV_FRAME_SET_HEIGHT(pFrame, height)             (pFrame)->s32FrameHeight = ((uint16_t) (height))

#define XV_FRAME_GET_PITCH(pFrame)                      ((pFrame)->s32FramePitch)
#define XV_FRAME_SET_PITCH(pFrame, pitch)               (pFrame)->s32FramePitch = ((uint16_t) (pitch))
#define XV_FRAME_GET_PITCH_IN_BYTES(pFrame)             ((pFrame)->s32FramePitch * (pFrame)->u8PixRes)

#define XV_FRAME_GET_PIXEL_RES(pFrame)                  ((pFrame)->u8PixelRes)
#define XV_FRAME_SET_PIXEL_RES(pFrame, pixRes)          (pFrame)->u8PixelRes = ((uint8_t) (pixRes))

#define XV_FRAME_GET_PIXEL_FORMAT(pFrame)               ((pFrame)->u8PixelPackFormat)
#define XV_FRAME_SET_PIXEL_FORMAT(pFrame, pixelFormat)  (pFrame)->u8PixelPackFormat = ((uint8_t) (pixelFormat))

#define XV_FRAME_GET_EDGE_WIDTH(pFrame)                 ((pFrame)->u8LeftEdgePadWidth < (pFrame)->u8RightEdgePadWidth ? (pFrame)->u8LeftEdgePadWidth : (pFrame)->u8RightEdgePadWidth)
#define XV_FRAME_SET_EDGE_WIDTH(pFrame, padWidth)       (pFrame)->u8LeftEdgePadWidth = (pFrame)->u8RightEdgePadWidth = ((uint8_t) (padWidth))

#define XV_FRAME_GET_EDGE_HEIGHT(pFrame)                ((pFrame)->u8TopEdgePadWidth < (pFrame)->u8BottomEdgePadWidth ? (pFrame)->u8TopEdgePadWidth : (pFrame)->u8BottomEdgePadWidth)
#define XV_FRAME_SET_EDGE_HEIGHT(pFrame, padHeight)     (pFrame)->u8TopEdgePadHeight = (pFrame)->u8BottomEdgePadHeight = ((uint8_t) (padHeight))

#define XV_FRAME_GET_EDGE_LEFT(pFrame)                  ((pFrame)->u8LeftEdgePadWidth)
#define XV_FRAME_SET_EDGE_LEFT(pFrame, padWidth)        (pFrame)->u8LeftEdgePadWidth = ((uint8_t) (padWidth))

#define XV_FRAME_GET_EDGE_RIGHT(pFrame)                 ((pFrame)->u8RightEdgePadWidth)
#define XV_FRAME_SET_EDGE_RIGHT(pFrame, padWidth)       (pFrame)->u8RightEdgePadWidth = ((uint8_t) (padWidth))

#define XV_FRAME_GET_EDGE_TOP(pFrame)                   ((pFrame)->u8TopEdgePadWidth)
#define XV_FRAME_SET_EDGE_TOP(pFrame, padHeight)        (pFrame)->u8TopEdgePadHeight = ((uint8_t) (padHeight))

#define XV_FRAME_GET_EDGE_BOTTOM(pFrame)                ((pFrame)->u8BottomEdgePadWidth)
#define XV_FRAME_SET_EDGE_BOTTOM(pFrame, padHeight)     (pFrame)->u8BottomEdgePadHeight = ((uint8_t) (padHeight))

#define XV_FRAME_GET_PADDING_TYPE(pFrame)               ((pFrame)->u8PaddingType)
#define XV_FRAME_SET_PADDING_TYPE(pFrame, padType)      (pFrame)->u8PaddingType = (padType)

/*****************************************
*                   Array Access Macros
*****************************************/

#define XV_ARRAY_GET_BUFF_PTR(pArray)              ((pArray)->pBuffer)
#define XV_ARRAY_SET_BUFF_PTR(pArray, pBuff)       (pArray)->pBuffer = ((void *) (pBuff))

#define XV_ARRAY_GET_BUFF_SIZE(pArray)             ((pArray)->bufferSize)
#define XV_ARRAY_SET_BUFF_SIZE(pArray, buffSize)   (pArray)->bufferSize = ((uint32_t) (buffSize))

#define XV_ARRAY_GET_DATA_PTR(pArray)              ((pArray)->pData)
#define XV_ARRAY_SET_DATA_PTR(pArray, pArrayData)  (pArray)->pData = ((void *) (pArrayData))

#define XV_ARRAY_GET_WIDTH(pArray)                 ((pArray)->width)
#define XV_ARRAY_SET_WIDTH(pArray, value)          (pArray)->width = ((int32_t) (value))

#define XV_ARRAY_GET_PITCH(pArray)                 ((pArray)->pitch)
#define XV_ARRAY_SET_PITCH(pArray, value)          (pArray)->pitch = ((int32_t) (value))

#define XV_ARRAY_GET_HEIGHT(pArray)                ((pArray)->height)
#define XV_ARRAY_SET_HEIGHT(pArray, value)         (pArray)->height = ((uint16_t) (value))

#define XV_ARRAY_GET_STATUS_FLAGS(pArray)          ((pArray)->status)
#define XV_ARRAY_SET_STATUS_FLAGS(pArray, value)   (pArray)->status = ((uint8_t) (value))

#define XV_ARRAY_GET_TYPE(pArray)                  ((pArray)->type)
#define XV_ARRAY_SET_TYPE(pArray, value)           (pArray)->type = ((uint16_t) (value))

#define XV_ARRAY_GET_CAPACITY(pArray)              XV_ARRAY_GET_PITCH(pArray)
#define XV_ARRAY_SET_CAPACITY(pArray, value)       XV_ARRAY_SET_PITCH(pArray, value)

#define XV_ARRAY_GET_ELEMENT_TYPE(pArray)          XV_TYPE_ELEMENT_TYPE(XV_ARRAY_GET_TYPE(pArray))
#define XV_ARRAY_GET_ELEMENT_SIZE(pArray)          XV_TYPE_ELEMENT_SIZE(XV_ARRAY_GET_TYPE(pArray))
#define XV_ARRAY_IS_TILE(pArray)                   XV_TYPE_IS_TILE(XV_ARRAY_GET_TYPE(pArray))

/*****************************************
*                   Tile Access Macros
*****************************************/

#define XV_TILE_GET_BUFF_PTR   XV_ARRAY_GET_BUFF_PTR
#define XV_TILE_SET_BUFF_PTR   XV_ARRAY_SET_BUFF_PTR

#define XV_TILE_GET_BUFF_SIZE  XV_ARRAY_GET_BUFF_SIZE
#define XV_TILE_SET_BUFF_SIZE  XV_ARRAY_SET_BUFF_SIZE

#define XV_TILE_GET_DATA_PTR   XV_ARRAY_GET_DATA_PTR
#define XV_TILE_SET_DATA_PTR   XV_ARRAY_SET_DATA_PTR

#define XV_TILE_GET_WIDTH      XV_ARRAY_GET_WIDTH
#define XV_TILE_SET_WIDTH      XV_ARRAY_SET_WIDTH

#define XV_TILE_GET_PITCH      XV_ARRAY_GET_PITCH
#define XV_TILE_SET_PITCH      XV_ARRAY_SET_PITCH
#define XV_TILE_SET_PITCH_IN_BYTES(pTile)  ((pTile)->pitch * (pTile)->pFrame->pixRes)

#define XV_TILE_GET_HEIGHT        XV_ARRAY_GET_HEIGHT
#define XV_TILE_SET_HEIGHT        XV_ARRAY_SET_HEIGHT

#define XV_TILE_GET_STATUS_FLAGS  XV_ARRAY_GET_STATUS_FLAGS
#define XV_TILE_SET_STATUS_FLAGS  XV_ARRAY_SET_STATUS_FLAGS

#define XV_TILE_GET_TYPE          XV_ARRAY_GET_TYPE
#define XV_TILE_SET_TYPE          XV_ARRAY_SET_TYPE

#define XV_TILE_GET_ELEMENT_TYPE  XV_ARRAY_GET_ELEMENT_TYPE
#define XV_TILE_GET_ELEMENT_SIZE  XV_ARRAY_GET_ELEMENT_SIZE
#define XV_TILE_IS_TILE           XV_ARRAY_IS_TILE

#define XV_TILE_GET_FRAME_PTR(pTile)                           ((pTile)->pFrame)
#define XV_TILE_SET_FRAME_PTR(pTile, ptrFrame)                 (pTile)->pFrame = ((xvFrame *) (ptrFrame))

#define XV_TILE_GET_X_COORD(pTile)                             ((pTile)->x)
#define XV_TILE_SET_X_COORD(pTile, xcoord)                     (pTile)->x = ((int16_t) (xcoord))

#define XV_TILE_GET_Y_COORD(pTile)                             ((pTile)->y)
#define XV_TILE_SET_Y_COORD(pTile, ycoord)                     (pTile)->y = ((int16_t) (ycoord))

#define XV_TILE_GET_EDGE_WIDTH(pTile)                          (((pTile)->tileEdgeLeft < (pTile)->tileEdgeRight) ? (pTile)->tileEdgeLeft : (pTile)->tileEdgeRight)
#define XV_TILE_SET_EDGE_WIDTH(pTile, edgeWidth)               ((pTile)->tileEdgeLeft = (pTile)->tileEdgeRight = ((uint8_t) (edgeWidth)))

#define XV_TILE_GET_EDGE_HEIGHT(pTile)                         (((pTile)->tileEdgeTop < (pTile)->tileEdgeBottom) ? (pTile)->tileEdgeTop : (pTile)->tileEdgeBottom)
#define XV_TILE_SET_EDGE_HEIGHT(pTile, edgeHeight)             ((pTile)->tileEdgeTop = (pTile)->tileEdgeBottom = ((uint8_t) (edgeHeight)))

#define XV_TILE_GET_EDGE_LEFT(pTile)                           ((pTile)->tileEdgeLeft)
#define XV_TILE_SET_EDGE_LEFT(pTile, edgeWidth)                (pTile)->tileEdgeLeft = ((uint8_t) (edgeWidth))

#define XV_TILE_GET_EDGE_RIGHT(pTile)                          ((pTile)->tileEdgeRight)
#define XV_TILE_SET_EDGE_RIGHT(pTile, edgeWidth)               (pTile)->tileEdgeRight = ((uint8_t) (edgeWidth))

#define XV_TILE_GET_EDGE_TOP(pTile)                            ((pTile)->tileEdgeTop)
#define XV_TILE_SET_EDGE_TOP(pTile, edgeHeight)                (pTile)->tileEdgeTop = (pTile)->tileEdgeBottom = ((uint8_t) (edgeHeight))

#define XV_TILE_GET_EDGE_BOTTOM(pTile)                         ((pTile)->tileEdgeBottom)
#define XV_TILE_SET_EDGE_BOTTOM(pTile, edgeHeight)             (pTile)->tileEdgeBottom = ((uint8_t) (edgeHeight))

#define XV_TILE_CHECK_STATUS_FLAGS_DMA_ONGOING(pTile)          (((pTile)->status & XV_TILE_STATUS_DMA_ONGOING) > 0)
#define XV_TILE_CHECK_STATUS_FLAGS_EDGE_PADDING_NEEDED(pTile)  (((pTile)->status & XV_TILE_STATUS_EDGE_PADDING_NEEDED) > 0)

/***********************************
*              Other Marcos
***********************************/

#define XV_TILE_CHECK_VIRTUAL_FRAME(pTile)    ((pTile)->pFrame->pFrameBuff == NULL)
#define XV_FRAME_CHECK_VIRTUAL_FRAME(pFrame)  ((pFrame)->pFrameBuff == NULL)

// Assumes pixel resolution of 8 bits
#define SETUP_TILE(pTile, pBuf, pFrame, width, height, type, edgeWidth, edgeHeight, x, y) \
  {                                                                                       \
    XV_TILE_SET_FRAME_PTR(pTile, pFrame);                                                 \
    XV_TILE_SET_BUFF_PTR(pTile, pBuf);                                                    \
    XV_TILE_SET_DATA_PTR(pTile, pBuf + edgeHeight * (width + 2 * edgeWidth) + edgeWidth); \
    XV_TILE_SET_BUFF_SIZE(pTile, (width + 2 * edgeWidth) * (height + 2 * edgeHeight));    \
    XV_TILE_SET_WIDTH(pTile, width);                                                      \
    XV_TILE_SET_HEIGHT(pTile, height);                                                    \
    XV_TILE_SET_PITCH(pTile, width + 2 * edgeWidth);                                      \
    XV_TILE_SET_TYPE(pTile, type);                                                        \
    XV_TILE_SET_EDGE_WIDTH(pTile, edgeWidth);                                             \
    XV_TILE_SET_EDGE_HEIGHT(pTile, edgeHeight);                                           \
    XV_TILE_SET_X_COORD(pTile, x);                                                        \
    XV_TILE_SET_Y_COORD(pTile, y);                                                        \
  }

// Assumes pixel resolution of 8 bits
/*
#define SETUP_FRAME(pFrame, pFrameBuffer, width, height, padWidth, padHeight, pixRes, ppf, paddingType)         \
  {                                                                                                             \
    XV_FRAME_SET_BUFF_PTR(pFrame, pFrameBuffer);                                                                \
    XV_FRAME_SET_BUFF_SIZE(pFrame, (width + 2 * padWidth) * (height + 2 * padHeight));                          \
    XV_FRAME_SET_WIDTH(pFrame, width);                                                                          \
    XV_FRAME_SET_HEIGHT(pFrame, height);                                                                        \
    XV_FRAME_SET_PITCH(pFrame, width + 2 * padWidth);                                                           \
    XV_FRAME_SET_PIXEL_RES(pFrame, pixRes);                                                                     \
    XV_FRAME_SET_DATA_PTR(pFrame, pFrameBuffer + (XV_FRAME_GET_PITCH(pFrame) * padHeight + padWidth) * pixRes); \
    XV_FRAME_SET_EDGE_WIDTH(pFrame, padWidth);                                                                  \
    XV_FRAME_SET_EDGE_HEIGHT(pFrame, padHeight);                                                                \
    XV_FRAME_SET_PIXEL_FORMAT(pFrame, ppf);                                                                     \
    XV_FRAME_SET_PADDING_TYPE(pFrame, paddingType);                                                             \
  }
 */
// Assumes pixel resolution of 8 bits
#define SETUP_FRAME(pFrame, pFrameBuffer, width, height, padWidth, padHeight, stride, pixRes, ppf, paddingType)         \
  {                                                                                                             \
    XV_FRAME_SET_BUFF_PTR(pFrame, pFrameBuffer);                                                                \
    XV_FRAME_SET_BUFF_SIZE(pFrame, stride * (height + 2 * padHeight));                          \
    XV_FRAME_SET_WIDTH(pFrame, width);                                                                          \
    XV_FRAME_SET_HEIGHT(pFrame, height);                                                                        \
    XV_FRAME_SET_PITCH(pFrame, stride);                                                           \
    XV_FRAME_SET_PIXEL_RES(pFrame, pixRes);                                                                     \
    XV_FRAME_SET_DATA_PTR(pFrame, pFrameBuffer + (XV_FRAME_GET_PITCH(pFrame) * padHeight + padWidth) * pixRes); \
    XV_FRAME_SET_EDGE_WIDTH(pFrame, padWidth);                                                                  \
    XV_FRAME_SET_EDGE_HEIGHT(pFrame, padHeight);                                                                \
    XV_FRAME_SET_PIXEL_FORMAT(pFrame, ppf);                                                                     \
    XV_FRAME_SET_PADDING_TYPE(pFrame, paddingType);                                                             \
  }

#define WAIT_FOR_TILE(pxvTM, pTile)                      \
  {                                                      \
    int32_t transferDone;                                \
    transferDone = xvCheckTileReady((pxvTM), (pTile));   \
    while (transferDone == 0)                            \
    {                                                    \
      transferDone = xvCheckTileReady((pxvTM), (pTile)); \
    }                                                    \
  }

// Assumes 8 bit pixRes
#define XV_TILE_UPDATE_EDGE_HEIGHT(pTile, newEdgeHeight)                          \
  {                                                                               \
    uint16_t currEdgeHeight = (uint16_t) XV_TILE_GET_EDGE_HEIGHT(pTile);          \
    uint32_t tilePitch      = (uint32_t) XV_TILE_GET_PITCH(pTile);                \
    uint32_t tileHeight     = (uint32_t) XV_TILE_GET_HEIGHT(pTile);               \
    uint32_t dataU32        = (uint32_t) XV_TILE_GET_DATA_PTR(pTile);             \
    dataU32 = dataU32 + tilePitch * (newEdgeHeight - currEdgeHeight);             \
    XV_TILE_SET_DATA_PTR(pTile, (void *) dataU32);                                \
    XV_TILE_SET_EDGE_HEIGHT(pTile, newEdgeHeight);                                \
    XV_TILE_SET_HEIGHT(pTile, tileHeight + 2 * (currEdgeHeight - newEdgeHeight)); \
  }

// Assumes 8 bit pixRes
#define XV_TILE_UPDATE_EDGE_WIDTH(pTile, newEdgeWidth)                 \
  {                                                                    \
    uint16_t currEdgeWidth = (uint16_t) XV_TILE_GET_EDGE_WIDTH(pTile); \
    uint32_t tilePitch     = (uint32_t) XV_TILE_GET_PITCH(pTile);      \
    uint32_t dataU32       = (uint32_t) XV_TILE_GET_DATA_PTR(pTile);   \
    dataU32 = dataU32 + newEdgeWidth - currEdgeWidth;                  \
    XV_TILE_SET_DATA_PTR(pTile, (void *) dataU32);                     \
    XV_TILE_SET_EDGE_WIDTH(pTile, newEdgeWidth);                       \
    XV_TILE_SET_WIDTH(pTile, tilePitch - 2 * newEdgeWidth);            \
  }

#define XV_TILE_UPDATE_DIMENSIONS(pTile, x, y, w, h, p) \
  {                                                     \
    XV_TILE_SET_X_COORD(pTile, x);                      \
    XV_TILE_SET_Y_COORD(pTile, y);                      \
    XV_TILE_SET_WIDTH(pTile, w);                        \
    XV_TILE_SET_HEIGHT(pTile, h);                       \
    XV_TILE_SET_PITCH(pTile, p);                        \
  }

#define XV_IS_TILE_OK(t)                                                                                                                                                                                                                                                                \
  ((t) &&                                                                                                                                                                                                                                                                               \
   (XV_TILE_GET_DATA_PTR(t)) &&                                                                                                                                                                                                                                                         \
   (XV_TILE_GET_BUFF_PTR(t)) &&                                                                                                                                                                                                                                                         \
   (XV_TILE_GET_PITCH(t) >= XV_TILE_GET_WIDTH(t) + XV_TILE_GET_EDGE_WIDTH(t) * 2) &&                                                                                                                                                                                                    \
   ((uint8_t *) XV_TILE_GET_DATA_PTR(t) - (XV_TILE_GET_EDGE_WIDTH(t) + XV_TILE_GET_PITCH(t) * XV_TILE_GET_EDGE_HEIGHT(t)) * XV_FRAME_GET_PIXEL_RES(XV_TILE_GET_FRAME_PTR(t)) * XV_FRAME_GET_PIXEL_FORMAT(XV_TILE_GET_FRAME_PTR(t))                                                      \
    >= (uint8_t *) XV_TILE_GET_BUFF_PTR(t)) &&                                                                                                                                                                                                                                          \
   ((uint8_t *) XV_TILE_GET_DATA_PTR(t) + (XV_TILE_GET_PITCH(t) * (XV_TILE_GET_HEIGHT(t) + XV_TILE_GET_EDGE_HEIGHT(t) - 1) + XV_TILE_GET_WIDTH(t) + XV_TILE_GET_EDGE_WIDTH(t)) * XV_FRAME_GET_PIXEL_RES(XV_TILE_GET_FRAME_PTR(t)) * XV_FRAME_GET_PIXEL_FORMAT(XV_TILE_GET_FRAME_PTR(t)) \
    <= (uint8_t *) XV_TILE_GET_BUFF_PTR(t) + XV_TILE_GET_BUFF_SIZE(t)))


/***********************************
*              Function  Prototypes
***********************************/

// Initializes iDMA
// pxvTM            - Tile manager object
// buf              - iDMA object and buffer
// numDesc          - Number of descriptors
// maxBlock         - Maximum block size
// maxPifReq        - Maximum pif request
// errCallbackFunc  - Callback function for iDMA interrupt error handling
// cbFunc           - Callback function for iDMA interrupt handling
// cbData           - Data required for interrupt callback function, cbFunc
// Returns -1 if it encounters an error, else returns 0
int32_t  xvInitIdma(xvTileManager *pxvTM, idma_buffer_t *buf, int32_t numDescs, int32_t maxBlock,
                    int32_t maxPifReq, idma_err_callback_fn errCallbackFunc, idma_callback_fn cbFunc, void * cbData);

int32_t xvIdmaCopyData(void *dst, void *src, size_t size,int32_t interruptOnCompletion);
int32_t xvCheckFor1DIdmaIndex(int32_t index);

// Initializes tile manager
// pxvTM        - Tile manager object
// pdmaObj      - iDMA buffer
// Returns -1 if it encounters an error, else returns 0
int32_t xvInitTileManager(xvTileManager *pxvTM, void *pdmaObj);


// Initializes memory manager
// pxvTM         - Tile manager object
// numMemBanks   - Number of memory pools
// pBankBuffPool - Array of start addresses of memory bank
// buffPoolSize  - Array of sizes of memory bank
// Returns -1 if it encounters an error, else returns 0
int32_t xvInitMemAllocator(xvTileManager *pxvTM, int32_t numMemBanks, void **pBankBuffPool, int32_t* buffPoolSize);


// Allocates buffer from the pool
// pxvTM         - Tile manager object
// buffSize      - size of requested buffer
// buffColor     - color/index of requested bufffer
// buffAlignment - Alignment of requested buffer
// Returns the buffer with requested parameters. If an error occurs, returns ((void *)(-1))
void *xvAllocateBuffer(xvTileManager *pxvTM, int32_t buffSize, int32_t buffColor, int32_t buffAlignment);


// Releases the given buffer
// pxvTM - Tile manager object
// pBuff - Input buffer that needs to be released
// Returns -1 if it encounters an error, else returns 0
int32_t xvFreeBuffer(xvTileManager *pxvTM, void *pBuff);


// Releases all buffers. Reinitializes the memory allocator
// pxvTM - Tile manager object
// Returns -1 if it encounters an error, else returns 0
int32_t xvFreeAllBuffers(xvTileManager *pxvTM);


// Allocates single frame
// pxvTM - Tile manager object
// Returns the pointer to allocated frame. Does not allocate frame data buffer.
// Returns ((xvFrame *)(-1)) if it encounters an error.
xvFrame *xvAllocateFrame(xvTileManager *pxvTM);


// Releases given frame
// pxvTM  - Tile manager object
// pFrame - Frame that needs to be released. Does not release buffer
// Returns -1 if it encounters an error, else returns 0
int32_t xvFreeFrame(xvTileManager *pxvTM, xvFrame *pFrame);


// Releases all allocated frames
// pxvTM  - Tile manager object
// Returns -1 if it encounters an error, else returns 0
int32_t  xvFreeAllFrames(xvTileManager *pxvTM);


// Allocates single tile
// pxvTM - Tile manager object
// Returns the pointer to allocated tile. Does not allocate tile data buffer
// Returns ((xvTile *)(-1)) if it encounters an error.
xvTile *xvAllocateTile(xvTileManager *pxvTM);


// Releases given tile
// pxvTM  - Tile manager object
// pFrame - Tile that needs to be released. Does not release buffer
// Returns -1 if it encounters an error, else returns 0
int32_t xvFreeTile(xvTileManager *pxvTM, xvTile *pTile);


// Releases all allocated tiles
// pxvTM  - Tile manager object
// Returns -1 if it encounters an error, else returns 0
int32_t xvFreeAllTiles(xvTileManager *pxvTM);


// Add iDMA transfer request
// pxvTM                 - Tile manager object
// dst                   - pointer to destination buffer
// src                   - pointer to source buffer
// rowSize               - number of bytes to transfer in a row
// numRows               - number of rows to transfer
// srcPitch              - source buffer's pitch in bytes
// dstPitch              - destination buffer's pitch in bytes
// interruptOnCompletion - if it is set, iDMA will interrupt after completing transfer
// Returns dmaIndex for this request. It returns -1 if it encounters an error
int32_t xvAddIdmaRequest(xvTileManager *pxvTM, void *dst, void *src, size_t rowSize,
                         int32_t numRows, int32_t srcPitch, int32_t dsPitch, int32_t interruptOnCompletion);


// Requests data transfer from frame present in system memory to local tile memory
// pxvTM          - Tile manager object
// pTile          - destination tile
// pPrevTile - data is copied from this tile to pTile if the buffer overlaps
// interruptOnCompletion - if it is set, iDMA will interrupt after completing transfer
// Returns -1 if it encounters an error, else it returns 0
int32_t xvReqTileTransferIn(xvTileManager *pxvTM, xvTile *pTile, xvTile *pPrevTile, int32_t interruptOnCompletion);


// Requests data transfer from tile present in local memory to frame in system memory
// pxvTM - Tile manager object
// pTile - source tile
// interruptOnCompletion - if it is set, iDMA will interrupt after completing transfer
// Returns -1 if it encounters an error, else it returns 0
int32_t xvReqTileTransferOut(xvTileManager *pxvTM, xvTile *pTile, int32_t interruptOnCompletion);


// Check if dma transfer is done
// pxvTM - Tile manager object
// index - index for dma transfer request
// Returns -1 if an error occurs
int32_t xvCheckForIdmaIndex(xvTileManager *pxvTM, int32_t index);


// Check if tile is ready
// pxvTM - Tile manager object
// pTile - input tile
// Takes care of padding, tile reuse
// Completes all tile transfers before the input tile
// Returns 1 if tile is ready, else returns 0
// Returns -1 if an error occurs
int32_t xvCheckTileReady(xvTileManager *pxvTM, xvTile *pTile);


// Check if input tile is free.
// Returns 1 if tile is free, else returns 0
// Returns -1 if an error occurs
// A tile is said to be free if all data transfers pertaining to data resue from this tile is completed
// pxvTM - Tile manager object
// pTile - output tile
int32_t xvCheckInputTileFree(xvTileManager *pxvTM, xvTile *pTile);

// Prints the most recent error information.
// It returns the most recent error code.
xvError_t xvGetErrorInfo(xvTileManager *pxvTM);

#endif

