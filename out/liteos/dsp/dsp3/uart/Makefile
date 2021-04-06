ifeq ($(PARAM_FILE), )
PARAM_FILE:=../../../../../Makefile.param
include $(PARAM_FILE)
endif
ifeq ($(PARAMFILE),)
    PARAMFILE:= ../Makefile.param
    include $(PARAMFILE)
endif

# The head file path
CFG_INC = -I $(DSP_PATH)/uart/include \

UART_SRC = $(wildcard ./src/*.c)
UART_OBJS = $(UART_SRC:%.c=%.o)

LIB_A := libdsp_uart.a

CFLAGS += $(CFG_INC)

COMPILE = $(CC) --xtensa-core=$(DSP_CORE) --xtensa-system=$(DSP_XTENSA_SYSTEM) $(CFLAGS) -Wall -fgnu89-inline

%.o: %.c
	@$(COMPILE) -o $@ -c $< $(CFG_INC)

#*******************************************************************************
.PHONY: all uart_lib clean

all: uart_lib


uart_lib:$(UART_OBJS)
	@$(AR) rvc $(LIB_A) $(UART_OBJS)

clean:
	@rm -rf $(LIB_A)
	@rm -rf $(UART_OBJS)
