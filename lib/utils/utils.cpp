#include <Arduino.h>
#include "utils.h""
#include "bottle.h"
#include "errors.h"
#include "../../config.h"

/**
 * Used to call something every 'time_period' milliseconds.
 *
 * This function is similar to TimedAction, but much more primitive.
 * http://playground.arduino.cc/Code/TimedAction
 *
 * See also IF_HAS_TIME_PASSED() macro.
 */
bool has_time_passed(long time_period, long& last_passed) {
    long now = millis();
    if (now - last_passed > time_period) {
        last_passed = now;
        return true;
    }
    else
        return false;
}

/**
 * Waits to get RESUME on the serial.
 * Returns 0 if it gets it,
 * EABORT if it gets ABORT.
 */
int wait_for_resume() {
    while(1) {
        if (Serial.available() > 0) {
            char cmd[MAX_COMMAND_LENGTH] = "";
            if(Serial.readBytesUntil(' ', cmd, MAX_COMMAND_LENGTH)) {
                String cmd_str = String(cmd);
                if (cmd_str.equals("RESUME")) {
                    break;
                } else if (cmd_str.equals("ABORT")) {
                    DEBUG_MSG_LN("Aborted.");
                    return ABORTED;
                } else {
                    ERROR("INVALID_COMMAND");
                    DEBUG_MSG_LN(String("Got string '") + String(cmd) + String("'"));
                }
            }
        }
    }
}

/**
 * Turns bottle 1 up while simultaneously turning bottle 2 down to
 * pause position. Works best if bottle 1 is at pause position at start.
 *
 * In audio, this is called a crossfade:
 *
 * b2 ______     ______ up
 *          \   /
 *           \ /
 *            x
 *           / \
 * b1 ______/   \______ pause position
 */
void crossfade(Bottle * b1, Bottle * b2, int delay_ms)
{
    int i = 0;
    int b1_start_pos = b1->servo.readMicroseconds();
    int b2_start_pos = b2->servo.readMicroseconds();
    int new_pos=0;
    bool done_something;

    while(1)
    {
        done_something=false;

        // Turn bottle 1 up
        new_pos = b1_start_pos + i;
        if(new_pos < b1->pos_up)
        {
            b1->servo.writeMicroseconds(new_pos);
            done_something=true;
        }

        delay(delay_ms/2); // Split delay in half so bottles move alternating

        // Turn bottle 2 down to pause position
        new_pos = b2_start_pos - i;
        if(new_pos > b2->get_pause_pos() )
        {
            b2->servo.writeMicroseconds(new_pos);
            done_something=true;
        }

        // Both bottles are already in position
        if(done_something==false)
            break;

        delay(delay_ms/2);
    }
}
