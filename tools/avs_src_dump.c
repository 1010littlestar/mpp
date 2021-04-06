#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include "hi_comm_vb.h"
#include "mpi_vb.h"
#include "mpi_avs.h"
#include "mpi_vgs.h"
#include "hi_math.h"
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



static VIDEO_FRAME_INFO_S stFrame;
static DUMP_MEMBUF_S stMem = {0};
static AVS_GRP AVSGrp = 0;
static AVS_PIPE  AVSPipe = 0;
static VB_POOL hPool  = VB_INVALID_POOLID;
static FILE* pfd = HI_NULL;
static HI_U32  u32BlkSize = 0;
static HI_CHAR* pUserPageAddr[2] = {HI_NULL, HI_NULL};
static HI_U32 size = 0;
static HI_U32 u32SignalFlag = 0;
static HI_U32 u32Size = 0;
static VGS_HANDLE hHandle = -1;
static unsigned char* TmpBuff = HI_NULL;
static HI_U16* src = HI_NULL;
static HI_U8* dest = HI_NULL;

static void sample_blk_yuv_dump(VIDEO_FRAME_S* pVBuf, FILE* pfd)
{
    unsigned int  h;
    char* pVBufVirt_Y;
    char* pMemContent;
    HI_U64 phy_addr;


    u32Size = (pVBuf->u32Stride[0]) * (pVBuf->u32Height) ;


    phy_addr = pVBuf->u64PhyAddr[0];

    pUserPageAddr[0] = (HI_CHAR*) HI_MPI_SYS_Mmap(phy_addr, u32Size);

    if (HI_NULL == pUserPageAddr[0])
    {
        return;
    }

    pVBufVirt_Y = pUserPageAddr[0];

    fprintf(stderr, "saving......blk......");
    fflush(stderr);

    for (h = 0; h < pVBuf->u32Height; h++)
    {
        pMemContent = pVBufVirt_Y + h * pVBuf->u32Stride[0];
        fwrite(pMemContent, pVBuf->u32Stride[0], 1, pfd);
    }


    fflush(pfd);

    fprintf(stderr, "done %d!\n", pVBuf->u32TimeRef);
    fflush(stderr);

    HI_MPI_SYS_Munmap(pUserPageAddr[0], u32Size);
    pUserPageAddr[0] = HI_NULL;
}

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


static HI_S32 AVS_SRC_Restore(AVS_GRP AVSGrp, AVS_PIPE AVSPipe)
{
    if (VB_INVALID_POOLID != stFrame.u32PoolId)
    {
        HI_MPI_AVS_ReleasePipeFrame(AVSGrp, AVSPipe, &stFrame);
        stFrame.u32PoolId = VB_INVALID_POOLID;
    }

    if (HI_NULL != pUserPageAddr[0])
    {
        HI_MPI_SYS_Munmap(pUserPageAddr[0], size);
        pUserPageAddr[0] = HI_NULL;
    }

    if (HI_NULL != stMem.pVirAddr)
    {
        HI_MPI_SYS_Munmap((HI_VOID*)stMem.pVirAddr, u32BlkSize);
        stMem.u64PhyAddr = HI_NULL;
    }

    if (VB_INVALID_POOLID != stMem.hPool)
    {
        HI_MPI_VB_ReleaseBlock(stMem.hBlock);
        stMem.hPool = VB_INVALID_POOLID;
    }

    if (VB_INVALID_POOLID != hPool)
    {
        HI_MPI_VB_DestroyPool(hPool);
        hPool = VB_INVALID_POOLID;
    }

    if (pfd)
    {
        fclose(pfd);
        pfd = HI_NULL;
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

void AVS_Src_Dump_HandleSig(HI_S32 signo)
{
    if (u32SignalFlag)
    {
        exit(-1);
    }

    if (SIGINT == signo || SIGTERM == signo)
    {
        u32SignalFlag++;
        AVS_SRC_Restore(AVSGrp, AVSPipe);
        u32SignalFlag--;
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }

    exit(-1);
}


HI_S32 SAMPLE_MISC_AVSDumpSrcImage(AVS_GRP Grp, AVS_PIPE Pipe, HI_U32 u32FrameCnt)
{
    HI_CHAR szYuvName[128];
    HI_CHAR szPixFrm[10];
    HI_S32 s32Times  = 10;
    AVS_GRP AVSGrp   = Grp;
    AVS_PIPE AVSPipe = Pipe;
    HI_S32 s32Ret;
    HI_BOOL bSendToVgs = HI_FALSE;
    VIDEO_FRAME_INFO_S stFrmInfo;
    VGS_TASK_ATTR_S stTask;
    HI_U32 u32Width      = 0;
    HI_U32 u32Height     = 0;
    HI_S32 i = 0;
    VB_POOL_CONFIG_S stVbPoolCfg;
    HI_U32                 u32Align;
    PIXEL_FORMAT_E         enPixelFormat;
    DATA_BITWIDTH_E        enBitWidth;
    COMPRESS_MODE_E        enCmpMode;
    VB_CAL_CONFIG_S        stCalConfig;


    while ((HI_MPI_AVS_GetPipeFrame(AVSGrp, AVSPipe, &stFrame) != HI_SUCCESS))
    {
        s32Times--;

        if (0 >= s32Times)
        {
            printf("get frame error for 10 times,now exit !!!\n");
            return -1;
        }

        sleep(2);
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

    if (VIDEO_FORMAT_LINEAR == stFrame.stVFrame.enVideoFormat)
    {
        snprintf(szYuvName, 128, "./avs%d_pipe%d_%ux%u_%s_%u.yuv", AVSGrp, AVSPipe,
                 stFrame.stVFrame.u32Width, stFrame.stVFrame.u32Height, szPixFrm, u32FrameCnt);
    }
    else
    {
        snprintf(szYuvName, 128, "./avs%d_pipe%d_%ux%u_s%u_%s_%u.blk", AVSGrp, AVSPipe,
                 stFrame.stVFrame.u32Width, stFrame.stVFrame.u32Height, stFrame.stVFrame.u32Stride[0], szPixFrm, u32FrameCnt);
    }

    printf("Dump YUV frame of vpss%d pipe%d to file: \"%s\"\n", AVSGrp, AVSPipe, szYuvName);

    s32Ret = HI_MPI_AVS_ReleasePipeFrame(AVSGrp, AVSPipe, &stFrame);

    if (HI_SUCCESS != s32Ret)
    {
        printf("Release frame error ,now exit !!!\n");
        AVS_SRC_Restore(Grp, AVSPipe);
        return -1;
    }

    pfd = fopen(szYuvName, "wb");

    if (HI_NULL == pfd)
    {
        printf("open file failed!\n");
        HI_MPI_AVS_ReleasePipeFrame(AVSGrp, AVSPipe, &stFrame);
        stFrame.u32PoolId = VB_INVALID_POOLID;
        return -1;
    }

    while (u32FrameCnt--)
    {
        if (HI_MPI_AVS_GetPipeFrame(Grp, Pipe, &stFrame) != HI_SUCCESS)
        {
            printf("Get frame fail \n");
            usleep(1000);
            continue;
        }

        if (VIDEO_FORMAT_TILE_16x8 == stFrame.stVFrame.enVideoFormat)
        {
            sample_blk_yuv_dump(&stFrame.stVFrame, pfd);
        }
        else
        {
            bSendToVgs = ((stFrame.stVFrame.enCompressMode > 0) || (stFrame.stVFrame.enVideoFormat > 0));

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

                hPool = HI_MPI_VB_CreatePool(&stVbPoolCfg);

                if (hPool == VB_INVALID_POOLID)
                {
                    printf("HI_MPI_VB_CreatePool failed! \n");
                    AVS_SRC_Restore(Grp, Pipe);
                    return -1;
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
                    AVS_SRC_Restore(Grp, Pipe);
                    return -1;
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
                    AVS_SRC_Restore(Grp, Pipe);
                    return -1;
                }

                memcpy(&stTask.stImgIn, &stFrame, sizeof(VIDEO_FRAME_INFO_S));
                memcpy(&stTask.stImgOut , &stFrmInfo, sizeof(VIDEO_FRAME_INFO_S));
                s32Ret = HI_MPI_VGS_AddScaleTask(hHandle, &stTask, VGS_SCLCOEF_NORMAL);

                if (s32Ret != HI_SUCCESS)
                {
                    printf("HI_MPI_VGS_AddScaleTask failed\n");
                    AVS_SRC_Restore(Grp, Pipe);
                    return -1;
                }

                s32Ret = HI_MPI_VGS_EndJob(hHandle);

                if (s32Ret != HI_SUCCESS)
                {
                    printf("HI_MPI_VGS_EndJob failed\n");
                    AVS_SRC_Restore(Grp, Pipe);
                    return -1;
                }

                hHandle = -1;

                /* save VO frame to file */
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

        }


        s32Ret = HI_MPI_AVS_ReleasePipeFrame(Grp, Pipe, &stFrame);
        stFrame.u32PoolId = VB_INVALID_POOLID;

        if (HI_SUCCESS != s32Ret)
        {
            printf("Release frame error ,now exit !!!\n");
            AVS_SRC_Restore(Grp, Pipe);
            return  - 1;
        }

    }

    AVS_SRC_Restore(Grp, Pipe);

    return 0;
}

static void usage(void)
{
    printf(
        "\n"
        "*************************************************\n"
        "Usage: ./avs_src_dump [Grp] [Pipe] [FrmCnt]\n"
        "1)Grp: \n"
        "   AVS group id\n"
        "2)Pipe:\n"
        "   AVS pipe\n"
        "3)FrmCnt: \n"
        "   the count of frame to be dump\n"
        "*)Example:\n"
        "e.g : ./avs_src_dump 0 0 1\n"
        "*************************************************\n"
        "\n");
}

#ifdef __HuaweiLite__
HI_S32 avs_src_dump(int argc, char* argv[])
#else
HI_S32 main(int argc, char* argv[])
#endif
{
    HI_U32 u32FrmCnt = 1;

    AVSGrp  = 0;
    AVSPipe = 0;

    printf("\nNOTICE: This tool only can be used for TESTING !!!\n");
    printf("\tTo see more usage, please enter: ./avs_src_dump -h\n\n");

    if (argc > 1)
    {
        if (!strncmp(argv[1], "-h", 2))
        {
            usage();
            exit(HI_SUCCESS);
        }

        AVSGrp = atoi(argv[1]);
    }

    if (argc > 2)
    {
        AVSPipe = atoi(argv[2]);
    }

    if (argc > 3)
    {
        u32FrmCnt = atoi(argv[3]);
    }


    if (!VALUE_BETWEEN(AVSGrp, 0, AVS_MAX_GRP_NUM - 1))
    {
        printf("grp id must be [0,%d]!!!!\n\n", AVS_MAX_GRP_NUM - 1);
        return -1;
    }

    if (!VALUE_BETWEEN(AVSPipe, 0, AVS_PIPE_NUM - 1))
    {
        printf("AVSPipe must be [0,%d]!!!!\n\n", AVS_PIPE_NUM - 1);
        return -1;
    }

    stFrame.u32PoolId = VB_INVALID_POOLID;
    pUserPageAddr[0]  = HI_NULL;
    stMem.u64PhyAddr  = HI_NULL;
    stMem.hPool       = VB_INVALID_POOLID;
    hPool             = VB_INVALID_POOLID;
    pfd               = HI_NULL;

#ifndef __HuaweiLite__
    signal(SIGINT, AVS_Src_Dump_HandleSig);
    signal(SIGTERM, AVS_Src_Dump_HandleSig);
#endif

    SAMPLE_MISC_AVSDumpSrcImage(AVSGrp, AVSPipe, u32FrmCnt);

    return HI_SUCCESS;
}

