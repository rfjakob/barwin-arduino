#include <ads1231.h>
#include <bottle.h>
#include <utils.h>
#include "../config.h"


DEFINE_BOTTLES();

void pouring_procedure(int liquid_mg);
int parse_int_params(int* params, int size);

void setup() {
    #if defined(__AVR_ATmega328P__)
    Serial.begin(9600);
    #else
    Serial.begin(115200);
    #endif
    Serial.setTimeout(SERIAL_TIMEOUT);
    MSG("READY");

    ads1231_init();
    Bottle::init(bottles, bottles_nr);
}

int long lastweight =  millis();


void loop() {
    // print some stuff every 500ms while idle
    // TODO this needs cleanup (use timedAction library?)
    long weight = ads1231_get_milligrams();
    //long raw    = ads1231_get_value();
    if (millis() - lastweight > 500) {
        DEBUG_VAL_LN(weight);
        lastweight = millis();
    }
    //DEBUG_VAL(raw);


    // Parse commands from Serial
    if (Serial.available() > 0) {
        char cmd[MAX_COMMAND_LENGTH] = "";
        if(Serial.readBytesUntil(' ', cmd, MAX_COMMAND_LENGTH)) {
            String cmd_str = String(cmd);
            if (cmd_str.equals("POUR")) {
                int liquid_mg[bottles_nr];
                parse_int_params(liquid_mg, bottles_nr);
                pouring_procedure(liquid_mg);
            }
            else if (cmd_str.equals("NOTHING")) {
                // dummy command, for testing
                MSG("DOING_NOTHING");
            }
            else {
                ERROR("INVALID_COMMAND");
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
        params[i] = Serial.parseInt();
    }
    return 0;
}


/**
 * Pouring procedure.
 * Waits for cup and turns each bottle in the order they were defined.
 */
void pouring_procedure(int* liquid_mg) {
    /*DEBUG_VAL_LN(liquid_mg);                               */
    /*if (!liquid_mg > 0) {                                  */
    /*    DEBUG_MSG_LN("Error. Please provide integer > 0.");*/
    /*    return;                                            */
    /*}                                                      */
    // TODO check MAX_DRINK_SIZE

    // TODO check if liquid_mg is significantly more than UPGRIGHT_OFFSET

    // wait for cup, wait until weight > WEIGHT_EPSILON or
    // CUP_TIMEOUT reached
    if (ads1231_get_milligrams() < WEIGHT_EPSILON) {
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
    for (int bottle = 0; bottle < bottles_nr; bottle++) {
        DEBUG_START();
        DEBUG_VAL(liquid_mg[bottle]);
        DEBUG_VAL(bottle);
        DEBUG_END();
        long cup_weight = ads1231_get_milligrams();

        DEBUG_VAL_LN(cup_weight);

        DEBUG_MSG_LN("Turning bottle down...");
        bottles[bottle].turn_down(TURN_DOWN_DELAY);

        // wait for requested weight
        // FIXME here we do not want WEIGHT_EPSILON and sharp >
        DEBUG_MSG_LN("Waiting for weight...");
        delay_until(POURING_TIMEOUT, cup_weight + liquid_mg[bottle] - UPGRIGHT_OFFSET);

        DEBUG_MSG_LN("Turn up again...");
        bottles[bottle].turn_up(TURN_UP_DELAY);

        int measured_output = ads1231_get_milligrams() - cup_weight;
        DEBUG_VAL_LN(measured_output);
    }

    // TODO send measured_output
    MSG("ENJOY");
}
