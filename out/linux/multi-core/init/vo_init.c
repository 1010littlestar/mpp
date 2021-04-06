/*
* Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <linux/module.h>

#include "hi_defines.h"
#include "hi_type.h"
#include "hi_osal.h"
#include "vou_exp.h"

extern VOU_EXPORT_SYMBOL_S g_vou_exp_symbol;

EXPORT_SYMBOL(g_vou_exp_symbol);

extern void vou_drv_enable_dev(int vo_dev);
void VOU_DRV_EnableDev(int dev)
{
    vou_drv_enable_dev(dev);
}
EXPORT_SYMBOL(VOU_DRV_EnableDev);

extern void vou_drv_disable_dev(int vo_dev);
void VOU_DRV_DisableDev(int dev)
{
    vou_drv_disable_dev(dev);
}
EXPORT_SYMBOL(VOU_DRV_DisableDev);

extern int vou_mod_init(void);
extern void vou_mod_exit(void);

#include <linux/of_platform.h>

extern void *g_vo_reg;
extern void *g_vo_hipp_reg;
extern unsigned int g_vou_irq;

#define VO_DEV_NAME_LENGTH 10

static int hi35xx_vo_probe(struct platform_device *pdev)
{
    HI_CHAR vo_dev_name[VO_DEV_NAME_LENGTH] = "vo";
    HI_VOID *reg = NULL;
    struct resource *mem;
    int ret;
    int tmp_vou_irq;

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, vo_dev_name);
    reg = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(reg)) {
        osal_printk("devm_ioremap_resource fail for %ld\n", PTR_ERR(reg));
        return PTR_ERR(reg);
    }
    g_vo_reg     = reg;
    g_vo_hipp_reg = reg;
    tmp_vou_irq = osal_platform_get_irq_byname(pdev, vo_dev_name);
    if (tmp_vou_irq <= 0) {
        dev_err(&pdev->dev, "cannot find vou IRQ\n");
        return HI_FAILURE;
    }
    else {
        g_vou_irq = (unsigned int)tmp_vou_irq;
    }

    ret = vou_mod_init();
    if (ret) {
        return -1;
    }

    return 0;
}

static int hi35xx_vo_remove(struct platform_device *pdev)
{
    vou_mod_exit();
    g_vo_reg     = NULL;
    g_vo_hipp_reg = NULL;

    return 0;
}

static const struct of_device_id g_hi35xx_vo_match[] = {
        { .compatible = "hisilicon,hisi-vo" },
        {},
};
MODULE_DEVICE_TABLE(of, g_hi35xx_vo_match);

static struct platform_driver g_hi35xx_vo_driver = {
        .probe          = hi35xx_vo_probe,
        .remove         = hi35xx_vo_remove,
        .driver         = {
                .name   = "hi35xx_vo",
                .of_match_table = g_hi35xx_vo_match,
        },
};

osal_module_platform_driver(g_hi35xx_vo_driver);

MODULE_LICENSE("Proprietary");
