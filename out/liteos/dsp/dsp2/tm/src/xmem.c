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
#include <xtensa/config/core-isa.h>
#if XCHAL_HAVE_S32C1I
#include <xtensa/tie/xt_sync.h>
#endif
#ifdef XMEM_USE_XOS
#include "xmem_xos.h"
#elif XMEM_USE_XTOS
#include "xmem_xtos.h"
#else
#include "xmem_noos.h"
#endif
#include "xmem_internal.h"

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
void *
xmem_alloc(xmem_mgr_t *mgr,
           size_t size,
           uint32_t align,
           xmem_status_t *err_code)
{
    if (mgr->_initialized != XMEM_INITIALIZED)
    {
        *err_code = XMEM_ERROR_UNINITIALIZED;
        return(NULL);
    }

    //uint32_t ps = xmem_disable_interrupts();

#if XCHAL_HAVE_S32C1I || XCHAL_HAVE_EXCLUSIVE
    xmp_simple_spinlock_acquire((xmp_atomic_int_t *) &mgr->_lock);
#endif

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
    xthal_dcache_region_invalidate((void *) mgr, sizeof(xmem_mgr_t));
#endif

    void *r = NULL;
    switch (mgr->_alloc_type)
    {
        case XMEM_HEAP1:
            r = xmem_heap1_alloc(&mgr->_m._heap1, size, align, err_code);
            break;
        case XMEM_HEAP2:
            r = xmem_heap2_alloc(&mgr->_m._heap2, size, align, err_code);
            break;
        case XMEM_HEAP3:
            r = xmem_heap3_alloc(&mgr->_m._heap3, size, align, err_code);
            break;
        default:
            XMEM_ABORT("xmem_alloc: Unknown mem manager type %d\n", mgr->_alloc_type);
            *err_code = XMEM_ERROR_UNKNOWN_HEAP_TYPE;
    }
#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
    xthal_dcache_region_writeback((void *) mgr, sizeof(xmem_mgr_t));
#endif

#if XCHAL_HAVE_S32C1I || XCHAL_HAVE_EXCLUSIVE
    xmp_simple_spinlock_release((xmp_atomic_int_t *) &mgr->_lock);
#endif

    //xmem_enable_interrupts(ps);
    return(r);
}

/* Free a pointer allocated using the memory manager mgr
 *
 * mgr : memory manager object
 * p   : pointer to free
 *
 * Returns void.
 */
void
xmem_free(xmem_mgr_t *mgr, void *p)
{
    if (mgr->_initialized != XMEM_INITIALIZED)
    {
        return;
    }

    //uint32_t ps = xmem_disable_interrupts();

#if XCHAL_HAVE_S32C1I || XCHAL_HAVE_EXCLUSIVE
    xmp_simple_spinlock_acquire((xmp_atomic_int_t *) &mgr->_lock);
#endif

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
    xthal_dcache_region_invalidate((void *) mgr, sizeof(xmem_mgr_t));
#endif

    switch (mgr->_alloc_type)
    {
        case XMEM_HEAP1:
            break;
        case XMEM_HEAP2:
            xmem_heap2_free(&mgr->_m._heap2, p);
            break;
        case XMEM_HEAP3:
            xmem_heap3_free(&mgr->_m._heap3, p);
            break;
        default:
            XMEM_ABORT("xmem_free: Unknown mem manager type %d\n", mgr->_alloc_type);
    }

#if XCHAL_DCACHE_SIZE > 0 && !XCHAL_DCACHE_IS_COHERENT
    xthal_dcache_region_writeback((void *) mgr, sizeof(xmem_mgr_t));
#endif

#if XCHAL_HAVE_S32C1I || XCHAL_HAVE_EXCLUSIVE
    xmp_simple_spinlock_release((xmp_atomic_int_t *) &mgr->_lock);
#endif

    //xmem_enable_interrupts(ps);
}

/* Print heap usage statistics
 *
 * mgr : memory manager object
 *
 * Returns void
 */
void
xmem_print_stats(xmem_mgr_t *mgr)
{
    switch (mgr->_alloc_type)
    {
    case XMEM_HEAP1:
        xmem_heap1_print_stats(&mgr->_m._heap1);
        break;
    case XMEM_HEAP2:
        xmem_heap2_print_stats(&mgr->_m._heap2);
        break;
    case XMEM_HEAP3:
        xmem_heap3_print_stats(&mgr->_m._heap3);
        break;
    default:
        XMEM_ABORT("xmem_print_stats: Unknown mem manager type %d\n", mgr->_alloc_type);
        return;
    }
}

