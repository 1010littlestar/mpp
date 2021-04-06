CPU_TYPE=multi-core
ifeq ($(PARAM_FILE), )
PARAM_FILE:=../../../../../../Makefile.param
include $(PARAM_FILE)
endif
ifeq ($(PARAMFILE),)
    PARAMFILE:=../../Makefile.param
    include $(PARAMFILE)
endif

#*******************************************************************************
# The rule of make runtime
#*******************************************************************************
RUNTIME_DIR := $(DSP_PATH)/runtime
DSP_LIB_DIR := $(DSP_PATH)/lib
DCCS_DIR := $(DSP_PATH)/dccs
IPCM_DIR := $(DSP_PATH)/liteos/ipcm
ALGO_DIR := $(DSP_PATH)/algo
XTENSA_ELF_DIR := /opt/xtensa/$(XTENSA_ELF_CORE_VER)/$(DSP_CORE)/xtensa-elf
DSP_LDSCRIPTS:="--multilib-dir=$(LITEOSTOPDIR)/../dsp$(DSP_ID)_ldscripts"

CFLAGS += $(CFG_INC)

KERNEL_LIBS += -ldsp_lib -ldsp_tm -ldsp_algo

ifeq ($(CONFIG_HI_PHOTO_SUPPORT),y)
ifeq ($(DSP_ID),0)
KERNEL_LIBS += -ldsp_photo -ldetail_dsp -lhdr_dsp -lmfnr_dsp -lsfnr_dsp
endif
endif

LIBDEP := --start-group $(KERNEL_LIBS) -ldsp_dccs -lipcm -lvirt-tty -ldsp_uart -lc -lgloss -lgcc -lm -lidma-xtos \
			 -lxtutil -lhandler-reset -lhandlers-board  -lhal --end-group

LDFLAGS += -L $(XTENSA_ELF_DIR)/arch/lib\
		-L $(XTENSA_ELF_DIR)/lib\
		-L $(XTENSA_ELF_DIR)/lib/xcc \
		-L $(IPCM_DIR)/out/node_5 \
		-L $(DCCS_DIR) \
		-L $(DSP_LIB_DIR) \
		-L $(DSP_PATH)/uart \
		-L $(ALGO_DIR) \
		-L $(DSP_PATH)/tm

OBJS := $(wildcard ./*.o)

BIN := ./bin
TARGET := $(BIN)/hi_dsp
MAP := $(TARGET).map

.PHONY: all clean

all: $(BIN) $(TARGET)

$(TARGET):$(OBJS)
	@$(LD) --xtensa-core=$(DSP_CORE) --xtensa-system=$(DSP_XTENSA_SYSTEM) $(DSP_LDSCRIPTS) $(LDFLAGS) -Map=$(MAP) -o $(TARGET) $(OBJS) $(LIBDEP)
	@chmod +x txt2bin.sh
	@chmod +x ftxt2bin
	@./txt2bin.sh
	@$(RM) $(BIN)/*.txt

$(BIN):
	@mkdir -p $(BIN)

clean:
	@$(RM) $(TARGET)
	@$(RM) $(MAP)
	@$(RM) $(BIN)
