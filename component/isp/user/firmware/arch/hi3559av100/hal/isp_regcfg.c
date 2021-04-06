/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "isp_regcfg.h"
#include "isp_config.h"
#include "isp_lut_config.h"
#include "isp_ext_config.h"
#include "isp_main.h"
#include "mpi_sys.h"
#include "hi_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


isp_be_buf  g_astBeBufCtx[ISP_MAX_PIPE_NUM]   = {{0}};
isp_reg_cfg_attr *g_pastRegCfgCtx[ISP_MAX_PIPE_NUM] = {HI_NULL};
const HI_U32  g_au32IsoLut[ISP_AUTO_ISO_STRENGTH_NUM] = {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800};
isp_ldci_read_stt_buf g_astLdciReadSttBufCtx[ISP_MAX_PIPE_NUM]  = { 0 };

#define BE_REG_GET_CTX(dev, pstCtx)   pstCtx = &g_astBeBufCtx[dev]
#define LDCI_BUF_GET_CTX(dev, pstCtx) pstCtx = &g_astLdciReadSttBufCtx[dev]
#define HRS_PIPE 0

extern HI_S32 g_as32IspFd[ISP_MAX_PIPE_NUM];

HI_S32 ISP_ModParamGet(hi_isp_mod_param *pstModParam)
{
    ISP_CHECK_POINTER(pstModParam);

    pstModParam->quick_start = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 ISP_ClutBufInit(VI_PIPE ViPipe)
{
    if (ioctl(g_as32IspFd[ViPipe], ISP_CLUT_BUF_INIT) != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] clut buffer init failed\n", ViPipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ClutBufExit(VI_PIPE ViPipe)
{
    if (ioctl(g_as32IspFd[ViPipe], ISP_CLUT_BUF_EXIT) != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] exit clut bufs failed\n", ViPipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SpecAwbBufInit(VI_PIPE ViPipe)
{
    if (ioctl(g_as32IspFd[ViPipe], ISP_SPECAWB_BUF_INIT) != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] specawb buffer init failed\n", ViPipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SpecAwbBufExit(VI_PIPE ViPipe)
{
    if (ioctl(g_as32IspFd[ViPipe], ISP_SPECAWB_BUF_EXIT) != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] exit specawb bufs failed\n", ViPipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SttBufInit(VI_PIPE ViPipe)
{
    if (ioctl(g_as32IspFd[ViPipe], ISP_STT_BUF_INIT) != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] stt buffer init failed\n", ViPipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SttBufExit(VI_PIPE ViPipe)
{
    if (ioctl(g_as32IspFd[ViPipe], ISP_STT_BUF_EXIT) != HI_SUCCESS) {
        ISP_ERR_TRACE("exit stt bufs failed\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SttAddrInit(VI_PIPE ViPipe)
{
    return HI_SUCCESS;
}

static HI_S32 ISP_UpdateLdciReadSttBufCtx(VI_PIPE ViPipe, HI_U64 u64PhyAddr)
{
    HI_U8  i;
    HI_U64 u64Size;
    HI_VOID   *pVirtAddr = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_ldci_read_stt_buf *pstLdciReadSttBuf = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->ldci_tpr_flt_en == HI_FALSE) {
        return HI_SUCCESS;
    }

    LDCI_BUF_GET_CTX(ViPipe, pstLdciReadSttBuf);

    u64Size   = sizeof(isp_ldci_stat);
    pVirtAddr = HI_MPI_SYS_Mmap(u64PhyAddr, u64Size * pstLdciReadSttBuf->buf_num);

    if (pVirtAddr == HI_NULL) {
        ISP_ERR_TRACE("ISP[%d]:map ldci read stt buffer failed\n", ViPipe);
        return HI_FAILURE;
    }

    for (i = 0; i < pstLdciReadSttBuf->buf_num; i++) {
        pstLdciReadSttBuf->read_buf[i].phy_addr = u64PhyAddr + i * u64Size;
        pstLdciReadSttBuf->read_buf[i].vir_addr = (HI_VOID *)((HI_U8 *)pVirtAddr + i * u64Size);
        pstLdciReadSttBuf->read_buf[i].size     = u64Size;
    }

    return HI_SUCCESS;

}

HI_S32 ISP_LdciReadSttBufAddInit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_U64 u64PhyAddr;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_ldci_read_stt_buf *pstLdciReadSttBuf = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->ldci_tpr_flt_en == HI_FALSE) {
        return HI_SUCCESS;
    }

    LDCI_BUF_GET_CTX(ViPipe, pstLdciReadSttBuf);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_LDCI_READ_STT_BUF_GET, pstLdciReadSttBuf);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d]:Get ldci read stt buffer address failed\n", ViPipe);
        return s32Ret;
    }

    u64PhyAddr = pstLdciReadSttBuf->head_phy_addr;
    hi_ext_system_ldci_read_stt_buffer_low_addr_write(ViPipe, u64PhyAddr & 0xFFFFFFFF);
    hi_ext_system_ldci_read_stt_buffer_high_addr_write(ViPipe, u64PhyAddr >> 32);

    s32Ret = ISP_UpdateLdciReadSttBufCtx(ViPipe, u64PhyAddr);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_LdciReadSttBufAddExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_ldci_read_stt_buf *pstLdciReadSttBuf = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->ldci_tpr_flt_en == HI_FALSE) {
        return HI_SUCCESS;
    }

    LDCI_BUF_GET_CTX(ViPipe, pstLdciReadSttBuf);

    if (pstLdciReadSttBuf->read_buf[0].vir_addr != HI_NULL) {
        HI_MPI_SYS_Munmap(pstLdciReadSttBuf->read_buf[0].vir_addr, sizeof(isp_ldci_stat) * pstLdciReadSttBuf->buf_num);

        for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
            pstLdciReadSttBuf->read_buf[i].vir_addr = HI_NULL;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_LdciBufInit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    ISP_CTRL_PARAM_S stIspCtrlParam = { 0 };
    isp_usr_ctx *isp_ctx = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_CTRL_PARAM, &stIspCtrlParam);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get ctrlparam failed\n", ViPipe);
        return s32Ret;
    }

    isp_ctx->ldci_tpr_flt_en = stIspCtrlParam.bLdciTprFltEn;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_LDCI_BUF_INIT);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] ldci buffer init failed\n", ViPipe);
        return HI_FAILURE;
    }

    if (isp_ctx->ldci_tpr_flt_en == HI_TRUE) {
        s32Ret = ISP_LdciReadSttBufAddInit(ViPipe);

        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] ldci read stt buffer address init failed\n", ViPipe);
            ISP_LdciBufExit(ViPipe);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_LdciBufExit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_usr_ctx *isp_ctx = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->ldci_tpr_flt_en == HI_TRUE) {
        s32Ret = ISP_LdciReadSttBufAddExit(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] exit readstt bufaddr failed\n", ViPipe);
            return HI_FAILURE;
        }
    }

    if (ioctl(g_as32IspFd[ViPipe], ISP_LDCI_BUF_EXIT) != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] exit ldci bufs failed\n", ViPipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_CfgBeBufInit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_be_buf *pstBeBuf = HI_NULL;
    HI_U32 u32BeBufSize;

    ISP_CHECK_OFFLINE_MODE(ViPipe);
    BE_REG_GET_CTX(ViPipe, pstBeBuf);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_CFG_BUF_INIT, &pstBeBuf->be_phy_addr);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe:%d init be config bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    pstBeBuf->be_virt_addr = HI_MPI_SYS_MmapCache(pstBeBuf->be_phy_addr, sizeof(isp_be_wo_reg_cfg) * MAX_ISP_BE_BUF_NUM);

    if (pstBeBuf->be_virt_addr == HI_NULL) {
        ISP_ERR_TRACE("Pipe:%d init be config bufs failed!\n", ViPipe);
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_CFG_BUF_EXIT);

        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("Pipe:%d exit be config bufs failed %x!\n", ViPipe, s32Ret);
            return s32Ret;
        }

        return HI_ERR_ISP_NOMEM;
    }

    pstBeBuf->be_wo_cfg_buf.phy_addr = pstBeBuf->be_phy_addr;

    /* Get be buffer start address & size */
    u32BeBufSize = sizeof(isp_be_wo_reg_cfg) * MAX_ISP_BE_BUF_NUM;
    hi_ext_system_be_buffer_address_high_write(ViPipe, (pstBeBuf->be_phy_addr >> 32));
    hi_ext_system_be_buffer_address_low_write(ViPipe, (pstBeBuf->be_phy_addr & 0xFFFFFFFF));
    hi_ext_system_be_buffer_size_write(ViPipe, u32BeBufSize);

    return HI_SUCCESS;
}

HI_S32 ISP_UpdateBeBufAddr(VI_PIPE ViPipe, HI_VOID *pVirtAddr)
{
    HI_U16 i;
    HI_U64 u64BufSize = 0;
    isp_running_mode enIspRuningMode;
    isp_usr_ctx *isp_ctx = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    enIspRuningMode = isp_ctx->block_attr.running_mode;
    u64BufSize = sizeof(isp_be_wo_reg_cfg) / ISP_STRIPING_MAX_NUM;

    switch (enIspRuningMode) {
        case ISP_MODE_RUNNING_STRIPING :
            for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
                isp_ctx->isp_be_virt_addr[i]  = (HI_VOID *)((HI_U8 *)pVirtAddr + i * u64BufSize);
                isp_ctx->viproc_virt_addr[i] = (HI_VOID *)((HI_U8 *)isp_ctx->isp_be_virt_addr[i] + VIPROC_OFFLINE_OFFSET);
            }
            break;

        case ISP_MODE_RUNNING_OFFLINE :
            for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
                if (i == 0) {
                    isp_ctx->isp_be_virt_addr[i]  = pVirtAddr;
                    isp_ctx->viproc_virt_addr[i] = (HI_VOID *)((HI_U8 *)pVirtAddr + VIPROC_OFFLINE_OFFSET);
                } else {
                    isp_ctx->isp_be_virt_addr[i]  = HI_NULL;
                    isp_ctx->viproc_virt_addr[i] = HI_NULL;
                }
            }
            break;

        default :
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_CfgBeBufMmap(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_U64 u64BePhyAddr;
    isp_be_buf *pstBeBuf = HI_NULL;

    BE_REG_GET_CTX(ViPipe, pstBeBuf);

    u64BePhyAddr = pstBeBuf->be_wo_cfg_buf.phy_addr;
    hi_ext_system_be_free_buffer_high_addr_write(ViPipe, (u64BePhyAddr >> 32));
    hi_ext_system_be_free_buffer_low_addr_write(ViPipe, (u64BePhyAddr & 0xFFFFFFFF));

    if (pstBeBuf->be_virt_addr != HI_NULL) {
        pstBeBuf->be_wo_cfg_buf.vir_addr = (HI_VOID *)((HI_U8 *)pstBeBuf->be_virt_addr + \
                                                      (pstBeBuf->be_wo_cfg_buf.phy_addr - pstBeBuf->be_phy_addr));
    } else {
        pstBeBuf->be_wo_cfg_buf.vir_addr = HI_NULL;
    }

    if (pstBeBuf->be_wo_cfg_buf.vir_addr == HI_NULL) {
        return HI_FAILURE;
    }

    s32Ret = ISP_UpdateBeBufAddr(ViPipe, pstBeBuf->be_wo_cfg_buf.vir_addr);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe:%d isp update BE bufs failed %x!\n", ViPipe, s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_GetBeBufFirst(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_be_buf *pstBeBuf = HI_NULL;

    ISP_CHECK_OFFLINE_MODE(ViPipe);
    BE_REG_GET_CTX(ViPipe, pstBeBuf);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_GET_BE_BUF_FIRST, &pstBeBuf->be_wo_cfg_buf.phy_addr);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe:%d Get be free bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    s32Ret = ISP_CfgBeBufMmap(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe:%d ISP_CfgBeBufMmap failed %x!\n", ViPipe, s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_GetBeFreeBuf(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_be_buf *pstBeBuf = HI_NULL;

    ISP_CHECK_OFFLINE_MODE(ViPipe);
    BE_REG_GET_CTX(ViPipe, pstBeBuf);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_FREE_BUF_GET, &pstBeBuf->be_wo_cfg_buf);

    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = ISP_CfgBeBufMmap(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe:%d ISP_CfgBeBufMmap failed %x!\n", ViPipe, s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_GetBeLastBuf(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_be_buf *pstBeBuf  = HI_NULL;

    ISP_CHECK_OFFLINE_MODE(ViPipe);
    BE_REG_GET_CTX(ViPipe, pstBeBuf);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_LAST_BUF_GET, &pstBeBuf->be_wo_cfg_buf.phy_addr);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe:%d Get be busy bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    s32Ret = ISP_CfgBeBufMmap(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe:%d ISP_CfgBeBufMmap failed %x!\n", ViPipe, s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_CfgBeBufExit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_be_buf *pstBeBuf = HI_NULL;

    ISP_CHECK_OFFLINE_MODE(ViPipe);
    BE_REG_GET_CTX(ViPipe, pstBeBuf);

    if (pstBeBuf->be_virt_addr != HI_NULL) {
        HI_MPI_SYS_Munmap(pstBeBuf->be_virt_addr, sizeof(isp_be_wo_reg_cfg) * MAX_ISP_BE_BUF_NUM);
        pstBeBuf->be_virt_addr = HI_NULL;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_CFG_BUF_EXIT);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe:%d exit be config bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_CfgBeBufCtl(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_be_buf *pstBeBuf = HI_NULL;

    ISP_CHECK_OFFLINE_MODE(ViPipe);
    BE_REG_GET_CTX(ViPipe, pstBeBuf);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_CFG_BUF_CTL, &pstBeBuf->be_wo_cfg_buf);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SetCfgBeBufState(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;

    ISP_CHECK_OFFLINE_MODE(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_CFG_BUF_RUNNING);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}

/* init isp be cfgs all buffer */
HI_S32 ISP_AllCfgsBeBufInit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;

    ISP_CHECK_OFFLINE_MODE(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_All_BUF_INIT);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] init be all bufs Failed with ec %#x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S8 ISP_GetBlockIdByPipe(VI_PIPE ViPipe)
{
    HI_S8 s8BlockId = 0;

    switch (ViPipe) {
        case ISP_BE0_PIPE_ID :
            s8BlockId = 0;
            break;

        case ISP_BE1_PIPE_ID :
            s8BlockId = 1;
            break;

        default:
            return HI_FAILURE;
    }

    return s8BlockId;
}

HI_S32 ISP_BeVregAddrInit(VI_PIPE ViPipe)
{
    HI_U8  k = 0;
    HI_S8  s8BlkDev = 0;
    HI_U8  u8BlockId = 0;
    HI_U64 u64BufSize = 0;
    isp_running_mode enIspRuningMode;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_be_buf *pstBeBuf = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    BE_REG_GET_CTX(ViPipe, pstBeBuf);

    enIspRuningMode = isp_ctx->block_attr.running_mode;
    u64BufSize      = sizeof(isp_be_wo_reg_cfg) / ISP_STRIPING_MAX_NUM;

    switch (enIspRuningMode) {
        case ISP_MODE_RUNNING_ONLINE :
            s8BlkDev = ISP_GetBlockIdByPipe(ViPipe);

            if (-1 == s8BlkDev) {
                ISP_ERR_TRACE("ISP[%d] init Online Mode Pipe Err!\n", ViPipe);
                return HI_FAILURE;
            }

            u8BlockId = (HI_U8)s8BlkDev;

            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    isp_ctx->isp_be_virt_addr[k]  = VReg_GetVirtAddrBase(ISP_BE_REG_BASE(u8BlockId));
                    isp_ctx->viproc_virt_addr[k] = VReg_GetVirtAddrBase(ISP_VIPROC_REG_BASE(u8BlockId));
                } else {
                    isp_ctx->isp_be_virt_addr[k]  = HI_NULL;
                    isp_ctx->viproc_virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_OFFLINE :
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    isp_ctx->isp_be_virt_addr[k]  = pstBeBuf->be_wo_cfg_buf.vir_addr;
                    isp_ctx->viproc_virt_addr[k] = (HI_VOID *)((HI_U8 *)pstBeBuf->be_wo_cfg_buf.vir_addr + VIPROC_OFFLINE_OFFSET);
                } else {
                    isp_ctx->isp_be_virt_addr[k]  = HI_NULL;
                    isp_ctx->viproc_virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_SIDEBYSIDE :
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k < ISP_MAX_BE_NUM) {
                    isp_ctx->isp_be_virt_addr[k]  = VReg_GetVirtAddrBase(ISP_BE_REG_BASE(k));
                    isp_ctx->viproc_virt_addr[k] = VReg_GetVirtAddrBase(ISP_VIPROC_REG_BASE(k));
                } else {
                    isp_ctx->isp_be_virt_addr[k]  = HI_NULL;
                    isp_ctx->viproc_virt_addr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_STRIPING :
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                isp_ctx->isp_be_virt_addr[k]  = (HI_VOID *)((HI_U8 *)pstBeBuf->be_wo_cfg_buf.vir_addr + k * u64BufSize);
                isp_ctx->viproc_virt_addr[k] = (HI_VOID *)((HI_U8 *)isp_ctx->isp_be_virt_addr[k] + VIPROC_OFFLINE_OFFSET);
            }
            break;

        default:
            ISP_ERR_TRACE("ISP[%d] init Running Mode Err!\n", ViPipe);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID *ISP_VRegCfgBufAddr(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    HI_U64 u64Size = 0;
    HI_U64 u64PhyAddrHigh;
    HI_U64 u64PhyAddrTemp;
    isp_be_buf *pstBeBuf = HI_NULL;

    BE_REG_GET_CTX(ViPipe, pstBeBuf);

    u64Size = sizeof(isp_be_wo_reg_cfg) / ISP_STRIPING_MAX_NUM;

    if (pstBeBuf->be_wo_cfg_buf.vir_addr != HI_NULL) {
        return ((HI_U8 *)pstBeBuf->be_wo_cfg_buf.vir_addr + BlkDev * u64Size);
    }

    u64PhyAddrHigh = (HI_U64)hi_ext_system_be_free_buffer_high_addr_read(ViPipe);
    u64PhyAddrTemp = (HI_U64)hi_ext_system_be_free_buffer_low_addr_read(ViPipe);
    u64PhyAddrTemp |= (u64PhyAddrHigh << 32);

    pstBeBuf->be_wo_cfg_buf.phy_addr = u64PhyAddrTemp;
    pstBeBuf->be_wo_cfg_buf.vir_addr = HI_MPI_SYS_MmapCache(pstBeBuf->be_wo_cfg_buf.phy_addr, sizeof(isp_be_wo_reg_cfg));

    return ((HI_U8 *)pstBeBuf->be_wo_cfg_buf.vir_addr + BlkDev * u64Size);
}

HI_S32 ISP_GetBeVregCfgAddr(VI_PIPE ViPipe, HI_VOID *pVirtAddr[])
{
    HI_U8  k = 0;
    HI_S8  s8BlkDev = 0;
    HI_U8  u8BlockId = 0;
    HI_S32 s32Ret;
    isp_working_mode stIspWorkMode;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_GET, &stIspWorkMode);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("get isp work mode failed!\n");
        return s32Ret;
    }

    switch (stIspWorkMode.running_mode) {
        case ISP_MODE_RUNNING_ONLINE :
            s8BlkDev = ISP_GetBlockIdByPipe(ViPipe);

            if (-1 == s8BlkDev) {
                ISP_ERR_TRACE("ISP[%d] Online Mode Pipe Err!\n", ViPipe);
                return HI_FAILURE;
            }

            u8BlockId = (HI_U8)s8BlkDev;

            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    pVirtAddr[k] = VReg_GetVirtAddrBase(ISP_BE_REG_BASE(u8BlockId));
                } else {
                    pVirtAddr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_OFFLINE :
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    pVirtAddr[k] = ISP_VRegCfgBufAddr(ViPipe, (BLK_DEV)k);
                } else {
                    pVirtAddr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_SIDEBYSIDE :
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k < ISP_MAX_BE_NUM) {
                    pVirtAddr[k] = VReg_GetVirtAddrBase(ISP_BE_REG_BASE(k));
                } else {
                    pVirtAddr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_STRIPING :
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                pVirtAddr[k] = ISP_VRegCfgBufAddr(ViPipe, (BLK_DEV)k);
            }
            break;

        default:
            ISP_ERR_TRACE("ISP[%d] GetBe Running Mode Err!\n", ViPipe);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_GetViProcCfgAddr(VI_PIPE ViPipe, HI_VOID *pVirtAddr[])
{
    HI_U8  k = 0;
    HI_S8  s8BlkDev = 0;
    HI_U8  u8BlockId = 0;
    HI_S32 s32Ret;
    HI_VOID *pBeVirtAddr;
    isp_working_mode stIspWorkMode;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_GET, &stIspWorkMode);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("get isp work mode failed!\n");
        return s32Ret;
    }

    switch (stIspWorkMode.running_mode) {
        case ISP_MODE_RUNNING_ONLINE :
            s8BlkDev = ISP_GetBlockIdByPipe(ViPipe);

            if (-1 == s8BlkDev) {
                ISP_ERR_TRACE("ISP[%d] Online Mode Pipe Err!\n", ViPipe);
                return HI_FAILURE;
            }

            u8BlockId = (HI_U8)s8BlkDev;

            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    pVirtAddr[k] = VReg_GetVirtAddrBase(ISP_VIPROC_REG_BASE(u8BlockId));
                } else {
                    pVirtAddr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_OFFLINE :
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k == 0) {
                    pBeVirtAddr = ISP_VRegCfgBufAddr(ViPipe, (BLK_DEV)k);
                    pVirtAddr[k] = (HI_VOID *)((HI_U8 *)pBeVirtAddr + VIPROC_OFFLINE_OFFSET);
                } else {
                    pVirtAddr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_SIDEBYSIDE :
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                if (k < ISP_MAX_BE_NUM) {
                    pVirtAddr[k] = VReg_GetVirtAddrBase(ISP_VIPROC_REG_BASE(k));
                } else {
                    pVirtAddr[k] = HI_NULL;
                }
            }
            break;

        case ISP_MODE_RUNNING_STRIPING :
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                pBeVirtAddr = ISP_VRegCfgBufAddr(ViPipe, (BLK_DEV)k);
                pVirtAddr[k] = (HI_VOID *)((HI_U8 *)pBeVirtAddr + VIPROC_OFFLINE_OFFSET);
            }
            break;

        default:
            ISP_ERR_TRACE("ISP[%d] GetBe Running Mode Err!\n", ViPipe);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID *ISP_GetFeVirAddr(VI_PIPE ViPipe)
{
    ISP_CHECK_FE_PIPE(ViPipe);

    return VReg_GetVirtAddrBase(ISP_FE_REG_BASE(ViPipe));
}

HI_VOID *ISP_GetBeVirAddr(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    HI_S32 s32Ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    HI_VOID *pVirtAddr[ISP_STRIPING_MAX_NUM] = {HI_NULL};

    ISP_CHECK_FE_PIPE(ViPipe);
    ISP_CHECK_BE_DEV(BlkDev);
    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->isp_be_virt_addr[BlkDev]) {
        return isp_ctx->isp_be_virt_addr[BlkDev];
    }

    s32Ret = ISP_GetBeVregCfgAddr(ViPipe, pVirtAddr);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Get Be CfgAddr Failed!\n", ViPipe);
        return HI_NULL;
    }

    ISP_CHECK_NULLPTR(pVirtAddr[BlkDev]);

    return pVirtAddr[BlkDev];
}

HI_VOID *ISP_GetViProcVirAddr(VI_PIPE ViPipe, BLK_DEV BlkDev)
{
    HI_S32 s32Ret;
    isp_usr_ctx *isp_ctx = HI_NULL;
    HI_VOID *pVirtAddr[ISP_STRIPING_MAX_NUM] = {HI_NULL};

    ISP_CHECK_FE_PIPE(ViPipe);
    ISP_CHECK_BE_DEV(BlkDev);
    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->viproc_virt_addr[BlkDev]) {
        return isp_ctx->viproc_virt_addr[BlkDev];
    }

    s32Ret = ISP_GetViProcCfgAddr(ViPipe, pVirtAddr);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Get viproc CfgAddr Failed!\n", ViPipe);
        return HI_NULL;
    }

    ISP_CHECK_NULLPTR(pVirtAddr[BlkDev]);

    return pVirtAddr[BlkDev];
}

HI_VOID *ISP_GetLdciReadSttVirAddr(VI_PIPE ViPipe, HI_U8 u8BufIdx)
{
    HI_S32 s32Ret;
    HI_U64 u64PhyAddrHigh, u64PhyAddrTmp;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_ldci_read_stt_buf *pstLdciReadSttBuf = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->ldci_tpr_flt_en == HI_FALSE) {
        return HI_NULL;
    }

    LDCI_BUF_GET_CTX(ViPipe, pstLdciReadSttBuf);

    if (pstLdciReadSttBuf->read_buf[u8BufIdx].vir_addr != HI_NULL) {
        return pstLdciReadSttBuf->read_buf[u8BufIdx].vir_addr;
    }

    u64PhyAddrHigh = (HI_U64)hi_ext_system_ldci_read_stt_buffer_high_addr_read(ViPipe);
    u64PhyAddrTmp  = (HI_U64)hi_ext_system_ldci_read_stt_buffer_low_addr_read(ViPipe);
    u64PhyAddrTmp |= (u64PhyAddrHigh << 32);

    s32Ret = ISP_UpdateLdciReadSttBufCtx(ViPipe, u64PhyAddrTmp);
    if (s32Ret != HI_SUCCESS) {
        return HI_NULL;
    }

    return pstLdciReadSttBuf->read_buf[u8BufIdx].vir_addr;
}

static HI_S32  ISP_FeHrsRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    ISP_HRS_STATIC_CFG_S   *pstHrsStaticRegCfg  = HI_NULL;
    S_ISPFE_REGS_TYPE      *pstFeReg            = HI_NULL;

    pstHrsStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[0].stHrsRegCfg.stStaticRegCfg;

    if (pstRegCfgInfo->cfg_key.bit1HrsCfg) {
        pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(0);
        ISP_CHECK_POINTER(pstFeReg);
        pstHrsStaticRegCfg->u8RSEnable = (IS_HRS_ON(ViPipe) ? HI_TRUE : HI_FALSE);
        isp_hrs_ds_en_write(pstFeReg, pstHrsStaticRegCfg->u8RSEnable);

        if (pstHrsStaticRegCfg->bResh) {
            isp_fe_hrs_en_write(pstFeReg, pstHrsStaticRegCfg->u8Enable);
            isp_hrs_filterlut0_0_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut0[0]);
            isp_hrs_filterlut1_0_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut0[1]);
            isp_hrs_filterlut2_0_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut0[2]);
            isp_hrs_filterlut3_0_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut0[3]);
            isp_hrs_filterlut4_0_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut0[4]);
            isp_hrs_filterlut5_0_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut0[5]);
            isp_hrs_filterlut0_1_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut1[0]);
            isp_hrs_filterlut1_1_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut1[1]);
            isp_hrs_filterlut2_1_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut1[2]);
            isp_hrs_filterlut3_1_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut1[3]);
            isp_hrs_filterlut4_1_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut1[4]);
            isp_hrs_filterlut5_1_write(pstFeReg, pstHrsStaticRegCfg->as16HRSFilterLut1[5]);

            pstHrsStaticRegCfg->bResh = HI_FALSE;
        }
    }

    return HI_SUCCESS;
}

static HI_VOID ISP_FeModifyLscXGridSize(S_ISPFE_REGS_TYPE *pstFeReg, HI_U32 u32Width, HI_U16 *pu16DeltaX)
{
    HI_U8  i;
    HI_U8  u8HalfGridSize  = (HI_ISP_LSC_GRID_COL - 1) / 2;
    HI_U16 u16SumGridWidth = 0;
    HI_U16 u16diff = 0;
    HI_U16 au16DeltaX[HI_ISP_LSC_GRID_COL - 1] = {0};
    HI_U16 au16InvX[HI_ISP_LSC_GRID_COL - 1]   = {0};

    for (i = 0; i < u8HalfGridSize; i++) {
        au16DeltaX[i]    = (pu16DeltaX[i] + 1) >> 1;
        u16SumGridWidth += au16DeltaX[i];
    }

    if (u16SumGridWidth > (u32Width >> 2)) {
        u16diff = u16SumGridWidth - (u32Width >> 2);

        for (i = 1; i <= u16diff; i++) {
            au16DeltaX[u8HalfGridSize - i] = au16DeltaX[u8HalfGridSize - i] - 1;
        }
    }

    for (i = 0; i < HI_ISP_LSC_GRID_COL - 1; i++) {
        au16InvX[i] = (0 == au16DeltaX[i]) ? (0) : ((4096 * 1024 / au16DeltaX[i] + 512) >> 10);
        isp_lsc1_winX_info_write(pstFeReg, i, au16DeltaX[i], au16InvX[i]);
    }
}

static HI_S32 ISP_FeLscRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_BOOL bLutUpdate = HI_FALSE;
    HI_U16  i, j;
    HI_U16  u16Width;
    VI_PIPE ViPipeBind;
    ISP_FE_LSC_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_FE_LSC_USR_CFG_S    *pstUsrRegCfg    = HI_NULL;
    S_ISPFE_REGS_TYPE       *pstFeReg        = HI_NULL;
    isp_usr_ctx               *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[0].stFeLscRegCfg.stStaticRegCfg;
    pstUsrRegCfg    = &pstRegCfgInfo->alg_reg_cfg[0].stFeLscRegCfg.stUsrRegCfg;

    if (isp_ctx->wdr_attr.mast_pipe) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            ViPipeBind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
            ISP_CHECK_PIPE(ViPipeBind);

            if (pstRegCfgInfo->cfg_key.bit1FeLscCfg) {
                pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
                ISP_CHECK_POINTER(pstFeReg);

                isp_fe_lsc1_en_write(pstFeReg, pstRegCfgInfo->alg_reg_cfg[0].stFeLscRegCfg.bLscEn);

                u16Width = IS_HRS_ON(ViPipeBind) ? (isp_ctx->sys_rect.width >> 1) : isp_ctx->sys_rect.width;

                if (pstStaticRegCfg->bResh) {
                    isp_lsc1_numh_write(pstFeReg, pstStaticRegCfg->u8WinNumH);
                    isp_lsc1_numv_write(pstFeReg, pstStaticRegCfg->u8WinNumV);
                    isp_lsc1_width_offset_write(pstFeReg, pstStaticRegCfg->u16WidthOffset);
                }

                if (pstUsrRegCfg->bResh) {
                    isp_lsc1_mesh_scale_write(pstFeReg, pstUsrRegCfg->u8MeshScale);
                    isp_lsc1_mesh_str_write(pstFeReg, pstUsrRegCfg->u16MeshStr);
                    isp_lsc1_mesh_weight_write(pstFeReg, pstUsrRegCfg->u16Weight);

                    if (pstUsrRegCfg->bLutUpdate) {
                        for (j = 0; j < (HI_ISP_LSC_GRID_ROW - 1) / 2; j++) {
                            isp_lsc1_winY_info_write(pstFeReg, j, pstUsrRegCfg->au16DeltaY[j], pstUsrRegCfg->au16InvY[j]);
                        }

                        if (IS_HRS_ON(ViPipeBind)) {
                            ISP_FeModifyLscXGridSize(pstFeReg, u16Width, pstUsrRegCfg->au16DeltaX);
                        } else {
                            for (j = 0; j < (HI_ISP_LSC_GRID_COL - 1); j++) {
                                isp_lsc1_winX_info_write(pstFeReg, j, pstUsrRegCfg->au16DeltaX[j], pstUsrRegCfg->au16InvX[j]);
                            }
                        }

                        isp_lsc1_rgain_waddr_write(pstFeReg, 0);

                        for (j = 0; j < HI_ISP_LSC_GRID_POINTS; j++) {
                            isp_lsc1_rgain_wdata_write(pstFeReg, pstUsrRegCfg->au32RGain[j]);
                        }

                        isp_lsc1_grgain_waddr_write(pstFeReg, 0);

                        for (j = 0; j < HI_ISP_LSC_GRID_POINTS; j++) {
                            isp_lsc1_grgain_wdata_write(pstFeReg, pstUsrRegCfg->au32GrGain[j]);
                        }

                        isp_lsc1_gbgain_waddr_write(pstFeReg, 0);

                        for (j = 0; j < HI_ISP_LSC_GRID_POINTS; j++) {
                            isp_lsc1_gbgain_wdata_write(pstFeReg, pstUsrRegCfg->au32GbGain[j]);
                        }

                        isp_lsc1_bgain_waddr_write(pstFeReg, 0);

                        for (j = 0; j < HI_ISP_LSC_GRID_POINTS; j++) {
                            isp_lsc1_bgain_wdata_write(pstFeReg, pstUsrRegCfg->au32BGain[j]);
                        }

                        bLutUpdate = HI_TRUE;
                    }
                }
            }
        }

        pstUsrRegCfg->bLutUpdate = HI_FALSE;
        pstStaticRegCfg->bResh   = HI_FALSE;
        pstUsrRegCfg->bResh      = HI_FALSE;
        pstRegCfgInfo->alg_reg_cfg[0].stFeLutUpdateCfg.bLsc1LutUpdate = bLutUpdate;
        pstRegCfgInfo->cfg_key.bit1FeLscCfg = 0;
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_FeDgRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_U16  i;
    VI_PIPE ViPipeBind;
    ISP_FE_DG_DYNA_CFG_S *pstDynaRegCfg = HI_NULL;
    isp_usr_ctx         *isp_ctx     = HI_NULL;
    S_ISPFE_REGS_TYPE *pstFeReg      = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    pstDynaRegCfg = &pstRegCfgInfo->alg_reg_cfg[0].stFeDgRegCfg.stDynaRegCfg;

    if (isp_ctx->wdr_attr.mast_pipe) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            ViPipeBind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
            ISP_CHECK_PIPE(ViPipeBind);
            pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
            ISP_CHECK_POINTER(pstFeReg);

            if (pstRegCfgInfo->cfg_key.bit1FeDgCfg) {
                isp_fe_dg2_en_write(pstFeReg, pstRegCfgInfo->alg_reg_cfg[0].stFeDgRegCfg.bDgEn);

                // dynamic
                if (pstDynaRegCfg->bResh) {
                    isp_dg2_rgain_write(pstFeReg, pstDynaRegCfg->au16GainR[i]);
                    isp_dg2_grgain_write(pstFeReg, pstDynaRegCfg->au16GainGR[i]);
                    isp_dg2_gbgain_write(pstFeReg, pstDynaRegCfg->au16GainGB[i]);
                    isp_dg2_bgain_write(pstFeReg, pstDynaRegCfg->au16GainB[i]);
                    isp_dg2_clip_value_write(pstFeReg, pstDynaRegCfg->u32ClipValue);
                }
            }
        }

        pstDynaRegCfg->bResh = HI_FALSE;
        pstRegCfgInfo->cfg_key.bit1FeDgCfg = 0;
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_FeLogLUTRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_U16  i, j;
    VI_PIPE ViPipeBind;
    HI_U8 u8FeLogLutEn[ISP_MAX_PIPE_NUM] = {ISP_PIPE_FELOGLUT_MODULE_ENABLE};
    isp_usr_ctx                  *isp_ctx       = HI_NULL;
    S_ISPFE_REGS_TYPE          *pstFeReg        = HI_NULL;
    ISP_FE_LOGLUT_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    ISP_FE_LOGLUT_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[0].stFeLogLUTRegCfg.stStaticRegCfg;
    pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[0].stFeLogLUTRegCfg.stDynaRegCfg;

    if (isp_ctx->wdr_attr.mast_pipe) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            ViPipeBind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
            ISP_CHECK_PIPE(ViPipeBind);

            if (!u8FeLogLutEn[ViPipeBind]) {
                continue;
            }

            if (pstRegCfgInfo->cfg_key.bit1FeLogLUTCfg) {
                pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
                ISP_CHECK_POINTER(pstFeReg);

                isp_fe_loglut1_en_write(pstFeReg, pstDynaRegCfg->bEnable);

                if (pstStaticRegCfg->bStaticResh) {
                    /* static */
                    isp_loglut1_bitw_out_write(pstFeReg, pstStaticRegCfg->u8BitDepthOut);

                    isp_loglut1_lut_waddr_write(pstFeReg, 0);

                    for (j = 0; j < PRE_LOG_LUT_SIZE; j++) {
                        isp_loglut1_lut_wdata_write(pstFeReg, pstStaticRegCfg->au32LogLUT[j]);
                    }
                }
            }

        }

        pstStaticRegCfg->bStaticResh         = HI_FALSE;
        pstRegCfgInfo->cfg_key.bit1FeLogLUTCfg = 0;
    }

    return HI_SUCCESS;
}

static hi_s32 isp_fe_blc_reg_config(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_u16  i;
    VI_PIPE vi_pipe_bind;
    isp_fe_blc_cfg    *fe_blc_cfg = HI_NULL;
    isp_usr_ctx       *isp_ctx    = HI_NULL;
    S_ISPFE_REGS_TYPE *fe_reg     = HI_NULL;

    ISP_GET_CTX(vi_pipe, isp_ctx);

    fe_blc_cfg = &reg_cfg_info->alg_reg_cfg[0].fe_blc_cfg;

    if (isp_ctx->wdr_attr.mast_pipe) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
            ISP_CHECK_PIPE(vi_pipe_bind);

            if (reg_cfg_info->cfg_key.bit1_fe_blc_cfg) {
                fe_reg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(vi_pipe_bind);
                ISP_CHECK_POINTER(fe_reg);

                if (fe_blc_cfg->resh_static) {
                    /* Fe Dg */
                    isp_dg2_en_in_write(fe_reg, fe_blc_cfg->fe_dg_blc.static_reg_cfg.blc_in);
                    isp_dg2_en_out_write(fe_reg, fe_blc_cfg->fe_dg_blc.static_reg_cfg.blc_out);
                    /* Fe Wb */
                    isp_wb1_en_in_write(fe_reg, fe_blc_cfg->fe_wb_blc.static_reg_cfg.blc_in);
                    isp_wb1_en_out_write(fe_reg, fe_blc_cfg->fe_wb_blc.static_reg_cfg.blc_out);
                    /* Fe Ae */
                    isp_ae1_blc_en_write(fe_reg, fe_blc_cfg->fe_ae_blc.static_reg_cfg.blc_in);
                    /* Fe LSC */
                    isp_lsc1_blc_in_en_write(fe_reg, fe_blc_cfg->fe_lsc_blc.static_reg_cfg.blc_in);
                    isp_lsc1_blc_out_en_write(fe_reg, fe_blc_cfg->fe_lsc_blc.static_reg_cfg.blc_out);
                    /* Fe RC */
                    isp_rc_blc_in_en_write(fe_reg, fe_blc_cfg->rc_blc.static_reg_cfg.blc_in);
                    isp_rc_blc_out_en_write(fe_reg, fe_blc_cfg->rc_blc.static_reg_cfg.blc_out);
                    /* Fe BLC */
                    isp_fe_blc1_en_write(fe_reg, fe_blc_cfg->fe_blc.static_reg_cfg.blc_in);
                }

                if (fe_blc_cfg->resh_dyna) {
                    /* Fe BLC */
                    isp_blc1_ofsr_write(fe_reg, fe_blc_cfg->fe_blc.usr_reg_cfg.blc[0]);
                    isp_blc1_ofsgr_write(fe_reg, fe_blc_cfg->fe_blc.usr_reg_cfg.blc[1]);
                    isp_blc1_ofsgb_write(fe_reg, fe_blc_cfg->fe_blc.usr_reg_cfg.blc[2]);
                    isp_blc1_ofsb_write(fe_reg, fe_blc_cfg->fe_blc.usr_reg_cfg.blc[3]);

                    /* Fe Dg */
                    isp_dg2_ofsr_write(fe_reg, fe_blc_cfg->fe_dg_blc.usr_reg_cfg.blc[0]);
                    isp_dg2_ofsgr_write(fe_reg, fe_blc_cfg->fe_dg_blc.usr_reg_cfg.blc[1]);
                    isp_dg2_ofsgb_write(fe_reg, fe_blc_cfg->fe_dg_blc.usr_reg_cfg.blc[2]);
                    isp_dg2_ofsb_write(fe_reg, fe_blc_cfg->fe_dg_blc.usr_reg_cfg.blc[3]);

                    /* Fe WB */
                    isp_wb1_ofsr_write(fe_reg, fe_blc_cfg->fe_wb_blc.usr_reg_cfg.blc[0]);
                    isp_wb1_ofsgr_write(fe_reg, fe_blc_cfg->fe_wb_blc.usr_reg_cfg.blc[1]);
                    isp_wb1_ofsgb_write(fe_reg, fe_blc_cfg->fe_wb_blc.usr_reg_cfg.blc[2]);
                    isp_wb1_ofsb_write(fe_reg, fe_blc_cfg->fe_wb_blc.usr_reg_cfg.blc[3]);

                    /* Fe AE */
                    isp_ae1_offset_r_write(fe_reg,  fe_blc_cfg->fe_ae_blc.usr_reg_cfg.blc[0]);
                    isp_ae1_offset_gr_write(fe_reg, fe_blc_cfg->fe_ae_blc.usr_reg_cfg.blc[1]);
                    isp_ae1_offset_gb_write(fe_reg, fe_blc_cfg->fe_ae_blc.usr_reg_cfg.blc[2]);
                    isp_ae1_offset_b_write(fe_reg,  fe_blc_cfg->fe_ae_blc.usr_reg_cfg.blc[3]);
                    /* Fe LSC */
                    isp_lsc1_blc_r_write(fe_reg, fe_blc_cfg->fe_lsc_blc.usr_reg_cfg.blc[0]);
                    isp_lsc1_blc_gr_write(fe_reg, fe_blc_cfg->fe_lsc_blc.usr_reg_cfg.blc[1]);
                    isp_lsc1_blc_gb_write(fe_reg, fe_blc_cfg->fe_lsc_blc.usr_reg_cfg.blc[2]);
                    isp_lsc1_blc_b_write(fe_reg, fe_blc_cfg->fe_lsc_blc.usr_reg_cfg.blc[3]);
                    /* Fe Rc */
                    isp_rc_blc_r_write(fe_reg, fe_blc_cfg->rc_blc.usr_reg_cfg.blc[0]);
                    isp_rc_blc_gr_write(fe_reg, fe_blc_cfg->rc_blc.usr_reg_cfg.blc[1]);
                    isp_rc_blc_gb_write(fe_reg, fe_blc_cfg->rc_blc.usr_reg_cfg.blc[2]);
                    isp_rc_blc_b_write(fe_reg, fe_blc_cfg->rc_blc.usr_reg_cfg.blc[3]);

                    /* Fe LogLUT */
                    isp_loglut1_offset_r_write(fe_reg, fe_blc_cfg->fe_log_lut_blc.usr_reg_cfg.blc[0]);
                    isp_loglut1_offset_gr_write(fe_reg, fe_blc_cfg->fe_log_lut_blc.usr_reg_cfg.blc[1]);
                    isp_loglut1_offset_gb_write(fe_reg, fe_blc_cfg->fe_log_lut_blc.usr_reg_cfg.blc[2]);
                    isp_loglut1_offset_b_write(fe_reg, fe_blc_cfg->fe_log_lut_blc.usr_reg_cfg.blc[3]);
                }
            }
        }

        fe_blc_cfg->resh_static = HI_FALSE;
        fe_blc_cfg->resh_dyna   = HI_FALSE;
        reg_cfg_info->cfg_key.bit1_fe_blc_cfg = 0;
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_FeAeRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_BOOL bLutUpdate = HI_FALSE;
    HI_U16  i, j, k;
    HI_U16  u16CropWidth, u16CropX;
    HI_U32  u32TableWeightTmp = 0;
    HI_U32  u32CombinWeight = 0;
    HI_U32  u32CombinWeightNum = 0;
    VI_PIPE ViPipeBind;
    ISP_AE_STATIC_CFG_S *pstStaticRegFeCfg = HI_NULL;
    ISP_AE_DYNA_CFG_S   *pstDynaRegFeCfg   = HI_NULL;
    S_ISPFE_REGS_TYPE   *pstFeReg          = HI_NULL;
    isp_usr_ctx *isp_ctx   = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->wdr_attr.mast_pipe && pstRegCfgInfo->cfg_key.bit1AeCfg1) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            ViPipeBind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
            ISP_CHECK_PIPE(ViPipeBind);

            pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
            ISP_CHECK_POINTER(pstFeReg);
            // ae fe static
            pstStaticRegFeCfg = &pstRegCfgInfo->alg_reg_cfg[0].stAeRegCfg.stStaticRegCfg;
            pstDynaRegFeCfg   = &pstRegCfgInfo->alg_reg_cfg[0].stAeRegCfg.stDynaRegCfg;

            isp_fe_ae1_en_write(pstFeReg, pstStaticRegFeCfg->u8FEEnable);
            u16CropX = IS_HRS_ON(ViPipeBind) ? (pstStaticRegFeCfg->u16FECropPosX >> 1) : pstStaticRegFeCfg->u16FECropPosX;
            isp_ae1_crop_pos_x_write(pstFeReg, u16CropX);
            isp_ae1_crop_pos_y_write(pstFeReg, pstStaticRegFeCfg->u16FECropPosY);
            u16CropWidth = IS_HRS_ON(ViPipeBind) ? (pstStaticRegFeCfg->u16FECropOutWidth >> 1) : pstStaticRegFeCfg->u16FECropOutWidth;
            isp_ae1_crop_out_width_write(pstFeReg, u16CropWidth - 1);
            isp_ae1_crop_out_height_write(pstFeReg, pstStaticRegFeCfg->u16FECropOutHeight - 1);

            // ae fe dynamic
            isp_ae1_hnum_write(pstFeReg, pstDynaRegFeCfg->u8FEWeightTableWidth);
            isp_ae1_vnum_write(pstFeReg, pstDynaRegFeCfg->u8FEWeightTableHeight);
            isp_ae1_skip_x_write(pstFeReg, pstDynaRegFeCfg->u8FEHistSkipX);
            isp_ae1_offset_x_write(pstFeReg, pstDynaRegFeCfg->u8FEHistOffsetX);
            isp_ae1_skip_y_write(pstFeReg, pstDynaRegFeCfg->u8FEHistSkipY);
            isp_ae1_offset_y_write(pstFeReg, pstDynaRegFeCfg->u8FEHistOffsetY);
            isp_ae1_bitmove_write(pstFeReg, pstDynaRegFeCfg->u8FEBitMove);
            isp_ae1_hist_gamma_mode_write(pstFeReg, pstDynaRegFeCfg->u8FEHistGammaMode);
            isp_ae1_aver_gamma_mode_write(pstFeReg, pstDynaRegFeCfg->u8FEAverGammaMode);
            isp_ae1_gamma_limit_write(pstFeReg, pstDynaRegFeCfg->u8FEGammaLimit);
            isp_ae1_fourplanemode_write(pstFeReg, pstDynaRegFeCfg->u8FEFourPlaneMode);

            u32CombinWeight = 0;
            u32CombinWeightNum = 0;

            isp_ae1_wei_waddr_write(pstFeReg, 0);

            for (j = 0; j < AE_ZONE_ROW; j++) {
                for (k = 0; k < AE_ZONE_COLUMN; k++) {
                    u32TableWeightTmp = (HI_U32)pstDynaRegFeCfg->au8FEWeightTable[j][k];
                    u32CombinWeight |= (u32TableWeightTmp << (8 * u32CombinWeightNum));
                    u32CombinWeightNum++;

                    if (u32CombinWeightNum == HI_ISP_AE_WEI_COMBIN_COUNT) {
                        isp_ae1_wei_wdata_write(pstFeReg, u32CombinWeight);
                        u32CombinWeightNum = 0;
                        u32CombinWeight = 0;
                    }
                }
            }

            if ((u32CombinWeightNum != HI_ISP_AE_WEI_COMBIN_COUNT) && (u32CombinWeightNum != 0)) {
                isp_ae1_wei_wdata_write(pstFeReg, u32CombinWeight);
            }

            bLutUpdate = pstDynaRegFeCfg->u8FEWightTableUpdate;
        }
    }

    pstRegCfgInfo->alg_reg_cfg[0].stFeLutUpdateCfg.bAe1LutUpdate = bLutUpdate;
    return HI_SUCCESS;
}

static HI_S32 ISP_AeRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL bLutUpdate = HI_FALSE;
    HI_U16  j, k, m;
    HI_U32  u32CombinWeight = 0;
    HI_U32  u32CombinWeightNum = 0;
    ISP_AE_STATIC_CFG_S *pstStaticRegBeCfg = HI_NULL;
    ISP_AE_DYNA_CFG_S   *pstDynaRegBeCfg   = HI_NULL;
    ISP_MG_STATIC_CFG_S *pstMgStaticRegCfg = HI_NULL;
    ISP_MG_DYNA_CFG_S   *pstMgDynaRegCfg   = HI_NULL;
    S_VIPROC_REGS_TYPE  *pstViProcReg      = HI_NULL;
    S_ISPBE_REGS_TYPE   *pstBeReg          = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;
    HI_U32 au32CombineWgt[64] = {0};

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (pstRegCfgInfo->cfg_key.bit1AeCfg1) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        // ae be static
        pstStaticRegBeCfg = &pstRegCfgInfo->alg_reg_cfg[i].stAeRegCfg.stStaticRegCfg;
        pstDynaRegBeCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stAeRegCfg.stDynaRegCfg;

        isp_ae_en_write(pstViProcReg, pstStaticRegBeCfg->u8BEEnable);
        isp_ae_crop_pos_x_write(pstBeReg, pstStaticRegBeCfg->u16BECropPosX);
        isp_ae_crop_pos_y_write(pstBeReg, pstStaticRegBeCfg->u16BECropPosY);
        isp_ae_crop_out_width_write(pstBeReg, pstStaticRegBeCfg->u16BECropOutWidth - 1);
        isp_ae_crop_out_height_write(pstBeReg, pstStaticRegBeCfg->u16BECropOutHeight - 1);
        // ae be dynamic
        isp_ae_sel_write(pstBeReg, pstDynaRegBeCfg->u8BEAESel);
        isp_ae_hnum_write(pstBeReg, pstDynaRegBeCfg->u8BEWeightTableWidth);
        isp_ae_vnum_write(pstBeReg, pstDynaRegBeCfg->u8BEWeightTableHeight);
        isp_ae_skip_x_write(pstBeReg, pstDynaRegBeCfg->u8BEHistSkipX);
        isp_ae_offset_x_write(pstBeReg, pstDynaRegBeCfg->u8BEHistOffsetX);
        isp_ae_skip_y_write(pstBeReg, pstDynaRegBeCfg->u8BEHistSkipY);
        isp_ae_offset_y_write(pstBeReg, pstDynaRegBeCfg->u8BEHistOffsetY);
        isp_ae_bitmove_write(pstBeReg, pstDynaRegBeCfg->u8BEBitMove);
        isp_ae_hist_gamma_mode_write(pstBeReg, pstDynaRegBeCfg->u8BEHistGammaMode);
        isp_ae_aver_gamma_mode_write(pstBeReg, pstDynaRegBeCfg->u8BEAverGammaMode);
        isp_ae_gamma_limit_write(pstBeReg, pstDynaRegBeCfg->u8BEGammaLimit);
        isp_ae_four_plane_mode_write(pstBeReg, pstDynaRegBeCfg->u8BEFourPlaneMode);

        m = 0;
        u32CombinWeight = 0;
        u32CombinWeightNum = 0;

        for (j = 0; j < pstDynaRegBeCfg->u8BEWeightTableHeight; j++) {
            for (k = 0; k < pstDynaRegBeCfg->u8BEWeightTableWidth; k++) {
                u32CombinWeight |= (pstDynaRegBeCfg->au8BEWeightTable[j][k] << (8 * u32CombinWeightNum));
                u32CombinWeightNum++;

                if (u32CombinWeightNum == HI_ISP_AE_WEI_COMBIN_COUNT) {
                    au32CombineWgt[m++] = u32CombinWeight;
                    u32CombinWeightNum = 0;
                    u32CombinWeight = 0;
                }
            }
        }

        if (u32CombinWeightNum != HI_ISP_AE_WEI_COMBIN_COUNT
            && u32CombinWeightNum != 0) {
            au32CombineWgt[m++] = u32CombinWeight;
        }

        if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
            || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
            isp_ae_wei_waddr_write(pstBeReg, 0);

            for (m = 0; m < 64; m++) {
                isp_ae_wei_wdata_write(pstBeReg, au32CombineWgt[m]);
            }
        } else {
            isp_ae_weight_write(pstBeReg, au32CombineWgt);
        }

        bLutUpdate = pstDynaRegBeCfg->u8BEWightTableUpdate;

        /* mg static */
        pstMgStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stMgRegCfg.stStaticRegCfg;
        pstMgDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stMgRegCfg.stDynaRegCfg;

        isp_la_en_write(pstViProcReg, pstMgStaticRegCfg->u8Enable);
        isp_la_crop_pos_x_write(pstBeReg, pstMgStaticRegCfg->u16CropPosX);
        isp_la_crop_pos_y_write(pstBeReg, pstMgStaticRegCfg->u16CropPosY);
        isp_la_crop_out_width_write(pstBeReg, pstMgStaticRegCfg->u16CropOutWidth - 1);
        isp_la_crop_out_height_write(pstBeReg, pstMgStaticRegCfg->u16CropOutHeight - 1);

        /* mg dynamic */
        isp_la_hnum_write(pstBeReg, pstMgDynaRegCfg->u8ZoneWidth);
        isp_la_vnum_write(pstBeReg, pstMgDynaRegCfg->u8ZoneHeight);
        isp_la_bitmove_write(pstBeReg, pstMgDynaRegCfg->u8BitMove);
        isp_la_gamma_en_write(pstBeReg, pstMgDynaRegCfg->u8GammaMode);
        isp_la_gamma_limit_write(pstBeReg, pstMgDynaRegCfg->u8GammaLimit);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bAeLutUpdate = bLutUpdate;

    return HI_SUCCESS;
}


static hi_s32 isp_fe_af_reg_config(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg_info)
{
    hi_u8  af_enbale[ISP_MAX_PIPE_NUM] = {ISP_PIPE_FEAF_MODULE_ENABLE};
    hi_u16 crop_width, crop_pos_x;
    hi_u32 i;
    VI_PIPE vi_pipe_bind;
    isp_af_reg_cfg  *af_reg_fe_cfg = HI_NULL;
    S_ISPFE_REGS_TYPE *fe_reg      = HI_NULL;
    isp_usr_ctx         *isp_ctx     = HI_NULL;

    ISP_GET_CTX(vi_pipe, isp_ctx);

    if (isp_ctx->wdr_attr.mast_pipe && reg_cfg_info->cfg_key.bit1_af_fe_cfg) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            vi_pipe_bind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];

            ISP_CHECK_PIPE(vi_pipe_bind);

            if (!af_enbale[vi_pipe_bind]) {
                continue;
            }

            fe_reg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(vi_pipe_bind);
            ISP_CHECK_POINTER(fe_reg);

            af_reg_fe_cfg    = &reg_cfg_info->alg_reg_cfg[0].fe_af_reg_cfg;

            isp_af1_bayer_mode_write(fe_reg, af_reg_fe_cfg->bayer_mode);
            isp_af1_lpf_en_write(fe_reg, af_reg_fe_cfg->lpf_enable);
            isp_af1_fir0_lpf_en_write(fe_reg, af_reg_fe_cfg->fir0_lpf_enable);
            isp_af1_fir1_lpf_en_write(fe_reg, af_reg_fe_cfg->fir1_lpf_enable);
            isp_af1_iir0_ds_en_write(fe_reg, af_reg_fe_cfg->iir0_ds_enable);
            isp_af1_iir1_ds_en_write(fe_reg, af_reg_fe_cfg->iir1_ds_enable);
            isp_af1_iir_delay0_write(fe_reg,  af_reg_fe_cfg->iir0_shift);
            isp_af1_iir_delay1_write(fe_reg,  af_reg_fe_cfg->iir1_shift);
            isp_af1_iirplg_0_write(fe_reg, af_reg_fe_cfg->iir_plg_group0);
            isp_af1_iirpls_0_write(fe_reg, af_reg_fe_cfg->iir_pls_group0);
            isp_af1_iirplg_1_write(fe_reg, af_reg_fe_cfg->iir_plg_group1);
            isp_af1_iirpls_1_write(fe_reg, af_reg_fe_cfg->iir_pls_group1);

            isp_fe_af1_en_write(fe_reg, af_reg_fe_cfg->af_enable);
            isp_af1_iir0_en0_write(fe_reg, af_reg_fe_cfg->iir0_enable0);
            isp_af1_iir0_en1_write(fe_reg, af_reg_fe_cfg->iir0_enable1);
            isp_af1_iir0_en2_write(fe_reg, af_reg_fe_cfg->iir0_enable2);
            isp_af1_iir1_en0_write(fe_reg, af_reg_fe_cfg->iir1_enable0);
            isp_af1_iir1_en1_write(fe_reg, af_reg_fe_cfg->iir1_enable1);
            isp_af1_iir1_en2_write(fe_reg, af_reg_fe_cfg->iir1_enable2);
            isp_af1_peak_mode_write(fe_reg, af_reg_fe_cfg->peak_mode);
            isp_af1_squ_mode_write(fe_reg, af_reg_fe_cfg->squ_mode);
            isp_af1_hnum_write(fe_reg, af_reg_fe_cfg->window_hnum);
            isp_af1_vnum_write(fe_reg, af_reg_fe_cfg->window_vnum);

            isp_af1_iirg0_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain0_group0);
            isp_af1_iirg0_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain0_group1);

            isp_af1_iirg1_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain1_group0);
            isp_af1_iirg1_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain1_group1);

            isp_af1_iirg2_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain2_group0);
            isp_af1_iirg2_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain2_group1);

            isp_af1_iirg3_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain3_group0);
            isp_af1_iirg3_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain3_group1);

            isp_af1_iirg4_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain4_group0);
            isp_af1_iirg4_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain4_group1);

            isp_af1_iirg5_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain5_group0);
            isp_af1_iirg5_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain5_group1);

            isp_af1_iirg6_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain6_group0);
            isp_af1_iirg6_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->iir_gain6_group1);

            isp_af1_iirshift0_0_write(fe_reg, af_reg_fe_cfg->iir0_shift_group0);
            isp_af1_iirshift0_1_write(fe_reg, af_reg_fe_cfg->iir1_shift_group0);
            isp_af1_iirshift0_2_write(fe_reg, af_reg_fe_cfg->iir2_shift_group0);
            isp_af1_iirshift0_3_write(fe_reg, af_reg_fe_cfg->iir3_shift_group0);
            isp_af1_iirshift1_0_write(fe_reg, af_reg_fe_cfg->iir0_shift_group1);
            isp_af1_iirshift1_1_write(fe_reg, af_reg_fe_cfg->iir1_shift_group1);
            isp_af1_iirshift1_2_write(fe_reg, af_reg_fe_cfg->iir2_shift_group1);
            isp_af1_iirshift1_3_write(fe_reg, af_reg_fe_cfg->iir3_shift_group1);

            isp_af1_firh0_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->fir_h_gain0_group0);
            isp_af1_firh0_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->fir_h_gain0_group1);

            isp_af1_firh1_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->fir_h_gain1_group0);
            isp_af1_firh1_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->fir_h_gain1_group1);

            isp_af1_firh2_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->fir_h_gain2_group0);
            isp_af1_firh2_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->fir_h_gain2_group1);

            isp_af1_firh3_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->fir_h_gain3_group0);
            isp_af1_firh3_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->fir_h_gain3_group1);

            isp_af1_firh4_0_write(fe_reg, (hi_u32)af_reg_fe_cfg->fir_h_gain4_group0);
            isp_af1_firh4_1_write(fe_reg, (hi_u32)af_reg_fe_cfg->fir_h_gain4_group1);

            /* AF FE crop */
            isp_af1_crop_en_write(fe_reg, af_reg_fe_cfg->crop_enable);
            if (af_reg_fe_cfg->crop_enable) {
                crop_pos_x  = IS_HRS_ON(vi_pipe_bind) ? (af_reg_fe_cfg->crop_pos_x >> 1) : af_reg_fe_cfg->crop_pos_x;
                isp_af1_pos_x_write(fe_reg, crop_pos_x);
                isp_af1_pos_y_write(fe_reg, af_reg_fe_cfg->crop_pos_y);

                crop_width = IS_HRS_ON(vi_pipe_bind) ? (af_reg_fe_cfg->crop_h_size >> 1) : af_reg_fe_cfg->crop_h_size;
                crop_width = ISP_ALIGNUP(crop_width, 8);
                isp_af1_crop_hsize_write(fe_reg, crop_width - 1);
                isp_af1_crop_vsize_write(fe_reg, af_reg_fe_cfg->crop_v_size - 1);
            }

            /* AF FE raw cfg */
            isp_af1_raw_mode_write(fe_reg, af_reg_fe_cfg->raw_mode);
            isp_af1_gain_lmt_write(fe_reg, af_reg_fe_cfg->gain_limit);
            isp_af1_gamma_write(fe_reg, af_reg_fe_cfg->gamma);
            isp_af1_bayer_mode_write(fe_reg, af_reg_fe_cfg->bayer_mode);
            isp_af1_offset_en_write(fe_reg, af_reg_fe_cfg->offset_enable);
            isp_af1_offset_gr_write(fe_reg, af_reg_fe_cfg->offset_gr);
            isp_af1_offset_gb_write(fe_reg, af_reg_fe_cfg->offset_gb);

            /* AF FE pre median filter */
            isp_af1_mean_en_write(fe_reg, af_reg_fe_cfg->mean_enable);
            isp_af1_mean_thres_write(fe_reg, 0xffff - af_reg_fe_cfg->mean_thres);

            /* level depend gain */
            isp_af1_iir0_ldg_en_write(fe_reg, af_reg_fe_cfg->iir0_ldg_enable);
            isp_af1_iir_thre0_l_write(fe_reg, af_reg_fe_cfg->iir_thre0_low);
            isp_af1_iir_thre0_h_write(fe_reg, af_reg_fe_cfg->iir_thre0_high);
            isp_af1_iir_slope0_l_write(fe_reg, af_reg_fe_cfg->iir_slope0_low);
            isp_af1_iir_slope0_h_write(fe_reg, af_reg_fe_cfg->iir_slope0_high);
            isp_af1_iir_gain0_l_write(fe_reg, af_reg_fe_cfg->iir_gain0_low);
            isp_af1_iir_gain0_h_write(fe_reg, af_reg_fe_cfg->iir_gain0_high);

            isp_af1_iir1_ldg_en_write(fe_reg, af_reg_fe_cfg->iir1_ldg_enable);
            isp_af1_iir_thre1_l_write(fe_reg, af_reg_fe_cfg->iir_thre1_low);
            isp_af1_iir_thre1_h_write(fe_reg, af_reg_fe_cfg->iir_thre1_high);
            isp_af1_iir_slope1_l_write(fe_reg, af_reg_fe_cfg->iir_slope1_low);
            isp_af1_iir_slope1_h_write(fe_reg, af_reg_fe_cfg->iir_slope1_high);
            isp_af1_iir_gain1_l_write(fe_reg, af_reg_fe_cfg->iir_gain1_low);
            isp_af1_iir_gain1_h_write(fe_reg, af_reg_fe_cfg->iir_gain1_high);

            isp_af1_fir0_ldg_en_write(fe_reg, af_reg_fe_cfg->fir0_ldg_enable);
            isp_af1_fir_thre0_l_write(fe_reg, af_reg_fe_cfg->fir_thre0_low);
            isp_af1_fir_thre0_h_write(fe_reg, af_reg_fe_cfg->fir_thre0_high);
            isp_af1_fir_slope0_l_write(fe_reg, af_reg_fe_cfg->fir_slope0_low);
            isp_af1_fir_slope0_h_write(fe_reg, af_reg_fe_cfg->fir_slope0_high);
            isp_af1_fir_gain0_l_write(fe_reg, af_reg_fe_cfg->fir_gain0_low);
            isp_af1_fir_gain0_h_write(fe_reg, af_reg_fe_cfg->fir_gain0_high);

            isp_af1_fir1_ldg_en_write(fe_reg, af_reg_fe_cfg->fir1_ldg_enable);
            isp_af1_fir_thre1_l_write(fe_reg, af_reg_fe_cfg->fir_thre1_low);
            isp_af1_fir_thre1_h_write(fe_reg, af_reg_fe_cfg->fir_thre1_high);
            isp_af1_fir_slope1_l_write(fe_reg, af_reg_fe_cfg->fir_slope1_low);
            isp_af1_fir_slope1_h_write(fe_reg, af_reg_fe_cfg->fir_slope1_high);
            isp_af1_fir_gain1_l_write(fe_reg, af_reg_fe_cfg->fir_gain1_low);
            isp_af1_fir_gain1_h_write(fe_reg, af_reg_fe_cfg->fir_gain1_high);

            /* AF FE coring */
            isp_af1_iir_thre0_c_write(fe_reg, af_reg_fe_cfg->iir_thre0_coring);
            isp_af1_iir_slope0_c_write(fe_reg, af_reg_fe_cfg->iir_slope0_coring);
            isp_af1_iir_peak0_c_write(fe_reg, af_reg_fe_cfg->iir_peak0_coring);

            isp_af1_iir_thre1_c_write(fe_reg, af_reg_fe_cfg->iir_thre1_coring);
            isp_af1_iir_slope1_c_write(fe_reg, af_reg_fe_cfg->iir_slope1_coring);
            isp_af1_iir_peak1_c_write(fe_reg, af_reg_fe_cfg->iir_peak1_coring);

            isp_af1_fir_thre0_c_write(fe_reg, af_reg_fe_cfg->fir_thre0_coring);
            isp_af1_fir_slope0_c_write(fe_reg, af_reg_fe_cfg->fir_slope0_coring);
            isp_af1_fir_peak0_c_write(fe_reg, af_reg_fe_cfg->fir_peak0_coring);

            isp_af1_fir_thre1_c_write(fe_reg, af_reg_fe_cfg->fir_thre1_coring);
            isp_af1_fir_slope1_c_write(fe_reg, af_reg_fe_cfg->fir_slope1_coring);
            isp_af1_fir_peak1_c_write(fe_reg, af_reg_fe_cfg->fir_peak1_coring);

            /* high luma counter */
            isp_af1_hilight_write(fe_reg, af_reg_fe_cfg->hiligh_thre);

            /* AF output shift */
            isp_af1_acc_shift0_h_write(fe_reg, af_reg_fe_cfg->acc_shift0_h);
            isp_af1_acc_shift1_h_write(fe_reg, af_reg_fe_cfg->acc_shift1_h);
            isp_af1_acc_shift0_v_write(fe_reg, af_reg_fe_cfg->acc_shift0_v);
            isp_af1_acc_shift1_v_write(fe_reg, af_reg_fe_cfg->acc_shift1_v);
            isp_af1_acc_shift_y_write(fe_reg, af_reg_fe_cfg->acc_shift_y);
            isp_af1_cnt_shift_y_write(fe_reg, af_reg_fe_cfg->shift_count_y);
            isp_af1_cnt_shift0_h_write(fe_reg, 0x0);
            isp_af1_cnt_shift1_h_write(fe_reg, 0x0);
            isp_af1_cnt_shift0_v_write(fe_reg, ISP_AF_CNT_SHIFT0_V_DEFAULT);
            isp_af1_cnt_shift1_v_write(fe_reg, 0x0);
        }

        reg_cfg_info->cfg_key.bit1_af_fe_cfg = 0;
    }


    return HI_SUCCESS;
}

static hi_s32 isp_af_reg_config(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool  is_offline_mode;
    hi_bool  usr_resh  = HI_FALSE;
    hi_bool  idx_resh  = HI_FALSE;
    hi_u8    blk_num  = reg_cfg_info->cfg_num;
    isp_af_reg_cfg   *af_reg_be_cfg = HI_NULL;
    S_VIPROC_REGS_TYPE *vi_proc_reg  = HI_NULL;
    S_ISPBE_REGS_TYPE  *be_reg      = HI_NULL;
    isp_usr_ctx          *isp_ctx     = HI_NULL;

    ISP_GET_CTX(vi_pipe, isp_ctx);

    af_reg_be_cfg  = &reg_cfg_info->alg_reg_cfg[i].be_af_reg_cfg;
    is_offline_mode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    be_reg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(vi_pipe, i);
    vi_proc_reg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(vi_pipe, i);
    ISP_CHECK_POINTER(be_reg);
    ISP_CHECK_POINTER(vi_proc_reg);

    idx_resh = (isp_af_update_index_read(be_reg) != af_reg_be_cfg->update_index);
    usr_resh = (is_offline_mode) ? (reg_cfg_info->cfg_key.bit1_af_be_cfg & idx_resh) : (reg_cfg_info->cfg_key.bit1_af_be_cfg);

    if (usr_resh) {
        isp_af_update_index_write(be_reg, af_reg_be_cfg->update_index);
        isp_af_lpf_en_write(be_reg, af_reg_be_cfg->lpf_enable);
        isp_af_fir0_lpf_en_write(be_reg, af_reg_be_cfg->fir0_lpf_enable);
        isp_af_fir1_lpf_en_write(be_reg, af_reg_be_cfg->fir1_lpf_enable);
        isp_af_iir0_ds_en_write(be_reg, af_reg_be_cfg->iir0_ds_enable);
        isp_af_iir1_ds_en_write(be_reg, af_reg_be_cfg->iir1_ds_enable);
        isp_af_iir_delay0_write(be_reg, af_reg_be_cfg->iir0_shift);
        isp_af_iir_delay1_write(be_reg, af_reg_be_cfg->iir1_shift);
        isp_af_iirplg_0_write(be_reg, af_reg_be_cfg->iir_plg_group0);
        isp_af_iirpls_0_write(be_reg, af_reg_be_cfg->iir_pls_group0);
        isp_af_iirplg_1_write(be_reg, af_reg_be_cfg->iir_plg_group1);
        isp_af_iirpls_1_write(be_reg, af_reg_be_cfg->iir_pls_group1);

        isp_af_en_write(vi_proc_reg, af_reg_be_cfg->af_enable);
        isp_af_iir0_en0_write(be_reg, af_reg_be_cfg->iir0_enable0);
        isp_af_iir0_en1_write(be_reg, af_reg_be_cfg->iir0_enable1);
        isp_af_iir0_en2_write(be_reg, af_reg_be_cfg->iir0_enable2);
        isp_af_iir1_en0_write(be_reg, af_reg_be_cfg->iir1_enable0);
        isp_af_iir1_en1_write(be_reg, af_reg_be_cfg->iir1_enable1);
        isp_af_iir1_en2_write(be_reg, af_reg_be_cfg->iir1_enable2);
        isp_af_peak_mode_write(be_reg, af_reg_be_cfg->peak_mode);
        isp_af_squ_mode_write(be_reg, af_reg_be_cfg->squ_mode);
        isp_af_hnum_write(be_reg, af_reg_be_cfg->window_hnum);
        isp_af_vnum_write(be_reg, af_reg_be_cfg->window_vnum);

        isp_af_iirg0_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain0_group0);
        isp_af_iirg0_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain0_group1);

        isp_af_iirg1_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain1_group0);
        isp_af_iirg1_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain1_group1);

        isp_af_iirg2_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain2_group0);
        isp_af_iirg2_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain2_group1);

        isp_af_iirg3_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain3_group0);
        isp_af_iirg3_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain3_group1);

        isp_af_iirg4_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain4_group0);
        isp_af_iirg4_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain4_group1);

        isp_af_iirg5_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain5_group0);
        isp_af_iirg5_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain5_group1);

        isp_af_iirg6_0_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain6_group0);
        isp_af_iirg6_1_write(be_reg, (hi_u32)af_reg_be_cfg->iir_gain6_group1);

        isp_af_iirshift0_0_write(be_reg, af_reg_be_cfg->iir0_shift_group0);
        isp_af_iirshift0_1_write(be_reg, af_reg_be_cfg->iir1_shift_group0);
        isp_af_iirshift0_2_write(be_reg, af_reg_be_cfg->iir2_shift_group0);
        isp_af_iirshift0_3_write(be_reg, af_reg_be_cfg->iir3_shift_group0);
        isp_af_iirshift1_0_write(be_reg, af_reg_be_cfg->iir0_shift_group1);
        isp_af_iirshift1_1_write(be_reg, af_reg_be_cfg->iir1_shift_group1);
        isp_af_iirshift1_2_write(be_reg, af_reg_be_cfg->iir2_shift_group1);
        isp_af_iirshift1_3_write(be_reg, af_reg_be_cfg->iir3_shift_group1);

        isp_af_firh0_0_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain0_group0);
        isp_af_firh0_1_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain0_group1);

        isp_af_firh1_0_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain1_group0);
        isp_af_firh1_1_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain1_group1);

        isp_af_firh2_0_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain2_group0);
        isp_af_firh2_1_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain2_group1);

        isp_af_firh3_0_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain3_group0);
        isp_af_firh3_1_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain3_group1);

        isp_af_firh4_0_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain4_group0);
        isp_af_firh4_1_write(be_reg, (hi_u32)af_reg_be_cfg->fir_h_gain4_group1);

        /* AF BE crop */
        isp_af_crop_en_write(be_reg, af_reg_be_cfg->crop_enable);
        if (af_reg_be_cfg->crop_enable) {
            isp_af_pos_x_write(be_reg, af_reg_be_cfg->crop_pos_x);
            isp_af_pos_y_write(be_reg, af_reg_be_cfg->crop_pos_y);
            isp_af_crop_hsize_write(be_reg, af_reg_be_cfg->crop_h_size - 1);
            isp_af_crop_vsize_write(be_reg, af_reg_be_cfg->crop_v_size - 1);
        }

        /* AF BE raw cfg */
        isp_af_sel_write(be_reg, af_reg_be_cfg->af_pos_sel);
        isp_af_raw_mode_write(be_reg, af_reg_be_cfg->raw_mode);
        isp_af_gain_lmt_write(be_reg, af_reg_be_cfg->gain_limit);
        isp_af_gamma_write(be_reg, af_reg_be_cfg->gamma);
        isp_af_bayer_mode_write(be_reg, af_reg_be_cfg->bayer_mode);
        isp_af_offset_en_write(be_reg, af_reg_be_cfg->offset_enable);
        isp_af_offset_gr_write(be_reg, af_reg_be_cfg->offset_gr);
        isp_af_offset_gb_write(be_reg, af_reg_be_cfg->offset_gb);

        /* AF BE pre median filter */
        isp_af_mean_en_write(be_reg, af_reg_be_cfg->mean_enable);
        isp_af_mean_thres_write(be_reg, 0xffff - af_reg_be_cfg->mean_thres);

        /* level depend gain */
        isp_af_iir0_ldg_en_write(be_reg, af_reg_be_cfg->iir0_ldg_enable);
        isp_af_iir_thre0_l_write(be_reg, af_reg_be_cfg->iir_thre0_low);
        isp_af_iir_thre0_h_write(be_reg, af_reg_be_cfg->iir_thre0_high);
        isp_af_iir_slope0_l_write(be_reg, af_reg_be_cfg->iir_slope0_low);
        isp_af_iir_slope0_h_write(be_reg, af_reg_be_cfg->iir_slope0_high);
        isp_af_iir_gain0_l_write(be_reg, af_reg_be_cfg->iir_gain0_low);
        isp_af_iir_gain0_h_write(be_reg, af_reg_be_cfg->iir_gain0_high);

        isp_af_iir1_ldg_en_write(be_reg, af_reg_be_cfg->iir1_ldg_enable);
        isp_af_iir_thre1_l_write(be_reg, af_reg_be_cfg->iir_thre1_low);
        isp_af_iir_thre1_h_write(be_reg, af_reg_be_cfg->iir_thre1_high);
        isp_af_iir_slope1_l_write(be_reg, af_reg_be_cfg->iir_slope1_low);
        isp_af_iir_slope1_h_write(be_reg, af_reg_be_cfg->iir_slope1_high);
        isp_af_iir_gain1_l_write(be_reg, af_reg_be_cfg->iir_gain1_low);
        isp_af_iir_gain1_h_write(be_reg, af_reg_be_cfg->iir_gain1_high);

        isp_af_fir0_ldg_en_write(be_reg, af_reg_be_cfg->fir0_ldg_enable);
        isp_af_fir_thre0_l_write(be_reg, af_reg_be_cfg->fir_thre0_low);
        isp_af_fir_thre0_h_write(be_reg, af_reg_be_cfg->fir_thre0_high);
        isp_af_fir_slope0_l_write(be_reg, af_reg_be_cfg->fir_slope0_low);
        isp_af_fir_slope0_h_write(be_reg, af_reg_be_cfg->fir_slope0_high);
        isp_af_fir_gain0_l_write(be_reg, af_reg_be_cfg->fir_gain0_low);
        isp_af_fir_gain0_h_write(be_reg, af_reg_be_cfg->fir_gain0_high);

        isp_af_fir1_ldg_en_write(be_reg, af_reg_be_cfg->fir1_ldg_enable);
        isp_af_fir_thre1_l_write(be_reg, af_reg_be_cfg->fir_thre1_low);
        isp_af_fir_thre1_h_write(be_reg, af_reg_be_cfg->fir_thre1_high);
        isp_af_fir_slope1_l_write(be_reg, af_reg_be_cfg->fir_slope1_low);
        isp_af_fir_slope1_h_write(be_reg, af_reg_be_cfg->fir_slope1_high);
        isp_af_fir_gain1_l_write(be_reg, af_reg_be_cfg->fir_gain1_low);
        isp_af_fir_gain1_h_write(be_reg, af_reg_be_cfg->fir_gain1_high);

        /* AF BE coring */
        isp_af_iir_thre0_c_write(be_reg, af_reg_be_cfg->iir_thre0_coring);
        isp_af_iir_slope0_c_write(be_reg, af_reg_be_cfg->iir_slope0_coring);
        isp_af_iir_peak0_c_write(be_reg, af_reg_be_cfg->iir_peak0_coring);

        isp_af_iir_thre1_c_write(be_reg, af_reg_be_cfg->iir_thre1_coring);
        isp_af_iir_slope1_c_write(be_reg, af_reg_be_cfg->iir_slope1_coring);
        isp_af_iir_peak1_c_write(be_reg, af_reg_be_cfg->iir_peak1_coring);

        isp_af_fir_thre0_c_write(be_reg, af_reg_be_cfg->fir_thre0_coring);
        isp_af_fir_slope0_c_write(be_reg, af_reg_be_cfg->fir_slope0_coring);
        isp_af_fir_peak0_c_write(be_reg, af_reg_be_cfg->fir_peak0_coring);

        isp_af_fir_thre1_c_write(be_reg, af_reg_be_cfg->fir_thre1_coring);
        isp_af_fir_slope1_c_write(be_reg, af_reg_be_cfg->fir_slope1_coring);
        isp_af_fir_peak1_c_write(be_reg, af_reg_be_cfg->fir_peak1_coring);

        /* high luma counter */
        isp_af_hilight_write(be_reg, af_reg_be_cfg->hiligh_thre);

        /* AF output shift */
        isp_af_acc_shift0_h_write(be_reg, af_reg_be_cfg->acc_shift0_h);
        isp_af_acc_shift1_h_write(be_reg, af_reg_be_cfg->acc_shift1_h);
        isp_af_acc_shift0_v_write(be_reg, af_reg_be_cfg->acc_shift0_v);
        isp_af_acc_shift1_v_write(be_reg, af_reg_be_cfg->acc_shift1_v);
        isp_af_acc_shift_y_write(be_reg, af_reg_be_cfg->acc_shift_y);
        isp_af_cnt_shift_y_write(be_reg, af_reg_be_cfg->shift_count_y);
        isp_af_cnt_shift0_v_write(be_reg, ISP_AF_CNT_SHIFT0_V_DEFAULT);
        isp_af_cnt_shift0_h_write(be_reg, 0x0);
        isp_af_cnt_shift1_h_write(be_reg, 0x0);
        isp_af_cnt_shift1_v_write(be_reg, 0x0);

        reg_cfg_info->cfg_key.bit1_af_be_cfg = is_offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_FeAwbRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_U32 i;
    VI_PIPE ViPipeBind;
    ISP_AWB_REG_DYN_CFG_S  *pstAwbRegDynCfg = HI_NULL;
    ISP_AWB_REG_STA_CFG_S  *pstAwbRegStaCfg = HI_NULL;
    S_ISPFE_REGS_TYPE      *pstFeReg        = HI_NULL;
    isp_usr_ctx              *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->wdr_attr.mast_pipe) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            ViPipeBind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
            ISP_CHECK_PIPE(ViPipeBind);
            pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
            ISP_CHECK_POINTER(pstFeReg);

            if (isp_ctx->linkage.snap_state == HI_TRUE) {
                isp_wb1_rgain_write(pstFeReg, isp_ctx->snap_isp_info.isp_cfg_info.white_balance_gain[0]);
                isp_wb1_grgain_write(pstFeReg, isp_ctx->snap_isp_info.isp_cfg_info.white_balance_gain[1]);
                isp_wb1_gbgain_write(pstFeReg, isp_ctx->snap_isp_info.isp_cfg_info.white_balance_gain[2]);
                isp_wb1_bgain_write(pstFeReg, isp_ctx->snap_isp_info.isp_cfg_info.white_balance_gain[3]);
            }

            if (pstRegCfgInfo->cfg_key.bit1AwbDynCfg) {
                pstAwbRegDynCfg = &pstRegCfgInfo->alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg;
                isp_wb1_rgain_write(pstFeReg, pstAwbRegDynCfg->au32FEWhiteBalanceGain[0]);
                isp_wb1_grgain_write(pstFeReg, pstAwbRegDynCfg->au32FEWhiteBalanceGain[1]);
                isp_wb1_gbgain_write(pstFeReg, pstAwbRegDynCfg->au32FEWhiteBalanceGain[2]);
                isp_wb1_bgain_write(pstFeReg, pstAwbRegDynCfg->au32FEWhiteBalanceGain[3]);
                isp_fe_wb1_en_write(pstFeReg, pstAwbRegDynCfg->u8FEWbWorkEn);
            }

            pstAwbRegStaCfg = &pstRegCfgInfo->alg_reg_cfg[0].stAwbRegCfg.stAwbRegStaCfg;

            if (pstAwbRegStaCfg->bFEAwbStaCfg) {
                pstAwbRegStaCfg = &pstRegCfgInfo->alg_reg_cfg[0].stAwbRegCfg.stAwbRegStaCfg;
                isp_wb1_clip_value_write(pstFeReg, pstAwbRegStaCfg->u32FEClipValue);
            }
        }
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_AwbCCSet(S_ISPBE_REGS_TYPE *pstBeReg, HI_U16 *pu16BeCC)
{
    isp_cc_coef00_write(pstBeReg, CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[0])));
    isp_cc_coef01_write(pstBeReg, CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[1])));
    isp_cc_coef02_write(pstBeReg, CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[2])));
    isp_cc_coef10_write(pstBeReg, CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[3])));
    isp_cc_coef11_write(pstBeReg, CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[4])));
    isp_cc_coef12_write(pstBeReg, CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[5])));
    isp_cc_coef20_write(pstBeReg, CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[6])));
    isp_cc_coef21_write(pstBeReg, CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[7])));
    isp_cc_coef22_write(pstBeReg, CCM_CONVERT(CCM_CONVERT_PRE(pu16BeCC[8])));

    return HI_SUCCESS;
}

static HI_S32 ISP_AwbGainSet(S_ISPBE_REGS_TYPE *pstBeReg, HI_U32 *pu32BeWBGain)
{
    isp_wb_rgain_write(pstBeReg, pu32BeWBGain[0]);
    isp_wb_grgain_write(pstBeReg, pu32BeWBGain[1]);
    isp_wb_gbgain_write(pstBeReg, pu32BeWBGain[2]);
    isp_wb_bgain_write(pstBeReg, pu32BeWBGain[3]);

    return HI_SUCCESS;
}

static HI_S32 ISP_StitchAwbSyncCfg(VI_PIPE ViPipe, ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfg, HI_U8 i, S_ISPBE_REGS_TYPE *pstBeReg)
{
    HI_S32 k;
    VI_PIPE ViPipeS;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_reg_cfg_attr      *pstRegCfgS = HI_NULL;
    S_ISPBE_REGS_TYPE *pstBeRegS  = HI_NULL;
    ISP_AWB_REG_DYN_CFG_S *pstAwbRegDynCfgS = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->para_rec.init == HI_TRUE) {
        if (isp_ctx->stitch_attr.main_pipe == HI_TRUE) {
            for (k = 0; k < isp_ctx->stitch_attr.stitch_pipe_num; k++) {
                ViPipeS = isp_ctx->stitch_attr.stitch_bind_id[k];
                ISP_CHECK_PIPE(ViPipeS);
                ISP_REGCFG_GET_CTX(ViPipeS, pstRegCfgS);
                pstBeRegS = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipeS, i);
                ISP_CHECK_POINTER(pstBeRegS);
                pstAwbRegDynCfgS = &pstRegCfgS->reg_cfg.alg_reg_cfg[i].stAwbRegCfg.stAwbRegDynCfg;

                ISP_AwbCCSet(pstBeRegS, pstAwbRegDynCfgS->au16BEColorMatrix);
                ISP_AwbGainSet(pstBeRegS, pstAwbRegDynCfgS->au32BEWhiteBalanceGain);
            }
        }
    } else {
        ISP_AwbCCSet(pstBeReg, pstAwbRegDynCfg->au16BEColorMatrix);
        ISP_AwbGainSet(pstBeReg, pstAwbRegDynCfg->au32BEWhiteBalanceGain);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_AwbRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    ISP_AWB_REG_DYN_CFG_S  *pstAwbRegDynCfg = HI_NULL;
    ISP_AWB_REG_STA_CFG_S  *pstAwbRegStaCfg = HI_NULL;
    ISP_AWB_REG_USR_CFG_S  *pstAwbRegUsrCfg = HI_NULL;
    S_VIPROC_REGS_TYPE     *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE      *pstBeReg        = HI_NULL;
    S_ISPBE_REGS_TYPE      *pstBeRegEx      = HI_NULL;
    HI_BOOL  bIsOfflineMode;
    isp_usr_ctx *isp_ctx   = HI_NULL;
    HI_BOOL bIdxResh, bUsrResh;

    ISP_GET_CTX(ViPipe, isp_ctx);

    pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
    pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
    ISP_CHECK_POINTER(pstBeReg);
    ISP_CHECK_POINTER(pstViProcReg);

    pstAwbRegDynCfg = &pstRegCfgInfo->alg_reg_cfg[i].stAwbRegCfg.stAwbRegDynCfg;
    pstAwbRegStaCfg = &pstRegCfgInfo->alg_reg_cfg[i].stAwbRegCfg.stAwbRegStaCfg;
    pstAwbRegUsrCfg = &pstRegCfgInfo->alg_reg_cfg[i].stAwbRegCfg.stAwbRegUsrCfg;

    if (isp_ctx->linkage.snap_state == HI_TRUE) {
        if (isp_ctx->linkage.load_ccm == HI_TRUE) {
            ISP_AwbCCSet(pstBeReg, isp_ctx->snap_isp_info.isp_cfg_info.cap_ccm);
        } else {
            ISP_AwbCCSet(pstBeReg, pstAwbRegDynCfg->au16BEColorMatrix);
        }

        ISP_AwbGainSet(pstBeReg, isp_ctx->snap_isp_info.isp_cfg_info.white_balance_gain);
    }

    if (pstRegCfgInfo->cfg_key.bit1AwbDynCfg) {
        if (isp_ctx->linkage.snap_state != HI_TRUE) {
            if (isp_ctx->linkage.snap_pipe_mode != ISP_SNAP_PICTURE) {
                if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
                    ISP_StitchAwbSyncCfg(ViPipe, pstAwbRegDynCfg, i, pstBeReg);
                } else {
                    ISP_AwbCCSet(pstBeReg, pstAwbRegDynCfg->au16BEColorMatrix);
                    ISP_AwbGainSet(pstBeReg, pstAwbRegDynCfg->au32BEWhiteBalanceGain);
                }
            }
        }

        if ((IS_ONLINE_MODE(isp_ctx->linkage.picture_running_mode) || \
             IS_SIDEBYSIDE_MODE(isp_ctx->linkage.picture_running_mode)) && \
            (isp_ctx->linkage.snap_pipe_mode == ISP_SNAP_PREVIEW)) {
            ISP_CHECK_PIPE(isp_ctx->linkage.picture_pipe_id);
            pstBeRegEx = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(isp_ctx->linkage.picture_pipe_id, i);
            ISP_CHECK_POINTER(pstBeRegEx);
            ISP_AwbCCSet(pstBeRegEx, pstAwbRegDynCfg->au16BEColorMatrix);
            ISP_AwbGainSet(pstBeRegEx, pstAwbRegDynCfg->au32BEWhiteBalanceGain);
        }

        isp_awb_threshold_max_write(pstBeReg, pstAwbRegDynCfg->u16BEMeteringWhiteLevelAwb);
        isp_awb_threshold_min_write(pstBeReg, pstAwbRegDynCfg->u16BEMeteringBlackLevelAwb);
        isp_awb_cr_ref_max_write(pstBeReg, pstAwbRegDynCfg->u16BEMeteringCrRefMaxAwb);
        isp_awb_cr_ref_min_write(pstBeReg, pstAwbRegDynCfg->u16BEMeteringCrRefMinAwb);
        isp_awb_cb_ref_max_write(pstBeReg, pstAwbRegDynCfg->u16BEMeteringCbRefMaxAwb);
        isp_awb_cb_ref_min_write(pstBeReg, pstAwbRegDynCfg->u16BEMeteringCbRefMinAwb);

        isp_wb_en_write(pstViProcReg, pstAwbRegDynCfg->u8BEWbWorkEn);
        isp_cc_en_write(pstViProcReg, pstAwbRegDynCfg->u8BECcEn);

        isp_cc_colortone_en_write(pstBeReg, pstAwbRegDynCfg->u16BECcColortoneEn);
        isp_cc_r_gain_write(pstBeReg, pstAwbRegDynCfg->u16BECcRGain);
        isp_cc_g_gain_write(pstBeReg, pstAwbRegDynCfg->u16BECcGGain);
        isp_cc_b_gain_write(pstBeReg, pstAwbRegDynCfg->u16BECcBGain);

        isp_awb_crop_pos_x_write(pstBeReg, pstAwbRegDynCfg->u32BECropPosX);
        isp_awb_crop_pos_y_write(pstBeReg, pstAwbRegDynCfg->u32BECropPosY);
        isp_awb_crop_out_width_write(pstBeReg, pstAwbRegDynCfg->u32BECropOutWidth - 1);
        isp_awb_crop_out_height_write(pstBeReg, pstAwbRegDynCfg->u32BECropOutHeight - 1);
    }

    if (pstAwbRegStaCfg->bBEAwbStaCfg) {
        isp_awb_bitmove_write(pstBeReg, pstAwbRegStaCfg->u8BEAwbBitmove);
        isp_awb_en_write(pstViProcReg, pstAwbRegStaCfg->u8BEAwbWorkEn);

        isp_awb_stat_raddr_write(pstBeReg, pstAwbRegStaCfg->u32BEAwbStatRaddr);

        isp_cc_prot_en_write(pstBeReg, HI_ISP_CCM_PROT_EN_DEFAULT);
        isp_cc_data_cc_thd0_write(pstBeReg, HI_ISP_CCM_CC_THD0_DEFAULT);
        isp_cc_data_cc_thd1_write(pstBeReg, HI_ISP_CCM_CC_THD1_DEFAULT);
        isp_cc_cc_prot_ratio_write(pstBeReg, HI_ISP_CCM_CC_PROT_RATIO_DEFAULT);
        isp_cc_data_rr_thd0_write(pstBeReg, HI_ISP_CCM_RR_THD0_DEFAULT);
        isp_cc_data_rr_thd1_write(pstBeReg, HI_ISP_CCM_RR_THD1_DEFAULT);
        isp_cc_data_gg_thd0_write(pstBeReg, HI_ISP_CCM_GG_THD0_DEFAULT);
        isp_cc_data_gg_thd1_write(pstBeReg, HI_ISP_CCM_GG_THD1_DEFAULT);
        isp_cc_data_bb_thd0_write(pstBeReg, HI_ISP_CCM_BB_THD0_DEFAULT);
        isp_cc_data_bb_thd1_write(pstBeReg, HI_ISP_CCM_BB_THD1_DEFAULT);
        isp_cc_max_rgb_thd_write(pstBeReg, HI_ISP_CCM_MAX_RGB_DEFAULT);
        isp_cc_rgb_prot_ratio_write(pstBeReg, HI_ISP_CCM_RGB_PROT_RATIO_DEFAULT);

        isp_cc_in_dc0_write(pstBeReg, pstAwbRegStaCfg->u32BECcInDc0);
        isp_cc_in_dc1_write(pstBeReg, pstAwbRegStaCfg->u32BECcInDc1);
        isp_cc_in_dc2_write(pstBeReg, pstAwbRegStaCfg->u32BECcInDc2);
        isp_cc_out_dc0_write(pstBeReg, pstAwbRegStaCfg->u32BECcOutDc0);
        isp_cc_out_dc1_write(pstBeReg, pstAwbRegStaCfg->u32BECcOutDc1);
        isp_cc_out_dc2_write(pstBeReg, pstAwbRegStaCfg->u32BECcOutDc2);
        isp_wb_clip_value_write(pstBeReg, pstAwbRegStaCfg->u32BEWbClipValue);
        isp_awb_offset_comp_write(pstBeReg, pstAwbRegStaCfg->u16BEAwbOffsetComp);
        isp_awb_hist_weight_b_write(pstBeReg, HI_ISP_AWB_HIST_WEIGHT_B_DEFAULT);
        isp_awb_hist_weight_g_write(pstBeReg, HI_ISP_AWB_HIST_WEIGHT_G_DEFAULT);
        isp_awb_hist_weight_r_write(pstBeReg, HI_ISP_AWB_HIST_WEIGHT_R_DEFAULT);

        pstAwbRegStaCfg->bBEAwbStaCfg = 0;
    }

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    bIdxResh = (isp_awb_update_index_read(pstBeReg) != pstAwbRegUsrCfg->u32UpdateIndex);
    bUsrResh = (bIsOfflineMode) ? (pstAwbRegUsrCfg->bResh & bIdxResh) : (pstAwbRegUsrCfg->bResh);

    if (bUsrResh) {
        isp_awb_update_index_write(pstBeReg, pstAwbRegUsrCfg->u32UpdateIndex);

        isp_awb_sel_write(pstBeReg, pstAwbRegUsrCfg->enBEAWBSwitch);
        isp_awb_hist_bin0_write(pstBeReg, pstAwbRegUsrCfg->u16BEMeteringBinHist0);
        isp_awb_hist_bin1_write(pstBeReg, pstAwbRegUsrCfg->u16BEMeteringBinHist1);
        isp_awb_hist_bin2_write(pstBeReg, pstAwbRegUsrCfg->u16BEMeteringBinHist2);
        isp_awb_hist_bin3_write(pstBeReg, pstAwbRegUsrCfg->u16BEMeteringBinHist3);
        pstAwbRegUsrCfg->bResh = bIsOfflineMode;  // if online mode, bResh=0; if offline mode, bResh=1; but only index != will resh
    }
    isp_awb_hnum_write(pstBeReg, pstAwbRegUsrCfg->u16BEZoneCol);
    isp_awb_vnum_write(pstBeReg, pstAwbRegUsrCfg->u16BEZoneRow);
    isp_awb_bin_num_write(pstBeReg, pstAwbRegUsrCfg->u16BEZoneBin);

    return HI_SUCCESS;
}

static HI_S32 ISP_SharpenRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_BOOL  bUsrResh   = HI_FALSE;
    HI_BOOL  bIdxResh   = HI_FALSE;
    HI_BOOL  bLutUpdate = HI_FALSE;
    HI_U8    u8BlkNum   = pstRegCfgInfo->cfg_num;
    HI_U16   j;

    ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S *pstSharpenDefaultDynaRegCfg = HI_NULL;
    ISP_SHARPEN_MPI_DYNA_REG_CFG_S     *pstSharpenMpiDynaRegCfg     = HI_NULL;
    ISP_SHARPEN_STATIC_REG_CFG_S       *pstSharpenStaticRegCfg      = HI_NULL;
    S_VIPROC_REGS_TYPE                 *pstViProcReg                = HI_NULL;
    S_ISPBE_REGS_TYPE                  *pstBeReg                    = HI_NULL;
    isp_usr_ctx                          *isp_ctx                   = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1SharpenCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER((pstViProcReg && pstBeReg));

        isp_sharpen_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stSharpenRegCfg.bEnable);

        pstSharpenStaticRegCfg      = &pstRegCfgInfo->alg_reg_cfg[i].stSharpenRegCfg.stStaticRegCfg;
        pstSharpenDefaultDynaRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stSharpenRegCfg.stDynaRegCfg.stDefaultDynaRegCfg;
        pstSharpenMpiDynaRegCfg     = &pstRegCfgInfo->alg_reg_cfg[i].stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg;

        if (pstSharpenStaticRegCfg->bStaticResh) {
            isp_sharpen_mfthdseld_write(pstBeReg, pstSharpenStaticRegCfg->u8mfThdSelD);
            isp_sharpen_hfthdseld_write(pstBeReg, pstSharpenStaticRegCfg->u8hfThdSelD);
            isp_sharpen_dirvarscale_write(pstBeReg, pstSharpenStaticRegCfg->u8dirVarScale);
            isp_sharpen_benchrsad_write(pstBeReg, pstSharpenStaticRegCfg->u8bEnChrSad);
            isp_sharpen_dirrly0_write(pstBeReg, pstSharpenStaticRegCfg->u8dirRly[0]);
            isp_sharpen_dirrly1_write(pstBeReg, pstSharpenStaticRegCfg->u8dirRly[1]);
            isp_sharpen_omaxchg_write(pstBeReg, pstSharpenStaticRegCfg->u16oMaxChg);
            isp_sharpen_umaxchg_write(pstBeReg, pstSharpenStaticRegCfg->u16uMaxChg);
            isp_sharpen_shtvarsft_write(pstBeReg, pstSharpenStaticRegCfg->u8shtVarSft);
            isp_sharpen_oshtvarthd0_write(pstBeReg, pstSharpenStaticRegCfg->u8oshtVarThd0);
            isp_sharpen_ushtvarthd0_write(pstBeReg, pstSharpenStaticRegCfg->u8ushtVarThd0);
            isp_sharpen_oshtvardiffwgt0_write(pstBeReg, pstSharpenStaticRegCfg->u8oshtVarDiffWgt0);
            isp_sharpen_ushtvardiffwgt0_write(pstBeReg, pstSharpenStaticRegCfg->u8ushtVarDiffWgt0);
            isp_sharpen_oshtvarwgt1_write(pstBeReg, pstSharpenStaticRegCfg->u8oshtVarWgt1);
            isp_sharpen_ushtvarwgt1_write(pstBeReg, pstSharpenStaticRegCfg->u8ushtVarWgt1);
            isp_sharpen_lmtmf0_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtMF[0]);
            isp_sharpen_lmthf0_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtHF[0]);
            isp_sharpen_lmtmf1_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtMF[1]);
            isp_sharpen_lmthf1_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtHF[1]);
            isp_sharpen_lmtmf2_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtMF[2]);
            isp_sharpen_lmthf2_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtHF[2]);
            isp_sharpen_lmtmf3_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtMF[3]);
            isp_sharpen_lmthf3_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtHF[3]);
            isp_sharpen_lmtmf4_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtMF[4]);
            isp_sharpen_lmthf4_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtHF[4]);
            isp_sharpen_lmtmf5_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtMF[5]);
            isp_sharpen_lmthf5_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtHF[5]);
            isp_sharpen_lmtmf6_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtMF[6]);
            isp_sharpen_lmthf6_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtHF[6]);
            isp_sharpen_lmtmf7_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtMF[7]);
            isp_sharpen_lmthf7_write(pstBeReg, pstSharpenStaticRegCfg->u8lmtHF[7]);

            isp_sharpen_skinsrcsel_write(pstBeReg, pstSharpenStaticRegCfg->u8skinSrcSel);
            isp_sharpen_skinedgesft_write(pstBeReg, pstSharpenStaticRegCfg->u8skinEdgeSft);

            isp_sharpen_skincntthd0_write  (pstBeReg, pstSharpenStaticRegCfg->u8skinCntThd[0]);
            isp_sharpen_skinedgethd0_write (pstBeReg, pstSharpenStaticRegCfg->u8skinEdgeThd[0]);
            isp_sharpen_skinaccumthd0_write(pstBeReg, pstSharpenStaticRegCfg->u16skinAccumThd[0]);
            isp_sharpen_skinaccumwgt0_write(pstBeReg, pstSharpenStaticRegCfg->u8skinAccumWgt[0]);

            isp_sharpen_skincntthd1_write  (pstBeReg, pstSharpenStaticRegCfg->u8skinCntThd[1]);
            isp_sharpen_skinedgethd1_write (pstBeReg, pstSharpenStaticRegCfg->u8skinEdgeThd[1]);
            isp_sharpen_skinaccumthd1_write(pstBeReg, pstSharpenStaticRegCfg->u16skinAccumThd[1]);
            isp_sharpen_skinaccumwgt1_write(pstBeReg, pstSharpenStaticRegCfg->u8skinAccumWgt[1]);

            isp_sharpen_chrrvarsft_write  (pstBeReg, pstSharpenStaticRegCfg->u8chrRVarSft);
            isp_sharpen_chrrvarscale_write(pstBeReg, pstSharpenStaticRegCfg->u8chrRVarScale);
            isp_sharpen_chrrscale_write   (pstBeReg, pstSharpenStaticRegCfg->u16chrRScale);
            isp_sharpen_chrgvarsft_write  (pstBeReg, pstSharpenStaticRegCfg->u8chrGVarSft);
            isp_sharpen_chrgvarscale_write(pstBeReg, pstSharpenStaticRegCfg->u8chrGVarScale);
            isp_sharpen_chrgscale_write   (pstBeReg, pstSharpenStaticRegCfg->u16chrGScale);
            isp_sharpen_chrbvarsft_write  (pstBeReg, pstSharpenStaticRegCfg->u8chrBVarSft);
            isp_sharpen_chrbvarscale_write(pstBeReg, pstSharpenStaticRegCfg->u8chrBVarScale);
            isp_sharpen_chrbscale_write   (pstBeReg, pstSharpenStaticRegCfg->u16chrBScale);

            isp_sharpen_chrrori0_write (pstBeReg, pstSharpenStaticRegCfg->u8chrROri[0]);
            isp_sharpen_chrrthd0_write (pstBeReg, pstSharpenStaticRegCfg->u8chrRThd[0]);
            isp_sharpen_chrgori0_write (pstBeReg, pstSharpenStaticRegCfg->u8chrGOri[0]);
            isp_sharpen_chrgthd0_write (pstBeReg, pstSharpenStaticRegCfg->u8chrGThd[0]);
            isp_sharpen_chrbori0_write (pstBeReg, pstSharpenStaticRegCfg->u8chrBOri[0]);
            isp_sharpen_chrbthd0_write (pstBeReg, pstSharpenStaticRegCfg->u8chrBThd[0]);

            isp_sharpen_chrrori1_write (pstBeReg, pstSharpenStaticRegCfg->u8chrROri[1]);
            isp_sharpen_chrrthd1_write (pstBeReg, pstSharpenStaticRegCfg->u8chrRThd[1]);
            isp_sharpen_chrrgain1_write(pstBeReg, pstSharpenStaticRegCfg->u8chrRGain1);
            isp_sharpen_chrgori1_write (pstBeReg, pstSharpenStaticRegCfg->u8chrGOri[1]);
            isp_sharpen_chrgthd1_write (pstBeReg, pstSharpenStaticRegCfg->u8chrGThd[1]);
            isp_sharpen_chrggain1_write(pstBeReg, pstSharpenStaticRegCfg->u16chrGGain1);
            isp_sharpen_chrbori1_write (pstBeReg, pstSharpenStaticRegCfg->u8chrBOri[1]);
            isp_sharpen_chrbthd1_write (pstBeReg, pstSharpenStaticRegCfg->u8chrBThd[1]);
            isp_sharpen_chrbgain1_write(pstBeReg, pstSharpenStaticRegCfg->u8chrBGain1);

            //isp_sharpen_dirrt0_write(pstBeReg, pstSharpenStaticRegCfg->u8dirRt[0]);
            //isp_sharpen_dirrt1_write(pstBeReg, pstSharpenStaticRegCfg->u8dirRt[1]);
            isp_sharpen_shtnoisemax_write(pstBeReg, pstSharpenStaticRegCfg->u8shtNoiseMax);
            isp_sharpen_shtnoisemin_write(pstBeReg, pstSharpenStaticRegCfg->u8shtNoiseMin);

            isp_sharpen_chrrsft0_write(pstBeReg, pstSharpenStaticRegCfg->u8chrRSft[0]);
            isp_sharpen_chrgsft0_write(pstBeReg, pstSharpenStaticRegCfg->u8chrGSft[0]);
            isp_sharpen_chrbsft0_write(pstBeReg, pstSharpenStaticRegCfg->u8chrBSft[0]);
            isp_sharpen_chrrsft1_write(pstBeReg, pstSharpenStaticRegCfg->u8chrRSft[1]);
            isp_sharpen_chrgsft1_write(pstBeReg, pstSharpenStaticRegCfg->u8chrGSft[1]);
            isp_sharpen_chrbsft1_write(pstBeReg, pstSharpenStaticRegCfg->u8chrBSft[1]);
            isp_sharpen_chrrsft2_write(pstBeReg, pstSharpenStaticRegCfg->u8chrRSft[2]);
            isp_sharpen_chrgsft2_write(pstBeReg, pstSharpenStaticRegCfg->u8chrGSft[2]);
            isp_sharpen_chrbsft2_write(pstBeReg, pstSharpenStaticRegCfg->u8chrBSft[2]);
            isp_sharpen_chrrsft3_write(pstBeReg, pstSharpenStaticRegCfg->u8chrRSft[3]);
            isp_sharpen_chrgsft3_write(pstBeReg, pstSharpenStaticRegCfg->u8chrGSft[3]);
            isp_sharpen_chrbsft3_write(pstBeReg, pstSharpenStaticRegCfg->u8chrBSft[3]);
            isp_sharpen_lumasrcsel_write(pstBeReg, pstSharpenStaticRegCfg->u8lumaSrcSel);
            isp_sharpen_skincntmul_write(pstBeReg, pstSharpenStaticRegCfg->u8skinCntMul);
            isp_sharpen_skinaccummul_write(pstBeReg, pstSharpenStaticRegCfg->s16skinAccumMul);

            isp_sharpen_ben8dir_sel_write(pstBeReg, pstSharpenStaticRegCfg->bEnShp8Dir);
            isp_sharpen_benshplowpow_write(pstBeReg, pstSharpenStaticRegCfg->bEnShpLowPow);
            isp_sharpen_hfgain_sft_write(pstBeReg, pstSharpenStaticRegCfg->u8hfGainSft);
            isp_sharpen_mfgain_sft_write(pstBeReg, pstSharpenStaticRegCfg->u8mfGainSft);
            isp_sharpen_lpf_sel_write(pstBeReg, pstSharpenStaticRegCfg->u8lpfSel);
            isp_sharpen_hsf_sel_write(pstBeReg, pstSharpenStaticRegCfg->u8hsfSel);
            isp_sharpen_benshtvar_sel_write(pstBeReg, pstSharpenStaticRegCfg->u8shtVarSel);
            isp_sharpen_shtvar5x5_sft_write(pstBeReg, pstSharpenStaticRegCfg->u8shtVar5x5Sft);
            isp_sharpen_detailthd_sel_write(pstBeReg, pstSharpenStaticRegCfg->u8detailThdSel);
            isp_sharpen_dtl_thdsft_write(pstBeReg, pstSharpenStaticRegCfg->u8detailThdSft);

            pstSharpenStaticRegCfg->bStaticResh = HI_FALSE;
        }

        if (pstSharpenDefaultDynaRegCfg->bResh) {
            isp_sharpen_mfthdsftd_write (pstBeReg, pstSharpenDefaultDynaRegCfg->u8mfThdSftD);
            isp_sharpen_mfthdselud_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8mfThdSelUD);
            isp_sharpen_mfthdsftud_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8mfThdSftUD);
            isp_sharpen_hfthdsftd_write (pstBeReg, pstSharpenDefaultDynaRegCfg->u8hfThdSftD);
            isp_sharpen_hfthdselud_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8hfThdSelUD);
            isp_sharpen_hfthdsftud_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8hfThdSftUD);
            isp_sharpen_dirvarsft_write  (pstBeReg, pstSharpenDefaultDynaRegCfg->u8dirVarSft);
            isp_sharpen_oshtvarwgt0_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8oshtVarWgt0);
            isp_sharpen_ushtvarwgt0_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8ushtVarWgt0);
            isp_sharpen_oshtvardiffthd0_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[0]);
            isp_sharpen_ushtvardiffthd0_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8ushtVarDiffThd[0]);
            isp_sharpen_selpixwgt_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8selPixWgt);
            isp_sharpen_oshtvardiffthd1_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8oshtVarDiffThd[1]);
            isp_sharpen_oshtvardiffwgt1_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8oshtVarDiffWgt1);
            isp_sharpen_ushtvardiffthd1_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8ushtVarDiffThd[1]);
            isp_sharpen_ushtvardiffwgt1_write(pstBeReg, pstSharpenDefaultDynaRegCfg->u8ushtVarDiffWgt1);
            isp_sharpen_oshtvardiffmul_write(pstBeReg, pstSharpenDefaultDynaRegCfg->s16oshtVarDiffMul);
            isp_sharpen_ushtvardiffmul_write(pstBeReg, pstSharpenDefaultDynaRegCfg->s16ushtVarDiffMul);

            pstSharpenDefaultDynaRegCfg->bResh = bIsOfflineMode;
        }

        bIdxResh = (isp_sharpen_update_index_read(pstBeReg) != pstSharpenMpiDynaRegCfg->u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstSharpenMpiDynaRegCfg->bResh & bIdxResh) : (pstSharpenMpiDynaRegCfg->bResh);

        if (bUsrResh) {
            isp_sharpen_update_index_write(pstBeReg, pstSharpenMpiDynaRegCfg->u32UpdateIndex);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) || \
                IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_sharpen_mfgaind_waddr_write(pstBeReg, 0);

                for (j = 0; j < SHRP_GAIN_LUT_SIZE; j++) {
                    isp_sharpen_mfgaind_wdata_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16mfGainD[j]);
                }

                isp_sharpen_mfgainud_waddr_write(pstBeReg, 0);

                for (j = 0; j < SHRP_GAIN_LUT_SIZE; j++) {
                    isp_sharpen_mfgainud_wdata_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16mfGainUD[j]);
                }

                isp_sharpen_hfgaind_waddr_write(pstBeReg, 0);

                for (j = 0; j < SHRP_GAIN_LUT_SIZE; j++) {
                    isp_sharpen_hfgaind_wdata_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16hfGainD[j]);
                }

                isp_sharpen_hfgainud_waddr_write(pstBeReg, 0);

                for (j = 0; j < SHRP_GAIN_LUT_SIZE; j++) {
                    isp_sharpen_hfgainud_wdata_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16hfGainUD[j]);
                }
            } else {
                isp_sharpen_mfgaind_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16mfGainD);
                isp_sharpen_mfgainud_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16mfGainUD);
                isp_sharpen_hfgaind_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16hfGainD);
                isp_sharpen_hfgainud_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16hfGainUD);
            }

            bLutUpdate = HI_TRUE;

            isp_sharpen_bendetailctrl_write(pstBeReg, pstSharpenMpiDynaRegCfg->bEnDetailCtrl);
            isp_sharpen_osht_dtl_wgt_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8detailOshtAmt);
            isp_sharpen_usht_dtl_wgt_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8detailUshtAmt);
            isp_sharpen_detl_oshtmul_write(pstBeReg, pstSharpenMpiDynaRegCfg->s16detailOshtMul);
            isp_sharpen_detl_ushtmul_write(pstBeReg, pstSharpenMpiDynaRegCfg->s16detailUshtMul);
            isp_sharpen_oshtamt_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8oshtAmt);
            isp_sharpen_ushtamt_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8ushtAmt);
            isp_sharpen_benshtctrlbyvar_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8bEnShtCtrlByVar);
            isp_sharpen_shtbldrt_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8shtBldRt);
            isp_sharpen_oshtvarthd1_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8oshtVarThd1);
            isp_sharpen_ushtvarthd1_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8ushtVarThd1);
            isp_sharpen_benlumactrl_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8bEnLumaCtrl);
            isp_sharpen_lumawgt0_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[0]);
            isp_sharpen_lumawgt1_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[1]);
            isp_sharpen_lumawgt2_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[2]);
            isp_sharpen_lumawgt3_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[3]);
            isp_sharpen_lumawgt4_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[4]);
            isp_sharpen_lumawgt5_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[5]);
            isp_sharpen_lumawgt6_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[6]);
            isp_sharpen_lumawgt7_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[7]);
            isp_sharpen_lumawgt8_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[8]);
            isp_sharpen_lumawgt9_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[9]);
            isp_sharpen_lumawgt10_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[10]);
            isp_sharpen_lumawgt11_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[11]);
            isp_sharpen_lumawgt12_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[12]);
            isp_sharpen_lumawgt13_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[13]);
            isp_sharpen_lumawgt14_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[14]);
            isp_sharpen_lumawgt15_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[15]);
            isp_sharpen_lumawgt16_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[16]);
            isp_sharpen_lumawgt17_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[17]);
            isp_sharpen_lumawgt18_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[18]);
            isp_sharpen_lumawgt19_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[19]);
            isp_sharpen_lumawgt20_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[20]);
            isp_sharpen_lumawgt21_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[21]);
            isp_sharpen_lumawgt22_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[22]);
            isp_sharpen_lumawgt23_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[23]);
            isp_sharpen_lumawgt24_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[24]);
            isp_sharpen_lumawgt25_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[25]);
            isp_sharpen_lumawgt26_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[26]);
            isp_sharpen_lumawgt27_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[27]);
            isp_sharpen_lumawgt28_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[28]);
            isp_sharpen_lumawgt29_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[29]);
            isp_sharpen_lumawgt30_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[30]);
            isp_sharpen_lumawgt31_write(pstBeReg, pstSharpenMpiDynaRegCfg->au8LumaWgt[31]);
            isp_sharpen_oshtvarmul_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16oshtVarMul);
            isp_sharpen_ushtvarmul_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16ushtVarMul);
            isp_sharpen_benchrctrl_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8bEnChrCtrl);
            isp_sharpen_chrrgain0_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8chrRGain0);
            isp_sharpen_chrggain0_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16chrGGain0);
            isp_sharpen_chrbgain0_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8chrBGain0);
            isp_sharpen_chrrmul_write(pstBeReg, pstSharpenMpiDynaRegCfg->s16chrRMul);
            isp_sharpen_chrgmul_write(pstBeReg, pstSharpenMpiDynaRegCfg->s16chrGMul);
            isp_sharpen_chrbmul_write(pstBeReg, pstSharpenMpiDynaRegCfg->s16chrBMul);
            isp_sharpen_benskinctrl_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8bEnSkinCtrl);
            isp_sharpen_skinedgewgt0_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[0]);
            isp_sharpen_skinedgewgt1_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8skinEdgeWgt[1]);
            isp_sharpen_skinedgemul_write(pstBeReg, pstSharpenMpiDynaRegCfg->s16skinEdgeMul);
            isp_sharpen_dirdiffsft_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8dirDiffSft);
            isp_sharpen_dirrt0_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8dirRt[0]);
            isp_sharpen_dirrt1_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8dirRt[1]);
            isp_sharpen_skinmaxu_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16skinMaxU);
            isp_sharpen_skinminu_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16skinMinU);
            isp_sharpen_skinmaxv_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16skinMaxV);
            isp_sharpen_skinminv_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16skinMinV);
            isp_sharpen_omaxgain_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16oMaxGain);
            isp_sharpen_umaxgain_write(pstBeReg, pstSharpenMpiDynaRegCfg->u16uMaxGain);
            isp_sharpen_osht_dtl_thd0_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8detailOshtThr[0]);
            isp_sharpen_osht_dtl_thd1_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8detailOshtThr[1]);
            isp_sharpen_usht_dtl_thd0_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8detailUshtThr[0]);
            isp_sharpen_usht_dtl_thd1_write(pstBeReg, pstSharpenMpiDynaRegCfg->u8detailUshtThr[1]);

            pstSharpenMpiDynaRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1SharpenCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bSharpenLutUpdate = bLutUpdate;

    return HI_SUCCESS;
}


static HI_S32 ISP_EdgeMarkRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    isp_usr_ctx         *isp_ctx  = HI_NULL;
    S_ISPBE_REGS_TYPE *pstBeReg   = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1EdgeMarkCfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        isp_sharpen_benmarkedge_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stEdgeMarkRegCfg.bEnable);
        isp_sharpen_mark_thdsft_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stEdgeMarkRegCfg.u8markEdgeSft);
        isp_sharpen_mark_udata_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stEdgeMarkRegCfg.u16uMarkValue);
        isp_sharpen_mark_vdata_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stEdgeMarkRegCfg.u16vMarkValue);
        isp_sharpen_mark_thd_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stEdgeMarkRegCfg.u8MarkEdgeThd);
    }

    pstRegCfgInfo->cfg_key.bit1EdgeMarkCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    return HI_SUCCESS;
}

static HI_S32 ISP_DemRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL bIsOfflineMode;
    HI_BOOL bGfLutUpdate  = HI_FALSE;
    HI_BOOL bUsmLutUpdate = HI_FALSE;
    HI_U8   u8BlkNum      = pstRegCfgInfo->cfg_num;
    HI_U16  j;

    ISP_DEMOSAIC_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_DEMOSAIC_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    S_VIPROC_REGS_TYPE        *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE         *pstBeReg        = HI_NULL;
    isp_usr_ctx                 *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)\
                      || IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1DemCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_dmnr_vhdm_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stDemRegCfg.bVhdmEnable);
        isp_dmnr_nddm_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stDemRegCfg.bNddmEnable);

        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stDemRegCfg.stStaticRegCfg;
        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stDemRegCfg.stDynaRegCfg;

        if (pstStaticRegCfg->bResh) { /* static */
            isp_demosaic_fcs_en_write(pstBeReg, pstStaticRegCfg->bFcsEnable);
            isp_nddm_apt_interp_en_write(pstBeReg, pstStaticRegCfg->bAptFltEn);
            isp_nddm_alpha_filter_en_write(pstBeReg, pstStaticRegCfg->bAlphaFilter);
            isp_demosaic_ahd_en_write(pstBeReg, pstStaticRegCfg->bAHDEnable);
            isp_demosaic_de_fake_en_write(pstBeReg, pstStaticRegCfg->bDeFakeEnable);
            isp_demosaic_bld_limit1_write(pstBeReg, pstStaticRegCfg->u8hvBlendLimit1);
            isp_demosaic_bld_limit2_write(pstBeReg, pstStaticRegCfg->u8hvBlendLimit2);
            isp_demosaic_bld_ratio1_write(pstBeReg, pstStaticRegCfg->u16hvBlendRatio1);
            isp_demosaic_bld_ratio2_write(pstBeReg, pstStaticRegCfg->u16hvBlendRatio2);
            isp_demosaic_ahd_par1_write(pstBeReg, pstStaticRegCfg->u16AhdPart1);
            isp_demosaic_ahd_par2_write(pstBeReg, pstStaticRegCfg->u16AhdPart2);
            isp_demosaic_cx_var_max_rate_write(pstBeReg, pstStaticRegCfg->u8CxVarMaxRate);
            isp_demosaic_cx_var_min_rate_write(pstBeReg, pstStaticRegCfg->u8CxVarMinRate);
            isp_demosaic_g_clip_sft_bit_write(pstBeReg, pstStaticRegCfg->u8GClipBitSft);
            isp_demosaic_hv_ratio_write(pstBeReg, pstStaticRegCfg->u8hvColorRatio);
            isp_demosaic_hv_sel_write(pstBeReg, pstStaticRegCfg->u8hvSelection);
            isp_demosaic_cbcr_avg_thld_write(pstBeReg, pstStaticRegCfg->u16CbCrAvgThr);
            isp_nddm_bldr_gr_gb_write(pstBeReg, pstStaticRegCfg->u8BldrGrGb);
            isp_nddm_multi_mf_r_write(pstBeReg, pstStaticRegCfg->u8MultiMFRed);
            isp_nddm_multi_mf_b_write(pstBeReg, pstStaticRegCfg->u8MultiMFBlue);
            isp_nddm_dith_mask_write(pstBeReg, pstStaticRegCfg->u8DitherMask);
            isp_nddm_dith_ratio_write(pstBeReg, pstStaticRegCfg->u8DitherRatio);
            isp_nddm_sht_ctrl_gain_write(pstBeReg, pstStaticRegCfg->u8ShtCtrlGain);
            isp_nddm_clip_delta_gain_write(pstBeReg, pstStaticRegCfg->u8ClipDeltaGain);
            isp_nddm_clip_adjust_max_write(pstBeReg, pstStaticRegCfg->u8ClipAdjustMax);
            isp_nddm_scale_write(pstBeReg, pstStaticRegCfg->u8FcrScale);
            isp_nddm_bldr_gf_str_write(pstBeReg, pstStaticRegCfg->u8BldrGFStr);
            isp_nddm_gf_th_low_write(pstBeReg, pstStaticRegCfg->u16GFThLow);
            isp_nddm_gf_th_high_write(pstBeReg, pstStaticRegCfg->u16GFThHig);
            isp_nddm_clip_usm_write(pstBeReg, pstStaticRegCfg->u16ClipUSM);
            isp_nddm_satu_th_fix_write(pstBeReg, pstStaticRegCfg->u16SatuThFix);
            isp_nddm_satu_th_low_write(pstBeReg, pstStaticRegCfg->u16SatuThLow);
            isp_nddm_satu_th_high_write(pstBeReg, pstStaticRegCfg->u16SatuThHig);
            isp_nddm_gray_th_low_write(pstBeReg, pstStaticRegCfg->u16GrayThLow);
            isp_nddm_gray_th_high_write(pstBeReg, pstStaticRegCfg->u16GrayThHig);
            isp_nddm_gray_th_fix_write(pstBeReg, pstStaticRegCfg->u16GrayThFixLow);
            isp_nddm_gray_th_fix2_write(pstBeReg, pstStaticRegCfg->u16GrayThFixHig);
            isp_nddm_fcr_limit_low_write(pstBeReg, pstStaticRegCfg->u16FcrLimitLow);
            isp_nddm_fcr_limit_high_write(pstBeReg, pstStaticRegCfg->u16FcrLimitHigh);
            isp_nddm_sht_ctrl_th_write(pstBeReg, pstStaticRegCfg->u16ShtCtrlTh);
            isp_nddm_clip_r_ud_sht_write(pstBeReg, pstStaticRegCfg->u16ClipRUdSht);
            isp_nddm_clip_r_ov_sht_write(pstBeReg, pstStaticRegCfg->u16ClipROvSht);
            isp_nddm_clip_b_ud_sht_write(pstBeReg, pstStaticRegCfg->u16ClipBUdSht);
            isp_nddm_clip_b_ov_sht_write(pstBeReg, pstStaticRegCfg->u16ClipBOvSht);

            pstRegCfgInfo->alg_reg_cfg[i].stDemRegCfg.stStaticRegCfg.bResh = HI_FALSE;
        }

        if (pstDynaRegCfg->bResh) { /* dynamic */
            isp_demosaic_fake_cr_var_thr_high_write(pstBeReg, pstDynaRegCfg->u16FakeCrVarThrHigh);
            isp_demosaic_fake_cr_var_thr_low_write(pstBeReg, pstDynaRegCfg->u16FakeCrVarThrLow);
            isp_demosaic_lpf_f0_write(pstBeReg, pstDynaRegCfg->u8Lpff0);
            isp_demosaic_lpf_f1_write(pstBeReg, pstDynaRegCfg->u8Lpff1);
            isp_demosaic_lpf_f2_write(pstBeReg, pstDynaRegCfg->u8Lpff2);
            isp_demosaic_lpf_f3_write(pstBeReg, pstDynaRegCfg->u8Lpff3);
            isp_demosaic_cc_hf_max_ratio_write(pstBeReg, pstDynaRegCfg->u8CcHFMaxRatio);
            isp_demosaic_cc_hf_min_ratio_write(pstBeReg, pstDynaRegCfg->u8CcHFMinRatio);
            isp_demosaic_interp_ratio1_write(pstBeReg, pstDynaRegCfg->u32hfIntpRatio);
            isp_demosaic_interp_ratio2_write(pstBeReg, pstDynaRegCfg->u32hfIntpRatio1);
            isp_demosaic_hf_intp_bld_low_write(pstBeReg, pstDynaRegCfg->u16hfIntpBldLow);
            isp_demosaic_hf_intp_bld_high_write(pstBeReg, pstDynaRegCfg->u16hfIntpBldHig);
            isp_demosaic_hf_intp_th_low_write(pstBeReg, pstDynaRegCfg->u16hfIntpThLow);
            isp_demosaic_hf_intp_th_high_write(pstBeReg, pstDynaRegCfg->u16hfIntpThHig);
            isp_demosaic_hf_intp_th_low1_write(pstBeReg, pstDynaRegCfg->u16hfIntpThLow1);
            isp_demosaic_hf_intp_th_high1_write(pstBeReg, pstDynaRegCfg->u16hfIntpThHig1);
            isp_nddm_bldr_cbcr_write(pstBeReg, pstDynaRegCfg->u8BldrCbCr);
            isp_nddm_awb_gf_gn_low_write(pstBeReg, pstDynaRegCfg->u8AwbGFGainLow);
            isp_nddm_awb_gf_gn_high_write(pstBeReg, pstDynaRegCfg->u8AwbGFGainHig);
            isp_nddm_awb_gf_gn_max_write(pstBeReg, pstDynaRegCfg->u8AwbGFGainMax);
            isp_nddm_dith_max_write(pstBeReg, pstDynaRegCfg->u8DitherMax);
            isp_nddm_multi_mf_write(pstBeReg, pstDynaRegCfg->u8MultiMF);
            isp_nddm_fcr_det_low_write(pstBeReg, pstDynaRegCfg->u16FcrDetLow);
            isp_nddm_fcr_gf_gain_write(pstBeReg, pstDynaRegCfg->u8FcrGFGain);
            isp_nddm_filter_str_intp_write(pstBeReg, pstDynaRegCfg->u8FilterStrIntp);
            isp_nddm_clip_delta_intp_low_write(pstBeReg, pstDynaRegCfg->u16ClipDeltaIntpLow);
            isp_nddm_clip_delta_intp_high_write(pstBeReg, pstDynaRegCfg->u16ClipDeltaIntpHigh);
            isp_nddm_filter_str_filt_write(pstBeReg, pstDynaRegCfg->u8FilterStrFilt);
            isp_nddm_clip_delta_filt_low_write(pstBeReg, pstDynaRegCfg->u16ClipDeltaFiltLow);
            isp_nddm_clip_delta_filt_high_write(pstBeReg, pstDynaRegCfg->u16ClipDeltaFiltHigh);
            isp_nddm_bldr_gray_write(pstBeReg, pstDynaRegCfg->u8BldrGray);
            isp_nddm_satu_r_th_fix_write(pstBeReg, pstDynaRegCfg->u16SatuRThFix);
            isp_nddm_satu_r_th_low_write(pstBeReg, pstDynaRegCfg->u16SatuRThLow);
            isp_nddm_satu_r_th_high_write(pstBeReg, pstDynaRegCfg->u16SatuRThHig);
            isp_nddm_satu_b_th_fix_write(pstBeReg, pstDynaRegCfg->u16SatuBThFix);
            isp_nddm_satu_b_th_low_write(pstBeReg, pstDynaRegCfg->u16SatuBThLow);
            isp_nddm_satu_b_th_high_write(pstBeReg, pstDynaRegCfg->u16SatuBThHig);
            isp_nddm_satu_fix_ehcy_write(pstBeReg, pstDynaRegCfg->s16SatuFixEhcY);

            isp_cc_prot_en_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.bCcmProtEn);
            isp_cc_data_cc_thd0_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u32DataCCThd0);
            isp_cc_data_cc_thd1_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u32DataCCThd1);
            isp_cc_cc_prot_ratio_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u16CCProtRatio);
            isp_cc_data_rr_thd0_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u32DataRRThd0);
            isp_cc_data_rr_thd1_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u32DataRRThd1);
            isp_cc_data_gg_thd0_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u32DataGGThd0);
            isp_cc_data_gg_thd1_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u32DataGGThd1);
            isp_cc_data_bb_thd0_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u32DataBBThd0);
            isp_cc_data_bb_thd1_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u32DataBBThd1);
            isp_cc_max_rgb_thd_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u32MaxRGBThd);
            isp_cc_rgb_prot_ratio_write(pstBeReg, pstDynaRegCfg->stProDarkRegion.u16RGBProtRatio);

            if (pstDynaRegCfg->bUpdateUsm) {
                if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
                    || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                    isp_nddm_usm_lut_waddr_write(pstBeReg, 0);

                    for (j = 0; j < HI_ISP_DEMOSAIC_LUT_LENGTH; j++) {
                        isp_nddm_usm_lut_wdata_write(pstBeReg, pstDynaRegCfg->au8EhcGainLut[j]);
                    }
                } else {
                    isp_nddm_usmlut_write(pstBeReg, pstDynaRegCfg->au8EhcGainLut);
                }

                bUsmLutUpdate = pstDynaRegCfg->bUpdateUsm;
                pstDynaRegCfg->bUpdateUsm = bIsOfflineMode;
            }

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
                || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_nddm_gf_lut_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_DEMOSAIC_LUT_LENGTH; j++) {
                    isp_nddm_gf_lut_wdata_write(pstBeReg, pstDynaRegCfg->au16GFBlurLut[j]);
                }
            } else {
                isp_nddm_gflut_write(pstBeReg, pstDynaRegCfg->au16GFBlurLut);
            }

            bGfLutUpdate         = pstDynaRegCfg->bUpdateGF;
            pstDynaRegCfg->bResh = bIsOfflineMode;

        }

        pstRegCfgInfo->cfg_key.bit1DemCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bNddmUsmLutUpdate = bUsmLutUpdate;
    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bNddmGfLutUpdate  = bGfLutUpdate;

    return HI_SUCCESS;
}

static HI_S32 ISP_FpnRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    ISP_FPN_DYNA_CFG_S *pstDynaRegCfg;
    HI_U8 u8BlkNum = pstRegCfgInfo->cfg_num;

    if (pstRegCfgInfo->cfg_key.bit1FpnCfg) {
        pstDynaRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stFpnRegCfg.stDynaRegCfg;

        isp_fpn_overflowthr_write(ViPipe, i, pstDynaRegCfg->u32IspFpnOverflowThr);
        isp_fpn_strength0_write(ViPipe, i, pstDynaRegCfg->u32IspFpnStrength[0]);
        isp_fpn_strength1_write(ViPipe, i, pstDynaRegCfg->u32IspFpnStrength[1]);
        isp_fpn_strength2_write(ViPipe, i, pstDynaRegCfg->u32IspFpnStrength[2]);
        isp_fpn_strength3_write(ViPipe, i, pstDynaRegCfg->u32IspFpnStrength[3]);

        pstRegCfgInfo->cfg_key.bit1FpnCfg = (u8BlkNum <= (i + 1)) ? 0 : 1;
    }

    return HI_SUCCESS;
}

static HI_S32 isp_ldci_read_stt_addr_write(S_VIPROC_REGS_TYPE *pstViProcReg, HI_U64 u64PhyAddr)
{
    if (u64PhyAddr == 0) {
        return HI_FAILURE;
    }

    viproc_para_dci_addr_low_write(pstViProcReg, GetLowAddr(u64PhyAddr));
    viproc_para_dci_addr_high_write(pstViProcReg, GetHighAddr(u64PhyAddr));

    return HI_SUCCESS;
}

static HI_S32 ISP_LdciRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL bIsOfflineMode, bRdstatEn;
    HI_BOOL bLdciDrcLutUpdate  = HI_FALSE;
    HI_BOOL bLdciCalcLutUpdate = HI_FALSE;
    HI_U8   u8ReadBufIdx;
    HI_U8   u8BlkNum           = pstRegCfgInfo->cfg_num;
    HI_U16  j;
    HI_S32  s32Ret = HI_SUCCESS;
    ISP_LDCI_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_LDCI_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    S_VIPROC_REGS_TYPE    *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE     *pstBeReg        = HI_NULL;
    isp_usr_ctx             *isp_ctx       = HI_NULL;
    isp_ldci_stat       *pstReadSttBuf   = HI_NULL;
    isp_ldci_read_stt_buf *pstLdciReadSttBuf = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) ||
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1LdciCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stLdciRegCfg.stStaticRegCfg;
        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stLdciRegCfg.stDynaRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            /*static*/
            isp_ldci_stat_evratio_write(pstBeReg, 0x1000);
            isp_ldci_luma_sel_write(pstBeReg, pstStaticRegCfg->u32CalcLumaSel);
            isp_ldci_bende_write(pstBeReg, pstStaticRegCfg->bDeEnable);
            isp_ldci_deref_write(pstBeReg, pstStaticRegCfg->bCalcDeRef);
            isp_ldci_deh_lpfsft_write(pstBeReg, pstStaticRegCfg->u32DehLpfSft);
            isp_ldci_coefh0_write(pstBeReg, pstStaticRegCfg->as16DehLpfCoefH[0]);
            isp_ldci_coefh1_write(pstBeReg, pstStaticRegCfg->as16DehLpfCoefH[1]);
            isp_ldci_coefh2_write(pstBeReg, pstStaticRegCfg->as16DehLpfCoefH[2]);
            isp_ldci_coefv0_write(pstBeReg, pstStaticRegCfg->as16DehLpfCoefV[0]);
            isp_ldci_coefv1_write(pstBeReg, pstStaticRegCfg->as16DehLpfCoefV[1]);
            isp_ldci_calc_drcen_write(pstBeReg, pstStaticRegCfg->bCalcDrcEnable);
            isp_ldci_pflmt_en_write(pstBeReg, pstStaticRegCfg->bCalcPfLmtEnable);
            isp_ldci_pfori0_write(pstBeReg, pstStaticRegCfg->u32CalcPfOri[0]);
            isp_ldci_pfori1_write(pstBeReg, pstStaticRegCfg->u32CalcPfOri[1]);
            isp_ldci_pfsft0_write(pstBeReg, pstStaticRegCfg->as16CalcPfSft[0]);
            isp_ldci_pfsft1_write(pstBeReg, pstStaticRegCfg->as16CalcPfSft[1]);
            isp_ldci_pfsft2_write(pstBeReg, pstStaticRegCfg->as16CalcPfSft[2]);
            isp_ldci_pfsft3_write(pstBeReg, pstStaticRegCfg->as16CalcPfSft[3]);
            isp_ldci_pfthd0_write(pstBeReg, pstStaticRegCfg->u32CalcPfThd[0]);
            isp_ldci_pfthd1_write(pstBeReg, pstStaticRegCfg->u32CalcPfThd[1]);
            isp_ldci_pfrly0_write(pstBeReg, pstStaticRegCfg->u32CalcPfRly[0]);
            isp_ldci_pfrly1_write(pstBeReg, pstStaticRegCfg->u32CalcPfRly[1]);
            isp_ldci_pfmul_write(pstBeReg, pstStaticRegCfg->u32CalcPfmul);
            isp_ldci_lpfsft_write(pstBeReg, pstStaticRegCfg->u32LpfSft);
            isp_ldci_stat_drcen_write(pstBeReg, pstStaticRegCfg->bStatDrcEnable);
            isp_ldci_chrposdamp_write(pstBeReg, pstStaticRegCfg->u32ChrPosDamp);
            isp_ldci_chrnegdamp_write(pstBeReg, pstStaticRegCfg->u32ChrNegDamp);
            isp_ldci_glb_hewgt_write(pstBeReg, pstStaticRegCfg->u32GlobalHeWgt);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) ||
                IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_ldci_poply_waddr_write(pstBeReg, 0);
                isp_ldci_plyq01_waddr_write(pstBeReg, 0);
                isp_ldci_plyq23_waddr_write(pstBeReg, 0);
                isp_ldci_drc_waddr_write(pstBeReg, 0);

                for (j = 0; j < 65; j++) {
                    isp_ldci_poply_wdata_write(pstBeReg, pstStaticRegCfg->as16PolyP1[j], pstStaticRegCfg->as16StatPolyP2[j], pstStaticRegCfg->as16StatPolyP3[j]);
                    isp_ldci_plyq01_wdata_write(pstBeReg, pstStaticRegCfg->as16PolyQ0[j], pstStaticRegCfg->as16PolyQ1[j]);
                    isp_ldci_plyq23_wdata_write(pstBeReg, pstStaticRegCfg->as16PolyQ2[j], pstStaticRegCfg->as16PolyQ3[j]);
                    isp_ldci_drc_wdata_write(pstBeReg, pstStaticRegCfg->as16CalcDynRngCmpLut[j], pstStaticRegCfg->as16StatDynRngCmpLut[j]);
                }
            } else {
                isp_ldci_poply1_wlut_write(pstBeReg, pstStaticRegCfg->as16PolyP1);
                isp_ldci_poply2_wlut_write(pstBeReg, pstStaticRegCfg->as16StatPolyP2);
                isp_ldci_poply3_wlut_write(pstBeReg, pstStaticRegCfg->as16StatPolyP3);
                isp_ldci_plyq0_wlut_write(pstBeReg, pstStaticRegCfg->as16PolyQ0);
                isp_ldci_plyq1_wlut_write(pstBeReg, pstStaticRegCfg->as16PolyQ1);
                isp_ldci_plyq2_wlut_write(pstBeReg, pstStaticRegCfg->as16PolyQ2);
                isp_ldci_plyq3_wlut_write(pstBeReg, pstStaticRegCfg->as16PolyQ3);
                isp_ldci_statdrc_wlut_write(pstBeReg, pstStaticRegCfg->as16StatDynRngCmpLut);
                isp_ldci_calcdrc_wlut_write(pstBeReg, pstStaticRegCfg->as16CalcDynRngCmpLut);

                isp_ldci_usmpos_wlut_write(pstBeReg, pstDynaRegCfg->u32UsmPosLut);
                isp_ldci_usmneg_wlut_write(pstBeReg, pstDynaRegCfg->u32UsmNegLut);
                isp_ldci_delut_wlut_write(pstBeReg, pstDynaRegCfg->u32DeLut);
                isp_ldci_cgain_wlut_write(pstBeReg, pstDynaRegCfg->u32ColorGainLut);
            }

            bLdciDrcLutUpdate            = HI_TRUE;
            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        /*dynamic*/
        if (isp_ctx->ldci_tpr_flt_en == HI_TRUE) {
            LDCI_BUF_GET_CTX(ViPipe, pstLdciReadSttBuf);
            u8ReadBufIdx = pstLdciReadSttBuf->buf_idx;

            if (i == 0) {
                pstReadSttBuf = (isp_ldci_stat *)ISP_GetLdciReadSttVirAddr(ViPipe, u8ReadBufIdx);
                ISP_CHECK_POINTER(pstReadSttBuf);
                memcpy(pstReadSttBuf, &pstDynaRegCfg->stTprStat, sizeof(isp_ldci_stat));
            }

            if ((i + 1) == u8BlkNum) {
                pstLdciReadSttBuf->buf_idx = (u8ReadBufIdx + 1) % DIV_0_TO_1(pstLdciReadSttBuf->buf_num);
            }

            /* Set ReadStt Addr */
            s32Ret = isp_ldci_read_stt_addr_write(pstViProcReg, pstLdciReadSttBuf->read_buf[u8ReadBufIdx].phy_addr);
            bRdstatEn = (s32Ret == HI_SUCCESS) ? (pstDynaRegCfg->bRdstatEn) : (HI_FALSE);
            isp_ldci_rdstat_en_write(pstBeReg, bRdstatEn);
        } else {
            isp_ldci_rdstat_en_write(pstBeReg, pstDynaRegCfg->bRdstatEn);
        }

        isp_ldci_en_write(pstViProcReg, pstDynaRegCfg->bEnable);
        isp_ldci_wrstat_en_write(pstBeReg, pstDynaRegCfg->bWrstatEn);
        isp_ldci_calc_en_write(pstBeReg, pstDynaRegCfg->bCalcEnable);
        isp_ldci_blc_ctrl_write(pstBeReg, pstDynaRegCfg->u32CalcBlcCtrl);
        isp_ldci_lpfcoef0_write(pstBeReg, pstDynaRegCfg->u32LpfCoef[0]);
        isp_ldci_lpfcoef1_write(pstBeReg, pstDynaRegCfg->u32LpfCoef[1]);
        isp_ldci_lpfcoef2_write(pstBeReg, pstDynaRegCfg->u32LpfCoef[2]);
        isp_ldci_lpfcoef3_write(pstBeReg, pstDynaRegCfg->u32LpfCoef[3]);
        isp_ldci_lpfcoef4_write(pstBeReg, pstDynaRegCfg->u32LpfCoef[4]);
        isp_ldci_lpfcoef5_write(pstBeReg, pstDynaRegCfg->u32LpfCoef[5]);
        isp_ldci_lpfcoef6_write(pstBeReg, pstDynaRegCfg->u32LpfCoef[6]);
        isp_ldci_lpfcoef7_write(pstBeReg, pstDynaRegCfg->u32LpfCoef[7]);
        isp_ldci_lpfcoef8_write(pstBeReg, pstDynaRegCfg->u32LpfCoef[8]);
        isp_ldci_calc_map_offsetx_write(pstBeReg, pstDynaRegCfg->u32CalcMapOffsetX);
        isp_ldci_smlmapstride_write(pstBeReg, pstDynaRegCfg->u32CalcSmlMapStride);
        isp_ldci_smlmapheight_write(pstBeReg, pstDynaRegCfg->u32CalcSmlMapHeight);
        isp_ldci_total_zone_write(pstBeReg, pstDynaRegCfg->u32CalcTotalZone);
        isp_ldci_scalex_write(pstBeReg, pstDynaRegCfg->u32CalcScaleX);
        isp_ldci_scaley_write(pstBeReg, pstDynaRegCfg->u32CalcScaleY);
        isp_ldci_stat_smlmapwidth_write(pstBeReg, pstDynaRegCfg->u32StatSmlMapWidth);
        isp_ldci_stat_smlmapheight_write(pstBeReg, pstDynaRegCfg->u32StatSmlMapHeight);
        isp_ldci_stat_total_zone_write(pstBeReg, pstDynaRegCfg->u32StatTotalZone);
        isp_ldci_blk_smlmapwidth0_write(pstBeReg, pstDynaRegCfg->u32BlkSmlMapWidth[0]);
        isp_ldci_blk_smlmapwidth1_write(pstBeReg, pstDynaRegCfg->u32BlkSmlMapWidth[1]);
        isp_ldci_blk_smlmapwidth2_write(pstBeReg, pstDynaRegCfg->u32BlkSmlMapWidth[2]);
        isp_ldci_hstart_write(pstBeReg, pstDynaRegCfg->u32StatHStart);
        isp_ldci_hend_write(pstBeReg, pstDynaRegCfg->u32StatHEnd);
        isp_ldci_vstart_write(pstBeReg, pstDynaRegCfg->u32StatVStart);
        isp_ldci_vend_write(pstBeReg, pstDynaRegCfg->u32StatVEnd);

        if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
            || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
            isp_ldci_de_usm_waddr_write(pstBeReg, 0);

            for (j = 0; j < LDCI_DE_USM_LUT_SIZE; j++) {
                isp_ldci_de_usm_wdata_write(pstBeReg, pstDynaRegCfg->u32UsmPosLut[j], pstDynaRegCfg->u32UsmNegLut[j], pstDynaRegCfg->u32DeLut[j]);
            }

            isp_ldci_cgain_waddr_write(pstBeReg, 0);

            for (j = 0; j < LDCI_COLOR_GAIN_LUT_SIZE; j++) {
                isp_ldci_cgain_wdata_write(pstBeReg, pstDynaRegCfg->u32ColorGainLut[j]);
            }

            isp_ldci_he_waddr_write(pstBeReg, 0);

            for (j = 0; j < LDCI_HE_LUT_SIZE; j++) {
                isp_ldci_he_wdata_write(pstBeReg, pstDynaRegCfg->u32HePosLut[j], pstDynaRegCfg->u32HeNegLut[j]);
            }
        } else {
            isp_ldci_hepos_wlut_write(pstBeReg, pstDynaRegCfg->u32HePosLut);
            isp_ldci_heneg_wlut_write(pstBeReg, pstDynaRegCfg->u32HeNegLut);
        }

        bLdciCalcLutUpdate = pstDynaRegCfg->bCalcLutUpdate;
        pstRegCfgInfo->cfg_key.bit1LdciCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bLdciCalcLutUpdate = bLdciCalcLutUpdate;
    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bLdciDrcLutUpdate  = bLdciDrcLutUpdate;

    return HI_SUCCESS;
}

static HI_S32 ISP_LogLUTRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_U32 j;
    HI_U8 u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_BOOL  bIsOfflineMode;
    ISP_LOGLUT_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    ISP_LOGLUT_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    S_VIPROC_REGS_TYPE      *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE       *pstBeReg        = HI_NULL;
    isp_usr_ctx               *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)\
                      || IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1LogLUTCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stLogLUTRegCfg.stStaticRegCfg;
        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stLogLUTRegCfg.stDynaRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            /* static */
            isp_loglut_bitw_out_write(pstBeReg, pstStaticRegCfg->u8BitDepthOut);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
                || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_loglut_lut_waddr_write(pstBeReg, 0);

                for (j = 0; j < LOG_LUT_SIZE; j++) {
                    isp_loglut_lut_wdata_write(pstBeReg, pstStaticRegCfg->au32LogLUT[j]);
                }
            } else {
                isp_loglut_lut_write(pstBeReg, pstStaticRegCfg->au32LogLUT);
            }

            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        /* dynamic */

        isp_loglut_en_write(pstViProcReg, pstDynaRegCfg->bEnable);

        pstRegCfgInfo->cfg_key.bit1LogLUTCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}


static HI_S32 ISP_LcacRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_U8 u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_BOOL  bIsOfflineMode;
    ISP_LOCAL_CAC_USR_CFG_S    *pstUsrRegCfg    = HI_NULL;
    ISP_LOCAL_CAC_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    ISP_LOCAL_CAC_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    S_ISPBE_REGS_TYPE          *pstBeReg        = HI_NULL;
    isp_usr_ctx                  *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)\
                      || IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1LocalCacCfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        isp_demosaic_local_cac_en_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stLCacRegCfg.bLocalCacEn);

        /* static */
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stLCacRegCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            isp_nddm_cac_blend_en_write(pstBeReg, pstStaticRegCfg->bNddmCacBlendEn);
            isp_nddm_cac_blend_rate_write(pstBeReg, pstStaticRegCfg->u16NddmCacBlendRate);
            isp_demosaic_r_counter_thr_write(pstBeReg, pstStaticRegCfg->u8RCounterThr);
            isp_demosaic_g_counter_thr_write(pstBeReg, pstStaticRegCfg->u8GCounterThr);
            isp_demosaic_b_counter_thr_write(pstBeReg, pstStaticRegCfg->u8BCounterThr);
            isp_demosaic_satu_thr_write(pstBeReg, pstStaticRegCfg->u16SatuThr);
            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        /* Usr */
        pstUsrRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stLCacRegCfg.stUsrRegCfg;

        if (pstUsrRegCfg->bResh) {
            isp_demosaic_purple_var_thr_write(pstBeReg, pstUsrRegCfg->u16VarThr);
            isp_demosaic_cb_thr_write(pstBeReg, pstUsrRegCfg->u16CbThr);
            isp_demosaic_cac_luma_high_cnt_thr_write(pstBeReg, pstUsrRegCfg->u8LumaHighCntThr);
            isp_demosaic_cac_cb_cnt_low_thr_write(pstBeReg, pstUsrRegCfg->u8CbCntLowThr);
            isp_demosaic_cac_cb_cnt_high_thr_write(pstBeReg, pstUsrRegCfg->u8CbCntHighThr);
            isp_demosaci_cac_bld_avg_cur_write(pstBeReg, pstUsrRegCfg->u8BldAvgCur);
            isp_demosaic_cbcr_ratio_high_limit_write(pstBeReg, pstUsrRegCfg->s16CbCrRatioLmtHigh);
            isp_demosaic_cbcr_ratio_low_limit_write(pstBeReg, pstUsrRegCfg->s16CbCrRatioLmtLow);

            pstUsrRegCfg->bResh = bIsOfflineMode;
        }

        /* dynamic */
        pstDynaRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stLCacRegCfg.stDynaRegCfg;

        if (pstDynaRegCfg->bResh) {
            isp_demosaic_r_luma_thr_write(pstBeReg, pstDynaRegCfg->u16RLumaThr);
            isp_demosaic_g_luma_thr_write(pstBeReg, pstDynaRegCfg->u16GLumaThr);
            isp_demosaic_b_luma_thr_write(pstBeReg, pstDynaRegCfg->u16BLumaThr);
            isp_demosaic_luma_thr_write(pstBeReg, pstDynaRegCfg->u16LumaThr);
            isp_demosaic_depurplectr_cr_write(pstBeReg, pstDynaRegCfg->u8DePurpleCtrCr);
            isp_demosaic_depurplectr_cb_write(pstBeReg, pstDynaRegCfg->u8DePurpleCtrCb);
            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1LocalCacCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_FcrRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_U8 u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_BOOL  bIsOfflineMode;
    ISP_ANTIFALSECOLOR_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    ISP_ANTIFALSECOLOR_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    S_ISPBE_REGS_TYPE               *pstBeReg        = HI_NULL;
    isp_usr_ctx                       *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1FcrCfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);

        isp_demosaic_fcr_en_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stAntiFalseColorRegCfg.bFcrEnable);

        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stAntiFalseColorRegCfg.stStaticRegCfg;
        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stAntiFalseColorRegCfg.stDynaRegCfg;

        /* static */
        if (pstStaticRegCfg->bResh) {
            isp_demosaic_fcr_limit1_write(pstBeReg, pstStaticRegCfg->u16FcrLimit1);
            isp_demosaic_fcr_limit2_write(pstBeReg, pstStaticRegCfg->u16FcrLimit2);
            isp_demosaic_fcr_thr_write(pstBeReg, pstStaticRegCfg->u16FcrThr);
            pstStaticRegCfg->bResh = HI_FALSE;
        }

        /* dynamic */
        if (pstDynaRegCfg->bResh) {
            isp_demosaic_fcr_gain_write(pstBeReg, pstDynaRegCfg->u8FcrGain);
            isp_demosaic_fcr_ratio_write(pstBeReg, pstDynaRegCfg->u8FcrRatio);
            isp_demosaic_fcr_gray_ratio_write(pstBeReg, pstDynaRegCfg->u8FcrGrayRatio);
            isp_demosaic_fcr_cmax_sel_write(pstBeReg, pstDynaRegCfg->u8FcrCmaxSel);
            isp_demosaic_fcr_detg_sel_write(pstBeReg, pstDynaRegCfg->u8FcrDetgSel);
            isp_demosaic_fcr_thresh1_write(pstBeReg, pstDynaRegCfg->u16FcrHfThreshLow);
            isp_demosaic_fcr_thresh2_write(pstBeReg, pstDynaRegCfg->u16FcrHfThreshHig);
            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1FcrCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_GcacRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_BOOL  bUsrResh  = HI_FALSE;
    HI_BOOL  bIdxResh  = HI_FALSE;
    HI_U8 u8BlkNum = pstRegCfgInfo->cfg_num;
    S_ISPBE_REGS_TYPE           *pstBeReg        = HI_NULL;
    ISP_GLOBAL_CAC_USR_CFG_S    *pstUsrRegCfg    = HI_NULL;
    ISP_GLOBAL_CAC_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    isp_usr_ctx                   *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)\
                      || IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1GlobalCacCfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);

        isp_gcac_en_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stGCacRegCfg.bGlobalCacEn);

        /* static */
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stGCacRegCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            isp_gcac_ver_filt_en_write(pstBeReg, pstStaticRegCfg->bGCacVerFilEn);
            isp_demosaic_gcac_blend_select_write(pstBeReg, pstStaticRegCfg->u8GCacBlendSel);
            isp_gcac_chrv_mode_write(pstBeReg, pstStaticRegCfg->u8GcacChrVerMode);
            isp_gcac_clip_mode_v_write(pstBeReg, pstStaticRegCfg->u8GcacClipModeVer);
            isp_gcac_clip_mode_h_write(pstBeReg, pstStaticRegCfg->u8GcacClipModeHor);
            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        /* usr */
        pstUsrRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stGCacRegCfg.stUsrRegCfg;
        bIdxResh = (isp_gcac_update_index_read(pstBeReg) != pstUsrRegCfg->u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bResh & bIdxResh) : (pstUsrRegCfg->bResh);

        if (bUsrResh) {
            isp_gcac_update_index_write(pstBeReg, pstUsrRegCfg->u32UpdateIndex);
            isp_gcac_red_a_write(pstBeReg, pstUsrRegCfg->s16ParamRedA);
            isp_gcac_red_b_write(pstBeReg, pstUsrRegCfg->s16ParamRedB);
            isp_gcac_red_c_write(pstBeReg, pstUsrRegCfg->s16ParamRedC);
            isp_gcac_blue_a_write(pstBeReg, pstUsrRegCfg->s16ParamBlueA);
            isp_gcac_blue_b_write(pstBeReg, pstUsrRegCfg->s16ParamBlueB);
            isp_gcac_blue_c_write(pstBeReg, pstUsrRegCfg->s16ParamBlueC);
            isp_gcac_ns_y_write(pstBeReg, pstUsrRegCfg->u8VerNormShift);
            isp_gcac_nf_y_write(pstBeReg, pstUsrRegCfg->u8VerNormFactor);
            isp_gcac_ns_x_write(pstBeReg, pstUsrRegCfg->u8HorNormShift);
            isp_gcac_nf_x_write(pstBeReg, pstUsrRegCfg->u8HorNormFactor);
            isp_gcac_cnt_start_h_write(pstBeReg, pstUsrRegCfg->u16CenterCoorHor);
            isp_gcac_cnt_start_v_write(pstBeReg, pstUsrRegCfg->u16CenterCoorVer);
            isp_demosaic_varthr_for_blend_write(pstBeReg, pstUsrRegCfg->u16CorVarThr);
            isp_gcac_cor_start_h_write(pstBeReg, pstUsrRegCfg->u16StartCoorHor);
            isp_gcac_cor_start_v_write(pstBeReg, pstUsrRegCfg->u16StartCoorVer);
            pstUsrRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1GlobalCacCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}


static HI_S32 ISP_DpcRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bUsrResh   = HI_FALSE;
    HI_BOOL  bIdxResh   = HI_FALSE;
    HI_BOOL  bLutUpdate = HI_FALSE;
    HI_BOOL  bIsOfflineMode;
    HI_U8 u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_U16 j;
    ISP_DPC_STATIC_CFG_S    *pstStaticRegCfg = HI_NULL;
    ISP_DPC_DYNA_CFG_S      *pstDynaRegCfg   = HI_NULL;
    ISP_DPC_USR_CFG_S       *pstUsrRegCfg    = HI_NULL;
    S_ISPBE_REGS_TYPE       *pstBeReg        = HI_NULL;
    S_VIPROC_REGS_TYPE      *pstViProcReg    = HI_NULL;
    isp_usr_ctx               *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1DpCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_dpc_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stDpRegCfg.abDpcEn[0]);
        isp_dpc_dpc_en1_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stDpRegCfg.abDpcEn[1]);
        isp_dpc_dpc_en2_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stDpRegCfg.abDpcEn[2]);
        isp_dpc_dpc_en3_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stDpRegCfg.abDpcEn[3]);

        /* static */
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stDpRegCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            isp_dpc_output_mode_write(pstBeReg, pstStaticRegCfg->u8DpccOutputMode);
            isp_dpc_bpt_ctrl_write(pstBeReg, pstStaticRegCfg->u32DpccBptCtrl);

            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        /* usr */
        pstUsrRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stDpRegCfg.stUsrRegCfg;

        if (pstUsrRegCfg->stUsrDynaCorRegCfg.bResh) {
            isp_dpc_ex_soft_thr_max_write(pstBeReg, pstUsrRegCfg->stUsrDynaCorRegCfg.s8DpccSupTwinkleThrMax);
            isp_dpc_ex_soft_thr_min_write(pstBeReg, pstUsrRegCfg->stUsrDynaCorRegCfg.s8DpccSupTwinkleThrMin);
            isp_dpc_ex_hard_thr_en_write(pstBeReg, pstUsrRegCfg->stUsrDynaCorRegCfg.bDpccHardThrEn);
            isp_dpc_ex_rake_ratio_write(pstBeReg, pstUsrRegCfg->stUsrDynaCorRegCfg.u16DpccRakeRatio);
            pstUsrRegCfg->stUsrDynaCorRegCfg.bResh = bIsOfflineMode;
        }

        bIdxResh = (isp_dpc_update_index_read(pstBeReg) != pstUsrRegCfg->stUsrStaCorRegCfg.u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->stUsrStaCorRegCfg.bResh & bIdxResh) : (pstUsrRegCfg->stUsrStaCorRegCfg.bResh);

        if (bUsrResh) {
            isp_dpc_update_index_write(pstBeReg, pstUsrRegCfg->stUsrStaCorRegCfg.u32UpdateIndex);
            isp_dpc_bpt_number_write(pstBeReg, pstUsrRegCfg->stUsrStaCorRegCfg.u16DpccBptNumber);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
                || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_dpc_bpt_waddr_write(pstBeReg, 0);

                for (j = 0; j < STATIC_DP_COUNT_NORMAL; j++) {
                    if (j < pstUsrRegCfg->stUsrStaCorRegCfg.u16DpccBptNumber) {
                        isp_dpc_bpt_wdata_write(pstBeReg, pstUsrRegCfg->stUsrStaCorRegCfg.au32DpccBpTable[j]);
                    } else {
                        isp_dpc_bpt_wdata_write(pstBeReg, 0);
                    }
                }
            } else {
                isp_dpc_bpt_wlut_write(pstBeReg, pstUsrRegCfg->stUsrStaCorRegCfg.au32DpccBpTable);
            }

            bLutUpdate = HI_TRUE;
            pstUsrRegCfg->stUsrStaCorRegCfg.bResh = bIsOfflineMode;
        }

        /* dynamic */
        pstDynaRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stDpRegCfg.stDynaRegCfg;

        if (pstDynaRegCfg->bResh) {
            isp_dpc_stat_en_write(pstViProcReg, pstDynaRegCfg->bDpcStatEn);
            isp_dpc_blend_write(pstBeReg, pstDynaRegCfg->u32DpccAlpha);
            isp_dpc_mode_write(pstBeReg, pstDynaRegCfg->u16DpccMode);
            isp_dpc_set_use_write(pstBeReg, pstDynaRegCfg->u8DpccSetUse);
            isp_dpc_methods_set_1_write(pstBeReg, pstDynaRegCfg->u16DpccMethodsSet1);
            isp_dpc_methods_set_2_write(pstBeReg, pstDynaRegCfg->u16DpccMethodsSet2);
            isp_dpc_methods_set_3_write(pstBeReg, pstDynaRegCfg->u16DpccMethodsSet3);
            isp_dpc_line_thresh_1_write(pstBeReg, pstDynaRegCfg->au16DpccLineThr[0]);
            isp_dpc_line_mad_fac_1_write(pstBeReg, pstDynaRegCfg->au16DpccLineMadFac[0]);
            isp_dpc_pg_fac_1_write(pstBeReg, pstDynaRegCfg->au16DpccPgFac[0]);
            isp_dpc_rnd_thresh_1_write(pstBeReg, pstDynaRegCfg->au16DpccRndThr[0]);
            isp_dpc_rg_fac_1_write(pstBeReg, pstDynaRegCfg->au16DpccRgFac[0]);
            isp_dpc_line_thresh_2_write(pstBeReg, pstDynaRegCfg->au16DpccLineThr[1]);
            isp_dpc_line_mad_fac_2_write(pstBeReg, pstDynaRegCfg->au16DpccLineMadFac[1]);
            isp_dpc_pg_fac_2_write(pstBeReg, pstDynaRegCfg->au16DpccPgFac[1]);
            isp_dpc_rnd_thresh_2_write(pstBeReg, pstDynaRegCfg->au16DpccRndThr[1]);
            isp_dpc_rg_fac_2_write(pstBeReg, pstDynaRegCfg->au16DpccRgFac[1]);
            isp_dpc_line_thresh_3_write(pstBeReg, pstDynaRegCfg->au16DpccLineThr[2]);
            isp_dpc_line_mad_fac_3_write(pstBeReg, pstDynaRegCfg->au16DpccLineMadFac[2]);
            isp_dpc_pg_fac_3_write(pstBeReg, pstDynaRegCfg->au16DpccPgFac[2]);
            isp_dpc_rnd_thresh_3_write(pstBeReg, pstDynaRegCfg->au16DpccRndThr[2]);
            isp_dpc_rg_fac_3_write(pstBeReg, pstDynaRegCfg->au16DpccRgFac[2]);
            isp_dpc_ro_limits_write(pstBeReg, pstDynaRegCfg->u16DpccRoLimits);
            isp_dpc_rnd_offs_write(pstBeReg, pstDynaRegCfg->u16DpccRndOffs);
            isp_dpc_bpt_thresh_write(pstBeReg, pstDynaRegCfg->u32DpccBadThresh);

            isp_dpc_line_std_thr_1_write(pstBeReg, pstDynaRegCfg->au8DpccLineStdThr[0]);
            isp_dpc_line_std_thr_2_write(pstBeReg, pstDynaRegCfg->au8DpccLineStdThr[1]);
            isp_dpc_line_std_thr_3_write(pstBeReg, pstDynaRegCfg->au8DpccLineStdThr[2]);
            isp_dpc_line_std_thr_4_write(pstBeReg, pstDynaRegCfg->au8DpccLineStdThr[3]);
            isp_dpc_line_std_thr_5_write(pstBeReg, pstDynaRegCfg->au8DpccLineStdThr[4]);


            isp_dpc_line_diff_thr_1_write(pstBeReg, pstDynaRegCfg->au8DpccLineDiffThr[0]);
            isp_dpc_line_diff_thr_2_write(pstBeReg, pstDynaRegCfg->au8DpccLineDiffThr[1]);
            isp_dpc_line_diff_thr_3_write(pstBeReg, pstDynaRegCfg->au8DpccLineDiffThr[2]);
            isp_dpc_line_diff_thr_4_write(pstBeReg, pstDynaRegCfg->au8DpccLineDiffThr[3]);
            isp_dpc_line_diff_thr_5_write(pstBeReg, pstDynaRegCfg->au8DpccLineDiffThr[4]);

            isp_dpc_line_aver_fac_1_write(pstBeReg, pstDynaRegCfg->au8DpccLineAverFac[0]);
            isp_dpc_line_aver_fac_2_write(pstBeReg, pstDynaRegCfg->au8DpccLineAverFac[1]);
            isp_dpc_line_aver_fac_3_write(pstBeReg, pstDynaRegCfg->au8DpccLineAverFac[2]);
            isp_dpc_line_aver_fac_4_write(pstBeReg, pstDynaRegCfg->au8DpccLineAverFac[3]);
            isp_dpc_line_aver_fac_5_write(pstBeReg, pstDynaRegCfg->au8DpccLineAverFac[4]);

            isp_dpc_line_kerdiff_fac_write(pstBeReg, pstDynaRegCfg->u8DpccLineKerdiffFac);
            isp_dpc_blend_mode_write(pstBeReg, pstDynaRegCfg->u8DpccBlendMode);
            isp_dpc_bit_depth_sel_write(pstBeReg, pstDynaRegCfg->u8DpccBitDepthSel);

            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1DpCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bDpcLutUpdate = bLutUpdate;

    return HI_SUCCESS;
}

static HI_S32 ISP_GeRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_U16 j;
    HI_U8 u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_BOOL  bIsOfflineMode;
    ISP_GE_STATIC_CFG_S     *pstStaticRegCfg = HI_NULL;
    ISP_GE_DYNA_CFG_S       *pstDynaRegCfg   = HI_NULL;
    ISP_GE_USR_CFG_S        *pstUsrRegCfg    = HI_NULL;
    S_VIPROC_REGS_TYPE      *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE       *pstBeReg        = HI_NULL;
    isp_usr_ctx               *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)\
                      || IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1GeCfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        isp_ge_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stGeRegCfg.abGeEn[0]);
        isp_ge_ge1_en_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stGeRegCfg.abGeEn[1]);
        isp_ge_ge2_en_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stGeRegCfg.abGeEn[2]);
        isp_ge_ge3_en_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stGeRegCfg.abGeEn[3]);

        /* static */
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stGeRegCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            isp_ge_vnum_write(pstBeReg, pstStaticRegCfg->u8GeNumV);
            isp_ge_gr_en_write(pstBeReg, pstStaticRegCfg->bGeGrEn);
            isp_ge_gb_en_write(pstBeReg, pstStaticRegCfg->bGeGbEn);
            isp_ge_gr_gb_en_write(pstBeReg, pstStaticRegCfg->bGeGrGbEn);
            isp_ge_bit_depth_sel_write(pstBeReg, HI_ISP_GE_BIT_DEPTH_DEFAULT);
            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        /* usr */
        pstUsrRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stGeRegCfg.stUsrRegCfg;

        if (pstUsrRegCfg->bResh) {
            for (j = 0; j < pstRegCfgInfo->alg_reg_cfg[i].stGeRegCfg.u8ChnNum; j++) {
                isp_ge_ct_th2_write(pstBeReg, j, pstUsrRegCfg->au16GeCtTh2[j]);
                isp_ge_ct_slope1_write(pstBeReg, j, pstUsrRegCfg->au8GeCtSlope1[j]);
                isp_ge_ct_slope2_write(pstBeReg, j, pstUsrRegCfg->au8GeCtSlope2[j]);
            }

            isp_ge_hnum_write(pstBeReg, pstUsrRegCfg->u8GeNumH);
            isp_ge_crop_pos_x_write(pstBeReg, pstUsrRegCfg->u16GeCropPosX);
            isp_ge_crop_pos_y_write(pstBeReg, pstUsrRegCfg->u16GeCropPosY);
            isp_ge_crop_out_width_write(pstBeReg, pstUsrRegCfg->u16GeCropOutWidth - 1);
            isp_ge_crop_out_height_write(pstBeReg, pstUsrRegCfg->u16GeCropOutHeight - 1);
            pstUsrRegCfg->bResh = bIsOfflineMode;
        }

        /* dynamic */
        pstDynaRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stGeRegCfg.stDynaRegCfg;

        if (pstDynaRegCfg->bResh) {
            for (j = 0; j < pstRegCfgInfo->alg_reg_cfg[i].stGeRegCfg.u8ChnNum; j++) {
                isp_ge_ct_th1_write(pstBeReg, j, pstDynaRegCfg->au16GeCtTh1[j]);
                isp_ge_ct_th3_write(pstBeReg, j, pstDynaRegCfg->au16GeCtTh3[j]);
            }

            isp_ge_strength_write(pstBeReg, pstDynaRegCfg->u16GeStrength);
            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1GeCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_LscRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_BOOL  bUsrResh   = HI_FALSE;
    HI_BOOL  bIdxResh   = HI_FALSE;
    HI_BOOL  bLutUpdate = HI_FALSE;
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_U16   j;
    ISP_LSC_USR_CFG_S       *pstUsrRegCfg    = HI_NULL;
    ISP_LSC_STATIC_CFG_S    *pstStaticRegCfg = HI_NULL;
    S_VIPROC_REGS_TYPE      *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE       *pstBeReg        = HI_NULL;
    isp_usr_ctx               *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1LscCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        isp_lsc_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stLscRegCfg.bLscEn);

        /* static */
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stLscRegCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            isp_lsc_numh_write(pstBeReg, pstStaticRegCfg->u8WinNumH);
            isp_lsc_numv_write(pstBeReg, pstStaticRegCfg->u8WinNumV);

            isp_bnr_lsc_numh_write(pstBeReg, pstStaticRegCfg->u8WinNumH);
            isp_bnr_lsc_numv_write(pstBeReg, pstStaticRegCfg->u8WinNumV);

            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        /* usr */
        pstUsrRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stLscRegCfg.stUsrRegCfg;

        if (pstUsrRegCfg->bCoefUpdate) {
            isp_lsc_mesh_str_write(pstBeReg, pstUsrRegCfg->u16MeshStr);
            isp_lsc_mesh_weight_write(pstBeReg, pstUsrRegCfg->u16Weight);
            pstUsrRegCfg->bCoefUpdate = bIsOfflineMode;
        }

        bIdxResh = (isp_lsc_update_index_read(pstBeReg) != pstUsrRegCfg->u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bLutUpdate & bIdxResh) : (pstUsrRegCfg->bLutUpdate);

        if (bUsrResh) {
            isp_lsc_update_index_write(pstBeReg, pstUsrRegCfg->u32UpdateIndex);
            isp_lsc_mesh_scale_write(pstBeReg, pstUsrRegCfg->u8MeshScale);
            isp_lsc_width_offset_write(pstBeReg, pstUsrRegCfg->u16WidthOffset);

            isp_bnr_lsc_meshscale_write(pstBeReg, pstUsrRegCfg->u8MeshScale);
            isp_bnr_lsc_mesh_weight_write(pstBeReg, pstUsrRegCfg->u16Weight);
            isp_bnr_lsc_width_offset_write(pstBeReg, pstUsrRegCfg->u16WidthOffset);

            for (j = 0; j < (HI_ISP_LSC_GRID_ROW - 1) / 2; j++) {
                isp_lsc_winY_info_write(pstBeReg, j, pstUsrRegCfg->au16DeltaY[j], pstUsrRegCfg->au16InvY[j]);
                isp_bnr_lsc_winY_info_write(pstBeReg, j, pstUsrRegCfg->au16DeltaY[j], pstUsrRegCfg->au16InvY[j]);
            }

            for (j = 0; j < (HI_ISP_LSC_GRID_COL - 1); j++) {
                isp_lsc_winX_info_write(pstBeReg, j, pstUsrRegCfg->au16DeltaX[j], pstUsrRegCfg->au16InvX[j]);
                isp_bnr_lsc_winX_info_write(pstBeReg, j, pstUsrRegCfg->au16DeltaX[j], pstUsrRegCfg->au16InvX[j]);
            }

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) || \
                IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_lsc_r_gain_waddr_write(pstBeReg, 0);
                isp_bnr_lsc_rgain_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_LSC_GRID_POINTS; j++) {
                    isp_lsc_r_gain_wdata_write(pstBeReg, pstUsrRegCfg->au32RGain[j]);
                    isp_bnr_lsc_rgain_wdata_write(pstBeReg, pstUsrRegCfg->au32RGain[j]);
                }

                isp_lsc_gr_gain_waddr_write(pstBeReg, 0);
                isp_bnr_lsc_grgain_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_LSC_GRID_POINTS; j++) {
                    isp_lsc_gr_gain_wdata_write(pstBeReg, pstUsrRegCfg->au32GrGain[j]);
                    isp_bnr_lsc_grgain_wdata_write(pstBeReg, pstUsrRegCfg->au32GrGain[j]);
                }

                isp_lsc_gb_gain_waddr_write(pstBeReg, 0);
                isp_bnr_lsc_gbgain_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_LSC_GRID_POINTS; j++) {
                    isp_lsc_gb_gain_wdata_write(pstBeReg, pstUsrRegCfg->au32GbGain[j]);
                    isp_bnr_lsc_gbgain_wdata_write(pstBeReg, pstUsrRegCfg->au32GbGain[j]);
                }

                isp_lsc_b_gain_waddr_write(pstBeReg, 0);
                isp_bnr_lsc_bgain_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_LSC_GRID_POINTS; j++) {
                    isp_lsc_b_gain_wdata_write(pstBeReg, pstUsrRegCfg->au32BGain[j]);
                    isp_bnr_lsc_bgain_wdata_write(pstBeReg, pstUsrRegCfg->au32BGain[j]);
                }
            } else {
                isp_lsc_rgain_write(pstBeReg,  pstUsrRegCfg->au32RGain);
                isp_lsc_grgain_write(pstBeReg, pstUsrRegCfg->au32GrGain);
                isp_lsc_gbgain_write(pstBeReg, pstUsrRegCfg->au32GbGain);
                isp_lsc_bgain_write(pstBeReg,  pstUsrRegCfg->au32BGain);

                isp_bnr_lsc_rgain_write(pstBeReg,  pstUsrRegCfg->au32RGain);
                isp_bnr_lsc_grgain_write(pstBeReg, pstUsrRegCfg->au32GrGain);
                isp_bnr_lsc_gbgain_write(pstBeReg, pstUsrRegCfg->au32GbGain);
                isp_bnr_lsc_bgain_write(pstBeReg,  pstUsrRegCfg->au32GbGain);
            }

            bLutUpdate = HI_TRUE;
            pstUsrRegCfg->bLutUpdate = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1LscCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bLscLutUpdate    = bLutUpdate;
    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bBnrLscLutUpdate = bLutUpdate;

    return HI_SUCCESS;
}

static HI_S32 ISP_RLscRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_BOOL  bUsrResh   = HI_FALSE;
    HI_BOOL  bIdxResh   = HI_FALSE;
    HI_BOOL  bLutUpdate = HI_FALSE;
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_U16   j;
    HI_U32   u32LUT0Data, u32LUT1Data;
    ISP_RLSC_USR_CFG_S      *pstUsrRegCfg    = HI_NULL;
    ISP_RLSC_STATIC_CFG_S   *pstStaticRegCfg = HI_NULL;
    S_VIPROC_REGS_TYPE      *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE       *pstBeReg        = HI_NULL;
    isp_usr_ctx               *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)\
                      || IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1RLscCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        isp_rlsc_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stRLscRegCfg.bRLscEn);

        /* static */
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stRLscRegCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            isp_rlsc_nodenum_write(pstBeReg, pstStaticRegCfg->u16NodeNum);

            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        /* usr */
        pstUsrRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stRLscRegCfg.stUsrRegCfg;

        isp_rlsc_rx_write(pstBeReg, pstUsrRegCfg->u16CenterRX);
        isp_rlsc_ry_write(pstBeReg, pstUsrRegCfg->u16CenterRY);
        isp_rlsc_grx_write(pstBeReg, pstUsrRegCfg->u16CenterGrX);
        isp_rlsc_gry_write(pstBeReg, pstUsrRegCfg->u16CenterGrY);
        isp_rlsc_gbx_write(pstBeReg, pstUsrRegCfg->u16CenterGbX);
        isp_rlsc_gby_write(pstBeReg, pstUsrRegCfg->u16CenterGbY);
        isp_rlsc_bx_write(pstBeReg, pstUsrRegCfg->u16CenterBX);
        isp_rlsc_by_write(pstBeReg, pstUsrRegCfg->u16CenterBY);

        isp_rlsc_offsetcenterr_write(pstBeReg, pstUsrRegCfg->u16OffCenterR);
        isp_rlsc_offsetcentergr_write(pstBeReg, pstUsrRegCfg->u16OffCenterGr);
        isp_rlsc_offsetcentergb_write(pstBeReg, pstUsrRegCfg->u16OffCenterGb);
        isp_rlsc_offsetcenterb_write(pstBeReg, pstUsrRegCfg->u16OffCenterB);

        isp_rlsc_validradius_write(pstBeReg, pstUsrRegCfg->u32ValidRadius);

        if (pstUsrRegCfg->bCoefUpdate) {
            isp_rlsc_lsc_en_write(pstBeReg, pstUsrRegCfg->bRLscFuncEn);
            isp_rlsc_crop_en_write(pstBeReg, pstUsrRegCfg->bRadialCropEn);
            isp_rlsc_gainstr_write(pstBeReg, pstUsrRegCfg->u16GainStr);

            pstUsrRegCfg->bCoefUpdate = bIsOfflineMode;
        }

        bIdxResh = (isp_rlsc_update_index_read(pstBeReg) != pstUsrRegCfg->u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bLutUpdate & bIdxResh) : (pstUsrRegCfg->bLutUpdate);

        if (bUsrResh) {
            isp_rlsc_update_index_write(pstBeReg, pstUsrRegCfg->u32UpdateIndex);
            isp_rlsc_gainscale_write(pstBeReg, pstUsrRegCfg->u8GainScale);
            isp_rlsc_widthoffset_write(pstBeReg, pstUsrRegCfg->u16WidthOffset);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
                || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {

                isp_rlsc_lut0_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_RLSC_POINTS + 1; j++) {
                    u32LUT0Data = ((pstUsrRegCfg->au32Lut0Chn1[j] & 0xffff) << 16) + pstUsrRegCfg->au32Lut0Chn0[j];
                    isp_rlsc_lut0_wdata_write(pstBeReg, u32LUT0Data);
                }

                isp_rlsc_lut1_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_RLSC_POINTS + 1; j++) {
                    u32LUT1Data = ((pstUsrRegCfg->au32Lut1Chn3[j] & 0xffff) << 16) + pstUsrRegCfg->au32Lut1Chn2[j];
                    isp_rlsc_lut1_wdata_write(pstBeReg, u32LUT1Data);
                }
            } else {
                isp_rlsc_lut0_ch0_write(pstBeReg, pstUsrRegCfg->au32Lut0Chn0);
                isp_rlsc_lut0_ch1_write(pstBeReg, pstUsrRegCfg->au32Lut0Chn1);
                isp_rlsc_lut1_ch2_write(pstBeReg, pstUsrRegCfg->au32Lut1Chn2);
                isp_rlsc_lut1_ch3_write(pstBeReg, pstUsrRegCfg->au32Lut1Chn3);
            }

            bLutUpdate = HI_TRUE;
            pstUsrRegCfg->bLutUpdate = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1RLscCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bRlscLutUpdate = bLutUpdate;

    return HI_SUCCESS;
}

static HI_S32 ISP_GammaRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_BOOL  bUsrResh   = HI_FALSE;
    HI_BOOL  bIdxResh   = HI_FALSE;
    HI_BOOL  bLutUpdate = HI_FALSE;
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_U16   j;
    S_ISPBE_REGS_TYPE      *pstBeReg        = HI_NULL;
    S_VIPROC_REGS_TYPE     *pstViProcReg    = HI_NULL;
    ISP_GAMMA_USR_CFG_S    *pstUsrRegCfg    = HI_NULL;
    isp_usr_ctx              *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1GammaCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_gamma_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stGammaCfg.bGammaEn);

        pstUsrRegCfg    = &pstRegCfgInfo->alg_reg_cfg[i].stGammaCfg.stUsrRegCfg;

        bIdxResh = (isp_gamma_update_index_read(pstBeReg) != pstUsrRegCfg->u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bGammaLutUpdateEn & bIdxResh) : (pstUsrRegCfg->bGammaLutUpdateEn);

        // LUT update
        if (bUsrResh) {
            isp_gamma_update_index_write(pstBeReg, pstUsrRegCfg->u32UpdateIndex);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) || \
                IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_gamma_lut_waddr_write(pstBeReg, 0);

                for (j = 0; j < GAMMA_NODE_NUMBER; j++) {
                    isp_gamma_lut_wdata_write(pstBeReg, (pstUsrRegCfg->au16GammaLUT[j]) << 2);
                }
            } else {
                isp_gamma_lut_write(pstBeReg, pstUsrRegCfg->au16GammaLUT);
            }

            bLutUpdate = HI_TRUE;

            pstUsrRegCfg->bGammaLutUpdateEn = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1GammaCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bGammaLutUpdate = bLutUpdate;

    return HI_SUCCESS;
}
static HI_S32 ISP_CscRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_U8 u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_BOOL  bIsOfflineMode;
    ISP_CSC_DYNA_CFG_S    *pstDynaRegCfg   = HI_NULL;
    S_VIPROC_REGS_TYPE    *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE     *pstBeReg        = HI_NULL;
    isp_usr_ctx             *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1CscCfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stCscCfg.stDynaRegCfg;

        /* General */
        isp_csc_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stCscCfg.bEnable);

        /* Dynamic */
        if (pstDynaRegCfg->bResh) {
            isp_csc_coef00_write(pstBeReg, pstDynaRegCfg->s16CscCoef00);
            isp_csc_coef01_write(pstBeReg, pstDynaRegCfg->s16CscCoef01);
            isp_csc_coef02_write(pstBeReg, pstDynaRegCfg->s16CscCoef02);
            isp_csc_coef10_write(pstBeReg, pstDynaRegCfg->s16CscCoef10);
            isp_csc_coef11_write(pstBeReg, pstDynaRegCfg->s16CscCoef11);
            isp_csc_coef12_write(pstBeReg, pstDynaRegCfg->s16CscCoef12);
            isp_csc_coef20_write(pstBeReg, pstDynaRegCfg->s16CscCoef20);
            isp_csc_coef21_write(pstBeReg, pstDynaRegCfg->s16CscCoef21);
            isp_csc_coef22_write(pstBeReg, pstDynaRegCfg->s16CscCoef22);

            isp_csc_in_dc0_write(pstBeReg, pstDynaRegCfg->s16CscInDC0); // 10bit, left shift 2 bits
            isp_csc_in_dc1_write(pstBeReg, pstDynaRegCfg->s16CscInDC1); // 10bit, left shift 2 bits
            isp_csc_in_dc2_write(pstBeReg, pstDynaRegCfg->s16CscInDC2); // 10bit, left shift 2 bits

            isp_csc_out_dc0_write(pstBeReg, pstDynaRegCfg->s16CscOutDC0); // 10bit, left shift 2 bits
            isp_csc_out_dc1_write(pstBeReg, pstDynaRegCfg->s16CscOutDC1); // 10bit, left shift 2 bits
            isp_csc_out_dc2_write(pstBeReg, pstDynaRegCfg->s16CscOutDC2); // 10bit, left shift 2 bits

            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1CscCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_CaRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_BOOL  bUsrResh   = HI_FALSE;
    HI_BOOL  bIdxResh   = HI_FALSE;
    HI_BOOL  bLutUpdate = HI_FALSE;
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_U16   j;
    ISP_CA_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_CA_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    ISP_CA_USR_CFG_S    *pstUsrRegCfg    = HI_NULL;
    S_VIPROC_REGS_TYPE  *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE   *pstBeReg        = HI_NULL;
    isp_usr_ctx           *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1CaCfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_ca_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stCaRegCfg.bCaEn);

        /* usr */
        pstUsrRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stCaRegCfg.stUsrRegCfg;
        bIdxResh = (isp_ca_update_index_read(pstBeReg) != pstUsrRegCfg->u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bResh & bIdxResh) : (pstUsrRegCfg->bResh);

        if (bUsrResh) {
            isp_ca_update_index_write(pstBeReg, pstUsrRegCfg->u32UpdateIndex);
            isp_ca_cp_en_write(pstBeReg, pstUsrRegCfg->bCaCpEn);
            isp_ca_lumath_high_write(pstBeReg, pstUsrRegCfg->u16CaLumaThrHigh);
            isp_ca_lumaratio_high_write(pstBeReg, pstUsrRegCfg->u16CaLumaRatioHigh);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
                || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_ca_lut_waddr_write(pstBeReg, 0);
                for (j = 0; j < HI_ISP_CA_YRATIO_LUT_LENGTH; j++) {
                    isp_ca_lut_wdata_write(pstBeReg, pstUsrRegCfg->au32YRatioLUT[j]);
                }
            } else {
                isp_ca_lut_write(pstBeReg, pstUsrRegCfg->au32YRatioLUT);
            }

            bLutUpdate = HI_TRUE;
            pstUsrRegCfg->bResh = bIsOfflineMode;
        }

        /* dynamic */
        pstDynaRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stCaRegCfg.stDynaRegCfg;

        if (pstDynaRegCfg->bResh) {
            isp_ca_isoratio_write(pstBeReg, pstDynaRegCfg->u16CaISORatio);
            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        /* static */
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stCaRegCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            isp_ca_llhcproc_en_write(pstBeReg, pstStaticRegCfg->bCaLlhcProcEn);
            isp_ca_skinproc_en_write(pstBeReg, pstStaticRegCfg->bCaSkinProcEn);
            isp_ca_satadj_en_write(pstBeReg, pstStaticRegCfg->bCaSatuAdjEn);
            isp_ca_lumath_low_write(pstBeReg, pstStaticRegCfg->u16CaLumaThrLow);
            isp_ca_darkchromath_low_write(pstBeReg, pstStaticRegCfg->u16CaDarkChromaThrLow);
            isp_ca_darkchromath_high_write(pstBeReg, pstStaticRegCfg->u16CaDarkChromaThrHigh);
            isp_ca_sdarkchromath_low_write(pstBeReg, pstStaticRegCfg->u16CaSDarkChromaThrLow);
            isp_ca_sdarkchromath_high_write(pstBeReg, pstStaticRegCfg->u16CaSDarkChromaThrHigh);
            isp_ca_lumaratio_low_write(pstBeReg, pstStaticRegCfg->u16CaLumaRatioLow);
            isp_ca_yuv2rgb_coef00_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbCoef00);
            isp_ca_yuv2rgb_coef01_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbCoef01);
            isp_ca_yuv2rgb_coef02_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbCoef02);
            isp_ca_yuv2rgb_coef10_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbCoef10);
            isp_ca_yuv2rgb_coef11_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbCoef11);
            isp_ca_yuv2rgb_coef12_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbCoef12);
            isp_ca_yuv2rgb_coef20_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbCoef20);
            isp_ca_yuv2rgb_coef21_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbCoef21);
            isp_ca_yuv2rgb_coef22_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbCoef22);
            isp_ca_yuv2rgb_indc0_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbInDc0);
            isp_ca_yuv2rgb_indc1_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbInDc1);
            isp_ca_yuv2rgb_indc2_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbInDc2);
            isp_ca_yuv2rgb_outdc0_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbOutDc0);
            isp_ca_yuv2rgb_outdc1_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbOutDc1);
            isp_ca_yuv2rgb_outdc2_write(pstBeReg, pstStaticRegCfg->s16CaYuv2RgbOutDc2);
            isp_ca_skinlluma_umin_write(pstBeReg, pstStaticRegCfg->u16CaSkinLowLumaMinU);
            isp_ca_skinlluma_umax_write(pstBeReg, pstStaticRegCfg->u16CaSkinLowLumaMaxU);
            isp_ca_skinlluma_uymin_write(pstBeReg, pstStaticRegCfg->u16CaSkinLowLumaMinUy);
            isp_ca_skinlluma_uymax_write(pstBeReg, pstStaticRegCfg->u16CaSkinLowLumaMaxUy);
            isp_ca_skinhluma_umin_write(pstBeReg, pstStaticRegCfg->u16CaSkinHighLumaMinU);
            isp_ca_skinhluma_umax_write(pstBeReg, pstStaticRegCfg->u16CaSkinHighLumaMaxU);
            isp_ca_skinhluma_uymin_write(pstBeReg, pstStaticRegCfg->u16CaSkinHighLumaMinUy);
            isp_ca_skinhluma_uymax_write(pstBeReg, pstStaticRegCfg->u16CaSkinHighLumaMaxUy);
            isp_ca_skinlluma_vmin_write(pstBeReg, pstStaticRegCfg->u16CaSkinLowLumaMinV);
            isp_ca_skinlluma_vmax_write(pstBeReg, pstStaticRegCfg->u16CaSkinLowLumaMaxV);
            isp_ca_skinlluma_vymin_write(pstBeReg, pstStaticRegCfg->u16CaSkinLowLumaMinVy);
            isp_ca_skinlluma_vymax_write(pstBeReg, pstStaticRegCfg->u16CaSkinLowLumaMaxVy);
            isp_ca_skinhluma_vmin_write(pstBeReg, pstStaticRegCfg->u16CaSkinHighLumaMinV);
            isp_ca_skinhluma_vmax_write(pstBeReg, pstStaticRegCfg->u16CaSkinHighLumaMaxV);
            isp_ca_skinhluma_vymin_write(pstBeReg, pstStaticRegCfg->u16CaSkinHighLumaMinVy);
            isp_ca_skinhluma_vymax_write(pstBeReg, pstStaticRegCfg->u16CaSkinHighLumaMaxVy);
            isp_ca_skin_uvdiff_write(pstBeReg, pstStaticRegCfg->s16CaSkinUvDiff);
            isp_ca_skinratioth_low_write(pstBeReg, pstStaticRegCfg->u16CaSkinRatioThrLow);
            isp_ca_skinratioth_mid_write(pstBeReg, pstStaticRegCfg->u16CaSkinRatioThrMid);
            isp_ca_skinratioth_high_write(pstBeReg, pstStaticRegCfg->u16CaSkinRatioThrHigh);

            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        pstRegCfgInfo->cfg_key.bit1CaCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bCaLutUpdate = bLutUpdate;

    return HI_SUCCESS;
}

static HI_S32 ISP_McdsRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_U8 u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_BOOL  bIsOfflineMode;
    ISP_MCDS_DYNA_REG_CFG_S    *pstDynaRegCfg   = HI_NULL;
    ISP_MCDS_STATIC_REG_CFG_S  *pstStaticRegCfg = HI_NULL;
    S_VIPROC_REGS_TYPE         *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE          *pstBeReg        = HI_NULL;
    isp_usr_ctx                  *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1McdsCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_mcds_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stMcdsRegCfg.bMCDSen);

        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stMcdsRegCfg.stDynaRegCfg;
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stMcdsRegCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            isp_mcds_hcds_en_write(pstBeReg, pstStaticRegCfg->bHcdsEn);
            isp_mcds_coefh0_write(pstBeReg, pstStaticRegCfg->s32HCoef0);
            isp_mcds_coefh1_write(pstBeReg, pstStaticRegCfg->s32HCoef1);
            isp_mcds_coefh2_write(pstBeReg, pstStaticRegCfg->s32HCoef2);
            isp_mcds_coefh3_write(pstBeReg, pstStaticRegCfg->s32HCoef3);
            isp_mcds_coefh4_write(pstBeReg, pstStaticRegCfg->s32HCoef4);
            isp_mcds_coefh5_write(pstBeReg, pstStaticRegCfg->s32HCoef5);
            isp_mcds_coefh6_write(pstBeReg, pstStaticRegCfg->s32HCoef6);
            isp_mcds_coefh7_write(pstBeReg, pstStaticRegCfg->s32HCoef7);
            isp_mcds_coefv0_write(pstBeReg, pstStaticRegCfg->s32VCoef0);
            isp_mcds_coefv1_write(pstBeReg, pstStaticRegCfg->s32VCoef1);
            isp_mcds_limit_write(pstBeReg, pstStaticRegCfg->u16CoringLimit);
            isp_mcds_midf_bldr_write(pstBeReg, pstStaticRegCfg->u8MidfBldr);
            pstStaticRegCfg->bStaticResh = 0;
        }

        if (pstDynaRegCfg->bDynaResh) {
            pstDynaRegCfg->bDynaResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1McdsCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_VOID isp_wdr_first_frame_reg_config(S_ISPBE_REGS_TYPE *pstBeReg, ISP_WDR_REG_CFG_S *wdr_reg_cfg)
{
    HI_U32 expo_value[4];
    HI_U8  bit_shift = 14 - wdr_reg_cfg->stStaticRegCfg.u8BitDepthInvalid;
    ISP_FSWDR_STATIC_CFG_S *static_reg_cfg = &wdr_reg_cfg->stStaticRegCfg;
    ISP_FSWDR_SYNC_CFG_S    *sync_reg_cfg  = &wdr_reg_cfg->stSyncRegCfg;

    expo_value[0] = static_reg_cfg->au16ExpoValue[0];
    expo_value[1] = static_reg_cfg->au16ExpoValue[1];
    expo_value[2] = static_reg_cfg->au16ExpoValue[2];
    expo_value[3] = static_reg_cfg->au16ExpoValue[3];

    isp_wdr_expovalue0_write(pstBeReg, expo_value[0]);
    isp_wdr_expovalue1_write(pstBeReg, expo_value[1]);
    isp_wdr_expovalue2_write(pstBeReg, expo_value[2]);
    isp_wdr_expovalue3_write(pstBeReg, expo_value[3]);
    isp_wdr_maxratio_write(pstBeReg, static_reg_cfg->u32MaxRatio);
    isp_wdr_exporratio0_write(pstBeReg, static_reg_cfg->au16ExpoRRatio[0]);
    isp_wdr_exporratio1_write(pstBeReg, static_reg_cfg->au16ExpoRRatio[1]);
    isp_wdr_exporratio2_write(pstBeReg, static_reg_cfg->au16ExpoRRatio[2]);
    isp_wdr_fusionmode_write(pstBeReg, wdr_reg_cfg->stUsrRegCfg.bFusionMode);
    isp_wdr_mdt_en_write(pstBeReg, wdr_reg_cfg->stDynaRegCfg.bWDRMdtEn);
    isp_wdr_short_thr_write(pstBeReg, wdr_reg_cfg->stDynaRegCfg.u16ShortThr);
    isp_wdr_long_thr_write(pstBeReg, wdr_reg_cfg->stDynaRegCfg.u16LongThr);

    static_reg_cfg->au32BlcComp[0] = (expo_value[0] - expo_value[1]) * sync_reg_cfg->u16Offset0 >> bit_shift;
    static_reg_cfg->au32BlcComp[1] = (expo_value[1] - expo_value[2]) * sync_reg_cfg->u16Offset1 >> bit_shift;
    static_reg_cfg->au32BlcComp[2] = (expo_value[2] - expo_value[3]) * sync_reg_cfg->u16Offset2 >> bit_shift;
    isp_wdr_blc_comp0_write(pstBeReg, static_reg_cfg->au32BlcComp[0]);
    isp_wdr_blc_comp1_write(pstBeReg, static_reg_cfg->au32BlcComp[1]);
    isp_wdr_blc_comp2_write(pstBeReg, static_reg_cfg->au32BlcComp[2]);
}

static HI_S32 ISP_WdrRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_BOOL  bUsrResh   = HI_FALSE;
    HI_BOOL  bIdxResh   = HI_FALSE;
    HI_BOOL  bLutUpdate = HI_FALSE;
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_U16   j;

    ISP_FSWDR_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_FSWDR_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    ISP_FSWDR_USR_CFG_S    *pstUsrRegCfg    = HI_NULL;
    S_VIPROC_REGS_TYPE     *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE      *pstBeReg        = HI_NULL;
    isp_usr_ctx              *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1FsWdrCfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        isp_wdr_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stWdrRegCfg.bWDREn);

        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stWdrRegCfg.stStaticRegCfg;
        pstUsrRegCfg    = &pstRegCfgInfo->alg_reg_cfg[i].stWdrRegCfg.stUsrRegCfg;
        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stWdrRegCfg.stDynaRegCfg;

        if (pstStaticRegCfg->bFirstFrame == HI_TRUE) {
            isp_wdr_first_frame_reg_config(pstBeReg, &pstRegCfgInfo->alg_reg_cfg[i].stWdrRegCfg);
            pstStaticRegCfg->bFirstFrame = HI_FALSE;
        }

        if (pstStaticRegCfg->bResh) {
            isp_wdr_bitdepth_invalid_write(pstBeReg, pstStaticRegCfg->u8BitDepthInvalid);
            isp_wdr_grayscale_mode_write(pstBeReg, pstStaticRegCfg->bGrayScaleMode);
            isp_wdr_bsaveblc_write(pstBeReg, pstStaticRegCfg->bSaveBLC);
            isp_wdr_mask_similar_thr_write(pstBeReg, pstStaticRegCfg->u8MaskSimilarThr);
            isp_wdr_mask_similar_cnt_write(pstBeReg, pstStaticRegCfg->u8MaskSimilarCnt);
            isp_wdr_dftwgt_fl_write(pstBeReg, pstStaticRegCfg->u16dftWgtFL);

            isp_wdr_bldrlhfidx_write(pstBeReg, pstStaticRegCfg->u8bldrLHFIdx);
            isp_wdr_fusion_rlow_thr_write(pstBeReg, pstStaticRegCfg->u16FusionRLowThr);
            isp_wdr_fusion_rhig_thr_write(pstBeReg, pstStaticRegCfg->u16FusionRHigThr);
            isp_wdr_bnr_nosmode_write(pstBeReg, pstStaticRegCfg->bNrNosMode);
            isp_wdr_saturate_thr_write(pstBeReg, pstStaticRegCfg->u16SaturateThr);
            pstStaticRegCfg->bResh = HI_FALSE;
        }

        /* usr */
        bIdxResh = (isp_wdr_update_index_read(pstBeReg) != pstUsrRegCfg->u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bResh & bIdxResh) : (pstUsrRegCfg->bResh);

        if (bUsrResh) {
            isp_wdr_update_index_write(pstBeReg, pstUsrRegCfg->u32UpdateIndex);
            isp_wdr_bnr_write(pstBeReg, pstUsrRegCfg->bWDRBnr);
            isp_wdr_shortexpo_chk_write(pstBeReg, pstUsrRegCfg->bShortExpoChk);
            isp_wdr_fusion_bnr_write(pstBeReg, pstUsrRegCfg->bFusionBnr);
            isp_wdr_mdtlbld_write(pstBeReg, pstUsrRegCfg->u8MdtLBld);
            isp_wdr_mdt_full_thr_write(pstBeReg, pstUsrRegCfg->u8MdtFullThr);
            isp_wdr_bnr_fullmdt_thr_write(pstBeReg, pstUsrRegCfg->u8BnrFullMdtThr);
            isp_wdr_2dnr_weightg0_write(pstBeReg, pstUsrRegCfg->u8BnrWgtG0);
            isp_wdr_2dnr_weightg1_write(pstBeReg, pstUsrRegCfg->u8BnrWgtG1);
            isp_wdr_2dnr_weightg2_write(pstBeReg, pstUsrRegCfg->u8BnrWgtG2);
            isp_wdr_2dnr_weightc0_write(pstBeReg, pstUsrRegCfg->u8BnrWgtC0);
            isp_wdr_2dnr_weightc1_write(pstBeReg, pstUsrRegCfg->u8BnrWgtC1);
            isp_wdr_2dnr_weightc2_write(pstBeReg, pstUsrRegCfg->u8BnrWgtC2);
            isp_wdr_fullmdt_sigwgt_write(pstBeReg, pstUsrRegCfg->u8FullMotSigWgt);
            isp_wdr_mdt_still_thr_write(pstBeReg, pstUsrRegCfg->u8MdtStillThr);
            isp_wdr_fullmdt_sigwgt_write(pstBeReg, pstUsrRegCfg->u8FullMotSigWgt);
            isp_wdr_fusion_f0_thr_write(pstBeReg, pstUsrRegCfg->au16FusionThr[0]);
            isp_wdr_fusion_f1_thr_write(pstBeReg, pstUsrRegCfg->au16FusionThr[1]);
            isp_wdr_fusion_f2_thr_write(pstBeReg, pstUsrRegCfg->au16FusionThr[2]);
            isp_wdr_fusion_f3_thr_write(pstBeReg, pstUsrRegCfg->au16FusionThr[3]);
            isp_wdr_gsigma_gain1_write(pstBeReg, pstUsrRegCfg->u8Gsigma_gain1);
            isp_wdr_csigma_gain1_write(pstBeReg, pstUsrRegCfg->u8Csigma_gain1);
            isp_wdr_gsigma_gain2_write(pstBeReg, pstUsrRegCfg->u8Gsigma_gain2);
            isp_wdr_gsigma_gain3_write(pstBeReg, pstUsrRegCfg->u8Gsigma_gain3);
            isp_wdr_csigma_gain2_write(pstBeReg, pstUsrRegCfg->u8Csigma_gain2);
            isp_wdr_csigma_gain3_write(pstBeReg, pstUsrRegCfg->u8Csigma_gain3);

            pstUsrRegCfg->bResh = bIsOfflineMode;
        }

        /* dynamic */
        if (pstDynaRegCfg->bResh) {
            isp_wdr_sqrt_again_g_write(pstBeReg, pstDynaRegCfg->u8SqrtAgainG);
            isp_wdr_sqrt_dgain_g_write(pstBeReg, pstDynaRegCfg->u8SqrtDgainG);
            isp_wdr_mdt_nosfloor_write(pstBeReg, pstDynaRegCfg->u8MdtNosFloor);
            isp_wdr_mdthr_low_gain_write(pstBeReg, pstDynaRegCfg->u8MdThrLowGain);
            isp_wdr_mdthr_hig_gain_write(pstBeReg, pstDynaRegCfg->u8MdThrHigGain);
            isp_wdr_tnosfloor_write(pstBeReg, pstDynaRegCfg->u16TNosFloor);
            isp_wdr_f0_still_thr_write(pstBeReg, pstDynaRegCfg->au16StillThr[0]);
            isp_wdr_f1_still_thr_write(pstBeReg, pstDynaRegCfg->au16StillThr[1]);
            isp_wdr_f2_still_thr_write(pstBeReg, pstDynaRegCfg->au16StillThr[2]);
            isp_wdr_nosfloor_r_write(pstBeReg, pstDynaRegCfg->u16NosFloorR);
            isp_wdr_nosfloor_g_write(pstBeReg, pstDynaRegCfg->u16NosFloorG);
            isp_wdr_nosfloor_b_write(pstBeReg, pstDynaRegCfg->u16NosFloorB);

            isp_wdr_modelcoef_rgain_write(pstBeReg, pstDynaRegCfg->u16ModelCoefRgain);
            isp_wdr_modelcoef_ggain_write(pstBeReg, pstDynaRegCfg->u16ModelCoefGgain);
            isp_wdr_modelcoef_bgain_write(pstBeReg, pstDynaRegCfg->u16ModelCoefBgain);

            isp_wdr_erosion_en_write(pstBeReg, pstDynaRegCfg->bErosionEn);

            isp_wdr_nos_np_thr_write(pstBeReg, pstDynaRegCfg->u16NosNpThr);

            isp_bcom_en_write(pstViProcReg, pstDynaRegCfg->bBcomEn);
            isp_bdec_en_write(pstViProcReg, pstDynaRegCfg->bBdecEn);
            isp_bcom_alpha_write(pstBeReg, pstDynaRegCfg->u8bcom_alpha);
            isp_bdec_alpha_write(pstBeReg, pstDynaRegCfg->u8bdec_alpha);
            isp_wdr_mergeframe_write(pstBeReg, pstDynaRegCfg->u8FrmMerge);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
                || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_wdr_noslut129x8_waddr_write(pstBeReg, 0);

                for (j = 0; j < NLUT_LENGTH; j++) {
                    isp_wdr_noslut129x8_wdata_write(pstBeReg, pstDynaRegCfg->as32BnrNosMDTLut[j]);
                }

                bLutUpdate = pstDynaRegCfg->bUpdateNosLut;
            } else {
                isp_wdr_noslut129x8_write(pstBeReg, pstDynaRegCfg->as32BnrNosMDTLut);
                bLutUpdate = HI_TRUE;
            }

            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1FsWdrCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bWdrLutUpdate = bLutUpdate;

    return HI_SUCCESS;
}

static HI_S32 ISP_DrcRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_BOOL  bUsrResh  = HI_FALSE;
    HI_BOOL  bIdxResh  = HI_FALSE;
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_U16   j;
    ISP_DRC_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_DRC_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    ISP_DRC_USR_CFG_S    *pstUsrRegCfg    = HI_NULL;
    S_VIPROC_REGS_TYPE   *pstViProcReg    = HI_NULL;
    S_ISPFE_REGS_TYPE    *pstFeReg        = HI_NULL;
    S_ISPBE_REGS_TYPE    *pstBeReg        = HI_NULL;
    isp_usr_ctx            *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1DrcCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstFeReg     = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(0);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstFeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_drc_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stDrcRegCfg.bDrcEn);

        if (pstRegCfgInfo->alg_reg_cfg[i].stDrcRegCfg.bDrcEn == HI_TRUE) {
            isp_drc_dither_en_write(pstBeReg, HI_FALSE);
        } else {
            isp_drc_dither_en_write(pstBeReg, !(isp_ctx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR));
        }

        /* Static */
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stDrcRegCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bStaticResh) {
            isp_drc_outbits_write(pstBeReg, pstStaticRegCfg->u8BitWidthOut);

            isp_drc_wrstat_en_write(pstBeReg, pstStaticRegCfg->bWrstatEn);
            isp_drc_rdstat_en_write(pstBeReg, pstStaticRegCfg->bRdstatEn);

            isp_drc_bin_write(pstBeReg, pstStaticRegCfg->u8BinNumZ);

            /* Static Registers */
            isp_drc_local_edge_lmt_write(pstBeReg, pstStaticRegCfg->u8LocalEdgeLmt);

            isp_drc_r_wgt_write(pstBeReg, pstStaticRegCfg->u8RWgt);
            isp_drc_g_wgt_write(pstBeReg, pstStaticRegCfg->u8GWgt);
            isp_drc_b_wgt_write(pstBeReg, pstStaticRegCfg->u8BWgt);

            isp_drc_cc_lin_pow_write(pstBeReg, pstStaticRegCfg->u8ColorControlMode);
            isp_drc_cc_lut_ctrl_write(pstBeReg, pstStaticRegCfg->u8ColorControlLUTCtrl);
            isp_drc_cc_global_corr_write(pstBeReg, pstStaticRegCfg->u16GlobalColorCorr);

            isp_drc_wgt_box_tri_sel_write(pstBeReg, pstStaticRegCfg->bWgtBoxTriSel);
            isp_drc_detail_boost_en_write(pstBeReg, pstStaticRegCfg->bDetailBoostEnable);
            isp_drc_pdw_sum_en_write(pstBeReg, pstStaticRegCfg->bPdwSumEnable);

            // Purple Fringe Detection & Reduction
            isp_drc_rg_ctr_write(pstBeReg, pstStaticRegCfg->u8PFRRGCtr);
            isp_drc_rg_wid_write(pstBeReg, pstStaticRegCfg->u8PFRRGWid);
            isp_drc_rg_slo_write(pstBeReg, pstStaticRegCfg->u8PFRRGSlo);

            isp_drc_bg_thr_write(pstBeReg, pstStaticRegCfg->u8PFRBGThr);
            isp_drc_bg_slo_write(pstBeReg, pstStaticRegCfg->u8PFRBGSlo);

            // ISP-FE DRCS
            isp_fe_drcs_en_write(pstFeReg, pstStaticRegCfg->bDrcsEn);
            isp_drcs_vbiflt_en_write(pstFeReg, pstStaticRegCfg->bDrcsVbiFltEn);
            isp_drcs_wrstat_en_write(pstFeReg, pstStaticRegCfg->bDrcsWrtStatEn);
            isp_drcs_bin_write(pstFeReg, pstStaticRegCfg->u8BinNumZ);

            isp_drcs_r_wgt_write(pstFeReg, pstStaticRegCfg->u8RWgt);
            isp_drcs_g_wgt_write(pstFeReg, pstStaticRegCfg->u8GWgt);
            isp_drcs_b_wgt_write(pstFeReg, pstStaticRegCfg->u8BWgt);
            isp_drcs_wgt_box_tri_sel_write(pstFeReg, pstStaticRegCfg->bWgtBoxTriSel);
            if (pstStaticRegCfg->bFirstFrame) {
                isp_drc_shp_log_write(pstBeReg, pstStaticRegCfg->u8ShpLog);
                isp_drc_shp_exp_write(pstBeReg, pstStaticRegCfg->u8ShpExp);
                isp_drc_div_denom_log_write(pstBeReg, pstStaticRegCfg->u32DivDenomLog);
                isp_drc_denom_exp_write(pstBeReg, pstStaticRegCfg->u32DenomExp);
                isp_drc_prev_luma_0_write(pstBeReg, pstStaticRegCfg->au32PrevLuma[0]);
                isp_drc_prev_luma_1_write(pstBeReg, pstStaticRegCfg->au32PrevLuma[1]);
                isp_drc_prev_luma_2_write(pstBeReg, pstStaticRegCfg->au32PrevLuma[2]);
                isp_drc_prev_luma_3_write(pstBeReg, pstStaticRegCfg->au32PrevLuma[3]);
                isp_drc_prev_luma_4_write(pstBeReg, pstStaticRegCfg->au32PrevLuma[4]);
                isp_drc_prev_luma_5_write(pstBeReg, pstStaticRegCfg->au32PrevLuma[5]);
                isp_drc_prev_luma_6_write(pstBeReg, pstStaticRegCfg->au32PrevLuma[6]);
                isp_drc_prev_luma_7_write(pstBeReg, pstStaticRegCfg->au32PrevLuma[7]);

                pstStaticRegCfg->bFirstFrame = HI_FALSE;
            }

            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        /* User */
        pstUsrRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stDrcRegCfg.stUsrRegCfg;
        bIdxResh = (isp_drc_update_index_read(pstBeReg) != pstUsrRegCfg->u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstUsrRegCfg->bUsrResh & bIdxResh) : (pstUsrRegCfg->bUsrResh);

        if (bUsrResh) {
            isp_drc_update_index_write(pstBeReg, pstUsrRegCfg->u32UpdateIndex);

            isp_drc_sft1_y_write(pstBeReg, pstUsrRegCfg->u8YSFT1);
            isp_drc_val1_y_write(pstBeReg, pstUsrRegCfg->u8YVAL1);
            isp_drc_sft2_y_write(pstBeReg, pstUsrRegCfg->u8YSFT2);
            isp_drc_val2_y_write(pstBeReg, pstUsrRegCfg->u8YVAL2);

            isp_drc_sft1_c_write(pstBeReg, pstUsrRegCfg->u8CSFT1);
            isp_drc_val1_c_write(pstBeReg, pstUsrRegCfg->u8CVAL1);
            isp_drc_sft2_c_write(pstBeReg, pstUsrRegCfg->u8CSFT2);
            isp_drc_val2_c_write(pstBeReg, pstUsrRegCfg->u8CVAL2);

            isp_drc_sft_write(pstBeReg, pstUsrRegCfg->u8SFT);
            isp_drc_val_write(pstBeReg, pstUsrRegCfg->u8VAL);

            isp_drc_var_spa_coarse_write(pstBeReg, pstUsrRegCfg->u8VarSpaCoarse);
            isp_drc_var_spa_medium_write(pstBeReg, pstUsrRegCfg->u8VarSpaMedium);
            isp_drc_var_spa_fine_write(pstBeReg, pstUsrRegCfg->u8VarSpaFine);

            isp_drc_var_rng_coarse_write(pstBeReg, pstUsrRegCfg->u8VarRngCoarse);
            isp_drc_var_rng_medium_write(pstBeReg, pstUsrRegCfg->u8VarRngMedium);
            isp_drc_var_rng_fine_write(pstBeReg, pstUsrRegCfg->u8VarRngFine);

            isp_drc_grad_rev_shift_write(pstBeReg, pstUsrRegCfg->u8GradShift);
            isp_drc_grad_rev_slope_write(pstBeReg, pstUsrRegCfg->u8GradSlope);
            isp_drc_grad_rev_max_write(pstBeReg, pstUsrRegCfg->u8GradMax);
            isp_drc_grad_rev_thres_write(pstBeReg, pstUsrRegCfg->u8GradThr);

            isp_drc_high_slo_write(pstBeReg, pstUsrRegCfg->u8PFRHighSlo);
            isp_drc_low_slo_write(pstBeReg, pstUsrRegCfg->u8PFRLowSlo);
            isp_drc_low_thr_write(pstBeReg, pstUsrRegCfg->u8PFRLowThr);

            isp_drc_gain_clip_knee_write(pstBeReg, pstUsrRegCfg->u8GainClipKnee);
            isp_drc_gain_clip_step_write(pstBeReg, pstUsrRegCfg->u8GainClipStep);

            isp_drc_mixing_coring_write(pstBeReg, pstUsrRegCfg->u8MixingCoring);
            isp_drc_dark_min_write(pstBeReg, pstUsrRegCfg->u8MixingDarkMin);
            isp_drc_dark_max_write(pstBeReg, pstUsrRegCfg->u8MixingDarkMax);
            isp_drc_dark_thr_write(pstBeReg, pstUsrRegCfg->u8MixingDarkThr);
            isp_drc_dark_slo_write(pstBeReg, pstUsrRegCfg->s8MixingDarkSlo);

            isp_drc_bright_min_write(pstBeReg, pstUsrRegCfg->u8MixingBrightMin);
            isp_drc_bright_max_write(pstBeReg, pstUsrRegCfg->u8MixingBrightMax);
            isp_drc_bright_thr_write(pstBeReg, pstUsrRegCfg->u8MixingBrightThr);
            isp_drc_bright_slo_write(pstBeReg, pstUsrRegCfg->s8MixingBrightSlo);

            isp_drc_detail_coring_write(pstBeReg, pstUsrRegCfg->u8DetailCoring);
            isp_drc_dark_step_write(pstBeReg, pstUsrRegCfg->u8DetailDarkStep);
            isp_drc_bright_step_write(pstBeReg, pstUsrRegCfg->u8DetailBrightStep);

            isp_drc_detail_dark_min_write(pstBeReg, pstUsrRegCfg->u8DetailDarkMin);
            isp_drc_detail_dark_max_write(pstBeReg, pstUsrRegCfg->u8DetailDarkMax);
            isp_drc_detail_dark_thr_write(pstBeReg, pstUsrRegCfg->u8DetailDarkThr);
            isp_drc_detail_dark_slo_write(pstBeReg, pstUsrRegCfg->s8DetailDarkSlo);

            isp_drc_detail_bright_min_write(pstBeReg, pstUsrRegCfg->u8DetailBrightMin);
            isp_drc_detail_bright_max_write(pstBeReg, pstUsrRegCfg->u8DetailBrightMax);
            isp_drc_detail_bright_thr_write(pstBeReg, pstUsrRegCfg->u8DetailBrightThr);
            isp_drc_detail_bright_slo_write(pstBeReg, pstUsrRegCfg->s8DetailBrightSlo);

            // dark & bright curve write
            isp_drc_detail_dark_curve0_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[0]);
            isp_drc_detail_dark_curve1_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[1]);
            isp_drc_detail_dark_curve2_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[2]);
            isp_drc_detail_dark_curve3_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[3]);
            isp_drc_detail_dark_curve4_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[4]);
            isp_drc_detail_dark_curve5_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[5]);
            isp_drc_detail_dark_curve6_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[6]);
            isp_drc_detail_dark_curve7_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[7]);
            isp_drc_detail_dark_curve8_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[8]);
            isp_drc_detail_dark_curve9_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[9]);
            isp_drc_detail_dark_curve10_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[10]);
            isp_drc_detail_dark_curve11_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[11]);
            isp_drc_detail_dark_curve12_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[12]);
            isp_drc_detail_dark_curve13_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[13]);
            isp_drc_detail_dark_curve14_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[14]);
            isp_drc_detail_dark_curve15_write(pstBeReg, pstUsrRegCfg->u8DetailDarkCurve[15]);

            isp_drc_detail_bright_curve0_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[0]);
            isp_drc_detail_bright_curve1_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[1]);
            isp_drc_detail_bright_curve2_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[2]);
            isp_drc_detail_bright_curve3_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[3]);
            isp_drc_detail_bright_curve4_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[4]);
            isp_drc_detail_bright_curve5_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[5]);
            isp_drc_detail_bright_curve6_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[6]);
            isp_drc_detail_bright_curve7_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[7]);
            isp_drc_detail_bright_curve8_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[8]);
            isp_drc_detail_bright_curve9_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[9]);
            isp_drc_detail_bright_curve10_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[10]);
            isp_drc_detail_bright_curve11_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[11]);
            isp_drc_detail_bright_curve12_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[12]);
            isp_drc_detail_bright_curve13_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[13]);
            isp_drc_detail_bright_curve14_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[14]);
            isp_drc_detail_bright_curve15_write(pstBeReg, pstUsrRegCfg->u8DetailBrightCurve[15]);


            isp_drc_cubic_sel_write(pstBeReg, pstUsrRegCfg->bCubicCurveSel);

            isp_drc_cubic_thresx01_write(pstBeReg, pstUsrRegCfg->u16CubicThres01);
            isp_drc_cubic_thresx10_write(pstBeReg, pstUsrRegCfg->u16CubicThres10);
            isp_drc_cubic_thresx11_write(pstBeReg, pstUsrRegCfg->u16CubicThres11);

            isp_drc_cubic_coef00_aexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef00AExp);
            isp_drc_cubic_coef00_a_write(pstBeReg, pstUsrRegCfg->u32CubicCoef00A);
            isp_drc_cubic_coef01_bexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef01BExp);
            isp_drc_cubic_coef01_b_write(pstBeReg, pstUsrRegCfg->u32CubicCoef01B);
            isp_drc_cubic_coef02_cexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef02CExp);
            isp_drc_cubic_coef02_c_write(pstBeReg, pstUsrRegCfg->u32CubicCoef02C);
            isp_drc_cubic_coef03_d_write(pstBeReg, pstUsrRegCfg->u32CubicCoef03D);

            isp_drc_cubic_coef10_aexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef10AExp);
            isp_drc_cubic_coef10_a_write(pstBeReg, pstUsrRegCfg->u32CubicCoef10A);
            isp_drc_cubic_coef11_bexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef11BExp);
            isp_drc_cubic_coef11_b_write(pstBeReg, pstUsrRegCfg->u32CubicCoef11B);
            isp_drc_cubic_coef12_cexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef12CExp);
            isp_drc_cubic_coef12_c_write(pstBeReg, pstUsrRegCfg->u32CubicCoef12C);
            isp_drc_cubic_coef13_d_write(pstBeReg, pstUsrRegCfg->u32CubicCoef13D);

            isp_drc_cubic_coef20_aexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef20AExp);
            isp_drc_cubic_coef20_a_write(pstBeReg, pstUsrRegCfg->u32CubicCoef20A);
            isp_drc_cubic_coef21_bexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef21BExp);
            isp_drc_cubic_coef21_b_write(pstBeReg, pstUsrRegCfg->u32CubicCoef21B);
            isp_drc_cubic_coef22_cexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef22CExp);
            isp_drc_cubic_coef22_c_write(pstBeReg, pstUsrRegCfg->u32CubicCoef22C);
            isp_drc_cubic_coef23_d_write(pstBeReg, pstUsrRegCfg->u32CubicCoef23D);

            isp_drc_cubic_coef30_aexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef30AExp);
            isp_drc_cubic_coef30_a_write(pstBeReg, pstUsrRegCfg->u32CubicCoef30A);
            isp_drc_cubic_coef31_bexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef31BExp);
            isp_drc_cubic_coef31_b_write(pstBeReg, pstUsrRegCfg->u32CubicCoef31B);
            isp_drc_cubic_coef32_cexp_write(pstBeReg, pstUsrRegCfg->u8CubicCoef32CExp);
            isp_drc_cubic_coef32_c_write(pstBeReg, pstUsrRegCfg->u32CubicCoef32C);
            isp_drc_cubic_coef33_d_write(pstBeReg, pstUsrRegCfg->u32CubicCoef33D);

            /* Spatial/range filtering coefficients */
            isp_drc_flt_spa_fine_write(pstBeReg, pstUsrRegCfg->u8FltSpaFine);
            isp_drc_flt_spa_medium_write(pstBeReg, pstUsrRegCfg->u8FltSpaMedium);
            isp_drc_flt_spa_coarse_write(pstBeReg, pstUsrRegCfg->u8FltSpaCoarse);
            isp_drc_flt_rng_fine_write(pstBeReg, pstUsrRegCfg->u8FltRngFine);
            isp_drc_flt_rng_medium_write(pstBeReg, pstUsrRegCfg->u8FltRngMedium);
            isp_drc_flt_rng_coarse_write(pstBeReg, pstUsrRegCfg->u8FltRngCoarse);

            /* Adaptive range filtering parameters */
            isp_drc_fr_ada_max_write(pstBeReg, pstUsrRegCfg->u8FltRngAdaMax);
            isp_drc_dis_offset_coef_write(pstBeReg, pstUsrRegCfg->u8DisOffsetCoef);
            isp_drc_thr_coef_low_write(pstBeReg, pstUsrRegCfg->u8DisThrCoefLow);
            isp_drc_thr_coef_high_write(pstBeReg, pstUsrRegCfg->u8DisThrCoefHigh);

            isp_drc_detail_sub_factor_write(pstBeReg, pstUsrRegCfg->s8DetailSubFactor);


            isp_drc_bin_mix_factor_coarse_0_write(pstBeReg, pstUsrRegCfg->au8BinMixCoarse[0]);
            isp_drc_bin_mix_factor_coarse_1_write(pstBeReg, pstUsrRegCfg->au8BinMixCoarse[1]);
            isp_drc_bin_mix_factor_coarse_2_write(pstBeReg, pstUsrRegCfg->au8BinMixCoarse[2]);
            isp_drc_bin_mix_factor_coarse_3_write(pstBeReg, pstUsrRegCfg->au8BinMixCoarse[3]);
            isp_drc_bin_mix_factor_coarse_4_write(pstBeReg, pstUsrRegCfg->au8BinMixCoarse[4]);
            isp_drc_bin_mix_factor_coarse_5_write(pstBeReg, pstUsrRegCfg->au8BinMixCoarse[5]);
            isp_drc_bin_mix_factor_coarse_6_write(pstBeReg, pstUsrRegCfg->au8BinMixCoarse[6]);
            isp_drc_bin_mix_factor_coarse_7_write(pstBeReg, pstUsrRegCfg->au8BinMixCoarse[7]);

            isp_drc_bin_mix_factor_medium_0_write(pstBeReg, pstUsrRegCfg->au8BinMixMedium[0]);
            isp_drc_bin_mix_factor_medium_1_write(pstBeReg, pstUsrRegCfg->au8BinMixMedium[1]);
            isp_drc_bin_mix_factor_medium_2_write(pstBeReg, pstUsrRegCfg->au8BinMixMedium[2]);
            isp_drc_bin_mix_factor_medium_3_write(pstBeReg, pstUsrRegCfg->au8BinMixMedium[3]);
            isp_drc_bin_mix_factor_medium_4_write(pstBeReg, pstUsrRegCfg->au8BinMixMedium[4]);
            isp_drc_bin_mix_factor_medium_5_write(pstBeReg, pstUsrRegCfg->au8BinMixMedium[5]);
            isp_drc_bin_mix_factor_medium_6_write(pstBeReg, pstUsrRegCfg->au8BinMixMedium[6]);
            isp_drc_bin_mix_factor_medium_7_write(pstBeReg, pstUsrRegCfg->au8BinMixMedium[7]);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
                || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_drc_cclut_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_DRC_CC_NODE_NUM; j++) {
                    isp_drc_cclut_wdata_write(pstBeReg, pstUsrRegCfg->u16CCLUT[j]);
                }
            } else {
                isp_drc_cclut_write(pstBeReg, pstUsrRegCfg->u16CCLUT);
            }

            // ISP-FE DRCS
            isp_drcs_var_spa_coarse_write(pstFeReg, pstUsrRegCfg->u8VarSpaCoarse);
            isp_drcs_var_spa_medium_write(pstFeReg, pstUsrRegCfg->u8VarSpaMedium);
            isp_drcs_var_spa_fine_write(pstFeReg, pstUsrRegCfg->u8VarSpaFine);

            isp_drcs_var_rng_coarse_write(pstFeReg, pstUsrRegCfg->u8VarRngCoarse);
            isp_drcs_var_rng_medium_write(pstFeReg, pstUsrRegCfg->u8VarRngMedium);
            isp_drcs_var_rng_fine_write(pstFeReg, pstUsrRegCfg->u8VarRngFine);

            isp_drcs_fr_ada_max_write(pstFeReg, pstUsrRegCfg->u8FltRngAdaMax);
            isp_drcs_dis_offset_coef_write(pstFeReg, pstUsrRegCfg->u8DisOffsetCoef);
            isp_drcs_thr_coef_low_write(pstFeReg, pstUsrRegCfg->u8DisThrCoefLow);
            isp_drcs_thr_coef_high_write(pstFeReg, pstUsrRegCfg->u8DisThrCoefHigh);

            isp_drcs_bin_mix_factor_coarse_0_write(pstFeReg, pstUsrRegCfg->au8BinMixCoarse[0]);
            isp_drcs_bin_mix_factor_coarse_1_write(pstFeReg, pstUsrRegCfg->au8BinMixCoarse[1]);
            isp_drcs_bin_mix_factor_coarse_2_write(pstFeReg, pstUsrRegCfg->au8BinMixCoarse[2]);
            isp_drcs_bin_mix_factor_coarse_3_write(pstFeReg, pstUsrRegCfg->au8BinMixCoarse[3]);
            isp_drcs_bin_mix_factor_coarse_4_write(pstFeReg, pstUsrRegCfg->au8BinMixCoarse[4]);
            isp_drcs_bin_mix_factor_coarse_5_write(pstFeReg, pstUsrRegCfg->au8BinMixCoarse[5]);
            isp_drcs_bin_mix_factor_coarse_6_write(pstFeReg, pstUsrRegCfg->au8BinMixCoarse[6]);
            isp_drcs_bin_mix_factor_coarse_7_write(pstFeReg, pstUsrRegCfg->au8BinMixCoarse[7]);

            isp_drcs_bin_mix_factor_medium_0_write(pstFeReg, pstUsrRegCfg->au8BinMixMedium[0]);
            isp_drcs_bin_mix_factor_medium_1_write(pstFeReg, pstUsrRegCfg->au8BinMixMedium[1]);
            isp_drcs_bin_mix_factor_medium_2_write(pstFeReg, pstUsrRegCfg->au8BinMixMedium[2]);
            isp_drcs_bin_mix_factor_medium_3_write(pstFeReg, pstUsrRegCfg->au8BinMixMedium[3]);
            isp_drcs_bin_mix_factor_medium_4_write(pstFeReg, pstUsrRegCfg->au8BinMixMedium[4]);
            isp_drcs_bin_mix_factor_medium_5_write(pstFeReg, pstUsrRegCfg->au8BinMixMedium[5]);
            isp_drcs_bin_mix_factor_medium_6_write(pstFeReg, pstUsrRegCfg->au8BinMixMedium[6]);
            isp_drcs_bin_mix_factor_medium_7_write(pstFeReg, pstUsrRegCfg->au8BinMixMedium[7]);

            isp_drcs_flt_spa_fine_write(pstFeReg, pstUsrRegCfg->u8FltSpaFine);
            isp_drcs_flt_spa_medium_write(pstFeReg, pstUsrRegCfg->u8FltSpaMedium);
            isp_drcs_flt_spa_coarse_write(pstFeReg, pstUsrRegCfg->u8FltSpaCoarse);
            isp_drcs_flt_rng_fine_write(pstFeReg, pstUsrRegCfg->u8FltRngFine);
            isp_drcs_flt_rng_medium_write(pstFeReg, pstUsrRegCfg->u8FltRngMedium);
            isp_drcs_flt_rng_coarse_write(pstFeReg, pstUsrRegCfg->u8FltRngCoarse);

            isp_drcs_shp_exp_write(pstFeReg, pstUsrRegCfg->u8ShpExp);
            isp_drcs_shp_log_write(pstFeReg, pstUsrRegCfg->u8ShpLog);
            isp_drcs_div_denom_log_write(pstFeReg, pstUsrRegCfg->u32DivDenomLog);
            isp_drcs_denom_exp_write(pstFeReg, pstUsrRegCfg->u32DenomExp);

            pstUsrRegCfg->bUsrResh = bIsOfflineMode;
        }

        /* Dynamic */
        pstDynaRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stDrcRegCfg.stDynaRegCfg;

        if (pstDynaRegCfg->bDynaResh) {
            isp_drc_high_thr_write(pstBeReg, pstDynaRegCfg->u8PFRHighThr);
            isp_drc_vbiflt_en_write(pstBeReg, pstDynaRegCfg->bVbifltEn);
            isp_drc_color_corr_en_write(pstBeReg, pstDynaRegCfg->bColorCorrEnable);

            if (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                IS_STRIPING_MODE(isp_ctx->block_attr.running_mode)) {
                isp_drc_wrstat_en_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stDrcRegCfg.bDrcEn);
                isp_drc_rdstat_en_write(pstBeReg, pstRegCfgInfo->alg_reg_cfg[i].stDrcRegCfg.bDrcEn);
            } else if (IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_fe_drcs_en_write(pstFeReg, pstRegCfgInfo->alg_reg_cfg[i].stDrcRegCfg.bDrcEn);
            }

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) || \
                IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                // LutUpdateMode: 0 - update none, 1 - update Lut0, 2 - update Lut1, 3 - update both
                if (pstDynaRegCfg->u8LutUpdateMode & 0x1) { // 1 or 3
                    isp_drc_tmlut0_waddr_write(pstBeReg, 0);
                    for (j = 0; j < HI_ISP_DRC_TM_NODE_NUM; j++) {
                        isp_drc_tmlut0_wdata_write(pstBeReg, (((HI_U32)pstDynaRegCfg->au16ToneMappingValue0[j]) << 12) | (pstDynaRegCfg->au16ToneMappingDiff0[j]));
                    }
                }

                if (pstDynaRegCfg->u8LutUpdateMode & 0x2) { // 2 or 3
                    isp_drc_tmlut1_waddr_write(pstBeReg, 0);
                    for (j = 0; j < HI_ISP_DRC_TM_NODE_NUM; j++) {
                        isp_drc_tmlut1_wdata_write(pstBeReg, (((HI_U32)pstDynaRegCfg->au16ToneMappingValue1[j]) << 12) | (pstDynaRegCfg->au16ToneMappingDiff1[j]));
                    }
                }
            } else {
                isp_drc_tmlut0_value_write(pstBeReg, pstDynaRegCfg->au16ToneMappingValue0);
                isp_drc_tmlut0_diff_write(pstBeReg, pstDynaRegCfg->au16ToneMappingDiff0);
                isp_drc_tmlut1_value_write(pstBeReg, pstDynaRegCfg->au16ToneMappingValue1);
                isp_drc_tmlut1_diff_write(pstBeReg, pstDynaRegCfg->au16ToneMappingDiff1);
            }

            isp_drc_strength_write(pstBeReg, pstDynaRegCfg->u16Strength);
            isp_drc_lut_mix_ctrl_write(pstBeReg, pstDynaRegCfg->u16LutMixCtrl);

            if (pstDynaRegCfg->bImgSizeChanged) {
                isp_drc_big_x_init_write(pstBeReg, pstDynaRegCfg->u8BigXInit);
                isp_drc_idx_x_init_write(pstBeReg, pstDynaRegCfg->u8IdxXInit);
                isp_drc_cnt_x_init_write(pstBeReg, pstDynaRegCfg->u16CntXInit);
                isp_drc_acc_x_init_write(pstBeReg, pstDynaRegCfg->u16AccXInit);
                isp_drc_blk_wgt_init_write(pstBeReg, pstDynaRegCfg->u16WgtXInit); // drc v4.0
                isp_drc_total_width_write(pstBeReg, pstDynaRegCfg->u16TotalWidth - 1);
                isp_drc_stat_width_write(pstBeReg, pstDynaRegCfg->u16StatWidth - 1);

                isp_drc_hnum_write(pstBeReg, pstDynaRegCfg->u8BlockHNum);
                isp_drc_vnum_write(pstBeReg, pstDynaRegCfg->u8BlockVNum);

                isp_drc_zone_hsize_write(pstBeReg, pstDynaRegCfg->u16BlockHSize - 1);
                isp_drc_zone_vsize_write(pstBeReg, pstDynaRegCfg->u16BlockVSize - 1);
                isp_drc_chk_x_write(pstBeReg, pstDynaRegCfg->u8BlockChkX);
                isp_drc_chk_y_write(pstBeReg, pstDynaRegCfg->u8BlockChkY);

                isp_drc_div_x0_write(pstBeReg, pstDynaRegCfg->u16DivX0);
                isp_drc_div_x1_write(pstBeReg, pstDynaRegCfg->u16DivX1);
                isp_drc_div_y0_write(pstBeReg, pstDynaRegCfg->u16DivY0);
                isp_drc_div_y1_write(pstBeReg, pstDynaRegCfg->u16DivY1);

                isp_drc_bin_scale_write(pstBeReg, pstDynaRegCfg->u8BinScale);

                // DRCS configuration
                isp_drcs_total_width_write(pstFeReg, pstDynaRegCfg->u16DrcsTotalWidth - 1);
                isp_drcs_stat_width_write(pstFeReg, pstDynaRegCfg->u16DrcsStatWidth - 1);

                isp_drcs_big_x_init_write(pstFeReg, pstDynaRegCfg->u8DrcsBigXInit);
                isp_drcs_idx_x_init_write(pstFeReg, pstDynaRegCfg->u8DrcsIdxXInit);
                isp_drcs_cnt_x_init_write(pstFeReg, pstDynaRegCfg->u16DrcsCntXInit);
                isp_drcs_acc_x_init_write(pstFeReg, pstDynaRegCfg->u16DrcsAccXInit);
                isp_drcs_blk_wgt_init_write(pstFeReg, pstDynaRegCfg->u16DrcsWgtXInit); // drc v4.0

                isp_drcs_bin_scale_write(pstFeReg, pstDynaRegCfg->u8BinScale);
                isp_drcs_vnum_write(pstFeReg, pstDynaRegCfg->u8BlockVNum);
                isp_drcs_hnum_write(pstFeReg, pstDynaRegCfg->u8BlockHNum);
                isp_drcs_zone_vsize_write(pstFeReg, pstDynaRegCfg->u16BlockVSize - 1);
                isp_drcs_zone_hsize_write(pstFeReg, pstDynaRegCfg->u16BlockHSize - 1);
                isp_drcs_chk_x_write(pstFeReg, pstDynaRegCfg->u8BlockChkX);
                isp_drcs_chk_y_write(pstFeReg, pstDynaRegCfg->u8BlockChkY);

                pstDynaRegCfg->bImgSizeChanged = bIsOfflineMode;
            }

            pstDynaRegCfg->bDynaResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1DrcCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_DehazeRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bDehazeLutUpdate  = HI_FALSE;
    HI_BOOL  bPreStatLutUpdate = HI_FALSE;
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_U16   j;
    HI_U16   u16BlkNum;
    HI_BOOL  bIsOfflineMode;
    ISP_DEHAZE_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_DEHAZE_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    S_VIPROC_REGS_TYPE      *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE       *pstBeReg        = HI_NULL;
    isp_usr_ctx               *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)\
                      || IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1DehazeCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stDehazeRegCfg.stStaticRegCfg;
        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stDehazeRegCfg.stDynaRegCfg;

        u16BlkNum = ((pstStaticRegCfg->u8Dchnum + 1) * (pstStaticRegCfg->u8Dcvnum + 1) + 1) / 2;
        isp_dehaze_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stDehazeRegCfg.bDehazeEn);

        /* Static Registers */
        if (pstStaticRegCfg->bResh) {
            isp_dehaze_max_mode_write(pstBeReg, pstStaticRegCfg->u8MaxMode);
            isp_dehaze_thld_write(pstBeReg, pstStaticRegCfg->u16DehazeThld);
            isp_dehaze_blthld_write(pstBeReg, pstStaticRegCfg->u16DehazeBlthld);
            isp_dehaze_neg_mode_write(pstBeReg, pstStaticRegCfg->u8DehazeNegMode);
            isp_dehaze_block_sum_write(pstBeReg, pstStaticRegCfg->u16BlockSum);
            isp_dehaze_dc_numh_write(pstBeReg, pstStaticRegCfg->u8Dchnum);
            isp_dehaze_dc_numv_write(pstBeReg, pstStaticRegCfg->u8Dcvnum);
            pstStaticRegCfg->bResh = HI_FALSE;
        }

        /* Dynamic Regs */
        if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) || \
            IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
            if (pstDynaRegCfg->u32LutUpdate) {
                isp_dehaze_lut_waddr_write(pstBeReg, 0);

                for (j = 0; j < 256; j++) {
                    isp_dehaze_lut_wdata_write(pstBeReg, pstDynaRegCfg->u8Lut[j]);
                }
            }

            isp_dehaze_prestat_waddr_write(pstBeReg, 0);

            for (j = 0; j < u16BlkNum; j++) {
                isp_dehaze_prestat_wdata_write(pstBeReg, pstDynaRegCfg->prestat[j]);
            }

            bDehazeLutUpdate = pstDynaRegCfg->u32LutUpdate;
        } else {
            isp_dehaze_dehaze_lut_write(pstBeReg, pstDynaRegCfg->u8Lut);
            isp_dehaze_prestat_write(pstBeReg, pstDynaRegCfg->prestat);
            bDehazeLutUpdate = HI_TRUE;
        }

        bPreStatLutUpdate = pstDynaRegCfg->u32Update;
        isp_dehaze_air_r_write(pstBeReg, pstDynaRegCfg->u16AirR);
        isp_dehaze_air_g_write(pstBeReg, pstDynaRegCfg->u16AirG);
        isp_dehaze_air_b_write(pstBeReg, pstDynaRegCfg->u16AirB);
        isp_dehaze_gstrth_write(pstBeReg, pstDynaRegCfg->u8Strength);

        isp_dehaze_block_sizeh_write(pstBeReg, pstDynaRegCfg->u16Blockhsize);
        isp_dehaze_block_sizev_write(pstBeReg, pstDynaRegCfg->u16Blockvsize);
        isp_dehaze_phase_x_write(pstBeReg, pstDynaRegCfg->u32phasex);
        isp_dehaze_phase_y_write(pstBeReg, pstDynaRegCfg->u32phasey);

        pstRegCfgInfo->cfg_key.bit1DehazeCfg  = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bDehazeLutUpdate        = bDehazeLutUpdate;
    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bDehazePreStatLutUpdate = bPreStatLutUpdate;

    return HI_SUCCESS;
}

static HI_S32 ISP_BayerNrRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_U16   j;
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_BOOL  bIsOfflineMode;
    HI_BOOL  bLutUpdate = HI_FALSE;
    ISP_BAYERNR_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_BAYERNR_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    ISP_BAYERNR_USR_CFG_S    *pstUsrRegCfg    = HI_NULL;
    S_VIPROC_REGS_TYPE       *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE        *pstBeReg        = HI_NULL;
    isp_usr_ctx                *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)\
                      || IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1BayernrCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_bnr_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stBnrRegCfg.bBnrEnable);

        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stBnrRegCfg.stStaticRegCfg;
        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stBnrRegCfg.stDynaRegCfg;
        pstUsrRegCfg    = &pstRegCfgInfo->alg_reg_cfg[i].stBnrRegCfg.stUsrRegCfg;

        if (pstStaticRegCfg->bResh) {       /* satic */
            isp_bnr_de_enable_write(pstBeReg, pstStaticRegCfg->bBnrDetailEnhanceEn);
            isp_bnr_skip_enable_write(pstBeReg, pstStaticRegCfg->bSkipEnable);
            isp_bnr_skip_lev1_enable_write(pstBeReg, pstStaticRegCfg->bSkipLevel1Enable);
            isp_bnr_skip_lev3_enable_write(pstBeReg, pstStaticRegCfg->bSkipLevel3Enable);
            isp_bnr_jnlm_sel_write(pstBeReg, pstStaticRegCfg->u8JnlmSel);
            isp_bnr_de_posclip_write(pstBeReg, pstStaticRegCfg->u8BnrDePosClip);
            isp_bnr_de_negclip_write(pstBeReg, pstStaticRegCfg->u8BnrDeNegClip);
            isp_bnr_de_blcvalue_write(pstBeReg, pstStaticRegCfg->u16BnrDeBlcValue);
            isp_bnr_rlmt_blc_write(pstBeReg, pstStaticRegCfg->u16RLmtBlc);
            isp_bnr_jnlm_maxwtcoef_write(pstBeReg, pstStaticRegCfg->u16JnlmMaxWtCoef);
            isp_bnr_wti_midcoef_write(pstBeReg, pstStaticRegCfg->u8WtiCoefMid);
            isp_bnr_wti_dval_th_write(pstBeReg, pstStaticRegCfg->u8WtiDvalThr);
            isp_bnr_wti_sval_th_write(pstBeReg, pstStaticRegCfg->u8WtiSvalThr);
            isp_bnr_wti_denom_offset_write(pstBeReg, pstStaticRegCfg->s16WtiDenomOffset);
            isp_bnr_wti_maxcoef_write(pstBeReg, pstStaticRegCfg->u16WtiCoefMax);
            pstStaticRegCfg->bResh = HI_FALSE;
        }

        if (pstDynaRegCfg->bResh) {
            isp_bnr_medc_enable_write(pstBeReg, pstDynaRegCfg->bMedcEnable);
            isp_bnr_trisad_enable_write(pstBeReg, pstDynaRegCfg->bTriSadEn);
            isp_bnr_skip_lev2_enable_write(pstBeReg, pstDynaRegCfg->bSkipLevel2Enable);
            isp_bnr_skip_lev4_enable_write(pstBeReg, pstDynaRegCfg->bSkipLevel4Enable);
            isp_bnr_ratio_r_write(pstBeReg, pstDynaRegCfg->au8BnrCRatio[0]);
            isp_bnr_ratio_gr_write(pstBeReg, pstDynaRegCfg->au8BnrCRatio[1]);
            isp_bnr_ratio_gb_write(pstBeReg, pstDynaRegCfg->au8BnrCRatio[2]);
            isp_bnr_ratio_b_write(pstBeReg, pstDynaRegCfg->au8BnrCRatio[3]);
            isp_bnr_amed_mode_r_write(pstBeReg, pstDynaRegCfg->au8AmedMode[0]);
            isp_bnr_amed_mode_gr_write(pstBeReg, pstDynaRegCfg->au8AmedMode[1]);
            isp_bnr_amed_mode_gb_write(pstBeReg, pstDynaRegCfg->au8AmedMode[2]);
            isp_bnr_amed_mode_b_write(pstBeReg, pstDynaRegCfg->au8AmedMode[3]);
            isp_bnr_amed_lev_r_write(pstBeReg, pstDynaRegCfg->au8AmedLevel[0]);
            isp_bnr_amed_lev_gr_write(pstBeReg, pstDynaRegCfg->au8AmedLevel[1]);
            isp_bnr_amed_lev_gb_write(pstBeReg, pstDynaRegCfg->au8AmedLevel[2]);
            isp_bnr_amed_lev_b_write(pstBeReg, pstDynaRegCfg->au8AmedLevel[3]);
            isp_bnr_jnlm_symcoef_write(pstBeReg, pstDynaRegCfg->u8JnlmSymCoef);
            isp_bnr_jnlm_gain_write(pstBeReg, pstDynaRegCfg->u8JnlmGain);
            isp_bnr_lmt_npthresh_write(pstBeReg, pstDynaRegCfg->u16LmtNpThresh);
            isp_bnr_jnlm_coringhig_write(pstBeReg, pstDynaRegCfg->u16JnlmCoringHig);
            isp_bnr_rlmt_rgain_write(pstBeReg, pstDynaRegCfg->u16RLmtRgain);
            isp_bnr_rlmt_bgain_write(pstBeReg, pstDynaRegCfg->u16RLmtBgain);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) || \
                IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_bnr_lmt_odd_waddr_write(pstBeReg, 0);

                for (j = 1; j < HI_ISP_BAYERNR_LMTLUTNUM; j += 2) {
                    isp_bnr_lmt_odd_wdata_write(pstBeReg, pstDynaRegCfg->au8JnlmLimitLut[j]);
                }

                isp_bnr_lmt_even_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_BAYERNR_LMTLUTNUM; j += 2) {
                    isp_bnr_lmt_even_wdata_write(pstBeReg, pstDynaRegCfg->au8JnlmLimitLut[j]);
                }

                isp_bnr_cor_odd_waddr_write(pstBeReg, 0);

                for (j = 1; j < HI_ISP_BAYERNR_LUT_LENGTH; j += 2) {
                    isp_bnr_cor_odd_wdata_write(pstBeReg, pstDynaRegCfg->au16JnlmCoringLowLUT[j]);
                }

                isp_bnr_cor_even_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_BAYERNR_LUT_LENGTH; j += 2) {
                    isp_bnr_cor_even_wdata_write(pstBeReg, pstDynaRegCfg->au16JnlmCoringLowLUT[j]);
                }
            } else {
                isp_bnr_lmt_even_write(pstBeReg, pstDynaRegCfg->au8JnlmLimitLut);
                isp_bnr_lmt_odd_write(pstBeReg, pstDynaRegCfg->au8JnlmLimitLut);
                isp_bnr_cor_even_write(pstBeReg, pstDynaRegCfg->au16JnlmCoringLowLUT);
                isp_bnr_cor_odd_write(pstBeReg, pstDynaRegCfg->au16JnlmCoringLowLUT);
            }

            isp_bnr_jnlmgain_r0_write(pstBeReg, pstDynaRegCfg->au32JnlmLimitMultGain[0][BAYER_RGGB]);
            isp_bnr_jnlmgain_gr0_write(pstBeReg, pstDynaRegCfg->au32JnlmLimitMultGain[0][BAYER_GRBG]);
            isp_bnr_jnlmgain_gb0_write(pstBeReg, pstDynaRegCfg->au32JnlmLimitMultGain[0][BAYER_GBRG]);
            isp_bnr_jnlmgain_b0_write(pstBeReg, pstDynaRegCfg->au32JnlmLimitMultGain[0][BAYER_BGGR]);
            isp_bnr_jnlmgain_r1_write(pstBeReg, pstDynaRegCfg->au32JnlmLimitMultGain[1][BAYER_RGGB]);
            isp_bnr_jnlmgain_gr1_write(pstBeReg, pstDynaRegCfg->au32JnlmLimitMultGain[1][BAYER_GRBG]);
            isp_bnr_jnlmgain_gb1_write(pstBeReg, pstDynaRegCfg->au32JnlmLimitMultGain[1][BAYER_GBRG]);
            isp_bnr_jnlmgain_b1_write(pstBeReg, pstDynaRegCfg->au32JnlmLimitMultGain[1][BAYER_BGGR]);

            bLutUpdate = HI_TRUE;
            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        if (pstUsrRegCfg->bResh) {
            isp_bnr_mono_sensor_write(pstBeReg, pstUsrRegCfg->bBnrMonoSensorEn);
            isp_bnr_lsc_en_write(pstBeReg, pstUsrRegCfg->bBnrLscEn);
            isp_bnr_lsc_nrratio_write(pstBeReg, pstUsrRegCfg->u8BnrLscRatio);

            pstUsrRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1BayernrCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bBnrLutUpdate = bLutUpdate;

    return HI_SUCCESS;
}

static HI_S32 ISP_DgRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_U8  u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_BOOL  bIsOfflineMode;
    ISP_DG_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_DG_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    S_VIPROC_REGS_TYPE  *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE   *pstBeReg        = HI_NULL;
    isp_usr_ctx           *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1DgCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_dg_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stDgRegCfg.bDgEn);

        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stDgRegCfg.stStaticRegCfg;
        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stDgRegCfg.stDynaRegCfg;

        if (pstStaticRegCfg->bResh) {
            isp_dg_rgain_write(pstBeReg, pstDynaRegCfg->u16GainR);
            isp_dg_grgain_write(pstBeReg, pstDynaRegCfg->u16GainGR);
            isp_dg_gbgain_write(pstBeReg, pstDynaRegCfg->u16GainGB);
            isp_dg_bgain_write(pstBeReg, pstDynaRegCfg->u16GainB);
            pstStaticRegCfg->bResh = HI_FALSE;
        }

        if (pstDynaRegCfg->bResh) {
            isp_dg_clip_value_write(pstBeReg, pstDynaRegCfg->u32ClipValue);
            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1DgCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_4DgRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    HI_BOOL  bIsOfflineMode;
    ISP_4DG_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    ISP_4DG_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    isp_usr_ctx            *isp_ctx       = HI_NULL;
    S_VIPROC_REGS_TYPE   *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE    *pstBeReg        = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1WDRDgCfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_4dg_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].st4DgRegCfg.bEnable);

        // static
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].st4DgRegCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bResh) {
            isp_4dg0_rgain_write(pstBeReg, pstStaticRegCfg->u16GainR0);
            isp_4dg0_grgain_write(pstBeReg, pstStaticRegCfg->u16GainGR0);
            isp_4dg0_gbgain_write(pstBeReg, pstStaticRegCfg->u16GainGB0);
            isp_4dg0_bgain_write(pstBeReg, pstStaticRegCfg->u16GainB0);
            isp_4dg1_rgain_write(pstBeReg, pstStaticRegCfg->u16GainR1);
            isp_4dg1_grgain_write(pstBeReg, pstStaticRegCfg->u16GainGR1);
            isp_4dg1_gbgain_write(pstBeReg, pstStaticRegCfg->u16GainGB1);
            isp_4dg1_bgain_write(pstBeReg, pstStaticRegCfg->u16GainB1);
            isp_4dg2_rgain_write(pstBeReg, pstStaticRegCfg->u16GainR2);
            isp_4dg2_grgain_write(pstBeReg, pstStaticRegCfg->u16GainGR2);
            isp_4dg2_gbgain_write(pstBeReg, pstStaticRegCfg->u16GainGB2);
            isp_4dg2_bgain_write(pstBeReg, pstStaticRegCfg->u16GainB2);
            isp_4dg3_rgain_write(pstBeReg, pstStaticRegCfg->u16GainR3);
            isp_4dg3_grgain_write(pstBeReg, pstStaticRegCfg->u16GainGR3);
            isp_4dg3_gbgain_write(pstBeReg, pstStaticRegCfg->u16GainGB3);
            isp_4dg3_bgain_write(pstBeReg, pstStaticRegCfg->u16GainB3);
            pstStaticRegCfg->bResh = HI_FALSE;
        }

        // dynamic
        pstDynaRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].st4DgRegCfg.stDynaRegCfg;

        if (pstDynaRegCfg->bResh) {
            isp_4dg0_clip_value_write(pstBeReg, pstDynaRegCfg->u32ClipValue0);
            isp_4dg1_clip_value_write(pstBeReg, pstDynaRegCfg->u32ClipValue1);
            isp_4dg2_clip_value_write(pstBeReg, pstDynaRegCfg->u32ClipValue2);
            isp_4dg3_clip_value_write(pstBeReg, pstDynaRegCfg->u32ClipValue3);

            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1WDRDgCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_FeRcRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_U16  i;
    VI_PIPE ViPipeBind;
    ISP_RC_USR_CFG_S  *pstUsrRegCfg = HI_NULL;
    S_ISPFE_REGS_TYPE *pstFeReg     = HI_NULL;
    isp_usr_ctx         *isp_ctx    = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    pstUsrRegCfg = &pstRegCfgInfo->alg_reg_cfg[0].stRcRegCfg.stUsrRegCfg;

    if (isp_ctx->wdr_attr.mast_pipe) {
        for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
            ViPipeBind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
            ISP_CHECK_PIPE(ViPipeBind);

            if (pstRegCfgInfo->cfg_key.bit1RcCfg) {
                pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
                ISP_CHECK_POINTER(pstFeReg);

                isp_fe_rc_en_write(pstFeReg, pstRegCfgInfo->alg_reg_cfg[0].stRcRegCfg.bRcEn);

                if (pstUsrRegCfg->bResh) {
                    isp_rc_sqradius_write(pstFeReg, pstUsrRegCfg->u32SquareRadius);
                    isp_rc_cenhor_coor_write(pstFeReg, pstUsrRegCfg->u16CenterHorCoor);
                    isp_rc_cenver_coor_write(pstFeReg, pstUsrRegCfg->u16CenterVerCoor);
                }
            }
        }

        pstUsrRegCfg->bResh            = HI_FALSE;
        pstRegCfgInfo->cfg_key.bit1RcCfg = 0;
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_FlickRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL bIsOfflineMode;
    HI_U8   u8BlkNum = pstRegCfgInfo->cfg_num;
    S_VIPROC_REGS_TYPE       *pstViProcReg    = HI_NULL;
    ISP_FLICKER_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    isp_usr_ctx                *isp_ctx       = HI_NULL;
    S_ISPBE_REGS_TYPE        *pstBeReg        = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1FlickerCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        isp_flicker_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stFlickerRegCfg.bEnable);

        pstDynaRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stFlickerRegCfg.stDynaRegCfg;

        if (pstDynaRegCfg->bResh) {
            isp_flick_mergeframe_write(pstBeReg, pstDynaRegCfg->u8MergeFrm);
            isp_flick_overth_write(pstBeReg, pstDynaRegCfg->u16OverThr);
            isp_flick_gravg_pre_write(pstBeReg, pstDynaRegCfg->s16GrAvgPre);
            isp_flick_gbavg_pre_write(pstBeReg, pstDynaRegCfg->s16GbAvgPre);
            isp_flick_overcountth_write(pstBeReg, pstDynaRegCfg->u32OverCntThr);
            isp_flick_countover_pre_write(pstBeReg, pstDynaRegCfg->u32CntOverPre);
            pstDynaRegCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1FlickerCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_PreGammaRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL bIsOfflineMode;
    HI_BOOL bUsrResh  = HI_FALSE;
    HI_BOOL bIdxResh  = HI_FALSE;
    HI_U8   u8BlkNum  = pstRegCfgInfo->cfg_num;
    HI_U16  j;
    ISP_PREGAMMA_DYNA_CFG_S   *pstDynaRegCfg   = HI_NULL;
    ISP_PREGAMMA_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    S_VIPROC_REGS_TYPE        *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE         *pstBeReg        = HI_NULL;
    isp_usr_ctx                 *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1PreGammaCfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        isp_pregamma_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stPreGammaCfg.bPreGammaEn);

        pstDynaRegCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stPreGammaCfg.stDynaRegCfg;
        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stPreGammaCfg.stStaticRegCfg;

        // Enable PreGamma
        if (pstStaticRegCfg->bStaticResh) {
            isp_pregamma_bitw_out_write(pstBeReg, pstStaticRegCfg->u8BitDepthOut);
            isp_pregamma_bitw_in_write(pstBeReg, pstStaticRegCfg->u8BitDepthIn);

            pstStaticRegCfg->bStaticResh = HI_FALSE;
        }

        bIdxResh = (isp_pregamma_update_index_read(pstBeReg) != pstDynaRegCfg->u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstDynaRegCfg->bPreGammaLutUpdateEn & bIdxResh) : (pstDynaRegCfg->bPreGammaLutUpdateEn);

        // LUT update
        if (bUsrResh) {
            isp_pregamma_update_index_write(pstBeReg, pstDynaRegCfg->u32UpdateIndex);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) || \
                IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_pregamma_lut_waddr_write(pstBeReg, 0);

                for (j = 0; j < PREGAMMA_NODE_NUM; j++) {
                    isp_pregamma_lut_wdata_write(pstBeReg, (pstDynaRegCfg->u32PreGammaLUT[j]));
                }
            } else {
                isp_pregamma_lut_write(pstBeReg, pstDynaRegCfg->u32PreGammaLUT);
            }

            pstDynaRegCfg->bPreGammaLutUpdateEn = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1PreGammaCfg  = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_be_blc_reg_config(VI_PIPE vi_pipe, isp_reg_cfg *reg_cfg_info, hi_u8 i)
{
    hi_bool is_offline_mode;
    hi_u8   blk_num = reg_cfg_info->cfg_num;
    isp_be_blc_cfg    *be_blc_cfg = HI_NULL;
    isp_usr_ctx       *isp_ctx  = HI_NULL;
    S_ISPBE_REGS_TYPE *pstBeReg   = HI_NULL;
    isp_fe_blc_cfg   *fe_blc_cfg = HI_NULL;

    ISP_GET_CTX(vi_pipe, isp_ctx);
    is_offline_mode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                       IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (reg_cfg_info->cfg_key.bit1_be_blc_cfg) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(vi_pipe, i);
        ISP_CHECK_POINTER(pstBeReg);

        be_blc_cfg = &reg_cfg_info->alg_reg_cfg[i].be_blc_cfg;
        fe_blc_cfg = &reg_cfg_info->alg_reg_cfg[0].fe_blc_cfg;
        if (be_blc_cfg->resh_static) {
            /* 4Dg */
            isp_4dg_en_in_write(pstBeReg, be_blc_cfg->wdr_dg_blc[0].static_reg_cfg.blc_in);
            isp_4dg_en_out_write(pstBeReg, be_blc_cfg->wdr_dg_blc[0].static_reg_cfg.blc_out);

            /* WDR */
            isp_wdr_bsaveblc_write(pstBeReg, be_blc_cfg->wdr_blc[0].static_reg_cfg.blc_out);
            /* rlsc */
            isp_rlsc_blcoffsetin_en_write(pstBeReg, be_blc_cfg->rlsc_blc.static_reg_cfg.blc_in);
            isp_rlsc_blcoffsetout_en_write(pstBeReg, be_blc_cfg->rlsc_blc.static_reg_cfg.blc_out);
            /* lsc */
            isp_lsc_blc_in_en_write(pstBeReg, be_blc_cfg->lsc_blc.static_reg_cfg.blc_in);
            isp_lsc_blc_out_en_write(pstBeReg, be_blc_cfg->lsc_blc.static_reg_cfg.blc_out);
            /* Dg */
            isp_dg_en_in_write(pstBeReg, be_blc_cfg->dg_blc.static_reg_cfg.blc_in);
            isp_dg_en_out_write(pstBeReg, be_blc_cfg->dg_blc.static_reg_cfg.blc_out);
            /* AE */
            isp_ae_blc_en_write(pstBeReg, be_blc_cfg->ae_blc.static_reg_cfg.blc_in);
            /* MG */
            isp_la_blc_en_write(pstBeReg, be_blc_cfg->mg_blc.static_reg_cfg.blc_in);
            /* WB */
            isp_wb_en_in_write(pstBeReg, be_blc_cfg->wb_blc.static_reg_cfg.blc_in);
            isp_wb_en_out_write(pstBeReg, be_blc_cfg->wb_blc.static_reg_cfg.blc_out);

            be_blc_cfg->resh_static = HI_FALSE;
        }

        if (is_offline_mode && (fe_blc_cfg->fe_blc_cfg_changed == HI_TRUE)) {
            be_blc_cfg = &reg_cfg_info->alg_reg_cfg[i].be_last_blc_cfg;
        }

        /* 4Dg */
        isp_4dg0_ofsr_write(pstBeReg, be_blc_cfg->wdr_dg_blc[0].usr_reg_cfg.blc[0]);
        isp_4dg0_ofsgr_write(pstBeReg, be_blc_cfg->wdr_dg_blc[0].usr_reg_cfg.blc[1]);
        isp_4dg0_ofsgb_write(pstBeReg, be_blc_cfg->wdr_dg_blc[0].usr_reg_cfg.blc[2]);
        isp_4dg0_ofsb_write(pstBeReg, be_blc_cfg->wdr_dg_blc[0].usr_reg_cfg.blc[3]);

        isp_4dg1_ofsr_write(pstBeReg, be_blc_cfg->wdr_dg_blc[1].usr_reg_cfg.blc[0]);
        isp_4dg1_ofsgr_write(pstBeReg, be_blc_cfg->wdr_dg_blc[1].usr_reg_cfg.blc[1]);
        isp_4dg1_ofsgb_write(pstBeReg, be_blc_cfg->wdr_dg_blc[1].usr_reg_cfg.blc[2]);
        isp_4dg1_ofsb_write(pstBeReg, be_blc_cfg->wdr_dg_blc[1].usr_reg_cfg.blc[3]);

        isp_4dg2_ofsr_write(pstBeReg, be_blc_cfg->wdr_dg_blc[2].usr_reg_cfg.blc[0]);
        isp_4dg2_ofsgr_write(pstBeReg, be_blc_cfg->wdr_dg_blc[2].usr_reg_cfg.blc[1]);
        isp_4dg2_ofsgb_write(pstBeReg, be_blc_cfg->wdr_dg_blc[2].usr_reg_cfg.blc[2]);
        isp_4dg2_ofsb_write(pstBeReg, be_blc_cfg->wdr_dg_blc[2].usr_reg_cfg.blc[3]);

        isp_4dg3_ofsr_write(pstBeReg, be_blc_cfg->wdr_dg_blc[3].usr_reg_cfg.blc[0]);
        isp_4dg3_ofsgr_write(pstBeReg, be_blc_cfg->wdr_dg_blc[3].usr_reg_cfg.blc[1]);
        isp_4dg3_ofsgb_write(pstBeReg, be_blc_cfg->wdr_dg_blc[3].usr_reg_cfg.blc[2]);
        isp_4dg3_ofsb_write(pstBeReg, be_blc_cfg->wdr_dg_blc[3].usr_reg_cfg.blc[3]);
        /* WDR */
        isp_wdr_outblc_write(pstBeReg, be_blc_cfg->wdr_blc[0].usr_reg_cfg.out_blc);
        isp_wdr_f0_inblc_r_write(pstBeReg, be_blc_cfg->wdr_blc[0].usr_reg_cfg.blc[0]);
        isp_wdr_f0_inblc_gr_write(pstBeReg, be_blc_cfg->wdr_blc[0].usr_reg_cfg.blc[1]);
        isp_wdr_f0_inblc_gb_write(pstBeReg, be_blc_cfg->wdr_blc[0].usr_reg_cfg.blc[2]);
        isp_wdr_f0_inblc_b_write(pstBeReg, be_blc_cfg->wdr_blc[0].usr_reg_cfg.blc[3]);

        isp_wdr_f1_inblc_r_write(pstBeReg, be_blc_cfg->wdr_blc[1].usr_reg_cfg.blc[0]);
        isp_wdr_f1_inblc_gr_write(pstBeReg, be_blc_cfg->wdr_blc[1].usr_reg_cfg.blc[1]);
        isp_wdr_f1_inblc_gb_write(pstBeReg, be_blc_cfg->wdr_blc[1].usr_reg_cfg.blc[2]);
        isp_wdr_f1_inblc_b_write(pstBeReg, be_blc_cfg->wdr_blc[1].usr_reg_cfg.blc[3]);

        isp_wdr_f2_inblc_r_write(pstBeReg, be_blc_cfg->wdr_blc[2].usr_reg_cfg.blc[0]);
        isp_wdr_f2_inblc_gr_write(pstBeReg, be_blc_cfg->wdr_blc[2].usr_reg_cfg.blc[1]);
        isp_wdr_f2_inblc_gb_write(pstBeReg, be_blc_cfg->wdr_blc[2].usr_reg_cfg.blc[2]);
        isp_wdr_f2_inblc_b_write(pstBeReg, be_blc_cfg->wdr_blc[2].usr_reg_cfg.blc[3]);

        isp_wdr_f3_inblc_r_write(pstBeReg, be_blc_cfg->wdr_blc[3].usr_reg_cfg.blc[0]);
        isp_wdr_f3_inblc_gr_write(pstBeReg, be_blc_cfg->wdr_blc[3].usr_reg_cfg.blc[1]);
        isp_wdr_f3_inblc_gb_write(pstBeReg, be_blc_cfg->wdr_blc[3].usr_reg_cfg.blc[2]);
        isp_wdr_f3_inblc_b_write(pstBeReg, be_blc_cfg->wdr_blc[3].usr_reg_cfg.blc[3]);

        /* flicker */
        isp_flick_f0_inblc_r_write(pstBeReg, be_blc_cfg->flicker_blc[0].usr_reg_cfg.blc[0]);
        isp_flick_f0_inblc_gr_write(pstBeReg, be_blc_cfg->flicker_blc[0].usr_reg_cfg.blc[1]);
        isp_flick_f0_inblc_gb_write(pstBeReg, be_blc_cfg->flicker_blc[0].usr_reg_cfg.blc[2]);
        isp_flick_f0_inblc_b_write(pstBeReg, be_blc_cfg->flicker_blc[0].usr_reg_cfg.blc[3]);

        isp_flick_f1_inblc_r_write(pstBeReg, be_blc_cfg->flicker_blc[1].usr_reg_cfg.blc[0]);
        isp_flick_f1_inblc_gr_write(pstBeReg, be_blc_cfg->flicker_blc[1].usr_reg_cfg.blc[1]);
        isp_flick_f1_inblc_gb_write(pstBeReg, be_blc_cfg->flicker_blc[1].usr_reg_cfg.blc[2]);
        isp_flick_f1_inblc_b_write(pstBeReg, be_blc_cfg->flicker_blc[1].usr_reg_cfg.blc[3]);

        isp_flick_f2_inblc_r_write(pstBeReg, be_blc_cfg->flicker_blc[2].usr_reg_cfg.blc[0]);
        isp_flick_f2_inblc_gr_write(pstBeReg, be_blc_cfg->flicker_blc[2].usr_reg_cfg.blc[1]);
        isp_flick_f2_inblc_gb_write(pstBeReg, be_blc_cfg->flicker_blc[2].usr_reg_cfg.blc[2]);
        isp_flick_f2_inblc_b_write(pstBeReg, be_blc_cfg->flicker_blc[2].usr_reg_cfg.blc[3]);

        isp_flick_f3_inblc_r_write(pstBeReg, be_blc_cfg->flicker_blc[3].usr_reg_cfg.blc[0]);
        isp_flick_f3_inblc_gr_write(pstBeReg, be_blc_cfg->flicker_blc[3].usr_reg_cfg.blc[1]);
        isp_flick_f3_inblc_gb_write(pstBeReg, be_blc_cfg->flicker_blc[3].usr_reg_cfg.blc[2]);
        isp_flick_f3_inblc_b_write(pstBeReg, be_blc_cfg->flicker_blc[3].usr_reg_cfg.blc[3]);

        /* pregamma */
        isp_pregamma_offset_r_write(pstBeReg, be_blc_cfg->pre_gamma_blc.usr_reg_cfg.blc[0]);
        isp_pregamma_offset_gr_write(pstBeReg, be_blc_cfg->pre_gamma_blc.usr_reg_cfg.blc[1]);
        isp_pregamma_offset_gb_write(pstBeReg, be_blc_cfg->pre_gamma_blc.usr_reg_cfg.blc[2]);
        isp_pregamma_offset_b_write(pstBeReg, be_blc_cfg->pre_gamma_blc.usr_reg_cfg.blc[3]);

        /* rlsc */
        isp_rlsc_blcoffsetr_write(pstBeReg, be_blc_cfg->rlsc_blc.usr_reg_cfg.blc[0]);
        isp_rlsc_blcoffsetgr_write(pstBeReg, be_blc_cfg->rlsc_blc.usr_reg_cfg.blc[1]);
        isp_rlsc_blcoffsetgb_write(pstBeReg, be_blc_cfg->rlsc_blc.usr_reg_cfg.blc[2]);
        isp_rlsc_blcoffsetb_write(pstBeReg, be_blc_cfg->rlsc_blc.usr_reg_cfg.blc[3]);

        /* lsc */
        isp_lsc_blc_r_write(pstBeReg, be_blc_cfg->lsc_blc.usr_reg_cfg.blc[0]);
        isp_lsc_blc_gr_write(pstBeReg, be_blc_cfg->lsc_blc.usr_reg_cfg.blc[1]);
        isp_lsc_blc_gb_write(pstBeReg, be_blc_cfg->lsc_blc.usr_reg_cfg.blc[2]);
        isp_lsc_blc_b_write(pstBeReg, be_blc_cfg->lsc_blc.usr_reg_cfg.blc[3]);

        /* Dg */
        isp_dg_ofsr_write(pstBeReg, be_blc_cfg->dg_blc.usr_reg_cfg.blc[0]);
        isp_dg_ofsgr_write(pstBeReg, be_blc_cfg->dg_blc.usr_reg_cfg.blc[1]);
        isp_dg_ofsgb_write(pstBeReg, be_blc_cfg->dg_blc.usr_reg_cfg.blc[2]);
        isp_dg_ofsb_write(pstBeReg, be_blc_cfg->dg_blc.usr_reg_cfg.blc[3]);

        /* AE */
        isp_ae_offset_r_write(pstBeReg, be_blc_cfg->ae_blc.usr_reg_cfg.blc[0]);
        isp_ae_offset_gr_write(pstBeReg, be_blc_cfg->ae_blc.usr_reg_cfg.blc[1]);
        isp_ae_offset_gb_write(pstBeReg, be_blc_cfg->ae_blc.usr_reg_cfg.blc[2]);
        isp_ae_offset_b_write(pstBeReg, be_blc_cfg->ae_blc.usr_reg_cfg.blc[3]);
        /* MG */
        isp_la_offset_r_write(pstBeReg, be_blc_cfg->mg_blc.usr_reg_cfg.blc[0]);
        isp_la_offset_gr_write(pstBeReg, be_blc_cfg->mg_blc.usr_reg_cfg.blc[1]);
        isp_la_offset_gb_write(pstBeReg, be_blc_cfg->mg_blc.usr_reg_cfg.blc[2]);
        isp_la_offset_b_write(pstBeReg, be_blc_cfg->mg_blc.usr_reg_cfg.blc[3]);
        /* WB */
        isp_wb_ofsr_write(pstBeReg, be_blc_cfg->wb_blc.usr_reg_cfg.blc[0]);
        isp_wb_ofsgr_write(pstBeReg, be_blc_cfg->wb_blc.usr_reg_cfg.blc[1]);
        isp_wb_ofsgb_write(pstBeReg, be_blc_cfg->wb_blc.usr_reg_cfg.blc[2]);
        isp_wb_ofsb_write(pstBeReg, be_blc_cfg->wb_blc.usr_reg_cfg.blc[3]);
        /* split */
        isp_wdrsplit_offset_r_write(pstBeReg, be_blc_cfg->split_blc.usr_reg_cfg.blc[0]);
        isp_wdrsplit_offset_gr_write(pstBeReg, be_blc_cfg->split_blc.usr_reg_cfg.blc[1]);
        isp_wdrsplit_offset_gb_write(pstBeReg, be_blc_cfg->split_blc.usr_reg_cfg.blc[2]);
        isp_wdrsplit_offset_b_write(pstBeReg, be_blc_cfg->split_blc.usr_reg_cfg.blc[3]);
        isp_wdrsplit_blc_write(pstBeReg, be_blc_cfg->split_blc.usr_reg_cfg.out_blc);

        /* LogLUT */
        isp_loglut_offset_r_write(pstBeReg, be_blc_cfg->log_lut_blc.usr_reg_cfg.blc[0]);
        isp_loglut_offset_gr_write(pstBeReg, be_blc_cfg->log_lut_blc.usr_reg_cfg.blc[1]);
        isp_loglut_offset_gb_write(pstBeReg, be_blc_cfg->log_lut_blc.usr_reg_cfg.blc[2]);
        isp_loglut_offset_b_write(pstBeReg, be_blc_cfg->log_lut_blc.usr_reg_cfg.blc[3]);

        memcpy_s(&reg_cfg_info->alg_reg_cfg[i].be_last_blc_cfg, sizeof(isp_be_blc_cfg),
                 &reg_cfg_info->alg_reg_cfg[i].be_blc_cfg, sizeof(isp_be_blc_cfg));

        be_blc_cfg->resh_dyna = is_offline_mode;

        reg_cfg_info->cfg_key.bit1_be_blc_cfg = is_offline_mode ? 1 : ((blk_num <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_WdrSplitRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_U8    j;
    HI_U8    u8BlkNum = pstRegCfgInfo->cfg_num;
    ISP_SPLIT_STATIC_CFG_S *pstStaticRegCfg = HI_NULL;
    S_VIPROC_REGS_TYPE     *pstViProcReg    = HI_NULL;
    S_ISPBE_REGS_TYPE      *pstBeReg        = HI_NULL;
    isp_usr_ctx              *isp_ctx       = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)\
                      || IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));

    if (pstRegCfgInfo->cfg_key.bit1SplitCfg) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);

        isp_wdrsplit1_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stSplitCfg.bEnable);

        pstStaticRegCfg = &pstRegCfgInfo->alg_reg_cfg[i].stSplitCfg.stStaticRegCfg;

        if (pstStaticRegCfg->bResh) {
            isp_wdrsplit_bitw_out_write(pstBeReg, pstStaticRegCfg->u8BitDepthOut);
            isp_wdrsplit_bitw_in_write(pstBeReg, pstStaticRegCfg->u8BitDepthIn);
            isp_wdrsplit_mode_out_write(pstBeReg, pstStaticRegCfg->u8ModeOut);
            isp_wdrsplit_mode_in_write(pstBeReg, pstStaticRegCfg->u8ModeIn);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
                || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_wdrsplit_lut_waddr_write(pstBeReg, 0);

                for (j = 0; j < 129; j++) {
                    isp_wdrsplit_lut_wdata_write(pstBeReg, pstStaticRegCfg->au16WdrSplitLut[j]);
                }
            } else {
                isp_wdrsplit_lut_write(pstBeReg, pstStaticRegCfg->au16WdrSplitLut);
            }

            pstStaticRegCfg->bResh = HI_FALSE;
        }

        pstRegCfgInfo->cfg_key.bit1SplitCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_ClutRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_BOOL  bIsOfflineMode;
    HI_BOOL  bUsrResh   = HI_FALSE;
    HI_BOOL  bIdxResh   = HI_FALSE;
    HI_BOOL  bLutUpdate = HI_FALSE;
    HI_U16   j;
    HI_U8 u8BlkNum = pstRegCfgInfo->cfg_num;
    ISP_CLUT_USR_COEF_CFG_S     *pstClutUsrCoefCfg   = HI_NULL;
    ISP_CLUT_USR_CTRL_CFG_S     *pstClutUsrCtrlCfg   = HI_NULL;
    S_VIPROC_REGS_TYPE          *pstViProcReg        = HI_NULL;
    S_ISPBE_REGS_TYPE           *pstBeReg            = HI_NULL;
    isp_usr_ctx                   *isp_ctx           = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    bIsOfflineMode = (IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode) || \
                      IS_STRIPING_MODE(isp_ctx->block_attr.running_mode));


    if ((pstRegCfgInfo->cfg_key.bit1ClutCfg)) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstBeReg);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_clut_en_write(pstViProcReg, pstRegCfgInfo->alg_reg_cfg[i].stClutCfg.bEnable);
        isp_clut_sel_write(pstBeReg, HI_ISP_CLUT_SEL_WRITE);
        pstClutUsrCoefCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stClutCfg.stUsrRegCfg.stClutUsrCoefCfg;

        bIdxResh = (isp_clut_update_index_read(pstBeReg) != pstClutUsrCoefCfg->u32UpdateIndex);
        bUsrResh = (bIsOfflineMode) ? (pstClutUsrCoefCfg->bResh & bIdxResh) : (pstClutUsrCoefCfg->bResh);

        if (bUsrResh) {
            isp_clut_update_index_write(pstBeReg, pstClutUsrCoefCfg->u32UpdateIndex);

            if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode)\
                || IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
                isp_clut_lut0_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_CLUT_LUT0; j++) {
                    isp_clut_lut0_wdata_write(pstBeReg, pstClutUsrCoefCfg->au32lut0[j]);
                }

                isp_clut_lut1_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_CLUT_LUT1; j++) {
                    isp_clut_lut1_wdata_write(pstBeReg, pstClutUsrCoefCfg->au32lut1[j]);
                }

                isp_clut_lut2_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_CLUT_LUT2; j++) {
                    isp_clut_lut2_wdata_write(pstBeReg, pstClutUsrCoefCfg->au32lut2[j]);
                }

                isp_clut_lut3_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_CLUT_LUT3; j++) {
                    isp_clut_lut3_wdata_write(pstBeReg, pstClutUsrCoefCfg->au32lut3[j]);
                }

                isp_clut_lut4_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_CLUT_LUT4; j++) {
                    isp_clut_lut4_wdata_write(pstBeReg, pstClutUsrCoefCfg->au32lut4[j]);
                }

                isp_clut_lut5_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_CLUT_LUT5; j++) {
                    isp_clut_lut5_wdata_write(pstBeReg, pstClutUsrCoefCfg->au32lut5[j]);
                }

                isp_clut_lut6_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_CLUT_LUT6; j++) {
                    isp_clut_lut6_wdata_write(pstBeReg, pstClutUsrCoefCfg->au32lut6[j]);
                }

                isp_clut_lut7_waddr_write(pstBeReg, 0);

                for (j = 0; j < HI_ISP_CLUT_LUT7; j++) {
                    isp_clut_lut7_wdata_write(pstBeReg, pstClutUsrCoefCfg->au32lut7[j]);
                }
            } else {
                isp_clut_lut0_write(pstBeReg, pstClutUsrCoefCfg->au32lut0);
                isp_clut_lut1_write(pstBeReg, pstClutUsrCoefCfg->au32lut1);
                isp_clut_lut2_write(pstBeReg, pstClutUsrCoefCfg->au32lut2);
                isp_clut_lut3_write(pstBeReg, pstClutUsrCoefCfg->au32lut3);
                isp_clut_lut4_write(pstBeReg, pstClutUsrCoefCfg->au32lut4);
                isp_clut_lut5_write(pstBeReg, pstClutUsrCoefCfg->au32lut5);
                isp_clut_lut6_write(pstBeReg, pstClutUsrCoefCfg->au32lut6);
                isp_clut_lut7_write(pstBeReg, pstClutUsrCoefCfg->au32lut7);
            }

            bLutUpdate = HI_TRUE;
            pstClutUsrCoefCfg->bResh = bIsOfflineMode;
        }

        pstClutUsrCtrlCfg   = &pstRegCfgInfo->alg_reg_cfg[i].stClutCfg.stUsrRegCfg.stClutUsrCtrlCfg;

        if (pstClutUsrCtrlCfg->bResh) {
            isp_clut_gain2_write(pstBeReg, pstClutUsrCtrlCfg->u32GainB);
            isp_clut_gain1_write(pstBeReg, pstClutUsrCtrlCfg->u32GainG);
            isp_clut_gain0_write(pstBeReg, pstClutUsrCtrlCfg->u32GainR);
            pstClutUsrCtrlCfg->bResh = bIsOfflineMode;
        }

        pstRegCfgInfo->cfg_key.bit1ClutCfg = bIsOfflineMode ? 1 : ((u8BlkNum <= (i + 1)) ? 0 : 1);
    }

    pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg.bClutLutUpdate = bLutUpdate;

    return HI_SUCCESS;
}

static HI_S32 ISP_FeUpdateRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_U32  i;
    VI_PIPE ViPipeBind;
    isp_usr_ctx         *isp_ctx = HI_NULL;
    S_ISPFE_REGS_TYPE *pstFeReg  = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
        ViPipeBind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
        ISP_CHECK_PIPE(ViPipeBind);
        pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
        ISP_CHECK_POINTER(pstFeReg);

        isp_fe_update_mode_write(pstFeReg, HI_FALSE);
        isp_fe_update_write(pstFeReg, HI_TRUE);

        if (pstRegCfgInfo->alg_reg_cfg[0].stFeLutUpdateCfg.bLsc1LutUpdate) {
            isp_lsc1_lut_update_write(pstFeReg, pstRegCfgInfo->alg_reg_cfg[0].stFeLutUpdateCfg.bLsc1LutUpdate);
        }

        if (pstRegCfgInfo->alg_reg_cfg[0].stFeLutUpdateCfg.bAe1LutUpdate) {
            isp_ae1_lut_update_write(pstFeReg, pstRegCfgInfo->alg_reg_cfg[0].stFeLutUpdateCfg.bAe1LutUpdate);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_FeSystemRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_BOOL bIspCropEn = HI_FALSE;
    HI_U8   u8RggbCfg;
    HI_U32  i;
    HI_S32  s32X, s32Y;
    HI_U32  u32Width, u32Height;
    HI_U32  u32PipeW, u32PipeH;
    VI_PIPE ViPipeBind, ViPipeId;
    isp_usr_ctx         *isp_ctx = HI_NULL;
    S_ISPFE_REGS_TYPE *pstFeReg  = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    ViPipeId  = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[0];
    u8RggbCfg = hi_ext_system_rggb_cfg_read(ViPipeId);

    s32X      = isp_ctx->sys_rect.x;
    s32Y      = isp_ctx->sys_rect.y;
    u32Width  = isp_ctx->sys_rect.width;
    u32Height = isp_ctx->sys_rect.height;
    u32PipeW  = isp_ctx->pipe_size.width;
    u32PipeH  = isp_ctx->pipe_size.height;

    /* ISP crop low-power process */
    if ((s32X == 0) &&
        (s32Y == 0) &&
        (u32Width  == u32PipeW) &&
        (u32Height == u32PipeH)) {
        bIspCropEn = HI_FALSE;
    } else {
        bIspCropEn = HI_TRUE;
    }

    for (i = 0; i < isp_ctx->wdr_attr.dev_bind_pipe.num; i++) {
        ViPipeBind = isp_ctx->wdr_attr.dev_bind_pipe.pipe_id[i];
        ISP_CHECK_PIPE(ViPipeBind);
        pstFeReg = (S_ISPFE_REGS_TYPE *)ISP_GetFeVirAddr(ViPipeBind);
        ISP_CHECK_POINTER(pstFeReg);

        isp_fe_crop_en_write(pstFeReg, bIspCropEn);
        isp_crop_pos_x_write(pstFeReg, s32X);
        isp_crop_pos_y_write(pstFeReg, s32Y);
        isp_crop_width_out_write(pstFeReg, u32Width - 1);
        isp_crop_height_out_write(pstFeReg, u32Height - 1);
        isp_fe_rggb_cfg_write(pstFeReg, u8RggbCfg);
        isp_fe_fix_timing_write(pstFeReg, HI_ISP_FE_FIX_TIMING_STAT);
        isp_fe_width_write(pstFeReg, u32PipeW  - 1);
        isp_fe_height_write(pstFeReg, u32PipeH - 1);
        isp_fe_blk_width_write(pstFeReg, u32PipeW  - 1);
        isp_fe_blk_height_write(pstFeReg, u32PipeH - 1);
        isp_fe_blk_f_hblank_write(pstFeReg, 0);
        isp_fe_hsync_mode_write(pstFeReg, 0);
        isp_fe_vsync_mode_write(pstFeReg, 0);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_SystemRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    HI_U32  u32RggbCfg;
    isp_usr_ctx *isp_ctx = HI_NULL;
    S_VIPROC_REGS_TYPE *pstViProcReg = HI_NULL;
    S_ISPBE_REGS_TYPE  *pstBeReg     = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    u32RggbCfg = hi_ext_system_rggb_cfg_read(ViPipe);

    pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
    pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
    ISP_CHECK_POINTER(pstBeReg);
    ISP_CHECK_POINTER(pstViProcReg);
    isp_blk_f_hblank_write(pstBeReg, HI_ISP_BLK_F_HBLANK_DEFAULT);
    isp_blk_f_vblank_write(pstBeReg, HI_ISP_BLK_F_VBLANK_DEFAULT);
    isp_blk_b_hblank_write(pstBeReg, HI_ISP_BLK_B_HBLANK_DEFAULT);
    isp_blk_b_vblank_write(pstBeReg, HI_ISP_BLK_B_VBLANK_DEFAULT);
    isp_clip_y_min_write(pstBeReg, ISP_CLIP_Y_MIN_DEFAULT);
    isp_clip_y_max_write(pstBeReg, ISP_CLIP_Y_MAX_DEFAULT);
    isp_clip_c_min_write(pstBeReg, ISP_CLIP_C_MIN_DEFAULT);
    isp_clip_c_max_write(pstBeReg, ISP_CLIP_C_MAX_DEFAULT);
    isp_csc_sum_en_write(pstBeReg, ISP_CSC_SUM_EN_DEFAULT);
    isp_yuv444_sum_en_write(pstBeReg, ISP_YUV444_SUM_EN_DEFAULT);
    isp_yuv422_sum_en_write(pstBeReg, ISP_YUV422_SUM_EN_DEFAULT);
    isp_wdr_sum_en_write(pstBeReg, ISP_WDR_SUM_EN_DEFAULT);
    isp_demosaic_sum_en_write(pstBeReg, ISP_DEMOSAIC_SUM_EN_DEFAULT);

    isp_be_rggb_cfg_write(pstViProcReg, u32RggbCfg);

    if ((IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)) ||
        (IS_STRIPING_MODE(isp_ctx->block_attr.running_mode))) {
        isp_stt_en_write(pstBeReg, HI_TRUE);
    } else {
        isp_stt_en_write(pstBeReg, HI_FALSE);
    }

    if (isp_ctx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR) {
        isp_sqrt_en_write(pstViProcReg, HI_TRUE); // sqrt in raw
        isp_sq_en_write(pstViProcReg, HI_TRUE); // sq in rgb
        isp_sqrt1_en_write(pstViProcReg, HI_TRUE); // sqrt in rgb
    } else {
        isp_sqrt_en_write(pstViProcReg, HI_FALSE); // sqrt in raw
        isp_sq_en_write(pstViProcReg, HI_FALSE); // sq in rgb
        isp_sqrt1_en_write(pstViProcReg, HI_FALSE); // sqrt in rgb
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_DitherRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    isp_usr_ctx         *isp_ctx = HI_NULL;
    S_ISPBE_REGS_TYPE *pstBeReg  = HI_NULL;
    ISP_GET_CTX(ViPipe, isp_ctx);
    pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
    ISP_CHECK_POINTER(pstBeReg);

    /* after drc module */
    isp_drc_dither_out_bits_write(pstBeReg, HI_ISP_DRC_DITHER_OUT_BITS_DEFAULT);
    isp_drc_dither_round_write(pstBeReg, HI_ISP_DRC_DITHER_ROUND_DEFAULT);
    isp_drc_dither_spatial_mode_write(pstBeReg, HI_ISP_DRC_DITHER_SPATIAL_MODE_DEFAULT);

    /* after gamma module */
    isp_dmnr_dither_en_write(pstBeReg, HI_TRUE);
    isp_dmnr_dither_out_bits_write(pstBeReg, HI_ISP_DMNR_DITHER_OUT_BITS_DEFAULT);
    isp_dmnr_dither_round_write(pstBeReg, HI_ISP_DMNR_DITHER_ROUND_DEFAULT);
    isp_dmnr_dither_spatial_mode_write(pstBeReg, HI_ISP_DMNR_DITHER_SPATIAL_MODE_DEFAULT);

    /* after sqrt1 module */
    if (isp_ctx->hdr_attr.dynamic_range == DYNAMIC_RANGE_XDR) {
        isp_sqrt1_dither_en_write(pstBeReg, HI_FALSE);
    } else {
        isp_sqrt1_dither_en_write(pstBeReg, HI_TRUE);
        isp_sqrt1_dither_out_bits_write(pstBeReg, HI_ISP_SQRT1_DITHER_OUT_BITS_DEFAULT);
        isp_sqrt1_dither_round_write(pstBeReg, HI_ISP_SQRT1_DITHER_ROUND_DEFAULT);
        isp_sqrt1_dither_spatial_mode_write(pstBeReg, HI_ISP_SQRT1_DITHER_SPATIAL_MODE_DEFAULT);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_BeAlgLutUpdateRegConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i)
{
    ISP_BE_LUT_UPDATE_REG_CFG_S  *pstBeLutUpdateCfg = &pstRegCfgInfo->alg_reg_cfg[i].stBeLutUpdateCfg;
    S_ISPBE_REGS_TYPE            *pstBeReg          = HI_NULL;

    pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
    ISP_CHECK_POINTER(pstBeReg);

    if (pstBeLutUpdateCfg->bAeLutUpdate) {
        isp_ae_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bAeLutUpdate);
    }

    if (pstBeLutUpdateCfg->bSharpenLutUpdate) {
        isp_sharpen_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bSharpenLutUpdate);
    }

    if (pstBeLutUpdateCfg->bNddmUsmLutUpdate) {
        isp_nddm_usm_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bNddmUsmLutUpdate);
    }

    if (pstBeLutUpdateCfg->bNddmGfLutUpdate) {
        isp_nddm_gf_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bNddmGfLutUpdate);
    }

    if (pstBeLutUpdateCfg->bLdciDrcLutUpdate) {
        isp_ldci_drc_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bLdciDrcLutUpdate);
    }

    if (pstBeLutUpdateCfg->bLdciCalcLutUpdate) {
        isp_ldci_calc_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bLdciCalcLutUpdate);
    }

    if (pstBeLutUpdateCfg->bDpcLutUpdate) {
        isp_dpc_ex_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bDpcLutUpdate);
    }

    if (pstBeLutUpdateCfg->bLscLutUpdate) {
        isp_lsc_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bLscLutUpdate);
    }

    if (pstBeLutUpdateCfg->bBnrLscLutUpdate) {
        isp_bnr_lsc_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bBnrLscLutUpdate);
    }

    if (pstBeLutUpdateCfg->bRlscLutUpdate) {
        isp_rlsc_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bRlscLutUpdate);
    }

    if (pstBeLutUpdateCfg->bGammaLutUpdate) {
        isp_gamma_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bGammaLutUpdate);
    }

    if (pstBeLutUpdateCfg->bCaLutUpdate) {
        isp_ca_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bCaLutUpdate);
    }

    if (pstBeLutUpdateCfg->bWdrLutUpdate) {
        isp_wdr_129x8_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bWdrLutUpdate);
    }

    if (pstBeLutUpdateCfg->bDehazeLutUpdate) {
        isp_dehaze_str_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bDehazeLutUpdate);
    }
    if (pstBeLutUpdateCfg->bDehazePreStatLutUpdate) {
        isp_dehaze_pre_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bDehazePreStatLutUpdate);
    }
    if (pstBeLutUpdateCfg->bBnrLutUpdate) {
        isp_bnr_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bBnrLutUpdate);
    }
    if (pstBeLutUpdateCfg->bClutLutUpdate) {
        isp_clut_lut_update_write(pstBeReg, pstBeLutUpdateCfg->bClutLutUpdate);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_BeReshCfg(isp_alg_reg_cfg *alg_reg_cfg)
{
    alg_reg_cfg->stAwbRegCfg.stAwbRegStaCfg.bBEAwbStaCfg     = HI_TRUE;
    alg_reg_cfg->stAwbRegCfg.stAwbRegUsrCfg.bResh            = HI_TRUE;

    alg_reg_cfg->stDemRegCfg.stStaticRegCfg.bResh            = HI_TRUE;
    alg_reg_cfg->stDemRegCfg.stDynaRegCfg.bResh              = HI_TRUE;

    alg_reg_cfg->stLdciRegCfg.stStaticRegCfg.bStaticResh     = HI_TRUE;

    alg_reg_cfg->stLCacRegCfg.stStaticRegCfg.bStaticResh     = HI_TRUE;
    alg_reg_cfg->stLCacRegCfg.stUsrRegCfg.bResh              = HI_TRUE;
    alg_reg_cfg->stLCacRegCfg.stDynaRegCfg.bResh             = HI_TRUE;
    alg_reg_cfg->stGCacRegCfg.stStaticRegCfg.bStaticResh     = HI_TRUE;
    alg_reg_cfg->stGCacRegCfg.stUsrRegCfg.bResh              = HI_TRUE;

    alg_reg_cfg->stAntiFalseColorRegCfg.stStaticRegCfg.bResh = HI_TRUE;
    alg_reg_cfg->stAntiFalseColorRegCfg.stDynaRegCfg.bResh   = HI_TRUE;

    alg_reg_cfg->stDpRegCfg.stStaticRegCfg.bStaticResh           = HI_TRUE;
    alg_reg_cfg->stDpRegCfg.stUsrRegCfg.stUsrDynaCorRegCfg.bResh = HI_TRUE;
    alg_reg_cfg->stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.bResh  = HI_TRUE;
    alg_reg_cfg->stDpRegCfg.stDynaRegCfg.bResh                   = HI_TRUE;

    alg_reg_cfg->stGeRegCfg.stStaticRegCfg.bStaticResh       = HI_TRUE;
    alg_reg_cfg->stGeRegCfg.stUsrRegCfg.bResh                = HI_TRUE;
    alg_reg_cfg->stGeRegCfg.stDynaRegCfg.bResh               = HI_TRUE;

    alg_reg_cfg->stLscRegCfg.stStaticRegCfg.bStaticResh      = HI_TRUE;
    alg_reg_cfg->stLscRegCfg.stUsrRegCfg.bCoefUpdate         = HI_TRUE;
    alg_reg_cfg->stLscRegCfg.stUsrRegCfg.bLutUpdate          = HI_TRUE;

    alg_reg_cfg->stRLscRegCfg.stStaticRegCfg.bStaticResh     = HI_TRUE;
    alg_reg_cfg->stRLscRegCfg.stUsrRegCfg.bCoefUpdate        = HI_TRUE;
    alg_reg_cfg->stRLscRegCfg.stUsrRegCfg.bLutUpdate         = HI_TRUE;

    alg_reg_cfg->stGammaCfg.stUsrRegCfg.bGammaLutUpdateEn    = HI_TRUE;
    alg_reg_cfg->stCscCfg.stDynaRegCfg.bResh                 = HI_TRUE;

    alg_reg_cfg->stCaRegCfg.stStaticRegCfg.bStaticResh       = HI_TRUE;
    alg_reg_cfg->stCaRegCfg.stDynaRegCfg.bResh               = HI_TRUE;
    alg_reg_cfg->stCaRegCfg.stUsrRegCfg.bResh                = HI_TRUE;
    alg_reg_cfg->stCaRegCfg.stUsrRegCfg.bCaLutUpdateEn       = HI_TRUE;

    alg_reg_cfg->stMcdsRegCfg.stStaticRegCfg.bStaticResh     = HI_TRUE;
    alg_reg_cfg->stMcdsRegCfg.stDynaRegCfg.bDynaResh         = HI_TRUE;

    alg_reg_cfg->stWdrRegCfg.stStaticRegCfg.bResh            = HI_TRUE;
    alg_reg_cfg->stWdrRegCfg.stUsrRegCfg.bResh               = HI_TRUE;
    alg_reg_cfg->stWdrRegCfg.stDynaRegCfg.bResh              = HI_TRUE;

    alg_reg_cfg->stDrcRegCfg.stStaticRegCfg.bStaticResh      = HI_TRUE;
    alg_reg_cfg->stDrcRegCfg.stUsrRegCfg.bUsrResh            = HI_TRUE;
    alg_reg_cfg->stDrcRegCfg.stDynaRegCfg.bDynaResh          = HI_TRUE;

    alg_reg_cfg->stDehazeRegCfg.stStaticRegCfg.bResh         = HI_TRUE;
    alg_reg_cfg->stDehazeRegCfg.stDynaRegCfg.u32LutUpdate    = 1;

    alg_reg_cfg->stBnrRegCfg.stStaticRegCfg.bResh            = HI_TRUE;
    alg_reg_cfg->stBnrRegCfg.stDynaRegCfg.bResh              = HI_TRUE;
    alg_reg_cfg->stBnrRegCfg.stUsrRegCfg.bResh               = HI_TRUE;

    alg_reg_cfg->st4DgRegCfg.stStaticRegCfg.bResh            = HI_TRUE;
    alg_reg_cfg->st4DgRegCfg.stDynaRegCfg.bResh              = HI_TRUE;
    alg_reg_cfg->stDgRegCfg.stStaticRegCfg.bResh             = HI_TRUE;
    alg_reg_cfg->stDgRegCfg.stDynaRegCfg.bResh               = HI_TRUE;

    alg_reg_cfg->stPreGammaCfg.stStaticRegCfg.bStaticResh        = HI_TRUE;
    alg_reg_cfg->stPreGammaCfg.stDynaRegCfg.bPreGammaLutUpdateEn = HI_TRUE;
    alg_reg_cfg->stFlickerRegCfg.stDynaRegCfg.bResh              = HI_TRUE;
    alg_reg_cfg->stLogLUTRegCfg.stStaticRegCfg.bStaticResh       = HI_TRUE;

    alg_reg_cfg->be_blc_cfg.resh_static                       = HI_TRUE;
    alg_reg_cfg->be_blc_cfg.resh_dyna                         = HI_TRUE;

    alg_reg_cfg->stClutCfg.stUsrRegCfg.stClutUsrCtrlCfg.bResh = HI_TRUE;
    alg_reg_cfg->stClutCfg.stUsrRegCfg.stClutUsrCoefCfg.bResh = HI_TRUE;

    alg_reg_cfg->stSplitCfg.stStaticRegCfg.bResh              = HI_TRUE;

    alg_reg_cfg->stSharpenRegCfg.stStaticRegCfg.bStaticResh             = HI_TRUE;
    alg_reg_cfg->stSharpenRegCfg.stDynaRegCfg.stDefaultDynaRegCfg.bResh = HI_TRUE;
    alg_reg_cfg->stSharpenRegCfg.stDynaRegCfg.stMpiDynaRegCfg.bResh     = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 ISP_AlgEnExit(VI_PIPE ViPipe)
{
    HI_U8 i;
    S_ISPBE_REGS_TYPE *pstBeReg        = HI_NULL;
    S_VIPROC_REGS_TYPE *pstViProcReg = HI_NULL;
    isp_usr_ctx *isp_ctx   = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    ISP_CHECK_POINTER(isp_ctx);

    if (isp_ctx->para_rec.init == HI_FALSE) {
        return HI_SUCCESS;
    }

    for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
        pstBeReg     = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstViProcReg);
        ISP_CHECK_POINTER(pstBeReg);

        isp_ldci_en_write(pstViProcReg, HI_FALSE);
        isp_ldci_wrstat_en_write(pstBeReg, HI_FALSE);
        isp_ldci_rdstat_en_write(pstBeReg, HI_FALSE);

        isp_be_manual_reg_up_write(pstViProcReg, HI_TRUE);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_FeRegsConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    isp_usr_ctx *isp_ctx   = HI_NULL;

    ISP_CHECK_VIR_PIPE(ViPipe);

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->wdr_attr.mast_pipe) {
        if (ViPipe == HRS_PIPE) {
            ISP_FeHrsRegConfig(HRS_PIPE, pstRegCfgInfo); /* Hrs */
        }

        /* FE alg cfgs setting to register */
        ISP_FeAeRegConfig(ViPipe, pstRegCfgInfo);  /* Ae */
        ISP_FeAwbRegConfig(ViPipe, pstRegCfgInfo); /* awb */
        isp_fe_af_reg_config(ViPipe, pstRegCfgInfo);  /* Af */
        ISP_FeLscRegConfig(ViPipe, pstRegCfgInfo); /* LSC */
        ISP_FeDgRegConfig(ViPipe, pstRegCfgInfo);  /* DG */
        ISP_FeRcRegConfig(ViPipe, pstRegCfgInfo);  /* Rc */
        isp_fe_blc_reg_config(ViPipe, pstRegCfgInfo);
        ISP_FeLogLUTRegConfig(ViPipe, pstRegCfgInfo);

        ISP_FeSystemRegConfig(ViPipe, pstRegCfgInfo);

        ISP_FeUpdateRegConfig(ViPipe, pstRegCfgInfo);
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_BeSyncParamInit(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_U8  j, k;
    HI_S32 s32Ret;
    isp_be_sync_para   stBeSyncParam = {0};
    isp_alg_reg_cfg  *alg_reg_cfg = &pstRegCfgInfo->alg_reg_cfg[0];
    isp_usr_ctx          *isp_ctx    = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) ||
        IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
        return HI_SUCCESS;
    }

    /* DG */
    stBeSyncParam.isp_dgain[0] = alg_reg_cfg->stDgRegCfg.stDynaRegCfg.u16GainR;
    stBeSyncParam.isp_dgain[1] = alg_reg_cfg->stDgRegCfg.stDynaRegCfg.u16GainGR;
    stBeSyncParam.isp_dgain[2] = alg_reg_cfg->stDgRegCfg.stDynaRegCfg.u16GainGB;
    stBeSyncParam.isp_dgain[3] = alg_reg_cfg->stDgRegCfg.stDynaRegCfg.u16GainB;

    /* LDCI */
    stBeSyncParam.ldci_comp = 0x1000;

    /* DRC */
    stBeSyncParam.drc_shp_log       = alg_reg_cfg->stDrcRegCfg.stStaticRegCfg.u8ShpLog;
    stBeSyncParam.drc_div_denom_log = alg_reg_cfg->stDrcRegCfg.stStaticRegCfg.u32DivDenomLog;
    stBeSyncParam.drc_denom_exp     = alg_reg_cfg->stDrcRegCfg.stStaticRegCfg.u32DenomExp;

    for (j = 0; j < SYNC_DRC_PRELUMA_NUM; j++) {
        stBeSyncParam.drc_prev_luma[j] = alg_reg_cfg->stDrcRegCfg.stStaticRegCfg.au32PrevLuma[j];
    }

    /* WDR */
    for (j = 0; j < SYNC_EXP_RATIO_NUM; j++) {
        stBeSyncParam.wdr_exp_ratio[j]   = alg_reg_cfg->stWdrRegCfg.stStaticRegCfg.au16ExpoRRatio[j];
        stBeSyncParam.flick_exp_ratio[j] = alg_reg_cfg->stWdrRegCfg.stStaticRegCfg.au16ExpoValue[j];
    }

    for (j = 0; j < SYNC_WDR_BLC_COMP_NUM; j++) {
        stBeSyncParam.wdr_blc_comp[j] = alg_reg_cfg->stWdrRegCfg.stStaticRegCfg.au32BlcComp[j];
    }

    for (j = 0; j < SYNC_WDR_EXP_VAL_NUM; j++) {
        stBeSyncParam.wdr_exp_val[j] = alg_reg_cfg->stWdrRegCfg.stStaticRegCfg.au16ExpoValue[j];

        for (k = 0; k < ISP_BAYER_CHN_NUM; k++) {
            stBeSyncParam.wdr_gain[j][k]   = 0x100;
        }
    }

    stBeSyncParam.wdr_mdt_en    = alg_reg_cfg->stWdrRegCfg.stDynaRegCfg.bWDRMdtEn;
    stBeSyncParam.fusion_mode   = alg_reg_cfg->stWdrRegCfg.stUsrRegCfg.bFusionMode;
    stBeSyncParam.short_thr     = alg_reg_cfg->stWdrRegCfg.stDynaRegCfg.u16ShortThr;
    stBeSyncParam.long_thr      = alg_reg_cfg->stWdrRegCfg.stDynaRegCfg.u16LongThr;
    stBeSyncParam.wdr_max_ratio = alg_reg_cfg->stWdrRegCfg.stStaticRegCfg.u32MaxRatio;

    /* AWB */
    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        stBeSyncParam.wdr_wb_gain[j] = alg_reg_cfg->stAwbRegCfg.stAwbRegDynCfg.au32BEWhiteBalanceGain[j];
    }

    for (j = 0; j < CCM_MATRIX_SIZE; j++) {
        stBeSyncParam.ccm[j] = alg_reg_cfg->stAwbRegCfg.stAwbRegDynCfg.au16BEColorMatrix[j];
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_SYNC_PARAM_INIT, &stBeSyncParam);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Init BE Sync Param Failed with ec %#x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}


static HI_S32 ISP_BeRegsConfig(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_U32  i;
    HI_S32 s32Ret = 0;
    S_VIPROC_REGS_TYPE *pstViProcReg = HI_NULL;
    isp_usr_ctx          *isp_ctx    = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    ISP_SetBeRaw(ViPipe);
    for (i = 0; i < pstRegCfgInfo->cfg_num; i++) {
        ISP_SystemRegConfig(ViPipe, pstRegCfgInfo, i); /* sys */
        ISP_DitherRegConfig(ViPipe, pstRegCfgInfo, i); /* dither */
        /* Be alg cfgs setting to register */
        ISP_AeRegConfig(ViPipe, pstRegCfgInfo, i);   /* ae */
        ISP_AwbRegConfig(ViPipe, pstRegCfgInfo, i);  /* awb */
        isp_af_reg_config(ViPipe, pstRegCfgInfo, i);  /* AF */
        ISP_SharpenRegConfig(ViPipe, pstRegCfgInfo, i); /* sharpen */
        ISP_EdgeMarkRegConfig(ViPipe, pstRegCfgInfo, i); /* sharpen */
        ISP_DemRegConfig(ViPipe, pstRegCfgInfo, i);  /* demosaic */
        ISP_FpnRegConfig(ViPipe, pstRegCfgInfo, i);  /* FPN */
        ISP_LdciRegConfig(ViPipe, pstRegCfgInfo, i); /* ldci */
        ISP_LcacRegConfig(ViPipe, pstRegCfgInfo, i); /* Local cac */
        ISP_GcacRegConfig(ViPipe, pstRegCfgInfo, i); /* global cac */
        ISP_FcrRegConfig(ViPipe, pstRegCfgInfo, i);  /* FCR */
        ISP_DpcRegConfig(ViPipe, pstRegCfgInfo, i);  /* dpc */
        ISP_GeRegConfig(ViPipe, pstRegCfgInfo, i);   /* ge */
        ISP_LscRegConfig(ViPipe, pstRegCfgInfo, i);  /* BE LSC */
        ISP_RLscRegConfig(ViPipe, pstRegCfgInfo, i);  /* Radial LSC */
        ISP_GammaRegConfig(ViPipe, pstRegCfgInfo, i); /* gamma */
        ISP_CscRegConfig(ViPipe, pstRegCfgInfo, i);  /* csc */
        ISP_CaRegConfig(ViPipe, pstRegCfgInfo, i);   /* ca */
        ISP_McdsRegConfig(ViPipe, pstRegCfgInfo, i); /* mcds */
        ISP_WdrRegConfig(ViPipe, pstRegCfgInfo, i);  /* wdr */
        ISP_DrcRegConfig(ViPipe, pstRegCfgInfo, i);  /* drc */
        ISP_DehazeRegConfig(ViPipe, pstRegCfgInfo, i);  /* Dehaze */
        ISP_BayerNrRegConfig(ViPipe, pstRegCfgInfo, i); /* BayerNR */
        ISP_DgRegConfig(ViPipe, pstRegCfgInfo, i);   /* DG */
        ISP_4DgRegConfig(ViPipe, pstRegCfgInfo, i);   /* 4DG */
        ISP_PreGammaRegConfig(ViPipe, pstRegCfgInfo, i); /* PreGamma */
        ISP_FlickRegConfig(ViPipe, pstRegCfgInfo, i); /* Flicker */
        isp_be_blc_reg_config(ViPipe, pstRegCfgInfo, i);
        ISP_ClutRegConfig(ViPipe, pstRegCfgInfo, i); /* CLUT */
        ISP_WdrSplitRegConfig(ViPipe, pstRegCfgInfo, i);
        ISP_LogLUTRegConfig(ViPipe, pstRegCfgInfo, i);

    }

    for (i = 0; i < pstRegCfgInfo->cfg_num; i++) {
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstViProcReg);
        ISP_BeAlgLutUpdateRegConfig(ViPipe, pstRegCfgInfo, i);
    }

    if ((IS_OFFLINE_MODE(isp_ctx->block_attr.running_mode)) ||
        (IS_STRIPING_MODE(isp_ctx->block_attr.running_mode))) {
        s32Ret = ISP_CfgBeBufCtl(ViPipe);

        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("Pipe:%d Be config bufs ctl failed %x!\n", ViPipe, s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_BeRegsConfigInit(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo)
{
    HI_U32  i;
    S_VIPROC_REGS_TYPE *pstViProcReg = HI_NULL;
    isp_usr_ctx          *isp_ctx    = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    for (i = 0; i < pstRegCfgInfo->cfg_num; i++) {
        ISP_SystemRegConfig(ViPipe, pstRegCfgInfo, i); /* sys */
        ISP_DitherRegConfig(ViPipe, pstRegCfgInfo, i); /* dither */
        /* Be alg cfgs setting to register */
        ISP_AeRegConfig(ViPipe, pstRegCfgInfo, i);   /* ae */
        ISP_AwbRegConfig(ViPipe, pstRegCfgInfo, i);  /* awb */
        isp_af_reg_config(ViPipe, pstRegCfgInfo, i);  /* AF */
        ISP_SharpenRegConfig(ViPipe, pstRegCfgInfo, i); /* sharpen */
        ISP_EdgeMarkRegConfig(ViPipe, pstRegCfgInfo, i); /* sharpen */
        ISP_DemRegConfig(ViPipe, pstRegCfgInfo, i);  /* demosaic */
        ISP_FpnRegConfig(ViPipe, pstRegCfgInfo, i);  /* FPN */
        ISP_LdciRegConfig(ViPipe, pstRegCfgInfo, i); /* ldci */
        ISP_LcacRegConfig(ViPipe, pstRegCfgInfo, i); /* Local cac */
        ISP_GcacRegConfig(ViPipe, pstRegCfgInfo, i); /* global cac */
        ISP_FcrRegConfig(ViPipe, pstRegCfgInfo, i);  /* FCR */
        ISP_DpcRegConfig(ViPipe, pstRegCfgInfo, i);  /* dpc */
        ISP_GeRegConfig(ViPipe, pstRegCfgInfo, i);   /* ge */
        ISP_LscRegConfig(ViPipe, pstRegCfgInfo, i);  /* BE LSC */
        ISP_RLscRegConfig(ViPipe, pstRegCfgInfo, i);  /* Radial LSC */
        ISP_GammaRegConfig(ViPipe, pstRegCfgInfo, i); /* gamma */
        ISP_CscRegConfig(ViPipe, pstRegCfgInfo, i);  /* csc */
        ISP_CaRegConfig(ViPipe, pstRegCfgInfo, i);   /* ca */
        ISP_McdsRegConfig(ViPipe, pstRegCfgInfo, i); /* mcds */
        ISP_WdrRegConfig(ViPipe, pstRegCfgInfo, i);  /* wdr */
        ISP_DrcRegConfig(ViPipe, pstRegCfgInfo, i);  /* drc */
        ISP_DehazeRegConfig(ViPipe, pstRegCfgInfo, i);  /* Dehaze */
        ISP_BayerNrRegConfig(ViPipe, pstRegCfgInfo, i); /* BayerNR */
        ISP_DgRegConfig(ViPipe, pstRegCfgInfo, i);   /* DG */
        ISP_4DgRegConfig(ViPipe, pstRegCfgInfo, i);   /* 4DG */
        ISP_PreGammaRegConfig(ViPipe, pstRegCfgInfo, i); /* PreGamma */
        ISP_FlickRegConfig(ViPipe, pstRegCfgInfo, i); /* Flicker */
        isp_be_blc_reg_config(ViPipe, pstRegCfgInfo, i);
        ISP_ClutRegConfig(ViPipe, pstRegCfgInfo, i); /* CLUT */
        ISP_WdrSplitRegConfig(ViPipe, pstRegCfgInfo, i);
        ISP_LogLUTRegConfig(ViPipe, pstRegCfgInfo, i);

    }

    for (i = 0; i < pstRegCfgInfo->cfg_num; i++) {
        pstViProcReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstViProcReg);
        isp_be_reg_up_write(pstViProcReg, HI_TRUE);
        ISP_BeAlgLutUpdateRegConfig(ViPipe, pstRegCfgInfo, i);
    }

    if (isp_ctx->frame_cnt == 0) {
        ISP_BeSyncParamInit(ViPipe, pstRegCfgInfo);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegCfgInit(VI_PIPE ViPipe)
{
    isp_reg_cfg_attr *pstRegCfgCtx = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfgCtx);

    if (pstRegCfgCtx == HI_NULL) {
        pstRegCfgCtx = (isp_reg_cfg_attr *)ISP_MALLOC(sizeof(isp_reg_cfg_attr));
        if (pstRegCfgCtx == HI_NULL) {
            ISP_ERR_TRACE("Isp[%d] RegCfgCtx malloc memory failed!\n", ViPipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset(pstRegCfgCtx, 0, sizeof(isp_reg_cfg_attr));

    ISP_REGCFG_SET_CTX(ViPipe, pstRegCfgCtx);

    return HI_SUCCESS;
}

HI_S32 ISP_RegCfgExit(VI_PIPE ViPipe)
{
    isp_reg_cfg_attr *pstRegCfgCtx = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfgCtx);
    ISP_FREE(pstRegCfgCtx);
    ISP_REGCFG_RESET_CTX(ViPipe);

    return HI_SUCCESS;
}

HI_S32 ISP_GetRegCfgCtx(VI_PIPE ViPipe, HI_VOID **ppCfg)
{
    isp_usr_ctx    *isp_ctx = HI_NULL;
    isp_reg_cfg_attr *pstRegCfg = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
    ISP_CHECK_POINTER(pstRegCfg);

    if (!pstRegCfg->bInit) {
        pstRegCfg->reg_cfg.cfg_key.key = 0;

        pstRegCfg->bInit = HI_TRUE;
    }

    pstRegCfg->reg_cfg.cfg_num = isp_ctx->block_attr.block_num;

    *ppCfg = &pstRegCfg->reg_cfg;

    return HI_SUCCESS;
}

HI_S32 ISP_RegCfgInfoInit(VI_PIPE ViPipe)
{
    isp_reg_cfg_attr *pstRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
    ISP_CHECK_POINTER(pstRegCfg);

    ISP_FeRegsConfig(ViPipe, &pstRegCfg->reg_cfg);
    ISP_BeRegsConfigInit(ViPipe, &pstRegCfg->reg_cfg);

    return HI_SUCCESS;
}

HI_S32 ISP_RegCfgInfoSet(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_reg_cfg_attr *pstRegCfg = HI_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
    ISP_CHECK_POINTER(pstRegCfg);

    ISP_FeRegsConfig(ViPipe, &pstRegCfg->reg_cfg);
    ISP_BeRegsConfig(ViPipe, &pstRegCfg->reg_cfg);

    if (pstRegCfg->reg_cfg.kernel_reg_cfg.cfg_key.key) {
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_REG_CFG_SET, &pstRegCfg->reg_cfg.kernel_reg_cfg);

        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("Config ISP register Failed with ec %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_VOID ISP_SnsRegsInfoCheck(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    if ((pstSnsRegsInfo->enSnsType >= ISP_SNS_TYPE_BUTT)) {
        ISP_ERR_TRACE("senor's regs info invalid, enSnsType %d\n", pstSnsRegsInfo->enSnsType);
        return;
    }

    if (pstSnsRegsInfo->u32RegNum > ISP_MAX_SNS_REGS) {
        ISP_ERR_TRACE("senor's regs info invalid, u32RegNum %d\n", pstSnsRegsInfo->u32RegNum);
        return;
    }

    return;
}

HI_S32 ISP_SyncCfgSet(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_S32 s32PipeSt = 0;
    HI_S32 s32PipeEd = 0;
    HI_S8 s8StitchMainPipe;
    isp_usr_ctx    *isp_ctx  = HI_NULL;
    isp_reg_cfg_attr *pstRegCfg  = HI_NULL;
    isp_reg_cfg_attr *pstRegCfgS = HI_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
        s8StitchMainPipe = isp_ctx->stitch_attr.stitch_bind_id[0];

        if (IS_STITCH_MAIN_PIPE(ViPipe, s8StitchMainPipe)) {
            s32PipeSt = 0;
            s32PipeEd = isp_ctx->stitch_attr.stitch_pipe_num- 1;
        } else {
            s32PipeSt = ViPipe;
            s32PipeEd = ViPipe - 1;
        }
    } else {
        s32PipeSt = ViPipe;
        s32PipeEd = ViPipe;
    }

    while (s32PipeSt <= s32PipeEd) {
        if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
            ViPipe = isp_ctx->stitch_attr.stitch_bind_id[s32PipeSt];
        } else {
            ViPipe = s32PipeSt;
        }

        ISP_GET_CTX(ViPipe, isp_ctx);
        ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);
        ISP_CHECK_POINTER(pstRegCfg);
        ISP_CHECK_OPEN(ViPipe);

        if (ISP_SensorUpdateSnsReg(ViPipe) != HI_SUCCESS) {
            /* If Users need to config AE sync info themselves, they can set pfn_cmos_get_sns_reg_info to NULL in cmos.c */
            /* Then there will be NO AE sync configs in kernel of firmware */
            return HI_SUCCESS;
        }

        ISP_SensorGetSnsReg(ViPipe, &pstSnsRegsInfo);
        memcpy(&pstRegCfg->sync_cfg_node.stSnsRegsInfo, pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
        ISP_SnsRegsInfoCheck(ViPipe, &pstRegCfg->sync_cfg_node.stSnsRegsInfo);
        memcpy(&pstRegCfg->sync_cfg_node.stAERegCfg, &pstRegCfg->reg_cfg.alg_reg_cfg[0].stAeRegCfg2, sizeof(ISP_AE_REG_CFG_2_S));
        memcpy(&pstRegCfg->sync_cfg_node.stAWBRegCfg.au32WDRWBGain[0], &pstRegCfg->reg_cfg.alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au32WDRWBGain[0], sizeof(HI_U32) * ISP_BAYER_CHN_NUM);
        memcpy(&pstRegCfg->sync_cfg_node.stAWBRegCfg.au32BEWhiteBalanceGain[0], &pstRegCfg->reg_cfg.alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au32BEWhiteBalanceGain[0], sizeof(HI_U32) * ISP_BAYER_CHN_NUM);
        memcpy(&pstRegCfg->sync_cfg_node.stAWBRegCfg.au16ColorMatrix[0], &pstRegCfg->reg_cfg.alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au16BEColorMatrix[0], sizeof(HI_U16)*CCM_MATRIX_SIZE);
        memcpy(&pstRegCfg->sync_cfg_node.stDRCRegCfg, &pstRegCfg->reg_cfg.alg_reg_cfg[0].stDrcRegCfg.stSyncRegCfg, sizeof(ISP_DRC_REG_CFG_2_S));
        memcpy(&pstRegCfg->sync_cfg_node.stWDRRegCfg, &pstRegCfg->reg_cfg.alg_reg_cfg[0].stWdrRegCfg.stSyncRegCfg, sizeof(ISP_FSWDR_SYNC_CFG_S));
        pstRegCfg->sync_cfg_node.stAWBRegCfg.enBEAWBSwitch = pstRegCfg->reg_cfg.alg_reg_cfg[0].stAwbRegCfg.stAwbRegUsrCfg.enBEAWBSwitch;
        memcpy(&pstRegCfg->sync_cfg_node.stAWBRegCfg.au32SensorWBGain[0], &pstRegCfg->reg_cfg.alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au32SensorWBGain[0], sizeof(HI_U32) * ISP_BAYER_CHN_NUM);

        if (isp_ctx->stitch_attr.stitch_enable == HI_TRUE) {
            s8StitchMainPipe = isp_ctx->stitch_attr.stitch_bind_id[0];
            ISP_REGCFG_GET_CTX(s8StitchMainPipe, pstRegCfgS);
            if (pstRegCfgS == HI_NULL) {
                return HI_FAILURE;
            }

            if (!IS_STITCH_MAIN_PIPE(ViPipe, s8StitchMainPipe)) {
                memcpy(&pstRegCfg->sync_cfg_node.stSnsRegsInfo, &pstRegCfgS->sync_cfg_node.stSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
                memcpy(&pstRegCfg->sync_cfg_node.stSnsRegsInfo.unComBus, &pstSnsRegsInfo->unComBus, sizeof(ISP_SNS_COMMBUS_U));
                memcpy(&pstRegCfg->sync_cfg_node.stSnsRegsInfo.stSlvSync.u32SlaveBindDev, &pstSnsRegsInfo->stSlvSync.u32SlaveBindDev, sizeof(HI_U32));
                memcpy(&pstRegCfg->sync_cfg_node.stAERegCfg, &pstRegCfgS->reg_cfg.alg_reg_cfg[0].stAeRegCfg2, sizeof(ISP_AE_REG_CFG_2_S));
                memcpy(&pstRegCfg->sync_cfg_node.stAWBRegCfg.au16ColorMatrix[0], &pstRegCfg->reg_cfg.alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au16BEColorMatrix[0], sizeof(HI_U16)*CCM_MATRIX_SIZE);
                memcpy(&pstRegCfg->sync_cfg_node.stAWBRegCfg.au32BEWhiteBalanceGain[0], &pstRegCfg->reg_cfg.alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au32BEWhiteBalanceGain[0], sizeof(HI_U32) * ISP_BAYER_CHN_NUM);
                memcpy(&pstRegCfg->sync_cfg_node.stAWBRegCfg.au32WDRWBGain[0], &pstRegCfg->reg_cfg.alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au32WDRWBGain[0], sizeof(HI_U32) * ISP_BAYER_CHN_NUM);
                pstRegCfg->sync_cfg_node.stAWBRegCfg.enBEAWBSwitch = pstRegCfg->reg_cfg.alg_reg_cfg[0].stAwbRegCfg.stAwbRegUsrCfg.enBEAWBSwitch;
                memcpy(&pstRegCfg->sync_cfg_node.stAWBRegCfg.au32SensorWBGain[0], &pstRegCfg->reg_cfg.alg_reg_cfg[0].stAwbRegCfg.stAwbRegDynCfg.au32SensorWBGain[0], sizeof(HI_U32) * ISP_BAYER_CHN_NUM);
            }
        }

        pstRegCfg->sync_cfg_node.valid = HI_TRUE;

        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_CFG_SET, &pstRegCfg->sync_cfg_node);

        if (s32Ret != HI_SUCCESS) {
            return s32Ret;
        }

        pstSnsRegsInfo->bConfig = HI_TRUE;

        s32PipeSt++;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SnapRegCfgSet(VI_PIPE ViPipe, hi_isp_config_info *pstSnapInfo)
{
    HI_S32 s32Ret;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_CONFIG_INFO_SET, pstSnapInfo);

    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SnapRegCfgGet(VI_PIPE ViPipe, isp_snap_info *pstSnapInfo)
{
    HI_S32 s32Ret;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SNAP_INFO_GET, pstSnapInfo);

    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_BOOL ISP_ProTriggerGet(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_BOOL bEnable;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PRO_TRIGGER_GET, &bEnable);

    if (s32Ret != HI_SUCCESS) {
        return HI_FALSE;
    }

    return bEnable;
}

HI_S32 ISP_ResetFeSttEn(VI_PIPE ViPipe)
{
    return HI_SUCCESS;
}
HI_S32 ISP_RegCfgCtrl(VI_PIPE ViPipe)
{
    HI_U8 i;
    isp_usr_ctx    *isp_ctx = HI_NULL;
    isp_reg_cfg_attr *pstRegCfg = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);
    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);

    pstRegCfg->reg_cfg.cfg_key.key = 0xFFFFFFFFFFFFFFFF;

    for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
        isp_ctx->special_opt.be_on_stt_update[i] = HI_TRUE;
    }

    for (i = isp_ctx->block_attr.pre_block_num; i <  isp_ctx->block_attr.block_num; i++) {
        memcpy(&pstRegCfg->reg_cfg.alg_reg_cfg[i], &pstRegCfg->reg_cfg.alg_reg_cfg[0], sizeof(isp_alg_reg_cfg));
    }

    if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) || \
        IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
        for (i = isp_ctx->block_attr.pre_block_num; i <  isp_ctx->block_attr.block_num; i++) {
            ISP_BeReshCfg(&pstRegCfg->reg_cfg.alg_reg_cfg[i]);
        }
    }

    pstRegCfg->reg_cfg.cfg_num = isp_ctx->block_attr.block_num;

    return HI_SUCCESS;
}

HI_S32  ISP_SetBeRawPos(VI_PIPE ViPipe, isp_reg_cfg *pstRegCfgInfo, HI_U8 i, HI_BOOL  bEnable)
{

    pstRegCfgInfo->alg_reg_cfg[i].stSharpenRegCfg.bEnable = bEnable;
    pstRegCfgInfo->alg_reg_cfg[i].stMcdsRegCfg.bMCDSen = bEnable;

    pstRegCfgInfo->alg_reg_cfg[i].stSplitCfg.bEnable = bEnable;
    pstRegCfgInfo->alg_reg_cfg[i].stCaRegCfg.bCaEn = bEnable;
    pstRegCfgInfo->alg_reg_cfg[i].stLdciRegCfg.stDynaRegCfg.bEnable = bEnable;
    pstRegCfgInfo->alg_reg_cfg[i].stCscCfg.bEnable = bEnable;
    pstRegCfgInfo->alg_reg_cfg[i].stDehazeRegCfg.bDehazeEn = bEnable;
    pstRegCfgInfo->alg_reg_cfg[i].stGammaCfg.bGammaEn = bEnable;
    pstRegCfgInfo->alg_reg_cfg[i].stClutCfg.bEnable = bEnable;

    pstRegCfgInfo->alg_reg_cfg[i].stAwbRegCfg.stAwbRegDynCfg.u8BECcEn = bEnable;

    pstRegCfgInfo->alg_reg_cfg[i].stDemRegCfg.bNddmEnable = bEnable;
    pstRegCfgInfo->alg_reg_cfg[i].stDemRegCfg.bVhdmEnable = bEnable;



    return HI_SUCCESS;

}

HI_S32  ISP_SetBeRaw(VI_PIPE ViPipe)
{
    HI_U8 i;
    HI_BOOL bEnable;
     isp_usr_ctx    *isp_ctx = HI_NULL;
    isp_reg_cfg_attr *pstRegCfg = HI_NULL;

    S_VIPROC_REGS_TYPE *pstViprocReg = HI_NULL;



    ISP_GET_CTX(ViPipe, isp_ctx);
    ISP_REGCFG_GET_CTX(ViPipe, pstRegCfg);

    ISP_CHECK_POINTER(pstRegCfg);

    for (i = 0; i < pstRegCfg->reg_cfg.cfg_num; i++) {
        pstViprocReg = (S_VIPROC_REGS_TYPE *)ISP_GetViProcVirAddr(ViPipe, i);
        ISP_CHECK_POINTER(pstViprocReg);

   if ((HI_FALSE ==  isp_ctx->be_raw_info.enable_be_raw) && ( HI_FALSE == isp_ctx->be_raw_info.pre_enabe_be_raw)) {

        isp_ctx->be_raw_info.be_ctrl0 = pstViprocReg->VIPROC_ISPBE_CTRL0.u32;
        isp_ctx->be_raw_info.be_ctrl1 =  pstViprocReg->VIPROC_ISPBE_CTRL1.u32;
    }

    if (HI_TRUE == isp_ctx->be_raw_info.enable_be_raw ) {

        if ( RAW_POS_DEMOSAIC ==  isp_ctx->be_raw_info.isp_raw_pos ) {

            isp_sqrt1_en_write(pstViprocReg, 0);
            isp_sq_en_write(pstViprocReg, 0);


            pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = 0x0;

            bEnable = HI_FALSE;
            ISP_SetBeRawPos(ViPipe, &pstRegCfg->reg_cfg, i, bEnable);

        }
    } else if ((HI_FALSE == isp_ctx->be_raw_info.enable_be_raw) && (HI_TRUE == isp_ctx->be_raw_info.pre_enabe_be_raw)) {

        pstViprocReg->VIPROC_ISPBE_CTRL0.u32 = isp_ctx->be_raw_info.be_ctrl0;
        pstViprocReg->VIPROC_ISPBE_CTRL1.u32 = isp_ctx->be_raw_info.be_ctrl1;

    } else {

    }

    isp_ctx->be_raw_info.pre_enabe_be_raw   =   isp_ctx->be_raw_info.enable_be_raw;

    }


    return HI_SUCCESS;

}

HI_S32 ISP_SwitchRegSet(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    isp_usr_ctx *isp_ctx = HI_NULL;

    ISP_GET_CTX(ViPipe, isp_ctx);

    if (IS_ONLINE_MODE(isp_ctx->block_attr.running_mode) || \
        IS_SIDEBYSIDE_MODE(isp_ctx->block_attr.running_mode)) {
        ISP_RegCfgInfoSet(ViPipe);
        isp_ctx->block_attr.pre_block_num = isp_ctx->block_attr.block_num;
        return HI_SUCCESS;
    }

    /* record the register config infomation to fhy and kernel,and be valid in next frame. */
    s32Ret = ISP_RegCfgInfoInit(ViPipe);
    if (s32Ret) {
        return s32Ret;
    }

    isp_ctx->para_rec.stitch_sync = HI_TRUE;
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_SYNC_INIT_SET, &isp_ctx->para_rec.stitch_sync);
    if (s32Ret != HI_SUCCESS) {
        isp_ctx->para_rec.stitch_sync = HI_FALSE;
        ISP_ERR_TRACE("ISP[%d] set isp stitch sync failed!\n", ViPipe);
    }

    s32Ret = ISP_AllCfgsBeBufInit(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe:%d init all be bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    isp_ctx->block_attr.pre_block_num = isp_ctx->block_attr.block_num;

    return HI_SUCCESS;
}

HI_S32 ISP_Lut2SttApbReg(VI_PIPE ViPipe)
{
    return HI_SUCCESS;
}

HI_S32 ISP_SwitchStateSet(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_BE_SWITCH_FINISH_STATE_SET);

    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
