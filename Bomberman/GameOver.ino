#include "GameOver.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

GameOver::GameOver() {
  started = false;
  level = 1;
}

void GameOver::render(int dummy1, int buttonPressed) {
  if (started == false && buttonPressed != -1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Congrats! You've");
    lcd.setCursor(0, 1);
    lcd.print("reached ");
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
    started = false;
    level = 1;
    setGameState(GameState::MainMenu);
  } 
}

bool GameOver::isGameOver() {
  return true;
}
