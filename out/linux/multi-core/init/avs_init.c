/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#include <linux/module.h>
#include <linux/of_platform.h>
#include "hi_osal.h"

extern int avs_mod_init(void);
extern void avs_mod_exit(void);

extern unsigned int g_avs_irq;
extern void *g_avs_reg;

static int hi35xx_avs_probe(struct platform_device *pdev)
{
    struct resource *mem;

    mem = osal_platform_get_resource(pdev, IORESOURCE_MEM, 0);

    g_avs_reg = devm_ioremap_resource(&pdev->dev, mem);

    if (IS_ERR(g_avs_reg)) {
        return PTR_ERR(g_avs_reg);
    }

    g_avs_irq = osal_platform_get_irq(pdev, 0);

    if (g_avs_irq <= 0) {
        dev_err(&pdev->dev, "cannot find avs IRQ\n");
    }

    if (avs_mod_init()) {
        return -1;
    }

    return 0;
}

static int hi35xx_avs_remove(struct platform_device *pdev)
{
    avs_mod_exit();
    g_avs_reg = NULL;
    return 0;
}

static const struct of_device_id hi35xx_avs_match[] = {
    { .compatible = "hisilicon,hisi-avs" },
    {},
};

MODULE_DEVICE_TABLE(of, hi35xx_avs_match);

static struct platform_driver hi35xx_avs_driver = {
    .probe = hi35xx_avs_probe,
    .remove = hi35xx_avs_remove,
    .driver = {
        .name = "hi35xx_avs",
        .of_match_table = hi35xx_avs_match,
    },
};

osal_module_platform_driver(hi35xx_avs_driver);

MODULE_LICENSE("Proprietary");
