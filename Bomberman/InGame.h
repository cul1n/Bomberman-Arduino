#pragma once
#include "State.h"
#include "Entity.h"
#include "List.h"

const byte bombId = 1;
const byte explosionId = 2;
const byte breakableWallId = 3;
const byte solidWallId = 4;
const byte playerId = 5;
const byte gateId = 6;
const byte enemyId = 7;

class InGame : public State {
  Player p;
  ShortList<Bomb> bombs;
  List<Explosion> explosions;
  ShortList<Enemy> enemies; 
  bool levelStarted;
  bool gameStarted;
  bool nextRoom;
  bool shop;
  bool shopDisplayed;
  char playerName[7];
  unsigned long long int startTime;
  int maxTime;
  byte level;
  byte currentRoom;
  int score;
  byte shopIndex;
  bool items[3];
  
  byte bombBlinker;
  byte enemyBlinker;
  byte matrix[8][8] = {
    {4, 4, 4, 4, 4, 4, 4, 4},
    {4, 0, 0, 0, 0, 0, 0, 4},
    {4, 0, 0, 0, 0, 0, 0, 4},
    {0, 0, 0, 0, 0, 0, 0, 4},
    {0, 0, 0, 0, 0, 0, 0, 4},
    {4, 0, 0, 0, 0, 0, 0, 4},
    {4, 0, 0, 0, 0, 0, 0, 4},
    {4, 4, 4, 4, 4, 4, 4, 4} 
  };
  
  public:
    InGame();
    
    void render(int index, int lastIndex);

    bool isPlaying() override;

    void playerController(int, int, bool);

    void matrixUpdate();

    void updateTimer(); 

    void updateHealth();

    void updateBombs();

    void updateScore();

    void updateLevel();

    void updateHighScore(int);

    void gameOver();

    void clearRoom();
    
    void generateRoom();
};
