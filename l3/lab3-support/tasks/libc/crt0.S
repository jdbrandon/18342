@ crt0.S: Execution startup routine
@
@ Authors: Jeff Brandon jdbrando@andrew.cmu.edu
@ Date:    Wed Oct 1 21:02:28 UTC 2014

#include <bits/swi.h>
	.file	"crt0.S"
	.text

	.global	_start
_start:
	ldr	r0, [sp] 	@move argc from address of sp into r0
	ldr	r1, [sp , #4]	@move first address of argv into r1
	bl	main		@call main
	swi	EXIT_SWI	@if exit hasn't already been called, call it
