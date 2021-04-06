/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#ifndef __STRFUNC_H__
#define __STRFUNC_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define STRFMT_ADDR32   "%#010lX"
#define STRFMT_ADDR32_2 "0x%08lX"

int StrToNumber(char *str, unsigned int *ulValue);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __STRFUNC_H__ */
