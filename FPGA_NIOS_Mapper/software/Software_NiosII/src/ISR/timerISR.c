/**
 * @file    timerISR.c
 * @author  Azeol
 * @brief   Timer Interrupt Service Routine (ISR) implementation
 * @version 0.1
 * @date    2025-10-15
 * 
 */

 // Standard lib
#include <stdio.h>
#include <unistd.h>

// Altera lib
#include <alt_types.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"
#include "sys/alt_irq.h"

// Project lib
#include "ISR/timerISR.h"

/* REMINDER : TIMER_CONTROL/STATUS register :
 * CONTROL : last 4 bits : STOP, START, CONT, ITO
 * STATUS  : last 2 bits : RUN, TO
 */

/**
 * @brief INIT ToF Camera TIMER
 *  This timer generates the interrupts required for the ToF camera operating at 80 Hz
 *  It is configured to generate an interrupt every second
 *  (625,000 cycles of the clock running at 50 MHz)
 */
void init_isrTimer_ToF()
{
    // Configure and initialize the interrupt
    alt_ic_isr_register(TIMER_TOF_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_TOF_IRQ, (void *)isrTimer_ToF, NULL, 0x0);

    // Timer configuration (see p.116 of the course)
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_TOF_BASE, (625000 & 0xFFFF));         // Define the first 16 bits of the timer
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_TOF_BASE, ((625000 >> 16) & 0xFFFF)); // Define the last 16 bits of the timer
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_TOF_BASE, 7); // STOP = 0, START = 1, CONT = 1, ITO = 1

    return;
}

/**
 * @brief INIT ToF Camera TIMER
 *  This timer generates the interrupts required for the ToF camera operating at 1 kHz
 *  It is configured to generate an interrupt every second
 *  (50,000 cycles of the clock running at 50 MHz)
 */
void init_isrTimer_IMU()
{
    // Configure and initialize the interrupt
    alt_ic_isr_register(TIMER_IMU_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_IMU_IRQ, (void *)isrTimer_IMU, NULL, 0x0);

    // Timer configuration (see p.116 of the course)
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_IMU_BASE, (50000 & 0xFFFF));         // Define the first 16 bits of the timer
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_IMU_BASE, ((50000 >> 16) & 0xFFFF)); // Define the last 16 bits of the timer
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_IMU_BASE, 7); // STOP = 0, START = 1, CONT = 1, ITO = 1

    return;
}

/**
 * @brief Timer Interrupt Service Routine for ToF Camera
 * 
 * @param context   Isn't necessary here, can be NULL
 * @param id      Isn't necessary here, can be 0
 */
void isrTimer_ToF(void *context, alt_u32 id)
{
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_TOF_BASE, 0); // RESET the interrupt /!\ to do it each time

    // Code here

    return;
}

/**
 * @brief Timer Interrupt Service Routine for IMU
 * 
 * @param context   Isn't necessary here, can be NULL
 * @param id        Isn't necessary here, can be 0
 */
void isrTimer_IMU(void *context, alt_u32 id)
{
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_IMU_BASE, 0); // RESET the interrupt /!\ to do it each time

    // Code here

    return;
}