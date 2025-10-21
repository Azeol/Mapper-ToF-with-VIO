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
#include "sensors/MPU6050.h"
#include "sensors/LiDAR.h"

// I2C variables

ALT_AVALON_I2C_DEV_t* i2c_dev;              // Pointer to I2C device structure
ALT_AVALON_I2C_MASTER_CONFIG_t i2c_config;  // I2C configuration structure
ALT_AVALON_I2C_STATUS_CODE i2c_status;      // I2C status code
alt_u8 i2c_txbuffer[0x210];                 // Transmission buffer
alt_u8 i2c_rxbuffer[0x200];                 // Reception buffer

// MPU6050 variables

static int mpu6050_initialization; // Status variable
static alt_u8 g_who; // WHO_AM_I Variable

int main()
{
  printf("Nios II successfully initialized\n");

  //testing hex 
  hex_display("hhhhhh", 6, 0);

  // Initialize I2C
  i2c_dev = i2c_init(I2C_NAME, I2C_IMU_ADDRESS);
  if (i2c_dev == NULL) 
  {
      printf("I2C initialization failed!\n");
      return -1;
  }
  //IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 0); // Indicate I2C init success

  // IMU Configuration
  mpu6050_set_sample_rate_div(&g_imu, 0); // max Hz (1kHz)
  mpu6050_set_dlpf(&g_imu, MPU6050_DLPF_260HZ); // max bandwidth
  mpu6050_set_accel_range(&g_imu, MPU6050_ACCEL_4G); // +-4G
  mpu6050_set_gyro_range(&g_imu, MPU6050_GYRO_500DPS); // +-500 deg/s

  // Sensors initialization
  lidar_init(UART_LIDAR_BASE, 50000000u, 115200u);
  //IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 1); // Indicate LiDAR init

  mpu6050_initialization = mpu6050_init(&g_imu, i2c_dev, 0);
  if (mpu6050_initialization != 0) 
  {
      printf("MPU6050 initialization failed with code %d\n", mpu6050_initialization);
      return -1;
  }
  //IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 2); // Indicate IMU init success

  // Verify MPU6050 identity
  mpu6050_who_am_i(&g_imu, &g_who); // expect 0x68
  printf("MPU6050: 0x%02X\n", g_who);

  hex_display("000000", 6, 0); // Indicate all inits done
  // Init timers for ISRs
  init_isrTimer_MAIN();

  return 0;
}
