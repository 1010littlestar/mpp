#
# (C) COPYRIGHT 2011-2013 ARM Limited. All rights reserved.
#
# This program is free software and is provided to you under the terms of the
# GNU General Public License version 2 as published by the Free Software
# Foundation, and any use by you of this program is subject to the terms
# of such GNU licence.
#
# A copy of the licence is included with the program, and can also be obtained
# from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA  02110-1301, USA.
#
#


# linux build system bootstrap for out-of-tree module

# default to building for the host
PLATFORM:=BVT

# export ARCH=arm
# export CROSS_COMPILE?=arm-hisiv400-linux-
# export KDIR?=/home/gpu/xiaqing/B01/SDK/code/osdrv/hi3536/opensource/kernel/linux-3.10.y

ifeq ($(KDIR),)
$(error Must specify KDIR to point to the kernel to target))
endif

ifeq ($(KDIR),)
$(error Must specify KDIR to point to the kernel to target))
endif

all:
	$(MAKE) ARCH=$(ARCH) -C $(KDIR) M=$(CURDIR) EXTRA_CFLAGS="-I$(CURDIR) -I$(SDK_DIR)/osal/include" CONFIG_HI_DMA_BUFFER_EXPORTER=m
clean:
	$(MAKE) ARCH=$(ARCH) -C $(KDIR) M=$(CURDIR) clean

