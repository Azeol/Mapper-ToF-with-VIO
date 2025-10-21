/**
 * @file LiDAR.c
 * @author Azeol
 * @brief LiDAR sensor driver implementation
 * @version 0.1
 * @date 2025-10-17
 * 
 */

// Standard lib
#include <stdint.h>
#include <stddef.h>

// Altera lib
#include "alt_types.h"
#include "system.h"
#include "sys/alt_irq.h"

// Project lib
#include "drivers/uart.h"
#include "sensors/LiDAR.h"

// LiDAR context
static alt_u32 s_uart_base = 0;

static volatile LiDAR_Data_t s_latest;
static volatile alt_u8       s_has_new = 0;

/* FSM */
enum{
    LIDAR_STATE_WAIT_HEADER_1 = 0,
    LIDAR_STATE_WAIT_HEADER_2 = 1,
    LIDAR_STATE_READ_DATA     = 2
};

static alt_u8 s_state = LIDAR_STATE_WAIT_HEADER_1;
static alt_u8 s_data_index = 0;

// Non global function prototypes

static inline alt_u8 lidar_checksum(const alt_u8* u, size_t UART_BASE_BITS);

/**
 * @brief Initialize the LiDAR sensor
 * 
 * @param base          Base address of the UART
 * @param sys_clk_freq  System clock frequency
 * @param baud_rate     UART baud rate
 */
void lidar_init(alt_u32 base, alt_u32 sys_clk_freq, alt_u32 baud_rate)
{
    s_uart_base = base;
    s_state = LIDAR_STATE_WAIT_HEADER_1;
    s_data_index = 0;
    s_has_new = 0;

    uart_init(base, sys_clk_freq, baud_rate);
}

/**
 * @brief Checksum calculation for LiDAR data packet as shown in datasheet
 * 
 * @param base  Base address of the UART
 * @param data  Pointer to LiDAR_Data_t structure to store the read data
 * @return int  0 on success, -1 on failure
 */
static inline alt_u8 lidar_checksum(const alt_u8* u, size_t UART_BASE_BITS)
{
    alt_u16 sum = 0;
    for (int i = 0; i < UART_BASE_BITS; i++) sum += u[i];
    return (alt_u8)(sum & 0xFF);
}

/**
 * @brief LiDAR ISR step function to be called in a timer ISR to not block the CPU for too long
 * using an FSM for logic
 */
void lidar_isr_step(void)
{
    if (s_uart_base == 0) return; // Not initialized

    int processed = 0;
    const int max_bytes_per_call = 32; // Limit processing to avoid long blocking

    while (processed < max_bytes_per_call)
    {
        int ch = uart_read_char_nonblocking(s_uart_base);
        if (ch < 0) break; // No more data
        processed += 1;

        alt_u8 byte = (alt_u8)ch;

        // FSM processing
        switch (s_state)
        {
        // First Header byte
        case LIDAR_STATE_WAIT_HEADER_1:
            if (byte == LIDAR_HEADER)
            {
                s_data_buffer[0] = byte;
                s_state = LIDAR_STATE_WAIT_HEADER_2;
            }
            break;

        // Second Header byte
        case LIDAR_STATE_WAIT_HEADER_2:
            if (byte == LIDAR_HEADER)
            {
                s_data_buffer[1] = byte;
                s_data_index = 2;
                s_state = LIDAR_STATE_READ_DATA;
            } else if (byte == 0x59) { // Redundant but keeps synchronization on 0x59
                s_data_buffer[0] = byte;
                s_state = LIDAR_STATE_WAIT_HEADER_2;
            } else {
                s_state = LIDAR_STATE_WAIT_HEADER_1;
            }
            break;

        // Reading Data bytes
        case LIDAR_STATE_READ_DATA:
            s_data_buffer[s_data_index++] = byte;
            
            if (s_data_index >= LIDAR_DATA_LENGTH)
            {
                if(lidar_checksum(s_data_buffer, LIDAR_DATA_LENGTH - 1) == s_data_buffer[LIDAR_DATA_LENGTH - 1])
                {
                    alt_u16 distance = (alt_u16)(s_data_buffer[2] | (s_data_buffer[3] << 8));
                    alt_u16 strength = (alt_u16)(s_data_buffer[4] | (s_data_buffer[5] << 8));
                    alt_u16 temp_raw = (alt_u16)(s_data_buffer[6] | (s_data_buffer[7] << 8));
                    float temperature = ((float)(temp_raw)) / 8.0f - 256.0f;

                    s_latest.distance = distance;
                    s_latest.strength = strength;
                    s_latest.temperature = temperature;
                    s_has_new = 1;
                }
            }
            break;

            default:
                s_state = LIDAR_STATE_WAIT_HEADER_1;
                break;
        }
    }
}

/**
 * @brief Try to get the latest LiDAR data if available
 * this function is to be used in the loops as an if-check
 * 
 * @param base  Base address of the UART
 * @param data  Pointer to LiDAR_Data_t structure to store the read data
 * @return int  1 if new data was available and copied, 0 if no new data, -1 on error
 */
int lidar_try_get(alt_u32 base, LiDAR_Data_t* data)
{
    if (data == NULL) return -1;

    alt_irq_context ctx = alt_irq_disable_all();
    uint8_t has = s_has_new;
    if (has) {
        *data = s_latest;   // Atomic copy with IRQ off
        s_has_new = 0;
    }
    alt_irq_enable_all(ctx);

    return has ? 1 : 0;
}