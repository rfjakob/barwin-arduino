/*
 * Configuration file for the evobot Arduino code
 */

#ifndef CONFIG_H
#define CONFIG_H


#define ADS1231_DATA_PIN 7
#define ADS1231_CLK_PIN  6

// ADC counts per milligram
#define ADS1231_DIVISOR  1
// Zero offset, milligrams
#define ADS1231_OFFSET   0

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



#endif