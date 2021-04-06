/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP algorithm implement file
 * Author: svp group
 * Create: 2017-10-17
 */
#include "hi_dsp.h"
#include "svp_dsp_frm.h"
#include "svp_dsp_algo.h"
#include "svp_dsp_proc_algo.h"

/*****************************************************************************
*   Prototype    : SVP_DSP_ALGO_Init
*   Description:   Algorithm initialization
*   Input        : HI_VOID [IN] No input
*
*   Output       :
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
HI_S32 SVP_DSP_ALGO_Init(HI_VOID)
{
    return SVP_DSP_FRM_Init();
}

/*****************************************************************************
*   Prototype    : SVP_DSP_ALGO_DeInit
*   Description:   Algorithm deinitialization
*   Input        : HI_VOID [IN] No input
*
*   Output       :
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
HI_S32 SVP_DSP_ALGO_DeInit(HI_VOID)
{
    return SVP_DSP_FRM_Exit();
}

/*****************************************************************************
*   Prototype    : SVP_DSP_ALGO_Process
*   Description  : Process kernel
*   Input        : HI_U64     u64IdmaOffset   IDMA Address offset
*                  HI_U32     u32CMD          CMD
*                  HI_U32     u32MsgId        Message Id
*                  HI_U64     u64Body         Body
*                  HI_U32     u32BodyLen      Body Len
*   Output       :
*   Return Value : HI_SUCCESS: Success;Error codes: Failure.
*   Spec         :
*   Calls        :
*   Called By    :
*   History:
*
*       1.  Date         : 2017-11-1
*           Author       :
*           Modification : Created function
*
*****************************************************************************/
HI_S32 SVP_DSP_ALGO_Process(HI_U64 u64IdmaOffset, HI_U32 u32CMD, HI_U32 u32MsgId, HI_U64 u64Body,
                            HI_U32 u32BodyLen)
{
    HI_S32 s32Ret = HI_SUCCESS;

    /* Add customize cmd in here */
    switch (u32CMD) {
        case SVP_DSP_CMD_ERODE_3X3: {
            s32Ret = SVP_DSP_ProcessErode3x3(u64IdmaOffset, u64Body, u32BodyLen);
        }
        break;
        case SVP_DSP_CMD_DILATE_3X3: {
            s32Ret = SVP_DSP_ProcessDilate3x3(u64IdmaOffset, u64Body, u32BodyLen);
        }
        break;
#ifdef CONFIG_HI_PHOTO_SUPPORT
#if (0 == DSP_ID)
        case SVP_DSP_CMD_PHOTO_PROC: {
            extern HI_S32 SVP_DSP_PhotoProcess(HI_U64 u64IdmaOffset, HI_U64 u64Body, HI_U32 u32BodyLen);
            s32Ret = SVP_DSP_PhotoProcess(u64IdmaOffset, u64Body, u32BodyLen);
        }
        break;
#endif
#endif

#ifdef CONFIG_HI_MONO_COLOR_FUSION_SUPPORT
        case SVP_DSP_CMD_MCF_PROC: {
            extern HI_S32 SVP_DSP_MCFProcess(HI_U64 u64IdmaOffset, HI_U64 u64Body, HI_U32 u32BodyLen);
            s32Ret = SVP_DSP_MCFProcess(u64IdmaOffset, u64Body, u32BodyLen);
        }
        break;
#endif

        default:
        {
            s32Ret = HI_ERR_SVP_DSP_ILLEGAL_PARAM;
        }
        break;
    }

    return s32Ret;
}

