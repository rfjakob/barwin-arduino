/*
 * Library for the TI ADS1231 24-Bit Analog-to-Digital Converter
 * 
 * Written in plain C, but for some reason ino expects it to have .cpp
 * file extension, otherwise linking fails.
 */

#include <Arduino.h>
#include <limits.h>

#include <ads1231.h>
#include "../../config.h"

/*
 * Initialize the interface pins
 */
void ads1231_init(void)
{
	// We send the clock
	pinMode(ADS1231_CLK_PIN, OUTPUT);
	// ADS1231 sends the data
	pinMode(ADS1231_DATA_PIN, INPUT);
	// Set CLK low to get the ADS1231 out of suspend
	digitalWrite(ADS1231_DATA_PIN, 0);
}

/*
 * Get the raw ADC value. Can block up to 100ms.
 */
long ads1231_get_value(void)
{
	long val=0;
	int i;
	unsigned long start;

	/* A high to low transition on the data pin means that the ADS1231
	 * has finished a measurement (see datasheet page 13).
	 * This can take up to 100ms (the ADS1231 runs at 10 samples per
	 * second!).
	 * Note that just testing for the state of the pin is unsafe.
	 */
	start=millis();
	while(digitalRead(ADS1231_DATA_PIN) != HIGH)
	{
		if(millis() > start+150)
			return ADS1231_ERR_TIMEOUT_HIGH; // Timeout waiting for HIGH
	}
	start=millis();
	while(digitalRead(ADS1231_DATA_PIN) != LOW)
	{
		if(millis() > start+150)
			return ADS1231_ERR_TIMEOUT_LOW; // Timeout waiting for LOW
	}

	// Read 24 bits
	for(i=23 ; i >= 0; i--) {
		digitalWrite(ADS1231_CLK_PIN, HIGH);
		val = (val << 1) + digitalRead(ADS1231_DATA_PIN);
		digitalWrite(ADS1231_CLK_PIN, LOW);
	}

	/* Bit 23 is acutally the sign bit. Shift by 8 to get it to the 
	 * right position (31), divide by 256 to restore the correct value.
	 */
	val = (val << 8) / 256;

	/* The data pin now is high or low depending on the last bit that
	 * was read.
	 * To get it to the default state (high) we toggle the clock one
	 * more time (see datasheet page 14 figure 19).
	 */
	digitalWrite(ADS1231_CLK_PIN, HIGH);
	digitalWrite(ADS1231_CLK_PIN, LOW); 

	return val;
}

/*
 * Get the weight in milligrams. Can block up to 100ms.
 */
long ads1231_get_milligrams()
{
	long val;
	
	val=ads1231_get_value();
	if(val>=ADS1231_ERR)
		return val;
	else
		return val/ADS1231_SLOPE + ADS1231_OFFSET;
}
