@ exit_user.S: assembly instructions for restoring the kernel sp and lr
@		and then continuing kernel execution when exiting user mode
@ Author: Jeff Brandon <jdbrando@andrew.cmu.edu>
@ Date: Sun Oct 12 18:54:40 UTC 2014

	.file "exit_user.S"
	.text
	.align 2
	.global exit_user

exit_user:
	mov 	sp, r1
	ldr 	r8, [sp], #4
	mov	lr, r0
	mov	r0, r2
	mov	pc, lr
