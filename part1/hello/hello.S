@ hello.S: Hello world! in assembly
@
@ Authors: Jeff Brandon jdbrando@andrew.cmu.edu 
@ Date:	   Wed Oct 1 21:15:29 UTC 2014

#include <bits/swi.h>
#include <bits/fileno.h>
	.file	"hello.S"
	.text

	.global	_start
_start:
	mov	r0, #STDIN_FILENO	@specify stdout
	ldr 	r1, =hello_str	@set r1 to point to the buffer to print
	mov 	r2, #hello_size	@set r2 to the number of bytes to print
	swi 	WRITE_SWI	@call write using swi
	mov 	r0, #0		@set exit status to 0
	swi 	EXIT_SWI	@call exit using swi

hello_str:			@Define the string to print
	.string	"Hello world!\n"
	.set	hello_size, .-hello_str	@this allows us to calculate the size
					@of hello_str at compile time.
