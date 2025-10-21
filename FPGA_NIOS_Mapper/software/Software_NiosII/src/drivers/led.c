/**
 * @file led.c
 * @author Azeol
 * @brief LED driver implementation
 * @version 0.1
 * @date 2025-10-21
 * 
 */

// Altera lib
#include <alt_types.h>
#include "altera_avalon_pio_regs.h"
#include "system.h"

// Project lib
#include "drivers/led.h"

int led_SetLed(int led, int state)
{
    // Input checks
    if ((led < 0) | (led > 9))
    {
        return -1;
    }

    // Read actual IO port status
    int actual = IORD_ALTERA_AVALON_PIO_DATA(LEDR_BASE);

    // Applying changes
    if (state) // led to be set
    {
        actual |= (1 << led);
    }
    else // led to be cleared
    {
        actual &= ~(1 << led);
    }

    // Perform IO
    IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, actual);
    return 0;
}