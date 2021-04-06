/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2007-2019. All rights reserved.
 * Description: chip system private head file
 * Author: Hisilicon multimedia software group
 * Create: 2007-01-31
 */

#ifndef __MKP_SYS_H__
#define __MKP_SYS_H__

#include "mkp_ioctl.h"
#include "hi_common_adapt.h"
#include "hi_comm_sys_adapt.h"
#include "hi_comm_video_adapt.h"
#include "sys_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define SYS_CHECK_NULL_PTR(ptr)                        \
    do {                                               \
        if ((ptr) == NULL) {                           \
            HI_TRACE_SYS(HI_DBG_ERR, "Null point \n"); \
            return HI_ERR_SYS_NULL_PTR;                \
        }                                              \
    } while (0)

typedef struct {
    hi_mpp_chn src_chn;
    hi_mpp_chn dest_chn;
} sys_bind_args;

typedef struct {
    hi_mpp_chn src_chn;
    hi_mpp_bind_dest dest_chns;
} sys_bind_src_args;

typedef struct {
    hi_mpp_chn mpp_chn;
    hi_char mmz_name[MAX_MMZ_NAME_LEN];
} sys_mem_args;

typedef struct {
    hi_s32 hr_timer;
    hi_s32 rr_mode;
} kernel_config;

typedef struct {
    hi_u32 size;
    hi_u64 phy_addr;
    hi_void ATTRIBUTE *vir_addr;
} sys_mem_cache_info;

typedef struct {
    hi_vpss_venc_wrap_param wrap_param;
    hi_u32 buf_line;
} vpss_venc_wrap_args;

typedef enum {
    IOC_NR_SYS_INIT = 0,
    IOC_NR_SYS_EXIT,
    IOC_NR_SYS_SETCONFIG,
    IOC_NR_SYS_GETCONFIG,
    IOC_NR_SYS_INITPTSBASE,
    IOC_NR_SYS_SYNCPTS,
    IOC_NR_SYS_GETCURPTS,

    IOC_NR_SYS_BIND,
    IOC_NR_SYS_UNBIND,
    IOC_NR_SYS_GETBINDBYDEST,
    IOC_NR_SYS_GETBINDBYSRC,

    IOC_NR_SYS_MEM_SET,
    IOC_NR_SYS_MEM_GET,

    IOC_NR_SYS_GET_CUST_CODE,

    IOC_NR_SYS_GET_KERNELCONFIG,

    IOC_NR_SYS_GET_CHIPID,

    IOC_NR_SYS_SET_VIVPSS_MODE,
    IOC_NR_SYS_GET_VIVPSS_MODE,

    IOC_NR_SYS_SET_TUNING_CONNECT,
    IOC_NR_SYS_GET_TUNING_CONNECT,

    IOC_NR_SYS_MFLUSH_CACHE,

    IOC_NR_SYS_SET_SCALE_COEFF,
    IOC_NR_SYS_GET_SCALE_COEFF,

    IOC_NR_SYS_SET_TIME_ZONE,
    IOC_NR_SYS_GET_TIME_ZONE,

    IOC_NR_SYS_SET_GPS_INFO,
    IOC_NR_SYS_GET_GPS_INFO,

    IOC_NR_SYS_GET_VPSSVENC_WRAP_BUF_LINE,

    IOC_NR_SYS_SET_RAW_FRAME_COMPRESS_RATE,
    IOC_NR_SYS_GET_RAW_FRAME_COMPRESS_RATE,

#ifdef HI_DEBUG
    IOC_NR_SYS_SET_COMPRESS_RATE,
    IOC_NR_SYS_GET_COMPRESS_RATE,

    IOC_NR_SYS_SET_COMPRESSV2_RATE,
    IOC_NR_SYS_GET_COMPRESSV2_RATE,
#endif
} ioc_nr_sys;

#define SYS_INIT_CTRL                   _IO(IOC_TYPE_SYS, IOC_NR_SYS_INIT)
#define SYS_EXIT_CTRL                   _IO(IOC_TYPE_SYS, IOC_NR_SYS_EXIT)
#define SYS_SET_CONFIG_CTRL             _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SETCONFIG, hi_mpp_sys_config)
#define SYS_GET_CONFIG_CTRL             _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GETCONFIG, hi_mpp_sys_config)
#define SYS_INIT_PTSBASE                _IOW(IOC_TYPE_SYS, IOC_NR_SYS_INITPTSBASE, hi_u64)
#define SYS_SYNC_PTS                    _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SYNCPTS, hi_u64)
#define SYS_GET_CURPTS                  _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GETCURPTS, hi_u64)

#define SYS_BIND_CTRL                   _IOW(IOC_TYPE_SYS, IOC_NR_SYS_BIND, sys_bind_args)
#define SYS_UNBIND_CTRL                 _IOW(IOC_TYPE_SYS, IOC_NR_SYS_UNBIND, sys_bind_args)
#define SYS_GETBINDBYDEST               _IOWR(IOC_TYPE_SYS, IOC_NR_SYS_GETBINDBYDEST, sys_bind_args)
#define SYS_GETBINDBYSRC                _IOWR(IOC_TYPE_SYS, IOC_NR_SYS_GETBINDBYSRC, sys_bind_src_args)

#define SYS_MEM_SET_CTRL                _IOW(IOC_TYPE_SYS, IOC_NR_SYS_MEM_SET, sys_mem_args)
#define SYS_MEM_GET_CTRL                _IOWR(IOC_TYPE_SYS, IOC_NR_SYS_MEM_GET, sys_mem_args)

#define SYS_GET_CUST_CODE               _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_CUST_CODE, hi_u32)

#define SYS_GET_KERNELCONFIG            _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_KERNELCONFIG, kernel_config)

#define SYS_GET_CHIPID                  _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_CHIPID, hi_u32)

#define SYS_SET_TUNING_CONNECT          _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_TUNING_CONNECT, hi_s32)
#define SYS_GET_TUNING_CONNECT          _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_TUNING_CONNECT, hi_s32)

#define SYS_MFLUSH_CACHE                _IOW(IOC_TYPE_SYS, IOC_NR_SYS_MFLUSH_CACHE, sys_mem_cache_info)

#define SYS_SET_SCALE_COEFF             _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_SCALE_COEFF, hi_scale_coeff_info)
#define SYS_GET_SCALE_COEFF             _IOWR(IOC_TYPE_SYS, IOC_NR_SYS_GET_SCALE_COEFF, hi_scale_coeff_info)

#define SYS_SET_TIME_ZONE               _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_TIME_ZONE, hi_s32)
#define SYS_GET_TIME_ZONE               _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_TIME_ZONE, hi_s32)

#define SYS_SET_GPS_INFO                _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_GPS_INFO, hi_gps_info)
#define SYS_GET_GPS_INFO                _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_GPS_INFO, hi_gps_info)

#define SYS_SET_VIVPSS_MODE             _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_VIVPSS_MODE, hi_vi_vpss_mode)
#define SYS_GET_VIVPSS_MODE             _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_VIVPSS_MODE, hi_vi_vpss_mode)

#define SYS_GET_VPSSVENC_WRAP_BUF_LINE  _IOWR(IOC_TYPE_SYS, \
    IOC_NR_SYS_GET_VPSSVENC_WRAP_BUF_LINE, vpss_venc_wrap_args)

#define SYS_SET_RAW_FRAME_COMPRESS_RATE _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_RAW_FRAME_COMPRESS_RATE, \
    hi_raw_frame_compress_param)
#define SYS_GET_RAW_FRAME_COMPRESS_RATE _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_RAW_FRAME_COMPRESS_RATE, \
    hi_raw_frame_compress_param)

#ifdef HI_DEBUG
#define SYS_SET_COMPRESS_RATE           _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_COMPRESS_RATE, sys_compress_param)
#define SYS_GET_COMPRESS_RATE           _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_COMPRESS_RATE, sys_compress_param)

#define SYS_SET_COMPRESSV2_RATE         _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_COMPRESSV2_RATE, sys_compress_v2_param)
#define SYS_GET_COMPRESSV2_RATE         _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_COMPRESSV2_RATE, sys_compress_v2_param)
#endif

/* cur sender:VIU,VOU,VDEC,VPSS,AI
 * cur receive:VOU,VPSS,GRP,AO
 */
#define BIND_ADJUST_SRC_DEVID(mod_id, dev_id) \
    do {                                         \
        if ((mod_id) == HI_ID_VDEC) {           \
            (dev_id) = 0;                      \
        }                                        \
    } while (0)

#define BIND_ADJUST_SRC_CHNID(mod_id, chn_id) \
    do {                                         \
        if ((mod_id) == HI_ID_VO) {             \
            (chn_id) = 0;                      \
        }                                        \
    } while (0)

#define BIND_ADJUST_DEST_DEVID(mod_id, dev_id) \
    do {                                          \
        if ((mod_id) == HI_ID_VENC) {            \
            (dev_id) = 0;                       \
        }                                         \
    } while (0)

#define BIND_ADJUST_DEST_CHNID(mod_id, chn_id) \
    do {                                          \
        if ((mod_id) == HI_ID_VPSS) {            \
            (chn_id) = 0;                       \
        }                                         \
    } while (0)

hi_s32 sys_bind(hi_mpp_chn *bind_src, hi_mpp_chn *bind_dest);
hi_s32 sys_unbind(hi_mpp_chn *bind_src, hi_mpp_chn *bind_dest);
hi_s32 sys_get_bind_by_dest(hi_mpp_chn *dest_chn, hi_mpp_chn *src_chn, hi_bool inner_call);

hi_s32 sys_get_bind_num_by_src(hi_mpp_chn *src_chn, hi_u32 *bind_num);

hi_s32 sys_get_bind_by_dest_inner(hi_mpp_chn *dest_chn, hi_mpp_chn *src_chn);
hi_s32 sys_get_bind_by_src(hi_mpp_chn *src_chn, hi_mpp_bind_dest *bind_src);

hi_s32 sys_bind_register_sender(bind_sender_info *info);
hi_s32 sys_bind_unregister_sender(hi_mod_id mod_id);
hi_s32 sys_bind_send_data(hi_mod_id mod_id, hi_s32 dev_id, hi_s32 chn_id, hi_u32 flag,
                         mpp_data_type data_type, hi_void *v_data);

hi_s32 sys_bind_reset_data(hi_mod_id mod_id, hi_s32 dev_id,
                          hi_s32 chn_id, hi_void *v_data);

hi_s32 sys_bind_register_receiver(bind_receiver_info *info);
hi_s32 sys_bind_unregister_receiver(hi_mod_id mod_id);

hi_s32 sys_bind_init(hi_void);
hi_void sys_bind_exit(hi_void);

hi_s32 sys_bind_mod_init(hi_void);
hi_void sys_bind_mod_exit(hi_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MKP_SYS_H__ */

