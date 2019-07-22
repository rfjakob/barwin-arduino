#ifndef LCD_H
#define LCD_H
#include <LiquidCrystal.h>

void to_lcd(String msg, int line);


class Lcd {
    public:
        Lcd(int rs, int en, int d4, int d5, int d6, int d7):
            liquid_crystal(rs, en, d4, d5, d6, d7) { };

        void write(String msg, int line);
        void begin();

    private:
        const LiquidCrystal liquid_crystal;
};

#endif
