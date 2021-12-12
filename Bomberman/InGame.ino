#include "InGame.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

InGame::InGame() : p(1, 1) {
  playerName = "";
  for (int i = 0; i < 6; i++) {
    char c = EEPROM.read(i);
    if (c != 0) {
      playerName += c;
    }
  }
  if (!playerName.length())
    playerName = "A";
  matrix[1][1] = playerId;
  level = 1;
  levelStarted = true;
}

void InGame::render(int index, int lastIndex) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(playerName);
  lcd.setCursor(7, 0);
  lcd.print("LV01");
  lcd.setCursor(12, 0);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.print("x");
  lcd.print(p.getPlayerHealth());
  lcd.setCursor(4,1);
  lcd.write(byte(2));
  lcd.print("x");
  lcd.print(p.getNumberOfBombs());
  lcd.setCursor(8,1);
  lcd.print("SCORE");
}

bool InGame::isPlaying() {
  return true;
}


int lastTimer = 0;
void InGame::updateTimer() {
  int timer = maxTime - (millis() - startTime) / 1000;
  if (lastTimer != timer) {
    lcd.setCursor(13, 0);
    lcd.print("   ");
    lcd.setCursor(13, 0);
    lcd.print(timer);
  }
  lastTimer = timer;
}


void InGame::updateHealth() {
  lcd.setCursor(2, 1);
  lcd.print(p.getPlayerHealth());
}

void InGame::updateBombs() {
  lcd.setCursor(6, 1);
  lcd.print(p.getNumberOfBombs() - bombs.length);
}


void InGame::playerController(int xChange, int yChange, bool swChange) {
  if (levelStarted) {
    startTime = millis();
    maxTime = 25 * level;
    render(0, 0);
    levelStarted = false;  
  }
  
  updateTimer();
  
  byte xLastPos = p.getPos().getPosX();
  byte yLastPos = p.getPos().getPosY();
  bool bombSpawned = false;

  if (matrix[xLastPos][yLastPos] == explosionId) {
    p.loseHealth();
    updateHealth();
  }
  
  if (swChange && bombs.length < p.getNumberOfBombs()) {
    Bomb bomb(p.getPos().getPosX(), p.getPos().getPosY());
    bombSpawned = true;
    matrix[bomb.getPos().getPosX()][bomb.getPos().getPosY()] = 1;
    bombs.append(bomb);
    updateBombs();
  }

  if (xChange == -1) {
    if (p.getPos().getPosX() > 0) {
      if (matrix[xLastPos - 1][yLastPos] != breakableWallId && matrix[xLastPos - 1][yLastPos] != solidWallId)
        p.modifyPos(-1, 0);
    }
    else {
      p.modifyPos(7, 0);
    }
  }

  else if (xChange == 1) {
    if (p.getPos().getPosX() < 7) {
      if (matrix[xLastPos + 1][yLastPos] != breakableWallId && matrix[xLastPos + 1][yLastPos] != solidWallId)
        p.modifyPos(1, 0);
    }
    else {
      p.modifyPos(-7, 0);
    }
  }

  else if (yChange == -1) {
    if (p.getPos().getPosY() > 0) {
      if (matrix[xLastPos][yLastPos - 1] != breakableWallId && matrix[xLastPos][yLastPos - 1] != solidWallId)
        p.modifyPos(0, -1);
    }
    else {
      p.modifyPos(0, 7);
    }
  }

  else if (yChange == 1) {
    if (p.getPos().getPosY() < 7) {
      if (matrix[xLastPos][yLastPos + 1] != breakableWallId && matrix[xLastPos][yLastPos + 1] != solidWallId)
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
      Explosion explosion(bombs.getItem(i).getPos().getPosX(), bombs.getItem(i).getPos().getPosY(), 2, 0);
      explosions.append(explosion);

      matrix[bombs.getItem(i).getPos().getPosX()][bombs.getItem(i).getPos().getPosY()] = 0;
      bombs.remove(i);
      updateBombs();
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

      switch (dir) {
        case 0: {
            if (matrix[x - 1][y] == breakableWallId)
              matrix[x - 1][y] = 0;
            else if (matrix[x - 1][y] != solidWallId) {
              Explosion explosion(x - 1, y, spread - 1, 1);
              explosions.append(explosion);
            }

            if (matrix[x + 1][y] == breakableWallId)
              matrix[x + 1][y] = 0;
            else if (matrix[x + 1][y] != solidWallId) {
              Explosion explosion(x + 1, y, spread - 1, 2);
              explosions.append(explosion);
            }

            if (matrix[x][y - 1] == breakableWallId)
              matrix[x][y - 1] = 0;
            else if (matrix[x][y - 1] != solidWallId) {
              Explosion explosion(x, y - 1, spread - 1, 3);
              explosions.append(explosion);
            }

            if (matrix[x][y + 1] == breakableWallId)
              matrix[x][y + 1] = 0;
            else if (matrix[x][y + 1] != solidWallId) {
              Explosion explosion(x, y + 1, spread - 1, 4);
              explosions.append(explosion);
            }

            break;
          }
        case 1: {
            if (matrix[x - 1][y] == breakableWallId)
              matrix[x - 1][y] = 0;
            else if (matrix[x - 1][y] != solidWallId) {
              Explosion explosion(x - 1, y, spread - 1, dir);
              explosions.append(explosion);
            }
            break;
          }
        case 2: {
            if (matrix[x + 1][y] == breakableWallId)
              matrix[x + 1][y] = 0;
            else if (matrix[x + 1][y] != solidWallId) {
              Explosion explosion(x + 1, y, spread - 1, dir);
              explosions.append(explosion);
            }
            break;
          }
        case 3: {
            if (matrix[x][y - 1] == breakableWallId)
              matrix[x][y - 1] = 0;
            else if (matrix[x][y - 1] != solidWallId) {
              Explosion explosion(x, y - 1, spread - 1, dir);
              explosions.append(explosion);
            }
            break;
          }
        case 4: {
            if (matrix[x][y + 1] == breakableWallId)
              matrix[x][y + 1] = 0;
            else if (matrix[x][y + 1] != solidWallId) {
              Explosion explosion(x, y + 1, spread - 1, dir);
              explosions.append(explosion);
            }
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
    else {
      matrix[explosions.getItem(i).getPos().getPosX()][explosions.getItem(i).getPos().getPosY()] = explosionId;
    }
  }

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}
