/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];
tcb_t* get_cur_tcb(void);

void mutex_init()
{
	int i;
	for(i = 0; i < OS_NUM_MUTEX; i++){
		gtMutex[i].bAvailable = 1;
		gtMutex[i].bLock = 0;
		gtMutex[i].pSleep_queue = NULL;
		gtMutex[i].pHolding_Tcb = NULL;
	}
			
}

int mutex_create(void)
{
	int i;
	disable_interrupts();
	for(i = 0; i < OS_NUM_MUTEX; i++)
		if(gtMutex[i].bAvailable){
			gtMutex[i].bAvailable = 0;
			enable_interrupts();
			return i;
		}
	enable_interrupts();
	//set errno??
	return -1;
}

int mutex_lock(int mutex)
{
	mutex_t tmp;
	tcb_t* temp_q, * cur_tcb;
	disable_interrupts();
	tmp = gtMutex[mutex];
	if(tmp.bLock){
		//add to sleep queue
		temp_q = tmp.pSleep_queue;
		if(temp_q == NULL){
			tmp.pSleep_queue = get_cur_tcb();
			tmp.pSleep_queue->sleep_queue = NULL;
		} else {
			while(temp_q->sleep_queue != NULL)
				temp_q = temp_q->sleep_queue;
			temp_q->sleep_queue = get_cur_tcb();
			temp_q->sleep_queue->sleep_queue = NULL;
		}
		runqueue_remove(get_cur_tcb()->cur_prio);
		enable_interrupts();
		dispatch_sleep();
		//revoke control of cpu from current process
		//until the mutex is avaialble.	
	} else {
		tmp.bLock = 1;
		cur_tcb = get_cur_tcb();
		cur_tcb->holds_lock = 1;
		cur_tcb->cur_prio = 0;
		runqueue_add(cur_tcb, cur_tcb->cur_prio);
	}
	enable_interrupts();
	return 0; // fix this to return the correct value
}

int mutex_unlock(int mutex)
{
	int holds_other_lock(tcb_t*, int);
	mutex_t tmp;
	tcb_t* cur_tcb;
	disable_interrupts();
	cur_tcb = get_cur_tcb();
	cur_tcb->holds_lock = holds_other_lock(cur_tcb, mutex);
	if(!cur_tcb->holds_lock){
		cur_tcb->cur_prio = cur_tcb->native_prio;
		runqueue_remove(0);
		runqueue_add(cur_tcb, cur_tcb->cur_prio);
	}
	tmp = gtMutex[mutex];
	if(tmp.pSleep_queue == NULL){
		tmp.bLock = 0;
	} else {
		tmp.pHolding_Tcb = tmp.pSleep_queue;
		tmp.pSleep_queue = tmp.pSleep_queue->sleep_queue;
		tmp.pHolding_Tcb->cur_prio = 0;
		runqueue_add(tmp.pHolding_Tcb, 0);
	}
	enable_interrupts();
	dispatch_save();
	return 0;
}

int holds_other_lock(tcb_t* tcb, int mtx){
	int i;
	for(i=0; i<OS_NUM_MUTEX; i++){
		if(i == mtx){ 
			continue;
		}
		if(gtMutex[i].pHolding_Tcb == tcb){
			return 1;
		}
	}
	return 0;
}
