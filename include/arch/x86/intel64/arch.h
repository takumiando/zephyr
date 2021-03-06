/*
 * Copyright (c) 2019 Intel Corp.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_ARCH_X86_INTEL64_ARCH_H_
#define ZEPHYR_INCLUDE_ARCH_X86_INTEL64_ARCH_H_

#include <arch/x86/intel64/thread.h>
#include <arch/x86/thread_stack.h>

#if CONFIG_ISR_STACK_SIZE != (CONFIG_ISR_SUBSTACK_SIZE * CONFIG_ISR_DEPTH)
#error "Check ISR stack configuration (CONFIG_ISR_*)"
#endif

#if CONFIG_ISR_SUBSTACK_SIZE % STACK_ALIGN
#error "CONFIG_ISR_SUBSTACK_SIZE must be a multiple of 16"
#endif

#ifndef _ASMLANGUAGE
static ALWAYS_INLINE unsigned int arch_irq_lock(void)
{
	unsigned long key;

	__asm__ volatile ("pushfq; cli; popq %0" : "=g" (key) : : "memory");

	return (unsigned int) key;
}

/*
 * the exception stack frame
 */

struct x86_esf {
#ifdef CONFIG_EXCEPTION_DEBUG
	/* callee-saved */
	unsigned long rbx;
	unsigned long rbp;
	unsigned long r12;
	unsigned long r13;
	unsigned long r14;
	unsigned long r15;
#endif /* CONFIG_EXCEPTION_DEBUG */

	/* Caller-saved regs */
	unsigned long rax;
	unsigned long rcx;
	unsigned long rdx;
	unsigned long rsi;
	unsigned long rdi;
	unsigned long r8;
	unsigned long r9;
	unsigned long r10;
	/* Must be aligned 16 bytes from the end of this struct due to
	 * requirements of 'fxsave (%rsp)'
	 */
	char fxsave[X86_FXSAVE_SIZE];
	unsigned long r11;

	/* Pushed by CPU or assembly stub */
	unsigned long vector;
	unsigned long code;
	unsigned long rip;
	unsigned long cs;
	unsigned long rflags;
	unsigned long rsp;
	unsigned long ss;
};

typedef struct x86_esf z_arch_esf_t;

struct x86_ssf {
	unsigned long rip;
	unsigned long rflags;
	unsigned long r10;
	unsigned long r9;
	unsigned long r8;
	unsigned long rdx;
	unsigned long rsi;
	char fxsave[X86_FXSAVE_SIZE];
	unsigned long rdi;
	unsigned long rsp;
};

#define ARCH_EXCEPT(reason_p) do { \
	__asm__ volatile( \
		"movq %[reason], %%rax\n\t" \
		"int $32\n\t" \
		: \
		: [reason] "i" (reason_p)); \
	CODE_UNREACHABLE; \
} while (false)

#endif /* _ASMLANGUAGE */

/*
 * All Intel64 interrupts are dynamically connected.
 */

#define ARCH_IRQ_CONNECT arch_irq_connect_dynamic

#endif /* ZEPHYR_INCLUDE_ARCH_X86_INTEL64_ARCH_H_ */
