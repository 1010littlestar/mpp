/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2012-09-19
 */

#include "hi_osal.h"
#include "hi_debug.h"
#include "hi_math.h"
#include "hifb_reg.h"
#include "hifb_graphics_drv.h"
#include "hifb_graphic_hal.h"
#include "hifb_def.h"
#include "hifb_coef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define HAL_PRINT       HI_PRINT

#define VOU_REGS_ADDR   0x11100000
#define VOU_REGS_SIZE   0x20000

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
volatile S_VDP_REGS_TYPE *g_pHifbReg = NULL;
volatile S_HIPP_REGS_TYPE *g_pHifbHippReg = NULL;

/*****************************************************************************
 Prototype       : Hal initial
 Description     :
*****************************************************************************/

HI_VOID HAL_VOU_Init(HI_VOID)
{
    HI_VOID *regs;

    if (!g_pHifbReg || !g_pHifbHippReg) {
        regs = osal_ioremap(VO_BASE_ADDR, (HI_U32)VOU_REG_SIZE);
        g_pHifbReg = (volatile S_VDP_REGS_TYPE *)regs;
        g_pHifbHippReg = (volatile S_HIPP_REGS_TYPE *)regs;
    }
}

/*****************************************************************************
 Prototype         : Hal exit
 Description       : exit
*****************************************************************************/
HI_VOID HAL_VOU_Exit(HI_VOID)
{
    if (g_pHifbReg != HI_NULL) {
        osal_iounmap((void *)g_pHifbReg);
        g_pHifbReg = HI_NULL;
    }
}

/*****************************************************************************
 Prototype         : HAL_WriteReg
 Description       : write reg
*****************************************************************************/
HI_VOID HAL_WriteReg(HI_U32 *pAddress, HI_U32 Value)
{
    *(volatile HI_U32 *)pAddress = Value;
    return;
}

/*****************************************************************************
 Prototype         : HAL_ReadReg
 Description       : read reg
*****************************************************************************/
HI_U32 HAL_ReadReg(HI_U32 *pAddress)
{
    return *(volatile HI_U32 *)(pAddress);
}

/*****************************************************************************
 Prototype         : HAL_GetXDC_BUMA
 Description       : TODO
*****************************************************************************/
HI_U32 inline HAL_GetXDC_BUMA(HI_S32 s32Value)
{
    HI_U32 u32AbsValue = s32Value << 2;

    if (s32Value >= 0) {
        return u32AbsValue & 0x7ff;
    } else { /* 0~10bit is a valid bit, and the 10th bit is a sign bit. */
        u32AbsValue = (~((-1) * u32AbsValue) + 1);
        u32AbsValue = ((u32AbsValue & 0x7ff) | 0x400);
        return u32AbsValue;
    }
}

HI_U32 inline HAL_Conver_CscCoef(HI_S32 s32Value)
{
    HI_S32 s32Result = ((s32Value << 10) / 1000);

    if (s32Result < 0) {
        s32Result = (~((-1) * s32Result) + 1);

        /* 5.8 format */
        s32Result = ((s32Result & 0x7fff) | 0x4000);
    } else {
        s32Result = s32Result & 0x7fff;
    }

    return (HI_U32)s32Result;
}

HI_U32 inline HAL_Conver_CscCoef_NEW(HI_S32 s32Value)
{
    HI_S32 s32Result = ((s32Value << 10) / 1000);

    if (s32Result < 0) {
        s32Result = (~((-1) * s32Result) + 1);

        s32Result = ((s32Result & 0xffff) | 0x8000);
    } else {
        s32Result = s32Result & 0xffff;
    }

    return (HI_U32)s32Result;
}

/*****************************************************************************
 Prototype         : Vou_GetAbsAddr
 Description       : Get the absolute address of the layer (video layer and graphics layer)
*****************************************************************************/
HI_UL Vou_GetAbsAddr(HAL_DISP_LAYER_E enLayer, HI_UL pReg)
{
    HI_UL RegAbsAddr;

    switch (enLayer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2: {
            RegAbsAddr = (pReg) + (enLayer - HAL_DISP_LAYER_VHD0) * VHD_REGS_LEN;
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3: {
            RegAbsAddr = (pReg) + (enLayer - HAL_DISP_LAYER_GFX0) * GFX_REGS_LEN;
            break;
        }

        // one wbc dev
        case HAL_DISP_LAYER_WBC:
            RegAbsAddr = (pReg);
            break;

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}

/*****************************************************************************
 Prototype         : Vou_GetChnAbsAddr
 Description       : Get the absolute address of the video channel
*****************************************************************************/
HI_UL Vou_GetChnAbsAddr(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_UL pReg)
{
    volatile HI_UL RegAbsAddr;

    switch (enChan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1: {
            RegAbsAddr = pReg + (enChan - HAL_DISP_CHANNEL_DHD0) * DHD_REGS_LEN;
            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}

HI_UL Vou_GetGfxAbsAddr(HAL_DISP_LAYER_E enLayer, HI_UL pReg)
{
    volatile HI_UL RegAbsAddr;

    switch (enLayer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3: {
            RegAbsAddr = pReg + (enLayer - HAL_DISP_LAYER_GFX0) * GRF_REGS_LEN;
            break;
        }
        default:
        {
            HAL_PRINT("Error layer id found in FUNC:%s,LINE:%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}

/*
* Name : HAL_DISP_GetIntfEnable
* Desc : Get the status (enable,disable status) of display interface.
*/
HI_BOOL HAL_DISP_GetIntfEnable(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL *pbIntfEn)
{
    volatile U_DHD0_CTRL DHD0_CTRL;
    volatile HI_UL addr_REG;

    switch (enChan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1: {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_CTRL.u32));
            DHD0_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);

            *pbIntfEn = DHD0_CTRL.bits.intf_en;
            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

// Get interface timing
HI_BOOL HAL_DISP_GetIntfSync(HAL_DISP_OUTPUTCHANNEL_E enChan,
                             HAL_DISP_SYNCINFO_S *pstSyncInfo)
{
    volatile U_DHD0_CTRL DHD0_CTRL;
    volatile U_DHD0_VSYNC1 DHD0_VSYNC1;
    volatile U_DHD0_VSYNC2 DHD0_VSYNC2;
    volatile U_DHD0_HSYNC1 DHD0_HSYNC1;
    volatile U_DHD0_HSYNC2 DHD0_HSYNC2;
    volatile U_DHD0_VPLUS1 DHD0_VPLUS1;
    volatile U_DHD0_VPLUS2 DHD0_VPLUS2;
    volatile U_DHD0_PWR DHD0_PWR;
    volatile HI_UL addr_REG;

    switch (enChan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1: {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_CTRL.u32));
            DHD0_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            pstSyncInfo->bIop = DHD0_CTRL.bits.iop;

            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_HSYNC1.u32));
            DHD0_HSYNC1.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            pstSyncInfo->u16Hact = DHD0_HSYNC1.bits.hact + 1;
            pstSyncInfo->u16Hbb = DHD0_HSYNC1.bits.hbb + 1;

            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_HSYNC2.u32));
            DHD0_HSYNC2.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            pstSyncInfo->u16Hmid = (DHD0_HSYNC2.bits.hmid == 0) ? 0 : DHD0_HSYNC2.bits.hmid + 1;
            pstSyncInfo->u16Hfb = DHD0_HSYNC2.bits.hfb + 1;

            // Config VHD interface veritical timming
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_VSYNC1.u32));
            DHD0_VSYNC1.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            pstSyncInfo->u16Vact = DHD0_VSYNC1.bits.vact + 1;
            pstSyncInfo->u16Vbb = DHD0_VSYNC1.bits.vbb + 1;

            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_VSYNC2.u32));
            DHD0_VSYNC2.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            pstSyncInfo->u16Vfb = DHD0_VSYNC2.bits.vfb + 1;

            // Config VHD interface veritical bottom timming,no use in progressive mode
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_VPLUS1.u32));
            DHD0_VPLUS1.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            pstSyncInfo->u16Bvact = DHD0_VPLUS1.bits.bvact + 1;
            pstSyncInfo->u16Bvbb = DHD0_VPLUS1.bits.bvbb + 1;

            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_VPLUS2.u32));
            DHD0_VPLUS2.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            pstSyncInfo->u16Bvfb = DHD0_VPLUS2.bits.bvfb + 1;

            // Config VHD interface veritical bottom timming,
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_PWR.u32));
            DHD0_PWR.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            pstSyncInfo->u16Hpw = DHD0_PWR.bits.hpw + 1;
            pstSyncInfo->u16Vpw = DHD0_PWR.bits.vpw + 1;

            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/*****************************************************************************
 Prototype         : HAL_DISP_GetDispIoP
 Description       : Interlace or Progressive
*****************************************************************************/
HI_BOOL HAL_DISP_GetDispIoP(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL *pbIop)
{
    U_DHD0_CTRL DHD0_CTRL;
    volatile HI_UL addr_REG;

    switch (enChan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1: {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_CTRL.u32));
            DHD0_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            *pbIop = DHD0_CTRL.bits.iop;
            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/* Get the mux data selection of the output interface */
HI_BOOL HAL_DISP_GetIntfMuxSel(HAL_DISP_OUTPUTCHANNEL_E enChan, VO_INTF_TYPE_E *pbenIntfType)
{
    volatile U_VO_MUX VO_MUX;
    if (enChan > HAL_DISP_CHANNEL_DHD1) {
        HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    VO_MUX.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->VO_MUX.u32));

    switch (VO_MUX.bits.digital_sel) {
        case 0: {
            *pbenIntfType = HAL_DISP_INTF_BT1120;
            break;
        }
        case 1: {
            *pbenIntfType = HAL_DISP_INTF_BT656;
            break;
        }
        case 2: {
            *pbenIntfType = HAL_DISP_INTF_LCD;
            break;
        }

        default:
        {
            HI_PRINT("Error! DP_DISP_GetIntfMuxSel Wrong Select\n");
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/* Get display channel interrupt generation mode */
HI_BOOL HAL_DISP_GetVtThdMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL *pbFieldMode)
{
    volatile U_DHD0_VTTHD DHD0_VTTHD;
    volatile HI_UL addr_REG;

    switch (enChan) {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1: {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (g_pHifbReg->DHD0_VTTHD.u32));
            DHD0_VTTHD.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            *pbFieldMode = DHD0_VTTHD.bits.thd1_mode;
            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/*
* Name : HAL_DISP_SetIntMask
* Desc : Set intterupt mask to open or close intterupt.
*/
HI_BOOL HAL_DISP_SetIntMask(HI_U32 u32MaskEn)
{
    volatile U_VOINTMSK1 VOINTMSK1;
    /* Dispaly interrupt mask enable */
    VOINTMSK1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->VOINTMSK1.u32));
    VOINTMSK1.u32 = VOINTMSK1.u32 | u32MaskEn;

    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->VOINTMSK1.u32), VOINTMSK1.u32);
    VOINTMSK1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->VOINTMSK1.u32));

    return HI_TRUE;
}

HI_BOOL HAL_DISP_ClrIntMask(HI_U32 u32MaskEn)
{
    volatile U_VOINTMSK1 VOINTMSK1;

    /* Dispaly interrupt mask enable */
    VOINTMSK1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->VOINTMSK1.u32));
    VOINTMSK1.u32 = VOINTMSK1.u32 & (~u32MaskEn);
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->VOINTMSK1.u32), VOINTMSK1.u32);

    return HI_TRUE;
}

// Get interrupt status
HI_U32 HAL_DISP_GetIntStatus(HI_U32 u32IntMsk)
{
    volatile U_VOMSKINTSTA1 VOMSKINTSTA1;

    /* read interrupt status */
    VOMSKINTSTA1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->VOMSKINTSTA1.u32));

    return (VOMSKINTSTA1.u32 & u32IntMsk);
}

/*
* Name : HAL_DISP_ClearIntStatus
* Desc : Clear interrupt status.
*/
HI_BOOL HAL_DISP_ClearIntStatus(HI_U32 u32IntMsk)
{
    /* read interrupt status */
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->VOMSKINTSTA.u32), u32IntMsk);
    return HI_TRUE;
}

// Set the video display area window
HI_BOOL HAL_VIDEO_SetLayerDispRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect)
{
    volatile U_G0_DFPOS G0_DFPOS;
    volatile U_G0_DLPOS G0_DLPOS;
    volatile HI_UL addr_REG;

    switch (enLayer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3: {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->G0_DFPOS.u32));
            G0_DFPOS.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            G0_DFPOS.bits.disp_xfpos = pstRect->x;
            G0_DFPOS.bits.disp_yfpos = pstRect->y;
            HAL_WriteReg((HI_U32 *)addr_REG, G0_DFPOS.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->G0_DLPOS.u32));
            G0_DLPOS.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            G0_DLPOS.bits.disp_xlpos = pstRect->x + pstRect->w - 1;
            G0_DLPOS.bits.disp_ylpos = pstRect->y + pstRect->h - 1;
            HAL_WriteReg((HI_U32 *)addr_REG, G0_DLPOS.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

// Set the video image display area window
HI_BOOL HAL_VIDEO_SetLayerVideoRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect)
{
    volatile U_G0_VFPOS G0_VFPOS;
    volatile U_G0_VLPOS G0_VLPOS;
    volatile HI_UL addr_REG;

    switch (enLayer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3: {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->G0_VFPOS.u32));
            G0_VFPOS.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            G0_VFPOS.bits.video_xfpos = pstRect->x;
            G0_VFPOS.bits.video_yfpos = pstRect->y;
            HAL_WriteReg((HI_U32 *)addr_REG, G0_VFPOS.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->G0_VLPOS.u32));
            G0_VLPOS.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            G0_VLPOS.bits.video_xlpos = pstRect->x + pstRect->w - 1;
            G0_VLPOS.bits.video_ylpos = pstRect->y + pstRect->h - 1;
            HAL_WriteReg((HI_U32 *)addr_REG, G0_VLPOS.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id %d# found in %s,%s: L%d\n", enLayer, __FILE__, __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_VOID HAL_LINK_GetHcLink(HI_U32 *pu32Data)
{
    U_LINK_CTRL LINK_CTRL;

    LINK_CTRL.u32 = HAL_ReadReg((HI_U32 *)(HI_UL) & (g_pHifbReg->LINK_CTRL.u32));
    *pu32Data = LINK_CTRL.bits.hc_link;

    return;
}

/*
* Name : HAL_LAYER_EnableLayer
* Desc : Set layer enable
*/
HI_BOOL HAL_LAYER_EnableLayer(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    volatile U_G0_CTRL G0_CTRL;
    volatile HI_UL addr_REG;

    switch (enLayer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3: {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->G0_CTRL.u32));
            G0_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            G0_CTRL.bits.surface_en = bEnable;
            HAL_WriteReg((HI_U32 *)addr_REG, G0_CTRL.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/*
* Name : HAL_LAYER_SetLayerDataFmt
* Desc : Set layer data type
*/
HI_BOOL HAL_LAYER_SetLayerDataFmt(HAL_DISP_LAYER_E enLayer,
                                  HAL_DISP_PIXEL_FORMAT_E enDataFmt)
{
    volatile U_GFX_SRC_INFO GFX_SRC_INFO;
    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_SRC_INFO.u32));
        GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_SRC_INFO.bits.ifmt = enDataFmt;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_SRC_INFO.u32);
    } else {
        HAL_PRINT("Error layer id found in FILE:%s,FUNC:%s,LINE%d\n", __FILE__, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

/*
* Name : HAL_LAYER_SetSrcResolution
* Desc : Set the original image resolution, which can be used to
*        write back other modules such as compressed reads.
*/
HI_BOOL HAL_LAYER_SetSrcResolution(HAL_DISP_LAYER_E enLayer,
                                   HIFB_RECT *pstRect)
{
    U_GFX_SRC_RESO GFX_SRC_RESO;
    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_SRC_RESO.u32));
        GFX_SRC_RESO.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_SRC_RESO.bits.src_w = pstRect->w - 1;
        GFX_SRC_RESO.bits.src_h = pstRect->h - 1;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_SRC_RESO.u32);
    } else {
        HAL_PRINT("Error:layer id not found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_GetLayerDataFmt(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32Fmt)
{
    volatile U_GFX_SRC_INFO GFX_SRC_INFO;
    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_SRC_INFO.u32));
        GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        *pu32Fmt = GFX_SRC_INFO.bits.ifmt;
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

/*
* Name : HAL_LAYER_SetCscCoef
* Desc : Set csc coefficient.
*/
HI_BOOL HAL_LAYER_SetCscCoef(HAL_DISP_LAYER_E enLayer, CscCoef_S *pstCscCoef)
{
    // for G0
    volatile U_HIHDR_G_RGB2YUV_COEF0 HIHDR_G_RGB2YUV_COEF0;
    volatile U_HIHDR_G_RGB2YUV_COEF1 HIHDR_G_RGB2YUV_COEF1;
    volatile U_HIHDR_G_RGB2YUV_COEF2 HIHDR_G_RGB2YUV_COEF2;
    volatile U_HIHDR_G_RGB2YUV_COEF3 HIHDR_G_RGB2YUV_COEF3;
    volatile U_HIHDR_G_RGB2YUV_COEF4 HIHDR_G_RGB2YUV_COEF4;
    volatile U_HIHDR_G_RGB2YUV_COEF5 HIHDR_G_RGB2YUV_COEF5;
    volatile U_HIHDR_G_RGB2YUV_COEF6 HIHDR_G_RGB2YUV_COEF6;
    volatile U_HIHDR_G_RGB2YUV_COEF7 HIHDR_G_RGB2YUV_COEF7;
    volatile U_HIHDR_G_RGB2YUV_COEF8 HIHDR_G_RGB2YUV_COEF8;

    volatile U_HIHDR_G_RGB2YUV_IDC0 HIHDR_G_RGB2YUV_IDC0;
    volatile U_HIHDR_G_RGB2YUV_IDC1 HIHDR_G_RGB2YUV_IDC1;
    volatile U_HIHDR_G_RGB2YUV_IDC2 HIHDR_G_RGB2YUV_IDC2;
    volatile U_HIHDR_G_RGB2YUV_ODC0 HIHDR_G_RGB2YUV_ODC0;
    volatile U_HIHDR_G_RGB2YUV_ODC1 HIHDR_G_RGB2YUV_ODC1;
    volatile U_HIHDR_G_RGB2YUV_ODC2 HIHDR_G_RGB2YUV_ODC2;

    // for G1
    volatile U_G1_CSC_IDC G1_CSC_IDC;
    volatile U_G1_CSC_ODC G1_CSC_ODC;
    volatile U_G1_CSC_IODC G1_CSC_IODC;

    volatile U_G1_CSC_P0 G1_CSC_P0;
    volatile U_G1_CSC_P1 G1_CSC_P1;
    volatile U_G1_CSC_P2 G1_CSC_P2;
    volatile U_G1_CSC_P3 G1_CSC_P3;
    volatile U_G1_CSC_P4 G1_CSC_P4;

    // for G3
    volatile U_G3_CSC_IDC G3_CSC_IDC;
    volatile U_G3_CSC_ODC G3_CSC_ODC;
    volatile U_G3_CSC_IODC G3_CSC_IODC;

    volatile U_G3_CSC_P0 G3_CSC_P0;
    volatile U_G3_CSC_P1 G3_CSC_P1;
    volatile U_G3_CSC_P2 G3_CSC_P2;
    volatile U_G3_CSC_P3 G3_CSC_P3;
    volatile U_G3_CSC_P4 G3_CSC_P4;

    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0) {
        // coef
        HIHDR_G_RGB2YUV_COEF0.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF0.u32));
        HIHDR_G_RGB2YUV_COEF0.bits.hihdr_r2y_coef00 = HAL_Conver_CscCoef_NEW(pstCscCoef->csc_coef00);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF0.u32), HIHDR_G_RGB2YUV_COEF0.u32);

        HIHDR_G_RGB2YUV_COEF1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF1.u32));
        HIHDR_G_RGB2YUV_COEF1.bits.hihdr_r2y_coef01 = HAL_Conver_CscCoef_NEW(pstCscCoef->csc_coef01);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF1.u32), HIHDR_G_RGB2YUV_COEF1.u32);

        HIHDR_G_RGB2YUV_COEF2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF2.u32));
        HIHDR_G_RGB2YUV_COEF2.bits.hihdr_r2y_coef02 = HAL_Conver_CscCoef_NEW(pstCscCoef->csc_coef02);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF2.u32), HIHDR_G_RGB2YUV_COEF2.u32);

        HIHDR_G_RGB2YUV_COEF3.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF3.u32));
        HIHDR_G_RGB2YUV_COEF3.bits.hihdr_r2y_coef10 = HAL_Conver_CscCoef_NEW(pstCscCoef->csc_coef10);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF3.u32), HIHDR_G_RGB2YUV_COEF3.u32);

        HIHDR_G_RGB2YUV_COEF4.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF4.u32));
        HIHDR_G_RGB2YUV_COEF4.bits.hihdr_r2y_coef11 = HAL_Conver_CscCoef_NEW(pstCscCoef->csc_coef11);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF4.u32), HIHDR_G_RGB2YUV_COEF4.u32);

        HIHDR_G_RGB2YUV_COEF5.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF5.u32));
        HIHDR_G_RGB2YUV_COEF5.bits.hihdr_r2y_coef12 = HAL_Conver_CscCoef_NEW(pstCscCoef->csc_coef12);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF5.u32), HIHDR_G_RGB2YUV_COEF5.u32);

        HIHDR_G_RGB2YUV_COEF6.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF6.u32));
        HIHDR_G_RGB2YUV_COEF6.bits.hihdr_r2y_coef20 = HAL_Conver_CscCoef_NEW(pstCscCoef->csc_coef20);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF6.u32), HIHDR_G_RGB2YUV_COEF6.u32);

        HIHDR_G_RGB2YUV_COEF7.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF7.u32));
        HIHDR_G_RGB2YUV_COEF7.bits.hihdr_r2y_coef21 = HAL_Conver_CscCoef_NEW(pstCscCoef->csc_coef21);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF7.u32), HIHDR_G_RGB2YUV_COEF7.u32);

        HIHDR_G_RGB2YUV_COEF8.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF8.u32));
        HIHDR_G_RGB2YUV_COEF8.bits.hihdr_r2y_coef22 = HAL_Conver_CscCoef_NEW(pstCscCoef->csc_coef22);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_COEF8.u32), HIHDR_G_RGB2YUV_COEF8.u32);

        // idc
        HIHDR_G_RGB2YUV_IDC0.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_IDC0.u32));
        HIHDR_G_RGB2YUV_IDC0.bits.hihdr_r2y_idc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc0);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_IDC0.u32), HIHDR_G_RGB2YUV_IDC0.u32);

        HIHDR_G_RGB2YUV_IDC1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_IDC1.u32));
        HIHDR_G_RGB2YUV_IDC1.bits.hihdr_r2y_idc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc1);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_IDC1.u32), HIHDR_G_RGB2YUV_IDC1.u32);

        HIHDR_G_RGB2YUV_IDC2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_IDC2.u32));
        HIHDR_G_RGB2YUV_IDC2.bits.hihdr_r2y_idc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc2);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_IDC2.u32), HIHDR_G_RGB2YUV_IDC2.u32);

        // odc
        HIHDR_G_RGB2YUV_ODC0.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_ODC0.u32));
        HIHDR_G_RGB2YUV_ODC0.bits.hihdr_r2y_odc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc0);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_ODC0.u32), HIHDR_G_RGB2YUV_ODC0.u32);

        HIHDR_G_RGB2YUV_ODC1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_ODC1.u32));
        HIHDR_G_RGB2YUV_ODC1.bits.hihdr_r2y_odc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc1);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_ODC1.u32), HIHDR_G_RGB2YUV_ODC1.u32);

        HIHDR_G_RGB2YUV_ODC2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_ODC2.u32));
        HIHDR_G_RGB2YUV_ODC2.bits.hihdr_r2y_odc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc2);
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_ODC2.u32), HIHDR_G_RGB2YUV_ODC2.u32);
    } else if (enLayer == HAL_DISP_LAYER_GFX1) {
        addr_REG = (HI_UL) & (g_pHifbReg->G1_CSC_IDC.u32);
        G1_CSC_IDC.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G1_CSC_IDC.bits.cscidc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc1);
        G1_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc0);
        HAL_WriteReg((HI_U32 *)addr_REG, G1_CSC_IDC.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G1_CSC_ODC.u32);
        G1_CSC_ODC.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G1_CSC_ODC.bits.cscodc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc1);
        G1_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc0);
        HAL_WriteReg((HI_U32 *)addr_REG, G1_CSC_ODC.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G1_CSC_IODC.u32);
        G1_CSC_IODC.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G1_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc2);
        G1_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc2);
        HAL_WriteReg((HI_U32 *)addr_REG, G1_CSC_IODC.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G1_CSC_P0.u32);
        G1_CSC_P0.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G1_CSC_P0.bits.cscp00 = HAL_Conver_CscCoef(pstCscCoef->csc_coef00);
        G1_CSC_P0.bits.cscp01 = HAL_Conver_CscCoef(pstCscCoef->csc_coef01);
        HAL_WriteReg((HI_U32 *)addr_REG, G1_CSC_P0.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G1_CSC_P1.u32);
        G1_CSC_P1.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G1_CSC_P1.bits.cscp02 = HAL_Conver_CscCoef(pstCscCoef->csc_coef02);
        G1_CSC_P1.bits.cscp10 = HAL_Conver_CscCoef(pstCscCoef->csc_coef10);
        HAL_WriteReg((HI_U32 *)addr_REG, G1_CSC_P1.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G1_CSC_P2.u32);
        G1_CSC_P2.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G1_CSC_P2.bits.cscp11 = HAL_Conver_CscCoef(pstCscCoef->csc_coef11);
        G1_CSC_P2.bits.cscp12 = HAL_Conver_CscCoef(pstCscCoef->csc_coef12);
        HAL_WriteReg((HI_U32 *)addr_REG, G1_CSC_P2.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G1_CSC_P3.u32);
        G1_CSC_P3.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G1_CSC_P3.bits.cscp20 = HAL_Conver_CscCoef(pstCscCoef->csc_coef20);
        G1_CSC_P3.bits.cscp21 = HAL_Conver_CscCoef(pstCscCoef->csc_coef21);
        HAL_WriteReg((HI_U32 *)addr_REG, G1_CSC_P3.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G1_CSC_P4.u32);
        G1_CSC_P4.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G1_CSC_P4.bits.cscp22 = HAL_Conver_CscCoef(pstCscCoef->csc_coef22);
        HAL_WriteReg((HI_U32 *)addr_REG, G1_CSC_P4.u32);
    } else if (enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = (HI_UL) & (g_pHifbReg->G3_CSC_IDC.u32);
        G3_CSC_IDC.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G3_CSC_IDC.bits.cscidc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc1);
        G3_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc0);
        HAL_WriteReg((HI_U32 *)addr_REG, G3_CSC_IDC.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G3_CSC_ODC.u32);
        G3_CSC_ODC.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G3_CSC_ODC.bits.cscodc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc1);
        G3_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc0);
        HAL_WriteReg((HI_U32 *)addr_REG, G3_CSC_ODC.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G3_CSC_IODC.u32);
        G3_CSC_IODC.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G3_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc2);
        G3_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc2);
        HAL_WriteReg((HI_U32 *)addr_REG, G3_CSC_IODC.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G3_CSC_P0.u32);
        G3_CSC_P0.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G3_CSC_P0.bits.cscp00 = HAL_Conver_CscCoef(pstCscCoef->csc_coef00);
        G3_CSC_P0.bits.cscp01 = HAL_Conver_CscCoef(pstCscCoef->csc_coef01);
        HAL_WriteReg((HI_U32 *)addr_REG, G3_CSC_P0.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G3_CSC_P1.u32);
        G3_CSC_P1.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G3_CSC_P1.bits.cscp02 = HAL_Conver_CscCoef(pstCscCoef->csc_coef02);
        G3_CSC_P1.bits.cscp10 = HAL_Conver_CscCoef(pstCscCoef->csc_coef10);
        HAL_WriteReg((HI_U32 *)addr_REG, G3_CSC_P1.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G3_CSC_P2.u32);
        G3_CSC_P2.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G3_CSC_P2.bits.cscp11 = HAL_Conver_CscCoef(pstCscCoef->csc_coef11);
        G3_CSC_P2.bits.cscp12 = HAL_Conver_CscCoef(pstCscCoef->csc_coef12);
        HAL_WriteReg((HI_U32 *)addr_REG, G3_CSC_P2.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G3_CSC_P3.u32);
        G3_CSC_P3.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G3_CSC_P3.bits.cscp20 = HAL_Conver_CscCoef(pstCscCoef->csc_coef20);
        G3_CSC_P3.bits.cscp21 = HAL_Conver_CscCoef(pstCscCoef->csc_coef21);
        HAL_WriteReg((HI_U32 *)addr_REG, G3_CSC_P3.u32);

        addr_REG = (HI_UL) & (g_pHifbReg->G3_CSC_P4.u32);
        G3_CSC_P4.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        G3_CSC_P4.bits.cscp22 = HAL_Conver_CscCoef(pstCscCoef->csc_coef22);
        HAL_WriteReg((HI_U32 *)addr_REG, G3_CSC_P4.u32);
    } else {
        HAL_PRINT("Error layer id found in %s,%s,%d\n", __FILE__, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetCscMode(HAL_DISP_LAYER_E enLayer, HI_BOOL bIsHCModeBy709)
{
    return HI_TRUE;
}

/*
* Name : VOU_DRV_LayerCSCEnable
* Desc : Set layer csc enable.
*/
HI_BOOL HAL_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn)
{
    U_HIHDR_G_RGB2YUV_CTRL HIHDR_G_RGB2YUV_CTRL;
    U_G1_CSC_IDC G1_CSC_IDC;
    U_G3_CSC_IDC G3_CSC_IDC;

    if (enLayer == HAL_DISP_LAYER_GFX0) {
        HIHDR_G_RGB2YUV_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_CTRL.u32));
        HIHDR_G_RGB2YUV_CTRL.bits.hihdr_r2y_en = bCscEn;
        HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_CTRL.u32), HIHDR_G_RGB2YUV_CTRL.u32);
    } else if (enLayer == HAL_DISP_LAYER_GFX1) {
        G1_CSC_IDC.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G1_CSC_IDC.u32));
        G1_CSC_IDC.bits.csc_en = bCscEn;
        HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G1_CSC_IDC.u32), G1_CSC_IDC.u32);
    } else if (enLayer == HAL_DISP_LAYER_GFX3) {
        G3_CSC_IDC.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G3_CSC_IDC.u32));
        G3_CSC_IDC.bits.csc_en = bCscEn;
        HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G3_CSC_IDC.u32), G3_CSC_IDC.u32);
    } else {
        HAL_PRINT("Error:Wrong layer ID in HAL_LAYER_SetCscEn !\n");
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetLayerInRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect)
{
    U_GFX_IRESO GFX_IRESO;
    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_IRESO.u32));
        GFX_IRESO.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_IRESO.bits.ireso_w = pstRect->w - 1;
        GFX_IRESO.bits.ireso_h = pstRect->h - 1;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_IRESO.u32);
    } else {
        HAL_PRINT("Error layer id found in %s,%s,%d\n", __FILE__, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetLayerOutRect(HAL_DISP_LAYER_E enLayer, HIFB_RECT *pstRect)
{
    return HI_FALSE;
}

/*
* Name : HAL_LAYER_SetLayerGAlpha
* Desc : Set video/graphic layer's global alpha
*/
HI_BOOL HAL_LAYER_SetLayerGAlpha(HAL_DISP_LAYER_E enLayer,
                                 HI_U8 u8Alpha0)
{
    volatile U_G0_CTRL G0_CTRL;

    volatile HI_UL addr_REG;

    switch (enLayer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3: {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->G0_CTRL.u32));
            G0_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            G0_CTRL.bits.galpha = u8Alpha0;
            HAL_WriteReg((HI_U32 *)addr_REG, G0_CTRL.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id %d found in %s: L%d\n", enLayer, __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

// get global alpha
HI_BOOL HAL_LAYER_GetLayerGAlpha(HAL_DISP_LAYER_E enLayer, HI_U8 *pu8Alpha0)
{
    volatile U_G0_CTRL G0_CTRL;
    volatile HI_UL addr_REG;

    switch (enLayer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3: {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->G0_CTRL.u32));
            G0_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            *pu8Alpha0 = G0_CTRL.bits.galpha;
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/*
* Name : HAL_LAYER_SetRegUp
* Desc : Set layer(video or graphic) register update.
*/
HI_BOOL HAL_LAYER_SetRegUp(HAL_DISP_LAYER_E enLayer)
{
    U_G0_UPD G0_UPD;
    volatile HI_UL addr_REG;

    switch (enLayer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3: {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->G0_UPD.u32));
            G0_UPD.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            /* graphic layer register update */
            G0_UPD.bits.regup = 0x1;
            HAL_WriteReg((HI_U32 *)addr_REG, G0_UPD.u32);
            break;
        }
        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

/* set layer addr */
HI_BOOL HAL_GRAPHIC_SetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 u64LAddr)
{
    volatile HI_UL ul_GFX_ADDR_H = 0;
    volatile HI_UL ul_GFX_ADDR_L = 0;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        // Write low address to register.
        ul_GFX_ADDR_L = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_ADDR_L));
        HAL_WriteReg((HI_U32 *)ul_GFX_ADDR_L, GetLowAddr(u64LAddr));

        // Write high address to register.
        ul_GFX_ADDR_H = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_ADDR_H));
        HAL_WriteReg((HI_U32 *)ul_GFX_ADDR_H, GetHighAddr(u64LAddr));
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

/*
* Name : HAL_GRAPHIC_GetGfxAddr
* Desc : get layer addr.
*/
HI_BOOL HAL_GRAPHIC_GetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 *pu64GfxAddr)
{
    volatile HI_UL addr_REG;
    HI_U64 u64AddrH = 0x0;
    HI_U32 u32AddrL = 0x0;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_ADDR_L));
        u32AddrL = HAL_ReadReg((HI_U32 *)addr_REG);
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_ADDR_H));
        u64AddrH = HAL_ReadReg((HI_U32 *)addr_REG);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    *pu64GfxAddr = u32AddrL + (u64AddrH << 32);
    return HI_TRUE;
}

/* layer stride */
HI_BOOL HAL_GRAPHIC_SetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U16 u16pitch)
{
    volatile U_GFX_STRIDE GFX_STRIDE;
    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_STRIDE.u32));
        GFX_STRIDE.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_STRIDE.bits.surface_stride = u16pitch;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_STRIDE.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

/* get layer stride */
HI_BOOL HAL_GRAPHIC_GetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U32 *pu32GfxStride)
{
    volatile HI_UL addr_REG;
    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_STRIDE.u32));
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    *pu32GfxStride = HAL_ReadReg((HI_U32 *)addr_REG);

    return HI_TRUE;
}

/* layer bit ext. */
HI_BOOL HAL_GRAPHIC_SetGfxExt(HAL_DISP_LAYER_E enLayer,
                              HAL_GFX_BITEXTEND_E enMode)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_OUT_CTRL.bits.bitext = enMode;
        HAL_WriteReg ((HI_U32 *)(HI_UL) addr_REG, GFX_OUT_CTRL.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_OUT_CTRL.bits.premulti_en = bEnable;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_OUT_CTRL.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_GetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 *pbEnable)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        *pbEnable = GFX_OUT_CTRL.bits.premulti_en;
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxPalpha(HAL_DISP_LAYER_E enLayer,
                                 HI_U32 bAlphaEn, HI_U32 bArange,
                                 HI_U8 u8Alpha0, HI_U8 u8Alpha1)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;
    U_GFX_1555_ALPHA GFX_1555_ALPHA;

    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_OUT_CTRL.bits.palpha_en = bAlphaEn;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_OUT_CTRL.u32);

        if (bAlphaEn == HI_TRUE) {
            addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_1555_ALPHA.u32));
            GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            GFX_1555_ALPHA.bits.alpha_1 = u8Alpha1;
            HAL_WriteReg((HI_U32 *)addr_REG, GFX_1555_ALPHA.u32);

            addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_1555_ALPHA.u32));
            GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
            GFX_1555_ALPHA.bits.alpha_0 = u8Alpha0;
            HAL_WriteReg((HI_U32 *)addr_REG, GFX_1555_ALPHA.u32);
        } else {
            addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_1555_ALPHA.u32));
            GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32 *)(HI_UL) addr_REG);
            GFX_1555_ALPHA.bits.alpha_1 = 0xff;
            HAL_WriteReg ((HI_U32 *)(HI_UL) addr_REG, GFX_1555_ALPHA.u32);

            addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_1555_ALPHA.u32));
            GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32 *)(HI_UL) addr_REG);
            GFX_1555_ALPHA.bits.alpha_0 = 0xff;
            HAL_WriteReg ((HI_U32 *)(HI_UL) addr_REG, GFX_1555_ALPHA.u32);
        }
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxKeyEn(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyEnable)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_OUT_CTRL.bits.key_en = u32KeyEnable;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_OUT_CTRL.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n",
                  (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxKeyMode(HAL_DISP_LAYER_E enLayer, HI_U32 u32KeyOut)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_OUT_CTRL.bits.key_mode = u32KeyOut;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_OUT_CTRL.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey mode in %s: L%d\n",
                  (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetColorKeyValue(HAL_DISP_LAYER_E enLayer,
                                     HAL_GFX_KEY_MAX_S stKeyMax, HAL_GFX_KEY_MIN_S stKeyMin)
{
    U_GFX_CKEY_MAX GFX_CKEY_MAX;
    U_GFX_CKEY_MIN GFX_CKEY_MIN;

    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_CKEY_MAX.u32));
        GFX_CKEY_MAX.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_CKEY_MAX.bits.key_r_max = stKeyMax.u8KeyMax_R;
        GFX_CKEY_MAX.bits.key_g_max = stKeyMax.u8KeyMax_G;
        GFX_CKEY_MAX.bits.key_b_max = stKeyMax.u8KeyMax_B;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_CKEY_MAX.u32);

        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_CKEY_MIN.u32));
        GFX_CKEY_MIN.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_CKEY_MIN.bits.key_r_min = stKeyMin.u8KeyMin_R;
        GFX_CKEY_MIN.bits.key_g_min = stKeyMin.u8KeyMin_G;
        GFX_CKEY_MIN.bits.key_b_min = stKeyMin.u8KeyMin_B;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_CKEY_MIN.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n",
                  (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetColorKeyMask(HAL_DISP_LAYER_E enLayer, HAL_GFX_MASK_S stMsk)
{
    U_GFX_CKEY_MASK GFX_CKEY_MASK;

    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_CKEY_MASK.u32));
        GFX_CKEY_MASK.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_CKEY_MASK.bits.key_r_msk = stMsk.u8Mask_r;
        GFX_CKEY_MASK.bits.key_g_msk = stMsk.u8Mask_g;
        GFX_CKEY_MASK.bits.key_b_msk = stMsk.u8Mask_b;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_CKEY_MASK.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey mask in %s: L%d\n",
                  (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

// for gfx decmpress
HI_BOOL HAL_GRAPHIC_SetGfxDcmpEnable(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    volatile U_GFX_SRC_INFO GFX_SRC_INFO;
    volatile HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_GFX0 ||
        enLayer == HAL_DISP_LAYER_GFX1 ||
        enLayer == HAL_DISP_LAYER_GFX3) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_SRC_INFO.u32));

        GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        GFX_SRC_INFO.bits.dcmp_en = bEnable;
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_SRC_INFO.u32);
    } else {
        HAL_PRINT("Error layer id %d not support dcmp in %s: L%d\n",
                  (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_GetGfxDcmpEnableState(HAL_DISP_LAYER_E enLayer, HI_BOOL *pbEnable)
{
    volatile U_GFX_SRC_INFO GFX_SRC_INFO;
    volatile HI_UL addr_REG;
    if ((enLayer == HAL_DISP_LAYER_GFX0) || (enLayer == HAL_DISP_LAYER_GFX1)) {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_SRC_INFO.u32));
        GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32 *)addr_REG);
        *pbEnable = GFX_SRC_INFO.bits.dcmp_en;
    } else {
        HAL_PRINT("Error layer id %d not support dcmp in %s: L%d\n",
                  (HI_S32)enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxDcmpAddr(HAL_DISP_LAYER_E enLayer, HI_U64 addr_AR, HI_U64 addr_GB)
{
    volatile HI_U32 GFX_ADDR_H;
    volatile HI_U32 GFX_ADDR_L;

    volatile HI_U32 GFX_DCMP_ADDR_H;
    volatile HI_U32 GFX_DCMP_ADDR_L;
    volatile HI_UL addr_REG;

    if ((enLayer == HAL_DISP_LAYER_GFX0) || (enLayer == HAL_DISP_LAYER_GFX1)) {
        // DCMP low addr for AR
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_ADDR_L));
        GFX_ADDR_L = GetLowAddr(addr_AR);
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_ADDR_L);

        // DCMP high addr for AR
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_ADDR_H));
        GFX_ADDR_H = GetHighAddr(addr_AR);
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_ADDR_H);

        // DCMP low addr for GB
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_DCMP_ADDR_L));
        GFX_DCMP_ADDR_L = GetLowAddr(addr_GB);
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_DCMP_ADDR_L);

        // DCMP high addr for GB
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (g_pHifbReg->GFX_DCMP_ADDR_H));
        GFX_DCMP_ADDR_H = GetHighAddr(addr_GB);
        HAL_WriteReg((HI_U32 *)addr_REG, GFX_DCMP_ADDR_H);

    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_CBM_SetCbmBkg(HI_U32 u32MixerId, HAL_DISP_BKCOLOR_S *pstBkg)
{
    U_CBM_BKG1 CBM_BKG1;
    U_CBM_BKG2 CBM_BKG2;

    if (u32MixerId == HAL_CBMMIX1) {
        CBM_BKG1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->CBM_BKG1.u32));
        CBM_BKG1.bits.cbm_bkgy1 = (pstBkg->u16Bkg_y);
        CBM_BKG1.bits.cbm_bkgcb1 = (pstBkg->u16Bkg_cb);
        CBM_BKG1.bits.cbm_bkgcr1 = (pstBkg->u16Bkg_cr);
        HAL_WriteReg((HI_U32 *)&(g_pHifbReg->CBM_BKG1.u32), CBM_BKG1.u32);
    } else if (u32MixerId == HAL_CBMMIX2) {
        CBM_BKG2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->CBM_BKG2.u32));
        CBM_BKG2.bits.cbm_bkgy2 = (pstBkg->u16Bkg_y);
        CBM_BKG2.bits.cbm_bkgcb2 = (pstBkg->u16Bkg_cb);
        CBM_BKG2.bits.cbm_bkgcr2 = (pstBkg->u16Bkg_cr);
        HAL_WriteReg((HI_U32 *)&(g_pHifbReg->CBM_BKG2.u32), CBM_BKG2.u32);
    } else {
        HAL_PRINT("Error mix id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


/***************************************************************
    Begin: Set  parameter functions
****************************************************************/
HI_VOID HAL_PARA_SetParaAddrVhdChn04(HI_U64 para_addr_vhd_chn04)
{
    volatile HI_U64 PARA_ADDR_VHD_CHN04;

    PARA_ADDR_VHD_CHN04 = para_addr_vhd_chn04;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->PARA_HADDR_VHD_CHN04), GetHighAddr(PARA_ADDR_VHD_CHN04));
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->PARA_ADDR_VHD_CHN04), GetLowAddr(PARA_ADDR_VHD_CHN04));

    return;
}

HI_VOID HAL_PARA_SetParaAddrVhdChn05(HI_U64 para_addr_vhd_chn05)
{
    volatile HI_U64 PARA_ADDR_VHD_CHN05;

    PARA_ADDR_VHD_CHN05 = para_addr_vhd_chn05;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->PARA_HADDR_VHD_CHN05), GetHighAddr(PARA_ADDR_VHD_CHN05));
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->PARA_ADDR_VHD_CHN05), GetLowAddr(PARA_ADDR_VHD_CHN05));

    return;
}

HI_VOID HAL_PARA_SetParaAddrVhdChn06(HI_U64 para_addr_vhd_chn06)
{
    volatile HI_U64 PARA_ADDR_VHD_CHN06;

    PARA_ADDR_VHD_CHN06 = para_addr_vhd_chn06;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->PARA_HADDR_VHD_CHN06), GetHighAddr(PARA_ADDR_VHD_CHN06));
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->PARA_ADDR_VHD_CHN06), GetLowAddr(PARA_ADDR_VHD_CHN06));

    return;
}

HI_VOID HAL_PARA_SetParaUpVhdChn(HI_U32 u32ChnNum)
{
    U_PARA_UP_VHD PARA_UP_VHD;

    PARA_UP_VHD.u32 = (1 << u32ChnNum);
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->PARA_UP_VHD.u32), PARA_UP_VHD.u32);

    return;
}

HI_VOID HAL_PARA_SetParaUpVhdChnAll(void)
{
    U_PARA_UP_VHD PARA_UP_VHD;

    PARA_UP_VHD.u32 = 0xffffffff;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->PARA_UP_VHD.u32), PARA_UP_VHD.u32);

    return;
}

/*********************************************************************
        End : Set  parameter functions
*********************************************************************/
/**********************************************************************************
*  Begin   : Graphic layer HDR relative hal functions.
**********************************************************************************/
HI_VOID HAL_HIHDR_G_SetGhdrDemoPos(HI_U32 ghdr_demo_pos)
{
    U_GHDR_CTRL GHDR_CTRL;

    GHDR_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_CTRL.u32));
    GHDR_CTRL.bits.ghdr_demo_pos = ghdr_demo_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_CTRL.u32), GHDR_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDemoMode(HI_U32 ghdr_demo_mode)
{
    U_GHDR_CTRL GHDR_CTRL;

    GHDR_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_CTRL.u32));
    GHDR_CTRL.bits.ghdr_demo_mode = ghdr_demo_mode;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_CTRL.u32), GHDR_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDemoEn(HI_U32 ghdr_demo_en)
{
    U_GHDR_CTRL GHDR_CTRL;

    GHDR_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_CTRL.u32));
    GHDR_CTRL.bits.ghdr_demo_en = ghdr_demo_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_CTRL.u32), GHDR_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrCkGtEn(HI_U32 ghdr_ck_gt_en)
{
    U_GHDR_CTRL GHDR_CTRL;

    GHDR_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_CTRL.u32));
    GHDR_CTRL.bits.ghdr_ck_gt_en = ghdr_ck_gt_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_CTRL.u32), GHDR_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrEn(HI_U32 ghdr_en)
{
    U_GHDR_CTRL GHDR_CTRL;

    GHDR_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_CTRL.u32));
    GHDR_CTRL.bits.ghdr_en = ghdr_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_CTRL.u32), GHDR_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDegmmEn(HI_U32 ghdr_degmm_en)
{
    U_GHDR_DEGAMMA_CTRL GHDR_DEGAMMA_CTRL;

    GHDR_DEGAMMA_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_CTRL.u32));
    GHDR_DEGAMMA_CTRL.bits.ghdr_degmm_en = ghdr_degmm_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_CTRL.u32), GHDR_DEGAMMA_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX4Step(HI_U32 g_degmm_x4_step)
{
    U_GHDR_DEGAMMA_STEP GHDR_DEGAMMA_STEP;

    GHDR_DEGAMMA_STEP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_STEP.u32));
    GHDR_DEGAMMA_STEP.bits.g_degmm_x4_step = g_degmm_x4_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_STEP.u32), GHDR_DEGAMMA_STEP.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX3Step(HI_U32 g_degmm_x3_step)
{
    U_GHDR_DEGAMMA_STEP GHDR_DEGAMMA_STEP;

    GHDR_DEGAMMA_STEP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_STEP.u32));
    GHDR_DEGAMMA_STEP.bits.g_degmm_x3_step = g_degmm_x3_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_STEP.u32), GHDR_DEGAMMA_STEP.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX2Step(HI_U32 g_degmm_x2_step)
{
    U_GHDR_DEGAMMA_STEP GHDR_DEGAMMA_STEP;

    GHDR_DEGAMMA_STEP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_STEP.u32));
    GHDR_DEGAMMA_STEP.bits.g_degmm_x2_step = g_degmm_x2_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_STEP.u32), GHDR_DEGAMMA_STEP.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX1Step(HI_U32 g_degmm_x1_step)
{
    U_GHDR_DEGAMMA_STEP GHDR_DEGAMMA_STEP;

    GHDR_DEGAMMA_STEP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_STEP.u32));
    GHDR_DEGAMMA_STEP.bits.g_degmm_x1_step = g_degmm_x1_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_STEP.u32), GHDR_DEGAMMA_STEP.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX2Pos(HI_U32 g_degmm_x2_pos)
{
    U_GHDR_DEGAMMA_POS1 GHDR_DEGAMMA_POS1;

    GHDR_DEGAMMA_POS1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_POS1.u32));
    GHDR_DEGAMMA_POS1.bits.g_degmm_x2_pos = g_degmm_x2_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_POS1.u32), GHDR_DEGAMMA_POS1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX1Pos(HI_U32 g_degmm_x1_pos)
{
    U_GHDR_DEGAMMA_POS1 GHDR_DEGAMMA_POS1;

    GHDR_DEGAMMA_POS1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_POS1.u32));
    GHDR_DEGAMMA_POS1.bits.g_degmm_x1_pos = g_degmm_x1_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_POS1.u32), GHDR_DEGAMMA_POS1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX4Pos(HI_U32 g_degmm_x4_pos)
{
    U_GHDR_DEGAMMA_POS2 GHDR_DEGAMMA_POS2;

    GHDR_DEGAMMA_POS2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_POS2.u32));
    GHDR_DEGAMMA_POS2.bits.g_degmm_x4_pos = g_degmm_x4_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_POS2.u32), GHDR_DEGAMMA_POS2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX3Pos(HI_U32 g_degmm_x3_pos)
{
    U_GHDR_DEGAMMA_POS2 GHDR_DEGAMMA_POS2;

    GHDR_DEGAMMA_POS2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_POS2.u32));
    GHDR_DEGAMMA_POS2.bits.g_degmm_x3_pos = g_degmm_x3_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_POS2.u32), GHDR_DEGAMMA_POS2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX4Num(HI_U32 g_degmm_x4_num)
{
    U_GHDR_DEGAMMA_NUM GHDR_DEGAMMA_NUM;

    GHDR_DEGAMMA_NUM.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_NUM.u32));
    GHDR_DEGAMMA_NUM.bits.g_degmm_x4_num = g_degmm_x4_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_NUM.u32), GHDR_DEGAMMA_NUM.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX3Num(HI_U32 g_degmm_x3_num)
{
    U_GHDR_DEGAMMA_NUM GHDR_DEGAMMA_NUM;

    GHDR_DEGAMMA_NUM.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_NUM.u32));
    GHDR_DEGAMMA_NUM.bits.g_degmm_x3_num = g_degmm_x3_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_NUM.u32), GHDR_DEGAMMA_NUM.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX2Num(HI_U32 g_degmm_x2_num)
{
    U_GHDR_DEGAMMA_NUM GHDR_DEGAMMA_NUM;

    GHDR_DEGAMMA_NUM.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_NUM.u32));
    GHDR_DEGAMMA_NUM.bits.g_degmm_x2_num = g_degmm_x2_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_NUM.u32), GHDR_DEGAMMA_NUM.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGDegmmX1Num(HI_U32 g_degmm_x1_num)
{
    U_GHDR_DEGAMMA_NUM GHDR_DEGAMMA_NUM;

    GHDR_DEGAMMA_NUM.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_NUM.u32));
    GHDR_DEGAMMA_NUM.bits.g_degmm_x1_num = g_degmm_x1_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DEGAMMA_NUM.u32), GHDR_DEGAMMA_NUM.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutEn(HI_U32 ghdr_gamut_en)
{
    U_GHDR_GAMUT_CTRL GHDR_GAMUT_CTRL;

    GHDR_GAMUT_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_CTRL.u32));
    GHDR_GAMUT_CTRL.bits.ghdr_gamut_en = ghdr_gamut_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_CTRL.u32), GHDR_GAMUT_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef00(HI_U32 ghdr_gamut_coef00)
{
    U_GHDR_GAMUT_COEF00 GHDR_GAMUT_COEF00;

    GHDR_GAMUT_COEF00.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF00.u32));
    GHDR_GAMUT_COEF00.bits.ghdr_gamut_coef00 = ghdr_gamut_coef00;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF00.u32), GHDR_GAMUT_COEF00.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef01(HI_U32 ghdr_gamut_coef01)
{
    U_GHDR_GAMUT_COEF01 GHDR_GAMUT_COEF01;

    GHDR_GAMUT_COEF01.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF01.u32));
    GHDR_GAMUT_COEF01.bits.ghdr_gamut_coef01 = ghdr_gamut_coef01;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF01.u32), GHDR_GAMUT_COEF01.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef02(HI_U32 ghdr_gamut_coef02)
{
    U_GHDR_GAMUT_COEF02 GHDR_GAMUT_COEF02;

    GHDR_GAMUT_COEF02.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF02.u32));
    GHDR_GAMUT_COEF02.bits.ghdr_gamut_coef02 = ghdr_gamut_coef02;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF02.u32), GHDR_GAMUT_COEF02.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef10(HI_U32 ghdr_gamut_coef10)
{
    U_GHDR_GAMUT_COEF10 GHDR_GAMUT_COEF10;

    GHDR_GAMUT_COEF10.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF10.u32));
    GHDR_GAMUT_COEF10.bits.ghdr_gamut_coef10 = ghdr_gamut_coef10;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF10.u32), GHDR_GAMUT_COEF10.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef11(HI_U32 ghdr_gamut_coef11)
{
    U_GHDR_GAMUT_COEF11 GHDR_GAMUT_COEF11;

    GHDR_GAMUT_COEF11.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF11.u32));
    GHDR_GAMUT_COEF11.bits.ghdr_gamut_coef11 = ghdr_gamut_coef11;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF11.u32), GHDR_GAMUT_COEF11.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef12(HI_U32 ghdr_gamut_coef12)
{
    U_GHDR_GAMUT_COEF12 GHDR_GAMUT_COEF12;

    GHDR_GAMUT_COEF12.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF12.u32));
    GHDR_GAMUT_COEF12.bits.ghdr_gamut_coef12 = ghdr_gamut_coef12;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF12.u32), GHDR_GAMUT_COEF12.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef20(HI_U32 ghdr_gamut_coef20)
{
    U_GHDR_GAMUT_COEF20 GHDR_GAMUT_COEF20;

    GHDR_GAMUT_COEF20.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF20.u32));
    GHDR_GAMUT_COEF20.bits.ghdr_gamut_coef20 = ghdr_gamut_coef20;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF20.u32), GHDR_GAMUT_COEF20.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef21(HI_U32 ghdr_gamut_coef21)
{
    U_GHDR_GAMUT_COEF21 GHDR_GAMUT_COEF21;

    GHDR_GAMUT_COEF21.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF21.u32));
    GHDR_GAMUT_COEF21.bits.ghdr_gamut_coef21 = ghdr_gamut_coef21;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF21.u32), GHDR_GAMUT_COEF21.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutCoef22(HI_U32 ghdr_gamut_coef22)
{
    U_GHDR_GAMUT_COEF22 GHDR_GAMUT_COEF22;

    GHDR_GAMUT_COEF22.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF22.u32));
    GHDR_GAMUT_COEF22.bits.ghdr_gamut_coef22 = ghdr_gamut_coef22;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_COEF22.u32), GHDR_GAMUT_COEF22.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutScale(HI_U32 ghdr_gamut_scale)
{
    U_GHDR_GAMUT_SCALE GHDR_GAMUT_SCALE;

    GHDR_GAMUT_SCALE.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_SCALE.u32));
    GHDR_GAMUT_SCALE.bits.ghdr_gamut_scale = ghdr_gamut_scale;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_SCALE.u32), GHDR_GAMUT_SCALE.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutClipMin(HI_U32 ghdr_gamut_clip_min)
{
    U_GHDR_GAMUT_CLIP_MIN GHDR_GAMUT_CLIP_MIN;

    GHDR_GAMUT_CLIP_MIN.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_CLIP_MIN.u32));
    GHDR_GAMUT_CLIP_MIN.bits.ghdr_gamut_clip_min = ghdr_gamut_clip_min;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_CLIP_MIN.u32), GHDR_GAMUT_CLIP_MIN.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGamutClipMax(HI_U32 ghdr_gamut_clip_max)
{
    U_GHDR_GAMUT_CLIP_MAX GHDR_GAMUT_CLIP_MAX;

    GHDR_GAMUT_CLIP_MAX.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_CLIP_MAX.u32));
    GHDR_GAMUT_CLIP_MAX.bits.ghdr_gamut_clip_max = ghdr_gamut_clip_max;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMUT_CLIP_MAX.u32), GHDR_GAMUT_CLIP_MAX.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrTmapEn(HI_U32 ghdr_tmap_en)
{
    U_GHDR_TONEMAP_CTRL GHDR_TONEMAP_CTRL;

    GHDR_TONEMAP_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_CTRL.u32));
    GHDR_TONEMAP_CTRL.bits.ghdr_tmap_en = ghdr_tmap_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_CTRL.u32), GHDR_TONEMAP_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX4Step(HI_U32 g_tmap_x4_step)
{
    U_GHDR_TONEMAP_STEP GHDR_TONEMAP_STEP;

    GHDR_TONEMAP_STEP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_STEP.u32));
    GHDR_TONEMAP_STEP.bits.g_tmap_x4_step = g_tmap_x4_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_STEP.u32), GHDR_TONEMAP_STEP.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX3Step(HI_U32 g_tmap_x3_step)
{
    U_GHDR_TONEMAP_STEP GHDR_TONEMAP_STEP;

    GHDR_TONEMAP_STEP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_STEP.u32));
    GHDR_TONEMAP_STEP.bits.g_tmap_x3_step = g_tmap_x3_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_STEP.u32), GHDR_TONEMAP_STEP.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX2Step(HI_U32 g_tmap_x2_step)
{
    U_GHDR_TONEMAP_STEP GHDR_TONEMAP_STEP;

    GHDR_TONEMAP_STEP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_STEP.u32));
    GHDR_TONEMAP_STEP.bits.g_tmap_x2_step = g_tmap_x2_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_STEP.u32), GHDR_TONEMAP_STEP.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX1Step(HI_U32 g_tmap_x1_step)
{
    U_GHDR_TONEMAP_STEP GHDR_TONEMAP_STEP;

    GHDR_TONEMAP_STEP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_STEP.u32));
    GHDR_TONEMAP_STEP.bits.g_tmap_x1_step = g_tmap_x1_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_STEP.u32), GHDR_TONEMAP_STEP.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX1Pos(HI_U32 g_tmap_x1_pos)
{
    U_GHDR_TONEMAP_POS1 GHDR_TONEMAP_POS1;

    GHDR_TONEMAP_POS1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_POS1.u32));
    GHDR_TONEMAP_POS1.bits.g_tmap_x1_pos = g_tmap_x1_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_POS1.u32), GHDR_TONEMAP_POS1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX2Pos(HI_U32 g_tmap_x2_pos)
{
    U_GHDR_TONEMAP_POS2 GHDR_TONEMAP_POS2;

    GHDR_TONEMAP_POS2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_POS2.u32));
    GHDR_TONEMAP_POS2.bits.g_tmap_x2_pos = g_tmap_x2_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_POS2.u32), GHDR_TONEMAP_POS2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX3Pos(HI_U32 g_tmap_x3_pos)
{
    U_GHDR_TONEMAP_POS3 GHDR_TONEMAP_POS3;

    GHDR_TONEMAP_POS3.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_POS3.u32));
    GHDR_TONEMAP_POS3.bits.g_tmap_x3_pos = g_tmap_x3_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_POS3.u32), GHDR_TONEMAP_POS3.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX4Pos(HI_U32 g_tmap_x4_pos)
{
    U_GHDR_TONEMAP_POS4 GHDR_TONEMAP_POS4;

    GHDR_TONEMAP_POS4.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_POS4.u32));
    GHDR_TONEMAP_POS4.bits.g_tmap_x4_pos = g_tmap_x4_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_POS4.u32), GHDR_TONEMAP_POS4.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX4Num(HI_U32 g_tmap_x4_num)
{
    U_GHDR_TONEMAP_NUM GHDR_TONEMAP_NUM;

    GHDR_TONEMAP_NUM.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_NUM.u32));
    GHDR_TONEMAP_NUM.bits.g_tmap_x4_num = g_tmap_x4_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_NUM.u32), GHDR_TONEMAP_NUM.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX3Num(HI_U32 g_tmap_x3_num)
{
    U_GHDR_TONEMAP_NUM GHDR_TONEMAP_NUM;

    GHDR_TONEMAP_NUM.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_NUM.u32));
    GHDR_TONEMAP_NUM.bits.g_tmap_x3_num = g_tmap_x3_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_NUM.u32), GHDR_TONEMAP_NUM.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX2Num(HI_U32 g_tmap_x2_num)
{
    U_GHDR_TONEMAP_NUM GHDR_TONEMAP_NUM;

    GHDR_TONEMAP_NUM.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_NUM.u32));
    GHDR_TONEMAP_NUM.bits.g_tmap_x2_num = g_tmap_x2_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_NUM.u32), GHDR_TONEMAP_NUM.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGTmapX1Num(HI_U32 g_tmap_x1_num)
{
    U_GHDR_TONEMAP_NUM GHDR_TONEMAP_NUM;

    GHDR_TONEMAP_NUM.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_NUM.u32));
    GHDR_TONEMAP_NUM.bits.g_tmap_x1_num = g_tmap_x1_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_NUM.u32), GHDR_TONEMAP_NUM.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrTmapLumaCoef0(HI_U32 ghdr_tmap_luma_coef0)
{
    U_GHDR_TONEMAP_LUMA_COEF0 GHDR_TONEMAP_LUMA_COEF0;

    GHDR_TONEMAP_LUMA_COEF0.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_LUMA_COEF0.u32));
    GHDR_TONEMAP_LUMA_COEF0.bits.ghdr_tmap_luma_coef0 = ghdr_tmap_luma_coef0;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_LUMA_COEF0.u32), GHDR_TONEMAP_LUMA_COEF0.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrTmapLumaCoef1(HI_U32 ghdr_tmap_luma_coef1)
{
    U_GHDR_TONEMAP_LUMA_COEF1 GHDR_TONEMAP_LUMA_COEF1;

    GHDR_TONEMAP_LUMA_COEF1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_LUMA_COEF1.u32));
    GHDR_TONEMAP_LUMA_COEF1.bits.ghdr_tmap_luma_coef1 = ghdr_tmap_luma_coef1;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_LUMA_COEF1.u32), GHDR_TONEMAP_LUMA_COEF1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrTmapLumaCoef2(HI_U32 ghdr_tmap_luma_coef2)
{
    U_GHDR_TONEMAP_LUMA_COEF2 GHDR_TONEMAP_LUMA_COEF2;

    GHDR_TONEMAP_LUMA_COEF2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_LUMA_COEF2.u32));
    GHDR_TONEMAP_LUMA_COEF2.bits.ghdr_tmap_luma_coef2 = ghdr_tmap_luma_coef2;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_LUMA_COEF2.u32), GHDR_TONEMAP_LUMA_COEF2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrTmapLumaScale(HI_U32 ghdr_tmap_luma_scale)
{
    U_GHDR_TONEMAP_LUMA_SCALE GHDR_TONEMAP_LUMA_SCALE;

    GHDR_TONEMAP_LUMA_SCALE.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_LUMA_SCALE.u32));
    GHDR_TONEMAP_LUMA_SCALE.bits.ghdr_tmap_luma_scale = ghdr_tmap_luma_scale;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_LUMA_SCALE.u32), GHDR_TONEMAP_LUMA_SCALE.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrTmapCoefScale(HI_U32 ghdr_tmap_coef_scale)
{
    U_GHDR_TONEMAP_COEF_SCALE GHDR_TONEMAP_COEF_SCALE;

    GHDR_TONEMAP_COEF_SCALE.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_COEF_SCALE.u32));
    GHDR_TONEMAP_COEF_SCALE.bits.ghdr_tmap_coef_scale = ghdr_tmap_coef_scale;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_COEF_SCALE.u32), GHDR_TONEMAP_COEF_SCALE.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrTmapOutClipMin(HI_U32 ghdr_tmap_out_clip_min)
{
    U_GHDR_TONEMAP_OUT_CLIP_MIN GHDR_TONEMAP_OUT_CLIP_MIN;

    GHDR_TONEMAP_OUT_CLIP_MIN.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_OUT_CLIP_MIN.u32));
    GHDR_TONEMAP_OUT_CLIP_MIN.bits.ghdr_tmap_out_clip_min = ghdr_tmap_out_clip_min;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_OUT_CLIP_MIN.u32), GHDR_TONEMAP_OUT_CLIP_MIN.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrTmapOutClipMax(HI_U32 ghdr_tmap_out_clip_max)
{
    U_GHDR_TONEMAP_OUT_CLIP_MAX GHDR_TONEMAP_OUT_CLIP_MAX;

    GHDR_TONEMAP_OUT_CLIP_MAX.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_OUT_CLIP_MAX.u32));
    GHDR_TONEMAP_OUT_CLIP_MAX.bits.ghdr_tmap_out_clip_max = ghdr_tmap_out_clip_max;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_TONEMAP_OUT_CLIP_MAX.u32), GHDR_TONEMAP_OUT_CLIP_MAX.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrGmmEn(HI_U32 ghdr_gmm_en)
{
    U_GHDR_GAMMA_CTRL GHDR_GAMMA_CTRL;

    GHDR_GAMMA_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_CTRL.u32));
    GHDR_GAMMA_CTRL.bits.ghdr_gmm_en = ghdr_gmm_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_CTRL.u32), GHDR_GAMMA_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX4Step(HI_U32 g_gmm_x4_step)
{
    U_GHDR_GAMMA_STEP1 GHDR_GAMMA_STEP1;

    GHDR_GAMMA_STEP1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP1.u32));
    GHDR_GAMMA_STEP1.bits.g_gmm_x4_step = g_gmm_x4_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP1.u32), GHDR_GAMMA_STEP1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX3Step(HI_U32 g_gmm_x3_step)
{
    U_GHDR_GAMMA_STEP1 GHDR_GAMMA_STEP1;

    GHDR_GAMMA_STEP1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP1.u32));
    GHDR_GAMMA_STEP1.bits.g_gmm_x3_step = g_gmm_x3_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP1.u32), GHDR_GAMMA_STEP1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX2Step(HI_U32 g_gmm_x2_step)
{
    U_GHDR_GAMMA_STEP1 GHDR_GAMMA_STEP1;

    GHDR_GAMMA_STEP1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP1.u32));
    GHDR_GAMMA_STEP1.bits.g_gmm_x2_step = g_gmm_x2_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP1.u32), GHDR_GAMMA_STEP1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX1Step(HI_U32 g_gmm_x1_step)
{
    U_GHDR_GAMMA_STEP1 GHDR_GAMMA_STEP1;

    GHDR_GAMMA_STEP1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP1.u32));
    GHDR_GAMMA_STEP1.bits.g_gmm_x1_step = g_gmm_x1_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP1.u32), GHDR_GAMMA_STEP1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX8Step(HI_U32 g_gmm_x8_step)
{
    U_GHDR_GAMMA_STEP2 GHDR_GAMMA_STEP2;

    GHDR_GAMMA_STEP2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP2.u32));
    GHDR_GAMMA_STEP2.bits.g_gmm_x8_step = g_gmm_x8_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP2.u32), GHDR_GAMMA_STEP2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX7Step(HI_U32 g_gmm_x7_step)
{
    U_GHDR_GAMMA_STEP2 GHDR_GAMMA_STEP2;

    GHDR_GAMMA_STEP2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP2.u32));
    GHDR_GAMMA_STEP2.bits.g_gmm_x7_step = g_gmm_x7_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP2.u32), GHDR_GAMMA_STEP2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX6Step(HI_U32 g_gmm_x6_step)
{
    U_GHDR_GAMMA_STEP2 GHDR_GAMMA_STEP2;

    GHDR_GAMMA_STEP2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP2.u32));
    GHDR_GAMMA_STEP2.bits.g_gmm_x6_step = g_gmm_x6_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP2.u32), GHDR_GAMMA_STEP2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX5Step(HI_U32 g_gmm_x5_step)
{
    U_GHDR_GAMMA_STEP2 GHDR_GAMMA_STEP2;

    GHDR_GAMMA_STEP2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP2.u32));
    GHDR_GAMMA_STEP2.bits.g_gmm_x5_step = g_gmm_x5_step;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_STEP2.u32), GHDR_GAMMA_STEP2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX2Pos(HI_U32 g_gmm_x2_pos)
{
    U_GHDR_GAMMA_POS1 GHDR_GAMMA_POS1;

    GHDR_GAMMA_POS1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS1.u32));
    GHDR_GAMMA_POS1.bits.g_gmm_x2_pos = g_gmm_x2_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS1.u32), GHDR_GAMMA_POS1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX1Pos(HI_U32 g_gmm_x1_pos)
{
    U_GHDR_GAMMA_POS1 GHDR_GAMMA_POS1;

    GHDR_GAMMA_POS1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS1.u32));
    GHDR_GAMMA_POS1.bits.g_gmm_x1_pos = g_gmm_x1_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS1.u32), GHDR_GAMMA_POS1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX4Pos(HI_U32 g_gmm_x4_pos)
{
    U_GHDR_GAMMA_POS2 GHDR_GAMMA_POS2;

    GHDR_GAMMA_POS2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS2.u32));
    GHDR_GAMMA_POS2.bits.g_gmm_x4_pos = g_gmm_x4_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS2.u32), GHDR_GAMMA_POS2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX3Pos(HI_U32 g_gmm_x3_pos)
{
    U_GHDR_GAMMA_POS2 GHDR_GAMMA_POS2;

    GHDR_GAMMA_POS2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS2.u32));
    GHDR_GAMMA_POS2.bits.g_gmm_x3_pos = g_gmm_x3_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS2.u32), GHDR_GAMMA_POS2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX6Pos(HI_U32 g_gmm_x6_pos)
{
    U_GHDR_GAMMA_POS3 GHDR_GAMMA_POS3;

    GHDR_GAMMA_POS3.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS3.u32));
    GHDR_GAMMA_POS3.bits.g_gmm_x6_pos = g_gmm_x6_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS3.u32), GHDR_GAMMA_POS3.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX5Pos(HI_U32 g_gmm_x5_pos)
{
    U_GHDR_GAMMA_POS3 GHDR_GAMMA_POS3;

    GHDR_GAMMA_POS3.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS3.u32));
    GHDR_GAMMA_POS3.bits.g_gmm_x5_pos = g_gmm_x5_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS3.u32), GHDR_GAMMA_POS3.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX8Pos(HI_U32 g_gmm_x8_pos)
{
    U_GHDR_GAMMA_POS4 GHDR_GAMMA_POS4;

    GHDR_GAMMA_POS4.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS4.u32));
    GHDR_GAMMA_POS4.bits.g_gmm_x8_pos = g_gmm_x8_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS4.u32), GHDR_GAMMA_POS4.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX7Pos(HI_U32 g_gmm_x7_pos)
{
    U_GHDR_GAMMA_POS4 GHDR_GAMMA_POS4;

    GHDR_GAMMA_POS4.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS4.u32));
    GHDR_GAMMA_POS4.bits.g_gmm_x7_pos = g_gmm_x7_pos;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_POS4.u32), GHDR_GAMMA_POS4.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX4Num(HI_U32 g_gmm_x4_num)
{
    U_GHDR_GAMMA_NUM1 GHDR_GAMMA_NUM1;

    GHDR_GAMMA_NUM1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM1.u32));
    GHDR_GAMMA_NUM1.bits.g_gmm_x4_num = g_gmm_x4_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM1.u32), GHDR_GAMMA_NUM1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX3Num(HI_U32 g_gmm_x3_num)
{
    U_GHDR_GAMMA_NUM1 GHDR_GAMMA_NUM1;

    GHDR_GAMMA_NUM1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM1.u32));
    GHDR_GAMMA_NUM1.bits.g_gmm_x3_num = g_gmm_x3_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM1.u32), GHDR_GAMMA_NUM1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX2Num(HI_U32 g_gmm_x2_num)
{
    U_GHDR_GAMMA_NUM1 GHDR_GAMMA_NUM1;

    GHDR_GAMMA_NUM1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM1.u32));
    GHDR_GAMMA_NUM1.bits.g_gmm_x2_num = g_gmm_x2_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM1.u32), GHDR_GAMMA_NUM1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX1Num(HI_U32 g_gmm_x1_num)
{
    U_GHDR_GAMMA_NUM1 GHDR_GAMMA_NUM1;

    GHDR_GAMMA_NUM1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM1.u32));
    GHDR_GAMMA_NUM1.bits.g_gmm_x1_num = g_gmm_x1_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM1.u32), GHDR_GAMMA_NUM1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX8Num(HI_U32 g_gmm_x8_num)
{
    U_GHDR_GAMMA_NUM2 GHDR_GAMMA_NUM2;

    GHDR_GAMMA_NUM2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM2.u32));
    GHDR_GAMMA_NUM2.bits.g_gmm_x8_num = g_gmm_x8_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM2.u32), GHDR_GAMMA_NUM2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX7Num(HI_U32 g_gmm_x7_num)
{
    U_GHDR_GAMMA_NUM2 GHDR_GAMMA_NUM2;

    GHDR_GAMMA_NUM2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM2.u32));
    GHDR_GAMMA_NUM2.bits.g_gmm_x7_num = g_gmm_x7_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM2.u32), GHDR_GAMMA_NUM2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX6Num(HI_U32 g_gmm_x6_num)
{
    U_GHDR_GAMMA_NUM2 GHDR_GAMMA_NUM2;

    GHDR_GAMMA_NUM2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM2.u32));
    GHDR_GAMMA_NUM2.bits.g_gmm_x6_num = g_gmm_x6_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM2.u32), GHDR_GAMMA_NUM2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGGmmX5Num(HI_U32 g_gmm_x5_num)
{
    U_GHDR_GAMMA_NUM2 GHDR_GAMMA_NUM2;

    GHDR_GAMMA_NUM2.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM2.u32));
    GHDR_GAMMA_NUM2.bits.g_gmm_x5_num = g_gmm_x5_num;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_GAMMA_NUM2.u32), GHDR_GAMMA_NUM2.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherRoundUnlim(HI_U32 ghdr_dither_round_unlim)
{
    U_GHDR_DITHER_CTRL GHDR_DITHER_CTRL;

    GHDR_DITHER_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32));
    GHDR_DITHER_CTRL.bits.ghdr_dither_round_unlim = ghdr_dither_round_unlim;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32), GHDR_DITHER_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherEn(HI_U32 ghdr_dither_en)
{
    U_GHDR_DITHER_CTRL GHDR_DITHER_CTRL;

    GHDR_DITHER_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32));
    GHDR_DITHER_CTRL.bits.ghdr_dither_en = ghdr_dither_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32), GHDR_DITHER_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherMode(HI_U32 ghdr_dither_mode)
{
    U_GHDR_DITHER_CTRL GHDR_DITHER_CTRL;

    GHDR_DITHER_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32));
    GHDR_DITHER_CTRL.bits.ghdr_dither_mode = ghdr_dither_mode;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32), GHDR_DITHER_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherRound(HI_U32 ghdr_dither_round)
{
    U_GHDR_DITHER_CTRL GHDR_DITHER_CTRL;

    GHDR_DITHER_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32));
    GHDR_DITHER_CTRL.bits.ghdr_dither_round = ghdr_dither_round;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32), GHDR_DITHER_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherDomainMode(HI_U32 ghdr_dither_domain_mode)
{
    U_GHDR_DITHER_CTRL GHDR_DITHER_CTRL;

    GHDR_DITHER_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32));
    GHDR_DITHER_CTRL.bits.ghdr_dither_domain_mode = ghdr_dither_domain_mode;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32), GHDR_DITHER_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherTapMode(HI_U32 ghdr_dither_tap_mode)
{
    U_GHDR_DITHER_CTRL GHDR_DITHER_CTRL;

    GHDR_DITHER_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32));
    GHDR_DITHER_CTRL.bits.ghdr_dither_tap_mode = ghdr_dither_tap_mode;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_CTRL.u32), GHDR_DITHER_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherThrMax(HI_U32 ghdr_dither_thr_max)
{
    U_GHDR_DITHER_THR GHDR_DITHER_THR;

    GHDR_DITHER_THR.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_THR.u32));
    GHDR_DITHER_THR.bits.ghdr_dither_thr_max = ghdr_dither_thr_max;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_THR.u32), GHDR_DITHER_THR.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherThrMin(HI_U32 ghdr_dither_thr_min)
{
    U_GHDR_DITHER_THR GHDR_DITHER_THR;

    GHDR_DITHER_THR.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_THR.u32));
    GHDR_DITHER_THR.bits.ghdr_dither_thr_min = ghdr_dither_thr_min;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_THR.u32), GHDR_DITHER_THR.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherSedY0(HI_U32 ghdr_dither_sed_y0)
{
    U_GHDR_DITHER_SED_Y0 GHDR_DITHER_SED_Y0;

    GHDR_DITHER_SED_Y0.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_Y0.u32));
    GHDR_DITHER_SED_Y0.bits.ghdr_dither_sed_y0 = ghdr_dither_sed_y0;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_Y0.u32), GHDR_DITHER_SED_Y0.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherSedU0(HI_U32 ghdr_dither_sed_u0)
{
    U_GHDR_DITHER_SED_U0 GHDR_DITHER_SED_U0;

    GHDR_DITHER_SED_U0.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_U0.u32));
    GHDR_DITHER_SED_U0.bits.ghdr_dither_sed_u0 = ghdr_dither_sed_u0;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_U0.u32), GHDR_DITHER_SED_U0.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherSedV0(HI_U32 ghdr_dither_sed_v0)
{
    U_GHDR_DITHER_SED_V0 GHDR_DITHER_SED_V0;

    GHDR_DITHER_SED_V0.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_V0.u32));
    GHDR_DITHER_SED_V0.bits.ghdr_dither_sed_v0 = ghdr_dither_sed_v0;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_V0.u32), GHDR_DITHER_SED_V0.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherSedW0(HI_U32 ghdr_dither_sed_w0)
{
    U_GHDR_DITHER_SED_W0 GHDR_DITHER_SED_W0;

    GHDR_DITHER_SED_W0.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_W0.u32));
    GHDR_DITHER_SED_W0.bits.ghdr_dither_sed_w0 = ghdr_dither_sed_w0;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_W0.u32), GHDR_DITHER_SED_W0.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherSedY1(HI_U32 ghdr_dither_sed_y1)
{
    U_GHDR_DITHER_SED_Y1 GHDR_DITHER_SED_Y1;

    GHDR_DITHER_SED_Y1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_Y1.u32));
    GHDR_DITHER_SED_Y1.bits.ghdr_dither_sed_y1 = ghdr_dither_sed_y1;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_Y1.u32), GHDR_DITHER_SED_Y1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherSedU1(HI_U32 ghdr_dither_sed_u1)
{
    U_GHDR_DITHER_SED_U1 GHDR_DITHER_SED_U1;

    GHDR_DITHER_SED_U1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_U1.u32));
    GHDR_DITHER_SED_U1.bits.ghdr_dither_sed_u1 = ghdr_dither_sed_u1;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_U1.u32), GHDR_DITHER_SED_U1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherSedV1(HI_U32 ghdr_dither_sed_v1)
{
    U_GHDR_DITHER_SED_V1 GHDR_DITHER_SED_V1;

    GHDR_DITHER_SED_V1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_V1.u32));
    GHDR_DITHER_SED_V1.bits.ghdr_dither_sed_v1 = ghdr_dither_sed_v1;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_V1.u32), GHDR_DITHER_SED_V1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetGhdrDitherSedW1(HI_U32 ghdr_dither_sed_w1)
{
    U_GHDR_DITHER_SED_W1 GHDR_DITHER_SED_W1;

    GHDR_DITHER_SED_W1.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_W1.u32));
    GHDR_DITHER_SED_W1.bits.ghdr_dither_sed_w1 = ghdr_dither_sed_w1;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->GHDR_DITHER_SED_W1.u32), GHDR_DITHER_SED_W1.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetHihdrR2yEn(HI_U32 hihdr_r2y_en)
{
    U_HIHDR_G_RGB2YUV_CTRL HIHDR_G_RGB2YUV_CTRL;

    HIHDR_G_RGB2YUV_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_CTRL.u32));
    HIHDR_G_RGB2YUV_CTRL.bits.hihdr_r2y_en = hihdr_r2y_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_CTRL.u32), HIHDR_G_RGB2YUV_CTRL.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetHihdrR2yScale2p(HI_U32 hihdr_r2y_scale2p)
{
    U_HIHDR_G_RGB2YUV_SCALE2P HIHDR_G_RGB2YUV_SCALE2P;

    HIHDR_G_RGB2YUV_SCALE2P.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_SCALE2P.u32));
    HIHDR_G_RGB2YUV_SCALE2P.bits.hihdr_r2y_scale2p = hihdr_r2y_scale2p;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_SCALE2P.u32), HIHDR_G_RGB2YUV_SCALE2P.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetHihdrR2yClipMin(HI_U32 hihdr_r2y_clip_min)
{
    U_HIHDR_G_RGB2YUV_MIN HIHDR_G_RGB2YUV_MIN;

    HIHDR_G_RGB2YUV_MIN.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_MIN.u32));
    HIHDR_G_RGB2YUV_MIN.bits.hihdr_r2y_clip_min = hihdr_r2y_clip_min;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_MIN.u32), HIHDR_G_RGB2YUV_MIN.u32);

    return;
}

HI_VOID HAL_HIHDR_G_SetHihdrR2yClipMax(HI_U32 hihdr_r2y_clip_max)
{
    U_HIHDR_G_RGB2YUV_MAX HIHDR_G_RGB2YUV_MAX;

    HIHDR_G_RGB2YUV_MAX.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_MAX.u32));
    HIHDR_G_RGB2YUV_MAX.bits.hihdr_r2y_clip_max = hihdr_r2y_clip_max;
    HAL_WriteReg((HI_U32 *)&(g_pHifbHippReg->HIHDR_G_RGB2YUV_MAX.u32), HIHDR_G_RGB2YUV_MAX.u32);

    return;
}

/**********************************************************************************
*  End     : Graphic layer HDR relative hal functions.
**********************************************************************************/
/**********************************************************************************
*  Begin   : Graphic layer DCMP relative hal functions.
**********************************************************************************/
HI_VOID HAL_FDR_GFX_SetDcmpEn(HI_U32 layer, HI_U32 dcmp_en)
{
    U_GFX_SRC_INFO GFX_SRC_INFO;
    U_GFX_OSD_GLB_INFO GFX_OSD_GLB_INFO;

    if (layer >= GFX_MAX) {
        HI_TRACE(HI_DBG_ERR, HI_ID_VO, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32 *)(&(g_pHifbReg->GFX_SRC_INFO.u32) + layer * FDR_GFX_OFFSET));
    GFX_SRC_INFO.bits.dcmp_en = dcmp_en;
    HAL_WriteReg ((HI_U32 *)(&(g_pHifbReg->GFX_SRC_INFO.u32) + layer * FDR_GFX_OFFSET), GFX_SRC_INFO.u32);

    GFX_OSD_GLB_INFO.u32 = HAL_ReadReg((HI_U32 *)(&(g_pHifbReg->GFX_OSD_GLB_INFO.u32) + layer * FDR_GFX_OFFSET));
    GFX_OSD_GLB_INFO.bits.dcmp_en = dcmp_en;
    HAL_WriteReg ((HI_U32 *)(&(g_pHifbReg->GFX_OSD_GLB_INFO.u32) + layer * FDR_GFX_OFFSET), GFX_OSD_GLB_INFO.u32);

    return;
}

HI_VOID HAL_FDR_GFX_SetFrameSize0(HI_U32 layer, HI_U32 frame_size0)
{
    volatile HI_UL GFX_DCMP_FRAMESIZE0;

    if (layer >= GFX_MAX) {
        HI_TRACE(HI_DBG_ERR, HI_ID_VO, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    GFX_DCMP_FRAMESIZE0 = HAL_ReadReg((HI_U32 *)(&(g_pHifbReg->GFX_DCMP_FRAMESIZE0) + layer * FDR_GFX_OFFSET));
    GFX_DCMP_FRAMESIZE0 = frame_size0;
    HAL_WriteReg ((HI_U32 *)(&(g_pHifbReg->GFX_DCMP_FRAMESIZE0) + layer * FDR_GFX_OFFSET), GFX_DCMP_FRAMESIZE0);

    return;
}

HI_VOID HAL_FDR_GFX_SetFrameSize1(HI_U32 layer, HI_U32 frame_size1)
{
    volatile HI_UL GFX_DCMP_FRAMESIZE1;

    if (layer >= GFX_MAX) {
        HI_TRACE(HI_DBG_ERR, HI_ID_VO, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    GFX_DCMP_FRAMESIZE1 = HAL_ReadReg((HI_U32 *)((HI_UL) & (g_pHifbReg->GFX_DCMP_FRAMESIZE1) + layer * FDR_GFX_OFFSET *
                                                 4));
    GFX_DCMP_FRAMESIZE1 = frame_size1;
    HAL_WriteReg((HI_U32 *)((HI_UL) & (g_pHifbReg->GFX_DCMP_FRAMESIZE1) + layer * FDR_GFX_OFFSET * 4),
                 GFX_DCMP_FRAMESIZE1);

    return;
}

HI_VOID HAL_FDR_GFX_SetSourceMode(HI_U32 layer, HI_U32 source_mode)
{
    U_GFX_OSD_GLB_INFO GFX_OSD_GLB_INFO;

    if (layer >= GFX_MAX) {
        HI_TRACE(HI_DBG_ERR, HI_ID_VO, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    GFX_OSD_GLB_INFO.u32 = HAL_ReadReg((HI_U32 *)(&(g_pHifbReg->GFX_OSD_GLB_INFO.u32) + layer * FDR_GFX_OFFSET));
    GFX_OSD_GLB_INFO.bits.source_mode = source_mode;
    HAL_WriteReg ((HI_U32 *)(&(g_pHifbReg->GFX_OSD_GLB_INFO.u32) + layer * FDR_GFX_OFFSET), GFX_OSD_GLB_INFO.u32);

    return;
}

HI_VOID HAL_FDR_GFX_SetCmpMode(HI_U32 layer, HI_U32 cmp_mode)
{
    U_GFX_OSD_GLB_INFO GFX_OSD_GLB_INFO;

    if (layer >= GFX_MAX) {
        HI_TRACE(HI_DBG_ERR, HI_ID_VO, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    GFX_OSD_GLB_INFO.u32 = HAL_ReadReg((HI_U32 *)(&(g_pHifbReg->GFX_OSD_GLB_INFO.u32) + layer * FDR_GFX_OFFSET));
    GFX_OSD_GLB_INFO.bits.cmp_mode = cmp_mode;
    HAL_WriteReg ((HI_U32 *)(&(g_pHifbReg->GFX_OSD_GLB_INFO.u32) + layer * FDR_GFX_OFFSET), GFX_OSD_GLB_INFO.u32);

    return;
}

HI_VOID HAL_FDR_GFX_SetIsLosslessA(HI_U32 layer, HI_U32 is_lossless_a)
{
    U_GFX_OSD_GLB_INFO GFX_OSD_GLB_INFO;

    if (layer >= GFX_MAX) {
        HI_TRACE(HI_DBG_ERR, HI_ID_VO, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    GFX_OSD_GLB_INFO.u32 = HAL_ReadReg((HI_U32 *)(&(g_pHifbReg->GFX_OSD_GLB_INFO.u32) + layer * FDR_GFX_OFFSET));
    GFX_OSD_GLB_INFO.bits.is_lossless_a = is_lossless_a;
    HAL_WriteReg ((HI_U32 *)(&(g_pHifbReg->GFX_OSD_GLB_INFO.u32) + layer * FDR_GFX_OFFSET), GFX_OSD_GLB_INFO.u32);

    return;
}

HI_VOID HAL_FDR_GFX_SetIsLossless(HI_U32 layer, HI_U32 is_lossless)
{
    U_GFX_OSD_GLB_INFO GFX_OSD_GLB_INFO;

    if (layer >= GFX_MAX) {
        HI_TRACE(HI_DBG_ERR, HI_ID_VO, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    GFX_OSD_GLB_INFO.u32 = HAL_ReadReg((HI_U32 *)(&(g_pHifbReg->GFX_OSD_GLB_INFO.u32) + layer * FDR_GFX_OFFSET));
    GFX_OSD_GLB_INFO.bits.is_lossless = is_lossless;
    HAL_WriteReg ((HI_U32 *)(&(g_pHifbReg->GFX_OSD_GLB_INFO.u32) + layer * FDR_GFX_OFFSET), GFX_OSD_GLB_INFO.u32);

    return;
}

HI_VOID HAL_FDR_GFX_SetFrameHeight(HI_U32 layer, HI_U32 frame_height)
{
    U_GFX_OSD_FRAME_SIZE GFX_OSD_FRAME_SIZE;

    if (layer >= GFX_MAX) {
        HI_TRACE(HI_DBG_ERR, HI_ID_VO, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    GFX_OSD_FRAME_SIZE.u32 = HAL_ReadReg((HI_U32 *)(&(g_pHifbReg->GFX_OSD_FRAME_SIZE.u32) + layer * FDR_GFX_OFFSET));
    GFX_OSD_FRAME_SIZE.bits.frame_height = frame_height;
    HAL_WriteReg ((HI_U32 *)(&(g_pHifbReg->GFX_OSD_FRAME_SIZE.u32) + layer * FDR_GFX_OFFSET), GFX_OSD_FRAME_SIZE.u32);

    return;
}

HI_VOID HAL_FDR_GFX_SetFrameWidth(HI_U32 layer, HI_U32 frame_width)
{
    U_GFX_OSD_FRAME_SIZE GFX_OSD_FRAME_SIZE;

    if (layer >= GFX_MAX) {
        HI_TRACE(HI_DBG_ERR, HI_ID_VO, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    GFX_OSD_FRAME_SIZE.u32 = HAL_ReadReg((HI_U32 *)(&(g_pHifbReg->GFX_OSD_FRAME_SIZE.u32) + layer * FDR_GFX_OFFSET));
    GFX_OSD_FRAME_SIZE.bits.frame_width = frame_width;
    HAL_WriteReg ((HI_U32 *)(&(g_pHifbReg->GFX_OSD_FRAME_SIZE.u32) + layer * FDR_GFX_OFFSET), GFX_OSD_FRAME_SIZE.u32);

    return;
}

/**********************************************************************************
*  End     : Graphic layer DCMP relative hal functions.
**********************************************************************************/
/**********************************************************************************
*  Begin   : Graphic layer ZME relative hal functions.
**********************************************************************************/
HI_VOID HAL_G0_ZME_SetCkGtEn(HI_U32 ck_gt_en)
{
    U_G0_ZME_HINFO G0_ZME_HINFO;

    G0_ZME_HINFO.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HINFO.u32));
    G0_ZME_HINFO.bits.ck_gt_en = ck_gt_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HINFO.u32), G0_ZME_HINFO.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetOutWidth(HI_U32 out_width)
{
    U_G0_ZME_HINFO G0_ZME_HINFO;

    G0_ZME_HINFO.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HINFO.u32));
    G0_ZME_HINFO.bits.out_width = out_width - 1;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HINFO.u32), G0_ZME_HINFO.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetHfirEn(HI_U32 hfir_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.hfir_en = hfir_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32), G0_ZME_HSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetAhfirMidEn(HI_U32 ahfir_mid_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.ahfir_mid_en = ahfir_mid_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32), G0_ZME_HSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetLhfirMidEn(HI_U32 lhfir_mid_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.lhfir_mid_en = lhfir_mid_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32), G0_ZME_HSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetChfirMidEn(HI_U32 chfir_mid_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.chfir_mid_en = chfir_mid_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32), G0_ZME_HSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetLhfirMode(HI_U32 lhfir_mode)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.lhfir_mode = lhfir_mode;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32), G0_ZME_HSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetAhfirMode(HI_U32 ahfir_mode)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.ahfir_mode = ahfir_mode;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32), G0_ZME_HSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetHfirOrder(HI_U32 hfir_order)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.hfir_order = hfir_order;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32), G0_ZME_HSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetHratio(HI_U32 hratio)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.hratio = hratio;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HSP.u32), G0_ZME_HSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetLhfirOffset(HI_U32 lhfir_offset)
{
    U_G0_ZME_HLOFFSET G0_ZME_HLOFFSET;

    G0_ZME_HLOFFSET.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HLOFFSET.u32));
    G0_ZME_HLOFFSET.bits.lhfir_offset = lhfir_offset;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HLOFFSET.u32), G0_ZME_HLOFFSET.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetChfirOffset(HI_U32 chfir_offset)
{
    U_G0_ZME_HCOFFSET G0_ZME_HCOFFSET;

    G0_ZME_HCOFFSET.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HCOFFSET.u32));
    G0_ZME_HCOFFSET.bits.chfir_offset = chfir_offset;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_HCOFFSET.u32), G0_ZME_HCOFFSET.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetOutPro(HI_U32 out_pro)
{
    U_G0_ZME_VINFO G0_ZME_VINFO;

    G0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VINFO.u32));
    G0_ZME_VINFO.bits.out_pro = out_pro;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VINFO.u32), G0_ZME_VINFO.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetOutHeight(HI_U32 out_height)
{
    U_G0_ZME_VINFO G0_ZME_VINFO;

    G0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VINFO.u32));
    G0_ZME_VINFO.bits.out_height = out_height - 1;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VINFO.u32), G0_ZME_VINFO.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetVfirEn(HI_U32 vfir_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.vfir_en = vfir_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32), G0_ZME_VSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetAvfirMidEn(HI_U32 avfir_mid_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.avfir_mid_en = avfir_mid_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32), G0_ZME_VSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetLvfirMidEn(HI_U32 lvfir_mid_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.lvfir_mid_en = lvfir_mid_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32), G0_ZME_VSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetCvfirMidEn(HI_U32 cvfir_mid_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.cvfir_mid_en = cvfir_mid_en;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32), G0_ZME_VSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetLvfirMode(HI_U32 lvfir_mode)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.lvfir_mode = lvfir_mode;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32), G0_ZME_VSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetVafirMode(HI_U32 vafir_mode)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.vafir_mode = vafir_mode;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32), G0_ZME_VSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetVratio(HI_U32 vratio)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.vratio = vratio;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VSP.u32), G0_ZME_VSP.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetVtpOffset(HI_U32 vtp_offset)
{
    U_G0_ZME_VOFFSET G0_ZME_VOFFSET;

    G0_ZME_VOFFSET.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VOFFSET.u32));
    G0_ZME_VOFFSET.bits.vtp_offset = vtp_offset;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VOFFSET.u32), G0_ZME_VOFFSET.u32);

    return;
}

HI_VOID HAL_G0_ZME_SetVbtmOffset(HI_U32 vbtm_offset)
{
    U_G0_ZME_VOFFSET G0_ZME_VOFFSET;

    G0_ZME_VOFFSET.u32 = HAL_ReadReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VOFFSET.u32));
    G0_ZME_VOFFSET.bits.vbtm_offset = vbtm_offset;
    HAL_WriteReg((HI_U32 *)&(g_pHifbReg->G0_ZME_VOFFSET.u32), G0_ZME_VOFFSET.u32);

    return;
}

/**********************************************************************************
*  End     : Graphic layer ZME relative hal functions.
**********************************************************************************/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

