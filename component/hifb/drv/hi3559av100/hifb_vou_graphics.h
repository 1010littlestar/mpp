/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#ifndef __HIFB_VOU_GRAPHICS_H__
#define __HIFB_VOU_GRAPHICS_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hifb.h"
#include "hifb_vou_drv.h"
#include "hi_comm_vo.h"
#include "hifb_graphic_hal.h"
#include "hifb_graphics_drv.h"

#define MDDRC_ZONE_MAX_NUM 32
#define HIFB_LINE_BUF      3840
#define HIFB_MAX_LAYER_NUM 3

typedef enum hiVO_BITEXT_MODE_E {
    VO_BITEXT_LOW_ZERO = 0x0,
    VO_BITEXT_LOW_HIGHBIT = 0x2,
    VO_BITEXT_LOW_HIGHBITS = 0x3,
    VO_BITEXT_BUTT
} VO_BITEXT_MODE_E;

typedef enum hiVO_COLORKEY_MODE_E {
    VO_COLORKEY_IN = 0x0,
    VO_COLORKEY_OUT = 0x1,
    VO_COLORKEY_BUTT
} VO_COLORKEY_MODE_E;

typedef enum hiVO_SCAN_MODE_E {
    VO_SCAN_MODE_INTERLACE = 0x0,
    VO_SCAN_MODE_PROGRESSIVE = 0x1,
    VO_SCAN_MODE_BUTT
} VO_SCAN_MODE_E;

typedef enum hiVO_DITHER_OUT_BITWIDTH_E {
    DITHER_OUT_BITWIDTH_8 = 0x0, /* dither output 8bit */
    DITHER_OUT_BITWIDTH_10 = 0x1, /* dither output 10bit */

    DITHER_OUT_BITWIDTH_BUTT
} VO_DITHER_OUT_BITWIDTH_E;

typedef struct tagVO_GFX_KEY_MAX_S {
    HI_U8 u8KeyMax_R;
    HI_U8 u8KeyMax_G;
    HI_U8 u8KeyMax_B;

} VO_GFX_KEY_MAX_S;

typedef struct tagVO_GFX_KEY_MIN_S {
    HI_U8 u8KeyMin_R;
    HI_U8 u8KeyMin_G;
    HI_U8 u8KeyMin_B;

} VO_GFX_KEY_MIN_S;

typedef enum {
    VO_GFX_BITEXTEND_1ST = 0,
    VO_GFX_BITEXTEND_2ND = 0x2,
    VO_GFX_BITEXTEND_3RD = 0x3,

    VO_GFX_BITEXTEND_BUTT
} VO_GFX_BITEXTEND_E;

typedef struct tagVO_GFX_MASK_S {
    HI_U8 u8Mask_r;
    HI_U8 u8Mask_g;
    HI_U8 u8Mask_b;

} VO_GFX_MASK_S;

typedef HI_S32 (*VO_FB_IntCallBack)(const HI_VOID *pParaml, HI_VOID *pParamr);

HI_VOID VOU_GraphicsEnableWbc(GRAPHIC_LAYER gfxLayer, HI_BOOL bEnable);
HI_S32 VOU_GraphicsCheckDDRDectectZone(GRAPHIC_LAYER gfxLayer, HI_U32 u32NewStartID, HI_U32 u32NewCnt,
                                       HI_U32 u32PreStartID, HI_U32 u32PreCnt);
HI_VOID VOU_GraphicsGetDDRZoneCfg(GRAPHIC_LAYER gfxLayer, HI_U32 *pu32StartID, HI_U32 *pu32Cnt);

HI_BOOL VOU_GraphicsEnableDcmp(GRAPHIC_LAYER gfxLayer, HI_BOOL bEnable);
HI_BOOL VOU_GraphicsGetDcmpEnableState(GRAPHIC_LAYER gfxLayer, HI_BOOL *pbEnable);
HI_BOOL VOU_GraphicsSetDcmpInfo(GRAPHIC_LAYER gfxLayer, VO_GRAPHIC_DCMP_INFO_S *pstDcmpInfo);

HI_BOOL VOU_GraphicsSetGfxExt(GRAPHIC_LAYER enLayer, VO_GFX_BITEXTEND_E enMode);
HI_BOOL VOU_GraphicsSetGfxPalpha(GRAPHIC_LAYER enLayer, HI_U32 bAlphaEn, HI_U32 bArange, HI_U8 u8Alpha0,
                                 HI_U8 u8Alpha1);
HI_BOOL VOU_GraphicsSetLayerGalpha(GRAPHIC_LAYER enLayer, HI_U8 u8Alpha0);
HI_BOOL VOU_GraphicsSetCscEn(GRAPHIC_LAYER enLayer, HI_BOOL bCscEn);
HI_BOOL VOU_GraphicsSetGfxAddr(GRAPHIC_LAYER enLayer, HI_U64 u64LAddr);
HI_BOOL VOU_GraphicsSetGfxStride(GRAPHIC_LAYER enLayer, HI_U16 u16pitch);

HI_BOOL VOU_GraphicsGetGfxPreMult(GRAPHIC_LAYER enLayer, HI_U32 *pbEnable);
HI_BOOL VOU_GraphicsSetGfxPreMult(GRAPHIC_LAYER enLayer, HI_U32 bEnable);
HI_BOOL VOU_GraphicsSetLayerDataFmt(GRAPHIC_LAYER enLayer, VO_DISP_PIXEL_FORMAT_E enDataFmt);
HI_BOOL VOU_GraphicsSetLayerInRect(GRAPHIC_LAYER enLayer, HIFB_RECT *pstRect);
HI_BOOL VOU_GraphicsSetLayerSrcImageReso(GRAPHIC_LAYER u32Layer, HIFB_RECT *pstRect);
HI_BOOL VOU_GraphicsSetLayerOutRect(GRAPHIC_LAYER enLayer, HIFB_RECT *pstRect);
HI_BOOL VOU_GraphicsSetColorKeyValue(GRAPHIC_LAYER enLayer, VO_GFX_KEY_MAX_S stKeyMax, VO_GFX_KEY_MIN_S stKeyMin);
HI_BOOL VOU_GraphicsSetColorKeyMask(GRAPHIC_LAYER enLayer, VO_GFX_MASK_S stMsk);

HI_BOOL VOU_GraphicsSetGfxKeyEn(GRAPHIC_LAYER enLayer, HI_U32 u32KeyEnable);
HI_BOOL VOU_GraphicsSetGfxKeyMode(GRAPHIC_LAYER enLayer, HI_U32 u32KeyOut);

HI_BOOL VOU_GraphicsSetRegUp(GRAPHIC_LAYER enLayer);
HI_BOOL VOU_GraphicsGetLayerGalpha(GRAPHIC_LAYER enLayer, HI_U8 *pu8Alpha0);
HI_BOOL VOU_GraphicsGetLayerDataFmt(GRAPHIC_LAYER enLayer, HI_U32 *pu32Fmt);
HI_BOOL VOU_GraphicsGetGfxStride(GRAPHIC_LAYER enLayer, HI_U32 *pu32GfxStride);
HI_BOOL VOU_GraphicsGetGfxAddr(GRAPHIC_LAYER enLayer, HI_U64 *pu64GfxAddr);

HI_S32 VOU_GraphicsGetDevMode(GRAPHIC_LAYER gfxLayer, VO_SCAN_MODE_E *pScanMode, HI_BOOL *pbFeildUpdate);
HI_VOID VOU_GraphicsVtthIntProcess(VO_DEV VoDev);

HI_S32 VOU_GraphicsInit(HI_VOID);
HI_S32 VOU_GraphicsDeInit(HI_VOID);
HI_S32 VOU_GraphicsResourceInit(HI_VOID);
HI_S32 VOU_GraphicsResourceDeInit(HI_VOID);
HI_S32 VOU_GraphicsOpenLayer(GRAPHIC_LAYER gfxLayer);
HI_S32 VOU_GraphicsCloseLayer(GRAPHIC_LAYER gfxLayer);
HI_S32 VOU_GraphicsEnableLayer(GRAPHIC_LAYER gfxLayer, HI_BOOL bEnable);
HI_S32 VOU_GraphicsSetCallback(GRAPHIC_LAYER gfxLayer, VO_FB_INT_TYPE_E enIntType, VO_FB_IntCallBack pCallBack,
                               HI_VOID *pArg);
HI_S32 VOU_GraphicsGetIntfSize(GRAPHIC_LAYER gfxLayer, HI_U32 *pu32Width, HI_U32 *pu32Height);
HI_S32 VOU_GraphicsGetIntfType(GRAPHIC_LAYER gfxLayer, VO_FB_INT_TYPE_E *penIntfType);

HI_S32 VOU_GraphicsSetCscCoef(GRAPHIC_LAYER gfxLayer);

HI_S32 VOU_GraphicsEnableInt(HI_U32 u32LayerIndex, HI_BOOL bEnable);
HI_BOOL VOU_GraphicsClearInt(HI_U32 u32IntClear, HI_S32 s32Irq);
HI_BOOL VOU_GraphicsGetInt(HI_U32 *pu32IntStaus);
HI_BOOL VOU_GraphicsClearIntStatus(HI_U32 u32IntStatus);
HI_S32 VOU_GraphicsGetInterruptDev(HI_U32 IntStatus, VO_DEV *pVoDev);
HI_BOOL VOU_GraphicsEnableGHDR(GRAPHIC_LAYER gfxLayer, HI_BOOL bEnable);
HI_BOOL VOU_GraphicsEnableZME(GRAPHIC_LAYER gfxLayer, RECT_S *stInRect, RECT_S *stOutRect, HI_BOOL bEnable);

#endif /* __VOU_GRAPHICS_H__ */

