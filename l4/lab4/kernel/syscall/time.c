/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * @author Jeff Brandon <jdbrando@andrew.cmu.edu>
 * @author Keane Lucas <kjlucas@andrew.cmu.edu>
 * @date   Nov 23, 2014
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>

extern volatile unsigned rollovercount;
extern volatile unsigned start_time;

/* dotime implements the time system call. Current time is calculated using a start time offset
      generated at kernel boot time and a rollover count. Rollover count keeps track of the 
      number of times OSCR has passed the start time and is used to add an appropriate number of
      seconds to the system time for each rollover that has occured.

   Parameters:
	ret - output parameter that is set to current system time in miliseconds.
   Returns:
	Current time in miliseconds since system boot in the value pointed to by ret.
*/
void dotime(unsigned* ret){
	mmio_t oscr = (mmio_t)OSCR;
	uint64_t currenttime = (uint64_t)(unsigned)(((uint64_t)0xffffffff + (uint64_t)(1)) + (uint64_t)*oscr - (uint64_t)start_time);
	currenttime += ((uint64_t)rollovercount)<<32;
	*ret = (unsigned)(currenttime / TIME_CONVERT_CONST);
}

/* dosleep implements the sleep system call. Busy wait solution that waits for system time to be
	equal to start time plus the number of miliseconds passed as a parameter.

   Parameters:
	args - args[0] is the number of miliseconds to sleep.

   Returns: nothing

   Note: This implementation of sleep handles the edge case when (current_time+sleep_time) overflows
         a 32 bit value.
*/
void dosleep(unsigned* args){
	//wait for time to elapse
	unsigned start = 0;
	unsigned now = 0;
	if(args[0]){
		dotime(&start);
		now = start;
		if(start+args[0] > start){ 
			while(now < (start + args[0]))
				dotime(&now);
		} else {
			while((now/10) > 0) //make sure loop exits in case 0 isn't sampled
				dotime(&now);			
			while(now < (start + args[0]))
				dotime(&now);
		}
	}
}
