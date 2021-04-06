/*
 *
 * (C) COPYRIGHT 2015, 2017 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
 *
 * SPDX-License-Identifier: GPL-2.0
 *
 */

#include <mali_kbase.h>
#include <mali_kbase_defs.h>
#include <linux/pm_runtime.h>
#include "mali_kbase_config_platform.h"


static int s_power_state = 0;

#define KBASE_HISI_GPU_REST 	0x120100d8
static volatile u32* g_GPUReset = NULL;

#define KBASE_HISI_GPU_CLK      0x120100e4
static volatile u32 * g_clk_mask = NULL;

int kbase_clk_enable(void)
{
	u32 gpuReset;

	if(NULL == g_GPUReset)
	{
		g_GPUReset = (volatile u32*)ioremap_nocache(KBASE_HISI_GPU_REST,  32);
	}

	gpuReset = *g_GPUReset;

	/* gpu clk enable */
	*g_GPUReset = gpuReset | 0x13;

	/* gpu cancel reset */
	gpuReset = *g_GPUReset;
	*g_GPUReset = gpuReset & 0xfffffffc;

	return 0;
}

void kbase_clk_disable(void)
{
	u32 gpuReset;

	if(NULL == g_GPUReset)
	{
		g_GPUReset = (volatile u32*)ioremap_nocache(KBASE_HISI_GPU_REST,  32);
	}

	gpuReset = *g_GPUReset;

	/* gpu reset */
	*g_GPUReset = gpuReset | 0x10;

	/* gpu disable */
	gpuReset = *g_GPUReset;
	*g_GPUReset = gpuReset & 0xffffffef;
}

void kbase_clk_pll_init(void)
{
	/* switch gpu clk to be pll */
	if(NULL == g_clk_mask)
	{
		g_clk_mask = (volatile u32*)ioremap_nocache(KBASE_HISI_GPU_CLK,  32);
	}

	*g_clk_mask |= 0x60000;
}

void kbase_clk_pll_deinit(void)
{
	if(NULL != g_clk_mask)
	{
		iounmap(g_clk_mask);
	}
}

int kbase_power_status(void)
{
	return s_power_state;
}

static int pm_callback_power_on(struct kbase_device *kbdev)
{
	int ret = 1; /* Assume GPU has been powered off */
	int error;

	dev_dbg(kbdev->dev, "pm_callback_power_on %p\n",
			(void *)kbdev->dev->pm_domain);

	error = pm_runtime_get_sync(kbdev->dev);
	if (error == 1) {
		/*
		 * Let core know that the chip has not been
		 * powered off, so we can save on re-initialization.
		 */
		ret = 0;
	}

	dev_dbg(kbdev->dev, "pm_runtime_get_sync returned %d\n", error);

	s_power_state = 1;
	return ret;
}

static void pm_callback_power_off(struct kbase_device *kbdev)
{
	dev_dbg(kbdev->dev, "pm_callback_power_off\n");

	pm_runtime_mark_last_busy(kbdev->dev);
	pm_runtime_put_autosuspend(kbdev->dev);
	s_power_state = 0;
}

#ifdef KBASE_PM_RUNTIME
static int kbase_device_runtime_init(struct kbase_device *kbdev)
{
	int ret = 0;

	dev_dbg(kbdev->dev, "kbase_device_runtime_init\n");

	pm_runtime_set_autosuspend_delay(kbdev->dev, AUTO_SUSPEND_DELAY);
	pm_runtime_use_autosuspend(kbdev->dev);

	pm_runtime_set_active(kbdev->dev);
	pm_runtime_enable(kbdev->dev);

	if (!pm_runtime_enabled(kbdev->dev)) {
		dev_warn(kbdev->dev, "pm_runtime not enabled");
		ret = -ENOSYS;
	}

	return ret;
}

static void kbase_device_runtime_disable(struct kbase_device *kbdev)
{
	dev_dbg(kbdev->dev, "kbase_device_runtime_disable\n");
	pm_runtime_disable(kbdev->dev);
}
#endif

static int pm_callback_runtime_on(struct kbase_device *kbdev)
{
	dev_dbg(kbdev->dev, "pm_callback_runtime_on\n");

	return 0;
}

static void pm_callback_runtime_off(struct kbase_device *kbdev)
{
	dev_dbg(kbdev->dev, "pm_callback_runtime_off\n");
}

static void pm_callback_resume(struct kbase_device *kbdev)
{
	int ret;

#ifdef CONFIG_HAVE_CLK

	kbase_clk_enable();

#endif

	ret = pm_callback_runtime_on(kbdev);

	WARN_ON(ret);
}

static void pm_callback_suspend(struct kbase_device *kbdev)
{
	pm_callback_runtime_off(kbdev);
#ifdef CONFIG_HAVE_CLK

	kbase_clk_disable();

#endif
}

struct kbase_pm_callback_conf pm_callbacks = {
	.power_on_callback = pm_callback_power_on,
	.power_off_callback = pm_callback_power_off,
	.power_suspend_callback = pm_callback_suspend,
	.power_resume_callback = pm_callback_resume,
#ifdef KBASE_PM_RUNTIME
	.power_runtime_init_callback = kbase_device_runtime_init,
	.power_runtime_term_callback = kbase_device_runtime_disable,
	.power_runtime_on_callback = pm_callback_runtime_on,
	.power_runtime_off_callback = pm_callback_runtime_off,
#else				/* KBASE_PM_RUNTIME */
	.power_runtime_init_callback = NULL,
	.power_runtime_term_callback = NULL,
	.power_runtime_on_callback = NULL,
	.power_runtime_off_callback = NULL,
#endif				/* KBASE_PM_RUNTIME */
};


