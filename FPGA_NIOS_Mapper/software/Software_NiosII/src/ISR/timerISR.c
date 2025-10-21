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
#include "sensors/LiDAR.h"
#include "sensors/MPU6050.h"
#include "drivers/uart.h"
#include "drivers/hex.h"
#include "drivers/i2c.h"

// Std
#include <stddef.h>
#include <stdint.h>

// Local struct

typedef struct {
    alt_u32 timestamp;        // Timestamp in ms
    alt_u16 lidar_distance;   // LiDAR distance in mm
    alt_u16 lidar_strength;   // LiDAR signal strength
    alt_16 ax, ay, az;         // Acceleration in g
    alt_16 gx, gy, gz;         // Gyroscope in deg/s
} data_PC_t;

// Variables

static data_PC_t sensor_data;
static mpu6050_data_t imu_data;
static alt_u32 timestamp_counter;
int err;

// UART Helpers (little-endian)
static inline void uart_send_u16le(alt_u32 base, alt_u16 v) {
    uart_send_char(base, (char)(v & 0xFF));
    uart_send_char(base, (char)((v >> 8) & 0xFF));
}
static inline void uart_send_s16le(alt_u32 base, alt_16 v) {
    uart_send_u16le(base, (alt_u16)v);
}
static inline void uart_send_u32le(alt_u32 base, alt_u32 v) {
    uart_send_char(base, (char)(v & 0xFF));
    uart_send_char(base, (char)((v >> 8) & 0xFF));
    uart_send_char(base, (char)((v >> 16) & 0xFF));
    uart_send_char(base, (char)((v >> 24) & 0xFF));
}

/* REMINDER : TIMER_CONTROL/STATUS register :
 * CONTROL : last 4 bits : STOP, START, CONT, ITO
 * STATUS  : last 2 bits : RUN, TO
 */

/**
 * @brief INIT Main TIMER
 *  This timer generates the interrupts required for the MAIN timer at 1 kHz
 *  It is configured to generate an interrupt every second
 *  (50,000 cycles of the clock running at 50 MHz)
 */
void init_isrTimer_MAIN()
{
    printf("Main timer ISR initialized.\n");
    // Configure and initialize the interrupt
    alt_ic_isr_register(TIMER_MAIN_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_MAIN_IRQ, (void *)isrTimer_MAIN, NULL, 0x0);

    // Timer configuration
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_MAIN_BASE, (50000 & 0xFFFF));         // Define the first 16 bits of the timer
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_MAIN_BASE, ((50000 >> 16) & 0xFFFF)); // Define the last 16 bits of the timer
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_MAIN_BASE, 7); // STOP = 0, START = 1, CONT = 1, ITO = 1

    return;
}

/**
 * @brief Timer Interrupt Service Routine for MAIN timer only to be called by init_isrTimer_MAIN()
 * 
 * @param context Isn't necessary
 * @param id      Isn't necessary
 */
void isrTimer_MAIN(void *context, alt_u32 id)
{
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_MAIN_BASE, 0); // RESET the interrupt /!\ to do it each time

    // Read user inputs
    int press = IORD_ALTERA_AVALON_PIO_DATA(PB_BASE);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PB_BASE, 0); // Clear buttons after read

    //if (press == 0b01)
    //{
    //    // Reset timestamp counter
    //    printf("Timestamp counter reset by user!\n");
    //    timestamp_counter = 0;
    //} else if (press == 0b10)
    //{
    //    // Pause timestamp counter for an iteration
    //    printf("Timestamp counter paused for one iteration.\n");
    //    return;
    //} else if (press == 0b01)
    //{
    //    alt_irq_disable_all();
    //    (void)hex_display("000000", 6, 0); // Clear display
    //    for(int i = 0; i < 10; i++)
    //    {
    //        (void)led_SetLed(9 - i, 1); // Light up LEDs one by one
    //    }
    //    printf("System reset triggered by user!\n");
    //    usleep(2000000); // 2s delay before reset
    //    ((void (*)(void))NIOS2_RESET_ADDR)();
    //}
    

    // Call LiDAR FSM to step
    lidar_isr_step();

    // Read raw IMU data (14-byte burst, no float math in ISR)
    err = mpu6050_read_conv(&g_imu,
                           &imu_data.ax, &imu_data.ay, &imu_data.az,
                           &imu_data.gx, &imu_data.gy, &imu_data.gz,
                           &imu_data.temp_raw);
    if (err != 0) {
        printf("MPU6050 read error %d\n", err);
    }

    // Update sensor data structure
    sensor_data.timestamp = timestamp_counter;
    sensor_data.lidar_distance = s_latest.distance;
    sensor_data.lidar_strength = s_latest.strength;
    sensor_data.ax = imu_data.ax;
    sensor_data.ay = imu_data.ay;
    sensor_data.az = imu_data.az;
    sensor_data.gx = imu_data.gx;
    sensor_data.gy = imu_data.gy;
    sensor_data.gz = imu_data.gz;

    // Send a compact binary frame over UART (little-endian fields)
    uart_send_u32le(UART_PC_BASE, sensor_data.timestamp);
    uart_send_u16le(UART_PC_BASE, sensor_data.lidar_distance);
    uart_send_u16le(UART_PC_BASE, sensor_data.lidar_strength);
    uart_send_s16le(UART_PC_BASE, sensor_data.ax);
    uart_send_s16le(UART_PC_BASE, sensor_data.ay);
    uart_send_s16le(UART_PC_BASE, sensor_data.az);
    uart_send_s16le(UART_PC_BASE, sensor_data.gx);
    uart_send_s16le(UART_PC_BASE, sensor_data.gy);
    uart_send_s16le(UART_PC_BASE, sensor_data.gz);

    // Hex display of timestamp on HEX5 and HEX4
    hex_display(sensor_data.timestamp, 2, 0);

    // 2Hz Heartbeat LED toggle
    static int led_state = 0;
    if (timestamp_counter % 500 == 0) {
        led_state = !led_state;
    }
    err = led_SetLed(0, led_state);
    if (err != 0)
    {
        printf("LED indication failed with %d!\n", err);
    }

    // Increment timestamp
    timestamp_counter += 1;
    return;
}