#include "HighScoreMenu.h"

void HighScoreMenu::update(int index) {
  switch (index) {
    case 4:
      setGameState(GameState::MainMenu);
      break;
    default:
      break;
  }
}

void HighScoreMenu::updateList() {
  for (byte i = 0; i < 3; i++) {
    if (EEPROM.read(highScoreAddress + i * (maxNameLength + sizeof(int))) != 0) {
      String score = "";
      int scoreValue = 0;
      for (byte j = 0; j < maxNameLength; j++) {
        char c = EEPROM.read(highScoreAddress + i * (maxNameLength + sizeof(int)) + j);
        if (c != 0) score += c;
        else score += " ";
      }
      score += " ";
      score += String(EEPROM.get(highScoreAddress + (i + 1) * maxNameLength + i * sizeof(int), scoreValue));
      listOfOptions[i + 1] = score;
    }
  }
}
