/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include "isp_block.h"
#include "mkp_isp.h"
#include "hi_isp_debug.h"
#include "isp_main.h"
#include "isp_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 ISP_GetWorkMode(VI_PIPE ViPipe, isp_working_mode *pstIspWorkMode)
{
    return HI_SUCCESS;
}

static HI_S32 ISP_GetBlockHwAttr(VI_PIPE ViPipe, isp_block_attr *pstBlock)
{
    HI_S32 s32Ret = HI_FAILURE;
    isp_block_attr stBlkAttr = { 0 };

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_INIT, &stBlkAttr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("s32Ret:%d!\n", s32Ret);
        return s32Ret;
    }

    ISP_CHECK_RUNNING_MODE(stBlkAttr.running_mode);
    ISP_CHECK_BLOCK_NUM(stBlkAttr.block_num);

    if (((stBlkAttr.running_mode == ISP_MODE_RUNNING_OFFLINE) ||
         (stBlkAttr.running_mode == ISP_MODE_RUNNING_ONLINE)) &&
        (stBlkAttr.block_num != ISP_NORMAL_BLOCK_NUM)) {
        ISP_ERR_TRACE("ViPipe :%d,When enIspRunningMode = %d,u8BlockNum should be equal to %d!\n",
                  ViPipe, stBlkAttr.running_mode, ISP_NORMAL_BLOCK_NUM);
        return HI_FAILURE;
    } else if ((stBlkAttr.running_mode == ISP_MODE_RUNNING_SIDEBYSIDE) &&
               (stBlkAttr.block_num != ISP_SBS_BLOCK_NUM)) {
        ISP_ERR_TRACE("ViPipe :%d,When enIspRunningMode = %d,u8BlockNum should be equal to %d!\n",
                  ViPipe, stBlkAttr.running_mode, ISP_SBS_BLOCK_NUM);
        return HI_FAILURE;
    } else if ((stBlkAttr.running_mode == ISP_MODE_RUNNING_STRIPING) && (stBlkAttr.block_num < 2)) {
        ISP_ERR_TRACE("ViPipe :%d,When enIspRunningMode = %d,u8BlockNum should not be less than %d!\n",
                  ViPipe, stBlkAttr.running_mode, 2);
        return HI_FAILURE;
    }

    pstBlock->running_mode = stBlkAttr.running_mode;
    pstBlock->block_num    = stBlkAttr.block_num;
    pstBlock->over_lap     = stBlkAttr.over_lap;

    pstBlock->frame_rect.width  = stBlkAttr.frame_rect.width;
    pstBlock->frame_rect.height = stBlkAttr.frame_rect.height;

    memcpy(pstBlock->block_rect, stBlkAttr.block_rect, sizeof(hi_rect) * ISP_STRIPING_MAX_NUM);

    hi_ext_system_be_total_width_write(ViPipe,  pstBlock->frame_rect.width);
    hi_ext_system_be_total_height_write(ViPipe, pstBlock->frame_rect.height);

    return HI_SUCCESS;
}

HI_S32 ISP_BlockInit(VI_PIPE ViPipe, isp_block_attr *pstBlock)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = ISP_GetBlockHwAttr(ViPipe, pstBlock);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("get isp block HW attr failed!\n");
        return s32Ret;
    }

    pstBlock->pre_block_num = pstBlock->block_num;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PRE_BLK_NUM_UPDATE, &pstBlock->pre_block_num);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d]:update pre block num failed\n", ViPipe);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_BlockUpdate(VI_PIPE ViPipe, isp_block_attr *pstBlock)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = ISP_GetBlockHwAttr(ViPipe, pstBlock);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("get isp block HW attr failed!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_BlockExit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_EXIT);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_U32 ISP_GetBlockRect(isp_rect *pstBlockRect, isp_block_attr *pstBlock, HI_U8 u8BlockId)
{
    ISP_CHECK_BLOCK_ID(u8BlockId);

    if (pstBlock->block_num == 1) {
        pstBlockRect->x      = 0;
        pstBlockRect->y      = 0;
        pstBlockRect->width  = pstBlock->frame_rect.width;
        pstBlockRect->height = pstBlock->frame_rect.height;

        return HI_SUCCESS;
    }

    pstBlockRect->x      = pstBlock->block_rect[u8BlockId].x;
    pstBlockRect->y      = pstBlock->block_rect[u8BlockId].y;
    pstBlockRect->width  = pstBlock->block_rect[u8BlockId].width;
    pstBlockRect->height = pstBlock->block_rect[u8BlockId].height;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
