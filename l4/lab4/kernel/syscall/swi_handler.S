@ swi_handler.S: custom swi handling assembly code. Saves user mode registers
@		restores saved value of r8, prepares parameters, and extracts
@		the swi number, then calls a swi handler written in c. Before
@		returning to user mode, register values are restored.
@ Author: Jeff Brandon <jdbrando@andrew.cmu.edu>
@ Author: Keane Lucas <kjlucas@andrew.cmu.edu>
@ Date: Sun Oct 12 18:55:20 UTC 2014

	.file "swi_handler.S"
	.text
	.align 2
	.global swi_handler

#include <const.h>

swi_handler:
	sub	sp, sp, #4		@save user registers
	stmfd 	sp!, {r0-r12, lr}
	ldr	r8, =R8_VAL 		@restore value of r8 
	mrs	r2, spsr
	str	r2, [sp, #14*4]
	mov	r1, sp			@r1 points to arguments on stack
	ldr	r0, [lr, #-4]		@extract swi num
	mov	r4, #0xff
	bic 	r0, r0, r4, ror #8
	mrs 	r4, cpsr
	bic	r4, r4, #0x80
	msr	cpsr, r4
	bl	c_swi_handler		@call c_swi_handler
	ldr	r2, [sp, #14*4]
	msr	spsr, r2
	add	sp, sp, #4
	ldmfd	sp!, {r1-r12, lr}	@restore saved values
	add	sp, sp, #4
	movs	pc, lr
