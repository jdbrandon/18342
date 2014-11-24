/*
 * main.c: Kernel main (entry) function
 *
 * Author: Jeff Brandon <jdbrando@andrew.cmu.edu> 
 *	   Keane Lucas  <kjlucas@andrew.cmu.edu>
 * Date: put the date here
 */
 
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <exports.h>
#include <const.h>
#include <arm/timer.h>
#include <arm/interrupt.h>

/* swi_handler - Assembly function that performs preliminary tasks for swi 
   handling before calling a c swi handler.
*/
extern void swi_handler(void*);

/* irq_wrapepr- Assembly function that performs preliminary tasks for irq 
   handling before calling a c irq handler.
*/
extern void irq_wrapper(void*);

/* user_mode - Assembly function that switches processor state from service
   mode to user mode, then begins execution of a user program. 
	Parameters:
	argc - int: provided to kernel main and passed to user program main
	argv - char*[]: similar to argc, passed to user program main. 
	lr_ptr - unsigned*: a pointer to a variable where the kernel value
				of lr will be saved.
	sp_ptr - unsigned*: a pointer to a variable where the kernel value
				of sp will be saved.
	Returns: exit status of the user program.
*/
extern int user_mode(int, char*[], unsigned*, unsigned*);

/* function declarations */
unsigned *install_handler(int, interrupt_handler_t, unsigned*, unsigned*);
void start_timer();
void init_sched();

/* global variables */
unsigned swi_instr1;	//first instruction we clobber
unsigned swi_instr2;	//second instruction we clobber
unsigned irq_instr1;
unsigned irq_instr2;
unsigned lr_k; 		//store value of kernel link register  
unsigned sp_k;		//store value of kernel stack pointer
uint32_t global_data;
volatile unsigned rollovercount;
volatile unsigned start_time;

/* restore_old_handlers - Restores the instructions at the 
   uboot swi and irq handlers that are clobbered when this kernel
   installs its own handlers.
*/
inline void restore_old_handlers(unsigned* swiaddr, unsigned* irqaddr){
	unsigned* restore = swiaddr;
	*restore++ = swi_instr1;
	*restore = swi_instr2;
	
	restore = (unsigned*) irqaddr;
	*restore++ = irq_instr1;
	*restore = irq_instr2;
}

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{
	int ret;
	unsigned* swiaddr, * irqaddr;
	app_startup();
	global_data = table;
	swiaddr = install_handler(SWI_VEC, swi_handler, &swi_instr1, &swi_instr2);
	irqaddr = install_handler(IRQ_VEC, irq_wrapper, &irq_instr1, &irq_instr2);
	if(swiaddr == ERROR_CASE)
		puts("SWI_VEC has bad value!\n");
	if(irqaddr == ERROR_CASE)
		puts("IRQ_VEC has bad value!\n");
	init_timer();
	/* init other stuff */
	ret = user_mode(argc, argv, &lr_k, &sp_k);
	/* should never get here */
	restore_old_handlers(swiaddr, irqaddr);
	assert(0);        
}

/* init_timer sets up the system timer. It saves the boot time offset in 
      a global variable start_time. Also the global variable rollovercount
      is initialized to 0. Lastly the interrupt for rollovers occuring is set
      up by setting OSMR0, OIER, ICMR, and ICLR to appropriate values.
*/
void init_timer(){
        mmio_t oscr = (mmio_t)OSCR;
	start_time = *oscr;
	rollovercount = 0;
}

void start_timer(){
	mmio_t osmr0 = (mmio_t)OSMR_0;
        mmio_t oier = (mmio_t)OIER;
        mmio_t oscr = (mmio_t)OSCR;
        mmio_t icmr = (mmio_t)ICMR;
        mmio_t iclr = (mmio_t)ICLR;
	*osmr0 = *oscr+OSTMR_FREQ/100;
        *oier |= 0x1;
        *icmr |= 0x04000000;
        *iclr = 0x0;
}

/* install_handler - Injects a given custom handler at a specified location.
   Specifically, at the given address an instruction is written the causes
   execution to jump to the custom handler.
	Parameters:
	vec - Where the offset to where the handler should be written is found.
	handler - the address of the custom handler.
	store1 and store2 - used to save the values overwritten
*/
unsigned *install_handler(int vec, interrupt_handler_t handler, unsigned* store1, unsigned* store2){
//find UBOOT Handler
	unsigned* vector, * addr;
	vector = (unsigned *) vec; 
	if((*vector & 0xfffff000) != 0xe59ff000) // if instruction is not ldr return with error
		return ERROR_CASE; //it doesnt like me returning in a
	
	addr = (unsigned *)((*vector & 0x00000fff) + 0x8 + vec); //take offset in instruction and add pc offset (0x8) and initial vector to get absolute address
	addr = (unsigned *) *addr; //take offset in instruction and add pc of 0x10 (0x8 + 8)
	*store1 = *addr;
	*addr = 0xe51ff004; // ldr pc, [pc, #-4]
	*store2 = *(addr+1);
	*(addr+1) = (unsigned) handler;
	return addr;
}
