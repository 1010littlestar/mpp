/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#if !defined(__IMX477_CMOS_H_)
#define __IMX477_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_comm_sns.h"
#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#include "imx477_slave_priv.h"
#include "imx477_cmos_ex.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define IMX477_ID                       477
#define IMX477_FULL_LINES_STD_FPGA_COMP 0
#define IMX477_VS_TIME_MAX              0xFFFFFFFF
#define IMX477_FLL_MAX                  0xFFFF

#define IMX477_AGAIN_TBL_RANGE          62
#define IMX477_DGAIN_TBL_RANGE          97

// sensor gain
#define IMX477_AGAIN_21X                (21504)
#define IMX477_AGAIN_MAX                (21845)

#define IMX477_DGAIN_1X                 (1024)
#define IMX477_DGAIN_8X                 (8192)
#define IMX477_DGAIN_9X                 (9216)
#define IMX477_DGAIN_10X                (10240)
#define IMX477_DGAIN_11X                (11264)
#define IMX477_DGAIN_12X                (12288)
#define IMX477_DGAIN_13X                (13312)
#define IMX477_DGAIN_14X                (14336)
#define IMX477_DGAIN_15X                (15360)
#define IMX477_DGAIN_16X                (16384-1)

extern unsigned char imx477_i2c_addr;
extern unsigned int  imx477_addr_byte;
extern unsigned int  imx477_data_byte;

const IMX477_VIDEO_MODE_TBL_S g_astImx477ModeTbl[IMX477_MODE_BUTT] = {
    {24000000, 228.5714, 800000, 3500, 30,     "4K3K_4CH_12BIT_30FPS"  }, // MODE0
    {24000000, 352.0004, 800800, 2275, 29.97,  "4K2K_4CH_12BIT_30FPS"  }, // MODE1
    {24000000, 180.5869, 400000, 2215, 60,     "4K2K_4CH_12BIT_60FPS"  }, // MODE2
    {24000000, 142.8571, 480000, 3360, 50,     "3K3K_4CH_10BIT_50FPS"  }, // MODE3
    {24000000, 88.5734,  100088, 1130, 239.79, "2K1K_4CH_10BIT_240FPS" }, // real fps: 239.79
    {24000000, 131.1926, 400400, 3052, 59.94,  "3K3K_4CH_10BIT_60FPS"  },
    {24000000, 86.6667,  100100, 1155, 239.76, "720P_4CH_10BIT_240FPS" }, //real fps: 239.76
};

ISP_SLAVE_SNS_SYNC_S gstImx477Sync[ISP_MAX_PIPE_NUM];


/****************************************************************************
 * extern function reference                                                *
 ****************************************************************************/

extern void imx477_init(VI_PIPE ViPipe);
extern void imx477_exit(VI_PIPE ViPipe);
extern void imx477_standby(VI_PIPE ViPipe);
extern void imx477_restart(VI_PIPE ViPipe);
extern void imx477_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);
extern int  imx477_write_register(VI_PIPE ViPipe, int addr, int data);
extern int  imx477_read_register(VI_PIPE ViPipe, int addr);
extern HI_S32 ISP_SetSnsSlaveUnSyncAttr(SLAVE_DEV SlaveDev, const ISP_SLAVE_SNS_SYNC_S *pstSnsSync);

ISP_SNS_STATE_S *g_pastImx477[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define IMX477_SENSOR_GET_CTX(dev, pstCtx)   (pstCtx = g_pastImx477[dev])
#define IMX477_SENSOR_SET_CTX(dev, pstCtx)   (g_pastImx477[dev] = pstCtx)
#define IMX477_SENSOR_RESET_CTX(dev)         (g_pastImx477[dev] = HI_NULL)

ISP_SNS_COMMBUS_U g_aunImx477BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

static HI_U32   g_u32Imx477AGain[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 1024};
static HI_U32   g_u32Imx477DGain[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 1024};

/* Depart different sensor mode to get CCM/AWB param */
HI_U8 g_u8SensorImageMode = IMX477_12M30FPS_LINER_MODE;
HI_U8 g_u8Imx477LensMode = IMX477_SINGLE_LENS_MODE;

/* Sensor slave mode, default binding setting : Slave[0]->Pipe[0]&[1]; Slave[1]->Pipe[2]&[3]; Slave[2]->Pipe[4]&[5] */
HI_S32 g_Imx477SlaveBindDev[ISP_MAX_PIPE_NUM] = {0, 0, 1, 1, 2, 2, 3, 3};
HI_U32 g_Imx477SlaveSensorModeTime[ISP_MAX_PIPE_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};

static HI_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};

static HI_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32Fll;
    HI_FLOAT F32MaxFps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u32Fll = g_astImx477ModeTbl[pstSnsState->u8ImgMode].u32VertiLines;
    F32MaxFps = g_astImx477ModeTbl[pstSnsState->u8ImgMode].f32MaxFps;
    pstSnsState->u32FLStd = u32Fll;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256;
    pstAeSnsDft->u32HmaxTimes = (1000000) / (u32Fll * F32MaxFps);

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;

    if (g_au32InitExposure[ViPipe] == 0) {
        pstAeSnsDft->u32InitExposure = 1000000;
    } else {
        pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe];
    }

    if (g_au32LinesPer500ms[ViPipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = ((HI_U64)(u32Fll * F32MaxFps)) >> 1;
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
    }

    switch (pstSnsState->u8ImgMode) {
        case IMX477_12M30FPS_LINER_MODE:
        case IMX477_8M30FPS_LINER_MODE:
        case IMX477_8M60FPS_LINER_MODE:
        case IMX477_9M50FPS_LINER_MODE:
        case IMX477_9M60FPS_LINER_MODE:
        case IMX477_2M240FPS_LINER_MODE:
        case IMX477_1M240FPS_LINER_MODE:
            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.221;
            break;

        default:
            ISP_ERR_TRACE("NOT support this mode!\n");
            break;
    }

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.3;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.3;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 6 << pstAeSnsDft->u32ISPDgainShift;

    memcpy(&pstAeSnsDft->stPirisAttr, &gstPirisAttr, sizeof(ISP_PIRIS_ATTR_S));
    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE: {
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u8AeCompensation = 0x38;

            pstAeSnsDft->u32MinIntTime = 8;
            pstAeSnsDft->u32MaxIntTimeTarget = 65515;
            pstAeSnsDft->u32MinIntTimeTarget = 8;

            pstAeSnsDft->u32MaxAgain = 21845;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 16384 - 1; /* if Dgain enable,please set ispdgain bigger than 1 */
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            break;
        }
    }

    return HI_SUCCESS;
}


static HI_VOID cmos_fps_set(VI_PIPE ViPipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_FLOAT f32maxFps;
    HI_U32 u32Lines;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    f32maxFps = g_astImx477ModeTbl[pstSnsState->u8ImgMode].f32MaxFps;
    u32Lines = (g_astImx477ModeTbl[pstSnsState->u8ImgMode].u32VertiLines) * f32maxFps / DIV_0_TO_1_FLOAT(f32Fps);

    /* SHR was 16bit, so limit full_lines as 0xFFFF */
    if (f32Fps > f32maxFps || u32Lines > FULL_LINES_MAX) {
        ISP_ERR_TRACE("Not support Fps: %f\n", f32Fps);
        return;
    }

    gstImx477Sync[ViPipe].u32VsTime = (g_astImx477ModeTbl[pstSnsState->u8ImgMode].u32InckPerVs) * (f32maxFps / DIV_0_TO_1_FLOAT(f32Fps));
    pstSnsState->u32FLStd = (g_astImx477ModeTbl[pstSnsState->u8ImgMode].u32VertiLines) * (f32maxFps / DIV_0_TO_1_FLOAT(f32Fps));

    pstSnsState->astRegsInfo[0].stSlvSync.u32SlaveVsTime = gstImx477Sync[ViPipe].u32VsTime;
    pstSnsState->u32FLStd = (pstSnsState->u32FLStd > FULL_LINES_MAX) ? FULL_LINES_MAX : pstSnsState->u32FLStd;

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = (pstSnsState->u32FLStd & 0xFF);
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (pstSnsState->u32FLStd & 0xFF00) >> 8;

    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = (pstSnsState->u32FLStd & 0xFF);
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = (pstSnsState->u32FLStd & 0xFF00) >> 8;

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = (HI_U32)(g_astImx477ModeTbl[pstSnsState->u8ImgMode].u32VertiLines*f32maxFps/2);

    switch (pstSnsState->u8ImgMode) {
        case IMX477_12M30FPS_LINER_MODE:
        case IMX477_8M30FPS_LINER_MODE:
        case IMX477_8M60FPS_LINER_MODE:
        case IMX477_9M50FPS_LINER_MODE:
        case IMX477_9M60FPS_LINER_MODE:
        case IMX477_2M240FPS_LINER_MODE:
        case IMX477_1M240FPS_LINER_MODE:
            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 20;
            break;

        default:
            ISP_ERR_TRACE("NOT support this mode!\n");
            return;
    }

    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32HmaxTimes = (1000000) / (pstSnsState->u32FLStd * DIV_0_TO_1_FLOAT(f32Fps));

    return;
}


static HI_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, HI_U32 u32FullLines,
                                       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32FullLines = MIN(u32FullLines, IMX477_FLL_MAX);
    gstImx477Sync[ViPipe].u32VsTime =  MIN((HI_U64)(g_astImx477ModeTbl[pstSnsState->u8ImgMode].f32InckPerHs * u32FullLines), IMX477_VS_TIME_MAX);
    pstSnsState->au32FL[0] = u32FullLines;
    pstSnsState->astRegsInfo[0].stSlvSync.u32SlaveVsTime = gstImx477Sync[ViPipe].u32VsTime;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = (pstSnsState->au32FL[0] & 0xFF);
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (pstSnsState->au32FL[0] & 0xFF00) >> 8;

    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = (pstSnsState->au32FL[0] & 0xFF);
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = (pstSnsState->au32FL[0] & 0xFF00) >> 8;

    switch (pstSnsState->u8ImgMode) {
        case IMX477_12M30FPS_LINER_MODE:
        case IMX477_8M30FPS_LINER_MODE:
        case IMX477_8M60FPS_LINER_MODE:
        case IMX477_9M50FPS_LINER_MODE:
        case IMX477_9M60FPS_LINER_MODE:
        case IMX477_2M240FPS_LINER_MODE:
        case IMX477_1M240FPS_LINER_MODE:
            pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 20;
            break;

        default:
            ISP_ERR_TRACE("NOT support this mode!\n");
            return;
    }

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE ViPipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->au32FL[0] = u32IntTime;

    // SET CORASE_INTEG_TIME
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = LOW_8BITS(pstSnsState->au32FL[0]);
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]);

    return;
}

/* Again segmentation = 62 */
static HI_U32 ad_gain_table[IMX477_AGAIN_TBL_RANGE] = {
    1024, 1040, 1057, 1074, 1092, 1111, 1130, 1150, 1170, 1192,
    1214, 1237, 1260, 1285, 1311, 1337, 1365, 1394, 1425, 1456,
    1489, 1524, 1560, 1598, 1638, 1680, 1725, 1771, 1820, 1872,
    1928, 1986, 2048, 2114, 2185, 2260, 2341, 2427, 2521, 2621,
    2731, 2849, 2979, 3121, 3277, 3449, 3641, 3855, 4096, 4369,
    4681, 5041, 5461, 5958, 6554, 7282, 8192, 9362, 10923, 13107,
    16384, 21845
};

static HI_VOID cmos_again_calc_table(VI_PIPE ViPipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (*pu32AgainLin >= ad_gain_table[IMX477_AGAIN_TBL_RANGE - 1]) {
        *pu32AgainLin = ad_gain_table[IMX477_AGAIN_TBL_RANGE - 1];
        *pu32AgainDb = IMX477_AGAIN_TBL_RANGE - 1;
        goto calc_table_end;
    }

    for (i = 1; i < IMX477_AGAIN_TBL_RANGE; i++) {
        if (*pu32AgainLin < ad_gain_table[i]) {
            *pu32AgainLin = ad_gain_table[i - 1];
            *pu32AgainDb = i - 1;
            goto calc_table_end;;
        }
    }


calc_table_end:

    g_u32Imx477AGain[ViPipe] = *pu32AgainLin;

    *pu32AgainDb <<= 4;

    return;
}

/* dgain segmentation = 97 */
static HI_U32 dgain_table[IMX477_DGAIN_TBL_RANGE] = {
    1024, 1034, 1044, 1055, 1066, 1077, 1088, 1099, 1111, 1123,
    1135, 1147, 1160, 1173, 1186, 1200, 1214, 1228, 1242, 1257,
    1273, 1288, 1304, 1321, 1337, 1355, 1372, 1391, 1409, 1429,
    1448, 1469, 1489, 1511, 1533, 1556, 1579, 1603, 1628, 1654,
    1680, 1708, 1736, 1765, 1796, 1827, 1859, 1893, 1928, 1964,
    2001, 2040, 2081, 2123, 2166, 2212, 2260, 2310, 2362, 2416,
    2473, 2533, 2595, 2661, 2731, 2804, 2881, 2962, 3048, 3139,
    3236, 3339, 3449, 3567, 3692, 3827, 3972, 4128, 4297, 4481,
    4681, 4900, 5140, 5405, 5699, 6026, 6394, 6809, 7282, 7825,
    8456, 9198, 10082, 11155, 12483, 14170, (16384 - 1)
};

static HI_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, HI_U32 *pu32DgainLin, HI_U32 *pu32DgainDb)
{
    CMOS_CHECK_POINTER_VOID(pu32DgainLin);
    CMOS_CHECK_POINTER_VOID(pu32DgainDb);

    if (*pu32DgainLin >= dgain_table[IMX477_DGAIN_TBL_RANGE - 1]) {
        *pu32DgainLin = dgain_table[IMX477_DGAIN_TBL_RANGE - 1];
    }
    g_u32Imx477DGain[ViPipe] = *pu32DgainLin;
    *pu32DgainDb = *pu32DgainLin;

    return;

}


static HI_VOID cmos_gains_update(VI_PIPE ViPipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    // Again
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = (u32Again & 0xFF);
    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = ((u32Again >> 8) & 0x0003);

    // Dgain
    if (u32Dgain % 1024 != 0) {
        pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = (((u32Dgain % 1024) >> 2) & 0x00FF);
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = ((u32Dgain >> 10) & 0xF);
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 1;
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = 0xFF;
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = ((u32Dgain >> 10) & 0xF) - 1;
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 1;

    }

    return;
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

    return HI_SUCCESS;
}

static HI_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    CMOS_CHECK_POINTER(pstAwbSnsDft);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = 5120;

    if (g_u8Imx477LensMode == IMX477_SINGLE_LENS_MODE) {
        pstAwbSnsDft->au16GainOffset[0] = 491; // CALIBRATE_STATIC_WB_R_GAIN;
        pstAwbSnsDft->au16GainOffset[1] = 0X100;  // CALIBRATE_STATIC_WB_GR_GAIN;
        pstAwbSnsDft->au16GainOffset[2] = 0x100;  // CALIBRATE_STATIC_WB_GB_GAIN;
        pstAwbSnsDft->au16GainOffset[3] = 458; // CALIBRATE_STATIC_WB_B_GAIN;

        pstAwbSnsDft->as32WbPara[0] = (-4); // CALIBRATE_AWB_P1;
        pstAwbSnsDft->as32WbPara[1] = 235; // CALIBRATE_AWB_P2;
        pstAwbSnsDft->as32WbPara[2] = (-24); // CALIBRATE_AWB_Q1;
        pstAwbSnsDft->as32WbPara[3] = 153042; // CALIBRATE_AWB_A1;
        pstAwbSnsDft->as32WbPara[4] = 128; // CALIBRATE_AWB_B1;
        pstAwbSnsDft->as32WbPara[5] = (-101980); // CALIBRATE_AWB_C1;

        pstAwbSnsDft->u16GoldenRgain = 0; // GOLDEN_RGAIN;
        pstAwbSnsDft->u16GoldenBgain = 0; // GOLDEN_BGAIN;

        memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm_NormalLens, sizeof(AWB_CCM_S));
    } else if (g_u8Imx477LensMode == IMX477_2FISH_LENS_MODE) {
        pstAwbSnsDft->au16GainOffset[0] = 549; // CALIBRATE_STATIC_WB_R_GAIN;
        pstAwbSnsDft->au16GainOffset[1] = 0X100;  // CALIBRATE_STATIC_WB_GR_GAIN;
        pstAwbSnsDft->au16GainOffset[2] = 0x100;  // CALIBRATE_STATIC_WB_GB_GAIN;
        pstAwbSnsDft->au16GainOffset[3] = 480; // CALIBRATE_STATIC_WB_B_GAIN;

        pstAwbSnsDft->as32WbPara[0] = 81; // CALIBRATE_AWB_P1;
        pstAwbSnsDft->as32WbPara[1] = 109; // CALIBRATE_AWB_P2;
        pstAwbSnsDft->as32WbPara[2] = (-67); // CALIBRATE_AWB_Q1;
        pstAwbSnsDft->as32WbPara[3] = 156314; // CALIBRATE_AWB_A1;
        pstAwbSnsDft->as32WbPara[4] = 128; // CALIBRATE_AWB_B1;
        pstAwbSnsDft->as32WbPara[5] = (-100470); // CALIBRATE_AWB_C1;

        pstAwbSnsDft->u16GoldenRgain = 0; // GOLDEN_RGAIN;
        pstAwbSnsDft->u16GoldenBgain = 0; // GOLDEN_BGAIN;

        memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm_FishLens, sizeof(AWB_CCM_S));
    }

    memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
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
    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));
    pstDef->unKey.bit1Ca       = 1;
    pstDef->pstCa              = &g_stIspCA;
    pstDef->unKey.bit1Clut     = 1;
    pstDef->pstClut            = &g_stIspCLUT;
    pstDef->unKey.bit1Wdr      = 1;
    pstDef->pstWdr             = &g_stIspWDR;
    pstDef->unKey.bit1Dpc      = 1;
    pstDef->pstDpc             = &g_stCmosDpc;
    pstDef->unKey.bit1Awb      = 1;
    pstDef->stAwbAttr.enAwbGainSwitch = ISP_AWB_GAIN_IN_ISP;

    if (g_u8Imx477LensMode == IMX477_SINGLE_LENS_MODE) {
        pstDef->unKey.bit1Lsc      = 1;
        pstDef->pstLsc             = &g_stCmosLsc_NormalLens;
    } else if (g_u8Imx477LensMode == IMX477_2FISH_LENS_MODE) {
        if (ViPipe == 0) {
            pstDef->unKey.bit1Lsc      = 1;
            pstDef->pstLsc             = &g_stCmosLsc_FishlLens_Pipe0;
        } else {
            pstDef->unKey.bit1Lsc      = 1;
            pstDef->pstLsc             = &g_stCmosLsc_FishlLens_Pipe1;
        }
    }

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            pstDef->unKey.bit1Demosaic       = 1;
            pstDef->pstDemosaic              = &g_stIspDemosaic;
            pstDef->unKey.bit1Drc            = 1;
            pstDef->pstDrc                   = &g_stIspDRC;
            pstDef->unKey.bit1Gamma          = 1;
            pstDef->pstGamma                 = &g_stIspGamma;
            pstDef->unKey.bit1BayerNr        = 1;
            pstDef->pstBayerNr               = &g_stIspBayerNr;
            pstDef->unKey.bit1Sharpen        = 1;
            pstDef->pstSharpen               = &g_stIspYuvSharpen;
            pstDef->unKey.bit1EdgeMark       = 0;
            pstDef->pstEdgeMark              = &g_stIspEdgeMark;
            pstDef->unKey.bit1Ge             = 1;
            pstDef->pstGe                    = &g_stIspGe;
            pstDef->unKey.bit1AntiFalseColor = 1;
            pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspLdci;
            memcpy(&pstDef->stNoiseCalibration,    &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
    }

    pstDef->stSensorMaxResolution.u32MaxWidth  = 4056;
    pstDef->stSensorMaxResolution.u32MaxHeight = 3040;
    pstDef->stSensorMode.u32SensorID = IMX477_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));
    switch (pstSnsState->u8ImgMode) {
        default:
        case IMX477_8M60FPS_LINER_MODE:
        case IMX477_8M30FPS_LINER_MODE:
        case IMX477_12M30FPS_LINER_MODE:

            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12;
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;
            break;

        case IMX477_9M50FPS_LINER_MODE:
        case IMX477_9M60FPS_LINER_MODE:
        case IMX477_2M240FPS_LINER_MODE:
        case IMX477_1M240FPS_LINER_MODE:

            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 1023;

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
    CMOS_CHECK_POINTER(pstBlackLevel);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    if (pstBlackLevel->bUpdate == HI_TRUE) {
        if (g_u32Imx477AGain[ViPipe] >= IMX477_AGAIN_21X) {
            if ((g_u32Imx477DGain[ViPipe] <= IMX477_DGAIN_8X)) {
                pstBlackLevel->au16BlackLevel[0] = 258;
                pstBlackLevel->au16BlackLevel[1] = 261;
                pstBlackLevel->au16BlackLevel[2] = 261;
                pstBlackLevel->au16BlackLevel[3] = 258;
            }
            if ((g_u32Imx477DGain[ViPipe] > IMX477_DGAIN_8X) && (g_u32Imx477DGain[ViPipe] <= IMX477_DGAIN_9X)) {
                pstBlackLevel->au16BlackLevel[0] = 261;
                pstBlackLevel->au16BlackLevel[1] = 262;
                pstBlackLevel->au16BlackLevel[2] = 262;
                pstBlackLevel->au16BlackLevel[3] = 261;
            } else if ((g_u32Imx477DGain[ViPipe] > IMX477_DGAIN_9X) && (g_u32Imx477DGain[ViPipe] <= IMX477_DGAIN_11X)) {
                pstBlackLevel->au16BlackLevel[0] = 262;
                pstBlackLevel->au16BlackLevel[1] = 263;
                pstBlackLevel->au16BlackLevel[2] = 263;
                pstBlackLevel->au16BlackLevel[3] = 262;
            } else if ((g_u32Imx477DGain[ViPipe] > IMX477_DGAIN_11X) && (g_u32Imx477DGain[ViPipe] <= IMX477_DGAIN_12X)) {
                pstBlackLevel->au16BlackLevel[0] = 264;
                pstBlackLevel->au16BlackLevel[1] = 265;
                pstBlackLevel->au16BlackLevel[2] = 265;
                pstBlackLevel->au16BlackLevel[3] = 264;
            } else if ((g_u32Imx477DGain[ViPipe] > IMX477_DGAIN_12X) && (g_u32Imx477DGain[ViPipe] <= IMX477_DGAIN_13X)) {
                pstBlackLevel->au16BlackLevel[0] = 269;
                pstBlackLevel->au16BlackLevel[1] = 268;
                pstBlackLevel->au16BlackLevel[2] = 268;
                pstBlackLevel->au16BlackLevel[3] = 269;
            } else if ((g_u32Imx477DGain[ViPipe] > IMX477_DGAIN_13X) && (g_u32Imx477DGain[ViPipe] <= IMX477_DGAIN_14X)) {
                pstBlackLevel->au16BlackLevel[0] = 270;
                pstBlackLevel->au16BlackLevel[1] = 272;
                pstBlackLevel->au16BlackLevel[2] = 272;
                pstBlackLevel->au16BlackLevel[3] = 270;
            } else if ((g_u32Imx477DGain[ViPipe] > IMX477_DGAIN_14X) && (g_u32Imx477DGain[ViPipe] <= IMX477_DGAIN_15X)) {
                pstBlackLevel->au16BlackLevel[0] = 277;
                pstBlackLevel->au16BlackLevel[1] = 275;
                pstBlackLevel->au16BlackLevel[2] = 275;
                pstBlackLevel->au16BlackLevel[3] = 277;
            } else if ((g_u32Imx477DGain[ViPipe] > IMX477_DGAIN_15X) && (g_u32Imx477DGain[ViPipe] <= IMX477_DGAIN_16X)) {
                pstBlackLevel->au16BlackLevel[0] = 282;
                pstBlackLevel->au16BlackLevel[1] = 278;
                pstBlackLevel->au16BlackLevel[2] = 278;
                pstBlackLevel->au16BlackLevel[3] = 282;
            }
        } else {
            pstBlackLevel->au16BlackLevel[0] = 257;
            pstBlackLevel->au16BlackLevel[1] = 257;
            pstBlackLevel->au16BlackLevel[2] = 257;
            pstBlackLevel->au16BlackLevel[3] = 257;
        }
    } else {
        pstBlackLevel->au16BlackLevel[0] = 257;
        pstBlackLevel->au16BlackLevel[1] = 257;
        pstBlackLevel->au16BlackLevel[2] = 257;
        pstBlackLevel->au16BlackLevel[3] = 257;
    }

    return HI_SUCCESS;
}

static HI_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, HI_BOOL bEnable)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    /* Detect set 5fps */
    CHECK_RET(HI_MPI_ISP_GetSnsSlaveAttr(ViPipe, &gstImx477Sync[ViPipe]));

    gstImx477Sync[ViPipe].u32VsTime = (g_astImx477ModeTbl[pstSnsState->u8ImgMode].u32InckPerVs) * g_astImx477ModeTbl[pstSnsState->u8ImgMode].f32MaxFps / 5;
    pstSnsState->u32FLStd = (g_astImx477ModeTbl[pstSnsState->u8ImgMode].u32VertiLines) * g_astImx477ModeTbl[pstSnsState->u8ImgMode].f32MaxFps / 5;

    if (bEnable) {
        imx477_write_register(ViPipe, IMX477_ANA_GAIN_GLOBAL_L, 0x00);  //Again
        imx477_write_register(ViPipe, IMX477_ANA_GAIN_GLOBAL_H, 0x00);

        imx477_write_register(ViPipe, IMX477_DIG_GAIN_GR_L, 0xFF);	//Dgain
        imx477_write_register(ViPipe, IMX477_DIG_GAIN_GR_H, 0x00);
    } else {
        gstImx477Sync[ViPipe].u32VsTime = (g_astImx477ModeTbl[pstSnsState->u8ImgMode].u32InckPerVs);
        pstSnsState->bSyncInit = HI_FALSE;
    }

    CHECK_RET(ISP_SetSnsSlaveUnSyncAttr(ViPipe, &gstImx477Sync[ViPipe]));

    return;
}

static HI_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    HI_S32 i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
        pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunImx477BusInfo[ViPipe].s8I2cDev;
        pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 3;
        pstSnsState->astRegsInfo[0].u32RegNum = 11;

        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = imx477_i2c_addr;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = imx477_addr_byte;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = imx477_data_byte;
        }

        /* Slave Sensor VsTime cfg */
        pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = IMX477_LINE_LENGTH_PCK_L;
        pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = IMX477_LINE_LENGTH_PCK_H;
        pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;

        // Again related
        pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = IMX477_ANA_GAIN_GLOBAL_L;
        pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = IMX477_ANA_GAIN_GLOBAL_H;
        pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 1;

        /* Dgain cfg */
        pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = IMX477_DIG_GAIN_GR_L;
        pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX477_DIG_GAIN_GR_H;
        pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX477_DPGA_USE_GLOBAL_GAIN;
        pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 1;

        pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX477_COARSE_INTEG_TIME_L;

        pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = IMX477_COARSE_INTEG_TIME_H;

        pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = IMX477_PRSH_LENGTH_LINE_L;
        pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = IMX477_PRSH_LENGTH_LINE_H;
        pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 0;

        // Again related
        /* Dgain cfg */
        pstSnsState->astRegsInfo[0].stSlvSync.bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].stSlvSync.u8DelayFrmNum  = 2;
        pstSnsState->astRegsInfo[0].stSlvSync.u32SlaveBindDev = g_Imx477SlaveBindDev[ViPipe];

        pstSnsState->bSyncInit = HI_TRUE;
    } else {
        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data == pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_FALSE;
            } else {
                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
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

    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return HI_SUCCESS;
}

static HI_S32 cmos_set_image_mode(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{

    HI_U8 u8M = 0, u8SensorImageMode;
    HI_U32 u32W, u32H;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;

    u32H = pstSensorImageMode->u16Height;
    u32W = pstSensorImageMode->u16Width;
    u8M = pstSensorImageMode->u8SnsMode;

    if (IMX477_RES_IS_8M(u32W, u32H)) {
        if (u8M == 0) {
            u8SensorImageMode = IMX477_8M60FPS_LINER_MODE;
        } else {
            u8SensorImageMode = IMX477_8M30FPS_LINER_MODE;
        }
    } else if (IMX477_RES_IS_12M(u32W, u32H)) {
        u8SensorImageMode = IMX477_12M30FPS_LINER_MODE;
    } else if (IMX477_RES_IS_2M(u32W, u32H)) {
        u8SensorImageMode = IMX477_2M240FPS_LINER_MODE;
    } else if (IMX477_RES_IS_1M(u32W, u32H)) {
        u8SensorImageMode = IMX477_1M240FPS_LINER_MODE;
    } else {
        if (u8M == 0) {
            u8SensorImageMode = IMX477_9M50FPS_LINER_MODE;
        } else {
            u8SensorImageMode = IMX477_9M60FPS_LINER_MODE;
        }
    }

    g_u8SensorImageMode = u8SensorImageMode;

    if ((pstSnsState->bInit == HI_TRUE) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }

    pstSnsState->u8ImgMode = u8SensorImageMode;

    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;

    pstSnsState->u32FLStd = 3500 + IMX477_FULL_LINES_STD_FPGA_COMP;
    pstSnsState->u8ImgMode = IMX477_12M30FPS_LINER_MODE;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->u32FLStd;

    memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, HI_U8 u8Mode)
{
    return HI_SUCCESS;
}

static HI_S32 cmos_get_sensor_awb_gain(VI_PIPE ViPipe, HI_U32 *pu32SensorAwbGain)
{
    HI_S32 i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pu32SensorAwbGain);
    IMX477_SENSOR_GET_CTX(ViPipe, pstSnsState);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        pstSnsState->au32SensorWBGain[i] = pu32SensorAwbGain[i];
    }

    return HI_SUCCESS;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init                  = imx477_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit                  = imx477_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init           = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode               = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode                 = cmos_set_wdr_mode;
    pstSensorExpFunc->pfn_cmos_get_isp_default              = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level          = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect             = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info             = cmos_get_sns_regs_info;
    pstSensorExpFunc->pfn_cmos_get_awb_gains                = cmos_get_sensor_awb_gain;

    return HI_SUCCESS;
}

static HI_S32 imx477_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunImx477BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX477_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    IMX477_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX477_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    IMX477_SENSOR_RESET_CTX(ViPipe);
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/
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

    stSnsAttrInfo.eSensorId = IMX477_ID;

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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(ViPipe, IMX477_ID);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(ViPipe, pstAeLib, IMX477_ID);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, IMX477_ID);
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

ISP_SNS_OBJ_S stSnsImx477Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = imx477_standby,
    .pfnRestart             = imx477_restart,
    .pfnMirrorFlip          = imx477_mirror_flip,
    .pfnWriteReg            = imx477_write_register,
    .pfnReadReg             = imx477_read_register,
    .pfnSetBusInfo          = imx477_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
