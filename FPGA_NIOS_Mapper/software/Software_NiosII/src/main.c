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

// Altera lib
#include "alt_types.h"
#include "system.h"
#include "altera_avalon_i2c.h"

// Project libs
#include "drivers/hex.h"
#include "drivers/i2c.h"
#include "drivers/uart.h"
#include "ISR/timerISR.h"

// I2C variables

ALT_AVALON_I2C_DEV_t* i2c_dev;              // Pointer to I2C device structure
ALT_AVALON_I2C_MASTER_CONFIG_t i2c_config;  // I2C configuration structure
ALT_AVALON_I2C_STATUS_CODE i2c_status;      // I2C status code
alt_u8 i2c_txbuffer[0x210];                 // Transmission buffer
alt_u8 i2c_rxbuffer[0x200];                 // Reception buffer

int main()
{
  printf("Hello from Nios II!\n");

  //testing hex
  hex_display("123456", 6, 0);

  //testing i2c
  i2c_dev = i2c_init(I2C_NAME, I2C_IMU_ADDRESS);
  if (i2c_dev == NULL) 
  {
      printf("I2C initialization failed!\n");
      return -1;
  }

  alt_u8 tx_data[2] = {0x01, 0x02}; // test data to send
  alt_u8 rx_data[1];                // buffer to receive data

  if (i2c_write(i2c_dev, tx_data, sizeof(tx_data)) != 0) 
  {
      printf("I2C write failed!\n");
      return -1;
  }

  if (i2c_read(i2c_dev, rx_data, sizeof(rx_data)) != 0) 
  {
      printf("I2C read failed!\n");
      return -1;
  }

  printf("Received: 0x%02X\n", rx_data[0]);

  return 0;
}
