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

#include "hi_sns_ctrl.h"
#include "hi_ssp.h"

#include "sharp8k_slave_priv.h"
#include "mpi_isp.h"

extern const SHARP8K_VIDEO_MODE_TBL_S g_astSharp8kModeTbl[];
extern ISP_SLAVE_SNS_SYNC_S gstSharp8kSync[];

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

extern HI_S32 g_Sharp8kSlaveBindDev[];
extern HI_U32 g_Sharp8kSalveSensorModeTime[];

extern ISP_SNS_STATE_S   *g_pastSharp8k[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U  g_aunSharp8kBusInfo[];

int sharp8k_spi_init(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        return HI_SUCCESS;
    }

    g_fd[ViPipe] = open("/dev/ssp", O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[ViPipe] < 0) {
        ISP_ERR_TRACE("Open %s error!\n", "/dev/ssp");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

int sharp8k_spi_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

int sharp8k_read_register(VI_PIPE ViPipe, unsigned int addr)
{
    HI_S32 s32Ret = 0;
    HI_U8  u8SpiNo;
    SPI_DATA_S spi_data = {0};

    u8SpiNo = g_aunSharp8kBusInfo[ViPipe].s8SspDev.bit4SspDev;

    spi_data.spi_no = u8SpiNo;
    spi_data.reg_addr = addr;
    spi_data.data = 0;

    spi_data.addr_byte_num = 1;
    spi_data.data_byte_num = 1;
    spi_data.dev_addr = 1;
    spi_data.dev_byte_num = 1;

    s32Ret = ioctl(g_fd[ViPipe], SSP_READ_ALT, &spi_data);
    if (s32Ret) {
        ISP_ERR_TRACE("SSP_READ_ALT Error!\n");
        return HI_FAILURE;
    }

    return spi_data.data;
}

int sharp8k_write_register(VI_PIPE ViPipe, unsigned int addr, unsigned char data)
{
    HI_S32 s32Ret = 0;
    HI_U8  u8SpiNo;
    SPI_DATA_S spi_data = {0};

    u8SpiNo = g_aunSharp8kBusInfo[ViPipe].s8SspDev.bit4SspDev;

    spi_data.spi_no = u8SpiNo;
    spi_data.reg_addr = addr;
    spi_data.data = data;

    spi_data.addr_byte_num = 1;
    spi_data.data_byte_num = 1;
    spi_data.dev_addr = 1;
    spi_data.dev_byte_num = 1;

    s32Ret = ioctl(g_fd[ViPipe], SSP_WRITE_ALT, &spi_data);
    if (s32Ret) {
        ISP_ERR_TRACE("SSP_WRITE_ALT Error!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);
}

void sharp8k_standby(VI_PIPE ViPipe)
{
    return;
}

void sharp8k_restart(VI_PIPE ViPipe)
{
    return;
}


void sharp8k_linear_8K30fps_init(VI_PIPE ViPipe);

void sharp8k_init(VI_PIPE ViPipe)
{
    HI_U8      u8ImgMode;
    HI_S32     SlaveDev;
    HI_U32     u32Data;

    u8ImgMode   = g_pastSharp8k[ViPipe]->u8ImgMode;
    SlaveDev    = g_Sharp8kSlaveBindDev[ViPipe];
    u32Data     = g_Sharp8kSalveSensorModeTime[ViPipe];

    /* 1.hold sync signal as fixed */
    CHECK_RET(HI_MPI_ISP_GetSnsSlaveAttr(SlaveDev, &gstSharp8kSync[ViPipe]));
    gstSharp8kSync[ViPipe].unCfg.stBits.bitHEnable = 0;
    gstSharp8kSync[ViPipe].unCfg.stBits.bitVEnable = 0;
    gstSharp8kSync[ViPipe].u32SlaveModeTime = u32Data;

    CHECK_RET(HI_MPI_ISP_SetSnsSlaveAttr(SlaveDev, &gstSharp8kSync[ViPipe]));

    /* 2. sharp8k spi init */
    if (sharp8k_spi_init(ViPipe)) {
        return;
    }

    /* 3. When sensor first init, config all registers */
    // release hv sync
    gstSharp8kSync[ViPipe].u32HsTime = g_astSharp8kModeTbl[u8ImgMode].u32InckPerHs;
    if (g_pastSharp8k[ViPipe]->astRegsInfo[0].stSlvSync.u32SlaveVsTime == 0) {
        gstSharp8kSync[ViPipe].u32VsTime = g_astSharp8kModeTbl[u8ImgMode].u32InckPerVs;
    } else {
        gstSharp8kSync[ViPipe].u32VsTime = g_pastSharp8k[ViPipe]->astRegsInfo[0].stSlvSync.u32SlaveVsTime;
    }
    gstSharp8kSync[ViPipe].unCfg.u32Bytes = 0xc0030000;
    gstSharp8kSync[ViPipe].u32HsCyc = 0x1;
    gstSharp8kSync[ViPipe].u32VsCyc = 0x1;

    printf("u32HsTime = %x u32VsTime = %x\n", gstSharp8kSync[ViPipe].u32HsTime, gstSharp8kSync[ViPipe].u32VsTime);
    CHECK_RET(HI_MPI_ISP_SetSnsSlaveAttr(SlaveDev, &gstSharp8kSync[ViPipe]));

    switch (u8ImgMode) {
        case SHARP8K_8K30FPS_LINER_MODE:
            sharp8k_linear_8K30fps_init(ViPipe);
            break;

        default:
            break;
    }

    g_pastSharp8k[ViPipe]->bInit = HI_TRUE;

    return;
}

void sharp8k_exit(VI_PIPE ViPipe)
{
    sharp8k_spi_exit(ViPipe);

    return;
}

/* CMOSIS Sharp8k 8K30fps (7680*4320) init */
void sharp8k_linear_8K30fps_init(VI_PIPE ViPipe)
{
    sharp8k_write_register(ViPipe, 0x0007, 0x01);
    sharp8k_write_register(ViPipe, 0x06A5, 0xF0);
    sharp8k_write_register(ViPipe, 0x06A6, 0xF0);
    sharp8k_write_register(ViPipe, 0x0704, 0x0A);
    sharp8k_write_register(ViPipe, 0x06C6, 0x01);
    sharp8k_write_register(ViPipe, 0x06C5, 0x01);
    sharp8k_write_register(ViPipe, 0x06C3, 0x01);
    sharp8k_write_register(ViPipe, 0x06CD, 0x00);
    sharp8k_write_register(ViPipe, 0x06CE, 0x00);
    sharp8k_write_register(ViPipe, 0x001E, 0x10);
    sharp8k_write_register(ViPipe, 0x000C, 0x0C);
    sharp8k_write_register(ViPipe, 0x001F, 0x03);
    sharp8k_write_register(ViPipe, 0x0020, 0x38);
    sharp8k_write_register(ViPipe, 0x0721, 0x17);
    sharp8k_write_register(ViPipe, 0x001F, 0x01);
    sharp8k_write_register(ViPipe, 0x0020, 0x9C);
    sharp8k_write_register(ViPipe, 0x0721, 0x1F);
    sharp8k_write_register(ViPipe, 0x00AF, 0x04);

    sharp8k_write_register(ViPipe, 0x0021, 0x16);
    sharp8k_write_register(ViPipe, 0x0022, 0x81);
    sharp8k_write_register(ViPipe, 0x0023, 0x68);
    sharp8k_write_register(ViPipe, 0x0024, 0x17);

    sharp8k_write_register(ViPipe, 0x0025, 0x16);
    sharp8k_write_register(ViPipe, 0x0026, 0x81);
    sharp8k_write_register(ViPipe, 0x0027, 0x68);
    sharp8k_write_register(ViPipe, 0x0028, 0x16);

    sharp8k_write_register(ViPipe, 0x0559, 0x00);
    sharp8k_write_register(ViPipe, 0x055B, 0x80);
    sharp8k_write_register(ViPipe, 0x067B, 0x00);
    sharp8k_write_register(ViPipe, 0x067C, 0x00);
    sharp8k_write_register(ViPipe, 0x067D, 0x00);
    sharp8k_write_register(ViPipe, 0x067E, 0x00);
    sharp8k_write_register(ViPipe, 0x01BF, 0x90);
    sharp8k_write_register(ViPipe, 0x01C0, 0x44);
    sharp8k_write_register(ViPipe, 0x01C1, 0x44);
    sharp8k_write_register(ViPipe, 0x01C2, 0x13);
    sharp8k_write_register(ViPipe, 0x0464, 0x10);
    sharp8k_write_register(ViPipe, 0x0443, 0x05);
    sharp8k_write_register(ViPipe, 0x02AC, 0x0C);
    sharp8k_write_register(ViPipe, 0x03A4, 0x0C);
    sharp8k_write_register(ViPipe, 0x02AD, 0x0C);
    sharp8k_write_register(ViPipe, 0x03A5, 0x0C);
    sharp8k_write_register(ViPipe, 0x0441, 0x06);
    sharp8k_write_register(ViPipe, 0x0442, 0x06);
    sharp8k_write_register(ViPipe, 0x0155, 0x07);
    sharp8k_write_register(ViPipe, 0x0156, 0x05);
    sharp8k_write_register(ViPipe, 0x0197, 0x07);
    sharp8k_write_register(ViPipe, 0x0198, 0x05);
    sharp8k_write_register(ViPipe, 0x016A, 0x02);
    sharp8k_write_register(ViPipe, 0x01AC, 0x02);
    sharp8k_write_register(ViPipe, 0x03C5, 0x00);
    sharp8k_write_register(ViPipe, 0x0446, 0x0B);
    sharp8k_write_register(ViPipe, 0x0447, 0x3A);
    sharp8k_write_register(ViPipe, 0x01B9, 0x00);
    sharp8k_write_register(ViPipe, 0x01BA, 0x00);
    sharp8k_write_register(ViPipe, 0x01BB, 0x02);
    sharp8k_write_register(ViPipe, 0x01BC, 0x00);
    sharp8k_write_register(ViPipe, 0x01BD, 0x03);
    sharp8k_write_register(ViPipe, 0x01BE, 0x00);
    sharp8k_write_register(ViPipe, 0x0123, 0x00);
    sharp8k_write_register(ViPipe, 0x0124, 0x00);
    sharp8k_write_register(ViPipe, 0x0125, 0x02);
    sharp8k_write_register(ViPipe, 0x0126, 0x00);
    sharp8k_write_register(ViPipe, 0x0127, 0x03);
    sharp8k_write_register(ViPipe, 0x0128, 0x00);
    sharp8k_write_register(ViPipe, 0x03BE, 0x00);
    sharp8k_write_register(ViPipe, 0x03BF, 0x00);
    sharp8k_write_register(ViPipe, 0x03C0, 0x02);
    sharp8k_write_register(ViPipe, 0x03C1, 0x00);
    sharp8k_write_register(ViPipe, 0x03C2, 0x03);
    sharp8k_write_register(ViPipe, 0x03C3, 0x00);
    sharp8k_write_register(ViPipe, 0x01CE, 0x07);
    sharp8k_write_register(ViPipe, 0x01CF, 0x07);
    sharp8k_write_register(ViPipe, 0x01D0, 0x04);
    sharp8k_write_register(ViPipe, 0x01D1, 0x03);
    sharp8k_write_register(ViPipe, 0x01D3, 0x7F);
    sharp8k_write_register(ViPipe, 0x01D4, 0x70);
    sharp8k_write_register(ViPipe, 0x01D5, 0x40);
    sharp8k_write_register(ViPipe, 0x01D6, 0x60);
    sharp8k_write_register(ViPipe, 0x02C6, 0x07);
    sharp8k_write_register(ViPipe, 0x02C7, 0x07);
    sharp8k_write_register(ViPipe, 0x02C8, 0x04);
    sharp8k_write_register(ViPipe, 0x02C9, 0x03);
    sharp8k_write_register(ViPipe, 0x02CB, 0x7F);
    sharp8k_write_register(ViPipe, 0x02CC, 0x70);
    sharp8k_write_register(ViPipe, 0x02CD, 0x08);
    sharp8k_write_register(ViPipe, 0x02CE, 0x30);
    sharp8k_write_register(ViPipe, 0x0402, 0x03);
    sharp8k_write_register(ViPipe, 0x0403, 0x05);
    sharp8k_write_register(ViPipe, 0x0404, 0x06);
    sharp8k_write_register(ViPipe, 0x0405, 0x07);
    sharp8k_write_register(ViPipe, 0x0407, 0x48);
    sharp8k_write_register(ViPipe, 0x0408, 0x58);
    sharp8k_write_register(ViPipe, 0x0409, 0x28);
    sharp8k_write_register(ViPipe, 0x040A, 0x08);
    sharp8k_write_register(ViPipe, 0x042D, 0x03);
    sharp8k_write_register(ViPipe, 0x042E, 0x05);
    sharp8k_write_register(ViPipe, 0x042F, 0x06);
    sharp8k_write_register(ViPipe, 0x0430, 0x07);
    sharp8k_write_register(ViPipe, 0x0432, 0x78);
    sharp8k_write_register(ViPipe, 0x0433, 0x78);
    sharp8k_write_register(ViPipe, 0x0434, 0x42);
    sharp8k_write_register(ViPipe, 0x0435, 0x20);
    sharp8k_write_register(ViPipe, 0x0586, 0x01);
    sharp8k_write_register(ViPipe, 0x0096, 0x0A);
    sharp8k_write_register(ViPipe, 0x009A, 0x0A);
    sharp8k_write_register(ViPipe, 0x00E9, 0x09);
    sharp8k_write_register(ViPipe, 0x00ED, 0x09);
    sharp8k_write_register(ViPipe, 0x00EF, 0x20);
    sharp8k_write_register(ViPipe, 0x00F1, 0x09);
    sharp8k_write_register(ViPipe, 0x00F3, 0x6E);
    sharp8k_write_register(ViPipe, 0x00F5, 0x89);
    sharp8k_write_register(ViPipe, 0x0129, 0x01);
    sharp8k_write_register(ViPipe, 0x012A, 0x03);
    sharp8k_write_register(ViPipe, 0x012B, 0x09);
    sharp8k_write_register(ViPipe, 0x0138, 0x09);
    sharp8k_write_register(ViPipe, 0x017A, 0x09);
    sharp8k_write_register(ViPipe, 0x01DD, 0x72);
    sharp8k_write_register(ViPipe, 0x01DF, 0x76);
    sharp8k_write_register(ViPipe, 0x01F1, 0x74);
    sharp8k_write_register(ViPipe, 0x01F3, 0x78);
    sharp8k_write_register(ViPipe, 0x024D, 0xAF);
    sharp8k_write_register(ViPipe, 0x024F, 0x71);
    sharp8k_write_register(ViPipe, 0x0251, 0xD0);
    sharp8k_write_register(ViPipe, 0x0252, 0x03);
    sharp8k_write_register(ViPipe, 0x0253, 0x09);
    sharp8k_write_register(ViPipe, 0x0255, 0xAF);
    sharp8k_write_register(ViPipe, 0x0257, 0x71);
    sharp8k_write_register(ViPipe, 0x0259, 0xD0);
    sharp8k_write_register(ViPipe, 0x025A, 0x03);
    sharp8k_write_register(ViPipe, 0x025B, 0x09);
    sharp8k_write_register(ViPipe, 0x025F, 0xBE);
    sharp8k_write_register(ViPipe, 0x0261, 0xA8);
    sharp8k_write_register(ViPipe, 0x0263, 0xDE);
    sharp8k_write_register(ViPipe, 0x0267, 0x6D);
    sharp8k_write_register(ViPipe, 0x0269, 0xD5);
    sharp8k_write_register(ViPipe, 0x026A, 0x03);
    sharp8k_write_register(ViPipe, 0x026B, 0x01);
    sharp8k_write_register(ViPipe, 0x026F, 0x71);
    sharp8k_write_register(ViPipe, 0x0271, 0xA9);
    sharp8k_write_register(ViPipe, 0x0272, 0x03);
    sharp8k_write_register(ViPipe, 0x0273, 0x09);
    sharp8k_write_register(ViPipe, 0x0277, 0x71);
    sharp8k_write_register(ViPipe, 0x0279, 0xD9);
    sharp8k_write_register(ViPipe, 0x027A, 0x03);
    sharp8k_write_register(ViPipe, 0x027B, 0x09);
    sharp8k_write_register(ViPipe, 0x027D, 0x20);
    sharp8k_write_register(ViPipe, 0x027E, 0x00);
    sharp8k_write_register(ViPipe, 0x0280, 0x20);
    sharp8k_write_register(ViPipe, 0x0281, 0x00);
    sharp8k_write_register(ViPipe, 0x0283, 0x89);
    sharp8k_write_register(ViPipe, 0x0284, 0x00);
    sharp8k_write_register(ViPipe, 0x0286, 0x89);
    sharp8k_write_register(ViPipe, 0x0287, 0x00);
    sharp8k_write_register(ViPipe, 0x0289, 0x6E);
    sharp8k_write_register(ViPipe, 0x028A, 0x02);
    sharp8k_write_register(ViPipe, 0x028B, 0x03);
    sharp8k_write_register(ViPipe, 0x028C, 0x06);
    sharp8k_write_register(ViPipe, 0x028D, 0x02);
    sharp8k_write_register(ViPipe, 0x028E, 0x03);
    sharp8k_write_register(ViPipe, 0x028F, 0x02);
    sharp8k_write_register(ViPipe, 0x0290, 0x02);
    sharp8k_write_register(ViPipe, 0x0291, 0x03);
    sharp8k_write_register(ViPipe, 0x0292, 0x06);
    sharp8k_write_register(ViPipe, 0x0293, 0x02);
    sharp8k_write_register(ViPipe, 0x0294, 0x03);
    sharp8k_write_register(ViPipe, 0x0295, 0x02);
    sharp8k_write_register(ViPipe, 0x0296, 0x02);
    sharp8k_write_register(ViPipe, 0x029C, 0xD7);
    sharp8k_write_register(ViPipe, 0x029E, 0xDC);
    sharp8k_write_register(ViPipe, 0x02D5, 0x72);
    sharp8k_write_register(ViPipe, 0x02D7, 0x76);
    sharp8k_write_register(ViPipe, 0x02E9, 0x74);
    sharp8k_write_register(ViPipe, 0x02EB, 0x78);
    sharp8k_write_register(ViPipe, 0x0345, 0xAF);
    sharp8k_write_register(ViPipe, 0x0347, 0x71);
    sharp8k_write_register(ViPipe, 0x0349, 0xD0);
    sharp8k_write_register(ViPipe, 0x034A, 0x03);
    sharp8k_write_register(ViPipe, 0x034B, 0x09);
    sharp8k_write_register(ViPipe, 0x034D, 0xAF);
    sharp8k_write_register(ViPipe, 0x034F, 0x71);
    sharp8k_write_register(ViPipe, 0x0351, 0xD0);
    sharp8k_write_register(ViPipe, 0x0352, 0x03);
    sharp8k_write_register(ViPipe, 0x0353, 0x09);
    sharp8k_write_register(ViPipe, 0x0357, 0xBE);
    sharp8k_write_register(ViPipe, 0x0359, 0xA8);
    sharp8k_write_register(ViPipe, 0x035B, 0xDE);
    sharp8k_write_register(ViPipe, 0x035F, 0x6D);
    sharp8k_write_register(ViPipe, 0x0361, 0xD5);
    sharp8k_write_register(ViPipe, 0x0362, 0x03);
    sharp8k_write_register(ViPipe, 0x0363, 0x01);
    sharp8k_write_register(ViPipe, 0x0367, 0x71);
    sharp8k_write_register(ViPipe, 0x0369, 0xA9);
    sharp8k_write_register(ViPipe, 0x036A, 0x03);
    sharp8k_write_register(ViPipe, 0x036B, 0x09);
    sharp8k_write_register(ViPipe, 0x036F, 0x71);
    sharp8k_write_register(ViPipe, 0x0371, 0xD9);
    sharp8k_write_register(ViPipe, 0x0372, 0x03);
    sharp8k_write_register(ViPipe, 0x0373, 0x09);
    sharp8k_write_register(ViPipe, 0x0375, 0x20);
    sharp8k_write_register(ViPipe, 0x0376, 0x00);
    sharp8k_write_register(ViPipe, 0x0378, 0x20);
    sharp8k_write_register(ViPipe, 0x0379, 0x00);
    sharp8k_write_register(ViPipe, 0x037B, 0x89);
    sharp8k_write_register(ViPipe, 0x037C, 0x00);
    sharp8k_write_register(ViPipe, 0x037E, 0x89);
    sharp8k_write_register(ViPipe, 0x037F, 0x00);
    sharp8k_write_register(ViPipe, 0x0381, 0x6E);
    sharp8k_write_register(ViPipe, 0x0382, 0x02);
    sharp8k_write_register(ViPipe, 0x0383, 0x03);
    sharp8k_write_register(ViPipe, 0x0384, 0x06);
    sharp8k_write_register(ViPipe, 0x0385, 0x02);
    sharp8k_write_register(ViPipe, 0x0386, 0x03);
    sharp8k_write_register(ViPipe, 0x0387, 0x02);
    sharp8k_write_register(ViPipe, 0x0388, 0x02);
    sharp8k_write_register(ViPipe, 0x0389, 0x03);
    sharp8k_write_register(ViPipe, 0x038A, 0x06);
    sharp8k_write_register(ViPipe, 0x038B, 0x02);
    sharp8k_write_register(ViPipe, 0x038C, 0x03);
    sharp8k_write_register(ViPipe, 0x038D, 0x02);
    sharp8k_write_register(ViPipe, 0x038E, 0x02);
    sharp8k_write_register(ViPipe, 0x0394, 0xD7);
    sharp8k_write_register(ViPipe, 0x0396, 0xDC);
    sharp8k_write_register(ViPipe, 0x03EF, 0x6F);
    sharp8k_write_register(ViPipe, 0x03F1, 0xD9);
    sharp8k_write_register(ViPipe, 0x03FD, 0x6D);
    sharp8k_write_register(ViPipe, 0x03FF, 0xDA);
    sharp8k_write_register(ViPipe, 0x0400, 0x03);
    sharp8k_write_register(ViPipe, 0x0401, 0x01);
    sharp8k_write_register(ViPipe, 0x041A, 0x6F);
    sharp8k_write_register(ViPipe, 0x041C, 0xD9);
    sharp8k_write_register(ViPipe, 0x0428, 0x6D);
    sharp8k_write_register(ViPipe, 0x042A, 0xDA);
    sharp8k_write_register(ViPipe, 0x042B, 0x03);
    sharp8k_write_register(ViPipe, 0x042C, 0x01);
    sharp8k_write_register(ViPipe, 0x04F5, 0x71);
    sharp8k_write_register(ViPipe, 0x04F7, 0xD9);
    sharp8k_write_register(ViPipe, 0x04F8, 0x03);
    sharp8k_write_register(ViPipe, 0x04F9, 0x09);
    sharp8k_write_register(ViPipe, 0x052E, 0x71);
    sharp8k_write_register(ViPipe, 0x0530, 0xD9);
    sharp8k_write_register(ViPipe, 0x0531, 0x03);
    sharp8k_write_register(ViPipe, 0x0532, 0x09);
    sharp8k_write_register(ViPipe, 0x04FD, 0xBE);
    sharp8k_write_register(ViPipe, 0x04FF, 0xA8);
    sharp8k_write_register(ViPipe, 0x0501, 0xDE);
    sharp8k_write_register(ViPipe, 0x0536, 0xBE);
    sharp8k_write_register(ViPipe, 0x0538, 0xA8);
    sharp8k_write_register(ViPipe, 0x053A, 0xDE);
    sharp8k_write_register(ViPipe, 0x0569, 0xEF);
    sharp8k_write_register(ViPipe, 0x0742, 0x04);
    sharp8k_write_register(ViPipe, 0x072D, 0x01);
    sharp8k_write_register(ViPipe, 0x036D, 0xB0);
    sharp8k_write_register(ViPipe, 0x04F3, 0xB0);
    sharp8k_write_register(ViPipe, 0x0371, 0xD8);
    sharp8k_write_register(ViPipe, 0x04F7, 0xD8);
    sharp8k_write_register(ViPipe, 0x06F0, 0x05);
    sharp8k_write_register(ViPipe, 0x06F3, 0x05);
    sharp8k_write_register(ViPipe, 0x06F4, 0x05);
    sharp8k_write_register(ViPipe, 0x06F7, 0x05);
    sharp8k_write_register(ViPipe, 0x06F8, 0x05);
    sharp8k_write_register(ViPipe, 0x06FB, 0x05);
    sharp8k_write_register(ViPipe, 0x06FC, 0x05);
    sharp8k_write_register(ViPipe, 0x06FF, 0x05);
    sharp8k_write_register(ViPipe, 0x06F1, 0x06);
    sharp8k_write_register(ViPipe, 0x06F2, 0x06);
    sharp8k_write_register(ViPipe, 0x06F5, 0x06);
    sharp8k_write_register(ViPipe, 0x06F6, 0x06);
    sharp8k_write_register(ViPipe, 0x06F9, 0x06);
    sharp8k_write_register(ViPipe, 0x06FA, 0x06);
    sharp8k_write_register(ViPipe, 0x06FD, 0x06);
    sharp8k_write_register(ViPipe, 0x06FE, 0x06);
    sharp8k_write_register(ViPipe, 0x0569, 0xFF);
    sharp8k_write_register(ViPipe, 0x06A0, 0x3F);
    sharp8k_write_register(ViPipe, 0x0470, 0x00);
    sharp8k_write_register(ViPipe, 0x009B, 0x00);
    sharp8k_write_register(ViPipe, 0x009C, 0x00);
    sharp8k_write_register(ViPipe, 0x0472, 0x00);
    sharp8k_write_register(ViPipe, 0x073F, 0x09);
    sharp8k_write_register(ViPipe, 0x273A, 0x00);
    sharp8k_write_register(ViPipe, 0x173A, 0x00);
    sharp8k_write_register(ViPipe, 0x273B, 0x00);
    sharp8k_write_register(ViPipe, 0x173B, 0x00);
    sharp8k_write_register(ViPipe, 0x273C, 0x01);
    sharp8k_write_register(ViPipe, 0x173C, 0x03);
    sharp8k_write_register(ViPipe, 0x273D, 0x03);
    sharp8k_write_register(ViPipe, 0x173D, 0x03);
    sharp8k_write_register(ViPipe, 0x2555, 0x08);
    sharp8k_write_register(ViPipe, 0x1555, 0x08);
    sharp8k_write_register(ViPipe, 0x2556, 0xFF);
    sharp8k_write_register(ViPipe, 0x1556, 0xFF);
    sharp8k_write_register(ViPipe, 0x272E, 0x01);
    sharp8k_write_register(ViPipe, 0x172E, 0x01);
    sharp8k_write_register(ViPipe, 0x2742, 0x00);
    sharp8k_write_register(ViPipe, 0x1742, 0x00);
    sharp8k_write_register(ViPipe, 0x272D, 0x01);
    sharp8k_write_register(ViPipe, 0x172D, 0x01);
    sharp8k_write_register(ViPipe, 0x2471, 0x06);
    sharp8k_write_register(ViPipe, 0x1471, 0x06);
    sharp8k_write_register(ViPipe, 0x25EF, 0x01);
    sharp8k_write_register(ViPipe, 0x15EF, 0x01);
    sharp8k_write_register(ViPipe, 0x02AF, 0x00);
    sharp8k_write_register(ViPipe, 0x02B0, 0x00);
    sharp8k_write_register(ViPipe, 0x03A7, 0x00);
    sharp8k_write_register(ViPipe, 0x03A8, 0x00);
    sharp8k_write_register(ViPipe, 0x016D, 0x01);
    sharp8k_write_register(ViPipe, 0x01AF, 0x01);
    sharp8k_write_register(ViPipe, 0x02BC, 0x08);
    sharp8k_write_register(ViPipe, 0x03B4, 0x08);
    sharp8k_write_register(ViPipe, 0x0558, 0x5B);
    sharp8k_write_register(ViPipe, 0x055A, 0x5E);
    sharp8k_write_register(ViPipe, 0x044E, 0x62);
    sharp8k_write_register(ViPipe, 0x044F, 0x65);
    sharp8k_write_register(ViPipe, 0x064E, 0x00);
    sharp8k_write_register(ViPipe, 0x064F, 0x3C);
    sharp8k_write_register(ViPipe, 0x0650, 0x00);
    sharp8k_write_register(ViPipe, 0x0651, 0x05);
    sharp8k_write_register(ViPipe, 0x0652, 0x00);
    sharp8k_write_register(ViPipe, 0x0653, 0x00);
    sharp8k_write_register(ViPipe, 0x064B, 0x03);
    sharp8k_write_register(ViPipe, 0x064C, 0xE8);
    sharp8k_write_register(ViPipe, 0x0561, 0x06);
    sharp8k_write_register(ViPipe, 0x02A9, 0x00);
    sharp8k_write_register(ViPipe, 0x02AA, 0x00);
    sharp8k_write_register(ViPipe, 0x03A1, 0x00);
    sharp8k_write_register(ViPipe, 0x03A2, 0x00);
    sharp8k_write_register(ViPipe, 0x0449, 0x07);
    sharp8k_write_register(ViPipe, 0x044A, 0x07);
    sharp8k_write_register(ViPipe, 0x0705, 0xAA);
    sharp8k_write_register(ViPipe, 0x0706, 0xAA);
    sharp8k_write_register(ViPipe, 0x0707, 0xAA);
    sharp8k_write_register(ViPipe, 0x0708, 0xAA);
    sharp8k_write_register(ViPipe, 0x0709, 0xAA);
    sharp8k_write_register(ViPipe, 0x070A, 0xAA);
    sharp8k_write_register(ViPipe, 0x070B, 0xAA);
    sharp8k_write_register(ViPipe, 0x070C, 0xAA);
    sharp8k_write_register(ViPipe, 0x070D, 0xAA);
    sharp8k_write_register(ViPipe, 0x070E, 0xAA);
    sharp8k_write_register(ViPipe, 0x0013, 0x00);
    sharp8k_write_register(ViPipe, 0x0014, 0xEC);
    sharp8k_write_register(ViPipe, 0x0444, 0x3F);
    sharp8k_write_register(ViPipe, 0x0006, 0x01);

    delay_ms(10);

    printf("CMOS sharp8k sensor 7680x4320@30fps linear mode init success!\n");

    return;
}
