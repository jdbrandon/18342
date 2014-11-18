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
extern void idle(void);
extern void sched_init(task_t*);
extern void allocate_tasks(task_t**, size_t);

int task_create(task_t* tasks, size_t num_tasks)
{
	size_t i;
	task_t* best = NULL;
	unsigned long tempT = 0, mintime = 0 ,maxtime = 0;
	runqueue_init();
	for(i = 0; i<num_tasks; i++){
		tempT = tasks[i].T;
		if(tempT < mintime || mintime == 0){
			mintime = tempT;
			best = &tasks[i];
		}
		if(tempT > maxtime || maxtime == 0){
			maxtime = tempT;
		}
	}
	for(i = 0; i<num_tasks; i++){
		runqueue_add(&system_tcb[i], (uint8_t)(1+((tasks[i].T-mintime)/(maxtime-mintime))*62)); //if something goes wrong DEBUG ME
	}
	//add idle queue
	//set up the link register

	sched_init(best);
//should never get to this point
  return 0; /* remove this line after adding your code */
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
  return 1; /* remove this line after adding your code */	
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
