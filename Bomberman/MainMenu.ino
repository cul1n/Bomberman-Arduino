#include "MainMenu.h"

void MainMenu::update(int index) {
  switch (index) {
    case 0:
      setGameState(GameState::InGame);
      break;
    case 1:
      setGameState(GameState::SettingsMenu);
      break;
    case 2:
      setGameState(GameState::HighScoreMenu);
      break;
    case 3:
      setGameState(GameState::Credits);
      break;
  }
}

void MainMenu::updateList(byte index) {
  switch (index) {
    case 0:
      displayIcon(playIcon);
      break;
    case 1:
      displayIcon(settingsIcon);
      break;
    case 2:
      displayIcon(highScoreIcon);
      break;
    case 3:
      displayIcon(creditsIcon);
      break;
  }
}

void MainMenu::displayIcon(uint64_t icon) {
  for (int i = 0; i < numberOfBytes; i++) {
    byte row = (icon >> i * numberOfBytes) & 0xFF;
    
    for (int j = 0; j < numberOfBytes; j++) {
      lc.setLed(0, i, j, bitRead(row, j));
    }
    
  }
}
