/******************************************************************************
  Some simple Hisilicon Hi3516A system functions.

  Copyright (C), 2017-2018, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2018-1 Created
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include "hi_type.h"

#include "osal_mmz.h"

//#define SUPPORT_PRORES

static void BASE_exit(void)
{
    extern void base_mod_exit(void);

    base_mod_exit();
}

static void MMZ_exit(void)
{
    extern void media_mem_exit(void);
    media_mem_exit();
}
static void SYS_exit(void)
{
    extern void sys_mod_exit(void);

    sys_mod_exit();
}

static void ISP_exit(void)
{
    extern void isp_mod_exit(void);

    return isp_mod_exit();
}

static void VI_exit(void)
{
    extern void vi_mod_exit(void);

    return vi_mod_exit();
}

static void RGN_exit(void)
{
    extern void rgn_mod_exit(void);

    return rgn_mod_exit();
}

static void GDC_exit(void)
{
    extern void gdc_mod_exit(void);

    return gdc_mod_exit();
}

static void DIS_exit(void)
{
    extern void dis_mod_exit(void);

    return dis_mod_exit();
}

static void VGS_exit(void)
{
    extern void vgs_mod_exit(void);

    return vgs_mod_exit();
}

static void VPSS_exit(void)
{
    extern void vpss_mod_exit(void);

    return vpss_mod_exit();
}

static void AVS_exit(void)
{
    extern void avs_module_exit(void);

    return avs_module_exit();
}

static void VO_exit(void)
{
    extern void vou_module_exit(void);

    return vou_module_exit();
}

//NO tde&hifb on LiteOS
#if 0
static void TDE_exit(void)
{
    return tde_mod_exit();
}

static void HIFB_exit(void)
{
    extern HI_VOID hifb_cleanup(HI_VOID);

    hifb_cleanup();
}
#endif

static void HDMI_exit(void)
{
    extern void hdmi_mod_exit(void);

    return hdmi_mod_exit();
}

static void MIPI_exit(void)
{
    extern void mipi_mod_exit(void);

    return mipi_mod_exit();
}

static void MIPITX_exit(void)
{
    extern void mipi_tx_module_exit(void);

    return mipi_tx_module_exit();
}

static void RC_exit(void)
{
    extern void rc_mod_exit(void);

    return rc_mod_exit();
}

static void VENC_exit(void)
{
    extern void  venc_mod_exit(void);

    return venc_mod_exit();
}

static void CHNL_exit(void)
{
    extern void chnl_mod_exit(void);

    return chnl_mod_exit();
}

static void VEDU_exit(void)
{
    extern void  vedu_mod_exit(void);

    return vedu_mod_exit();
}

static void H264e_exit(void)
{
    extern void  h264e_mod_exit(void);

    return h264e_mod_exit();
}

static void H265e_exit(void)
{
    extern void h265e_mod_exit(void);

    return h265e_mod_exit();
}

static void JPEGE_exit(void)
{
    extern void  jpege_mod_exit(void);

    return jpege_mod_exit();
}

#ifdef SUPPORT_PRORES
static void PRORES_exit(void)
{
    extern void  prores_mod_exit(void);

    return prores_mod_exit();
}
#endif
static void PWM_exit(void)
{
    extern void pwm_exit(void);

    return pwm_exit();
}

static void PIRIS_exit(void)
{
    extern void piris_exit(void);

    return piris_exit();
}

static void hi_sensor_spi_exit(void)
{
    extern void sensor_spi_dev_exit(void);

    return sensor_spi_dev_exit();
}

static void hi_sensor_i2c_exit(void)
{
    extern void hi_dev_exit(void);

    return hi_dev_exit();
}

static void JPEGD_exit(void)
{
    extern void jpegd_mod_exit(void);

    return jpegd_mod_exit();
}

static void VFMW_exit(void)
{
    extern void vfmw_mod_exit(void);

    return vfmw_mod_exit();
}

static void VDEC_exit(void)
{
    extern void vdec_mod_exit(void);

    return vdec_mod_exit();
}

static void DPU_RECT_exit(void)
{
    extern void dpu_rect_mod_exit(void);

    return dpu_rect_mod_exit();
}

static void DPU_MATCH_exit(void)
{
    extern void dpu_match_mod_exit(void);

    return dpu_match_mod_exit();
}

//static void MOTIONFUSION_exit(void)
//{
//    extern void motionfusion_mod_exit(void);
//
//    return motionfusion_mod_exit();
//}

static void HI_USER_exit(void)
{
    extern int hi_user_exit(void);
    hi_user_exit();
}

extern void osal_proc_exit(void);

HI_VOID SDK_exit(void)
{
    MIPI_exit();
    MIPITX_exit();

    JPEGD_exit();
    VFMW_exit();
    DPU_MATCH_exit();
    DPU_RECT_exit();
    VDEC_exit();

    RC_exit();
#ifdef SUPPORT_PRORES
    PRORES_exit();
#endif
    JPEGE_exit();
    H264e_exit();
    H265e_exit();
    VENC_exit();
    VEDU_exit();
    CHNL_exit();

    VO_exit();
    AVS_exit();
    VPSS_exit();
    ISP_exit();
    VI_exit();
    GDC_exit();
    DIS_exit();
    VGS_exit();
    RGN_exit();

    HDMI_exit();
    hi_sensor_i2c_exit();
    hi_sensor_spi_exit();
    PWM_exit();
    PIRIS_exit();

    SYS_exit();
    BASE_exit();
    MMZ_exit();
    //MOTIONFUSION_exit();
    osal_proc_exit();
    HI_USER_exit();
    printf("SDK exit ok...\n");
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


