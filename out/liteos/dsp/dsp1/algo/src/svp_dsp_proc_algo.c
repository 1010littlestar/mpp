/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP process algorithm implement file
 * Author: svp group
 * Create: 2017-10-17
 */

#include "hi_dsp.h"
#include "svp_dsp_frm.h"
#include "svp_dsp_proc_algo.h"
#include "svp_dsp_algo_comm.h"
#include "svp_dsp_trace.h"

/*****************************************************************************
*   Prototype    : SvpDspParseAndFillParamForDilate3x3
*   Description  : Parse and fill kernel parameter for dilate 3x3
*   Input        : HI_U64     u64IdmaOffset   IDMA offset
*                  HI_U64     u64Body         Body
*                  HI_U32     u32BodyLen      Body len
*   Output       : SVP_DSP_SRC_FRAME_S *pstSrcFrm : output frame;
*                  SVP_DSP_DST_FRAME_S *pstDstFrm : output frame;
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
static HI_S32 SvpDspParseAndFillParamForDilate3x3(HI_U64 u64IdmaOffset, HI_U64 u64Body, HI_U32 u32BodyLen,
                                                  SVP_DSP_SRC_FRAME_S *pstSrcFrm, SVP_DSP_DST_FRAME_S *pstDstFrm)
{
    HI_S32 s32Ret;
    HI_U8 au8Param[SVP_DSP_1_K];
    HI_U32 u32BodyTmpLen = sizeof(SVP_IMAGE_S) * 2; /* Src+Dst */
    SVP_IMAGE_S *pstSrc = NULL;
    SVP_IMAGE_S *pstDst = NULL;
    HI_U32 au32PhyAddr[3];

    SVP_DSP_CHECK_EXPR_RET(u32BodyLen != u32BodyTmpLen, HI_ERR_SVP_DSP_ILLEGAL_PARAM, HI_DBG_ERR,
        "Error(%#x): The u32BodyLen(%u) must be equal to %u\n", HI_ERR_SVP_DSP_ILLEGAL_PARAM, u32BodyLen,
        u32BodyTmpLen);
    SVP_DSP_CHECK_EXPR_RET(u64Body == 0, HI_ERR_SVP_DSP_ILLEGAL_PARAM, HI_DBG_ERR,
        "Error(%#x): The u64Body(0x%llx) can not be 0\n", HI_ERR_SVP_DSP_BADADDR, u64Body);

    /* IDMA body */
    s32Ret = SVP_DSP_CopyData(au8Param, (HI_VOID *)(HI_UL)(u64Body - u64IdmaOffset), u32BodyLen);
    SVP_DSP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, HI_DBG_ERR,
                           "Error(%#x): Idma copy data failed!\n", s32Ret);

    pstSrc = (SVP_IMAGE_S *)au8Param;
    pstDst = (SVP_IMAGE_S *)(au8Param + sizeof(SVP_IMAGE_S));
    /* Fill Src */
    s32Ret = SVP_DSP_GetImgPhyAddr(u64IdmaOffset, pstSrc, au32PhyAddr);
    SVP_DSP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, HI_DBG_ERR,
                           "Error(%#x): Get image phy addr failed!\n", s32Ret);
    SVP_DSP_SETUP_FRAME(pstSrcFrm, au32PhyAddr[0], pstSrc->u32Width,
                        pstSrc->u32Height, 0, 0, pstSrc->au32Stride[0],
                        1, SVP_DSP_PIXEL_PACK_FORMAT_ONE, SVP_DSP_PADDING_TYPE_EDGE);

    s32Ret = SVP_DSP_GetImgPhyAddr(u64IdmaOffset, pstDst, au32PhyAddr);
    SVP_DSP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, HI_DBG_ERR,
                           "Error(%#x): Get image phy addr failed!\n", s32Ret);
    SVP_DSP_SETUP_FRAME(pstDstFrm, au32PhyAddr[0], pstDst->u32Width,
                        pstDst->u32Height, 0, 0, pstDst->au32Stride[0],
                        1, SVP_DSP_PIXEL_PACK_FORMAT_ONE, SVP_DSP_PADDING_TYPE_EDGE);

    return HI_SUCCESS;
}

/*****************************************************************************
*   Prototype    : SVP_DSP_ProcessDilate3x3
*   Description  : Dilate 3x3 process
*   Input        : HI_U64     u64IdmaOffset   IDMA Address offset
*                  HI_U64     u64Body         Body
*                  HI_U32     u32BodyLen      Body Len
*
*   Output       :
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
HI_S32 SVP_DSP_ProcessDilate3x3(HI_U64 u64IdmaOffset, HI_U64 u64Body, HI_U32 u32BodyLen)
{
    HI_S32 s32Ret;
    SVP_DSP_SRC_FRAME_S stSrcFrm = {0};
    SVP_DSP_DST_FRAME_S stDstFrm = {0};

    s32Ret = SvpDspParseAndFillParamForDilate3x3(u64IdmaOffset, u64Body, u32BodyLen, &stSrcFrm, &stDstFrm);
    SVP_DSP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, HI_DBG_ERR,
                           "Error(%#x): DSP parse and fill param for dilate3x3 failed!\n", s32Ret);
    s32Ret = SVP_DSP_Dilate_3x3_U8_U8_Frm(&stSrcFrm, &stDstFrm);
    SVP_DSP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, HI_DBG_ERR,
                           "Error(%#x): DSP dilate_3x3_u8_u8_frm failed!\n", s32Ret);

    return s32Ret;
}

/*****************************************************************************
*   Prototype    : SvpDspParseAndFillParamForErode3x3
*   Description  : Parse and fill kernel parameter for erode 3x3
*   Input        : HI_U64     u64IdmaOffset   IDMA offset
*                  HI_U64     u64Body         Body
*                  HI_U32     u32BodyLen      Body len
*   Output       : SVP_DSP_SRC_FRAME_S *pstSrcFrm : output frame;
*                  SVP_DSP_DST_FRAME_S *pstDstFrm : output frame;
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   Calls        :
*   Called By    :
*   History:
*
*       1.  Date         : 2017-03-24
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
static HI_S32 SvpDspParseAndFillParamForErode3x3(HI_U64 u64IdmaOffset, HI_U64 u64Body, HI_U32 u32BodyLen,
                                                 SVP_DSP_SRC_FRAME_S *pstSrcFrm, SVP_DSP_DST_FRAME_S *pstDstFrm)
{
    HI_S32 s32Ret;
    HI_U8 au8Param[SVP_DSP_1_K];
    HI_U32 u32BodyTmpLen = sizeof(SVP_IMAGE_S) * 2; /* Src+Dst */
    SVP_IMAGE_S *pstSrc = NULL;
    SVP_IMAGE_S *pstDst = NULL;
    HI_U32 au32PhyAddr[3];

    SVP_DSP_CHECK_EXPR_RET(u32BodyLen != u32BodyTmpLen, HI_ERR_SVP_DSP_ILLEGAL_PARAM, HI_DBG_ERR,
        "Error(%#x): The u32BodyLen(%u) must be equal to %u\n", HI_ERR_SVP_DSP_ILLEGAL_PARAM, u32BodyLen,
        u32BodyTmpLen);
    SVP_DSP_CHECK_EXPR_RET(u64Body == 0, HI_ERR_SVP_DSP_ILLEGAL_PARAM, HI_DBG_ERR,
        "Error(%#x): The u64Body(0x%llx) can not be 0\n", HI_ERR_SVP_DSP_BADADDR, u64Body);
    /* IDMA body */
    s32Ret = SVP_DSP_CopyData(au8Param, (HI_VOID *)(HI_UL)(u64Body - u64IdmaOffset), u32BodyLen);
    SVP_DSP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, HI_DBG_ERR,
                           "Error(%#x): Idma copy data failed!\n", s32Ret);

    pstSrc = (SVP_IMAGE_S *)au8Param;
    pstDst = (SVP_IMAGE_S *)(au8Param + sizeof(SVP_IMAGE_S));
    /* Fill Src */
    s32Ret = SVP_DSP_GetImgPhyAddr(u64IdmaOffset, pstSrc, au32PhyAddr);
    SVP_DSP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, HI_DBG_ERR,
                           "Error(%#x): Get image phy addr failed!\n", s32Ret);
    SVP_DSP_SETUP_FRAME(pstSrcFrm, au32PhyAddr[0], pstSrc->u32Width,
                        pstSrc->u32Height, 0, 0, pstSrc->au32Stride[0],
                        1, SVP_DSP_PIXEL_PACK_FORMAT_ONE, SVP_DSP_PADDING_TYPE_EDGE);

    s32Ret = SVP_DSP_GetImgPhyAddr(u64IdmaOffset, pstDst, au32PhyAddr);
    SVP_DSP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, HI_DBG_ERR,
                           "Error(%#x): Get image phy addr failed!\n", s32Ret);
    SVP_DSP_SETUP_FRAME(pstDstFrm, au32PhyAddr[0], pstDst->u32Width,
                        pstDst->u32Height, 0, 0, pstDst->au32Stride[0],
                        1, SVP_DSP_PIXEL_PACK_FORMAT_ONE, SVP_DSP_PADDING_TYPE_EDGE);

    return HI_SUCCESS;
}

/*****************************************************************************
*   Prototype    : SVP_DSP_ProcessErode3x3
*   Description  : Erode 3x3 process
*   Input        : HI_U64     u64IdmaOffset   IDMA Address offset
*                  HI_U64     u64Body         Body
*                  HI_U32     u32BodyLen      Body Len
*
*   Output       :
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
HI_S32 SVP_DSP_ProcessErode3x3(HI_U64 u64IdmaOffset, HI_U64 u64Body, HI_U32 u32BodyLen)
{
    HI_S32 s32Ret;
    SVP_DSP_SRC_FRAME_S stSrcFrm = {0};
    SVP_DSP_DST_FRAME_S stDstFrm = {0};

    s32Ret = SvpDspParseAndFillParamForErode3x3(u64IdmaOffset, u64Body, u32BodyLen, &stSrcFrm, &stDstFrm);
    SVP_DSP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, HI_DBG_ERR,
                           "Error(%#x): DSP parse and fill param for erode3x3 failed!\n", s32Ret);

    s32Ret = SVP_DSP_Erode_3x3_U8_U8_Frm(&stSrcFrm, &stDstFrm);
    SVP_DSP_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, HI_DBG_ERR,
                           "Error(%#x): DSP erode_3x3_u8_u8_frm failed!\n", s32Ret);

    return s32Ret;
}


