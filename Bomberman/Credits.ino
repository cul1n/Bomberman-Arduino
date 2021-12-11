#include "Credits.h"

void Credits::update(int index) {
  switch (index) {
    case 3:
      setGameState(GameState::MainMenu);
      break;
    default:
      break;
  }
}
