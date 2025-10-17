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

#endif /* TIMERISR_H_ */