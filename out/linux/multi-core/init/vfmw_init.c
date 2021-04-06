#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_type.h"
#include "hi_osal.h"
#include "hi_defines.h"

extern int VFMW_ModInit(void);
extern void VFMW_ModExit(void);

extern int g_scd_int;
extern int g_vdm_ilp_int;
extern int g_vdm_olp_int;
extern void *g_vdm_reg_addr;

extern int g_vfmw_max_chn_num;
module_param(g_vfmw_max_chn_num, uint, S_IRUGO);

#define VFMW_DEV_NAME_LENGTH 10

static int hi35xx_vfmw_probe(struct platform_device *pdev)
{
    struct resource *mem;
    HI_CHAR DevName[VFMW_DEV_NAME_LENGTH] = {'\0'};

    snprintf(DevName, VFMW_DEV_NAME_LENGTH, "vdh_scd");
    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, DevName);
    g_vdm_reg_addr = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_vdm_reg_addr))
            return PTR_ERR(g_vdm_reg_addr);

    g_vdm_ilp_int = osal_platform_get_irq_byname(pdev, "vdh_ilp");
    if (g_vdm_ilp_int <= 0) {
            dev_err(&pdev->dev, "cannot find vdh_ilp IRQ\n");
    }

    g_vdm_olp_int = osal_platform_get_irq_byname(pdev, "vdh_olp");
    if (g_vdm_olp_int <= 0) {
        dev_err(&pdev->dev, "cannot find vdh_olp IRQ\n");
    }

    g_scd_int = osal_platform_get_irq_byname(pdev, "scd");
    if (g_scd_int <= 0) {
        dev_err(&pdev->dev, "cannot find scd IRQ\n");
    }

    VFMW_ModInit();

    return 0;
}

static int hi35xx_vfmw_remove(struct platform_device *pdev)
{
    VFMW_ModExit();

    return 0;
}


static const struct of_device_id hi35xx_vfmw_match[] = {
        { .compatible = "hisilicon,hisi-vdh" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_vfmw_match);

static struct platform_driver hi35xx_vfmw_driver = {
        .probe          = hi35xx_vfmw_probe,
        .remove         = hi35xx_vfmw_remove,
        .driver         = {
                .name   = "hi35xx_vdh",
                .of_match_table = hi35xx_vfmw_match,
        },
};

osal_module_platform_driver(hi35xx_vfmw_driver);

MODULE_LICENSE("Proprietary");
