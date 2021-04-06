/******************************************************************************

  Copyright (C), 2013-2033, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : photo_init.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       :
  Last Modified :
  Description   : hisilicon HDR algorithm
  Function List :
  </$>
  History       :
  1.Date        : 20160607
  Author      :
  Modification: Created file
  <$/>
 ******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"

extern int PHOTO_DRV_ModInit(void);
extern void PHOTO_DRV_ModExit(void);

static int __init photo_mod_init(void)
{
    return PHOTO_DRV_ModInit();
}

static void __exit photo_mod_exit(void)
{
    PHOTO_DRV_ModExit();
}

module_init(photo_mod_init);
module_exit(photo_mod_exit);

MODULE_LICENSE("Proprietary");


