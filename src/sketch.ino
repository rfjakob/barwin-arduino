#include <Arduino.h>
#include <ads1231.h>
#include <bottle.h>
#include <utils.h>
#include "../config.h"


DEFINE_BOTTLES();

void pouring_procedure(int* requested_output);
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
                int requested_output[bottles_nr];
                parse_int_params(requested_output, bottles_nr);
                pouring_procedure(requested_output);
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
 * 'requested_output' is the amount of liquid in grams to be poured from
 * each bottle(int array of size bottles_nr).
 */
void pouring_procedure(int* requested_output) {
    /*DEBUG_VAL_LN(requested_output);                               */
    /*if (!requested_output > 0) {                                  */
    /*    DEBUG_MSG_LN("Error. Please provide integer > 0.");*/
    /*    return;                                            */
    /*}                                                      */
    // TODO check MAX_DRINK_SIZE

    // TODO check if requested_output is significantly more than UPGRIGHT_OFFSET

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
    int measured_output[bottles_nr];
    // initializing array with 0
    memset(measured_output, 0, sizeof(int) * bottles_nr);

    for (int bottle = 0; bottle < bottles_nr; bottle++) {
        // we cannot pour less than UPGRIGHT_OFFSET --> do not pour if it is
        // less than UPGRIGHT_OFFSET/2.0 and print warning...
        if (requested_output[bottle] < UPGRIGHT_OFFSET) {
            if (UPGRIGHT_OFFSET / 2. > requested_output[bottle]) {
                if (requested_output[bottle] > 0 ) {
                    DEBUG_MSG_LN("Warning! Requested output is between: UPGRIGHT_OFFSET/2 > outpout > 0 --> will not pour!");
                }
                continue;
            } else {
                DEBUG_MSG_LN("Warning! Requested output is between: UPGRIGHT_OFFSET > outpout >= UPGRIGHT_OFFSET/2 --> will pour too much!");
            }
        }

        // orig_weight is weight including ingredients poured until now
        long orig_weight = ads1231_get_grams();
        MSG(String("POURING ") + String(bottle) + String(" ") + String(orig_weight));

        DEBUG_START();
        DEBUG_MSG("Start pouring bottle: ");
        DEBUG_VAL(bottles[bottle].name);
        DEBUG_VAL(requested_output[bottle]);
        DEBUG_VAL(bottle);
        DEBUG_VAL(orig_weight);
        DEBUG_END();

        DEBUG_MSG_LN("Turning bottle down...");
        bottles[bottle].turn_down(TURN_DOWN_DELAY);

        // wait for requested weight
        // FIXME here we do not want WEIGHT_EPSILON and sharp >
        DEBUG_MSG_LN("Waiting for weight...");
        delay_until(POURING_TIMEOUT, orig_weight + requested_output[bottle] - UPGRIGHT_OFFSET

        DEBUG_MSG_LN("Turn up again...");
        bottles[bottle].turn_up(TURN_UP_DELAY);

        measured_output[bottle] = ads1231_get_grams() - orig_weight;

        DEBUG_START();
        DEBUG_MSG("Bottle statistics: ");
        DEBUG_VAL(bottle);
        DEBUG_VAL(requested_output[bottle]);
        DEBUG_VAL(measured_output[bottle]);
        DEBUG_END();
    }

    // Send success message, measured_output as params
    String msg = "ENJOY ";
    for (int bottle = 0; bottle < bottles_nr; bottle++)
        msg += String(measured_output[bottle]) + String(" ");
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
