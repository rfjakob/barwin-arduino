#include <ads1231.h>
#include <bottle.h>
#include <utils.h>
#include "../config.h"


// number of bottles (and therefore also servos)
#define BOTTLES_CNT 2

//Bottle bottles[BOTTLES_CNT];

INIT_BOTTLES();

void setup()
{
    #if defined(__AVR_ATmega328P__)
    Serial.begin(9600);
    #else
    Serial.begin(115200);
    #endif
    Serial.println("READY");

    ads1231_init();

    bottles[0].pin = 6;
    bottles[1].pin = 11;

    for (int bottle = 0; bottle < BOTTLES_CNT; bottle++) {
        bottles[bottle].pos_down = POS_BOTTLE_DOWN;
        bottles[bottle].pos_up   = POS_BOTTLE_UP;
        bottles[bottle].servo.attach(bottles[bottle].pin);
        bottles[bottle].servo.writeMicroseconds(bottles[bottle].pos_up);
    }
}

int long lastweight =  millis();


void loop()
{
    long weight = ads1231_get_milligrams();
    //long raw    = ads1231_get_value();
    if (millis() - lastweight > 500) {
        DEBUG_VAL_LN(weight);
        lastweight = millis();
    }
    //DEBUG_VAL(raw);

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

        // Pour liquid for each bottle
        for (int bottle = 0; bottle < BOTTLES_CNT; bottle++) {
            DEBUG_VAL_LN(bottle);
            long cup_weight = ads1231_get_milligrams();

            DEBUG_VAL_LN(cup_weight);

            DEBUG_MSG_LN("Turning bottle down...");
            bottles[bottle].turn_to(bottles[bottle].pos_down, TURN_DOWN_DELAY);

            // wait for requested weight
            // FIXME here we do not want WEIGHT_EPSILON and sharp >
            DEBUG_MSG_LN("Waiting for weight...");
            delay_until(POURING_TIMEOUT, cup_weight + liquid_mg - UPGRIGHT_OFFSET);

            DEBUG_MSG_LN("Turn up again...");
            bottles[bottle].turn_to(bottles[bottle].pos_up, TURN_UP_DELAY);

            int measured_output = ads1231_get_milligrams() - cup_weight;
            DEBUG_VAL_LN(measured_output);
        }


        // TODO send success
    }
}
