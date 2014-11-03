/*
 * main.c: Kernel main (entry) function
 *
 * Author: Jeff Brandon <jdbrando@andrew.cmu.edu> 
 *	   Keane Lucas  <keansemailaddress@andrew.cmu.edu>
 * Date: Sun Oct 12 19:14:20 UTC 2014
 */

/* Define constants in this region */
#define SWI_ADDR 0x5c0009c0
#define SWI_VEC 0x08
#define IRQ_VEC 0x18
#define SDRAM_BASE 0xa0000000
#define SDRAM_LIMIT 0xa3ffffff
#define SFROM_BASE 0x0
#define SFROM_LIMIT 0xffffff 
#define EOT 4
#define BACK_SPACE 8
#define DELETE 127
#define NEW_LINE 10
#define RETURN 13
#define ERROR_CASE ((unsigned*) 0xbadc0de)

/* include necessary header files */
#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <bits/swi.h>
#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

/* declare functions */

/* swi_handler - Assembly function that performs preliminary tasks for swi 
   handling before calling a c swi handler.
*/
extern void swi_handler(void*);

/* irq_handler - Assembly function that performs preliminary tasks for irq 
   handling before calling a c irq handler.
*/
extern void irq_handler(void*);

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

/* exit_user - performs steps to exit user mode
	Parameters:
	kernel_lr - value of kernel link register
	kernel_sp - value of kernel stack pointer
	exit_status - exit status of user program
*/
extern void exit_user(unsigned, unsigned, unsigned);

unsigned *install_handler(int, interrupt_handler_t, unsigned*, unsigned*);

/* global variables */
unsigned swi_instr1;	//first instruction we clobber
unsigned swi_instr2;	//second instruction we clobber
unsigned irq_instr1;
unsigned irq_instr2;
unsigned lr_k; 		//store value of kernel link register  
unsigned sp_k;		//store value of kernel stack pointer
uint32_t global_data;

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

/* main - installs custom swi handler, executes a user program, restores
   the default swi handler, and returns the exit status of the user program. 
*/
int kmain(int argc, char** argv, uint32_t table) {
	int ret;
	unsigned* swiaddr, * irqaddr;
	app_startup();
	global_data = table;
	swiaddr = install_handler(SWI_VEC, swi_handler, &swi_instr1, &swi_instr2);
	irqaddr = install_handler(IRQ_VEC, irq_handler, &irq_instr1, &irq_instr2);
	if(swiaddr == ERROR_CASE)
		puts("SWI_VEC has bad value!\n");
	if(irqaddr == ERROR_CASE)
		puts("IRQ_VEC has bad value!\n");
//	puts("1\n");
	ret = user_mode(argc, argv, &lr_k, &sp_k);
	restore_old_handlers(swiaddr, irqaddr);
	return ret;
}

/* install_handler - Injects a given custom handler at a specified location.
   Specifically, at the given address an instruction is written the causes
   execution to jump to the custom handler.
	Parameters:
	vec - The address of memory where the injected instructions 
		should be placed.
	handler - the address of the custom swi handler.
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
