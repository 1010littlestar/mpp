/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#if !defined(__SHARP8K_CMOS_H_)
#define __SHARP8K_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "hi_comm_sns.h"
#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#include "sharp8k_cmos_ex.h"
#include "sharp8k_slave_priv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#define SHARP8K_ID                  833

/* InckPerHs = Inck/MaxFps/InckVertiLines */
const SHARP8K_VIDEO_MODE_TBL_S g_astSharp8kModeTbl[SHARP8K_MODE_BUTT] = {
    {27000000, 848, 900000, 4368, 30, "8K_30FPS_RAW12_SLVS_16LANE"}, /* Actually 30fps */
};

HI_S32 g_Sharp8kSlaveBindDev[ISP_MAX_PIPE_NUM] = {0, 0, 1, 1, 2, 2, 3, 3};
HI_U32 g_Sharp8kSalveSensorModeTime[ISP_MAX_PIPE_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};

ISP_SLAVE_SNS_SYNC_S        gstSharp8kSync[ISP_MAX_PIPE_NUM];

ISP_SNS_STATE_S *g_pastSharp8k[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define SHARP8K_SENSOR_GET_CTX(dev, pstCtx)   (pstCtx = g_pastSharp8k[dev])
#define SHARP8K_SENSOR_SET_CTX(dev, pstCtx)   (g_pastSharp8k[dev] = pstCtx)
#define SHARP8K_SENSOR_RESET_CTX(dev)         (g_pastSharp8k[dev] = HI_NULL)

ISP_SNS_COMMBUS_U g_aunSharp8kBusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = {.s8SspDev = { .bit4SspDev = 0, .bit4SspCs = 0}},
    [1] = {.s8SspDev = { .bit4SspDev = 1, .bit4SspCs = 0}},
    [2] = {.s8SspDev = { .bit4SspDev = 2, .bit4SspCs = 0}},
    [3] = {.s8SspDev = { .bit4SspDev = 3, .bit4SspCs = 0}},
    [4] = {.s8SspDev = { .bit4SspDev = 4, .bit4SspCs = 0}},
    [5] = {.s8SspDev = { .bit4SspDev = 5, .bit4SspCs = 0}},
    [6] = {.s8SspDev = { .bit4SspDev = 4, .bit4SspCs = 0}},
    [7] = {.s8SspDev = { .bit4SspDev = 5, .bit4SspCs = 0}}
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static HI_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/

extern void sharp8k_init(VI_PIPE ViPipe);
extern void sharp8k_exit(VI_PIPE ViPipe);
extern void sharp8k_standby(VI_PIPE ViPipe);
extern void sharp8k_restart(VI_PIPE ViPipe);
extern int  sharp8k_write_register(VI_PIPE ViPipe, int addr, int data);
extern int  sharp8k_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define SHARP8K_EXP_TIME_ADDR_L     (0x0C)  //shutter[7:0]
#define SHARP8K_EXP_TIME_ADDR_H     (0x0B)  //shutter[15:8]

#define SHARP8K_AGAIN_ADDR_L        (0x14)  // AGAIN[7:0]
#define SHARP8K_AGAIN_ADDR_H        (0x13)  // AGAIN[9:8]

#define SHARP8K_AGAIN_ADDR_CHANGE_1 (0x036D)
#define SHARP8K_AGAIN_ADDR_CHANGE_2 (0x04F3)
#define SHARP8K_AGAIN_ADDR_CHANGE_3 (0x0371)
#define SHARP8K_AGAIN_ADDR_CHANGE_4 (0x04F7)
#define SHARP8K_AGAIN_ADDR_CHANGE_5 (0x0443)
#define SHARP8K_AGAIN_ADDR_CHANGE_6 (0x00EF)
#define SHARP8K_AGAIN_ADDR_CHANGE_7 (0x0129)
#define SHARP8K_AGAIN_ADDR_CHANGE_8 (0x012A)
#define SHARP8K_AGAIN_ADDR_CHANGE_9 (0x012B)

#define SHARP8K_DGAIN_ADDR_GR_L     (0x16)  // DGAIN_GR[11:8]
#define SHARP8K_DGAIN_ADDR_GR_H     (0x15)  // DGAIN_GR[7:0]

#define SHARP8K_DGAIN_ADDR_R_L      (0x18)  // DGAIN_R[11:8]
#define SHARP8K_DGAIN_ADDR_R_H      (0x17)  // DGAIN_R[7:0]

#define SHARP8K_DGAIN_ADDR_B_L      (0x1A)  // DGAIN_B[11:8]
#define SHARP8K_DGAIN_ADDR_B_H      (0x19)  // DGAIN_B[7:0]

#define SHARP8K_DGAIN_ADDR_GB_L     (0x1C)  // DGAIN_GB[11:8]
#define SHARP8K_DGAIN_ADDR_GB_H     (0x1B)  // DGAIN_GB[7:0]

#define SHARP8K_VMAX_ADDR_L         (0x1E)  // Frame-lines[7:0]
#define SHARP8K_VMAX_ADDR_H         (0x1D)  // Frame-lines[15:8]

#define SHARP8K_FPS_ADDR_L1         (0x21)  // Frame-Adjust registers
#define SHARP8K_FPS_ADDR_L2         (0x22)
#define SHARP8K_FPS_ADDR_L3         (0x23)
#define SHARP8K_FPS_ADDR_L4         (0x24)

#define SHARP8K_FPS_ADDR_R1         (0x25)
#define SHARP8K_FPS_ADDR_R2         (0x26)
#define SHARP8K_FPS_ADDR_R3         (0x27)
#define SHARP8K_FPS_ADDR_R4         (0x28)

#define SHARP8K_FULL_LINES_MAX      (0xFFFF)
#define SHARP8K_8K30FPS_VMAX_LINEAR (4368)

static HI_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32LinesPer500ms = (pstSnsState->u32FLStd * 30) / 2;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->u32FullLinesMax = SHARP8K_FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 2;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 8 << pstAeSnsDft->u32ISPDgainShift;

    pstAeSnsDft->u32InitExposure = 2560000;

    memcpy(&pstAeSnsDft->stPirisAttr, &gstPirisAttr, sizeof(ISP_PIRIS_ATTR_S));
    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:   /* linear mode */
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u8AeCompensation = 0x38;
            pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4;
            pstAeSnsDft->u32MinIntTime = 2;
            pstAeSnsDft->u32MaxIntTimeTarget = SHARP8K_FULL_LINES_MAX - 4;
            pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;

            pstAeSnsDft->u32MaxAgain = 8192;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 16352;
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
            break;
    }
    return HI_SUCCESS;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE ViPipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32MaxFps = 0;
    HI_U32 u32Lines = 0;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    HI_DOUBLE diff = 0.0;
    HI_DOUBLE quan = 0.0;
    HI_U32 u32RigistersL = 0;
    HI_U32 u32RigistersR = 0;
    HI_U32 u32LineLength = 412;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32MaxFps = g_astSharp8kModeTbl[pstSnsState->u8ImgMode].u32MaxFps;

    u32Lines = 54000000 / (DIV_0_TO_1_FLOAT(f32Fps) * u32LineLength);
    u32Lines = (u32Lines > SHARP8K_FULL_LINES_MAX) ? SHARP8K_FULL_LINES_MAX : u32Lines;
    u32Lines = u32Lines - (u32Lines % 2);

    diff = 2 * 54000000 / (HI_DOUBLE)(DIV_0_TO_1_FLOAT(f32Fps) * u32Lines) - 2 * u32LineLength;
    u32RigistersL = round(diff * 2147483648);

    quan = (HI_DOUBLE)u32RigistersL - (diff * 2147483648);

    if (quan > 0) {
        u32RigistersR = u32RigistersL - 1;
    } else if (quan < 0) {
        u32RigistersR = u32RigistersL + 1;
    } else {
        u32RigistersR = u32RigistersL;
    }

    /* SHR 16bit, So limit full_lines as 0xFFFF */
    if (f32Fps > u32MaxFps || u32Lines > SHARP8K_FULL_LINES_MAX) {
        ISP_ERR_TRACE("Not support Fps: %f\n", f32Fps);
        return;
    } else {
    }

    gstSharp8kSync[ViPipe].u32VsTime = (g_astSharp8kModeTbl[pstSnsState->u8ImgMode].u32InckPerVs) * u32MaxFps / DIV_0_TO_1_FLOAT(f32Fps);
    pstSnsState->astRegsInfo[0].stSlvSync.u32SlaveVsTime = gstSharp8kSync[ViPipe].u32VsTime;

    pstSnsState->astRegsInfo[0].astSspData[21].u32Data = LOW_8BITS(u32Lines);
    pstSnsState->astRegsInfo[0].astSspData[22].u32Data = HIGH_8BITS(u32Lines);

    /* set frame adjust registers to 0x21-0x28 */
    pstSnsState->astRegsInfo[0].astSspData[23].u32Data = X_32BITS(u32RigistersL);
    pstSnsState->astRegsInfo[0].astSspData[24].u32Data = X_24BITS(u32RigistersL);
    pstSnsState->astRegsInfo[0].astSspData[25].u32Data = X_16BITS(u32RigistersL);
    pstSnsState->astRegsInfo[0].astSspData[26].u32Data = X_8BITS(u32RigistersL);

    pstSnsState->astRegsInfo[0].astSspData[27].u32Data = X_32BITS(u32RigistersR);
    pstSnsState->astRegsInfo[0].astSspData[28].u32Data = X_24BITS(u32RigistersR);
    pstSnsState->astRegsInfo[0].astSspData[29].u32Data = X_16BITS(u32RigistersR);
    pstSnsState->astRegsInfo[0].astSspData[30].u32Data = X_8BITS(u32RigistersR);

    pstAeSnsDft->f32Fps = f32Fps;
    pstSnsState->u32FLStd = u32Lines;
    pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * f32Fps / 2;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    return;
}

static HI_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, HI_U32 u32FullLines,
                                       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_DOUBLE dtimes = 0.0;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32FullLines = (u32FullLines > SHARP8K_FULL_LINES_MAX) ? SHARP8K_FULL_LINES_MAX : u32FullLines;
    u32FullLines = u32FullLines - (u32FullLines % 2);

    dtimes = (HI_DOUBLE)(u32FullLines) / (HI_DOUBLE)(g_astSharp8kModeTbl[pstSnsState->u8ImgMode].u32VertiLines);

    gstSharp8kSync[ViPipe].u32VsTime = (HI_U32)(g_astSharp8kModeTbl[pstSnsState->u8ImgMode].u32InckPerVs * dtimes);

    pstSnsState->astRegsInfo[0].astSspData[21].u32Data = LOW_8BITS(u32FullLines);
    pstSnsState->astRegsInfo[0].astSspData[22].u32Data = HIGH_8BITS(u32FullLines);
    pstSnsState->astRegsInfo[0].stSlvSync.u32SlaveVsTime = gstSharp8kSync[ViPipe].u32VsTime;

    pstSnsState->au32FL[0] = u32FullLines;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 4;

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE ViPipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    /* Exposure Time */
    pstSnsState->astRegsInfo[0].astSspData[0].u32Data = LOW_8BITS(u32IntTime);
    pstSnsState->astRegsInfo[0].astSspData[1].u32Data = HIGH_8BITS(u32IntTime);

    return;
}

static HI_U32 again_index[97] = {
    1024, 1056, 1088, 1120, 1152, 1184, 1216, 1248, 1280, 1312, 1344, 1376, 1408, 1440, 1472, 1504, 1536, 1568, 1600, 1632, 1664, 1696, 1728, 1760, 1792, 1824, 1856, 1888, 1920, 1952, 1984, 2016, 2048,
    2112, 2176, 2240, 2304, 2368, 2432, 2496, 2560, 2624, 2688, 2752, 2816, 2880, 2944, 3008, 3072, 3136, 3200, 3264, 3328, 3392, 3456, 3520, 3584, 3648, 3712, 3776, 3840, 3904, 3968, 4032, 4096,
    4224, 4352, 4480, 4608, 4736, 4864, 4992, 5120, 5248, 5376, 5504, 5632, 5760, 5888, 6016, 6144, 6272, 6400, 6528, 6656, 6784, 6912, 7040, 7168, 7296, 7424, 7552, 7680, 7808, 7936, 8064, 8192,
};

static HI_U32 again_table[97] = {
    0, 31, 60, 87, 113, 138, 161, 183, 204, 224, 243, 261, 279, 295, 311, 326, 341, 355, 368, 381, 393, 405, 417, 428, 438, 449, 459, 468, 477, 486, 495, 503, 512,
    527, 542, 555, 568, 581, 592, 603, 614, 624, 633, 642, 651, 659, 667, 675, 682, 689, 696, 702, 708, 714, 720, 726, 731, 736, 741, 746, 750, 755, 759, 763, 768,
    775, 783, 789, 796, 802, 808, 813, 819, 824, 828, 833, 837, 841, 845, 849, 853, 856, 860, 863, 866, 869, 872, 875, 877, 880, 882, 885, 887, 889, 891, 893, 896,
};

static HI_U32 dgain_index[480] = {
    1024, 1056, 1088, 1120, 1152, 1184, 1216, 1248, 1280, 1312, 1344, 1376, 1408, 1440, 1472, 1504, 1536, 1568, 1600, 1632, 1664, 1696, 1728, 1760, 1792, 1824, 1856, 1888, 1920, 1952, 1984, 2016,
    2048, 2080, 2112, 2144, 2176, 2208, 2240, 2272, 2304, 2336, 2368, 2400, 2432, 2464, 2496, 2528, 2560, 2592, 2624, 2656, 2688, 2720, 2752, 2784, 2816, 2848, 2880, 2912, 2944, 2976, 3008, 3040,
    3072, 3104, 3136, 3168, 3200, 3232, 3264, 3296, 3328, 3360, 3392, 3424, 3456, 3488, 3520, 3552, 3584, 3616, 3648, 3680, 3712, 3744, 3776, 3808, 3840, 3872, 3904, 3936, 3968, 4000, 4032, 4064,
    4096, 4128, 4160, 4192, 4224, 4256, 4288, 4320, 4352, 4384, 4416, 4448, 4480, 4512, 4544, 4576, 4608, 4640, 4672, 4704, 4736, 4768, 4800, 4832, 4864, 4896, 4928, 4960, 4992, 5024, 5056, 5088,
    5120, 5152, 5184, 5216, 5248, 5280, 5312, 5344, 5376, 5408, 5440, 5472, 5504, 5536, 5568, 5600, 5632, 5664, 5696, 5728, 5760, 5792, 5824, 5856, 5888, 5920, 5952, 5984, 6016, 6048, 6080, 6112,
    6144, 6176, 6208, 6240, 6272, 6304, 6336, 6368, 6400, 6432, 6464, 6496, 6528, 6560, 6592, 6624, 6656, 6688, 6720, 6752, 6784, 6816, 6848, 6880, 6912, 6944, 6976, 7008, 7040, 7072, 7104, 7136,
    7168, 7200, 7232, 7264, 7296, 7328, 7360, 7392, 7424, 7456, 7488, 7520, 7552, 7584, 7616, 7648, 7680, 7712, 7744, 7776, 7808, 7840, 7872, 7904, 7936, 7968, 8000, 8032, 8064, 8096, 8128, 8160,
    8192, 8224, 8256, 8288, 8320, 8352, 8384, 8416, 8448, 8480, 8512, 8544, 8576, 8608, 8640, 8672, 8704, 8736, 8768, 8800, 8832, 8864, 8896, 8928, 8960, 8992, 9024, 9056, 9088, 9120, 9152, 9184,
    9216, 9248, 9280, 9312, 9344, 9376, 9408, 9440, 9472, 9504, 9536, 9568, 9600, 9632, 9664, 9696, 9728, 9760, 9792, 9824, 9856, 9888, 9920, 9952, 9984, 10016, 10048, 10080, 10112, 10144, 10176, 10208,
    10240, 10272, 10304, 10336, 10368, 10400, 10432, 10464, 10496, 10528, 10560, 10592, 10624, 10656, 10688, 10720, 10752, 10784, 10816, 10848, 10880, 10912, 10944, 10976, 11008, 11040, 11072, 11104, 11136, 11168, 11200, 11232,
    11264, 11296, 11328, 11360, 11392, 11424, 11456, 11488, 11520, 11552, 11584, 11616, 11648, 11680, 11712, 11744, 11776, 11808, 11840, 11872, 11904, 11936, 11968, 12000, 12032, 12064, 12096, 12128, 12160, 12192, 12224, 12256,
    12288, 12320, 12352, 12384, 12416, 12448, 12480, 12512, 12544, 12576, 12608, 12640, 12672, 12704, 12736, 12768, 12800, 12832, 12864, 12896, 12928, 12960, 12992, 13024, 13056, 13088, 13120, 13152, 13184, 13216, 13248, 13280,
    13312, 13344, 13376, 13408, 13440, 13472, 13504, 13536, 13568, 13600, 13632, 13664, 13696, 13728, 13760, 13792, 13824, 13856, 13888, 13920, 13952, 13984, 14016, 14048, 14080, 14112, 14144, 14176, 14208, 14240, 14272, 14304,
    14336, 14368, 14400, 14432, 14464, 14496, 14528, 14560, 14592, 14624, 14656, 14688, 14720, 14752, 14784, 14816, 14848, 14880, 14912, 14944, 14976, 15008, 15040, 15072, 15104, 15136, 15168, 15200, 15232, 15264, 15296, 15328,
    15360, 15392, 15424, 15456, 15488, 15520, 15552, 15584, 15616, 15648, 15680, 15712, 15744, 15776, 15808, 15840, 15872, 15904, 15936, 15968, 16000, 16032, 16064, 16096, 16128, 16160, 16192, 16224, 16256, 16288, 16320, 16352,
};

static HI_U32 dgain_table[480] = {
    0x100, 0x108, 0x110, 0x118, 0x120, 0x128, 0x130, 0x138, 0x140, 0x148, 0x150, 0x158, 0x160, 0x168, 0x170, 0x178, 0x180, 0x188, 0x190, 0x198, 0x1A0, 0x1A8, 0x1B0, 0x1B8, 0x1C0, 0x1C8, 0x1D0, 0x1D8, 0x1E0, 0x1E8, 0x1F0, 0x1F8,
    0x200, 0x208, 0x210, 0x218, 0x220, 0x228, 0x230, 0x238, 0x240, 0x248, 0x250, 0x258, 0x260, 0x268, 0x270, 0x278, 0x280, 0x288, 0x290, 0x298, 0x2A0, 0x2A8, 0x2B0, 0x2B8, 0x2C0, 0x2C8, 0x2D0, 0x2D8, 0x2E0, 0x2E8, 0x2F0, 0x2F8,
    0x300, 0x308, 0x310, 0x318, 0x320, 0x328, 0x330, 0x338, 0x340, 0x348, 0x350, 0x358, 0x360, 0x368, 0x370, 0x378, 0x380, 0x388, 0x390, 0x398, 0x3A0, 0x3A8, 0x3B0, 0x3B8, 0x3C0, 0x3C8, 0x3D0, 0x3D8, 0x3E0, 0x3E8, 0x3F0, 0x3F8,
    0x400, 0x408, 0x410, 0x418, 0x420, 0x428, 0x430, 0x438, 0x440, 0x448, 0x450, 0x458, 0x460, 0x468, 0x470, 0x478, 0x480, 0x488, 0x490, 0x498, 0x4A0, 0x4A8, 0x4B0, 0x4B8, 0x4C0, 0x4C8, 0x4D0, 0x4D8, 0x4E0, 0x4E8, 0x4F0, 0x4F8,
    0x500, 0x508, 0x510, 0x518, 0x520, 0x528, 0x530, 0x538, 0x540, 0x548, 0x550, 0x558, 0x560, 0x568, 0x570, 0x578, 0x580, 0x588, 0x590, 0x598, 0x5A0, 0x5A8, 0x5B0, 0x5B8, 0x5C0, 0x5C8, 0x5D0, 0x5D8, 0x5E0, 0x5E8, 0x5F0, 0x5F8,
    0x600, 0x608, 0x610, 0x618, 0x620, 0x628, 0x630, 0x638, 0x640, 0x648, 0x650, 0x658, 0x660, 0x668, 0x670, 0x678, 0x680, 0x688, 0x690, 0x698, 0x6A0, 0x6A8, 0x6B0, 0x6B8, 0x6C0, 0x6C8, 0x6D0, 0x6D8, 0x6E0, 0x6E8, 0x6F0, 0x6F8,
    0x700, 0x708, 0x710, 0x718, 0x720, 0x728, 0x730, 0x738, 0x740, 0x748, 0x750, 0x758, 0x760, 0x768, 0x770, 0x778, 0x780, 0x788, 0x790, 0x798, 0x7A0, 0x7A8, 0x7B0, 0x7B8, 0x7C0, 0x7C8, 0x7D0, 0x7D8, 0x7E0, 0x7E8, 0x7F0, 0x7F8,
    0x800, 0x808, 0x810, 0x818, 0x820, 0x828, 0x830, 0x838, 0x840, 0x848, 0x850, 0x858, 0x860, 0x868, 0x870, 0x878, 0x880, 0x888, 0x890, 0x898, 0x8A0, 0x8A8, 0x8B0, 0x8B8, 0x8C0, 0x8C8, 0x8D0, 0x8D8, 0x8E0, 0x8E8, 0x8F0, 0x8F8,
    0x900, 0x908, 0x910, 0x918, 0x920, 0x928, 0x930, 0x938, 0x940, 0x948, 0x950, 0x958, 0x960, 0x968, 0x970, 0x978, 0x980, 0x988, 0x990, 0x998, 0x9A0, 0x9A8, 0x9B0, 0x9B8, 0x9C0, 0x9C8, 0x9D0, 0x9D8, 0x9E0, 0x9E8, 0x9F0, 0x9F8,
    0xA00, 0xA08, 0xA10, 0xA18, 0xA20, 0xA28, 0xA30, 0xA38, 0xA40, 0xA48, 0xA50, 0xA58, 0xA60, 0xA68, 0xA70, 0xA78, 0xA80, 0xA88, 0xA90, 0xA98, 0xAA0, 0xAA8, 0xAB0, 0xAB8, 0xAC0, 0xAC8, 0xAD0, 0xAD8, 0xAE0, 0xAE8, 0xAF0, 0xAF8,
    0xB00, 0xB08, 0xB10, 0xB18, 0xB20, 0xB28, 0xB30, 0xB38, 0xB40, 0xB48, 0xB50, 0xB58, 0xB60, 0xB68, 0xB70, 0xB78, 0xB80, 0xB88, 0xB90, 0xB98, 0xBA0, 0xBA8, 0xBB0, 0xBB8, 0xBC0, 0xBC8, 0xBD0, 0xBD8, 0xBE0, 0xBE8, 0xBF0, 0xBF8,
    0xC00, 0xC08, 0xC10, 0xC18, 0xC20, 0xC28, 0xC30, 0xC38, 0xC40, 0xC48, 0xC50, 0xC58, 0xC60, 0xC68, 0xC70, 0xC78, 0xC80, 0xC88, 0xC90, 0xC98, 0xCA0, 0xCA8, 0xCB0, 0xCB8, 0xCC0, 0xCC8, 0xCD0, 0xCD8, 0xCE0, 0xCE8, 0xCF0, 0xCF8,
    0xD00, 0xD08, 0xD10, 0xD18, 0xD20, 0xD28, 0xD30, 0xD38, 0xD40, 0xD48, 0xD50, 0xD58, 0xD60, 0xD68, 0xD70, 0xD78, 0xD80, 0xD88, 0xD90, 0xD98, 0xDA0, 0xDA8, 0xDB0, 0xDB8, 0xDC0, 0xDC8, 0xDD0, 0xDD8, 0xDE0, 0xDE8, 0xDF0, 0xDF8,
    0xE00, 0xE08, 0xE10, 0xE18, 0xE20, 0xE28, 0xE30, 0xE38, 0xE40, 0xE48, 0xE50, 0xE58, 0xE60, 0xE68, 0xE70, 0xE78, 0xE80, 0xE88, 0xE90, 0xE98, 0xEA0, 0xEA8, 0xEB0, 0xEB8, 0xEC0, 0xEC8, 0xED0, 0xED8, 0xEE0, 0xEE8, 0xEF0, 0xEF8,
    0xF00, 0xF08, 0xF10, 0xF18, 0xF20, 0xF28, 0xF30, 0xF38, 0xF40, 0xF48, 0xF50, 0xF58, 0xF60, 0xF68, 0xF70, 0xF78, 0xF80, 0xF88, 0xF90, 0xF98, 0xFA0, 0xFA8, 0xFB0, 0xFB8, 0xFC0, 0xFC8, 0xFD0, 0xFD8, 0xFE0, 0xFE8, 0xFF0, 0xFF8,
};


static HI_VOID cmos_again_calc_table(VI_PIPE ViPipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (*pu32AgainLin >= again_index[96]) {
        *pu32AgainLin = again_index[96];
        *pu32AgainDb = again_table[96];
        return;
    }

    for (i = 1; i < 97; i++) {
        if (*pu32AgainLin < again_index[i]) {
            *pu32AgainLin = again_index[i - 1];
            *pu32AgainDb = again_table[i - 1];
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

    if (*pu32DgainLin >= dgain_index[479]) {
        *pu32DgainLin = dgain_index[479];
        *pu32DgainDb = dgain_table[479];
        return;
    }

    for (i = 1; i < 480; i++) {
        if (*pu32DgainLin < dgain_index[i]) {
            *pu32DgainLin = dgain_index[i - 1];
            *pu32DgainDb = dgain_table[i - 1];
            break;
        }
    }
    return;
}


static HI_VOID cmos_gains_update(VI_PIPE ViPipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U8 u8Reg0x0013, u8Reg0x0014;           /* AG */
    HI_U8 u8Reg0x0015, u8Reg0x0016;           /* DG_GR */
    HI_U8 u8Reg0x0017, u8Reg0x0018;           /* DG_R */
    HI_U8 u8Reg0x0019, u8Reg0x001A;           /* DG_B */
    HI_U8 u8Reg0x001B, u8Reg0x001C;           /* DG_GB */

    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u8Reg0x0013 = HIGH_8BITS(u32Again);
    u8Reg0x0014 = LOW_8BITS(u32Again);

    u8Reg0x0015 = HIGH_8BITS(u32Dgain);
    u8Reg0x0016 = LOW_8BITS(u32Dgain);

    u8Reg0x0017 = HIGH_8BITS(u32Dgain);
    u8Reg0x0018 = LOW_8BITS(u32Dgain);

    u8Reg0x0019 = HIGH_8BITS(u32Dgain);
    u8Reg0x001A = LOW_8BITS(u32Dgain);

    u8Reg0x001B = HIGH_8BITS(u32Dgain);
    u8Reg0x001C = LOW_8BITS(u32Dgain);


    /* Again */
    pstSnsState->astRegsInfo[0].astSspData[2].u32Data = u8Reg0x0014;
    pstSnsState->astRegsInfo[0].astSspData[3].u32Data = u8Reg0x0013;

    if (u32Again >= 0 && u32Again <= 512) {
        pstSnsState->astRegsInfo[0].astSspData[4].u32Data = 0xB0;
        pstSnsState->astRegsInfo[0].astSspData[5].u32Data = 0xB0;
        pstSnsState->astRegsInfo[0].astSspData[6].u32Data = 0xD8;
        pstSnsState->astRegsInfo[0].astSspData[7].u32Data = 0xD8;

        pstSnsState->astRegsInfo[0].astSspData[8].u32Data = 0x05;
        pstSnsState->astRegsInfo[0].astSspData[9].u32Data = 0x20;
        pstSnsState->astRegsInfo[0].astSspData[10].u32Data = 0x01;
        pstSnsState->astRegsInfo[0].astSspData[11].u32Data = 0x03;
        pstSnsState->astRegsInfo[0].astSspData[12].u32Data = 0x09;
    } else if (u32Again > 512 && u32Again <= 768) {
        pstSnsState->astRegsInfo[0].astSspData[4].u32Data = 0xA0;
        pstSnsState->astRegsInfo[0].astSspData[5].u32Data = 0xA0;
        pstSnsState->astRegsInfo[0].astSspData[6].u32Data = 0xC8;
        pstSnsState->astRegsInfo[0].astSspData[7].u32Data = 0xC8;

        pstSnsState->astRegsInfo[0].astSspData[8].u32Data = 0x08;
        pstSnsState->astRegsInfo[0].astSspData[9].u32Data = 0x60;
        pstSnsState->astRegsInfo[0].astSspData[10].u32Data = 0x80;
        pstSnsState->astRegsInfo[0].astSspData[11].u32Data = 0x00;
        pstSnsState->astRegsInfo[0].astSspData[12].u32Data = 0x00;
    } else if (u32Again > 768 && u32Again <= 896) {
        pstSnsState->astRegsInfo[0].astSspData[4].u32Data = 0x88;
        pstSnsState->astRegsInfo[0].astSspData[5].u32Data = 0x88;
        pstSnsState->astRegsInfo[0].astSspData[6].u32Data = 0xB0;
        pstSnsState->astRegsInfo[0].astSspData[7].u32Data = 0xB0;

        pstSnsState->astRegsInfo[0].astSspData[8].u32Data = 0x08;
        pstSnsState->astRegsInfo[0].astSspData[9].u32Data = 0x60;
        pstSnsState->astRegsInfo[0].astSspData[10].u32Data = 0x80;
        pstSnsState->astRegsInfo[0].astSspData[11].u32Data = 0x00;
        pstSnsState->astRegsInfo[0].astSspData[12].u32Data = 0x00;
    }

    /* Dgain */
    pstSnsState->astRegsInfo[0].astSspData[13].u32Data = u8Reg0x0016;
    pstSnsState->astRegsInfo[0].astSspData[14].u32Data = u8Reg0x0015;

    pstSnsState->astRegsInfo[0].astSspData[15].u32Data = u8Reg0x0018;
    pstSnsState->astRegsInfo[0].astSspData[16].u32Data = u8Reg0x0017;

    pstSnsState->astRegsInfo[0].astSspData[17].u32Data = u8Reg0x001A;
    pstSnsState->astRegsInfo[0].astSspData[18].u32Data = u8Reg0x0019;

    pstSnsState->astRegsInfo[0].astSspData[19].u32Data = u8Reg0x001C;
    pstSnsState->astRegsInfo[0].astSspData[20].u32Data = u8Reg0x001B;

    return;
}


static HI_VOID cmos_get_inttime_max(VI_PIPE ViPipe, HI_U16 u16ManRatioEnable, HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    return;
}


/* Only used in Line_WDR mode */
static HI_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

    genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
    gu32MaxTimeGetCnt[ViPipe] = 0;

    return;
}

static HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default     = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set            = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update     = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update       = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table   = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table   = cmos_dgain_calc_table;
    pstExpFuncs->pfn_cmos_get_inttime_max    = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set  = cmos_ae_fswdr_attr_set;

    return HI_SUCCESS;
}

/* Calibration results for Static WB */
#define CALIBRATE_STATIC_WB_R_GAIN  478
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  438

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1            22
#define CALIBRATE_AWB_P2            234
#define CALIBRATE_AWB_Q1            0
#define CALIBRATE_AWB_A1            146060
#define CALIBRATE_AWB_B1            128
#define CALIBRATE_AWB_C1            -94343

static HI_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = 5000;

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

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;
    }

    return HI_SUCCESS;
}


static HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

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
    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));
    pstDef->unKey.bit1Lsc      = 0;
    pstDef->pstLsc             = &g_stCmosLsc;
    pstDef->unKey.bit1Ca       = 1;
    pstDef->pstCa              = &g_stIspCA;
    pstDef->unKey.bit1Clut     = 1;
    pstDef->pstClut            = &g_stIspCLUT;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            pstDef->unKey.bit1Demosaic       = 1;
            pstDef->pstDemosaic              = &g_stIspDemosaic;
            pstDef->unKey.bit1Sharpen        = 1;
            pstDef->pstSharpen               = &g_stIspYuvSharpen;
            pstDef->unKey.bit1EdgeMark       = 0;
            pstDef->pstEdgeMark              = &g_stIspEdgeMark;
            pstDef->unKey.bit1Drc            = 1;
            pstDef->pstDrc                   = &g_stIspDRC;
            pstDef->unKey.bit1Gamma          = 1;
            pstDef->pstGamma                 = &g_stIspGamma;
            pstDef->unKey.bit1BayerNr        = 1;
            pstDef->pstBayerNr               = &g_stIspBayerNr;
            pstDef->unKey.bit1Ge             = 1;
            pstDef->pstGe                    = &g_stIspGe;
            pstDef->unKey.bit1AntiFalseColor = 1;
            pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
            pstDef->unKey.bit1Dpc            = 1;
            pstDef->pstDpc                   = &g_stCmosDpc;
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspLdci;
            memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibratio, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
    }

    pstDef->stSensorMaxResolution.u32MaxWidth  = 7680;
    pstDef->stSensorMaxResolution.u32MaxHeight = 4320;
    pstDef->stSensorMode.u32SensorID = SHARP8K_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));
    switch (pstSnsState->u8ImgMode) {
        default:

        case SHARP8K_8K30FPS_LINER_MODE:
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
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 2;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 0;
    pstDef->stSensorMode.bValidDngRawFormat = HI_TRUE;

    return HI_SUCCESS;
}


static HI_S32 cmos_get_isp_black_level(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    for (i = 0; i < 4; i++) {
        pstBlackLevel->au16BlackLevel[i] = 0x80;
    }
    return HI_SUCCESS;
}

static HI_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    HI_U32 u32MaxFps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    }

    /* Detect set 5fps */
    u32MaxFps = g_astSharp8kModeTbl[pstSnsState->u8ImgMode].u32MaxFps;
    CHECK_RET(HI_MPI_ISP_GetSnsSlaveAttr(ViPipe, &gstSharp8kSync[ViPipe]));

    gstSharp8kSync[ViPipe].u32VsTime = ((g_astSharp8kModeTbl[pstSnsState->u8ImgMode].u32InckPerVs * u32MaxFps)) / 5;
    u32FullLines_5Fps = (g_astSharp8kModeTbl[pstSnsState->u8ImgMode].u32VertiLines * 30) / 5;
    u32MaxIntTime_5Fps = u32FullLines_5Fps - 4;

    if (bEnable) { /* setup for ISP pixel calibration mode */
        sharp8k_write_register(ViPipe, SHARP8K_AGAIN_ADDR_L, 0x00);                            /* min AG */
        sharp8k_write_register(ViPipe, SHARP8K_AGAIN_ADDR_H, 0x00);                            /* min AG */

        sharp8k_write_register(ViPipe, SHARP8K_DGAIN_ADDR_GR_L, 0x00);                         /* min DG_GR */
        sharp8k_write_register(ViPipe, SHARP8K_DGAIN_ADDR_GR_H, 0x01);                         /* min DG_GR */

        sharp8k_write_register(ViPipe, SHARP8K_DGAIN_ADDR_R_L, 0x00);                          /* min DG_R */
        sharp8k_write_register(ViPipe, SHARP8K_DGAIN_ADDR_R_H, 0x01);                          /* min DG_R */

        sharp8k_write_register(ViPipe, SHARP8K_DGAIN_ADDR_B_L, 0x00);                          /* min DG_B */
        sharp8k_write_register(ViPipe, SHARP8K_DGAIN_ADDR_B_H, 0x01);                          /* min DG_B */

        sharp8k_write_register(ViPipe, SHARP8K_DGAIN_ADDR_GB_L, 0x00);                         /* min DG_GB */
        sharp8k_write_register(ViPipe, SHARP8K_DGAIN_ADDR_GB_H, 0x01);                         /* min DG_GB */

        sharp8k_write_register(ViPipe, SHARP8K_VMAX_ADDR_L, LOW_8BITS(u32FullLines_5Fps));
        sharp8k_write_register(ViPipe, SHARP8K_VMAX_ADDR_H, HIGH_8BITS(u32FullLines_5Fps));

        sharp8k_write_register(ViPipe, SHARP8K_EXP_TIME_ADDR_L, LOW_8BITS(u32MaxIntTime_5Fps));
        sharp8k_write_register(ViPipe, SHARP8K_EXP_TIME_ADDR_H, HIGH_8BITS(u32MaxIntTime_5Fps));

    } else { /* setup for ISP 'normal mode' */
        gstSharp8kSync[ViPipe].u32VsTime = (g_astSharp8kModeTbl->u32InckPerVs);

        sharp8k_write_register(ViPipe, SHARP8K_VMAX_ADDR_L, LOW_8BITS(pstSnsState->u32FLStd));
        sharp8k_write_register(ViPipe, SHARP8K_VMAX_ADDR_H, HIGH_8BITS(pstSnsState->u32FLStd));

        pstSnsState->bSyncInit = HI_FALSE;
    }

    CHECK_RET(HI_MPI_ISP_SetSnsSlaveAttr(ViPipe, &gstSharp8kSync[ViPipe]));

    return;
}


static HI_S32  cmos_set_wdr_mode(VI_PIPE ViPipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;

    switch (u8Mode) {
        case WDR_MODE_NONE:
            if (pstSnsState->u8ImgMode == SHARP8K_8K30FPS_LINER_MODE) {
                pstSnsState->u32FLStd = g_astSharp8kModeTbl[pstSnsState->u8ImgMode].u32VertiLines;
            }
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            ISP_ERR_TRACE("Sharp 8k30fps linear mode!\n");
            break;

        default:
            ISP_ERR_TRACE("NOT support this mode!\n");
            return HI_FAILURE;
    }

    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    memset(pstSnsState->au32WDRIntTime, 0, sizeof(pstSnsState->au32WDRIntTime));
    return HI_SUCCESS;
}

static HI_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    HI_S32 i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_SSP_TYPE;
        pstSnsState->astRegsInfo[0].unComBus.s8SspDev.bit4SspDev = g_aunSharp8kBusInfo[ViPipe].s8SspDev.bit4SspDev;
        pstSnsState->astRegsInfo[0].unComBus.s8SspDev.bit4SspCs  = g_aunSharp8kBusInfo[ViPipe].s8SspDev.bit4SspCs;
        pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        pstSnsState->astRegsInfo[0].u32RegNum = 31;

        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            pstSnsState->astRegsInfo[0].astSspData[i].bUpdate = HI_TRUE;
            pstSnsState->astRegsInfo[0].astSspData[i].u32DevAddr = 0x1;
            pstSnsState->astRegsInfo[0].astSspData[i].u32DevAddrByteNum = 1;
            pstSnsState->astRegsInfo[0].astSspData[i].u32RegAddrByteNum = 1;
            pstSnsState->astRegsInfo[0].astSspData[i].u32DataByteNum = 1;
        }

        /* ExpTime */
        pstSnsState->astRegsInfo[0].astSspData[0].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[0].u32RegAddr = SHARP8K_EXP_TIME_ADDR_L;
        pstSnsState->astRegsInfo[0].astSspData[1].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[1].u32RegAddr = SHARP8K_EXP_TIME_ADDR_H;

        /* AG */
        pstSnsState->astRegsInfo[0].astSspData[2].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[2].u32RegAddr = SHARP8K_AGAIN_ADDR_L;
        pstSnsState->astRegsInfo[0].astSspData[3].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[3].u32RegAddr = SHARP8K_AGAIN_ADDR_H;

        pstSnsState->astRegsInfo[0].astSspData[4].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[4].u32RegAddr = SHARP8K_AGAIN_ADDR_CHANGE_1;
        pstSnsState->astRegsInfo[0].astSspData[5].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[5].u32RegAddr = SHARP8K_AGAIN_ADDR_CHANGE_2;
        pstSnsState->astRegsInfo[0].astSspData[6].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[6].u32RegAddr = SHARP8K_AGAIN_ADDR_CHANGE_3;
        pstSnsState->astRegsInfo[0].astSspData[7].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[7].u32RegAddr = SHARP8K_AGAIN_ADDR_CHANGE_4;

        pstSnsState->astRegsInfo[0].astSspData[8].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[8].u32RegAddr = SHARP8K_AGAIN_ADDR_CHANGE_5;
        pstSnsState->astRegsInfo[0].astSspData[9].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[9].u32RegAddr = SHARP8K_AGAIN_ADDR_CHANGE_6;
        pstSnsState->astRegsInfo[0].astSspData[10].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[10].u32RegAddr = SHARP8K_AGAIN_ADDR_CHANGE_7;
        pstSnsState->astRegsInfo[0].astSspData[11].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[11].u32RegAddr = SHARP8K_AGAIN_ADDR_CHANGE_8;
        pstSnsState->astRegsInfo[0].astSspData[12].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[12].u32RegAddr = SHARP8K_AGAIN_ADDR_CHANGE_9;

        /* DG */
        pstSnsState->astRegsInfo[0].astSspData[13].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[13].u32RegAddr = SHARP8K_DGAIN_ADDR_GR_L;
        pstSnsState->astRegsInfo[0].astSspData[14].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[14].u32RegAddr = SHARP8K_DGAIN_ADDR_GR_H;

        pstSnsState->astRegsInfo[0].astSspData[15].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[15].u32RegAddr = SHARP8K_DGAIN_ADDR_R_L;
        pstSnsState->astRegsInfo[0].astSspData[16].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[16].u32RegAddr = SHARP8K_DGAIN_ADDR_R_H;

        pstSnsState->astRegsInfo[0].astSspData[17].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[17].u32RegAddr = SHARP8K_DGAIN_ADDR_B_L;
        pstSnsState->astRegsInfo[0].astSspData[18].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[18].u32RegAddr = SHARP8K_DGAIN_ADDR_B_H;

        pstSnsState->astRegsInfo[0].astSspData[19].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[19].u32RegAddr = SHARP8K_DGAIN_ADDR_GB_L;
        pstSnsState->astRegsInfo[0].astSspData[20].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[20].u32RegAddr = SHARP8K_DGAIN_ADDR_GB_H;

        /* VMAX */
        pstSnsState->astRegsInfo[0].astSspData[21].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[21].u32RegAddr = SHARP8K_VMAX_ADDR_L;

        pstSnsState->astRegsInfo[0].astSspData[22].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[22].u32RegAddr = SHARP8K_VMAX_ADDR_H;

        /* FrameRate_Adjustment registers */
        pstSnsState->astRegsInfo[0].astSspData[23].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[23].u32RegAddr = SHARP8K_FPS_ADDR_L1;
        pstSnsState->astRegsInfo[0].astSspData[24].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[24].u32RegAddr = SHARP8K_FPS_ADDR_L2;

        pstSnsState->astRegsInfo[0].astSspData[25].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[25].u32RegAddr = SHARP8K_FPS_ADDR_L3;
        pstSnsState->astRegsInfo[0].astSspData[26].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[26].u32RegAddr = SHARP8K_FPS_ADDR_L4;

        pstSnsState->astRegsInfo[0].astSspData[27].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[27].u32RegAddr = SHARP8K_FPS_ADDR_R1;
        pstSnsState->astRegsInfo[0].astSspData[28].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[28].u32RegAddr = SHARP8K_FPS_ADDR_R2;

        pstSnsState->astRegsInfo[0].astSspData[29].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[29].u32RegAddr = SHARP8K_FPS_ADDR_R3;
        pstSnsState->astRegsInfo[0].astSspData[30].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[30].u32RegAddr = SHARP8K_FPS_ADDR_R4;

        /* Slave Sensor VsTime cfg */
        pstSnsState->astRegsInfo[0].stSlvSync.bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].stSlvSync.u8DelayFrmNum = 1;

        pstSnsState->astRegsInfo[0].astSspData[4].u8IntPos = 1;
        pstSnsState->astRegsInfo[0].astSspData[5].u8IntPos = 1;
        pstSnsState->astRegsInfo[0].astSspData[6].u8IntPos = 1;
        pstSnsState->astRegsInfo[0].astSspData[7].u8IntPos = 1;
        pstSnsState->astRegsInfo[0].astSspData[8].u8IntPos = 1;
        pstSnsState->astRegsInfo[0].astSspData[9].u8IntPos = 1;
        pstSnsState->astRegsInfo[0].astSspData[10].u8IntPos = 1;
        pstSnsState->astRegsInfo[0].astSspData[11].u8IntPos = 1;
        pstSnsState->astRegsInfo[0].astSspData[12].u8IntPos = 1;

        pstSnsState->astRegsInfo[0].stSlvSync.u32SlaveBindDev = g_Sharp8kSlaveBindDev[ViPipe];

        pstSnsState->bSyncInit = HI_TRUE;

    } else {
        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            if (pstSnsState->astRegsInfo[0].astSspData[i].u32Data == pstSnsState->astRegsInfo[1].astSspData[i].u32Data) {

                pstSnsState->astRegsInfo[0].astSspData[i].bUpdate = HI_FALSE;
            } else {
                pstSnsState->astRegsInfo[0].astSspData[i].bUpdate = HI_TRUE;
            }
        }

        if (pstSnsState->astRegsInfo[0].stSlvSync.u32SlaveVsTime == pstSnsState->astRegsInfo[1].stSlvSync.u32SlaveVsTime) {
            pstSnsState->astRegsInfo[0].stSlvSync.bUpdate = HI_FALSE;

        } else {
            pstSnsState->astRegsInfo[0].stSlvSync.bUpdate = HI_TRUE;

        }
    }

    pstSnsRegsInfo->bConfig = HI_FALSE;
    memcpy(pstSnsRegsInfo, &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy(&pstSnsState->astRegsInfo[1], &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));

    return HI_SUCCESS;
}

static HI_S32 cmos_set_image_mode(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode = 0;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;

    if ((pstSensorImageMode->u16Width <= 7680) && (pstSensorImageMode->u16Height <= 4320)) {
        u8SensorImageMode = SHARP8K_8K30FPS_LINER_MODE;
    } else {
        ISP_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                  pstSensorImageMode->u16Width,
                  pstSensorImageMode->u16Height,
                  pstSensorImageMode->f32Fps,
                  pstSnsState->enWDRMode);
        return HI_FAILURE;
    }

    /* Sensor first init */
    if (pstSnsState->bInit == HI_FALSE) {
        pstSnsState->u8ImgMode = u8SensorImageMode;
        return HI_SUCCESS;
    }

    /* Switch SensorImageMode */
    if (u8SensorImageMode == pstSnsState->u8ImgMode) {
        /* Don't need to switch SensorImageMode */
        return HI_FAILURE;
    }

    pstSnsState->u8ImgMode = u8SensorImageMode;
    memset(pstSnsState->au32WDRIntTime, 0, sizeof(pstSnsState->au32WDRIntTime));

    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SHARP8K_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit     = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = SHARP8K_8K30FPS_LINER_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd  = SHARP8K_8K30FPS_VMAX_LINEAR;
    pstSnsState->au32FL[0] = SHARP8K_8K30FPS_VMAX_LINEAR;
    pstSnsState->au32FL[1] = SHARP8K_8K30FPS_VMAX_LINEAR;

    memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init         = sharp8k_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit         = sharp8k_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init  = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode      = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode        = cmos_set_wdr_mode;
    pstSensorExpFunc->pfn_cmos_get_isp_default     = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect    = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info    = cmos_get_sns_regs_info;

    return HI_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static HI_S32 sharp8k_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunSharp8kBusInfo[ViPipe].s8SspDev.bit4SspDev = unSNSBusInfo.s8SspDev.bit4SspDev;
    g_aunSharp8kBusInfo[ViPipe].s8SspDev.bit4SspCs  = unSNSBusInfo.s8SspDev.bit4SspCs;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    SHARP8K_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    SHARP8K_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    SHARP8K_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    SHARP8K_SENSOR_RESET_CTX(ViPipe);
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

    stSnsAttrInfo.eSensorId = SHARP8K_ID;

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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(ViPipe, SHARP8K_ID);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SHARP8K_ID);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SHARP8K_ID);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    sensor_ctx_exit(ViPipe);

    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsSharp8kObj = {
    .pfnRegisterCallback   = sensor_register_callback,
    .pfnUnRegisterCallback = sensor_unregister_callback,
    .pfnStandby            = sharp8k_standby,
    .pfnRestart            = sharp8k_restart,
    .pfnMirrorFlip         = HI_NULL,
    .pfnWriteReg           = sharp8k_write_register,
    .pfnReadReg            = sharp8k_read_register,
    .pfnSetBusInfo         = sharp8k_set_bus_info
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __SHARP8K_CMOS_H_ */
