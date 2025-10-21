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
#include "sensors/LiDAR.h"
#include "sensors/MPU6050.h"

/* REMINDER : TIMER_CONTROL/STATUS register :
 * CONTROL : last 4 bits : STOP, START, CONT, ITO
 * STATUS  : last 2 bits : RUN, TO
 */

/**
 * @brief INIT Main TIMER
 *  This timer generates the interrupts required for the MAIN timer at 0.8 kHz
 *  It is configured to generate an interrupt every second
 *  (62,500 cycles of the clock running at 50 MHz)
 */
void init_isrTimer_MAIN()
{
    // Configure and initialize the interrupt
    alt_ic_isr_register(TIMER_MAIN_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_MAIN_IRQ, (void *)isrTimer_MAIN, NULL, 0x0);

    // Timer configuration
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_MAIN_BASE, (62500 & 0xFFFF));         // Define the first 16 bits of the timer
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_MAIN_BASE, ((62500 >> 16) & 0xFFFF)); // Define the last 16 bits of the timer
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_MAIN_BASE, 7); // STOP = 0, START = 1, CONT = 1, ITO = 1

    return;
}

/**
 * @brief Timer Interrupt Service Routine for MAIN timer only to be called by init_isrTimer_MAIN()
 * 
 * @param context Isn't necessary
 * @param id      Isn't necessary
 */
void isrTimer_MAIN(void *context, alt_u32 id)
{
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_MAIN_BASE, 0); // RESET the interrupt /!\ to do it each time

    lidar_isr_step();
    mpu6050_read_conv(&g_imu, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    return;
}