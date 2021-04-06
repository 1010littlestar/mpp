/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description   : struct definition for pciv firmware
 * Author        : Hisilicon multimedia software group
 * Created       : 2009/07/16
 */

#ifndef __PCIV_FIRMWARE_H__
#define __PCIV_FIRMWARE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    struct list_head list;
    hi_pciv_chn      pciv_chn;
} pciv_portmap_node;

typedef struct {
    hi_u32 pool_count;
    hi_u32 pool_id[PCIV_MAX_VBCOUNT];
    hi_u32 size[PCIV_MAX_VBCOUNT];
} pciv_vb_pool;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

