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
#include "alt_types.h"
#include "altera_avalon_i2c.h"
#include "system.h"

// Project lib
#include "drivers/i2c.h"

/**
 * @brief Initialize the I2C interface to the specified slave device.
 * 
 * @param i2c_name                  The name of the I2C device.
 * @param slave_address             The address of the I2C slave device.
 * @return ALT_AVALON_I2C_DEV_t*    A pointer to the I2C device structure, or NULL on failure.
 */
ALT_AVALON_I2C_DEV_t* i2c_init(const char* i2c_name, uint8_t slave_address)
{
    ALT_AVALON_I2C_DEV_t* i2c_dev = alt_avalon_i2c_open(i2c_name);

    if (i2c_dev == NULL) 
    {
        printf("Failed to open I2C device: %s\n", i2c_name);
        return NULL;
    }

    alt_avalon_i2c_master_target_set(i2c_dev, slave_address);

    // Speed and other configurations are set to defaults
    // If needed, configure i2c_config here and apply it
    // with i2c_config properly initialized and set

    // alt_avalon_i2c_master_config_set(i2c_dev, &i2c_config);

    printf("I2C device %s initialized with slave address 0x%02X\n", i2c_name, slave_address);
    return i2c_dev;
}

/**
 * @brief Write data to the I2C bus.
 * 
 * @param i2c_dev   The I2C device structure.
 * @param data      The data buffer to send.
 * @param length    The length of the data buffer.
 * @return int      0 on success, -1 on failure.
 * 
 */
ALT_AVALON_I2C_STATUS_CODE i2c_write(ALT_AVALON_I2C_DEV_t* i2c_dev, alt_u8* data, size_t length)
{
    if (!i2c_dev || !data || length == 0) 
    {
        return ALT_AVALON_I2C_ERROR; // Invalid parameters
    }

    int result = alt_avalon_i2c_master_tx(i2c_dev, data, length, ALT_AVALON_I2C_NO_INTERRUPTS);
    if (result != 0) 
    {
        printf("I2C write failed with error code: %d\n", result);
        return ALT_AVALON_I2C_ERROR;
    }

    return ALT_AVALON_I2C_SUCCESS;
}

/**
 * @brief Read data from the I2C bus.
 * 
 * @param i2c_dev   The I2C device structure.
 * @param buffer    The buffer to store the received data.
 * @param length    The length of the data to read.
 * @return int      0 on success, -1 on failure.
 */
ALT_AVALON_I2C_STATUS_CODE i2c_read(ALT_AVALON_I2C_DEV_t* i2c_dev, alt_u8* buffer, size_t length)
{
    if (!i2c_dev || !buffer || length == 0) 
    {
        return ALT_AVALON_I2C_ERROR; // Invalid parameters
    }

    int result = alt_avalon_i2c_master_rx(i2c_dev, buffer, length, ALT_AVALON_I2C_NO_INTERRUPTS);

    if (result != 0) 
    {
        printf("I2C read failed with error code: %d\n", result);
        return ALT_AVALON_I2C_ERROR;
    }

    return ALT_AVALON_I2C_SUCCESS;
}

/**
 * @brief Perform a combined I2C write followed by a read operation.
 * 
 * @param i2c_dev   The I2C device structure.
 * @param tx        The data buffer to send.
 * @param tx_length The length of the data buffer to send.
 * @param rx        The buffer to store the received data.
 * @param rx_length The length of the data to read.
 * @return ALT_AVALON_I2C_STATUS_CODE 
 */
ALT_AVALON_I2C_STATUS_CODE i2c_write_read(ALT_AVALON_I2C_DEV_t* i2c_dev,
                   alt_u8* tx, size_t tx_length,
                   alt_u8* rx, size_t rx_length)
{
    if( !i2c_dev || !tx || tx_length == 0 || !rx || rx_length == 0) 
    {
        return ALT_AVALON_I2C_ERROR; // Invalid parameters
    }

    return alt_avalon_i2c_master_tx_rx(i2c_dev, tx, tx_length, rx, rx_length, ALT_AVALON_I2C_NO_INTERRUPTS);
}