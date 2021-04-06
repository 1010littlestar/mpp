/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#ifndef __SHARP8K_SLAVE_PRIV_H_
#define __SHARP8K_SLAVE_PRIV_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


// usefull macro
#define HIGH_8BITS(x) ((x & 0xff00) >> 8)
#define LOW_8BITS(x)  (x & 0x00ff)

#define X_8BITS(x)   (x & 0x000000ff)
#define X_16BITS(x) ((x & 0x0000ff00) >> 8)
#define X_24BITS(x) ((x & 0x00ff0000) >> 16)
#define X_32BITS(x) ((x & 0xff000000) >> 24)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#define SHARP8K_RES_IS_4K30(w, h)    (w <= 3840 && h <= 2160)

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
#define     SHARP8K_VIDEO_MODE_REG_NUM   (0x0004)

typedef struct hiSHARP8K_SENSOR_REG_S {

    HI_U16 u16Addr;
    HI_U8  u8Data;
} SHARP8K_SENSOR_REG_S;

typedef enum {
    SHARP8K_8K30FPS_LINER_MODE = 0,
    SHARP8K_MODE_BUTT
} SHARP8K_RES_MODE_E;

typedef struct hiSHARP8K_VIDEO_MODE_TBL_S {
    HI_U32  u32Inck;
    HI_U32  u32InckPerHs;
    HI_U32  u32InckPerVs;
    HI_U32  u32VertiLines;
    HI_U32  u32MaxFps;
    const char *pszModeName;

} SHARP8K_VIDEO_MODE_TBL_S;


#endif /* __SHARP8K_SLAVE_PRIV_H_ */
