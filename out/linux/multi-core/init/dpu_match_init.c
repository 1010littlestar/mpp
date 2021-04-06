#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include <asm/io.h>

#include "hi_comm_video.h"
#include "hi_common.h"
#include "hi_osal.h"

extern int dpu_match_init(void);
extern void dpu_match_exit(void);

extern void *g_dpu_match_reg;

extern unsigned int g_dpu_match_irq;

#define REG_BASE 0x11630000
#define DPU_MATCH_NAME_LENGTH 16

static int hi35xx_dpu_match_probe(struct platform_device *dev)
{
    HI_CHAR aDevName[DPU_MATCH_NAME_LENGTH] = "match";
    HI_U32 u32RegLen = 0x10000;
    int irq;

    g_dpu_match_reg = ioremap(REG_BASE, u32RegLen);
    if (IS_ERR(g_dpu_match_reg)) {
        return PTR_ERR(g_dpu_match_reg);
    }

    irq = osal_platform_get_irq_byname(dev, aDevName);
    if (irq <= 0) {
        dev_err(&dev->dev, "cannot find match IRQ\n");
    }
    g_dpu_match_irq = (unsigned int)irq;

    dpu_match_init();

    return 0;
}

static int hi35xx_dpu_match_remove(struct platform_device *dev)
{
    dpu_match_exit();

    if (g_dpu_match_reg) {
        iounmap(g_dpu_match_reg);
        g_dpu_match_reg = NULL;
    }

    return 0;
}

static const struct of_device_id hi35xx_dpu_match_match[] = {
    { .compatible = "hisilicon,hisi-dpu_match" },
    {},
};
MODULE_DEVICE_TABLE(of, hi35xx_dpu_match_match);

static struct platform_driver hi35xx_dpu_match_driver = {
    .probe = hi35xx_dpu_match_probe,
    .remove = hi35xx_dpu_match_remove,
    .driver = {
        .name = "hi35xx_dpu_match",
        .of_match_table = hi35xx_dpu_match_match,
    },
};

osal_module_platform_driver(hi35xx_dpu_match_driver);

MODULE_LICENSE("Proprietary");

