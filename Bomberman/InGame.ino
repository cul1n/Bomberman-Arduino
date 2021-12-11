#include "InGame.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

InGame::InGame() : p(0, 0) {
  matrix[0][0] = 1;
}

void InGame::render(int index, int lastIndex) {
}

bool InGame::isPlaying() {
  return true;
}

void InGame::playerController(int xChange, int yChange, bool swChange) {
  byte xLastPos = p.getPos().getPosX();
  byte yLastPos = p.getPos().getPosY();
  bool bombSpawned = false;

  if (swChange) {
    Entity bomb(p.getPos().getPosX(), p.getPos().getPosY());
    bombSpawned = true;
    if (bombs.length >= p.getNumberOfBombs()) {
      matrix[bombs.getItem(0).getPos().getPosX()][bombs.getItem(0).getPos().getPosY()] = 0;
      bombs.remove(0);
      bombs.append(bomb);
      matrix[bomb.getPos().getPosX()][bomb.getPos().getPosY()] = 1;
    }
    else {
      matrix[bomb.getPos().getPosX()][bomb.getPos().getPosY()] = 1;
      bombs.append(bomb);
    }
  }
  
  if (xChange == -1) {
    if (p.getPos().getPosX() > 0) {
      p.modifyPos(-1, 0);
    }
    else {
      p.modifyPos(7, 0);
    }
  }
  
  if (xChange == 1) {
    if (p.getPos().getPosX() < 7) {
      p.modifyPos(1, 0);
    }
    else {
      p.modifyPos(-7, 0);
    }
  }

  if (yChange == -1) {
    if (p.getPos().getPosY() > 0) {
      p.modifyPos(0, -1);
    }
    else {
      p.modifyPos(0, 7);
    }
  }
  
  if (yChange == 1) {
    if (p.getPos().getPosY() < 7) {
      p.modifyPos(0, 1);
    }
    else {
      p.modifyPos(0, -7);
    }
  }

  if (xLastPos != p.getPos().getPosX() || yLastPos != p.getPos().getPosY()) {
    matrix[p.getPos().getPosX()][p.getPos().getPosY()] = 1;
    matrix[xLastPos][yLastPos] = 0;
    if (!bombSpawned) {
      matrix[xLastPos][yLastPos] = 0;
    }
    matrixUpdate();
  }
};

void InGame::matrixUpdate() {
  for (int i = 0; i < bombs.length; i++) {
    matrix[bombs.getItem(i).getPos().getPosX()][bombs.getItem(i).getPos().getPosY()] = 1;
  }
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}
