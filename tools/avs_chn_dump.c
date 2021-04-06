#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "hi_math.h"
#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include "hi_comm_vb.h"
#include "mpi_vb.h"
#include "hi_comm_avs.h"
#include "mpi_avs.h"
#include "mpi_vgs.h"
#include "hi_buffer.h"

#define MAX_FRM_WIDTH   20480

#define VALUE_BETWEEN(x,min,max) (((x)>=(min)) && ((x) <= (max)))

typedef struct hiDUMP_MEMBUF_S
{
    VB_BLK  hBlock;
    VB_POOL hPool;
    HI_U32  u32PoolId;

    HI_U64  u64PhyAddr;
    HI_U8*   pVirAddr;
    HI_S32  s32Mdev;
} DUMP_MEMBUF_S;


static HI_U32 u32AVSDepthFlag = 0;
static HI_U32 u32SignalFlag = 0;

static AVS_GRP AVSGrp = 0;
static AVS_CHN AVSChn = 0;
static HI_U32 u32OrigDepth = 0;
static VIDEO_FRAME_INFO_S stFrame;

static VB_POOL hPool  = VB_INVALID_POOLID;
static DUMP_MEMBUF_S stMem = {0};
static VGS_HANDLE hHandle = -1;
static HI_U32  u32BlkSize = 0;

static HI_CHAR* pUserPageAddr[2] = {HI_NULL, HI_NULL};
static HI_U32 u32Size = 0;

static FILE* pfd = HI_NULL;

static unsigned char* TmpBuff = HI_NULL;
static HI_U16* src = HI_NULL;
static HI_U8* dest = HI_NULL;


static void sample_yuv_8bit_dump(VIDEO_FRAME_S* pVBuf, FILE* pfd)
{
    unsigned int w, h;
    char* pVBufVirt_Y;
    char* pVBufVirt_C;
    char* pMemContent;
    HI_U64 phy_addr;
    PIXEL_FORMAT_E  enPixelFormat = pVBuf->enPixelFormat;
    HI_U32 u32UvHeight;

    TmpBuff = (unsigned char*)malloc(MAX_FRM_WIDTH);

    if (NULL == TmpBuff)
    {
        printf("malloc fail !\n");
        return;
    }

    if (PIXEL_FORMAT_YVU_SEMIPLANAR_420 == enPixelFormat)
    {
        u32Size = (pVBuf->u32Stride[0]) * (pVBuf->u32Height) * 3 / 2;
        u32UvHeight = pVBuf->u32Height / 2;
    }
    else if (PIXEL_FORMAT_YVU_SEMIPLANAR_422 == enPixelFormat)
    {
        u32Size = (pVBuf->u32Stride[0]) * (pVBuf->u32Height) * 2;
        u32UvHeight = pVBuf->u32Height;
    }
    else
    {
        u32Size = (pVBuf->u32Stride[0]) * (pVBuf->u32Height);
        u32UvHeight = pVBuf->u32Height;
    }

    phy_addr = pVBuf->u64PhyAddr[0];

    pUserPageAddr[0] = (HI_CHAR*) HI_MPI_SYS_Mmap(phy_addr, u32Size);

    if (HI_NULL == pUserPageAddr[0])
    {
        free(TmpBuff);
        TmpBuff = HI_NULL;
        return;
    }

    pVBufVirt_Y = pUserPageAddr[0];
    pVBufVirt_C = pVBufVirt_Y + (pVBuf->u32Stride[0]) * (pVBuf->u32Height);

    /* save Y ----------------------------------------------------------------*/
    fprintf(stderr, "saving......Y......");
    fflush(stderr);

    for (h = 0; h < pVBuf->u32Height; h++)
    {
        pMemContent = pVBufVirt_Y + h * pVBuf->u32Stride[0];
        fwrite(pMemContent, pVBuf->u32Width, 1, pfd);
    }

    if (PIXEL_FORMAT_YUV_400 != enPixelFormat)
    {
        fflush(pfd);
        /* save U ----------------------------------------------------------------*/
        fprintf(stderr, "U......");
        fflush(stderr);

        for (h = 0; h < u32UvHeight; h++)
        {
            pMemContent = pVBufVirt_C + h * pVBuf->u32Stride[1];

            pMemContent += 1;

            for (w = 0; w < pVBuf->u32Width / 2; w++)
            {
                TmpBuff[w] = *pMemContent;
                pMemContent += 2;
            }

            fwrite(TmpBuff, pVBuf->u32Width / 2, 1, pfd);
        }

        fflush(pfd);

        /* save V ----------------------------------------------------------------*/
        fprintf(stderr, "V......");
        fflush(stderr);

        for (h = 0; h < u32UvHeight; h++)
        {
            pMemContent = pVBufVirt_C + h * pVBuf->u32Stride[1];

            for (w = 0; w < pVBuf->u32Width / 2; w++)
            {
                TmpBuff[w] = *pMemContent;
                pMemContent += 2;
            }

            fwrite(TmpBuff, pVBuf->u32Width / 2, 1, pfd);
        }
    }

    fflush(pfd);

    fprintf(stderr, "done %d!\n", pVBuf->u32TimeRef);
    fflush(stderr);

    HI_MPI_SYS_Munmap(pUserPageAddr[0], u32Size);
    pUserPageAddr[0] = HI_NULL;

    free(TmpBuff);
    TmpBuff = HI_NULL;

}


static void sample_yuv_10bit_dump(VIDEO_FRAME_S* pVBuf, FILE* pfd)
{
    unsigned int w, h, k, wy, wuv;
    char* pVBufVirt_Y;
    char* pVBufVirt_C;
    char* pMemContent;
    HI_U64 phy_addr;
    PIXEL_FORMAT_E  enPixelFormat = pVBuf->enPixelFormat;
    HI_U32 u32UvHeight;
    HI_U32 u32YWidth;

    src  = (HI_U16*)malloc(MAX_FRM_WIDTH * sizeof(HI_U16));

    if (NULL == src)
    {
        printf("malloc fail !\n");
        return;
    }

    dest = (HI_U8*)malloc(MAX_FRM_WIDTH * sizeof(HI_U8));

    if (NULL == dest)
    {
        printf("malloc fail !\n");
        return;
    }

    if (PIXEL_FORMAT_YVU_SEMIPLANAR_420 == enPixelFormat)
    {
        u32Size = (pVBuf->u32Stride[0]) * (pVBuf->u32Height) * 3 / 2;
        u32UvHeight = pVBuf->u32Height / 2;
    }
    else if (PIXEL_FORMAT_YVU_SEMIPLANAR_422 == enPixelFormat)
    {
        u32Size = (pVBuf->u32Stride[0]) * (pVBuf->u32Height) * 2;
        u32UvHeight = pVBuf->u32Height;
    }
    else
    {
        u32Size = (pVBuf->u32Stride[0]) * (pVBuf->u32Height);
        u32UvHeight = pVBuf->u32Height;
    }

    u32YWidth = (pVBuf->u32Width * 10 + 7) / 8;

    phy_addr = pVBuf->u64PhyAddr[0];
    pUserPageAddr[0] = (HI_CHAR*) HI_MPI_SYS_Mmap(phy_addr, u32Size);

    if (HI_NULL == pUserPageAddr[0])
    {
        free(src);
        src = HI_NULL;
        free(dest);
        dest = HI_NULL;
        return;
    }

    pVBufVirt_Y = pUserPageAddr[0];
    pVBufVirt_C = pVBufVirt_Y + (pVBuf->u32Stride[0]) * (pVBuf->u32Height);

    /* save Y ----------------------------------------------------------------*/
    fprintf(stderr, "saving......Y......");
    fflush(stderr);

    for (h = 0; h < pVBuf->u32Height; h++)
    {
        pMemContent = pVBufVirt_Y + h * pVBuf->u32Stride[0];
        wy = 0;

        for (w = 0; w < u32YWidth - 1; w++)
        {
            dest[w] = *pMemContent;
            dest[w + 1] = *(pMemContent + 1);
            k = wy % 4;

            switch (k)
            {
                case 0:
                    src[wy] = (((HI_U16)(dest[w]))) + (((dest[w + 1]) & 0x3) << 8);
                    break;

                case 1:
                    src[wy] = ((((HI_U16)(dest[w])) & 0xfc) >> 2) + (((HI_U16)(dest[w + 1]) & 0xf) << 6);
                    break;

                case 2:
                    src[wy] = ((((HI_U16)(dest[w])) & 0xf0) >> 4) + (((HI_U16)(dest[w + 1]) & 0x3f) << 4);
                    break;

                case 3:
                    src[wy] = ((((HI_U16)(dest[w])) & 0xc0) >> 6) + ((HI_U16)(dest[w + 1]) << 2);
                    w++;
                    pMemContent += 1;
                    break;
            }

            pMemContent += 1;
            wy++;
        }

        fwrite(src, pVBuf->u32Width * 2, 1, pfd);
    }

    if (PIXEL_FORMAT_YUV_400 != enPixelFormat)
    {
        fflush(pfd);
        /* save U ----------------------------------------------------------------*/
        fprintf(stderr, "U......");
        fflush(stderr);

        for (h = 0; h < u32UvHeight; h++)
        {
            pMemContent = pVBufVirt_C + h * pVBuf->u32Stride[1];

            //pMemContent += 1;
            wy = 0;
            wuv = 0;

            for (w = 0; w < u32YWidth - 1; w++)
            {
                dest[w] = *pMemContent;
                dest[w + 1] = *(pMemContent + 1);
                k = wuv % 4;

                switch (k)
                {
                    case 0:
                        // src[wy] = (((HI_U16)(dest[w]))) + (((dest[w+1])&0x3)<<8);
                        break;

                    case 1:
                        src[wy] = ((((HI_U16)(dest[w])) & 0xfc) >> 2) + (((HI_U16)(dest[w + 1]) & 0xf) << 6);
                        wy++;
                        break;

                    case 2:
                        //src[wy] = ((((HI_U16)(dest[w]))&0xf0)>>4) + (((HI_U16)(dest[w+1])&0x3f)<<4);
                        break;

                    case 3:
                        src[wy] = ((((HI_U16)(dest[w])) & 0xc0) >> 6) + ((HI_U16)(dest[w + 1]) << 2);
                        wy++;
                        w++;
                        pMemContent += 1;
                        break;
                }

                wuv++;
                pMemContent += 1;

            }

            fwrite(src, pVBuf->u32Width , 1, pfd);
        }

        fflush(pfd);

        /* save V ----------------------------------------------------------------*/
        fprintf(stderr, "V......");
        fflush(stderr);

        for (h = 0; h < u32UvHeight; h++)
        {
            pMemContent = pVBufVirt_C + h * pVBuf->u32Stride[1];
            wy = 0;
            wuv = 0;

            for (w = 0; w < u32YWidth - 1; w++)
            {
                dest[w] = *pMemContent;
                dest[w + 1] = *(pMemContent + 1);
                k = wuv % 4;

                switch (k)
                {
                    case 0:
                        src[wy] = (((HI_U16)(dest[w]))) + (((dest[w + 1]) & 0x3) << 8);
                        wy++;
                        break;

                    case 1:
                        //src[wy] = ((((HI_U16)(dest[w]))&0xfc)>>2) + (((HI_U16)(dest[w+1])&0xf)<<6);
                        break;

                    case 2:
                        src[wy] = ((((HI_U16)(dest[w])) & 0xf0) >> 4) + (((HI_U16)(dest[w + 1]) & 0x3f) << 4);
                        wy++;
                        break;

                    case 3:
                        //src[wy] = ((((HI_U16)(dest[w]))&0xc0)>>6) + ((HI_U16)(dest[w+1])<<2);
                        w++;
                        pMemContent += 1;
                        break;
                }

                pMemContent += 1;
                wuv ++;
            }

            fwrite(src, pVBuf->u32Width, 1, pfd);
        }
    }

    fflush(pfd);

    fprintf(stderr, "done %d!\n", pVBuf->u32TimeRef);
    fflush(stderr);

    HI_MPI_SYS_Munmap(pUserPageAddr[0], u32Size);
    pUserPageAddr[0] = HI_NULL;

    free(src);
    src = HI_NULL;
    free(dest);
    dest = HI_NULL;

}


static HI_S32 AVS_Restore(AVS_GRP AVSGrp, AVS_CHN AVSChn)
{
    HI_S32 s32Ret = HI_FAILURE;
    AVS_CHN_ATTR_S stChnAttr = {0};


    if (VB_INVALID_POOLID != stFrame.u32PoolId)
    {
        s32Ret = HI_MPI_AVS_ReleaseChnFrame(AVSGrp, AVSChn, &stFrame);

        if (HI_SUCCESS != s32Ret)
        {
            printf("Release Chn Frame error!!!\n");
        }

        stFrame.u32PoolId = VB_INVALID_POOLID;
    }

    if (-1 != hHandle)
    {
        HI_MPI_VGS_CancelJob(hHandle);
        hHandle = -1;
    }

    if (HI_NULL != stMem.pVirAddr)
    {
        HI_MPI_SYS_Munmap((HI_VOID*)stMem.pVirAddr, u32BlkSize );
        stMem.u64PhyAddr = HI_NULL;
    }

    if (VB_INVALID_POOLID != stMem.hPool)
    {
        HI_MPI_VB_ReleaseBlock(stMem.hBlock);
        stMem.hPool = VB_INVALID_POOLID;
    }

    if (VB_INVALID_POOLID != hPool)
    {
        HI_MPI_VB_DestroyPool( hPool );
        hPool = VB_INVALID_POOLID;
    }

    if (HI_NULL != pUserPageAddr[0])
    {
        HI_MPI_SYS_Munmap(pUserPageAddr[0], u32Size);
        pUserPageAddr[0] = HI_NULL;
    }

    if (pfd)
    {
        fclose(pfd);
        pfd = HI_NULL;
    }

    if (u32AVSDepthFlag)
    {

        if (HI_MPI_AVS_GetChnAttr(AVSGrp, AVSChn, &stChnAttr) != HI_SUCCESS)
        {
            printf("get depth error!!!\n");
        }

        stChnAttr.u32Depth = u32OrigDepth;

        if (HI_MPI_AVS_SetChnAttr(AVSGrp, AVSChn, &stChnAttr) != HI_SUCCESS)
        {
            printf("set depth error!!!\n");
        }

        u32AVSDepthFlag = 0;
    }

    if (TmpBuff)
    {
        free(TmpBuff);
        TmpBuff = HI_NULL;
    }

    if (src)
    {
        free(src);
        src = HI_NULL;
    }

    if (dest)
    {
        free(dest);
        dest = HI_NULL;
    }


    return HI_SUCCESS;
}

void AVS_Chn_Dump_HandleSig(HI_S32 signo)
{
    if (u32SignalFlag)
    {
        exit(-1);
    }

    if (SIGINT == signo || SIGTERM == signo)
    {
        u32SignalFlag++;
        AVS_Restore(AVSGrp, AVSChn);
        u32SignalFlag--;
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }

    exit(-1);
}


HI_VOID SAMPLE_MISC_AVSDump(AVS_GRP Grp, AVS_CHN Chn, HI_U32 u32FrameCnt)
{
    HI_CHAR szYuvName[128];
    HI_CHAR szPixFrm[10];
    HI_U32  u32Cnt = u32FrameCnt;
    HI_S32  s32MilliSec = -1;
    HI_S32  s32Times = 10;
    HI_BOOL bSendToVgs = HI_FALSE;
    VIDEO_FRAME_INFO_S stFrmInfo;
    VGS_TASK_ATTR_S stTask;
    HI_U32 u32Width = 0;
    HI_U32 u32Height = 0;
    HI_S32 i = 0;
    HI_S32 s32Ret;
    VB_POOL_CONFIG_S stVbPoolCfg;
    HI_U32                 u32Align;
    PIXEL_FORMAT_E         enPixelFormat;
    DATA_BITWIDTH_E        enBitWidth;
    COMPRESS_MODE_E        enCmpMode;
    VB_CAL_CONFIG_S        stCalConfig;

    AVS_CHN_ATTR_S stChnAttr;

    if (HI_MPI_AVS_GetChnAttr(Grp, Chn, &stChnAttr) != HI_SUCCESS)
    {
        printf("get depth error!!!\n");
        return;
    }

    u32OrigDepth = stChnAttr.u32Depth;

    stChnAttr.u32Depth = 2;

    if (HI_MPI_AVS_SetChnAttr(Grp, Chn, &stChnAttr) != HI_SUCCESS)
    {
        printf("set depth error!!!\n");
        AVS_Restore(Grp, Chn);
        return;
    }

    u32AVSDepthFlag = 1;

    memset(&stFrame, 0, sizeof(stFrame));
    stFrame.u32PoolId = VB_INVALID_POOLID;

    while (HI_MPI_AVS_GetChnFrame(Grp, Chn, &stFrame, s32MilliSec) != HI_SUCCESS)
    {
        s32Times--;

        if (0 >= s32Times)
        {
            printf("get frame error for 10 times,now exit !!!\n");
            AVS_Restore(Grp, Chn);
            return;
        }

        usleep(40000);
    }

    switch (stFrame.stVFrame.enPixelFormat)
    {
        case PIXEL_FORMAT_YVU_SEMIPLANAR_420:
            snprintf(szPixFrm, 10, "P420");
            break;

        case PIXEL_FORMAT_YVU_SEMIPLANAR_422:
            snprintf(szPixFrm, 10, "P422");
            break;

        case PIXEL_FORMAT_YUV_400:
            snprintf(szPixFrm, 10, "P400");
            break;

        default:
            snprintf(szPixFrm, 10, "--");
            break;
    }

    /* make file name */
    snprintf(szYuvName, 128, "./avs_grp%d_chn%d_%dx%d_%s_%d.yuv", Grp, Chn,
             stFrame.stVFrame.u32Width, stFrame.stVFrame.u32Height, szPixFrm, u32Cnt);
    printf("Dump YUV frame of AVS chn %d  to file: \"%s\"\n", Chn, szYuvName);
    fflush(stdout);

    s32Ret = HI_MPI_AVS_ReleaseChnFrame(Grp, Chn, &stFrame);

    if (HI_SUCCESS != s32Ret)
    {
        printf("Release frame error ,now exit !!!\n");
        AVS_Restore(Grp, Chn);
        return;
    }

    stFrame.u32PoolId = VB_INVALID_POOLID;

    pfd = fopen(szYuvName, "wb");

    if (HI_NULL == pfd)
    {
        printf("open file failed!\n");
        AVS_Restore(Grp, Chn);
        return;
    }

    while (u32Cnt--)
    {
        if (HI_MPI_AVS_GetChnFrame(Grp, Chn, &stFrame, s32MilliSec) != HI_SUCCESS)
        {
            printf("Get frame fail \n");
            usleep(1000);
            continue;
        }

        bSendToVgs = ((COMPRESS_MODE_NONE != stFrame.stVFrame.enCompressMode)
                                    || (VIDEO_FORMAT_LINEAR != stFrame.stVFrame.enVideoFormat));

        if (bSendToVgs)
        {
            u32Align      = 0;
            enCmpMode     = COMPRESS_MODE_NONE;
            enPixelFormat = stFrame.stVFrame.enPixelFormat;
            enBitWidth    = (DYNAMIC_RANGE_SDR8 == stFrame.stVFrame.enDynamicRange) ? DATA_BITWIDTH_8 : DATA_BITWIDTH_10;
            u32Width      = stFrame.stVFrame.u32Width;
            u32Height     = stFrame.stVFrame.u32Height;

            COMMON_GetPicBufferConfig(u32Width, u32Height, enPixelFormat, enBitWidth, enCmpMode, u32Align, &stCalConfig);

            u32BlkSize = stCalConfig.u32VBSize;

            memset(&stVbPoolCfg, 0, sizeof(VB_POOL_CONFIG_S));
            stVbPoolCfg.u64BlkSize  = u32BlkSize;
            stVbPoolCfg.u32BlkCnt   = 1;
            stVbPoolCfg.enRemapMode = VB_REMAP_MODE_NONE;
            hPool   = HI_MPI_VB_CreatePool(&stVbPoolCfg);
            if (hPool == VB_INVALID_POOLID)
            {
                printf("HI_MPI_VB_CreatePool failed! \n");
                AVS_Restore(Grp, Chn);
                return;
            }

            stMem.hPool = hPool;

            while ((stMem.hBlock = HI_MPI_VB_GetBlock(stMem.hPool, u32BlkSize, HI_NULL)) == VB_INVALID_HANDLE)
            {
                ;
            }

            stMem.u64PhyAddr = HI_MPI_VB_Handle2PhysAddr(stMem.hBlock);

            stMem.pVirAddr = (HI_U8*) HI_MPI_SYS_Mmap( stMem.u64PhyAddr, u32BlkSize );

            if (stMem.pVirAddr == HI_NULL)
            {
                printf("Mem dev may not open\n");
                AVS_Restore(Grp, Chn);
                return;
            }

            memset(&stFrmInfo.stVFrame, 0, sizeof(VIDEO_FRAME_S));
            stFrmInfo.stVFrame.u64PhyAddr[0] = stMem.u64PhyAddr;
            stFrmInfo.stVFrame.u64PhyAddr[1] = stFrmInfo.stVFrame.u64PhyAddr[0] + stCalConfig.u32MainYSize;

            stFrmInfo.stVFrame.u64VirAddr[0] = (HI_U64)(HI_UL)stMem.pVirAddr;
            stFrmInfo.stVFrame.u64VirAddr[1] = (stFrmInfo.stVFrame.u64VirAddr[0] + stCalConfig.u32MainYSize);

            stFrmInfo.stVFrame.u32Width     = u32Width;
            stFrmInfo.stVFrame.u32Height    = u32Height;
            stFrmInfo.stVFrame.u32Stride[0] = stCalConfig.u32MainStride;
            stFrmInfo.stVFrame.u32Stride[1] = stCalConfig.u32MainStride;

            stFrmInfo.stVFrame.enCompressMode = COMPRESS_MODE_NONE;
            stFrmInfo.stVFrame.enPixelFormat  = stFrame.stVFrame.enPixelFormat;
            stFrmInfo.stVFrame.enVideoFormat  = VIDEO_FORMAT_LINEAR;
            stFrmInfo.stVFrame.enDynamicRange =  stFrame.stVFrame.enDynamicRange;

            stFrmInfo.stVFrame.u64PTS     = (i * 40);
            stFrmInfo.stVFrame.u32TimeRef = (i * 2);

            stFrmInfo.u32PoolId = hPool;
            stFrmInfo.enModId = HI_ID_VGS;

            s32Ret = HI_MPI_VGS_BeginJob(&hHandle);

            if (s32Ret != HI_SUCCESS)
            {
                printf("HI_MPI_VGS_BeginJob failed\n");
                hHandle = -1;
                AVS_Restore(Grp, Chn);
                return;
            }

            memcpy(&stTask.stImgIn, &stFrame, sizeof(VIDEO_FRAME_INFO_S));
            memcpy(&stTask.stImgOut , &stFrmInfo, sizeof(VIDEO_FRAME_INFO_S));
            s32Ret = HI_MPI_VGS_AddScaleTask(hHandle, &stTask, VGS_SCLCOEF_NORMAL);

            if (s32Ret != HI_SUCCESS)
            {
                printf("HI_MPI_VGS_AddScaleTask failed\n");
                AVS_Restore(Grp, Chn);
                return;
            }

            s32Ret = HI_MPI_VGS_EndJob(hHandle);

            if (s32Ret != HI_SUCCESS)
            {
                printf("HI_MPI_VGS_EndJob failed\n");
                AVS_Restore(Grp, Chn);
                return;
            }

            hHandle = -1;

            if (DYNAMIC_RANGE_SDR8 == stFrame.stVFrame.enDynamicRange)
            {
                sample_yuv_8bit_dump(&stFrmInfo.stVFrame, pfd);
            }
            else
            {
                sample_yuv_10bit_dump(&stFrmInfo.stVFrame, pfd);
            }

            HI_MPI_VB_ReleaseBlock(stMem.hBlock);

            stMem.hPool =  VB_INVALID_POOLID;
            hHandle = -1;

            if (HI_NULL != stMem.pVirAddr)
            {
                HI_MPI_SYS_Munmap((HI_VOID*)stMem.pVirAddr, u32BlkSize );
                stMem.u64PhyAddr = HI_NULL;
            }

            if (hPool != VB_INVALID_POOLID)
            {
                HI_MPI_VB_DestroyPool( hPool );
                hPool = VB_INVALID_POOLID;
            }

        }
        else
        {
            if (DYNAMIC_RANGE_SDR8 == stFrame.stVFrame.enDynamicRange)
            {
                sample_yuv_8bit_dump(&stFrame.stVFrame, pfd);
            }
            else
            {
                sample_yuv_10bit_dump(&stFrame.stVFrame, pfd);
            }
        }

        printf("Get frame %d!!\n", u32Cnt);

        s32Ret = HI_MPI_AVS_ReleaseChnFrame(Grp, Chn, &stFrame);

        if (HI_SUCCESS != s32Ret)
        {
            printf("Release frame error ,now exit !!!\n");
            AVS_Restore(Grp, Chn);
            return;
        }

        stFrame.u32PoolId = VB_INVALID_POOLID;
    }

    AVS_Restore(Grp, Chn);
    return;
}
static void usage(void)
{
    printf(
        "\n"
        "*************************************************\n"
        "Usage: ./avs_chn_dump [AVSGrp] [AVSChn] [FrmCnt]\n"
        "1)AVSGrp: \n"
        "   AVS group id\n"
        "2)AVSChn: \n"
        "   AVS chn id\n"
        "3)FrmCnt: \n"
        "   the count of frame to be dump\n"
        "*)Example:\n"
        "   e.g : ./avs_chn_dump 0 0 1\n"
        "*************************************************\n"
        "\n");
}

#ifdef __HuaweiLite__
HI_S32 avs_chn_dump(int argc, char* argv[])
#else
HI_S32 main(int argc, char* argv[])
#endif
{
    HI_U32 u32FrmCnt = 1;

    printf("\nNOTICE: This tool only can be used for TESTING !!!\n");
    printf("\tTo see more usage, please enter: ./avs_chn_dump -h\n\n");

    if (argc > 1)
    {
        if (!strncmp(argv[1], "-h", 2))
        {
            usage();
            exit(HI_SUCCESS);
        }
    }

    if (argc < 4)
    {
        usage();
        exit(HI_SUCCESS);
    }

    AVSGrp = atoi(argv[1]);

    if (!VALUE_BETWEEN(AVSGrp, 0, AVS_MAX_GRP_NUM - 1))
    {
        printf("grp id must be [0,%d]!!!!\n\n", AVS_MAX_GRP_NUM - 1);
        return -1;
    }

    AVSChn = atoi(argv[2]);

    if (!VALUE_BETWEEN(AVSChn, 0, AVS_MAX_CHN_NUM - 1))
    {
        printf("chn id must be [0,%d]!!!!\n\n", AVS_MAX_CHN_NUM - 1);
        return -1;
    }

    u32AVSDepthFlag   = 0;
    u32SignalFlag     = 0;
    pUserPageAddr[0]  = HI_NULL;
    stFrame.u32PoolId = VB_INVALID_POOLID;
    u32OrigDepth      = 0;
    hPool             = VB_INVALID_POOLID;
    hHandle           = -1;
    u32BlkSize        = 0;
    u32Size           = 0;
    pfd               = HI_NULL;

#ifndef __HuaweiLite__
    signal(SIGINT, AVS_Chn_Dump_HandleSig);
    signal(SIGTERM, AVS_Chn_Dump_HandleSig);
#endif

    u32FrmCnt = atoi(argv[3]);

    SAMPLE_MISC_AVSDump(AVSGrp, AVSChn, u32FrmCnt);

    return HI_SUCCESS;
}
