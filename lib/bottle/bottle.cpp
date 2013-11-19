/**
 * Bottle class.
 */

#include <Arduino.h>
#include "bottle.h"
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
