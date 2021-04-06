#ifndef __MALI_KBASE_RUNTIME_PM_H__
#define __MALI_KBASE_RUNTIME_PM_H__

#ifdef __cplusplus
extern "C" {
#endif

int kbase_clk_enable(void);
void kbase_clk_disable(void);
void kbase_clk_pll_init(void);
void kbase_clk_pll_deinit(void);

int kbase_power_status(void);

#ifdef __cplusplus
}
#endif
#endif
