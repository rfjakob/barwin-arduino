/*
 * Library for the TI ADS1231 24-Bit Analog-to-Digital Converter
 */

#ifndef ADS1231_H
#define ADS1231_H

// All return values higer than this are error codes
#define ADS1231_ERR 2147483000
// Timeout waiting for HIGH
#define ADS1231_ERR_TIMEOUT_HIGH ADS1231_ERR+1
// Timeout waiting for LOW
#define ADS1231_ERR_TIMEOUT_LOW ADS1231_ERR+2

void ads1231_init(void);
long ads1231_get_value(void);
long ads1231_get_grams();

int delay_until(unsigned long max_delay, long max_weight);

#endif
