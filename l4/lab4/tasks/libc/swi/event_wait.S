/** @file event_wait.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

	.file "event_wait.S"
	.align 2
	.global event_wait

FUNC(event_wait)
	push 	{r4,lr}
	mov 	r4, sp
	swi 	EVENT_WAIT
	cmp 	r0, #0
	rsbmi 	r0, r0, #0
	ldrmi 	r1, =errno
	strmi 	r0, [r1]
	movmi 	r0, #-1
	pop	{r1}
	mov 	sp, r4
	pop 	{r4,lr}
	mov 	pc, lr
