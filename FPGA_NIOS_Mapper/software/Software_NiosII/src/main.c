/**
 * @file    main.c
 * @author  Azeol
 * @brief   Main file for Nios II application
 * @version 0.1
 * @date    2025-10-14
 */

// Standard lib
#include <stdio.h>
#include <stdint.h>

// Project libs
#include "drivers/hex.h"
#include "drivers/i2c.h"

int main()
{
  printf("Hello from Nios II!\n");
  //testing hex
  hex_display("123456", 6, 0);

  return 0;
}
