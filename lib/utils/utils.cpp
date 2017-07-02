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
errv_t wait_for_resume() {
    while(1) {
        errv_t ret = check_aborted(true);
        if (ret == RESUMED) {
            return 0;
        }
        else if (ret) {
            return ret;
        }
    }
}

/**
 * Check if button is pressed. Assumes that pin is configured as input with
 * (internal or external) pullup.
 */
bool is_button_pressed(int pin) {
    return (digitalRead(pin) == LOW);
}


/**
 * Check if button in matrix keypad is pressed. pin1 and pin2 define row/column
 * of button. pin1 has pullup, pin2 is output and must be always high and set
 * to low while reading from pin1 for this button.
 */
bool is_button_pressed(int pin1, int pin2) {
    digitalWrite(pin2, LOW);
    bool is_pressed = (digitalRead(pin1) == LOW);
    digitalWrite(pin2, HIGH);
    return is_pressed;
}


/**
 * Check if we should abort whatever we ware doing right now.
 * Returns 0 if we should not abort, ABORTED if we should abort.
 *
 * If receive_resume is set to true, 'RESUME' is a valid cmd as well.
 */
errv_t check_aborted(bool receive_resume) {
    int ret = 0;
    if (Serial.available() > 0) {
        char cmd[8+1];
        // The conversion to String depends on having a trailing NULL!
        memset(cmd, 0, 8+1);

        // TODO this is not nice, other parsing code is in loop()
        // move to one function somehow?
        // XXX why do we use readBytes here and not readBytesUntil as in
        // sketch.ino? Why 8 bytes and not MAX_COMMAND_LENGTH?
        //    --> introduced in commit 612367ef
        if(Serial.readBytes(cmd, 8)) {
            String cmd_str = String(cmd);
            if (cmd_str.equals("ABORT\r\n")) {
                ret = ABORTED;
            }
            else if (receive_resume && cmd_str.equals("RESUME\r\n")) {
                DEBUG_MSG_LN(String("Free mem: ") + String(get_free_memory()));
                return RESUMED;
            } else {
                ERROR(strerror(INVALID_COMMAND));
                DEBUG_MSG_LN(String("check_aborted: got '") + String(cmd) + String("'"));
            }
        }
        // To save some bytes commenting out:
        //else {
        //    // we check above Serial.available() > 0 --> can we get here?
        //    DEBUG_MSG_LN("check_aborted: something went wrong");
        //}
    }
    else if (is_button_pressed(ABORT_BTN_PIN)) { // pull up inverts logic!
        ret = ABORTED;
    }
    else if (receive_resume && is_button_pressed(RESUME_BTN_PIN)) {
        return RESUMED;
    }

    return ret;
}

/**
 * Blocks until user abort or max_dalay (in ms) is reached. Will wait
 * infinitely for user abort if max_delay is negative.
 * Return values:   see also errors.h!
 *  0 max_delay was reached (success)
 *  ABORTED 
 */
errv_t delay_abortable(long max_delay) {
    unsigned long start = millis();
    while(1) {
        if(max_delay > 0 && millis() - start > max_delay)
            return 0;

        RETURN_IFN_0(check_aborted());
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
 *
 * Returns ABORTED if aborted.
 */
errv_t crossfade(Bottle * b1, Bottle * b2, int delay_ms) {
    int step = 1;
    int b1_pos = b1->servo.readMicroseconds();
    int b2_pos = b2->servo.readMicroseconds();
    int new_pos=0;
    bool done_something;

    DEBUG_MSG_LN("crossfade " + String(b1->number) + String(" ") + String(b2->number));

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

        if (check_aborted()) {
            // no other cleanup, other bottles should be alright
            b1->turn_up(FAST_TURN_UP_DELAY);
            b2->turn_up(FAST_TURN_UP_DELAY);
            return ABORTED;
        }
    }

    return 0;
}
