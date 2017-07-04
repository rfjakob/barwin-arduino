/**
 * Configuration file for the evobot Arduino code
 */

#ifndef CONFIG_H
#define CONFIG_H

// emulate a scale
//#define ADS1231_EMULATION 1

#define ADS1231_DATA_PIN A0
#define ADS1231_CLK_PIN  A1

#define WITHOUT_SCALE 1
#define MS_PER_GRAMS  50.


// Set to for button array (2 pins used per button)
#define USE_TWO_PIN_BUTTONS  1

// if USE_TWO_PIN_BUTTONS is set, set two values comma separated
#define ABORT_BTN_PIN    A5, 0
#define RESUME_BTN_PIN   A4, 0

// Predefined drinks for hardware buttons (pin2 only used if USE_TWO_PIN_BUTTONS is set)
// Note: Values not more than 255, because we use unsigned char!
// Drinks: Screw driver, Spezi, Long I.I. Tea, Cuba Libre
// Bottles: Vodka, Rum, Gin, Triple Sec, Orange juice, Lime, Cola
//
// A5...row 1      4...column 1
// A4...row 2      2...column 2
// A3...row 3      1...column 3
// A2...row 4      0...column 4
//
//                   amount in g for each bottle           PIN1  PIN2
#define DRINK_BTNS {{50,  0,   0,   0,   120,  0,   0,     A5,   4      }, \
                    {0,   0,   0,   0,   85,   10,  85,    A5,   2      }, \
                    {15,  20,  15,  15,  0,    25,  70,    A5,   1      }, \
                    {0,   45,  0,   0,   0,    10,  120,   A4,   4      } \
}

// Define bottles (number, pin, up/down position for servo)
//                     nr,  pin,  pos_down,  pos_up
#define BOTTLES Bottle(0,    3,   1200,    2350), \
                Bottle(1,    5,   1200,    2350), \
                Bottle(2,    6,   1200,    2350), \
                Bottle(3,    9,   1200,    2350), \
                Bottle(4,   10,   1200,    2350), \
                Bottle(5,   11,   1200,    2350), \
                Bottle(6,   13,   1200,    2350)

// ADC counts per milligram
#define ADS1231_DIVISOR  1565.1671343537414
// Zero offset, grams
//#define ADS1231_OFFSET   (127.97810572652163 - 3)
// ADS1231_OFFSET will be stored in EPROM on command TARE!

// How to calibrate using a weight (in grams) and the measured raw value
// as returned by ads1231_get_value():
// raw1    = -159119.02777777778
// weight1 = 0.
// raw2    = 761393.42307692312
// weight2 = 679.2
// ADS1231_DIVISOR = (raw1 - raw2) / (weight1 - weight2)
// ADS1231_OFFSET  = weight1 - (raw1 * (weight1 - weight2)) / (raw1 - raw2)

// Delay between single servo steps when turning bottle up/down, in milliseconds
#define TURN_DOWN_DELAY        2
#define TURN_UP_DELAY          4
#define FAST_TURN_UP_DELAY     1  // used for abort and init
#define CALIBRATION_TURN_DELAY 8  // used for calibrate_bottle_pos()
#define DANCING_DELAY          3  // used for DANCING_BOTTLES

// Time to wait until cup is placed on scale (in seconds)
#define CUP_TIMEOUT     180*1000

// Time to weight for a stable weight on scale
// if ads1231_get_stable_grams() is called
#define ADS1231_STABLE_MILLIS 5000

// Milliseconds to wait until desired weight reached. If timeout is reached, probably
// bottle is empty or screwed. This should never be reached because we should get a
// BOTTLE_EMPTY error first. Only if something goes terribly wrong (unknown what)
// this timeout will be reached.
#define POURING_TIMEOUT  20000

// Check if the bottle is empty (i.e. weight gain while pouring)
// every x milliseconds
#define BOTTLE_EMPTY_INTERVAL 1500

// On BOTTE_EMPTY error, move the bottle to pos_up and add following value.
// This turns the bottle a bit back and makes the error visible.
// Note that pos_up + BOTTLE_EMPTY_POS_OFFSET must be < 2400!
#define BOTTLE_EMPTY_POS_OFFSET -200

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

// If the requested and the measured output for at least one ingredient is
// larger, an ERROR message will be printed instead of ENJOY (in grams).
#define MAX_POUR_ERROR  20

#endif
