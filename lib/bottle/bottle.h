/*
 * Bottle class.
 */

#ifndef BOTTLE_H
#define BOTTLE_H

#include <Servo.h>

// Macro initialising a array 'bottles' of Bottle instances as defined
// in config.h by the comma separated list BOTTLES of constructor calls:
#define DEFINE_BOTTLES()  Bottle bottles[] = {BOTTLES};\
                          int bottles_nr = sizeof(bottles)/sizeof(bottles[0]);


class Bottle;

void bottles_init(Bottle* bottles, int bottles_nr);


class Bottle {
    public:
        Bottle(int, int, int);

        char name[50];  // human readable name, e.g "Vodka"
        Servo servo;    // servo used for turning the bottle
        char pin;


        #define SERVO_MIN 700
        #define SERVO_MAX 2000

        #define POS_BOTTLE_DOWN 1820// must be > SERVO_MIN
        #define POS_BOTTLE_UP   SERVO_MIN// FIXME do we need SERVO_MIN here?
                                   // depends on how the servo is mounted!

        int pos_down;   // servo position for bottle down (pouring)
        int pos_up;     // servo position for bottle up

        // TODO static?
        int turn_to(int pos,int delay_ms);
};

#endif

