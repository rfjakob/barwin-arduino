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
 * Get free memory (in bytes?).
 * From: http://forum.pololu.com/viewtopic.php?f=10&t=989
 */
extern char __bss_end;
extern char *__brkval;

int get_free_memory()
{
  int free_memory;

  if((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return free_memory;
}

/**
 * Waits to get RESUME on the serial.
 * Returns 0 if it gets it,
 * ABORTED if it gets ABORT.
 */
int wait_for_resume() {
    while(1) {
        if (Serial.available() > 0) {
            char cmd[8+1];
            // The conversion to String depends on having a trailing NULL!
            memset(cmd, 0, 8+1);

            if(Serial.readBytes(cmd, 8)) {
                String cmd_str = String(cmd);
                if (cmd_str.equals("RESUME\r\n")) {
                    DEBUG_MSG(String("Free memory: ") + String(get_free_memory()));
                    break;
                } else if (cmd_str.equals("ABORT\r\n")) {
                    DEBUG_MSG_LN("Aborted.");
                    return ABORTED;
                } else {
                    ERROR(strerror(INVALID_COMMAND));
                    DEBUG_MSG_LN(String("Got string '") + String(cmd) + String("'"));
                }
            }
        }
    }
    return 0;
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
void crossfade(Bottle * b1, Bottle * b2, int delay_ms) {
    int step = 1;
    int b1_pos = b1->servo.readMicroseconds();
    int b2_pos = b2->servo.readMicroseconds();
    int new_pos=0;
    bool done_something;

    while(1) {
        done_something=false;

        // Turn bottle 1 up
        b1_pos += step;
        if(b1_pos < b1->pos_up) {
            b1->servo.writeMicroseconds(b1_pos);
            done_something=true;
        }

        delay(delay_ms/2); // Split delay in half so bottles move alternating

        // Turn bottle 2 down to pause position
        b2_pos -= step;
        if(b2_pos > b2->get_pause_pos() ) {
            b2->servo.writeMicroseconds(b2_pos);
            done_something=true;
        }

        // Both bottles are already in position
        if(done_something==false)
            break;

        delay(delay_ms/2);
    }
}
