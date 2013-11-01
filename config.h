/*
 * Configuration file for the evobot Arduino code
 */

#ifndef CONFIG_H
#define CONFIG_H


#define ADS1231_DATA_PIN 7
#define ADS1231_CLK_PIN  6

// ADC counts per milligram
#define ADS1231_DIVISOR  1237
// Zero offset, milligrams
#define ADS1231_OFFSET   126

// How to calibrate using a weight (in milligrams) and the measured raw value
// as returned by ads1231_get_value():
// raw1    = -155481.0
// weight1 = 0.
// raw2    = 646414.0
// weight2 = 648.3
// ADS1231_DIVISOR = (raw1 - raw2) / (weight1 - weight2)
// ADS1231_OFFSET  = weight1 - (raw1 * (weight1 - weight2)) / (raw1 - raw2)

// PWM pin for servo number 1
#define SERVO1_PIN      11

// Delay between single servo steps when turning bottle up/down
#define TURN_DOWN_DELAY 10
#define TURN_DOWN_FAST_DELAY 5
#define TURN_UP_DELAY 2

// Time to wait until cup is placed on scale (in seconds)
#define CUP_TIMEOUT     180

// Delay after cup placed on scale (before setting zero point for first
// ingredient), in milliseconds
#define CUP_SETTLING_TIME

// Time to wait until desired weight reached. If timeout is reached, probably
// bottle is empty or screwed.
#define POURING_TIMEOUT  20000

// Amount of liquid in milligrams poured while turning the bottle up
#define UPGRIGHT_OFFSET  5

// When waiting for changes of weight on scale, ignore changes less than... (in milligrams)
#define WEIGHT_EPSILON  1

// Wait at least x milliseconds before calling ads1231_get_milligrams() again
// when turning servo, because it takes to long to call it always. Increasing
// this value makes the servo faster but the reponse time to the scale slower.
#define ADS1231_INTERVAL 100


#endif
