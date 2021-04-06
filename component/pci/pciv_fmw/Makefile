# $(ROOT)/mpp/code/component/pci/pciv_fmw/Makefile

ifeq ($(PARAM_FILE), ) 
    PARAM_FILE:=../../../Makefile.param
    include $(PARAM_FILE)
endif

MPP_PATH := $(SDK_PATH)/mpp

INC_PATH := -I$(MPP_PATH)/component/pci/include -I$(MPP_PATH)/component/pci/pciv/kernel -I$(MPP_PATH)/component/pci/include/adapt -I$(REL_INC)

obj-m += $(HIARCH)_pciv_fmw.o
$(HIARCH)_pciv_fmw-y := pciv_firmware.o pciv_pic_queue.o

obj-m += $(HIARCH)_pciv_fmw.o
$(HIARCH)_pciv_fmw-y := pciv_firmware.o pciv_pic_queue.o

EXTRA_CFLAGS += $(INC_PATH) -I.
ifeq ($(HIGDB),HI_GDB)
EXTRA_CFLAGS += -g
endif

EXTRA_CFLAGS += $(MPP_CFLAGS)
#EXTRA_CFLAGS += -Wall -D$(HIARCH) -D$(HIDBG) -D$(HI_FPGA)
EXTRA_CFLAGS += -Wall -D$(HIARCH) -D$(HI_FPGA)

all:
	@echo "EXTRA_CFLAGS = $(EXTRA_CFLAGS)"
	make -C $(KERNEL_ROOT) M=$(PWD) modules
	cp $(HIARCH)_pciv_fmw.ko $(REL_KO)/

clean:
	make -C $(KERNEL_ROOT) M=$(PWD) clean
	rm -f $(REL_KO)/$(HIARCH)_pciv_fmw*.ko
