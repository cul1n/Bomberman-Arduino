#include "InGame.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

InGame::InGame() : p(3, 0) {
  matrix[1][1] = playerId;
  level = 1;
  levelStarted = true;
  gameStarted = true;
  nextRoom = true;
  shop = false;
  shopDisplayed = false;
  score = 0;
  bombBlinker = 0;
  enemyBlinker = 0;
  for (int i = 0; i < 3; i++) {
    items[i] = true;
  }
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
  matrix[3][0] = playerId;
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

  byte numberOfEnemies = (level + 1) / 3;

  while (numberOfEnemies) {
    byte i = random(2,6);
    byte j = random(2,6);
    if (matrix[i][j] != enemyId) {
      byte dir = random(0,4);
      matrix[i][j] = enemyId;
      Enemy enemy(i, j, dir);
      enemies.append(enemy);
      numberOfEnemies--;
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

void InGame::updateHighScore(int currentScore) {
  int intType = 0;
  if (currentScore > EEPROM.get(highScoreAddress + maxNameLength, intType)) {
    for (byte i = highScoreAddress; i < highScoreAddress + maxNameLength; i++) {
      EEPROM.put(i + 2 * (maxNameLength + sizeof(int)), EEPROM.read(i + maxNameLength + sizeof(int)));
      EEPROM.put(i + maxNameLength + sizeof(int), EEPROM.read(i));
      EEPROM.put(i, playerName[i - highScoreAddress]);
    }
    EEPROM.put(highScoreAddress + 3 * maxNameLength + 2 * sizeof(int) , EEPROM.get(highScoreAddress + 2 * maxNameLength + sizeof(int), intType));
    EEPROM.put(highScoreAddress + 2 * maxNameLength + sizeof(int) , EEPROM.get(highScoreAddress + maxNameLength, intType));
    EEPROM.put(highScoreAddress + maxNameLength, currentScore);
  }
  
  else if (currentScore > EEPROM.get(highScoreAddress + 2 * maxNameLength + sizeof(int), intType)) {
    for (byte i = highScoreAddress; i < highScoreAddress + maxNameLength; i++) {
      EEPROM.put(i + 2 * (maxNameLength + sizeof(int)), EEPROM.read(i + maxNameLength + sizeof(int)));
      EEPROM.put(i + maxNameLength + sizeof(int), playerName[i - highScoreAddress]);
    }
    EEPROM.put(highScoreAddress + 3 * maxNameLength + 2 * sizeof(int) , EEPROM.get(highScoreAddress + 2 * maxNameLength + sizeof(int), intType));
    EEPROM.put(highScoreAddress + 2 * maxNameLength + sizeof(int) , currentScore);
  }

  else if (currentScore > EEPROM.get(highScoreAddress + 3 * maxNameLength + 2 * sizeof(int), intType)) {
    for (byte i = highScoreAddress; i < highScoreAddress + maxNameLength; i++) {
      EEPROM.put(i + 2 * (maxNameLength + sizeof(int)), playerName[i - highScoreAddress]);
    }
    EEPROM.put(highScoreAddress + 3 * maxNameLength + 2 * sizeof(int) , currentScore);
  }
  
}

void InGame::clearRoom() {
  while (enemies.length)
    enemies.remove(0);
  while (bombs.length)
    bombs.remove(0);
  while (explosions.length)
    explosions.remove(0);
}

void InGame::gameOver() {
  clearRoom();
  gameStarted = true;
  levelStarted = true;
  nextRoom = true;
  updateHighScore(score);
  EEPROM.put(statsAddress, level);
  setGameState(GameState::GameOver);
}

void InGame::playerController(int xChange, int yChange, bool swChange) {
  if (gameStarted) {
    strcpy(playerName, "      ");
    for (int i = nameAddress; i < nameAddress + 6; i++) {
      char c = EEPROM.read(i);
      Serial.println(c);
      if (c != '\0') {
        playerName[i - nameAddress] = c;
        Serial.println(playerName);
      }
    }
    playerName[6] = '\0';
    if (!strlen(playerName))
      strcpy(playerName, "A");
    score = 0;

    int intType = 0;
    level = EEPROM.get(levelAddress, intType);
    if (level == 0)
      level = 1;
    gameStarted = false;
    p.setStats(3, 1, 1);
  }

  if (shop) {
    if (!shopDisplayed) {
      bool removed = false;

      if (p.getExplosionSpread() == 2)  {
        items[2] = false;
        removed = true;
      }

      if (p.getNumberOfBombs() == 3) {
        items[1] = false;
        removed = true;
      }

      if (!removed) {
        byte removedItem  = random(0,6);
        
        if (removedItem == 0) {
          items[0] = false;
        }
        else if (removedItem == 1) {
          items[1] = false;
        }
        else {
          items[2] = false;
        }
      }
      
      lc.clearDisplay(0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SHOP: SCORE: ");
      lcd.print(score);
      lcd.setCursor(0, 1);
      lcd.print(">");

      for (int i = 0; i < 3; i++) {
        if (items[i] == true) {
          byte itemValue = (level / 2 + i) * 25;
          lcd.write(byte(i + 1));
          lcd.print(":");
          lcd.print(itemValue);
          if (itemValue < 100) {
            lcd.print("  ");
          }
          else {
            lcd.print(" ");
          }
        }
      }

      lcd.setCursor(13, 1);
      lcd.print("NO");
      shopDisplayed = true;
      shopIndex = 0;
    }
    if (xChange == -1) {
      if (shopIndex != 0) {
        tone(buzzerPin, 2000, 100);
        lcd.setCursor(shopIndex * 6, 1);
        lcd.print(" ");
        shopIndex--;
        lcd.setCursor(shopIndex * 6, 1);
        lcd.print(">");
      }
    }
  
    else if (xChange == 1) {
      if (shopIndex != 2) {
        tone(buzzerPin, 2000, 100);
        lcd.setCursor(shopIndex * 6, 1);
        lcd.print(" ");
        shopIndex++;
        lcd.setCursor(shopIndex * 6, 1);
        lcd.print(">");
      }
    }

    else if (swChange) {
      
      switch (shopIndex) {
        case 0: {
          byte i = 0;
          
          while (items[i] == false && i < 3) {
            i++;
          }

          byte itemValue = (level / 2 + i) * 25;

          if (score > itemValue) {
            score -= itemValue;
            
            if (i == 0) {
              p.setStats(p.getPlayerHealth() + 1, p.getNumberOfBombs(), p.getExplosionSpread());
            }
            
            else if (i == 1) {
              p.setStats(p.getPlayerHealth(), p.getNumberOfBombs() + 1, p.getExplosionSpread());
            }
            
            else if (i == 2) {
              p.setStats(p.getPlayerHealth(), p.getNumberOfBombs(), p.getExplosionSpread() + 1);
            }

            for (int i = 0; i < 3; i++) {
              items[i] = true;
            }
            tone(buzzerPin, 1000, 150);            
            shop = false;             
          }
          else {
            tone(buzzerPin, 500, 150);
          }
          break;
        }
        
        case 1: {
          byte i = 2;
          
          while (items[i] == false && i > 0) {
            i--;
          }

          byte itemValue = (level / 2 + i) * 25;

          if (score > itemValue && i != 0) {
            score -= itemValue;

            if (i == 1) {
              p.setStats(p.getPlayerHealth(), p.getNumberOfBombs() + 1, p.getExplosionSpread());
            }
            else if (i == 2) {
              p.setStats(p.getPlayerHealth(), p.getNumberOfBombs(), p.getExplosionSpread() + 1);
            }
            
            for (int i = 0; i < 3; i++) {
              items[i] = true;
            }
            tone(buzzerPin, 1000, 150);
            shop = false;
          }
          else {
            tone(buzzerPin, 500, 150);
          }
          
          break;
        }
        
        case 2: {
          shop = false;
          
          for (int i = 0; i < 3; i++) {
            items[i] = true;
          }
          tone(buzzerPin, 1000, 150);
          break;
        }
        
        default: {
          break;
        }
        
      }
    }
  
  }
  
  else {
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
      clearRoom();
      generateRoom();
      nextRoom = false;
      currentRoom++;
    }
    
    updateTimer();
    
    byte xLastPos = p.getPos().getPosX();
    byte yLastPos = p.getPos().getPosY();
    bool bombSpawned = false;
    
    if (matrix[xLastPos][yLastPos] == explosionId || matrix[xLastPos][yLastPos] == enemyId) {
      p.loseHealth();
      updateHealth();
    }
    
    if (swChange && bombs.length < p.getNumberOfBombs()) {
      tone(buzzerPin, 1000, 100);
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
        if (currentRoom == 1) { //ATENTIE = LEVEL
          tone(buzzerPin, 2500, 100);
          score += (maxTime - (millis() - startTime) / 1000);
          level++;
          levelStarted = true; 
          if (level % 2 == 1) {
            shop = true;
            shopDisplayed = false;
          }
        }
        nextRoom = true;
      }
      matrix[p.getPos().getPosX()][p.getPos().getPosY()] = playerId;
      matrix[xLastPos][yLastPos] = 0;
    }
    
    matrixUpdate();
  }
};

void InGame::matrixUpdate() {
  for (int i = 0; i < bombs.length; i++) {
    if (!bombs.getItem(i).stillActive()) {
      Explosion explosion(bombs.getItem(i).getPos().getPosX(), bombs.getItem(i).getPos().getPosY(), p.getExplosionSpread(), 0);
      explosions.append(explosion);

      matrix[bombs.getItem(i).getPos().getPosX()][bombs.getItem(i).getPos().getPosY()] = 0;
      bombs.remove(i);
      tone(buzzerPin, 100, 400);
      updateBombs();
      i--;
    }
    else {
      matrix[bombs.getItem(i).getPos().getPosX()][bombs.getItem(i).getPos().getPosY()] = 1;
    }
  }

  if (enemyBlinker == 18) {
    for (int i = 0; i  < enemies.length; i++) {
      byte x = enemies.getItem(i).getPos().getPosX();
      byte y = enemies.getItem(i).getPos().getPosY();
      byte dir = enemies.getItem(i).getDirection();
      byte newDir = dir;
      matrix[x][y] = 0;
      switch (dir) {
        case 0:
          if (x + 1 < 7 && (matrix[x + 1][y] == 0 || matrix[x + 1][y] == playerId || matrix[x + 1][y] == explosionId)) {
            enemies.getItem(i).modifyPos(1, 0);
          }
          else {
            newDir = random(0, 4);
          }
          break;
        case 1:
          if (x - 1 > 0 && (matrix[x - 1][y] == 0 || matrix[x - 1][y] == playerId || matrix[x - 1][y] == explosionId)) {
            enemies.getItem(i).modifyPos(-1, 0);
          }
          else {
            newDir = random(0, 4);
          }
          break;
        case 2:
          if (y + 1 < 7 && (matrix[x][y + 1] == 0 || matrix[x][y + 1] == playerId || matrix[x][y + 1] == explosionId)) {
            enemies.getItem(i).modifyPos(0, 1);
          }
          else {
            newDir = random(0, 4);
          }
          break;
        case 3:
          if (y - 1 > 0 && (matrix[x][y - 1] == 0 || matrix[x][y - 1] == playerId || matrix[x][y - 1] == explosionId)) {
            enemies.getItem(i).modifyPos(0, -1);
          }
          else {
            newDir = random(0, 4);
          }
          break;
        default:
          break;
      }
      
      enemies.getItem(i).setDirection(newDir);
      
      if (matrix[enemies.getItem(i).getPos().getPosX()][enemies.getItem(i).getPos().getPosY()] == explosionId) {
        enemies.remove(i);
        score += 20;
        updateScore();
      }
      
      else {
        matrix[enemies.getItem(i).getPos().getPosX()][enemies.getItem(i).getPos().getPosY()] = enemyId;
      }
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
      if (matrix[row][col] != gateId && matrix[row][col] != bombId && matrix[row][col] != enemyId)
        lc.setLed(0, row, col, matrix[row][col]);
      else
        lc.setLed(0, row, col, 0);

      if (matrix[row][col] == bombId) {
        if (bombBlinker > 3) {
          lc.setLed(0, row, col, matrix[row][col]);
        }
        else
          lc.setLed(0, row, col, 0); 
      }

      if (matrix[row][col] == enemyId) {
        if (enemyBlinker % 3 == 0)
          lc.setLed(0, row, col, matrix[row][col]);
        else
          lc.setLed(0, row, col, 0);
      }
    }
  }

  bombBlinker++;
  if (bombBlinker == 10)
    bombBlinker = 0;
  enemyBlinker++;
  if (enemyBlinker == 19)
    enemyBlinker = 0;
}
