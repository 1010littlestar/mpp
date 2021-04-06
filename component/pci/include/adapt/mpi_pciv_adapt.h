/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description   : mpi functions declaration
 * Author        : Hisilicon Hi3511 MPP Team
 * Created       : 2009/06/23
 */
#ifndef __MPI_PCIV_ADAPT_H__
#define __MPI_PCIV_ADAPT_H__

#include "hi_comm_pciv_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define MPI_STATIC

MPI_STATIC hi_s32 hi_mpi_pciv_create(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr);

MPI_STATIC hi_s32 hi_mpi_pciv_destroy(hi_pciv_chn pciv_chn);

MPI_STATIC hi_s32 hi_mpi_pciv_set_attr(hi_pciv_chn pciv_chn, const hi_pciv_attr *attr);

MPI_STATIC hi_s32 hi_mpi_pciv_get_attr(hi_pciv_chn pciv_chn, hi_pciv_attr *attr);


MPI_STATIC hi_s32 hi_mpi_pciv_start(hi_pciv_chn pciv_chn);

MPI_STATIC hi_s32 hi_mpi_pciv_stop(hi_pciv_chn pciv_chn);

MPI_STATIC hi_s32 hi_mpi_pciv_dma_task(hi_pciv_dma_task *task);

MPI_STATIC hi_s32 hi_mpi_pciv_malloc(hi_u32 blk_size, hi_u32 blk_cnt, hi_u64 phy_addr[]);

MPI_STATIC hi_s32 hi_mpi_pciv_free(hi_u32 blk_cnt, const hi_u64 phy_addr[]);

MPI_STATIC hi_s32 hi_mpi_pciv_malloc_chn_buffer(hi_pciv_chn pciv_chn, hi_u32 blk_size,
    hi_u32 blk_cnt, hi_u64 phy_addr[]);

MPI_STATIC hi_s32 hi_mpi_pciv_free_chn_buffer(hi_pciv_chn pciv_chn, hi_u32 blk_cnt);


MPI_STATIC hi_s32 hi_mpi_pciv_get_local_id(hi_void);

MPI_STATIC hi_s32 hi_mpi_pciv_enum_chip(hi_s32 chip_id[PCIV_MAX_CHIPNUM]);

MPI_STATIC hi_s32 hi_mpi_pciv_get_base_window(hi_s32 chip_id, hi_pciv_base_window *base);

MPI_STATIC hi_s32 hi_mpi_pciv_win_vb_create(const hi_pciv_win_vb_cfg *cfg);

MPI_STATIC hi_s32 hi_mpi_pciv_win_vb_destroy(hi_void);

MPI_STATIC hi_s32 hi_mpi_pciv_show(hi_pciv_chn pciv_chn);

MPI_STATIC hi_s32 hi_mpi_pciv_hide(hi_pciv_chn pciv_chn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_VENC_H__ */

