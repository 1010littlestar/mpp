/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <stdio.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#include "isp_frameinfo.h"
#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

extern HI_S32 g_as32IspFd[ISP_MAX_PIPE_NUM];

HI_S32 ISP_TransInfoInit(VI_PIPE ViPipe, isp_trans_info_buf *pstTransInfo)
{
    HI_S32 s32Ret;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstTransInfo);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_TRANS_BUF_INIT, pstTransInfo);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("isp[%d] init trans info bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_TransInfoExit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    ISP_CHECK_PIPE(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_TRANS_BUF_EXIT);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("isp[%d] exit trans info buf failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_FrameInfoInit(VI_PIPE ViPipe)
{
    HI_U64 u64PhyAddr;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->isp_trans_info.frame_info.phy_addr;

    pstIspCtx->frame_info_ctrl.isp_frame = HI_MPI_SYS_Mmap(u64PhyAddr,
                                                           sizeof(hi_isp_frame_info) * ISP_MAX_FRAMEINFO_BUF_NUM);

    if (pstIspCtx->frame_info_ctrl.isp_frame== HI_NULL) {
        ISP_ERR_TRACE("isp[%d] mmap frame info buf failed!\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_FrameInfoExit(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->frame_info_ctrl.isp_frame != HI_NULL) {
        HI_MPI_SYS_Munmap(pstIspCtx->frame_info_ctrl.isp_frame, sizeof(hi_isp_frame_info) * ISP_MAX_FRAMEINFO_BUF_NUM);
        pstIspCtx->frame_info_ctrl.isp_frame = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AttachInfoInit(VI_PIPE ViPipe)
{
    HI_U64 u64PhyAddr;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->isp_trans_info.atta_info.phy_addr;

    pstIspCtx->attach_info_ctrl.attach_info = HI_MPI_SYS_Mmap(u64PhyAddr, sizeof(hi_isp_attach_info));

    if (pstIspCtx->attach_info_ctrl.attach_info == HI_NULL) {
        ISP_ERR_TRACE("isp[%d] mmap attach info buf failed!\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AttachInfoExit(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->attach_info_ctrl.attach_info != HI_NULL) {
        HI_MPI_SYS_Munmap(pstIspCtx->attach_info_ctrl.attach_info, sizeof(hi_isp_attach_info));
        pstIspCtx->attach_info_ctrl.attach_info = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ColorGamutInfoInit(VI_PIPE ViPipe)
{
    HI_U64 u64PhyAddr;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->isp_trans_info.color_gammut_info.phy_addr;

    pstIspCtx->gamut_info_ctrl.color_gamut_info = HI_MPI_SYS_Mmap(u64PhyAddr, sizeof(hi_isp_colorgammut_info));

    if (pstIspCtx->gamut_info_ctrl.color_gamut_info == HI_NULL) {
        ISP_ERR_TRACE("isp[%d] mmap color gamut info buf failed!\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    pstIspCtx->gamut_info_ctrl.color_gamut_info->color_gamut = COLOR_GAMUT_BT709;

    return HI_SUCCESS;
}

HI_S32 ISP_ColorGamutInfoExit(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->gamut_info_ctrl.color_gamut_info != HI_NULL) {
        HI_MPI_SYS_Munmap(pstIspCtx->gamut_info_ctrl.color_gamut_info, sizeof(hi_isp_colorgammut_info));
        pstIspCtx->gamut_info_ctrl.color_gamut_info = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ProInfoInit(VI_PIPE ViPipe, isp_pro_info_buf *pstProInfo)
{
    HI_S32 s32Ret;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProInfo);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PRO_BUF_INIT, pstProInfo);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("isp[%d] init pro info bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ProInfoExit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    ISP_CHECK_PIPE(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PRO_BUF_EXIT);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("isp[%d] exit pro info buf failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ProNrParamInit(VI_PIPE ViPipe)
{
    HI_U64 u64PhyAddr;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->isp_pro_info.pro_nr_param_info.phy_addr;

    pstIspCtx->pro_nr_param_ctrl.pro_nr_param = HI_MPI_SYS_Mmap(u64PhyAddr, sizeof(isp_pro_nr_param));

    if (pstIspCtx->pro_nr_param_ctrl.pro_nr_param == HI_NULL) {
        ISP_ERR_TRACE("isp[%d] mmap pro nr paramt buf failed!\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ProNrParamExit(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->pro_nr_param_ctrl.pro_nr_param != HI_NULL) {
        HI_MPI_SYS_Munmap(pstIspCtx->pro_nr_param_ctrl.pro_nr_param, sizeof(isp_pro_nr_param));
        pstIspCtx->pro_nr_param_ctrl.pro_nr_param = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ProShpParamInit(VI_PIPE ViPipe)
{
    HI_U64 u64PhyAddr;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->isp_pro_info.pro_shp_param_info.phy_addr;

    pstIspCtx->pro_shp_param_ctrl.pro_shp_param = HI_MPI_SYS_Mmap(u64PhyAddr, sizeof(isp_pro_shp_param));

    if (pstIspCtx->pro_shp_param_ctrl.pro_shp_param == HI_NULL) {
        ISP_ERR_TRACE("isp[%d] mmap pro shp paramt buf failed!\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ProShpParamExit(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->pro_shp_param_ctrl.pro_shp_param != HI_NULL) {
        HI_MPI_SYS_Munmap(pstIspCtx->pro_shp_param_ctrl.pro_shp_param, sizeof(isp_pro_shp_param));
        pstIspCtx->pro_shp_param_ctrl.pro_shp_param = HI_NULL;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
