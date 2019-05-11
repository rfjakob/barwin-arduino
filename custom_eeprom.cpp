/**
 * Extends the Arduino standard library by read/write functions for all data
 * types and defines storage addresses.
 */

#include "custom_eeprom.h"

#define byte uint8_t

// TODO this functions are complicated, because they should support floats,
// long, ... as well. But does not work as expected. See commit 41062e26e70.

/**
 * Write an int to EEPROM. Return how many bytes where written.
 *
 * Stolen from:
 * http://playground.arduino.cc/Code/EEPROMWriteAnything
 */
int EEPROM_write(int ee, const int& value)
{
    const byte* p = (const byte*)(const void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
        EEPROM.write(ee++, *p++);
    return i;
}


/**
 * Write an int from EEPROM. Return how many bytes where read.
 *
 * Stolen from:
 * http://playground.arduino.cc/Code/EEPROMWriteAnything
 */
int EEPROM_read(int ee, int& value)
{
    byte* p = (byte*)(void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(ee++);
    return i;
}

