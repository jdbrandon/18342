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

	.file "task_create.S"
	.align 2
	.global task_create

task_create:
	swi CREATE_SWI 
