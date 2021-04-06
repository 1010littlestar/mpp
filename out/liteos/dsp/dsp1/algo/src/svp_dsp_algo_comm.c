/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP algorithm comm implement file
 * Author: svp group
 * Create: 2017-10-17
 */

#include "hi_dsp.h"
#include "svp_dsp_algo_comm.h"

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
HI_S32 SVP_DSP_GetImgPhyAddr(HI_U64 u64IdmaOffset, SVP_IMAGE_S *pstImg, HI_U32 au32PhyAddr[3])
{
    HI_U64 u64Size = 0;
    HI_U64 u64BeginAddr = u64IdmaOffset;
    HI_U64 u64EndAddr = u64IdmaOffset + SVP_DSP_PHYADDR_4GB;

    SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0], u64BeginAddr, u64EndAddr, HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                                  "Error(%#x): pstImg->au64PhyAddr[0](0x%llx) must be in [0x%llx ,0x%llx]!\n",
                                  HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64BeginAddr, u64EndAddr);
    switch (pstImg->enType) {
        case SVP_IMAGE_TYPE_U8C1: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
        }
        break;
        case SVP_IMAGE_TYPE_S8C1: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
        }
        break;
        case SVP_IMAGE_TYPE_YUV420SP: {
            /* Y */
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
            /* VU */
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr, 
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): pstImg->au64PhyAddr[1](0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr);
            u64Size = pstImg->au32Stride[1] * pstImg->u32Height / 2;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[1](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64Size, pstImg->au64PhyAddr[1] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[1] = pstImg->au64PhyAddr[1] - u64IdmaOffset;

        }
        break;
        case SVP_IMAGE_TYPE_YUV422SP: {
            /* Y */
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
            /* VU */
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): pstImg->au64PhyAddr[1](0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr);
            u64Size = pstImg->au32Stride[1] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[1](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64Size, pstImg->au64PhyAddr[1] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[1] = pstImg->au64PhyAddr[1] - u64IdmaOffset;
        }
        break;
        case SVP_IMAGE_TYPE_YUV420P: {
            /* Y */
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
            /* V */
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): pstImg->au64PhyAddr[1](0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr);
            u64Size = pstImg->au32Stride[1] * pstImg->u32Height / 2;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[1](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64Size, pstImg->au64PhyAddr[1] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[1] = pstImg->au64PhyAddr[1] - u64IdmaOffset;
            /* U */
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[2], u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): pstImg->au64PhyAddr[2](0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[2], u64BeginAddr, u64EndAddr);
            u64Size = pstImg->au32Stride[2] * pstImg->u32Height / 2;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[2] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[2](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[2], u64Size, pstImg->au64PhyAddr[2] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[2] = pstImg->au64PhyAddr[2] - u64IdmaOffset;

        }
        break;
        case SVP_IMAGE_TYPE_YUV422P: {
            /* Y */
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
            /* V */
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): pstImg->au64PhyAddr[1](0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr);
            u64Size = pstImg->au32Stride[1] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[1](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n", 
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64Size, pstImg->au64PhyAddr[1] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[1] = pstImg->au64PhyAddr[1] - u64IdmaOffset;
            /* U */
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[2], u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): pstImg->au64PhyAddr[2](0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[2], u64BeginAddr, u64EndAddr);
            u64Size = pstImg->au32Stride[2] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[2] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[2](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[2], u64Size, pstImg->au64PhyAddr[2] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[2] = pstImg->au64PhyAddr[2] - u64IdmaOffset;

        }
        break;
        case SVP_IMAGE_TYPE_S8C2_PACKAGE: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height * 2;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;

        }
        break;
        case SVP_IMAGE_TYPE_S8C2_PLANAR: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): pstImg->au64PhyAddr[1](0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr);
            u64Size = pstImg->au32Stride[1] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[1](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64Size, pstImg->au64PhyAddr[1] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[1] = pstImg->au64PhyAddr[1] - u64IdmaOffset;

        }
        break;
        case SVP_IMAGE_TYPE_S16C1: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(HI_S16);
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
        }
        break;
        case SVP_IMAGE_TYPE_U16C1: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(HI_U16);
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
        }
        break;
        case SVP_IMAGE_TYPE_U8C3_PACKAGE: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height * 3;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
        }
        break;
        case SVP_IMAGE_TYPE_U8C3_PLANAR: {
            /* ch1 */
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
            /* ch2 */
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): pstImg->au64PhyAddr[1](0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64BeginAddr, u64EndAddr);
            u64Size = pstImg->au32Stride[1] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[1] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[1](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[1], u64Size, pstImg->au64PhyAddr[1] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[1] = pstImg->au64PhyAddr[1] - u64IdmaOffset;
            /* ch3 */
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[2], u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): pstImg->au64PhyAddr[2](0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[2], u64BeginAddr, u64EndAddr);
            u64Size = pstImg->au32Stride[2] * pstImg->u32Height;
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[2] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[2](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[2], u64Size, pstImg->au64PhyAddr[2] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[2] = pstImg->au64PhyAddr[2] - u64IdmaOffset;

        }
        break;
        case SVP_IMAGE_TYPE_S32C1: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(HI_S32);
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
        }
        break;
        case SVP_IMAGE_TYPE_U32C1: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(HI_U32);
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
        }
        break;
        case SVP_IMAGE_TYPE_S64C1: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(HI_S64);
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;

        }
        break;
        case SVP_IMAGE_TYPE_U64C1: {
            u64Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(HI_U64);
            SVP_DSP_CHECK_NIN_CLOSE_RANGE(pstImg->au64PhyAddr[0] + u64Size, u64BeginAddr, u64EndAddr,
                HI_ERR_SVP_DSP_ILLEGAL_PARAM,
                "Error(%#x): (pstImg->au64PhyAddr[0](0x%llx) + Size(0x%llx))(0x%llx) must be in [0x%llx ,0x%llx]!\n",
                HI_ERR_SVP_DSP_ILLEGAL_PARAM, pstImg->au64PhyAddr[0], u64Size, pstImg->au64PhyAddr[0] + u64Size,
                u64BeginAddr, u64EndAddr);
            au32PhyAddr[0] = pstImg->au64PhyAddr[0] - u64IdmaOffset;
        }
        break;
        default:
        {
            HI_TRACE_SVP_DSP(HI_DBG_ERR, "Error,unknow image type(%d)", pstImg->enType);
            return HI_ERR_SVP_DSP_ILLEGAL_PARAM;
        }
        break;
    }

    return HI_SUCCESS;
}

