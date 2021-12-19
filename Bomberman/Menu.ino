#include "Menu.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

void Menu::render(int index, int lastIndex) {
  updateList(index);
  if (index % 2 == 0 && lastIndex == index - 1) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(">");
    lcd.print(listOfOptions[index]);
    if (index + 1 < numberOfOptions) {
      lcd.setCursor(1,1);
      lcd.print(listOfOptions[index + 1]);
    }
  }
  else if (index % 2 == 1 && lastIndex == index - 1) {
    lcd.setCursor(0,0);
    lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print(">");
  }
  else if (index % 2 == 0 && lastIndex == index + 1) {
      lcd.setCursor(0,0);
      lcd.print(">");
      lcd.setCursor(0,1);
      lcd.print(" ");
  }
  else if (index % 2 == 1 && lastIndex == index + 1) {
      if (index - 1 >= 0) {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print(listOfOptions[index - 1]);
      }
      lcd.setCursor(0,1);
      lcd.print(">");
      lcd.print(listOfOptions[index]);
  }
}


int Menu::getNumberOfOptions() {
  return numberOfOptions;
}
