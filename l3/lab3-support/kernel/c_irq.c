#include <types.h>
#include <arm/timer.h>

/* global variables */
unsigned instr1;	//first instruction we clobber
unsigned instr2;	//second instruction we clobber
extern unsigned lr_k; 		//store value of kernel link register  
extern unsigned sp_k;		//store value of kernel stack pointer

extern volatile unsigned interrupt;
extern volatile unsigned rollovercount;
extern volatile unsigned start_time;

extern void exit_user(unsigned, unsigned, unsigned);

/* c_irq_handler - custom irq handler called by assembly irq handler 
   after state has been saved/restored appropriately.
	Parameters: None 
*/
void c_irq_handler(){
	mmio_t ossr = (mmio_t)OSSR;
	//set global boolean true to allow sleep function to continue
	if(*ossr * OSSR_M0){
		interrupt = 1;
		*ossr |= OSSR_M0;
	}
	if(*ossr & OSSR_M1){
		rollovercount++;
		*ossr |= OSSR_M1;
	}
	if(*ossr & OSSR_M2)
		*ossr |= OSSR_M2;
	if(*ossr & OSSR_M3)
		*ossr |= OSSR_M3;
}
