#pragma once
#include "State.h"
#include "Entity.h"
#include "List.h"

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
  char playerName[maxNameLength + 1];
  unsigned long long int startTime;
  int maxTime;
  byte level;
  byte currentRoom;
  int score;
  byte shopIndex;
  bool items[numberOfItems];

  // Animation timers
  byte bombBlinker;
  byte enemyBlinker;
  byte playerBlinker;

  // Variables used to keep track of stats
  int timePlayed;
  int bombsPlaced;
  bool upgrade;
  
  byte matrix[matrixSize][matrixSize];
  
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
