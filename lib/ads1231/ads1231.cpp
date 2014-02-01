/**
 * Library for the TI ADS1231 24-Bit Analog-to-Digital Converter
 *
 * Written in plain C, but for some reason ino expects it to have .cpp
 * file extension, otherwise linking fails.
 *
 * File is written in plain C, because rfjakob insists on it. He thinks the
 * following comment suffices to end the discussion and to have it in plain C:
 *
 * Originally written by lumbric, commmented and slightly modified by
 * rfjakob
 *
 * But the discussion is not over yet. We are going to continue it in
 * ../bottle/bottle.h! :)
 */

#include <Arduino.h>
#include <limits.h>

#include <ads1231.h>
#include <custom_eeprom.h>
#include <utils.h>
#include "../../config.h"
#include "errors.h"

unsigned long ads1231_last_millis = 0;
int ads1231_offset = 0;

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

    // Read absolute offset from EPROM
    EEPROM_read(ADS1231_OFFSET_EEPROM_POS, ads1231_offset);
}

/*
 * Get the raw ADC value. Can block up to 100ms in normal operation.
 * Returns 0 on success, an error code otherwise (see ads1231.h)
 */
errv_t ads1231_get_value(long& val)
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
    ads1231_last_millis = millis();

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
errv_t ads1231_get_grams(int& grams)
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

    grams = raw/ADS1231_DIVISOR + ads1231_offset;
    return 0; // Success
}


/*
 * Get the weight in grams but measure often until the same weight is measured
 * for 3 three times.
 * Can block for longer if the weight on scale is not stable.
 * Returns 0 on sucess, an error code otherwise (see errors.h)
 */
errv_t ads1231_get_stable_grams(int& grams) {
    grams = 0; // needs to be 0 on error
    int i = 0;
    unsigned long start = millis();
    int weight_last, weight;
    RETURN_IFN_0(ads1231_get_grams(weight));
    while (i < 2) {
        delay(100); // TODO make this a constant in config.h
        weight_last = weight;
        RETURN_IFN_0(ads1231_get_grams(weight));
        // TODO maybe this would be more correct...? do we have abs?
        //if (abs((weight_last - weight) < WEIGHT_EPSILON)
        if (weight_last == weight) {
            // weight stable
            i++;
        } else {
            // weight not stable
            i = 0;
        }
        DEBUG_START();
        DEBUG_MSG("Not stable: ");
        DEBUG_VAL(weight);
        DEBUG_VAL(weight_last);
        DEBUG_END();

        if (millis() - start > ADS1231_STABLE_MILLIS) {
            return ADS1231_STABLE_TIMEOUT;
        }
    }
    grams = weight;
    return 0;
}


/**
 * Tare scale. Call this if there is nothing on scale to store offset and zero
 * current measured value.
 */
errv_t ads1231_tare(int& grams) {
    // get grams or return error immediately on error
    RETURN_IFN_0( ads1231_get_stable_grams(grams) );

    // success
    ads1231_offset += -grams;
    EEPROM_write(ADS1231_OFFSET_EEPROM_POS, ads1231_offset);

    return 0;
}


/**
 * Get grams from scale if measurement fast enough, otherwise returns
 * with error ADS1231_WOULD_BLOCK. Should not block longer than 10ms.
 */
errv_t ads1231_get_noblock(int& grams) {
     // ADS1231 supports 10 samples per second. That means after the last
     // sample we need to wait 100ms. If 90ms passed already, it should be OK.
    unsigned long t = (millis() - ads1231_last_millis) % 100;
    if (t < 90) {
        return ADS1231_WOULD_BLOCK;
    }
    return ads1231_get_grams(grams);
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
errv_t delay_until(long max_delay, int weight, bool pour_handling, bool reverse) {
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

        RETURN_IFN_0(check_aborted());

        // "one" inverts the inequality
        if(cur * one > (weight + WEIGHT_EPSILON) * one)
            return 0;

        // Just waiting for weight, no special pouring error detection
        if (!pour_handling)
            continue;

        if(last > cur + WEIGHT_EPSILON || cur < WEIGHT_EPSILON)
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
errv_t wait_for_cup() {
    int weight;
    ads1231_get_grams(weight); // weight will be 0 in case of error
    if ( weight < WEIGHT_EPSILON) {
        MSG("WAITING_FOR_CUP");
        int ret = delay_until(CUP_TIMEOUT, 0, false);
        if (ret == DELAY_UNTIL_TIMEOUT) {
            // FIXME if wait_for_cup() is caused by a WHERE_THE_FUCK_IS_THE_CUP
            // error, then the bottle will remain in pause position...
            ERROR(strerror(CUP_TIMEOUT_REACHED));
            return CUP_TIMEOUT_REACHED;
        } else if (ret != 0) {
            ERROR(strerror(ret));
        }
        return ret;
    }
    return 0;
}
