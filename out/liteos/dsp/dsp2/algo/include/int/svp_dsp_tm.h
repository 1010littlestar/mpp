/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP encapsulation tile manager header file
 * Author: svp group
 * Create: 2017-10-17
 */

#ifndef __SVP_DSP_TM_H__
#define __SVP_DSP_TM_H__

#include "tileManager.h"
/*
* Tile type define
*/
#define SVP_DSP_MAKETYPE   XV_MAKETYPE
#define SVP_DSP_CUSTOMTYPE XV_CUSTOMTYPE

#define SVP_DSP_U8  XV_U8
#define SVP_DSP_U16 XV_U16
#define SVP_DSP_U32 XV_U32

#define SVP_DSP_S8  XV_S8
#define SVP_DSP_S16 XV_S16
#define SVP_DSP_S32 XV_S32

#define SVP_DSP_ARRAY_U8  XV_ARRAY_U8
#define SVP_DSP_ARRAY_S8  XV_ARRAY_S8
#define SVP_DSP_ARRAY_U16 XV_ARRAY_U16
#define SVP_DSP_ARRAY_S16 XV_ARRAY_S16
#define SVP_DSP_ARRAY_U32 XV_ARRAY_U32
#define SVP_DSP_ARRAY_S32 XV_ARRAY_S32

#define SVP_DSP_TILE_U8  XV_TILE_U8
#define SVP_DSP_TILE_S8  XV_TILE_S8
#define SVP_DSP_TILE_U16 XV_TILE_U16
#define SVP_DSP_TILE_S16 XV_TILE_S16
#define SVP_DSP_TILE_U32 XV_TILE_U32
#define SVP_DSP_TILE_S32 XV_TILE_S32

/*
 * Frame define
 */
#define SVP_DSP_FRAME_GET_BUFF_PTR XV_FRAME_GET_BUFF_PTR
#define SVP_DSP_FRAME_SET_BUFF_PTR XV_FRAME_SET_BUFF_PTR

#define SVP_DSP_FRAME_GET_BUFF_SIZE XV_FRAME_GET_BUFF_SIZE
#define SVP_DSP_FRAME_SET_BUFF_SIZE XV_FRAME_SET_BUFF_SIZE

#define SVP_DSP_FRAME_GET_DATA_PTR XV_FRAME_GET_DATA_PTR
#define SVP_DSP_FRAME_SET_DATA_PTR XV_FRAME_SET_DATA_PTR

#define SVP_DSP_FRAME_GET_WIDTH XV_FRAME_GET_WIDTH
#define SVP_DSP_FRAME_SET_WIDTH XV_FRAME_SET_WIDTH

#define SVP_DSP_FRAME_GET_HEIGHT XV_FRAME_GET_HEIGHT
#define SVP_DSP_FRAME_SET_HEIGHT XV_FRAME_SET_HEIGHT

#define SVP_DSP_FRAME_GET_PITCH          XV_FRAME_GET_PITCH
#define SVP_DSP_FRAME_SET_PITCH          XV_FRAME_SET_PITCH
#define SVP_DSP_FRAME_GET_PITCH_IN_BYTES XV_FRAME_GET_PITCH_IN_BYTES

#define SVP_DSP_FRAME_GET_PIXEL_RES XV_FRAME_GET_PIXEL_RES
#define SVP_DSP_FRAME_SET_PIXEL_RES XV_FRAME_SET_PIXEL_RES

#define SVP_DSP_FRAME_GET_PIXEL_FORMAT XV_FRAME_GET_PIXEL_FORMAT
#define SVP_DSP_FRAME_SET_PIXEL_FORMAT XV_FRAME_SET_PIXEL_FORMAT

#define SVP_DSP_FRAME_GET_EDGE_WIDTH XV_FRAME_GET_EDGE_WIDTH
#define SVP_DSP_FRAME_SET_EDGE_WIDTH XV_FRAME_SET_EDGE_WIDTH

#define SVP_DSP_FRAME_GET_EDGE_HEIGHT XV_FRAME_GET_EDGE_HEIGHT
#define SVP_DSP_FRAME_SET_EDGE_HEIGHT XV_FRAME_SET_EDGE_HEIGHT

#define SVP_DSP_FRAME_GET_EDGE_LEFT XV_FRAME_GET_EDGE_LEFT
#define SVP_DSP_FRAME_SET_EDGE_LEFT XV_FRAME_SET_EDGE_LEFT

#define SVP_DSP_FRAME_GET_EDGE_RIGHT XV_FRAME_GET_EDGE_RIGHT
#define SVP_DSP_FRAME_SET_EDGE_RIGHT XV_FRAME_SET_EDGE_RIGHT

#define SVP_DSP_FRAME_GET_EDGE_TOP XV_FRAME_GET_EDGE_TOP
#define SVP_DSP_FRAME_SET_EDGE_TOP XV_FRAME_SET_EDGE_TOP

#define SVP_DSP_FRAME_GET_EDGE_BOTTOM XV_FRAME_GET_EDGE_BOTTOM
#define SVP_DSP_FRAME_SET_EDGE_BOTTOM XV_FRAME_SET_EDGE_BOTTOM

#define SVP_DSP_FRAME_GET_PADDING_TYPE XV_FRAME_GET_PADDING_TYPE
#define SVP_DSP_FRAME_SET_PADDING_TYPE XV_FRAME_SET_PADDING_TYPE

/*
* Tile define
*/
#define SVP_DSP_TILE_GET_BUFF_PTR XV_TILE_GET_BUFF_PTR
#define SVP_DSP_TILE_SET_BUFF_PTR XV_TILE_SET_BUFF_PTR

#define SVP_DSP_TILE_GET_BUFF_SIZE XV_TILE_GET_BUFF_SIZE
#define SVP_DSP_TILE_SET_BUFF_SIZE XV_TILE_SET_BUFF_SIZE

#define SVP_DSP_TILE_GET_DATA_PTR XV_TILE_GET_DATA_PTR
#define SVP_DSP_TILE_SET_DATA_PTR XV_TILE_SET_DATA_PTR

#define SVP_DSP_TILE_GET_WIDTH XV_TILE_GET_WIDTH
#define SVP_DSP_TILE_SET_WIDTH XV_TILE_SET_WIDTH

#define SVP_DSP_TILE_GET_PITCH XV_TILE_GET_PITCH
#define SVP_DSP_TILE_SET_PITCH XV_TILE_SET_PITCH

#define SVP_DSP_TILE_GET_HEIGHT XV_TILE_GET_HEIGHT
#define SVP_DSP_TILE_SET_HEIGHT XV_TILE_SET_HEIGHT

#define SVP_DSP_TILE_GET_STATUS_FLAGS XV_TILE_GET_STATUS_FLAGS
#define SVP_DSP_TILE_SET_STATUS_FLAGS XV_TILE_SET_STATUS_FLAGS

#define SVP_DSP_TILE_GET_TYPE XV_TILE_GET_TYPE
#define SVP_DSP_TILE_SET_TYPE XV_TILE_SET_TYPE

#define SVP_DSP_TILE_GET_ELEMENT_TYPE XV_TILE_GET_ELEMENT_TYPE
#define SVP_DSP_TILE_GET_ELEMENT_SIZE XV_TILE_GET_ELEMENT_SIZE
#define SVP_DSP_TILE_IS_TILE          XV_TILE_IS_TILE

#define SVP_DSP_TYPE_ELEMENT_SIZE XV_TYPE_ELEMENT_SIZE
#define SVP_DSP_TYPE_ELEMENT_TYPE XV_TYPE_ELEMENT_TYPE
#define SVP_DSP_TYPE_IS_TILE      XV_TYPE_IS_TILE
#define SVP_DSP_TYPE_IS_SIGNED    XV_TYPE_IS_SIGNED

#define SVP_DSP_TILE_GET_FRAME_PTR XV_TILE_GET_FRAME_PTR
#define SVP_DSP_TILE_SET_FRAME_PTR XV_TILE_SET_FRAME_PTR

#define SVP_DSP_TILE_GET_X_COORD XV_TILE_GET_X_COORD
#define SVP_DSP_TILE_SET_X_COORD XV_TILE_SET_X_COORD

#define SVP_DSP_TILE_GET_Y_COORD XV_TILE_GET_Y_COORD
#define SVP_DSP_TILE_SET_Y_COORD XV_TILE_SET_Y_COORD

#define SVP_DSP_TILE_GET_EDGE_WIDTH XV_TILE_GET_EDGE_WIDTH
#define SVP_DSP_TILE_SET_EDGE_WIDTH XV_TILE_SET_EDGE_WIDTH

#define SVP_DSP_TILE_GET_EDGE_HEIGHT XV_TILE_GET_EDGE_HEIGHT
#define SVP_DSP_TILE_SET_EDGE_HEIGHT XV_TILE_SET_EDGE_HEIGHT

#define SVP_DSP_TILE_GET_EDGE_LEFT XV_TILE_GET_EDGE_LEFT
#define SVP_DSP_TILE_SET_EDGE_LEFT XV_TILE_SET_EDGE_LEFT

#define SVP_DSP_TILE_GET_EDGE_RIGHT XV_TILE_GET_EDGE_RIGHT
#define SVP_DSP_TILE_SET_EDGE_RIGHT XV_TILE_SET_EDGE_RIGHT

#define SVP_DSP_TILE_GET_EDGE_TOP XV_TILE_GET_EDGE_TOP
#define SVP_DSP_TILE_SET_EDGE_TOP XV_TILE_SET_EDGE_TOP

#define SVP_DSP_TILE_GET_EDGE_BOTTOM XV_TILE_GET_EDGE_BOTTOM
#define SVP_DSP_TILE_SET_EDGE_BOTTOM XV_TILE_SET_EDGE_BOTTOM

/*
 * Set up tile by type
 */
#define SVP_DSP_SETUP_TILE_BY_TYPE(pTile, pBuf, pFrame, width, height, type, edgeWidth, edgeHeight, x, y)        \
    {                                                                                                            \
        SVP_DSP_TILE_SET_FRAME_PTR((pTile), (pFrame));                                                           \
        SVP_DSP_TILE_SET_BUFF_PTR((pTile), (pBuf));                                                              \
        SVP_DSP_TILE_SET_DATA_PTR((pTile), (pBuf) + ((edgeHeight) * ((width) + 2 * (edgeWidth)) + (edgeWidth))\
        * SVP_DSP_TYPE_ELEMENT_SIZE((type)));                                                                   \
        SVP_DSP_TILE_SET_BUFF_SIZE((pTile), ((width) + 2 * (edgeWidth)) * ((height) + 2 * (edgeHeight))         \
        * SVP_DSP_TYPE_ELEMENT_SIZE((type)));                                                                    \
        SVP_DSP_TILE_SET_WIDTH((pTile), (width));                                                                 \
        SVP_DSP_TILE_SET_HEIGHT((pTile), (height));                                                               \
        SVP_DSP_TILE_SET_PITCH((pTile), (width) + 2 * (edgeWidth));                                               \
        SVP_DSP_TILE_SET_TYPE((pTile), (type));                                                                   \
        SVP_DSP_TILE_SET_EDGE_WIDTH((pTile), (edgeWidth));                                                        \
        SVP_DSP_TILE_SET_EDGE_HEIGHT((pTile), (edgeHeight));                                                      \
        SVP_DSP_TILE_SET_X_COORD((pTile), (x));                                                                   \
        SVP_DSP_TILE_SET_Y_COORD((pTile), (y));                                                                   \
    }

#define SVP_DSP_MOVE_X_TO_Y(indX, indY, tileWidth, tileHeight, imageWidth, imageHeight)   \
    {                                                                                     \
        (indX) += (tileWidth);                                                            \
        if (((indX) >= (imageWidth)) || (((indX) + (tileWidth)) > (imageWidth))) {          \
            (indY) += (tileHeight);                                                       \
            (indX) = 0;                                                                   \
            if (((indY) >= (imageHeight)) || (((indY) + (tileHeight)) > (imageHeight))) { \
                (indY) = 0;                                                               \
            }                                                                             \
        }                                                                                 \
    }

#define SVP_DSP_MOVE(start, len) ((start) += (len))

/*****************************************************************************
*   Prototype    : SVP_DSP_TM_Init
*   Description  : DSP Tile manager init.
*   Parameters   :
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
HI_S32 SVP_DSP_TM_Init(HI_VOID);

/*****************************************************************************
*   Prototype    : SVP_DSP_TM_Exit
*   Description  : DSP Tile manager uninit.
*   Parameters   :
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
HI_S32 SVP_DSP_TM_Exit(HI_VOID);

/*****************************************************************************
*   Prototype    : SVP_DSP_AllocateFrames
*   Description  : DSP Allocate frames.
*   Parameters   : SVP_DSP_DST_FRAME_S      **ppstFrm   Frame
*                  HI_U32                   u32Num      Number
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
HI_S32 SVP_DSP_AllocateFrames(SVP_DSP_DST_FRAME_S **ppstFrm, HI_U32 u32Num);

/*****************************************************************************
*   Prototype    : SVP_DSP_FreeFrames
*   Description  : DSP Free frames.
*   Parameters   : SVP_DSP_FRAME_S      **ppstFrm   Frame
*                  HI_U32                   u32Num      Number
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
HI_S32 SVP_DSP_FreeFrames(SVP_DSP_FRAME_S **ppstFrm, HI_U32 u32Num);

/*****************************************************************************
*   Prototype    : SVP_DSP_FreeAllFrames
*   Description  : DSP Free all frames.
*   Parameters   :
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
HI_S32 SVP_DSP_FreeAllFrames(HI_VOID);

/*****************************************************************************
*   Prototype    : SVP_DSP_AllocateBuffers
*   Description  : DSP Allocate buffers.
*   Parameters   : HI_VOID        **ppvBuff       Buffer
*                  HI_U32         u32Num	       Buffer number
*                  HI_S32         s32BuffSize     Buffer size
*                  HI_S32         s32BuffColor    Bank
*                  HI_S32         s32BuffAlign  align
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
HI_S32 SVP_DSP_AllocateBuffers(HI_VOID **ppvBuff, HI_U32 u32Num, HI_S32 s32BuffSize,
                               HI_S32 s32BuffColor, HI_S32 s32BuffAlign);

/*****************************************************************************
*   Prototype    : SVP_DSP_FreeBuffers
*   Description  : DSP Free buffers.
*   Parameters   : HI_VOID            **ppvBuff       Buffer
*                  HI_U32             u32Num          Buffer number
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
HI_S32 SVP_DSP_FreeBuffers(HI_VOID **ppvBuff, HI_U32 u32Num);

/*****************************************************************************
*   Prototype    : SVP_DSP_AllocateTiles
*   Description  : DSP Allocate tiles.
*   Parameters   : SVP_DSP_SRC_TILE_S      **ppstTile   Tile
*                  HI_U32                  u32Num       Tile number
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
HI_S32 SVP_DSP_AllocateTiles(SVP_DSP_SRC_TILE_S **ppstTile, HI_U32 u32Num);

/*****************************************************************************
*   Prototype    : SVP_DSP_FreeTiles
*   Description  : DSP Free tiles.
*   Parameters   : SVP_DSP_TILE_S      **ppstTile   Tile
*                  HI_U32              u32Num       Tile number
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
HI_S32 SVP_DSP_FreeTiles(SVP_DSP_TILE_S **ppstTile, HI_U32 u32Num);

/*****************************************************************************
*   Prototype    : SVP_DSP_FreeAllTiles
*   Description  : DSP Free all tiles.
*   Parameters   :
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
HI_S32 SVP_DSP_FreeAllTiles(HI_VOID);

/*****************************************************************************
*   Prototype    : SVP_DSP_CopyDataByIdma
*   Description  : DSP Copy data by idma.
*   Parameters   : HI_VOID          *pvDst                     Dst
*                  HI_VOID          *pvSrc                     Src
*                  HI_S32           s32Size	                    Size
*                  HI_S32           s32IntOnCompletion         Flag
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
HI_S32 SVP_DSP_CopyDataByIdma(HI_VOID *pvDst, HI_VOID *pvSrc, HI_S32 s32Size, HI_S32 s32IntOnCompletion);

/*****************************************************************************
*   Prototype    : SVP_DSP_ReqTileTransferIn
*   Description  : DSP Request tile transfer from system memory to local memory
*   Parameters   : SVP_DSP_SRC_TILE_S     *pstTile                 Current tile
*                  SVP_DSP_SRC_TILE_S     *pstPrevTile             Prev tile
*                  HI_S32                 s32IntOnCompletion       Flag
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
HI_S32 SVP_DSP_ReqTileTransferIn(SVP_DSP_SRC_TILE_S *pstTile, SVP_DSP_SRC_TILE_S *pstPrevTile,
                                 HI_S32 s32IntOnCompletion);

/*****************************************************************************
*   Prototype    : SVP_DSP_ReqTileTransferOut
*   Description  : DSP Request tile transfer from local memory to system memory
*   Parameters   : SVP_DSP_SRC_TILE_S     *pstTile                 Tile
*                  HI_S32                 s32IntOnCompletion       Flag
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
HI_S32 SVP_DSP_ReqTileTransferOut(SVP_DSP_SRC_TILE_S *pstTile, HI_S32 s32IntOnCompletion);

/*****************************************************************************
*   Prototype    : SVP_DSP_CheckTileReady
*   Description  : DSP Check tile is finish
*   Parameters   : SVP_DSP_SRC_TILE_S     *pstTile                 Tile
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
HI_S32 SVP_DSP_CheckTileReady(SVP_DSP_SRC_TILE_S *pstTile);

/*****************************************************************************
*   Prototype    : SVP_DSP_GetErrorInfo
*   Description  : DSP Get error info
*   Parameters   :
*
*
*   Return Value : HI_CHAR*
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
HI_CHAR *SVP_DSP_GetErrorInfo(HI_VOID);

/*****************************************************************************
*   Prototype    : SVP_DSP_WaitForTile
*   Description  : DSP Wait for tile
*   Parameters   : SVP_DSP_SRC_TILE_S     *pstTile                 Tile
*
*
*   Return Value : HI_VOID
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
HI_VOID SVP_DSP_WaitForTile(SVP_DSP_SRC_TILE_S *pstTile);

#endif /* __SVP_DSP_TM_H__ */

