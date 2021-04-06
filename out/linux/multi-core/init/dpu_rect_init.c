#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include <asm/io.h>

#include "hi_comm_video.h"
#include "hi_common.h"
#include "hi_osal.h"

extern int dpu_rect_init(void);
extern void dpu_rect_exit(void);

extern void *g_dpu_rect_reg;

extern unsigned int g_dpu_rect_irq;

#define REG_BASE             0x11630000
#define DPU_RECT_NAME_LENGTH 16

static int hi35xx_dpu_rect_probe(struct platform_device *dev)
{
    HI_CHAR aDevName[DPU_RECT_NAME_LENGTH] = "rect";
    HI_U32 u32RegLen = 0x10000;
    int iIrq;

    g_dpu_rect_reg = ioremap(REG_BASE, u32RegLen);

    if (IS_ERR(g_dpu_rect_reg)) {
        return PTR_ERR(g_dpu_rect_reg);
    }

    iIrq = osal_platform_get_irq_byname(dev, aDevName);

    if (iIrq <= 0) {
        dev_err(&dev->dev, "cannot find rect IRQ\n");
    }

    g_dpu_rect_irq = (unsigned int)iIrq;

    dpu_rect_init();

    return 0;
}

static int hi35xx_dpu_rect_remove(struct platform_device *dev)
{
    dpu_rect_exit();

    if (g_dpu_rect_reg) {
        iounmap(g_dpu_rect_reg);
        g_dpu_rect_reg = NULL;
    }

    return 0;
}

static const struct of_device_id hi35xx_dpu_rect_match[] = {
    { .compatible = "hisilicon,hisi-dpu_rect" },
    {},
};
MODULE_DEVICE_TABLE(of, hi35xx_dpu_rect_match);

static struct platform_driver hi35xx_dpu_rect_driver = {
    .probe = hi35xx_dpu_rect_probe,
    .remove = hi35xx_dpu_rect_remove,
    .driver = {
        .name = "hi35xx_dpu_rect",
        .of_match_table = hi35xx_dpu_rect_match,
    },
};

osal_module_platform_driver(hi35xx_dpu_rect_driver);

MODULE_LICENSE("Proprietary");

