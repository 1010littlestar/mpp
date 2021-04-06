#*******************************************************************************
#The rule of make Linux kernel module
#*******************************************************************************
ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

EXTRA_CFLAGS += $(MPP_CFLAGS)
EXTRA_CFLAGS += -I$(REL_INC)
EXTRA_CFLAGS += -I$(REL_INC)/adapt
EXTRA_CFLAGS += -I$(OSAL_ROOT)/include
EXTRA_CFLAGS += -I$(MPP_PATH)/component/isp/ext_inc
EXTRA_CFLAGS += -I$(MPP_PATH)/component/isp/include

$(HIARCH)_acodec-objs =  hi_acodec.o ../init/acodec_init.o
$(HIARCH)_aenc-objs  = hi_aenc.o  ../init/aenc_init.o
$(HIARCH)_aio-objs  = hi_aio.o  ../init/aio_init.o
$(HIARCH)_avs-objs  = hi_avs.o  ../init/avs_init.o
$(HIARCH)_chnl-objs  = hi_chnl.o  ../init/chnl_init.o
$(HIARCH)_dpu_match-objs = hi_dpu_match.o  ../init/dpu_match_init.o
$(HIARCH)_dsp-objs  = hi_dsp.o  ../init/dsp_init.o
$(HIARCH)_h264e-objs  = hi_h264e.o  ../init/h264e_init.o
$(HIARCH)_hdmi-objs  = hi_hdmi.o   ../init/hdmi_init.o
$(HIARCH)_ive-objs  = hi_ive.o    ../init/ive_init.o
$(HIARCH)_jpege-objs  = hi_jpege.o  ../init/jpege_init.o
$(HIARCH)_rc-objs  = hi_rc.o   ../init/rc_init.o
$(HIARCH)_tde-objs  = hi_tde.o   ../init/tde_init.o
$(HIARCH)_vedu-objs  = hi_vedu.o  ../init/vedu_init.o
$(HIARCH)_vfmw-objs  = hi_vfmw.o  ../init/vfmw_init.o
$(HIARCH)_vi-objs  = hi_vi.o  ../init/vi_init.o
$(HIARCH)_vpss-objs  = hi_vpss.o  ../init/vpss_init.o
$(HIARCH)_adec-objs  = hi_adec.o    ../init/adec_init.o
$(HIARCH)_ai-objs  = hi_ai.o    ../init/ai_init.o
$(HIARCH)_ao-objs  = hi_ao.o   ../init/ao_init.o
$(HIARCH)_base-objs = hi_base.o  ../init/base_init.o
$(HIARCH)_dis-objs = hi_dis.o   ../init/dis_init.o
$(HIARCH)_dpu_rect-objs = hi_dpu_rect.o   ../init/dpu_rect_init.o
$(HIARCH)_gdc-objs = hi_gdc.o  ../init/gdc_init.o
$(HIARCH)_h265e-objs = hi_h265e.o  ../init/h265e_init.o
$(HIARCH)_isp-objs = hi_isp.o  ../init/isp_init.o
$(HIARCH)_jpegd-objs = hi_jpegd.o  ../init/jpegd_init.o
$(HIARCH)_nnie-objs = hi_nnie.o   ../init/nnie_init.o
$(HIARCH)_rgn-objs = hi_rgn.o  ../init/rgn_init.o
$(HIARCH)_sys-objs = hi_sys.o    ../init/sys_init.o
$(HIARCH)_vdec-objs = hi_vdec.o  ../init/vdec_init.o
$(HIARCH)_venc-objs = hi_venc.o  ../init/venc_init.o
$(HIARCH)_vgs-objs = hi_vgs.o   ../init/vgs_init.o
$(HIARCH)_vo-objs = hi_vo.o  ../init/vo_init.o
$(HIARCH)_svprt-objs = hi_svprt.o

obj-m += $(HIARCH)_acodec.o
obj-m += $(HIARCH)_aenc.o
obj-m += $(HIARCH)_aio.o
obj-m += $(HIARCH)_avs.o
obj-m += $(HIARCH)_chnl.o
obj-m += $(HIARCH)_dpu_match.o
obj-m += $(HIARCH)_dsp.o
obj-m += $(HIARCH)_h264e.o
obj-m += $(HIARCH)_hdmi.o
obj-m += $(HIARCH)_ive.o
obj-m += $(HIARCH)_jpege.o
obj-m += $(HIARCH)_rc.o
obj-m += $(HIARCH)_tde.o
obj-m += $(HIARCH)_vedu.o
obj-m += $(HIARCH)_vfmw.o
obj-m += $(HIARCH)_vi.o
obj-m += $(HIARCH)_vpss.o
obj-m += $(HIARCH)_adec.o
obj-m += $(HIARCH)_ai.o
obj-m += $(HIARCH)_ao.o
obj-m += $(HIARCH)_base.o
obj-m += $(HIARCH)_dis.o
obj-m += $(HIARCH)_dpu_rect.o
obj-m += $(HIARCH)_gdc.o
obj-m += $(HIARCH)_h265e.o
obj-m += $(HIARCH)_isp.o
obj-m += $(HIARCH)_jpegd.o
obj-m += $(HIARCH)_nnie.o
obj-m += $(HIARCH)_rgn.o
obj-m += $(HIARCH)_sys.o
obj-m += $(HIARCH)_vdec.o
obj-m += $(HIARCH)_venc.o
obj-m += $(HIARCH)_vgs.o
obj-m += $(HIARCH)_vo.o
obj-m += $(HIARCH)_svprt.o


default:
	@echo -e "\e[0;32;1m--Compiling '$(TARGET)'... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS) REL_INC=$(REL_INC)
	@echo ---- HIARCH=$(HIARCH), HICHIP=$(HICHIP), CVER=$(CVER), DBG=$(HIDBG), HI_FPGA=$(HI_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE) REL_KO=$(REL_KO)
	@echo ---- MPP_CFLAGS=$(MPP_CFLAGS)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@echo ---- KERNEL_ROOT=$(KERNEL_ROOT)
	@echo ---- ARCH_ROOT=$(ARCH_ROOT), ARCH_HAL=$(ARCH_HAL)
	@@echo -e "\e[0m"
	@$(MAKE) -C $(KERNEL_ROOT) M=$(PWD) V=10 modules -j 24
	@mkdir -p $(REL_KO) && cp -rf *.ko $(REL_KO)

clean:
	@rm -f *.ko
	@rm .tmp_versions  -rf
	@rm Module.symvers -f
	@rm modules.order  -f
	@rm .*.cmd  -f
	@rm *.mod.c  -f
	@rm hi*.mod.o  -f
	@rm hi3519av100_*.o  -f
	@rm ../init/*.o  -f
	@rm ../init/.*.cmd  -f
