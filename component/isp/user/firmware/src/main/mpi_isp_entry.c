/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include <stdio.h>
#include <string.h>

#include <sys/ioctl.h>
#include <signal.h>

#include "mpi_sys.h"
#include "mkp_isp.h"
#include "hi_isp_debug.h"
#include "isp_debug.h"
#include "isp_defaults.h"
#include "isp_main.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_statistics.h"
#include "isp_regcfg.h"
#include "isp_proc.h"
#include "isp_vreg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "mpi_isp_adapt.c"
#include "hi_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* Isp Version Proc will be Hi3518_ISP_V1.0.4.x, ISP_LIB_VERSION stands for x */
#define ISP_LIB_VERSION   "0"    // [0~F]

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
isp_usr_ctx  g_astIspCtx[ISP_MAX_PIPE_NUM] = {{0}};
HI_S32     g_as32IspFd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

HI_U16     g_au16ResWMax[ISP_MAX_PIPE_NUM] = {ISP_RES_WIDTH_MAX};
HI_U16     g_au16ResHMax[ISP_MAX_PIPE_NUM] = {ISP_RES_HEIGHT_MAX};

static ISP_VERSION_S gs_stVersion = {
    .au8MppVersion = ISP_LIB_VERSION,
    .u32Magic = 0,
};

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/

#define ISP_CHECK_SENSOR_REGISTER(dev)\
    do {\
        if ((g_astIspCtx[dev].sns_reg != HI_TRUE) && (g_astIspCtx[dev].isp_yuv_mode != HI_TRUE))\
        {\
            ISP_ERR_TRACE("Sensor doesn't register to ISP[%d]!\n", dev);\
            return HI_ERR_ISP_SNS_UNREGISTER;\
        }\
    }while(0)

#define ISP_VERSION_MAGIC 20130305

/****************************************************************************
 * static functions
 ****************************************************************************/

HI_S32 ISP_CheckDevOpen(ISP_DEV dev)
{
    if (g_as32IspFd[dev] <= 0) {
        HI_U32 arg = (dev);
        g_as32IspFd[dev] = open("/dev/isp_dev", O_RDONLY, S_IRUSR);
        if (g_as32IspFd[dev] < 0) {
            perror("open isp device error!\n");
            return HI_ERR_ISP_NOT_INIT;
        }
        if (ioctl(g_as32IspFd[dev], ISP_DEV_SET_FD, &arg)) {
            close(g_as32IspFd[dev]);
            g_as32IspFd[dev] = -1;
            return HI_ERR_ISP_NOT_INIT;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_CheckMemInit(ISP_DEV dev)
{
    if (g_astIspCtx[dev].mem_init != HI_TRUE) {
        if (ioctl(g_as32IspFd[dev], ISP_MEM_INFO_GET, &g_astIspCtx[dev].mem_init)) {
            ISP_ERR_TRACE("ISP[%d] get Mem info failed!\n", dev);
            return HI_ERR_ISP_MEM_NOT_INIT;
        }
        if (g_astIspCtx[dev].mem_init != HI_TRUE) {
            ISP_ERR_TRACE("ISP[%d] Mem NOT Init %d!\n", dev, g_astIspCtx[dev].mem_init);
            return HI_ERR_ISP_MEM_NOT_INIT;
        }
    }

    return HI_SUCCESS;
}

static HI_BOOL ISP_GetVersion(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_VERSION, &gs_stVersion);
    if (s32Ret) {
        ISP_ERR_TRACE("register ISP[%d] lib info ec %x!\n", ViPipe, s32Ret);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_S32 ISP_SetSnsSlaveUnSyncAttr(SLAVE_DEV SlaveDev, const ISP_SLAVE_SNS_SYNC_S *pstSnsSync)
{
    SLAVE_CHECK_DEV(SlaveDev);
    ISP_CHECK_POINTER(pstSnsSync);

    hi_isp_slave_mode_time_cfg_select_write(SlaveDev, pstSnsSync->u32SlaveModeTime);
    hi_isp_slave_mode_configs_write(SlaveDev, pstSnsSync->unCfg.u32Bytes);
    hi_isp_slave_mode_vstime_low_write(SlaveDev, pstSnsSync->u32VsTime);
    hi_isp_slave_mode_vstime_high_write(SlaveDev, 0);
    hi_isp_slave_mode_hstime_write(SlaveDev, pstSnsSync->u32HsTime);
    hi_isp_slave_mode_vscyc_low_write(SlaveDev, pstSnsSync->u32VsCyc);
    hi_isp_slave_mode_vscyc_high_write(SlaveDev, 0);
    hi_isp_slave_mode_hscyc_write(SlaveDev, pstSnsSync->u32HsCyc);

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_SetModParam
 Description     : isp module parameter

*****************************************************************************/
HI_S32 HI_MPI_ISP_SetModParam(const ISP_MOD_PARAM_S *pstModParam)
{
    HI_ASSERT(sizeof(ISP_MOD_PARAM_S) == sizeof(hi_isp_mod_param));
    return hi_mpi_isp_set_mod_param((hi_isp_mod_param *)pstModParam);

}

HI_S32 HI_MPI_ISP_GetModParam(ISP_MOD_PARAM_S *pstModParam)
{
    HI_ASSERT(sizeof(ISP_MOD_PARAM_S) == sizeof(hi_isp_mod_param));
    return hi_mpi_isp_get_mod_param((hi_isp_mod_param *)pstModParam);
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_MemInit
 Description     : isp initial extent memory
*****************************************************************************/
HI_S32 HI_MPI_ISP_MemInit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_working_mode stIspWorkMode;

    /* check status */
    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    if (ioctl(g_as32IspFd[ViPipe], ISP_MEM_INFO_GET, &pstIspCtx->mem_init)) {
        ISP_ERR_TRACE("ISP[%d] get Mem info failed!\n", ViPipe);
        return HI_ERR_ISP_MEM_NOT_INIT;
    }

    if (pstIspCtx->mem_init == HI_TRUE) {
        s32Ret = ISP_Exit(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] Exit failed!\n", ViPipe);
            return s32Ret;
        }
    }

    /* WDR attribute */
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_WDR_ATTR, &pstIspCtx->wdr_attr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get WDR attr failed\n", ViPipe);
        return s32Ret;
    }

    /* Wdr mode abnormal */
    if ((!pstIspCtx->wdr_attr.mast_pipe) && \
        (IS_WDR_MODE(pstIspCtx->wdr_attr.wdr_mode))) {
        return HI_SUCCESS;
    }

    /* Get pipe size */
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_PIPE_SIZE, &pstIspCtx->pipe_size);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get Pipe size failed\n", ViPipe);
        return s32Ret;
    }

    /* HDR attribute */
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_HDR_ATTR, &pstIspCtx->hdr_attr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get HDR attr failed\n", ViPipe);
        return s32Ret;
    }

    /* Stitch attribute */
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_STITCH_ATTR, &pstIspCtx->stitch_attr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get Stitch attr failed\n", ViPipe);
        return s32Ret;
    }

    /* p2en info */
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_P2EN_INFO_GET, &pstIspCtx->isp0_p2_en);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get p2en info failed\n", ViPipe);
        return s32Ret;
    }

    if (ISP_GetVersion(ViPipe) != HI_TRUE) {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* Creat extern registers */
    s32Ret = VReg_Init(ViPipe, ISP_VIR_REG_BASE(ViPipe), ISP_VREG_SIZE);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init Mem failed\n", ViPipe);
        return s32Ret;
    }

    /* Init block attribute */
    s32Ret = ISP_BlockInit(ViPipe, &pstIspCtx->block_attr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init block failed!\n", ViPipe);
        goto Fail0;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_GET, &stIspWorkMode);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] work mode get failed!\n", ViPipe);
        goto Fail0;
    }

    if (stIspWorkMode.data_input_mode == ISP_MODE_BT1120_YUV) {
        pstIspCtx->isp_yuv_mode = HI_TRUE;
        s32Ret = ISP_SensorCtxInit(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            goto Fail0;
        }
    } else if (stIspWorkMode.data_input_mode == ISP_MODE_RAW) {
        pstIspCtx->isp_yuv_mode = HI_FALSE;
    }

    if ((g_astIspCtx[ViPipe].sns_reg != HI_TRUE) && (g_astIspCtx[ViPipe].isp_yuv_mode != HI_TRUE)) {
        ISP_ERR_TRACE("Sensor doesn't register to ISP[%d]!\n", ViPipe);
        s32Ret = HI_ERR_ISP_SNS_UNREGISTER;
        goto Fail1;
    }


    memset(&pstIspCtx->para_rec, 0, sizeof(isp_para_rec));
    hi_ext_top_wdr_cfg_write(ViPipe, pstIspCtx->para_rec.wdr_cfg);
    hi_ext_top_pub_attr_cfg_write(ViPipe, pstIspCtx->para_rec.pub_cfg);

    hi_ext_top_wdr_switch_write(ViPipe, HI_FALSE);
    hi_ext_top_res_switch_write(ViPipe, HI_FALSE);

    pstIspCtx->mem_init = HI_TRUE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_MEM_INFO_SET, &pstIspCtx->mem_init);
    if (s32Ret != HI_SUCCESS) {
        pstIspCtx->mem_init = HI_FALSE;
        ISP_ERR_TRACE("ISP[%d] set Mem info failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_MEM_NOT_INIT;
        goto Fail1;
    }

    return HI_SUCCESS;
Fail1:
    ISP_SensorUnRegCallBack(ViPipe);
Fail0:
    VReg_Exit(ViPipe, ISP_VIR_REG_BASE(ViPipe), ISP_VREG_SIZE);
    return s32Ret;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_Init
 Description     : isp initial process, include extent memory, top structure,
                    default value, etc.
*****************************************************************************/
HI_S32 HI_ISP_Init(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_VOID *pRegCfg = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_U8 u8WDRMode;
    HI_U32 u32Value = 0;
    const HI_U64 u64Handsignal = ISP_INIT_HAND_SIGNAL;
    HI_VOID *pValue = HI_NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S stSnsImageMode;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;
    hi_isp_mod_param stModParam = {0};

    /* check status */
    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_SENSOR_REGISTER(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    /* Wdr mode abnormal */
    if ((!pstIspCtx->wdr_attr.mast_pipe) && \
        (IS_WDR_MODE(pstIspCtx->wdr_attr.wdr_mode))) {
        return HI_SUCCESS;
    }

    pstIspCtx->para_rec.wdr_cfg = hi_ext_top_wdr_cfg_read(ViPipe);
    ISP_CHECK_ISP_WDR_CFG(ViPipe);

    pstIspCtx->para_rec.pub_cfg = hi_ext_top_pub_attr_cfg_read(ViPipe);
    ISP_CHECK_ISP_PUB_ATTR_CFG(ViPipe);

    pstIspCtx->sys_rect.x      = hi_ext_system_corp_pos_x_read(ViPipe);
    pstIspCtx->sys_rect.y      = hi_ext_system_corp_pos_y_read(ViPipe);
    pstIspCtx->sys_rect.width  = hi_ext_sync_total_width_read(ViPipe);
    pstIspCtx->sys_rect.height = hi_ext_sync_total_height_read(ViPipe);

    if (pstIspCtx->para_rec.init == HI_TRUE) {
        ISP_ERR_TRACE("ISP[%d] Init failed!\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    s32Ret = ISP_LdciBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail00;
    }

    s32Ret = ISP_CfgBeBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail01;
    }

    s32Ret = ISP_GetBeBufFirst(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail02;
    }

    s32Ret = ISP_BeVregAddrInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail02;
    }

    s32Ret = ISP_SttBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail02;
    }

    s32Ret = ISP_SttAddrInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail03;
    }

    s32Ret = ISP_ClutBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail03;
    }

    s32Ret = ISP_SpecAwbBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail04;
    }

    /* init statistics bufs. */
    s32Ret = ISP_StatisticsInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail05;
    }

    /* init proc bufs. */
    s32Ret = ISP_ProcInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail06;
    }

    /* init trans info bufs. */
    s32Ret = ISP_TransInfoInit(ViPipe, &pstIspCtx->isp_trans_info);
    if (s32Ret != HI_SUCCESS) {
        goto Fail07;
    }

    s32Ret = ISP_UpdateInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail08;
    }

    s32Ret = ISP_FrameInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail09;
    }

    s32Ret = ISP_AttachInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail10;
    }

    s32Ret = ISP_ColorGamutInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail11;
    }

    s32Ret = ISP_DngInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail12;
    }

    s32Ret = ISP_ProInfoInit(ViPipe, &pstIspCtx->isp_pro_info);
    if (s32Ret != HI_SUCCESS) {
        goto Fail13;
    }

    s32Ret = ISP_ProNrParamInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail14;
    }

    s32Ret = ISP_ProShpParamInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail15;
    }

    /* regcfg init */
    s32Ret = ISP_RegCfgInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail16;
    }

    /* set sensor wdr mode */
    u8WDRMode = hi_ext_system_sensor_wdr_mode_read(ViPipe);
    s32Ret    = ISP_SensorSetWDRMode(ViPipe, u8WDRMode);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set sensor WDR mode failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail17;
    }

    stSnsImageMode.u16Width  = hi_ext_top_sensor_width_read(ViPipe);
    stSnsImageMode.u16Height = hi_ext_top_sensor_height_read(ViPipe);
    u32Value = hi_ext_system_fps_base_read(ViPipe);
    pValue   = (HI_VOID *)&u32Value;


    stSnsImageMode.f32Fps    = *(HI_FLOAT *)pValue;
    stSnsImageMode.u8SnsMode = hi_ext_system_sensor_mode_read(ViPipe);

    s32Ret = ISP_SensorSetImageMode(ViPipe, &stSnsImageMode);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set sensor image mode failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail17;
    }

    s32Ret = ISP_SensorUpdateAll(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] update sensor all failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail17;
    }

    isp_sensor_get_default(ViPipe, &sns_dft);
    pstIspCtx->frame_info_ctrl.isp_frame->sensor_id   = sns_dft->sensor_mode.sensor_id;
    pstIspCtx->frame_info_ctrl.isp_frame->sensor_mode = sns_dft->sensor_mode.sensor_mode;

    /* Get Dng paramters from CMOS.c */
    ISP_DngExtRegsInitialize(ViPipe, (ISP_DNG_COLORPARAM_S *)(&sns_dft->dng_color_param));
    hi_ext_system_dng_static_info_valid_write(ViPipe, sns_dft->sensor_mode.valid_dng_raw_format);

    if (sns_dft->sensor_mode.valid_dng_raw_format == HI_TRUE) {
        memcpy(&pstIspCtx->dng_info_ctrl.isp_dng->dng_raw_format, \
               &sns_dft->sensor_mode.dng_raw_format, sizeof(hi_isp_dng_raw_format));
    } else {
        ISP_ERR_TRACE("ISP[%d] DngInfo not initialized in Cmos.c!\n", ViPipe);
    }

    /* init the common part of extern registers and real registers */
    ISP_ExtRegsDefault(ViPipe);
    ISP_RegsDefault(ViPipe);
    ISP_ExtRegsInitialize(ViPipe);
    ISP_RegsInitialize(ViPipe);

    /* isp algs global variable initialize */
    ISP_GlobalInitialize(ViPipe);

    /* register all algorithm to isp, and init them. */
    ISP_AlgsRegister(ViPipe);
    pstIspCtx->frame_info.sensor_id   = sns_dft->sensor_mode.sensor_id;
    pstIspCtx->frame_info.sensor_mode = sns_dft->sensor_mode.sensor_mode;

    /* get regcfg */
    ISP_GetRegCfgCtx(ViPipe, &pRegCfg);

    ISP_AlgsInit(pstIspCtx->algs, ViPipe, pRegCfg);

    s32Ret = ISP_ModParamGet(&stModParam);
    if (s32Ret != HI_SUCCESS) {
        goto Fail18;
    }

    s32Ret = ISP_Lut2SttApbReg(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init lut2stt reg failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail18;
    }

    /* set WDR mode to kernel. */
    s32Ret = ISP_WDRCfgSet(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set WDR mode to kernel failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail18;
    }

    s32Ret = ISP_SensorInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init sensor failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail18;
    }
    ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);
    pstIspCtx->linkage.cfg2valid_delay_max = pstSnsRegsInfo->u8Cfg2ValidDelayMax;

    /* regcfg info set */
    s32Ret = ISP_RegCfgInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init regcfgs info failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail19;
    }

    pstIspCtx->para_rec.stitch_sync = HI_TRUE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_INIT_SET, &pstIspCtx->para_rec.stitch_sync);
    if (s32Ret != HI_SUCCESS) {
        pstIspCtx->para_rec.stitch_sync = HI_FALSE;
        ISP_ERR_TRACE("ISP[%d] set isp stitch sync failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail19;
    }

    /* init isp be cfgs all buffer */
    s32Ret = ISP_AllCfgsBeBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init Becfgs buffer failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail19;
    }

    /* init isp global variables */
    MUTEX_INIT_LOCK(pstIspCtx->lock);
    MUTEX_LOCK(pstIspCtx->lock);

    pstIspCtx->para_rec.init = HI_TRUE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_INIT_INFO_SET, &pstIspCtx->para_rec.init);
    if (s32Ret != HI_SUCCESS) {
        pstIspCtx->para_rec.init = HI_FALSE;
        MUTEX_UNLOCK(pstIspCtx->lock);
        ISP_ERR_TRACE("ISP[%d] set isp init info failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail20;
    }
    pstIspCtx->para_rec.run_en = HI_TRUE;

    /* set handshake signal */
    if (ioctl(g_as32IspFd[ViPipe], ISP_RUN_STATE_SET, &u64Handsignal)) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        MUTEX_UNLOCK(pstIspCtx->lock);
        goto Fail20;
    }

    MUTEX_UNLOCK(pstIspCtx->lock);

    return HI_SUCCESS;

Fail20:
    MUTEX_DESTROY(pstIspCtx->lock);
Fail19:
    ISP_SensorExit(ViPipe);
Fail18:
    ISP_AlgsExit(ViPipe, pstIspCtx->algs);
    ISP_AlgsUnRegister(ViPipe);
Fail17:
    ISP_RegCfgExit(ViPipe);
Fail16:
    ISP_ProShpParamExit(ViPipe);
Fail15:
    ISP_ProNrParamExit(ViPipe);
Fail14:
    ISP_ProInfoExit(ViPipe);
Fail13:
    ISP_DngInfoExit(ViPipe);
Fail12:
    ISP_ColorGamutInfoExit(ViPipe);
Fail11:
    ISP_AttachInfoExit(ViPipe);
Fail10:
    ISP_FrameInfoExit(ViPipe);
Fail09:
    ISP_UpdateInfoExit(ViPipe);
Fail08:
    ISP_TransInfoExit(ViPipe);
Fail07:
    ISP_ProcExit(ViPipe);
Fail06:
    ISP_StatisticsExit(ViPipe);
Fail05:
    ISP_SpecAwbBufExit(ViPipe);
Fail04:
    ISP_ClutBufExit(ViPipe);
Fail03:
    ISP_SttBufExit(ViPipe);
Fail02:
    ISP_CfgBeBufExit(ViPipe);
Fail01:
    ISP_LdciBufExit(ViPipe);
Fail00:
    return s32Ret;
}

HI_S32 HI_ISP_YUV_Init(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_VOID *pRegCfg = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    const HI_U64 u64Handsignal = ISP_INIT_HAND_SIGNAL;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    /* check status */
    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    /* Wdr mode abnormal */
    if ((!pstIspCtx->wdr_attr.mast_pipe) && \
        (IS_WDR_MODE(pstIspCtx->wdr_attr.wdr_mode))) {
        return HI_SUCCESS;
    }

    pstIspCtx->para_rec.wdr_cfg = hi_ext_top_wdr_cfg_read(ViPipe);
    ISP_CHECK_ISP_WDR_CFG(ViPipe);

    pstIspCtx->para_rec.pub_cfg = hi_ext_top_pub_attr_cfg_read(ViPipe);
    ISP_CHECK_ISP_PUB_ATTR_CFG(ViPipe);

    pstIspCtx->sys_rect.x      = hi_ext_system_corp_pos_x_read(ViPipe);
    pstIspCtx->sys_rect.y      = hi_ext_system_corp_pos_y_read(ViPipe);
    pstIspCtx->sys_rect.width  = hi_ext_sync_total_width_read(ViPipe);
    pstIspCtx->sys_rect.height = hi_ext_sync_total_height_read(ViPipe);

    if (pstIspCtx->para_rec.init == HI_TRUE) {
        ISP_ERR_TRACE("ISP[%d] Init failed!\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    s32Ret = ISP_LdciBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail00;
    }

    s32Ret = ISP_CfgBeBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail01;
    }

    s32Ret = ISP_GetBeBufFirst(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail02;
    }

    s32Ret = ISP_BeVregAddrInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail02;
    }

    s32Ret = ISP_SttBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail02;
    }

    s32Ret = ISP_SttAddrInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail03;
    }

    /* init statistics bufs. */
    s32Ret = ISP_StatisticsInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail03;
    }

    /* init proc bufs. */
    s32Ret = ISP_ProcInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail04;
    }

    /* init trans info bufs. */
    s32Ret = ISP_TransInfoInit(ViPipe, &pstIspCtx->isp_trans_info);
    if (s32Ret != HI_SUCCESS) {
        goto Fail05;
    }

    s32Ret = ISP_UpdateInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail06;
    }

    s32Ret = ISP_FrameInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail07;
    }

    s32Ret = ISP_AttachInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail08;
    }

    s32Ret = ISP_ColorGamutInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail09;
    }

    s32Ret = ISP_DngInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail10;
    }

    s32Ret = ISP_ProInfoInit(ViPipe, &pstIspCtx->isp_pro_info);
    if (s32Ret != HI_SUCCESS) {
        goto Fail11;
    }

    s32Ret = ISP_ProNrParamInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail12;
    }

    s32Ret = ISP_ProShpParamInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail13;
    }

    /* regcfg init */
    s32Ret = ISP_RegCfgInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        goto Fail14;
    }

    s32Ret = ISP_SensorUpdateAll_YUV(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] update sensor all failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail15;
    }

    isp_sensor_get_default(ViPipe, &sns_dft);
    pstIspCtx->frame_info_ctrl.isp_frame->sensor_id   = sns_dft->sensor_mode.sensor_id;
    pstIspCtx->frame_info_ctrl.isp_frame->sensor_mode = sns_dft->sensor_mode.sensor_mode;

    /* Get Dng paramters from CMOS.c */
    ISP_DngExtRegsInitialize(ViPipe, (ISP_DNG_COLORPARAM_S *)(&sns_dft->dng_color_param));
    hi_ext_system_dng_static_info_valid_write(ViPipe, sns_dft->sensor_mode.valid_dng_raw_format);

    if (sns_dft->sensor_mode.valid_dng_raw_format == HI_TRUE) {
        memcpy(&pstIspCtx->dng_info_ctrl.isp_dng->dng_raw_format, \
               &sns_dft->sensor_mode.dng_raw_format, sizeof(hi_isp_dng_raw_format));
    } else {
        ISP_ERR_TRACE("ISP[%d] DngInfo not initialized in Cmos.c!\n", ViPipe);
    }

    /* init the common part of extern registers and real registers */
    ISP_ExtRegsDefault(ViPipe);
    ISP_RegsDefault(ViPipe);
    ISP_ExtRegsInitialize(ViPipe);
    ISP_RegsInitialize(ViPipe);

    /* isp algs global variable initialize */
    ISP_GlobalInitialize(ViPipe);

    /* register all algorithm to isp, and init them. */
    ISP_YUVAlgsRegister(ViPipe);

    /* get regcfg */
    ISP_GetRegCfgCtx(ViPipe, &pRegCfg);

    ISP_AlgsInit(pstIspCtx->algs, ViPipe, pRegCfg);

    pstIspCtx->linkage.cfg2valid_delay_max = 2;

    /* regcfg info set */
    s32Ret = ISP_RegCfgInfoInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init regcfgs info failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail16;
    }

    /* set WDR mode to kernel. */
    s32Ret = ISP_WDRCfgSet(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set WDR mode to kernel failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail16;
    }

    pstIspCtx->para_rec.stitch_sync = HI_TRUE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_INIT_SET, &pstIspCtx->para_rec.stitch_sync);
    if (s32Ret != HI_SUCCESS) {
        pstIspCtx->para_rec.stitch_sync = HI_FALSE;
        ISP_ERR_TRACE("ISP[%d] set isp stitch sync failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail16;
    }

    /* init isp be cfgs all buffer */
    s32Ret = ISP_AllCfgsBeBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init Becfgs buffer failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail16;
    }

    /* init isp global variables */
    MUTEX_INIT_LOCK(pstIspCtx->lock);
    MUTEX_LOCK(pstIspCtx->lock);

    pstIspCtx->para_rec.init = HI_TRUE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_INIT_INFO_SET, &pstIspCtx->para_rec.init);
    if (s32Ret != HI_SUCCESS) {
        pstIspCtx->para_rec.init = HI_FALSE;
        MUTEX_UNLOCK(pstIspCtx->lock);
        ISP_ERR_TRACE("ISP[%d] set isp init info failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto Fail17;
    }
    pstIspCtx->para_rec.run_en = HI_TRUE;

    /* set handshake signal */
    if (ioctl(g_as32IspFd[ViPipe], ISP_RUN_STATE_SET, &u64Handsignal)) {
        s32Ret = HI_ERR_ISP_NOT_INIT;
        MUTEX_UNLOCK(pstIspCtx->lock);
        goto Fail17;
    }

    MUTEX_UNLOCK(pstIspCtx->lock);

    return HI_SUCCESS;

Fail17:
    MUTEX_DESTROY(pstIspCtx->lock);
Fail16:
    ISP_SensorExit(ViPipe);
Fail15:
    ISP_RegCfgExit(ViPipe);
Fail14:
    ISP_ProShpParamExit(ViPipe);
Fail13:
    ISP_ProNrParamExit(ViPipe);
Fail12:
    ISP_ProInfoExit(ViPipe);
Fail11:
    ISP_DngInfoExit(ViPipe);
Fail10:
    ISP_ColorGamutInfoExit(ViPipe);
Fail09:
    ISP_AttachInfoExit(ViPipe);
Fail08:
    ISP_FrameInfoExit(ViPipe);
Fail07:
    ISP_UpdateInfoExit(ViPipe);
Fail06:
    ISP_TransInfoExit(ViPipe);
Fail05:
    ISP_ProcExit(ViPipe);
Fail04:
    ISP_StatisticsExit(ViPipe);
Fail03:
    ISP_SttBufExit(ViPipe);
Fail02:
    ISP_CfgBeBufExit(ViPipe);
Fail01:
    ISP_LdciBufExit(ViPipe);
Fail00:
    return s32Ret;
}

HI_S32  HI_MPI_ISP_Init(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_S32 s32Ret;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    if (pstIspCtx->isp_yuv_mode == HI_FALSE) {
        s32Ret = HI_ISP_Init(ViPipe);
    } else {
        s32Ret = HI_ISP_YUV_Init(ViPipe);
    }

    return s32Ret;
}

HI_S32  HI_ISP_YUV_RunOnce(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_U32 u32WDRmode;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    /* check status */
    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    ISP_CHECK_MEM_INIT(ViPipe);

    ISP_CHECK_ISP_INIT(ViPipe);

    /* Wdr mode abnormal */
    if ((!pstIspCtx->wdr_attr.mast_pipe) && \
        (IS_WDR_MODE(pstIspCtx->wdr_attr.wdr_mode))) {
        return HI_SUCCESS;
    }

    MUTEX_LOCK(pstIspCtx->lock);

    if (pstIspCtx->para_rec.run == HI_TRUE) {
        ISP_ERR_TRACE("ISP[%d]  is already running !\n", ViPipe);
        MUTEX_UNLOCK(pstIspCtx->lock);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* Sometimes HI_MPI_ISP_Run thread is not scheduled to run before calling HI_MPI_ISP_Exit. */
    if (pstIspCtx->para_rec.run_en == HI_FALSE) {
        MUTEX_UNLOCK(pstIspCtx->lock);
        return HI_SUCCESS;
    }

    pstIspCtx->para_rec.run = HI_TRUE;

    u32WDRmode = hi_ext_system_sensor_wdr_mode_read(ViPipe);
    pstIspCtx->sns_wdr_mode = u32WDRmode;

    pstIspCtx->linkage.run_once = HI_TRUE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_YUV_RUNONCE_INFO, &pstIspCtx->linkage.run_once);
    if (s32Ret != HI_SUCCESS) {
        pstIspCtx->linkage.run_once = HI_FALSE;
        pstIspCtx->para_rec.run  = HI_FALSE;
        ISP_ERR_TRACE("ISP[%d] set runonce info failed!\n", ViPipe);
        MUTEX_UNLOCK(pstIspCtx->lock);
        return s32Ret;
    }

    ISP_Run(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_KERNEL_YUV_RUNONCE);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] kernel runonce  failed!\n", ViPipe);
        pstIspCtx->para_rec.run  = HI_FALSE;
        pstIspCtx->linkage.run_once = HI_FALSE;
        ioctl(g_as32IspFd[ViPipe], ISP_YUV_RUNONCE_INFO, &pstIspCtx->linkage.run_once);
        MUTEX_UNLOCK(pstIspCtx->lock);
        return s32Ret;
    }

    pstIspCtx->para_rec.run = HI_FALSE;

    MUTEX_UNLOCK(pstIspCtx->lock);

    return HI_SUCCESS;
}

/* When offline mode user send raw to BE, firstly need call this function to insure paramters ready */
HI_S32 HI_ISP_RunOnce(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_running_mode enIspRunMode;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    /* check status */
    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_SENSOR_REGISTER(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    ISP_CHECK_ISP_INIT(ViPipe);

    /* Online mode not support */
    enIspRunMode = pstIspCtx->block_attr.running_mode;
    if ((IS_ONLINE_MODE(enIspRunMode)) || \
        (IS_SIDEBYSIDE_MODE(enIspRunMode))) {
        ISP_ERR_TRACE("ISP[%d] RunOnce not support for online!\n", ViPipe);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    /* Wdr mode abnormal */
    if ((!pstIspCtx->wdr_attr.mast_pipe) && \
        (IS_WDR_MODE(pstIspCtx->wdr_attr.wdr_mode))) {
        return HI_SUCCESS;
    }

    MUTEX_LOCK(pstIspCtx->lock);

    if (pstIspCtx->para_rec.run == HI_TRUE) {
        ISP_ERR_TRACE("ISP[%d] Run failed!\n", ViPipe);
        MUTEX_UNLOCK(pstIspCtx->lock);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* Sometimes HI_MPI_ISP_Run thread is not scheduled to run before calling HI_MPI_ISP_Exit. */
    if (pstIspCtx->para_rec.run_en == HI_FALSE) {
        MUTEX_UNLOCK(pstIspCtx->lock);
        return HI_SUCCESS;
    }

    pstIspCtx->para_rec.run = HI_TRUE;

#if 1
    /* change image mode (WDR mode or resolution) */
    s32Ret = ISP_SwitchMode(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] switch mode failed!\n", ViPipe);
        MUTEX_UNLOCK(pstIspCtx->lock);
        return s32Ret;
    }
#else
    /* change  resolution  */
    ISP_SwitchImageMode(ViPipe);

    u32WDRmode = hi_ext_system_sensor_wdr_mode_read(ViPipe);
    /* swtich linear/WDR mode, width/height, fps  */
    if (pstIspCtx->sns_wdr_mode != u32WDRmode) {
        pstIspCtx->sns_wdr_mode = u32WDRmode;
        ISP_SwitchWDRMode(ViPipe);
    }
#endif
    pstIspCtx->linkage.run_once = HI_TRUE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_OPT_RUNONCE_INFO, &pstIspCtx->linkage.run_once);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set runonce info failed!\n", ViPipe);
        goto Fail0;
    }

    ISP_Run(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_KERNEL_RUNONCE);
    if (s32Ret != HI_SUCCESS) {
        goto Fail0;
    }

    pstIspCtx->para_rec.run = HI_FALSE;
    MUTEX_UNLOCK(pstIspCtx->lock);

    return HI_SUCCESS;

Fail0:
    pstIspCtx->linkage.run_once = HI_FALSE;
    if (ioctl(g_as32IspFd[ViPipe], ISP_OPT_RUNONCE_INFO, &pstIspCtx->linkage.run_once)) {
        ISP_ERR_TRACE("ISP[%d] set runonce info failed!\n", ViPipe);
    }
    pstIspCtx->para_rec.run = HI_FALSE;
    MUTEX_UNLOCK(pstIspCtx->lock);

    return s32Ret;
}

HI_S32 HI_MPI_ISP_RunOnce(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_S32 s32Ret;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    if (pstIspCtx->isp_yuv_mode == HI_FALSE) {
        s32Ret = HI_ISP_RunOnce(ViPipe);
    } else {
        s32Ret = HI_ISP_YUV_RunOnce(ViPipe);
    }

    return s32Ret;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_Run
 Description     : isp firmware recurrent task, always run in a single thread.
*****************************************************************************/
HI_S32 HI_MPI_ISP_Run(VI_PIPE ViPipe)
{
    HI_BOOL bEn;
    HI_S32 s32Ret;
    HI_U32 u32IntStatus = 0;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    /* check status */
    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_SENSOR_REGISTER(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    ISP_CHECK_ISP_INIT(ViPipe);

    /* Wdr mode abnormal */
    if ((!pstIspCtx->wdr_attr.mast_pipe) && \
        (IS_WDR_MODE(pstIspCtx->wdr_attr.wdr_mode))) {
        return HI_SUCCESS;
    }

    if (pstIspCtx->para_rec.run == HI_TRUE) {
        ISP_ERR_TRACE("ISP[%d] Run failed!\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    MUTEX_LOCK(pstIspCtx->lock);

    /* Sometimes ISP run thread is not scheduled to run before calling ISP exit. */
    if (pstIspCtx->para_rec.run_en == HI_FALSE) {
        MUTEX_UNLOCK(pstIspCtx->lock);
        return HI_SUCCESS;
    }

    /* enable interrupt */
    bEn = HI_TRUE;
    if (ioctl(g_as32IspFd[ViPipe], ISP_SET_INT_ENABLE, &bEn) < 0) {
        ISP_ERR_TRACE("Enable ISP[%d] interrupt failed!\n", ViPipe);
        MUTEX_UNLOCK(pstIspCtx->lock);
        return HI_FAILURE;
    }

    pstIspCtx->para_rec.run = HI_TRUE;
    MUTEX_UNLOCK(pstIspCtx->lock);

    while (1) {
        MUTEX_LOCK(pstIspCtx->lock);

        if (pstIspCtx->para_rec.run_en == HI_FALSE) {
            MUTEX_UNLOCK(pstIspCtx->lock);
            break;
        }
#if 1
        /* change image mode (WDR mode or resolution) */
        s32Ret = ISP_SwitchMode(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] switch mode failed!\n", ViPipe);
            MUTEX_UNLOCK(pstIspCtx->lock);
            break;
        }
#else
        /* Change WDR mode  */
        s32Ret  = ISP_SwitchWDRMode(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] switch WDR mode failed!\n", ViPipe);
            MUTEX_UNLOCK(pstIspCtx->lock);
            break;
        }

        /* Change resolution  */
        s32Ret = ISP_SwitchImageMode(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] switch image mode failed!\n", ViPipe);
            MUTEX_UNLOCK(pstIspCtx->lock);
            break;
        }
#endif

        MUTEX_UNLOCK(pstIspCtx->lock);

        {
            u32IntStatus = 0;
            /* waked up by the interrupt */
            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_FRAME_EDGE, &u32IntStatus);
            if (s32Ret == HI_SUCCESS) {
                /* isp firmware calculate, include AE/AWB, etc. */
                if (ISP_1ST_INT & u32IntStatus) {
                    MUTEX_LOCK(pstIspCtx->lock);

                    if (pstIspCtx->para_rec.run_en == HI_FALSE) {
                        MUTEX_UNLOCK(pstIspCtx->lock);
                        break;
                    }

                    ISP_Run(ViPipe);

                    MUTEX_UNLOCK(pstIspCtx->lock);
                }
            }
        }
    }

    /* disable interrupt */
    bEn = HI_FALSE;
    if (ioctl(g_as32IspFd[ViPipe], ISP_SET_INT_ENABLE, &bEn) < 0) {
        ISP_ERR_TRACE("Disable ISP[%d] interrupt failed!\n", ViPipe);
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_Exit
 Description     : control isp to exit recurrent task, always run in main process.
*****************************************************************************/
HI_S32 HI_MPI_ISP_Exit(VI_PIPE ViPipe)
{
    HI_S32  s32Ret;
    HI_BOOL bEnable = HI_FALSE;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    /* Check status */
    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* Wdr mode abnormal */
    if ((!pstIspCtx->wdr_attr.mast_pipe) && \
        (IS_WDR_MODE(pstIspCtx->wdr_attr.wdr_mode))) {
        return HI_SUCCESS;
    }

    ISP_StitchSyncExit(ViPipe);
    MUTEX_LOCK(pstIspCtx->lock);
    ISP_AlgEnExit(ViPipe);
    if (ioctl(g_as32IspFd[ViPipe], ISP_SET_INT_ENABLE, &bEnable) < 0) {
        ISP_ERR_TRACE("Disable ISP[%d] interrupt failed!\n", ViPipe);
        MUTEX_UNLOCK(pstIspCtx->lock);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    s32Ret = ISP_Exit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Exit failed!\n", ViPipe);
    }

    ISP_LibsUnRegister(ViPipe);
    MUTEX_UNLOCK(pstIspCtx->lock);
    MUTEX_DESTROY(pstIspCtx->lock);

    return s32Ret;

}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_SetRegister
 Description     : set isp register, include extent memory.
*****************************************************************************/
HI_S32 HI_MPI_ISP_SetRegister(VI_PIPE ViPipe, HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_CHECK_PIPE(ViPipe);
    s32Ret = IO_WRITE32_Ex(u32Addr, u32Value);

    return s32Ret;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_GetRegister
 Description     : get isp register, include extent memory.
*****************************************************************************/
HI_S32 HI_MPI_ISP_GetRegister(VI_PIPE ViPipe, HI_U32 u32Addr, HI_U32 *pu32Value)
{
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pu32Value);
    s32Ret = IO_READ32_Ex(u32Addr, pu32Value);

    return s32Ret;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_SensorRegister
 Description     : sensor register callback function to isp firmware
*****************************************************************************/

HI_S32 HI_MPI_ISP_SetSnsSlaveAttr(SLAVE_DEV SlaveDev, const ISP_SLAVE_SNS_SYNC_S *pstSnsSync)
{
    SLAVE_CHECK_DEV(SlaveDev);
    ISP_CHECK_POINTER(pstSnsSync);

    hi_isp_slave_mode_time_cfg_select_write(SlaveDev, pstSnsSync->u32SlaveModeTime);
    hi_isp_slave_mode_sync_write(pstSnsSync->u32SlaveModeTime);
    hi_isp_slave_mode_configs_write(SlaveDev, pstSnsSync->unCfg.u32Bytes);
    hi_isp_slave_mode_vstime_low_write(SlaveDev, pstSnsSync->u32VsTime);
    hi_isp_slave_mode_vstime_high_write(SlaveDev, 0);
    hi_isp_slave_mode_hstime_write(SlaveDev, pstSnsSync->u32HsTime);
    hi_isp_slave_mode_vscyc_low_write(SlaveDev, pstSnsSync->u32VsCyc);
    hi_isp_slave_mode_vscyc_high_write(SlaveDev, 0);
    hi_isp_slave_mode_hscyc_write(SlaveDev, pstSnsSync->u32HsCyc);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetSnsSlaveAttr(SLAVE_DEV SlaveDev, ISP_SLAVE_SNS_SYNC_S *pstSnsSync)
{
    SLAVE_CHECK_DEV(SlaveDev);
    ISP_CHECK_POINTER(pstSnsSync);

    pstSnsSync->u32SlaveModeTime = hi_isp_slave_mode_time_cfg_select_read(SlaveDev);
    pstSnsSync->unCfg.u32Bytes = hi_isp_slave_mode_configs_read(SlaveDev);
    pstSnsSync->u32VsTime = hi_isp_slave_mode_vstime_low_read(SlaveDev);
    pstSnsSync->u32HsTime = hi_isp_slave_mode_hstime_read(SlaveDev);
    pstSnsSync->u32VsCyc = hi_isp_slave_mode_vscyc_low_read(SlaveDev);
    pstSnsSync->u32HsCyc = hi_isp_slave_mode_hscyc_read(SlaveDev);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SensorRegCallBack(VI_PIPE ViPipe, ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo, ISP_SENSOR_REGISTER_S *pstRegister)
{
    HI_S32 s32Ret;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstRegister);
    ISP_CHECK_POINTER(pstSnsAttrInfo);

    ISP_CHECK_POINTER(pstRegister->stSnsExp.pfn_cmos_sensor_init);
    ISP_CHECK_POINTER(pstRegister->stSnsExp.pfn_cmos_get_isp_default);
    ISP_CHECK_POINTER(pstRegister->stSnsExp.pfn_cmos_get_isp_black_level);
    ISP_CHECK_POINTER(pstRegister->stSnsExp.pfn_cmos_get_sns_reg_info);
    ISP_CHECK_POINTER(pstRegister->stSnsExp.pfn_cmos_set_pixel_detect);


    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    if (pstIspCtx->sns_reg == HI_TRUE) {
        ISP_ERR_TRACE("Reg ERR! Sensor have registered to ISP[%d]!\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    s32Ret = ISP_SensorRegCallBack(ViPipe, pstSnsAttrInfo, pstRegister);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    pstIspCtx->bind_attr.sensor_id = pstSnsAttrInfo->eSensorId;
    pstIspCtx->sns_reg = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AELibRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
                                   ISP_AE_REGISTER_S *pstRegister)
{
    HI_S32 s32Ret = HI_SUCCESS;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_lib_node *pstAeLibNode = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAeLib);
    ISP_CHECK_POINTER(pstRegister);

    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_init);
    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_run);
    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_ctrl);
    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_exit);

    /* whether the lib have been registered */
    s32Ret = ISP_FindLib(pstIspCtx->ae_lib_info.libs, pstAeLib);
    if (s32Ret != -1) {
        ISP_ERR_TRACE("Reg ERR! aelib have registered to ISP[%d].\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* whether can we register a new lib  */
    pstAeLibNode = ISP_SearchLib(pstIspCtx->ae_lib_info.libs);
    if (pstAeLibNode == HI_NULL) {
        ISP_ERR_TRACE("can't register aelib to ISP[%d], there is too many libs.\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* record register info */
    memcpy(&pstAeLibNode->alg_lib, pstAeLib, sizeof(ALG_LIB_S));
    memcpy(&pstAeLibNode->ae_regsiter, pstRegister, sizeof(ISP_AE_REGISTER_S));
    pstAeLibNode->used = HI_TRUE;

    /* set active lib */
    pstIspCtx->ae_lib_info.active_lib = ISP_FindLib(pstIspCtx->ae_lib_info.libs, pstAeLib);
    memcpy(&pstIspCtx->bind_attr.ae_lib, pstAeLib, sizeof(ALG_LIB_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AWBLibRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
                                    ISP_AWB_REGISTER_S *pstRegister)
{
    HI_S32 s32Ret = HI_SUCCESS;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_lib_node *pstAwbLibNode = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAwbLib);
    ISP_CHECK_POINTER(pstRegister);

    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_init);
    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_run);
    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_ctrl);
    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_exit);

    /* whether the lib have been registered */
    s32Ret = ISP_FindLib(pstIspCtx->awb_lib_info.libs, pstAwbLib);
    if (s32Ret != -1) {
        ISP_ERR_TRACE("Reg ERR! awblib have registered to ISP[%d].\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* whether can we register a new lib  */
    pstAwbLibNode = ISP_SearchLib(pstIspCtx->awb_lib_info.libs);
    if (pstAwbLibNode == HI_NULL) {
        ISP_ERR_TRACE("can't register awblib to ISP[%d], there is too many libs.\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* record register info */
    memcpy(&pstAwbLibNode->alg_lib, pstAwbLib, sizeof(ALG_LIB_S));
    memcpy(&pstAwbLibNode->awb_regsiter, pstRegister, sizeof(ISP_AWB_REGISTER_S));
    pstAwbLibNode->used = HI_TRUE;

    /* set active lib */
    pstIspCtx->awb_lib_info.active_lib = ISP_FindLib(pstIspCtx->awb_lib_info.libs, pstAwbLib);
    memcpy(&pstIspCtx->bind_attr.awb_lib, pstAwbLib, sizeof(ALG_LIB_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SensorUnRegCallBack(VI_PIPE ViPipe, SENSOR_ID SensorId)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_SENSOR_REGISTER(ViPipe);

    /* check sensor id */
    if (pstIspCtx->bind_attr.sensor_id != SensorId) {
        ISP_ERR_TRACE("UnReg ERR! ISP[%d] Registered sensor is %d, present sensor is %d.\n",
                  ViPipe, pstIspCtx->bind_attr.sensor_id, SensorId);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    pstIspCtx->bind_attr.sensor_id = 0;
    pstIspCtx->sns_reg = HI_FALSE;

    ISP_SensorUnRegCallBack(ViPipe);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AELibUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib)
{
    HI_S32 s32SearchId;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAeLib);

    s32SearchId = ISP_FindLib(pstIspCtx->ae_lib_info.libs, pstAeLib);
    if (-1 == s32SearchId) {
        ISP_ERR_TRACE("can't find ae lib in ISP[%d].\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    memset(&pstIspCtx->ae_lib_info.libs[s32SearchId], 0, sizeof(isp_lib_node));

    /* set active lib */
    pstIspCtx->ae_lib_info.active_lib = 0;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AWBLibUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32SearchId;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAwbLib);

    s32SearchId = ISP_FindLib(pstIspCtx->awb_lib_info.libs, pstAwbLib);
    if (s32SearchId == -1) {
        ISP_ERR_TRACE("can't find awb lib in ISP[%d].\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    memset(&pstIspCtx->awb_lib_info.libs[s32SearchId], 0, sizeof(isp_lib_node));

    /* set active lib */
    pstIspCtx->awb_lib_info.active_lib = 0;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetBindAttr(VI_PIPE ViPipe, const ISP_BIND_ATTR_S *pstBindAttr)
{
    SENSOR_ID SensorId = 0;
    HI_S32    s32SearchId;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_S32 s32Ret = HI_SUCCESS;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    ISP_CHECK_POINTER(pstBindAttr);

    s32Ret = ISP_SensorGetId(ViPipe, &SensorId);
    if (s32Ret  != HI_SUCCESS ) {
        ISP_ERR_TRACE("ISP[%d] Get Sensor id err\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* check sensor id */
    if (pstBindAttr->SensorId != SensorId) {
        ISP_ERR_TRACE("ISP[%d] Register sensor is %d, present sensor is %d.\n",
                  ViPipe, SensorId, pstBindAttr->SensorId);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* check ae lib */
    s32SearchId = ISP_FindLib(pstIspCtx->ae_lib_info.libs, &pstBindAttr->stAeLib);
    if (s32SearchId != -1) {
        pstIspCtx->ae_lib_info.active_lib = s32SearchId;
    } else {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* check awb lib */
    s32SearchId = ISP_FindLib(pstIspCtx->awb_lib_info.libs, &pstBindAttr->stAwbLib);
    if (s32SearchId != -1) {
        pstIspCtx->awb_lib_info.active_lib = s32SearchId;
    } else {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* check af lib */
    s32SearchId = ISP_FindLib(pstIspCtx->af_lib_info.libs, &pstBindAttr->stAfLib);
    if (s32SearchId != -1) {
        pstIspCtx->af_lib_info.active_lib = s32SearchId;
    } else {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* save global variable */
    memcpy(&pstIspCtx->bind_attr, pstBindAttr, sizeof(ISP_BIND_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetBindAttr(VI_PIPE ViPipe, ISP_BIND_ATTR_S *pstBindAttr)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    ISP_CHECK_POINTER(pstBindAttr);

    /* get global variable */
    memcpy(pstBindAttr, &pstIspCtx->bind_attr, sizeof(ISP_BIND_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetVDTimeOut(VI_PIPE ViPipe, ISP_VD_TYPE_E enIspVDType, HI_U32 u32MilliSec)
{
    HI_S32 s32Ret;
    isp_vd_timeout   stIspVdTimeOut;
    isp_working_mode stIspWorkMode;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    stIspVdTimeOut.milli_sec  = u32MilliSec;
    stIspVdTimeOut.int_status = 0x0;

    switch (enIspVDType) {
        case ISP_VD_FE_START:
            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_VD_TIMEOUT, &stIspVdTimeOut);
            if (s32Ret != HI_SUCCESS) {
                return s32Ret;
            }
            break;
        case ISP_VD_FE_END:
            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_VD_END_TIMEOUT, &stIspVdTimeOut);
            if (s32Ret != HI_SUCCESS) {
                return s32Ret;
            }
            break;
        case ISP_VD_BE_END:
            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_GET, &stIspWorkMode);
            if (s32Ret != HI_SUCCESS) {
                ISP_ERR_TRACE("Get Work Mode error!\n");
                return HI_FAILURE;
            }

            if ((stIspWorkMode.running_mode == ISP_MODE_RUNNING_ONLINE) ||
                (stIspWorkMode.running_mode == ISP_MODE_RUNNING_SIDEBYSIDE)) {
                ISP_ERR_TRACE("Only support ISP_VD_BE_END under ISP offline mode!\n");
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_VD_BEEND_TIMEOUT, &stIspVdTimeOut);
            if (s32Ret != HI_SUCCESS) {
                return s32Ret;
            }
            break;
        default:
            ISP_ERR_TRACE("ISP[%d] Get VD type %d not support!\n", ViPipe, enIspVDType);
            return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_SetDCFInfo
 Description     : set dcf info to isp firmware
*****************************************************************************/
HI_S32 HI_MPI_ISP_SetDCFInfo(VI_PIPE ViPipe, const ISP_DCF_INFO_S *pstIspDCF)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_BOOL bTempMap = HI_FALSE;
    HI_U64 u64PhyAddrHigh;
    HI_U64 u64PhyAddrTemp;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspDCF);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u64PhyAddrHigh = (HI_U64)hi_ext_system_update_info_high_phyaddr_read(ViPipe);
    u64PhyAddrTemp = (HI_U64)hi_ext_system_update_info_low_phyaddr_read(ViPipe);
    u64PhyAddrTemp |= (u64PhyAddrHigh << 32);

    if (pstIspCtx->isp_trans_info.update_info.phy_addr == 0) {
        pstIspCtx->isp_trans_info.update_info.phy_addr = u64PhyAddrTemp;

        pstIspCtx->update_info_ctrl.isp_update_info = HI_MPI_SYS_Mmap(pstIspCtx->isp_trans_info.update_info.phy_addr, \
                                                       (sizeof(hi_isp_dcf_update_info) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(hi_isp_dcf_const_info)));

        if (pstIspCtx->update_info_ctrl.isp_update_info == HI_NULL) {
            ISP_ERR_TRACE("isp[%d] set dcf info mmap failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }

        pstIspCtx->update_info_ctrl.isp_dcf_const_info = \
                (hi_isp_dcf_const_info *)(pstIspCtx->update_info_ctrl.isp_update_info + ISP_MAX_UPDATEINFO_BUF_NUM);

        bTempMap = HI_TRUE;
    }

    memcpy(pstIspCtx->update_info_ctrl.isp_dcf_const_info, &pstIspDCF->stIspDCFConstInfo, sizeof(hi_isp_dcf_const_info));
    memcpy(pstIspCtx->update_info_ctrl.isp_update_info, &pstIspDCF->stIspDCFUpdateInfo, sizeof(hi_isp_dcf_update_info));

    if (bTempMap) {
        HI_MPI_SYS_Munmap(pstIspCtx->update_info_ctrl.isp_update_info, \
                          (sizeof(hi_isp_dcf_update_info) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(hi_isp_dcf_const_info)));

        pstIspCtx->isp_trans_info.update_info.phy_addr = 0;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_GetDCFInfo
 Description     : get dcf info from isp firmware
*****************************************************************************/
HI_S32 HI_MPI_ISP_GetDCFInfo(VI_PIPE ViPipe, ISP_DCF_INFO_S *pstIspDCF)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_BOOL bTempMap = HI_FALSE;
    HI_U64 u64PhyAddrHigh;
    HI_U64 u64PhyAddrTemp;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspDCF);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    u64PhyAddrHigh = (HI_U64)hi_ext_system_update_info_high_phyaddr_read(ViPipe);
    u64PhyAddrTemp = (HI_U64)hi_ext_system_update_info_low_phyaddr_read(ViPipe);
    u64PhyAddrTemp |= (u64PhyAddrHigh << 32);

    if (pstIspCtx->isp_trans_info.update_info.phy_addr == 0) {
        pstIspCtx->isp_trans_info.update_info.phy_addr = u64PhyAddrTemp;

        pstIspCtx->update_info_ctrl.isp_update_info = HI_MPI_SYS_Mmap(pstIspCtx->isp_trans_info.update_info.phy_addr, \
                                                       (sizeof(hi_isp_dcf_update_info) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(hi_isp_dcf_const_info)));

        if (pstIspCtx->update_info_ctrl.isp_update_info == HI_NULL) {
            ISP_ERR_TRACE("isp[%d] get dcf info mmap failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }

        pstIspCtx->update_info_ctrl.isp_dcf_const_info = \
                (hi_isp_dcf_const_info *)(pstIspCtx->update_info_ctrl.isp_update_info + ISP_MAX_UPDATEINFO_BUF_NUM);

        bTempMap = HI_TRUE;
    }

    memcpy(&pstIspDCF->stIspDCFConstInfo, pstIspCtx->update_info_ctrl.isp_dcf_const_info, sizeof(hi_isp_dcf_const_info));
    memcpy(&pstIspDCF->stIspDCFUpdateInfo, pstIspCtx->update_info_ctrl.isp_update_info, sizeof(hi_isp_dcf_update_info));

    if (bTempMap) {
        HI_MPI_SYS_Munmap(pstIspCtx->update_info_ctrl.isp_update_info,
                          (sizeof(hi_isp_dcf_update_info) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(hi_isp_dcf_const_info)));

        pstIspCtx->isp_trans_info.update_info.phy_addr = 0;
    }

    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_SetFrameInfo(VI_PIPE ViPipe, const ISP_FRAME_INFO_S *pstIspFrame)
{
    HI_S32 s32Ret;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx->frame_info_ctrl.isp_frame);
    ISP_CHECK_POINTER(pstIspFrame);
    ISP_CHECK_OPEN(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_FRAME_INFO_SET, pstIspFrame);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    memcpy(pstIspCtx->frame_info_ctrl.isp_frame, pstIspFrame, sizeof(hi_isp_frame_info));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetFrameInfo(VI_PIPE ViPipe, ISP_FRAME_INFO_S *pstIspFrame)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspFrame);
    ISP_CHECK_OPEN(ViPipe);

    if (ioctl(g_as32IspFd[ViPipe], ISP_FRAME_INFO_GET, pstIspFrame)) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
