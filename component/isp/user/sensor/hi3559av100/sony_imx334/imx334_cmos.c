/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#if !defined(__IMX334_CMOS_H_)
#define __IMX334_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hi_comm_sns.h"
#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#include "imx334_cmos_ex.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#define IMX334_ID                           334

#define HIGHER_4BITS(x) (((x) & 0xf0000) >> 16)
#define HIGH_8BITS(x) (((x) & 0xff00) >> 8)
#define LOW_8BITS(x)  ((x) & 0x00ff)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastImx334[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define IMX334_SENSOR_GET_CTX(dev, pstCtx)   (pstCtx = g_pastImx334[dev])
#define IMX334_SENSOR_SET_CTX(dev, pstCtx)   (g_pastImx334[dev] = pstCtx)
#define IMX334_SENSOR_RESET_CTX(dev)         (g_pastImx334[dev] = HI_NULL)

static HI_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM]   = {0};
static HI_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};

ISP_SNS_COMMBUS_U g_aunImx334BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static HI_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};

typedef struct hiIMX334_STATE_S {
    HI_U32      u32BRL;
    HI_U32      u32RHS1_MAX;
    HI_U32      u32deltaRHS1;
} IMX334_STATE_S;

IMX334_STATE_S g_astimx334State[ISP_MAX_PIPE_NUM] = {{0}};


/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern unsigned char imx334_i2c_addr;
extern unsigned int  imx334_addr_byte;
extern unsigned int  imx334_data_byte;

extern void imx334_init(VI_PIPE ViPipe);
extern void imx334_exit(VI_PIPE ViPipe);
extern void imx334_standby(VI_PIPE ViPipe);
extern void imx334_restart(VI_PIPE ViPipe);
extern int  imx334_write_register(VI_PIPE ViPipe, int addr, int data);
extern int  imx334_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define IMX334_FULL_LINES_MAX               (0xFFFFF)  /* VMAX is 20bit */
#define IMX334_FULL_LINES_MAX_2TO1_WDR      (0x1194)  // considering the YOUT_SIZE and bad frame


#define     IMX334_SHR0_L               (0x3058)  /*  SHR0[7:0]  */
#define     IMX334_SHR0_M               (0x3059)  /*  SHR0[15:8] */
#define     IMX334_SHR0_H               (0x305a)  /*  SHR0[19:16] */

#define     IMX334_SHR1_L               (0x305c)  /*  SHR1[7:0]  */
#define     IMX334_SHR1_M               (0x305d)  /*  SHR1[15:8] */
#define     IMX334_SHR1_H               (0x305e)  /*  SHR1[19:16] */

#define     IMX334_RHS1_L               (0x3068)  /*  RHS1[7:0]  */
#define     IMX334_RHS1_M               (0x3069)  /*  RHS1[15:8] */
#define     IMX334_RHS1_H               (0x306a)  /*  RHS1[19:16] */

#define     IMX334_PGC_L                (0x30e8)  /* GAIN[7:0] */
#define     IMX334_PGC_H                (0x30e9)  /* GAIN[10:8] */

#define     IMX334_VMAX_L               (0x3030)  /* VMAX[7:0] */
#define     IMX334_VMAX_M               (0x3031)  /* VMAX[15:8] */
#define     IMX334_VMAX_H               (0x3032)  /* VMAX[19:16] */

#define     IMX334_Y_OUTSIZE_L          (0x3308)  /* Y_OUT_SIZE [7:0] */
#define     IMX334_Y_OUTSIZE_H          (0x3309)  /* Y_OUT_SIZE [12:8] */


#define IMX334_INCREASE_LINES               (0) /* make real fps less than stand fps because NVR require */

#define IMX334_VMAX_8M_30FPS_12BIT_LINEAR   (0x8CA+IMX334_INCREASE_LINES)
#define IMX334_VMAX_8M_30FPS_12BIT_2TO1_WDR (0x8CA+IMX334_INCREASE_LINES)

// sensor fps mode
#define IMX334_8M_30FPS_12BIT_LINEAR_MODE   (1) /* 4K@30fps Linear 12bit */
#define IMX334_8M_30FPS_12BIT_2t1_DOL_MODE  (2)

#define IMX334_RES_IS_4K30_12BIT(w, h)    (((w) == 3840) && ((h) == 2160))

// sensor gain
#define IMX334_AGAIN_MAX                    (31356)     // the max again is 31356
#define IMX334_DGAIN_MAX                    (124833)    // the max dgain is 124833

static HI_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->u32FullLinesMax = IMX334_FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 4 << pstAeSnsDft->u32ISPDgainShift;

    memcpy(&pstAeSnsDft->stPirisAttr, &gstPirisAttr, sizeof(ISP_PIRIS_ATTR_S));
    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    if (g_au32InitExposure[ViPipe] == 0) {
        pstAeSnsDft->u32InitExposure = 1000000;
    }

    if (g_au32LinesPer500ms[ViPipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2;
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
    }

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u8AeCompensation = 0x38;
            pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 5;
            pstAeSnsDft->u32MinIntTime = 2;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.156;

            pstAeSnsDft->u32MaxAgain = 31356;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 128913;  /* if Dgain enable,please set ispdgain bigger than 1 */
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
            pstAeSnsDft->u32HmaxTimes = (1000000 * 1000) / (IMX334_VMAX_8M_30FPS_12BIT_LINEAR * 30);
            break;
        case WDR_MODE_2To1_LINE:
            pstAeSnsDft->au8HistThresh[0] = 0xC;
            pstAeSnsDft->au8HistThresh[1] = 0x18;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;


            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2;

            pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
            pstAeSnsDft->stIntTimeAccu.f32Accuracy = 2;
            pstAeSnsDft->u32MinIntTime = 2;
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.311;

            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;

            pstAeSnsDft->u32MaxAgain = 31356;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 128913;
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe] ? g_au32InitExposure[ViPipe] : 16462;
            pstAeSnsDft->u32HmaxTimes = (1000000 * 1000) / (IMX334_VMAX_8M_30FPS_12BIT_2TO1_WDR * 30);
            if (genFSWDRMode[ViPipe] == ISP_FSWDR_LONG_FRAME_MODE) {
                pstAeSnsDft->u8AeCompensation = 56;
                pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
            } else {
                pstAeSnsDft->u32MaxDgainTarget = 1024;
                pstAeSnsDft->u32MaxISPDgainTarget = 4096;
                pstAeSnsDft->u8AeCompensation = 24;
                pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
                pstAeSnsDft->u16ManRatioEnable = HI_FALSE;
                pstAeSnsDft->au32Ratio[0] = 0x400;
                pstAeSnsDft->au32Ratio[1] = 0x40;
                pstAeSnsDft->au32Ratio[2] = 0x40;
            }
            break;
    }

    return HI_SUCCESS;
}


/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE ViPipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32MaxFps;
    HI_U32 u32Lines;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case IMX334_8M_30FPS_12BIT_LINEAR_MODE:
            u32MaxFps = 30;
            if ((f32Fps <= 30) && (f32Fps >= 0.07)) {
                u32Lines = IMX334_VMAX_8M_30FPS_12BIT_LINEAR * u32MaxFps / DIV_0_TO_1_FLOAT(f32Fps);
                pstAeSnsDft->u32LinesPer500ms = IMX334_VMAX_8M_30FPS_12BIT_LINEAR*15;
            } else {
                ISP_ERR_TRACE("Not support Fps: %f\n", f32Fps);
                return;
            }
            u32Lines = (u32Lines > IMX334_FULL_LINES_MAX) ? IMX334_FULL_LINES_MAX : u32Lines;
            break;

        case IMX334_8M_30FPS_12BIT_2t1_DOL_MODE:
            u32MaxFps = 30;
            if ((f32Fps <= 30) && (f32Fps >= 15)) {
                u32Lines = IMX334_VMAX_8M_30FPS_12BIT_2TO1_WDR * u32MaxFps / DIV_0_TO_1_FLOAT(f32Fps);
                pstAeSnsDft->u32LinesPer500ms = IMX334_VMAX_8M_30FPS_12BIT_2TO1_WDR*30;
            } else {
                ISP_ERR_TRACE("Not support Fps: %f\n", f32Fps);
                return;
            }
            u32Lines = u32Lines + (u32Lines % 2);    // VMAX is 2n(n-0,1,2.....)
            u32Lines = (u32Lines > IMX334_FULL_LINES_MAX_2TO1_WDR) ? IMX334_FULL_LINES_MAX_2TO1_WDR : u32Lines;
            break;
        default:
            ISP_ERR_TRACE("Not support ImgMode: %d\n", pstSnsState->u8ImgMode);
            return;
    }

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = (u32Lines & 0xFF);
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = ((u32Lines & 0xFF00) >> 8);
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((u32Lines & 0xF0000) >> 16);
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = (u32Lines & 0xFF);
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = ((u32Lines & 0xFF00) >> 8);
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = ((u32Lines & 0xF0000) >> 16);
    }

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->u32FLStd = u32Lines * 2;

        /*   RHS1 limitation:    4n + 1 and RHS1 <= 2 * BRL
             RHS1(N+1)>= RHS1(N)+ 2*BRL - 2*VMAX + 2 ===> RHS1(N)-RHS1(N+1)<= 2*VMAX - 2*BRL -2 */
        g_astimx334State[ViPipe].u32RHS1_MAX = g_astimx334State[ViPipe].u32BRL * 2;
        g_astimx334State[ViPipe].u32deltaRHS1 = u32Lines * 2 - g_astimx334State[ViPipe].u32BRL * 2 - 2;
    } else {
        pstSnsState->u32FLStd = u32Lines;
    }

    pstAeSnsDft->f32Fps = f32Fps;

    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 5;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    return;
}

static HI_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, HI_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        u32FullLines = (u32FullLines > 2 * IMX334_FULL_LINES_MAX_2TO1_WDR) ? 2 * IMX334_FULL_LINES_MAX_2TO1_WDR : u32FullLines;
        pstSnsState->au32FL[0] = (u32FullLines >> 1) << 1;
        g_astimx334State[ViPipe].u32RHS1_MAX = 2 * 2200;
        g_astimx334State[ViPipe].u32deltaRHS1 = u32FullLines - g_astimx334State[ViPipe].u32BRL * 2 - 2;
    } else {
        u32FullLines = (u32FullLines > IMX334_FULL_LINES_MAX) ? IMX334_FULL_LINES_MAX : u32FullLines;
        pstSnsState->au32FL[0] = u32FullLines;
    }

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = (pstSnsState->au32FL[0] & 0xFF);
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = ((pstSnsState->au32FL[0] & 0xFF00) >> 8);
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((pstSnsState->au32FL[0] & 0xF0000) >> 16);
    } else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = ((pstSnsState->au32FL[0] >> 1) & 0xFF);
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = (((pstSnsState->au32FL[0] >> 1) & 0xFF00) >> 8);
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = (((pstSnsState->au32FL[0] >> 1) & 0xF0000) >> 16);
    } else {
    }

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 5;

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE ViPipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    static HI_BOOL bFirst[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 1};
    HI_U32 u32Value = 0;

    static HI_U32 u32ShortIntTime[ISP_MAX_PIPE_NUM] = {0};
    static HI_U32 u32LongIntTime[ISP_MAX_PIPE_NUM] = {0};

    static HI_U32 u32RHS1[ISP_MAX_PIPE_NUM]  = {0};
    static HI_U32 u32LastRHS1[ISP_MAX_PIPE_NUM]  = {0};
    static HI_U32 u32deltaRHS1[ISP_MAX_PIPE_NUM]  = {0};
    static HI_U32 u32SHR0[ISP_MAX_PIPE_NUM]  = {0};
    static HI_U32 u32SHR1[ISP_MAX_PIPE_NUM]  = {0};
    static HI_U32 u32YOUTSIZE[ISP_MAX_PIPE_NUM] = {0};

    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        /* short exposure */
        if (bFirst[ViPipe]) {
            pstSnsState->au32WDRIntTime[0] = u32IntTime;
            u32ShortIntTime[ViPipe] = u32IntTime;
            bFirst[ViPipe] = HI_FALSE;
        } else {
        /* long exposure */
            pstSnsState->au32WDRIntTime[1] = u32IntTime;
            u32LongIntTime[ViPipe] = u32IntTime;

            u32SHR0[ViPipe] = pstSnsState->au32FL[1] - u32LongIntTime[ViPipe];
            u32SHR1[ViPipe] = 9;

            if (u32ShortIntTime[ViPipe] % 4 == 0) {
                u32SHR1[ViPipe] = 9;
                u32RHS1[ViPipe] = u32ShortIntTime[ViPipe] + u32SHR1[ViPipe];
            } else if (u32ShortIntTime[ViPipe] % 4 == 2) {
                u32SHR1[ViPipe] = 11;
                u32RHS1[ViPipe] = u32ShortIntTime[ViPipe] + u32SHR1[ViPipe];
            }

            if (u32RHS1[ViPipe] >= u32LastRHS1[ViPipe]) {
            } else if (u32RHS1[ViPipe] < u32LastRHS1[ViPipe]) {
                u32deltaRHS1[ViPipe] = u32LastRHS1[ViPipe] - u32RHS1[ViPipe];
                if (u32deltaRHS1[ViPipe] <= g_astimx334State[ViPipe].u32deltaRHS1) {
                } else {
                    if (u32ShortIntTime[ViPipe] % 4 == 0) {
                        u32SHR1[ViPipe] = 9 + (u32deltaRHS1[ViPipe] - g_astimx334State[ViPipe].u32deltaRHS1 + 2);
                        u32RHS1[ViPipe] = u32LastRHS1[ViPipe] - (g_astimx334State[ViPipe].u32deltaRHS1 - 2);
                    } else if (u32ShortIntTime[ViPipe] % 4 == 2) {
                        u32SHR1[ViPipe] = 11 + (u32deltaRHS1[ViPipe] - g_astimx334State[ViPipe].u32deltaRHS1 + 2);
                        u32RHS1[ViPipe] = u32LastRHS1[ViPipe] - (g_astimx334State[ViPipe].u32deltaRHS1 - 2);
                    }
                }

                if (u32SHR0[ViPipe] < u32LastRHS1[ViPipe] + 9) {
                    u32SHR0[ViPipe] = u32LastRHS1[ViPipe] + 9;
                } else {
                }
            } else {
            }

            // Virtural Channel Mode
            u32YOUTSIZE[ViPipe] = 2180 + (u32RHS1[ViPipe] - 3) / 2;
            u32YOUTSIZE[ViPipe] = (u32YOUTSIZE[ViPipe] >= 0x1FFF) ? 0x1FFF : u32YOUTSIZE[ViPipe];

            pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(u32SHR0[ViPipe]);
            pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(u32SHR0[ViPipe]);
            pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = HIGHER_4BITS(u32SHR0[ViPipe]);

            pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = LOW_8BITS(u32SHR1[ViPipe]);
            pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = HIGH_8BITS(u32SHR1[ViPipe]);
            pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = HIGHER_4BITS(u32SHR1[ViPipe]);

            pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = LOW_8BITS(u32RHS1[ViPipe]);
            pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = HIGH_8BITS(u32RHS1[ViPipe]);
            pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = HIGHER_4BITS(u32RHS1[ViPipe]);

            pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = (u32YOUTSIZE[ViPipe] & 0xFF);
            pstSnsState->astRegsInfo[0].astI2cData[15].u32Data = ((u32YOUTSIZE[ViPipe] & 0x1F00) >> 8);

            bFirst[ViPipe] = HI_TRUE;
            u32LastRHS1[ViPipe] = u32RHS1[ViPipe];
        }

    } else {

        u32Value = pstSnsState->au32FL[0] - u32IntTime;
        u32Value = MIN(u32Value, 0xfffff);
        u32Value = MIN(MAX(u32Value, 5), pstSnsState->au32FL[0] - 2);

        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(u32Value);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(u32Value);
        pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = HIGHER_4BITS(u32Value);

        bFirst[ViPipe] = HI_TRUE;
    }

    return;

}

static HI_U32 gain_table[262] = {
    1024,  1059,  1097,  1135,  1175,  1217,  1259,  1304,  1349,  1397,  1446,  1497,  1549,  1604,  1660,  1719,  1779,  1842,  1906,
    1973,  2043,  2048,  2119,  2194,  2271,  2351,  2434,  2519,  2608,  2699,  2794,  2892,  2994,  3099,  3208,  3321,  3438,  3559,
    3684,  3813,  3947,  4086,  4229,  4378,  4532,  4691,  4856,  5027,  5203,  5386,  5576,  5772,  5974,  6184,  6402,  6627,  6860,
    7101,  7350,  7609,  7876,  8153,  8439,  8736,  9043,  9361,  9690, 10030, 10383, 10748, 11125, 11516, 11921, 12340, 12774, 13222,
    13687, 14168, 14666, 15182, 15715, 16267, 16839, 17431, 18043, 18677, 19334, 20013, 20717, 21445, 22198, 22978, 23786, 24622, 25487,
    26383, 27310, 28270, 29263, 30292, 31356, 32458, 33599, 34780, 36002, 37267, 38577, 39932, 41336, 42788, 44292, 45849, 47460, 49128,
    50854, 52641, 54491, 56406, 58388, 60440, 62564, 64763, 67039, 69395, 71833, 74358, 76971, 79676, 82476, 85374, 88375, 91480, 94695,
    98023, 101468, 105034, 108725, 112545, 116501, 120595, 124833, 129220, 133761, 138461, 143327, 148364, 153578, 158975, 164562, 170345, 176331, 182528,
    188942, 195582, 202455, 209570, 216935, 224558, 232450, 240619, 249074, 257827, 266888, 276267, 285976, 296026, 306429, 317197, 328344, 339883, 351827,
    364191, 376990, 390238, 403952, 418147, 432842, 448053, 463799, 480098, 496969, 514434, 532512, 551226, 570597, 590649, 611406, 632892, 655133, 678156,
    701988, 726657, 752194, 778627, 805990, 834314, 863634, 893984, 925400, 957921, 991585, 1026431, 1062502, 1099841, 1138491, 1178500, 1219916, 1262786,
    1307163, 1353100, 1400651, 1449872, 1500824, 1553566, 1608162, 1664676, 1723177, 1783733, 1846417, 1911304, 1978472, 2048000, 2119971, 2194471, 2271590,
    2351418, 2434052, 2519590, 2608134, 2699789, 2794666, 2892876, 2994538, 3099773, 3208706, 3321467, 3438190, 3559016, 3684087, 3813554, 3947571, 4086297,
    4229898, 4378546, 4532417, 4691696, 4856573, 5027243, 5203912, 5386788, 5576092, 5772048, 5974890, 6184861, 6402210, 6627198, 6860092, 7101170, 7350721,
    7609041, 7876439, 8153234
};

static HI_VOID cmos_again_calc_table(VI_PIPE ViPipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (*pu32AgainLin >= gain_table[100]) {
        *pu32AgainLin = gain_table[100];
        *pu32AgainDb = 100;
        return;
    }

    for (i = 1; i < 101; i++) {
        if (*pu32AgainLin < gain_table[i]) {
            *pu32AgainLin = gain_table[i - 1];
            *pu32AgainDb = i - 1;
            break;
        }
    }
    return;
}

static HI_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, HI_U32 *pu32DgainLin, HI_U32 *pu32DgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32DgainLin);
    CMOS_CHECK_POINTER_VOID(pu32DgainDb);

    if (*pu32DgainLin >= gain_table[140]) {
        *pu32DgainLin = gain_table[140];
        *pu32DgainDb = 140;
        return;
    }

    for (i = 1; i < 141; i++) {
        if (*pu32DgainLin < gain_table[i]) {
            *pu32DgainLin = gain_table[i - 1];
            *pu32DgainDb = i - 1;
            break;
        }
    }

    return;
}


static HI_VOID cmos_gains_update(VI_PIPE ViPipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U32 u32Tmp;

    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32Tmp = u32Again + u32Dgain;

    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = LOW_8BITS(u32Tmp);
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = HIGH_8BITS(u32Tmp);
    return;
}

/*
           Linear:     SHR0 [5, VMAX-1];    Initime = VMAX - SHR0  Initime [1,VMAX -5]
           WDR 2t1 :   SHS1 2n+1 (n=0,1,2....)  and 9 <= SHS1 <= RHS1-2
                       RHS1 4n+1 (n=0,1,2....)  and (SHR1+2)<= RHS1<=(SHR0-9) and RHS1<(BRL*2)
                       SHR0 2n(n=0,1,2.....) and (RHS1+9) <= SHR0<=(2*VMAX-2)  VMAX :2n (n=0,1,2.....)
*/
static HI_VOID cmos_get_inttime_max(VI_PIPE ViPipe, HI_U16 u16ManRatioEnable, HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32IntTimeMaxTmp0 = 0;
    HI_U32 u32IntTimeMaxTmp  = 0;
    HI_U32 u32RatioTmp = 0x40;
    HI_U32 u32ShortTimeMinLimit = 0;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(au32Ratio);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
    CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortTimeMinLimit = 2;

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        /*  limitation for line base WDR

             SHR1 limitation:
             9 or more
             RHS1 - 2 or less  and 2n+1(n=0,1,2....)

             SHR0 limitation:
             2n(n=0,1,2....)
             RHS1 + 9 or more
             FSC - 2 or less

             RHS1 Limitation
             4n+1 (n=0,1,2....)
             (SHR1+2)<=RHS1 <= (SHR0-9)

             RHS1 = st + 9 - (st % 4) +4
             SHR0 = FSC - st * ratio + (lt %2) - 2

             RHS1<= SHR0 -9=FSC -st*ratio +(lt%2) - 11
             st *(1+ratio) <= FSC -24 + (lt %2) +(st %4)
             st *(1+ratio) < = FSC -24

             short exposure time = RHS1 - SHR1 <= RHS1 - 9
             long exposure time = FSC -  SHR0  <= FSC - (RHS1 + 9)
             ExposureShort + ExposureLong <= FSC - 18
             short exposure time <= (FSC - 18) / (ratio + 1)
           */
        if (genFSWDRMode[ViPipe] == ISP_FSWDR_LONG_FRAME_MODE) {
            u32IntTimeMaxTmp0 = pstSnsState->au32FL[1] - 280 - pstSnsState->au32WDRIntTime[0];
            u32IntTimeMaxTmp = pstSnsState->au32FL[0] - 280;
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ? u32IntTimeMaxTmp0 : u32IntTimeMaxTmp;
            au32IntTimeMax[0] = u32IntTimeMaxTmp;
            au32IntTimeMin[0] = u32ShortTimeMinLimit;
            return;
        } else {
            u32IntTimeMaxTmp0 = ((pstSnsState->au32FL[1] - 280 - pstSnsState->au32WDRIntTime[0]) * 0x40)  / DIV_0_TO_1(au32Ratio[0]);
            u32IntTimeMaxTmp = ((pstSnsState->au32FL[0] - 280) * 0x40)  / DIV_0_TO_1(au32Ratio[0] + 0x40);
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ? u32IntTimeMaxTmp0 : u32IntTimeMaxTmp;
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp > (g_astimx334State[ViPipe].u32RHS1_MAX - 2)) ? (g_astimx334State[ViPipe].u32RHS1_MAX - 2) : u32IntTimeMaxTmp;
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp == 0) ? 1 : u32IntTimeMaxTmp;
        }

    } else {

    }

    if (u32IntTimeMaxTmp >= u32ShortTimeMinLimit) {
        if (IS_LINE_WDR_MODE(pstSnsState->enWDRMode)) {
            au32IntTimeMax[0] = u32IntTimeMaxTmp;
            au32IntTimeMax[1] = au32IntTimeMax[0] * au32Ratio[0] >> 6;
            au32IntTimeMax[2] = au32IntTimeMax[1] * au32Ratio[1] >> 6;
            au32IntTimeMax[3] = au32IntTimeMax[2] * au32Ratio[2] >> 6;
            au32IntTimeMin[0] = u32ShortTimeMinLimit;
            au32IntTimeMin[1] = au32IntTimeMin[0] * au32Ratio[0] >> 6;
            au32IntTimeMin[2] = au32IntTimeMin[1] * au32Ratio[1] >> 6;
            au32IntTimeMin[3] = au32IntTimeMin[2] * au32Ratio[2] >> 6;
        } else {
        }
    } else {
        if (u16ManRatioEnable == 1) {
            ISP_ERR_TRACE("Manaul ExpRatio is too large!\n");
            return;
        } else {
            u32IntTimeMaxTmp = u32ShortTimeMinLimit;

            if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
                u32RatioTmp = 0xFFF;
                au32IntTimeMax[0] = u32IntTimeMaxTmp;
                au32IntTimeMax[1] = au32IntTimeMax[0] * u32RatioTmp >> 6;
            } else {
            }
            au32IntTimeMin[0] = au32IntTimeMax[0];
            au32IntTimeMin[1] = au32IntTimeMax[1];
            au32IntTimeMin[2] = au32IntTimeMax[2];
            au32IntTimeMin[3] = au32IntTimeMax[3];
        }
    }

    return;

}

/* Only used in LINE_WDR mode */
static HI_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

    genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
    gu32MaxTimeGetCnt[ViPipe] = 0;
}


static HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set           = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table  = cmos_dgain_calc_table;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return HI_SUCCESS;
}

// awb static param for Fuji Lens New IR_Cut
#define CALIBRATE_STATIC_WB_R_GAIN          494
#define CALIBRATE_STATIC_WB_GR_GAIN         256
#define CALIBRATE_STATIC_WB_GB_GAIN         256
#define CALIBRATE_STATIC_WB_B_GAIN          464

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                    -41
#define CALIBRATE_AWB_P2                    297
#define CALIBRATE_AWB_Q1                    0
#define CALIBRATE_AWB_A1                    155760
#define CALIBRATE_AWB_B1                    128
#define CALIBRATE_AWB_C1                    -105396

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                        0
#define GOLDEN_BGAIN                        0
static HI_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = 4900;

    pstAwbSnsDft->au16GainOffset[0] = CALIBRATE_STATIC_WB_R_GAIN;
    pstAwbSnsDft->au16GainOffset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
    pstAwbSnsDft->au16GainOffset[2] = CALIBRATE_STATIC_WB_GB_GAIN;
    pstAwbSnsDft->au16GainOffset[3] = CALIBRATE_STATIC_WB_B_GAIN;

    pstAwbSnsDft->as32WbPara[0] = CALIBRATE_AWB_P1;
    pstAwbSnsDft->as32WbPara[1] = CALIBRATE_AWB_P2;
    pstAwbSnsDft->as32WbPara[2] = CALIBRATE_AWB_Q1;
    pstAwbSnsDft->as32WbPara[3] = CALIBRATE_AWB_A1;
    pstAwbSnsDft->as32WbPara[4] = CALIBRATE_AWB_B1;
    pstAwbSnsDft->as32WbPara[5] = CALIBRATE_AWB_C1;

    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;
    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;
        case WDR_MODE_2To1_LINE:
            memcpy(&pstAwbSnsDft->stCcm,    &g_stAwbCcmFsWdr, sizeof(AWB_CCM_S));
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));

            break;
    }
    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];

    return HI_SUCCESS;
}

static HI_S32 cmos_get_awb_spec_default(VI_PIPE ViPipe, AWB_SPEC_SENSOR_DEFAULT_S *pstAwbSpecSnsDft)
{
    CMOS_CHECK_POINTER(pstAwbSpecSnsDft);

    memset(pstAwbSpecSnsDft, 0, sizeof(AWB_SPEC_SENSOR_DEFAULT_S));

    memcpy(&pstAwbSpecSnsDft->stSpecAwbAttrs, &g_SpecAWBFactTbl, sizeof(ISP_SPECAWB_ATTR_S));
    memcpy(&pstAwbSpecSnsDft->stCaaControl, &g_SpecKCAWBCaaTblControl, sizeof(ISP_SPECAWB_CAA_CONTROl_S));

    return HI_SUCCESS;
}

static HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;
    pstExpFuncs->pfn_cmos_get_awb_spec_default = cmos_get_awb_spec_default;

    return HI_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {
    {378, 256, 430},
    {439, 256, 439}
};
static HI_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));
    pstDef->unKey.bit1RLsc     = 0;
    pstDef->pstRLsc            = &g_stCmosRLsc;
    pstDef->unKey.bit1Clut     = 1;
    pstDef->pstClut            = &g_stIspCLUT;
    pstDef->unKey.bit1EdgeMark = 1;
    pstDef->pstEdgeMark        = &g_stIspEdgeMark;
    pstDef->unKey.bit1Wdr      = 1;
    pstDef->pstWdr             = &g_stIspWDR;
    pstDef->unKey.bit1Lsc      = 0;
    pstDef->pstLsc             = &g_stCmosLsc;
    pstDef->unKey.bit1Dpc      = 1;
    pstDef->pstDpc             = &g_stCmosDpc;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            pstDef->unKey.bit1Demosaic       = 1;
            pstDef->pstDemosaic              = &g_stIspDemosaic;
            pstDef->unKey.bit1Sharpen        = 1;
            pstDef->pstSharpen               = &g_stIspYuvSharpen;
            pstDef->unKey.bit1Drc            = 1;
            pstDef->pstDrc                   = &g_stIspDRC;
            pstDef->unKey.bit1Ca             = 1;
            pstDef->pstCa                    = &g_stIspCA;
            pstDef->unKey.bit1Gamma          = 1;
            pstDef->pstGamma                 = &g_stIspGamma;
            pstDef->unKey.bit1BayerNr        = 1;
            pstDef->pstBayerNr               = &g_stIspBayerNr;
            pstDef->unKey.bit1Ge             = 1;
            pstDef->pstGe                    = &g_stIspGe;
            pstDef->unKey.bit1AntiFalseColor = 1;
            pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspLdci;
            memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
        case WDR_MODE_2To1_LINE:
            pstDef->unKey.bit1Demosaic       = 1;
            pstDef->pstDemosaic              = &g_stIspDemosaicWdr;
            pstDef->unKey.bit1Sharpen        = 1;
            pstDef->pstSharpen               = &g_stIspYuvSharpenWdr;
            pstDef->unKey.bit1Drc            = 1;
            pstDef->pstDrc                   = &g_stIspDRCWDR;
            pstDef->unKey.bit1Ca       = 1;
            pstDef->pstCa              = &g_stIspCAWDR;
            pstDef->unKey.bit1Gamma          = 1;
            pstDef->pstGamma                 = &g_stIspGammaFSWDR;
            pstDef->unKey.bit1PreGamma       = 0;
            pstDef->pstPreGamma              = &g_stPreGamma;
            pstDef->unKey.bit1BayerNr        = 1;
            pstDef->pstBayerNr               = &g_stIspBayerNrWdr2To1;
            pstDef->unKey.bit1Ge             = 1;
            pstDef->pstGe                    = &g_stIspWdrGe;
            pstDef->unKey.bit1AntiFalseColor = 1;
            pstDef->pstAntiFalseColor        = &g_stIspWdrAntiFalseColor;
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspWdrLdci;
            pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x40;
            memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
    }

    pstDef->stSensorMode.u32SensorID = IMX334_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));
    switch (pstSnsState->u8ImgMode) {
        default:
        case IMX334_8M_30FPS_12BIT_LINEAR_MODE:
        case IMX334_8M_30FPS_12BIT_2t1_DOL_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12;
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;
            break;
    }
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Denominator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Numerator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Denominator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Numerator = 1;
    pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimRows = 2;
    pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimCols = 2;
    pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatRows = 2;
    pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatCols = 2;
    pstDef->stSensorMode.stDngRawFormat.enCfaLayout = CFALAYOUT_TYPE_RECTANGULAR;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[2] = 2;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 2;
    pstDef->stSensorMode.bValidDngRawFormat = HI_TRUE;
    return HI_SUCCESS;
}

static HI_S32 cmos_get_isp_black_level(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i = 0;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* It need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    /* black level of linear mode */
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        for (i = 0; i < 4; i++) {
            pstBlackLevel->au16BlackLevel[i] = 200;
        }
    } else {

        pstBlackLevel->au16BlackLevel[0] = 200;
        pstBlackLevel->au16BlackLevel[1] = 200;
        pstBlackLevel->au16BlackLevel[2] = 200;
        pstBlackLevel->au16BlackLevel[3] = 200;
    }

    return HI_SUCCESS;

}
static HI_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        u32FullLines_5Fps = (IMX334_VMAX_8M_30FPS_12BIT_2TO1_WDR * 30) / 5;
    } else {
        if (pstSnsState->u8ImgMode == IMX334_8M_30FPS_12BIT_LINEAR_MODE) {
            u32FullLines_5Fps = (IMX334_VMAX_8M_30FPS_12BIT_LINEAR * 30) / 5;
        } else {
            return;
        }
    }

    if (bEnable) { /* setup for ISP pixel calibration mode */
        /* Sensor must be programmed for slow frame rate (5 fps and below) */
        /* change frame rate to 5 fps by setting 1 frame length  */
        /* Analog and Digital gains both must be programmed for their minimum values */
        imx334_write_register(ViPipe, IMX334_PGC_L, 0x00);
        imx334_write_register(ViPipe, IMX334_PGC_H, 0x00);

        imx334_write_register(ViPipe, IMX334_VMAX_L, u32FullLines_5Fps & 0xFF);
        imx334_write_register(ViPipe, IMX334_VMAX_M, (u32FullLines_5Fps & 0xFF00) >> 8);
        imx334_write_register(ViPipe, IMX334_VMAX_H, (u32FullLines_5Fps & 0xF0000) >> 16);
    } else { /* setup for ISP 'normal mode' */
        pstSnsState->u32FLStd = (pstSnsState->u32FLStd > IMX334_FULL_LINES_MAX) ? IMX334_FULL_LINES_MAX : pstSnsState->u32FLStd;
        imx334_write_register(ViPipe, IMX334_VMAX_L, (pstSnsState->u32FLStd & 0xff));
        imx334_write_register(ViPipe, IMX334_VMAX_M, (pstSnsState->u32FLStd & 0xff00) >> 8);
        imx334_write_register(ViPipe, IMX334_VMAX_H, (pstSnsState->u32FLStd & 0xf0000) >> 16);
        pstSnsState->bSyncInit = HI_FALSE;
    }

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;

    switch (u8Mode) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
            break;

        case WDR_MODE_2To1_LINE:
            pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
            printf("2to1 line WDR 4k mode(60fps->30fps)\n");
            break;

        default:
            ISP_ERR_TRACE("NOT support this mode!\n");
            return HI_FAILURE;
    }

    memset(pstSnsState->au32WDRIntTime, 0, sizeof(pstSnsState->au32WDRIntTime));

    return HI_SUCCESS;
}

static HI_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    HI_S32 i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
        pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunImx334BusInfo[ViPipe].s8I2cDev;
        pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        pstSnsState->astRegsInfo[0].u32RegNum = 8;

        if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
            pstSnsState->astRegsInfo[0].u32RegNum += 8;
            pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        }

        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = imx334_i2c_addr;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = imx334_addr_byte;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = imx334_data_byte;
        }

        // SHR0 related
        pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = IMX334_SHR0_L;
        pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = IMX334_SHR0_M;
        pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = IMX334_SHR0_H;

        // gain related
        pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = IMX334_PGC_L;
        pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = IMX334_PGC_H;
        pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;

        // Vmax
        pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX334_VMAX_L;
        pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX334_VMAX_M;
        pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX334_VMAX_H;

        // DOL 2t1 Mode Regs
        if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
            pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = IMX334_SHR0_L;
            pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = IMX334_SHR0_M;
            pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = IMX334_SHR0_H;

            pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = IMX334_PGC_L;
            pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = IMX334_PGC_H;

            pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX334_SHR1_L;
            pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX334_SHR1_M;
            pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX334_SHR1_H;

            pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1;
            pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = IMX334_VMAX_L;
            pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1;
            pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = IMX334_VMAX_M;
            pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1;
            pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = IMX334_VMAX_H;

            pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = IMX334_RHS1_L;
            pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = IMX334_RHS1_M;
            pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = IMX334_RHS1_H;

            pstSnsState->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr = IMX334_Y_OUTSIZE_L;
            pstSnsState->astRegsInfo[0].astI2cData[15].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[15].u32RegAddr = IMX334_Y_OUTSIZE_H;

        }

        pstSnsState->bSyncInit = HI_TRUE;
    } else {
        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data == pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_FALSE;
            } else {

                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
            }
        }
    }

    pstSnsRegsInfo->bConfig = HI_FALSE;
    memcpy(pstSnsRegsInfo, &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy(&pstSnsState->astRegsInfo[1], &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));

    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return HI_SUCCESS;
}

static HI_S32 cmos_set_image_mode(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode;
    HI_U32 u32W, u32H;
    HI_FLOAT f32Fps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;
    u8SensorImageMode = pstSnsState->u8ImgMode;
    u32H = pstSensorImageMode->u16Height;
    u32W = pstSensorImageMode->u16Width;
    f32Fps = pstSensorImageMode->f32Fps;
    u8SensorImageMode = pstSnsState->u8ImgMode;

    if (f32Fps <= 30) {
        if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
            if (IMX334_RES_IS_4K30_12BIT(u32W, u32H)) {
                u8SensorImageMode     = IMX334_8M_30FPS_12BIT_LINEAR_MODE;
                pstSnsState->u32FLStd = IMX334_VMAX_8M_30FPS_12BIT_LINEAR;
            } else {
                ISP_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                          pstSensorImageMode->u16Width,
                          pstSensorImageMode->u16Height,
                          pstSensorImageMode->f32Fps,
                          pstSnsState->enWDRMode);
                return HI_FAILURE;
            }
        } else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
            if (IMX334_RES_IS_4K30_12BIT(u32W, u32H)) {
                u8SensorImageMode                = IMX334_8M_30FPS_12BIT_2t1_DOL_MODE;
                g_astimx334State[ViPipe].u32BRL  = 2200;
                pstSnsState->u32FLStd            = IMX334_VMAX_8M_30FPS_12BIT_2TO1_WDR * 2;
            } else {
                ISP_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                          pstSensorImageMode->u16Width,
                          pstSensorImageMode->u16Height,
                          pstSensorImageMode->f32Fps,
                          pstSnsState->enWDRMode);
                return HI_FAILURE;
            }
        } else {
            ISP_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                      pstSensorImageMode->u16Width,
                      pstSensorImageMode->u16Height,
                      pstSensorImageMode->f32Fps,
                      pstSnsState->enWDRMode);
            return HI_FAILURE;
        }

    } else {
    }

    /* Switch SensorImageMode */
    if ((pstSnsState->bInit == HI_TRUE) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }

    pstSnsState->u8ImgMode = u8SensorImageMode;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX334_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = IMX334_8M_30FPS_12BIT_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = IMX334_8M_30FPS_12BIT_LINEAR_MODE;
    pstSnsState->au32FL[0] = IMX334_8M_30FPS_12BIT_LINEAR_MODE;
    pstSnsState->au32FL[1] = IMX334_8M_30FPS_12BIT_LINEAR_MODE;

    memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = imx334_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = imx334_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;
    pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return HI_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static HI_S32 imx334_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunImx334BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX334_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    IMX334_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX334_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    IMX334_SENSOR_RESET_CTX(ViPipe);
}

static HI_S32 sensor_register_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    ISP_SNS_ATTR_INFO_S   stSnsAttrInfo;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = sensor_ctx_init(ViPipe);

    if (s32Ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    stSnsAttrInfo.eSensorId = IMX334_ID;

    s32Ret  = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret |= HI_MPI_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo, &stIspRegister);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor register callback function failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret |= HI_MPI_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret |= HI_MPI_AWB_SensorRegCallBack(ViPipe, pstAwbLib, &stSnsAttrInfo, &stAwbRegister);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor register callback function to awb lib failed!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 sensor_unregister_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(ViPipe, IMX334_ID);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(ViPipe, pstAeLib, IMX334_ID);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, IMX334_ID);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    sensor_ctx_exit(ViPipe);

    return HI_SUCCESS;
}

static HI_S32 sensor_set_init(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr)
{
    CMOS_CHECK_POINTER(pstInitAttr);

    g_au32InitExposure[ViPipe] = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[ViPipe][0] = pstInitAttr->u16WBRgain;
    g_au16InitWBGain[ViPipe][1] = pstInitAttr->u16WBGgain;
    g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain;
    g_au16SampleRgain[ViPipe] = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[ViPipe] = pstInitAttr->u16SampleBgain;

    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsImx334Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = imx334_standby,
    .pfnRestart             = imx334_restart,
    .pfnMirrorFlip          = HI_NULL,
    .pfnWriteReg            = imx334_write_register,
    .pfnReadReg             = imx334_read_register,
    .pfnSetBusInfo          = imx334_set_bus_info,
    .pfnSetInit             = sensor_set_init
};


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __IMX334_CMOS_H_ */
