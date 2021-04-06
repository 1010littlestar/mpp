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



#include <stddef.h>
#include <xtensa/hal.h>
#include "xmem_internal.h"

/* Heap usage statistics of the type 1 heap.
 *
 * mgr : type 1 heap manager
 *
 * Returns void
 */
void
xmem_heap1_print_stats(xmem_heap1_mgr_t *mgr)
{
  XMEM_LOG("Heap buffer @ %p, size: %d, free bytes: %d, allocated bytes: %d, unused bytes: %d, percent overhead: %d\n",
           mgr->_buffer,
           mgr->_buffer_size,
           mgr->_buffer_end - mgr->_buffer_p + 1,
           mgr->_buffer_p - (uintptr_t) mgr->_buffer,
           mgr->_unused_bytes,
           (mgr->_unused_bytes * 100) / mgr->_buffer_size);
}

/* Initialize type 1 memory manager.
 *
 * mgr    : generic memory manager
 * buffer : user provided buffer for allocation
 * size   : size of the buffer
 *
 * Returns XMEM_OK if successful, else returns XMEM_ERROR_INTERNAL, or
 * XMEM_ERROR_POOL_NULL.
 */
xmem_status_t
xmem_heap1_init(xmem_mgr_t *m, void *buffer, size_t size)
{
  if (sizeof(xmem_mgr_t) != XMEM_MGR_SIZE)
  {
    XMEM_LOG("xmem_heap1_init: Sizeof xmem_mgr_t expected to be %d, but got %d\n", XMEM_MGR_SIZE, sizeof(xmem_mgr_t));
    return(XMEM_ERROR_INTERNAL);
  }

  if (buffer == NULL)
  {
    XMEM_LOG("xmem_heap1_init: pool is null\n");
    return(XMEM_ERROR_POOL_NULL);
  }

  m->_lock       = 0;
  m->_alloc_type = XMEM_HEAP1;
  xmem_heap1_mgr_t *mgr = &m->_m._heap1;
  mgr->_buffer       = buffer;
  mgr->_buffer_size  = size;
  mgr->_buffer_p     = (uintptr_t) buffer;
  mgr->_buffer_end   = (uintptr_t) buffer + size - 1;
  mgr->_unused_bytes = 0;

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_writeback((void *) mgr, sizeof(xmem_mgr_t));
#endif

  m->_initialized = XMEM_INITIALIZED;
  return(XMEM_OK);
}

/* Allocate a buffer of requested size and alignment from the type 1 memory
 * manager
 *
 * mgr      : heap1 type memory manager
 * size     : size of the buffer
 * align    : alignment of the buffer
 * err_code : error code if any is returned
 *
 * Returns a pointer to the allocated buffer. If the requested size
 * cannot be allocated at the alignment return NULL and set err_code to
 * XMEM_ERROR_ALLOC_FAILED. If the alignment is not a power-of-2,
 * return NULL and set err_code to XMEM_ERROR_ILLEGAL_ALIGN.
 */
void *
xmem_heap1_alloc(xmem_heap1_mgr_t *mgr,
                 size_t size,
                 uint32_t align,
                 xmem_status_t *err_code)
{
  uintptr_t r;

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_invalidate((void *) mgr, sizeof(xmem_mgr_t));
#endif

  /* Expect alignment to be a power-of-2 */
  if (align & (align - 1))
  {
    XMEM_ABORT("xmem_heap1_alloc: alignment should be a power of 2\n");
    *err_code = XMEM_ERROR_ILLEGAL_ALIGN;
    return(NULL);
  }

  /* Round up to align bytes */
  size_t unused_bytes    = mgr->_unused_bytes;
  uintptr_t buffer_p     = mgr->_buffer_p;
  uintptr_t buffer_end   = mgr->_buffer_end;
  uint32_t p             = xmem_find_msbit(align);
  uintptr_t new_buffer_p = ((buffer_p + align - 1) >> p) << p;
  /* Check if the requested size + aligned buffer pointer exceeds the end of
   * buffer space. If yes, the allocation fails, else returns the buffer
   * at the alignment */
  if (size + new_buffer_p <= buffer_end)
  {
    r                  = new_buffer_p;
    unused_bytes      += (new_buffer_p - buffer_p);
    mgr->_buffer_p     = new_buffer_p + size;
    mgr->_unused_bytes = unused_bytes;
#ifdef XMEM_VERIFY
    if (mgr->_buffer_p > mgr->_buffer_end)
    {
      XMEM_ABORT("xmem_heap1_alloc: buffer_p: %p exceeds buffer_end %p\n", mgr->_buffer_p, mgr->_buffer_end);
    }
#endif
  }
  else
  {
    XMEM_LOG("xmem_heap1_alloc: Could not allocate %d bytes\n", size);
    *err_code = XMEM_ERROR_ALLOC_FAILED;
    return(NULL);
  }

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_writeback((void *) mgr, sizeof(xmem_mgr_t));
#endif
  *err_code = XMEM_OK;
  return((void *) r);
}

