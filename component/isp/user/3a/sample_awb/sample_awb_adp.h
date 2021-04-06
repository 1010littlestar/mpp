/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#ifndef __SAMPLE_AWB_ADP_H__
#define __SAMPLE_AWB_ADP_H__

#include <string.h>
#include "hi_type.h"
#include "hi_comm_3a.h"
#include "hi_awb_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiSAMPLE_AWB_CTX_S {
    /* usr var */
    HI_U16                  u16DetectTemp;
    HI_U8                   u8WbType;

    /* communicate with isp */
    ISP_AWB_PARAM_S         stAwbParam;
    HI_U32                  u32FrameCnt;
    ISP_AWB_INFO_S          stAwbInfo;
    ISP_AWB_RESULT_S        stAwbResult;
    VI_PIPE                 IspBindDev;

    /* communicate with sensor, defined by user. */
    HI_BOOL                 bSnsRegister;
    ISP_SNS_ATTR_INFO_S     stSnsAttrInfo;
    AWB_SENSOR_DEFAULT_S    stSnsDft;
    AWB_SENSOR_REGISTER_S   stSnsRegister;

    /* global variables of awb algorithm */
} SAMPLE_AWB_CTX_S;

#define MAX_AWB_REGISTER_SNS_NUM 1

extern SAMPLE_AWB_CTX_S g_astAwbCtx[MAX_AWB_LIB_NUM];

/* we assumed that the different lib instance have different id,
 * hisi use the id 0 & 1.
 */

#define AWB_GET_EXTREG_ID(s32Handle)   (((s32Handle) == 0) ? 0x4 : 0x5)

#define AWB_GET_CTX(s32Handle)           (&g_astAwbCtx[s32Handle])

#define AWB_CHECK_HANDLE_ID(s32Handle)\
    do {\
        if (((s32Handle) < 0) || ((s32Handle) >= MAX_AWB_LIB_NUM))\
        {\
            printf("Illegal handle id %d in %s!\n", (s32Handle), __FUNCTION__);\
            return HI_FAILURE;\
        }\
    }while(0)

#define AWB_CHECK_LIB_NAME(acName)\
    do {\
        if (strncmp((acName), HI_AWB_LIB_NAME, ALG_LIB_NAME_SIZE_MAX) != 0)\
        {\
            printf("Illegal lib name %s in %s!\n", (acName), __FUNCTION__);\
            return HI_FAILURE;\
        }\
    }while(0)

#define AWB_CHECK_POINTER(ptr)\
    do {\
        if (ptr == HI_NULL)\
        {\
            printf("Null Pointer in %s!\n", __FUNCTION__);\
            return HI_FAILURE;\
        }\
    }while(0)

#define AWB_CHECK_DEV(dev)\
    do {\
        if (((dev) < 0) || ((dev) >= ISP_MAX_PIPE_NUM))\
        {\
            ISP_ERR_TRACE("Err AWB dev %d in %s!\n", dev, __FUNCTION__);\
            return HI_ERR_ISP_ILLEGAL_PARAM;\
        }\
    }while(0)

#define AWB_EMERG_TRACE(fmt, ...)                                                                            \
    do {                                                                                                    \
        HI_EMERG_TRACE(HI_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define AWB_ALERT_TRACE(fmt, ...)                                                                            \
    do {                                                                                                    \
        HI_ALERT_TRACE(HI_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define AWB_CRIT_TRACE(fmt, ...)                                                                            \
    do {                                                                                                    \
        HI_CRIT_TRACE(HI_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define AWB_ERR_TRACE(fmt, ...)                                                                            \
    do {                                                                                                    \
        HI_ERR_TRACE(HI_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define AWB_WARN_TRACE(fmt, ...)                                                                            \
    do {                                                                                                    \
        HI_WARN_TRACE(HI_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define AWB_NOTICE_TRACE(fmt, ...)                                                                            \
    do {                                                                                                    \
        HI_NOTICE_TRACE(HI_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define AWB_INFO_TRACE(fmt, ...)                                                                            \
    do {                                                                                                    \
        HI_INFO_TRACE(HI_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define AWB_DEBUG_TRACE(fmt, ...)                                                                            \
    do {                                                                                                    \
        HI_DEBUG_TRACE(HI_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
