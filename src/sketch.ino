/**
 * sketch.ino: barwin-arduino
 *
 * This file contains the basic procedure of pouring a cocktail. Weight/scale
 * specific routines are defined in lib ads1231. Also in utils and errors you
 * might find some stuff useful for other Arduino projects. The class Bottles
 * and this file contains only barwin related code. Receiving and parsing
 * serial messages, is done here. Everything which needs to be stored or done
 * for each bottle should be in the Bottle class.
 *
 * General information on how to use the Arduno part of the barwin project
 * should be documented in the README.md. General description (web interface,
 * construction specific, other stuff) should be collected in other repositories
 * or the web page.
 *
 * See also:
 *      https://barwin.suuf.cc
 *      https://github.com/petres/genBotWI
 */

#include <Arduino.h>
#include <ads1231.h>
#include <bottle.h>
#include <utils.h>
#include <errors.h>
#include "../config.h"


// Macro Magic that creates and initializes the variables
//     Bottle bottles[]
//     int bottles_nr
// Macro is defined in bottle.h.
DEFINE_BOTTLES();

// FIXME currently hardcoded: array size for second dimension must
// be at least bottles_nr + 1 (i.e. 8 in most cases)
unsigned char drink_btns[][8] = DRINK_BTNS;

errv_t pour_cocktail(int* requested_amount);
void parse_int_params(int* params, int size);
void init_drink_btns();
void process_drink_btns();

void setup() {
    pinMode(ABORT_BTN_PIN, INPUT_PULLUP);
    pinMode(RESUME_BTN_PIN, INPUT_PULLUP);
    init_drink_btns();

    // This is obligatory on the Uno, and a noop on the Leonardo.
    // Means we can just do it unconditionally.
    Serial.begin(9600);
    Serial.setTimeout(SERIAL_TIMEOUT);

    ads1231_init();
    Bottle::init(bottles, bottles_nr);

    // Warn users of emulation mode to avoid unnecessary debugging...
    #ifdef ADS1231_EMULATION
    DEBUG_MSG_LN("Scale emulation active");
    #endif

    DEBUG_MSG_LN("setup() end");
}


void loop() {
    // print some stuff every SEND_READY_INTERVAL milliseconds while idle
    IF_HAS_TIME_PASSED(SEND_READY_INTERVAL)  {
        int ret, weight = 0;
        ret = ads1231_get_grams(weight);
        if (ret != 0) {
            ERROR(strerror(ret));
            return;
        }

        // send message: READY weight is_cup_there
        String msg = String("READY ")
            + String(weight) + String(" ")
            + String(weight > WEIGHT_EPSILON ? 1 : 0);
        MSG(msg);

        // XXX often used debugging code to get raw weight value:
        //long weight_raw;
        //ads1231_get_value(weight_raw);
        //DEBUG_VAL_LN(weight_raw);
    }

    // Parse commands from Serial
    if (Serial.available() > 0) {

        char cmd[MAX_COMMAND_LENGTH + 1];
        // The conversion to String depends on having a trailing NULL!
        memset(cmd, 0, MAX_COMMAND_LENGTH + 1);

        // readBytesUntil() will block until SERIAL_TIMEOUT is reached
        // or a space is read from serial input. It returns the number of bytes
        // read.
        if(Serial.readBytesUntil(' ', cmd, MAX_COMMAND_LENGTH) == 0)
            return; // Can that even happen??

        String cmd_str = String(cmd);

        // Example: POUR 0 20 10 30 10 0 40\r\n
        if (cmd_str.equals("POUR")) {
            int requested_amount[bottles_nr];
            parse_int_params(requested_amount, bottles_nr); // Also handles the "\r\n"
            pour_cocktail(requested_amount);
        }
        // Example: TURN_BOTTLE 3 2100\r\n
        else if (cmd_str.equals("TURN")) {
            // turn bottle to specific position
            int params[2];
            parse_int_params(params, 2); // Also handles the "\r\n"

            // bottle number (int starting at 0) first parameter, position
            // as microseconds second parameter
            bottles[params[0]].turn_to(params[1], TURN_DOWN_DELAY);
        }
        // Example: ECHO ENJOY\r\n
        // Arduino will then print "ENJOY"
        // This is a workaround to resend garbled messages manually.
        // see also: https://github.com/rfjakob/barwin-arduino/issues/5
        else if (cmd_str.equals("ECHO")) {
            DEBUG_MSG_LN("Got ECHO");
            // Clear buffer for reuse
            memset(cmd, 0, MAX_COMMAND_LENGTH + 1);
            // Read rest of command
            Serial.readBytesUntil('\r', cmd, MAX_COMMAND_LENGTH);
            // Print it out
            MSG(cmd);
        }
        // Example: TARE\r\n
        else if (cmd_str.equals("TARE\r\n")) {
            int weight;
            DEBUG_MSG_LN("Measuring");
            int ret = ads1231_get_grams(weight);
            if (ret != 0) {
                ERROR(strerror(ret));
                return;
            }
            ads1231_additional_offset = -weight;
            DEBUG_MSG_LN(
                String("Scale tared to ") + String(-weight)
            );
        }
        // Example: DANCE\r\n
        else if (cmd_str.equals("DANCE\r\n")) {
            dancing_bottles();
        }
        // Example: NOP\r\n
        // readBytesUntil read the trailing "\r\n" because there was no " " to stop at
        else if (cmd_str.equals("NOP\r\n")) {
            // dummy command, for testing
            MSG("NOP");
        }
        else {
            ERROR(strerror(INVALID_COMMAND));
            DEBUG_MSG_LN(String("Got '") + String(cmd) + String("'"));
        }
    } else {
        process_drink_btns();
    }

}


/**
 * Initializes hardware buttons for predefined drinks.
 */
void init_drink_btns() {
    char drink_btns_nr = sizeof(drink_btns)/sizeof(drink_btns[0]);
    for (int i = 0; i < drink_btns_nr; i++) {
        Serial.println(drink_btns[i][0]);
        pinMode(drink_btns[i][0], INPUT_PULLUP);
    }
}


/**
 * Check if any of the hardware buttons for predefined drinks is currently pressed.
 * If yes call pour the according drink.
 *
 * Note that this function should run quite fast and needs to be called often
 * (i.e. fast polling in loop()), otherwise the event is not detected.
 */
void process_drink_btns(){
     char drink_btns_nr = sizeof(drink_btns)/sizeof(drink_btns[0]);
    for (int i = 0; i < drink_btns_nr; i++) {
        if(digitalRead(drink_btns[i][0]) == LOW){
            // Button of i-th predefined drink pressed
            // TODO typecast
            int requested_amount[bottles_nr];

            // TODO is this the best way to cast an char array --> int array?
            for (int j = 0; j < bottles_nr; j++) {
                requested_amount[j] = drink_btns[i][j+1];
            }
            pour_cocktail(requested_amount);
            break;
        }
    }
}


/**
 * Parse space separated int values from Serial to array.
 */
void parse_int_params(int* params, int size) {
    for (int i = 0; i < size; i++) {
        // see /usr/share/arduino/hardware/arduino/cores/arduino/Stream.cpp:138
        // Note: parseInt returns 0 in case of an parsing error... (uargh!)
        params[i] = Serial.parseInt();
    }

    // The line is terminated by \r\n - read two characters and throw them away.
    char junk[2];
    Serial.readBytes(junk, 2);
}

/**
 * Pouring procedure.
 * Waits for cup and turns each bottle in the order they were defined.
 * 'requested_amount' is the amount of liquid in grams to be poured from
 * each bottle(int array of size bottles_nr).
 */
errv_t pour_cocktail(int* requested_amount) {

    // Sanity check: Never pour more than MAX_DRINK_GRAMS
    long sum = 0; // Use long to rule out overflow
    for (int i = 0; i < bottles_nr; i++) {
        sum += requested_amount[i];
    }
    if(sum > MAX_DRINK_GRAMS) {
        ERROR(strerror(MAX_DRINK_GRAMS_EXCEEDED));
        return MAX_DRINK_GRAMS_EXCEEDED;
    }

    // wait until weight > WEIGHT_EPSILON or CUP_TIMEOUT reached
    RETURN_IFN_0(wait_for_cup());

    // wait a bit until cup weight can be measured safely
    // TODO is this obsolete since we have ads1231_get_stable_grams?
    delay(CUP_SETTLING_TIME);

    // Actually poured liquid for each bottle
    int measured_amount[bottles_nr];
    // initializing array with 0
    memset(measured_amount, 0, sizeof(int) * bottles_nr);

    Bottle *cur_bottle = NULL;
    Bottle *last_bottle = NULL;
    for (int i = 0; i < bottles_nr; i++) {

        // This bottle is not used for the cocktail. Skip it silently.
        if (requested_amount[i] == 0) {
            continue;
        }

        // we cannot pour less than UPGRIGHT_OFFSET --> do not pour if it is
        // less than UPGRIGHT_OFFSET/2 and print warning...
        if (requested_amount[i] < UPGRIGHT_OFFSET) {
            if (UPGRIGHT_OFFSET / 2 > requested_amount[i]) {
                DEBUG_MSG_LN("Will not pour");
                continue;
            } else {
                DEBUG_MSG_LN("Will pour too much");
            }
        }

        cur_bottle = &bottles[i];

        if (last_bottle != 0) { // On the first iteration last_bottle is NULL
            crossfade(last_bottle, cur_bottle, TURN_UP_DELAY);
            // At this point, last_bottle is up and cur_bottle is at pause position
        }

        int ret;
        ret = cur_bottle->pour(requested_amount[i], measured_amount[i]);
        // At this point, cur_bottle is at pause position again. Next crossfade
        // will turn it up completely.

        if (ret == ABORTED) {
            cur_bottle->turn_up(FAST_TURN_UP_DELAY);
            return ret;
        }
        else if (ret != 0) {
            DEBUG_MSG_LN(String("pour_cocktail: got ") + String(ret));
            ERROR(strerror(ret));
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

    delay_until(-1, 0, false, true);
}

/**
 * If the bot is bored it lets the bottles dance! :)
 */
void dancing_bottles() {
    Bottle *cur_bottle = NULL;
    Bottle *last_bottle = NULL;
    bottles[0].turn_to_pause_pos(DANCING_DELAY);
    for (int i = 0; i < bottles_nr; i++) {
        cur_bottle = &bottles[i];

        if (last_bottle != 0) { // On the first iteration last_bottle is NULL
            crossfade(last_bottle, cur_bottle, DANCING_DELAY);
            // At this point, last_bottle is up and cur_bottle is at pause position
        }
        // At this point, cur_bottle is at pause position again. Next crossfade
        // will turn it up completely.

        // Save bottle for next iteration
        last_bottle=cur_bottle;
    }
    last_bottle->turn_up(DANCING_DELAY);
}
