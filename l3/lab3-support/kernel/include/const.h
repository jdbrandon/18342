/* @file const.h
   @Description - contains values that are reused throughout
      the kernel
   @Author Jeff Brandon <jdbrando@andrew.cmu.edu>
   @Author Keane Lucas  <kjlucas@andrew.cmu.edu>
   @Date 11-06-2014
*/
#ifndef _CONST_VALS_H_
#define _CONST_VALS_H_

#define SWI_VEC 	0x08
#define IRQ_VEC 	0x18
#define SDRAM_BASE 	0xa0000000
#define SDRAM_LIMIT 	0xa3ffffff
#define SFROM_BASE 	0x0
#define SFROM_LIMIT 	0xffffff 
#define EOT 		4
#define BACK_SPACE 	8
#define DELETE 		127
#define NEW_LINE 	10
#define RETURN 		13
#define ERROR_CASE 	((unsigned*) 0xbadc0de)
#define R8_VAL		0xa3ebefdc

#define USER_STACK_BASE	0xa0000000
#define IRQ_STACK_BASE 	0xa2500000

#endif /* _CONST_VALS_H_ */
