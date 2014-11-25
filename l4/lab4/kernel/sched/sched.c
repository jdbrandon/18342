/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @author Jeff Brandon <jdbrando@andrew.cmu.edu>
 * @author Keane Lucas <kjlucas@andrew.cmu.edu>
 * @date 2014-11-24
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
void start_timer(void);

/**
 * @brief Initializes interrupt timer and devices before first task is launched
 *
 * @param main_task  Highest priority task
 */
void sched_init(task_t* main_task)
{
	start_timer();
	dev_init();
	
	//quiet compiler errors
	main_task = main_task;

	//load r4, r5, and r6 from highest priority tcb 
	dispatch_nosave(); 
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void idle(void)
{
	enable_interrupts();
	while(1);
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
void allocate_tasks(task_t** tasks_args, size_t num_tasks)
{
	//initialize run_queue
	runqueue_init();
	unsigned i;
	task_t* tasks;
	tasks = *tasks_args;

	//initialize all tasks context to be started by launch_task()
	for(i = 0; i<num_tasks; i++){
		runqueue_add(&system_tcb[i], i+1);
		system_tcb[i].context.r4 = (uint32_t)tasks[i].lambda;
		system_tcb[i].context.r5 = (uint32_t)tasks[i].data;
		system_tcb[i].context.r6 = (uint32_t)tasks[i].stack_pos;
		system_tcb[i].context.sp = &system_tcb[i].kstack_high[0];
		system_tcb[i].context.lr = launch_task;
	}

	//create idle tcb and add to run_queue as lowest priority
	runqueue_add(&system_tcb[IDLE_PRIO], IDLE_PRIO);
	system_tcb[IDLE_PRIO].context.r4 = (uint32_t)idle;
	system_tcb[IDLE_PRIO].context.r5 = (uint32_t)0;
	system_tcb[IDLE_PRIO].context.r6 = (uint32_t)&system_tcb[IDLE_PRIO].kstack[128];
	system_tcb[IDLE_PRIO].context.sp = &system_tcb[IDLE_PRIO].kstack_high[0];
	system_tcb[IDLE_PRIO].context.lr = launch_task;

	//initialize schedule and run first task
	sched_init(&tasks[0]);
}

