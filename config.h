/**
 * Configuration file for the evobot Arduino code
 */

#ifndef CONFIG_H
#define CONFIG_H


#define ADS1231_DATA_PIN A0
#define ADS1231_CLK_PIN  A1

// Define bottles (pin, name, up/down position for servo)
//                     pin,  pos_down,  pos_up
#define BOTTLES Bottle("Vodka", 6,    1820,      1100), \
                Bottle("Orange", 11,   1820,      1100)//, \
                //Bottle(5,    1820,      700), \
                //Bottle(10,   1820,      700), \
                //Bottle(9,    1820,      700), \
                //Bottle(3,    1820,      700), \
                //Bottle(13,   1820,      700)

// ADC counts per milligram
#define ADS1231_DIVISOR  1355.2892385964383
// Zero offset, milligrams
#define ADS1231_OFFSET   117.40595530926247 + 76.0

// How to calibrate using a weight (in milligrams) and the measured raw value
// as returned by ads1231_get_value():
// raw1    = -159119.02777777778
// weight1 = 0.
// raw2    = 761393.42307692312
// weight2 = 679.2
// ADS1231_DIVISOR = (raw1 - raw2) / (weight1 - weight2)
// ADS1231_OFFSET  = weight1 - (raw1 * (weight1 - weight2)) / (raw1 - raw2)

// Delay between single servo steps when turning bottle up/down
#define TURN_DOWN_DELAY 4
#define TURN_UP_DELAY 4
#define FAST_TURN_UP_DELAY 2      // used for abort and init
#define CALIBRATION_TURN_DELAY 8  // used for calibrate_bottle_pos()

// Time to wait until cup is placed on scale (in seconds)
#define CUP_TIMEOUT     180*1000

// Delay after cup placed on scale (before setting zero point for first
// ingredient), in milliseconds
#define CUP_SETTLING_TIME 1500

// Time to wait until desired weight reached. If timeout is reached, probably
// bottle is empty or screwed.
#define POURING_TIMEOUT  20000

// Amount of liquid in milligrams poured while turning the bottle up
#define UPGRIGHT_OFFSET  20

// When waiting for changes of weight on scale, ignore changes less than... (in milligrams)
#define WEIGHT_EPSILON  2

// If ready, send READY message every x milliseconds
#define SEND_READY_INTERVAL 500

// Wait at least x milliseconds before calling ads1231_get_milligrams() again
// when turning servo, because it takes to long to call it always. Increasing
// this value makes the servo faster but the response time to the scale slower.
#define ADS1231_INTERVAL 200

// Commands must be sent faster than SERIAL_TIMEOUT milliseconds.
#define SERIAL_TIMEOUT 50

// max length of serial commands, number of characters
#define MAX_COMMAND_LENGTH 50

// For safety: we will never pour more than this amount at once (in milligrams)
// (not per bottle, but per pouring procedure)
#define MAX_DRINK_SIZE 400

#endif
