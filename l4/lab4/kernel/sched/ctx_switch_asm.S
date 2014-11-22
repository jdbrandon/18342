/** @file ctx_switch_asm.S
 *
 * @brief Contains the core context switch routine.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * @author Kartik Subramanian
 * @date   2008-11-21
 */

	.file "ctx_switch_asm.S"

#include <asm.h>
#include <arm/psr.h>

/**
 * @brief Kernel context initialization routine.
 *
 * r0 contains the user entry point.
 * r1 contains the single argument to the user function called.
 * r2 contains the user-mode stack pointer.
 * r3 contains the kernel stack address
 * Upon completion, we should be in user mode.
 */

FUNC(ctx_init)
	mov	 sp, r3
	stmfd    sp!, {r0}	@pc will be usr entrance address
	ldr	 lr, =0xfeedb007
	mov	 r5, r1
	mov	 r6, r2
	stmfd    sp!, {r0-r3, r8, ip, lr} 
	mrs      r0, spsr
	stmfd    sp, {r0, sp, lr}^
	add      sp, sp, #-12
	mov	 pc, lr

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * Upon completion, we should be in user mode.
 */
FUNC(launch_task)
	mov     r0, r5
	mov     r1, #0
	mov     r2, #0
	mov     r3, #0
	mov     r5, #0
	mov     r8, #0
	mov     ip, #0
	msr     cpsr_c, #(PSR_MODE_USR)
	mov     sp, r6
	mov     r6, #0
	ldr     lr, =0xdeadbeef   /* Causes a crash instead of calling the reset vector */
	mov     pc, r4

/* r0 points to the target context, r1 to the current context. */
/* add your code to perform a full context switch */
FUNC(ctx_switch_full)
	stmia	r1, {r4-r11, sp, lr}

/*add your code to perform a half context switch */	
FUNC(ctx_switch_half)
	ldmia	r0, {r4-r11, sp, lr}
	mov	pc, lr	@should return to dispatcher	
