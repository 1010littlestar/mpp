/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2006-2019. All rights reserved.
 * Description   : MKP for PCIV
 * Author        : Hisilicon multimedia software group
 * Created       : 2006/04/05
 */
#ifndef __MKP_PCIV_H__
#define __MKP_PCIV_H__

#include "hi_comm_pciv_adapt.h"
#include "mkp_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    hi_s32 chip_array[PCIV_MAX_CHIPNUM];
} hi_pciv_ioctl_enum_chip;

typedef struct {
    hi_u64 phy_addr;
    hi_u32 size;
} hi_pciv_ioctl_malloc;

typedef struct {
    hi_u32 chn_id;
    hi_u32 index;
    hi_u64 phy_addr;
    hi_u32 size;
} hi_pciv_ioctl_malloc_chn_buf;


typedef enum {
    IOC_NR_PCIV_CREATE = 0,
    IOC_NR_PCIV_DESTROY,
    IOC_NR_PCIV_SETATTR,
    IOC_NR_PCIV_GETATTR,
    IOC_NR_PCIV_START,
    IOC_NR_PCIV_STOP,
    IOC_NR_PCIV_MALLOC,
    IOC_NR_PCIV_FREE,
    IOC_NR_PCIV_MALLOC_CHN_BUF,
    IOC_NR_PCIV_FREE_CHN_BUF,
    IOC_NR_PCIV_ENUMBINDOBJ,
    IOC_NR_PCIV_GETBASEWINDOW,
    IOC_NR_PCIV_GETLOCALID,
    IOC_NR_PCIV_ENUMCHIPID,
    IOC_NR_PCIV_DMATASK,
    IOC_NR_PCIV_BINDCHN2FD,
    IOC_NR_PCIV_WINVBCREATE,
    IOC_NR_PCIV_WINVBDESTROY,
    IOC_NR_PCIV_SHOW,
    IOC_NR_PCIV_HIDE,
} hi_ioc_nr_pciv;

#define PCIV_CREATE_CTRL         _IOW (IOC_TYPE_PCIV, IOC_NR_PCIV_CREATE, hi_pciv_attr)
#define PCIV_DESTROY_CTRL        _IO  (IOC_TYPE_PCIV, IOC_NR_PCIV_DESTROY)
#define PCIV_SETATTR_CTRL        _IOW (IOC_TYPE_PCIV, IOC_NR_PCIV_SETATTR, hi_pciv_attr)
#define PCIV_GETATTR_CTRL        _IOR (IOC_TYPE_PCIV, IOC_NR_PCIV_GETATTR, hi_pciv_attr)
#define PCIV_START_CTRL          _IO  (IOC_TYPE_PCIV, IOC_NR_PCIV_START)
#define PCIV_STOP_CTRL           _IO  (IOC_TYPE_PCIV, IOC_NR_PCIV_STOP)
#define PCIV_MALLOC_CTRL         _IOWR(IOC_TYPE_PCIV, IOC_NR_PCIV_MALLOC, hi_pciv_ioctl_malloc)
#define PCIV_FREE_CTRL           _IOW (IOC_TYPE_PCIV, IOC_NR_PCIV_FREE, hi_u64)
#define PCIV_MALLOC_CHN_BUF_CTRL _IOWR(IOC_TYPE_PCIV, IOC_NR_PCIV_MALLOC_CHN_BUF, hi_pciv_ioctl_malloc_chn_buf)
#define PCIV_FREE_CHN_BUF_CTRL   _IOW (IOC_TYPE_PCIV, IOC_NR_PCIV_FREE_CHN_BUF, hi_u32)
#define PCIV_DMATASK_CTRL        _IOW (IOC_TYPE_PCIV, IOC_NR_PCIV_DMATASK, hi_pciv_dma_task)
#define PCIV_GETBASEWINDOW_CTRL  _IOWR(IOC_TYPE_PCIV, IOC_NR_PCIV_GETBASEWINDOW, hi_pciv_base_window)
#define PCIV_GETLOCALID_CTRL     _IOR (IOC_TYPE_PCIV, IOC_NR_PCIV_GETLOCALID, hi_s32)
#define PCIV_ENUMCHIPID_CTRL     _IOR (IOC_TYPE_PCIV, IOC_NR_PCIV_ENUMCHIPID, hi_pciv_ioctl_enum_chip)
#define PCIV_BINDCHN2FD_CTRL     _IOW (IOC_TYPE_PCIV, IOC_NR_PCIV_BINDCHN2FD, hi_pciv_chn)
#define PCIV_WINVBCREATE_CTRL    _IOW (IOC_TYPE_PCIV, IOC_NR_PCIV_WINVBCREATE,  hi_pciv_win_vb_cfg)
#define PCIV_WINVBDESTROY_CTRL   _IO  (IOC_TYPE_PCIV, IOC_NR_PCIV_WINVBDESTROY)
#define PCIV_SHOW_CTRL           _IO  (IOC_TYPE_PCIV, IOC_NR_PCIV_SHOW)
#define PCIV_HIDE_CTRL           _IO  (IOC_TYPE_PCIV, IOC_NR_PCIV_HIDE)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



