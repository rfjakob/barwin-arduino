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
        case        SUCCESS:
            return "SUCCESS";

        case        DELAY_UNTIL_TIMEOUT:
            return "DELAY_UNTIL_TIMEOUT";

        case        BOTTLE_EMPTY:
            return "BOTTLE_EMPTY";

        case        WHERE_THE_FUCK_IS_THE_CUP:
            return "WHERE_THE_FUCK_IS_THE_CUP";

        case        CUP_TIMEOUT_REACHED:
            return "CUP_TIMEOUT_REACHED";

        case        POURING_TIMEOUT_REACHED:
            return "POURING_TIMEOUT_REACHED";

        case        ABORTED:
            return "ABORTED";

        case        MAX_DRINK_GRAMS_EXCEEDED:
            return "MAX_DRINK_GRAMS_EXCEEDED";

        case        INVALID_COMMAND:
            return "INVALID_COMMAND";

        case        ADS1231_TIMEOUT_HIGH:
            return "ADS1231_TIMEOUT_HIGH";

        case        ADS1231_TIMEOUT_LOW:
            return "ADS1231_TIMEOUT_LOW";

        default:
            return "UNDEFINED_ERROR_NUMBER_" + String(errno);
    }
}
