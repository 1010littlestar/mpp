/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <stdio.h>
#include "isp_main.h"
#include "hi_isp_debug.h"
#include "isp_debug.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "mpi_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 isp_dbg_set(VI_PIPE ViPipe, const hi_isp_debug_info *dbg_info)
{
    HI_U32 u32Size = 0;

    if (hi_ext_system_sys_debug_enable_read(ViPipe) && dbg_info->debug_en) {
        ISP_ERR_TRACE("Hisi isp has enabled debug info!\n");
        return HI_FAILURE;
    }

    if (dbg_info->debug_en) {
        if (dbg_info->phy_addr == 0) {
            ISP_ERR_TRACE("Hisi isp lib's debug phyaddr is 0!\n");
            return HI_FAILURE;
        }

        if (dbg_info->depth == 0) {
            ISP_ERR_TRACE("Hisi ae lib's debug depth is 0!\n");
            return HI_FAILURE;
        }
        u32Size = sizeof(ISP_DBG_ATTR_S) +
                  sizeof(ISP_DBG_ATTR_S) * dbg_info->depth;
    }

    /* don't clear phyaddr and size when disable dbg info. */
    if (dbg_info->debug_en) {
        hi_ext_system_sys_debug_enable_write(ViPipe, dbg_info->debug_en);
        hi_ext_system_sys_debug_high_addr_write(ViPipe, ((dbg_info->phy_addr >> 32) & 0xFFFFFFFF));
        hi_ext_system_sys_debug_low_addr_write(ViPipe, (dbg_info->phy_addr & 0xFFFFFFFF));
        hi_ext_system_sys_debug_depth_write(ViPipe, dbg_info->depth);
        hi_ext_system_sys_debug_size_write(ViPipe, u32Size);
    } else {
        hi_ext_system_sys_debug_enable_write(ViPipe, dbg_info->debug_en);
    }

    return HI_SUCCESS;
}

HI_S32 isp_dbg_get(VI_PIPE ViPipe, hi_isp_debug_info *dbg_info)
{
    HI_U64 u64PhyAddrHigh;
    HI_U64 u64PhyAddrTemp;

    u64PhyAddrHigh  = (HI_U64)hi_ext_system_sys_debug_high_addr_read(ViPipe);
    u64PhyAddrTemp  = (HI_U64)hi_ext_system_sys_debug_low_addr_read(ViPipe);
    u64PhyAddrTemp |= (u64PhyAddrHigh << 32);

    dbg_info->phy_addr = u64PhyAddrTemp;
    dbg_info->debug_en = hi_ext_system_sys_debug_enable_read(ViPipe);
    dbg_info->depth    = hi_ext_system_sys_debug_depth_read(ViPipe);

    return HI_SUCCESS;
}

HI_S32 ISP_DbgRunBgn(isp_dbg_ctrl *pstDbg, HI_U32 u32FrmCnt)
{
    hi_isp_dbg_status  *pstDbgStatus = HI_NULL;

    if (!pstDbg->debug_en) {
        if (pstDbg->dbg_attr != HI_NULL) {
            HI_MPI_SYS_Munmap(pstDbg->dbg_attr, pstDbg->size);
            pstDbg->dbg_attr = HI_NULL;
            pstDbg->dbg_status = HI_NULL;
        }
        return HI_SUCCESS;
    }

    if ((pstDbg->debug_en) && (pstDbg->dbg_attr == HI_NULL)) {
        pstDbg->dbg_attr = (hi_isp_dbg_attr *)HI_MPI_SYS_Mmap(pstDbg->phy_addr, pstDbg->size);
        if (pstDbg->dbg_attr == HI_NULL) {
            ISP_ERR_TRACE("isp map debug buf failed!\n");
            return HI_FAILURE;
        }
        pstDbg->dbg_status = (hi_isp_dbg_status *)(pstDbg->dbg_attr + 1);

        /* ------------- record attr ------------------ */
    }

    pstDbgStatus = pstDbg->dbg_status + (u32FrmCnt % DIV_0_TO_1(pstDbg->depth));

    pstDbgStatus->frm_num_bgn = u32FrmCnt;

    return HI_SUCCESS;
}

HI_S32 ISP_DbgRunEnd(isp_dbg_ctrl *pstDbg, HI_U32 u32FrmCnt)
{
    hi_isp_dbg_status *pstDbgStatus = HI_NULL;

    if ((!pstDbg->debug_en) || (pstDbg->dbg_status == HI_NULL)) {
        return HI_SUCCESS;
    }

    pstDbgStatus = pstDbg->dbg_status + (u32FrmCnt % DIV_0_TO_1(pstDbg->depth));

    /* ------------- record status ------------------ */

    pstDbgStatus->frm_num_end = u32FrmCnt;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
