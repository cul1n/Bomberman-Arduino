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
  Serial.println(explosions.length);
  byte xLastPos = p.getPos().getPosX();
  byte yLastPos = p.getPos().getPosY();
  bool bombSpawned = false;

  if (swChange && bombs.length < p.getNumberOfBombs()) {
    Bomb bomb(p.getPos().getPosX(), p.getPos().getPosY());
    bombSpawned = true;
    matrix[bomb.getPos().getPosX()][bomb.getPos().getPosY()] = 1;
    bombs.append(bomb);
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
  }
  matrixUpdate();
};

void InGame::matrixUpdate() {
  for (int i = 0; i < bombs.length; i++) {
    if (!bombs.getItem(i).stillActive()) {
      Explosion explosion(bombs.getItem(i).getPos().getPosX(), bombs.getItem(i).getPos().getPosY(), 1, 0);
      explosions.append(explosion);
      
      matrix[bombs.getItem(i).getPos().getPosX()][bombs.getItem(i).getPos().getPosY()] = 0;
      bombs.remove(i);
      i--;
    }
    else {
      matrix[bombs.getItem(i).getPos().getPosX()][bombs.getItem(i).getPos().getPosY()] = 1;
    }
  }

  int explosionsLength = explosions.length;
  // Expend the explosions
  for (int i = 0; i < explosionsLength; i++) {
    if (!explosions.getItem(i).isResolved()) {
      explosions.getItem(i).setResolved();
      byte x = explosions.getItem(i).getPos().getPosX();
      byte y = explosions.getItem(i).getPos().getPosY();
      byte spread = explosions.getItem(i).getSpread();
      byte dir = explosions.getItem(i).getDirection();

      Serial.print(i);
      Serial.print(" ");
      Serial.print(x);
      Serial.print(" ");
      Serial.print(y);
      Serial.print(" ");
      Serial.print(explosions.getItem(i).isResolved());
      Serial.println();
      switch(dir) {
        case 0: {
          Explosion explosion1(x - 1, y, spread - 1, 1);
          Explosion explosion2(x + 1, y, spread - 1, 2);
          Explosion explosion3(x, y - 1, spread - 1, 3);
          Explosion explosion4(x, y + 1, spread - 1, 4);
          explosions.append(explosion1);
          explosions.append(explosion2);
          explosions.append(explosion3);
          explosions.append(explosion4);
          break;
        }
        case 1: {
          Explosion explosion(x - 1, y, spread - 1, dir);
          explosions.append(explosion);
          break;
        }
        case 2: {
          Explosion explosion(x + 1, y, spread - 1, dir);
          explosions.append(explosion);
          break;
        }
        case 3: {
          Explosion explosion(x, y - 1, spread - 1, dir);
          explosions.append(explosion);
          break;
        }
        case 4: {
          Explosion explosion(x, y + 1, spread - 1, dir);
          explosions.append(explosion);
          break;
        }
        default: {
          break;
        }
      }
    }
    matrix[explosions.getItem(i).getPos().getPosX()][explosions.getItem(i).getPos().getPosY()] = 1;
  }

  for (int i = 0; i < explosions.length; i++) {
    if (!explosions.getItem(i).stillActive()) {
      matrix[explosions.getItem(i).getPos().getPosX()][explosions.getItem(i).getPos().getPosY()] = 0;
      explosions.remove(i);
      i--;
    }
  }
  
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}
