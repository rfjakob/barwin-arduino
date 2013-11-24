#include <Arduino.h>
#include <ads1231.h>
#include <bottle.h>
#include <utils.h>
#include "../config.h"

/*
 * Macro Magic that creates and initializes the variables
 *     Bottle bottles[]
 *     int bottles_nr
 * Macro is defined in bottle.h.
 */
DEFINE_BOTTLES();

void pour_cocktail(int* requested_amount);
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
}


void loop() {
    // print some stuff every 500ms while idle
    IF_HAS_TIME_PASSED(SEND_READY_INTERVAL)  {
        long weight = ads1231_get_grams();
        if (weight >= ADS1231_ERR) {
            // TODO print specific error code / msg
            ERROR("SCALE_ERROR");
            // FIXME this is wrong, return does not suffice if commands
            // should not be processed!
            return;
        }

        // send message: READY weight is_cup_there
        String msg = String("READY ")
            + String(weight) + String(" ")
            + String(weight > WEIGHT_EPSILON ? 1 : 0);
        MSG(msg);
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
            else if (cmd_str.equals("CALIBRATE_BOTTLE_POS")) {
                calibrate_bottle_pos();
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
void pour_cocktail(int* requested_amount) {
    /*DEBUG_VAL_LN(requested_amount);                               */
    /*if (!requested_amount > 0) {                                  */
    /*    DEBUG_MSG_LN("Error. Please provide integer > 0.");*/
    /*    return;                                            */
    /*}                                                      */

    // TODO check if requested_amount is significantly more than UPGRIGHT_OFFSET


    // Sanity check: Never pour more than MAX_DRINK_GRAMS
    long sum = 0; // Use long to rule out overflow
    for (int i = 0; i < bottles_nr; i++) {
        sum += requested_amount[i];
    }
    if(sum > MAX_DRINK_GRAMS)
    {
        DEBUG_MSG_LN("Total amount greater than MAX_DRINK_GRAMS");
        ERROR("INVALID_COMMAND");
        return;
    }

    // wait for cup, wait until weight > WEIGHT_EPSILON or
    // CUP_TIMEOUT reached
    if (ads1231_get_grams() < WEIGHT_EPSILON) {
        MSG("WAITING_FOR_CUP");
        if (delay_until(CUP_TIMEOUT, WEIGHT_EPSILON) == 0) {
            DEBUG_MSG_LN("CUP_TIMEOUT reached. Aborting.");
            ERROR("CUP_TIMEOUT_REACHED");
            return;
        }
    }

    // wait a bit until cup weight can be measured safely
    delay(CUP_SETTLING_TIME);

    // Pour liquid for each bottle
    int measured_amount[bottles_nr];
    // initializing array with 0
    memset(measured_amount, 0, sizeof(int) * bottles_nr);

    for (int i = 0; i < bottles_nr; i++) {

        if(requested_amount[i] == 0)
            continue;

        // we cannot pour less than UPGRIGHT_OFFSET --> do not pour if it is
        // less than UPGRIGHT_OFFSET/2.0 and print warning...
        if (requested_amount[i] < UPGRIGHT_OFFSET) {
            if (UPGRIGHT_OFFSET / 2.0 > requested_amount[i]) {
                DEBUG_MSG_LN("Warning! Requested output is between: "
                     "UPGRIGHT_OFFSET/2 > outpout > 0 --> will not pour!");
                continue;
            } else {
                DEBUG_MSG_LN("Warning! Requested output is between: "
                    "UPGRIGHT_OFFSET > outpout >= UPGRIGHT_OFFSET/2 --> will pour too much!");
            }
        }

        bottles[i].pour(requested_amount[i], measured_amount[i]);
    }

    // Send success message, measured_amount as params
    String msg = "ENJOY ";
    for (int i = 0; i < bottles_nr; i++)
        msg += String(measured_amount[i]) + String(" ");
    MSG(msg);
}

/**
 *
 */
void calibrate_bottle_pos() {
    for (int bottle = 0; bottle < bottles_nr; bottle++) {
        DEBUG_START();
        DEBUG_VAL(bottle);
        DEBUG_VAL(bottles[bottle].name);
        // FIXME private members...
        //DEBUG_VAL(bottles[bottle].pos_down);
        //DEBUG_VAL(bottles[bottle].pos_up);
        DEBUG_END();
        bottles[bottle].turn_down(CALIBRATION_TURN_DELAY, true);
        bottles[bottle].turn_up(CALIBRATION_TURN_DELAY, true);
    }
    DEBUG_MSG_LN("Calibration procedure for bottle position finished.");
}
