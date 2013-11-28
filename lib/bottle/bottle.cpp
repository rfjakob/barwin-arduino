/**
 * Bottle class.
 */

#include <Arduino.h>
#include "bottle.h"
#include "ads1231.h"
#include <utils.h>
#include "../../config.h"


/*
 * Init bottles.
 * Attach servos and turn to initial position.
 * 'bottles' is an array (size 'bottles_nr') of Bottle objects.
 */
void Bottle::init(Bottle* bottles, int bottles_nr) {
    for (int i=0; i < bottles_nr; i++) {
        bottles[i].servo.attach(bottles[i].pin);
        bottles[i].servo.writeMicroseconds(bottles[i].pos_up); // Make sure bottle is pointing up
        delay(500);
    }
}


/**
 * Bottle constructor.
 * Initialize values of member variables. Note: Servo cannot be used here.
 * This should be done in the setup function, not in the global scope using
 * bottles_init().
 */
Bottle::Bottle(String _name, int _pin, int _pos_down, int _pos_up) :
    name(_name), pin(_pin), pos_down(_pos_down), pos_up(_pos_up) {
}

/**
 * Turn servo towards 'pos' in 1 microsecond steps, waiting delay_ms
 * milliseconds between steps (speed = 1/delay).
 * Returns 0 when the position is reached or -1 on error.
 *
 * For details about the built-in Servo class see:
 *     /usr/share/arduino/libraries/Servo/Servo.cpp
 *
 */
int Bottle::turn_to(int pos, int delay_ms, bool print_steps) {
    if (pos < SERVO_MIN || pos > SERVO_MAX) {
        DEBUG_MSG_LN("Error turning bottle, wrong servo pos!");
        return -1;
    }

    int current_pos = servo.readMicroseconds();
    if (pos == current_pos)
        return 0;
    int step = (current_pos < pos) ? 1 : -1;

    DEBUG_START();
    DEBUG_MSG("Start turning bottle '");
    DEBUG_MSG(name);
    DEBUG_MSG("' with params: ");
    DEBUG_VAL(current_pos);
    DEBUG_VAL(step);
    DEBUG_VAL(pos);
    DEBUG_VAL(delay_ms);
    DEBUG_END();
    unsigned long last_called = millis();
    for (int i = current_pos + step; i * step <= pos * step; i += step) {
        //                             ˆˆˆˆˆˆ        ˆˆˆˆˆˆ
        //                             this inverts the relation if turning down

        // Warning: printing to serial delays turning!
        if (print_steps && i % 10 == 0) {
            DEBUG_VAL_LN(i);
        }
        delay(delay_ms);

        // turn servo one step
        servo.writeMicroseconds(i);
    }
    DEBUG_START();
    DEBUG_MSG("Finished turning bottle '");
    DEBUG_MSG(name);
    DEBUG_MSG("'.");
    DEBUG_END();
    return 0;
}

/**
 * Turn bottle to upright position.
 */
int Bottle::turn_up(int delay_ms, bool print_steps) {
    return turn_to(pos_up, delay_ms, print_steps);
}

/**
 * Turn bottle to pouring position.
 */
int Bottle::turn_down(int delay_ms, bool print_steps) {
    return turn_to(pos_down, delay_ms, print_steps);
}

/**
 * Turn bottle to pause position.
 * Used e.g. in case of WHERE_THE_FUCK_IS_THE_CUP error.
 */
int Bottle::turn_to_pause_pos(int delay_ms, bool print_steps) {
    return turn_to((pos_down + pos_up) / 2.0, delay_ms, print_steps);
}


/**
 * Pour requested_amount grams from bottle..
 * Return 0 on success, other values are return values of
 * delay_until (including scale error codes).
 */
int Bottle::pour(int requested_amount, int& measured_amount) {
    // orig_weight is weight including ingredients poured until now
    int orig_weight, ret;
    ret = ads1231_get_grams(orig_weight);
    if (ret != 0) {
        ERROR(String("SCALE_ERROR ") + String(ret));
        return ret;
    }

    MSG(String("POURING ") + name + String(" ") + String(orig_weight));

    DEBUG_START();
    DEBUG_MSG("Start pouring bottle: ");
    DEBUG_VAL(name);
    DEBUG_VAL(requested_amount);
    DEBUG_VAL(orig_weight);
    DEBUG_END();

    while(1) {
        DEBUG_MSG_LN("Turning bottle down...");
        turn_down(TURN_DOWN_DELAY);

        // wait for requested weight
        // FIXME here we do not want WEIGHT_EPSILON and sharp >
        DEBUG_MSG_LN("Waiting for weight...");
        ret = delay_until(POURING_TIMEOUT,
                orig_weight + requested_amount - UPGRIGHT_OFFSET, true);
        if (ret == 0)
            break; // All good

        ERROR(String("DELAY_UNTIL ") + String(ret));
        // move bottle to pause position (in the middle)

        // Bottle empty
        if(ret == 2) {
            ERROR("BOTTE_EMPTY");
            // TODO other speed here? it is empty already!
            turn_up(TURN_UP_DELAY);
            RETURN_IFN_0(wait_for_resume());
        }

        // Cup was removed early
        if(ret == 3) {
            turn_to_pause_pos(TURN_UP_DELAY);
            // TODO abort command should be processed in waiting_for_cup()
            RETURN_IFN_0(wait_for_cup());
        }
    }

    DEBUG_MSG_LN("Turn up again...");
    turn_up(TURN_UP_DELAY);

    ads1231_get_grams(measured_amount);
    measured_amount -= orig_weight;

    DEBUG_START();
    DEBUG_MSG("Bottle statistics: ");
    DEBUG_VAL(name);
    DEBUG_VAL(requested_amount);
    DEBUG_VAL(measured_amount);
    DEBUG_END();
}
