/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */
void dev_init();
void dev_update(unsigned long);
void dotime(unsigned*);

void sched_init(task_t* main_task)
{
	dev_init();
	
	//WHAT IS MAIN TASK USED FOR?!
	main_task = main_task;

	/*what needs to happen is the sp must be changed to the main_tasks kernel sp, something needs to load r4, r5, and r6 with lambda, user data, and user stack respectively. That way launch task can put it into user mode*/
	dispatch_nosave(); //this will load r4, r5, and r6 from highest priority tcb and return to launch_task() 
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void idle(void)
{
	 enable_interrupts();
	 while(1);
}
void* get_idle(){
	return (void*)idle;
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks, size_t num_tasks)
{
	//divide up user stack
	
	//initialize kernel context (if any?)	
	size_t i;
	for(i = 0; i < num_tasks; i++){
		ctx_init(tasks[i]->stack_pos, tasks[i]->lambda, tasks[i]->data, system_tcb[i+1].kstack_high);
	}
}

