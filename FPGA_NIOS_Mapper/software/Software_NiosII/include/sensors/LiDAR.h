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
    uint16_t distance;    // Distance measurement in mm
    uint16_t strength;    // Signal strength
    float temperature;    // Chip temperature in Celsius
} LiDAR_Data_t;

void lidar_init(alt_u32 base, alt_u32 sys_clk_freq, alt_u32 baud_rate);
int lidar_read_data(alt_u32 base, LiDAR_Data_t* data);
int lidar_try_get(alt_u32 base, LiDAR_Data_t* data);

#endif /* LIDAR_H_ */