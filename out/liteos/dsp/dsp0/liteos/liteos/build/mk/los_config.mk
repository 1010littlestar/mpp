CC = $(DSP_XTENSA_COMPILE)/xt-xcc
AS = $(DSP_XTENSA_COMPILE)/xt-as
AR = $(DSP_XTENSA_COMPILE)/xt-ar
LD = $(DSP_XTENSA_COMPILE)/xt-ld
GPP = $(DSP_XTENSA_COMPILE)/xt-xc++
OBJCOPY = $(DSP_XTENSA_COMPILE)/xt-objcopy
OBJDUMP = $(DSP_XTENSA_COMPILE)/xt-objdump
DUMPELF = $(DSP_XTENSA_COMPILE)/xt-dumpelf

## c as cxx ld options ##
LITEOS_ASOPTS :=
LITEOS_COPTS_BASE :=
LITEOS_COPTS_EXTRA :=
LITEOS_COPTS :=
LITEOS_LD_OPTS :=
LITEOS_CXXOPTS :=
LITEOS_CXXOPTS_BASE :=
## macro define ##
LITEOS_CMACRO :=
LITEOS_CXXMACRO :=
## head file path and ld path ##
LITEOS_INCLUDE :=
LITEOS_CXXINCLUDE :=
LITEOS_LD_PATH :=
## c as cxx ld flags ##
LITEOS_ASFLAGS :=
LITEOS_CFLAGS :=
LITEOS_LDFLAGS :=
LITEOS_CXXFLAGS :=
## depended lib ##
LITEOS_BASELIB :=
LITEOS_LIBDEP :=
## directory ##
LIB_BIGODIR :=
LIB_SUBDIRS :=

## variable define ##
OUT = $(LITEOSTOPDIR)/out/dsp
BUILD = $(OUT)/obj
MK_PATH = $(LITEOSTOPDIR)/build/mk
LITEOS_LIB_BIGODIR = $(OUT)/lib/

AS_OBJS_LIBC_FLAGS  = -D__ASSEMBLY__

LITEOS_KERNEL_INCLUDE	:= -I $(LITEOSTOPDIR)/kernel/include \
			   -I $(LITEOSTOPDIR)/kernel/base/include \
                           -I $(LITEOSTOPDIR)/kernel/extended/include
LIB_SUBDIRS += kernel/base kernel/extended/cpup
LITEOS_BASELIB += -llitekernel -lcpup

LITEOS_PLATFORM_INCLUDE := -I $(LITEOSTOPDIR)/platform/cpu/include \
			   -I $(LITEOSTOPDIR)/platform/cpu/IVP32_HHS_K3V5_P6/include
LIB_SUBDIRS += platform/cpu/IVP32_HHS_K3V5_P6
LITEOS_BASELIB += -ldspcore

LITEOS_PLATFORM_INCLUDE += -I $(LITEOSTOPDIR)/platform/bsp/common \
			   -I $(LITEOSTOPDIR)/platform/bsp/IVP32/arch/$(HIARCH)/include \
			   -I $(LITEOSTOPDIR)/platform/bsp/IVP32 \
			   -I $(LITEOSTOPDIR)/platform/bsp/IVP32/config
LIB_SUBDIRS += platform/bsp/IVP32
LIB_SUBDIRS += platform/bsp/common
LITEOS_BASELIB += -ldsp
LITEOS_BASELIB += -lbspcommon

LITEOS_CMSIS_INCLUDE	:= -I $(LITEOSTOPDIR)/compat/cmsis
LIB_SUBDIRS += compat/cmsis
LITEOS_BASELIB += -lcmsis

LITEOS_POSIX_INCLUDE	:= -I $(LITEOSTOPDIR)/compat/posix
LIB_SUBDIRS += compat/posix
LITEOS_BASELIB += -lposix

LITEOS_UART_INCLUDE 	:= -I $(LITEOSTOPDIR)/drivers/uart/include

LITEOS_SHELL_INCLUDE 	:= -I $(LITEOSTOPDIR)/shell/include
LIB_SUBDIRS += shell
LITEOS_BASELIB += -lshell

############################# Tools && Debug Option Begin ##############################
ifeq ($(LOSCFG_COMPILE_DEBUG), y)
    LITEOS_ASOPTS   += -g -gdwarf-2
    LITEOS_COPTS    += -g -gdwarf-2 -O0 -Wall
    LITEOS_CXXOPTS  += -g -gdwarf-2 -O0 -Wall
else
    LITEOS_COPTS    += -O2
    LITEOS_CXXOPTS  += -O2 -Wall
endif

LITEOS_COMPAT_INCLUDE   := $(LITEOS_CMSIS_INCLUDE) $(LITEOS_POSIX_INCLUDE)
LITEOS_DRIVERS_INCLUDE  := $(LITEOS_UART_INCLUDE)


LITEOS_COPTS_EXTRA  	:= -std=c99 -O2 -mlongcalls -ffunction-sections -fdata-sections

LITEOS_LD_OPTS += -nostartfiles -static --gc-sections
LITEOS_LD_PATH += -L$(OUT)/lib -L$(LITEOS_LIB_BIGODIR)
