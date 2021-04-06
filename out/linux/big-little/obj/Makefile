#*******************************************************************************
#The rule of make Linux kernel module
#*******************************************************************************
ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

EXTRA_CFLAGS += $(MPP_CFLAGS)
EXTRA_CFLAGS += -I$(REL_INC)
EXTRA_CFLAGS += -I$(REL_INC)/adapt
EXTRA_CFLAGS += -I$(OSAL_ROOT)/include
EXTRA_CFLAGS += -I$(MPP_PATH)/component/isp/ext_inc
EXTRA_CFLAGS += -I$(MPP_PATH)/component/isp/include

$(HIARCH)_base-objs = hi_base.o  ../init/base_init.o
$(HIARCH)_dsp-objs  = hi_dsp.o  ../init/dsp_init.o
$(HIARCH)_ive-objs  = hi_ive.o    ../init/ive_init.o
$(HIARCH)_nnie-objs = hi_nnie.o   ../init/nnie_init.o
$(HIARCH)_photo-objs = photo_drv.o  ../init/photo_init.o
$(HIARCH)_sys-objs = hi_sys.o    ../init/sys_init.o

obj-m += $(HIARCH)_base.o
obj-m += $(HIARCH)_dsp.o
obj-m += $(HIARCH)_ive.o
obj-m += $(HIARCH)_nnie.o
obj-m += $(HIARCH)_photo.o
obj-m += $(HIARCH)_sys.o

default:
	@echo -e "\e[0;32;1m--Compiling '$(TARGET)'... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS) REL_INC=$(REL_INC)
	@echo ---- HIARCH=$(HIARCH), HICHIP=$(HICHIP), CVER=$(CVER), DBG=$(HIDBG), HI_FPGA=$(HI_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE) REL_KO=$(REL_KO)
	@echo ---- MPP_CFLAGS=$(MPP_CFLAGS)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@echo ---- KERNEL_ROOT=$(KERNEL_ROOT)
	@echo ---- ARCH_ROOT=$(ARCH_ROOT), ARCH_HAL=$(ARCH_HAL)
	@@echo -e "\e[0m"
	@$(MAKE) -C $(KERNEL_ROOT) M=$(PWD) V=10 modules -j 24
	@mkdir -p $(REL_KO) && cp -rf *.ko $(REL_KO)

clean:
	@rm -f *.ko
	@rm .tmp_versions  -rf
	@rm Module.symvers -f
	@rm modules.order  -f
	@rm .*.cmd  -f
	@rm *.mod.c  -f
	@rm hi*.mod.o  -f
	@rm hi3519av100_*.o  -f
	@rm ../init/*.o  -f
	@rm ../init/.*.cmd  -f
