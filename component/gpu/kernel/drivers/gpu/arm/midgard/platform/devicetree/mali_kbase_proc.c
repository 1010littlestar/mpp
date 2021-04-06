#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/kernel.h>
/*#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#endif*/
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/clkdev.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>


#include <mali_kbase.h>
#include <mali_kbase_defs.h>
#include <mali_kbase_config.h>
#include <mali_kbase_pm.h>
#include <mali_kbase_config_platform.h>

#include "mali_kbase_proc_helper.h"
#include "mali_kbase_proc.h"
#include "mali_kbase_runtime_pm.h"

#include "hi_osal.h"

#define GPU_CMD_MAX_NUM (2)
#define GPU_CMD_MAX_CMD_LEN (32)
#define GPU_CMD_MAX_VLAUE_LEN (32)
#define GPU_CMD_AVS "avs"
#define GPU_CMD_DVFS "dvfs"
#define GPU_CMD_ON "on"
#define GPU_CMD_OFF "off"
#define GPU_CMD_VOLT "volt"
#define GPU_CMD_FREQ "freq"
#define GPU_CMD_HELP "help"
#define GPU_CMD_WAKEUPRESET "reset"
#define GPU_CMD_DEBUG "debug"

typedef struct tagGPU_CMD_DATA_S
{
	char aszCmd[GPU_CMD_MAX_CMD_LEN];
	char aszValue[GPU_CMD_MAX_VLAUE_LEN];
} GPU_CMD_DATA_S;

static GPU_CMD_DATA_S g_astGPUProcCmd[GPU_CMD_MAX_NUM];

static int GPUParseValue(char* pu8Value)
{
	if (strlen(pu8Value) < 2)
	{
		return (unsigned int)osal_strtoul(pu8Value, NULL, 10);
	}
	else
	{
		if ((pu8Value[0] == '0') && ((pu8Value[1] == 'x') || (pu8Value[1] == 'X')))
		{
			return (unsigned int)osal_strtoul(pu8Value, NULL, 16);
		}
		else
		{
			return (unsigned int)osal_strtoul(pu8Value, NULL, 10);
		}
	}
}

static int GPUParseCmd(char* pu8Cmd, unsigned int u32Len, GPU_CMD_DATA_S** pstCmd, unsigned int* pu32Num)
{
	unsigned int u32Pos = 0;
	unsigned int u32SavePos = 0;
	unsigned int u32CmdNum = 0;
	bool bIsCmd   = true;
	char u8LastChar = ' ';

	if ((NULL == pu8Cmd) || (0 == u32Len) || (NULL == pstCmd) || (NULL == pu32Num))
	{
		printk(KERN_ERR "Invalid.\n");
		return -1;
	}

	u32CmdNum = 0;
	memset(g_astGPUProcCmd, 0, sizeof(g_astGPUProcCmd));

	while ((u32Pos < u32Len) && (pu8Cmd[u32Pos]))
	{
		switch (pu8Cmd[u32Pos])
		{
			case '\0':
			case '\n':
			case '\t':
				break;

			case '=':
				if (bIsCmd)
				{
				    bIsCmd = false;
				    u32SavePos = 0;
				}
				break;
			case ' ':
				if (!((' ' == u8LastChar) || ('=' == u8LastChar)))
				{
					bIsCmd = !bIsCmd;
					u32SavePos = 0;
					if (bIsCmd)
					{
						if (u32CmdNum < GPU_CMD_MAX_NUM - 1)
						{
 							u32CmdNum++;
   						}
   						else
						{
							goto out;
						}
					}
				}
				break;
			default:
				if (bIsCmd)
				{
					if (u32SavePos < GPU_CMD_MAX_CMD_LEN)
					{
						g_astGPUProcCmd[u32CmdNum].aszCmd[u32SavePos++] = pu8Cmd[u32Pos];
 					}
				}
				else
				{
					if (u32SavePos < GPU_CMD_MAX_VLAUE_LEN)
					{
						g_astGPUProcCmd[u32CmdNum].aszValue[u32SavePos++] = pu8Cmd[u32Pos];
					}
				}
				break;
		}

		u8LastChar = pu8Cmd[u32Pos];
		u32Pos++;
	}

out:
	if (strlen(g_astGPUProcCmd[u32CmdNum].aszCmd) > 0)
	{
		u32CmdNum += 1;
	}

	*pstCmd  = g_astGPUProcCmd;
	*pu32Num = u32CmdNum;
	return 0;
}

static int GPUDebugCtrl(unsigned int u32Para1, unsigned int u32Para2)
{
	if ((0 == u32Para1) && (0 == u32Para2))
	{
		printk(KERN_ERR "plese set valid value \n");
		return 0;
	}

	if (0 == u32Para2)
	{
		kbase_clk_set(u32Para1);
		udelay(100);
		/* printk("\nFrequency set to %d!\n", kbase_clk_get()); */

		return 0;
	}

	if (0 == u32Para1)
	{
		kbase_regulator_set(u32Para2);
		/* printk("\nVoltage set to %d!\n", kbase_regulator_get()); */

		return 0;
	}

	kbase_clk_set(u32Para1);
	kbase_regulator_set(u32Para2);

	return 0;
}

static int GPUProcRead(osal_proc_entry_t* p)
{
	osal_seq_printf(p, "---------Hisilicon GPU Info---------\n");
	osal_seq_printf(p, "Frequency			:%lu(kHz)\n", kbase_clk_get());
	osal_seq_printf(p, "Voltage				:%d(mv)\n", kbase_regulator_get());
	osal_seq_printf(p, "Utilization			:%d(%%)\n", kbase_get_utilisation());

	if (1 == kbase_power_status())
	{
		osal_seq_printf(p, "Power_status			:power up\n");
	}
	else
	{
		osal_seq_printf(p, "Power_status			:power down\n");
	}

	if (1 == kbase_dvfs_status())
	{
		osal_seq_printf(p, "DVFS_status			:on\n");
	}
	else
	{
		osal_seq_printf(p, "DVFS_status			:off\n");
	}

	if (1 == kbase_debug_status())
	{
		osal_seq_printf(p, "Debug_status			:on\n");
	}
	return 0;
}

static void GPUProcHelper(void)
{
    printk(
        "echo volt 800 > /proc/msp/pm_gpu, set gpu volt in mv.\n"
        "echo freq 400000 > /proc/msp/pm_gpu, set gpu freq in kHz.\n"
        "echo dvfs on/off > /proc/msp/pm_gpu, open/close gpu dvfs.\n"
    );

    return;
}

static int GPUProcWrite(osal_proc_entry_t *p, const char * buf, int count, long long * ppos)
{
	char ProcPara[64] = {0};
	int s32Ret;
	unsigned int u32CmdNum = 0;
	GPU_CMD_DATA_S* pstCmd = NULL;

	if(count > sizeof(ProcPara))
	{
		return -EFAULT;
	}

	if (copy_from_user(ProcPara, buf, count))
	{
		return -EFAULT;
	}
	s32Ret = GPUParseCmd(ProcPara, count, &pstCmd, &u32CmdNum);
	if (0 != s32Ret)
	{
		goto err;
	}
	if (1 == u32CmdNum)
	{
		/* Only set GPU volt */
		if (0 == osal_strncasecmp(GPU_CMD_VOLT, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
		{
			GPUDebugCtrl(0, GPUParseValue(pstCmd[0].aszValue));
		}
		/* Only set GPU freq */
		else if (0 == osal_strncasecmp(GPU_CMD_FREQ, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
		{
			GPUDebugCtrl(GPUParseValue(pstCmd[0].aszValue), 0);
		}
		else if (0 == osal_strncasecmp(GPU_CMD_DVFS, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
		{
		/* GPU DVFS ON */
			if (0 == osal_strncasecmp(GPU_CMD_ON, pstCmd[0].aszValue, strlen(pstCmd[0].aszCmd)))
			{
				kbase_dvfs_enable(1);
				printk("DVFS enabled!\n");
			}
		/* GPU DVFS OFF */
			else if (0 == osal_strncasecmp(GPU_CMD_OFF, pstCmd[0].aszValue, strlen(pstCmd[0].aszCmd)))
			{
				kbase_dvfs_enable(0);
				printk("DVFS disabled!\n");
			}
		}
		else if(0 == osal_strncasecmp(GPU_CMD_DEBUG, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
		{
		/* GPU DEBUG ON */
			if (0 == osal_strncasecmp(GPU_CMD_ON, pstCmd[0].aszValue, strlen(pstCmd[0].aszCmd)))
			{
				kbase_debug_enable(1);
				printk("DEBUG enabled!\n");
			}
		/* GPU DEBUG OFF */
			else if (0 == osal_strncasecmp(GPU_CMD_OFF, pstCmd[0].aszValue, strlen(pstCmd[0].aszCmd)))
			{
				kbase_debug_enable(0);
				printk("DEBUG disabled!\n");
			}
		}

	/* Support 0xXXX 0xXXX command */
		else /*if (('0' == pstCmd[0].aszCmd[0]) && ('0' == pstCmd[0].aszValue[0]))*/
		{
			GPUDebugCtrl(GPUParseValue(pstCmd[0].aszCmd), GPUParseValue(pstCmd[0].aszValue));
		}

	}

	else if (2 == u32CmdNum)
	{
		if ((0 == osal_strncasecmp(GPU_CMD_VOLT, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
			&& (0 == osal_strncasecmp(GPU_CMD_FREQ, pstCmd[1].aszCmd, strlen(pstCmd[1].aszCmd))))
		{
			GPUDebugCtrl(GPUParseValue(pstCmd[1].aszValue), GPUParseValue(pstCmd[0].aszValue));
		}
		else if ((0 == osal_strncasecmp(GPU_CMD_FREQ, pstCmd[0].aszCmd, strlen(pstCmd[0].aszCmd)))
			&& (0 == osal_strncasecmp(GPU_CMD_VOLT, pstCmd[1].aszCmd, strlen(pstCmd[1].aszCmd))))
		{
			GPUDebugCtrl(GPUParseValue(pstCmd[0].aszValue), GPUParseValue(pstCmd[1].aszValue));
		}
		else
		{
			goto err;
		}
	}
	else
	{
		goto err;
	}

	return count;

err:
	printk("Invaid parameter.\n");
	GPUProcHelper();
	return count;
}

int kbase_proc_create(void)
{
	osal_proc_entry_t *proc = NULL;

	proc = osal_create_proc_entry("pm_gpu", NULL);
	if (!proc)
	{
		printk(KERN_ERR "Create GPU proc module failed\n");
		return -1;
	}

	proc->read = GPUProcRead;
	proc->write = GPUProcWrite;

	return 0;
}

int kbase_proc_destroy(void)
{
	osal_remove_proc_entry("pm_gpu", NULL);

	return 0;
}
