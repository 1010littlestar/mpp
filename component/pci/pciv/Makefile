# $(ROOT)/mpp/code/component/pci/pciv/Makefile

ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../Makefile.param
    include $(PARAM_FILE)
endif

INC_PATH := -I$(MPP_PATH)/component/pci/include -I$(MPP_PATH)/component/pci/include/adapt -I$(REL_INC)


ifeq ($(HIGDB),HI_GDB)
FLAGS += -Wall -c -g
else
FLAGS += -c $(LIBS_CFLAGS)
endif

FLAGS += -I. $(INC_PATH)

#pciv compile(cd $(PWD)/kernel; make) will use FLAGS and LINUXROOT
export FLAGS
export KERNEL_ROOT

all:$(MPP_OBJ)
	@echo "LINUX_ROOT=$(KERNEL_ROOT)"
	@echo "FLAGS=$(FLAGS)"
	@$(CC) -o mpi_pciv.o  mpi_pciv.c $(FLAGS)
	@$(CC) -fPIC -shared -o libpciv.so mpi_pciv.o
	@$(AR) -rsv libpciv.a mpi_pciv.o
	cp libpciv.a $(REL_LIB)/
	cp libpciv.so $(REL_LIB)/
	cp ../include/mpi_pciv.h $(REL_INC)
	@cd $(PWD)/kernel; make

clean:
	rm -f mpi_pciv.o libpciv.a $(REL_LIB)/libpciv.a libpciv.so $(REL_LIB)/libpciv.so $(REL_INC)/mpi_pciv.h
	@cd $(PWD)/kernel; make clean
