@ irq_handler.S: custom irq handling assembly code. Saves user mode registers
@		then calls an irq handler written in c. Before returning to 
@		user mode, register values are restored.
@ Author: Jeff Brandon <jdbrando@andrew.cmu.edu>
@ Author: Keane Lucas <kjlucas@andrew.cmu.edu>
@ Date: Sat Nov 01 15:01:20 UTC 2014

	.file "irq_handler.S"
	.text
	.align 2
	.global irq_handler

#include <const.h>

irq_handler:
	ldr	sp, =IRQ_STACK_BASE
	sub	sp, sp, #4
	stmfd 	sp!, {r0-r12, lr}
	mrs	r2, spsr
	str	r2, [sp, #14*4]
	mov	r1, sp			@r1 points to arguments on stack (not sure if this is needed yet...)
	bl	c_irq_handler		@call c_irq_handler
	ldr	r2, [sp, #14*4]
	msr	spsr, r2
	ldmfd	sp!, {r0-r12, lr}	@restore saved values
	add	sp, sp, #4
	movs	pc, lr
