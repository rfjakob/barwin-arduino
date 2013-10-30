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


int turn_until(int max_pos, long max_weight, int delay_ms) {
    for (int i = SERVO_MIN(); i <= SERVO_MAX(); i++) {
        if (delay_until(delay_ms, max_weight) < 0)
            return -1;
        servo.writeMicroseconds(i);
        // see /usr/share/arduino/libraries/Servo/Servo.cpp
    }
    return 0;
}


void loop()
{
    long mg;
    int pos=1000;
    int dir=1;

/*
    while(1)
    {
        mg=ads1231_get_milligrams();
        if(mg > ADS1231_ERR)
        {
            Serial.print("Timeout, code ");
            Serial.println(mg - ADS1231_ERR);
        }
        else
            Serial.println(mg);
    }
*/


    while(1)
    {
        Serial.println(pos);
        servo.writeMicroseconds(pos);

        pos+=dir;

        if(pos>=2500)
            dir=-1;

        if(pos<=500)
            dir=1;

        delay(1);
    }

}
