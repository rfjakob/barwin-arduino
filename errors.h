/**
 * Error codes
 *
 * Used as return values of functions in case of errors. Should be globally
 * unique so they can passed on from one function to the next, e.g. using
 * RETURN_IFN_0(). *Not passing* an error code to the next function (using
 * RETURN_IFN_0) is similar to catching an exception, while passing an error
 * to the next function is similar to an uncaught exception which is passed to
 * the next function. The internet says that nobody uses exceptions on
 * microcontrollers. Actually after messing around with error codes, I ask
 * myself why I trust the internet.
 *
 * If you do not pass an error code until top level, you should print it using
 * the macro ERROR(). For errors passed until top this is done in do_stuff().
 */
#ifndef ERRORS_H
#define ERRORS_H

typedef unsigned char errv_t;

// Helper function
String c_strerror(errv_t errno);

// return values by delay_until()
#define DELAY_UNTIL_TIMEOUT          1
#define BOTTLE_EMPTY                 2
#define WHERE_THE_FUCK_IS_THE_CUP    3

// misc pouring procedure related
#define CUP_TIMEOUT_REACHED          11    // no cup placed after POUR received
#define POURING_TIMEOUT_REACHED      14
#define ABORTED                      12    // user abort
#define MAX_DRINK_GRAMS_EXCEEDED     13    // sum of ingredients exceeds MAX_DRINK_GRAMS
#define SERVO_OUT_OF_RANGE           15    // if Bottle::turn_to is called with wrong pos
#define RESUMED                      16    // after BOTTLE_EMPTY
#define WEIGHT_NOT_STABLE            17    // if turn_to() reaches pos before weight stable
#define POURING_INACCURATE         18    // requested and measured amount differ too much

// Serial message parsing
#define INVALID_COMMAND              21

// errors when communicating with ADS1231
#define ADS1231_TIMEOUT_HIGH         101   // Timeout waiting for HIGH
#define ADS1231_TIMEOUT_LOW          100   // Timeout waiting for LOW
#define ADS1231_WOULD_BLOCK          102   // weight not measured, measuring takes too long
#define ADS1231_STABLE_TIMEOUT       103   // weight not stable within timeout

#endif
