/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @author Jeff Brandon <jdbrando@andrew.cmu.edu>
 * @author Keane Lucas <kjlucas@andrew.cmu.edu>
 * @date 2014-11-24
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static tcb_t* cur_tcb; 
tcb_t* highest_tcb(void);

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
	cur_tcb = &system_tcb[IDLE_PRIO];
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
	tcb_t* htcb = highest_tcb();	//get highest priority tcb
	//if switching from idle, dont save context
	if(get_cur_prio() == IDLE_PRIO){
		dispatch_nosave();
	}
	if(cur_tcb != htcb){
		tcb_t* temp = cur_tcb;
		cur_tcb = htcb;
		//full context switch to new task
		ctx_switch_full(&htcb->context.r4, &temp->context.r4);
	}
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
	tcb_t* htcb = highest_tcb(); //run_list[hprio];	//get highest priority tcb
//	printf("htcb: %x\n",(uint32_t)htcb);
//	printf("htcblr: %x\n",(uint32_t)htcb->context.lr);
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
	tcb_t* htcb = highest_tcb(); 	//get highest priority tcb
	tcb_t* temp = cur_tcb;
	//if switching from idle, dont save context
	if(get_cur_prio() == IDLE_PRIO){
		dispatch_nosave();
	}
	cur_tcb = htcb;
	//full context switch to new task
	ctx_switch_full(&htcb->context.r4, &temp->context.r4); 
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return (tcb_t *) cur_tcb; 
}
