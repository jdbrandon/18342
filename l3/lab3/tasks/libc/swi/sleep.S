/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author Keane Lucas <kjlucas@andrew.cmu.edu>
 * @date 2014-11-01
 */

#include <asm.h>
#include <bits/swi.h>

	.file "sleep.S"
	.text
	.global sleep
#@FUNC(sleep)
sleep:
	swi SLEEP_SWI
	mov pc, lr
