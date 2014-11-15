@ user_mode.S: Switches execution from kernel mode to user mode.
@		Saves value of r8 on the stack for later use, saves
@		current value of lr and sp, updates cpsr and sets up
@		the user stack. Then begins executing at 0xa2000000 
@		where the user program is expected to be loaded. 
@		If the user program doesn't call exit, when it returns
@		an exit swi is issued.
@ Author: Jeff Brandon <jdbrando@andrew.cmu.edu>
@ Date: Sun Oct 12 19:05:18 UTC 2014

#include <bits/swi.h>
#include <const.h>
	
	.file "user_mode.S"
	.text
	.align 2
	.global user_mode
	
user_mode:
	str 	r8, [sp, #-4]!
	str	lr, [r2]
	str	sp, [r3]
	mrs	r7, cpsr
	bic	r7, r7, #0xff
	orr	r7, r7, #0x50	@specifies user mode with fiq's masked
	msr	cpsr, r7
	mov	sp, #0xa3
	mov	sp, sp, ROR #8	@ldr	sp, =0xa3000000
	push	{r1}
	push	{r0}
	mov	lr, pc
	ldr	pc, =USER_STACK_BASE
	@swi	EXIT_SWI -- no longer support exit
