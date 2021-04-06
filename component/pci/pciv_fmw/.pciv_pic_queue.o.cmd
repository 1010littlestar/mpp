cmd_/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/pciv_pic_queue.o := aarch64-himix100-linux-gcc -Wp,-MD,/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/.pciv_pic_queue.o.d  -nostdinc -isystem /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/include -I./arch/arm64/include -I./arch/arm64/include/generated/uapi -I./arch/arm64/include/generated  -I./include -I./arch/arm64/include/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -fno-PIE -mgeneral-regs-only -DCONFIG_AS_LSE=1 -fno-asynchronous-unwind-tables -mpc-relative-literal-loads -fno-delete-null-pointer-checks -Wno-frame-address -O2 --param=allow-store-data-races=0 -DCC_HAVE_ASM_GOTO -Wframe-larger-than=2048 -fstack-protector-strong -Wno-unused-but-set-variable -Wno-unused-const-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -Werror=incompatible-pointer-types -I/home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/include -I/home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/pciv/kernel -I/home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/include/adapt -I/home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include -I. -Wall -DHI_RELEASE -Wunused-but-set-variable -DVER_X=2 -DVER_Y=0 -DVER_Z=3 -DVER_P=1 -DVER_B=20 -DUSER_BIT_64 -DKERNEL_BIT_64 -I/home/pub/himpp_git/himpp/board/mpp/./../osal/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/include/adapt -I/home/pub/himpp_git/himpp/board/mpp/./cbb/base/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/base/include/adapt -I/home/pub/himpp_git/himpp/board/mpp/./cbb/base/arch/hi3559av100/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/sys/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/vdec/include/h7 -I/home/pub/himpp_git/himpp/board/mpp/./cbb/vo/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/audio/include/inner -I/home/pub/himpp_git/himpp/board/mpp/./component/securec/include -Wno-date-time -Wall -Dhi3559av100 -DHI_XXXX  -DMODULE -mcmodel=large  -DKBUILD_BASENAME='"pciv_pic_queue"'  -DKBUILD_MODNAME='"hi3559av100_pciv_fmw"' -c -o /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/pciv_pic_queue.o /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/pciv_pic_queue.c

source_/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/pciv_pic_queue.o := /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/pciv_pic_queue.c

deps_/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/pciv_pic_queue.o := \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/include/mm_ext.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/include/hi_osal.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/hisi/snapshot/boot.h) \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/include/osal_list.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/include/osal_ioctl.h \
  /opt/hisi-linux/x86-arm/aarch64-himix100-linux/lib/gcc/aarch64-linux-gnu/6.3.0/include/stdarg.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_math.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_type.h \
  include/linux/types.h \
    $(wildcard include/config/have/uid16.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  arch/arm64/include/generated/asm/types.h \
  include/uapi/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  arch/arm64/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/gcov/kernel.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  arch/arm64/include/uapi/asm/posix_types.h \
  include/uapi/asm-generic/posix_types.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_common.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/autoconf.h \
    $(wildcard include/config/hi3559av100.h) \
    $(wildcard include/config/hi/chip/type.h) \
    $(wildcard include/config/hi/arch.h) \
    $(wildcard include/config/hi/subarch.h) \
    $(wildcard include/config/hi/subchip/type.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/arm/arch/type.h) \
    $(wildcard include/config/cpu/type/multi/core.h) \
    $(wildcard include/config/cpu/type.h) \
    $(wildcard include/config/version/asic.h) \
    $(wildcard include/config/hi/fpga.h) \
    $(wildcard include/config/linux/os.h) \
    $(wildcard include/config/os/type.h) \
    $(wildcard include/config/linux/4/9/y.h) \
    $(wildcard include/config/kernel/version.h) \
    $(wildcard include/config/kernel/aarch64/himix100.h) \
    $(wildcard include/config/hi/cross.h) \
    $(wildcard include/config/libc/type.h) \
    $(wildcard include/config/kernel/bit.h) \
    $(wildcard include/config/user/aarch64/himix100.h) \
    $(wildcard include/config/hi/cross/lib.h) \
    $(wildcard include/config/user/bit.h) \
    $(wildcard include/config/release/type/release.h) \
    $(wildcard include/config/hi/rls/mode.h) \
    $(wildcard include/config/customer/version/common.h) \
    $(wildcard include/config/customer/version.h) \
    $(wildcard include/config/hi/vb/extpool/support.h) \
    $(wildcard include/config/hi/sys/support.h) \
    $(wildcard include/config/hi/sys/scale/coef/support.h) \
    $(wildcard include/config/hi/sys/scale/coef/online/support.h) \
    $(wildcard include/config/hi/vi/support.h) \
    $(wildcard include/config/hi/vi/bt656.h) \
    $(wildcard include/config/hi/vi/mipi.h) \
    $(wildcard include/config/hi/isp/2dof/dis/support.h) \
    $(wildcard include/config/hi/vi/ldcv1/support.h) \
    $(wildcard include/config/hi/vi/sendyuv/support.h) \
    $(wildcard include/config/hi/vi/src/support.h) \
    $(wildcard include/config/hi/vi/stitch/support.h) \
    $(wildcard include/config/hi/vi/parallel/support.h) \
    $(wildcard include/config/hi/vi/3dnrx/support.h) \
    $(wildcard include/config/hi/vi/userpic/support.h) \
    $(wildcard include/config/hi/vi/coverex/overlayex/support.h) \
    $(wildcard include/config/hi/vi/luma/support.h) \
    $(wildcard include/config/hi/vi/fpn/support.h) \
    $(wildcard include/config/hi/vi/extchn/support.h) \
    $(wildcard include/config/hi/snap/support.h) \
    $(wildcard include/config/hi/pipe/receiver.h) \
    $(wildcard include/config/hi/bnr/support.h) \
    $(wildcard include/config/hi/isp/support.h) \
    $(wildcard include/config/hi/isp/af/support.h) \
    $(wildcard include/config/hi/isp/cr/support.h) \
    $(wildcard include/config/hi/isp/pregamma/support.h) \
    $(wildcard include/config/hi/isp/gcac/support.h) \
    $(wildcard include/config/hi/isp/ca/support.h) \
    $(wildcard include/config/hi/isp/edgemark/support.h) \
    $(wildcard include/config/hi/isp/hlc/support.h) \
    $(wildcard include/config/hi/isp/specawb/support.h) \
    $(wildcard include/config/hi/isp/dpc/static/table/support.h) \
    $(wildcard include/config/hi/dis/support.h) \
    $(wildcard include/config/hi/vpss/support.h) \
    $(wildcard include/config/hi/vpss/sbs/support.h) \
    $(wildcard include/config/hi/vpss/sharpen.h) \
    $(wildcard include/config/hi/vpss/auto/support.h) \
    $(wildcard include/config/hi/vpss/backup/support.h) \
    $(wildcard include/config/hi/vpss/luma/stat/support.h) \
    $(wildcard include/config/hi/vpss/cover/support.h) \
    $(wildcard include/config/hi/vpss/mosaic/support.h) \
    $(wildcard include/config/hi/vpss/delay/support.h) \
    $(wildcard include/config/hi/vpss/2scale/support.h) \
    $(wildcard include/config/hi/avs/support.h) \
    $(wildcard include/config/hi/gdc/support.h) \
    $(wildcard include/config/hi/gdc/fisheye/lmf/support.h) \
    $(wildcard include/config/hi/gdc/fisheye/support.h) \
    $(wildcard include/config/hi/vgs/support.h) \
    $(wildcard include/config/hi/vgs/stitch/support.h) \
    $(wildcard include/config/hi/vgs/luma/stat/support.h) \
    $(wildcard include/config/hi/chnl/support.h) \
    $(wildcard include/config/hi/venc/support.h) \
    $(wildcard include/config/hi/h265e/support.h) \
    $(wildcard include/config/hi/h265e/userdata/support.h) \
    $(wildcard include/config/hi/h265e/intra/refresh/support.h) \
    $(wildcard include/config/hi/h264e/support.h) \
    $(wildcard include/config/hi/h264e/svc/support.h) \
    $(wildcard include/config/hi/h264e/userdata/support.h) \
    $(wildcard include/config/hi/h264e/intra/refresh/support.h) \
    $(wildcard include/config/hi/jpege/support.h) \
    $(wildcard include/config/hi/mjpege/support.h) \
    $(wildcard include/config/hi/jpege/dcf/support.h) \
    $(wildcard include/config/hi/jpege/userdata/support.h) \
    $(wildcard include/config/hi/prores/support.h) \
    $(wildcard include/config/hi/venc/lowdelay/support.h) \
    $(wildcard include/config/hi/venc/vpssauto/support.h) \
    $(wildcard include/config/hi/venc/overlayex/support.h) \
    $(wildcard include/config/hi/venc/framebuf/recycle/support.h) \
    $(wildcard include/config/hi/venc/mpf/vgs/support.h) \
    $(wildcard include/config/hi/venc/vgs/support.h) \
    $(wildcard include/config/hi/venc/advsmartp/support.h) \
    $(wildcard include/config/hi/venc/smartp/support.h) \
    $(wildcard include/config/hi/venc/bipredb/support.h) \
    $(wildcard include/config/hi/venc/dualp/support.h) \
    $(wildcard include/config/hi/venc/skipref/support.h) \
    $(wildcard include/config/hi/venc/scene0/support.h) \
    $(wildcard include/config/hi/venc/scene1/support.h) \
    $(wildcard include/config/hi/venc/scene2/support.h) \
    $(wildcard include/config/hi/rc/avbr/support.h) \
    $(wildcard include/config/hi/rc/qpmap/support.h) \
    $(wildcard include/config/hi/rc/qvbr/support.h) \
    $(wildcard include/config/hi/rc/cvbr/support.h) \
    $(wildcard include/config/hi/vdec/support.h) \
    $(wildcard include/config/hi/h265d/support.h) \
    $(wildcard include/config/hi/h264d/support.h) \
    $(wildcard include/config/vdec/ip.h) \
    $(wildcard include/config/hi/jpegd/support.h) \
    $(wildcard include/config/hi/jpegd/rgb/output/support.h) \
    $(wildcard include/config/hi/jpegd/progressive.h) \
    $(wildcard include/config/vdec/rotation/support.h) \
    $(wildcard include/config/vdec/userpic/support.h) \
    $(wildcard include/config/vdec/userdata/support.h) \
    $(wildcard include/config/hi/vo/support.h) \
    $(wildcard include/config/hi/vo/hd.h) \
    $(wildcard include/config/hi/vo/play/ctl.h) \
    $(wildcard include/config/hi/vo/luma.h) \
    $(wildcard include/config/hi/vo/fb/separate.h) \
    $(wildcard include/config/hi/vo/cover/osd/support.h) \
    $(wildcard include/config/hi/vo/virtdev/support.h) \
    $(wildcard include/config/hi/vo/vgs.h) \
    $(wildcard include/config/hi/vo/graph.h) \
    $(wildcard include/config/hi/vo/batch.h) \
    $(wildcard include/config/hi/vo/wbc.h) \
    $(wildcard include/config/hi/region/support.h) \
    $(wildcard include/config/hi/rgn/vgs/quickcopy/support.h) \
    $(wildcard include/config/hi/audio/support.h) \
    $(wildcard include/config/hi/acodec/support.h) \
    $(wildcard include/config/hi/acodec/version.h) \
    $(wildcard include/config/hi/acodec/max/gain.h) \
    $(wildcard include/config/hi/acodec/min/gain.h) \
    $(wildcard include/config/hi/acodec/gain/step.h) \
    $(wildcard include/config/hi/acodec/fast/power/support.h) \
    $(wildcard include/config/hi/downvqe/support.h) \
    $(wildcard include/config/hi/talkvqe/support.h) \
    $(wildcard include/config/hi/recordvqe/support.h) \
    $(wildcard include/config/hi/hdr/support.h) \
    $(wildcard include/config/drv.h) \
    $(wildcard include/config/extdrv.h) \
    $(wildcard include/config/interdrv.h) \
    $(wildcard include/config/cipher.h) \
    $(wildcard include/config/hiuser.h) \
    $(wildcard include/config/mipi/tx.h) \
    $(wildcard include/config/mipi/rx.h) \
    $(wildcard include/config/hi/ir.h) \
    $(wildcard include/config/hi/wdg.h) \
    $(wildcard include/config/hi/syscfg.h) \
    $(wildcard include/config/hi/hdmi/support.h) \
    $(wildcard include/config/hi/hifb/support.h) \
    $(wildcard include/config/hi/svp/support.h) \
    $(wildcard include/config/hi/svp/dsp.h) \
    $(wildcard include/config/hi/svp/liteos.h) \
    $(wildcard include/config/hi/svp/cnn.h) \
    $(wildcard include/config/hi/svp/runtime.h) \
    $(wildcard include/config/hi/svp/ive.h) \
    $(wildcard include/config/hi/svp/ive/csc.h) \
    $(wildcard include/config/hi/svp/ive/filter/and/csc.h) \
    $(wildcard include/config/hi/svp/ive/ncc.h) \
    $(wildcard include/config/hi/svp/ive/gmm.h) \
    $(wildcard include/config/hi/svp/ive/lbp.h) \
    $(wildcard include/config/hi/svp/ive/norm/grad.h) \
    $(wildcard include/config/hi/svp/ive/st/candi/corner.h) \
    $(wildcard include/config/hi/svp/ive/grad/fg.h) \
    $(wildcard include/config/hi/svp/ive/ann.h) \
    $(wildcard include/config/hi/svp/ive/svm.h) \
    $(wildcard include/config/hi/svp/ive/resize.h) \
    $(wildcard include/config/hi/svp/ive/cnn.h) \
    $(wildcard include/config/hi/svp/md.h) \
    $(wildcard include/config/hi/svp/dpu/rect.h) \
    $(wildcard include/config/hi/svp/dpu/match.h) \
    $(wildcard include/config/hi/photo/support.h) \
    $(wildcard include/config/hi/tde/support.h) \
    $(wildcard include/config/hi/pciv/support.h) \
    $(wildcard include/config/hi/avs/lut/support.h) \
    $(wildcard include/config/hi/pos/query/support.h) \
    $(wildcard include/config/hisyslink.h) \
    $(wildcard include/config/hi/gdb/no.h) \
    $(wildcard include/config/hi/gdb.h) \
    $(wildcard include/config/hi/log/trace/support.h) \
    $(wildcard include/config/hi/log/trace/all.h) \
    $(wildcard include/config/hi/log/trace/level.h) \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_defines.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/include/osal_mmz.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_debug.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_common.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_comm_video.h \
    $(wildcard include/config/info/s.h) \
    $(wildcard include/config/s.h) \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/pciv_pic_queue.h \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
    $(wildcard include/config/page/poisoning/zero.h) \
  include/uapi/linux/const.h \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/panic/timeout.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  include/linux/linkage.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/trim/unused/ksyms.h) \
    $(wildcard include/config/unused/symbols.h) \
  arch/arm64/include/asm/linkage.h \
  include/linux/bitops.h \
  arch/arm64/include/asm/bitops.h \
  arch/arm64/include/asm/barrier.h \
  include/asm-generic/barrier.h \
  include/asm-generic/bitops/builtin-__ffs.h \
  include/asm-generic/bitops/builtin-ffs.h \
  include/asm-generic/bitops/builtin-__fls.h \
  include/asm-generic/bitops/builtin-fls.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/le.h \
  arch/arm64/include/uapi/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/uapi/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  arch/arm64/include/generated/asm/swab.h \
  include/uapi/asm-generic/swab.h \
  include/linux/byteorder/generic.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/typecheck.h \
  include/linux/printk.h \
    $(wildcard include/config/message/loglevel/default.h) \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk/nmi.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/debug/rodata.h) \
  include/linux/kern_levels.h \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/uapi/linux/kernel.h \
  include/uapi/linux/sysinfo.h \
  arch/arm64/include/asm/cache.h \
  arch/arm64/include/asm/cachetype.h \
  arch/arm64/include/asm/cputype.h \
  arch/arm64/include/asm/sysreg.h \
    $(wildcard include/config/arm64/4k/pages.h) \
    $(wildcard include/config/arm64/16k/pages.h) \
    $(wildcard include/config/arm64/64k/pages.h) \
  arch/arm64/include/asm/opcodes.h \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/cpu/endian/be8.h) \
  arch/arm64/include/../../arm/include/asm/opcodes.h \
    $(wildcard include/config/cpu/endian/be32.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/include/adapt/hi_common_adapt.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/autoconf.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_type.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_defines.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/include/adapt/hi_comm_video_adapt.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/component/pci/include/adapt/hi_common_adapt.h \

/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/pciv_pic_queue.o: $(deps_/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/pciv_pic_queue.o)

$(deps_/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv_fmw/pciv_pic_queue.o):
