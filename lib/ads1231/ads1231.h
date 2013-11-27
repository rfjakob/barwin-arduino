/*
 * Library for the TI ADS1231 24-Bit Analog-to-Digital Converter
 */

#ifndef ADS1231_H
#define ADS1231_H

// Timeout waiting for HIGH
#define ADS1231_ERR_TIMEOUT_HIGH 101
// Timeout waiting for LOW
#define ADS1231_ERR_TIMEOUT_LOW  100

void ads1231_init(void);
int ads1231_get_value(long& val);
int ads1231_get_grams(int& grams);

int delay_until(unsigned long max_delay, long max_weight, bool pour_handling);

#endif
