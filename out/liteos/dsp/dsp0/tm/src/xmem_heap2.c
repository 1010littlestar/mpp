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



#include <string.h>
#include <stddef.h>
#include <xtensa/hal.h>
#include "xmem_internal.h"

/* Heap usage statistics of the type 2 heap.
 *
 * mgr : type 2 heap manager
 *
 * Returns void
 */
void
xmem_heap2_print_stats(xmem_heap2_mgr_t *mgr)
{
  XMEM_LOG("Heap buffer @ %p, size: %d, free bytes: %d, allocated bytes: %d, unused bytes: %d, percent overhead: %d\n",
           mgr->_buffer, mgr->_buffer_size, mgr->_free_bytes,
           mgr->_allocated_bytes, mgr->_unused_bytes,
           (mgr->_unused_bytes * 100) / mgr->_buffer_size);
}

/* Initialize the type 2 heap.
 *
 * m          : generic memory manager
 * buf        : user provided buffer for allocation
 * size       : size of the buffer
 * block_size : size of the block
 * header     : Optional buffer used to store the bitvectors. If NULL, the
 *              headers are allocated out of the user provided buf.
 *
 * Returns XMEM_OK if successful, else returns one of XMEM_ERROR_POOL_NULL,
 * XMEM_ERROR_INTERNAL, XMEM_ERROR_BLOCK_SIZE, XMEM_ERROR_POOL_SIZE,
 * XMEM_ERROR_POOL_NOT_ALIGNED.
 */
xmem_status_t
xmem_heap2_init(xmem_mgr_t *m,
                void *buf,
                size_t size,
                uint32_t block_size,
                void *header)
{
  if (sizeof(xmem_mgr_t) != XMEM_MGR_SIZE)
  {
    XMEM_LOG("xmem_heap2_init: Sizeof xmem_mgr_t expected to be %d, but got %d\n", XMEM_MGR_SIZE, sizeof(xmem_mgr_t));
    return(XMEM_ERROR_INTERNAL);
  }

  m->_lock       = 0;
  m->_alloc_type = XMEM_HEAP2;
  xmem_heap2_mgr_t *mgr = &m->_m._heap2;

  if (buf == NULL)
  {
    XMEM_LOG("xmem_heap2_init: pool is null\n");
    return(XMEM_ERROR_POOL_NULL);
  }

  if ((block_size & (block_size - 1)) != 0)
  {
    XMEM_LOG("xmem_heap2_init: block_size %d has to be a power of 2\n");
    return(XMEM_ERROR_BLOCK_SIZE);
  }

  /* Size has to be a multiple of the default block size */
  if ((size & (block_size - 1)) != 0)
  {
    XMEM_LOG("xmem_heap2_init: Expecting size of buffer to be a multiple of %d\n", block_size);
    return(XMEM_ERROR_POOL_SIZE);
  }

  /* The buffer has to be aligned to the default block size */
  if (((uintptr_t) buf & (block_size - 1)) != 0)
  {
    XMEM_LOG("xmem_heap2_init: Expecting buffer to be aligned to %d\n",
             block_size);
    return(XMEM_ERROR_POOL_NOT_ALIGNED);
  }

  mgr->_buffer          = buf;
  mgr->_buffer_size     = size;
  mgr->_block_size      = block_size;
  mgr->_block_size_log2 = xmem_find_msbit(block_size);
  /* Maximum number of blocks */
  uint32_t num_blocks = size >> mgr->_block_size_log2;
  /* Maximum number of 32-bit words required by the bitvector to hold the
   * above num_blocks. Note, below we are rounding the num_blocks so that
   * we have an integer number of words */
  uint32_t num_words_in_block_bitvec = (num_blocks + 31) >> 5;
  mgr->_block_bitvec_size = num_words_in_block_bitvec * 2 * 4;
  void *actual_buffer;
  if (!header)
  {
    /* Both block_free_bitvec and num_allocated_bitvectors can support more
     * blocks (num_blocks, defined above) than the actual number of
     * blocks available for allocation. The block_free_bitvec is stored at
     * the beginning of the user provided buffer. The num_allocated_bitvec
     * follows that. So the actual size available for allocation is
     * size - 2*num_words_in_block_bitvec */
    mgr->_block_free_bitvec    = (uint32_t *) buf;
    mgr->_num_allocated_bitvec = (uint32_t *) ((uintptr_t) buf +
                                               num_words_in_block_bitvec * 4);
    uintptr_t b = (uintptr_t) mgr->_num_allocated_bitvec +
                  num_words_in_block_bitvec * 4;
    uint32_t p = xmem_find_msbit(mgr->_block_size);
    /* Start of actual buffer used for allocation rounded to block_size bytes*/
    actual_buffer       = (void *) (((b + mgr->_block_size - 1) >> p) << p);
    mgr->_actual_buffer = actual_buffer;
    mgr->_has_header    = 0;
  }
  else
  {
    /* The block_free_bitvec and num_allocated_bitvec come out of the header
     * buffer. The actual buffer for allocation is the user provided buffer */
    mgr->_block_free_bitvec    = (uint32_t *) header;
    mgr->_num_allocated_bitvec = (uint32_t *) ((uintptr_t) header +
                                               num_words_in_block_bitvec * 4);
    actual_buffer       = buf;
    mgr->_actual_buffer = buf;
    mgr->_has_header    = 1;
  }
  /* Size of actual buffer available for allocation */
  uint32_t actual_buffer_size = size - (actual_buffer - buf);
  mgr->_actual_buffer_size = actual_buffer_size;
  /* Number of blocks available for actual allocation */
  mgr->_num_blocks      = actual_buffer_size >> mgr->_block_size_log2;
  mgr->_allocated_bytes = 0;
  mgr->_free_bytes      = actual_buffer_size;
  mgr->_unused_bytes    = actual_buffer - buf;
  /* Initially, mark all blocks as available */
  memset(mgr->_block_free_bitvec, 0xff, num_words_in_block_bitvec * 4);
  /* Initially, clear the allocated bit */
  memset(mgr->_num_allocated_bitvec, 0, num_words_in_block_bitvec * 4);

  /* Debug dump */
  XMEM_LOG("xmem_heap2_init: Original buffer @ %p, block_free_bitvec @ %p, num_allocated_bitvec @ %p, allocatable buffer @ %p, allocatable buffer size %d, block_size %d (log2: %d) num_blocks available for allocation: %d allocated_bytes: %d free_bytes: %d unused_bytes: %d bitvec size %d\n", buf, mgr->_block_free_bitvec, mgr->_num_allocated_bitvec, mgr->_actual_buffer, mgr->_actual_buffer_size, mgr->_block_size, mgr->_block_size_log2, mgr->_num_blocks, mgr->_allocated_bytes, mgr->_free_bytes, mgr->_unused_bytes, mgr->_block_bitvec_size);
#ifdef XMEM_DEBUG
  xmem_print_bitvec("xmem_heap2_init: block_free_bitvec", mgr->_block_free_bitvec, mgr->_num_blocks);
  xmem_print_bitvec("xmem_heap2_init: num_allocated_bitvec", mgr->_num_allocated_bitvec, mgr->_num_blocks);
#endif

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_writeback((void *) mgr, sizeof(xmem_mgr_t));
  if (header)
  {
    xthal_dcache_region_writeback(header, mgr->_block_bitvec_size);
  }
#endif

  m->_initialized = XMEM_INITIALIZED;
  return(XMEM_OK);
}

/* Allocate buffer from the type 2 heap of specified size and alignment.
 * Alignment has to be a power-of-2. The minimum alignment returned is
 * block_size. The number of bytes actually allocated is
 * the nearest interger multiple of block_size >= size.
 *
 * mgr      : heap2 type memory manager
 * size     : size of buffer to allocate
 * align    : alignment of the requested buffer
 * err_code : error code if any is returned.
 *
 * Returns a pointer to the allocated buffer. If the requested size
 * cannot be allocated at the alignment return NULL and set err_code to
 * XMEM_ERROR_ALLOC_FAILED. If the alignment is not a power-of-2,
 * return NULL and set err_code to XMEM_ERROR_ILLEGAL_ALIGN.
 */
void *
xmem_heap2_alloc(xmem_heap2_mgr_t *mgr,
                 size_t size,
                 uint32_t align,
                 xmem_status_t *err_code)
{
  void *r;

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_invalidate((void *) mgr, sizeof(xmem_mgr_t));
  if (mgr->_has_header)
  {
    xthal_dcache_region_invalidate(mgr->_block_free_bitvec,
                                   mgr->_block_bitvec_size);
  }
#endif

  /* Expect alignment to be a power-of-2 */
  if (align & (align - 1))
  {
    XMEM_LOG("xmem_heap2_alloc: alignment should be a power of 2\n");
    *err_code = XMEM_ERROR_ILLEGAL_ALIGN;
    return(NULL);
  }

  /* Bump alignment to block_size if lesser */
  uint32_t new_align = align < mgr->_block_size ? mgr->_block_size : align;

  /* Round size to multiple of block_size */
  uint32_t p        = xmem_find_msbit(mgr->_block_size);
  uint32_t new_size = ((size + mgr->_block_size - 1) >> p) << p;

  /* Compute the size in units of block_size */
  uint32_t size_in_blocks = new_size >> mgr->_block_size_log2;

  /* Round buffer to align */
  p = xmem_find_msbit(new_align);
  uintptr_t buffer =
    (((uintptr_t) mgr->_actual_buffer + new_align - 1) >> p) << p;
  if (buffer >= ((uintptr_t) mgr->_actual_buffer + mgr->_actual_buffer_size))
  {
    XMEM_LOG("xmem_heap2_alloc: Failed to allocate %d bytes (requested size is %d bytes) at alignment %d (requested alignment is %d bytes). Heap has %d free bytes and %d allocated bytes\n", new_size, size, new_align, align, mgr->_free_bytes, mgr->_allocated_bytes);
    *err_code = XMEM_ERROR_ALLOC_FAILED;
    return(NULL);
  }
  /* Compute block idx of the aligned buffer */
  uint32_t idx =
    (buffer - (uintptr_t) mgr->_actual_buffer) >> mgr->_block_size_log2;

  XMEM_LOG("xmem_heap2_alloc: Searching for %d free blocks starting from idx %d (address: %p)\n", size_in_blocks, idx, buffer);
  uint8_t free        = 0;
  uint32_t num_blocks = mgr->_num_blocks;
  /* Search for a contiguous set of free blocks >= size_in_blocks */
  while (idx < num_blocks)
  {
    /* Repeat while there are no free blocks available starting at the block
     * that is aligned at the requested alignment boundary */
    /* Load word at idx */
    uint32_t w = mgr->_block_free_bitvec[idx >> 5];
    while (idx < num_blocks)
    {
      void *actual_buffer         = mgr->_actual_buffer;
      uint32_t block_size_log2    = mgr->_block_size_log2;
      uint32_t *block_free_bitvec = mgr->_block_free_bitvec;
      /* Find leading zero count (non-free buffers) starting at idx */
      uint32_t num_zeros = xmem_find_leading_zero_one_count(block_free_bitvec,
                                                            num_blocks,
                                                            idx,
                                                            w,
                                                            0);
#pragma no_reorder
      /* Quit if we have found free buffers available starting at idx */
      if (num_zeros == 0)
      {
        break;
      }
      XMEM_LOG("xmem_heap2_alloc: leading zero count at idx %d is %d\n", idx, num_zeros);
      /* Skip over allocated blocks */
      idx += num_zeros;
      XMEM_LOG("xmem_heap2_alloc: Skipping to idx %d\n", idx);
      /* Find the buffer address after the above skip */
      buffer = (uintptr_t) actual_buffer + (idx << block_size_log2);
      /* Realign the buffer address */
      buffer = ((buffer + new_align - 1) >> p) << p;
      /* Adjust block idx to the start of block that is aligned at the
       * requested alignment boundary */
      idx = (buffer - (uintptr_t) actual_buffer) >> block_size_log2;
      /* Load word at idx */
      w = block_free_bitvec[idx >> 5];
      XMEM_LOG("xmem_heap2_alloc: Realigning idx to %d (address: %p)\n", idx, buffer);
    }
    /* Exceeded buffer bounds, quit */
    if (idx >= num_blocks)
    {
      break;
    }
    /* Find number of free blocks; if it is < the requested number of blocks
     * continue; else allocation starts at idx */
    uint32_t num_ones =
      xmem_find_leading_zero_one_count(mgr->_block_free_bitvec,
                                       num_blocks,
                                       idx,
                                       w,
                                       ~0);
    XMEM_LOG("xmem_heap2_alloc: leading one count at idx %d is %d\n", idx, num_ones);
    /* Found free blocks */
    if (num_ones >= size_in_blocks)
    {
      free = 1;
      break;
    }
    /* Skip past the free blocks since they are less than the size we want */
    idx += num_ones;
  }

  /* Found a set of free blocks that is >= size_in_blocks */
  if (free)
  {
    mgr->_allocated_bytes += new_size;
    mgr->_free_bytes      -= new_size;

#ifdef XMEM_VERIFY
    /* Check if there are size_in_blocks 0s in the num_allocated_bitvec starting
     * idx */
    uint32_t n0s =
      xmem_find_leading_zero_one_count(mgr->_num_allocated_bitvec,
                                       mgr->_num_blocks,
                                       idx,
                                       mgr->_num_allocated_bitvec[idx >> 5],
                                       0);
    if (n0s < size_in_blocks)
    {
      XMEM_ABORT("xmem_heap2_alloc: Expecting %d (or more) free blocks in num_allocated_bitvec idx %d but got %d\n", size_in_blocks, idx, n0s);
    }
#endif

    /* Mark size_in_blocks bits as allocated in block_free_bitvec */
    xmem_toggle_bitvec(mgr->_block_free_bitvec, num_blocks,
                       idx, size_in_blocks);
    /* Store size_in_blocks as a bitvector. '1' denotes the current block
     * counts towards this allocation and there are more blocks as part of this
     * allocation. A '0' means this is the last block of this allocation */
    xmem_toggle_bitvec(mgr->_num_allocated_bitvec, num_blocks, idx,
                       size_in_blocks - 1);
    /* Compute address of the allocated block */
    r = (void *) ((uintptr_t) mgr->_actual_buffer +
                  (idx << mgr->_block_size_log2));

#ifdef XMEM_VERIFY
    if (r >= (mgr->_buffer + mgr->_buffer_size))
    {
      XMEM_ABORT("xmem_heap2_alloc: Allocated ptr %p is not within buffer bounds (_buffer: %p, size: %d)\n", r, mgr->_buffer, mgr->_buffer_size);
    }
#endif

    /* Debug dump */
    XMEM_LOG("xmem_heap2_alloc: Allocated at %p %d bytes (requested size is %d bytes) in %d blocks with alignment %d (requested alignment is %d bytes) at block idx %d. Heap has %d free bytes and %d allocated bytes\n", r, new_size, size, size_in_blocks, new_align, align, idx, mgr->_free_bytes, mgr->_allocated_bytes);
#ifdef XMEM_DEBUG
    xmem_print_bitvec("block_free_bitvec", mgr->_block_free_bitvec,
                      mgr->_num_blocks);
    xmem_print_bitvec("num_allocated_bitvec", mgr->_num_allocated_bitvec,
                      mgr->_num_blocks);
#endif
  }
  else
  {
    XMEM_LOG("xmem_heap2_alloc: Failed to allocate %d bytes (requested size is %d bytes) at alignment %d (requested alignment is %d bytes). Heap has %d free bytes and %d allocated bytes\n", new_size, size, new_align, align, mgr->_free_bytes, mgr->_allocated_bytes);
#ifdef XMEM_DEBUG
    xmem_print_bitvec("block_free_bitvec", mgr->_block_free_bitvec,
                      mgr->_num_blocks);
    xmem_print_bitvec("num_allocated_bitvec", mgr->_num_allocated_bitvec,
                      mgr->_num_blocks);
#endif
    *err_code = XMEM_ERROR_ALLOC_FAILED;
    return(NULL);
  }

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_writeback((void *) mgr, sizeof(xmem_mgr_t));
  if (mgr->_has_header)
  {
    xthal_dcache_region_writeback(mgr->_block_free_bitvec,
                                  mgr->_block_bitvec_size);
  }
#endif
  *err_code = XMEM_OK;
  return(r);
}

/* Free buffer allocated from the type 2 heap.
 *
 * mgr : heap2 type memory manager
 * p   : buffer to free
 */
void
xmem_heap2_free(xmem_heap2_mgr_t *mgr, void *p)
{
  if (p == NULL)
  {
    return;
  }

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_invalidate((void *) mgr, sizeof(xmem_mgr_t));
  if (mgr->_has_header)
  {
    xthal_dcache_region_invalidate(mgr->_block_free_bitvec,
                                   mgr->_block_bitvec_size);
  }
#endif

  /* Find the block index within the allocatable buffer of the address to
   * be freed */
  uint32_t idx = ((uintptr_t) p - (uintptr_t) mgr->_actual_buffer) >>
                 mgr->_block_size_log2;
  /* Find the number of leading 1s starting from idx within the
   * num_allocated_bitvec. The number of blocks allocated at address p
   * is one more than the number of leadings 1s. */
  uint32_t size_in_blocks = 1;
  size_in_blocks +=
    xmem_find_leading_zero_one_count(mgr->_num_allocated_bitvec,
                                     mgr->_num_blocks,
                                     idx,
                                     mgr->_num_allocated_bitvec[idx >> 5],
                                     ~0);
  XMEM_LOG("xmem_heap2_free: Freeing %d blocks at idx %d (%p)\n", size_in_blocks, idx, p);

#ifdef XMEM_VERIFY
  /* Check if there are size_in_blocks 0s in the block_free_bitvec starting
   * idx */
  uint32_t n0s =
    xmem_find_leading_zero_one_count(mgr->_block_free_bitvec,
                                     mgr->_num_blocks,
                                     idx,
                                     mgr->_block_free_bitvec[idx >> 5],
                                     0);
  if (n0s < size_in_blocks)
  {
    XMEM_ABORT("xmem_heap2_free: Expecting %d (or more) allocated blocks in block_free_bitvec at idx %d but got %d\n", size_in_blocks, idx, n0s);
  }
#endif

  /* Mark the blocks as free */
  xmem_toggle_bitvec(mgr->_block_free_bitvec, mgr->_num_blocks,
                     idx, size_in_blocks);
  /* Clear the allocated bit */
  xmem_toggle_bitvec(mgr->_num_allocated_bitvec, mgr->_num_blocks,
                     idx, size_in_blocks - 1);
  mgr->_allocated_bytes -= (size_in_blocks << mgr->_block_size_log2);
  mgr->_free_bytes      += (size_in_blocks << mgr->_block_size_log2);

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_writeback((void *) mgr, sizeof(xmem_mgr_t));
  if (mgr->_has_header)
  {
    xthal_dcache_region_writeback(mgr->_block_free_bitvec,
                                  mgr->_block_bitvec_size);
  }
#endif

  /* Debug dump */
  XMEM_LOG("xmem_heap2_free: Heap has %d free bytes, %d allocated bytes, and %d unused bytes\n", mgr->_free_bytes, mgr->_allocated_bytes, mgr->_unused_bytes);
#ifdef XMEM_DEBUG
  xmem_print_bitvec("xmem_heap2_free: block_free_bitvec", mgr->_block_free_bitvec, mgr->_num_blocks);
  xmem_print_bitvec("xmem_heap2_free: num_allocated_bitvec", mgr->_num_allocated_bitvec, mgr->_num_blocks);
#endif
}

