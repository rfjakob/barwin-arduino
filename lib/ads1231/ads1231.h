/*
 * Library for the TI ADS1231 24-Bit Analog-to-Digital Converter
 */

#ifndef ADS1231_H
#define ADS1231_H

#include "errors.h"

extern unsigned long ads1231_last_millis;
extern int ads1231_additional_offset;

void ads1231_init(void);
errv_t ads1231_get_value(long& val);
errv_t ads1231_get_grams(int& grams);
errv_t ads1231_get_stable_grams(int& grams);
errv_t ads1231_get_noblock(int& grams);
errv_t ads1231_tare(int& grams);
errv_t delay_until(long max_delay, int max_weight, bool pour_handling, bool revers=false);
errv_t wait_for_cup();

#endif
