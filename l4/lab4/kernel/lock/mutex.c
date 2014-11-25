/**
 * @file mutex.c
 *
 * @brief Implementation of mutices.
 *
 * @author Jeff Brandon <jdbrando@andrew.cmu.edu>
 * @author Keane Lucas <kjlucas@andrew.cmu.edu>
 * 
 * @Nov 23, 2014
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

/* mutex_init
	iterates through gtMutex and initializes mutex state 
	for each of the kernels muticies. Each is marked as 
	available and unlocked. Pointers to the sleep queue and
	the current holding processes tcb are initialized to null.
*/
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

/* mutex_create syscall implementation.
	This function is synchronized by disabling interrupts while it executes.
	Interrupts are enabled before it returns. This routine searches the array
	of mutecies for an available mutex. If one is found, its availability is 
	set to 0 and the index into the mutex array is returned.
	If there are no available mutecies, ENOMEM is returned.
	
	Returns:
	   Success - The index into the mutex struct array, a unique identifier for the mutex
	   Error - ENOMEM
*/
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
	
	return -ENOMEM;
}

/* mutex_lock syscall implementation.
	This routine is used to request mutual exclusion on a mutex.
	If the mutex is not locked, it becomes locked and the current
	task is used to set the holding tcb value of the mutex. 
	If however, the mutex is already locked the current task is placed
	on the sleep queue of the mutex to be awoken when the mutex is later
	unlocked.
	This operation is synchronized, meaning interrupts are disabled while
	while it is executing and enabled after.
	
	Parameters:
	   mutex - The index into gtMutex where the mutex data structure can be found.

	Returns:
	   Success - 0
	   Error - error code
		EINVAL: invalid argument
		EDEADLOCK: mutex is already locked by current task.
		
*/
int mutex_lock(int mutex)
{
	mutex_t* tmp;
	tcb_t* temp_q, * cur_tcb;
	disable_interrupts();
	tmp = &gtMutex[mutex];
	if(tmp->bAvailable || mutex > OS_NUM_MUTEX || mutex < 0){
		return -EINVAL;
	}
	if(tmp->pHolding_Tcb == get_cur_tcb()){
		return -EDEADLOCK;
	}
	if(tmp->bLock){
		//add to sleep queue
		temp_q = tmp->pSleep_queue;
		if(temp_q == NULL){
			tmp->pSleep_queue = get_cur_tcb();
			tmp->pSleep_queue->sleep_queue = NULL;
		} else {
			while(temp_q->sleep_queue != NULL)
				temp_q = temp_q->sleep_queue;
			temp_q->sleep_queue = get_cur_tcb();
			temp_q->sleep_queue->sleep_queue = NULL;
		}
		runqueue_remove(get_cur_tcb()->cur_prio);
		enable_interrupts();
		//revoke control of cpu from current process
		//until the mutex is avaialble.	
	} else {
		tmp->bLock = 1;
		cur_tcb = get_cur_tcb();
		cur_tcb->holds_lock = 1;
		cur_tcb->cur_prio = 0;
		tmp->pHolding_Tcb = cur_tcb;
		runqueue_add(cur_tcb, cur_tcb->cur_prio);
	}
	enable_interrupts();
	return 0;
}

/* mutex_unlock syscall implementation.
	This routine is used by a task that whishes to relinquish its
	lock on a mutex. This returns the tasks current priority to its
	native priority.
	If the mutex has any other tasks actively waiting for access to
	the mutex in the sleep queue, the current holding task is updated
	with the first task on the sleep queue and the mutex remains locked.
	If the mutex has no other tasks waiting for it, the mutex is unlocked.

	Parameters:
	   mutex - index into the gtMutex struct array
	
	Returns:
	   Success - 0
	   Error - error code
	      EINVAL: invalid operand, mutex outside the range of valid mutecies,
			or mutex is not created.
	      EPERM: attempt to unlock a mutex that isn't controlled by the
			current task.
*/
int mutex_unlock(int mutex)
{
	int holds_other_lock(tcb_t*, int);
	mutex_t* tmp;
	tcb_t* cur_tcb;
	disable_interrupts();
	tmp = &gtMutex[mutex];
	if(tmp->bAvailable || mutex > OS_NUM_MUTEX || mutex < 0)
		return -EINVAL;
	if(tmp->pHolding_Tcb != get_cur_tcb())
		return -EPERM;
	cur_tcb = get_cur_tcb();
	cur_tcb->holds_lock = holds_other_lock(cur_tcb, mutex);
	if(!cur_tcb->holds_lock){
		cur_tcb->cur_prio = cur_tcb->native_prio;
		runqueue_remove(0);
		runqueue_add(cur_tcb, cur_tcb->cur_prio);
	}
	if(tmp->pSleep_queue == NULL){
		tmp->pHolding_Tcb = NULL;
		tmp->bLock = 0;
	} else {
		tmp->pHolding_Tcb = tmp->pSleep_queue;
		tmp->pSleep_queue = tmp->pSleep_queue->sleep_queue;
		tmp->pHolding_Tcb->cur_prio = 0;
		runqueue_add(tmp->pHolding_Tcb, 0);
	}
	enable_interrupts();
	return 0;
}

/* holds_other_lock
	given a tasks tcb and a mutex that is going to be unlocked,
	determines if the task holds a lock on any other mutecies.

	Parameters:
	   tcb - pointer to the task control block of the task being tested
	   mtx - index of mutex being released.

	Returns:
	   1 if the task specified by tcb currently holds a lock on any mutex
	   other than the one specified by mtx.
	   0 is returned if the mutex specified by mtx is the only mutex the
	   task holds a lock on.
*/
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
