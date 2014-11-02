/* Define constants in this region */
#define SWI_ADDR 0x5c0009c0
#define SDRAM_BASE 0xa0000000
#define SDRAM_LIMIT 0xa3ffffff
#define SFROM_BASE 0x0
#define SFROM_LIMIT 0xffffff 
#define EOT 4
#define BACK_SPACE 8
#define DELETE 127
#define NEW_LINE 10
#define RETURN 13
#define TIME_CONVERT_CONST 3686.4

/* Memory mapped io constants */
#define TIMER_BASE_ADDR 0x40a00000
#define OSMR_0 (TIMER_BASE_ADDR + 0x0) /* OS Timer Match register 0 */
#define OSMR_1 (TIMER_BASE_ADDR + 0x4) /* OS Timer Match register 1 */
#define OSMR_2 (TIMER_BASE_ADDR + 0x8) /* OS Timer Match register 2 */
#define OSMR_3 (TIMER_BASE_ADDR + 0xc) /* OS Timer Match register 3 */
#define OSCR (TIMER_BASE_ADDR + 0x10) /* OS Timer counter register */
#define OSSR (TIMER_BASE_ADDR + 0x14) /* OS Timer status register */
#define OWER (TIMER_BASE_ADDR + 0x18) /* OS Timer watchdog enable register */
#define OIER (TIMER_BASE_ADDR + 0x1c) /* OS Timer interrupt enable register */

#define INTERRUPT_BASE 0x40d00000
#define ICIP (INTERRUPT_BASE + 0x0)  /* Interrupt controller IRQ pending register */
#define ICMR (INTERRUPT_BASE + 0x4)  /* Interrupt controller mask register */
#define ICLR (INTERRUPT_BASE + 0x8)  /* Interrupt controller level register */
#define ICFP (INTERRUPT_BASE + 0xc)  /* Interrupt controller FIQ pending register */
#define ICPR (INTERRUPT_BASE + 0x10) /* Interrupt controller pending register */
#define ICCR (INTERRUPT_BASE + 0x14) /* Interrupt controller control register */

typedef volatile unsigned* mmio_t;

extern void exit_user(unsigned, unsigned, unsigned);

#include <types.h>
#include <bits/swi.h>
#include <bits/fileno.h>
#include <bits/errno.h>
#include <exports.h>

void doread(unsigned*, unsigned*);
void dowrite(unsigned*, unsigned*);
void dosleep(unsigned*);
void dotime(unsigned*, unsigned*);
/* global variables */
extern unsigned lr_k; 		//store value of kernel link register  
extern unsigned sp_k;		//store value of kernel stack pointer
unsigned interrupt;

/* c_swi_handler - custom swi handler called by assembly swi handler
   after state has been saved/restored appropriately.
	Parameters: 
	swi_num - the number the swi was invoked with.
	args - a pointer to arguments associated with the swi.
*/
void c_swi_handler(int swi_num, unsigned *args){
	unsigned *ret = args;
	puts("3\n");
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
}

void doread(unsigned* args, unsigned* ret){
	int fileno, c;
	char* buf;
	size_t count;;
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
	mmio_t currenttime = (unsigned *)OSCR;
	*ret = *currenttime / TIME_CONVERT_CONST;
}

void dosleep(unsigned* args){
	//set timer registers to interrupt when current time + specified time is reached
	mmio_t mytimeout = (unsigned *)OSMR_0;
	mmio_t interruptenable = (unsigned *) OIER;
	mmio_t currenttime = (unsigned *)OSCR;

	mytimeout = ((int)(args[0] * TIME_CONVERT_CONST)) + currenttime; 
	interruptenable = (unsigned *)0x1;
	//set global variable to false and wait for interrupt
	interrupt = 0;
	while(!interrupt);
}
