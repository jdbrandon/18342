/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @Author Jeff Brandon
 * @date 2014-11-01
 */

#include <asm.h>
#include <bits/swi.h>

	.file "time.S"

FUNC(time)
	swi 	TIME_SWI
	mov 	pc, lr
