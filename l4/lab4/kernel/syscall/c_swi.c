/* @file c_swi.c

   @description - swi handler written in C to be called from assembly after initial 
      handling of the intterupt (preparing swi number and args)

   @author Jeff Brandon <jdbrando@andrew.cmu.edu>
   @author Keane Lucas  <kjlucas@andrew.cmu.edu>

   @date 2014-11-06
*/

#include <bits/swi.h>
#include <bits/fileno.h>
#include <bits/errno.h>
#include <exports.h>
#include <const.h>
#include <arm/timer.h>
#include <arm/interrupt.h>

void doread(unsigned*, unsigned*);
void dowrite(unsigned*, unsigned*);
void dosleep(unsigned*);
void dotime(unsigned*);

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
	default:
		*args = -0xbadc0de;
	}
}
