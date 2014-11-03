@ irq_handler.S: custom swi handling assembly code. Saves user mode registers
@		restores saved value of r8, prepares parameters, and extracts
@		the swi number, then calls a swi handler written in c. Before
@		returning to user mode, register values are restored.
@ Author: Jeff Brandon <jdbrando@andrew.cmu.edu>
@ Author: Keane Lucas <kjlucas@andrew.cmu.edu>
@ Date: Sat Nov 01 15:01:20 UTC 2014

	.file "irq_handler.S"
	.text
	.align 2
	.global irq_handler

irq_handler:
	ldr	sp, =0xa2500000
	sub	sp, sp, #4
	stmfd 	sp!, {r0-r12, lr}
	mrs	r2, spsr
	str	r2, [sp, #14*4]
	@ldr	r8, [sp, #60]		@restore value of r8 
	mov	r1, sp			@r1 points to arguments on stack (not sure if this is needed yet...)
	bl	c_irq_handler		@call c_irq_handler
	ldr	r2, [sp, #14*4]
	msr	spsr, r2
	ldmfd	sp!, {r0-r12, lr}	@restore saved values
	add	sp, sp, #4
	movs	pc, lr
