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
#include "isp_dcfinfo.h"
#include "isp_ext_config.h"
#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

extern HI_S32 g_as32IspFd[ISP_MAX_PIPE_NUM];

HI_S32 ISP_UpdateInfoInit(VI_PIPE ViPipe)
{
    HI_U64 u64PhyAddr;
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->isp_trans_info.update_info.phy_addr;

    hi_ext_system_update_info_high_phyaddr_write(ViPipe, ((u64PhyAddr >> 32) & 0xFFFFFFFF));
    hi_ext_system_update_info_low_phyaddr_write(ViPipe, (u64PhyAddr & 0xFFFFFFFF));

    pstIspCtx->update_info_ctrl.isp_update_info = HI_MPI_SYS_Mmap(u64PhyAddr,
                                                  (sizeof(hi_isp_dcf_update_info) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(hi_isp_dcf_const_info)));

    if (pstIspCtx->update_info_ctrl.isp_update_info == HI_NULL) {
        ISP_ERR_TRACE("isp[%d] mmap update info buf failed!\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    pstIspCtx->update_info_ctrl.isp_dcf_const_info =
       (hi_isp_dcf_const_info *)(pstIspCtx->update_info_ctrl.isp_update_info + ISP_MAX_UPDATEINFO_BUF_NUM);

    return HI_SUCCESS;
}

HI_S32 ISP_UpdateInfoExit(VI_PIPE ViPipe)
{
    isp_usr_ctx *pstIspCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->update_info_ctrl.isp_update_info != HI_NULL) {
        HI_MPI_SYS_Munmap(pstIspCtx->update_info_ctrl.isp_update_info,
                          (sizeof(hi_isp_dcf_update_info) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(hi_isp_dcf_const_info)));
        pstIspCtx->update_info_ctrl.isp_update_info = HI_NULL;
        pstIspCtx->update_info_ctrl.isp_dcf_const_info = HI_NULL;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
