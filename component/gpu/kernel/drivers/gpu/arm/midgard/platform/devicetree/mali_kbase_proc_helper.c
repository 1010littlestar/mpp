#undef CONFIG_REGULATOR

#include <linux/of.h>
#include <linux/of_platform.h>
#ifdef CONFIG_DEVFREQ_THERMAL
#include <linux/devfreq_cooling.h>
#endif
#include <linux/thermal.h>
#ifdef CONFIG_PM_DEVFREQ
#include <linux/devfreq.h>
#endif

#include <mali_kbase.h>

#define UNIT    1000

struct kbase_device *s_pkbdev = NULL;

static int s_dvfs_enable = 1;
static int s_debug_enable = 0;

#define DEVICE_ININT_CHECK \
    do {\
        if ( NULL == s_pkbdev) {\
            kbase_get_device();\
        }\
    }while(0)

int kbase_get_device(void)
{
#ifdef CONFIG_OF
	struct device_node *np;
	struct platform_device *pdev;

	np = of_find_node_by_name(NULL, "gpu");
	if (!np) {
		pr_err("Failed to find DT entry for Mali\n");
		return -EFAULT;
	}

	pdev = of_find_device_by_node(np);
	if (!pdev) {
		pr_err("Failed to find device for Mali\n");
		of_node_put(np);
		return -EFAULT;
	}

	s_pkbdev = dev_get_drvdata(&pdev->dev);

	of_node_put(np);
#endif
	return 0;
}


int kbase_regulator_get(void)
{
#ifdef CONFIG_REGULATOR
	int voltage;
	DEVICE_ININT_CHECK;

	if (s_pkbdev->regulator) {
		voltage = regulator_get_voltage(s_pkbdev->regulator);
		return voltage/UNIT;
	}
#endif
    return 0;
}

int kbase_regulator_set(int voltage)
{
	int ret = 0;
#ifdef CONFIG_REGULATOR
	DEVICE_ININT_CHECK;
	if (s_pkbdev->regulator)
	{
		if(s_debug_enable == 1)
		{
			ret = regulator_set_voltage(s_pkbdev->regulator, voltage * UNIT, voltage * UNIT);
		}
		else
		{
			printk("Voltage can only be set in debug mode!\n");
		}
	}
#endif
	return ret;
}

unsigned long kbase_clk_get(void)
{
#ifdef CONFIG_HAVE_CLK
	unsigned long freq;
	DEVICE_ININT_CHECK;
	freq = clk_get_rate(s_pkbdev->clock)/UNIT;
	return freq;
#else
	return 0;
#endif
}

int kbase_clk_set(unsigned long freq)
{
#ifdef CONFIG_PM_DEVFREQ
	int i = 0;
	unsigned long current_freq;
    int is_valid = 0;
	DEVICE_ININT_CHECK;
	current_freq = s_pkbdev->current_freq;

	freq *= UNIT;
	for(i = 0; i < s_pkbdev->devfreq_profile.max_state; i++) {
		if(freq == s_pkbdev->devfreq_profile.freq_table[i]) {
			is_valid = 1;
		}
	}

	if(0 == is_valid){
		return 0;
	}

	for(i = 0; i < s_pkbdev->devfreq_profile.max_state; i++) {
		if(freq != s_pkbdev->devfreq_profile.freq_table[i]) {
			if(s_pkbdev->current_freq != s_pkbdev->devfreq_profile.freq_table[i]) {
				dev_pm_opp_disable(s_pkbdev->dev, s_pkbdev->devfreq_profile.freq_table[i]);
			}
		}
		else {
			dev_pm_opp_enable(s_pkbdev->dev, s_pkbdev->devfreq_profile.freq_table[i]);
		}
	}
	if(current_freq !=  freq)  {
		dev_pm_opp_disable(s_pkbdev->dev, current_freq);
	}
	s_dvfs_enable = 0;
#endif
	return 0;
}

int kbase_dvfs_enable(int enable)
{
#ifdef CONFIG_PM_DEVFREQ
	int i = 0;
	DEVICE_ININT_CHECK;

	for(i = 0; i < s_pkbdev->devfreq_profile.max_state; i++) {
		if(enable) {
			dev_pm_opp_enable(s_pkbdev->dev, s_pkbdev->devfreq_profile.freq_table[i]);
		}
		else {
			if(s_pkbdev->current_freq != s_pkbdev->devfreq_profile.freq_table[i]) {
				dev_pm_opp_disable(s_pkbdev->dev, s_pkbdev->devfreq_profile.freq_table[i]);
			}
			else {
				dev_pm_opp_enable(s_pkbdev->dev, s_pkbdev->devfreq_profile.freq_table[i]);
			}
		}
	}

	s_dvfs_enable = enable;
#endif
	return 0;
}

int kbase_dvfs_status(void)
{
	return s_dvfs_enable;
}

int kbase_debug_enable(int enable)
{
	s_debug_enable = enable;

	return 0;
}

int kbase_debug_status(void)
{
	return s_debug_enable;
}

int kbase_get_utilisation(void)
{
#ifdef CONFIG_PM_DEVFREQ
	unsigned int utilisation = 0;
	struct devfreq_dev_status stat;

	s_pkbdev->devfreq_profile.get_dev_status(s_pkbdev->dev, &stat);
	utilisation = stat.busy_time * 100/stat.total_time;
	return utilisation;
#else
	return 0;
#endif
}
