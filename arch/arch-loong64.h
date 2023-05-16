#ifndef ARCH_LOONGARCH64_H
#define ARCH_LOONGARCH64_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FIO_ARCH	(arch_loong64)

#define nop		__asm__ __volatile__("": : :"memory")
#define read_barrier()	__asm__ __volatile__("dbar 0": : :"memory")
#define write_barrier()	__asm__ __volatile__("dbar 0": : :"memory")

static inline int arch_ffz(unsigned long bitmask)
{
	unsigned long count, reversed_bits;
	if (~bitmask == 0)
		return 63;

	__asm__ __volatile__ ("bitrev.d %1, %2\n"
			      "clz.d %0, %1\n" : 
			      "=r"(count), "=&r"(reversed_bits) :
			      "r"(~bitmask));
	return count;
}

#define ARCH_HAVE_FFZ

#define dbar()	asm volatile("dbar 0" : : : "memory")

static inline unsigned long long get_cpu_clock(void)
{
	unsigned long val;
	unsigned long id;
	dbar();
	asm volatile("rdtime.d %0, %1" : "=r" (val), "=r" (id));
	return val;
}
#define ARCH_HAVE_CPU_CLOCK

#define ARCH_HAVE_INIT
extern bool tsc_reliable;
static inline int arch_init(char *envp[])
{
	tsc_reliable = true;
	return 0;
}

#endif
