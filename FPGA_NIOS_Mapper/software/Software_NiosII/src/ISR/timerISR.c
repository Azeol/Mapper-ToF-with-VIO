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

/*  INIT TIMER COMPTAGE TEMPS
    Ce timer permet de generer les interruptions necessaires pour la camera ToF fonctionnant à 80 Hz
    Il est configure pour generer une interruption toutes les secondes
    (625 000 cycles de la clock allant a 50 MHz) */