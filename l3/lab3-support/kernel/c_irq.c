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


extern void exit_user(unsigned, unsigned, unsigned);

#include <types.h>
#include <bits/swi.h>
#include <bits/fileno.h>
#include <bits/errno.h>
#include <exports.h>

/* global variables */
unsigned instr1;	//first instruction we clobber
unsigned instr2;	//second instruction we clobber
extern unsigned lr_k; 		//store value of kernel link register  
extern unsigned sp_k;		//store value of kernel stack pointer

extern unsigned interrupt;

/* c_irq_handler - custom irq handler called by assembly irq handler 
   after state has been saved/restored appropriately.
	Parameters: None 
*/
void c_irq_handler(){
	puts("caught interrupt yo\n");
	interrupt = 1;
	//set global boolean true to allow sleep function to continue

}
