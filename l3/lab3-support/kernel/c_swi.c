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
void dotime(unsigned*, unsigned*);
extern void exit_user(unsigned, unsigned, unsigned);

/* global variables */
extern unsigned lr_k; 		//store value of kernel link register  
extern unsigned sp_k;		//store value of kernel stack pointer
extern volatile unsigned interrupt;
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
	case EXIT_SWI:
		/* exit */
		exit_user(lr_k, sp_k, *args);
		break;
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
		dotime(args, ret);
		break;
	case SLEEP_SWI:
		/*do sleep*/
		dosleep(args);
		break;
	default:
		*args = -0xbadc0de;
	}
//	puts("goin back to userland...\n");
}

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

void dotime(unsigned* args, unsigned* ret){
	mmio_t oscr = (mmio_t)OSCR;
	uint64_t currenttime = (uint64_t)(unsigned)(((uint64_t)0xffffffff + (uint64_t)(1)) + (uint64_t)*oscr - (uint64_t)start_time);
	currenttime += ((uint64_t)rollovercount)<<32;
//	printf("roll:%d\n", rollovercount);
	*ret = (unsigned)(currenttime / TIME_CONVERT_CONST);
}

void dosleep(unsigned* args){
	//wait for time to elapse
	unsigned start = 0;
	unsigned now = 0;
	if(args[0]){
		dotime(args, &start);
		now = start;
		if(start+args[0] > start){ 
			while(now < (start + args[0]))
				dotime(args, &now);
		} else {
			while((now/10) > 0)
				dotime(args, &now);			
			while(now < (start + args[0]))
				dotime(args, &now);
		}
	}
}
