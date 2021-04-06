/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#ifndef __HIFB_DEF_H__
#define __HIFB_DEF_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define VO_BASE_ADDR       0x011100000
#define VOU_REG_SIZE       0x20000

#define VHD_REGS_LEN       0x1000 /* len of V0's regs */
#define VSD_REGS_LEN       0x1000
#define GFX_REGS_LEN       0x800
#define DHD_REGS_LEN       0x1000
#define GRF_REGS_LEN       0x200 /* len of GFX regs */

// Offset define
#define FDR_GFX_OFFSET     (0x200 / 4)

// architecture define
#define GFX_MAX            3
#define WBC_MAX            1

// --------------------
// Value define
// --------------------
#define ZME_HPREC          (1 << 20)
#define ZME_VPREC          (1 << 12)

// --------------------
// Defined for ZME
// --------------------
#define MAX_OFFSET         3
#define MIN_OFFSET         (-1)

/* vou device enumeration */
typedef enum hiVO_DEV_E {
    VO_DEV_DHD0 = 0, /* ultra high definition device */
    VO_DEV_DHD1 = 1, /* high definition device */
    VO_DEV_BUTT
} VO_DEV_E;

typedef enum {
    VO_LAYER_VHD0 = 0, /* V0 layer */
    VO_LAYER_VHD1 = 1, /* V1 layer */
    VO_LAYER_VHD2 = 2, /* V2 layer */

    VO_LAYER_G0 = 3, /* G0 layer */
    VO_LAYER_G1 = 4, /* G1 layer */
    VO_LAYER_G2 = 5,
    /* G2 layer */  // not used
    VO_LAYER_G3 = 6, /* G3 layer */

    VO_LAYER_WBC = 7, /* wbc layer */

    VO_LAYER_BUTT
} VO_LAYER_E;

typedef enum tagHAL_DISP_OUTPUTCHANNEL_E {
    HAL_DISP_CHANNEL_DHD0 = 0,
    HAL_DISP_CHANNEL_DHD1 = 1,

    HAL_DISP_CHANNEL_WBC = 2,

    HAL_DISP_CHANNEL_NONE = 3,

    HAL_DISP_CHANNEL_BUTT
} HAL_DISP_OUTPUTCHANNEL_E;

typedef enum tagHAL_DISP_LAYER_E {
    HAL_DISP_LAYER_VHD0 = 0,
    HAL_DISP_LAYER_VHD1 = 1,
    HAL_DISP_LAYER_VHD2 = 2,

    HAL_DISP_LAYER_GFX0 = 3,
    HAL_DISP_LAYER_GFX1 = 4,
    HAL_DISP_LAYER_GFX2 = 5,  // not used
    HAL_DISP_LAYER_GFX3 = 6,  // for hardware cursor

    HAL_DISP_LAYER_WBC = 7,

    HAL_DISP_LAYER_TT = 8,
    HAL_DISP_LAYER_BUTT,
    HAL_DISP_INVALID_LAYER = -1
} HAL_DISP_LAYER_E;

#define LAYER_GFX_START    HAL_DISP_LAYER_GFX0  // GFX0
#define LAYER_GFX_END      HAL_DISP_LAYER_GFX3  // GFX3

typedef enum tagHAL_DISP_PIXEL_FORMAT_E {
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_400 = 0x1,
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_420 = 0x2,
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_422 = 0x3,
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_444 = 0x4,
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_411_4X1 = 0x6,
    HAL_INPUTFMT_YCbCr_SEMIPLANAR_422_2X1 = 0x7,

    HAL_INPUTFMT_CbYCrY_PACKAGE_422 = 0x9,
    HAL_INPUTFMT_YCbYCr_PACKAGE_422 = 0xa,
    HAL_INPUTFMT_YCrYCb_PACKAGE_422 = 0xb,
    HAL_INPUTFMT_YCbCr_PACKAGE_444 = 0x1000,

    HAL_INPUTFMT_CLUT_1BPP = 0x00,
    HAL_INPUTFMT_CLUT_2BPP = 0x10,
    HAL_INPUTFMT_CLUT_4BPP = 0x20,
    HAL_INPUTFMT_CLUT_8BPP = 0x30,
    HAL_INPUTFMT_ACLUT_44 = 0x38,

    HAL_INPUTFMT_RGB_444 = 0x40,
    HAL_INPUTFMT_RGB_555 = 0x41,
    HAL_INPUTFMT_RGB_565 = 0x42,
    HAL_INPUTFMT_CbYCrY_PACKAGE_422_GRC = 0x43,
    HAL_INPUTFMT_YCbYCr_PACKAGE_422_GRC = 0x44,
    HAL_INPUTFMT_YCrYCb_PACKAGE_422_GRC = 0x45,
    HAL_INPUTFMT_ACLUT_88 = 0x46,
    HAL_INPUTFMT_ARGB_4444 = 0x48,
    HAL_INPUTFMT_ARGB_1555 = 0x49,

    HAL_INPUTFMT_RGB_888 = 0x50,
    HAL_INPUTFMT_YCbCr_888 = 0x51,
    HAL_INPUTFMT_ARGB_8565 = 0x5a,
    HAL_INPUTFMT_ARGB_6666 = 0x5b,

    HAL_INPUTFMT_KRGB_888 = 0x60,
    HAL_INPUTFMT_ARGB_8888 = 0x68,
    HAL_INPUTFMT_AYCbCr_8888 = 0x69,

    HAL_INPUTFMT_RGBA_4444 = 0xc8,
    HAL_INPUTFMT_RGBA_5551 = 0xc9,

    HAL_INPUTFMT_RGBA_6666 = 0xd8,
    HAL_INPUTFMT_RGBA_5658 = 0xda,

    HAL_INPUTFMT_RGBA_8888 = 0xe8,
    HAL_INPUTFMT_YCbCrA_8888 = 0xe9,

    HAL_DISP_PIXELFORMAT_BUTT

} HAL_DISP_PIXEL_FORMAT_E;

/*
* Name : HAL_CSC_MODE_E
* Desc : HAL Csc mode,CSC: color space convert.
*/
typedef enum hiHAL_CSC_MODE_E {
    HAL_CSC_MODE_NONE = 0,

    HAL_CSC_MODE_BT601_TO_BT601,
    HAL_CSC_MODE_BT709_TO_BT709,
    HAL_CSC_MODE_RGB_TO_RGB,

    HAL_CSC_MODE_BT601_TO_BT709,
    HAL_CSC_MODE_BT709_TO_BT601,

    HAL_CSC_MODE_BT601_TO_RGB_PC,
    HAL_CSC_MODE_BT709_TO_RGB_PC,
    HAL_CSC_MODE_BT2020_TO_RGB_PC,
    HAL_CSC_MODE_RGB_TO_BT601_PC,
    HAL_CSC_MODE_RGB_TO_BT709_PC,
    HAL_CSC_MODE_RGB_TO_BT2020_PC,

    HAL_CSC_MODE_BT601_TO_RGB_TV,
    HAL_CSC_MODE_BT709_TO_RGB_TV,
    HAL_CSC_MODE_RGB_TO_BT601_TV,
    HAL_CSC_MODE_RGB_TO_BT709_TV,

    HAL_CSC_MODE_BUTT
} HAL_CSC_MODE_E;

/* vou CBM MIXER */
typedef enum {
    HAL_CBMMIX1 = 0,
    HAL_CBMMIX2 = 1,
    HAL_CBMMIX3 = 2,

    HAL_CBMMIX1_BUTT
} HAL_CBMMIX_E;

/* vou graphic layer data extend mode */
typedef enum {
    HAL_GFX_BITEXTEND_1ST = 0,
    HAL_GFX_BITEXTEND_2ND = 0x2,
    HAL_GFX_BITEXTEND_3RD = 0x3,

    HAL_GFX_BITEXTEND_BUTT
} HAL_GFX_BITEXTEND_E;

typedef struct tagHAL_DISP_SYNCINFO_S {
    HI_U32 bSynm;
    HI_U32 bIop;
    HI_U8 u8Intfb;

    HI_U16 u16Vact;
    HI_U16 u16Vbb;
    HI_U16 u16Vfb;

    HI_U16 u16Hact;
    HI_U16 u16Hbb;
    HI_U16 u16Hfb;
    HI_U16 u16Hmid;

    HI_U16 u16Bvact;
    HI_U16 u16Bvbb;
    HI_U16 u16Bvfb;

    HI_U16 u16Hpw;
    HI_U16 u16Vpw;

    HI_U32 bIdv;
    HI_U32 bIhs;
    HI_U32 bIvs;
} HAL_DISP_SYNCINFO_S;

typedef enum tagHAL_DISP_INTF_E {

    HAL_DISP_INTF_CVBS = (0x01L << 0),
    HAL_DISP_INTF_HDDATE = (0x01L << 1),
    HAL_DISP_INTF_VGA = (0x01L << 2),
    HAL_DISP_INTF_BT656 = (0x01L << 3),
    HAL_DISP_INTF_BT1120 = (0x01L << 4),
    HAL_DISP_INTF_HDMI = (0x01L << 5),
    HAL_DISP_INTF_LCD = (0x01L << 6),
    HAL_DISP_INTF_DATE = (0x01L << 7),
    HAL_DISP_INTF_LCD_6BIT = (0x01L << 9),
    HAL_DISP_INTF_LCD_8BIT = (0x01L << 10),
    HAL_DISP_INTF_LCD_16BIT = (0x01L << 11),
    HAL_DISP_INTF_LCD_18BIT = (0x01L << 12),
    HAL_DISP_INTF_LCD_24BIT = (0x01L << 13),
    HAL_DISP_INTF_MIPI = (0x01L << 14),
    HAL_DISP_INTF_BUTT = (0x01L << 15),

} HAL_DISP_INTF_E;

typedef struct tagHAL_DISP_BKCOLOR_S {
    HI_U16 u16Bkg_a;
    HI_U16 u16Bkg_y;
    HI_U16 u16Bkg_cb;
    HI_U16 u16Bkg_cr;
} HAL_DISP_BKCOLOR_S;

/* vou graphic layer mask  */
typedef struct tagHAL_GFX_MASK_S {
    HI_U8 u8Mask_r;
    HI_U8 u8Mask_g;
    HI_U8 u8Mask_b;

} HAL_GFX_MASK_S;

typedef struct tagHAL_GFX_KEY_MAX_S {
    HI_U8 u8KeyMax_R;
    HI_U8 u8KeyMax_G;
    HI_U8 u8KeyMax_B;

} HAL_GFX_KEY_MAX_S;

typedef struct tagHAL_GFX_KEY_MIN_S {
    HI_U8 u8KeyMin_R;
    HI_U8 u8KeyMin_G;
    HI_U8 u8KeyMin_B;

} HAL_GFX_KEY_MIN_S;

/*******************************************************************
* Begin  :  For CMP(Compress) and DCMP(Decompress).
*******************************************************************/
typedef enum tagVDP_DATA_RMODE_E {
    VDP_RMODE_INTERFACE = 0,
    VDP_RMODE_INTERLACE = 0,
    VDP_RMODE_PROGRESSIVE = 1,
    VDP_RMODE_TOP = 2,
    VDP_RMODE_BOTTOM = 3,
    VDP_RMODE_PRO_TOP = 4,
    VDP_RMODE_PRO_BOTTOM = 5,
    VDP_RMODE_BUTT

} VDP_DATA_RMODE_E;

/*******************************************************************
* End    :    For CMP(Compress) and DCMP(Decompress).
*******************************************************************/
typedef enum tagVDP_CSC_MODE_E {
    VDP_CSC_YUV2YUV = 1,
    VDP_CSC_YUV2RGB_601,
    VDP_CSC_YUV2RGB_709,
    VDP_CSC_YUV2YUV_709_601,
    VDP_CSC_YUV2YUV_601_709,
    VDP_CSC_RGB2YUV_601,
    VDP_CSC_RGB2YUV_709,
    VDP_CSC_YUV2YUV_MAX,
    VDP_CSC_YUV2YUV_MIN,
    VDP_CSC_YUV2YUV_RAND,

    VDP_CSC_BUTT
} VDP_CSC_MODE_E;

typedef struct {
    HI_S32 csc_coef00;
    HI_S32 csc_coef01;
    HI_S32 csc_coef02;

    HI_S32 csc_coef10;
    HI_S32 csc_coef11;
    HI_S32 csc_coef12;

    HI_S32 csc_coef20;
    HI_S32 csc_coef21;
    HI_S32 csc_coef22;

} VDP_CSC_COEF_S;

typedef struct {
    HI_S32 csc_in_dc0;
    HI_S32 csc_in_dc1;
    HI_S32 csc_in_dc2;

    HI_S32 csc_out_dc0;
    HI_S32 csc_out_dc1;
    HI_S32 csc_out_dc2;
} VDP_CSC_DC_COEF_S;

typedef struct {
    HI_S32 csc_coef00;
    HI_S32 csc_coef01;
    HI_S32 csc_coef02;

    HI_S32 csc_coef10;
    HI_S32 csc_coef11;
    HI_S32 csc_coef12;

    HI_S32 csc_coef20;
    HI_S32 csc_coef21;
    HI_S32 csc_coef22;

    HI_S32 csc_in_dc0;
    HI_S32 csc_in_dc1;
    HI_S32 csc_in_dc2;

    HI_S32 csc_out_dc0;
    HI_S32 csc_out_dc1;
    HI_S32 csc_out_dc2;
} VDP_CSC_CFG_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* End of __VOU_DEF_H__ */

