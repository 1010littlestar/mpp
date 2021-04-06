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


#ifndef __XMEM_OS_H__
#define __XMEM_OS_H__

#include <stdint.h>
#include <xtensa/xtruntime.h>

extern uint32_t xmem_disable_interrupts();
extern void xmem_enable_interrupts(uint32_t level);

#endif /* __XMEM_OS_H__ */
