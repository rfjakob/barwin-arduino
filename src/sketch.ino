#include <Arduino.h>
#include <ads1231.h>
#include <bottle.h>
#include <utils.h>
#include <errors.h>
#include "../config.h"

/*
 * Macro Magic that creates and initializes the variables
 *     Bottle bottles[]
 *     int bottles_nr
 * Macro is defined in bottle.h.
 */
DEFINE_BOTTLES();

int pour_cocktail(int* requested_amount);
int parse_int_params(int* params, int size);

void setup() {
    #if defined(__AVR_ATmega328P__)
    Serial.begin(9600);
    #else
    Serial.begin(115200);
    #endif
    Serial.setTimeout(SERIAL_TIMEOUT);

    ads1231_init();
    Bottle::init(bottles, bottles_nr);

    DEBUG_MSG_LN("setup() finished.");

    // Warn users of emulation mode to avoid unnecessary debugging...
    #ifdef ADS1231_EMULATION
    DEBUG_MSG_LN("Warning! Scale emulation active!");
    #endif
}


void loop() {
    // print some stuff every 500ms while idle
    IF_HAS_TIME_PASSED(SEND_READY_INTERVAL)  {
        int ret, weight = 0;
        ret = ads1231_get_grams(weight);
        if (ret != 0) {
            ads1231_error_msg(ret);
            return;
        }

        // send message: READY weight is_cup_there
        String msg = String("READY ")
            + String(weight) + String(" ")
            + String(weight > WEIGHT_EPSILON ? 1 : 0);
        MSG(msg);

        //long weight_raw;
        //ads1231_get_value(weight_raw);
        //DEBUG_VAL_LN(weight_raw);
    }

    // Parse commands from Serial
    if (Serial.available() > 0) {
        char cmd[MAX_COMMAND_LENGTH] = "";
        // Note that readBytesUntil() will block until SERIAL_TIMEOUT is reached
        // or a space is read from serial input.
        if(Serial.readBytesUntil(' ', cmd, MAX_COMMAND_LENGTH)) {
            String cmd_str = String(cmd);
            if (cmd_str.equals("POUR")) {
                int requested_amount[bottles_nr];
                parse_int_params(requested_amount, bottles_nr);
                pour_cocktail(requested_amount);
            }
            else if (cmd_str.equals("TURN_BOTTLE")) {
                // turn bottle to specific position
                DEBUG_MSG_LN("Turn bottle...");
                int params[2];
                parse_int_params(params, bottles_nr);
                // bottle number (int starting at 0) first parameter, position
                // as microseconds second parameter
                bottles[params[0]].turn_to(params[1], TURN_DOWN_DELAY);
            }
            else if (cmd_str.equals("NOTHING")) {
                // dummy command, for testing
                MSG("DOING_NOTHING");
            }
            else {
                ERROR("INVALID_COMMAND");
                DEBUG_MSG_LN(String("Got string '") + String(cmd) + String("'"));
            }
        }
    }
}


/**
 * Parse space separated int values from Serial to array.
 */
int parse_int_params(int* params, int size) {
    for (int i = 0; i < size; i++) {
        // see /usr/share/arduino/hardware/arduino/cores/arduino/Stream.cpp:138
        // Note: parseInt returns 0 in case of an parsing error... (uargh!)
        params[i] = Serial.parseInt();
    }
    return 0;
}


/**
 * Pouring procedure.
 * Waits for cup and turns each bottle in the order they were defined.
 * 'requested_amount' is the amount of liquid in grams to be poured from
 * each bottle(int array of size bottles_nr).
 */
int pour_cocktail(int* requested_amount) {

    // Sanity check: Never pour more than MAX_DRINK_GRAMS
    long sum = 0; // Use long to rule out overflow
    for (int i = 0; i < bottles_nr; i++) {
        sum += requested_amount[i];
    }
    if(sum > MAX_DRINK_GRAMS) {
        DEBUG_MSG_LN("Total amount greater than MAX_DRINK_GRAMS");
        ERROR("MAX_DRINK_GRAMS_EXCEEDED");
        return MAX_DRINK_GRAMS_EXCEEDED;
    }

    // wait until weight > WEIGHT_EPSILON or CUP_TIMEOUT reached
    RETURN_IFN_0(wait_for_cup());

    // wait a bit until cup weight can be measured safely
    delay(CUP_SETTLING_TIME);

    // Actually poured liquid for each bottle
    int measured_amount[bottles_nr];
    // initializing array with 0
    memset(measured_amount, 0, sizeof(int) * bottles_nr);

    Bottle *cur_bottle = NULL;
    Bottle *last_bottle = NULL;
    for (int i = 0; i < bottles_nr; i++) {

        if(requested_amount[i] == 0)
            continue;

        // we cannot pour less than UPGRIGHT_OFFSET --> do not pour if it is
        // less than UPGRIGHT_OFFSET/2.0 and print warning...
        if (requested_amount[i] < UPGRIGHT_OFFSET) {
            if (UPGRIGHT_OFFSET / 2.0 > requested_amount[i]) {
                DEBUG_MSG_LN("Warning! Requested output is between: "
                     "UPGRIGHT_OFFSET/2 > output > 0 --> will not pour!");
                continue;
            } else {
                DEBUG_MSG_LN("Warning! Requested output is between: "
                    "UPGRIGHT_OFFSET > output >= UPGRIGHT_OFFSET/2 --> will pour too much!");
            }
        }

        cur_bottle = &bottles[i];

        if(last_bottle != 0) // On the first iteration last_bottle is NULL
        {
            DEBUG_MSG_LN("pour_cocktail: Crossfading...");
            crossfade(last_bottle, cur_bottle, TURN_UP_DELAY);
            // At this point, last_bottle is up and cur_bottle is at pause position
        }

        int ret;
        ret = cur_bottle->pour(requested_amount[i], measured_amount[i]);
        // At this point, cur_bottle is at pause position again. Next crossfade
        // will turn it up completely.

        if(ret != 0)
        {
            DEBUG_MSG_LN(String("pour_cocktail: cur_bottle->pour returned error ") + String(ret));
            ads1231_error_msg(ret);
        }

        // Save bottle for next iteration
        last_bottle=cur_bottle;
    }

    // Last bottle is hanging at pause position at this point. Turn up completely.
    last_bottle->turn_up(TURN_UP_DELAY);

    // Send success message, measured_amount as params
    String msg = "ENJOY ";
    for (int i = 0; i < bottles_nr; i++)
        msg += String(measured_amount[i]) + String(" ");
    MSG(msg);

    DEBUG_MSG_LN("Please take cup!");
    delay_until(-1, 0, false, true);
}
