/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#ifndef __IMX277_SLAVE_PRIV_H_
#define __IMX277_SLAVE_PRIV_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


// usefull macro
#define HIGH_8BITS(x) ((x & 0xff00) >> 8)
#define LOW_8BITS(x)  (x & 0x00ff)
#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#define IMX277_RES_IS_4K30(w, h)    (w <= 3840 && h <= 2160)

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
#define     FULL_LINES_MAX              (0xFFFF)

// difference registers in various mode
#define     IMX277_VIDEO_MODE_REG_NUM   (0x0004)

typedef struct hiIMX277_SENSOR_REG_S {

    HI_U16 u16Addr;
    HI_U8  u8Data;
} IMX277_SENSOR_REG_S;

typedef enum {
    IMX277_8M120FPS_LINER_MODE = 0,
    IMX277_8M30FPS_LINER_MODE,
    IMX277_8M60FPS_LINER_MODE,
    IMX277_12M30FPS_LINER_MODE,
    IMX277_2M240FPS_LINER_MODE,
    IMX277_8M60FPS_WDR_MODE,
    IMX277_MODE_BUTT

} IMX277_RES_MODE_E;

typedef struct hiIMX277_VIDEO_MODE_TBL_S {
    HI_U32  u32Inck;
    HI_U32  u32InckPerHs;
    HI_U32  u32InckPerVs;
    HI_U32  u32VertiLines;
    HI_U32  u32MaxFps;
    const char *pszModeName;

} IMX277_VIDEO_MODE_TBL_S;


#endif /* __IMX277_SLAVE_PRIV_H_ */
