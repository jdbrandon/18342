/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>
#include <errno.h>

	.file "mutex_lock.S"

FUNC(mutex_lock)
	push 	{r1}
	swi	MUTEX_LOCK
	cmp	r0, #0
	rsbmi	r0, r0, #0
	ldrmi	r1, =errno
	strmi	r0, [r1]
	movmi	r0, #-1
	pop	{r1}
	mov	pc, lr
