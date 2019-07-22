#include <Arduino.h>

#include "lcd.h"
#include "config.h"


const int rs = 53, en = 52, d4 = 51, d5 = 50, d6 = 49, d7 = 48;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void start_lcd() {
    // this is probably width and height, right?
    lcd.begin(16, 2);
}


void print_lcd(String msg, int line) {
    lcd.setCursor(0, line - 1) ;
    String msg_(msg);
    while((msg_.length()) < 16)
        msg_ = msg_ + String(" ");
    lcd.print(msg_);
}
