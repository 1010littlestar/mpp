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


#ifndef __XMEM_INTERNAL_H__
#define __XMEM_INTERNAL_H__

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <xtensa/xtutil.h>
#include <xtensa/tie/xt_core.h>
#include <xtensa/config/core-isa.h>
#if XCHAL_HAVE_NSA
#include <xtensa/tie/xt_misc.h>
#endif
#if XCHAL_HAVE_CCOUNT
#include <xtensa/tie/xt_timer.h>
#endif
#if XCHAL_HAVE_S32C1I || XCHAL_HAVE_EXCLUSIVE
#include "xmp_primitives.h"
#endif
#include "xmem.h"

#ifdef XMEM_DEBUG
#define XMEM_INLINE
#else
#define XMEM_INLINE  __attribute__((always_inline))
#endif

#if XCHAL_HAVE_PRID
#define XMEM_GET_PRID()  XT_RSR_PRID()
#else
#define XMEM_GET_PRID()  (0)
#endif

#if XCHAL_HAVE_CCOUNT
#define XMEM_GET_CLOCK()  XT_RSR_CCOUNT()
#else
#define XMEM_GET_CLOCK()  (100)
#endif

__attribute__((unused)) static void
xmem_log(const char *fmt, ...)
{
  int l = 0;
  char _lbuf[1024];

  l += xt_sprintf(&_lbuf[l], "(%10d) ", (int) XMEM_GET_CLOCK());

  char _tbuf[256];
  int i;
  for (i = 0; i < XMEM_GET_PRID() * 10; i++)
  {
    _tbuf[i] = ' ';
  }
  _tbuf[i] = '\0';
  l       += xt_sprintf(&_lbuf[l], "%s", _tbuf);

  l += xt_sprintf(&_lbuf[l], "XMEM_LOG: PRID:%d: ", XMEM_GET_PRID());

  va_list argp;
  va_start(argp, fmt);
  xt_vsprintf(&_lbuf[l], fmt, argp);
  va_end(argp);
  xt_printf("%s", _lbuf);
}

#ifdef XMEM_DEBUG
#define XMEM_LOG(...)    xmem_log(__VA_ARGS__);
#define XMEM_ABORT(...)  { xmem_log(__VA_ARGS__); exit(-1); };
#else
#define XMEM_LOG(...)
#define XMEM_ABORT(...)  { exit(-1); }
#endif

#ifndef XMEM_DEBUG
#define XMEM_IRAM0  __attribute__ ((section(".iram0.text")))
#else
#define XMEM_IRAM0
#endif
#define XMEM_DRAM0                          __attribute__ ((section(".dram0.data")))

#define XMEM_HEAP2_DEFAULT_BLOCK_SIZE       (16)
#define XMEM_HEAP2_LOG2_DEFAULT_BLOCK_SIZE  (4)

#define XMEM_HEAP3_DEFAULT_NUM_BLOCKS       (32)
#define XMEM_HEAP3_MIN_ALLOC_SIZE           (4)
#define XMEM_HEAP3_BLOCK_STRUCT_SIZE_LOG2   (4)

#define XMEM_INITIALIZED                    (0x1234abcd)

/* Memory manager types */
typedef enum
{
  XMEM_HEAP1,    // Stack-based memory manager; no support for free
  XMEM_HEAP2,    // Block-based allocation with free
  XMEM_HEAP3,    // List-based allocation with free
} xmem_alloc_t;

/* Type 1 heap:
 *
 * This is a simple heap that supports allocation of buffers from a user
 * provided memory pool. Freeing of the allocated buffers is not supported.
 * The expected use case is to perform buffer allocation from the pool
 * and free up the entire pool when all of the allocated items are
 * no longer required. The allocation is performed linearly
 * from the start of the user provided memory pool. A current allocation pointer
 * is maintained that starts at the beginning of the pool. For every request,
 * the current pointer is adjusted to the requested alignment. If the requested
 * size + the current pointer is within the bounds of the memory
 * pool, allocation succeeds and the current pointer is returned. If not, the
 * allocation fails and returns NULL.
 */
typedef struct
{
  void      *_buffer;      // user provided pool
  size_t    _buffer_size;  // number of bytes in the pool
  uintptr_t _buffer_p;     // track allocation
  uintptr_t _buffer_end;   // end of buffer
  size_t    _unused_bytes; // unused bytes in the heap
} xmem_heap1_mgr_t;

/* Type 2 heap:
 *
 * This heap supports both allocation and free of buffers from a user
 * provided memory pool. The pool is divided into fixed size blocks. Allocation
 * is always performed in multiples of the block size with a minimum
 * alignment of the block size. Two bitvectors, block_free and num_allocated,
 * of size equal to the number of blocks are used to manage the blocks in the
 * pool. The block_free bitvector tracks the blocks that are
 * allocated/freed, while the num_allocated bitvector
 * counts the number of blocks allocated per
 * allocation request. To allocate a buffer of N blocks, the block_free
 * bitvector is checked for N contiguous free bits starting from the requested
 * alignment. The allocated buffer address is the address of the block
 * starting at the bitvector index with N free bits. In the num_allocated
 * bitvector, N-1 bits are set at this index to denote that there are
 * N allocated blocks. A '0' at the N-th bit of the num_allocated
 * bitvector marks the end of allocation. To free a buffer, the index of the
 * block is computed based on the address of the buffer to be freed.
 * The size of the allocation in blocks is one more than the number of
 * contiguous 1s in the num_allocated bitvector starting
 * at the index. On a free, the corresponding number of bits are cleared
 * starting from the index. The bitvectors are of size equal to the
 * (number of blocks)/32 32-bit words. The bitvectors are stored at the
 * start of the user provided  pool and this space is unavailable for
 * allocation. The actual memory available for allocation begins after that.
 * Alternately, during initialization time a separate buffer to maintain the
 * bitvectors can be provided. This allows all of the pool to be available for
 * allocation.
 */
typedef struct
{
  void     *_buffer;                    // user provided pool for allocation
  size_t   _buffer_size;                // num bytes in pool
  uint32_t _block_size;                 // size of the block
  uint32_t _block_size_log2;            // log size of the block
  uint32_t _block_bitvec_size;          // number of bytes to store the bitvecs
  void     *_actual_buffer;             // buffer used for allocation
  size_t   _actual_buffer_size;         // num bytes available for allocation
  uint32_t _num_blocks;                 // num blocks available for allocation
  uint32_t *_block_free_bitvec;         // bitvec to mark alloc/free blocks
  uint32_t *_num_allocated_bitvec;      // bitvec to count num allocated blocks
  size_t   _free_bytes;                 // free bytes in the heap
  size_t   _allocated_bytes;            // allocated bytes in the heap
  size_t   _unused_bytes;               // unused bytes in the heap
  uint8_t  _has_header;                 // is the bit vectors part of buffer
} xmem_heap2_mgr_t;

/* Type 3 heap:
 *
 * This heap supports both allocation and free of buffers from a user
 * provided memory pool. The allocated and free blocks are maintained as a
 * list that gets dynamically populated based on the allocation request.
 * A block array that holds book-keeping information for each
 * allocated/free block is maintained outside the pool. At initialization, the
 * number of such blocks needs to be provided (else, a default of
 * XMEM_HEAP3_DEFAULT_NUM_BLOCKS is assumed).  The user can either provide a
 * separate buffer space for maintaining the block array. If not, part of the
 * pool is dedicted to maintaining the block array and this space in not
 * available for allocation.
 */

/* Struct to maintain list of free/allocated blocks for type 3 heap. */
typedef struct xmem_block_struct
{
  /* next block in the list */
  struct xmem_block_struct *_next_block;     // next block in the list
  size_t                   _block_size;      // size of the block
  void                     *_buffer;         // ptr to the buffer
  void                     *_aligned_buffer; // aligned ptr to buffer
} xmem_block_t;

/* Type 3 heap manager */
typedef struct
{
  void         *_buffer;                    // user provided pool for allocation
  size_t       _buffer_size;                // num bytes in pool
  size_t       _free_bytes;                 // free bytes in the heap
  size_t       _allocated_bytes;            // allocated bytes in the heap
  size_t       _unused_bytes;               // unused bytes in the heap
  xmem_block_t _free_list_head;             // sentinel for the free list
  xmem_block_t _free_list_tail;             // sentinel for the free list
  xmem_block_t _alloc_list_head;            // sentinel for the allocated list
  xmem_block_t *_blocks;                    // list of blocks
  uint32_t     _num_blocks;                 // number of blocks
  uint32_t     *_block_free_bitvec;         // bitvec to mark free/alloc blocks
  uint16_t     _header_size;                // size of head in bytes
  uint16_t     _has_header;                 // is the block info part of buffer
} xmem_heap3_mgr_t;

/* Generic memory manager type */
struct xmem_mgr_struct
{
  volatile uint32_t _lock;
  xmem_alloc_t      _alloc_type;
  uint32_t          _initialized;
  union
  {
    xmem_heap1_mgr_t _heap1;
    xmem_heap2_mgr_t _heap2;
    xmem_heap3_mgr_t _heap3;
  } _m;
};

extern void *
xmem_heap1_alloc(xmem_heap1_mgr_t *mgr,
                 size_t size,
                 uint32_t align,
                 xmem_status_t *err_code);

extern void *
xmem_heap2_alloc(xmem_heap2_mgr_t *mgr,
                 size_t size,
                 uint32_t align,
                 xmem_status_t *err_code);

extern void *
xmem_heap3_alloc(xmem_heap3_mgr_t *mgr,
                 size_t size,
                 uint32_t align,
                 xmem_status_t *err_code);

extern void
xmem_heap2_free(xmem_heap2_mgr_t *mgr, void *p);

extern void
xmem_heap3_free(xmem_heap3_mgr_t *mgr, void *p);

extern void
xmem_heap1_print_stats(xmem_heap1_mgr_t *mgr);

extern void
xmem_heap2_print_stats(xmem_heap2_mgr_t *m);

extern void
xmem_heap3_print_stats(xmem_heap3_mgr_t *m);

/* Return index of msb bit set in 'n', assuming n is a power of 2 */
__attribute__((unused)) static XMEM_INLINE int32_t
xmem_find_msbit(uint32_t n)
{
#ifdef XMEM_VERIFY
  if (n & (n - 1))
  {
    XMEM_ABORT("xmem_find_msbit: n (%d) is not a power of 2\n", n);
  }
#endif
#if XCHAL_HAVE_NSA
  return(31 - XT_NSAU(n));
#else
  uint32_t nbits = 0;
  while (n && (n & 1) == 0)
  {
    n >>= 1;
    nbits++;
  }
  return(nbits);
#endif
}

__attribute__((unused)) static XMEM_INLINE void
xmem_print_bitvec(const char *msg, const uint32_t *bitvec, uint32_t num_bits)
{
#ifdef XMEM_DEBUG
  int i;
  char b[256];
  sprintf(b, "%s: (%d-bits) ", msg, num_bits);
  int num_words = (num_bits + 31) >> 5;
  xmem_log("%s", b);
  for (i = num_words - 1; i >= 0; i--)
  {
    xt_printf("0x%x,", bitvec[i]);
  }
  xt_printf("\n");
#endif
}

/* Function that toggles 'num_bits' starting from the bit position 'pos'
 * of the bitvector 'bitvec'. 'bitvec' is an arbitrary length bitvector of
 * 'num_bits_in_bitvec' length that is stored in an array of 32-bit words.
 *
 * bitvec             : arbitrary length bitvector
 * num_bits_in_bitvec : number of bits in the bitvector
 * pos                : position within the bitvector to start toggling bits
 * num_bits           : number of bits to toggle
 *
 * Returns void
 */
__attribute__((unused)) static XMEM_INLINE void
xmem_toggle_bitvec(uint32_t *bitvec,
                   uint32_t num_bits_in_bitvec,
                   uint32_t pos,
                   uint32_t num_bits)
{
#ifdef XMEM_VERIFY
  if (pos >= num_bits_in_bitvec)
  {
    XMEM_ABORT("xmem_toggle_bitvec: pos %d has to be between 0 and %d\n",
               pos, num_bits_in_bitvec - 1);
  }
  if (num_bits > num_bits_in_bitvec)
  {
    XMEM_ABORT("xmem_toggle_bitvec: num_bits %d has to be <= %d\n",
               num_bits, num_bits_in_bitvec);
  }
#endif
  /* Find the word corresponding to bit position pos */
  uint32_t word_idx = pos >> 5;
  /* Find bit offset within this word */
  uint32_t word_off = pos & 31;
  /* Find remaining bits in word starting from off to end of word */
  uint32_t rem_bits_in_word = 32 - word_off;
  uint32_t nbits            = rem_bits_in_word;
  /* If num_bits is greater than the remaining number of bits to end of word
   * toggle the remaining number of bits in word, else only toggle num_bits */
  XT_MOVLTZ(nbits, num_bits, num_bits - rem_bits_in_word);
  /* Form bit mask of the number of bits to toggle */
  uint32_t n = 1 << nbits;
  XT_MOVEQZ(n, 0, nbits - 32);
  /* Align the mask to the word offset */
  uint32_t bmask = (n - 1) << word_off;
  /* Negate the bits in the word */
  *(bitvec + word_idx) ^= bmask;
  num_bits             -= nbits;
  word_idx++;
  /* If there are more bits remaining, continue with the next word */
  while (num_bits > 0)
  {
    nbits = 32;
    /* Form bit mask of either 32 1s or num_bits 1s if num_bits < 32 */
    XT_MOVLTZ(nbits, num_bits, num_bits - 32);
    n = 1 << nbits;
    XT_MOVEQZ(n, 0, nbits - 32);
    bmask = n - 1;
    /* Negate the bits in the word */
    *(bitvec + word_idx) ^= bmask;
    num_bits             -= nbits;
    word_idx++;
  }
}

/* Function that finds the leading number of 0s or 1s starting from the bit
 * position 'pos' of the bitvector 'bitvec'. 'bitvec' is an arbitrary length
 * bitvector of 'num_bits' length that is stored in an array of 32-bit words.
 *
 * bitvec     : arbitrary length bitvector
 * num_bits   : number of bits in the bitvector
 * pos        : position within the bitvector to find leading 1 or 0 count
 * w          : initial word where pos points to
 * zero_count : If 0, find the 0 count, if 0xffffffff, find 1 count
 *
 * Returns the leading 1 or 0 count
 */
__attribute__((unused)) static uint32_t
xmem_find_leading_zero_one_count(const uint32_t *bitvec,
                                 uint32_t num_bits,
                                 uint32_t pos,
                                 uint32_t w,
                                 uint32_t zero_count)
{
#ifdef XMEM_VERIFY
  if (pos >= num_bits)
  {
    XMEM_ABORT("xmem_find_leading_zero_one_count: pos %d has to be between 0 and %d\n", pos, num_bits - 1);
  }
#endif
  /* Find the word corresponding to bit position pos */
  uint32_t word_idx = pos >> 5;
  /* Find bit offset within this word */
  uint32_t word_off = pos & 31;
  /* Not all of the word will be in use. If num_bits is < 32, then only
   * num_bits are used else all 32-bits are used */
  uint32_t num_bits_used_in_word = 32;
  XT_MOVLTZ(num_bits_used_in_word, num_bits, (int32_t) (num_bits - 32));
  /* Find remaining bits in word starting from off to end of word */
  uint32_t num_rem_bits_in_word = num_bits_used_in_word - word_off;
  uint32_t num_words_in_bitvec  = (num_bits + 31) >> 5;
  w ^= zero_count;
  uint32_t ws = w >> word_off;
  /* Find position of the least signficant 1b */
  uint32_t ls1b     = ws & - ws;
  uint32_t ls1b_pos = xmem_find_msbit(ls1b);
  /* If there are no 1s in the word then the leading count is the remaining
   * bits in the word */
  uint32_t lzc = ls1b_pos;
  XT_MOVEQZ(lzc, num_rem_bits_in_word, ls1b);
  uint32_t num_rem_words = num_words_in_bitvec - (word_idx + 1);
  /* While there is no 1 in the word and there are words remaining, continue */
  while (ls1b == 0 && num_rem_words != 0)
  {
    num_bits -= 32;
    word_idx++;
    num_rem_words--;
    w                     = bitvec[word_idx];
    w                    ^= zero_count;
    ls1b                  = w & - w;
    ls1b_pos              = xmem_find_msbit(ls1b);
    num_bits_used_in_word = 32;
    XT_MOVLTZ(num_bits_used_in_word, num_bits, (int32_t) (num_bits - 32));
    uint32_t l = ls1b_pos;
    XT_MOVEQZ(l, num_bits_used_in_word, ls1b);
    lzc += l;
  }
  return(lzc);
}

/* Find number of zero/one bits in bit vector
 *
 * bitvec     : arbitrary length bitvector
 * num_bits   : number of bits in the bitvector
 * one_count : If 0, find the 1 count, if 0xffffffff, find 0 count
 *
 * Returns number of set bits
 */
__attribute__((unused)) static uint32_t
xmem_pop_count(const uint32_t *bitvec,
               uint32_t num_bits,
               uint32_t one_count)
{
  int i = 0;
  int n = num_bits;
  int c = 0;
  while (n > 0)
  {
    uint32_t v = bitvec[i];
    v ^= one_count;
    v  = v - ((v >> 1) & 0x55555555);
    v  = (v & 0x33333333) + ((v >> 2) & 0x33333333);
    c += (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
    n -= 32;
    i++;
  }
  return(c);
}

#endif /* __XMEM_INTERNAL_H__ */
