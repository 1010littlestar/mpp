/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2008-2019. All rights reserved.
 * Description   : common struct definition for PCIV
 * Author        : Hisilicon multimedia software pciv
 * Created       : 2008/06/04
 */
#ifndef __HI_COMM_PCIV_ADAPT_H__
#define __HI_COMM_PCIV_ADAPT_H__

#include "hi_comm_video_adapt.h"
#include "hi_comm_pciv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef PCIV_CHN hi_pciv_chn;


typedef struct {
    hi_vi_dev vi_dev; /*  vi device number  */
    hi_vi_chn vi_chn; /*  vi channel number  */
} hi_pciv_vi_device;

typedef struct {
    hi_vo_dev vo_dev; /*  vo device number  */
    hi_vo_chn vo_chn; /*  vo channel number  */
} hi_pciv_vo_device;

typedef struct {
    hi_vdec_chn vdec_chn; /*  vedc channel number  */
} hi_pciv_vdec_device;

typedef struct {
    hi_vpss_grp vpss_grp; /*  vpss group number  */
    hi_vpss_chn vpss_chn; /*  vpss channel number  */
} hi_pciv_vpss_device;

typedef struct {
    hi_venc_chn venc_chn; /*  venc channel number  */
} hi_pciv_venc_device;

/*  bind type for pciv  */
typedef PCIV_BIND_TYPE_E hi_pciv_bind_type;

/*  status of share buff  */
typedef PCIV_BUFF_STATUS_E hi_pciv_buff_status;

/*  bind object struct for pciv  */
typedef struct {
    hi_bool             vpss_send;
    hi_pciv_bind_type   type; /*  bind type for pciv  */
    union {
        hi_pciv_vi_device   vi_device;
        hi_pciv_vo_device   vo_device;
        hi_pciv_vdec_device vdec_device;
        hi_pciv_vpss_device vpss_device;
        hi_pciv_venc_device venc_device;
    } hi_un_attach_obj;
} hi_pciv_bind_obj;

/*  remote pciv object  */
typedef struct {
    hi_s32      chip_id;  /*  remote pciv device id number  */
    hi_pciv_chn pciv_chn; /*  pciv channel number of remote pciv device  */
} hi_pciv_remote_obj;

/*  attribution of target picture   */
typedef struct {
    hi_u32             width;          /*  pciture width of pciv channel  */
    hi_u32             height;         /*  picture height of pciv channel  */
    hi_u32             stride[3];      /*  pciture stride of pciv channel  */
    hi_video_field     field;          /*  video frame field type of pciv channel  */
    hi_pixel_format    pixel_format;   /*  pixel format of pciture of pciv channel  */
    hi_dynamic_range   dynamic_range;  /*   */
    hi_compress_mode   compress_mode;  /*   */
    hi_video_format    video_format;   /*   */
} hi_pciv_pic_attr;

/*  attribution of pciv chn  */
typedef struct {
    hi_pciv_pic_attr    pic_attr;                   /*  picture attibute  */
    hi_s32              buf_chip;                   /*  the chip id which buffer is belong to  */
    hi_u32              blk_size;                   /*  vb size of receiver for preview  */
    hi_u32              count;                      /*  lenght of address list  */
    hi_u64              phy_addr[PCIV_MAX_BUF_NUM]; /*  address list for picture move  */
    hi_pciv_remote_obj  remote_obj;                 /*  remote pciv object  */
} hi_pciv_attr;


/*  mpp video buffer config for pci window  */
typedef struct {
    hi_u32 pool_count;                   /*  total number of video buffer pool   */
    hi_u32 blk_size[PCIV_MAX_VBCOUNT];  /*  size of video buffer pool  */
    hi_u32 blk_count[PCIV_MAX_VBCOUNT]; /*  number of video buffer pool  */
} hi_pciv_win_vb_cfg;

typedef struct {
    hi_s32 chip_id;      /*  pciv device number  */
    hi_u64 np_win_base;  /*  non-prefetch window pcie base address  */
    hi_u64 pf_win_base;  /*  prefetch window pcie base address  */
    hi_u64 cfg_win_base; /*  config window pcie base address  */
    hi_u64 pf_ahb_addr;  /*  prefetch window AHB base address  */
} hi_pciv_base_window;

typedef struct {
    hi_u64 src_addr; /*  source address of dma task  */
    hi_u64 dst_addr; /*  destination address of dma task  */
    hi_u32 blk_size; /*  data block size of dma task  */
} hi_pciv_dma_block;

typedef struct {
    hi_u32              count;  /*  total dma task number  */
    hi_bool             read;   /*  dam task is  read or write data  */
    hi_pciv_dma_block   *block;
} hi_pciv_dma_task;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
#endif /*  __HI_COMM_PCIV_H__  */

