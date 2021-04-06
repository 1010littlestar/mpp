/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description   : MPI for user
 * Author        : Hisilicon multimedia software group
 * Created       : 2019/05/08
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>

#include "mpi_pciv_adapt.h"
#include "mkp_pciv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static hi_s32 g_pciv_fd[PCIV_MAX_CHN_NUM] = { [0 ...(PCIV_MAX_CHN_NUM - 1)] = -1 };

/*  If the ioctl is interrupt by the sema, it will called again
* if in the program the signal is been capture,but not set SA_RESTART attribute,then when the xx_interruptible in the kernel interrupt,
* it will not re-called by the system,So The best way to avoid this is encapsule again in the user mode
*/
static pthread_mutex_t s_picv_mutex = PTHREAD_MUTEX_INITIALIZER;

#define PCIV_MUTEX_LOCK()                       \
do {                                            \
    (hi_void)pthread_mutex_lock(&s_picv_mutex); \
} while (0)

#define PCIV_MUTEX_UNLOCK()                       \
do {                                              \
    (hi_void)pthread_mutex_unlock(&s_picv_mutex); \
} while (0)

static hi_void mkpi_pciv_show_ioctl_err_msg(hi_s32 errcode)
{
    if (errcode == -1) {
        perror("ioctl error");
        printf("func:%s,line:%d,err_no:0x%x\n", __FUNCTION__, __LINE__, errcode);
    }
    return;
}

#define IOCTL(arg...)                                 \
({                                                    \
    hi_s32 ret_ = HI_SUCCESS;                         \
    do {                                              \
        ret_ = ioctl(arg);                            \
    } while ((ret_ == -1) && (errno == EINTR));       \
    mkpi_pciv_show_ioctl_err_msg(ret_);               \
    ret_;                                             \
})

static hi_s32 mkpi_pciv_check_chn_open(hi_pciv_chn chn)
{
    PCIV_MUTEX_LOCK();
    if (g_pciv_fd[chn] < 0) {
        g_pciv_fd[chn] = open("/dev/pciv", O_RDONLY);
        if (g_pciv_fd[chn] < 0) {
            perror("open PCIV error");
            PCIV_MUTEX_UNLOCK();
            return HI_ERR_PCIV_SYS_NOTREADY;
        }
        if (IOCTL(g_pciv_fd[chn], PCIV_BINDCHN2FD_CTRL, &chn)) {
            close(g_pciv_fd[chn]);
            g_pciv_fd[chn] = -1;
            PCIV_MUTEX_UNLOCK();
            return HI_ERR_PCIV_SYS_NOTREADY;
        }
    }
    PCIV_MUTEX_UNLOCK();
    return HI_SUCCESS;
}


#define PCIV_CHECK_OPEN(id)                           \
do {                                                  \
    if (mkpi_pciv_check_chn_open(id) != HI_SUCCESS) { \
        return HI_ERR_PCIV_SYS_NOTREADY;                  \
    }                                                 \
} while (0)                                           \

MPI_STATIC hi_s32 hi_mpi_pciv_create(hi_pciv_chn chn, const hi_pciv_attr *pciv_attr)
{
    PCIV_CHECK_CHNID(chn);
    PCIV_CHECK_OPEN(chn);
    PCIV_CHECK_PTR(pciv_attr);
    return IOCTL(g_pciv_fd[chn], PCIV_CREATE_CTRL, pciv_attr);
}

MPI_STATIC hi_s32 hi_mpi_pciv_destroy(hi_pciv_chn chn)
{
    PCIV_CHECK_CHNID(chn);
    PCIV_CHECK_OPEN(chn);

    return IOCTL(g_pciv_fd[chn], PCIV_DESTROY_CTRL);
}

MPI_STATIC hi_s32 hi_mpi_pciv_set_attr(hi_pciv_chn chn, const hi_pciv_attr *pciv_attr)
{
    PCIV_CHECK_CHNID(chn);
    PCIV_CHECK_OPEN(chn);
    PCIV_CHECK_PTR(pciv_attr);

    return IOCTL(g_pciv_fd[chn], PCIV_SETATTR_CTRL, pciv_attr);
}

MPI_STATIC hi_s32 hi_mpi_pciv_get_attr(hi_pciv_chn chn, hi_pciv_attr *pciv_attr)
{
    PCIV_CHECK_CHNID(chn);
    PCIV_CHECK_OPEN(chn);
    PCIV_CHECK_PTR(pciv_attr);

    return IOCTL(g_pciv_fd[chn], PCIV_GETATTR_CTRL, pciv_attr);
}

MPI_STATIC hi_s32 hi_mpi_pciv_start(hi_pciv_chn chn)
{
    PCIV_CHECK_CHNID(chn);
    PCIV_CHECK_OPEN(chn);

    return IOCTL(g_pciv_fd[chn], PCIV_START_CTRL);
}

MPI_STATIC hi_s32 hi_mpi_pciv_stop(hi_pciv_chn chn)
{
    PCIV_CHECK_CHNID(chn);
    PCIV_CHECK_OPEN(chn);

    return IOCTL(g_pciv_fd[chn], PCIV_STOP_CTRL);
}

MPI_STATIC hi_s32 hi_mpi_pciv_dma_task(hi_pciv_dma_task *task)
{
    PCIV_CHECK_OPEN(0);
    PCIV_CHECK_PTR(task);
    PCIV_CHECK_PTR(task->block);

    return IOCTL(g_pciv_fd[0], PCIV_DMATASK_CTRL, task);
}

MPI_STATIC hi_s32 hi_mpi_pciv_malloc(hi_u32 blk_size, hi_u32 blk_cnt, hi_u64 phy_addr[])
{
    hi_s32 ret = HI_SUCCESS;
    hi_s32 cnt = 0;
    hi_pciv_ioctl_malloc malloc_buf;

    PCIV_CHECK_OPEN(0);
    PCIV_CHECK_PTR(phy_addr);
    if (blk_cnt == 0) {
        PCIV_ERR_TRACE("blk_cnt:%d is illegal!\n", blk_cnt);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if (blk_size == 0) {
        PCIV_ERR_TRACE("blk_size:%d is illegal!\n", blk_size);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    for (cnt = 0; cnt < blk_cnt; cnt++) {
        malloc_buf.size = blk_size;
        malloc_buf.phy_addr = 0;
        ret = IOCTL(g_pciv_fd[0], PCIV_MALLOC_CTRL, &malloc_buf);
        if (ret != HI_SUCCESS) {
            break;
        }

        phy_addr[cnt] = malloc_buf.phy_addr;
    }
    /*  If one block malloc failure then free all the memory  */
    if ((ret != HI_SUCCESS) && (cnt != 0)) {
        cnt--;
        for (; cnt >= 0; cnt--) {
            (hi_void)IOCTL(g_pciv_fd[0], PCIV_FREE_CTRL, &phy_addr[cnt]);
        }
    }

    return ret;
}

MPI_STATIC hi_s32 hi_mpi_pciv_free(hi_u32 blk_cnt, const hi_u64 phy_addr[])
{
    hi_u32 i;
    hi_s32 ret = HI_SUCCESS;
    PCIV_CHECK_OPEN(0);
    PCIV_CHECK_PTR(phy_addr);

    if (blk_cnt == 0) {
        PCIV_ERR_TRACE("blk_cnt:%d is illegal!\n", blk_cnt);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    for (i = 0; i < blk_cnt; i++) {
        ret = IOCTL(g_pciv_fd[0], PCIV_FREE_CTRL, &phy_addr[i]);
        if (ret != HI_SUCCESS) {
            break;
        }
    }

    return ret;
}

MPI_STATIC hi_s32 hi_mpi_pciv_malloc_chn_buffer(hi_pciv_chn chn, hi_u32 blk_size, hi_u32 blk_cnt, hi_u64 phy_addr[])
{
    hi_s32 ret = HI_SUCCESS;
    hi_s32 cnt = 0;
    hi_pciv_ioctl_malloc_chn_buf malloc_chn_buf;

    PCIV_CHECK_CHNID(chn);
    PCIV_CHECK_OPEN(chn);
    PCIV_CHECK_PTR(phy_addr);
    malloc_chn_buf.chn_id = chn;
    if (blk_size == 0) {
        PCIV_ERR_TRACE("blk_size:%d is illegal!\n", blk_size);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    if ((blk_cnt == 0) || (blk_cnt > PCIV_MAX_BUF_NUM)) {
        PCIV_ERR_TRACE("blk_cnt:%d is illegal!\n", blk_cnt);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    for (cnt = 0; cnt < blk_cnt; cnt++) {
        malloc_chn_buf.index = cnt;
        malloc_chn_buf.size = blk_size;
        malloc_chn_buf.phy_addr = 0;
        ret = IOCTL(g_pciv_fd[chn], PCIV_MALLOC_CHN_BUF_CTRL, &malloc_chn_buf);
        if (ret != HI_SUCCESS) {
            break;
        }

        phy_addr[cnt] = malloc_chn_buf.phy_addr;
    }

    /*  if one block malloc failure then free all the memory  */
    if (ret != HI_SUCCESS) {
        cnt--;
        for (; cnt >= 0; cnt--) {
            (hi_void)IOCTL(g_pciv_fd[chn], PCIV_FREE_CHN_BUF_CTRL, &cnt);
            phy_addr[cnt] = 0;
        }
    }

    return ret;
}

MPI_STATIC hi_s32 hi_mpi_pciv_free_chn_buffer(hi_pciv_chn chn, hi_u32 blk_cnt)
{
    hi_u32 i;
    hi_s32 ret = HI_SUCCESS;
    PCIV_CHECK_CHNID(chn);
    PCIV_CHECK_OPEN(chn);
    if ((blk_cnt == 0) || (blk_cnt > PCIV_MAX_BUF_NUM)) {
        PCIV_ERR_TRACE("blk_cnt:%d is illegal!\n", blk_cnt);
        return HI_ERR_PCIV_ILLEGAL_PARAM;
    }
    for (i = 0; i < blk_cnt; i++) {
        ret = IOCTL(g_pciv_fd[chn], PCIV_FREE_CHN_BUF_CTRL, &i);
        if (ret != HI_SUCCESS) {
            break;
        }
    }

    return ret;
}

MPI_STATIC hi_s32 hi_mpi_pciv_get_local_id(hi_void)
{
    hi_s32 chip_id;

    PCIV_CHECK_OPEN(0);

    if (IOCTL(g_pciv_fd[0], PCIV_GETLOCALID_CTRL, &chip_id) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return chip_id;
}

MPI_STATIC hi_s32 hi_mpi_pciv_enum_chip(hi_s32 chip_id[PCIV_MAX_CHIPNUM])
{
    hi_ulong len;
    hi_s32 ret;
    hi_pciv_ioctl_enum_chip chip;

    PCIV_CHECK_OPEN(0);
    PCIV_CHECK_PTR(chip_id);
    if (IOCTL(g_pciv_fd[0], PCIV_ENUMCHIPID_CTRL, &chip) != HI_SUCCESS) {
        return HI_FAILURE;
    }
    len = (hi_ulong)sizeof(chip.chip_array) / sizeof(chip.chip_array[0]);
    if (len > PCIV_MAX_CHIPNUM || len < 0) {
        PCIV_ERR_TRACE("len:%lu is illegal!\n", len);
        return HI_FAILURE;
    }
    ret = memcpy_s(chip_id, PCIV_MAX_CHIPNUM, chip.chip_array, len);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

MPI_STATIC hi_s32 hi_mpi_pciv_get_base_window(hi_s32 chip_id, hi_pciv_base_window *base)
{
    PCIV_CHECK_OPEN(0);
    PCIV_CHECK_PTR(base);

    base->chip_id = chip_id;

    return IOCTL(g_pciv_fd[0], PCIV_GETBASEWINDOW_CTRL, base);
}

MPI_STATIC hi_s32 hi_mpi_pciv_win_vb_create(const hi_pciv_win_vb_cfg *cfg)
{
    PCIV_CHECK_OPEN(0);
    PCIV_CHECK_PTR(cfg);

    return IOCTL(g_pciv_fd[0], PCIV_WINVBCREATE_CTRL, cfg);
}

MPI_STATIC hi_s32 hi_mpi_pciv_win_vb_destroy(hi_void)
{
    PCIV_CHECK_OPEN(0);

    return IOCTL(g_pciv_fd[0], PCIV_WINVBDESTROY_CTRL);
}

MPI_STATIC hi_s32 hi_mpi_pciv_show(hi_pciv_chn chn)
{
    PCIV_CHECK_CHNID(chn);
    PCIV_CHECK_OPEN(chn);

    return IOCTL(g_pciv_fd[chn], PCIV_SHOW_CTRL);
}

MPI_STATIC hi_s32 hi_mpi_pciv_hide(hi_pciv_chn chn)
{
    PCIV_CHECK_CHNID(chn);
    PCIV_CHECK_OPEN(chn);

    return IOCTL(g_pciv_fd[chn], PCIV_HIDE_CTRL);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

