/**
 * Extends the Arduino standard library by read/write functions for all data
 * types and defines storage addresses.
 */

#ifndef CUSTOM_EEPROM_H
#define CUSTOM_EEPROM_H

#include <EEPROM.h>


int EEPROM_read(int ee, int& value);
int EEPROM_write(int ee, const int& value);

#define ADS1231_OFFSET_EEPROM_POS   0

#endif
