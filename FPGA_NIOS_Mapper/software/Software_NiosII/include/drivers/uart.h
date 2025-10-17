/**
 * @file uart.h
 * @author Azeol
 * @brief UART driver header file
 * @version 0.1
 * @date 2025-10-17
 * 
 */

#ifndef UART_H_
#define UART_H_

// UART driver function prototypes
void uart_init(alt_u32 base, alt_u32 sys_clk_freq, alt_u32 baud_rate);
void uart_send_char(alt_u32 base, char c);
char uart_read_char(alt_u32 base);
int uart_read_char_nonblocking(alt_u32 base);
void uart_send_string(alt_u32 base, const char* str);

#endif /* UART_H_ */