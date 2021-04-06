/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: DSP trace
 * Author: svp group
 * Create: 2017-10-17
 */
#ifndef __SVP_DSP_TRACE_H__
#define __SVP_DSP_TRACE_H__

/******************************************************************************/
/****************** #if defined(_WIN32), for x86 windows *************************/
/******************************************************************************/
#if defined(_WIN32)

#define HI_TRACE(level, enModId, ...) fprintf(stderr, __VA_ARGS__)
/* Using samples:
* HI_TRACE(HI_DBG_DEBUG, HI_ID_SVP, "Test %d, %s\n", 12, "Test");
*/
/********************************* for WIN32 ************************************/
#define HI_TRACE_SVP_DSP(level, ...)                                                           \
    do {                                                                                       \
        HI_TRACE(level, HI_ID_SVP_DSP, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__); \
        HI_TRACE(level, HI_ID_SVP_DSP, __VA_ARGS__);                                           \
    } while (0)

#define SVP_DSP_CHECK_EXPR_GOTO(expr, label, level, ...) \
    do {                                                 \
        if (expr) {                                      \
            HI_TRACE_SVP_DSP(level, __VA_ARGS__);        \
            goto label;                                  \
        }                                                \
    } while (0)

#define SVP_DSP_CHECK_EXPR_RET_VOID(expr, level, ...) \
    do {                                              \
        if (expr) {                                   \
            HI_TRACE_SVP_DSP(level, __VA_ARGS__);     \
            return;                                   \
        }                                             \
    } while (0)

#define SVP_DSP_CHECK_EXPR_RET(expr, ret, level, ...) \
    do {                                              \
        if (expr) {                                   \
            HI_TRACE_SVP_DSP(level, __VA_ARGS__);     \
            return ret;                               \
        }                                             \
    } while (0)

#define SVP_DSP_CHECK_NIN_CLOSE_RANGE(srcData, minVal, maxVal, ret, ...) \
    do {                                                                 \
        if (((srcData) < (minVal)) || ((srcData) > (maxVal))) {          \
            HI_TRACE_SVP_DSP(HI_DBG_ERR, __VA_ARGS__);                   \
            return (ret);                                                \
        }                                                                \
    } while (0)

/*
* Left open right close
*/
#define SVP_DSP_CHECK_NIN_LO_RC(srcData, minVal, maxVal, ret, ...) \
    do {                                                           \
        if (((srcData) <= (minVal)) || ((srcData) > (maxVal))) {   \
            HI_TRACE_SVP_DSP(HI_DBG_ERR, __VA_ARGS__);             \
            return (ret);                                          \
        }                                                          \
    } while (0)

/*
* left close right open
*/
#define SVP_DSP_CHECK_NIN_LC_RO(param, ret, level, minVal, maxVal, ...) \
    do {                                                                \
        if (((param) < (minVal)) || ((param) >= (maxVal))) {            \
            HI_TRACE_SVP_DSP(level, __VA_ARGS__);                       \
            return (ret);                                               \
        }                                                               \
    } while (0)

#define SVP_DSP_CHECK_PHYADDR_ALIGN(phyAddr, align, ret, ...)

/******************************************************************************/
/********************** #else, for x86 linux & board & tesilica **************************/
/******************************************************************************/
#else

#ifndef HI_TRACE
#define HI_TRACE(level, enModId, fmt...) printf(##fmt)
#endif
/********************************* for SVP ************************************/
#define HI_TRACE_SVP_DSP(level, fmt...)                                                        \
    do {                                                                                       \
        HI_TRACE(level, HI_ID_SVP_DSP, "[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__); \
        HI_TRACE(level, HI_ID_SVP_DSP, ##fmt);                                                 \
    } while (0)
/*
* Expr is true,goto
*/
#define SVP_DSP_CHECK_EXPR_GOTO(expr, label, level, fmt...) \
    do {                                                    \
        if (expr) {                                         \
            HI_TRACE_SVP_DSP(level, ##fmt);                 \
            goto label;                                     \
        }                                                   \
    } while (0)

/*
* Expr is true,return void
*/
#define SVP_DSP_CHECK_EXPR_RET_VOID(expr, level, fmt...) \
    do {                                                 \
        if (expr) {                                      \
            HI_TRACE_SVP_DSP(level, ##fmt);              \
            return;                                      \
        }                                                \
    } while (0)
/*
* Expr is true,return ret
*/
#define SVP_DSP_CHECK_EXPR_RET(expr, ret, level, fmt...) \
    do {                                                 \
        if (expr) {                                      \
            HI_TRACE_SVP_DSP(level, ##fmt);              \
            return ret;                                  \
        }                                                \
    } while (0)

#define SVP_DSP_CHECK_NIN_CLOSE_RANGE(srcData, minVal, maxVal, ret, fmt...) \
    do {                                                                    \
        if (((srcData) < (minVal)) || ((srcData) > (maxVal))) {             \
            HI_TRACE_SVP_DSP(HI_DBG_ERR, ##fmt);                            \
            return (ret);                                                   \
        }                                                                   \
    } while (0)

/*
* Left open right close
*/
#define SVP_DSP_CHECK_NIN_LO_RC(srcData, minVal, maxVal, ret, fmt...) \
    do {                                                              \
        if (((srcData) <= (minVal)) || ((srcData) > (maxVal))) {      \
            HI_TRACE_SVP_DSP(HI_DBG_ERR, ##fmt);                      \
            return (ret);                                             \
        }                                                             \
    } while (0)

/*
* left close right open
*/
#define SVP_DSP_CHECK_NIN_LC_RO(param, ret, level, minVal, maxVal, fmt...) \
    do {                                                                   \
        if (((param) < (minVal)) || ((param) >= (maxVal))) {               \
            HI_TRACE_SVP_DSP(level, ##fmt);                                \
            return (ret);                                                  \
        }                                                                  \
    } while (0)

/*
* Check phyaddr
*/
#define SVP_DSP_CHECK_PHYADDR_ALIGN(phyAddr, align, ret, fmt...) \
    do {                                                         \
        if (((phyAddr) % (align)) != 0) {                        \
            HI_TRACE_SVP_DSP(HI_DBG_ERR, ##fmt);                 \
            return (ret);                                        \
        }                                                        \
    } while (0)

#endif

/* NULL pointer check */
#define SVP_DSP_CHECK_NULL_PTR_RET(ptr) SVP_DSP_CHECK_EXPR_RET(NULL == (ptr), HI_ERR_SVP_DSP_NULL_PTR,\
    HI_DBG_ERR, "Error," #ptr " is NULL!\n")

#endif  // __SVP_DSP_TRACE_H__


