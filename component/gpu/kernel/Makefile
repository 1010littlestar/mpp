

ifeq ($(PARAM_FILE), ) 
    PARAM_FILE:=../../../Makefile.param
    include $(PARAM_FILE)
endif

export CUR_DIR=$(shell pwd)
export KDIR?=$(KERNEL_ROOT)
export SDK_DIR=$(SDK_PATH)

##########################################################################################
#	set task
##########################################################################################
all:
	make -C $(CUR_DIR)/drivers/base/hi_dbe
	make -C $(CUR_DIR)/drivers/gpu/arm/midgard

	cp $(CUR_DIR)/drivers/base/hi_dbe/hi_dbe.ko $(CUR_DIR)/../release/ko/
	cp $(CUR_DIR)/drivers/gpu/arm/midgard/mali_kbase.ko $(CUR_DIR)/../release/ko/

clean:
	make -C $(CUR_DIR)/drivers/base/hi_dbe/ clean
	make -C $(CUR_DIR)/drivers/gpu/arm/midgard/ clean

	-@rm $(CUR_DIR)/../release/ko/hi_dbe.ko
	-@rm $(CUR_DIR)/../release/ko/mali_kbase.ko
