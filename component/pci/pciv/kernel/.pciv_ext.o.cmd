cmd_/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_ext.o := aarch64-himix100-linux-gcc -Wp,-MD,/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/.pciv_ext.o.d  -nostdinc -isystem /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/include -I./arch/arm64/include -I./arch/arm64/include/generated/uapi -I./arch/arm64/include/generated  -I./include -I./arch/arm64/include/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -fno-PIE -mgeneral-regs-only -DCONFIG_AS_LSE=1 -fno-asynchronous-unwind-tables -mpc-relative-literal-loads -fno-delete-null-pointer-checks -Wno-frame-address -O2 --param=allow-store-data-races=0 -DCC_HAVE_ASM_GOTO -Wframe-larger-than=2048 -fstack-protector-strong -Wno-unused-but-set-variable -Wno-unused-const-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -Werror=incompatible-pointer-types -Wall -DHI_RELEASE -Wunused-but-set-variable -DVER_X=2 -DVER_Y=0 -DVER_Z=3 -DVER_P=1 -DVER_B=20 -DUSER_BIT_64 -DKERNEL_BIT_64 -I/home/pub/himpp_git/himpp/board/mpp/./../osal/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/include/adapt -I/home/pub/himpp_git/himpp/board/mpp/./cbb/base/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/base/include/adapt -I/home/pub/himpp_git/himpp/board/mpp/./cbb/base/arch/hi3559av100/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/sys/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/vdec/include/h7 -I/home/pub/himpp_git/himpp/board/mpp/./cbb/vo/include -I/home/pub/himpp_git/himpp/board/mpp/./cbb/audio/include/inner -I/home/pub/himpp_git/himpp/board/mpp/./component/securec/include -Wno-date-time -Wall -Dhi3559av100 -DHI_XXXX -I/home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include -I/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include -I/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/adapt  -DMODULE -mcmodel=large  -DKBUILD_BASENAME='"pciv_ext"'  -DKBUILD_MODNAME='"hi3559av100_pciv"' -c -o /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_ext.o /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_ext.c

source_/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_ext.o := /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_ext.c

deps_/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_ext.o := \
  include/linux/module.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/modules/tree/lookup.h) \
    $(wildcard include/config/livepatch.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/module/sig.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/debug/set/module/ronx.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
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
  /opt/hisi-linux/x86-arm/aarch64-himix100-linux/lib/gcc/aarch64-linux-gnu/6.3.0/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/trim/unused/ksyms.h) \
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
  include/linux/stat.h \
  arch/arm64/include/asm/stat.h \
    $(wildcard include/config/compat.h) \
  arch/arm64/include/uapi/asm/stat.h \
  include/uapi/asm-generic/stat.h \
  arch/arm64/include/asm/compat.h \
  include/linux/sched.h \
    $(wildcard include/config/no/hz/common.h) \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/prove/rcu.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/no/hz/full.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/bpf/syscall.h) \
    $(wildcard include/config/sched/info.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/thread/info/in/task.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/preempt/notifiers.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/tasks/rcu.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/ubsan.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/arch/want/batched/unmap/tlb/flush.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/kcov.h) \
    $(wildcard include/config/uprobes.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/vmap/stack.h) \
    $(wildcard include/config/arch/wants/dynamic/task/struct.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/have/copy/thread/tls.h) \
    $(wildcard include/config/have/exit/thread.h) \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/cpu/freq.h) \
  include/uapi/linux/sched.h \
  include/linux/sched/prio.h \
  arch/arm64/include/uapi/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/uapi/asm-generic/param.h \
  include/linux/capability.h \
    $(wildcard include/config/multiuser.h) \
  include/uapi/linux/capability.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/timex.h \
  include/uapi/linux/timex.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/seqlock.h \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
  include/linux/preempt.h \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/preempt/tracer.h) \
  arch/arm64/include/generated/asm/preempt.h \
  include/asm-generic/preempt.h \
  include/linux/thread_info.h \
    $(wildcard include/config/have/arch/within/stack/frames.h) \
    $(wildcard include/config/hardened/usercopy.h) \
  include/linux/bug.h \
  arch/arm64/include/asm/bug.h \
    $(wildcard include/config/debug/bugverbose.h) \
  arch/arm64/include/asm/brk-imm.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  arch/arm64/include/asm/thread_info.h \
    $(wildcard include/config/arm64/sw/ttbr0/pan.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  arch/arm64/include/asm/irqflags.h \
  arch/arm64/include/asm/ptrace.h \
  arch/arm64/include/uapi/asm/ptrace.h \
  arch/arm64/include/asm/hwcap.h \
  arch/arm64/include/uapi/asm/hwcap.h \
  include/asm-generic/ptrace.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  arch/arm64/include/asm/spinlock_types.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lock/stat.h) \
  include/linux/rwlock_types.h \
  arch/arm64/include/asm/spinlock.h \
  arch/arm64/include/asm/lse.h \
    $(wildcard include/config/as/lse.h) \
    $(wildcard include/config/arm64/lse/atomics.h) \
  arch/arm64/include/asm/processor.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/uapi/linux/string.h \
  arch/arm64/include/asm/string.h \
  arch/arm64/include/asm/alternative.h \
    $(wildcard include/config/arm64/uao.h) \
    $(wildcard include/config/foo.h) \
  arch/arm64/include/asm/cpucaps.h \
  arch/arm64/include/asm/insn.h \
  arch/arm64/include/asm/fpsimd.h \
  arch/arm64/include/asm/hw_breakpoint.h \
    $(wildcard include/config/have/hw/breakpoint.h) \
  arch/arm64/include/asm/cpufeature.h \
  include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  include/linux/atomic.h \
    $(wildcard include/config/generic/atomic64.h) \
  arch/arm64/include/asm/atomic.h \
  arch/arm64/include/asm/atomic_ll_sc.h \
  arch/arm64/include/asm/cmpxchg.h \
  include/asm-generic/atomic-long.h \
  arch/arm64/include/asm/virt.h \
    $(wildcard include/config/arm64/vhe.h) \
  arch/arm64/include/asm/sections.h \
  include/asm-generic/sections.h \
  arch/arm64/include/asm/pgtable-hwdef.h \
    $(wildcard include/config/pgtable/levels.h) \
  include/linux/rwlock.h \
  include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  arch/arm64/include/generated/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/time64.h \
  include/uapi/linux/time.h \
  include/uapi/linux/param.h \
  arch/arm64/include/asm/timex.h \
  arch/arm64/include/asm/arch_timer.h \
    $(wildcard include/config/fsl/erratum/a008585.h) \
  include/clocksource/arm_arch_timer.h \
    $(wildcard include/config/arm/arch/timer.h) \
  include/linux/timecounter.h \
  include/asm-generic/timex.h \
  include/linux/jiffies.h \
  include/generated/timeconst.h \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/rbtree.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/rcu/stall/common.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/rcu/nocb/cpu/all.h) \
    $(wildcard include/config/no/hz/full/sysidle.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
  include/linux/bitmap.h \
    $(wildcard include/config/s390.h) \
  include/linux/completion.h \
  include/linux/wait.h \
  arch/arm64/include/generated/asm/current.h \
  include/asm-generic/current.h \
  include/uapi/linux/wait.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/ktime.h \
  include/linux/timekeeping.h \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  arch/arm64/include/generated/asm/errno.h \
  include/uapi/asm-generic/errno.h \
  include/uapi/asm-generic/errno-base.h \
  include/linux/rcutree.h \
  include/linux/nodemask.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/movable/node.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/debug/vm/pgflags.h) \
  include/linux/smp.h \
    $(wildcard include/config/up/late/init.h) \
  include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  arch/arm64/include/asm/smp.h \
    $(wildcard include/config/arm64/acpi/parking/protocol.h) \
  include/linux/pfn.h \
  arch/arm64/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/arch/enable/split/pmd/ptlock.h) \
    $(wildcard include/config/have/cmpxchg/double.h) \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/userfaultfd.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/x86/intel/mpx.h) \
    $(wildcard include/config/hugetlb/page.h) \
  include/linux/auxvec.h \
  include/uapi/linux/auxvec.h \
  arch/arm64/include/uapi/asm/auxvec.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/spin/on/owner.h) \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/err.h \
  include/linux/osq_lock.h \
  arch/arm64/include/generated/asm/rwsem.h \
  include/asm-generic/rwsem.h \
  include/linux/uprobes.h \
  include/linux/page-flags-layout.h \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/generated/bounds.h \
  arch/arm64/include/asm/sparsemem.h \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/wq/watchdog.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  arch/arm64/include/asm/page.h \
    $(wildcard include/config/arm64/page/shift.h) \
    $(wildcard include/config/arm64/cont/shift.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
  include/linux/personality.h \
  include/uapi/linux/personality.h \
  arch/arm64/include/asm/pgtable-types.h \
  include/asm-generic/pgtable-nopud.h \
  arch/arm64/include/asm/memory.h \
    $(wildcard include/config/arm64/va/bits.h) \
    $(wildcard include/config/blk/dev/initrd.h) \
  arch/arm64/include/generated/asm/sizes.h \
  include/asm-generic/sizes.h \
  include/linux/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
  include/asm-generic/getorder.h \
  arch/arm64/include/asm/mmu.h \
    $(wildcard include/config/unmap/kernel/at/el0.h) \
    $(wildcard include/config/harden/branch/predictor.h) \
  include/linux/cputime.h \
  arch/arm64/include/generated/asm/cputime.h \
  include/asm-generic/cputime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/asm-generic/cputime_jiffies.h \
  include/linux/sem.h \
  include/uapi/linux/sem.h \
  include/linux/ipc.h \
  include/linux/uidgid.h \
    $(wildcard include/config/user/ns.h) \
  include/linux/highuid.h \
  include/uapi/linux/ipc.h \
  arch/arm64/include/generated/asm/ipcbuf.h \
  include/uapi/asm-generic/ipcbuf.h \
  arch/arm64/include/generated/asm/sembuf.h \
  include/uapi/asm-generic/sembuf.h \
  include/linux/shm.h \
  include/uapi/linux/shm.h \
  arch/arm64/include/generated/asm/shmbuf.h \
  include/uapi/asm-generic/shmbuf.h \
  arch/arm64/include/asm/shmparam.h \
  include/uapi/asm-generic/shmparam.h \
  include/linux/signal.h \
    $(wildcard include/config/old/sigaction.h) \
  include/uapi/linux/signal.h \
  arch/arm64/include/uapi/asm/signal.h \
  include/asm-generic/signal.h \
  include/uapi/asm-generic/signal.h \
  include/uapi/asm-generic/signal-defs.h \
  arch/arm64/include/uapi/asm/sigcontext.h \
  arch/arm64/include/uapi/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/uapi/asm-generic/siginfo.h \
  include/linux/pid.h \
  include/linux/topology.h \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/cma.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/zsmalloc.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/zone/device.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/page/extension.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/deferred/struct/page/init.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/mutex/spin/on/owner.h) \
  include/linux/srcu.h \
  arch/arm64/include/asm/topology.h \
  include/asm-generic/topology.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/have/arch/seccomp/filter.h) \
    $(wildcard include/config/seccomp/filter.h) \
    $(wildcard include/config/checkpoint/restore.h) \
  include/uapi/linux/seccomp.h \
  include/linux/rculist.h \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/resource.h \
  include/uapi/linux/resource.h \
  arch/arm64/include/generated/asm/resource.h \
  include/asm-generic/resource.h \
  include/uapi/asm-generic/resource.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/time/low/res.h) \
    $(wildcard include/config/timerfd.h) \
  include/linux/timerqueue.h \
  include/linux/kcov.h \
  include/uapi/linux/kcov.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/security.h) \
  include/linux/key.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/sysctl.h \
  include/uapi/linux/sysctl.h \
  include/linux/assoc_array.h \
    $(wildcard include/config/associative/array.h) \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/gfp.h \
    $(wildcard include/config/pm/sleep.h) \
  include/uapi/linux/magic.h \
  include/linux/cgroup-defs.h \
    $(wildcard include/config/sock/cgroup/data.h) \
  include/uapi/linux/limits.h \
  include/linux/idr.h \
  include/linux/percpu-refcount.h \
  include/linux/percpu-rwsem.h \
  include/linux/rcu_sync.h \
  include/uapi/linux/stat.h \
  include/linux/kmod.h \
  include/linux/elf.h \
  arch/arm64/include/asm/elf.h \
  arch/arm64/include/generated/asm/user.h \
  include/asm-generic/user.h \
  include/uapi/linux/elf.h \
  include/uapi/linux/elf-em.h \
  include/linux/kobject.h \
    $(wildcard include/config/uevent/helper.h) \
    $(wildcard include/config/debug/kobject/release.h) \
  include/linux/sysfs.h \
  include/linux/kernfs.h \
    $(wildcard include/config/kernfs.h) \
  include/linux/kobject_ns.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/extable.h \
  include/linux/rbtree_latch.h \
  arch/arm64/include/asm/module.h \
    $(wildcard include/config/arm64/module/plts.h) \
    $(wildcard include/config/randomize/base.h) \
  include/asm-generic/module.h \
    $(wildcard include/config/have/mod/arch/specific.h) \
    $(wildcard include/config/modules/use/elf/rel.h) \
    $(wildcard include/config/modules/use/elf/rela.h) \
  include/linux/fs.h \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/cgroup/writeback.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/fs/encryption.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fs/dax.h) \
    $(wildcard include/config/mandatory/file/locking.h) \
    $(wildcard include/config/migration.h) \
  include/linux/kdev_t.h \
  include/uapi/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist_bl.h \
  include/linux/list_bl.h \
  include/linux/bit_spinlock.h \
  include/linux/lockref.h \
    $(wildcard include/config/arch/use/cmpxchg/lockref.h) \
  include/linux/stringhash.h \
    $(wildcard include/config/dcache/word/access.h) \
  include/linux/hash.h \
    $(wildcard include/config/have/arch/hash.h) \
  include/linux/path.h \
  include/linux/list_lru.h \
  include/linux/shrinker.h \
  include/linux/radix-tree.h \
    $(wildcard include/config/radix/tree/multiorder.h) \
  include/linux/semaphore.h \
  include/uapi/linux/fiemap.h \
  include/linux/migrate_mode.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/linux/bvec.h \
  include/linux/delayed_call.h \
  include/uapi/linux/fs.h \
  include/uapi/linux/ioctl.h \
  arch/arm64/include/generated/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/uapi/asm-generic/ioctl.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  include/linux/percpu_counter.h \
  include/uapi/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/projid.h \
  include/uapi/linux/quota.h \
  include/linux/nfs_fs_i.h \
  include/linux/fcntl.h \
  include/uapi/linux/fcntl.h \
  arch/arm64/include/uapi/asm/fcntl.h \
  include/uapi/asm-generic/fcntl.h \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/generic/msi/irq/domain.h) \
    $(wildcard include/config/pinctrl.h) \
    $(wildcard include/config/generic/msi/irq.h) \
    $(wildcard include/config/dma/cma.h) \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  include/linux/ioport.h \
  include/linux/klist.h \
  include/linux/pinctrl/devinfo.h \
    $(wildcard include/config/pm.h) \
  include/linux/pinctrl/consumer.h \
  include/linux/seq_file.h \
  include/linux/pinctrl/pinctrl-state.h \
  include/linux/pm.h \
    $(wildcard include/config/vt/console/sleep.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  include/linux/ratelimit.h \
  arch/arm64/include/asm/device.h \
    $(wildcard include/config/iommu/api.h) \
  include/linux/pm_wakeup.h \
  include/linux/mm.h \
    $(wildcard include/config/have/arch/mmap/rnd/bits.h) \
    $(wildcard include/config/have/arch/mmap/rnd/compat/bits.h) \
    $(wildcard include/config/mem/soft/dirty.h) \
    $(wildcard include/config/arch/uses/high/vma/flags.h) \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/x86/intel/memory/protection/keys.h) \
    $(wildcard include/config/ppc.h) \
    $(wildcard include/config/parisc.h) \
    $(wildcard include/config/metag.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/debug/vm/rb.h) \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/range.h \
  include/linux/page_ext.h \
    $(wildcard include/config/idle/page/tracking.h) \
  include/linux/stacktrace.h \
    $(wildcard include/config/stacktrace.h) \
    $(wildcard include/config/user/stacktrace/support.h) \
  include/linux/stackdepot.h \
  include/linux/page_ref.h \
    $(wildcard include/config/debug/page/ref.h) \
  include/linux/page-flags.h \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/ksm.h) \
  include/linux/tracepoint-defs.h \
  include/linux/static_key.h \
  arch/arm64/include/asm/pgtable.h \
    $(wildcard include/config/arm64/hw/afdbm.h) \
  arch/arm64/include/asm/proc-fns.h \
  arch/arm64/include/asm/pgtable-prot.h \
  arch/arm64/include/asm/fixmap.h \
  arch/arm64/include/asm/boot.h \
  include/asm-generic/fixmap.h \
  include/asm-generic/pgtable.h \
    $(wildcard include/config/have/arch/soft/dirty.h) \
    $(wildcard include/config/have/arch/huge/vmap.h) \
  include/linux/huge_mm.h \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
    $(wildcard include/config/debug/tlbflush.h) \
    $(wildcard include/config/debug/vm/vmacache.h) \
  include/linux/vm_event_item.h \
    $(wildcard include/config/memory/balloon.h) \
    $(wildcard include/config/balloon/compaction.h) \
  include/linux/poll.h \
  arch/arm64/include/asm/uaccess.h \
    $(wildcard include/config/arm64/pan.h) \
  arch/arm64/include/asm/kernel-pgtable.h \
  include/linux/kasan-checks.h \
  arch/arm64/include/asm/compiler.h \
  include/uapi/linux/poll.h \
  arch/arm64/include/generated/asm/poll.h \
  include/uapi/asm-generic/poll.h \
  include/linux/miscdevice.h \
  include/uapi/linux/major.h \
  include/generated/uapi/linux/version.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_ext.h \
  /home/pub/himpp_git/himpp/board/mpp/./../osal/include/hi_osal.h \
    $(wildcard include/config/hisi/snapshot/boot.h) \
  /home/pub/himpp_git/himpp/board/mpp/./../osal/include/osal_list.h \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/hi_errno.h \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/hi_debug.h \
    $(wildcard include/config/hi/log/trace/support.h) \
    $(wildcard include/config/hi/log/trace/level.h) \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/hi_type.h \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/hi_common.h \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/autoconf.h \
    $(wildcard include/config/hi3559av100.h) \
    $(wildcard include/config/hi/chip/type.h) \
    $(wildcard include/config/hi/arch.h) \
    $(wildcard include/config/hi/subarch.h) \
    $(wildcard include/config/hi/subchip/type.h) \
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
    $(wildcard include/config/hi/log/trace/all.h) \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/base/arch/hi3559av100/include/hi_defines.h \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/autoconf.h \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/hi_debug.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/mod_ext.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/hi_osal.h \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/hi_type.h \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/adapt/hi_common_adapt.h \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/hi_common.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/dev_ext.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/proc_ext.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/adapt/hi_comm_pciv_adapt.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/adapt/hi_comm_video_adapt.h \
  /home/pub/himpp_git/himpp/board/mpp/./cbb/include/hi_comm_video.h \
    $(wildcard include/config/info/s.h) \
    $(wildcard include/config/s.h) \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/adapt/hi_common_adapt.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_comm_pciv.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_type.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_common.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_errno.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_comm_video.h \
  /home/pub/himpp_git/himpp/board/mpp/./../mpp/out/hi3559av100/linux/multi-core/include/hi_comm_vpss.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/pciv_fmwext.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_drvadp.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/mkp_pciv.h \
  /home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/../../include/mkp_ioctl.h \

/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_ext.o: $(deps_/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_ext.o)

$(deps_/home/pub/himpp_git/himpp/board/mpp/component/pci/pciv/kernel/pciv_ext.o):
