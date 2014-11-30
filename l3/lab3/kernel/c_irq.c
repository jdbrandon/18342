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

extern volatile unsigned rollovercount;

/* c_irq_handler - custom irq handler called by assembly irq handler 
   after state has been saved/restored appropriately.
	Parameters: None 
*/
void c_irq_handler(){
	mmio_t ossr = (mmio_t)OSSR;
	if(*ossr * OSSR_M0){
		rollovercount++;
		*ossr |= OSSR_M0;
	}
	if(*ossr & OSSR_M1)
		*ossr |= OSSR_M1;
	if(*ossr & OSSR_M2)
		*ossr |= OSSR_M2;
	if(*ossr & OSSR_M3)
		*ossr |= OSSR_M3;
}
