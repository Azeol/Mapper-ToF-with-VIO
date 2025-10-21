/**
 * @file LiDAR.h
 * @author Azeol
 * @brief LiDAR sensor driver implementation
 * @version 0.1
 * @date 2025-10-17
 * 
 */

#ifndef LIDAR_H_
#define LIDAR_H_

#define LIDAR_HEADER        0x59u  // Frame header of data package
#define LIDAR_DATA_LENGTH   UART_LIDAR_DATA_BITS     // Total bytes in a data package

typedef struct {
    alt_u16 distance;    // Distance measurement in mm
    alt_u16 strength;    // Signal strength
    float temperature;    // Chip temperature in Celsius
} LiDAR_Data_t;

// Global variables

static alt_u8 s_data_buffer[LIDAR_DATA_LENGTH];

// Global function prototypes

void lidar_init(alt_u32 base, alt_u32 sys_clk_freq, alt_u32 baud_rate);
int lidar_read_data(alt_u32 base, LiDAR_Data_t* data);
int lidar_try_get(alt_u32 base, LiDAR_Data_t* data);

/*
Quick usage:
    // Init once (after clocks/peripherals are ready)
    lidar_init(UART_LIDAR_BASE, 50000000u, 115200u);

    // In your timer ISR (see timerISR.c):
    // void isrTimer_MAIN(...) { ... lidar_isr_step(); }

    // In your main loop, poll for new data
    LiDAR_Data_t d;
    if (lidar_try_get(UART_LIDAR_BASE, &d) == 1) {
        // use d.distance (mm), d.strength, d.temperature (°C)
    }
*/

#endif /* LIDAR_H_ */