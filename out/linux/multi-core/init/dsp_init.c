#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>

#include "hi_common.h"
#include "hi_osal.h"
#include "hi_dsp.h"

extern hi_u16 max_node_num;
extern hi_u16 dsp_init_mode;

extern int svp_dsp_std_mod_init(void);
extern void svp_dsp_std_mod_exit(void);

module_param(max_node_num, ushort, S_IRUGO);
module_param(dsp_init_mode, ushort, S_IRUGO);

#include <linux/of_platform.h>
#define SVP_DSP_DEV_NAME_LENGTH 10
extern volatile void *g_svp_dsp_reg[SVP_DSP_ID_BUTT];

static int hi35xx_svp_dsp_probe(struct platform_device *pdev)
{
    unsigned int i;
    char dev_name[SVP_DSP_DEV_NAME_LENGTH] = { '\0' };
    struct resource *mem = NULL;

    for (i = 0; i < SVP_DSP_ID_BUTT; i++) {
        snprintf(dev_name, SVP_DSP_DEV_NAME_LENGTH, "dsp%u", i);
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, dev_name);
        g_svp_dsp_reg[i] = devm_ioremap_resource(&pdev->dev, mem);
        if (IS_ERR((void *)g_svp_dsp_reg[i])) {
            printk("Line:%d,Func:%s,CoreId(%u)\n", __LINE__, __FUNCTION__, i);

            return PTR_ERR((const void *)g_svp_dsp_reg[i]);
        }
        /* do not must get irq */
    }

    return svp_dsp_std_mod_init();
}

static int hi35xx_svp_dsp_remove(struct platform_device *pdev)
{
    unsigned int i;
    svp_dsp_std_mod_exit();

    for (i = 0; i < SVP_DSP_ID_BUTT; i++) {
        g_svp_dsp_reg[i] = HI_NULL;
    }
    return 0;
}

static const struct of_device_id hi35xx_svp_dsp_match[] = {
    { .compatible = "hisilicon,hisi-dsp" },
    {},
};
MODULE_DEVICE_TABLE(of, hi35xx_svp_dsp_match);

static struct platform_driver hi35xx_svp_dsp_driver = {
    .probe = hi35xx_svp_dsp_probe,
    .remove = hi35xx_svp_dsp_remove,
    .driver = {
        .name = "hi35xx_dsp",
        .of_match_table = hi35xx_svp_dsp_match,
    },
};

osal_module_platform_driver(hi35xx_svp_dsp_driver);

MODULE_LICENSE("Proprietary");

