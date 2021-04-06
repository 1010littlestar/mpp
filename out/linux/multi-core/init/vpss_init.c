#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"
#include "hi_osal.h"
#include "hi_defines.h"

extern int vpss_module_init(void);
extern void vpss_module_exit(void);

extern unsigned int vpss_en[VPSS_IP_NUM];
module_param_array(vpss_en, uint, HI_NULL, S_IRUGO);


extern void * g_vpss_reg[VPSS_IP_NUM];
extern int g_vpss_irq[VPSS_IP_NUM];

#define VPSS_DEV_NAME_LENGTH 10

static int hi35xx_vpss_probe(struct platform_device *pdev)
{
    hi_u32 i;
    hi_char vpss_dev_name[VPSS_DEV_NAME_LENGTH] = {'\0'};
    struct resource* mem;

    for (i = 0; i < VPSS_IP_NUM; i++)
    {
        snprintf(vpss_dev_name, VPSS_DEV_NAME_LENGTH, "vpss%d", i);
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, vpss_dev_name);
        g_vpss_reg[i] = devm_ioremap_resource(&pdev->dev, mem);

        if (IS_ERR(g_vpss_reg[i]))
        { return PTR_ERR(g_vpss_reg[i]); }

        g_vpss_irq[i] = osal_platform_get_irq_byname(pdev, vpss_dev_name);

        if (g_vpss_irq[i] <= 0)
        {
            dev_err(&pdev->dev, "cannot find vpss%d IRQ\n", i);
        }
    }

    vpss_module_init();

    return 0;
}


static int hi35xx_vpss_remove(struct platform_device *pdev)
{
    hi_u32 i;

    vpss_module_exit();

    for (i = 0; i < VPSS_IP_NUM; i++)
    {
        g_vpss_reg[i] = HI_NULL;
    }

    return 0;
}


static const struct of_device_id hi35xx_vpss_match[] = {
        { .compatible = "hisilicon,hisi-vpss" },
        {},
};

MODULE_DEVICE_TABLE(of, hi35xx_vpss_match);

static struct platform_driver hi35xx_vpss_driver = {
        .probe          = hi35xx_vpss_probe,
        .remove         = hi35xx_vpss_remove,
        .driver         = {
                .name   = "hi35xx_vpss",
                .of_match_table = hi35xx_vpss_match,
        },
};

osal_module_platform_driver(hi35xx_vpss_driver);

MODULE_LICENSE("Proprietary");


