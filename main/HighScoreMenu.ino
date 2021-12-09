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
