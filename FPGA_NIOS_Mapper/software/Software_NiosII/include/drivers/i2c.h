/**
 * @file    i2c.h
 * @author  Azeol
 * @brief   I2C driver header file
 * @version 0.1
 * @date    2025-10-14
 */

#ifndef I2C_H_
#define I2C_H_

/* I2C device configuration and variables */

#define I2C_NAME "/dev/I2C"      // Defined in system.h
#define I2C_IMU_ADDRESS 0x51            // Specified in the IMU datasheet, see documentation folder

/* Function prototypes */

ALT_AVALON_I2C_DEV_t* i2c_init(const char* i2c_name, uint8_t slave_address);
ALT_AVALON_I2C_STATUS_CODE i2c_write(ALT_AVALON_I2C_DEV_t* i2c_dev, alt_u8* data, size_t length);
ALT_AVALON_I2C_STATUS_CODE i2c_read(ALT_AVALON_I2C_DEV_t* i2c_dev, alt_u8* data, size_t length);
ALT_AVALON_I2C_STATUS_CODE i2c_write_read(ALT_AVALON_I2C_DEV_t* i2c_dev,
                                          alt_u8* tx, size_t tx_length,
                                          alt_u8* rx, size_t rx_length);

#endif // I2C_H_