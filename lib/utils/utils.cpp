#include <Arduino.h>
#include "utils.h""
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
                    return 53;
                } else {
                    ERROR("INVALID_COMMAND");
                    DEBUG_MSG_LN(String("Got string '") + String(cmd) + String("'"));
                }
            }
        }
    }
}
