/******************************************************************************
  Some simple Hisilicon Hi3559A system functions.

  Copyright (C), 2017, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2017-11 Created
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include "hi_type.h"
#include "osal_mmz.h"
#include "asm/io.h"
#include "hi_module_param.h"

#include "stdlib.h"
#include "fcntl.h"
#include "string.h"

//#define SUPPORT_PRORES

static unsigned long long mmz_start = 0x90000000;
static unsigned int mmz_size = 2816;   //M Byte

static unsigned long long map_mmz_start = 0x70000000;   //linux mmz start
static unsigned int map_mmz_size = 512;   //M Byte

static char* sensor_type0 = "imx334"; // imx477  imx290  imx299  imx334  imx277_slvs

extern HI_U32  vedu_en[VEDU_IP_NUM];

/******************************************************************/
#define himm(address, value)        writel(value, address)


//#VICAP default setting is VI
static void sensor_pin_mux(void)
{
    //#CLK_A CLK_B
    himm(0x1F000000, 0x000004f1);
    himm(0x1F000004, 0x000004f1);

    himm(0x1F000008, 0x000004f1);
    himm(0x1F00000C, 0x000004f1);

    himm(0x1F000010, 0x000004f1);
    himm(0x1F000014, 0x000004f1);

    himm(0x1F000018, 0x000004f1);
    himm(0x1F00001C, 0x000004f1);


    //#RST
    himm(0x1F000020, 0x000004f1);
    himm(0x1F000024, 0x000004f1);
    himm(0x1F000028, 0x000004f1);
    himm(0x1F00002C, 0x000004f1);
    //echo "============vicap_pin_mux done============="
}

static void slave_pin_mux(void)
{
    //#VS HS
    himm(0x1F000030, 0x000004f1);
    himm(0x1F000034, 0x000004f1);

    himm(0x1F000038, 0x000004f1);
    himm(0x1F00003C, 0x000004f1);

    himm(0x1F000040, 0x000004f1);
    himm(0x1F000044, 0x000004f1);

    himm(0x1F000048, 0x000004f1);
    himm(0x1F00004C, 0x000004f1);

    //#VSOUT
    himm(0x1F000050, 0x000004f1);
    himm(0x1F000054, 0x000004f1);
    himm(0x1F000058, 0x000004f1);
    himm(0x1F00005C, 0x000004f1);
    //echo "============slave_pin_mux done============="
}



//#i2c0 -> sensor
static void i2c0_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F000068, 0x000014f3);
    himm(0x1F00006C, 0x000014f3);

    //echo "============i2c0_pin_mux done============="
}

//#i2c1 -> sensor
static void i2c1_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F00007C, 0x000014f3);
    himm(0x1F000080, 0x000014f3);

    //echo "============i2c1_pin_mux done============="
}


//#i2c2 -> sensor
static void i2c2_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F000090, 0x000014f3);
    himm(0x1F000094, 0x000014f3);

    //echo "============i2c2_pin_mux done============="
}

//#i2c3 -> sensor
static void i2c3_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F0000A4, 0x000014f3);
    himm(0x1F0000A8, 0x000014f3);

    //echo "============i2c3_pin_mux done============="
}


//#i2c4 -> sensor
static void i2c4_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F000074, 0x000014f3);
    himm(0x1F000078, 0x000014f3);

    //echo "============i2c4_pin_mux done============="
}


//#i2c5 -> sensor
static void i2c5_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F000088, 0x000014f3);
    himm(0x1F00008C, 0x000014f3);

    //echo "============i2c5_pin_mux done============="
}

//#i2c6 -> sensor
static void i2c6_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F00009C, 0x000014f3);
    himm(0x1F0000A0, 0x000014f3);


    //echo "============i2c6_pin_mux done============="
}

//#i2c7 -> sensor
static void i2c7_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F0000B0, 0x000014f3);
    himm(0x1F0000B4, 0x000014f3);

    //echo "============i2c7_pin_mux done============="
}

#if 0
static void i2c8_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F0010C8, 0x000014f2);
    himm(0x1F0010CC, 0x000014f2);

    //echo "============i2c8_pin_mux done============="
}


static void i2c9_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F0010D0, 0x000014f2);
    himm(0x1F0010D4, 0x000014f2);

    //echo "============i2c9_pin_mux done============="
}


static void i2c10_pin_mux(void)
{
    //#SCL SDA
    himm(0x1F0010DC, 0x000014f2);
    himm(0x1F0010E0, 0x000014f2);

    //echo "============i2c10_pin_mux done============="
}
#endif

//#i2c11 -> 9022
static void i2c11_pin_mux(void)
{
    himm(0x1F0010F4, 0x00001C00);
    himm(0x1F0010F0, 0x00001C00);
    //echo "============i2c11_pin_mux done============="
}

static void spi0_pin_mux(void)
{
    himm(0x1F000068, 0x000004f1);
    himm(0x1F00006c, 0x000004f1);
    himm(0x1F000070, 0x000014f1);
    himm(0x1F000074, 0x000004f1);
    himm(0x1F000078, 0x000004f1);
    //echo "============spi0_pin_mux done============="
}

#if 0
static void spi1_pin_mux(void)
{
    himm(0x1F00007c, 0x000004f1);
    himm(0x1F000080, 0x000004f1);
    himm(0x1F000084, 0x000014f1);
    himm(0x1F000088, 0x000004f1);
    himm(0x1F00008c, 0x000004f1);
    //echo "============spi1_pin_mux done============="
}
#endif

static void vo_bt1120_mode(void)
{
    himm(0x1F001084, 0x000000f1);

    himm(0x1F001088, 0x000004f1);
    himm(0x1F00108C, 0x000004f1);
    himm(0x1F001090, 0x000004f1);
    himm(0x1F001094, 0x000004f1);
    himm(0x1F001098, 0x000004f1);
    himm(0x1F00109C, 0x000004f1);
    himm(0x1F0010A0, 0x000004f1);
    himm(0x1F0010A4, 0x000004f1);
    himm(0x1F0010A8, 0x000004f1);
    himm(0x1F0010AC, 0x000004f1);
    himm(0x1F0010B0, 0x000004f1);
    himm(0x1F0010B4, 0x000004f1);
    himm(0x1F0010B8, 0x000004f1);
    himm(0x1F0010BC, 0x000004f1);
    himm(0x1F0010C0, 0x000004f1);
    himm(0x1F0010C4, 0x000004f1);

    //echo "============vo_output_mode done============="
}

#if 0
static void vo_lcd_mode(void)
{
    himm(0x1F0010EC, 0x000004f2);
    himm(0x1F0010A4, 0x000004f2);
    himm(0x1F00109C, 0x000004f2);
    himm(0x1F0010E4, 0x000004f2);
    himm(0x1F001098, 0x000004f2);
    himm(0x1F001094, 0x000004f2);
    himm(0x1F001090, 0x000004f2);
    himm(0x1F00108C, 0x000004f2);
    himm(0x1F001088, 0x000004f2);
    himm(0x1F0010C4, 0x000004f2);
    himm(0x1F0010C0, 0x000004f2);
    himm(0x1F0010A8, 0x000004f2);
    himm(0x1F0010B8, 0x000004f2);
    himm(0x1F0010B4, 0x000004f2);
    himm(0x1F0010B0, 0x000004f2);
    himm(0x1F0010AC, 0x000004f2);
    himm(0x1F001008, 0x000004f2);
    himm(0x1F00100C, 0x000004f2);
    himm(0x1F001010, 0x000004f2);
    himm(0x1F001000, 0x000004f2);
    himm(0x1F001014, 0x000004f2);
    himm(0x1F001038, 0x000004f2);
    himm(0x1F00101C, 0x000004f2);
    himm(0x1F001030, 0x000004f2);
    himm(0x1F001020, 0x000004f2);
    himm(0x1F001024, 0x000004f2);
    himm(0x1F001028, 0x000004f2);
    himm(0x1F00102C, 0x000004f2);

    //echo "============vo_output_mode done============="
}

static void vo_lcd8bit_mode(void)
{
    //#lcd8bit
    himm(0x1f001088, 0x4f2);
    himm(0x1f00108c, 0x4f2);
    himm(0x1f001090, 0x4f2);
    himm(0x1f001094, 0x4f2);
    himm(0x1f001098, 0x4f2);
    himm(0x1f00109c, 0x4f2);
    himm(0x1f0010a4, 0x4f2);
    himm(0x1f0010a8, 0x4f2);
    himm(0x1f0010c0, 0x4f2);
    himm(0x1f0010c4, 0x4f2);
    himm(0x1f0010e4, 0x4f2);
    himm(0x1f0010ec, 0x4f2);
    //echo "============vo_lcd8bit_mode done============="
}

static void spi4_pin_mux(void)
{
    himm(0x1f0010c8, 0x1);       //#(GPIO16_0,SPI4_SCLK,     I2C8_SCL)  = (0x0,0x1,0x2)
    himm(0x1f0010cc, 0x1);       //#(GPIO16_1,SPI4_SDO ,     I2C8_SCA)  = (0x0,0x1,0x2)
    himm(0x1f0010d0, 0x1);       //#(GPIO16_2,SPI4_SDI ,     I2C9_SCL)  = (0x0,0x1,0x2)
    himm(0x1f0010d4, 0x1);       //#(GPIO16_3,SPI4_CSN0,     I2C9_SDA)  = (0x0,0x1,0x2)
    himm(0x1f0010d8, 0x1);       //#(GPIO16_4,SPI4_CSN1)                = (0x0,0x1)
    himm(0x1f0010dc, 0x1);       //#(GPIO16_5,SPI4_CSN2,     I2C10_SCL) = (0x0,0x1,0x2)
    himm(0x1f0010e0, 0x1);       //#(GPIO16_6,SPI4_CSN3,     I2C10_SDA) = (0x0,0x1,0x2)
    //echo "============spi4_pin_mux done============="
}
#endif

static void vo_mipi_tx_mode(void)
{
    //#mipi_tx
    himm(0x1F001088 ,0x0);
    himm(0x12146004 ,0x1);
    himm(0x12146400 ,0x1);
    himm(0x12140010 ,0x4);
    himm(0x12140400 ,0x4);

    //echo( "============vo_mipi_tx_mode done=============\n");
}


static void hmdi_pin_mode(void)
{
    himm(0x1F0000C4, 0x00001400);
    himm(0x1F0000C8, 0x00001400);
    himm(0x1F0000CC, 0x00001400);
    himm(0x1F0000D0, 0x00001400);
}

static void i2s_pin_mux(void)
{
    himm(0x1F0000FC, 0x00000ef2);
    himm(0x1F000100, 0x00001ef2);
    himm(0x1F000104, 0x00001ef2);
    himm(0x1F000108, 0x00000ef2);
    himm(0x1F00010C, 0x00001ef2);

    //echo "============i2s_pin_mux done============="
}

static void bt1120_pin_mux(void)
{
    himm(0x1F001084 ,0x000014f3);
    himm(0x1F0010A4 ,0x000014f3);
    himm(0x1F0010A0 ,0x000014f3);
    himm(0x1F00109c ,0x000014f3);
    himm(0x1F001098 ,0x000014f3);
    himm(0x1F001094 ,0x000014f3);
    himm(0x1F001090 ,0x000014f3);
    himm(0x1F00108c ,0x000014f3);
    himm(0x1F001088 ,0x000014f3);
    himm(0x1F0010c4 ,0x000014f3);
    himm(0x1F0010c0 ,0x000014f3);
    himm(0x1F0010bc ,0x000014f3);
    himm(0x1F0010b8 ,0x000014f3);
    himm(0x1F0010b4 ,0x000014f3);
    himm(0x1F0010b0 ,0x000014f3);
    himm(0x1F0010ac ,0x000014f3);
    himm(0x1F0010a8 ,0x000014f3);
    //echo "============bt1120_pin_mux done============="
}

static void pinmux_cfg(void)
{
    sensor_pin_mux();
    slave_pin_mux();

    //#sensor
    i2c0_pin_mux();
    i2c1_pin_mux();
    i2c2_pin_mux();
    i2c3_pin_mux();
    i2c4_pin_mux();
    i2c5_pin_mux();
    i2c6_pin_mux();
    i2c7_pin_mux();


    //#9022
    i2c11_pin_mux();

    //#bt1120
    vo_bt1120_mode();

    //#lcd_24bit
    //vo_lcd_mode()

    //#lcd_8bit
    //spi4_pin_mux()
    //vo_lcd8bit_mode()

    // mipi_tx
    // vo_mipi_tx_mode();

    hmdi_pin_mode();

    i2s_pin_mux();
}

//# open module clock while you need it!
static void clk_cfg(void)
{
    himm(0x12010104, 0x006db6db);  //#MIPI_RX 600M
    himm(0x12010108, 0x006db6db);  //#ISP 600M
    himm(0x1201010c, 0x000db030);  //#(VI_PPC VIPROC0/ offline) 600M
    himm(0x12010100, 0x00000000);  //#VICAP VIPROC ISP LOW-POWER clock

    himm(0x1201011c, 0x00002017);
    himm(0x1201011c, 0x00000011);  //# AIAO(50M 1188M)and MIPITX clk and reset

    himm(0x12010120, 0x2815e4c3);  //# AIAO&LCD clk and reset

    himm(0x12010164, 0x00fdbfff);  //#(VEDU 710M; JPGE 750M;  VPSS GDC AVS 600M;  VGS 631M)
    himm(0x12010168, 0x00000005);  //#VPSS offline(IVE 750M;VDH 568M  GME 600M)


    //echo "clock configure operation done!"
}

//# set MISC_CTRL44, AVSP VDH VEDU2 VPSS1 VIPROC1 power up
static void pwr_en(void)
{
    himm(0x120300ac, 0x33333332);
    himm(0x120300ac, 0x22222222);
    himm(0x120300b0, 0x00033333);
    himm(0x120300b0, 0x00022222);
}

static void clkcfg(void)
{
    clk_cfg();
    pwr_en();
}

static void vi_vpss_online_config(void)
{
    //# -------------vi vpss online open
#if 0
    echo "==============vi_vcap_online==============";
#each module 3bit:           30:28           26:24           22:20          18:16        14:12         10:8         6:4         2:0
himm 0x12032000  0x00000011    #each module 3bit:
    SPACC             FMC           EDMA0          EDMA1          UFS         EMMC         GSF1         GSF0
himm 0x12032004  0x00454545    #each module 3bit:
    vedu2(W)        vedu2(R)     vedu1(W)     vedu1(R)     vedu0(W)     vedu0(R)
himm 0x12032008  0x34344545    #each module 3bit:
    vgs1(W)         vgs1(R)          vgs0(W)         vgs0(R)     vpss1(W)     vpss1(R)    vpss0 (W)     vpss0(R)
himm 0x1203200c  0x55554545    #each module 3bit:
    gdc1(W)         gdc1(R)          gdc0(W)         gdc0(R)     jpge1(W)     jpge1(R)     jpge0(W)     jpge0(R)
himm 0x12032010  0x55003333    #each module 3bit:
    gme(W)          gme(R)          gzip(W)         gzip(R)      jpgd(W)      jpgd(R)       pgd(W)       pgd(R)
himm 0x12032014  0x67676767    #each module 3bit:
    viproc1(W)      viproc1(R)       viproc0(W)      viproc0(R)     vi_m1(W)     vi_m1(R)     vi_m0(W)     vi_m0(R)
himm 0x12032018  0x33666767    #each module 3bit:
    gpu(W)          gpu(R)           aio(W)          aio(R)      vdp1(W)      vdp1(R)      vdp0(W)      vdp0(R)
himm 0x1203201c  0x00000000    #each module 3bit:
    sdio2(W)        sdio2(R)     sdio1(W)     sdio1(R)     sdio0(W)     sdio0(R)
himm 0x12032020  0x12333333    #each module 3bit:
    fd(W)           fd(R)           dpu(W)          dpu(R)      cnn1(W)      cnn1(R)      cnn0(W)      cnn0(R)
himm 0x12032024  0x33333333    #each module 3bit:
    dsp1_idma(W)    dsp1_idma(R)        dsp1_m(W)       dsp1_m(R) dsp0_idma(W) dsp0_idma(R)    dsp0_m(W)    dsp0_m(R)
himm 0x12032028  0x33333333    #each module 3bit:
    dsp3_idma(W)    dsp3_idma(R)        dsp3_m(W)       dsp3_m(R) dsp2_idma(W) dsp2_idma(R)    dsp2_m(W)    dsp2_m(R)
himm 0x1203202c  0x00444444    #each module 3bit:
    pcie(W)         pcie(R)        avs_m2(W)       avs_m2(R)    avs_m1(W)    avs_m1(R)    avs_m0(W)    avs_m0(R)
himm 0x12032030  0x55552211    #each module 3bit:
    peri(W)         peri(R)           a53(W)          a53(R)       ive(W)       ive(R)       TDE(W)      TDE(R)
himm 0x12032034  0x55550000    #each module 3bit:
    cpu1(W)         cpu1(R)          cpu0(W)         cpu0(R)     ddrt1(W)     ddrt1(R)     ddrt0(W)     ddrt0(R)
himm 0x12032038  0x77776666    #each module 3bit:
    usb3p1(W)       usb3p1(R)        usb3p0(W)       usb3p0(R)      vdh1(W)      vdh1(R)      vdh0(W)      vdh0(R)
#else
    himm(0x12032000, 0x00000011);
    himm(0x12032004, 0x00454545);
    himm(0x12032008, 0x44444545);
    himm(0x1203200c, 0x55554545);
    himm(0x12032010, 0x55003333);
    himm(0x12032014, 0x67677777);
    himm(0x12032018, 0x33666767);
    himm(0x1203201c, 0x00000000);
    himm(0x12032020, 0x11333333);
    himm(0x12032024, 0x33333333);
    himm(0x12032028, 0x33333333);
    himm(0x1203202c, 0x00444444);
    himm(0x12032030, 0x55552211);
    himm(0x12032034, 0x55550000);
    himm(0x12032038, 0x77776666);
#endif
}

static void amp_unmute(void)
{
    himm(0x1F002050, 0x00000cf0);
    himm(0x1214E200, 0x80);
    himm(0x1214E400, 0x80);
    himm(0x1214E200, 0x80);
}

static void qosbuf(void)
{
    himm(0x12064000, 0x2);
    himm(0x1206410c, 0x17);
    himm(0x12064110, 0x17);
    himm(0x12064088, 0x5);
    himm(0x1206408c, 0x92821e14);
    himm(0x12064090, 0x92821e14);
    himm(0x12064068, 0x51);
    himm(0x1206406c, 0x51);
    himm(0x120640ac, 0x80);
    himm(0x120640ec, 0x11);
    himm(0x120640f0, 0x1111);
    himm(0x120640f4, 0x33);
    himm(0x120641f0, 0x1);
    himm(0x120641f4, 0x0);
    himm(0x120641f8, 0x00800002);
}

static void sys_ctl(void)
{
    amp_unmute();
    qosbuf();
    vi_vpss_online_config();
}

static HI_VOID SYS_cfg(void)
{
    pinmux_cfg();
    clkcfg();
    sys_ctl();
}

static HI_S32 MMZ_init(void)
{
    extern int media_mem_init(void * pArgs);
    MMZ_MODULE_PARAMS_S stMMZ_Param;

    snprintf(stMMZ_Param.mmz, MMZ_SETUP_CMDLINE_LEN, "anonymous,0,0x%llx,%dM", mmz_start, mmz_size);
    snprintf(stMMZ_Param.map_mmz, MMZ_SETUP_CMDLINE_LEN, "0x%llx,%dM", map_mmz_start, map_mmz_size);
    stMMZ_Param.anony = 1;

    printf("mmz_start=0x%llx, mmz_size=0x%x\n", mmz_start, mmz_size);
    printf("mmz param= %s\n", stMMZ_Param.mmz);

    return media_mem_init(&stMMZ_Param);
}

static HI_S32 BASE_init(void)
{
    extern int base_mod_init(void);
    HI_U32 vb_force_exit;
    vb_force_exit = 0;

    return base_mod_init();
}

static HI_S32 SYS_init(void)
{
    extern int sys_mod_init(void);

    return sys_mod_init();
}

static HI_S32 RGN_init(void)
{
    extern int rgn_mod_init(void);

    return rgn_mod_init();
}

static HI_S32 ISP_init(void)
{
    extern int isp_mod_init(void);

    return isp_mod_init();
}

static HI_S32 VI_init(void)
{
    extern int vi_mod_init(void);

    return vi_mod_init();
}

static HI_S32 DIS_init(void)
{
    extern int dis_mod_init(void);

    return dis_mod_init();
}

static HI_S32 GDC_init(void)
{
    extern int gdc_mod_init(void* pArgs);

    GDC_MODULE_PARAMS_S stGdcModuleParam;
    stGdcModuleParam.u32MaxGdcJob   = 64;
    stGdcModuleParam.u32MaxGdcTask  = 100;
    stGdcModuleParam.u32MaxGdcNode  = 100;
    stGdcModuleParam.au32GdcEn[0]    = 1;
    stGdcModuleParam.au32GdcEn[1]    = 1;

    return gdc_mod_init(&stGdcModuleParam);
}

static HI_S32 VGS_init(void)
{
    extern void vgs_get_module_param(void *pArgs);
    extern int vgs_mod_init(void *pArgs);

    VGS_MODULE_PARAMS_S stVgsModuleParam;

    vgs_get_module_param((void *)&stVgsModuleParam);
    stVgsModuleParam.u32MaxVgsJob  = 64;
    stVgsModuleParam.u32MaxVgsTask = 100;
    stVgsModuleParam.u32MaxVgsNode = 100;
    stVgsModuleParam.au32VgsEn[0]  = 1;
    stVgsModuleParam.au32VgsEn[1]  = 1;
    stVgsModuleParam.bVgsHdrSupport = 0;

    return vgs_mod_init(&stVgsModuleParam);
}

static HI_S32 VPSS_init(void)
{
    extern int vpss_mod_init(void *pArgs);
    VPSS_MODULE_PARAMS_S stVpssModuleParam;
    stVpssModuleParam.u32VpssEn[0] = 1;
    stVpssModuleParam.u32VpssEn[1] = 1;

    return vpss_mod_init(&stVpssModuleParam);
}


static HI_S32 AVS_init(void)
{
    extern int avs_module_init(void);

    return avs_module_init();
}

static HI_S32 VO_init(void)
{
    extern int vou_module_init(void);

    return vou_module_init();
}

//NO tde&hifb on LiteOS
#if 0
static HI_S32 TDE_init(void)
{
    return tde_mod_init();
}

static HI_S32 HIFB_init(void)
{
    HIFB_MODULE_PARAMS_S stHIFB_Param;
    snprintf(stHIFB_Param.video, 64, "hifb:vram0_size:32400,vram1_size:16200,vram2_size:256");
    return hifb_init(&stHIFB_Param);
}
#endif

static HI_S32 CHNL_init(void)
{
    extern int chnl_mod_init(void);

    return chnl_mod_init();
}

static HI_S32 VEDU_init(void)
{
    extern int vedu_mod_init(void);

    return vedu_mod_init();
}

static HI_S32 RC_init(void)
{
    extern int  rc_mod_init(void);

    return rc_mod_init();
}

static HI_S32 VENC_init(void)
{
    extern int  venc_mod_init(void * pArgs);

    return venc_mod_init(NULL);
}

static HI_S32 H264e_init(void)
{
    extern int h264e_mod_init(void * pArgs);

    return h264e_mod_init(NULL);
}

static HI_S32 H265e_init(void)
{
    extern int h265e_mod_init(void * pArgs);

    return h265e_mod_init(NULL);
}

static HI_S32 JPEGE_init(void)
{
    extern int jpege_mod_init(void * pArgs);

    return jpege_mod_init(NULL);
}

#ifdef SUPPORT_PRORES
static HI_S32 PRORES_init(void)
{
    return prores_mod_init();
}
#endif

static HI_S32 JPEGD_init(void)
{
    extern int jpegd_mod_init(void);

    return jpegd_mod_init();
}


static HI_S32 VFMW_init(void)
{
    extern int vfmw_mod_init(void *pArgs);

    VFMW_MODULE_PARAMS_S stVfmwModuleParam;
    stVfmwModuleParam.s32VfmwMaxChnNum = VDEC_MAX_CHN_NUM;

    return vfmw_mod_init(&stVfmwModuleParam);
}

static HI_S32 VDEC_init(void)
{
    extern int vdec_mod_init(void *pArgs);

    VDEC_MODULE_PARAMS_S  stVdecModuleParam;
    stVdecModuleParam.u32VdecMaxChnNum = VDEC_MAX_CHN_NUM;

    return vdec_mod_init(&stVdecModuleParam);
}


static HI_S32 DPU_RECT_init(void)
{
    extern int dpu_rect_mod_init(void);

    return dpu_rect_mod_init();
}

static HI_S32 DPU_MATCH_init(void)
{
    extern int dpu_match_mod_init(void);

    return dpu_match_mod_init();
}

static HI_VOID sensor_config(void)
{
    if (!strcmp(sensor_type0, "imx477"))
    {
        himm(0x12030098, 0x0);
        himm(0x1203009c, 0x0);

        himm(0x12010114, 0x74747474);
        himm(0x12010114, 0x14141414);
    }
    else if(!strcmp(sensor_type0, "imx290"))
    {
        himm(0x12030098, 0x0);
        himm(0x1203009c, 0x0);
        himm(0x12010114, 0x78787878);
        himm(0x12010114, 0x18181818);
    }
    else if(!strcmp(sensor_type0, "imx290_slave"))
    {
        himm(0x12030098, 0x0);
        himm(0x1203009c, 0x0);
        himm(0x12010114, 0x78787878);
        himm(0x12010114, 0x10101010);
    }
    else if(!strcmp(sensor_type0, "imx299"))
    {
        i2c0_pin_mux();
        himm(0x12030098, 0x0);
        himm(0x1203009c, 0x0);
        himm(0x12010114, 0x74747474); // 0x71717171
        himm(0x12010114, 0x14141414); // 0x11111111
    }
    else if(!strcmp(sensor_type0, "imx334"))
    {
        himm(0x12030098, 0x0);
        himm(0x1203009c, 0x0);
        himm(0x12010114, 0x74747474);
        himm(0x12010114, 0x14141414);
    }
    else if(!strcmp(sensor_type0, "imx277_slvs"))
    {
        spi0_pin_mux();
        himm(0x12030098, 0x0);
        himm(0x1203009c, 0x0);
        himm(0x12010114, 0x71717171);
	    himm(0x12010114, 0x11111111);
    }
    else if(!strcmp(sensor_type0, "bt1120"))
    {
        bt1120_pin_mux();
        himm(0x12010104, 0x00c28e00);
    }
}

static HI_S32 hi_sensor_spi_init(void)
{
    extern int sensor_spi_dev_init(void *pArgs);

    return sensor_spi_dev_init(NULL);
}

static HI_S32 hi_sensor_i2c_init(void)
{
    extern int hi_dev_init(void);

    return hi_dev_init();
}

static HI_S32 PWM_init(void)
{
    extern int pwm_init(void);

    return pwm_init();
}

static HI_S32 PIRIS_init(void)
{
    extern int piris_init(void);

    return piris_init();
}

static HI_S32 HDMI_init(void)
{
    extern int hdmi_mod_init(void);

    return hdmi_mod_init();
}

static HI_S32 MIPI_init(void)
{
    extern int mipi_mod_init(void);

    return mipi_mod_init();
}

static HI_S32 MIPITX_init(void)
{
    extern int mipi_tx_module_init(void);

    return mipi_tx_module_init();
}

static HI_S32 HI_USER_init(void)
{
    extern int hi_user_init(void);
    return hi_user_init();
}
static HI_S32 AcodecMod_init(void)
{
    extern int acodec_mod_init(void * pArgs);

    return acodec_mod_init(NULL);
}

static HI_S32 AiaoMod_init(void)
{
    extern int aiao_mod_init(void);

    return aiao_mod_init();
}

static HI_S32 AiMod_init(void)
{
    extern int  ai_mod_init(void);

    return ai_mod_init();
}

static HI_S32 AoMod_init(void)
{
    extern int ao_mod_init(void);

    return ao_mod_init();
}

static HI_S32 AencMod_init(void)
{
    extern int  aenc_mod_init(void);

    return aenc_mod_init();
}

static HI_S32 AdecMod_init(void)
{
    extern int adec_mod_init(void);

    return adec_mod_init();
}

//static HI_S32 Motionfusion_init(void)
//{
//    extern int motionfusion_mod_init(void);
//
//    return motionfusion_mod_init();
//}


static void insert_audio(void)
{
    HI_S32 ret = 0;

    ret = AiaoMod_init();
    if (ret != 0)
    {
        printf("aiao init error.\n");
    }
    ret = AiMod_init();
    if (ret != 0)
    {
        printf("ai init error.\n");
    }
    ret = AoMod_init();
    if (ret != 0)
    {
        printf("ao init error.\n");
    }
    ret = AencMod_init();
    if (ret != 0)
    {
        printf("aenc init error.\n");
    }
    ret = AdecMod_init();
    if (ret != 0)
    {
        printf("adec init error.\n");
    }

    ret = AcodecMod_init();
    if (ret != 0)
    {
        printf("acodec init error.\n");
    }
}
extern void osal_proc_init(void);

void SDK_init(void)
{
    HI_S32        ret = 0;

    SYS_cfg();

    osal_proc_init();

    ret = MMZ_init();
    if (ret != 0)
    {
        printf("MMZ init error.\n");
    }

    ret = BASE_init();
    if (ret != 0)
    {
        printf("base init error.\n");
    }

    ret = SYS_init();
    if (ret != 0)
    {
        printf("sys init error.\n");
    }

    ret = RGN_init();
    if (ret != 0)
    {
        printf("rgn init error.\n");
    }

    ret = GDC_init();
    if (ret != 0)
    {
        printf("gdc init error.\n");
    }

    ret = VGS_init();
    if (ret != 0)
    {
        printf("vgs init error.\n");
    }

    ret = DIS_init();
    if (ret != 0)
    {
        printf("vgs init error.\n");
    }



    ret = VI_init();
    if (ret != 0)
    {
        printf("vi init error.\n");
    }
    ret = ISP_init();
    if (ret != 0)
    {
        printf("isp init error.\n");
    }

    ret = VPSS_init();
    if(ret != 0)
    {
        printf("vpss init error.\n");
    }

    ret = AVS_init();
    if(ret != 0)
    {
        printf("avs init error.\n");
    }

    ret = VO_init();
    if (ret != 0)
    {
        printf("vo init error.\n");
    }

#if 0
    ret = TDE_init();
    if (ret != 0)
    {
        printf("tde init error.\n");
    }

    ret = HIFB_init();
    if (ret != 0)
    {
        printf("hifb init error.\n");
    }
#endif

    ret = RC_init();
    if (ret != 0)
    {
        printf("rc init error.\n");
    }

    ret = VENC_init();
    if (ret != 0)
    {
        printf("venc init error.\n");
    }

    ret = CHNL_init();
    if (ret != 0)
    {
        printf("chnl init error.\n");
    }

    ret = VEDU_init();
    if (ret != 0)
    {
        printf("vedu init error.\n");
    }

    ret = H264e_init();
    if (ret != 0)
    {
        printf("H264e init error.\n");
    }

    ret = H265e_init();
    if (ret != 0)
    {
        printf("H265e init error.\n");
    }

    ret = JPEGE_init();
    if (ret != 0)
    {
        printf("jpege init error.\n");
    }

#ifdef SUPPORT_PRORES
    ret = PRORES_init();
    if (ret != 0)
    {
        printf("prores init error.\n");
    }
#endif

    ret = JPEGD_init();
    if (ret != 0)
    {
        printf("jpegd init error.\n");
    }

    ret = VFMW_init();
    if (ret != 0)
    {
        printf("vfmw init error.\n");
    }
    ret = VDEC_init();
    if (ret != 0)
    {
        printf("vdec init error.\n");
    }

    ret = DPU_RECT_init();
    if (ret != 0)
    {
        printf("dpu rect init error.\n");
    }

    ret = DPU_MATCH_init();
    if (ret != 0)
    {
        printf("dpu match init error.\n");
    }

    insert_audio();

    ret = PWM_init();
    if(ret != 0)
    {
        printf("pwm init error.\n");
    }

    ret = PIRIS_init();
    if(ret != 0)
    {
        printf("piris init error.\n");
    }

    ret = hi_sensor_i2c_init();
    if(ret != 0)
    {
        printf("sensor i2c init error.\n");
    }

    ret = hi_sensor_spi_init();
    if(ret != 0)
    {
        printf("sensor spi init error.\n");
    }

    ret = HDMI_init();
    if(ret != 0)
    {
        printf("hdmi init error.\n");
    }

    sensor_config();

    ret = MIPI_init();
    if (ret != 0)
    {
        printf("mipi init error.\n");
    }

    ret = MIPITX_init();
    if (ret != 0)
    {
        printf("mipiTx init error.\n");
    }

    //ret = Motionfusion_init();
    //if (ret != 0)
    //{
    //    printf("Motionfusion init error.\n");
    //}

    ret = HI_USER_init();
    if (ret != 0)
    {
        printf("hi user init error.\n");
    }

    printf("SDK init ok...\n");
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

