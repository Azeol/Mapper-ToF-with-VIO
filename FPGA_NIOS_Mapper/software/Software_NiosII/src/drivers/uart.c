/**
 * @file uart.c
 * @author Azeol
 * @brief UART driver implementation
 * @version 0.1
 * @date 2025-10-17
 * 
 */

// Standard lib
#include <stdio.h>

// Altera lib
#include "system.h"
#include "alt_types.h"
#include "altera_avalon_uart_regs.h"

// Project lib
#include "uart.h"

/**
 * @brief Initialize the UART peripheral
 * 
 * @param base          Base address of the UART peripheral
 * @param sys_clk_freq  System clock frequency in Hz (e.g., 50000000 for 50 MHz)
 * @param baud_rate     Desired baud rate for UART communication (default to put is 115200)
 */
void uart_init(alt_u32 base, alt_u32 sys_clk_freq, alt_u32 baud_rate)
{
    alt_u32 divisor = (sys_clk_freq / baud_rate) - 1; // Assuming a baud rate of 115200

    IOWR_32DIRECT(base, ALTERA_AVALON_UART_DIVISOR_REG, divisor);
    IOWR_32DIRECT(base, ALTERA_AVALON_UART_CONTROL_REG, 0x00000003); // Enable RX and TX

    printf("UART using base %u initialized with baud rate %u and divisor %u\n", base, baud_rate, divisor);
}

/**
 * @brief Send a character over UART
 * 
 * @param base  Base address of the UART peripheral
 * @param c     Character to send
 */
void uart_send_char(alt_u32 base, char c)
{
    // Wait until the transmitter is ready
    while (!(IORD_32DIRECT(base, ALTERA_AVALON_UART_STATUS_REG) & ALTERA_AVALON_UART_STATUS_TRDY_MSK));

    // Send the character
    IOWR_32DIRECT(base, ALTERA_AVALON_UART_TXDATA_REG, (alt_u32)c);
}

/**
 * @brief Read a character from UART
 * 
 * @param base  Base address of the UART peripheral
 * @return char Received character
 */
char uart_read_char(alt_u32 base)
{
    // Wait until data is available
    while (!(IORD_32DIRECT(base, ALTERA_AVALON_UART_STATUS_REG) & ALTERA_AVALON_UART_STATUS_RRDY_MSK));

    // Read the character
    return (char)(IORD_32DIRECT(base, ALTERA_AVALON_UART_RXDATA_REG) & 0xFF);
}

/**
 * @brief Read a character from UART without blocking
 * 
 * @param base  Base address of the UART peripheral
 * @return int  Received character or -1 if no data is available
 */
int uart_read_char_nonblocking(alt_u32 base)
{
    if(!(IORD_32DIRECT(base, ALTERA_AVALON_UART_STATUS_REG) & ALTERA_AVALON_UART_STATUS_RRDY_MSK))
    {
        return -1; // No data available
    }
    // Read the character
    return (char)(IORD_32DIRECT(base, ALTERA_AVALON_UART_RXDATA_REG) & 0xFF);
}

/**
 * @brief Send a string over UART
 * 
 * @param base  Base address of the UART peripheral
 * @param str   Null-terminated string to send
 */
void uart_send_string(alt_u32 base, const char* str)
{
    while (*str)
    {
        uart_send_char(base, *str);
        str += 1;
    }
}