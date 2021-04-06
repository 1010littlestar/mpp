/*
 * Copyright (c) 2015 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of
 * Cadence Design Systems Inc.  They may be adapted and modified by bona fide
 * purchasers for internal use, but neither the original nor any adapted
 * or modified version may be disclosed or distributed to third parties
 * in any manner, medium, or form, in whole or in part, without the prior
 * written consent of Cadence Design Systems Inc.  This software and its
 * derivatives are to be executed solely on products incorporating a Cadence
 * Design Systems processor.
 */


#ifndef __XMEM_H__
#define __XMEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define XMEM_MGR_SIZE  (96)

#ifndef __XMEM_INTERNAL_H__
#ifndef XMEM_MAX_DCACHE_LINESIZE
#define XMEM_MAX_DCACHE_LINESIZE  64
#endif
/* Opaque memory manager structure */
struct xmem_mgr_struct
{
  char _[((XMEM_MGR_SIZE + XMEM_MAX_DCACHE_LINESIZE - 1) / XMEM_MAX_DCACHE_LINESIZE) * XMEM_MAX_DCACHE_LINESIZE];
}  __attribute__ ((aligned(XMEM_MAX_DCACHE_LINESIZE)));
#endif

#define XMEM_HEAP3_BLOCK_STRUCT_SIZE  (16)

typedef struct xmem_mgr_struct  xmem_mgr_t;

/* Return val of the API calls */
typedef enum
{
  XMEM_OK                      = 0,
  XMEM_ERROR_POOL_NOT_ALIGNED  = -1,
  XMEM_ERROR_POOL_SIZE         = -2,
  XMEM_ERROR_POOL_NULL         = -3,
  XMEM_ERROR_BLOCK_SIZE        = -4,
  XMEM_ERROR_UNKNOWN_HEAP_TYPE = -5,
  XMEM_ERROR_ALLOC_FAILED      = -6,
  XMEM_ERROR_ILLEGAL_ALIGN     = -7,
  XMEM_ERROR_UNINITIALIZED     = -8,
  XMEM_ERROR_INTERNAL          = -100
} xmem_status_t;

/* Initialize type 1 memory manager.
 *
 * m      : generic memory manager
 * buffer : user provided buffer for allocation
 * size   : size of the buffer
 *
 * Returns XMEM_OK if successful, else returns XMEM_ERROR_INTERNAL, or
 * XMEM_ERROR_POOL_NULL.
 */
xmem_status_t
xmem_heap1_init(xmem_mgr_t *m, void *buffer, size_t size);

/* Initialize the type 2 heap.
 *
 * m          : generic memory manager
 * buf        : user provided buffer for allocation
 * size       : size of the buffer
 * block_size : size of the block
 * header     : Optional buffer used to store the bitvectors. If NULL, the
 *              headers are allocated out of the user provided buf.
 * size       : size of the buffer
 *
 * Returns XMEM_OK if successful, else returns one of XMEM_ERROR_POOL_NULL,
 * XMEM_ERROR_INTERNAL, XMEM_ERROR_BLOCK_SIZE, XMEM_ERROR_POOL_SIZE,
 * XMEM_ERROR_POOL_NOT_ALIGNED.
 */
xmem_status_t
xmem_heap2_init(xmem_mgr_t *m,
                void *buf,
                size_t size,
                size_t block_size,
                void *header);

/* Macro to returns the number of bytes required to hold the header information
 * for heap2 - the 2 bitvectors that are used to track the free blocks and the
 * the number of allocated blocks. The caller can use this macro to
 * reserve bytes for the header that gets passed optionally to xmem_heap2_init.
 * Assumes that buffer_size is a multiple of block_size which is a power-of-2.
 *
 * buffer_size     : size of the user buffer in bytes
 * log2 block_size : log2(size of block in bytes)
 */
#define XMEM_HEAP2_HEADER_SIZE(buffer_size, log2_block_size) \
  (((((buffer_size) >> (log2_block_size)) + 31) >> 5) * 2 * 4)

#define XMEM_HEAP2_CACHE_ALIGNED_HEADER_SIZE(buffer_size, log2_block_size) \

/* Macro to returns the number of bytes required to hold the header information
 * for heap3 - the array of block structures that are used to track the free and
 * allocated blocks and a bitvector to track available block structures.
 * the caller can use this macro to reserve bytes for the header that gets
 * passed optionally to xmem_heap3_init.
 *
 * num_blocks      : Number of blocks that will be allocated in the heap
 */
#define XMEM_HEAP3_HEADER_SIZE(num_blocks) \
  (XMEM_HEAP3_BLOCK_STRUCT_SIZE * (num_blocks) + (((num_blocks) + 31) >> 5) * 4)

#define XMEM_HEAP3_CACHE_ALIGNED_HEADER_SIZE(num_blocks) \
  (((XMEM_HEAP3_HEADER_SIZE((num_blocks)) + XMEM_MAX_DCACHE_LINESIZE - 1) / XMEM_MAX_DCACHE_LINESIZE) * XMEM_MAX_DCACHE_LINESIZE)

/* Initialize the type 3 heap.
 *
 * m          : generic memory manager
 * buf        : user provided buffer for allocation
 * size       : size of the buffer
 * num_blocks : number of blocks available for allocation/free
 * header     : Optional buffer used to store the block information.
 *
 * Returns XMEM_OK if successful, else returns one of XMEM_ERROR_POOL_NULL,
 * XMEM_ERROR_POOL_SIZE or XMEM_ERROR_INTERNAL.
 */
xmem_status_t
xmem_heap3_init(xmem_mgr_t *m,
                void *buf,
                size_t size,
                uint32_t num_blocks,
                void *header);

/* Allocate a buffer of particular size and alignment using the
 * memory manager mgr
 *
 * mgr      : memory manager object
 * size     : buffer size to allocate
 * align    : requested alignment of the buffer
 * err_code : error code if any is returned
 *
 * Returns a pointer to the allocated buffer. If the requested size
 * cannot be allocated at the alignment return NULL and set err_code to
 * XMEM_ERROR_ALLOC_FAILED. If the alignment is not a power-of-2,
 * return NULL and set err_code to XMEM_ERROR_ILLEGAL_ALIGN.
 * Set err_code to XMEM_ERROR_UNKNOWN_HEAP_TYPE if the mgr is not HEAP1 or
 * HEAP2.
 */
extern void *
xmem_alloc(xmem_mgr_t *mgr,
           size_t size,
           uint32_t align,
           xmem_status_t *err_code);

/* Free a pointer allocated using the memory manager mgr
 *
 * mgr      : memory manager object
 * p        : pointer to free
 *
 * Returns void.
 */
extern void
xmem_free(xmem_mgr_t *mgr, void *p);

/* Print heap usage statistics. Only prints in debug builds.
 *
 * mgr : memory manager object
 *
 * Returns void
 */
extern void
xmem_print_stats(xmem_mgr_t *mgr);

#ifdef __cplusplus
}
#endif

#endif /* __XMEM_H__ */
