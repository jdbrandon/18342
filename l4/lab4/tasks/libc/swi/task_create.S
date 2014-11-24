/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Jeff Brandon <jdbrando@andrew.cmu.edu>
 * @author Keane Lucas <kjlucas@andrew.cmu.edu>
 * @date 11-16-2014
 */

#include <bits/swi.h>
#include <asm.h>
#include <errno.h>

	.file "task_create.S"
	.align 2
	.global task_create

FUNC(task_create)
	swi CREATE_SWI 
	push	{r1}
	cmp	r0, #0
	rsbmi	r0, r0, #0
	ldrmi	r1, =errno
	strmi	r0, [r1]
	movmi	r0, #-1
	pop	{r1}
	mov pc, lr
