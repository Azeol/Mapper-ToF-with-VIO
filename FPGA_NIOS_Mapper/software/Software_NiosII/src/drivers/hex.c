
/**
 * @file    hex.c
 * @author  Azeol
 * @brief   7-segment display driver
 * @version 0.1
 * @date    2025-10-14
 */

// Quartus system lib
#include <system.h>
#include <altera_avalon_pio_regs.h>

// Standard lib
#include <stdio.h>
#include <stdint.h>

// Project lib
#include "drivers/hex.h"

/**
 * @brief CHAR LUT for 7-segment display
 * 
 */
//							binary val	   id char   7 - 6 - 5 - 4 - 3 - 2 - 1 - 0
static const int chars[] = {0b11000000, // 0    0            x   x   x   x   x   x
							0b11111001, //      1 						 x   x
							0b10100100, //  	2        x       x   x       x   x
							0b10110000, //  	3        x           x   x   x   x
							0b10011001, //  	4 		 x   x           x   x
							0b10010010, //  	5        x   x       x   x       x
							0b10000010, //  	6        x   x   x   x   x       x
							0b11111000, //  	7 						 x   x   x
							0b10000000, //  	8        x   x   x   x   x   x   x
							0b10010000, //  	9        x   x       x   x   x   x
							0b10001000, // 10	a        x   x   x       x   x   x
							0b10000011, //  	b        x   x   x   x   x
							0b11000110, //  	c 	         x   x   x           x
							0b10100001, //  	d        x       x   x   x   x
							0b10000110, //  	e        x   x   x   x           x
							0b10001110, //  	f        x   x   x               x
							0b10001001,	// 16 	h        x   x   x       x   x
							0b11101111, //  	i            x
							0b11000111, // 18   l 	         x   x   x
							0b11001000,	// 		m	         x   x       x   x
							0b10100011, // 20 	o        x       x   x   x
							0b11100011, // 21 	u                x   x   x
							0b10101111, // 22	r		 x       x
							0b10111111, // 23	-        x
							0b11111111  //  	.    									!! Unwired on the board !!
};

/**
 * @brief Convert a character to its 7-segment display representation.
 * 
 * @param in    The input character.
 * @param val   Pointer to the variable to store the 7-segment value.
 * @return int  0 on success, -1 on invalid character.
 */
int char27seg(char in, int *val)
{
    *val = 0;

    switch(in)
    {
        // Numbers
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            *val = chars[(in - 0x30) + 0];
            return 0;
            break;

        // First letters
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            *val = chars[(in - 0x61) + 10];
            return 0;
            break;

        case 'h':
        case 'i':
            *val = chars[(in - 0x61) + 16];
            return 0;
            break;
        
        case 'l':
        case 'm':
            *val = chars[(in - 0x61) + 18];
            return 0;
            break;

        case 'o':
            *val = chars[(in - 0x61) + 20];
            return 0;
            break;
        
        case 'r':
        case 'u':
            *val = chars[(in - 0x61) + 22];
            return 0;
            break;
        
        // two special chars
        case '-':
        case '.':
            *val = chars[(in - 0x2D) + 23];
            return 0;
            break;

        default:
            *val = 0b00000000; // ALL
            return -1; // invalid char
    }

    return -1; // should never happen
}

/**
 * @brief Convert a hexadecimal string to its 7-segment display representation. Only accepted chars are 0-9, a-f, h, i, l, m, o, r, u and -.
 * 
 * @param value  The input hexadecimal string.
 * @param len    The length of the input string.
 * @param first  The starting position for the display.
 * @return int   >0 for the number of invalid char, 0 on success, -1 on length overflow, -2 on empty string, -3 on invalid first index.
 */
int hex_display(char *value, int len, int first)
{
    // Input validation
    // Check for length overflow
    if ((first + len) > HEX_NB)
    {
        return -1;
    }
    // Check for empty string
    if (len > HEX_NB)
    {
        return -2;
    }
    // Check for invalid first index
    if ((first > HEX_NB) | (first < 0))
    {
        return -3;
    }

    // Copy the data
    char buff[] = {'\0','\0','\0','\0','\0','\0'};
    for (int k = first; k < (first + len); k++)
    {
        buff[k] = value[k - first];
    }

    // Temp variables
    int tmp = 0;
    int err = 0;

    // 64 bits variable to store the value to be sent to the HEX displays
    uint64_t bits = 0;
    uint64_t mask = 0;
    uint64_t actual = ((uint64_t)IORD_ALTERA_AVALON_PIO_DATA(HEX5_HEX4_BASE) << 32) | (uint64_t)IORD_ALTERA_AVALON_PIO_DATA(HEX3_HEX0_BASE);

    for (int k = 0; k < HEX_NB; k++)
    {
        // Get the associated char and add error counter if a char is invalid
        if (char27seg(buff[k], &tmp) < 0)
        {
            err += 1;
        }

        // Clear the bits to write
        bits = bits << 8;
        mask = mask << 8;

        bits |= tmp;

        if (buff[k] == 0x00)
        {
            mask |= 0xFF; // Mask the bits if no char to display
        }
    }

    // Clear the needed bits
    actual &= mask;

    // Inverting the mask and getting the masked bits
    mask = ~mask;
    bits &= mask;

    // Finally getting the final bit stream
    bits = actual | bits;

    // Writing the new value
    IOWR_ALTERA_AVALON_PIO_DATA(HEX5_HEX4_BASE, (bits & 0xFFFFFFFF00000000) >> 32); // HEX 5 and 4, 16 LSB are ignored
    IOWR_ALTERA_AVALON_PIO_DATA(HEX3_HEX0_BASE, bits & 0xFFFFFFFF); // HEX 3 to 0

    return err;
}