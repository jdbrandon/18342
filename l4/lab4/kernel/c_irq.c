/* @file c_irq.c

   @description - irq interrupt handler. Currently only responds to
      OSMR0 interrupts in a defined way. After handling the interrupt
      the corresponding bit in OSSR is cleared.

   @author Keane Lucas  <kjlucas@andrew.cmu.edu>
   @author Jeff Brandon <jdbrando@andrew.cmu.edu>

   @date 2014-11-06
*/
#include <types.h>
#include <exports.h>
#include <arm/timer.h>

void dev_update(unsigned long);
void dispatch_save();
void dotime(unsigned*);

extern volatile unsigned rollovercount;
extern volatile unsigned start_time;

//initialize to highest value to avoid initial increment before it is set correctly
static unsigned lastoscr = 0xffffffff; 

/* c_irq_handler - custom irq handler called by assembly irq handler 
   after state has been saved/restored appropriately.
	Parameters: None 
*/
void c_irq_handler(){
	unsigned stamp;
	mmio_t oscr = (mmio_t)OSCR;
	mmio_t ossr = (mmio_t)OSSR;
	mmio_t osmr0 = (mmio_t)OSMR_0;
	if(*ossr & OSSR_M0){
		//keep global time by checking if oscr has overlapped start_time
		if(lastoscr<start_time && *oscr>=start_time){
			rollovercount++;
		}
		lastoscr = *oscr;
		dotime(&stamp);

		//set next interrupt for 10ms later
		*osmr0 = *oscr + OSTMR_FREQ/100;

		//unset interrupts
		*ossr |= OSSR_M0;

		//update devices and switch to highest prio task
		dev_update((unsigned long)stamp);
		dispatch_save();
	}
	if(*ossr & OSSR_M1)
		*ossr |= OSSR_M1;
	if(*ossr & OSSR_M2)
		*ossr |= OSSR_M2;
	if(*ossr & OSSR_M3)
		*ossr |= OSSR_M3;
}
