#include <Arduino.h>
#include "errors.h"

/**
 * Returns the name for an error code. Keep in sync with errors.h (until a better
 * method to implement this comes along.
 *
 * The awkward formatting is on purpose: This way you spot immediately if the
 * case value and the returned string do not match (typos, copy paste errors).
 */
String strerror(int errno)
{
    switch(errno){
        case        DELAY_UNTIL_TIMEOUT:
            return "DELAY_UNTIL_TIMEOUT";
            break;
        case        BOTTLE_EMPTY:
            return "BOTTLE_EMPTY";
            break;
        case        WHERE_THE_FUCK_IS_THE_CUP:
            return "WHERE_THE_FUCK_IS_THE_CUP";
            break;
        case        CUP_TIMEOUT_REACHED:
            return "CUP_TIMEOUT_REACHED";
            break;
        case        POURING_TIMEOUT_REACHED:
            return "POURING_TIMEOUT_REACHED";
            break;
        case        ABORTED:
            return "ABORTED";
            break;
        case        MAX_DRINK_GRAMS_EXCEEDED:
            return "MAX_DRINK_GRAMS_EXCEEDED";
            break;
        case        INVALID_COMMAND:
            return "INVALID_COMMAND";
            break;
        case        ADS1231_TIMEOUT_HIGH:
            return "ADS1231_TIMEOUT_HIGH";
            break;
        case        ADS1231_TIMEOUT_LOW:
            return "ADS1231_TIMEOUT_LOW";
            break;
        default:
            return "UNDEFINED_ERROR_NUMBER_" + String(errno);
    }
}
