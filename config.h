/**
 * Configuration file for the evobot Arduino code
 */

#ifndef CONFIG_H
#define CONFIG_H


#define ADS1231_DATA_PIN A0
#define ADS1231_CLK_PIN  A1

// Define bottles (number, pin, up/down position for servo)
//                     nr,  pin,  pos_down,  pos_up
#define BOTTLES Bottle(0,    3,   1200,    2380), \
                Bottle(1,    5,   1220,    2400), \
                Bottle(2,    6,   1220,    2400), \
                Bottle(3,    9,   1140,    2320), \
                Bottle(4,   10,    950,    2100), \
                Bottle(5,   11,   1240,    2420), \
                Bottle(6,   13,    920,    2100)

// ADC counts per milligram
#define ADS1231_DIVISOR  1565.1671343537414
// Zero offset, grams
#define ADS1231_OFFSET   (127.97810572652163 - 3)

// How to calibrate using a weight (in grams) and the measured raw value
// as returned by ads1231_get_value():
// raw1    = -159119.02777777778
// weight1 = 0.
// raw2    = 761393.42307692312
// weight2 = 679.2
// ADS1231_DIVISOR = (raw1 - raw2) / (weight1 - weight2)
// ADS1231_OFFSET  = weight1 - (raw1 * (weight1 - weight2)) / (raw1 - raw2)

// Delay between single servo steps when turning bottle up/down, in milliseconds
#define TURN_DOWN_DELAY 4
#define TURN_UP_DELAY 4
#define FAST_TURN_UP_DELAY 1      // used for abort and init
#define CALIBRATION_TURN_DELAY 8  // used for calibrate_bottle_pos()

// Time to wait until cup is placed on scale (in seconds)
#define CUP_TIMEOUT     180*1000

// Delay after cup placed on scale (before setting zero point for first
// ingredient), in milliseconds
#define CUP_SETTLING_TIME 1500

// Milliseconds to wait until desired weight reached. If timeout is reached, probably
// bottle is empty or screwed. This should never be reached because we should get a
// BOTTLE_EMPTY error first. Only if something goes terribly wrong (unknown what)
// this timeout will be reached.
#define POURING_TIMEOUT  20000

// Check if the bottle is empty (i.e. weight gain while pouring)
// every x milliseconds
#define BOTTLE_EMPTY_INTERVAL 1500

// Stop pouring early in grams to account for the liquid pouring out while turning
// the bottle up
#define UPGRIGHT_OFFSET  17

// When waiting for changes of weight on scale, ignore changes less than... (in grams)
#define WEIGHT_EPSILON  2

// If ready, send READY message every x milliseconds
#define SEND_READY_INTERVAL 2000

// Wait at least x milliseconds before calling ads1231_get_grams() again
// when turning servo, because it takes to long to call it always. Increasing
// this value makes the servo faster but the response time to the scale slower.
#define ADS1231_INTERVAL 200

// Commands must be sent faster than SERIAL_TIMEOUT milliseconds.
#define SERIAL_TIMEOUT 50

// max length of serial commands, number of characters
#define MAX_COMMAND_LENGTH 50

// For safety: we will never pour more than this amount at once (in grams)
// (not per bottle, but per pouring procedure)
#define MAX_DRINK_GRAMS 250

#endif
