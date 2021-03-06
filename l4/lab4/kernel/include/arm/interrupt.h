/**
 * @file interrupt.h
 *
 * @brief Definitions for the interrupt controller.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 *
 * @date 2008-07-21
 *
 * @note The addresses here are the addresses stated in the Intel PXA255
 *       Processor Developer's Manual minus 0x40000000.  This is so that
 *       this memory region can be relocated if we ever turn on the MMU.
 */

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#define INTERRUPT_BASE 0x40d00000
#define ICIP (INTERRUPT_BASE + 0x0)  /* Interrupt controller IRQ pending register */
#define ICMR (INTERRUPT_BASE + 0x4)  /* Interrupt controller mask register */
#define ICLR (INTERRUPT_BASE + 0x8)  /* Interrupt controller level register */
#define ICFP (INTERRUPT_BASE + 0xc)  /* Interrupt controller FIQ pending register */
#define ICPR (INTERRUPT_BASE + 0x10) /* Interrupt controller pending register */
#define ICCR (INTERRUPT_BASE + 0x14) /* Interrupt controller control register */

#define INT_ICCR_DIM    0x00000001  /* Activate Idle Mask and ICMR */

#define INT_HUART        7    /* HUART Service Request Interrupt */
#define INT_GPIO_0       8    /* GPIO[0] Edge Detect Interrupt */
#define INT_GPIO_1       9    /* GPIO[1] Edge Detect Interrupt */
#define INT_GPIO_HI     10    /* GPIO[84:2] Edge Detect Interrupt */
#define INT_USB         11    /* USB Service Interrupt */
#define INT_PMU         12    /* PMU Interrupt */
#define INT_I2S         13    /* I2S Interrupt */
#define INT_AC97        14    /* AC97 Interrupt */
#define INT_NSSP        16    /* Network SSP Service Request Interrupt */
#define INT_LCD         17    /* LCD Controller Service Request Interrupt */
#define INT_I2C         18    /* I2C Service Request Interrupt */
#define INT_ICP         19    /* ICP Transmit/Receive/Error Interrupt */
#define INT_STUART      20    /* STUART Transmit/Receive/Error Interrupt */
#define INT_BTUART      21    /* BTUART Transmit/Receive/Error Interrupt */
#define INT_FFUART      22    /* FFUART Transmit/Receive/Error Interrupt */
#define INT_MMC         23    /* MMC Status/Error Detection Interrupt */
#define INT_SSP         24    /* SSP Service Request Interrupt */
#define INT_DMA         25    /* DMA Channel Service Request Interrupt */
#define INT_OSTMR_0     26    /* OS Timer Match Register 0 Interrupt */
#define INT_OSTMR_1     27    /* OS Timer Match Register 1 Interrupt */
#define INT_OSTMR_2     28    /* OS Timer Match Register 2 Interrupt */
#define INT_OSTMR_3     29    /* OS Timer Match Register 3 Interrupt */
#define INT_RTC_HZ      30    /* RTC HZ Clock Tick Interrupt */
#define INT_RTC_MATCH   31    /* RTC Alarm Match Register Interrupt */

#define INT_RESERVED_MASK  0x0000807f  /* Reserved bits that must be ignored */

#define NUM_INTERRUPTS  32

#ifndef ASSEMBLER

void interrupt_panic(unsigned int int_num) __attribute__((noreturn));
void init_interrupt(void);
void destroy_interrupt(void);
void request_reschedule(void);
void install_int_handler(unsigned int int_num, void (*int_handler)(unsigned int))
	__attribute__((nonnull));

#endif /* ASSEMBLER */

#endif /* _INTERRUPT_H_ */
