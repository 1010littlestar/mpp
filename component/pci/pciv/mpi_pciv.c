/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description   : MPI for user
 * Author        : Hisilicon multimedia software group
 * Created       : 2009/06/24
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>

#include "mpi_pciv.h"
#include "mkp_pciv.h"

#include "mpi_pciv_adapt.c"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* ****************************************************************************
Description     : Create and initialize the pciv channel.
Input           : pcivChn  ** The pciv channel id between [0, PCIV_MAX_CHN_NUM)
Output          : None
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_INVALID_CHNID
                  HI_ERR_PCIV_SYS_NOTREADY
                  HI_ERR_PCIV_EXIST
                  HI_FAILURE

See Also        : HI_MPI_PCIV_Destroy
**************************************************************************** */
HI_S32 HI_MPI_PCIV_Create(PCIV_CHN pcivChn, const PCIV_ATTR_S *pPcivAttr)
{
    HI_ASSERT(sizeof(PCIV_ATTR_S) == sizeof(hi_pciv_attr));
    return hi_mpi_pciv_create((hi_pciv_chn)pcivChn, (hi_pciv_attr*)pPcivAttr);
}

/* ****************************************************************************
Description     : Destroy the pciv channel
Input           : pcivChn  ** The pciv channel id
Output          : None
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_INVALID_CHNID
                  HI_ERR_PCIV_SYS_NOTREADY
                  HI_FAILURE

See Also        : HI_MPI_PCIV_Create
**************************************************************************** */
HI_S32 HI_MPI_PCIV_Destroy(PCIV_CHN pcivChn)
{
    return hi_mpi_pciv_destroy((hi_pciv_chn)pcivChn);
}

/* ****************************************************************************
Description     : Set the attribute of pciv channel
Input           : pcivChn    ** The pciv channel id
               pPcivAttr  ** The attribute of pciv channel
Output          : None
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_INVALID_CHNID
                  HI_ERR_PCIV_SYS_NOTREADY
                  HI_ERR_PCIV_NULL_PTR
                  HI_ERR_PCIV_UNEXIST
                  HI_ERR_PCIV_ILLEGAL_PARAM
                  HI_FAILURE

See Also        : HI_MPI_PCIV_GetAttr
**************************************************************************** */
HI_S32 HI_MPI_PCIV_SetAttr(PCIV_CHN pcivChn, const PCIV_ATTR_S *pPcivAttr)
{
    HI_ASSERT(sizeof(PCIV_ATTR_S) == sizeof(hi_pciv_attr));
    return hi_mpi_pciv_set_attr((hi_pciv_chn)pcivChn, (hi_pciv_attr*)pPcivAttr);
}

/* ****************************************************************************
Description     : Get the attribute of pciv channel
Input           : pcivChn    ** The pciv channel id
Output          : pPcivAttr  ** The attribute of pciv channel
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_INVALID_CHNID
                  HI_ERR_PCIV_SYS_NOTREADY
                  HI_ERR_PCIV_NULL_PTR
                  HI_ERR_PCIV_UNEXIST
                  HI_FAILURE

See Also        : HI_MPI_PCIV_SetAttr
**************************************************************************** */
HI_S32 HI_MPI_PCIV_GetAttr(PCIV_CHN pcivChn, PCIV_ATTR_S *pPcivAttr)
{
    HI_ASSERT(sizeof(PCIV_ATTR_S) == sizeof(hi_pciv_attr));
    return hi_mpi_pciv_get_attr((hi_pciv_chn)pcivChn, (hi_pciv_attr*)pPcivAttr);
}

/* ****************************************************************************
Description     : Start to send or receive video frame
Input           : pcivChn    ** The pciv channel id
Output          : None
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_INVALID_CHNID
                  HI_ERR_PCIV_SYS_NOTREADY
                  HI_ERR_PCIV_UNEXIST
                  HI_ERR_PCIV_NOT_CONFIG
                  HI_FAILURE

See Also        : HI_MPI_PCIV_Stop
**************************************************************************** */
HI_S32 HI_MPI_PCIV_Start(PCIV_CHN pcivChn)
{
    return hi_mpi_pciv_start((hi_pciv_chn)pcivChn);
}

/* ****************************************************************************
Description     : Stop send or receive video frame
Input           : pcivChn    ** The pciv channel id
Output          : None
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_INVALID_CHNID
                  HI_ERR_PCIV_SYS_NOTREADY
                  HI_ERR_PCIV_UNEXIST
                  HI_FAILURE

See Also        : HI_MPI_PCIV_Start
**************************************************************************** */
HI_S32 HI_MPI_PCIV_Stop(PCIV_CHN pcivChn)
{
    return hi_mpi_pciv_stop((hi_pciv_chn)pcivChn);
}

/* ****************************************************************************
Description     : Create a series of dma task
Input           : pTask    ** The task list to create
Output          : None
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_SYS_NOTREADY
                  HI_ERR_PCIV_NULL_PTR
                  HI_ERR_PCIV_ILLEGAL_PARAM
                  HI_ERR_PCIV_NOBUF
                  HI_FAILURE

See Also        : None
**************************************************************************** */
HI_S32 HI_MPI_PCIV_DmaTask(PCIV_DMA_TASK_S *pTask)
{
    HI_ASSERT(sizeof(PCIV_DMA_TASK_S) == sizeof(hi_pciv_dma_task));

    return hi_mpi_pciv_dma_task((hi_pciv_dma_task*)pTask);
}

/* ****************************************************************************
Description     : Alloc 'u32BlkSize' bytes memory and give the physical address
                  The memory used by PCI must be located within the PCI window,
                  So you should call this function to alloc it.
Input           : u32BlkSize    ** The size of each memory block
                  u32BlkCnt     ** The count of memory block
Output          : u64PhyAddr    ** The physical address of the memory
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_SYS_NOTREADY
                  HI_ERR_PCIV_NOBUF
                  HI_FAILURE

See Also        : HI_MPI_PCIV_Free
**************************************************************************** */
HI_S32 HI_MPI_PCIV_Malloc(HI_U32 u32BlkSize, HI_U32 u32BlkCnt, HI_U64 au64PhyAddr[])
{
    return hi_mpi_pciv_malloc((hi_u32)u32BlkSize, (hi_u32)u32BlkCnt, (hi_u64*)au64PhyAddr);
}

/* ****************************************************************************
Description     : Free the memory
Input           : u32BlkCnt     ** The count of memory block
Output          : u64PhyAddr    ** The physical address of the memory
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_SYS_NOTREADY

See Also        : HI_MPI_PCIV_Free
**************************************************************************** */
HI_S32 HI_MPI_PCIV_Free(HI_U32 u32BlkCnt, const HI_U64 au64PhyAddr[])
{
    return hi_mpi_pciv_free((hi_u32)u32BlkCnt, (hi_u64*)au64PhyAddr);
}

/* ****************************************************************************
Description     : Alloc 'u32BlkSize' bytes memory and give the physical address
                  The memory used by PCI must be located within the PCI window,
                  So you should call this function to alloc it.
Input           : u32BlkSize    ** The size of each memory block
                  u32BlkCnt     ** The count of memory block
Output          : u64PhyAddr    ** The physical address of the memory
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_SYS_NOTREADY
                  HI_ERR_PCIV_NOBUF
                  HI_FAILURE

See Also        : HI_MPI_PCIV_Free
**************************************************************************** */
HI_S32 HI_MPI_PCIV_MallocChnBuffer(PCIV_CHN pcivChn, HI_U32 u32BlkSize, HI_U32 u32BlkCnt, HI_U64 au64PhyAddr[])
{
    return hi_mpi_pciv_malloc_chn_buffer((hi_pciv_chn)pcivChn, (hi_u32)u32BlkSize,
        (hi_u32)u32BlkCnt, (hi_u64*)au64PhyAddr);
}

/* ****************************************************************************
Description     : Free the memory
Input           : u32BlkCnt     ** The count of memory block
Output          : u64PhyAddr    ** The physical address of the memory
Return Value    : HI_SUCCESS
                  HI_ERR_PCIV_SYS_NOTREADY

See Also        : HI_MPI_PCIV_Free
**************************************************************************** */
HI_S32 HI_MPI_PCIV_FreeChnBuffer(PCIV_CHN pcivChn, HI_U32 u32BlkCnt)
{
    return hi_mpi_pciv_free_chn_buffer((hi_pciv_chn)pcivChn, (hi_u32)u32BlkCnt);
}

/* ****************************************************************************
Description     : Get the ID of this chip
Input           : None
Output          : None
Return Value    : The chip ID if success
                  HI_FAILURE or HI_ERR_PCIV_SYS_NOTREADY  if failure

See Also        : HI_MPI_PCIV_GetBaseWindow
**************************************************************************** */
HI_S32 HI_MPI_PCIV_GetLocalId(HI_VOID)
{
    return hi_mpi_pciv_get_local_id();
}

/* ****************************************************************************
 Description    : Enum all the connected chip. Need check the invalid value (-1)
Input           : s32ChipID  ** The chip id array
Output          : None
Return Value    : HI_SUCCESS if success.
                  HI_FAILURE if failure

See Also        : HI_MPI_PCIV_GetLocalId
                  HI_MPI_PCIV_GetBaseWindow
**************************************************************************** */
HI_S32 HI_MPI_PCIV_EnumChip(HI_S32 as32ChipID[PCIV_MAX_CHIPNUM])
{
    return hi_mpi_pciv_enum_chip((hi_s32*)as32ChipID);
}

/* ****************************************************************************
Description  : On the host, you can get all the slave chip's NP,PF and CFG window
               On the slave, you can only get the PF AHB Addres of itself.
Input        : s32ChipId     ** The chip Id which you want to access
Output       : pBase         ** On host  pBase->u32NpWinBase,
                                         pBase->u32PfWinBase,
                                         pBase->u32CfgWinBase
                                On Slave pBase->u32PfAHBAddr
Return Value : HI_SUCCESS if success.
               HI_ERR_PCIV_SYS_NOTREADY
               HI_ERR_PCIV_NULL_PTR
               HI_FAILURE

See Also     : HI_MPI_PCIV_GetLocalId
**************************************************************************** */
HI_S32 HI_MPI_PCIV_GetBaseWindow(HI_S32 s32ChipId, PCIV_BASEWINDOW_S *pBase)
{
    HI_ASSERT(sizeof(PCIV_BASEWINDOW_S) == sizeof(hi_pciv_base_window));
    return hi_mpi_pciv_get_base_window((hi_s32)s32ChipId, (hi_pciv_base_window*)pBase);
}

/* ****************************************************************************
Description  : Only on the slave chip, you need to create some VB Pool.
               Those pool will bee created on the PCI Window Zone.
Input        : pCfg.u32PoolCount ** The total number of pool want to create
               pCfg.u32BlkSize[] ** The size of each VB block
               pCfg.u32BlkCount[]** The number of each VB block

Output       : None
Return Value : HI_SUCCESS if success.
               HI_ERR_PCIV_SYS_NOTREADY
               HI_ERR_PCIV_NULL_PTR
               HI_ERR_PCIV_NOMEM
               HI_ERR_PCIV_BUSY
               HI_ERR_PCIV_NOT_SUPPORT
               HI_FAILURE

See Also     : HI_MPI_PCIV_GetLocalId
**************************************************************************** */
HI_S32 HI_MPI_PCIV_WinVbCreate(const PCIV_WINVBCFG_S *pCfg)
{
    HI_ASSERT(sizeof(PCIV_WINVBCFG_S) == sizeof(hi_pciv_win_vb_cfg));
    return hi_mpi_pciv_win_vb_create((hi_pciv_win_vb_cfg*)pCfg);
}

/* ****************************************************************************
Description  : Destroy the pools which's size is equal to the pCfg.u32BlkSize[]
Input        : pCfg.u32PoolCount ** The total number of pool want to destroy
               pCfg.u32BlkSize[] ** The size of each VB block
               pCfg.u32BlkCount[]** Don't care this parament

Output       : None
Return Value : HI_SUCCESS if success.
               HI_ERR_PCIV_SYS_NOTREADY
               HI_ERR_PCIV_NOT_SUPPORT
               HI_FAILURE

See Also     : HI_MPI_PCIV_GetLocalId
**************************************************************************** */
HI_S32 HI_MPI_PCIV_WinVbDestroy(HI_VOID)
{
    return hi_mpi_pciv_win_vb_destroy();
}

/* ****************************************************************************
Description  :
Input        : pCfg.u32PoolCount ** The total number of pool want to destroy
               pCfg.u32BlkSize[] ** The size of each VB block
               pCfg.u32BlkCount[]** Don't care this parament

Output       : None
Return Value : HI_SUCCESS if success.
               HI_ERR_PCIV_SYS_NOTREADY
               HI_ERR_PCIV_NOT_SUPPORT
               HI_FAILURE
**************************************************************************** */
HI_S32 HI_MPI_PCIV_Show(PCIV_CHN pcivChn)
{
    return hi_mpi_pciv_show((hi_pciv_chn)pcivChn);
}

/* ****************************************************************************
Description  :
Input        : pCfg.u32PoolCount ** The total number of pool want to destroy
               pCfg.u32BlkSize[] ** The size of each VB block
               pCfg.u32BlkCount[]** Don't care this parament

Output       : None
Return Value : HI_SUCCESS if success.
               HI_ERR_PCIV_SYS_NOTREADY
               HI_ERR_PCIV_NOT_SUPPORT
               HI_FAILURE
**************************************************************************** */
HI_S32 HI_MPI_PCIV_Hide(PCIV_CHN pcivChn)
{
    return hi_mpi_pciv_hide((hi_pciv_chn)pcivChn);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

