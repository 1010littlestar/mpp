/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#if !defined(__IMX277_CMOS_H_)
#define __IMX277_CMOS_H_

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

#include "imx277_cmos_slvs_ex.h"
#include "imx277_slave_priv.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define IMX277_ID             277

/* InckPerHs = Inck/MaxFps/InckVertiLines */
const IMX277_VIDEO_MODE_TBL_S g_astImx277ModeTbl[IMX277_MODE_BUTT] = {
    {72000000, 268,  600600,  2200, 120, "4K2K_120FPS_RAW10_SLVS_8LANE"}, /* Actually 119.88fps */
    {72000000, 1092, 2402402, 2200, 30,  "4K2K_30FPS_RAW12_SLVS_6LANE"}, /* Actually 29.97fps */
    {72000000, 546,  1201201, 2200, 60,  "4K2K_60FPS_RAW12_SLVS_6LANE"},  /* Actually 59.94fps */
    {72000000, 780,  2402402, 3080, 30,  "4K3K_30FPS_RAW12_SLVS_6LANE"},  /* Actually 29.97fps */
    {72000000, 269,  300300,  1118, 240, "2K1K_240FPS_RAW12_SLVS_6LANE"},  /* Actually 239.76fps */
};

HI_S32 g_Imx277SlaveBindDev[ISP_MAX_PIPE_NUM] = {0, 0, 1, 1, 2, 2, 3, 3};
HI_U32 g_Imx277SalveSensorModeTime[ISP_MAX_PIPE_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};

ISP_SLAVE_SNS_SYNC_S        gstImx277Sync[ISP_MAX_PIPE_NUM];

ISP_SNS_STATE_S *g_pastImx277Slvs[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define IMX277SLVS_SENSOR_GET_CTX(dev, pstCtx)   (pstCtx = g_pastImx277Slvs[dev])
#define IMX277SLVS_SENSOR_SET_CTX(dev, pstCtx)   (g_pastImx277Slvs[dev] = pstCtx)
#define IMX277SLVS_SENSOR_RESET_CTX(dev)         (g_pastImx277Slvs[dev] = HI_NULL)


ISP_SNS_COMMBUS_U g_aunImx277SlaveBusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = {.s8SspDev = { .bit4SspDev = 0, .bit4SspCs = 0}},
    [1] = {.s8SspDev = { .bit4SspDev = 1, .bit4SspCs = 0}},
    [2] = {.s8SspDev = { .bit4SspDev = 2, .bit4SspCs = 0}},
    [3] = {.s8SspDev = { .bit4SspDev = 3, .bit4SspCs = 0}},
    [4] = {.s8SspDev = { .bit4SspDev = 4, .bit4SspCs = 0}},
    [5] = {.s8SspDev = { .bit4SspDev = 5, .bit4SspCs = 0}}
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static HI_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};
static HI_U32 gu32DgainVal = 0;
static HI_U32 gu32AGain[ISP_MAX_PIPE_NUM] = {1024};
static HI_U32 gu32DGain[ISP_MAX_PIPE_NUM] = {1024};

static HI_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};

/****************************************************************************
 * extern function reference                                                *
 ****************************************************************************/

extern void imx277_slvs_init(VI_PIPE ViPipe);
extern void imx277_slvs_exit(VI_PIPE ViPipe);
extern void imx277_slvs_standby(VI_PIPE ViPipe);
extern void imx277_slvs_restart(VI_PIPE ViPipe);
extern int  imx277_slvs_write_register(VI_PIPE ViPipe, int addr, int data);
extern int  imx277_slvs_read_register(VI_PIPE ViPipe, int addr);


/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define IMX277_FULL_LINES_MAX (0xFFFF)

#define IMX277_SHR_ADDR_H     (0x000C)  // [7:0]
#define IMX277_SHR_ADDR_L     (0x000B)  // [7:0]
#define IMX277_SHR_SE_ADDR_H  (0x002F)
#define IMX277_SHR_SE_ADDR_L  (0x002E)
#define IMX277_SVR_ADDR_H     (0x000E)
#define IMX277_SVR_ADDR_L     (0x000D)
#define IMX277_PGC_ADDR_H     (0x000A)  // [7:0]
#define IMX277_PGC_ADDR_L     (0x0009)  // [2:0]
#define IMX277_PGC_SE_ADDR_H  (0x000D)
#define IMX277_PGC_SE_ADDR_L  (0x000C)
#define IMX277_DGAIN_ADDR     (0x0011)  // [1:0]

#define IMX277_IS_8M120FPS_LINER_10BIT(w, h, m)    ((((w == 3840) && (h == 2160)) || ((w == 4200) && (h == 2180))) && (m == 0))
#define IMX277_IS_8M60FPS_LINER_12BIT(w, h, m)     ((((w == 3840) && (h == 2160)) || ((w == 3936) && (h == 2182))) && (m == 2))
#define IMX277_IS_8M30FPS_LINER_12BIT(w, h, m)     ((((w == 3840) && (h == 2160)) || ((w == 3936) && (h == 2182))) && (m == 1))
#define IMX277_IS_12M30FPS_LINER_12BIT(w, h, m)    ((((w == 4000) && (h == 3000))|| ((w == 4104) && (h == 3062))) && (m == 0))
#define IMX277_IS_2M240FPS_LINER_12BIT(w, h, m)    ((((w == 1920) && (h == 1080))|| ((w == 1968) && (h == 1102))) && (m == 0))
#define IMX277_AGAIN_8X       (8192)
#define IMX277_AGAIN_MAX      (22795-1)

#define IMX277_DGAIN_1X       (1024)
#define IMX277_DGAIN_2X       (2048)
#define IMX277_DGAIN_4X       (4096)
#define IMX277_DGAIN_8X       (8192)

static HI_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{

    HI_U32 u32Fll, U32MaxFps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u32Fll = g_astImx277ModeTbl[pstSnsState->u8ImgMode].u32VertiLines;
    U32MaxFps = g_astImx277ModeTbl[pstSnsState->u8ImgMode].u32MaxFps;
    pstSnsState->u32FLStd = u32Fll;

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32HmaxTimes = (1000000) / (u32Fll * U32MaxFps);
    pstAeSnsDft->u32InitExposure = 921600;
    pstAeSnsDft->u32LinesPer500ms = (pstSnsState->u32FLStd * U32MaxFps) >> 1;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0.256;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.38;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.38;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift;

    if (g_au32InitExposure[ViPipe] == 0) {
        pstAeSnsDft->u32InitExposure = 921600;
    } else {
        pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe];
    }

    if (g_au32LinesPer500ms[ViPipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = (pstSnsState->u32FLStd * U32MaxFps) >> 1;
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
    }

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

            pstAeSnsDft->u8AeCompensation = 0x32;
            pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
            pstAeSnsDft->u32MinIntTime = 8;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 2;

            pstAeSnsDft->u32MaxAgain = 22795;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 8192;
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
            break;

        case WDR_MODE_2To1_LINE:
            pstAeSnsDft->au8HistThresh[0] = 0xC;
            pstAeSnsDft->au8HistThresh[1] = 0x18;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            if (pstSnsState->u8Hdr == HI_TRUE) {
                pstAeSnsDft->u8AeCompensation = 12;
                pstAeSnsDft->u16ManRatioEnable = HI_TRUE;
            } else {
                pstAeSnsDft->u8AeCompensation = 28;
                pstAeSnsDft->u16ManRatioEnable = HI_TRUE;
            }
            pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;

            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
            pstAeSnsDft->u32MinIntTime = 2;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 2;

            pstAeSnsDft->u32MaxAgain = 22795;
            pstAeSnsDft->u32MinAgain = 2048;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 8192;
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->au32Ratio[0] = 0x400;
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
    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32MaxFps = g_astImx277ModeTbl[pstSnsState->u8ImgMode].u32MaxFps;
    u32Lines = g_astImx277ModeTbl[pstSnsState->u8ImgMode].u32VertiLines * u32MaxFps / DIV_0_TO_1_FLOAT(f32Fps);

    /* SHR 16bit, So limit full_lines as 0xFFFF */
    if (f32Fps > u32MaxFps || u32Lines > FULL_LINES_MAX) {
        ISP_ERR_TRACE("Not support Fps: %f\n", f32Fps);
        return;
    }
    gstImx277Sync[ViPipe].u32VsTime = (g_astImx277ModeTbl[pstSnsState->u8ImgMode].u32InckPerVs) * u32MaxFps / DIV_0_TO_1_FLOAT(f32Fps);
    pstSnsState->u32FLStd = u32Lines;
    pstSnsState->astRegsInfo[0].stSlvSync.u32SlaveVsTime = gstImx277Sync[ViPipe].u32VsTime;

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = (HI_U32)(g_astImx277ModeTbl[pstSnsState->u8ImgMode].u32VertiLines*u32MaxFps / 2);
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
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
    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->au32FL[0] = (u32FullLines > FULL_LINES_MAX) ? FULL_LINES_MAX : u32FullLines;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    gstImx277Sync[ViPipe].u32VsTime = g_astImx277ModeTbl[pstSnsState->u8ImgMode].u32InckPerHs * u32FullLines;

    if (pstSnsState->u8ImgMode == IMX277_8M120FPS_LINER_MODE) {
        if (gstImx277Sync[ViPipe].u32VsTime <= g_astImx277ModeTbl[pstSnsState->u8ImgMode].u32InckPerVs) {
            gstImx277Sync[ViPipe].u32VsTime = g_astImx277ModeTbl[pstSnsState->u8ImgMode].u32InckPerVs;
        }
    }

    pstSnsState->astRegsInfo[0].stSlvSync.u32SlaveVsTime = gstImx277Sync[ViPipe].u32VsTime;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 5;

    return;
}


/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE ViPipe, HI_U32 u32IntTime)
{

    HI_U32 u32Shr = 0;
    static HI_BOOL bFirst[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = HI_TRUE};
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (bFirst[ViPipe]) { /* SHR-SE */
            pstSnsState->au32WDRIntTime[0] = u32IntTime;
            u32Shr = pstSnsState->au32FL[0] - u32IntTime;
            u32Shr = ((u32Shr % 2) == 0) ? u32Shr - 1 : u32Shr;
            u32Shr = MAX(u32Shr, 0x8);
            pstSnsState->astRegsInfo[0].astSspData[5].u32Data = ((u32Shr & 0xFF00) >> 8);
            pstSnsState->astRegsInfo[0].astSspData[6].u32Data = (u32Shr & 0xFF);
            bFirst[ViPipe] = HI_FALSE;
        } else { /* SHR-LE */
            pstSnsState->au32WDRIntTime[1] = u32IntTime;
            u32Shr = pstSnsState->au32FL[0] - u32IntTime;
            u32Shr = ((u32Shr % 2) == 0) ? u32Shr - 1 : u32Shr;
            u32Shr = MAX(u32Shr, 0x8);
            pstSnsState->astRegsInfo[0].astSspData[0].u32Data = ((u32Shr & 0xFF00) >> 8);
            pstSnsState->astRegsInfo[0].astSspData[1].u32Data = (u32Shr & 0xFF);
            bFirst[ViPipe] = HI_TRUE;
        }
    } else {
        u32Shr = pstSnsState->au32FL[0] - u32IntTime - 4;

        if (u32Shr < 8) {
            u32Shr = 0x8;
        }

        pstSnsState->astRegsInfo[0].astSspData[0].u32Data = ((u32Shr & 0xFF00) >> 8);
        pstSnsState->astRegsInfo[0].astSspData[1].u32Data = (u32Shr & 0xFF);
        bFirst[ViPipe] = HI_TRUE;
    }

    return;
}


static HI_U32 again_table[490] = {
    1024, 1026, 1028, 1030, 1032, 1034, 1036, 1038, 1040, 1042, 1044, 1046, 1049, 1051, 1053, 1055, 1057, 1059, 1061, 1063,
    1066, 1068, 1070, 1072, 1074, 1077, 1079, 1081, 1083, 1085, 1088, 1090, 1092, 1095, 1097, 1099, 1101, 1104, 1106, 1108,
    1111, 1113, 1116, 1118, 1120, 1123, 1125, 1128, 1130, 1132, 1135, 1137, 1140, 1142, 1145, 1147, 1150, 1152, 1155, 1157,
    1160, 1163, 1165, 1168, 1170, 1173, 1176, 1178, 1181, 1183, 1186, 1189, 1192, 1194, 1197, 1200, 1202, 1205, 1208, 1211,
    1214, 1216, 1219, 1222, 1225, 1228, 1231, 1234, 1237, 1239, 1242, 1245, 1248, 1251, 1254, 1257, 1260, 1263, 1266, 1269,
    1273, 1276, 1279, 1282, 1285, 1288, 1291, 1295, 1298, 1301, 1304, 1307, 1311, 1314, 1317, 1321, 1324, 1327, 1331, 1334,
    1337, 1341, 1344, 1348, 1351, 1355, 1358, 1362, 1365, 1369, 1372, 1376, 1380, 1383, 1387, 1391, 1394, 1398, 1402, 1406,
    1409, 1413, 1417, 1421, 1425, 1429, 1432, 1436, 1440, 1444, 1448, 1452, 1456, 1460, 1464, 1469, 1473, 1477, 1481, 1485,
    1489, 1494, 1498, 1502, 1507, 1511, 1515, 1520, 1524, 1529, 1533, 1538, 1542, 1547, 1551, 1556, 1560, 1565, 1570, 1574,
    1579, 1584, 1589, 1594, 1598, 1603, 1608, 1613, 1618, 1623, 1628, 1633, 1638, 1644, 1649, 1654, 1659, 1664, 1670, 1675,
    1680, 1686, 1691, 1697, 1702, 1708, 1713, 1719, 1725, 1730, 1736, 1742, 1748, 1753, 1759, 1765, 1771, 1777, 1783, 1789,
    1796, 1802, 1808, 1814, 1820, 1827, 1833, 1840, 1846, 1853, 1859, 1866, 1872, 1879, 1886, 1893, 1900, 1907, 1913, 1920,
    1928, 1935, 1942, 1949, 1956, 1964, 1971, 1978, 1986, 1993, 2001, 2009, 2016, 2024, 2032, 2040, 2048, 2056, 2064, 2072,
    2081, 2089, 2097, 2106, 2114, 2123, 2131, 2140, 2149, 2158, 2166, 2175, 2185, 2194, 2203, 2212, 2222, 2231, 2241, 2250,
    2260, 2270, 2280, 2289, 2300, 2310, 2320, 2330, 2341, 2351, 2362, 2372, 2383, 2394, 2405, 2416, 2427, 2439, 2450, 2461,
    2473, 2485, 2497, 2509, 2521, 2533, 2545, 2558, 2570, 2583, 2595, 2608, 2621, 2635, 2648, 2661, 2675, 2689, 2703, 2717,
    2731, 2745, 2759, 2774, 2789, 2804, 2819, 2834, 2849, 2865, 2881, 2897, 2913, 2929, 2945, 2962, 2979, 2996, 3013, 3031,
    3048, 3066, 3084, 3102, 3121, 3139, 3158, 3178, 3197, 3216, 3236, 3256, 3277, 3297, 3318, 3339, 3361, 3383, 3404, 3427,
    3449, 3472, 3495, 3519, 3542, 3567, 3591, 3616, 3641, 3666, 3692, 3718, 3745, 3772, 3799, 3827, 3855, 3884, 3913, 3942,
    3972, 4002, 4033, 4064, 4096, 4128, 4161, 4194, 4228, 4263, 4297, 4333, 4369, 4406, 4443, 4481, 4520, 4559, 4599, 4640,
    4681, 4723, 4766, 4810, 4855, 4900, 4946, 4993, 5041, 5090, 5140, 5191, 5243, 5296, 5350, 5405, 5461, 5519, 5578, 5638,
    5699, 5761, 5825, 5891, 5958, 6026, 6096, 6168, 6242, 6317, 6394, 6473, 6554, 6637, 6722, 6809, 6899, 6991, 7085, 7182,
    7282, 7384, 7490, 7598, 7710, 7825, 7944, 8066, 8192, 8322, 8456, 8595, 8738, 8886, 9039, 9198, 9362, 9533, 9709, 9892,
    10082, 10280, 10486, 10700, 10923, 11155, 11398, 11651, 11916, 12193, 12483, 12788, 13107, 13443, 13797, 14170, 14564,
    14980, 15420, 15888, 16384, 16913, 17476, 18079, 18725, 19418, 20165, 20972, 21845, 22795
};

static HI_VOID cmos_again_calc_table(VI_PIPE ViPipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i, j;
    HI_U32 u32AgainLinTmp = 0;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);
    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        j = 3;
        while ((*pu32AgainLin >> j) <= again_table[489]) {
            gu32DgainVal = j;
            if (j == 0) {
                break;
            }
            j--;
        }

        u32AgainLinTmp = *pu32AgainLin >> gu32DgainVal;
    } else {
        gu32DgainVal = 0;
        u32AgainLinTmp = *pu32AgainLin;
    }

    if (u32AgainLinTmp >= again_table[489]) {
        u32AgainLinTmp = again_table[489];
        *pu32AgainDb = 489;
        goto calc_table_end;
    }

    for (i = 1; i < 490; i++) {
        if (u32AgainLinTmp < again_table[i]) {
            u32AgainLinTmp = again_table[i - 1];
            *pu32AgainDb = i - 1;
            goto calc_table_end;;
        }
    }

calc_table_end:

    // downsample require
    *pu32AgainLin = u32AgainLinTmp << gu32DgainVal;
    gu32AGain[ViPipe] = *pu32AgainLin;  // For record Again value to BLC used.
    *pu32AgainDb <<= 2;

    return;
}

static HI_U32 dgain_table[4] = {
    1024, 2048, 4096, 8192
};

static HI_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, HI_U32 *pu32DgainLin, HI_U32 *pu32DgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32DgainLin);
    CMOS_CHECK_POINTER_VOID(pu32DgainDb);

    if (*pu32DgainLin >= dgain_table[3]) {
        *pu32DgainLin = dgain_table[3];
        gu32DGain[ViPipe] = *pu32DgainLin;
        *pu32DgainDb = 3;
        return;
    }

    for (i = 1; i < 4; i++) {
        if (*pu32DgainLin < dgain_table[i]) {
            *pu32DgainLin = dgain_table[i - 1];
            gu32DGain[ViPipe] = *pu32DgainLin;
            *pu32DgainDb = i - 1;
            break;
        }
    }
    return;
}

static HI_VOID cmos_gains_update(VI_PIPE ViPipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        /* LE */
        pstSnsState->astRegsInfo[0].astSspData[2].u32Data = (u32Again & 0xFF);
        pstSnsState->astRegsInfo[0].astSspData[3].u32Data = ((u32Again >> 8) & 0xF);
        /* SE */
        pstSnsState->astRegsInfo[0].astSspData[7].u32Data = (u32Again & 0xFF);
        pstSnsState->astRegsInfo[0].astSspData[8].u32Data = ((u32Again >> 8) & 0xF);

        pstSnsState->astRegsInfo[0].astSspData[4].u32Data = (((u32Dgain << 4) & 0xF0) + (u32Dgain & 0x0F)); // LE & SE
    } else {
        pstSnsState->astRegsInfo[0].astSspData[2].u32Data = (u32Again & 0xFF);
        pstSnsState->astRegsInfo[0].astSspData[3].u32Data = ((u32Again >> 8) & 0xF);
        pstSnsState->astRegsInfo[0].astSspData[4].u32Data = (u32Dgain & 0xF);
    }

    return;
}

static HI_VOID cmos_get_inttime_max(VI_PIPE ViPipe, HI_U16 u16ManRatioEnable, HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{

    HI_U32 u32IntTimeMaxTmp = 0;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(au32Ratio);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
    CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        u32IntTimeMaxTmp = pstSnsState->au32FL[0] - 1;
        au32IntTimeMax[0] = (u32IntTimeMaxTmp << 6) / au32Ratio[0];
        au32IntTimeMax[1] = u32IntTimeMaxTmp;
        au32IntTimeMin[0] = 5;
        au32IntTimeMin[1] = (5 * au32Ratio[0]) >> 6;
    }

    return;
}

/* Only used in Line_WDR mode */
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


/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN          0
#define GOLDEN_BGAIN          0

static HI_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = 5000;

    pstAwbSnsDft->au16GainOffset[0] = 0x1C0;      // R_gain
    pstAwbSnsDft->au16GainOffset[1] = 0x100;    // Gr_gain
    pstAwbSnsDft->au16GainOffset[2] = 0x100;    // Gb_gain
    pstAwbSnsDft->au16GainOffset[3] = 0x1C0;   // B_gain

    pstAwbSnsDft->as32WbPara[0] = -41;      // P1
    pstAwbSnsDft->as32WbPara[1] = 296;    // P2
    pstAwbSnsDft->as32WbPara[2] = -1;         // Q1
    pstAwbSnsDft->as32WbPara[3] = 161822;    // A1
    pstAwbSnsDft->as32WbPara[4] = 128;      // B1
    pstAwbSnsDft->as32WbPara[5] = -110103;  // C1
    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;
    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;
        case WDR_MODE_2To1_LINE:
            if (pstSnsState->u8Hdr == HI_TRUE) {
                pstAwbSnsDft->au16GainOffset[0] = 0x1b7;
                pstAwbSnsDft->au16GainOffset[3] = 0x1d1;
                pstAwbSnsDft->as32WbPara[3] = 171858;
                pstAwbSnsDft->as32WbPara[5] = -119413;
                memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcmHDR, sizeof(AWB_CCM_S));
                memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableHDR, sizeof(AWB_AGC_TABLE_S));
            } else {
                memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcmFsWdr, sizeof(AWB_CCM_S));
                memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));
            }
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
    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

    pstDef->unKey.bit1Ca       = 1;
    pstDef->pstCa              = &g_stIspCA;
    pstDef->unKey.bit1Clut     = 1;
    pstDef->pstClut            = &g_stIspCLUT;
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
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspLdci;
            memcpy(&pstDef->stNoiseCalibration,    &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
        case WDR_MODE_2To1_LINE:
            pstDef->unKey.bit1Demosaic       = 1;
            pstDef->pstDemosaic              = &g_stIspDemosaicWdr;
            pstDef->unKey.bit1Sharpen        = 1;
            pstDef->pstSharpen               = &g_stIspYuvSharpen;
            pstDef->unKey.bit1Drc            = 1;
            pstDef->pstDrc                   = &g_stIspDRCWDR;
            pstDef->unKey.bit1Gamma          = 1;
            pstDef->pstGamma                 = &g_stIspGammaFSWDR;
            pstDef->unKey.bit1PreGamma       = 0;
            pstDef->pstPreGamma              = &g_stPreGamma;
            pstDef->unKey.bit1BayerNr        = 1;
            pstDef->pstBayerNr               = &g_stIspBayerNrWdr2To1;
            pstDef->unKey.bit1Ge             = 1;
            pstDef->pstGe                    = &g_stIspWdrGe;
            pstDef->unKey.bit1AntiFalseColor = 1;
            pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspWdrLdci;
            pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x400;
            memcpy(&pstDef->stNoiseCalibration,    &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
    }

    pstDef->stSensorMaxResolution.u32MaxWidth  = 3840;
    pstDef->stSensorMaxResolution.u32MaxHeight = 2160;

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstDef->stSensorMaxResolution.u32MaxWidth  = 3840;
        pstDef->stSensorMaxResolution.u32MaxHeight = 2160;
    }
    pstDef->stSensorMode.u32SensorID = IMX277_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    switch (pstSnsState->u8ImgMode) {
        default:
        case IMX277_8M30FPS_LINER_MODE:
        case IMX277_8M60FPS_LINER_MODE:
        case IMX277_12M30FPS_LINER_MODE:
        case IMX277_2M240FPS_LINER_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12;
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;
            break;

        case IMX277_8M120FPS_LINER_MODE:
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
    pstBlackLevel->bUpdate = HI_TRUE;

    if (gu32AGain[ViPipe]  < IMX277_AGAIN_MAX) {
        pstBlackLevel->au16BlackLevel[0] = 200;
        pstBlackLevel->au16BlackLevel[1] = 200;
        pstBlackLevel->au16BlackLevel[2] = 200;
        pstBlackLevel->au16BlackLevel[3] = 200;
    } else {
        if (gu32DGain[ViPipe]  == IMX277_DGAIN_1X) {
            pstBlackLevel->au16BlackLevel[0] = 200;
            pstBlackLevel->au16BlackLevel[1] = 200;
            pstBlackLevel->au16BlackLevel[2] = 200;
            pstBlackLevel->au16BlackLevel[3] = 200;
        } else if (gu32DGain[ViPipe] == IMX277_DGAIN_2X) {
            pstBlackLevel->au16BlackLevel[0] = 204;
            pstBlackLevel->au16BlackLevel[1] = 204;
            pstBlackLevel->au16BlackLevel[2] = 204;
            pstBlackLevel->au16BlackLevel[3] = 204;
        } else if (gu32DGain[ViPipe] == IMX277_DGAIN_4X) {
            pstBlackLevel->au16BlackLevel[0] = 208;
            pstBlackLevel->au16BlackLevel[1] = 208;
            pstBlackLevel->au16BlackLevel[2] = 208;
            pstBlackLevel->au16BlackLevel[3] = 208;
        } else if (gu32DGain[ViPipe] == IMX277_DGAIN_8X) {
            pstBlackLevel->au16BlackLevel[0] = 218;
            pstBlackLevel->au16BlackLevel[1] = 218;
            pstBlackLevel->au16BlackLevel[2] = 218;
            pstBlackLevel->au16BlackLevel[3] = 218;
        }
    }

    return HI_SUCCESS;
}


static HI_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, HI_BOOL bEnable)
{

    HI_U32 u32MaxFps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    }

    /* Detect set 5fps */
    u32MaxFps = g_astImx277ModeTbl->u32MaxFps;
    CHECK_RET(HI_MPI_ISP_GetSnsSlaveAttr(ViPipe, &gstImx277Sync[ViPipe]));

    gstImx277Sync[ViPipe].u32VsTime = ((g_astImx277ModeTbl->u32InckPerVs) * u32MaxFps) / 5;

    if (bEnable) { /* setup for ISP pixel calibration mode */
        imx277_slvs_write_register(ViPipe, IMX277_PGC_ADDR_L, 0x01);                                /* min AG */
        imx277_slvs_write_register(ViPipe, IMX277_PGC_ADDR_H, 0x00);                                /* min AG */
        imx277_slvs_write_register(ViPipe, IMX277_DGAIN_ADDR, 0x01);                                /* min DG */
    } else { /* setup for ISP 'normal mode' */
        gstImx277Sync[ViPipe].u32VsTime = (g_astImx277ModeTbl->u32InckPerVs);

        pstSnsState->bSyncInit = HI_FALSE;
    }

    CHECK_RET(HI_MPI_ISP_SetSnsSlaveAttr(ViPipe, &gstImx277Sync[ViPipe]));
    return;
}


static HI_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;

    switch ((u8Mode & 0x3f)) {
        case WDR_MODE_NONE:
            pstSnsState->u8Hdr = HI_FAILURE;
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
            break;

        case WDR_MODE_2To1_LINE:
            if (1 == ((u8Mode & 0x40) >> 0x6)) {
                pstSnsState->u8Hdr = HI_TRUE;
            } else {
                pstSnsState->u8Hdr = HI_FAILURE;
            }

            pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
            memset(pstSnsState->au32WDRIntTime, 0, sizeof(pstSnsState->au32WDRIntTime));
            printf("2to1 line WDR mode\n");
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
    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_SSP_TYPE;
        pstSnsState->astRegsInfo[0].unComBus.s8SspDev.bit4SspDev = g_aunImx277SlaveBusInfo[ViPipe].s8SspDev.bit4SspDev;
        pstSnsState->astRegsInfo[0].unComBus.s8SspDev.bit4SspCs = g_aunImx277SlaveBusInfo[ViPipe].s8SspDev.bit4SspCs;
        pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        pstSnsState->astRegsInfo[0].u32RegNum = 5;

        if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
            pstSnsState->astRegsInfo[0].u32RegNum += 4;
            pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        }

        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            pstSnsState->astRegsInfo[0].astSspData[i].bUpdate = HI_TRUE;
            pstSnsState->astRegsInfo[0].astSspData[i].u32DevAddr = 0x81;
            pstSnsState->astRegsInfo[0].astSspData[i].u32DevAddrByteNum = 1;
            pstSnsState->astRegsInfo[0].astSspData[i].u32RegAddrByteNum = 2;
            pstSnsState->astRegsInfo[0].astSspData[i].u32DataByteNum = 1;
        }

        // Linear Mode Regs
        pstSnsState->astRegsInfo[0].astSspData[0].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[0].u32RegAddr = IMX277_SHR_ADDR_H;
        pstSnsState->astRegsInfo[0].astSspData[1].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astSspData[1].u32RegAddr = IMX277_SHR_ADDR_L;

        /* AG */
        pstSnsState->astRegsInfo[0].astSspData[2].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[2].u32RegAddr = IMX277_PGC_ADDR_L;
        pstSnsState->astRegsInfo[0].astSspData[3].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[3].u32RegAddr = IMX277_PGC_ADDR_H;

        /* DG */
        pstSnsState->astRegsInfo[0].astSspData[4].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astSspData[4].u32RegAddr = IMX277_DGAIN_ADDR;

        // DOL 2t1 Mode Regs
        if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
            pstSnsState->astRegsInfo[0].astSspData[5].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astSspData[5].u32RegAddr = IMX277_SHR_SE_ADDR_H;
            pstSnsState->astRegsInfo[0].astSspData[6].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astSspData[6].u32RegAddr = IMX277_SHR_SE_ADDR_L;

            pstSnsState->astRegsInfo[0].astSspData[7].u8DelayFrmNum = 1;
            pstSnsState->astRegsInfo[0].astSspData[7].u32RegAddr = IMX277_PGC_SE_ADDR_L;
            pstSnsState->astRegsInfo[0].astSspData[8].u8DelayFrmNum = 1;
            pstSnsState->astRegsInfo[0].astSspData[8].u32RegAddr = IMX277_PGC_SE_ADDR_H;
        }

        /* Slave Sensor VsTime cfg */
        pstSnsState->astRegsInfo[0].stSlvSync.bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].stSlvSync.u8DelayFrmNum = 1;

        pstSnsState->astRegsInfo[0].astSspData[2].u8IntPos = 0;
        pstSnsState->astRegsInfo[0].astSspData[3].u8IntPos = 0;
        pstSnsState->astRegsInfo[0].astSspData[4].u8IntPos = 0;
        pstSnsState->astRegsInfo[0].stSlvSync.u32SlaveBindDev = g_Imx277SlaveBindDev[ViPipe];

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

    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    return HI_SUCCESS;
}

static HI_S32 cmos_set_image_mode(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode = 0;

    HI_U32 u32W = 0;
    HI_U32 u32H = 0;
    HI_U32 u32Mode = 0;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;

    u32W = pstSensorImageMode->u16Width;
    u32H = pstSensorImageMode->u16Height;
    u32Mode = pstSensorImageMode->u8SnsMode;

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        ISP_ERR_TRACE("IMX277_8M60FPS_WDR_MODE not support yet...\n");

        return HI_FAILURE;
    } else {
        if (IMX277_IS_8M120FPS_LINER_10BIT(u32W, u32H, u32Mode)) {
            u8SensorImageMode     = IMX277_8M120FPS_LINER_MODE;
            pstSnsState->u32FLStd = 2200;
            printf("SNS MODE: IMX277_8M120FPS_LINER_MODE\n");
        } else if (IMX277_IS_8M30FPS_LINER_12BIT(u32W, u32H, u32Mode)) {
            u8SensorImageMode     = IMX277_8M30FPS_LINER_MODE;
            pstSnsState->u32FLStd = 2200;
            printf("SNS MODE: IMX277_8M30FPS_LINER_MODE\n");
        } else if (IMX277_IS_8M60FPS_LINER_12BIT(u32W, u32H, u32Mode)) {
            u8SensorImageMode     = IMX277_8M60FPS_LINER_MODE;
            pstSnsState->u32FLStd = 2200;
            printf("SNS MODE: IMX277_8M60FPS_LINER_MODE\n");
        } else if (IMX277_IS_12M30FPS_LINER_12BIT(u32W, u32H, u32Mode)) {
            u8SensorImageMode     = IMX277_12M30FPS_LINER_MODE;
            pstSnsState->u32FLStd = 3080;
            printf("SNS MODE: IMX277_12M30FPS_LINER_MODE\n");
        } else if (IMX277_IS_2M240FPS_LINER_12BIT(u32W, u32H, u32Mode)) {
            u8SensorImageMode     = IMX277_2M240FPS_LINER_MODE;
            pstSnsState->u32FLStd = 1118;
            printf("SNS MODE: IMX277_2M240FPS_LINER_MODE\n");
        } else {
            ISP_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                      pstSensorImageMode->u16Width,
                      pstSensorImageMode->u16Height,
                      pstSensorImageMode->f32Fps,
                      pstSnsState->enWDRMode);

            return HI_FAILURE;
        }
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

    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = IMX277_8M120FPS_LINER_MODE;
    pstSnsState->u32FLStd = 2200;
    memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = imx277_slvs_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = imx277_slvs_exit;
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

static HI_S32 imx277_slvs_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunImx277SlaveBusInfo[ViPipe].s8SspDev.bit4SspDev = unSNSBusInfo.s8SspDev.bit4SspDev;
    g_aunImx277SlaveBusInfo[ViPipe].s8SspDev.bit4SspCs  = unSNSBusInfo.s8SspDev.bit4SspCs;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    IMX277SLVS_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX277SLVS_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    IMX277SLVS_SENSOR_RESET_CTX(ViPipe);
}

static HI_S32 sensor_slvs_register_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
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

    stSnsAttrInfo.eSensorId = IMX277_ID;

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

static HI_S32 sensor_slvs_unregister_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(ViPipe, IMX277_ID);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(ViPipe, pstAeLib, IMX277_ID);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, IMX277_ID);

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

ISP_SNS_OBJ_S stSnsImx277SlvsObj = {
    .pfnRegisterCallback    = sensor_slvs_register_callback,
    .pfnUnRegisterCallback  = sensor_slvs_unregister_callback,
    .pfnStandby             = imx277_slvs_standby,
    .pfnRestart             = imx277_slvs_restart,
    .pfnWriteReg            = imx277_slvs_write_register,
    .pfnReadReg             = imx277_slvs_read_register,
    .pfnSetBusInfo          = imx277_slvs_set_bus_info,
    .pfnSetInit             = sensor_set_init
};
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
