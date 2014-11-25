/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

extern tcb_t system_tcb[OS_MAX_TASKS];
extern void runqueue_init(void);
extern void runqueue_add(tcb_t*, uint8_t);
extern void* get_idle(void);
extern void sched_init(task_t*);
extern void allocate_tasks(task_t**, size_t);
extern void dispatch_sleep(void);
void sorttasks(task_t*, size_t);
void swap(task_t*, task_t*);

/* task_create syscall implementation
	Creates a set of tasks provided by a user program and schedules them after
	allocating appropriate resources and state information. Performs sorting of 
	tasks, and some error checking for valid memory addresses. Then calls 
	allocate_tasks to complete task creation. allocate_tasks should not return.
	
	Parameters:
	   tasks - unsorted list of task_t structs initialized by user program
	   num_tasks - the number of tasks provided for scheduling.
	
	Returns:
	   This function should never return.
*/
int task_create(task_t* tasks, size_t num_tasks)
{
	if(num_tasks > OS_MAX_TASKS){
		return -EINVAL;
	}
	size_t i;
	sorttasks(tasks, num_tasks);
	for(i = 0; i<num_tasks; i++){
		if((uint32_t)tasks[i].stack_pos > 0xa2ffffff || 
			(uint32_t)tasks[i].stack_pos < 0xa0000000 || 
			(uint32_t)tasks[i].lambda > 0xa2ffffff || 
			(uint32_t)tasks[i].lambda < 0xa0000000){  
				return -EFAULT;
		}
	}
	allocate_tasks(&tasks, num_tasks);

	//should never get to this point
	return 0; /*shut up the compiler*/ 
}

/* event_wait syscall implementation
	Causes the calling process to halt until an event occurs on the specified device.

	Parameters:
	   dev - the identifier of the device to wait for.
*/
int event_wait(unsigned int dev)
{
	if(dev >= NUM_DEVICES){
		return -EINVAL;
	}
	dev_wait(dev);
	dispatch_sleep();
  	return 0; /* remove this line after adding your code */	
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}

/* sorttasks
	helper function to sort a list of tasks.
	Highly inefficient but effective select sort algorithm.

	Parameters:
	   tasks - pointer to the first task_t in the array of tasks
	   count - the number of tasks in the list
*/
void sorttasks(task_t* tasks, size_t count){
	size_t i, j, min = 0, minpos = 0;
	for(i = 0; i < (count-1); i++){
		min = tasks[i].T;
		minpos = i;
		for(j = i+1; j < count; j++){
			if(tasks[j].T < min){
				min = tasks[j].T;
				minpos = j;
			}
		}
		if(i != minpos)
			swap(&tasks[i], &tasks[minpos]);
	}
}

/* swap (tasks)
	helper function to swap the memory locations of two tasks.
	after completion a will now point to the values of b, and b will
	point to the values in a.
	Parameters:
	   a - the first task_t pointer
	   b - the second task_t pointer
*/
void swap(task_t* a, task_t* b){
	task_t tmp;
	tmp.lambda = a->lambda;
	tmp.data = a->data;
	tmp.stack_pos = a->stack_pos;
	tmp.C = a->C;
	tmp.T = a->T;
	
	a->lambda = b->lambda;
	a->data = b->data;
	a->stack_pos = b->stack_pos;
	a->C = b->C;
	a->T = b->T;

	b->lambda = tmp.lambda;
	b->data = tmp.data;
	b->stack_pos = tmp.stack_pos;
	b->C = tmp.C;
	b->T = tmp.T;
}
