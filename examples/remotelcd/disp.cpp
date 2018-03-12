#include "disp.h"

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

disp::disp(){;};

disp::~disp(){;};

void addchars()
{
  lcd.createChar(0, CUSTOM_0);
  lcd.createChar(1, CUSTOM_1);
  lcd.createChar(2, CUSTOM_2);
  lcd.createChar(3, CUSTOM_3);
  lcd.createChar(4, CUSTOM_4);
  lcd.createChar(5, CUSTOM_5);
  lcd.createChar(6, CUSTOM_6);
  lcd.createChar(7, CUSTOM_7);
}

void disp::print(char c){
  switch(c) 
  {
    case ASCII_CARRIAGE_RETURN :
      lcd.setCursor(0,1);
      break;
    case ASCII_END :
      lcd.clear();
      lcd.setCursor(0,0);
      break;
    default :
      if (c < 8)
      {
        lcd.write(byte(c));
        break;
      }
      if (c < 32)
      {
        break;
      }
      lcd.print(c);
  };
};

void disp::begin()
{
  lcd.begin(16, 2);
  addchars();
  lcd.clear();
  lcd.home();
  lcd.print("Booted !");
}
