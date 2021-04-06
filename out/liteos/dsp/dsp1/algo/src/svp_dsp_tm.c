/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP encapsulation tile manager implement file
 * Author: svp group
 * Create: 2017-10-17
 */

#include "hi_dsp.h"
#include "svp_dsp_def.h"
#include "svp_dsp.h"
#include "svp_dsp_tm.h"
#include "hi_comm_svp.h"
#include "svp_dsp_trace.h"
#include "tileManager.h"

#include <stdio.h>

/*
* IDMA Interrupt callback data struct
*/
typedef struct hiSVP_DSP_INT_CB_DATA_S {
    HI_U32 u32IntCount;
} SVP_DSP_INT_CB_DATA_S;

static xvTileManager s_stXvTmObj _LOCAL_DRAM1_;
IDMA_BUFFER_DEFINE(s_pvIdmaObjBuff, SVP_DSP_DMA_DESCR_CNT, IDMA_2D_DESC);

static SVP_DSP_INT_CB_DATA_S s_stCbData _LOCAL_DRAM0_;
static xvTileManager *s_pstXvTm = &s_stXvTmObj;

/*
* Memory banks, used by tile manager's memory allocator
*/
static HI_U8 ALIGN64 s_au8BankBuffPool0[SVP_DSP_POOL_SIZE] _LOCAL_DRAM0_;
static HI_U8 ALIGN64 s_au8BankBuffPool1[SVP_DSP_POOL_SIZE] _LOCAL_DRAM1_;

/*****************************************************************************
*   Prototype    : SVP_DSP_ErrCallbackFunc
*   Description  : DSP IDMA error callback function
*   Parameters   : idma_error_details_t* pstData   Error datail
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
static void SVP_DSP_ErrCallbackFunc(idma_error_details_t *pstData)
{
    HI_TRACE_SVP_DSP(HI_DBG_ERR, "ERROR CALLBACK: iDMA in Error!\n");
    HI_TRACE_SVP_DSP(HI_DBG_ERR, "COPY FAILED, Error %d at desc:%p, PIF src/dst=%x/%x\n", pstData->err_type,
                     (void *)pstData->currDesc, pstData->srcAddr, pstData->dstAddr);
}

/*****************************************************************************
*   Prototype    : SVP_DSP_IntCallbackFunc
*   Description  : DSP IDMA Interrupt callback function
*   Parameters   : void    *pvCallBackStr        CallBack
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
static void SVP_DSP_IntCallbackFunc(void *pvCallBackStr)
{
    ((SVP_DSP_INT_CB_DATA_S *)pvCallBackStr)->u32IntCount++;

    return;
}

/*****************************************************************************
*   Prototype    : SVP_DSP_XvErr2SvpErr
*   Description  : XvErrType 2 SvpErrType.
*   Parameters   : HI_S32          s32XvErr      SvErrType
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
static HI_S32 SVP_DSP_XvErr2SvpErr(HI_S32 s32XvErr)
{
    HI_S32 s32SvpErr = s32XvErr;
    if (s32XvErr == XV_ERROR_SUCCESS) {
        s32SvpErr = HI_SUCCESS;
    } else if ((s32XvErr == XV_ERROR_TILE_MANAGER_NULL) || (s32XvErr == XV_ERROR_POINTER_NULL) ||
               (s32XvErr == XV_ERROR_FRAME_NULL) || (s32XvErr == XV_ERROR_TILE_NULL) ||
               (s32XvErr == XV_ERROR_BUFFER_NULL) || (s32XvErr == XV_ERROR_FRAME_BUFFER_FULL) ||
               (s32XvErr == XV_ERROR_TILE_BUFFER_FULL)) {
        s32SvpErr = HI_ERR_SVP_DSP_NULL_PTR;
    } else if (s32XvErr == XV_ERROR_ALLOC_FAILED) {
        s32SvpErr = HI_ERR_SVP_DSP_NOMEM;
    } else if ((s32XvErr == XV_ERROR_DIMENSION_MISMATCH) || (s32XvErr == XV_ERROR_BUFFER_OVERFLOW)) {
        s32SvpErr = HI_ERR_SVP_DSP_ILLEGAL_PARAM;
    } else if (s32XvErr == XV_ERROR_BAD_ARG) {
        s32SvpErr = HI_ERR_SVP_DSP_BADADDR;
    } else {
        s32SvpErr = HI_FAILURE;
    }

    return s32SvpErr;
}

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
HI_S32 SVP_DSP_TM_Init(HI_VOID)
{
    HI_S32 s32Ret;
    void *apvBuffPool[SVP_DSP_PING_PONG_NUM];
    HI_S32 as32BuffSize[SVP_DSP_PING_PONG_NUM];

    // Initialize DMA and tile manager objects
    // Initialize the DMA
    s32Ret = xvInitIdma(s_pstXvTm, (idma_buffer_t *)s_pvIdmaObjBuff,
                        SVP_DSP_DMA_DESCR_CNT, MAX_BLOCK_8, SVP_DSP_MAX_PIF,
                        SVP_DSP_ErrCallbackFunc, SVP_DSP_IntCallbackFunc, (void *)&s_stCbData);
    if (s32Ret == XVTM_ERROR) {
        HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Init idma failed,%s!\n", SVP_DSP_GetErrorInfo());
        return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
    }
    // Initialize tile manager
    s32Ret = xvInitTileManager(s_pstXvTm, (void *)s_pvIdmaObjBuff);
    if (s32Ret == XVTM_ERROR) {
        HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Init tile manager failed,%s!\n", SVP_DSP_GetErrorInfo());
        return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
    }

    // Initializing Memory allocator
    apvBuffPool[0] = s_au8BankBuffPool0;
    apvBuffPool[1] = s_au8BankBuffPool1;
    as32BuffSize[0] = SVP_DSP_POOL_SIZE;
    as32BuffSize[1] = SVP_DSP_POOL_SIZE;
    s32Ret = xvInitMemAllocator(s_pstXvTm, SVP_DSP_PING_PONG_NUM, apvBuffPool, as32BuffSize);
    if (s32Ret == XVTM_ERROR) {
        HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Init mem allocator failed,%s!\n", SVP_DSP_GetErrorInfo());
        return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
    }

    return HI_SUCCESS;
}

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
HI_S32 SVP_DSP_TM_Exit(HI_VOID)
{
    return HI_SUCCESS;
}

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
HI_S32 SVP_DSP_AllocateFrames(SVP_DSP_DST_FRAME_S **ppstFrm, HI_U32 u32Num)
{
    HI_U32 i;
    for (i = 0; i < u32Num; i++) {
        ppstFrm[i] = xvAllocateFrame(s_pstXvTm);
        if ((HI_S32)ppstFrm[i] == XVTM_ERROR) {
            HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Allocate frames failed,%s!\n", SVP_DSP_GetErrorInfo());
            return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
        }
    }

    return HI_SUCCESS;
}

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
HI_S32 SVP_DSP_FreeFrames(SVP_DSP_FRAME_S **ppstFrm, HI_U32 u32Num)
{
    HI_S32 s32Ret;
    HI_U32 i;
    for (i = 0; i < u32Num; i++) {
        s32Ret = xvFreeFrame(s_pstXvTm, ppstFrm[i]);
        if (s32Ret == XVTM_ERROR) {
            HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Free frames failed,%s!\n", SVP_DSP_GetErrorInfo());
            return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
        }
        ppstFrm[i] = NULL;
    }

    return HI_SUCCESS;
}

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
HI_S32 SVP_DSP_FreeAllFrames(HI_VOID)
{
    HI_S32 s32Ret;
    s32Ret = xvFreeAllFrames(s_pstXvTm);
    if (s32Ret == XVTM_ERROR) {
        HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Free all frames failed,%s!\n", SVP_DSP_GetErrorInfo());
        return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
    }
    return HI_SUCCESS;
}

/*****************************************************************************
*   Prototype    : SVP_DSP_AllocateBuffers
*   Description  : DSP Allocate buffers.
*   Parameters   : HI_VOID        **ppvBuff   		    Buffer
*                  HI_U32         u32Num	   		    Buffer number
*                  HI_S32         s32BuffSize  		 Buffer size
*                  HI_S32         s32BuffColor		    Bank
*                  HI_S32         s32BuffAlign        Align
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
                               HI_S32 s32BuffColor, HI_S32 s32BuffAlign)
{
    HI_U32 i;
    for (i = 0; i < u32Num; i++) {
        ppvBuff[i] = xvAllocateBuffer(s_pstXvTm, s32BuffSize, s32BuffColor, s32BuffAlign);
        if ((HI_S32)ppvBuff[i] == XVTM_ERROR) {
            HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Allocate buffers failed,%s!\n", SVP_DSP_GetErrorInfo());
            return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
        }
    }
    return HI_SUCCESS;
}

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
HI_S32 SVP_DSP_FreeBuffers(HI_VOID **ppvBuff, HI_U32 u32Num)
{
    HI_U32 i;
    HI_S32 s32Ret;
    for (i = 0; i < u32Num; i++) {
        s32Ret = xvFreeBuffer(s_pstXvTm, ppvBuff[i]);
        if (s32Ret == XVTM_ERROR) {
            HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Free buffers failed,%s!\n", SVP_DSP_GetErrorInfo());
            return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
        }
        ppvBuff[i] = NULL;
    }
    return HI_SUCCESS;
}

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
HI_S32 SVP_DSP_AllocateTiles(SVP_DSP_SRC_TILE_S **ppstTile, HI_U32 u32Num)
{
    HI_U32 i;
    for (i = 0; i < u32Num; i++) {
        ppstTile[i] = xvAllocateTile(s_pstXvTm);
        if ((HI_S32)ppstTile[i] == XVTM_ERROR) {
            HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Allocate tiles failed,%s!\n", SVP_DSP_GetErrorInfo());
            return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
        }
    }
    return HI_SUCCESS;
}

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
HI_S32 SVP_DSP_FreeTiles(SVP_DSP_TILE_S **ppstTile, HI_U32 u32Num)
{
    HI_U32 i;
    HI_S32 s32Ret;
    for (i = 0; i < u32Num; i++) {
        s32Ret = xvFreeTile(s_pstXvTm, ppstTile[i]);
        if (s32Ret == XVTM_ERROR) {
            HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Free tile failed,%s!\n", SVP_DSP_GetErrorInfo());
            return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
        }
        ppstTile[i] = NULL;
    }
    return HI_SUCCESS;
}

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
HI_S32 SVP_DSP_FreeAllTiles(HI_VOID)
{
    HI_S32 s32Ret;
    s32Ret = xvFreeAllTiles(s_pstXvTm);
    if (s32Ret == XVTM_ERROR) {
        HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Free all tiles failed,%s!\n", SVP_DSP_GetErrorInfo());
        return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
    }
    return HI_SUCCESS;
}

/*****************************************************************************
*   Prototype    : SVP_DSP_CopyDataByIdma
*   Description  : DSP Copy data by idma.
*   Parameters   : HI_VOID          *pvDst                     Dst
*                  HI_VOID          *pvSrc                     Src
* 					 HI_S32           s32Size	                    Size
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
HI_S32 SVP_DSP_CopyDataByIdma(HI_VOID *pDst, HI_VOID *pSrc, HI_S32 s32Size, HI_S32 s32IntOnCompletion)
{
    HI_S32 s32IdmaIndex;
    HI_S32 s32DescDone = 0;
    HI_S32 s32Ret = HI_FAILURE;

    s32IdmaIndex = xvIdmaCopyData(pDst, pSrc, s32Size, s32IntOnCompletion);

    while (s32DescDone == 0) {
        s32DescDone = xvCheckFor1DIdmaIndex(s32IdmaIndex);
        if (s32DescDone == XVTM_ERROR) {
            s32Ret = HI_FAILURE;
            break;
        } else if (s32DescDone == 1) {
            s32Ret = HI_SUCCESS;
        } else {
        }
    }

    return s32Ret;
}

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
                                 HI_S32 s32IntOnCompletion)
{
    HI_S32 s32Ret;
    s32Ret = xvReqTileTransferIn(s_pstXvTm, pstTile, pstPrevTile, s32IntOnCompletion);
    if (s32Ret == XVTM_ERROR) {
        HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Request tile transfer in failed,%s!\n", SVP_DSP_GetErrorInfo());
        return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
    }

    return HI_SUCCESS;
}

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
HI_S32 SVP_DSP_ReqTileTransferOut(SVP_DSP_TILE_S *pstTile, HI_S32 s32IntOnCompletion)
{
    HI_S32 s32Ret;
    s32Ret = xvReqTileTransferOut(s_pstXvTm, pstTile, s32IntOnCompletion);
    if (s32Ret == XVTM_ERROR) {
        HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Request tile transfer out failed,%s!\n", SVP_DSP_GetErrorInfo());
        return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
    }

    return HI_SUCCESS;
}

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
HI_S32 SVP_DSP_CheckTileReady(SVP_DSP_TILE_S *pstTile)
{
    HI_S32 s32Ret;
    s32Ret = xvCheckTileReady(s_pstXvTm, pstTile);
    if (s32Ret == XVTM_ERROR) {
        HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,Check tile ready failed,%s!\n", SVP_DSP_GetErrorInfo());
        return SVP_DSP_XvErr2SvpErr(s_pstXvTm->errFlag);
    }

    return HI_SUCCESS;
}

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
HI_CHAR *SVP_DSP_GetErrorInfo(HI_VOID)
{
    if (s_pstXvTm == NULL) {
        return "Tile Manager pointer is NULL\n";
    }
    switch (s_pstXvTm->errFlag) {
        case XV_ERROR_SUCCESS: {
            return "No Errors detected\n";
        }
        break;
        case XV_ERROR_POINTER_NULL: {
            return "Pointer is NULL\n";
        }
        break;
        case XV_ERROR_FRAME_NULL: {
            return "Frame is NULL\n";
        }
        break;
        case XV_ERROR_TILE_NULL: {
            return "Tile is NULL\n";
        }
        break;
        case XV_ERROR_BUFFER_NULL: {
            return "Buffer is NULL\n";
        }
        break;
        case XV_ERROR_ALLOC_FAILED: {
            return "Buffer allocation failed\n";
        }
        break;
        case XV_ERROR_FRAME_BUFFER_FULL: {
            return "Frame buffer full. Try releasing unused frames\n";
        }
        break;
        case XV_ERROR_TILE_BUFFER_FULL: {
            return "Tile buffer full. Try releasing unused tiles\n";
        }
        break;
        case XV_ERROR_BAD_ARG: {
            return "Inconsistent arguments. Give correct arguments\n";
        }
        break;
        default:
        {
            return "Incorrect error flag\n";
        }
        break;
    }

    return "Incorrect error flag\n";
}

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
HI_VOID SVP_DSP_WaitForTile(SVP_DSP_TILE_S *pstTile)
{
    int32_t transferDone;
    transferDone = xvCheckTileReady((s_pstXvTm), (pstTile));
    while (transferDone == 0) {
        transferDone = xvCheckTileReady((s_pstXvTm), (pstTile));
    }
}

#if (defined CONFIG_HI_PHOTO_SUPPORT) || (defined CONFIG_HI_MONO_COLOR_FUSION_SUPPORT)
xvTileManager *SVP_DSP_GetTmObj()
{
    return &s_stXvTmObj;
}
unsigned char *SVP_DSP_GetBankBuffPool0()
{
    return s_au8BankBuffPool0;
}
unsigned char *SVP_DSP_GetBankBuffPool1()
{
    return s_au8BankBuffPool1;
}
#endif

