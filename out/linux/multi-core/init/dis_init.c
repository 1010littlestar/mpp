#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>

#include "hi_common.h"
#include "hi_osal.h"

#define DIS_DEV_NAME_LENGTH 10

extern int dis_module_init(void);
extern void dis_module_exit(void);

#if (HI3559A_V100ES == HICHIP)
static int __init dis_mod_init(void)
{
    dis_module_init();
    return 0;
}
static void __exit dis_mod_exit(void)
{
    dis_module_exit();
}

module_init(dis_mod_init);
module_exit(dis_mod_exit);

MODULE_LICENSE("Proprietary");
#else

#include <linux/of_platform.h>

extern void * g_dis_reg[2];
extern unsigned int g_dis_irq;

static int hi35xx_dis_probe(struct platform_device *pdev)
{
    struct resource *mem;
    HI_CHAR DisDevName[DIS_DEV_NAME_LENGTH] = "dis";

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, DisDevName);
    g_dis_reg[0] = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_dis_reg[0]))
            return PTR_ERR(g_dis_reg[0]);

    g_dis_irq = osal_platform_get_irq_byname(pdev, DisDevName);
    if (g_dis_irq <= 0) {
            dev_err(&pdev->dev, "cannot find dis IRQ\n");
    }

    dis_module_init();

    return 0;
}

static int hi35xx_dis_remove(struct platform_device *pdev)
{
    dis_module_exit();

    g_dis_reg[0] = 0;

    return 0;
}


static const struct of_device_id hi35xx_dis_match[] = {
        { .compatible = "hisilicon,hisi-dis" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_dis_match);

static struct platform_driver hi35xx_dis_driver = {
        .probe          = hi35xx_dis_probe,
        .remove         = hi35xx_dis_remove,
        .driver         = {
                .name   = "hi35xx_dis",
                .of_match_table = hi35xx_dis_match,
        },
};

osal_module_platform_driver(hi35xx_dis_driver);

MODULE_LICENSE("Proprietary");
#endif

