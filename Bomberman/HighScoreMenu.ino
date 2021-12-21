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

void HighScoreMenu::updateList(byte dummy) {
  for (byte i = 0; i < numberOfHighScores; i++) {
    String score = String(i + 1) + ".";
    
    if (EEPROM.read(highScoreAddress + i * (maxNameLength + sizeof(int))) != 0) {
      int scoreValue = 0;
      
      for (byte j = 0; j < maxNameLength; j++) {
        char character = EEPROM.read(highScoreAddress + i * (maxNameLength + sizeof(int)) + j);
        
        if (character != 0) {
          score += character;
        }
        
        else {
          score += " ";
        }
      }
      
      score += " ";
      score += String(EEPROM.get(highScoreAddress + (i + 1) * maxNameLength + i * sizeof(int), scoreValue));
    }
    
    else {
      score += noPlayer;
    }

    listOfOptions[i + 1] = score;
  }
}
