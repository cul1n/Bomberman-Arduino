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
