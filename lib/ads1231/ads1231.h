/*
 * Library for the TI ADS1231 24-Bit Analog-to-Digital Converter
 */

#ifndef ADS1231_H
#define ADS1231_H

extern unsigned long ads1231_last_millis;

void ads1231_init(void);
int ads1231_get_value(long& val);
int ads1231_get_grams(int& grams);
int ads1231_get_noblock(int& grams);
int delay_until(long max_delay, int max_weight, bool pour_handling, bool revers=false);
int wait_for_cup();

#endif
