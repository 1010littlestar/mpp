/*
* Copyright (c) Hisilicon Technologies Co., Ltd. 2011-2019. All rights reserved.
* Description: hi_comm_vb_adapt.h
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#ifndef __HI_COMM_VB_ADAPT_H__
#define __HI_COMM_VB_ADAPT_H__


#include "hi_comm_vb.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef VB_UID_E hi_vb_uid;

typedef VB_POOL hi_vb_pool;
typedef VB_BLK hi_vb_blk;

typedef VB_REMAP_MODE_E hi_vb_remap_mode;

typedef struct {
    hi_u64 blk_size;
    hi_u32 blk_cnt;
    hi_vb_remap_mode remap_mode;
    hi_char mmz_name[MAX_MMZ_NAME_LEN];
} hi_vb_pool_config;

typedef struct {
    hi_u32 max_pool_cnt;
    hi_vb_pool_config comm_pool[VB_MAX_COMM_POOLS];
} hi_vb_config;

typedef struct {
    hi_u32 is_comm_pool;
    hi_u32 blk_cnt;
    hi_u32 free_blk_cnt;
} hi_vb_pool_status;

typedef struct {
    hi_u32 supplement_config;
} hi_vb_supplement_config;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_COMM_VB_ADAPT_H_ */

