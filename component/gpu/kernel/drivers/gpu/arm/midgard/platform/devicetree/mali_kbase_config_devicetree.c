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

#include <mali_kbase_config.h>

#include <mali_kbase_proc.h>
#include "mali_kbase_runtime_pm.h"

static int platform_callback_init(struct kbase_device *kbdev)
{
	//TODO: This clk enable and disable part should be done in kernel.
	//Driver uses clk_enable and clk_disable to call the implemetation in kernel.

	kbase_clk_enable();


	kbase_clk_pll_init();

	kbase_proc_create();
	return 0;
}

static void platform_callback_term(struct kbase_device *kbdev)
{
	kbase_proc_destroy();

	//TODO: This clk enable and disable part should be done in kernel.
	//Driver uses clk_enable and clk_disable to call the implemetation in kernel.

	kbase_clk_disable();

	kbase_clk_pll_deinit();
}

struct kbase_platform_funcs_conf platform_callbacks = {
	.platform_init_func = platform_callback_init,
	.platform_term_func = platform_callback_term
};

static struct kbase_platform_config dummy_platform_config;

struct kbase_platform_config *kbase_get_platform_config(void)
{
	return &dummy_platform_config;
}

#ifndef CONFIG_OF
int kbase_platform_register(void)
{
	return 0;
}

void kbase_platform_unregister(void)
{
}
#endif
