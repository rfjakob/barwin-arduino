


#include <Arduino.h>

#include "bottle.h"
#include <utils.h>
#include "../../config.h"



/*
 * Turn servo towards 'pos' and stop if servo is at position 'pos' or if weight
 * is more than max_weight WEIGHT_EPSILON.
 * Returns 0 if 'pos' was reached or -1 if 'max_weight' was measured. Use
 * 'delay_ms' to set the speed of rotation (delay between two rotation steps,
 * i.e. speed = 1/delay).
 *
 * For details about the built-in Servo class see:
 *     /usr/share/arduino/libraries/Servo/Servo.cpp
 *
 */
int Bottle::turn_to(int pos, int delay_ms) {
    // throw error if not between SERVO_MIN and SERVO_MAX?
    int current_pos = servo.readMicroseconds();
    if (pos == current_pos)
        return 0;
    int step = (current_pos < pos) ? 1 : -1;

    DEBUG_VAL(current_pos);
    DEBUG_VAL(step);
    DEBUG_VAL(pos);
    DEBUG_VAL(delay_ms);
    DEBUG_MSG_LN("  - start turning...");
    unsigned long last_called = millis();
    for (int i = current_pos + step; i * step <= pos * step; i += step) {
        // printing to serial delays turning!
        //DEBUG_VAL_LN(i);
        delay(delay_ms);

        // turn servo one step
        servo.writeMicroseconds(i);
    }
    DEBUG_MSG_LN("Finished turning.");
    return 0;
}

