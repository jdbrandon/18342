@ write.S: write sycall wrapper
@
@ Authors: Jeff Brandon jdbrando@andrew.cmu.edu
@ Date:    Wed Oct 1 21:10:22 UTC 2014

#include <bits/swi.h>
	.file	"write.S"
	.text

	.global	write
write:
	@r0 - file descriptor to write to
	@r1 - pointer to the first byte in the buffer
	@r2 - number of bytes to write
	@returns - number of bytes written
	str	r1, [sp , #-4]! @store r1 on stack
	swi	WRITE_SWI	@perform write swi
	tst	r0, r0		@check return value for sign bit
	ldrlt	r1, =errno	@loads address of errno into r1 if error occured
	rsblt	r0, r0, #0	@stores positive error code in r0 if error occured
	strlt	r0, [r1]	@sets value of errno if error occured
	movlt	r0, #-1		@sets return value to -1 if error occured
	ldr	r1, [sp], #4 	@restore r1
	mov 	pc, lr		@return to caller
