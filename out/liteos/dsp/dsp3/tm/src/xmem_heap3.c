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

#ifdef XMEM_DEBUG
static char *
xmem_print_block_suffix(xmem_block_t *block, xmem_heap3_mgr_t *mgr)
{
  if (block == &mgr->_free_list_head)
  {
    return("fh");
  }
  else if (block == &mgr->_alloc_list_head)
  {
    return("ah");
  }
  else if (block == &mgr->_free_list_tail)
  {
    return("ft");
  }
  else
  {
    return("");
  }
}

static void
xmem_print_block(char *m, xmem_block_t *block, xmem_heap3_mgr_t *mgr)
{
  char *ct = xmem_print_block_suffix(block, mgr);
  if (block->_next_block)
  {
    char *nt = xmem_print_block_suffix(block->_next_block, mgr);
    xmem_log("%s : block@%p (%s) %p:%p, next_block (%s) %p:%p, size: %d\n",
             m, block, ct, block->_buffer, block->_aligned_buffer,
             nt, block->_next_block->_buffer,
             block->_next_block->_aligned_buffer,
             block->_block_size);
  }
  else
  {
    xmem_log("%s : block@%p (%s) %p:%p, size: %d\n",
             m, block, ct, block->_buffer, block->_aligned_buffer,
             block->_block_size);
  }
}

static void
xmem_heap3_print_free_list(xmem_heap3_mgr_t *mgr)
{
  xmem_block_t *block;
  XMEM_LOG("Dumping free list\n");
  for (block = &mgr->_free_list_head; block != NULL; block = block->_next_block)
  {
    xmem_print_block(" free_list_block", block, mgr);
  }
}

static void
xmem_heap3_print_alloc_list(xmem_heap3_mgr_t *mgr)
{
  xmem_block_t *block;
  XMEM_LOG("Dumping alloc list\n");
  for (block = &mgr->_alloc_list_head; block != NULL;
       block = block->_next_block)
  {
    xmem_print_block(" alloc_list_block", block, mgr);
  }
}

#endif

#ifdef XMEM_VERIFY
void xmem_heap3_verify(xmem_heap3_mgr_t *mgr)
{
  xmem_block_t *block;
  int free_size             = 0, alloc_size = 0;
  uintptr_t prev_block_addr = 0;
  int num_blocks            = 0;

  for (block = mgr->_free_list_head._next_block;
       block != &mgr->_free_list_tail;
       block = block->_next_block)
  {
    if ((uintptr_t) block->_buffer < (uintptr_t) mgr->_buffer ||
        ((uintptr_t) block->_buffer >=
         ((uintptr_t) mgr->_buffer + (uintptr_t) mgr->_buffer_size)))
    {
      XMEM_ABORT("Block %p in free list not contained within buffer\n", block, mgr->_buffer);
    }
    if ((uintptr_t) block->_aligned_buffer < (uintptr_t) mgr->_buffer ||
        ((uintptr_t) block->_aligned_buffer >=
         ((uintptr_t) mgr->_buffer + (uintptr_t) mgr->_buffer_size)))
    {
      XMEM_ABORT("Block %p in free list not contained within buffer\n", block, mgr->_buffer);
    }
    if ((uintptr_t) block->_buffer < prev_block_addr)
    {
#ifdef XMEM_DEBUG
      xmem_heap3_print_free_list(mgr);
#endif
      XMEM_ABORT("Free list not in sorted order of block size\n");
    }
    if (block->_next_block != &mgr->_free_list_tail)
    {
      if ((uintptr_t) block->_buffer + block->_block_size >
          (uintptr_t) block->_next_block->_buffer)
      {
#ifdef XMEM_DEBUG
        xmem_print_block("block: ", block, mgr);
#endif
        XMEM_ABORT("Free list blocks overlap\n");
      }
    }
    uint32_t idx = ((uintptr_t) block - (uintptr_t) &mgr->_blocks[0]) >>
                   XMEM_HEAP3_BLOCK_STRUCT_SIZE_LOG2;
    uint32_t lzc =
      xmem_find_leading_zero_one_count(mgr->_block_free_bitvec,
                                       mgr->_num_blocks, idx,
                                       mgr->_block_free_bitvec[0], 0);
    if (lzc < 1)
    {
      XMEM_ABORT("xmem_heap2_verify: Expecting bit at idx %d to be 0\n", idx);
    }

    prev_block_addr = (uintptr_t) block->_buffer;
    free_size      += block->_block_size;
    num_blocks++;
  }
  if (free_size != mgr->_free_bytes)
  {
    XMEM_ABORT("xmem_heap2_verify: Expecting free_size %d to be equal to free_bytes %d\n", free_size, mgr->_free_bytes);
  }

  for (block = mgr->_alloc_list_head._next_block; block != 0;
       block = block->_next_block)
  {
    if ((uintptr_t) block->_buffer < (uintptr_t) mgr->_buffer ||
        ((uintptr_t) block->_buffer >=
         ((uintptr_t) mgr->_buffer + (uintptr_t) mgr->_buffer_size)))
    {
      XMEM_ABORT("Block %p in alloc list not contained within buffer\n", block, mgr->_buffer);
    }
    if ((uintptr_t) block->_aligned_buffer < (uintptr_t) mgr->_buffer ||
        ((uintptr_t) block->_aligned_buffer >=
         ((uintptr_t) mgr->_buffer + (uintptr_t) mgr->_buffer_size)))
    {
      XMEM_ABORT("Block %p in alloc list not contained within buffer\n", block, mgr->_buffer);
    }
    uint32_t idx = ((uintptr_t) block - (uintptr_t) &mgr->_blocks[0]) >>
                   XMEM_HEAP3_BLOCK_STRUCT_SIZE_LOG2;
    uint32_t lzc =
      xmem_find_leading_zero_one_count(mgr->_block_free_bitvec,
                                       mgr->_num_blocks, idx,
                                       mgr->_block_free_bitvec[0], 0);
    if (lzc < 1)
    {
      XMEM_ABORT("xmem_heap2_verify: Expecting bit at idx %d to be 0\n", idx);
    }
    alloc_size += block->_block_size;
    num_blocks++;
  }
  if (alloc_size != mgr->_allocated_bytes)
  {
    XMEM_ABORT("xmem_heap2_verify: Expecting alloc_size %d to be equal to allocated_bytes %d\n", alloc_size, mgr->_allocated_bytes);
  }

  if ((mgr->_free_bytes + mgr->_allocated_bytes) != mgr->_buffer_size)
  {
    XMEM_ABORT("xmem_heap2_verify: Mismatch in allocated/free bytes, free_ytes: %d allocated_bytes %d buffer_size %d\n",
               mgr->_free_bytes, mgr->_allocated_bytes, mgr->_buffer_size);
  }

  uint32_t pop_count = xmem_pop_count(mgr->_block_free_bitvec,
                                      mgr->_num_blocks,
                                      0xffffffff);
  if (num_blocks != pop_count)
  {
    XMEM_ABORT("xmem_heap2_verify: Mismatch. num_blocks %d, pop_count %d\n",
               num_blocks, pop_count);
  }
}

#endif

/* Heap usage statistics of the type 3 heap.
 *
 * mgr : type 3 heap manager
 *
 * Returns void
 */
void
xmem_heap3_print_stats(xmem_heap3_mgr_t *mgr)
{
  XMEM_LOG("Heap buffer @ %p, size: %d, free bytes: %d, allocated bytes: %d, unused bytes: %d, percent overhead: %d\n",
           mgr->_buffer, mgr->_buffer_size, mgr->_free_bytes,
           mgr->_allocated_bytes, mgr->_unused_bytes,
           (mgr->_unused_bytes * 100) / mgr->_buffer_size);
}

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
                void *header)
{
  if (sizeof(xmem_mgr_t) != XMEM_MGR_SIZE)
  {
    XMEM_LOG("xmem_heap3_init: Sizeof xmem_mgr_t expected to be %d, but got %d\n", XMEM_MGR_SIZE, sizeof(xmem_mgr_t));
    return(XMEM_ERROR_INTERNAL);
  }

  if (sizeof(xmem_block_t) != XMEM_HEAP3_BLOCK_STRUCT_SIZE)
  {
    XMEM_LOG("xmem_heap3_init: Sizeof xmem_block_t expected to be %d, but got %d\n", XMEM_HEAP3_BLOCK_STRUCT_SIZE, sizeof(xmem_block_t));
    return(XMEM_ERROR_INTERNAL);
  }

  /* xmem_block_t has to be a power of 2 size */
  if (XMEM_HEAP3_BLOCK_STRUCT_SIZE & (XMEM_HEAP3_BLOCK_STRUCT_SIZE - 1))
  {
    XMEM_LOG("xmem_heap3_init: XMEM_HEAP3_BLOCK_STRUCT_SIZE %d has to be a power of 2\n", XMEM_HEAP3_BLOCK_STRUCT_SIZE);
    return(XMEM_ERROR_INTERNAL);
  }

  m->_lock       = 0;
  m->_alloc_type = XMEM_HEAP3;
  xmem_heap3_mgr_t *mgr = &m->_m._heap3;

  if (buf == 0)
  {
    XMEM_LOG("xmem_heap3_init: pool is null\n");
    return(XMEM_ERROR_POOL_NULL);
  }

  mgr->_buffer      = buf;
  mgr->_buffer_size = size;

  uint32_t num_words_in_block_bitvec;
  /* If there is an externally provided header, the blocks and the bitvector
   * are allocated from the header, else they are from the user provided pool */
  if (header)
  {
    mgr->_num_blocks          = num_blocks;
    num_words_in_block_bitvec = (mgr->_num_blocks + 31) >> 5;
    mgr->_blocks              = (xmem_block_t *) header;
    mgr->_has_header          = 1;
  }
  else
  {
    mgr->_num_blocks          = XMEM_HEAP3_DEFAULT_NUM_BLOCKS;
    num_words_in_block_bitvec = (mgr->_num_blocks + 31) >> 5;
    mgr->_blocks              = (xmem_block_t *) buf;
    mgr->_has_header          = 0;
  }
  mgr->_block_free_bitvec =
    (uint32_t *) ((uintptr_t) mgr->_blocks +
                  sizeof(xmem_block_t) * mgr->_num_blocks);
  mgr->_header_size = sizeof(xmem_block_t) * mgr->_num_blocks +
                      num_words_in_block_bitvec * 4;

  /* Check if there is the minimum required buffer size and number of blocks */
  if (!header)
  {
    if (size < (mgr->_header_size + XMEM_HEAP3_MIN_ALLOC_SIZE))
    {
      XMEM_LOG("xmem_heap3_init: Pool has %d bytes. Need at atleast %d bytes for pool\n", size, mgr->_header_size + XMEM_HEAP3_MIN_ALLOC_SIZE);
      return(XMEM_ERROR_POOL_SIZE);
    }
  }
  else
  {
    if (size < XMEM_HEAP3_MIN_ALLOC_SIZE)
    {
      XMEM_LOG("xmem_heap3_init: Pool has %d bytes. Need at atleast %d bytes for pool\n", size, XMEM_HEAP3_MIN_ALLOC_SIZE);
      return(XMEM_ERROR_POOL_SIZE);
    }
    if (mgr->_num_blocks < 1)
    {
      XMEM_LOG("xmem_heap3_init: num_blocks is %d, but need at least 1 block\n", num_blocks);
      return(XMEM_ERROR_POOL_SIZE);
    }
  }

  /* Initialize free and allocated list */
  mgr->_free_list_head._block_size     = 0;
  mgr->_free_list_head._buffer         = 0;
  mgr->_free_list_head._aligned_buffer = 0;
  mgr->_free_list_tail._block_size     = size;
  mgr->_free_list_tail._buffer         = (void *) 0xffffffff;
  mgr->_free_list_tail._aligned_buffer = (void *) 0xffffffff;

  mgr->_alloc_list_head._block_size     = 0;
  mgr->_alloc_list_head._buffer         = 0;
  mgr->_alloc_list_head._aligned_buffer = 0;

  mgr->_allocated_bytes = 0;

  if (header)
  {
    /* Initialize the first block with the start address of the buffer and
     * size as the whole buffer and add to the free list */
    mgr->_blocks[0]._block_size     = size;
    mgr->_blocks[0]._buffer         = buf;
    mgr->_blocks[0]._aligned_buffer = buf;

    mgr->_free_list_head._next_block = &mgr->_blocks[0];
    mgr->_free_list_tail._next_block = 0;
    mgr->_blocks[0]._next_block      = &mgr->_free_list_tail;

    mgr->_alloc_list_head._next_block = 0;

    /* Set the first block as in use (set to 0) and rest as
     * available (set to 1) */
    mgr->_block_free_bitvec[0] = 0xfffffffe;

    mgr->_free_bytes   = size;
    mgr->_unused_bytes = 0;
  }
  else
  {
    /* The header (array of xmem_block_t and the bitvector) is placed at the
     * start of buf. Data allocation begins after that. Initialize the first
     * block with the header address (start of buf) and header size. */
    mgr->_blocks[0]._block_size     = mgr->_header_size;
    mgr->_blocks[0]._buffer         = buf;
    mgr->_blocks[0]._aligned_buffer = buf;

    mgr->_alloc_list_head._next_block = &mgr->_blocks[0];
    mgr->_blocks[0]._next_block       = 0;

    /* Initialize the second block with the address of the area in buffer
     * following the xmem_block_t array and bitvector that is available for
     * user allocation. Size is set as the buffer size minus the space required
     * for the block array and bitvector */
    mgr->_blocks[1]._block_size = size - mgr->_blocks[0]._block_size;
    mgr->_blocks[1]._buffer     = (void *) ((uintptr_t) mgr->_blocks[0]._buffer +
                                            mgr->_blocks[0]._block_size);
    mgr->_blocks[1]._aligned_buffer = mgr->_blocks[1]._buffer;

    mgr->_free_list_head._next_block = &mgr->_blocks[1];
    mgr->_free_list_tail._next_block = 0;
    mgr->_blocks[1]._next_block      = &mgr->_free_list_tail;

    /* Set the first two blocks as in use (set to 0) and rest as
     * available (set to 1) */
    mgr->_block_free_bitvec[0] = 0xfffffffc;

    mgr->_free_bytes   = mgr->_blocks[1]._block_size;
    mgr->_unused_bytes = mgr->_blocks[0]._block_size;
  }

#ifdef XMEM_DEBUG
  xmem_heap3_print_free_list(mgr);
  xmem_heap3_print_alloc_list(mgr);
  xmem_print_bitvec("bitvec: ", mgr->_block_free_bitvec, mgr->_num_blocks);
#endif

  XMEM_LOG("xmem_heap3_init: Original buffer @ %p, blocks @ %p block_free_bivec @ %p allocatable buffer @ %p allocatable buffer size %d allocated_bytes: %d free_bytes: %d unused_bytes: %d\n", buf, mgr->_blocks, mgr->_block_free_bitvec, mgr->_free_list_head._next_block->_buffer, mgr->_free_list_head._block_size, mgr->_allocated_bytes, mgr->_free_bytes, mgr->_unused_bytes);
#ifdef XMEM_DEBUG
  xmem_print_bitvec("xmem_heap3_init: block_free_bivec: ", mgr->_block_free_bitvec, mgr->_num_blocks);
#endif

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_writeback((void *) mgr, sizeof(xmem_mgr_t));
  if (mgr->_has_header)
  {
    xthal_dcache_region_writeback(mgr->_blocks, mgr->_header_size);
  }
#endif

  m->_initialized = XMEM_INITIALIZED;

  return(XMEM_OK);
}

/* Compute the size required based on the alignment. If the block address is
 * not aligned, extra space is alloted to align the address to the requested
 * alignment.
 *
 * block : block from which allocation is done
 * size  : size to be allocated
 * align : requested alignment
 *
 * Returns the size after alignment
 */
static XMEM_INLINE uint32_t
xmem_compute_new_size(xmem_block_t *block, size_t size, uint32_t align)
{
  /* Align the block buffer address to the specified alignment */
  uint32_t p              = xmem_find_msbit(align);
  uintptr_t aligned_block = (uintptr_t) block->_buffer;
  aligned_block = ((aligned_block + align - 1) >> p) << p;
  /* compute extra space required for alignment */
  uint32_t extra_size = aligned_block - (uintptr_t) block->_buffer;
  uint32_t new_size   = size + extra_size;
  XMEM_LOG("xmem_compute_new_size: block buffer: %p aligned_block buffer: %p extra_size: %u new_size %u\n", block->_buffer, (void *) aligned_block,
           (int32_t) extra_size, (int32_t) new_size);
  return(new_size);
}

/* Allocate buffer from the type 3 heap of specified size and alignment.
 * Alignment has to be a power-of-2.
 *
 * mgr      : heap3 type memory manager
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
xmem_heap3_alloc(xmem_heap3_mgr_t *mgr,
                 size_t size,
                 uint32_t align,
                 xmem_status_t *err_code)
{
    void *r;

    XMEM_LOG("xmem_heap3_alloc: Requesting %d bytes at align %d\n", size, align);

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
    xthal_dcache_region_invalidate((void *) mgr, sizeof(xmem_mgr_t));
    if (mgr->_has_header)
    {
        xthal_dcache_region_invalidate(mgr->_blocks, mgr->_header_size);
    }
#endif

    /* Expect alignment to be a power-of-2 */
    if (align & (align - 1))
    {
        XMEM_LOG("xmem_heap3_alloc: alignment should be a power of 2\n");
        *err_code = XMEM_ERROR_ILLEGAL_ALIGN;
        return(0);
    }

    xmem_block_t *prev_block = &mgr->_free_list_head;
    xmem_block_t *curr_block = mgr->_free_list_head._next_block;

#ifdef XMEM_DEBUG
    xmem_print_block("xmem_heap3_alloc: prev_block", prev_block, mgr);
    xmem_print_block("xmem_heap3_alloc: curr_block", curr_block, mgr);
#endif

    /* Compute required size based on alignment */
    uint32_t new_size = xmem_compute_new_size(curr_block, size, align);

    /* Check for the first available block in the free list that satisfies
    * the required size */
    while (curr_block->_block_size < new_size)
    {
        prev_block = curr_block;
        curr_block = curr_block->_next_block;
#ifdef XMEM_DEBUG
        xmem_print_block("xmem_heap3_alloc: prev_block", prev_block, mgr);
        xmem_print_block("xmem_heap3_alloc: curr_block", curr_block, mgr);
#endif
        new_size = xmem_compute_new_size(curr_block, size, align);

    }

    if (curr_block != &mgr->_free_list_tail)
    {
        curr_block->_aligned_buffer = (void *) ((uintptr_t) curr_block->_buffer +
                                            new_size - size);
        /* The aligned buffer address gets returned to the user */
        r                   = curr_block->_aligned_buffer;
        mgr->_unused_bytes += (new_size - size);

        /* If the remaining space in the block is < than the minimum alloc size,
        * just allocate the whole block to this request, else split the block
        * and add the remaining to the free list */
        if ((curr_block->_block_size - new_size) > XMEM_HEAP3_MIN_ALLOC_SIZE)
        {
            /* Find a free block to hold the remaining of the current block */
            uint32_t avail_buf_idx =
            xmem_find_leading_zero_one_count(mgr->_block_free_bitvec,
                                             mgr->_num_blocks,
                                             0,
                                             mgr->_block_free_bitvec[0],
                                             0);
            XMEM_LOG("xmem_heap3_alloc: avail buf idx (leading zero count) is %d\n",
                   avail_buf_idx);
            /* Run out of block headers.
            * TODO: Do reallocate and expand the block header */
            if (avail_buf_idx >= mgr->_num_blocks)
            {
                XMEM_LOG("No available blocks. Failed to allocate %d bytes with alignment %d. Heap has %d free bytes and %d allocated bytes %d\n",
                         (int32_t) size, (int32_t) align, (int32_t) mgr->_free_bytes,
                         (int32_t) mgr->_allocated_bytes);
                *err_code = XMEM_ERROR_ALLOC_FAILED;
                return(0);
            }
            /* Mark the new created block as allocated */
            xmem_toggle_bitvec(mgr->_block_free_bitvec, mgr->_num_blocks,
                             avail_buf_idx, 1);
            xmem_block_t *new_block = &mgr->_blocks[avail_buf_idx];
            new_block->_block_size     = curr_block->_block_size - new_size;
            new_block->_buffer         = (void *) ((uintptr_t) curr_block->_buffer + new_size);
            new_block->_aligned_buffer = new_block->_buffer;
            curr_block->_block_size    = new_size;
            /* Add to free list */
            new_block->_next_block  = curr_block->_next_block;
            prev_block->_next_block = new_block;
#ifdef XMEM_DEBUG
            xmem_print_block("add_block_to_free_list", new_block, mgr);
#endif
        }
        else
        {
            new_size                = curr_block->_block_size;
            prev_block->_next_block = curr_block->_next_block;
        }
        /* Add the allocated block to beginning of the allocated list */
        curr_block->_next_block           = mgr->_alloc_list_head._next_block;
        mgr->_alloc_list_head._next_block = curr_block;
        mgr->_free_bytes                 -= curr_block->_block_size;
        mgr->_allocated_bytes            += curr_block->_block_size;
        XMEM_LOG("xmem_heap3_alloc: Allocated at %p %d bytes, (requested %d bytes) at alignment %d. Heap has free bytes %d, allocated bytes %d\n", r, new_size,
             size, align, mgr->_free_bytes, mgr->_allocated_bytes);
        *err_code = XMEM_OK;
    }
    else
    {
        XMEM_LOG("Failed to allocate %d bytes with alignment %d. Heap has %d free bytes and %d allocated bytes\n",
             (int32_t) size, (int32_t) align, (int32_t) mgr->_free_bytes,
             (int32_t) mgr->_allocated_bytes);
        *err_code = XMEM_ERROR_ALLOC_FAILED;
        r         = 0;
    }

#ifdef XMEM_DEBUG
    xmem_heap3_print_free_list(mgr);
    xmem_heap3_print_alloc_list(mgr);
    xmem_print_bitvec("bitvec: ", mgr->_block_free_bitvec, mgr->_num_blocks);
#endif

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
    xthal_dcache_region_writeback((void *) mgr, sizeof(xmem_mgr_t));
    if (mgr->_has_header)
    {
        xthal_dcache_region_writeback(mgr->_blocks, mgr->_header_size);
    }
#endif

#ifdef XMEM_VERIFY
    xmem_heap3_verify(mgr);
#endif

#ifdef XMEM_VERIFY
    if ((uintptr_t) r != 0 && ((uintptr_t) r < (uintptr_t) mgr->_buffer ||
                         ((uintptr_t) r >= ((uintptr_t) mgr->_buffer +
                                            (uintptr_t) mgr->_buffer_size))))
    {
        XMEM_ABORT("Allocated ptr %p not contained within buffer\n", r);
    }
#endif

    return(r);
}

/* Add a block to the free list that is sorted based on address. Coalesce
 * with the previous or next block if they happen to be contiguous.
 *
 * new_block : block to be added to free list
 * mgr       : heap3 type memory manager
 */
static XMEM_INLINE void
xmem_heap3_add_block_to_free_list(xmem_block_t *new_block,
                                  xmem_heap3_mgr_t *mgr)
{
  xmem_block_t *block;
  /* Find block whose next free block's buffer address is >= buffer address of
   * the new block to be added */
  for (block = &mgr->_free_list_head;
       (uintptr_t) block->_next_block->_buffer < (uintptr_t) new_block->_buffer;
       block = block->_next_block)
  {
    ;
  }
  int merge_with_prev = 0;
  int merge_with_next = 0;
  /* Check if the previous block can be merged with the new block */
  if (block != &mgr->_free_list_head)
  {
    if ((uintptr_t) block->_buffer + block->_block_size ==
        (uintptr_t) new_block->_buffer)
    {
      XMEM_LOG("Merging prev block with new_block\n");
#ifdef XMEM_DEBUG
      xmem_print_block("prev block", block, mgr);
      xmem_print_block("new block", new_block, mgr);
#endif
      block->_block_size += new_block->_block_size;
      uint32_t new_block_bv_idx =
        ((uintptr_t) new_block - (uintptr_t) &mgr->_blocks[0]) >>
        XMEM_HEAP3_BLOCK_STRUCT_SIZE_LOG2;
      xmem_toggle_bitvec(mgr->_block_free_bitvec, mgr->_num_blocks,
                         new_block_bv_idx, 1);
      new_block       = block;
      merge_with_prev = 1;
    }
  }

  /* Check if new_block (or new_block that is merged with previous from above)
   * can be merged with the next */
  if (block->_next_block != &mgr->_free_list_tail)
  {
    if ((uintptr_t) new_block->_buffer + new_block->_block_size ==
        (uintptr_t) block->_next_block->_buffer)
    {
      XMEM_LOG("Merging new block with next block\n");
#ifdef XMEM_DEBUG
      xmem_print_block("new block", new_block, mgr);
      xmem_print_block("next block", block->_next_block, mgr);
#endif
      uint32_t next_block_bv_idx =
        ((uintptr_t) block->_next_block - (uintptr_t) &mgr->_blocks[0]) >>
        XMEM_HEAP3_BLOCK_STRUCT_SIZE_LOG2;
      xmem_toggle_bitvec(mgr->_block_free_bitvec, mgr->_num_blocks,
                         next_block_bv_idx, 1);
      new_block->_block_size += block->_next_block->_block_size;
      new_block->_next_block  = block->_next_block->_next_block;
      if (merge_with_prev == 0)
      {
        block->_next_block = new_block;
      }
      merge_with_next = 1;
    }
  }

  if (merge_with_prev == 0 && merge_with_next == 0)
  {
    new_block->_next_block = block->_next_block;
    block->_next_block     = new_block;
  }
}

/* Free buffer allocated from the type 3 heap.
 *
 * mgr : heap3 type memory manager
 * p   : buffer to free
 */
void
xmem_heap3_free(xmem_heap3_mgr_t *mgr, void *p)
{
  if (p == NULL)
  {
    return;
  }

  XMEM_LOG("xmem_heap3_free: Requesting freeing at %p\n", p);

  xmem_block_t *block;
  xmem_block_t *prev_block;

  /* Check the allocated list to find the matching block that holds p */
  for (block = mgr->_alloc_list_head._next_block,
       prev_block = &mgr->_alloc_list_head;
       block != 0;
       prev_block = block, block = block->_next_block)
  {
    if (block->_aligned_buffer == p)
    {
      break;
    }
  }

  if (block == 0)
  {
    return;
  }

  /* Remove from allocated list and add to free list */
  prev_block->_next_block = block->_next_block;
  mgr->_free_bytes       += block->_block_size;
  mgr->_allocated_bytes  -= block->_block_size;
  mgr->_unused_bytes     -= ((uintptr_t) block->_buffer -
                             (uintptr_t) block->_aligned_buffer);
  xmem_heap3_add_block_to_free_list(block, mgr);

  XMEM_LOG("Free bytes %d, allocated bytes %d\n",
           mgr->_free_bytes, mgr->_allocated_bytes);


#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_invalidate((void *) mgr, sizeof(xmem_mgr_t));
  if (mgr->_has_header)
  {
    xthal_dcache_region_invalidate(mgr->_blocks, mgr->_header_size);
  }
#endif

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
  xthal_dcache_region_writeback((void *) mgr, sizeof(xmem_mgr_t));
  if (mgr->_has_header)
  {
    xthal_dcache_region_writeback(mgr->_blocks, mgr->_header_size);
  }
#endif

#ifdef XMEM_DEBUG
  xmem_heap3_print_free_list(mgr);
  xmem_heap3_print_alloc_list(mgr);
  xmem_print_bitvec("bitvec: ", mgr->_block_free_bitvec, mgr->_num_blocks);
#endif

#ifdef XMEM_VERIFY
  xmem_heap3_verify(mgr);
#endif
}

