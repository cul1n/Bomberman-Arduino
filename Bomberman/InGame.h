#pragma once
#include "State.h"
#include "Entity.h"
#include "List.h"

const byte bombId = 1;
const byte explosionId = 2;
const byte breakableWallId = 3;
const byte solidWallId = 4;
const byte playerId = 5;

class InGame : public State {
  const int matrixSize = 8;
  Player p;
  List<Bomb> bombs;
  List<Explosion> explosions;
  byte matrix[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 3, 0, 3, 0, 0},
    {0, 0, 0, 3, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}  
  };
  
  public:
    InGame();
    
    void render(int index, int lastIndex);

    bool isPlaying() override;

    void playerController(int, int, bool);

    void matrixUpdate();
    
};
