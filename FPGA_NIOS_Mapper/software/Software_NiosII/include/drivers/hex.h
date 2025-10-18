/**
 * @file    hex.h
 * @author  Azeol
 * @brief   7-segment display driver
 * @version 0.1
 * @date    2025-10-14
 */

#ifndef HEX_H_
#define HEX_H_

#define HEX_NB 6

/**
 * @brief Display a hexadecimal value on a 7-segment display
 * 
 * @warning Only 0-9, a-f, h, i, l, m, o, r, u and - are supported
 * 
 * @param value      char to be printed
 * @param len        number of char to be printed
 * @param first      first display to be modified
 * 
 * @return int 
 * @retval 0 operation successful
 * @retval -1 invalid len (too long?)
 * @retval -2 invalid char
 */

int hex_display(char* value, int len, int first);

/*
Quick usage:
	// Display "1234" starting at HEX0..HEX3
	(void)hex_display("1234", 4, 0);

	// Display "ab-c" starting at HEX2..HEX5
	(void)hex_display("ab-c", 4, 2);

Notes:
	- Returns 0 if all chars were valid; >0 equals number of invalid chars.
	- Supported chars: 0-9, a-f, h, i, l, m, o, r, u, '-'.
	- first is the starting display index (0 = HEX0).
*/

#endif /* HEX_H_ */
