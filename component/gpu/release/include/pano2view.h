#ifndef PANO2VIEW_H
#define PANO2VIEW_H

#include "hi_type.h"

/********************rotation and scope parameters of view*********************/
typedef struct hiPano2view
{
    HI_FLOAT fRoll;    //Angle of rotation in vertical direction, range from -90 to 90 degrees, Positive axis represents upward rotation, default value is 0
    HI_FLOAT fYaw;     //Angle of rotation in horizontal direction, range from -180 to 180 degrees, Positive axis represents right rotation, default value is 0
    HI_FLOAT fFovy;    //The scope of the field of vision, range from 0 to 180 degrees, default value is 120
}HI_PANO2VIEW_VIEW_S;

/*********************************buffer type*********************************/
typedef enum
{
    HI_PANO2VIEW_FRAME_TYPE_IN,     //input type
    HI_PANO2VIEW_FRAME_TYPE_OUT,    //output type
}HI_PANO2VIEW_FRAME_TYPE_E;

/*********************************format info********************************/
typedef enum
{
    HI_PIXEL_FORMAT_YUV420SP_NV12,    //YUV420sp 8bit
    HI_PIXEL_FORMAT_YUV420P_YV12,     //YUV420p  8bit
    HI_PIXEL_FORMAT_ARGB8888,         //ARGB888
    HI_PIXEL_FORMAT_YUV420SP_10BIT,   //YUV420sp 10bit
}HI_PANO2VIEW_FORMAT_TYPE_E;

/********************************frame info*********************************/
typedef struct hiPano2viewFrame
{
    HI_PANO2VIEW_FORMAT_TYPE_E   eFormat;    //format info
    HI_U32 u32Width;                   //width
    HI_U32 u32Height;                  //hegiht
    HI_U64 u64YPhyAddr;                //Luminance address
    HI_U32 u32YStride;                 //Luminance stride
    HI_U64 u64CbCrPhyAddr;             //Chrominance address
    HI_U32 u32CbCrStride;              //Chrominance stride
    HI_PANO2VIEW_FRAME_TYPE_E eFrameType;    //buffer type
}HI_PANO2VIEW_FRAME_S;

/***************************************************************************
* func          : HI_PANO2VIEW_Init
* description   : CNcomment: pano2view init  CNend\n
* param[in]     : NULL
* param[out]    : NA
* retval        : HI_SUCCESS
* retval        : error code
* others:       : NA
*****************************************************************************/
HI_S32  HI_PANO2VIEW_Init(HI_VOID);

/*****************************************************************************
* func          : HI_PANO2VIEW_DeInit
* description   : CNcomment: pano2view deinit CNend\n
* param[in]     : NULL      CNcomment: NA             CNend\n
* param[out]    : NA        CNcomment: NA   CNend\n
* retval        : HI_SUCCESS
* retval        : error code
* others:       : NA
*****************************************************************************/
HI_S32  HI_PANO2VIEW_DeInit(HI_VOID);

/*****************************************************************************
* func          : HI_PANO2VIEW_RegisterBuffer
* description   : CNcomment: register buffer to attain an internal resource handle  CNend\n
* param[in]     : HI_PANO2VIEW_FRAME_S stFrameUsr      CNcomment: frame info    CNend\n
* param[out]    : NA        CNcomment: a handle representing corresponding internal resource  CNend\n
* retval        : HI_HANDLE
* retval        : error code
* others:       : NA
*****************************************************************************/
HI_U64  HI_PANO2VIEW_RegisterBuffer(HI_PANO2VIEW_FRAME_S stFrameUsr);

/*****************************************************************************
* func          : HI_PANO2VIEW_UnRegisterBuffer
* description   : CNcomment: unregister buffer to release the handle  CNend\n
* param[in]     : HI_HANDLE  hBufferHandle      CNcomment: a handle representing corresponding internal resource    CNend\n
* param[out]    : NA        CNcomment: NA   CNend\n
* retval        : HI_SUCCESS
* retval        : error code
* others:       : NA
*****************************************************************************/
HI_S32  HI_PANO2VIEW_UnRegisterBuffer(HI_U64  hBufferHandle);

/*****************************************************************************
* func          : HI_PANO2VIEW_GeneView
* description   : CNcomment: generate a corrected view   CNend\n
* param[in]     : HI_HANDLE  hBufferHandleIn      CNcomment: a handle representing input buffer             CNend\n
* param[out]    : HI_HANDLE  hBufferHandleOut     CNcomment: a handle representing output buffer            CNend\n
* retval        : HI_SUCCESS
* retval        : error code
* others:       : NA
*****************************************************************************/
HI_S32  HI_PANO2VIEW_GeneView(HI_U64  hBufferHandleIn,  HI_U64  hBufferHandleOut);

/*****************************************************************************
* func          : HI_PANO2VIEW_SetViewRotation
* description   : CNcomment: set rotation location of view    CNend\n
* param[in]     : HI_PANO2VIEW_VIEW_S stRyfaUsr        CNcomment:rotation and scale parameters of view    CNend\n
* param[out]    : NA      CNcomment: NA     CNend\n
* retval        : HI_SUCCESS
* retval        : error code
* others:       : NA
*****************************************************************************/
HI_S32 HI_PANO2VIEW_SetViewRotation(HI_PANO2VIEW_VIEW_S stRyfaUsr);

/*****************************************************************************
* func          : HI_PANO2VIEW_SetViewScale
* description   : CNcomment: set scope of view CNend\n
* param[in]     : HI_PANO2VIEW_VIEW_S stRyfaUsr        CNcomment:location and scale parameters of view   CNend\n
* param[out]    : NA       CNcomment: NA       CNend\n
* retval        : HI_SUCCESS
* retval        : error code
* others:       : NA
*****************************************************************************/
HI_S32 HI_PANO2VIEW_SetViewScale(HI_PANO2VIEW_VIEW_S stRyfaUsr);

#endif
