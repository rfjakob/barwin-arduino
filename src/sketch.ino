#include <Servo.h> 

#include <ads1231.h>

#include "../config.h"


// TODO find out correct values...
#define SERVO_MIN 500
#define SERVO_MAX 2500

#define DEBUG 1
#ifdef DEBUG
    #define DEBUG_MSG(msg) Serial.print(msg)
    #define DEBUG_MSG_LN(msg) Serial.println(msg)
    #define DEBUG_VAL(val) do { Serial.print(#val); \
                                Serial.print(": "); \
                                Serial.print(val);  \
                                Serial.print(", "); \
                            } while (0)
#else
    #define DEBUG_MSG(msg) {}
    #define DEBUG_MSG_LN(msg) {}
#endif

Servo servo;

void setup()
{
    Serial.begin(115200);
    Serial.println("READY");

    ads1231_init();

    servo.attach(SERVO1_PIN);
}


/*
 * Turn servo towards 'pos' and stop at 'max_weight'. Return 0 if
 * 'pos' was reached or -1 if ''max_weight' was measured. Use 'delay_ms'
 * to set the speed of rotation (delay between two rotation steps, i.e.
 * speed = 1/delay).
 *
 * For details about the built-in Servo class see:
 *     /usr/share/arduino/libraries/Servo/Servo.cpp
 *
 * TODO move this to a class method
 */
int turn_until(int pos, long max_weight, int delay_ms) {
    // throw error if not between SERVO_MIN and SERVO_MAX?
    int current_pos = servo.readMicroseconds();
    int step = (current_pos < pos) ? 1 : -1;

    DEBUG_VAL(current_pos);
    DEBUG_VAL(step);
    DEBUG_VAL(max_weight);
    DEBUG_VAL(pos);
    DEBUG_VAL(delay_ms);
    DEBUG_MSG_LN("  - start turning...");
    for (int i = current_pos + step; i * step <= pos * step; i += step) {
        // delay and abort if max_weight reached
        if (delay_until(delay_ms, max_weight) < 0)
            return -1;

        // turn servo one step
        servo.writeMicroseconds(i);

        DEBUG_VAL(millis());
        DEBUG_VAL(ads1231_get_milligrams());
        DEBUG_MSG_LN(" - servo moved one step");
    }
    DEBUG_MSG_LN("Finished turning.");
    return 0;
}


void loop()
{
    turn_until(2000, 5, 50);
}
