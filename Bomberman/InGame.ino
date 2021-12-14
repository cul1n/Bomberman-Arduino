#include "InGame.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

InGame::InGame() : p(3, 0) {
  matrix[1][1] = playerId;
  level = 1;
  levelStarted = true;
  gameStarted = true;
  nextRoom = true;
  score = 0;
  bombBlink = 0;
}

void InGame::render(int index, int lastIndex) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(playerName);
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
  lcd.print("SCR: ");
}

bool InGame::isPlaying() {
  return true;
}

void InGame::generateRoom() {
  p.setPos(3, 0);
  for (byte i = 1; i < 7; i++) {
    for (byte j = 1 ; j < 7; j++) {
      matrix[i][j] = 0;
    }
  }
  // TO DO: make enter gate switch places
  matrix[0][3] = solidWallId;
  matrix[0][4] = solidWallId;
  matrix[3][7] = solidWallId;
  matrix[4][7] = solidWallId;
  matrix[7][3] = solidWallId;
  matrix[7][4] = solidWallId;
  
  byte gatePosition = random(3);
  
  if (gatePosition == 0) {
    matrix[0][3] = gateId;
    matrix[0][4] = gateId;
  }
  else if (gatePosition == 1) {
    matrix[3][7] = gateId;
    matrix[4][7] = gateId;
  }
  else if (gatePosition == 2) {
    matrix[7][3] = gateId;
    matrix[7][4] = gateId;
  }

  byte numberOfBlocks = ((level + 1) / 2) * 5;

  while (numberOfBlocks) {
    byte i = random(1, 7);
    byte j = random(1, 7);
    if (i != 3 || j != 1) {
      if (matrix[i][j] != breakableWallId) {
        matrix[i][j] = breakableWallId;
        numberOfBlocks--;
      }
    }
  }
  
}

int lastTimer = 0;
void InGame::updateTimer() {
  int timer = maxTime - (millis() - startTime) / 1000;
  if (timer == -1) {
    gameOver();
  }
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
  if (!p.getPlayerHealth()) {
    gameOver();
  }
}

void InGame::updateBombs() {
  lcd.setCursor(6, 1);
  lcd.print(p.getNumberOfBombs() - bombs.length);
}

void InGame::updateScore() {
  lcd.setCursor(13, 1);
  lcd.print("   ");
  lcd.setCursor(13, 1);
  lcd.print(score);
}

void InGame::updateLevel() {
  lcd.setCursor(7, 0);
  lcd.print("LV");
  if ( level != 10) {
    lcd.print(0);
  }
  lcd.print(level);
}

void InGame::gameOver() {
  while (bombs.length)
    bombs.remove(0);
  while (explosions.length)
    explosions.remove(0);
  gameStarted = true;
  levelStarted = true;
  nextRoom = true;
  Serial.println(score);
  setGameState(GameState::GameOver);
}



void InGame::playerController(int xChange, int yChange, bool swChange) {
  if (gameStarted) {
    //TO DO: set players health and bombs and spread
    //TO DO: clear bombs and explosions after exiting the room
    playerName = "";
    for (int i = nameAddress; i < nameAddress + 6; i++) {
      char c = EEPROM.read(i);
      Serial.println(c);
      if (c != 0) {
        playerName += c;
        Serial.println(playerName);
      }
    }
    if (!playerName.length())
      playerName = "A";
    score = 0;
    level = 1;
    gameStarted = false;
  }
  
  if (levelStarted) {
    startTime = millis();
    maxTime = 25 * level;
    render(0, 0);
    updateLevel();
    updateScore();
    levelStarted = false;
    currentRoom = 0;
  }

  if (nextRoom) {
    generateRoom();
    nextRoom = false;
    currentRoom++;
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
  }

  else if (xChange == 1) {
    if (p.getPos().getPosX() < 7) {
      if (matrix[xLastPos + 1][yLastPos] != breakableWallId && matrix[xLastPos + 1][yLastPos] != solidWallId)
        p.modifyPos(1, 0);
    }
  }

  else if (yChange == -1) {
    if (p.getPos().getPosY() > 0) {
      if (matrix[xLastPos][yLastPos - 1] != breakableWallId && matrix[xLastPos][yLastPos - 1] != solidWallId)
        p.modifyPos(0, -1);
    }
  }

  else if (yChange == 1) {
    if (p.getPos().getPosY() < 7) {
      if (matrix[xLastPos][yLastPos + 1] != breakableWallId && matrix[xLastPos][yLastPos + 1] != solidWallId)
        p.modifyPos(0, 1);
    }
  }

  if (xLastPos != p.getPos().getPosX() || yLastPos != p.getPos().getPosY()) {
    if (matrix[p.getPos().getPosX()][p.getPos().getPosY()] == gateId) {
      if (currentRoom == level) {
        score += (maxTime - (millis() - startTime) / 1000);
        level++;
        levelStarted = true; 
      }
      nextRoom = true;
    }
    matrix[p.getPos().getPosX()][p.getPos().getPosY()] = playerId;
    matrix[xLastPos][yLastPos] = 0;
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
            if (matrix[x - 1][y] == breakableWallId) {
              score += 5;
              updateScore();
              matrix[x - 1][y] = 0;
            }
            else if (matrix[x - 1][y] != solidWallId && matrix[x - 1][y] != gateId) {
              Explosion explosion(x - 1, y, spread - 1, 1);
              explosions.append(explosion);
            }

            if (matrix[x + 1][y] == breakableWallId) {
              score += 5;
              updateScore();
              matrix[x + 1][y] = 0;
            }
            else if (matrix[x + 1][y] != solidWallId && matrix[x + 1][y] != gateId) {
              Explosion explosion(x + 1, y, spread - 1, 2);
              explosions.append(explosion);
            }

            if (matrix[x][y - 1] == breakableWallId) {
              score += 5;
              updateScore();
              matrix[x][y - 1] = 0;
            }
            else if (matrix[x][y - 1] != solidWallId && matrix[x][y - 1] != gateId) {
              Explosion explosion(x, y - 1, spread - 1, 3);
              explosions.append(explosion);
            }

            if (matrix[x][y + 1] == breakableWallId) {
              score += 5;
              updateScore();
              matrix[x][y + 1] = 0;
            }
            else if (matrix[x][y + 1] != solidWallId && matrix[x][y + 1] != gateId) {
              Explosion explosion(x, y + 1, spread - 1, 4);
              explosions.append(explosion);
            }

            break;
          }
        case 1: {
            if (matrix[x - 1][y] == breakableWallId) {
              score += 5;
              updateScore();
              matrix[x - 1][y] = 0;
            }
            else if (matrix[x - 1][y] != solidWallId && matrix[x - 1][y] != gateId) {
              Explosion explosion(x - 1, y, spread - 1, dir);
              explosions.append(explosion);
            }
            break;
          }
        case 2: {
            if (matrix[x + 1][y] == breakableWallId) {
              score += 5;
              updateScore();
              matrix[x + 1][y] = 0;
            }
            else if (matrix[x + 1][y] != solidWallId && matrix[x + 1][y] != gateId) {
              Explosion explosion(x + 1, y, spread - 1, dir);
              explosions.append(explosion);
            }
            break;
          }
        case 3: {
            if (matrix[x][y - 1] == breakableWallId) {
              score += 5;
              updateScore();
              matrix[x][y - 1] = 0;
            }
            else if (matrix[x][y - 1] != solidWallId && matrix[x][y - 1] != gateId) {
              Explosion explosion(x, y - 1, spread - 1, dir);
              explosions.append(explosion);
            }
            break;
          }
        case 4: {
            if (matrix[x][y + 1] == breakableWallId) {
              score += 5;
              updateScore();
              matrix[x][y + 1] = 0;
            }
            else if (matrix[x][y + 1] != solidWallId && matrix[x][y + 1] != gateId) {
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
      if (matrix[row][col] != gateId && matrix[row][col] != bombId)
        lc.setLed(0, row, col, matrix[row][col]);
      else
        lc.setLed(0, row, col, 0);

      if (matrix[row][col] == bombId) {
        if (bombBlink > 3)
          lc.setLed(0, row, col, matrix[row][col]);
        else
          lc.setLed(0, row, col, 0); 
      }
    }
  }

  bombBlink++;
  if (bombBlink == 10)
    bombBlink = 0;
}
