
/**
 * @file 7segDiplay.c
 * @author Azeol
 * @brief 
 * @version 0.1
 * @date 2025-10-14
 *  
 */

#include "7segDisplay.h"
#include <stdio.h>

/**
 * @brief Converts a digit (0-9) and a segment index (0-3) to its 7-segment display representation.
 * 
 * @param value 
 * @param seg 
 * @return int 
 */

int seg7(int value, int seg){
    int decal[] = {0,8,16,24};

    switch (value) {
        case 0: return (0x40 << decal[seg]);
        case 1: return (0x79 << decal[seg]);
        case 2: return (0x24 << decal[seg]);
        case 3: return (0x30 << decal[seg]);
        case 4: return (0x19 << decal[seg]);
        case 5: return (0x12 << decal[seg]);
        case 6: return (0x02 << decal[seg]);
        case 7: return (0x78 << decal[seg]);
        case 8: return (0x00 << decal[seg]);
        case 9: return (0x10 << decal[seg]);
        default: return (0xFF << decal[seg]);
    }
}