#include "Intro.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

void Intro::render(int dummy1, int dummy2) {
  if (!started) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print(listOfLines[0]);
    lcd.setCursor(3, 1);
    lcd.print(listOfLines[1]);
    started = true;
    startTime = millis();
  }
  else if (millis() - startTime > waitTime) {
    started = false;
    setGameState(GameState::MainMenu);
    getGameState().render(0, -1);
  }
}

bool Intro::introSequence() {
  return true;
}
