#include <Servo.h> 

#include <ads1231.h>

#include "../config.h"


// TODO find out correct values...
#define SERVO_MIN 544
#define SERVO_MAX 2400

#define POS_BOTTLE_DOWN SERVO_MIN
#define POS_BOTTLE_UP   SERVO_MAX  // FIXME do we need SERVO_MIN here?
                                   // depends on how the servo is mounted!


#define DEBUG
#ifdef DEBUG
    #define DEBUG_MSG(msg) Serial.print(msg)
    #define DEBUG_MSG_LN(msg) Serial.println(msg)
    #define DEBUG_VAL(val) do { Serial.print(#val); \
                                Serial.print(": "); \
                                Serial.print(val);  \
                                Serial.print(", "); \
                            } while (0)

    #define DEBUG_VAL_LN(val) do { DEBUG_VAL(val); Serial.println();} while (0)
#else
    #define DEBUG_MSG(msg)
    #define DEBUG_MSG_LN(msg)

    #define DEBUG_VAL(val)
    #define DEBUG_VAL_LN(val)
#endif

Servo servo;
int last_pour_pos = POS_BOTTLE_UP;

void setup()
{
    Serial.begin(115200);
    Serial.println("READY");

    ads1231_init();

    servo.attach(SERVO1_PIN);
    servo.writeMicroseconds(POS_BOTTLE_UP);
}


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
 * TODO move this to a class method (class "Bottle", subclassing Servo?) for
 * more than one servo?
 */
int turn_until(int pos, long max_weight, int delay_ms) {
    // throw error if not between SERVO_MIN and SERVO_MAX?
    int current_pos = servo.readMicroseconds();
    if (pos == current_pos)
        return 0;
    int step = (current_pos < pos) ? 1 : -1;

    DEBUG_VAL(current_pos);
    DEBUG_VAL(step);
    DEBUG_VAL(max_weight);
    DEBUG_VAL(pos);
    DEBUG_VAL(delay_ms);
    DEBUG_MSG_LN("  - start turning...");
    unsigned long last_called = millis();
    for (int i = current_pos + step; i * step <= pos * step; i += step) {
        // abort if max_weight reached
        if (millis() > last_called + ADS1231_INTERVAL) {
            DEBUG_VAL_LN(i);
            last_called = millis();
            if (ads1231_get_milligrams() > max_weight + WEIGHT_EPSILON) {
                DEBUG_MSG_LN("Max weight reached. Aborting servo rotation.");
                return -1;
            }
        }
        delay(delay_ms);

        // turn servo one step
        servo.writeMicroseconds(i);
    }
    DEBUG_MSG_LN("Finished turning.");
    return 0;
}


void loop()
{
    /*
    turn_until(POS_BOTTLE_DOWN, 100000, 0);
    turn_until(POS_BOTTLE_UP, 100000, 1);
    long weight = ads1231_get_milligrams();
    long raw    = ads1231_get_value();
    DEBUG_VAL(weight);
    DEBUG_VAL(raw);
    DEBUG_MSG_LN("");

    delay(500);
    */
    if (Serial.available() > 0) {
        // where are the sources for Serial?
        // how to find out what parseInt does?
        int liquid_mg = Serial.parseInt();
        DEBUG_VAL_LN(liquid_mg);
        if (!liquid_mg > 0) {
            DEBUG_MSG_LN("Error. Please provide integer > 0.");
            return;
        }

        // TODO check if liquid_mg is significantly more than UPGRIGHT_OFFSET

        // wait for cup, wait until weight > WEIGHT_EPSILON or
        // CUP_TIMEOUT reached
        DEBUG_MSG("Waiting for cup...");
        if (delay_until(CUP_TIMEOUT, WEIGHT_EPSILON) == 0) {
            DEBUG_MSG_LN("CUP_TIMEOUT reached. Aborting.");
            return;
        }

        // wait a bit until cup weight can be measured safely
        delay(CUP_SETTLING_TIME);

        long cup_weight = ads1231_get_milligrams();

        DEBUG_VAL_LN(cup_weight);

        // TODO for each bottle
        // for () {
            DEBUG_MSG_LN("Turning fast to last_pour_pos...");
            turn_until(last_pour_pos, cup_weight, TURN_DOWN_FAST_DELAY);

            DEBUG_MSG_LN("Turning slow until pouring..");
            if (turn_until(POS_BOTTLE_DOWN, cup_weight, TURN_DOWN_DELAY) == 0) {
                DEBUG_MSG("Bottle empty? Position POS_BOTTLE_DOWN reached, no weight gain.");
                turn_until(POS_BOTTLE_UP, 10000, TURN_UP_DELAY);
                return;
            }

            // save current positon for next pouring
            last_pour_pos = servo.readMicroseconds();

            // wait for requested weight
            // FIXME here we do not want WEIGHT_EPSILON and sharp >
            DEBUG_MSG_LN("Waiting for weight...");
            delay_until(POURING_TIMEOUT, cup_weight + liquid_mg - UPGRIGHT_OFFSET);

            // FIXME do not pass 10 000 as  +inf weight, but find better solution
            DEBUG_MSG_LN("Turn up again...");
            turn_until(POS_BOTTLE_UP, 10000, TURN_UP_DELAY);

            // TODO measure real output
            int measured_output = ads1231_get_milligrams() - cup_weight;
            DEBUG_VAL_LN(measured_output);
        // }


        // TODO send success
    }
}
