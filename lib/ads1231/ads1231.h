/*
 * Library for the TI ADS1231 24-Bit Analog-to-Digital Converter
 */

#ifndef ADS1231_H
#define ADS1231_H


void ads1231_init(void);
int ads1231_get_value(long& val);
int ads1231_get_grams(int& grams);
void ads1231_error_msg(int error_code);
int delay_until(unsigned long max_delay, long max_weight, bool pour_handling);
int wait_for_cup();

#endif
