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

int task_create(task_t* tasks, size_t num_tasks)
{
	if(num_tasks > OS_MAX_TASKS){
		return EINVAL;
	}
	puts("creating tasks...\n");
	size_t i;
	sorttasks(tasks, num_tasks);
	runqueue_init();
	for(i = 0; i<num_tasks; i++){
		runqueue_add(&system_tcb[i], i+1);
		printf("tcb%x: %x\n", (uint32_t)i, (uint32_t)&system_tcb[i]);
		system_tcb[i].context.r4 = (uint32_t)tasks[i].lambda;
		system_tcb[i].context.r5 = (uint32_t)tasks[i].data;
		system_tcb[i].context.r6 = (uint32_t)tasks[i].stack_pos;
		system_tcb[i].context.sp = &system_tcb[i].kstack_high[0];
		system_tcb[i].context.lr = launch_task;
	}
	runqueue_add(&system_tcb[63], 63);
	printf("tcb%d: %x\n", (uint32_t)63, (uint32_t)&system_tcb[63]);
	system_tcb[63].context.r4 = (uint32_t)get_idle();
	system_tcb[63].context.r5 = (uint32_t)0;
	system_tcb[63].context.r6 = (uint32_t)&system_tcb[63].kstack[128];
	system_tcb[63].context.sp = &system_tcb[63].kstack_high[0];
	system_tcb[63].context.lr = launch_task;
//	allocate_tasks(&tasks, num_tasks); //i still dont know what this does...
	sched_init(&tasks[0]);

//should never get to this point
	return 0; /* remove this line after adding your code */
}

int event_wait(unsigned int dev)
{
	puts("event waiting...\n");
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
