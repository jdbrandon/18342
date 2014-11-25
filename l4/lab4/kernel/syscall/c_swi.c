/* @file c_swi.c

   @description - swi handler written in C to be called from assembly after initial 
      handling of the intterupt (preparing swi number and args)

   @author Jeff Brandon <jdbrando@andrew.cmu.edu>
   @author Keane Lucas  <kjlucas@andrew.cmu.edu>

   @date 2014-11-23
*/

#include <bits/swi.h>
#include <bits/fileno.h>
#include <bits/errno.h>
#include <exports.h>
#include <const.h>
#include <arm/timer.h>
#include <arm/psr.h>
#include <arm/interrupt.h>
#include <arm/exception.h>
#include <lock.h>

void doread(unsigned*, unsigned*);
void dowrite(unsigned*, unsigned*);
void dosleep(unsigned*);
void dotime(unsigned*);
int task_create(unsigned, unsigned);
int event_wait(unsigned);

/* global variables */
extern unsigned lr_k; 		//store value of kernel link register  
extern unsigned sp_k;		//store value of kernel stack pointer

/* c_swi_handler - custom swi handler called by assembly swi handler
   after state has been saved/restored appropriately.
	Parameters: 
	swi_num - the number the swi was invoked with.
	args - a pointer to arguments associated with the swi.
*/
void c_swi_handler(int swi_num, unsigned *args){
	unsigned *ret = args;
	switch(swi_num){
	case READ_SWI:
		/* read */
		doread(args, ret);
		break;
	case WRITE_SWI:
		/* write */
		dowrite(args, ret);
		break;
	case TIME_SWI:
		/*do time*/
		dotime(ret);
		break;
	case SLEEP_SWI:
		/*do sleep*/
		dosleep(args);
		break;
	case CREATE_SWI:
		/*do create*/
		*args = task_create(args[0], args[1]);
		break;
	case EVENT_WAIT:
		/*do event_wait*/
		disable_interrupts();
		*args = event_wait(args[0]);
		enable_interrupts();
		break;
	case MUTEX_CREATE:
		/*do mutex_create*/
		*args = mutex_create();
		break;
	case MUTEX_LOCK:
		/*do mutex_lock*/
		*args = mutex_lock(args[0]);
		break;
	case MUTEX_UNLOCK:
		/*do mutex_unlock*/
		*args = mutex_unlock(args[0]);
		break;
	default:
		*args = -0xbadc0de;
	}
}
