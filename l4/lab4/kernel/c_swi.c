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
extern volatile unsigned rollovercount;
extern volatile unsigned start_time;

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

/* doread performs the read system call
   Parameters:
	args - arguments passed to the function. args[0] is the file descriptor
		of the file to read from, args[1] is the address of the buffer
		to fill, and args[2] is the number of bytes to read.
	ret - output value set by doread on completion.
   Returns:
	On completion ret will be the number of bytes read if
        read is successful.
        EBADF is returned when the input file descriptor isn't stdin.
        EFAULT is returned when the buffer read into is outside valid bounds.
*/
void doread(unsigned* args, unsigned* ret){
	int fileno, c;
	char* buf;
	size_t count;
	size_t pcount = 0; //processed char count
	fileno = (int) *args++;
	buf = (char*) *args++;
	count = *args;
	if(fileno != STDIN_FILENO){
		*ret = -EBADF;
		return;
	}
	if(!(buf >= (char*)SDRAM_BASE && (buf+count) <= (char*)SDRAM_LIMIT)){
		*ret = -EFAULT;
		return;
	}
	char* bufstart = buf;
	while(pcount++ < count){
		c = getc();
		if(c == EOT){
			*ret = pcount - 1;
			break;
		}
		if(c == BACK_SPACE || c == DELETE){
			if(buf > bufstart)
				buf--;
			if(pcount-- > 1)
				pcount--; //don't count input character and the one it deleted
			/* make terminal display accurate */
			if(buf >= bufstart)
				printf("\b \b");
			continue;
		}
		if(c == NEW_LINE || c == RETURN){
			*buf++ = '\n';
			*buf = 0;
			/* map \r to \n */
			printf("\n");
			*ret = pcount;
			break;
		}
		/* echo input character */
		printf("%c", c);
		*buf++ = c;
	}
}

/* dowrite performs the write system call

   Parameters:
	args - arguments to the function. arg[0] is the file descriptor to write to,
		args[1] is the address of the buffer to write from, and args[2] is 
		the number of bytes that should be written.
	ret - output parameter set on completion of dowrite.
   Returns:
	On completion ret will contain the number of bytes written on success.
	EBADF will be returned if the file descriptor to write to isn't stdout.
	EFAULT will be returned if the buffer isn't within valid bounds.
*/
void dowrite(unsigned* args, unsigned* ret){
	int fileno;
	char* buf;
	size_t count;
	size_t pcount = 0;
	fileno = (int) *args++;
	buf = (char*) *args++;
	count = *args;
	if(fileno != STDOUT_FILENO){
		*ret = -EBADF;
		return;
	}
	if(!((buf >= (char*) SDRAM_BASE && (buf+count) <= (char*) SDRAM_LIMIT) ||
	     (buf >= (char*) SFROM_BASE && (buf+count) <= (char*) SFROM_LIMIT))){
		*ret = -EFAULT;
		return;
	}
	while(*buf){
		printf("%c",*buf++);
		pcount++;
	}
	*ret = pcount;
}

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
