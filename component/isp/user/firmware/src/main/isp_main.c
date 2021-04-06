/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include "isp_main.h"
#include "isp_ext_config.h"
#include "isp_defaults.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_statistics.h"
#include "isp_regcfg.h"
#include "isp_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 ISP_UpdateInfo(VI_PIPE ViPipe)
{
    HI_S32 i, s32Ret;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_CHECK_OPEN(ViPipe);

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (pstIspCtx->algs[i].used) {
            if (pstIspCtx->algs[i].alg_func.pfn_alg_ctrl != HI_NULL) {
                pstIspCtx->algs[i].alg_func.pfn_alg_ctrl(ViPipe, ISP_UPDATE_INFO_GET, &pstIspCtx->dcf_update_info);
            }
        }
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_UPDATE_INFO_SET, &pstIspCtx->dcf_update_info);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set dcf updateinfo failed\n", ViPipe);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_UpdateFrameInfo(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_FRAME_INFO_SET, &pstIspCtx->frame_info);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set frameinfo failed\n", ViPipe);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_UpdateAttachInfo(VI_PIPE ViPipe)
{
    HI_S32 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    if (pstIspCtx->attach_info_ctrl.attach_info == HI_NULL) {
        ISP_ERR_TRACE("pipe:%d the Isp attach Info hasn't init!\n", ViPipe);
        return HI_FAILURE;
    }

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (pstIspCtx->algs[i].used) {
            if (pstIspCtx->algs[i].alg_func.pfn_alg_ctrl != HI_NULL) {
                pstIspCtx->algs[i].alg_func.pfn_alg_ctrl(ViPipe, ISP_ATTACHINFO_GET, pstIspCtx->attach_info_ctrl.attach_info);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_UpdateColorGamutinfo(VI_PIPE ViPipe)
{
    HI_S32 i;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    if (pstIspCtx->gamut_info_ctrl.color_gamut_info == HI_NULL) {
        ISP_ERR_TRACE("pipe:%d the Isp colorGamut Info hasn't init!\n", ViPipe);
        return HI_FAILURE;
    }

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (pstIspCtx->algs[i].used) {
            if (pstIspCtx->algs[i].alg_func.pfn_alg_ctrl != HI_NULL) {
                pstIspCtx->algs[i].alg_func.pfn_alg_ctrl(ViPipe, ISP_COLORGAMUTINFO_GET,
                                                             pstIspCtx->gamut_info_ctrl.color_gamut_info);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ReadExtregs(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_U64 u64PhyAddrHigh;
    HI_U64 u64PhyAddrTemp;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddrHigh = (HI_U64)hi_ext_system_sys_debug_high_addr_read(ViPipe);
    u64PhyAddrTemp = (HI_U64)hi_ext_system_sys_debug_low_addr_read(ViPipe);
    u64PhyAddrTemp |= (u64PhyAddrHigh << 32);

    pstIspCtx->freeze_fw = hi_ext_system_freeze_firmware_read(ViPipe);

    pstIspCtx->isp_dbg.debug_en = hi_ext_system_sys_debug_enable_read(ViPipe);
    pstIspCtx->isp_dbg.phy_addr = u64PhyAddrTemp;

    pstIspCtx->isp_dbg.depth = hi_ext_system_sys_debug_depth_read(ViPipe);
    pstIspCtx->isp_dbg.size  = hi_ext_system_sys_debug_size_read(ViPipe);
    pstIspCtx->be_raw_info.enable_be_raw = hi_ext_system_isp_raw_position_enable_read(ViPipe);

    return HI_SUCCESS;
}

HI_S32 ISP_UpdateExtRegs(VI_PIPE ViPipe)
{
    return HI_SUCCESS;
}

HI_S32 ISP_SwitchImageMode(VI_PIPE ViPipe)
{
    HI_U8 u8WDRMode;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S stSnsImageMode;
    ISP_CMOS_SENSOR_IMAGE_MODE_S stIspImageMode;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstIspCtx->sns_image_mode.width  = hi_ext_top_sensor_width_read(ViPipe);
    pstIspCtx->sns_image_mode.height = hi_ext_top_sensor_height_read(ViPipe);
    u32Value = hi_ext_system_fps_base_read(ViPipe);
    pValue = (HI_VOID *)&u32Value;
    pstIspCtx->sns_image_mode.fps    = *(HI_FLOAT *)pValue;
    pstIspCtx->sns_image_mode.sns_mode = hi_ext_system_sensor_mode_read(ViPipe);

    pstIspCtx->sys_rect.x      = hi_ext_system_corp_pos_x_read(ViPipe);
    pstIspCtx->sys_rect.y      = hi_ext_system_corp_pos_y_read(ViPipe);
    pstIspCtx->sys_rect.width  = hi_ext_sync_total_width_read(ViPipe);
    pstIspCtx->sys_rect.height = hi_ext_sync_total_height_read(ViPipe);

    stIspImageMode.u16Width  = hi_ext_sync_total_width_read(ViPipe);
    stIspImageMode.u16Height = hi_ext_sync_total_height_read(ViPipe);
    stIspImageMode.f32Fps    = pstIspCtx->sns_image_mode.fps;
    stIspImageMode.u8SnsMode = pstIspCtx->sns_image_mode.sns_mode;

    ioctl(g_as32IspFd[ViPipe], ISP_GET_HDR_ATTR, &pstIspCtx->hdr_attr);

    if (pstIspCtx->isp_image_mode_flag == 0) {
        ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_AE_FPS_BASE_SET,
                     (HI_VOID *)&pstIspCtx->sns_image_mode.fps);

        memcpy(&pstIspCtx->pre_sns_image_mode, &pstIspCtx->sns_image_mode, sizeof(isp_sensor_image_mode));

        pstIspCtx->isp_image_mode_flag = 1;
    } else {
        if ((pstIspCtx->sns_image_mode.width  != pstIspCtx->pre_sns_image_mode.width) ||
            (pstIspCtx->sns_image_mode.height != pstIspCtx->pre_sns_image_mode.height) ||
            (pstIspCtx->sns_image_mode.fps    != pstIspCtx->pre_sns_image_mode.fps) ||
            (pstIspCtx->sns_image_mode.sns_mode != pstIspCtx->pre_sns_image_mode.sns_mode)) {
            s32Ret = ISP_BlockUpdate(ViPipe, &pstIspCtx->block_attr);
            if (s32Ret != HI_SUCCESS) {
                ISP_ERR_TRACE("pipe:%d Update ISP Block Attr Failed !\n", ViPipe);
                return s32Ret;
            }

            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_PIPE_SIZE, &pstIspCtx->pipe_size);
            if (s32Ret != HI_SUCCESS) {
                ISP_ERR_TRACE("ISP[%d] get Pipe size failed\n", ViPipe);
                return s32Ret;
            }

            /* p2en info */
            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_P2EN_INFO_GET, &pstIspCtx->isp0_p2_en);
            if (s32Ret != HI_SUCCESS) {
                ISP_ERR_TRACE("ISP[%d] get p2en info failed\n", ViPipe);
                return s32Ret;
            }

            stSnsImageMode.u16Width  = pstIspCtx->sns_image_mode.width;
            stSnsImageMode.u16Height = pstIspCtx->sns_image_mode.height;
            stSnsImageMode.f32Fps    = pstIspCtx->sns_image_mode.fps;
            stSnsImageMode.u8SnsMode = pstIspCtx->sns_image_mode.sns_mode;

            memcpy(&pstIspCtx->pre_sns_image_mode, &pstIspCtx->sns_image_mode, sizeof(isp_sensor_image_mode));

            s32Ret = ISP_SensorSetImageMode(ViPipe, &stSnsImageMode);

            if (s32Ret == HI_SUCCESS) {  /* Need to init sensor */
                u8WDRMode = hi_ext_system_sensor_wdr_mode_read(ViPipe);

                s32Ret = ISP_SensorSetWDRMode(ViPipe, u8WDRMode);
                if (s32Ret != HI_SUCCESS) {
                    ISP_ERR_TRACE("ISP[%d] set sensor wdr mode failed\n", ViPipe);
                    return s32Ret;
                }

                pstIspCtx->para_rec.stitch_sync = HI_FALSE;
                s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_INIT_SET, &pstIspCtx->para_rec.stitch_sync);
                if (s32Ret != HI_SUCCESS) {
                    ISP_ERR_TRACE("ISP[%d] set isp stitch sync failed!\n", ViPipe);
                }

                if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) {
                    ioctl(g_as32IspFd[ViPipe], ISP_SYNC_STITCH_PARAM_INIT);
                }

                s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_RES_SWITCH_SET);
                if (s32Ret != HI_SUCCESS) {
                    ISP_ERR_TRACE("pipe:%d Config ISP Res Switch Failed with ec %#x!\n", ViPipe, s32Ret);
                    return s32Ret;
                }

                s32Ret = ISP_RegCfgCtrl(ViPipe);
                if (s32Ret != HI_SUCCESS) {
                    return s32Ret;
                }

                s32Ret = ISP_GetBeLastBuf(ViPipe);
                if (s32Ret != HI_SUCCESS) {
                    ISP_ERR_TRACE("Pipe:%d Get be last bufs failed %x!\n", ViPipe, s32Ret);
                    return s32Ret;
                }

                ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_CHANGE_IMAGE_MODE_SET, (HI_VOID *)&stIspImageMode);
                ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_AE_FPS_BASE_SET,
                             (HI_VOID *)&pstIspCtx->sns_image_mode.fps);
                ISP_SensorUpdateSnsReg(ViPipe);

                if (IS_FS_WDR_MODE(u8WDRMode)) {
                    s32Ret = ISP_WDRCfgSet(ViPipe);
                    if (s32Ret != HI_SUCCESS) {
                        ISP_ERR_TRACE("pipe:%d ISP_WDRCfgSet ec %#x!\n", ViPipe, s32Ret);
                        return s32Ret;
                    }
                }

                s32Ret = ISP_SwitchRegSet(ViPipe);
                if (s32Ret != HI_SUCCESS) {
                    ISP_ERR_TRACE("pipe:%d set reg config failed!\n", ViPipe);
                    return HI_FAILURE;
                }

                s32Ret = ISP_SensorSwitch(ViPipe);
                if (s32Ret != HI_SUCCESS) {
                    ISP_ERR_TRACE("pipe:%d init sensor failed!\n", ViPipe);
                    return HI_FAILURE;
                }
            } else {
                ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_AE_FPS_BASE_SET, (HI_VOID *)&pstIspCtx->sns_image_mode.fps);
            }

            hi_ext_top_res_switch_write(ViPipe, HI_TRUE);

            ISP_SensorUpdateDefault(ViPipe);
            isp_sensor_update_blc(ViPipe);
            isp_sensor_get_default(ViPipe, &sns_dft);

            pstIspCtx->frame_info_ctrl.isp_frame->sensor_id = sns_dft->sensor_mode.sensor_id;
            pstIspCtx->frame_info_ctrl.isp_frame->sensor_mode = sns_dft->sensor_mode.sensor_mode;

            pstIspCtx->frame_info.sensor_id   = sns_dft->sensor_mode.sensor_id;
            pstIspCtx->frame_info.sensor_mode = sns_dft->sensor_mode.sensor_mode;

            ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);

            memcpy(&pstIspCtx->dng_info_ctrl.isp_dng->dng_raw_format, &sns_dft->sensor_mode.dng_raw_format,
                   sizeof(hi_isp_dng_raw_format));

            if ((pstSnsRegsInfo->u8Cfg2ValidDelayMax > CFG2VLD_DLY_LIMIT) || (pstSnsRegsInfo->u8Cfg2ValidDelayMax < 1)) {
                ISP_ERR_TRACE("pipe:%d Delay of config to invalid is:0x%x\n", ViPipe, pstSnsRegsInfo->u8Cfg2ValidDelayMax);
                pstSnsRegsInfo->u8Cfg2ValidDelayMax = 1;
            }

            pstIspCtx->linkage.cfg2valid_delay_max = pstSnsRegsInfo->u8Cfg2ValidDelayMax;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_WDRCfgSet(VI_PIPE ViPipe)
{
    HI_U8  i;
    HI_S32 s32Ret;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_reg_cfg_attr       *pstRegCfg = HI_NULL;
    isp_wdr_cfg        stWDRCfg;

    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
    ISP_CHECK_POINTER(pstRegCfg);

    ISP_GET_CTX(ViPipe, pstIspCtx);
    isp_sensor_get_default(ViPipe, &sns_dft);
    memset(&stWDRCfg, 0, sizeof(isp_wdr_cfg));

    stWDRCfg.wdr_mode = pstIspCtx->sns_wdr_mode;

    for (i = 0; i < EXP_RATIO_NUM; i++) {
        stWDRCfg.exp_ratio[i] = CLIP3(sns_dft->wdr_switch_attr.exp_ratio[i], 0x40, 0xFFF);
    }

    memcpy(&stWDRCfg.wdr_reg_cfg, &pstRegCfg->reg_cfg.alg_reg_cfg[0].stWdrRegCfg.stSyncRegCfg,
           sizeof(ISP_FSWDR_SYNC_CFG_S));

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WDR_CFG_SET, &stWDRCfg);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("pipe:%d Config WDR register Failed with ec %#x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SwitchWDR(VI_PIPE ViPipe)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8  u8SensorWDRMode;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    ISP_GET_CTX(ViPipe, pstIspCtx);

    s32Ret = ISP_SttAddrInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init stt address failed!\n", ViPipe);
        return s32Ret;
    }

    /* 0. block attr change */
    s32Ret = ISP_BlockUpdate(ViPipe, &pstIspCtx->block_attr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Update ISP Block Attr Failed !\n", ViPipe);
        return s32Ret;
    }

    s32Ret = ISP_RegCfgCtrl(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    /* 1. switch sensor to WDR mode */
    u8SensorWDRMode = pstIspCtx->sns_wdr_mode;
    s32Ret = ISP_SensorSetWDRMode(ViPipe, u8SensorWDRMode);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set sensor WDR mode failed!\n", ViPipe);
        return HI_FAILURE;
    }

    /* 2. get last buffer, note: when sensor timing change, callback */
    if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) {
        ioctl(g_as32IspFd[ViPipe], ISP_SYNC_STITCH_PARAM_INIT);
    }

    s32Ret = ISP_GetBeLastBuf(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Get be last bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    ISP_SensorUpdateDefault(ViPipe);
    isp_sensor_update_blc(ViPipe);
    isp_sensor_get_blc(ViPipe, &sns_black_level);

    ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_AE_BLC_SET, (HI_VOID *)&sns_black_level->black_level[1]);

    /* 3. init the common part of extern registers and real registers */
    ISP_ExtRegsInitialize(ViPipe);
    ISP_RegsInitialize(ViPipe);

    /* 5. notify algs to switch WDR mode */
    ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_WDR_MODE_SET, (HI_VOID *)&u8SensorWDRMode);

    s32Ret = ISP_WDRCfgSet(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] ISP_WDRCfgSet error \n", ViPipe);
        return s32Ret;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_MODE_SWITCH_SET);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("pipe:%d Config ISP Mode Switch Failed with ec %#x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    s32Ret = ISP_SwitchRegSet(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set reg config failed!\n", ViPipe);
        return HI_FAILURE;
    }

    /* 6. switch sensor to WDR mode */
    ISP_SensorUpdateSnsReg(ViPipe);
    s32Ret = ISP_SensorSwitch(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init sensor failed!\n", ViPipe);
        return HI_FAILURE;
    }

    /* 7. get new sensor default param */
    isp_sensor_get_default(ViPipe, &sns_dft);

    pstIspCtx->frame_info_ctrl.isp_frame->sensor_id   = sns_dft->sensor_mode.sensor_id;
    pstIspCtx->frame_info_ctrl.isp_frame->sensor_mode = sns_dft->sensor_mode.sensor_mode;

    ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);

    if ((pstSnsRegsInfo->u8Cfg2ValidDelayMax > CFG2VLD_DLY_LIMIT) || (pstSnsRegsInfo->u8Cfg2ValidDelayMax < 1)) {
        ISP_ERR_TRACE("ISP[%d] Delay of config to invalid is:0x%x\n", ViPipe, pstSnsRegsInfo->u8Cfg2ValidDelayMax);
        pstSnsRegsInfo->u8Cfg2ValidDelayMax = 1;
    }

    pstIspCtx->linkage.cfg2valid_delay_max = pstSnsRegsInfo->u8Cfg2ValidDelayMax;

    pstIspCtx->pre_sns_wdr_mode = pstIspCtx->sns_wdr_mode;
    hi_ext_top_wdr_switch_write(ViPipe, HI_TRUE);

    return HI_SUCCESS;
}

static HI_S32 ISP_SwitchModeProcess(VI_PIPE ViPipe)
{
    HI_S32  s32Ret   = HI_SUCCESS;
    ISP_CMOS_SENSOR_IMAGE_MODE_S  stIspImageMode;
    isp_usr_ctx              *pstIspCtx        = HI_NULL;
    hi_isp_cmos_black_level  *sns_black_level = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    stIspImageMode.u16Width  = hi_ext_sync_total_width_read(ViPipe);
    stIspImageMode.u16Height = hi_ext_sync_total_height_read(ViPipe);
    stIspImageMode.f32Fps    = pstIspCtx->sns_image_mode.fps;
    stIspImageMode.u8SnsMode = pstIspCtx->sns_image_mode.sns_mode;

    /* 1. if switch wdr mode, should disable festt enable first */
    if (pstIspCtx->change_wdr_mode == HI_TRUE) {
        s32Ret = ISP_ResetFeSttEn(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] Reset FeStt enable failed\n", ViPipe);
            return s32Ret;
        }
    }

    /* 2.Get info: WDR, HDR, Stitch */
    /* WDR attribute */
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_WDR_ATTR, &pstIspCtx->wdr_attr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get WDR attr failed\n", ViPipe);
        return s32Ret;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_PIPE_SIZE, &pstIspCtx->pipe_size);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get Pipe size failed\n", ViPipe);
        return s32Ret;
    }

    /* p2en info */
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_P2EN_INFO_GET, &pstIspCtx->isp0_p2_en);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get p2en info failed\n", ViPipe);
        return s32Ret;
    }

    if (pstIspCtx->change_wdr_mode == HI_TRUE) {
        s32Ret = ISP_SttAddrInit(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] init stt address failed!\n", ViPipe);
            return s32Ret;
        }
    }

    s32Ret = ISP_BlockUpdate(ViPipe, &pstIspCtx->block_attr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Update ISP Block Attr Failed !\n", ViPipe);
        return s32Ret;
    }

    pstIspCtx->para_rec.stitch_sync = HI_FALSE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_INIT_SET, &pstIspCtx->para_rec.stitch_sync);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set isp stitch sync failed!\n", ViPipe);
    }

    if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) {
        ioctl(g_as32IspFd[ViPipe], ISP_SYNC_STITCH_PARAM_INIT);
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_RES_SWITCH_SET);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Config ISP Res Switch Failed with ec %#x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    s32Ret = ISP_RegCfgCtrl(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = ISP_GetBeLastBuf(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Get be last bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    ISP_SensorUpdateDefault(ViPipe);
    isp_sensor_update_blc(ViPipe);
    isp_sensor_get_blc(ViPipe, &sns_black_level);

    ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_AE_BLC_SET, (HI_VOID *)&sns_black_level->black_level[1]);

    /* init the common part of extern registers and real registers */
    ISP_ExtRegsInitialize(ViPipe);

    if (pstIspCtx->change_wdr_mode == HI_TRUE) {
        ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_WDR_MODE_SET, (HI_VOID *)&pstIspCtx->sns_wdr_mode);
    }

    if ((pstIspCtx->change_isp_res == HI_TRUE) || (pstIspCtx->change_image_mode == HI_TRUE)) {
        ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_CHANGE_IMAGE_MODE_SET, (HI_VOID *)&stIspImageMode);
    }
    ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_AE_FPS_BASE_SET,
                 (HI_VOID *)&pstIspCtx->sns_image_mode.fps);

    s32Ret = ISP_WDRCfgSet(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] ISP_WDRCfgSet error \n", ViPipe);
        return s32Ret;
    }

    s32Ret = ISP_SwitchRegSet(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set reg config failed!\n", ViPipe);
        return HI_FAILURE;
    }

    ISP_SensorUpdateSnsReg(ViPipe);
    s32Ret = ISP_SensorSwitch(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init sensor failed!\n", ViPipe);
        return HI_FAILURE;
    }

    pstIspCtx->pre_sns_wdr_mode = pstIspCtx->sns_wdr_mode;

    if (pstIspCtx->change_wdr_mode == HI_TRUE) {
        hi_ext_top_wdr_switch_write(ViPipe, HI_TRUE);
    }

    if ((pstIspCtx->change_isp_res == HI_TRUE) || (pstIspCtx->change_image_mode == HI_TRUE)) {
        hi_ext_top_res_switch_write(ViPipe, HI_TRUE);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SwitchRes(VI_PIPE ViPipe)
{
    HI_U8  u8WDRMode;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S  stIspImageMode;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8WDRMode = hi_ext_system_sensor_wdr_mode_read(ViPipe);
    u32Value  = hi_ext_system_fps_base_read(ViPipe);
    pValue    = (HI_VOID *)&u32Value;

    stIspImageMode.u16Width  = hi_ext_sync_total_width_read(ViPipe);
    stIspImageMode.u16Height = hi_ext_sync_total_height_read(ViPipe);
    stIspImageMode.f32Fps    = *(HI_FLOAT *)pValue;
    stIspImageMode.u8SnsMode = hi_ext_system_sensor_mode_read(ViPipe);

    s32Ret = ISP_BlockUpdate(ViPipe, &pstIspCtx->block_attr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Update ISP Block Attr Failed !\n", ViPipe);
        return s32Ret;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_PIPE_SIZE, &pstIspCtx->pipe_size);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get Pipe size failed\n", ViPipe);
        return s32Ret;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_P2EN_INFO_GET, &pstIspCtx->isp0_p2_en);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get p2en info failed\n", ViPipe);
        return s32Ret;
    }

    pstIspCtx->para_rec.stitch_sync = HI_FALSE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_INIT_SET, &pstIspCtx->para_rec.stitch_sync);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set isp stitch sync failed!\n", ViPipe);
    }

    if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) {
        ioctl(g_as32IspFd[ViPipe], ISP_SYNC_STITCH_PARAM_INIT);
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_RES_SWITCH_SET);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Config ISP Res Switch Failed with ec %#x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    s32Ret = ISP_RegCfgCtrl(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = ISP_GetBeLastBuf(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Get be last bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    ISP_SensorUpdateDefault(ViPipe);
    isp_sensor_update_blc(ViPipe);

    ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_CHANGE_IMAGE_MODE_SET, (HI_VOID *)&stIspImageMode);
    ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_AE_FPS_BASE_SET,
                 (HI_VOID *)&pstIspCtx->sns_image_mode.fps);

    if (IS_FS_WDR_MODE(u8WDRMode)) {
        s32Ret = ISP_WDRCfgSet(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] ISP_WDRCfgSet ec %#x!\n", ViPipe, s32Ret);
            return s32Ret;
        }
    }

    s32Ret = ISP_SwitchRegSet(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] set reg config failed!\n", ViPipe);
        return HI_FAILURE;
    }

    ISP_SensorUpdateSnsReg(ViPipe);
    s32Ret = ISP_SensorSwitch(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("pipe:%d init sensor failed!\n", ViPipe);
        return HI_FAILURE;
    }

    hi_ext_top_res_switch_write(ViPipe, HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 ISP_SwitchMode(VI_PIPE ViPipe)
{
    HI_BOOL bHDREnable;
    HI_U8   u8WDRMode, u8HDRMode;
    HI_S8   stitch_main_pipe;
    HI_U32  u32Value = 0;
    HI_S32  s32Ret   = HI_SUCCESS;
    HI_VOID   *pValue    = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    hi_isp_cmos_default  *sns_dft      = HI_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S  stSnsImageMode;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    u8WDRMode = hi_ext_system_sensor_wdr_mode_read(ViPipe);
    u32Value  = hi_ext_system_fps_base_read(ViPipe);
    pValue    = (HI_VOID *)&u32Value;

    pstIspCtx->sns_image_mode.fps    = *(HI_FLOAT *)pValue;
    pstIspCtx->sns_image_mode.width  = hi_ext_top_sensor_width_read(ViPipe);
    pstIspCtx->sns_image_mode.height = hi_ext_top_sensor_height_read(ViPipe);
    pstIspCtx->sns_image_mode.sns_mode = hi_ext_system_sensor_mode_read(ViPipe);
    pstIspCtx->sys_rect.x           = hi_ext_system_corp_pos_x_read(ViPipe);
    pstIspCtx->sys_rect.y           = hi_ext_system_corp_pos_y_read(ViPipe);

    if (pstIspCtx->isp_image_mode_flag == 0) {
        ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_AE_FPS_BASE_SET,
                     (HI_VOID *)&pstIspCtx->sns_image_mode.fps);

        memcpy(&pstIspCtx->pre_sns_image_mode, &pstIspCtx->sns_image_mode, sizeof(isp_sensor_image_mode));

        pstIspCtx->isp_image_mode_flag = 1;
    } else {
        if (pstIspCtx->sns_wdr_mode != u8WDRMode) {
            pstIspCtx->change_wdr_mode = HI_TRUE;
        }

        if ((pstIspCtx->sns_image_mode.width  != pstIspCtx->pre_sns_image_mode.width ) ||
            (pstIspCtx->sns_image_mode.height != pstIspCtx->pre_sns_image_mode.height) ||
            (pstIspCtx->sns_image_mode.fps    != pstIspCtx->pre_sns_image_mode.fps) ||
            (pstIspCtx->sns_image_mode.sns_mode != pstIspCtx->pre_sns_image_mode.sns_mode)) {
            pstIspCtx->change_image_mode = HI_TRUE;
        }

        if (pstIspCtx->sys_rect.width  != hi_ext_sync_total_width_read(ViPipe) ||
            pstIspCtx->sys_rect.height != hi_ext_sync_total_height_read(ViPipe)) {
            pstIspCtx->sys_rect.width  = hi_ext_sync_total_width_read(ViPipe);
            pstIspCtx->sys_rect.height = hi_ext_sync_total_height_read(ViPipe);

            pstIspCtx->change_isp_res = HI_TRUE;
        }

        if ((pstIspCtx->change_wdr_mode == HI_TRUE) ||
            (pstIspCtx->change_image_mode == HI_TRUE) ||
            (pstIspCtx->change_isp_res == HI_TRUE)) {
            /* stitch mode, should switch main pipe first*/
            if ((pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) && (pstIspCtx->stitch_attr.main_pipe == HI_FALSE)) {
                stitch_main_pipe = pstIspCtx->stitch_attr.stitch_bind_id[0];

                if ((pstIspCtx->change_wdr_mode == HI_TRUE) &&
                    hi_ext_top_wdr_switch_read(stitch_main_pipe) != HI_TRUE) {
                    return HI_SUCCESS;
                }
                if (((pstIspCtx->change_image_mode == HI_TRUE) ||
                     (pstIspCtx->change_isp_res == HI_TRUE)) &&
                    hi_ext_top_res_switch_read(stitch_main_pipe) != HI_TRUE) {
                    return HI_SUCCESS;
                }
            }

            /* HDR attribute */
            s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_HDR_ATTR, &pstIspCtx->hdr_attr);
            if (s32Ret != HI_SUCCESS) {
                ISP_ERR_TRACE("ISP[%d] get HDR attr failed\n", ViPipe);
                return s32Ret;
            }

            if (pstIspCtx->change_wdr_mode == HI_TRUE) {
                bHDREnable = (pstIspCtx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR) ? 1 : 0;
                u8HDRMode  = (((bHDREnable) & 0x1) << 0x6);
                u8HDRMode  = u8HDRMode | hi_ext_system_sensor_wdr_mode_read(ViPipe);

                s32Ret = ISP_SensorSetWDRMode(ViPipe, u8HDRMode);
                if (s32Ret != HI_SUCCESS) {
                    ISP_ERR_TRACE("ISP[%d] set sensor wdr mode failed\n", ViPipe);
                    return s32Ret;
                }
            }

            stSnsImageMode.u16Width  = pstIspCtx->sns_image_mode.width;
            stSnsImageMode.u16Height = pstIspCtx->sns_image_mode.height;
            stSnsImageMode.f32Fps    = pstIspCtx->sns_image_mode.fps;
            stSnsImageMode.u8SnsMode = pstIspCtx->sns_image_mode.sns_mode;

            memcpy(&pstIspCtx->pre_sns_image_mode, &pstIspCtx->sns_image_mode, sizeof(isp_sensor_image_mode));

            s32Ret = ISP_SensorSetImageMode(ViPipe, &stSnsImageMode);
            if (s32Ret == HI_SUCCESS) {  /* Need to init sensor */
                pstIspCtx->sns_wdr_mode = u8WDRMode;
                pstIspCtx->count      = 0;
                pstIspCtx->special_opt.fe_stt_update = HI_TRUE; /* used for fe statistics */

                s32Ret = ISP_SwitchModeProcess(ViPipe);
                if (s32Ret != HI_SUCCESS) {
                    ISP_ERR_TRACE("ISP[%d] Switch Mode failed!\n", ViPipe);
                    return s32Ret;
                }
            } else {
                if (pstIspCtx->change_isp_res == HI_TRUE) {
                    ISP_SwitchRes(ViPipe);
                }
                ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_AE_FPS_BASE_SET, (HI_VOID *)&pstIspCtx->sns_image_mode.fps);
                hi_ext_top_res_switch_write(ViPipe, HI_TRUE);
            }

            isp_sensor_get_default(ViPipe, &sns_dft);

            pstIspCtx->frame_info_ctrl.isp_frame->sensor_id   = sns_dft->sensor_mode.sensor_id;
            pstIspCtx->frame_info_ctrl.isp_frame->sensor_mode = sns_dft->sensor_mode.sensor_mode;

            ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);
            ISP_SwitchStateSet(ViPipe);

            memcpy(&pstIspCtx->dng_info_ctrl.isp_dng->dng_raw_format, &sns_dft->sensor_mode.dng_raw_format,
                   sizeof(hi_isp_dng_raw_format));

            if ((pstSnsRegsInfo->u8Cfg2ValidDelayMax > CFG2VLD_DLY_LIMIT) || (pstSnsRegsInfo->u8Cfg2ValidDelayMax < 1)) {
                ISP_ERR_TRACE("ISP[%d] Delay of config to invalid is:0x%x\n", ViPipe, pstSnsRegsInfo->u8Cfg2ValidDelayMax);
                pstSnsRegsInfo->u8Cfg2ValidDelayMax = 1;
            }

            pstIspCtx->linkage.cfg2valid_delay_max = pstSnsRegsInfo->u8Cfg2ValidDelayMax;
        }

        pstIspCtx->change_wdr_mode   = HI_FALSE;
        pstIspCtx->change_image_mode = HI_FALSE;
        pstIspCtx->change_isp_res    = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SwitchWDRMode(VI_PIPE ViPipe)
{
    HI_U8  u8WDRMode;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32WDRmode;
    HI_U32 u32Value = 0;
    HI_BOOL bHDREnable;
    ISP_CMOS_SENSOR_IMAGE_MODE_S stSnsImageMode;
    HI_VOID *pValue = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    u32WDRmode = hi_ext_system_sensor_wdr_mode_read(ViPipe);

    /* swtich linear/WDR mode, width/height, fps  */
    if (u32WDRmode != pstIspCtx->sns_wdr_mode) {
        pstIspCtx->sns_wdr_mode = u32WDRmode;
        pstIspCtx->count = 0;

        s32Ret = ISP_ResetFeSttEn(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] Reset FeStt enable failed\n", ViPipe);
            return s32Ret;
        }

        /* Get info: PipeBindDev, RunningMode, WDR, HDR, Stitch */
        /* WDR attribute */
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_WDR_ATTR, &pstIspCtx->wdr_attr);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] get WDR attr failed\n", ViPipe);
            return s32Ret;
        }

        /* HDR attribute */
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_HDR_ATTR, &pstIspCtx->hdr_attr);

        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] get HDR attr failed\n", ViPipe);
            return s32Ret;
        }

        bHDREnable = (pstIspCtx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR) ? 1 : 0;
        u8WDRMode = (((bHDREnable) & 0x1) << 0x6);
        u8WDRMode = u8WDRMode | hi_ext_system_sensor_wdr_mode_read(ViPipe);

        s32Ret = ISP_SensorSetWDRMode(ViPipe, u8WDRMode);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] set sensor wdr mode failed\n", ViPipe);
            return s32Ret;
        }

        stSnsImageMode.u16Width = hi_ext_top_sensor_width_read(ViPipe);
        stSnsImageMode.u16Height = hi_ext_top_sensor_height_read(ViPipe);
        u32Value = hi_ext_system_fps_base_read(ViPipe);
        pValue = (HI_VOID *)&u32Value;
        stSnsImageMode.f32Fps = *(HI_FLOAT *)pValue;
        stSnsImageMode.u8SnsMode = hi_ext_system_sensor_mode_read(ViPipe);
        s32Ret = ISP_SensorSetImageMode(ViPipe, &stSnsImageMode);

        pstIspCtx->mem_init = HI_TRUE;
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_MEM_INFO_SET, &pstIspCtx->mem_init);
        if (s32Ret != HI_SUCCESS) {
            pstIspCtx->mem_init = HI_FALSE;
            VReg_ReleaseAll(ViPipe);

            ISP_ERR_TRACE("ISP[%d] set Mem info failed!\n", ViPipe);
            return s32Ret;
        }

        pstIspCtx->sns_wdr_mode              = u32WDRmode;
        pstIspCtx->special_opt.fe_stt_update = HI_TRUE; /* used for fe statistics */

        s32Ret = ISP_SwitchWDR(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] Switch WDR failed!\n", ViPipe);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_UpdatePosGet(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_S32 s32Ret;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_UPDATE_POS_GET, &pstIspCtx->linkage.update_pos);
    if (s32Ret) {
        ISP_ERR_TRACE("pipe:%d get update pos %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_U32 ISP_FrameCntGet(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    HI_S32 s32Ret;
    HI_U32 u32FrmCnt = 0;
    ISP_GET_CTX(ViPipe, pstIspCtx);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_FRAME_CNT_GET, &u32FrmCnt);
    if (s32Ret) {
        ISP_ERR_TRACE("pipe:%d get update pos %x!\n", ViPipe, s32Ret);
        return pstIspCtx->linkage.iso_done_frm_cnt;
    }

    return u32FrmCnt;
}

HI_S32 ISP_SnapAttrGet(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_snap_attr stIspSnapAttr;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_working_mode stIspWorkMode;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SNAP_ATTR_GET, &stIspSnapAttr);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    memcpy(&pstIspCtx->pro_param, &stIspSnapAttr.pro_param, sizeof(isp_pro_param));
    pstIspCtx->linkage.snap_type = stIspSnapAttr.snap_type;
    if ((stIspSnapAttr.picture_pipe_id == stIspSnapAttr.preview_pipe_id) && (stIspSnapAttr.preview_pipe_id!= -1)) {
        pstIspCtx->linkage.snap_pipe_mode = ISP_SNAP_PREVIEW_PICTURE;
    } else if (stIspSnapAttr.picture_pipe_id == ViPipe) {
        pstIspCtx->linkage.snap_pipe_mode = ISP_SNAP_PICTURE;
    } else if (stIspSnapAttr.preview_pipe_id == ViPipe) {
        pstIspCtx->linkage.snap_pipe_mode = ISP_SNAP_PREVIEW;
    } else {
        pstIspCtx->linkage.snap_pipe_mode = ISP_SNAP_NONE;
    }
    pstIspCtx->linkage.load_ccm = stIspSnapAttr.load_ccm;
    pstIspCtx->linkage.picture_pipe_id = stIspSnapAttr.picture_pipe_id;
    pstIspCtx->linkage.preview_pipe_id = stIspSnapAttr.preview_pipe_id;

    if (stIspSnapAttr.picture_pipe_id != -1) {
        s32Ret = ioctl(g_as32IspFd[stIspSnapAttr.picture_pipe_id], ISP_WORK_MODE_GET, &stIspWorkMode);

        if (s32Ret) {
            ISP_ERR_TRACE("get isp work mode failed!\n");
            return s32Ret;
        }
        pstIspCtx->linkage.picture_running_mode = stIspWorkMode.running_mode;
    } else {
        pstIspCtx->linkage.picture_running_mode = ISP_MODE_RUNNING_BUTT;
    }

    if (stIspSnapAttr.picture_pipe_id == stIspSnapAttr.preview_pipe_id) {
        pstIspCtx->linkage.preview_running_mode = pstIspCtx->linkage.picture_running_mode;
    } else {
        if (stIspSnapAttr.preview_pipe_id != -1) {
            s32Ret = ioctl(g_as32IspFd[stIspSnapAttr.preview_pipe_id], ISP_WORK_MODE_GET, &stIspWorkMode);

            if (s32Ret) {
                ISP_ERR_TRACE("get isp work mode failed!\n");
                return s32Ret;
            }
            pstIspCtx->linkage.preview_running_mode = stIspWorkMode.running_mode;
        } else {
            pstIspCtx->linkage.preview_running_mode = ISP_MODE_RUNNING_BUTT;
        }
    }

    return HI_SUCCESS;
}

HI_U8 ISP_IsoIndexCal(VI_PIPE ViPipe, HI_U32 u32AeDoneFrmCnt)
{
    HI_U8 u8Index;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8Index = pstIspCtx->linkage.cfg2valid_delay_max;

    if (pstIspCtx->linkage.iso_done_frm_cnt > u32AeDoneFrmCnt) {  // preview pipe the last frame dont run complete.
        u8Index = u8Index - (pstIspCtx->linkage.iso_done_frm_cnt - u32AeDoneFrmCnt);
    } else if (pstIspCtx->linkage.iso_done_frm_cnt < u32AeDoneFrmCnt) { // the preview pipe run first.
        u8Index = u8Index + ((u32AeDoneFrmCnt - pstIspCtx->linkage.iso_done_frm_cnt) - 1);
    } else if (pstIspCtx->linkage.iso_done_frm_cnt == u32AeDoneFrmCnt) { // the picture pipe run first.
        u8Index = u8Index - 1;
    }

    if (0 < pstIspCtx->linkage.update_pos) {
        u8Index = u8Index - 1;
    }

    u8Index = MIN2(u8Index, ISP_SYNC_ISO_BUF_MAX - 1);

    return u8Index;
}

HI_S32 ISP_SnapPreProcess(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_usr_ctx *pstPreviewIspCtx = HI_NULL;
    hi_snap_type enSnapType;
    isp_snap_pipe_mode enSnapPipeMode;
    HI_U8 u8Index = 0;
    HI_U32 u32PrePipeFrmCnt = 0;
    HI_U8 i;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_SnapAttrGet(ViPipe);

    enSnapType = pstIspCtx->linkage.snap_type;
    enSnapPipeMode = pstIspCtx->linkage.snap_pipe_mode;

    if (enSnapPipeMode == ISP_SNAP_NONE) {
        return HI_SUCCESS;
    }

    if ((pstIspCtx->linkage.preview_pipe_id > -1) && (pstIspCtx->linkage.preview_pipe_id < ISP_MAX_PIPE_NUM)) {
        ISP_GET_CTX(pstIspCtx->linkage.preview_pipe_id, pstPreviewIspCtx);
        u32PrePipeFrmCnt = pstPreviewIspCtx->linkage.iso_done_frm_cnt;
    }

    if (((enSnapPipeMode == ISP_SNAP_PICTURE) || (enSnapPipeMode == ISP_SNAP_PREVIEW_PICTURE)) &&
        (IS_OFFLINE_MODE(pstIspCtx->block_attr.running_mode) ||
         IS_STRIPING_MODE(pstIspCtx->block_attr.running_mode))) {
        ISP_SnapRegCfgGet(ViPipe, &pstIspCtx->snap_isp_info);

        if (pstIspCtx->snap_isp_info.snap_state == SNAP_STATE_CFG) {
            pstIspCtx->linkage.snap_state = HI_TRUE;
            pstIspCtx->linkage.iso = pstIspCtx->snap_isp_info.isp_cfg_info.iso;
            pstIspCtx->linkage.isp_dgain = pstIspCtx->snap_isp_info.isp_cfg_info.isp_dgain;
            pstIspCtx->linkage.color_temp = pstIspCtx->snap_isp_info.isp_cfg_info.color_temperature;
            pstIspCtx->linkage.pro_index = pstIspCtx->snap_isp_info.pro_index;
        } else {
            pstIspCtx->linkage.snap_state = HI_FALSE;
            pstIspCtx->linkage.pro_index = 0;
        }
    }

    if ((enSnapPipeMode == ISP_SNAP_PREVIEW) || (enSnapPipeMode == ISP_SNAP_PREVIEW_PICTURE)) {
        if (enSnapType == SNAP_TYPE_PRO) {
            pstIspCtx->linkage.pro_trigger = ISP_ProTriggerGet(ViPipe);
            if (pstIspCtx->linkage.pro_trigger == HI_TRUE) {
                ISP_AlgsCtrl(pstIspCtx->algs, ViPipe, ISP_PROTRIGGER_SET, (HI_VOID *)&pstIspCtx->pro_param);
            }
        }

        for (i = ISP_SYNC_ISO_BUF_MAX - 1; i >= 1; i--) {
            pstIspCtx->linkage.pro_index_buf[i] = pstIspCtx->linkage.pro_index_buf[i - 1];
        }
        if ((pstIspCtx->linkage.pro_trigger == HI_TRUE) ||
            ((pstIspCtx->pro_frm_cnt > 0) && (pstIspCtx->pro_frm_cnt < pstIspCtx->pro_param.pro_frame_num))) {
            pstIspCtx->linkage.pro_index_buf[0] = pstIspCtx->pro_frm_cnt + 1;
        } else {
            pstIspCtx->linkage.pro_index_buf[0] = 0;
        }
        pstIspCtx->linkage.pro_index = pstIspCtx->linkage.pro_index_buf[pstIspCtx->linkage.cfg2valid_delay_max];
        if (0 < pstIspCtx->linkage.update_pos) {
            pstIspCtx->linkage.pro_index = pstIspCtx->linkage.pro_index_buf[pstIspCtx->linkage.cfg2valid_delay_max - 1];
        }
    }

    if (IS_ONLINE_MODE(pstIspCtx->block_attr.running_mode) ||
        IS_SIDEBYSIDE_MODE(pstIspCtx->block_attr.running_mode)) {
        if (enSnapPipeMode == ISP_SNAP_PICTURE) {
            u8Index = ISP_IsoIndexCal(ViPipe, u32PrePipeFrmCnt);
            if (pstPreviewIspCtx != HI_NULL) {
                pstIspCtx->linkage.iso = pstPreviewIspCtx->linkage.sync_iso_buf[u8Index];
                pstIspCtx->linkage.pro_index = pstPreviewIspCtx->linkage.pro_index_buf[u8Index];
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SaveSnapInfo(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    hi_isp_config_info stSnapInfo;
    HI_U32 i;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    stSnapInfo.color_temperature = pstIspCtx->linkage.color_temp;
    stSnapInfo.iso = pstIspCtx->linkage.sync_iso_buf[0];
    stSnapInfo.isp_dgain = pstIspCtx->linkage.isp_dgain;
    stSnapInfo.exposure_time = pstIspCtx->linkage.int_time;
    stSnapInfo.white_balance_gain[0] = (pstIspCtx->linkage.white_balance_gain[0] + 128) >> 8;
    stSnapInfo.white_balance_gain[1] = (pstIspCtx->linkage.white_balance_gain[1] + 128) >> 8;
    stSnapInfo.white_balance_gain[2] = (pstIspCtx->linkage.white_balance_gain[2] + 128) >> 8;
    stSnapInfo.white_balance_gain[3] = (pstIspCtx->linkage.white_balance_gain[3] + 128) >> 8;

    for (i = 0; i < ISP_CAP_CCM_NUM; i++) {
        stSnapInfo.cap_ccm[i] = pstIspCtx->linkage.ccm[i];
    }

    ISP_SnapRegCfgSet(ViPipe, &stSnapInfo);

    return HI_SUCCESS;
}

HI_S32 isp_set_smart_info(VI_PIPE ViPipe, const ISP_SMART_INFO_S *smart_info)
{
    HI_U32 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(smart_info);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        hi_ext_system_smart_enable_write(ViPipe, i, smart_info->stROI[i].bEnable);
        hi_ext_system_smart_available_write(ViPipe, i, smart_info->stROI[i].bAvailable);
        hi_ext_system_smart_luma_write(ViPipe, i, smart_info->stROI[i].u8Luma);
    }

    hi_ext_system_smart_update_write(ViPipe, HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 isp_get_smart_info(VI_PIPE ViPipe, ISP_SMART_INFO_S *smart_info)
{
    HI_U32 i;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(smart_info);
    ISP_CHECK_OPEN(ViPipe);
    ISP_CHECK_MEM_INIT(ViPipe);

    for (i = 0; i < SMART_CLASS_MAX; i++) {
        smart_info->stROI[i].bEnable    = hi_ext_system_smart_enable_read(ViPipe, i);
        smart_info->stROI[i].bAvailable = hi_ext_system_smart_available_read(ViPipe, i);
        smart_info->stROI[i].u8Luma     = hi_ext_system_smart_luma_read(ViPipe, i);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SetProCalcDone(VI_PIPE ViPipe)
{
    HI_S32 s32Ret = 0;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SET_PROCALCDONE);

    if (s32Ret) {
        ISP_ERR_TRACE("set isp pro calculate done failed!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_StitchSyncExit(VI_PIPE ViPipe)
{
    HI_U32  k;
    VI_PIPE ViPipeS;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    /* 1. check status */
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    if (( pstIspCtx->stitch_attr.stitch_enable == HI_TRUE) && (pstIspCtx->stitch_attr.main_pipe != HI_TRUE )) {
        for (k = 0; k < pstIspCtx->stitch_attr.stitch_pipe_num; k++) {
            ViPipeS = pstIspCtx->stitch_attr.stitch_bind_id[k];
            ISP_GET_CTX(ViPipeS, pstIspCtx);

            while ((pstIspCtx->stitch_attr.main_pipe == HI_TRUE) && (pstIspCtx->mem_init != HI_FALSE )) {
                usleep(10);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_StitchSyncRun(VI_PIPE ViPipe)
{
    HI_U8 k;
    VI_PIPE ViPipeId;
    isp_usr_ctx *pstIspCtx  = HI_NULL;
    isp_usr_ctx *pstIspCtxS = HI_NULL;

    /* 1. check status */
    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE ) {
        if (pstIspCtx->stitch_attr.main_pipe == HI_TRUE ) {
            for (k = 0; k < pstIspCtx->stitch_attr.stitch_pipe_num; k++) {
                ViPipeId = pstIspCtx->stitch_attr.stitch_bind_id[k];
                ISP_GET_CTX(ViPipeId, pstIspCtxS);

                if (pstIspCtxS->mem_init != HI_TRUE) {
                    return HI_FAILURE;
                }

                if (pstIspCtxS->para_rec.init != HI_TRUE) {
                    return HI_FAILURE;
                }
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_StitchIsoSync(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;
    isp_usr_ctx *pstMainIspCtx = HI_NULL;
    HI_U8 u8Index;
    HI_U32 u32MainPipeFrmCnt;
    VI_PIPE MainPipe;
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->stitch_attr.stitch_enable == HI_TRUE ) {
        if (pstIspCtx->stitch_attr.main_pipe != HI_TRUE ) {
            MainPipe = pstIspCtx->stitch_attr.stitch_bind_id[0];
            ISP_CHECK_PIPE(MainPipe);
            ISP_GET_CTX(ViPipe, pstMainIspCtx);

            u32MainPipeFrmCnt = pstMainIspCtx->linkage.iso_done_frm_cnt;
            u8Index = ISP_IsoIndexCal(ViPipe, u32MainPipeFrmCnt);
            pstIspCtx->linkage.iso = pstMainIspCtx->linkage.sync_iso_buf[u8Index];
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_Run(VI_PIPE ViPipe)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_VOID *pStat = HI_NULL;
    HI_VOID *pRegCfg = HI_NULL;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    s32Ret = ISP_StitchSyncRun(ViPipe);
    if (s32Ret != HI_SUCCESS ) {
        return HI_SUCCESS;
    }

    /*  get isp BeBuf info. */
    s32Ret = ISP_GetBeFreeBuf(ViPipe);
    if (s32Ret != HI_SUCCESS ) {
        return s32Ret;
    }

    /*  get statistics buf info. */
    s32Ret = ISP_StatisticsGetBuf(ViPipe, &pStat);
    if (s32Ret != HI_SUCCESS ) {
        if (pstIspCtx->frame_cnt != 0) {
            return s32Ret;
        }

        pstIspCtx->linkage.stat_ready  = HI_FALSE;
    } else {
        pstIspCtx->linkage.stat_ready  = HI_TRUE;
    }

    s32Ret = ISP_SetCfgBeBufState(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    /* get regcfg */
    ISP_GetRegCfgCtx(ViPipe, &pRegCfg);

    ISP_ReadExtregs(ViPipe);

    if (pstIspCtx->freeze_fw) {
        ISP_RegCfgInfoSet(ViPipe);
        return HI_SUCCESS;
    }

    ISP_UpdatePosGet(ViPipe);

    ISP_SnapPreProcess(ViPipe);

    ISP_StitchIsoSync(ViPipe);

    pstIspCtx->frame_cnt++;

    ISP_DbgRunBgn(&pstIspCtx->isp_dbg, pstIspCtx->frame_cnt);

    ISP_AlgsRun(pstIspCtx->algs, ViPipe, pStat, pRegCfg, 0);

    /* update info */
    if (pstIspCtx->linkage.snap_pipe_mode != ISP_SNAP_PICTURE) {
        ISP_UpdateInfo(ViPipe);

        ISP_UpdateFrameInfo(ViPipe);
    }

    ISP_ProcWrite(pstIspCtx->algs, ViPipe);

    ISP_UpdateAttachInfo(ViPipe);

    ISP_UpdateColorGamutinfo(ViPipe);

    ISP_DbgRunEnd(&pstIspCtx->isp_dbg, pstIspCtx->frame_cnt);

    /* release statistics buf info. */
    if (pstIspCtx->linkage.stat_ready  == HI_TRUE) {
        ISP_StatisticsPutBuf(ViPipe);
    }

    /* record the register config infomation to fhy and kernel,and be valid in next frame. */
    s32Ret = ISP_RegCfgInfoSet(ViPipe);
    if (s32Ret != HI_SUCCESS ) {
        return s32Ret;
    }

    if (((pstIspCtx->pro_frm_cnt > 0) && (pstIspCtx->pro_frm_cnt < pstIspCtx->pro_param.pro_frame_num + 4)) ||
        ( pstIspCtx->linkage.pro_trigger == HI_TRUE)) {
        pstIspCtx->pro_frm_cnt++;
    } else {
        pstIspCtx->pro_frm_cnt = 0;
    }

    if ((pstIspCtx->frame_cnt % DIV_0_TO_1(pstIspCtx->linkage.ae_run_interval)  == 0) ||
        (pstIspCtx->pro_frm_cnt > 0)) {
        if (!pstIspCtx->linkage.defect_pixel) {
            ISP_SyncCfgSet(ViPipe);
        }
    }

    ISP_UpdateExtRegs(ViPipe);

    /* save capture info */
    ISP_SaveSnapInfo(ViPipe);

    ISP_UpdateDngImageDynamicInfo(ViPipe);

    /* pro snap mode, ae calculate done */
    if (pstIspCtx->pro_frm_cnt == 1) {
        ISP_SetProCalcDone(ViPipe);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_Exit(VI_PIPE ViPipe)
{
    HI_U8  i;
    HI_S32 s32Ret = HI_SUCCESS;
    isp_usr_ctx *pstIspCtx = HI_NULL;
    const HI_U64 u64Handsignal = ISP_EXIT_HAND_SIGNAL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(ViPipe);

    /* Set handsignal */
    if (ioctl(g_as32IspFd[ViPipe], ISP_RUN_STATE_SET, &u64Handsignal)) {
        ISP_ERR_TRACE("ISP[%d] set run state failed!\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    memset(&pstIspCtx->para_rec, 0, sizeof(isp_para_rec));

    hi_ext_top_wdr_cfg_write(ViPipe, pstIspCtx->para_rec.wdr_cfg);
    hi_ext_top_pub_attr_cfg_write(ViPipe, pstIspCtx->para_rec.pub_cfg);

    if (pstIspCtx->isp_yuv_mode == HI_FALSE) {
        s32Ret |= ISP_SensorExit(ViPipe);
    }

    ISP_AlgsExit(ViPipe, pstIspCtx->algs);

    ISP_AlgsUnRegister(ViPipe);

    s32Ret |= ISP_RegCfgExit(ViPipe);
    s32Ret |= ISP_ProShpParamExit(ViPipe);
    s32Ret |= ISP_ProNrParamExit(ViPipe);
    s32Ret |= ISP_ProInfoExit(ViPipe);
    s32Ret |= ISP_DngInfoExit(ViPipe);
    s32Ret |= ISP_ColorGamutInfoExit(ViPipe);
    s32Ret |= ISP_AttachInfoExit(ViPipe);
    s32Ret |= ISP_FrameInfoExit(ViPipe);
    s32Ret |= ISP_UpdateInfoExit(ViPipe);
    s32Ret |= ISP_TransInfoExit(ViPipe);
    s32Ret |= ISP_ProcExit(ViPipe);
    s32Ret |= ISP_StatisticsExit(ViPipe);
    s32Ret |= ISP_SpecAwbBufExit(ViPipe);
    s32Ret |= ISP_ClutBufExit(ViPipe);
    s32Ret |= ISP_SttBufExit(ViPipe);
    s32Ret |= ISP_CfgBeBufExit(ViPipe);
    s32Ret |= ISP_LdciBufExit(ViPipe);
    s32Ret |= ISP_BlockExit(ViPipe);
    /* exit global variables */
    pstIspCtx->mem_init                 = HI_FALSE;
    pstIspCtx->para_rec.init        = HI_FALSE;
    pstIspCtx->para_rec.stitch_sync = HI_FALSE;
    pstIspCtx->linkage.run_once         = HI_FALSE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        pstIspCtx->isp_be_virt_addr[i]  = HI_NULL;
        pstIspCtx->viproc_virt_addr[i] = HI_NULL;
    }

    if (ioctl(g_as32IspFd[ViPipe], ISP_RESET_CTX)) {
        ISP_ERR_TRACE("ISP[%d] reset ctx failed!\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* release vregs */
    VReg_Munmap(ISP_FE_REG_BASE(ViPipe), FE_REG_SIZE_ALIGN);
    VReg_Munmap(ISP_BE_REG_BASE(ViPipe), BE_REG_SIZE_ALIGN);
    VReg_Exit(ViPipe, ISP_VIR_REG_BASE(ViPipe), ISP_VREG_SIZE);
    VReg_ReleaseAll(ViPipe);

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
