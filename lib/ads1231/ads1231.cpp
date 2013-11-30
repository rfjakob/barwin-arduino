/*
 * Library for the TI ADS1231 24-Bit Analog-to-Digital Converter
 *
 * Written in plain C, but for some reason ino expects it to have .cpp
 * file extension, otherwise linking fails.
 *
 * Originally written by lumbric, commmented and slightly modified by
 * rfjakob
 */

#include <Arduino.h>
#include <limits.h>

#include <ads1231.h>
#include <utils.h>
#include "../../config.h"
#include "errors.h"

// emulate a scale
//#define ADS1231_EMULATION 1

/*
 * Initialize the interface pins
 */
void ads1231_init(void)
{
    // We send the clock
    pinMode(ADS1231_CLK_PIN, OUTPUT);

    // ADS1231 sends the data
    pinMode(ADS1231_DATA_PIN, INPUT);
    // Enable pullup to get a consistent state in case of disconnect
    digitalWrite(ADS1231_DATA_PIN, HIGH);

    // Set CLK low to get the ADS1231 out of suspend
    digitalWrite(ADS1231_CLK_PIN, 0);

}

/*
 * Get the raw ADC value. Can block up to 100ms in normal operation.
 * Returns 0 on success, an error code otherwise (see ads1231.h)
 */
int ads1231_get_value(long& val)
{
    int i=0;
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
            return ADS1231_TIMEOUT_HIGH; // Timeout waiting for HIGH
    }
    start=millis();
    while(digitalRead(ADS1231_DATA_PIN) != LOW)
    {
        if(millis() > start+150)
            return ADS1231_TIMEOUT_LOW; // Timeout waiting for LOW
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

    return 0; // Success
}

/*
 * Get the weight in grams. Can block up to 100ms in normal
 * operation because the ADS1231 makes only 10 measurements per second.
 * Returns 0 on sucess, an error code otherwise (see errors.h)
 */
int ads1231_get_grams(int& grams)
{
    // a primitive emulation using a potentiometer attached to pin A0
    // returns a value between 0 and 150 grams
    #ifdef ADS1231_EMULATION
    grams = map(analogRead(A0) , 0, 1023, 0, 150);
    return 0;
    #endif

    int ret;
    long raw;
    grams=0; // On error, grams should always be zero

    ret = ads1231_get_value(raw);
    if(ret != 0)
        return ret; // Scale error

    grams = raw/ADS1231_DIVISOR + ADS1231_OFFSET;
    return 0; // Success
}

/**
 * Send correct error message to serial for given error code.
 */
void ads1231_error_msg(int error_code) {
    if (error_code == ADS1231_TIMEOUT_HIGH) {
        ERROR("ADS1231_TIMEOUT_HIGH");
    }
    else if (error_code == ADS1231_TIMEOUT_LOW) {
        ERROR("ADS1231_TIMEOUT_LOW");
    }
}


/**
 * Blocks until weight is more than weight + WEIGHT_EPSILON
 * but at maximum for 'max_delay' milliseconds. max_delay is ignored if it is
 * negative.
 * weight might be current weight to detect any increase of weight.
 * If "reverse" is set to true, we wait until weight gets less than weight +
 * WEIGHT_EPSILON.
 * Return values:   see also errors.h!
 *  0 weight was reached (success)
 *  1 timeout (pouring too slow)
 *  2 bottle empty (weight did not change for BOTTLE_EMPTY_INTERVAL ms)
 *  3 cup removed (weight has decreased)
 *  other values: scale error (see ads1231.h).
 */
int delay_until(long max_delay, int weight, bool pour_handling, bool reverse) {
    unsigned long start = millis();
    int cur, ret;
    int last     = -999; // == -inf, because the first time checks should
    int last_old = -999; // always pass until we have a valid last/last_old
    unsigned long last_millis = 0;

    int one = 1;
    if (reverse) {
        one = -1;
    }

    while(1) {
        if(max_delay > 0 && millis() - start > max_delay)
            return DELAY_UNTIL_TIMEOUT; // Timeout

        // this blocks 100ms because ADS1231 runs at 10 samples per second
        ret = ads1231_get_grams(cur);
        if(ret != 0)
            return ret; // Scale error

        // this delays, we do not want it...
        //DEBUG_VAL_LN(cur);

        // "one" inverts the inequality
        if(cur * one > (weight + WEIGHT_EPSILON) * one)
            return SUCCESS;

        // Just waiting for weight, no special pouring error detection
        if (!pour_handling)
            continue;

        if(last > cur + WEIGHT_EPSILON)
            return WHERE_THE_FUCK_IS_THE_CUP; // Current weight is smaller than last measured

        // Jakob does not like abs, so we check first for
        // WHERE_THE_FUCK_IS_THE_CUP --> then we do not need
        // abs(cur - last_old) < WEIGHT_EPSILON
        if(millis() - last_millis > BOTTLE_EMPTY_INTERVAL) {
            // note that ads1231_get_grams() blocks ~100ms, so
            // BOTTLE_EMPTY_INTERVAL is not accurate.
            // Note: first time the check always passes, then within
            // BOTTLE_EMPTY_INTERVAL additional weight needs to be measured
            // in the cup.
            if (cur - last_old < WEIGHT_EPSILON) {
                return BOTTLE_EMPTY; // Weight does not change means bottle is empty
            }
            last_old = cur;
            last_millis = millis();
        }

        last = cur;
    }
}

/**
 * Wait for the cup.
 * Return error codes by delay_until.
 */
int wait_for_cup() {
    int weight;
    ads1231_get_grams(weight); // weight will be 0 in case of error
    if ( weight < WEIGHT_EPSILON) {
        MSG("WAITING_FOR_CUP");
        int ret = delay_until(CUP_TIMEOUT, 0, false);
        if (ret == DELAY_UNTIL_TIMEOUT) {
            ERROR("CUP_TIMEOUT_REACHED");
            return CUP_TIMEOUT_REACHED;
        } else if (ret != 0) {
            DEBUG_MSG_LN("Scale error when waiting for cup.");
            ads1231_error_msg(ret);
        }
        return ret;
    }
    return 0;
}
