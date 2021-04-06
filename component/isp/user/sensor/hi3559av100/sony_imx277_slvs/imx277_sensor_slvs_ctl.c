/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"
#include "mpi_sys.h"

#include "hi_spi.h"
#include "imx277_slave_priv.h"
#include "mpi_isp.h"

extern const IMX277_VIDEO_MODE_TBL_S g_astImx277ModeTbl[];
extern ISP_SLAVE_SNS_SYNC_S gstImx277Sync[];

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};
extern HI_S32 g_Imx277SlaveBindDev[];
extern HI_U32 g_Imx277SalveSensorModeTime[];

extern ISP_SNS_STATE_S      *g_pastImx277Slvs[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U     g_aunImx277SlaveBusInfo[];

void imx277_sensor_8m120fps_linear_init(VI_PIPE ViPipe);
void imx277_sensor_8m30fps_linear_init(VI_PIPE ViPipe);
void imx277_sensor_8m60fps_linear_init(VI_PIPE ViPipe);
void imx277_sensor_12m30fps_linear_init(VI_PIPE ViPipe);
void imx277_sensor_2m240fps_linear_init(VI_PIPE ViPipe);
void imx277_sensor_wdr_init(VI_PIPE ViPipe);
void imx277_sensor_common_init(VI_PIPE ViPipe);

int imx277_slvs_spi_init(VI_PIPE ViPipe)
{
    char acDevFile[16];
    HI_U8 u8DevNum, u8Cs;

    if (g_fd[ViPipe] >= 0) {
        return HI_SUCCESS;
    }
    unsigned int value;
    int ret = 0;

    u8DevNum = g_aunImx277SlaveBusInfo[ViPipe].s8SspDev.bit4SspDev;
    u8Cs = g_aunImx277SlaveBusInfo[ViPipe].s8SspDev.bit4SspCs;
    snprintf(acDevFile, sizeof(acDevFile), "/dev/spidev%d.%d", u8DevNum, u8Cs);

    g_fd[ViPipe] = open(acDevFile, O_RDONLY, S_IRUSR);
    if (g_fd[ViPipe] < 0) {
        ISP_ERR_TRACE("Open %s error!\n", acDevFile);
        return HI_FAILURE;
    }

    value = SPI_MODE_3 | SPI_LSB_FIRST; // | SPI_LOOP;
    ret = ioctl(g_fd[ViPipe], SPI_IOC_WR_MODE, &value);
    if (ret < 0) {
        ISP_ERR_TRACE("ioctl SPI_IOC_WR_MODE err, value = %u ret = %d\n", value, ret);
        return ret;
    }

    value = 8;
    ret = ioctl(g_fd[ViPipe], SPI_IOC_WR_BITS_PER_WORD, &value);
    if (ret < 0) {
        ISP_ERR_TRACE("ioctl SPI_IOC_WR_BITS_PER_WORD err, value = %u ret = %d\n", value, ret);
        return ret;
    }

    value = 2000000;
    ret = ioctl(g_fd[ViPipe], SPI_IOC_WR_MAX_SPEED_HZ, &value);
    if (ret < 0) {
        ISP_ERR_TRACE("ioctl SPI_IOC_WR_MAX_SPEED_HZ err, value = %u ret = %d\n", value, ret);
        return ret;
    }

    return HI_SUCCESS;
}

int imx277_slvs_spi_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int imx277_slvs_write_register(VI_PIPE ViPipe, unsigned int addr, unsigned char data)
{
    int ret;
    struct spi_ioc_transfer mesg[1];
    unsigned char  tx_buf[8] = {0};
    unsigned char  rx_buf[8] = {0};

    if (g_fd[ViPipe] < 0) {
        return HI_SUCCESS;
    }

    tx_buf[0] = 0x81;
    tx_buf[1] = addr >> 8;
    tx_buf[2] = addr & 0xff;
    tx_buf[3] = data;

    memset(mesg, 0, sizeof(mesg));
    mesg[0].tx_buf = (HI_U64)tx_buf;
    mesg[0].len    = 4;
    mesg[0].rx_buf = (HI_U64)rx_buf;
    mesg[0].cs_change = 1;

    ret = ioctl(g_fd[ViPipe], SPI_IOC_MESSAGE(1), mesg);
    if (ret < 0) {
        ISP_ERR_TRACE("SPI_IOC_MESSAGE error \n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

int imx277_slvs_read_register(VI_PIPE ViPipe, unsigned int addr)
{
    int ret = 0;
    struct spi_ioc_transfer mesg[1];
    unsigned char  tx_buf[8] = {0};
    unsigned char  rx_buf[8] = {0};


    tx_buf[0] = 0x80;
    tx_buf[1] = addr >> 8;
    tx_buf[2] = addr & 0xff;
    tx_buf[3] = 0;

    memset(mesg, 0, sizeof(mesg));
    mesg[0].tx_buf = (HI_U64)tx_buf;
    mesg[0].len    = 4;
    mesg[0].rx_buf = (HI_U64)rx_buf;
    mesg[0].cs_change = 1;

    ret = ioctl(g_fd[ViPipe], SPI_IOC_MESSAGE(1), mesg);
    if (ret  < 0) {
        ISP_ERR_TRACE("SPI_IOC_MESSAGE error \n");
        return HI_FAILURE;
    }

    return rx_buf[3];
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);
}

void imx277_slvs_standby(VI_PIPE ViPipe)
{
    return;
}

void imx277_slvs_restart(VI_PIPE ViPipe)
{
    return;
}

void imx277_slvs_init(VI_PIPE ViPipe)
{
    HI_U8         u8ImgMode;
    HI_S32        SlaveDev;
    HI_U32  u32Data;

    u8ImgMode   = g_pastImx277Slvs[ViPipe]->u8ImgMode;
    SlaveDev    = g_Imx277SlaveBindDev[ViPipe];
    u32Data     = g_Imx277SalveSensorModeTime[ViPipe];
    HI_U32 i;

    /* hold sync signal as fixed */
    CHECK_RET(HI_MPI_ISP_GetSnsSlaveAttr(SlaveDev, &gstImx277Sync[ViPipe]));
    gstImx277Sync[ViPipe].unCfg.stBits.bitHEnable = 0;
    gstImx277Sync[ViPipe].unCfg.stBits.bitVEnable = 0;
    gstImx277Sync[ViPipe].u32SlaveModeTime = u32Data;

    CHECK_RET(HI_MPI_ISP_SetSnsSlaveAttr(SlaveDev, &gstImx277Sync[ViPipe]));

    /* 2. imx277 spi init */
    imx277_slvs_spi_init(ViPipe);

    imx277_sensor_common_init(ViPipe);

    /* When sensor first init, config all registers */
    // release hv sync
    gstImx277Sync[ViPipe].u32HsTime = g_astImx277ModeTbl[u8ImgMode].u32InckPerHs;
    if (g_pastImx277Slvs[ViPipe]->astRegsInfo[0].stSlvSync.u32SlaveVsTime == 0) {
        gstImx277Sync[ViPipe].u32VsTime = g_astImx277ModeTbl[u8ImgMode].u32InckPerVs;
    } else {
        gstImx277Sync[ViPipe].u32VsTime = g_pastImx277Slvs[ViPipe]->astRegsInfo[0].stSlvSync.u32SlaveVsTime;
    }
    gstImx277Sync[ViPipe].unCfg.u32Bytes = 0xc0030000;
    gstImx277Sync[ViPipe].u32HsCyc = 0x3;
    gstImx277Sync[ViPipe].u32VsCyc = 0x3;
    CHECK_RET(HI_MPI_ISP_SetSnsSlaveAttr(SlaveDev, &gstImx277Sync[ViPipe]));

    switch (u8ImgMode) {
        case IMX277_8M120FPS_LINER_MODE:
            imx277_sensor_8m120fps_linear_init(ViPipe);
            break;

        case IMX277_8M30FPS_LINER_MODE:
            imx277_sensor_8m30fps_linear_init(ViPipe);
            break;

        case IMX277_8M60FPS_LINER_MODE:
            imx277_sensor_8m60fps_linear_init(ViPipe);
            break;

        case IMX277_12M30FPS_LINER_MODE:
            imx277_sensor_12m30fps_linear_init(ViPipe);
            break;

        case IMX277_2M240FPS_LINER_MODE:
            imx277_sensor_2m240fps_linear_init(ViPipe);
            break;

        case IMX277_8M60FPS_WDR_MODE:
            imx277_sensor_wdr_init(ViPipe);
            break;

        default:
            break;
    }


    for (i = 0; i < g_pastImx277Slvs[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
        imx277_slvs_write_register(ViPipe, g_pastImx277Slvs[ViPipe]->astRegsInfo[0].astSspData[i].u32RegAddr, g_pastImx277Slvs[ViPipe]->astRegsInfo[0].astSspData[i].u32Data);
    }

    printf("IMX277 %s init succuss!\n", g_astImx277ModeTbl[u8ImgMode].pszModeName);
    g_pastImx277Slvs[ViPipe]->bInit = HI_TRUE;

    return;
}

void imx277_slvs_exit(VI_PIPE ViPipe)
{
    imx277_slvs_spi_exit(ViPipe);

    return;
}


void imx277_sensor_common_init(VI_PIPE ViPipe)
{
    imx277_slvs_write_register(ViPipe, 0x0000, 0x1a);
    imx277_slvs_write_register(ViPipe, 0x003D, 0x00);
    imx277_slvs_write_register(ViPipe, 0x004E, 0x02);
    imx277_slvs_write_register(ViPipe, 0x0057, 0x4A);
    imx277_slvs_write_register(ViPipe, 0x0058, 0xF6);
    imx277_slvs_write_register(ViPipe, 0x0059, 0x00);
    imx277_slvs_write_register(ViPipe, 0x006B, 0x04);
    imx277_slvs_write_register(ViPipe, 0x0202, 0x63);
    imx277_slvs_write_register(ViPipe, 0x0203, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0236, 0x64);
    imx277_slvs_write_register(ViPipe, 0x0237, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0304, 0x0B);
    imx277_slvs_write_register(ViPipe, 0x0305, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0306, 0x0B);
    imx277_slvs_write_register(ViPipe, 0x0307, 0x00);
    imx277_slvs_write_register(ViPipe, 0x037F, 0x64);
    imx277_slvs_write_register(ViPipe, 0x0380, 0x00);
    imx277_slvs_write_register(ViPipe, 0x038D, 0x64);
    imx277_slvs_write_register(ViPipe, 0x038E, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0399, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0510, 0x72);
    imx277_slvs_write_register(ViPipe, 0x0511, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0528, 0x0F);
    imx277_slvs_write_register(ViPipe, 0x0529, 0x0F);
    imx277_slvs_write_register(ViPipe, 0x052A, 0x0F);
    imx277_slvs_write_register(ViPipe, 0x052B, 0x0F);
    imx277_slvs_write_register(ViPipe, 0x0538, 0x0F);
    imx277_slvs_write_register(ViPipe, 0x0539, 0x13);
    imx277_slvs_write_register(ViPipe, 0x053C, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0553, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0554, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0555, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0556, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0557, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0558, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0559, 0x00);
    imx277_slvs_write_register(ViPipe, 0x055A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x057D, 0x07);
    imx277_slvs_write_register(ViPipe, 0x057F, 0x07);
    imx277_slvs_write_register(ViPipe, 0x0580, 0x04);
    imx277_slvs_write_register(ViPipe, 0x0583, 0x60);
    imx277_slvs_write_register(ViPipe, 0x0587, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0590, 0x0B);
    imx277_slvs_write_register(ViPipe, 0x0591, 0x00);
    imx277_slvs_write_register(ViPipe, 0x05BA, 0x0F);
    imx277_slvs_write_register(ViPipe, 0x0606, 0xA3);
    imx277_slvs_write_register(ViPipe, 0x060E, 0x4E);
    imx277_slvs_write_register(ViPipe, 0x0610, 0x50);
    imx277_slvs_write_register(ViPipe, 0x0614, 0x20);
    imx277_slvs_write_register(ViPipe, 0x0618, 0x4B);
    imx277_slvs_write_register(ViPipe, 0x061A, 0x55);
    imx277_slvs_write_register(ViPipe, 0x061E, 0x90);
    imx277_slvs_write_register(ViPipe, 0x0620, 0x4D);
    imx277_slvs_write_register(ViPipe, 0x0636, 0x89);
    imx277_slvs_write_register(ViPipe, 0x0638, 0x8F);
    imx277_slvs_write_register(ViPipe, 0x063A, 0x22);
    imx277_slvs_write_register(ViPipe, 0x063C, 0x24);
    imx277_slvs_write_register(ViPipe, 0x063E, 0x4D);
    imx277_slvs_write_register(ViPipe, 0x0640, 0x4F);
    imx277_slvs_write_register(ViPipe, 0x0642, 0x48);
    imx277_slvs_write_register(ViPipe, 0x0646, 0x87);
    imx277_slvs_write_register(ViPipe, 0x0648, 0x92);
    imx277_slvs_write_register(ViPipe, 0x064C, 0x91);
    imx277_slvs_write_register(ViPipe, 0x064E, 0x22);
    imx277_slvs_write_register(ViPipe, 0x0650, 0x24);
    imx277_slvs_write_register(ViPipe, 0x0652, 0x4D);
    imx277_slvs_write_register(ViPipe, 0x0654, 0x4F);
    imx277_slvs_write_register(ViPipe, 0x0656, 0x25);
    imx277_slvs_write_register(ViPipe, 0x0658, 0x50);
    imx277_slvs_write_register(ViPipe, 0x066A, 0x0C);
    imx277_slvs_write_register(ViPipe, 0x066B, 0x0B);
    imx277_slvs_write_register(ViPipe, 0x066C, 0x07);
    imx277_slvs_write_register(ViPipe, 0x066D, 0x00);
    imx277_slvs_write_register(ViPipe, 0x066E, 0x00);
    imx277_slvs_write_register(ViPipe, 0x066F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0670, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0671, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0672, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0673, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0674, 0xDF);
    imx277_slvs_write_register(ViPipe, 0x0675, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0676, 0xA7);
    imx277_slvs_write_register(ViPipe, 0x0677, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0687, 0x00);
    imx277_slvs_write_register(ViPipe, 0x075C, 0x02);
    imx277_slvs_write_register(ViPipe, 0x080A, 0x0A);
    imx277_slvs_write_register(ViPipe, 0x082B, 0x16);

    delay_ms(20);

    imx277_slvs_write_register(ViPipe, 0x0123, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0127, 0x06);
    imx277_slvs_write_register(ViPipe, 0x0129, 0x0C);
    imx277_slvs_write_register(ViPipe, 0x010B, 0x00);

    delay_ms(1);
    return;
}

void imx277_sensor_8m120fps_linear_init(VI_PIPE ViPipe)
{
    imx277_slvs_write_register(ViPipe, 0x0000, 0x08);
    delay_ms(2);
    imx277_slvs_write_register(ViPipe, 0x0003, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0004, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0005, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0006, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0007, 0xA0);
    imx277_slvs_write_register(ViPipe, 0x0008, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0009, 0x01);
    imx277_slvs_write_register(ViPipe, 0x000A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0011, 0x03);
    imx277_slvs_write_register(ViPipe, 0x0112, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0113, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000B, 0x20);
    imx277_slvs_write_register(ViPipe, 0x000C, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000D, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000E, 0x00);
    imx277_slvs_write_register(ViPipe, 0x001A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0039, 0x00);
    imx277_slvs_write_register(ViPipe, 0x003A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x003E, 0x00);
    imx277_slvs_write_register(ViPipe, 0x003F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0040, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0036, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0068, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D5, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D6, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D7, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D8, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D9, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00DA, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00EE, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0001, 0x11);
    imx277_slvs_write_register(ViPipe, 0x032B, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x032C, 0x03);
    imx277_slvs_write_register(ViPipe, 0x034B, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x034C, 0x03);
    imx277_slvs_write_register(ViPipe, 0x05B6, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x05B7, 0x03);
    imx277_slvs_write_register(ViPipe, 0x05B8, 0x8e);
    imx277_slvs_write_register(ViPipe, 0x05B9, 0x0f);
    imx277_slvs_write_register(ViPipe, 0x0019, 0x00);
    imx277_slvs_write_register(ViPipe, 0x002E, 0x04);
    imx277_slvs_write_register(ViPipe, 0x002F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0030, 0x43);
    imx277_slvs_write_register(ViPipe, 0x0031, 0x05);
    imx277_slvs_write_register(ViPipe, 0x0032, 0x3E);
    imx277_slvs_write_register(ViPipe, 0x0033, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0041, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0042, 0x08);
    imx277_slvs_write_register(ViPipe, 0x0043, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0717, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0749, 0x90);
    imx277_slvs_write_register(ViPipe, 0x074C, 0x00);

    return;
}

void imx277_sensor_8m30fps_linear_init(VI_PIPE ViPipe)
{
    imx277_slvs_write_register(ViPipe, 0x0000, 0x08);
    delay_ms(2);
    imx277_slvs_write_register(ViPipe, 0x0003, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0004, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0005, 0x07);
    imx277_slvs_write_register(ViPipe, 0x0006, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0007, 0xA0);
    imx277_slvs_write_register(ViPipe, 0x0008, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0009, 0x01);
    imx277_slvs_write_register(ViPipe, 0x000A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0011, 0x03);
    imx277_slvs_write_register(ViPipe, 0x0112, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0113, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000B, 0x20);
    imx277_slvs_write_register(ViPipe, 0x000C, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000D, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000E, 0x00);
    imx277_slvs_write_register(ViPipe, 0x001A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0039, 0xE4);
    imx277_slvs_write_register(ViPipe, 0x003A, 0x0F);
    imx277_slvs_write_register(ViPipe, 0x003E, 0xB4);
    imx277_slvs_write_register(ViPipe, 0x003F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0040, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0036, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0068, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D5, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D6, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D7, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D8, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D9, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00DA, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00EE, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0001, 0x11);
    imx277_slvs_write_register(ViPipe, 0x032B, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x032C, 0x03);
    imx277_slvs_write_register(ViPipe, 0x034B, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x034C, 0x03);
    imx277_slvs_write_register(ViPipe, 0x05B6, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x05B7, 0x03);
    imx277_slvs_write_register(ViPipe, 0x05B8, 0x8e);
    imx277_slvs_write_register(ViPipe, 0x05B9, 0x0f);
    imx277_slvs_write_register(ViPipe, 0x0019, 0x00);
    imx277_slvs_write_register(ViPipe, 0x002E, 0x04);
    imx277_slvs_write_register(ViPipe, 0x002F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0030, 0x43);
    imx277_slvs_write_register(ViPipe, 0x0031, 0x05);
    imx277_slvs_write_register(ViPipe, 0x0032, 0x3E);
    imx277_slvs_write_register(ViPipe, 0x0033, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0041, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0042, 0x08);
    imx277_slvs_write_register(ViPipe, 0x0043, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0717, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0749, 0x90);
    imx277_slvs_write_register(ViPipe, 0x074C, 0x00);

    return;
}

void imx277_sensor_8m60fps_linear_init(VI_PIPE ViPipe)
{
    imx277_slvs_write_register(ViPipe, 0x0000, 0x08);
    delay_ms(2);
    imx277_slvs_write_register(ViPipe, 0x0003, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0004, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0005, 0x07);
    imx277_slvs_write_register(ViPipe, 0x0006, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0007, 0xA0);
    imx277_slvs_write_register(ViPipe, 0x0008, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0009, 0x01);
    imx277_slvs_write_register(ViPipe, 0x000A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0011, 0x03);
    imx277_slvs_write_register(ViPipe, 0x0112, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0113, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000B, 0x20);
    imx277_slvs_write_register(ViPipe, 0x000C, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000D, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000E, 0x00);
    imx277_slvs_write_register(ViPipe, 0x001A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0039, 0xE4);
    imx277_slvs_write_register(ViPipe, 0x003A, 0x0F);
    imx277_slvs_write_register(ViPipe, 0x003E, 0xB4);
    imx277_slvs_write_register(ViPipe, 0x003F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0040, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0036, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0068, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D5, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D6, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D7, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D8, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D9, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00DA, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00EE, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0001, 0x11);
    imx277_slvs_write_register(ViPipe, 0x032B, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x032C, 0x03);
    imx277_slvs_write_register(ViPipe, 0x034B, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x034C, 0x03);
    imx277_slvs_write_register(ViPipe, 0x05B6, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x05B7, 0x03);
    imx277_slvs_write_register(ViPipe, 0x05B8, 0x8e);
    imx277_slvs_write_register(ViPipe, 0x05B9, 0x0f);
    imx277_slvs_write_register(ViPipe, 0x0019, 0x00);
    imx277_slvs_write_register(ViPipe, 0x002E, 0x04);
    imx277_slvs_write_register(ViPipe, 0x002F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0030, 0x43);
    imx277_slvs_write_register(ViPipe, 0x0031, 0x05);
    imx277_slvs_write_register(ViPipe, 0x0032, 0x3E);
    imx277_slvs_write_register(ViPipe, 0x0033, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0041, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0042, 0x08);
    imx277_slvs_write_register(ViPipe, 0x0043, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0717, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0749, 0x90);
    imx277_slvs_write_register(ViPipe, 0x074C, 0x00);

    return;
}

void imx277_sensor_12m30fps_linear_init(VI_PIPE ViPipe)
{
    imx277_slvs_write_register(ViPipe, 0x0000, 0x08);
    delay_ms(2);
    imx277_slvs_write_register(ViPipe, 0x0003, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0004, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0005, 0x07);
    imx277_slvs_write_register(ViPipe, 0x0006, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0007, 0xA0);
    imx277_slvs_write_register(ViPipe, 0x0008, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0009, 0x01);
    imx277_slvs_write_register(ViPipe, 0x000A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0011, 0x03);
    imx277_slvs_write_register(ViPipe, 0x0112, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0113, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000B, 0x20);
    imx277_slvs_write_register(ViPipe, 0x000C, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000D, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000E, 0x00);
    imx277_slvs_write_register(ViPipe, 0x001A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0039, 0x00);
    imx277_slvs_write_register(ViPipe, 0x003A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x003E, 0x00);
    imx277_slvs_write_register(ViPipe, 0x003F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0040, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0036, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0068, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D5, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D6, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D7, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D8, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D9, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00DA, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00EE, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0001, 0x11);
    imx277_slvs_write_register(ViPipe, 0x032B, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x032C, 0x03);
    imx277_slvs_write_register(ViPipe, 0x034B, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x034C, 0x03);
    imx277_slvs_write_register(ViPipe, 0x05B6, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x05B7, 0x03);
    imx277_slvs_write_register(ViPipe, 0x05B8, 0x8e);
    imx277_slvs_write_register(ViPipe, 0x05B9, 0x0f);
    imx277_slvs_write_register(ViPipe, 0x0019, 0x00);
    imx277_slvs_write_register(ViPipe, 0x002E, 0x04);
    imx277_slvs_write_register(ViPipe, 0x002F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0030, 0x43);
    imx277_slvs_write_register(ViPipe, 0x0031, 0x05);
    imx277_slvs_write_register(ViPipe, 0x0032, 0x3E);
    imx277_slvs_write_register(ViPipe, 0x0033, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0041, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0042, 0x08);
    imx277_slvs_write_register(ViPipe, 0x0043, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0717, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0749, 0x90);
    imx277_slvs_write_register(ViPipe, 0x074C, 0x00);

    return;
}

void imx277_sensor_2m240fps_linear_init(VI_PIPE ViPipe)
{
    imx277_slvs_write_register(ViPipe, 0x0000, 0x08);
    delay_ms(2);
    imx277_slvs_write_register(ViPipe, 0x0003, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0004, 0x18);
    imx277_slvs_write_register(ViPipe, 0x0005, 0x25);
    imx277_slvs_write_register(ViPipe, 0x0006, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0007, 0xA0);
    imx277_slvs_write_register(ViPipe, 0x0008, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0009, 0x01);
    imx277_slvs_write_register(ViPipe, 0x000A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0011, 0x03);
    imx277_slvs_write_register(ViPipe, 0x0112, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0113, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000B, 0x20);
    imx277_slvs_write_register(ViPipe, 0x000C, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000D, 0x00);
    imx277_slvs_write_register(ViPipe, 0x000E, 0x00);
    imx277_slvs_write_register(ViPipe, 0x001A, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0039, 0xE4);
    imx277_slvs_write_register(ViPipe, 0x003A, 0x0F);
    imx277_slvs_write_register(ViPipe, 0x003E, 0xB4);
    imx277_slvs_write_register(ViPipe, 0x003F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0040, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0036, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0068, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D5, 0x01);
    imx277_slvs_write_register(ViPipe, 0x00D6, 0xD2);
    imx277_slvs_write_register(ViPipe, 0x00D7, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00D8, 0x69);
    imx277_slvs_write_register(ViPipe, 0x00D9, 0x00);
    imx277_slvs_write_register(ViPipe, 0x00DA, 0x06);
    imx277_slvs_write_register(ViPipe, 0x00EE, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0001, 0x11);
    imx277_slvs_write_register(ViPipe, 0x032B, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x032C, 0x03);
    imx277_slvs_write_register(ViPipe, 0x034B, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x034C, 0x03);
    imx277_slvs_write_register(ViPipe, 0x05B6, 0x9F);
    imx277_slvs_write_register(ViPipe, 0x05B7, 0x03);
    imx277_slvs_write_register(ViPipe, 0x05B8, 0x8e);
    imx277_slvs_write_register(ViPipe, 0x05B9, 0x0f);
    imx277_slvs_write_register(ViPipe, 0x0019, 0x00);
    imx277_slvs_write_register(ViPipe, 0x002E, 0x04);
    imx277_slvs_write_register(ViPipe, 0x002F, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0030, 0x43);
    imx277_slvs_write_register(ViPipe, 0x0031, 0x05);
    imx277_slvs_write_register(ViPipe, 0x0032, 0x3E);
    imx277_slvs_write_register(ViPipe, 0x0033, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0041, 0x10);
    imx277_slvs_write_register(ViPipe, 0x0042, 0x08);
    imx277_slvs_write_register(ViPipe, 0x0043, 0x01);
    imx277_slvs_write_register(ViPipe, 0x0717, 0x00);
    imx277_slvs_write_register(ViPipe, 0x0749, 0x90);
    imx277_slvs_write_register(ViPipe, 0x074C, 0x00);

    return;
}


void imx277_sensor_wdr_init(VI_PIPE ViPipe)
{
    return;
}
