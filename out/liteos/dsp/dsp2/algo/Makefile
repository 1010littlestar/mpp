CPU_TYPE=multi-core
ifeq ($(PARAM_FILE), )
PARAM_FILE:=../../../../../Makefile.param
include $(PARAM_FILE)
endif
ifeq ($(PARAMFILE),)
    PARAMFILE:= ../Makefile.param
    include $(PARAMFILE)
endif

TM_DIR := $(DSP_PATH)/tm
ALGO_DIR := $(DSP_PATH)/algo
DSP_LIB_DIR := $(DSP_PATH)/lib
DSP_ARCH_DIR := $(DSP_PATH)/arch/dsp$(DSP_ID)

# The head file path
CFG_INC = -I $(DSP_PATH)/include \
		-I $(DSP_ARCH_DIR)/include \
		-I $(ALGO_DIR)/include/ext \
		-I $(ALGO_DIR)/include/int \
		-I $(DSP_LIB_DIR)/include \
		-I $(TM_DIR)/include \
		-I $(DSP_PATH)/../include \
		-I $(REL_INC) \

KERNEL_SRC = $(wildcard ./src/*.c)

KERNEL_OBJS = $(KERNEL_SRC:%.c=%.o)

LIB_A := libdsp_algo.a

CFLAGS += $(CFG_INC)
CFLAGS += -DDSP_ID=$(DSP_ID)

COMPILE = $(CC) --xtensa-core=$(DSP_CORE) --xtensa-system=$(DSP_XTENSA_SYSTEM) $(CFLAGS) -Wall -fgnu89-inline

%.o: %.c
	@$(COMPILE) -o $@ -c $< $(CFG_INC)

#*******************************************************************************
.PHONY: all dsp_lib clean

all: kernel_lib


kernel_lib:$(KERNEL_OBJS)
	@$(AR) rvc $(LIB_A) $(KERNEL_OBJS)

clean:
	@rm -rf $(LIB_A)
	@rm -rf $(KERNEL_OBJS)

