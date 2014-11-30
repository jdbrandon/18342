/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @author Jeff Brandon <jdbrando@andrew.cmu.edu>
 * @author Keane Lucas <kjlucas@andrew.cmu.edu>
 *
 * @date 2014-11-24
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
extern int assign_schedule(task_t**, size_t);
extern void allocate_tasks(task_t**, size_t);
extern void dispatch_sleep(void);

/**
 * @brief Takes a list of task structs, schedules them, and runs them
 *        using a rate monotonic scheduling algorithm. Clobbers any
 *	  previous tasks set.
 *
 * @param tasks  A list of task structs
 * @param num_tasks  How many tasks_structs in list
 */
int task_create(task_t* tasks, size_t num_tasks)
{
	if(num_tasks > OS_MAX_TASKS){
		return -EINVAL;
	}
	size_t i;
	for(i = 0; i<num_tasks; i++){
		if((uint32_t)tasks[i].stack_pos > 0xa2ffffff || 
			(uint32_t)tasks[i].stack_pos < 0xa0000000 || 
			(uint32_t)tasks[i].lambda > 0xa2ffffff || 
			(uint32_t)tasks[i].lambda < 0xa0000000){  
				return -EFAULT;
		}
	}
	if(!assign_schedule(&tasks, num_tasks)){
		return -ESCHED;
	}
	allocate_tasks(&tasks, num_tasks);

	//should never get to this point
	return 0; 
}

int event_wait(unsigned int dev)
{
	if(dev >= NUM_DEVICES){
		return -EINVAL;
	}
	dev_wait(dev);
	dispatch_sleep();
  	return 0; 
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
