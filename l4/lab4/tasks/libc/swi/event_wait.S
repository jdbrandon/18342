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
	swi EVENT_WAIT
	mov pc, lr
