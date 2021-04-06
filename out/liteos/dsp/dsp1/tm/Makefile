ifeq ($(PARAM_FILE), )
PARAM_FILE:=../../../../../Makefile.param
include $(PARAM_FILE)
endif
ifeq ($(PARAMFILE),)
    PARAMFILE:= ../Makefile.param
    include $(PARAMFILE)
endif

TM_DIR := $(DSP_PATH)/tm
DSP_LIB_DIR := $(DSP_PATH)/lib

# The head file path
CFG_INC = -I $(TM_DIR)/include \
		  -I $(DSP_LIB_DIR)/include \
		  -I $(REL_INC)

TM_SRC = $(wildcard ./src/*.c)

TM_OBJS = $(TM_SRC:%.c=%.o)

LIB_A := libdsp_tm.a

CFLAGS += $(CFG_INC)
CFLAGS += -DDSP_ID=$(DSP_ID)

COMPILE = $(CC) --xtensa-core=$(DSP_CORE) --xtensa-system=$(DSP_XTENSA_SYSTEM) $(CFLAGS) -Wall -fgnu89-inline

%.o: %.c
	@$(COMPILE) -o $@ -c $< $(CFG_INC)

#*******************************************************************************
.PHONY: all tm_lib clean

all: tm_lib

tm_lib:$(TM_OBJS)
	@$(AR) rvc $(LIB_A) $(TM_OBJS)

clean:
	@rm -rf $(LIB_A)
	@rm -rf $(TM_OBJS)

