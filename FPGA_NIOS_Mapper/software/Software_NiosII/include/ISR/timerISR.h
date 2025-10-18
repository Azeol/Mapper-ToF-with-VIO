/**
 * @file timerISR.h
 * @author Azeol
 * @brief Timer Interrupt Service Routine (ISR) header file
 * @version 0.1
 * @date 2025-10-15
 * 
 */

#ifndef TIMERISR_H_
#define TIMERISR_H_

// Function prototypes

void init_isrTimer_MAIN();
void isrTimer_MAIN(void* context, alt_u32 id);

/*
Quick usage:
	// Initialize timer ISR once after system setup
	init_isrTimer_MAIN();

	// Your ISR implementation should acknowledge the timer and do minimal work.
	// Example (see src/ISR/timerISR.c):
	// void isrTimer_MAIN(void *context, alt_u32 id) {
	//     IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_MAIN_BASE, 0);
	//     // periodic work (e.g., lidar_isr_step());
	// }
*/

#endif /* TIMERISR_H_ */