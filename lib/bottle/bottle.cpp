/**
 * Bottle class.
 */

#include <Arduino.h>
#include "bottle.h"
#include "ads1231.h"
#include <utils.h>
#include "errors.h"
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
Bottle::Bottle(int _number, int _pin, int _pos_down, int _pos_up) :
    number(_number), pin(_pin), pos_down(_pos_down), pos_up(_pos_up) {
}

/**
 * Turn servo towards 'pos' in 1 microsecond steps, waiting delay_ms
 * milliseconds between steps (speed = 1/delay). If check_weight weight
 * is true, might abort with WHERE_THE_FUCK_IS_THE_CUP error.
 * Returns 0 when the position is reached or SERVO_OUT_OF_RANGE on error.
 *
 * For details about the built-in Servo class see:
 *     /usr/share/arduino/libraries/Servo/Servo.cpp
 *
 */
int Bottle::turn_to(int pos, int delay_ms, bool print_steps, bool check_weight) {
    if (pos < SERVO_MIN || pos > SERVO_MAX) {
        DEBUG_MSG_LN("Error turning bottle, wrong servo pos!");
        return SERVO_OUT_OF_RANGE;
    }

    int current_pos = servo.readMicroseconds();
    if (pos == current_pos)
        return 0;
    int step = (current_pos < pos) ? 1 : -1;

    DEBUG_START();
    DEBUG_MSG("Start turning bottle '");
    DEBUG_MSG(number);
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

        // Return if we should abort...
        RETURN_IFN_0(check_aborted());

        if (check_weight) {
            int weight;
            int ret = ads1231_get_noblock(weight);
            if (ret != ADS1231_WOULD_BLOCK) {
                if (weight < WEIGHT_EPSILON) {
                    DEBUG_MSG_LN("While turning bottle: where is the cup?");
                    return WHERE_THE_FUCK_IS_THE_CUP;
                }
            }
            // it would take too long to get weight...
            else delay(delay_ms);
        } else {
            delay(delay_ms);
        }

        // turn servo one step
        servo.writeMicroseconds(i);
    }
    DEBUG_START();
    DEBUG_MSG("Finished turning bottle '");
    DEBUG_MSG(number);
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
int Bottle::turn_down(int delay_ms, bool print_steps, bool check_weight) {
    return turn_to(pos_down, delay_ms, print_steps, check_weight);
}

/**
 * Returns the pause position for this bottle (unit: microseconds)
 */
int Bottle::get_pause_pos()
{
	return (pos_down + pos_up) / 2;
}

/**
 * Turn bottle to pause position.
 * Used e.g. in case of WHERE_THE_FUCK_IS_THE_CUP error.
 */
int Bottle::turn_to_pause_pos(int delay_ms, bool print_steps) {
    return turn_to(get_pause_pos(), delay_ms, print_steps);
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
        ERROR(strerror(ret));
        return ret;
    }

    MSG(String("POURING ") + String(number) + String(" ") + String(orig_weight));

    DEBUG_START();
    DEBUG_MSG("Start pouring bottle: ");
    DEBUG_VAL(requested_amount);
    DEBUG_VAL(orig_weight);
    DEBUG_END();

    while(1) {
        DEBUG_MSG_LN("Turning bottle down...");
        ret = turn_down(TURN_DOWN_DELAY, false, true); // enable check_weight

        // wait for requested weight
        // FIXME here we do not want WEIGHT_EPSILON and sharp >
        if (ret == 0) {
            DEBUG_MSG_LN("Waiting for weight...");
            ret = delay_until(POURING_TIMEOUT,
                    orig_weight + requested_amount - UPGRIGHT_OFFSET, true);
        }
        if (ret == 0)
            break; // All good

        DEBUG_MSG_LN(String("pour(): delay until returned error ") + String(ret));

        // Bottle empty
        // Note that this does not work if requested_amount is less than
        // UPGRIGHT_OFFSET!
        if(ret == BOTTLE_EMPTY) {
            ERROR("BOTTE_EMPTY");
            // TODO other speed here? it is empty already!
            turn_to(pos_up + BOTTLE_EMPTY_POS_OFFSET, TURN_UP_DELAY);
            RETURN_IFN_0(wait_for_resume()); // might return ABORTED
        }
        // Cup was removed early
        else if(ret == WHERE_THE_FUCK_IS_THE_CUP) {
            turn_to_pause_pos(FAST_TURN_UP_DELAY);
            // TODO abort command should be processed in wait_for_cup()
            RETURN_IFN_0(wait_for_cup());
        }
        // other error - turn bottle up and return error code
        // includes: scale error, user abort, ...
        else {
            turn_up(FAST_TURN_UP_DELAY);
            return ret;
        }
    }

    // We turn to pause pos and not completely up so we can crossfade
    DEBUG_MSG_LN("Bottle:pour(): Turining to pause pos");
    turn_to_pause_pos(TURN_UP_DELAY);

    ads1231_get_grams(measured_amount);
    measured_amount -= orig_weight;

    DEBUG_START();
    DEBUG_MSG("Bottle statistics: ");
    DEBUG_VAL(requested_amount);
    DEBUG_VAL(measured_amount);
    DEBUG_END();
    return 0;
}
