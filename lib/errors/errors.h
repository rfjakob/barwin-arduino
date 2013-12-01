/**
 * Error codes
 *
 * Used as return values of functions in case of errors. Should be globally
 * unique so they can passed on from one function to the next, e.g. using
 * RETURN_IFN_0(). On every error (except ABORTED?) an error message using
 * the macro name should be emitted using the ERROR() macro.
 */
#ifndef ERRORS_H
#define ERRORS_H

// Helper function
String strerror(int errno);

// Actual error codes
#define SUCCESS                      0

// return values by delay_until()
#define DELAY_UNTIL_TIMEOUT          1
#define BOTTLE_EMPTY                 2
#define WHERE_THE_FUCK_IS_THE_CUP    3

// misc pouring procedure related
#define CUP_TIMEOUT_REACHED          11    // no cup placed after POUR received
#define POURING_TIMEOUT_REACHED      14
#define ABORTED                      12    // user abort
#define MAX_DRINK_GRAMS_EXCEEDED     13    // sum of ingredients exceeds MAX_DRINK_GRAMS

// Serial message parsing
#define INVALID_COMMAND              21

// errors when communicating with ADS1231
#define ADS1231_TIMEOUT_HIGH         101   // Timeout waiting for HIGH
#define ADS1231_TIMEOUT_LOW          100   // Timeout waiting for LOW

#endif