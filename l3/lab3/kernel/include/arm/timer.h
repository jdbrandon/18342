/**
 * @file timer.h
 *
 * @brief Definitions for the OS timer.
 *
 * @author Kartik Subramanian 	<ksubrama@andrew.cmu.edu>
 * @author Jeff Brandon 	<jdbrando@andrew.cmu.edu>
 * @author Keane Lucas 		<kjlucas@andrew.cmu.edu>
 *
 * @date 2008-07-07 (original authored)
 * @date 2014-11-06 (Jeff refactored)
 *
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#define TIMER_BASE_ADDR 0x40a00000
#define OSMR_0 (TIMER_BASE_ADDR + 0x0) /* OS Timer Match register 0 */
#define OSMR_1 (TIMER_BASE_ADDR + 0x4) /* OS Timer Match register 1 */
#define OSMR_2 (TIMER_BASE_ADDR + 0x8) /* OS Timer Match register 2 */
#define OSMR_3 (TIMER_BASE_ADDR + 0xc) /* OS Timer Match register 3 */
#define OSCR (TIMER_BASE_ADDR + 0x10) /* OS Timer counter register */
#define OSSR (TIMER_BASE_ADDR + 0x14) /* OS Timer status register */
#define OWER (TIMER_BASE_ADDR + 0x18) /* OS Timer watchdog enable register */
#define OIER (TIMER_BASE_ADDR + 0x1c) /* OS Timer interrupt enable register */

#define OSSR_M0         0x00000001   /* Matched 0 */
#define OSSR_M1         0x00000002   /* Matched 1 */
#define OSSR_M2         0x00000004   /* Matched 2 */
#define OSSR_M3         0x00000008   /* Matched 3 */

#define OWER_WME        0x00000001   /* Watchdog mode enable */

#define OIER_E0         0x00000001   /* Enable match 0 */
#define OIER_E1         0x00000002   /* Enable match 1 */
#define OIER_E2         0x00000004   /* Enable match 2 */
#define OIER_E3         0x00000008   /* Enable match 3 */

#define OSTMR_FREQ            3686400      /* Oscillator frequency in hz */

#define TIME_CONVERT_CONST (OSTMR_FREQ / 1000)

#ifndef ASSEMBLER

/* Add your C code here.  Put your group name at the top! */

#endif /* ASSEMBLER */

#endif /* _TIMER_H_ */
