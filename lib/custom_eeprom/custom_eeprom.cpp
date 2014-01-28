/**
 * Extends the Arduino standard library by read/write functions for all data
 * types and defines storage addresses.
 */

#include "custom_eeprom.h"

#define byte uint8_t

/**
 * Write anything to EEPROM (double, long, int, ...). Return how many bytes
 * where written.
 *
 * Stolen from:
 * http://playground.arduino.cc/Code/EEPROMWriteAnything
 */
template <class T> int EEPROM_write(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
        EEPROM.write(ee++, *p++);
    return i;
}


/**
 * Read anything to EEPROM (double, long, int, ...). Return how many bytes
 * where read.
 *
 * Stolen from:
 * http://playground.arduino.cc/Code/EEPROMWriteAnything
 */
template <class T> int EEPROM_read(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(ee++);
    return i;
}

