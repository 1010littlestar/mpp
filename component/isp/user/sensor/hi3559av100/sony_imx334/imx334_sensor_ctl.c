/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#endif

const unsigned char imx334_i2c_addr     =    0x34;        /* I2C Address of Imx334 */
const unsigned int  imx334_addr_byte    =    2;
const unsigned int  imx334_data_byte    =    1;
static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

extern ISP_SNS_STATE_S   *g_pastImx334[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U  g_aunImx334BusInfo[];

#define IMX334_8M_30FPS_12BIT_LINEAR_MODE  (1)
#define IMX334_8M_30FPS_12BIT_2t1_DOL_MODE (2)


int imx334_i2c_init(VI_PIPE ViPipe)
{
    char acDevFile[16] = {0};
    HI_U8 u8DevNum;

    if (g_fd[ViPipe] >= 0) {
        return HI_SUCCESS;
    }
#ifdef HI_GPIO_I2C
    int ret;

    g_fd[ViPipe] = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (g_fd[ViPipe] < 0) {
        ISP_ERR_TRACE("Open gpioi2c_ex error!\n");
        return HI_FAILURE;
    }
#else
    int ret;
    u8DevNum = g_aunImx334BusInfo[ViPipe].s8I2cDev;

    snprintf(acDevFile, sizeof(acDevFile),  "/dev/i2c-%u", u8DevNum);

    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (imx334_i2c_addr >> 1));
    if (ret < 0) {
        ISP_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return HI_SUCCESS;
}

int imx334_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

int imx334_read_register(VI_PIPE ViPipe, int addr)
{
    return HI_SUCCESS;
}
int imx334_write_register(VI_PIPE ViPipe, int addr, int data)
{
    if (g_fd[ViPipe] < 0) {
        return HI_SUCCESS;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = imx334_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = imx334_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = imx334_data_byte;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

    if (ret) {
        ISP_ERR_TRACE("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];
    if (imx334_addr_byte == 2) {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (imx334_data_byte == 2) {
        buf[idx] = (data >> 8) & 0xff;
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, (imx334_addr_byte + imx334_data_byte));
    if (ret < 0) {
        ISP_ERR_TRACE("I2C_WRITE DATA error!\n");
        return HI_FAILURE;
    }

#endif
    return HI_SUCCESS;
}

void imx334_standby(VI_PIPE ViPipe)
{
    return;
}

void imx334_restart(VI_PIPE ViPipe)
{
    return;
}


static void delay_ms(int ms)
{
    usleep(ms * 1000);
}

void imx334_linear_8M30_12bit_init(VI_PIPE ViPipe);
void imx334_DOL_2t1_8M30_12bit_init(VI_PIPE ViPipe);

void imx334_default_reg_init(VI_PIPE ViPipe)
{
    HI_U32 i = 0;
    for (i = 0; i < g_pastImx334[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
        imx334_write_register(ViPipe, g_pastImx334[ViPipe]->astRegsInfo[0].astI2cData[i].u32RegAddr, g_pastImx334[ViPipe]->astRegsInfo[0].astI2cData[i].u32Data);
    }

}
void imx334_init(VI_PIPE ViPipe)
{
    HI_U8           u8ImgMode;
    HI_BOOL         bInit;

    bInit       = g_pastImx334[ViPipe]->bInit;
    u8ImgMode   = g_pastImx334[ViPipe]->u8ImgMode;

    /* 1. sensor i2c init */
    imx334_i2c_init(ViPipe);

    if (bInit == HI_FALSE) {
        /* 2.  sensor registers init */
        if (u8ImgMode == IMX334_8M_30FPS_12BIT_LINEAR_MODE) {     /* 4K@30fps Linear */
            imx334_linear_8M30_12bit_init(ViPipe);
        } else if (IMX334_8M_30FPS_12BIT_2t1_DOL_MODE == u8ImgMode) { /* 4K@30fps DOL2 */
            imx334_DOL_2t1_8M30_12bit_init(ViPipe);
        }
    } else {

    /* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
        /* 2.  sensor registers init */
        if (u8ImgMode == IMX334_8M_30FPS_12BIT_LINEAR_MODE) {     /* 4K@30fps Linear */
            imx334_linear_8M30_12bit_init(ViPipe);
        } else if (IMX334_8M_30FPS_12BIT_2t1_DOL_MODE == u8ImgMode) { /* 4K@30fps DOL2 */
            imx334_DOL_2t1_8M30_12bit_init(ViPipe);
        }
    }

    g_pastImx334[ViPipe]->bInit = HI_TRUE;

    return;

}

void imx334_exit(VI_PIPE ViPipe)
{
    imx334_i2c_exit(ViPipe);

    return;
}

void imx334_linear_8M30_12bit_init(VI_PIPE ViPipe)
{
    imx334_write_register(ViPipe, 0x3000, 0x01); // standby
    imx334_write_register(ViPipe, 0x3001, 0x00);
    imx334_write_register(ViPipe, 0x3002, 0x01);
    imx334_write_register(ViPipe, 0x3003, 0x00);
    imx334_write_register(ViPipe, 0x300C, 0x3B);
    imx334_write_register(ViPipe, 0x300D, 0x2A);
    imx334_write_register(ViPipe, 0x3018, 0x00);
    imx334_write_register(ViPipe, 0x302C, 0x30);
    imx334_write_register(ViPipe, 0x302D, 0x00);
    imx334_write_register(ViPipe, 0x302E, 0x18);
    imx334_write_register(ViPipe, 0x302F, 0x0F);
    imx334_write_register(ViPipe, 0x3030, 0xCA);
    imx334_write_register(ViPipe, 0x3031, 0x08);
    imx334_write_register(ViPipe, 0x3032, 0x00);
    imx334_write_register(ViPipe, 0x3033, 0x00);
    imx334_write_register(ViPipe, 0x3034, 0x4C);
    imx334_write_register(ViPipe, 0x3035, 0x04);
    imx334_write_register(ViPipe, 0x304C, 0x14);

    imx334_write_register(ViPipe, 0x3050, 0x01);

    imx334_write_register(ViPipe, 0x3076, 0x84);
    imx334_write_register(ViPipe, 0x3077, 0x08);

    imx334_write_register(ViPipe, 0x3090, 0x84);
    imx334_write_register(ViPipe, 0x3091, 0x08);

    imx334_write_register(ViPipe, 0x30C6, 0x00);
    imx334_write_register(ViPipe, 0x30C7, 0x00);
    imx334_write_register(ViPipe, 0x30CE, 0x00);
    imx334_write_register(ViPipe, 0x30CF, 0x00);
    imx334_write_register(ViPipe, 0x30D8, 0xF8);
    imx334_write_register(ViPipe, 0x30D9, 0x11);

    imx334_write_register(ViPipe, 0x3117, 0x00);
    imx334_write_register(ViPipe, 0x314C, 0x29);
    imx334_write_register(ViPipe, 0x314D, 0x01);
    imx334_write_register(ViPipe, 0x315A, 0x06);
    imx334_write_register(ViPipe, 0x3168, 0xA0);
    imx334_write_register(ViPipe, 0x316A, 0x7E);
    imx334_write_register(ViPipe, 0x3199, 0x00);
    imx334_write_register(ViPipe, 0x319D, 0x01);
    imx334_write_register(ViPipe, 0x319E, 0x02);
    imx334_write_register(ViPipe, 0x31A0, 0x2A);
    imx334_write_register(ViPipe, 0x31D4, 0x00);
    imx334_write_register(ViPipe, 0x31D5, 0x00);
    imx334_write_register(ViPipe, 0x31DD, 0x03);
    imx334_write_register(ViPipe, 0x3288, 0x21);
    imx334_write_register(ViPipe, 0x328A, 0x02);
    imx334_write_register(ViPipe, 0x3300, 0x00);
    imx334_write_register(ViPipe, 0x3302, 0x32);
    imx334_write_register(ViPipe, 0x3303, 0x00);
    imx334_write_register(ViPipe, 0x3308, 0x84);
    imx334_write_register(ViPipe, 0x3309, 0x08);
    imx334_write_register(ViPipe, 0x3414, 0x05);
    imx334_write_register(ViPipe, 0x3416, 0x18);
    imx334_write_register(ViPipe, 0x341C, 0x47);
    imx334_write_register(ViPipe, 0x341D, 0x00);
    imx334_write_register(ViPipe, 0x35AC, 0x0E);
    imx334_write_register(ViPipe, 0x3648, 0x01);
    imx334_write_register(ViPipe, 0x364A, 0x04);
    imx334_write_register(ViPipe, 0x364C, 0x04);
    imx334_write_register(ViPipe, 0x3678, 0x01);
    imx334_write_register(ViPipe, 0x367C, 0x31);
    imx334_write_register(ViPipe, 0x367E, 0x31);
    imx334_write_register(ViPipe, 0x3708, 0x02);
    imx334_write_register(ViPipe, 0x3714, 0x01);
    imx334_write_register(ViPipe, 0x3715, 0x02);
    imx334_write_register(ViPipe, 0x3716, 0x02);
    imx334_write_register(ViPipe, 0x3717, 0x02);
    imx334_write_register(ViPipe, 0x371C, 0x3D);
    imx334_write_register(ViPipe, 0x371D, 0x3F);
    imx334_write_register(ViPipe, 0x372C, 0x00);
    imx334_write_register(ViPipe, 0x372D, 0x00);
    imx334_write_register(ViPipe, 0x372E, 0x46);
    imx334_write_register(ViPipe, 0x372F, 0x00);
    imx334_write_register(ViPipe, 0x3730, 0x89);
    imx334_write_register(ViPipe, 0x3731, 0x00);
    imx334_write_register(ViPipe, 0x3732, 0x08);
    imx334_write_register(ViPipe, 0x3733, 0x01);
    imx334_write_register(ViPipe, 0x3734, 0xFE);
    imx334_write_register(ViPipe, 0x3735, 0x05);
    imx334_write_register(ViPipe, 0x375D, 0x00);
    imx334_write_register(ViPipe, 0x375E, 0x00);
    imx334_write_register(ViPipe, 0x375F, 0x61);
    imx334_write_register(ViPipe, 0x3760, 0x06);
    imx334_write_register(ViPipe, 0x3768, 0x1B);
    imx334_write_register(ViPipe, 0x3769, 0x1B);
    imx334_write_register(ViPipe, 0x376A, 0x1A);
    imx334_write_register(ViPipe, 0x376B, 0x19);
    imx334_write_register(ViPipe, 0x376C, 0x18);
    imx334_write_register(ViPipe, 0x376D, 0x14);
    imx334_write_register(ViPipe, 0x376E, 0x0F);
    imx334_write_register(ViPipe, 0x3776, 0x00);
    imx334_write_register(ViPipe, 0x3777, 0x00);
    imx334_write_register(ViPipe, 0x3778, 0x46);
    imx334_write_register(ViPipe, 0x3779, 0x00);
    imx334_write_register(ViPipe, 0x377A, 0x08);
    imx334_write_register(ViPipe, 0x377B, 0x01);
    imx334_write_register(ViPipe, 0x377C, 0x45);
    imx334_write_register(ViPipe, 0x377D, 0x01);
    imx334_write_register(ViPipe, 0x377E, 0x23);
    imx334_write_register(ViPipe, 0x377F, 0x02);
    imx334_write_register(ViPipe, 0x3780, 0xD9);
    imx334_write_register(ViPipe, 0x3781, 0x03);
    imx334_write_register(ViPipe, 0x3782, 0xF5);
    imx334_write_register(ViPipe, 0x3783, 0x06);
    imx334_write_register(ViPipe, 0x3784, 0xA5);
    imx334_write_register(ViPipe, 0x3788, 0x0F);
    imx334_write_register(ViPipe, 0x378A, 0xD9);
    imx334_write_register(ViPipe, 0x378B, 0x03);
    imx334_write_register(ViPipe, 0x378C, 0xEB);
    imx334_write_register(ViPipe, 0x378D, 0x05);
    imx334_write_register(ViPipe, 0x378E, 0x87);
    imx334_write_register(ViPipe, 0x378F, 0x06);
    imx334_write_register(ViPipe, 0x3790, 0xF5);
    imx334_write_register(ViPipe, 0x3792, 0x43);
    imx334_write_register(ViPipe, 0x3794, 0x7A);
    imx334_write_register(ViPipe, 0x3796, 0xA1);
    imx334_write_register(ViPipe, 0x3A01, 0x03);
    imx334_write_register(ViPipe, 0x3A18, 0x7F);
    imx334_write_register(ViPipe, 0x3A19, 0x00);
    imx334_write_register(ViPipe, 0x3A1A, 0x37);
    imx334_write_register(ViPipe, 0x3A1B, 0x00);
    imx334_write_register(ViPipe, 0x3A1C, 0x37);
    imx334_write_register(ViPipe, 0x3A1D, 0x00);
    imx334_write_register(ViPipe, 0x3A1E, 0xF7);
    imx334_write_register(ViPipe, 0x3A1F, 0x00);
    imx334_write_register(ViPipe, 0x3A20, 0x3F);
    imx334_write_register(ViPipe, 0x3A21, 0x00);
    imx334_write_register(ViPipe, 0x3A22, 0x6F);
    imx334_write_register(ViPipe, 0x3A23, 0x00);
    imx334_write_register(ViPipe, 0x3A24, 0x3F);
    imx334_write_register(ViPipe, 0x3A25, 0x00);
    imx334_write_register(ViPipe, 0x3A26, 0x5F);
    imx334_write_register(ViPipe, 0x3A27, 0x00);
    imx334_write_register(ViPipe, 0x3A28, 0x2F);
    imx334_write_register(ViPipe, 0x3A29, 0x00);
    imx334_write_register(ViPipe, 0x3E04, 0x0E);
    imx334_write_register(ViPipe, 0x3078, 0x02);
    imx334_write_register(ViPipe, 0x3079, 0x00);
    imx334_write_register(ViPipe, 0x307A, 0x00);
    imx334_write_register(ViPipe, 0x307B, 0x00);

    imx334_write_register(ViPipe, 0x3081, 0x00);
    imx334_write_register(ViPipe, 0x3082, 0x00);
    imx334_write_register(ViPipe, 0x3083, 0x00);
    imx334_write_register(ViPipe, 0x3088, 0x02);
    imx334_write_register(ViPipe, 0x3094, 0x00);
    imx334_write_register(ViPipe, 0x3095, 0x00);
    imx334_write_register(ViPipe, 0x3096, 0x00);

    imx334_write_register(ViPipe, 0x309C, 0x00);
    imx334_write_register(ViPipe, 0x309D, 0x00);
    imx334_write_register(ViPipe, 0x309E, 0x00);
    imx334_write_register(ViPipe, 0x30A4, 0x00);
    imx334_write_register(ViPipe, 0x30A5, 0x00);

    // Sensor registers used for normal image
#if 1
    imx334_write_register(ViPipe, 0x304E, 0x00);
    imx334_write_register(ViPipe, 0x304F, 0x00);

    imx334_write_register(ViPipe, 0x3074, 0xB0);
    imx334_write_register(ViPipe, 0x3075, 0x00);

    imx334_write_register(ViPipe, 0x308E, 0xB1);
    imx334_write_register(ViPipe, 0x308F, 0x00);

    imx334_write_register(ViPipe, 0x30B6, 0x00);
    imx334_write_register(ViPipe, 0x30B7, 0x00);

    imx334_write_register(ViPipe, 0x3116, 0x00);
    imx334_write_register(ViPipe, 0x3080, 0x02);
    imx334_write_register(ViPipe, 0x309B, 0x02);
#endif

    imx334_default_reg_init(ViPipe);
    imx334_write_register(ViPipe, 0x3000, 0x00); // Standby Cancel
    delay_ms(18);
    imx334_write_register(ViPipe, 0x3002, 0x00);

    printf("===Imx334 8M30fps 12bit LINE Init OK!===\n");
    return;
}

void imx334_DOL_2t1_8M30_12bit_init(VI_PIPE ViPipe)
{
    imx334_write_register(ViPipe, 0x3000, 0x01); // standby
    imx334_write_register(ViPipe, 0x3001, 0x00);
    imx334_write_register(ViPipe, 0x3002, 0x01);
    imx334_write_register(ViPipe, 0x3003, 0x00);
    imx334_write_register(ViPipe, 0x300C, 0x3B);
    imx334_write_register(ViPipe, 0x300D, 0x2A);
    imx334_write_register(ViPipe, 0x3018, 0x00);

    imx334_write_register(ViPipe, 0x302C, 0x30);
    imx334_write_register(ViPipe, 0x302D, 0x00);
    imx334_write_register(ViPipe, 0x302E, 0x18);
    imx334_write_register(ViPipe, 0x302F, 0x0F);

    imx334_write_register(ViPipe, 0x3030, 0xCA);
    imx334_write_register(ViPipe, 0x3031, 0x08);
    imx334_write_register(ViPipe, 0x3032, 0x00);
    imx334_write_register(ViPipe, 0x3033, 0x00);

    imx334_write_register(ViPipe, 0x3034, 0x26);
    imx334_write_register(ViPipe, 0x3035, 0x02);
    imx334_write_register(ViPipe, 0x304C, 0x14);  // Virtual Channel Mode OPB_SIZE_V

    imx334_write_register(ViPipe, 0x3048, 0x01);  // WDMODE
    imx334_write_register(ViPipe, 0x3049, 0x01);  // WDSEL[1:0]
    imx334_write_register(ViPipe, 0x304A, 0x01);  // WDSEL1[2:0]
    imx334_write_register(ViPipe, 0x304B, 0x02);  // WDSEL2[3:0]

    imx334_write_register(ViPipe, 0x3050, 0x01);

#if 1
    imx334_write_register(ViPipe, 0x3058, 0xd2);  // SHR0[7:0]
    imx334_write_register(ViPipe, 0x3059, 0x08);  // SHR0[15:8]
    imx334_write_register(ViPipe, 0x305A, 0x00);  // SHR0[19:16]

    imx334_write_register(ViPipe, 0x305C, 0x09);  // SHR1[7:0]
    imx334_write_register(ViPipe, 0x305D, 0x00);  // SHR1[15:8]
    imx334_write_register(ViPipe, 0x305E, 0x00);  // SHR1[19:16]

    imx334_write_register(ViPipe, 0x3060, 0x00);  // SHR2[7:0]
    imx334_write_register(ViPipe, 0x3061, 0x00);  // SHR2[15:8]
    imx334_write_register(ViPipe, 0x3062, 0x00);  // SHR2[19:16]

    imx334_write_register(ViPipe, 0x3068, 0xc9);  // RHS1[7:0]
    imx334_write_register(ViPipe, 0x3069, 0x08);  // RHS1[15:8]
    imx334_write_register(ViPipe, 0x306A, 0x00);  // RHS1[19:16]

    imx334_write_register(ViPipe, 0x306C, 0x00);  // RHS2[7:0]
    imx334_write_register(ViPipe, 0x306D, 0x00);  // RHS2[15:8]
    imx334_write_register(ViPipe, 0x306E, 0x00);  // RHS2[19:16]
#endif

    imx334_write_register(ViPipe, 0x3076, 0x84);
    imx334_write_register(ViPipe, 0x3077, 0x08);

    imx334_write_register(ViPipe, 0x3090, 0x84);
    imx334_write_register(ViPipe, 0x3091, 0x08);

    imx334_write_register(ViPipe, 0x30C6, 0x00);
    imx334_write_register(ViPipe, 0x30C7, 0x00);
    imx334_write_register(ViPipe, 0x30CE, 0x00);
    imx334_write_register(ViPipe, 0x30CF, 0x00);
    imx334_write_register(ViPipe, 0x30D8, 0xF8);
    imx334_write_register(ViPipe, 0x30D9, 0x11);

    imx334_write_register(ViPipe, 0x30E8, 0x00);  // GAIN[7:0]
    imx334_write_register(ViPipe, 0x30E9, 0x00);  // GAIN[10:8]

    imx334_write_register(ViPipe, 0x30EA, 0x00);  // GAIN1[7:0]
    imx334_write_register(ViPipe, 0x30EB, 0x00);  // GAIN1[10:8]

    imx334_write_register(ViPipe, 0x30EC, 0x00);  // GAIN2[7:0]
    imx334_write_register(ViPipe, 0x30ED, 0x00);  // GAIN2[10:8]

    imx334_write_register(ViPipe, 0x3117, 0x00);
    imx334_write_register(ViPipe, 0x314C, 0x29);
    imx334_write_register(ViPipe, 0x314D, 0x01);
    imx334_write_register(ViPipe, 0x315A, 0x02);
    imx334_write_register(ViPipe, 0x3168, 0xA0);
    imx334_write_register(ViPipe, 0x316A, 0x7E);
    imx334_write_register(ViPipe, 0x3199, 0x00);

    imx334_write_register(ViPipe, 0x319D, 0x01);
    imx334_write_register(ViPipe, 0x319E, 0x00);
    imx334_write_register(ViPipe, 0x319F, 0x03);  // VCEN:Virtual Channel Mode
    imx334_write_register(ViPipe, 0x31A0, 0x2A);

    imx334_write_register(ViPipe, 0x31D4, 0x00);
    imx334_write_register(ViPipe, 0x31D5, 0x00);

    imx334_write_register(ViPipe, 0x31D7, 0x01); // XVSMSKCNT:[1:0]:1  [2:3]:1  [5:4]:1 [7:6]:0

    imx334_write_register(ViPipe, 0x31DD, 0x03);

    imx334_write_register(ViPipe, 0x3200, 0x11); // FGAINEN: Enable of the each frame gain adjustment 10h:valid 11h:Invalid

    imx334_write_register(ViPipe, 0x3288, 0x21);
    imx334_write_register(ViPipe, 0x328A, 0x02);
    imx334_write_register(ViPipe, 0x3300, 0x00);

    imx334_write_register(ViPipe, 0x3302, 0x32);
    imx334_write_register(ViPipe, 0x3303, 0x00);

    imx334_write_register(ViPipe, 0x3308, 0x8B);
    imx334_write_register(ViPipe, 0x3309, 0x08);

    imx334_write_register(ViPipe, 0x3414, 0x05);
    imx334_write_register(ViPipe, 0x3416, 0x18);
    imx334_write_register(ViPipe, 0x341C, 0x47);
    imx334_write_register(ViPipe, 0x341D, 0x00);
    imx334_write_register(ViPipe, 0x35AC, 0x0E);

    imx334_write_register(ViPipe, 0x3648, 0x01);
    imx334_write_register(ViPipe, 0x364A, 0x04);
    imx334_write_register(ViPipe, 0x364C, 0x04);
    imx334_write_register(ViPipe, 0x3678, 0x01);
    imx334_write_register(ViPipe, 0x367C, 0x31);
    imx334_write_register(ViPipe, 0x367E, 0x31);
    imx334_write_register(ViPipe, 0x3708, 0x02);
    imx334_write_register(ViPipe, 0x3714, 0x01);
    imx334_write_register(ViPipe, 0x3715, 0x02);

    imx334_write_register(ViPipe, 0x3716, 0x02);
    imx334_write_register(ViPipe, 0x3717, 0x02);
    imx334_write_register(ViPipe, 0x371C, 0x3D);
    imx334_write_register(ViPipe, 0x371D, 0x3F);
    imx334_write_register(ViPipe, 0x372C, 0x00);
    imx334_write_register(ViPipe, 0x372D, 0x00);
    imx334_write_register(ViPipe, 0x372E, 0x46);
    imx334_write_register(ViPipe, 0x372F, 0x00);
    imx334_write_register(ViPipe, 0x3730, 0x89);

    imx334_write_register(ViPipe, 0x3731, 0x00);
    imx334_write_register(ViPipe, 0x3732, 0x08);
    imx334_write_register(ViPipe, 0x3733, 0x01);
    imx334_write_register(ViPipe, 0x3734, 0xFE);
    imx334_write_register(ViPipe, 0x3735, 0x05);
    imx334_write_register(ViPipe, 0x375D, 0x00);
    imx334_write_register(ViPipe, 0x375E, 0x00);
    imx334_write_register(ViPipe, 0x375F, 0x61);
    imx334_write_register(ViPipe, 0x3760, 0x06);

    imx334_write_register(ViPipe, 0x3768, 0x1B);
    imx334_write_register(ViPipe, 0x3769, 0x1B);
    imx334_write_register(ViPipe, 0x376A, 0x1A);
    imx334_write_register(ViPipe, 0x376B, 0x19);
    imx334_write_register(ViPipe, 0x376C, 0x18);
    imx334_write_register(ViPipe, 0x376D, 0x14);
    imx334_write_register(ViPipe, 0x376E, 0x0F);
    imx334_write_register(ViPipe, 0x3776, 0x00);
    imx334_write_register(ViPipe, 0x3777, 0x00);

    imx334_write_register(ViPipe, 0x3778, 0x46);
    imx334_write_register(ViPipe, 0x3779, 0x00);
    imx334_write_register(ViPipe, 0x377A, 0x08);
    imx334_write_register(ViPipe, 0x377B, 0x01);
    imx334_write_register(ViPipe, 0x377C, 0x45);
    imx334_write_register(ViPipe, 0x377D, 0x01);
    imx334_write_register(ViPipe, 0x377E, 0x23);
    imx334_write_register(ViPipe, 0x377F, 0x02);
    imx334_write_register(ViPipe, 0x3780, 0xD9);

    imx334_write_register(ViPipe, 0x3781, 0x03);
    imx334_write_register(ViPipe, 0x3782, 0xF5);
    imx334_write_register(ViPipe, 0x3783, 0x06);
    imx334_write_register(ViPipe, 0x3784, 0xA5);
    imx334_write_register(ViPipe, 0x3788, 0x0F);
    imx334_write_register(ViPipe, 0x378A, 0xD9);

    imx334_write_register(ViPipe, 0x378B, 0x03);
    imx334_write_register(ViPipe, 0x378C, 0xEB);
    imx334_write_register(ViPipe, 0x378D, 0x05);
    imx334_write_register(ViPipe, 0x378E, 0x87);
    imx334_write_register(ViPipe, 0x378F, 0x06);
    imx334_write_register(ViPipe, 0x3790, 0xF5);
    imx334_write_register(ViPipe, 0x3792, 0x43);
    imx334_write_register(ViPipe, 0x3794, 0x7A);
    imx334_write_register(ViPipe, 0x3796, 0xA1);

    imx334_write_register(ViPipe, 0x3A01, 0x03);
    imx334_write_register(ViPipe, 0x3A18, 0xB7);
    imx334_write_register(ViPipe, 0x3A19, 0x00);
    imx334_write_register(ViPipe, 0x3A1A, 0x67);
    imx334_write_register(ViPipe, 0x3A1B, 0x00);

    imx334_write_register(ViPipe, 0x3A1C, 0x6F);
    imx334_write_register(ViPipe, 0x3A1D, 0x00);
    imx334_write_register(ViPipe, 0x3A1E, 0xDF);
    imx334_write_register(ViPipe, 0x3A1F, 0x01);
    imx334_write_register(ViPipe, 0x3A20, 0x6F);
    imx334_write_register(ViPipe, 0x3A21, 0x00);

    imx334_write_register(ViPipe, 0x3A22, 0xCF);
    imx334_write_register(ViPipe, 0x3A23, 0x00);
    imx334_write_register(ViPipe, 0x3A24, 0x6F);
    imx334_write_register(ViPipe, 0x3A25, 0x00);
    imx334_write_register(ViPipe, 0x3A26, 0xB7);
    imx334_write_register(ViPipe, 0x3A27, 0x00);
    imx334_write_register(ViPipe, 0x3A28, 0x5F);
    imx334_write_register(ViPipe, 0x3A29, 0x00);
    imx334_write_register(ViPipe, 0x3E04, 0x0E);

    imx334_write_register(ViPipe, 0x3078, 0x02);
    imx334_write_register(ViPipe, 0x3079, 0x00);
    imx334_write_register(ViPipe, 0x307A, 0x00);
    imx334_write_register(ViPipe, 0x307B, 0x00);

    imx334_write_register(ViPipe, 0x3081, 0x00);
    imx334_write_register(ViPipe, 0x3082, 0x00);
    imx334_write_register(ViPipe, 0x3083, 0x00);

    imx334_write_register(ViPipe, 0x3088, 0x02);
    imx334_write_register(ViPipe, 0x3094, 0x00);
    imx334_write_register(ViPipe, 0x3095, 0x00);
    imx334_write_register(ViPipe, 0x3096, 0x00);

    imx334_write_register(ViPipe, 0x309C, 0x00);
    imx334_write_register(ViPipe, 0x309D, 0x00);
    imx334_write_register(ViPipe, 0x309E, 0x00);
    imx334_write_register(ViPipe, 0x30A4, 0x00);
    imx334_write_register(ViPipe, 0x30A5, 0x00);

    // Sensor registers used for normal image
#if 1
    imx334_write_register(ViPipe, 0x304E, 0x00);
    imx334_write_register(ViPipe, 0x304F, 0x00);

    imx334_write_register(ViPipe, 0x3074, 0xB0);
    imx334_write_register(ViPipe, 0x3075, 0x00);

    imx334_write_register(ViPipe, 0x308E, 0xB1);
    imx334_write_register(ViPipe, 0x308F, 0x00);

    imx334_write_register(ViPipe, 0x30B6, 0x00);
    imx334_write_register(ViPipe, 0x30B7, 0x00);

    imx334_write_register(ViPipe, 0x3116, 0x00);
    imx334_write_register(ViPipe, 0x3080, 0x02);
    imx334_write_register(ViPipe, 0x309B, 0x02);
#endif

    imx334_default_reg_init(ViPipe);
    delay_ms(1);
    imx334_write_register(ViPipe, 0x3000, 0x00); // Standby Cancel
    delay_ms(20);
    imx334_write_register(ViPipe, 0x3002, 0x00);

    printf("===Imx334 8M30fps 12bit DOL 2t1 Init OK!===\n");
    return;
}
