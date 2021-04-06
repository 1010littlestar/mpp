ifeq ($(PARAM_FILE), )
     PARAM_FILE:=../Makefile.param
     include $(PARAM_FILE)
endif

SRC  := $(wildcard *.c)
OBJ  := $(SRC:%.c=%.o)

INC_FLAGS := -I$(REL_INC)
INC_FLAGS += -I$(SDK_PATH)/mpp/cbb/vi/mkp/include
INC_FLAGS += -I$(SDK_PATH)/mpp/cbb/vi/arch/$(HIARCH)/include
INC_FLAGS += -I$(SDK_PATH)/mpp/cbb/isp/isp/user/firmware/arch/$(HIARCH)/include
INC_FLAGS += -I$(SDK_PATH)/mpp/cbb/gdc/gdc/include/inner
INC_FLAGS += -I$(SDK_PATH)/mpp/cbb/gdc/gdc/ext_inc
INC_FLAGS += -I$(SDK_PATH)/mpp/cbb/dis/ext_inc
CFLAGS += -Wall -g -lpthread $(INC_FLAGS)
CFLAGS += $(LIBS_CFLAGS)
CFLAGS += -ldl

LIBS += $(REL_LIB)/libisp.a
LIBS += $(REL_LIB)/libmpi.a
LIBS += $(REL_LIB)/lib_hidehaze.a
LIBS += $(REL_LIB)/lib_hidrc.a
LIBS += $(REL_LIB)/lib_hildci.a
LIBS += $(REL_LIB)/libhifisheyecalibrate.a
LIBS += $(REL_LIB)/lib_hicalcflicker.a
LIBS += $(AUDIO_LIBA)
CFLAGS += $(LIBS_CFLAGS)

CFLAGS += -lm

include $(PWD)/Make.$(OSTYPE)
