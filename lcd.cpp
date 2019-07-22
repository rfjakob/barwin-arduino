#include <Arduino.h>

#include "lcd.h"


void Lcd::begin() {
    // this is probably width and height, right?
    liquid_crystal.begin(16, 2);
}


void Lcd::write(String msg, int line) {
    liquid_crystal.setCursor(0, line - 1) ;
    String msg_(msg);
    while((msg_.length()) < 16)
        msg_ = msg_ + String(" ");
    liquid_crystal.print(msg_);
}
