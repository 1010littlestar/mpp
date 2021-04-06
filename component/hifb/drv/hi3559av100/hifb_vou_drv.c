/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/poll.h>
#include <linux/fb.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "hifb_vou_drv.h"
#include "hifb_def.h"
#include "hifb_vou_graphics.h"
#include "hi_tde_ext.h"
#include "mod_ext.h"

#define HIFB_MAX_LAYER_WIDTH_G0  3840
#define HIFB_MAX_LAYER_HEIGHT_G0 3840
#define HIFB_MIN_LAYER_WIDTH_G0  32
#define HIFB_MIN_LAYER_HEIGH_G0  32

#define HIFB_MAX_LAYER_WIDTH_G1  1920
#define HIFB_MAX_LAYER_HEIGTH_G1 1920
#define HIFB_MIN_LAYER_WIDTH_G1  32
#define HIFB_MIN_LAYER_HEIGTH_G1 32

#define HIFB_MAX_LAYER_WIDTH_G3  512
#define HIFB_MAX_LAYER_HEIGTH_G3 512
#define HIFB_MIN_LAYER_WIDTH_G3  2
#define HIFB_MIN_LAYER_HEIGTH_G3 2

#define GRAPHICS_LAYER_G0        0
#define GRAPHICS_LAYER_G1        1
#define GRAPHICS_LAYER_G3        2

HIFB_LAYER_S g_stLayer[HIFB_MAX_LAYER_NUM];

char *apszLayerMmzNames[HIFB_MAX_LAYER_NUM] = { [0 ... HIFB_MAX_LAYER_NUM - 1] = NULL };

module_param_array(apszLayerMmzNames, charp, NULL, S_IRUGO);
MODULE_PARM_DESC(apszLayerMmzNames, "The mmz names for the graphics layers.");

/*****************************************************************************
 Prototype       : Variable Definition
 Description     :

*****************************************************************************/
/*****************************************************************************
 Prototype       : Variable Definition
 Description     : hifb: a definition of the graphics layer capability set.
*****************************************************************************/
const HIFB_CAPABILITY_S s_astHifbGfxCap[HIFB_MAX_LAYER_NUM] = {
    /* G0 */
    {
        .bKeyRgb = 1,
        .bKeyAlpha = 0,
        .bGlobalAlpha = 1,
        .bCmap = 0,
        .bColFmt = {
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        },  // 25
        .u32MaxWidth = HIFB_MAX_LAYER_WIDTH_G0,
        .u32MaxHeight = HIFB_MAX_LAYER_HEIGHT_G0,
        .u32MinWidth = HIFB_MIN_LAYER_WIDTH_G0,
        .u32MinHeight = HIFB_MIN_LAYER_HEIGH_G0,
        .u32VDefLevel = 0,
        .u32HDefLevel = 0,
        .bLayerSupported = HI_TRUE,
        .bVoScale = HI_TRUE,
        .bHasCmapReg = 0,
        .bDcmp = 1,
        .bPreMul = 1,
        .bGHDR = 1,
    },
    /* G1 */
    {
        .bKeyRgb = 1,
        .bKeyAlpha = 0,
        .bGlobalAlpha = 1,
        .bCmap = 0,
        .bColFmt = {
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        },  // 25
        .u32MaxWidth = HIFB_MAX_LAYER_WIDTH_G1,
        .u32MaxHeight = HIFB_MAX_LAYER_HEIGTH_G1,
        .u32MinWidth = HIFB_MIN_LAYER_WIDTH_G1,
        .u32MinHeight = HIFB_MIN_LAYER_HEIGTH_G1,
        .u32VDefLevel = 0,
        .u32HDefLevel = 0,
        .bLayerSupported = HI_TRUE,
        .bVoScale = HI_FALSE,
        .bHasCmapReg = 0,
        .bDcmp = 1,
        .bPreMul = 1,
        .bGHDR = 0,
    },
    /* G3 */
    {
        .bKeyRgb = 1,
        .bKeyAlpha = 0,
        .bGlobalAlpha = 1,
        .bCmap = 0,
        .bColFmt = {
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        },  // 25
        .u32MaxWidth = HIFB_MAX_LAYER_WIDTH_G3,
        .u32MaxHeight = HIFB_MAX_LAYER_HEIGTH_G3,
        .u32MinWidth = HIFB_MIN_LAYER_WIDTH_G3,
        .u32MinHeight = HIFB_MIN_LAYER_HEIGTH_G3,
        .u32VDefLevel = 0,
        .u32HDefLevel = 0,
        .bLayerSupported = HI_TRUE,
        .bVoScale = HI_FALSE,
        .bHasCmapReg = 0,
        .bDcmp = 0,
        .bPreMul = 1,
        .bGHDR = 0,
    },
};

char video[64] = "hifb:vram0_size:8100";

/*
* the index number of fb and the actual graphics layer
* 0 - HAL_DISP_LAYER_GFX0
* 1 - HAL_DISP_LAYER_GFX1
* 2 - HAL_DISP_LAYER_GFX3
*/
static HAL_DISP_LAYER_E g_HifbLayerToHwLayer[HIFB_MAX_LAYER_NUM] = {
    HAL_DISP_LAYER_GFX0,
    HAL_DISP_LAYER_GFX1,
    HAL_DISP_LAYER_GFX3
};

HI_BOOL HIFB_DRV_SupportRotate()
{
    return HI_TRUE;
}

HI_BOOL IS_HD_LAYER(HI_U32 LayerId)
{
    return (LayerId == GRAPHICS_LAYER_G0) || (LayerId == GRAPHICS_LAYER_G1);
}

HI_BOOL IS_SD_LAYER(HI_U32 LayerId)
{
    return HI_FALSE;
}

HI_BOOL IS_AD_LAYER(HI_U32 LayerId)
{
    return HI_FALSE;
}

HI_BOOL IS_CURSOR_LAYER(HI_U32 LayerId)
{
    return (LayerId == GRAPHICS_LAYER_G3);
}

/* Frame decompression needs to be offset by 16 bytes;
line decompression does not need */
HI_U64 HIFB_DRV_GetDcmpOffsetAddr(HI_U64 u64DcmpAddr)
{
    return (u64DcmpAddr + 16);
}

HI_S32 HIFB_DRV_EnableLayer(HI_U32 u32LayerId, HI_BOOL bEnable)
{
    return VOU_GraphicsEnableLayer(g_HifbLayerToHwLayer[u32LayerId], bEnable);
}

/*
* Name : HIFB_DRV_LayerDefaultSetting
* Desc : The default settings for the layer, including the following:
            1.Color key mode
            2.Bit extension mode
            3.alpha, global alpha,CSC,CSC enable, get interface type
*/
HI_S32 HIFB_DRV_LayerDefaultSetting(HI_U32 u32LayerId)
{
    HAL_DISP_LAYER_E enDispLayer = g_HifbLayerToHwLayer[u32LayerId];

    if (s_astHifbGfxCap[u32LayerId].bKeyRgb || s_astHifbGfxCap[u32LayerId].bKeyAlpha) {
        VOU_GraphicsSetGfxKeyMode(enDispLayer, VO_COLORKEY_IN);
    }

    VOU_GraphicsSetGfxExt(enDispLayer, VO_BITEXT_LOW_HIGHBITS);

    VOU_GraphicsSetGfxPalpha(enDispLayer, HI_TRUE, HI_TRUE, GRAPHIC_ALPHA_OPACITY, GRAPHIC_ALPHA_OPACITY);

    VOU_GraphicsSetLayerGalpha(enDispLayer, GRAPHIC_ALPHA_OPACITY);

    VOU_GraphicsSetCscCoef(enDispLayer);

    VOU_GraphicsSetCscEn(enDispLayer, HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_SetLayerAddr(HI_U32 u32LayerId, HI_U64 u64Addr)
{
    if (VOU_GraphicsSetGfxAddr(g_HifbLayerToHwLayer[u32LayerId], u64Addr) == HI_FALSE) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_SetLayerStride(HI_U32 u32LayerId, HI_U32 u32Stride)
{
    if (VOU_GraphicsSetGfxStride(g_HifbLayerToHwLayer[u32LayerId], u32Stride) == HI_FALSE) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_SetLayerAlpha(HI_U32 u32LayerId, HIFB_ALPHA_S stAlpha)
{
    HI_U32 u32PreMultMode = 0;
    HAL_DISP_LAYER_E enDispLayer = g_HifbLayerToHwLayer[u32LayerId];

    if (u32LayerId >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }

    /* When the premultiply is enabled, the global alpha cannot be 1 */
    if (stAlpha.u8GlobalAlpha == 1) {
        if (VOU_GraphicsGetGfxPreMult(enDispLayer, &u32PreMultMode) == HI_FALSE) {
            return HI_FAILURE;
        }
        if (u32PreMultMode == 1) {
            HI_TRACE_FB(HI_DBG_ERR, "the global alpha can not set to 1 when the pre-mult mode is enable\n");
            return HI_FAILURE;
        }
    }
    if (stAlpha.bAlphaChannel) {
        VOU_GraphicsSetLayerGalpha(g_HifbLayerToHwLayer[u32LayerId], stAlpha.u8GlobalAlpha);
    } else {
        VOU_GraphicsSetLayerGalpha(g_HifbLayerToHwLayer[u32LayerId], GRAPHIC_ALPHA_OPACITY);
    }

    VOU_GraphicsSetGfxPalpha(g_HifbLayerToHwLayer[u32LayerId], stAlpha.bAlphaEnable,
                             HI_TRUE, stAlpha.u8Alpha0, stAlpha.u8Alpha1);

    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_SetLayerDataFmt(HI_U32 u32LayerId, HIFB_COLOR_FMT_E enDataFmt)
{
    VO_DISP_PIXEL_FORMAT_E enPixelFormatForHal;
    HAL_DISP_LAYER_E enDispLayer = g_HifbLayerToHwLayer[u32LayerId];

    switch (enDataFmt) {
        case HIFB_FMT_ARGB1555: {
            enPixelFormatForHal = HAL_INPUTFMT_ARGB_1555;
            /* Pre-multiply enable is not supported when the data format is ARGB1555 */
            VOU_GraphicsSetGfxPreMult(enDispLayer, HI_FALSE);
            break;
        }

        case HIFB_FMT_ARGB8888: {
            enPixelFormatForHal = HAL_INPUTFMT_ARGB_8888;
            break;
        }

        case HIFB_FMT_ARGB4444: {
            enPixelFormatForHal = HAL_INPUTFMT_ARGB_4444;
            break;
        }

        default:
        {
            HI_TRACE_FB(HI_DBG_ERR, "HiFB does not support this color format");
            return HI_FAILURE;
        }
    }

    if (VOU_GraphicsSetLayerDataFmt(enDispLayer, enPixelFormatForHal) == HI_FALSE) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_SetLayerSrcImageReso(HI_U32 enDispLayer, const HIFB_RECT *pstHifbRect)
{
    HIFB_RECT stHifbRect = {0};
    stHifbRect.x = pstHifbRect->x;
    stHifbRect.y = pstHifbRect->y;
    stHifbRect.w = pstHifbRect->w;
    stHifbRect.h = pstHifbRect->h;
    if (VOU_GraphicsSetLayerSrcImageReso(g_HifbLayerToHwLayer[enDispLayer], &stHifbRect) == HI_FALSE) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_SetLayerRect(HI_U32 u32LayerId, const HIFB_RECT *pstInputRect, const HIFB_RECT *pstOutputRect)
{
    HIFB_RECT Rect;
    VO_SCAN_MODE_E enScanMode = 0;
    HI_BOOL bFieldUpdate;

    Rect.x = pstOutputRect->x;
    Rect.y = pstOutputRect->y;
    Rect.w = pstOutputRect->w;
    Rect.h = pstOutputRect->h;

    if (VOU_GraphicsGetDevMode(g_HifbLayerToHwLayer[u32LayerId], &enScanMode, &bFieldUpdate) != HI_SUCCESS) {
        return HI_FAILURE;
    }

    if (enScanMode == VO_SCAN_MODE_INTERLACE) {
        /* When interlaced display, the ordinate and height are required to be even */
        if (((Rect.y & 0x1) == 1) || ((Rect.h & 0x1) == 1)) {
            HI_TRACE_FB(HI_DBG_ERR, "the height must be even for interlaced device\n");
            return HI_FAILURE;
        }
    }

    VOU_GraphicsSetLayerInRect(g_HifbLayerToHwLayer[u32LayerId], &Rect);
    VOU_GraphicsSetLayerOutRect(g_HifbLayerToHwLayer[u32LayerId], &Rect);

    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_SetLayKeyMask(HI_U32 u32LayerId, const HIFB_COLORKEYEX_S *pstColorkey)
{
    VO_GFX_KEY_MAX_S stKeyMaxRGB;
    VO_GFX_KEY_MIN_S stKeyMinRGB;
    VO_GFX_MASK_S stKeyMaskRGB;
    HAL_DISP_LAYER_E enDispLayer = g_HifbLayerToHwLayer[u32LayerId];

    if (pstColorkey->bKeyEnable) {
        if (VOU_GraphicsSetGfxKeyMode(enDispLayer, pstColorkey->u32KeyMode) == HI_FALSE) {
            return HI_FAILURE;
        }

        stKeyMaxRGB.u8KeyMax_R = pstColorkey->u8RedMax;
        stKeyMaxRGB.u8KeyMax_G = pstColorkey->u8GreenMax;
        stKeyMaxRGB.u8KeyMax_B = pstColorkey->u8BlueMax;

        stKeyMinRGB.u8KeyMin_R = pstColorkey->u8RedMin;
        stKeyMinRGB.u8KeyMin_G = pstColorkey->u8GreenMin;
        stKeyMinRGB.u8KeyMin_B = pstColorkey->u8BlueMin;

        VOU_GraphicsSetColorKeyValue(enDispLayer, stKeyMaxRGB, stKeyMinRGB);
        if (pstColorkey->bMaskEnable) {
            stKeyMaskRGB.u8Mask_r = pstColorkey->u8RedMask;
            stKeyMaskRGB.u8Mask_g = pstColorkey->u8GreenMask;
            stKeyMaskRGB.u8Mask_b = pstColorkey->u8BlueMask;
        } else {
            stKeyMaskRGB.u8Mask_r = 0;
            stKeyMaskRGB.u8Mask_g = 0;
            stKeyMaskRGB.u8Mask_b = 0;
        }
        VOU_GraphicsSetColorKeyMask(enDispLayer, stKeyMaskRGB);
    }

    if (VOU_GraphicsSetGfxKeyEn(enDispLayer, pstColorkey->bKeyEnable) == HI_FALSE) {
        return HI_FAILURE;
    }

    return HI_FAILURE;
}

HI_S32 HIFB_DRV_ColorConvert(const struct fb_var_screeninfo *pstVar, HIFB_COLORKEYEX_S *pCkey)
{
    HI_U8 r, g, b;
    HI_U32 u32Key = pCkey->u32Key;
    HI_U8 u8RMask, u8GMask, u8BMask;

    r = (u32Key >> pstVar->red.offset) << (8 - pstVar->red.length);
    g = (u32Key >> pstVar->green.offset) << (8 - pstVar->green.length);
    b = (u32Key >> pstVar->blue.offset) << (8 - pstVar->blue.length);

    u8RMask = (0xff >> pstVar->red.length);
    u8GMask = (0xff >> pstVar->green.length);
    u8BMask = (0xff >> pstVar->blue.length);

    pCkey->u8BlueMin = b;
    pCkey->u8GreenMin = g;
    pCkey->u8RedMin = r;

    pCkey->u8BlueMax = b | u8BMask;
    pCkey->u8GreenMax = g | u8BMask;
    pCkey->u8RedMax = r | u8BMask;

    /* low bits not using when do compare, after extend bits */
    pCkey->u8RedMask = ~u8RMask;
    pCkey->u8GreenMask = ~u8GMask;
    pCkey->u8BlueMask = ~u8BMask;

    pCkey->u32KeyMode = 0;

    pCkey->bMaskEnable = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_UpdataLayerReg(HI_U32 u32LayerId)
{
    if (VOU_GraphicsSetRegUp(g_HifbLayerToHwLayer[u32LayerId]) != HI_TRUE) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_SetLayerDeFlicker(HI_U32 u32LayerId, HIFB_DEFLICKER_S stDeFlicker)
{
    return HI_FAILURE;
}

HI_S32 HIFB_DRV_SetColorReg(HI_U32 u32LayerId, HI_U32 u32OffSet, HI_U32 u32Color, HI_S32 UpFlag)
{
    return HI_FAILURE;
}

HI_S32 HIFB_DRV_Init(HI_VOID)
{
    HI_S32 ret;
    hi_tde_export_func  *pTdeExportFunc = NULL;

    pTdeExportFunc = FUNC_ENTRY(hi_tde_export_func, HI_ID_TDE);
    if ((NULL == pTdeExportFunc) || (NULL == pTdeExportFunc->drv_tde_module_open))
    {
        HI_TRACE_FB(HI_DBG_ERR, "TDE pTdeExportFunc is NULL!\n");
    }
    else
    {
        ret = pTdeExportFunc->drv_tde_module_open();
        if(ret)
        {
            HI_TRACE_FB(HI_DBG_ERR, "TDE open failed\n");
            return ret;
        }
    }

    ret = VOU_GraphicsResourceInit();
    if (ret) {
        HI_TRACE_FB(HI_DBG_ERR, "resource init failed\n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_DeInit(HI_VOID)
{
    hi_tde_export_func  *pTdeExportFunc = NULL;

    pTdeExportFunc = FUNC_ENTRY(hi_tde_export_func, HI_ID_TDE);
    if ((NULL == pTdeExportFunc) || (NULL == pTdeExportFunc->drv_tde_module_close))
    {
        HI_TRACE_FB(HI_DBG_ERR, "TDE pTdeExportFunc is NULL!\n");
        return HI_FAILURE;
    }
    pTdeExportFunc->drv_tde_module_close();

    VOU_GraphicsResourceDeInit();

    return HI_SUCCESS;
}

/* set premul data */
HI_S32 HIFB_DRV_SetPreMul(HI_U32 u32LayerId, HI_BOOL bPreMul)
{
    HAL_DISP_PIXEL_FORMAT_E enPixelFormat = HAL_INPUTFMT_ARGB_1555;
    HAL_DISP_LAYER_E enLayer = g_HifbLayerToHwLayer[u32LayerId];
    HI_U8 u8GlobalAlpha;

    if (bPreMul == HI_TRUE) {
        /* When the global alpha configuration is 1, pre-multiplication cannot be enabled. */
        if (VOU_GraphicsGetLayerGalpha(enLayer, &u8GlobalAlpha) != HI_TRUE) {
            return HI_FAILURE;
        }
        if (u8GlobalAlpha == 1) {
            HI_TRACE_FB(HI_DBG_ERR, "not allow to set pre-mult mode when the GlobalAlpha is 1\n");
            return HI_FAILURE;
        }

        /* Pre-multiply enable is not supported when the data format is ARGB1555 */
        if (VOU_GraphicsGetLayerDataFmt(enLayer, &enPixelFormat) != HI_TRUE) {
            return HI_FAILURE;
        }
        if ((enPixelFormat == HAL_INPUTFMT_ARGB_1555) || (enPixelFormat == HAL_INPUTFMT_ARGB_4444)) {
            HI_TRACE_FB(HI_DBG_ERR, "not allow to set pre-mult mode when the color format is ARGB1555 or ARGB4444\n");
            return HI_FAILURE;
        }
    }

    if (VOU_GraphicsSetGfxPreMult(enLayer, bPreMul) != HI_TRUE) {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 HIFB_DRV_SetClutAddr(HI_U32 u32LayerId, HI_U32 u32PhyAddr)
{
    return HI_FAILURE;
}

HI_S32 HIFB_DRV_SetIntCallback(VO_FB_INT_TYPE_E eIntType, IntCallBack pCallBack,
                               HI_U32 u32LayerId, HI_VOID *pCallBackArg)
{
    if (u32LayerId >= HIFB_MAX_LAYER_NUM) {
        return HI_FAILURE;
    }

    return VOU_GraphicsSetCallback(g_HifbLayerToHwLayer[u32LayerId], eIntType, pCallBack, pCallBackArg);
}

/* The premise is that the VO device is enabled and an error is returned when the VO device is not enabled.
For X5HD, the current configuration content of the graphics layer is obtained for the boot screen;
for the 3531, since the boot screen is not required, only the device resolution and scan information
can be obtained. */
HI_S32 HIFB_DRV_GetOSDData(HI_U32 u32LayerId, HIFB_OSD_DATA_S *pstLayerData)
{
    HI_U32 u32Width = 0;
    HI_U32 u32Height = 0;
    HI_S32 ret;
    HAL_DISP_LAYER_E eLayer = g_HifbLayerToHwLayer[u32LayerId];
    VO_SCAN_MODE_E enScanMode = 0;

    // Get the maximum width and height assignment from the device context of the VO.
    ret = VOU_GraphicsGetIntfSize(eLayer, &u32Width, &u32Height);

    if (ret != HI_SUCCESS) {
        HI_TRACE_FB(HI_DBG_ERR, "Failed to get screen width and height!\n");
        return HI_FAILURE;
    }

    ret = VOU_GraphicsGetGfxAddr(eLayer, &pstLayerData->u64BufferPhyAddr);
    if (ret != HI_TRUE) {
        return HI_FAILURE;
    }

    ret = VOU_GraphicsGetGfxStride(eLayer, &pstLayerData->u32Stride);
    if (ret != HI_TRUE) {
        return HI_FAILURE;
    }

    ret = VOU_GraphicsGetDevMode(eLayer, &enScanMode, &pstLayerData->bFeildUpdate);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }
    if (enScanMode == VO_SCAN_MODE_INTERLACE) {
        pstLayerData->eScanMode = HIFB_SCANMODE_I;
    } else {
        pstLayerData->eScanMode = HIFB_SCANMODE_P;
    }

    pstLayerData->u32ScreenHeight = u32Height;
    pstLayerData->u32ScreenWidth = u32Width;

    return HI_SUCCESS;
}

HI_S32 HIFB_OpenLayer(HI_U32 u32LayerId)
{
    return VOU_GraphicsOpenLayer(g_HifbLayerToHwLayer[u32LayerId]);
}

HI_S32 HIFB_CloseLayer(HI_U32 u32LayerId)
{
    return VOU_GraphicsCloseLayer(g_HifbLayerToHwLayer[u32LayerId]);
}

/* open vo & close vo -- For internal debugging only
(for the API that does not adjust VO, the device can display the graphics layer) */
HI_S32 HIFB_DRV_OpenDisplay(HI_VOID)
{
    return HI_SUCCESS;
}

/* close vou */
HI_S32 HIFB_DRV_CloseDisplay(HI_VOID)
{
    return HI_SUCCESS;
}

hi_s32 TdeCalScaleRect_HIFB(const hi_tde_rect* pstSrcRect, const hi_tde_rect* pstDstRect,
                             hi_tde_rect* pstRectInSrc, hi_tde_rect* pstRectInDst)
{
        pstRectInDst->pos_x = pstRectInSrc->pos_x * pstDstRect->width / pstSrcRect->width;
        pstRectInDst->pos_y = pstRectInSrc->pos_y * pstDstRect->height/ pstSrcRect->height;
    pstRectInDst->height = pstDstRect->height;
    pstRectInDst->width = pstDstRect->width;
    return HI_SUCCESS;
}

HI_BOOL HIFB_DRV_EnableDcmp(HI_U32 u32LayerId, HI_BOOL bEnable)
{
    // See VOU_GraphicsEnableDcmp
    return VOU_GraphicsEnableDcmp(g_HifbLayerToHwLayer[u32LayerId], bEnable);
}

HI_BOOL HIFB_DRV_GetDcmpEnableState(HI_U32 u32LayerId, HI_BOOL *pbEnable)
{
    return VOU_GraphicsGetDcmpEnableState(g_HifbLayerToHwLayer[u32LayerId], pbEnable);
}

HI_BOOL HIFB_DRV_SetDcmpInfo(HI_U32 u32LayerId, VO_GRAPHIC_DCMP_INFO_S *pstDcmpInfo)
{
    if (pstDcmpInfo->u32Frame_Size0 == 0 || pstDcmpInfo->u32Frame_Size1 == 0) {
        GRAPHICS_DRV_TRACE(HI_DBG_WARN, "The compressed data is not ready yet\n");
        return HI_FALSE;
    }

    // VOU_GraphicsSetDcmpInfo
    return VOU_GraphicsSetDcmpInfo(g_HifbLayerToHwLayer[u32LayerId], pstDcmpInfo);
}

HI_BOOL HIFB_DRV_EnableGHDR(HI_U32 u32LayerId, HI_BOOL bEnable)
{
    HI_BOOL flags = HI_TRUE;
    HAL_DISP_LAYER_E enDispLayer = g_HifbLayerToHwLayer[u32LayerId];

    // VOU_GraphicsEnableGHDR
    if (enDispLayer == HAL_DISP_LAYER_GFX0) {
        flags = VOU_GraphicsEnableGHDR(enDispLayer, bEnable);
    } else {
        HI_TRACE_FB(HI_DBG_WARN, "G%d does not support GHDR.\n", u32LayerId);
        return HI_TRUE;
    }
    VOU_GraphicsSetCscEn(enDispLayer, HI_TRUE);

    return flags;
}

HI_BOOL HIFB_DRV_EnableZME(HI_U32 u32LayerId, HIFB_RECT *pstInRect, HIFB_RECT *pstOutRect, HI_BOOL bEnable)
{
    RECT_S stInRect;
    RECT_S stOutRect;

    stInRect.u32Width = pstInRect->w;
    stInRect.u32Height = pstInRect->h;
    stOutRect.u32Width = pstOutRect->w;
    stOutRect.u32Height = pstOutRect->h;

    if (g_HifbLayerToHwLayer[u32LayerId] == HAL_DISP_LAYER_GFX0) {
        // VOU_GraphicsEnableZME
        return VOU_GraphicsEnableZME(g_HifbLayerToHwLayer[u32LayerId], &stInRect, &stOutRect, bEnable);
    } else {
        if (bEnable) {
            HI_TRACE_FB(HI_DBG_ERR, "G%d does not support zme.\n", u32LayerId);
        }
        return HI_TRUE;
    }
}

HI_BOOL HIFB_DRV_IsLayerSupportZoomOut(HI_U32 u32LayerId)
{
    return HI_TRUE;
}

HI_VOID HIFB_DRV_EnableWbcInt(HI_U32 u32LayerId, HI_BOOL bEnable)
{
    return;
}

HI_VOID HIFB_DRV_SetWbcInfo(HI_U32 u32LayerId, VO_GRAPHIC_WBC_INFO_S *pstWbcInfo)
{
    return;
}

HI_VOID HIFB_DRV_EnableWbc(HI_U32 u32LayerId, HI_BOOL bEnable)
{
    return;
}

HI_VOID HIFB_DRV_GetWbcEnState(HI_U32 u32LayerId, HI_BOOL *pbWbcEn)
{
    return;
}

HI_VOID HIFB_DRV_GetWbcStopState(HI_U32 u32LayerId, HI_BOOL *pbWbcStop)
{
    return;
}

static HI_BOOL HIFB_DRV_IsLayerSupportDDRDetect(HI_U32 u32LayerId)
{
    return HI_FALSE;
}

static HI_VOID HIFB_DRV_SetDDR_DetectZone(HI_U32 u32StartID, HI_U32 u32Cnt, VO_MDDRC_ZONE_ADDR_S *pstZoneAddr)
{
    return;
}

static HI_VOID HIFB_DRV_GetMDDR_Status(HI_U32 u32StartID, HI_U32 u32Cnt, HI_U32 *pu32Status)
{
    return;
}

HI_VOID HIFB_DRV_ClearDDRDectectZone(HI_U32 u32StartID, HI_U32 u32Cnt)
{
    return;
}

HI_S32 HIFB_DRV_CheckDDR_DectectZone(HI_U32 u32LayerId, HI_U32 u32NewStartID,
                                     HI_U32 u32NewCnt, HI_U32 u32PreStartID, HI_U32 u32PreCnt)
{
    return HI_FAILURE;
}

HI_VOID HIFB_DRV_GetDDR_ZoneCfg(HI_U32 u32LayerId, HI_U32 *pu32StartID, HI_U32 *pu32Cnt)
{
    return;
}

HI_BOOL HIFB_DRV_GraphicsEnableInt(HI_U32 u32LayerId, HI_BOOL bEnable)
{
    HI_S32 s32Ret;

    s32Ret = VOU_GraphicsEnableInt(u32LayerId, bEnable);

    if (s32Ret == HI_SUCCESS) {
        return HI_TRUE;
    } else {
        return HI_FALSE;
    }
}

HI_BOOL HIFB_DRV_GraphicsClearInt(HI_U32 u32IntClear, HI_S32 s32Irq)
{
    return VOU_GraphicsClearInt(u32IntClear, s32Irq);
}
HI_BOOL HIFB_DRV_GraphicsGetInt(HI_U32 *pu32IntStatus)
{
    return VOU_GraphicsGetInt(pu32IntStatus);
}

HI_BOOL HIFB_DRV_GraphicsClearIntStatus(HI_U32 u32IntStatus)
{
    return VOU_GraphicsClearIntStatus(u32IntStatus);
}

HI_S32 HIFB_DRV_GraphicsGetInterruptDev(HI_U32 IntStatus, VO_DEV *pVoDev)
{
    return VOU_GraphicsGetInterruptDev(IntStatus, pVoDev);
}

HI_VOID HIFB_DRV_GetOps(HIFB_DRV_OPS_S *pstOps)
{
    pstOps->u32LayerCount = HIFB_MAX_LAYER_NUM;
    pstOps->pstCapability = (HIFB_CAPABILITY_S *)s_astHifbGfxCap;

    /* implement */
    pstOps->HIFB_DRV_Init = HIFB_DRV_Init;
    pstOps->HIFB_DRV_DeInit = HIFB_DRV_DeInit;
    pstOps->HIFB_DRV_EnableLayer = HIFB_DRV_EnableLayer;
    pstOps->HIFB_DRV_LayerDefaultSetting = HIFB_DRV_LayerDefaultSetting;
    pstOps->HIFB_DRV_SetLayerAddr = HIFB_DRV_SetLayerAddr;
    pstOps->HIFB_DRV_SetLayerStride = HIFB_DRV_SetLayerStride;
    pstOps->HIFB_DRV_SetLayerAlpha = HIFB_DRV_SetLayerAlpha;
    pstOps->HIFB_DRV_SetLayerDataFmt = HIFB_DRV_SetLayerDataFmt;
    pstOps->HIFB_DRV_SetLayerRect = HIFB_DRV_SetLayerRect;
    pstOps->HIFB_DRV_SetLayerSrcImageReso = HIFB_DRV_SetLayerSrcImageReso;
    pstOps->HIFB_DRV_SetLayerKeyMask = HIFB_DRV_SetLayKeyMask;
    pstOps->HIFB_DRV_ColorConvert = HIFB_DRV_ColorConvert;
    pstOps->HIFB_DRV_UpdataLayerReg = HIFB_DRV_UpdataLayerReg;
    pstOps->HIFB_DRV_SetIntCallback = HIFB_DRV_SetIntCallback;
    pstOps->HIFB_DRV_SetLayerDeFlicker = HIFB_DRV_SetLayerDeFlicker;
    pstOps->HIFB_DRV_SetColorReg = HIFB_DRV_SetColorReg;
    pstOps->HIFB_DRV_SetPreMul = HIFB_DRV_SetPreMul;
    pstOps->HIFB_DRV_SetClutAddr = HIFB_DRV_SetClutAddr;
    pstOps->HIFB_DRV_GetOSDData = HIFB_DRV_GetOSDData;
    pstOps->HIFB_DRV_OpenDisplay = HIFB_DRV_OpenDisplay;
    pstOps->HIFB_DRV_CloseDisplay = HIFB_DRV_CloseDisplay;
    pstOps->HIFB_OpenLayer = HIFB_OpenLayer;
    pstOps->HIFB_CloseLayer = HIFB_CloseLayer;

    pstOps->HIFB_DRV_EnableWbcInt = HIFB_DRV_EnableWbcInt;
    pstOps->HIFB_DRV_GetWbcStopState = HIFB_DRV_GetWbcStopState;
    pstOps->HIFB_DRV_EnableWbc = HIFB_DRV_EnableWbc;
    pstOps->HIFB_DRV_SetWbcInfo = HIFB_DRV_SetWbcInfo;
    pstOps->HIFB_DRV_GetWbcEnState = HIFB_DRV_GetWbcEnState;

    pstOps->HIFB_DRV_SetDcmpInfo = HIFB_DRV_SetDcmpInfo;
    pstOps->HIFB_DRV_EnableDcmp = HIFB_DRV_EnableDcmp;
    pstOps->HIFB_DRV_GetDcmpEnableState = HIFB_DRV_GetDcmpEnableState;

    pstOps->HIFB_DRV_IsLayerSupportDDRDetect = HIFB_DRV_IsLayerSupportDDRDetect;
    pstOps->HIFB_DRV_GetMDDR_Status = HIFB_DRV_GetMDDR_Status;
    pstOps->HIFB_DRV_SetMDDRDetectZone = HIFB_DRV_SetDDR_DetectZone;
    pstOps->HIFB_DRV_ClearDDRDectectZone = HIFB_DRV_ClearDDRDectectZone;
    pstOps->HIFB_DRV_CheckDDR_DectectZone = HIFB_DRV_CheckDDR_DectectZone;
    pstOps->HIFB_DRV_GetDDR_ZoneCfg = HIFB_DRV_GetDDR_ZoneCfg;

    // For GHDR
    pstOps->HIFB_DRV_EnableGHDR = HIFB_DRV_EnableGHDR;
    // For ZME
    pstOps->HIFB_DRV_EnableZME = HIFB_DRV_EnableZME;
    pstOps->HIFB_DRV_IsLayerSupportZoomOut = HIFB_DRV_IsLayerSupportZoomOut;
    // for Int
    pstOps->HIFB_DRV_GraphicsEnableINT = HIFB_DRV_GraphicsEnableInt;
    pstOps->HIFB_DRV_GraphicsClearINT = HIFB_DRV_GraphicsClearInt;
    pstOps->HIFB_DRV_GraphicsGetINT = HIFB_DRV_GraphicsGetInt;
    pstOps->HIFB_DRV_GraphicsClearINTStatus = HIFB_DRV_GraphicsClearIntStatus;
    pstOps->HIFB_DRV_GraphicsGetINTDev = HIFB_DRV_GraphicsGetInterruptDev;
}


