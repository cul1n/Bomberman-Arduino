#include "GameOver.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

GameOver::GameOver() {
  started = false;
  level = 1;
}

void GameOver::render(int dummy1, int screen) {
  if (started == false && screen == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Congrats! You've"));
    lcd.setCursor(0, 1);
    lcd.print(F("reached "));
    level = EEPROM.read(statsLevelAddress);
    if (level == maxLevel + 1) {
      lcd.print(F("the end!"));
    }
    else {
      lcd.print(F("level "));
      lcd.print(level);
      lcd.print(F("!"));
    }
    started = true;
  }

  if (screen == 1) {
    lcd.setCursor(0 ,0);
    byte byteType = 0;
    bool boolType = true;
    int intType = 0;
    if (EEPROM.get(statsHighScoreAddress, boolType)) {
      lcd.print(F("Highscore: "));
    }
    else {
      lcd.print(F("Score: "));
    }
    lcd.print(EEPROM.get(statsScoreAddress, intType));
    lcd.print(F("!"));
    lcd.print(F("       "));
    lcd.setCursor(0, 1);
    lcd.write(byte(timeChar));
    lcd.print(EEPROM.get(statsTimeAddress, intType));
    lcd.print(F(" "));
    lcd.write(byte(bombChar));
    lcd.print(F("x"));
    lcd.print(EEPROM.get(statsBombsAddress, intType));
    lcd.print(F(" "));
    lcd.write(byte(heartChar));
    lcd.print(F("x"));
    lcd.print(EEPROM.get(statsDamageTakenAddress, byteType));
    lcd.print(F(" "));
    if (EEPROM.get(statsSpreadAddress, boolType)) {
      lcd.write(byte(bonusSpreadChar));
    }
    lcd.print(F("       "));
  }

  if (screen == numberOfScreens) {
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
