#pragma once
#include "State.h"

class GameOver : public State {
  bool started;
  byte level;
  public:
    GameOver();
    
    void render(int, int);

    bool isGameOver() override;

    void setLevel(byte);
};
