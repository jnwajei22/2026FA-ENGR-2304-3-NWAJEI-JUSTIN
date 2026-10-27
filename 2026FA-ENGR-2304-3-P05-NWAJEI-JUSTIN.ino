/* 2026FA-ENGR-2304-3-P05-NWAJEI-JUSTIN */

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
    lcd.begin(16, 2);
    lcd.print("Hello World");
}

void loop()
{
}