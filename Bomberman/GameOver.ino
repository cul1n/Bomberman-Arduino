#include "GameOver.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

GameOver::GameOver() {
  started = false;
  level = 1;
}

void GameOver::render(int dummy1, int buttonPressed) {
  if (started == false && buttonPressed == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Congrats! You've");
    lcd.setCursor(0, 1);
    lcd.print("reached ");
    level = EEPROM.read(statsLevelAddress);
    if (level == 10) {
      lcd.print("the end!");
    }
    else {
      lcd.print("level ");
      lcd.print(level);
      lcd.print("!");
    }
    started = true;
  }

  if (buttonPressed == 1) {
    const String whiteSpace = "       ";
    lcd.setCursor(0 ,0);
    byte byteType = 0;
    bool boolType = true;
    int intType = 0;
    if (EEPROM.get(statsHighScoreAddress, boolType)) {
      lcd.print("Highscore: ");
    }
    else {
      lcd.print("Score: ");
    }
    lcd.print(EEPROM.get(statsScoreAddress, intType));
    lcd.print("!");
    lcd.print(whiteSpace);
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    lcd.print(EEPROM.get(statsTimeAddress, intType));
    lcd.print(" ");
    lcd.write(byte(2));
    lcd.print("x");
    lcd.print(EEPROM.get(statsBombsAddress, intType));
    lcd.print(" ");
    lcd.write(byte(1));
    lcd.print("x");
    lcd.print(EEPROM.get(statsDamageTakenAddress, byteType));
    lcd.print(" ");
    if (EEPROM.get(statsSpreadAddress, boolType)) {
      lcd.write(byte(3));
    }
    lcd.print(whiteSpace);
  }

  if (buttonPressed == 2) {
    started = false;
    level = 1;
    EEPROM.put(statsHighScoreAddress, false);
    EEPROM.put(statsScoreAddress, 0);
    EEPROM.put(statsTimeAddress, 0);
    EEPROM.put(statsBombsAddress, 0);
    EEPROM.put(statsDamageTakenAddress, 0);
    EEPROM.put(statsSpreadAddress, false);
    setGameState(GameState::MainMenu);
  } 
}

bool GameOver::isGameOver() {
  return true;
}
