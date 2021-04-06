
ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../Makefile.param
    include $(PARAM_FILE)
endif


obj-m += $(HIARCH)_pciv.o
$(HIARCH)_pciv-y := pciv.o pciv_ext.o pciv_drvadp.o

ifeq ($(HIGDB),HI_GDB)
EXTRA_CFLAGS += -g
endif

EXTRA_CFLAGS += $(MPP_CFLAGS)
#EXTRA_CFLAGS += -Wall -D$(HIARCH) -D$(HIDBG) -D$(HI_FPGA)
EXTRA_CFLAGS += -Wall -D$(HIARCH) -D$(HI_FPGA)
EXTRA_CFLAGS += -I$(REL_INC) -I$(PWD)/../../include -I$(PWD)/../../include/adapt

all:
	@echo "EXTRA_CFLAGS = $(EXTRA_CFLAGS)"
	@make -C $(KERNEL_ROOT) M=$(PWD) modules
	@cp $(HIARCH)_pciv.ko $(REL_KO)/

clean:
	@make -C $(KERNEL_ROOT) M=$(PWD) clean
	@rm -rf $(REL_KO)/$(HIARCH)_pciv.ko
