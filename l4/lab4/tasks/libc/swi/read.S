@ read.S: read sycall wrapper
@
@ Authors: Jeff Brandon jdbrando@andrew.cmu.edu
@ Date:    Wed Oct 1 21:07:05 UTC 2014

#include <bits/swi.h>
	.file	"read.S"
	.text

	.global	read
read:
	@r0 - fd to read from
	@r1 - pointer to first byte of the buffer
	@r2 - number of bytes to read
	@returns - number of bytes successfully read, 0 indicates EOF
	str	r1, [sp , #-4]!	@store r1 on stack
	swi	READ_SWI	@perform swi to read
	tst	r0, r0		@check r0 to see if sign bit is set
	ldrlt	r1, =errno	@loads address of errno in r1 if error occured
	rsblt	r0, r0, #0	@makes error code positive if error occured
	strlt	r0, [r1]	@set errno if error occured
	movlt	r0, #-1		@set return to -1 if error occured
	ldr	r1, [sp], #4	@restore value of r1 if error occured
	mov 	pc, lr		@return to caller
