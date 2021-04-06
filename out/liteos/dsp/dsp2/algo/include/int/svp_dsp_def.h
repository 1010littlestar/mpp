/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: VDSP algorithm define header file
 * Author: svp group
 * Create: 2017-10-17
 */
#ifndef __SVP_DSP_DEF_H__
#define __SVP_DSP_DEF_H__

#include <xtensa/tie/xt_ivpn.h>
#include <xtensa/xt_profiling.h>
#include <xtensa/tie/xt_timer.h>

#define IVP

#if defined(__XTENSA__) || defined(GCC)
#define ALIGN(x) __attribute__((aligned(x)))
#ifndef ALIGN16
#define ALIGN16 __attribute__((aligned(16)))
#endif
#define ALIGN32 __attribute__((aligned(32)))
#define ALIGN64 __attribute__((aligned(64)))
#else
#define ALIGN(x) _declspec(align(x))
#ifndef ALIGN16
#define ALIGN16 _declspec(align(16))
#endif
#define ALIGN32 _declspec(align(32))
#define ALIGN64 _declspec(align(64))
#define __restrict
#endif

#ifdef IVP
#define _LOCAL_DRAM0_ __attribute__((section(".dram0.data")))
#define _LOCAL_DRAM1_ __attribute__((section(".dram1.data")))
#define _TILEMGR_IRAM0_

#else
#define _LOCAL_DRAM0_
#define _LOCAL_DRAM1_
#define _TILEMGR_IRAM0_
#endif

#ifdef IVP
#define IVP_SIMD_WIDTH XCHAL_IVPN_SIMD_WIDTH
#else
#define IVP_SIMD_WIDTH 32
#endif

#define IVP_SIMD_WIDTH_LOG2 5

// SVP_DSP_TIME_STAMP macro is used to measure the cycles

#define SVP_DSP_POOL_SIZE         (180 * 1024)
#define SVP_DSP_DMA_DESCR_CNT     (32)  // number of DMA decsriptors
#define SVP_DSP_MAX_PIF           (32)
#define SVP_DSP_INT_ON_COMPLETION (1)
#define SVP_DSP_HAVE_HISTOGRAM    1  // XCHAL_HAVE_GRIVPEP_HISTOGRAM

#define SVP_DSP_CLIP(a, maxv, minv) (((a) > (maxv)) ? (maxv) : (((a) < (minv)) ? (minv) : (a)))
#define SVP_DSP_TIME_STAMP(cycCnt)  (cycCnt) = XT_RSR_CCOUNT()

/***********************************
 * for Filter1D_Hor and Filter1D_Ver
 **********************************/
#define SVP_DSP_FILTER1D_SHIFT_FACTOR 15
#define SVP_DSP_FILTER1D_BIAS         (1 << 14)

/***********************************
 * Common OFFSET_PTR macro
 **********************************/
#define SVP_DSP_OFFSET_PTR_2NX8U(ptr, nrows, stride, in_row_offset)   \
    ((xb_vec2Nx8U*)((uint8_t*)(ptr) + (in_row_offset) + (nrows) * (stride)))
#define SVP_DSP_OFFSET_PTR_N_2X32U(ptr, nrows, stride, in_row_offset) \
    ((xb_vecN_2x32Uv*)((uint32_t*)(ptr) + (in_row_offset) + (nrows) * (stride)))
#define SVP_DSP_OFFSET_PTR_N_2X32(ptr, nrows, stride, in_row_offset)  \
    ((xb_vecN_2x32v*)((int32_t*)(ptr) + (in_row_offset) + (nrows) * (stride)))
#define SVP_DSP_OFFSET_PTR_NX16U(ptr, nrows, stride, in_row_offset)   \
    ((xb_vecNx16U*)((uint16_t*)(ptr) + (in_row_offset) + (nrows) * (stride)))
#define SVP_DSP_OFFSET_PTR_NX16(ptr, nrows, stride, in_row_offset)    \
    ((xb_vecNx16*)((int16_t*)(ptr) + (in_row_offset) + (nrows) * (stride)))
#define SVP_DSP_OFFSET_PTR_N_2X16(ptr, nrows, stride, in_row_offset)  \
    ((xb_vecN_2x16*)((int16_t*)(ptr) + (in_row_offset) + (nrows) * (stride)))
#define SVP_DSP_OFFSET_PTR_NX8U(ptr, nrows, stride, in_row_offset)    \
    ((xb_vecNx8U*)((uint8_t*)(ptr) + (in_row_offset) + (nrows) * (stride)))
#define SVP_DSP_OFFSET_PTR_NX8(ptr, nrows, stride, in_row_offset)     \
    ((xb_vecNx8*)((int8_t*)(ptr) + (in_row_offset) + (nrows) * (stride)))
#define SVP_DSP_OFFSET_PTR_2NX8(ptr, nrows, stride, in_row_offset)    \
    ((xb_vec2Nx8*)((int8_t*)(ptr) + (in_row_offset) + (nrows) * (stride)))

// The number of erode frame and tile
// Erode tile width and height
#define SVP_DSP_ERODE_TILE_WIDTH        (256)
#define SVP_DSP_ERODE_TILE_HEIGHT       (128)
#define SVP_DSP_ERODE_FRAME_NUM         2
#define SVP_DSP_ERODE_IN_TILE_NUM       2
#define SVP_DSP_ERODE_OUT_TILE_NUM      2
#define SVP_DSP_ERODE_IN_TILE_BUFF_NUM  2
#define SVP_DSP_ERODE_OUT_TILE_BUFF_NUM 2
// Erode min/max width/height
#define SVP_DSP_ERODE_MIN_WIDTH  64
#define SVP_DSP_ERODE_MIN_HEIGHT 64
#define SVP_DSP_ERODE_MAX_WIDTH  1920
#define SVP_DSP_ERODE_MAX_HEIGHT 1080

// The number of dilate frame and tile
// Dilate tile width and height
#define SVP_DSP_DILATE_TILE_WIDTH        (256)
#define SVP_DSP_DILATE_TILE_HEIGHT       (128)
#define SVP_DSP_DILATE_FRAME_NUM         2
#define SVP_DSP_DILATE_IN_TILE_NUM       2
#define SVP_DSP_DILATE_OUT_TILE_NUM      2
#define SVP_DSP_DILATE_IN_TILE_BUFF_NUM  2
#define SVP_DSP_DILATE_OUT_TILE_BUFF_NUM 2
// Dilate min/max width/height
#define SVP_DSP_DILATE_MIN_WIDTH  64
#define SVP_DSP_DILATE_MIN_HEIGHT 64
#define SVP_DSP_DILATE_MAX_WIDTH  1920
#define SVP_DSP_DILATE_MAX_HEIGHT 1080

#define SVP_DSP_ALIGN_64       64
#define SVP_DSP_EDGE_PADDING_2 2
#define SVP_DSP_PING_PONG_NUM  2
#endif  // __SVP_DSP_DEF_H__

