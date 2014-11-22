/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
	//what else to do here? make TCB? pretty empty meow...
	//YEAH CREATE THE IDLE TCB - will be called in dispatch_save if no tasks available
	cur_tcb = &system_tcb[63];
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
	uint8_t hprio = highest_prio(); 			//get highest priority num
	tcb_t* htcb = &system_tcb[hprio-1]; //run_list[hprio]; 			//get highest priority tcb
	cur_tcb = htcb;
	ctx_switch_full(&htcb->context.r4, &cur_tcb->context.r4); 	//full context switch to new task
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
	uint8_t hprio = highest_prio();		//get highest priority num
	tcb_t* htcb = &system_tcb[hprio-1]; //run_list[hprio];	//get highest priority tcb
	printf("hprio: %d\n",hprio);
	printf("htcb: %d\n",(uint32_t)htcb);
	puts("setting context...\n");
	cur_tcb = htcb;
	ctx_switch_half(&htcb->context.r4);	//half context switch to new task

}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	uint8_t hprio = highest_prio(); 			//get highest priority num
	tcb_t* htcb = &system_tcb[hprio]; 			//get highest priority tcb
	ctx_switch_full(&htcb->context.r4, &cur_tcb->context.r4); 	//full context switch to new task
	
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->cur_prio; //fix this; dummy return to prevent compiler warning
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return (tcb_t *) cur_tcb; //fix this; dummy return to prevent compiler warning
}
