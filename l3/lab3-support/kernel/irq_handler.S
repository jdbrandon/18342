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
	sub	sp, sp, #4		@save user registers
	stmfd 	sp!, {r0-r12, lr}
	ldr	r8, [sp, #60]		@restore value of r8 
	mov	r1, sp			@r1 points to arguments on stack (not sure if this is needed yet...)
	bl	c_irq_handler		@call c_irq_handler
	ldmfd	sp!, {r0-r12, lr}	@restore saved values
	add	sp, sp, #4
	movs	pc, lr
