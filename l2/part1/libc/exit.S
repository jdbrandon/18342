@ exit.S: exit sycall wrapper
@
@ Authors: Jeff Brandon jdbrando@andrew.cmu.edu
@ Date:    Wed Oct 1 21:05:29 UTC 2014

#include <bits/swi.h>
	.file	"exit.S"
	.text

	.global	exit
exit:
	swi EXIT_SWI @exit swi with status code passed in r0
