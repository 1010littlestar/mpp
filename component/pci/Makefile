# $(ROOT)/mpp/code/component/pci/Makefile

ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../Makefile.param
    include $(PARAM_FILE)
endif

COMMON_ADAPT_HEADER     := hi_common_adapt.h
RGN_ADAPT_HEADER        := hi_comm_region_adapt.h
SYS_ADAPT_HEADER        := hi_comm_sys_adapt.h
VB_ADAPT_HEADER         := hi_comm_vb_adapt.h
VGS_ADAPT_HEADER        := hi_comm_vgs_adapt.h
VIDEO_ADAPT_HEADER      := hi_comm_video_adapt.h
VO_ADAPT_HEADER         := hi_comm_vo_adapt.h
VPSS_ADAPT_HEADER       := hi_comm_vpss_adapt.h
DEBUG_ADAPT_HEADER      := hi_debug_adapt.h
ERRNO_ADAPT_HEADER      := hi_errno_adapt.h

VB_HEADER       := hi_comm_vb.h
VGS_HEADER      := hi_comm_vgs.h
VPSS_HEADER     := hi_comm_vpss.h
RGN_HEADER      := hi_comm_region.h
PCIV_HEADER     := hi_comm_pciv.h

BASE_MKP_HEADER             := mkp_ioctl.h dev_ext.h mod_ext.h vb_ext.h mm_ext.h proc_ext.h
SYS_MKP_HEADER              := mkp_sys.h
VGS_EXT_HEADER              := vgs_ext.h
SYS_EXT_HEADER              := sys_ext.h
RGN_EXT_HEADER              := region_ext.h
VPSS_EXT_HEADER             := vpss_ext.h
HIOS_MCC_USRDEV_HEADER      := hi_mcc_usrdev.h

OSAL_HEADER := hi_osal.h osal_list.h osal_ioctl.h osal_mmz.h

INC_DIR=$(MPP_PATH)/component/pci/include
INC_ADAPT_DIR=$(MPP_PATH)/component/pci/include/adapt

CBB_PATH    := $(MPP_PATH)/cbb
COM_PATH    := $(CBB_PATH)/include
RGN_PATH    := $(CBB_PATH)/region/include
SYS_PATH    := $(CBB_PATH)/sys/include
BASE_PATH   := $(CBB_PATH)/base/include
VO_PATH     := $(CBB_PATH)/vo/include
VPSS_PATH   := $(CBB_PATH)/vpss

ifeq ($(HIARCH),hi3516ev200)
        VGS_PATH    := $(CBB_PATH)/vgs/vgs_hi3516ev200
else
        VGS_PATH    := $(CBB_PATH)/vgs/vgs
endif

OSDRV_PATH := ../$(MPP_PATH)

.PHONY: default clean

default:
ifeq ($(MPP_BUILD), y)
	@-pushd $(MPP_PATH)/component/pci/include > /dev/null;  rm -f $(BASE_MKP_HEADER); rm -f $(SYS_MKP_HEADER);  popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include > /dev/null;  rm -f $(VGS_EXT_HEADER); rm -f $(SYS_EXT_HEADER); rm -f $(RGN_EXT_HEADER); rm -f $(VPSS_EXT_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include > /dev/null;  rm -f $(OSAL_HEADER); popd > /dev/null

	@-pushd $(MPP_PATH)/component/pci/include/adapt > /dev/null;    rm -f $(COMMON_ADAPT_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include/adapt > /dev/null;    rm -f $(RGN_ADAPT_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include/adapt > /dev/null;    rm -f $(SYS_ADAPT_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include/adapt > /dev/null;    rm -f $(VB_ADAPT_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include/adapt > /dev/null;    rm -f $(VGS_ADAPT_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include/adapt > /dev/null;    rm -f $(VIDEO_ADAPT_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include/adapt > /dev/null;    rm -f $(VO_ADAPT_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include/adapt > /dev/null;    rm -f $(VPSS_ADAPT_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include/adapt > /dev/null;    rm -f $(DEBUG_ADAPT_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include/adapt > /dev/null;    rm -f $(ERRNO_ADAPT_HEADER); popd > /dev/null

	@-pushd $(MPP_PATH)/component/pci/include > /dev/null;  rm -f $(VB_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include > /dev/null;  rm -f $(VGS_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include > /dev/null;  rm -f $(VPSS_HEADER); popd > /dev/null
	@-pushd $(MPP_PATH)/component/pci/include > /dev/null;  rm -f $(RGN_HEADER); popd > /dev/null

	@-pushd $(MPP_PATH)/cbb/base/ext_inc > /dev/null;       cp $(BASE_MKP_HEADER) $(INC_DIR); popd > /dev/null
	@-pushd $(MPP_PATH)/cbb/sys/mkp/include > /dev/null;    cp $(SYS_MKP_HEADER) $(INC_DIR); popd > /dev/null
	@-pushd $(VGS_PATH)/ext_inc > /dev/null;                cp $(VGS_EXT_HEADER) $(INC_DIR); popd > /dev/null
	@-pushd $(MPP_PATH)/cbb/sys/ext_inc > /dev/null;        cp $(SYS_EXT_HEADER) $(INC_DIR); popd > /dev/null
	@-pushd $(MPP_PATH)/cbb/region/ext_inc > /dev/null;     cp $(RGN_EXT_HEADER) $(INC_DIR); popd > /dev/null
	@-pushd $(VPSS_PATH)/ext_inc > /dev/null;               cp $(VPSS_EXT_HEADER) $(INC_DIR); popd > /dev/null
	@-pushd $(SDK_PATH)/osal/include > /dev/null;           cp $(OSAL_HEADER) $(INC_DIR); popd > /dev/null

	@-pushd $(COM_PATH)/adapt > /dev/null;          cp $(COMMON_ADAPT_HEADER) $(INC_ADAPT_DIR); popd > /dev/null
	@-pushd $(RGN_PATH)/adapt > /dev/null;          cp $(RGN_ADAPT_HEADER) $(INC_ADAPT_DIR); popd > /dev/null
	@-pushd $(SYS_PATH)/adapt > /dev/null;          cp $(SYS_ADAPT_HEADER) $(INC_ADAPT_DIR); popd > /dev/null
	@-pushd $(BASE_PATH)/adapt > /dev/null;         cp $(VB_ADAPT_HEADER) $(INC_ADAPT_DIR); popd > /dev/null
	@-pushd $(VGS_PATH)/include/adapt > /dev/null;  cp $(VGS_ADAPT_HEADER) $(INC_ADAPT_DIR); popd > /dev/null
	@-pushd $(COM_PATH)/adapt > /dev/null;          cp $(VIDEO_ADAPT_HEADER) $(INC_ADAPT_DIR); popd > /dev/null
	@-pushd $(VO_PATH)/adapt > /dev/null;           cp $(VO_ADAPT_HEADER) $(INC_ADAPT_DIR); popd > /dev/null
	@-pushd $(VPSS_PATH)/include/adapt > /dev/null; cp $(VPSS_ADAPT_HEADER) $(INC_ADAPT_DIR); popd > /dev/null
	@-pushd $(COM_PATH)/adapt > /dev/null;          cp $(DEBUG_ADAPT_HEADER) $(INC_ADAPT_DIR); popd > /dev/null
	@-pushd $(COM_PATH)/adapt > /dev/null;          cp $(ERRNO_ADAPT_HEADER) $(INC_ADAPT_DIR); popd > /dev/null

	@-pushd $(BASE_PATH) > /dev/null;           cp $(VB_HEADER) $(INC_DIR); popd > /dev/null
	@-pushd $(VPSS_PATH)/include > /dev/null;   cp $(VPSS_HEADER) $(INC_DIR); popd > /dev/null
	@-pushd $(VGS_PATH)/include > /dev/null;    cp $(VGS_HEADER) $(INC_DIR); popd > /dev/null
	@-pushd $(RGN_PATH) > /dev/null;            cp $(RGN_HEADER) $(INC_DIR); popd > /dev/null
endif

ifeq ($(MPP_BUILD), y)
ifeq ($(HIARCH),hi3559av100)
	@-pushd $(HIWORK_PATH)/osdrv$(HIARCH)$(CVER)/components/pcie_mcc/pcie_mcc/mcc_core/include > /dev/null; cp $(HIOS_MCC_USRDEV_HEADER) $(REL_INC); popd > /dev/null
else
	@-pushd $(HIWORK_PATH)/osdrv$(HIARCH)$(CVER)/components/pcie_mcc/mcc_core/include > /dev/null; cp $(HIOS_MCC_USRDEV_HEADER) $(REL_INC); popd > /dev/null
endif
else
	@-pushd $(SDK_PATH)/osdrv/components/pcie_mcc/pcie_mcc/mcc_core/include > /dev/null; cp $(HIOS_MCC_USRDEV_HEADER) $(REL_INC); popd > /dev/null
endif
	@-pushd $(MPP_PATH)/component/pci/include > /dev/null;  cp $(PCIV_HEADER) $(REL_INC); popd > /dev/null
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	    do cd `dirname $$x`; if [ $$? ]; then make || exit 1; cd ../; fi; done

clean:
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	    do cd `dirname $$x`; if [ $$? ]; then make clean || exit 1; cd ../; fi; done

ifeq ($(MPP_BUILD), y)
	@-pushd $(MPP_PATH)/component/pci/include > /dev/null; popd > /dev/null
endif

