/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#ifndef __IMX334_SLAVE_PRIV_H_
#define __IMX334_SLAVE_PRIV_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#define LENS_TYPE_FOUR_STITCH // Donot remove this define

#define CHECK_RET(express)\
    do{\
        HI_S32 s32Ret;\
        s32Ret = express;\
        if (s32Ret != HI_SUCCESS)\
        {\
            printf("Failed at %s: LINE: %d with %#x!\n", __FUNCTION__, __LINE__, s32Ret);\
        }\
    }while(0)

/*
--------------------------------------------------------------------------------
- Structure For Slave Mode Sensor Using
--------------------------------------------------------------------------------
*/

typedef enum {
    IMX334_8M30FPS_LINER_MODE = 0,
    IMX334_8M30FPS_WDR_MODE = 1,
    IMX334_MODE_BUTT
} IMX334_RES_MODE_E;


typedef struct hiIMX334_VIDEO_MODE_TBL_S {
    HI_U32  u32Inck;
    HI_U32  u32InckPerHs;
    HI_U32  u32InckPerVs;
    HI_U32  u32VertiLines;
    HI_FLOAT  f32MaxFps;
    const char *pszModeName;

} IMX334_VIDEO_MODE_TBL_S;

#endif /* __IMX334_SLAVE_PRIV_H_ */
