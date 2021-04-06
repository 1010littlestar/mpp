#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include "hi_comm_vb.h"
#include "mpi_vb.h"
#include "mpi_vgs.h"
#include "hi_comm_vo.h"
#include "mpi_vo.h"
#include "hi_buffer.h"

#define       MAX_FRM_CNT 256

typedef struct hiDUMP_MEMBUF_S
{
    VB_BLK  hBlock;
    VB_POOL hPool;
    HI_U32  u32PoolId;

    HI_U64  u64PhyAddr;
    HI_U64  u64VirAddr;
    HI_S32  s32Mdev;
} DUMP_MEMBUF_S;
static VIDEO_FRAME_INFO_S g_stFrame;
static VB_POOL            g_hPool           = VB_INVALID_POOLID;
static DUMP_MEMBUF_S      g_stMem;


/* sp420 to p420 ; sp422 to p422  */
void sample_yuv_dump(VIDEO_FRAME_S * pVBuf, FILE *pfd)
{
    unsigned int    w, h;
    char *          pVBufVirt_Y;
    char *          pVBufVirt_C;
    char *          pMemContent;
    unsigned char   TmpBuff[4096];
    HI_U64          phy_addr;
    HI_U32          Ysize,Csize;
    PIXEL_FORMAT_E  enPixelFormat = pVBuf->enPixelFormat;
    HI_U32          u32UvHeight;/* uv height when saved for planar type */

    Ysize = (pVBuf->u32Stride[0])*(pVBuf->u32Height);
    if (PIXEL_FORMAT_YVU_SEMIPLANAR_420 == enPixelFormat)
    {
        Csize = (pVBuf->u32Stride[1])*(pVBuf->u32Height)/2;
        u32UvHeight = pVBuf->u32Height/2;
    }
    else
    {
        Csize = (pVBuf->u32Stride[1])*(pVBuf->u32Height);
        u32UvHeight = pVBuf->u32Height;
    }

    phy_addr = pVBuf->u64PhyAddr[0];

    //printf("phy_addr:%x, size:%d\n", phy_addr, size);
    pVBufVirt_Y = (HI_CHAR *) HI_MPI_SYS_Mmap(phy_addr, Ysize);
    if (NULL == pVBufVirt_Y)
    {
        return;
    }

    pVBufVirt_C = (HI_CHAR *) HI_MPI_SYS_Mmap(pVBuf->u64PhyAddr[1], Csize);
    if (NULL == pVBufVirt_C)
    {
        HI_MPI_SYS_Munmap(pVBufVirt_Y, Ysize);
        return;
    }

    /* save Y ----------------------------------------------------------------*/
    fprintf(stderr, "saving......Y......");
    fflush(stderr);
    for(h=0; h<pVBuf->u32Height; h++)
    {
        pMemContent = pVBufVirt_Y + h*pVBuf->u32Stride[0];
        fwrite(pMemContent, pVBuf->u32Width, 1, pfd);
    }
    fflush(pfd);


    /* save U ----------------------------------------------------------------*/
    fprintf(stderr, "U......");
    fflush(stderr);
    for(h=0; h<u32UvHeight; h++)
    {
        pMemContent = pVBufVirt_C + h*pVBuf->u32Stride[1];

        pMemContent += 1;

        for(w=0; w<pVBuf->u32Width/2; w++)
        {
            TmpBuff[w] = *pMemContent;
            pMemContent += 2;
        }
        fwrite(TmpBuff, pVBuf->u32Width/2, 1, pfd);
    }
    fflush(pfd);

    /* save V ----------------------------------------------------------------*/
    fprintf(stderr, "V......");
    fflush(stderr);
    for(h=0; h<u32UvHeight; h++)
    {
        pMemContent = pVBufVirt_C + h*pVBuf->u32Stride[1];

        for(w=0; w<pVBuf->u32Width/2; w++)
        {
            TmpBuff[w] = *pMemContent;
            pMemContent += 2;
        }
        fwrite(TmpBuff, pVBuf->u32Width/2, 1, pfd);
    }
    fflush(pfd);

    fprintf(stderr, "done %d!\n", pVBuf->u32TimeRef);
    fflush(stderr);

    HI_MPI_SYS_Munmap(pVBufVirt_Y, Ysize);
    HI_MPI_SYS_Munmap(pVBufVirt_C, Csize);

}

HI_S32 SAMPLE_MISC_WbcDump(VO_DEV VoDev, HI_U32 u32Cnt)
{
    HI_S32              i, s32Ret;
    VIDEO_FRAME_INFO_S  stFrmInfo;
    HI_CHAR             szYuvName[128];
    HI_CHAR             szPixFrm[10];
    FILE *              pfd;
    HI_S32              s32MilliSec = 1000*2;


    HI_U32             u32BlkSize               = 0;
    VGS_HANDLE         hHandle                  = -1;
    VGS_TASK_ATTR_S    stTask;
    HI_U32             u32Width                 = 0;
    HI_U32             u32Height                = 0;
    HI_BOOL            bSendToVgs               = HI_FALSE;
    VB_POOL_CONFIG_S   stVbPoolCfg;

    HI_U32             u32Align;
    PIXEL_FORMAT_E     enPixelFormat;
    DATA_BITWIDTH_E    enBitWidth;
    COMPRESS_MODE_E    enCmpMode;
    VB_CAL_CONFIG_S    stCalConfig;


    printf("\nNOTICE: This tool only can be used for TESTING !!!\n");
    printf("usage: ./vo_wbc_dump 0 [frmcnt]. sample: ./vo_wbc_dump 0 5\n\n");

    s32Ret = HI_MPI_VO_SetWBCDepth(VoDev, 5);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Dev(%d) HI_MPI_VO_SetWbcDepth errno %#x\n", VoDev, s32Ret);
        return s32Ret;
    }

    /* Get Frame to make file name*/
    s32Ret = HI_MPI_VO_GetWBCFrame(VoDev, &g_stFrame, s32MilliSec);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VO(%d)_WbcGetScreenFrame errno %#x\n", VoDev, s32Ret);
        return -1;
    }

    /* make file name */
    strncpy(szPixFrm,
    (PIXEL_FORMAT_YVU_SEMIPLANAR_420 == g_stFrame.stVFrame.enPixelFormat)?"p420":"p422",5);
    snprintf(szYuvName, 128, "./Wbc(%d)_%d_%d_%s_%d.yuv",VoDev,
        g_stFrame.stVFrame.u32Width, g_stFrame.stVFrame.u32Height,szPixFrm,u32Cnt);
    printf("Dump YUV frame of Wbc(%d) to file: \"%s\"\n",VoDev, szYuvName);

    bSendToVgs = (g_stFrame.stVFrame.enCompressMode > 0) || (g_stFrame.stVFrame.enVideoFormat > 0);


    s32Ret = HI_MPI_VO_ReleaseWBCFrame(VoDev, &g_stFrame);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Dev(%d) HI_MPI_VO_ReleaseWbcFrame errno %#x\n", VoDev, s32Ret);
        return -1;
    }

    /* open file */
    pfd = fopen(szYuvName, "wb");

    if (NULL == pfd)
    {
        return -1;
    }

    u32Width      = g_stFrame.stVFrame.u32Width;
    u32Height     = g_stFrame.stVFrame.u32Height;
    u32Align      = 0;
    enCmpMode     = COMPRESS_MODE_NONE;
    enPixelFormat = g_stFrame.stVFrame.enPixelFormat;
    enBitWidth    = (DYNAMIC_RANGE_SDR8 == g_stFrame.stVFrame.enDynamicRange) ? DATA_BITWIDTH_8 : DATA_BITWIDTH_10;

    if (bSendToVgs)
    {
        memset(&stCalConfig, 0, sizeof(VB_CAL_CONFIG_S));
        COMMON_GetPicBufferConfig(u32Width, u32Height, enPixelFormat, enBitWidth, enCmpMode, u32Align, &stCalConfig);

        u32BlkSize          = stCalConfig.u32VBSize;

        memset(&stVbPoolCfg, 0, sizeof(VB_POOL_CONFIG_S));
        stVbPoolCfg.u64BlkSize  = u32BlkSize;
        stVbPoolCfg.u32BlkCnt   = 1;
        stVbPoolCfg.enRemapMode = VB_REMAP_MODE_NONE;
        g_hPool   = HI_MPI_VB_CreatePool(&stVbPoolCfg);
        if (g_hPool == VB_INVALID_POOLID)
        {
            printf("HI_MPI_VB_CreatePool BlkSize = %d failed! \n",u32BlkSize);
            goto END1;
        }

        g_stMem.hPool = g_hPool;
    }


    /* get VO frame  */
    for (i=0; i<u32Cnt; i++)
    {
        s32Ret = HI_MPI_VO_GetWBCFrame(VoDev, &g_stFrame, s32MilliSec);
        if (HI_SUCCESS != s32Ret)
        {
            printf("get Wbc(%d) frame err\n", VoDev);
            printf("only get %d frame\n", i);
            break;
        }

        if (bSendToVgs)
        {
            while ((g_stMem.hBlock = HI_MPI_VB_GetBlock(g_stMem.hPool, u32BlkSize, NULL)) == VB_INVALID_HANDLE)
            {
                ;
            }

            g_stMem.u64PhyAddr = HI_MPI_VB_Handle2PhysAddr(g_stMem.hBlock);


            g_stMem.u64VirAddr = (HI_UL) HI_MPI_SYS_Mmap( g_stMem.u64PhyAddr, u32BlkSize );
            if (g_stMem.u64VirAddr == 0)
            {
                printf("Mem dev may not open\n");
                HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
                goto END2;
            }

            memset(&stFrmInfo.stVFrame, 0, sizeof(VIDEO_FRAME_S));
            stFrmInfo.stVFrame.u32Width       = u32Width;
            stFrmInfo.stVFrame.u32Height      = u32Height;
            stFrmInfo.stVFrame.enDynamicRange = g_stFrame.stVFrame.enDynamicRange;
            stFrmInfo.stVFrame.enField        = VIDEO_FIELD_FRAME;
            stFrmInfo.stVFrame.u64PhyAddr[0]  = g_stMem.u64PhyAddr;
            stFrmInfo.stVFrame.u64PhyAddr[1]  = stFrmInfo.stVFrame.u64PhyAddr[0] + stCalConfig.u32MainYSize;
            stFrmInfo.stVFrame.u64PhyAddr[2]  = stFrmInfo.stVFrame.u64PhyAddr[1] + stCalConfig.u32MainYSize;

            stFrmInfo.stVFrame.u64VirAddr[0]  = g_stMem.u64VirAddr;
            stFrmInfo.stVFrame.u64VirAddr[1]  = (HI_UL)stFrmInfo.stVFrame.u64VirAddr[0] + stCalConfig.u32MainYSize;
            stFrmInfo.stVFrame.u64VirAddr[2]  = (HI_UL)stFrmInfo.stVFrame.u64VirAddr[1] + stCalConfig.u32MainYSize;

            stFrmInfo.stVFrame.u32Stride[0]   = stCalConfig.u32MainStride;
            stFrmInfo.stVFrame.u32Stride[1]   = stCalConfig.u32MainStride;
            stFrmInfo.stVFrame.u32Stride[2]   = stCalConfig.u32MainStride;

            stFrmInfo.stVFrame.enCompressMode = COMPRESS_MODE_NONE;
            stFrmInfo.stVFrame.enPixelFormat  = g_stFrame.stVFrame.enPixelFormat;
            stFrmInfo.stVFrame.enVideoFormat  = VIDEO_FORMAT_LINEAR;

            stFrmInfo.stVFrame.u64PTS         = (i * 40);
            stFrmInfo.stVFrame.u32TimeRef     = (i * 2);

            stFrmInfo.enModId   = HI_ID_VGS;
            stFrmInfo.u32PoolId = g_hPool;

            s32Ret = HI_MPI_VGS_BeginJob(&hHandle);
            if (s32Ret != HI_SUCCESS)
            {
                printf("HI_MPI_VGS_BeginJob failed\n");
                HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
                g_stMem.hBlock = VB_INVALID_HANDLE;
                HI_MPI_VO_ReleaseWBCFrame(VoDev, &g_stFrame);
                goto END2;
            }

            memcpy(&stTask.stImgIn, &g_stFrame, sizeof(VIDEO_FRAME_INFO_S));
            memcpy(&stTask.stImgOut , &stFrmInfo, sizeof(VIDEO_FRAME_INFO_S));
            s32Ret = HI_MPI_VGS_AddScaleTask(hHandle, &stTask, VGS_SCLCOEF_NORMAL);
            if (s32Ret != HI_SUCCESS)
            {
                printf("HI_MPI_VGS_AddScaleTask failed\n");
                HI_MPI_VGS_CancelJob(hHandle);
                HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
                g_stMem.hBlock = VB_INVALID_HANDLE;
                HI_MPI_VO_ReleaseWBCFrame(VoDev, &g_stFrame);
                goto END2;
            }

            s32Ret = HI_MPI_VGS_EndJob(hHandle);
            if (s32Ret != HI_SUCCESS)
            {
                printf("HI_MPI_VGS_EndJob failed\n");
                HI_MPI_VGS_CancelJob(hHandle);
                HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
                g_stMem.hBlock = VB_INVALID_HANDLE;
                HI_MPI_VO_ReleaseWBCFrame(VoDev, &g_stFrame);
                goto END2;
            }

            /* save VO frame to file */
            //sample_yuv_dump(&stFrmInfo.stVFrame);

            sample_yuv_dump(&stFrmInfo.stVFrame, pfd);

            HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
            g_stMem.hBlock = VB_INVALID_HANDLE;
        }
        else
        {

            /* save VO frame to file */
            sample_yuv_dump(&g_stFrame.stVFrame, pfd);
        }
        /* release frame after using */
        s32Ret = HI_MPI_VO_ReleaseWBCFrame(VoDev, &g_stFrame);
        memset(&g_stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
        if (HI_SUCCESS != s32Ret)
        {
            printf("release Wbc(%d) frame err\n", VoDev);
            printf("only get %d frame\n", i);
            break;
        }
    }

END2:
    if (VB_INVALID_POOLID != g_hPool)
    {
        HI_MPI_VB_DestroyPool(g_hPool);
        g_hPool = VB_INVALID_POOLID;
    }
    memset(&g_stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));

END1:

    fclose(pfd);

    return 0;
}

HI_S32 main(int argc, char *argv[])
{
    VO_DEV VoDev     = 0;
    HI_U32 u32FrmCnt = 1;

    /* VO dev num*/
    if (argc > 1)
    {
        VoDev = atoi(argv[1]);
    }

    /* frame number that need to capture*/
    if (argc > 2)
    {
        u32FrmCnt = atoi(argv[2]);
    }

    SAMPLE_MISC_WbcDump(VoDev, u32FrmCnt);

    return HI_SUCCESS;
}

