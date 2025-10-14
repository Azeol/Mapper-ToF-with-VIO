/**
 * @file    i2c.c
 * @author  Azeol
 * @brief   I2C driver implementation using altera_avalon_i2c
 * @version 0.1
 * @date    2025-10-14
 */

 // Standard lib
#include <stdio.h>
#include <stdint.h>

// Altera lib
#include <altera_avalon_i2c.h>
#include <altera_avalon_pio_regs.h>
#include <system.h>

// Project lib
#include "drivers/i2c.h"

alt_avalon_i2c_register_optional_irq_handler(
    I2C_BASE,
    
)