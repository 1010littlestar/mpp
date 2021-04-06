/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mpi_sys.h"
#include "mkp_isp.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiISP_PROC_S {
    HI_U32 u32IntCount;
    HI_U32 u32ProcParam;
    isp_proc_mem stProcMem;
} ISP_PROC_S;

ISP_PROC_S g_astProcCtx[ISP_MAX_PIPE_NUM] = { { 0 } };
#define PROC_GET_CTX(dev, pstCtx) pstCtx = &g_astProcCtx[dev]

HI_S32 ISP_UpdateProcParam(VI_PIPE ViPipe);

HI_S32 ISP_ProcInit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    ISP_PROC_S *pstProc = HI_NULL;

    PROC_GET_CTX(ViPipe, pstProc);

    ISP_CHECK_OPEN(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_PARAM_GET, &pstProc->u32ProcParam);
    if (s32Ret) {
        ISP_ERR_TRACE("ISP[%d] get proc param %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    if (pstProc->u32ProcParam == 0) {
        return HI_SUCCESS;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_INIT, &pstProc->stProcMem);
    if (s32Ret) {
        ISP_ERR_TRACE("ISP[%d] init proc ec %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    pstProc->stProcMem.virt_addr = HI_MPI_SYS_Mmap(pstProc->stProcMem.phy_addr,
                                                       pstProc->stProcMem.size);
    if (pstProc->stProcMem.virt_addr == HI_NULL) {
        ISP_ERR_TRACE("ISP[%d] mmap proc mem failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOMEM;
        goto freeproc;
    }
    pstProc->u32IntCount = 0;

    return HI_SUCCESS;

freeproc:
    if (ioctl(g_as32IspFd[ViPipe], ISP_PROC_EXIT) != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] exit proc failed!\n", ViPipe);
        return HI_FAILURE;
    }
    return s32Ret;
}

HI_S32 ISP_ProcWrite(const isp_alg_node *astAlgs, VI_PIPE ViPipe)
{
    HI_S32 s32Ret, i;
    ISP_PROC_S *pstProc = HI_NULL;
    hi_isp_ctrl_proc_write stProcCtrl;

    PROC_GET_CTX(ViPipe, pstProc);

    ISP_CHECK_OPEN(ViPipe);

    ISP_UpdateProcParam(ViPipe);

    if (pstProc->u32ProcParam == 0) {
        return HI_SUCCESS;
    }

    if (pstProc->stProcMem.virt_addr == HI_NULL) {
        ISP_ERR_TRACE("ISP[%d] the proc hasn't init!\n", ViPipe);
        return HI_FAILURE;
    }

    /* write proc info 1s a time */
    pstProc->u32IntCount++;
    if (pstProc->u32IntCount < pstProc->u32ProcParam) {
        return HI_SUCCESS;
    }
    pstProc->u32IntCount = 0;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_WRITE_ING);
    if (s32Ret) {
        ISP_ERR_TRACE("ISP[%d] write proc failed, ec %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    stProcCtrl.proc_buff = (HI_CHAR *)pstProc->stProcMem.virt_addr;
    stProcCtrl.buff_len  = pstProc->stProcMem.size- 1;
    stProcCtrl.write_len = 0;

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (astAlgs[i].used) {
            if (astAlgs[i].alg_func.pfn_alg_ctrl != HI_NULL) {
                astAlgs[i].alg_func.pfn_alg_ctrl(ViPipe, ISP_PROC_WRITE, &stProcCtrl);
            }

            if (stProcCtrl.write_len > stProcCtrl.buff_len) {
                ISP_ERR_TRACE("ISP[%d] Warning!! Proc buff overflow!\n", ViPipe);
                stProcCtrl.write_len = stProcCtrl.buff_len;
                break;
            }

            if (stProcCtrl.write_len != 0) {
                if (stProcCtrl.proc_buff[stProcCtrl.write_len - 1] != '\0') {
                    ISP_ERR_TRACE("ISP[%d] Warning!! alg %d's proc doesn't finished with endl!\n", ViPipe, astAlgs[i].alg_type);
                }
                stProcCtrl.proc_buff[stProcCtrl.write_len - 1] = '\n';
            }

            /* update the proc ctrl */
            stProcCtrl.proc_buff = &stProcCtrl.proc_buff[stProcCtrl.write_len];
            stProcCtrl.buff_len  = stProcCtrl.buff_len - stProcCtrl.write_len;
            stProcCtrl.write_len = 0;
            if (stProcCtrl.buff_len == 0) {
                break;
            }
        }
    }

    stProcCtrl.proc_buff[stProcCtrl.write_len] = '\0';
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_WRITE_OK);
    if (s32Ret) {
        ISP_ERR_TRACE("ISP[%d] write proc failed, ec %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_ProcExit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_VOID *pVirtAddr;
    ISP_PROC_S *pstProc = HI_NULL;

    PROC_GET_CTX(ViPipe, pstProc);

    ISP_CHECK_OPEN(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_PARAM_GET, &pstProc->u32ProcParam);
    if (s32Ret) {
        ISP_ERR_TRACE("ISP[%d] get proc param %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    if (pstProc->u32ProcParam == 0) {
        return HI_SUCCESS;
    }

    if (pstProc->stProcMem.virt_addr!= HI_NULL) {
        pVirtAddr = pstProc->stProcMem.virt_addr;
        pstProc->stProcMem.virt_addr= HI_NULL;
        HI_MPI_SYS_Munmap(pVirtAddr, pstProc->stProcMem.size);
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_EXIT);
    if (s32Ret) {
        ISP_ERR_TRACE("ISP[%d] exit proc ec %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_UpdateProcParam(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    ISP_PROC_S *pstProc = HI_NULL;

    PROC_GET_CTX(ViPipe, pstProc);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_PARAM_GET, &pstProc->u32ProcParam);
    if (s32Ret) {
        ISP_ERR_TRACE("ISP[%d] get proc param %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
