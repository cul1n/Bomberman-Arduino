#include "InGame.h"
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

InGame::InGame() : p(3, 0) {
  level = 1;
  levelStarted = true;
  gameStarted = true;
  nextRoom = true;
  shop = false;
  shopDisplayed = false;
  score = 0;
  
  bombBlinker = 0;
  enemyBlinker = 0;
  playerBlinker = 0;
  
  for (int i = 0; i < numberOfItems; i++) {
    items[i] = true;
  }
  
  timePlayed = 0;
  bombsPlaced = 0;
  upgrade = false;
}

void InGame::render(int index, int lastIndex) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(playerName);
  lcd.setCursor(12, 0);
  lcd.write(byte(timeChar));
  lcd.setCursor(0, 1);
  lcd.write(byte(heartChar));
  lcd.print(F("x"));
  lcd.print(p.getPlayerHealth());
  lcd.setCursor(4,1);
  lcd.write(byte(bombChar));
  lcd.print(F("x"));
  lcd.print(p.getNumberOfBombs());
  lcd.setCursor(8,1);
  lcd.print(F("SCR: "));
}

bool InGame::isPlaying() {
  return true;
}

void InGame::generateRoom() {
  for (byte i = 0; i < matrixSize; i++) {
    for (byte j = 0 ; j < matrixSize; j++) {
      matrix[i][j] = 0;
      if (i == 0 || j == 0 || i == matrixSize - 1 || j == matrixSize - 1) {
        matrix[i][j] = solidWallId;
      }
    }
  }

  // Starting gate position
  matrix[3][0] = 0;
  matrix[4][0] = 0;

  // Player starting position
  p.setPos(3, 0);
  matrix[3][0] = playerId;

  // Choosing one of the 3 random positions for the gate
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

  // Formula to calculate the number of walls within one room
  byte numberOfWalls = ((level + 1) / 2) * 5;

  // After level 7, as there is not much space left, the number of walls plateaus
  if ( level > 7) {
    numberOfWalls = 13 + level;
  }

  while (numberOfWalls) {
    byte i = random(1, matrixSize - 1);
    byte j = random(1, matrixSize - 1);
    // Checking the position in front of the gate
    if (i != 3 || j != 1) {
      if (matrix[i][j] != breakableWallId) {
        matrix[i][j] = breakableWallId;
        numberOfWalls--;
      }
    }
  }

  // Formula to calculate the number of enemies
  byte numberOfEnemies = (level + 1) / 3;

  while (numberOfEnemies) {
    byte i = random(enemySpawnRange, matrixSize - enemySpawnRange);
    byte j = random(enemySpawnRange, matrixSize - enemySpawnRange);
    if (matrix[i][j] != enemyId) {
      // obtain 1 out of 4 random directions for the enemy
      byte dir = random(0, numberOfDirections);
      matrix[i][j] = enemyId;
      Enemy enemy(i, j, dir);
      enemies.append(enemy);
      numberOfEnemies--;
    }
  }
  
}

int lastTimer = 0;

void InGame::updateTimer() {
  int timer = maxTime - (millis() - startTime) / millisInSecond;
  if (timer == -1) {
    gameOver();
  }
  if (lastTimer != timer) {
    lcd.setCursor(13, 0);
    lcd.print(F("   "));
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
  lcd.setCursor(12, 1);
  lcd.print(F("    "));
  if (score > scoreThreshold) {
    lcd.setCursor(12, 1);
  }
  else {
    lcd.setCursor(13, 1);
  }
  lcd.print(score);
}

void InGame::updateLevel() {
  lcd.setCursor(7, 0);
  lcd.print(F("LV"));
  if ( level != maxLevel) {
    lcd.print(F("0"));
  }
  lcd.print(level);
}

void InGame::updateHighScore(int currentScore) {
  if (currentScore > EEPROM.get(highScoreAddress + maxNameLength, intType)) {
    for (byte i = highScoreAddress; i < highScoreAddress + maxNameLength; i++) {
      EEPROM.put(i + 2 * (maxNameLength + sizeof(int)), EEPROM.read(i + maxNameLength + sizeof(int)));
      EEPROM.put(i + maxNameLength + sizeof(int), EEPROM.read(i));
      EEPROM.put(i, playerName[i - highScoreAddress]);
    }
    EEPROM.put(highScoreAddress + 3 * maxNameLength + 2 * sizeof(int) , EEPROM.get(highScoreAddress + 2 * maxNameLength + sizeof(int), intType));
    EEPROM.put(highScoreAddress + 2 * maxNameLength + sizeof(int) , EEPROM.get(highScoreAddress + maxNameLength, intType));
    EEPROM.put(highScoreAddress + maxNameLength, currentScore);
    EEPROM.put(statsHighScoreAddress, true);
  }
  
  else if (currentScore > EEPROM.get(highScoreAddress + 2 * maxNameLength + sizeof(int), intType)) {
    for (byte i = highScoreAddress; i < highScoreAddress + maxNameLength; i++) {
      EEPROM.put(i + 2 * (maxNameLength + sizeof(int)), EEPROM.read(i + maxNameLength + sizeof(int)));
      EEPROM.put(i + maxNameLength + sizeof(int), playerName[i - highScoreAddress]);
    }
    EEPROM.put(highScoreAddress + 3 * maxNameLength + 2 * sizeof(int) , EEPROM.get(highScoreAddress + 2 * maxNameLength + sizeof(int), intType));
    EEPROM.put(highScoreAddress + 2 * maxNameLength + sizeof(int) , currentScore);
    EEPROM.put(statsHighScoreAddress, true);
  }

  else if (currentScore > EEPROM.get(highScoreAddress + 3 * maxNameLength + 2 * sizeof(int), intType)) {
    for (byte i = highScoreAddress; i < highScoreAddress + maxNameLength; i++) {
      EEPROM.put(i + 2 * (maxNameLength + sizeof(int)), playerName[i - highScoreAddress]);
    }
    EEPROM.put(highScoreAddress + 3 * maxNameLength + 2 * sizeof(int) , currentScore);
    EEPROM.put(statsHighScoreAddress, true);
  }
  else {
    EEPROM.put(statsHighScoreAddress, false);
  }
  
}

void InGame::clearRoom() {
  while (enemies.length) {
    enemies.remove(0);
  }
  
  while (bombs.length) {
    bombs.remove(0);
  }
  
  while (explosions.length) {
    explosions.remove(0);
  }
}

void InGame::gameOver() {
  clearRoom();
  gameStarted = true;
  levelStarted = true;
  nextRoom = true;
  updateHighScore(score);
  timePlayed += (millis() - startTime) / millisInSecond;
  EEPROM.put(statsLevelAddress, level);
  EEPROM.put(statsScoreAddress, score);
  EEPROM.put(statsTimeAddress, timePlayed);
  EEPROM.put(statsBombsAddress, bombsPlaced);
  EEPROM.put(statsSpreadAddress, upgrade);
  setGameState(GameState::GameOver);
}

void InGame::playerController(int xChange, int yChange, bool swChange) {
  if (gameStarted) {
    strcpy(playerName, "      ");
    for (int i = nameAddress; i < nameAddress + maxNameLength; i++) {
      char character = EEPROM.read(i);
      if (character != '\0') {
        playerName[i - nameAddress] = character;
      }
    }
    playerName[maxNameLength] = '\0';
    
    if (!strlen(playerName))
      strcpy(playerName, "A");
    score = 0;
    
    level = EEPROM.get(levelAddress, intType);
    timePlayed = 0;
    bombsPlaced = 0;
    EEPROM.put(statsDamageTakenAddress, 0);
    upgrade = false;
    if (level == 0)
      level = 1;
    gameStarted = false;

    p.setStats(defaultHealth, deafultNumberOfBombs, defaultExplosionSpread);
  }

  if (shop) {
    if (!shopDisplayed) {
      bool removed = false;

      if (p.getExplosionSpread() == maxExplosionSpread)  {
        items[spreadShopIndex] = false;
        removed = true;
      }

      if (p.getNumberOfBombs() == maxNumberOfBombs) {
        items[bombShopIndex] = false;
        removed = true;
      }

      if (!removed) {
        byte removedItem  = random(0, shopChance);
        
        if (removedItem == healthShopIndex) {
          items[healthShopIndex] = false;
        }
        else if (removedItem == bombShopIndex) {
          items[bombShopIndex] = false;
        }
        else {
          items[spreadShopIndex] = false;
        }
      }
      
      lc.clearDisplay(0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("SHOP: SCORE: "));
      lcd.print(score);
      lcd.setCursor(0, 1);
      lcd.print(F(">"));

      for (int i = 0; i < numberOfItems; i++) {
        if (items[i] == true) {
          // Formula to calculate price of item
          byte itemValue = (level / 2 + i) * basePrice;
          lcd.write(byte(i + 1));
          lcd.print(F(":"));
          lcd.print(itemValue);
          if (itemValue < itemValueDisplay) {
            lcd.print(F("  "));
          }
          else {
            lcd.print(F(" "));
          }
        }
      }

      lcd.setCursor(13, 1);
      lcd.print(F("NO"));
      shopDisplayed = true;
      shopIndex = 0;
    }
    if (yChange == negative) {
      if (shopIndex != 0) {
        controls.playSound(menuChangeFrequency, menuChangeDuration);
        lcd.setCursor(shopIndex * 6, 1);
        lcd.print(F(" "));
        shopIndex--;
        lcd.setCursor(shopIndex * 6, 1);
        lcd.print(F(">"));
      }
    }
  
    else if (yChange == positive) {
      if (shopIndex != 2) {
        controls.playSound(menuChangeFrequency, menuChangeDuration);
        lcd.setCursor(shopIndex * 6, 1);
        lcd.print(F(" "));
        shopIndex++;
        lcd.setCursor(shopIndex * 6, 1);
        lcd.print(F(">"));
      }
    }

    else if (swChange) {
      
      switch (shopIndex) {
        case 0: { // player chooses the first option
          byte i = 0;
          
          while (items[i] == false && i < 3) {
            i++;
          }

          byte itemValue = (level / 2 + i) * basePrice;

          if (score > itemValue) {
            score -= itemValue;
            
            if (i == healthShopIndex) {
              p.setStats(p.getPlayerHealth() + 1, p.getNumberOfBombs(), p.getExplosionSpread());
            }
            
            else if (i == bombShopIndex) {
              p.setStats(p.getPlayerHealth(), p.getNumberOfBombs() + 1, p.getExplosionSpread());
            }
            
            else if (i == spreadShopIndex) {
              p.setStats(p.getPlayerHealth(), p.getNumberOfBombs(), p.getExplosionSpread() + 1);
              upgrade = true;
            }

            for (int i = 0; i < numberOfItems; i++) {
              items[i] = true;
            }
            controls.playSound(menuChoiceFrequency, menuChoiceDuration);  
            shop = false;             
          }
          else {
            controls.playSound(noMoneyFrequency, noMoneyDuration);
          }
          break;
        }
        
        case 1: { // player chooses the second option
          byte i = numberOfItems - 1;
          
          while (items[i] == false && i > 0) {
            i--;
          }

          byte itemValue = (level / 2 + i) * basePrice;

          if (score > itemValue && i != 0) {
            score -= itemValue;

            if (i == 1) {
              p.setStats(p.getPlayerHealth(), p.getNumberOfBombs() + 1, p.getExplosionSpread());
            }
            else if (i == 2) {
              p.setStats(p.getPlayerHealth(), p.getNumberOfBombs(), p.getExplosionSpread() + 1);
              upgrade = true;
            }
            
            for (int i = 0; i < numberOfItems; i++) {
              items[i] = true;
            }
            controls.playSound(menuChoiceFrequency, menuChoiceDuration);
            shop = false;
          }
          else {
            controls.playSound(noMoneyFrequency, noMoneyDuration);
          }
          
          break;
        }
        
        case 2: { // player chooses the leave option
          shop = false;
          
          for (int i = 0; i < numberOfItems; i++) {
            items[i] = true;
          }
          controls.playSound(menuChoiceFrequency, menuChoiceDuration);
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
      if (level < lastLevelIncrease) {
        maxTime = baseTime * level;
      }
      else {
        maxTime = baseTime * lastLevelIncrease;
      }
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

    if (matrix[xLastPos][yLastPos] == 0) {
      matrix[xLastPos][yLastPos] = playerId;
    }
    
    if (swChange && bombs.length < p.getNumberOfBombs() && matrix[p.getPos().getPosX()][p.getPos().getPosY()] != bombId) {
      controls.playSound(bombPlacedFrequency, bombPlacedDuration);
      Bomb bomb(p.getPos().getPosX(), p.getPos().getPosY());
      bombSpawned = true;
      matrix[bomb.getPos().getPosX()][bomb.getPos().getPosY()] = bombId;
      bombs.append(bomb);
      updateBombs();
      bombsPlaced++;
    }
  
    if (xChange == negative) {
      if (p.getPos().getPosX() > 0) {
        if (matrix[xLastPos - 1][yLastPos] != breakableWallId && matrix[xLastPos - 1][yLastPos] != solidWallId)
          p.modifyPos(negative, 0);
      }
    }
  
    else if (xChange == positive) {
      if (p.getPos().getPosX() < matrixSize - 1) {
        if (matrix[xLastPos + 1][yLastPos] != breakableWallId && matrix[xLastPos + 1][yLastPos] != solidWallId)
          p.modifyPos(positive, 0);
      }
    }
  
    else if (yChange == negative) {
      if (p.getPos().getPosY() > 0) {
        if (matrix[xLastPos][yLastPos - 1] != breakableWallId && matrix[xLastPos][yLastPos - 1] != solidWallId)
          p.modifyPos(0, negative);
      }
    }
  
    else if (yChange == positive) {
      if (p.getPos().getPosY() < matrixSize - 1) {
        if (matrix[xLastPos][yLastPos + 1] != breakableWallId && matrix[xLastPos][yLastPos + 1] != solidWallId)
          p.modifyPos(0, positive);
      }
    }
  
    if (xLastPos != p.getPos().getPosX() || yLastPos != p.getPos().getPosY()) {
      if (matrix[p.getPos().getPosX()][p.getPos().getPosY()] == gateId) {
        if (currentRoom == level || currentRoom  >= lastLevelIncrease) {
          controls.playSound(nextLevelFrequency, nextLevelDuration);
          timePlayed += (millis() - startTime) / millisInSecond;
          // Adding only one third of the time left to the score to incentivise breaking walls and defeating enemies
          score += (maxTime - (millis() - startTime) / millisInSecond) / 3;
          level++;
          
          if (level == maxLevel + 1) {
            gameOver();
          }
          
          levelStarted = true; 
          
          // Every two levels the player gets a shop
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
      controls.playSound(bombExplosionFrequency, bombExplosionDuration);
      updateBombs();
      i--;
    }
    else {
      matrix[bombs.getItem(i).getPos().getPosX()][bombs.getItem(i).getPos().getPosY()] = bombId;
    }
  }

  if (enemyBlinker == enemyMoveTimer) {
    for (int i = 0; i  < enemies.length; i++) {
      byte x = enemies.getItem(i).getPos().getPosX();
      byte y = enemies.getItem(i).getPos().getPosY();
      byte dir = enemies.getItem(i).getDirection();
      byte newDir = dir;
      matrix[x][y] = 0;
      switch (dir) {
        case enemyRight:
          if (x + 1 < matrixSize - 1 && (matrix[x + 1][y] == 0 || matrix[x + 1][y] == playerId || matrix[x + 1][y] == explosionId)) {
            enemies.getItem(i).modifyPos(positive, 0);
          }
          else {
            newDir = random(0, numberOfDirections);
          }
          break;
        case enemyLeft:
          if (x - 1 > 0 && (matrix[x - 1][y] == 0 || matrix[x - 1][y] == playerId || matrix[x - 1][y] == explosionId)) {
            enemies.getItem(i).modifyPos(negative, 0);
          }
          else {
            newDir = random(0, numberOfDirections);
          }
          break;
        case enemyUp:
          if (y + 1 < matrixSize - 1 && (matrix[x][y + 1] == 0 || matrix[x][y + 1] == playerId || matrix[x][y + 1] == explosionId)) {
            enemies.getItem(i).modifyPos(0, positive);
          }
          else {
            newDir = random(0, numberOfDirections);
          }
          break;
        case enemyDown:
          if (y - 1 > 0 && (matrix[x][y - 1] == 0 || matrix[x][y - 1] == playerId || matrix[x][y - 1] == explosionId)) {
            enemies.getItem(i).modifyPos(0, negative);
          }
          else {
            newDir = random(0, numberOfDirections);
          }
          break;
        default:
          break;
      }
      
      enemies.getItem(i).setDirection(newDir);
      
      if (matrix[enemies.getItem(i).getPos().getPosX()][enemies.getItem(i).getPos().getPosY()] != explosionId) {
        matrix[enemies.getItem(i).getPos().getPosX()][enemies.getItem(i).getPos().getPosY()] = enemyId;
      }
    }
  }
  
  for (int i = 0; i  < enemies.length; i++) {
    if (matrix[enemies.getItem(i).getPos().getPosX()][enemies.getItem(i).getPos().getPosY()] == explosionId) {
        enemies.remove(i);
        score += enemyScore;
        updateScore();
        i--;
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
        case explosionAllDirections: {
            if (matrix[x - 1][y] == breakableWallId) {
              score += wallScore;
              updateScore();
              matrix[x - 1][y] = 0;
            }
            else if (matrix[x - 1][y] != solidWallId && matrix[x - 1][y] != gateId) {
              Explosion explosion(x - 1, y, spread - 1, explosionLeft);
              explosions.append(explosion);
            }

            if (matrix[x + 1][y] == breakableWallId) {
              score += wallScore;
              updateScore();
              matrix[x + 1][y] = 0;
            }
            else if (matrix[x + 1][y] != solidWallId && matrix[x + 1][y] != gateId) {
              Explosion explosion(x + 1, y, spread - 1, explosionRight);
              explosions.append(explosion);
            }

            if (matrix[x][y - 1] == breakableWallId) {
              score += wallScore;
              updateScore();
              matrix[x][y - 1] = 0;
            }
            else if (matrix[x][y - 1] != solidWallId && matrix[x][y - 1] != gateId) {
              Explosion explosion(x, y - 1, spread - 1, explosionDown);
              explosions.append(explosion);
            }

            if (matrix[x][y + 1] == breakableWallId) {
              score += wallScore;
              updateScore();
              matrix[x][y + 1] = 0;
            }
            else if (matrix[x][y + 1] != solidWallId && matrix[x][y + 1] != gateId) {
              Explosion explosion(x, y + 1, spread - 1, explosionUp);
              explosions.append(explosion);
            }

            break;
          }
          
        case explosionLeft: {
            if (matrix[x - 1][y] == breakableWallId) {
              score += wallScore;
              updateScore();
              matrix[x - 1][y] = 0;
            }
            else if (matrix[x - 1][y] != solidWallId && matrix[x - 1][y] != gateId) {
              Explosion explosion(x - 1, y, spread - 1, dir);
              explosions.append(explosion);
            }
            break;
          }
          
        case explosionRight: {
            if (matrix[x + 1][y] == breakableWallId) {
              score += wallScore;
              updateScore();
              matrix[x + 1][y] = 0;
            }
            else if (matrix[x + 1][y] != solidWallId && matrix[x + 1][y] != gateId) {
              Explosion explosion(x + 1, y, spread - 1, dir);
              explosions.append(explosion);
            }
            break;
          }
          
        case explosionDown: {
            if (matrix[x][y - 1] == breakableWallId) {
              score += wallScore;
              updateScore();
              matrix[x][y - 1] = 0;
            }
            else if (matrix[x][y - 1] != solidWallId && matrix[x][y - 1] != gateId) {
              Explosion explosion(x, y - 1, spread - 1, dir);
              explosions.append(explosion);
            }
            break;
          }
          
        case explosionUp: {
            if (matrix[x][y + 1] == breakableWallId) {
              score += wallScore;
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
    matrix[explosions.getItem(i).getPos().getPosX()][explosions.getItem(i).getPos().getPosY()] = explosionId;
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

  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (matrix[row][col] != gateId && matrix[row][col] != bombId && matrix[row][col] != enemyId)
        lc.setLed(0, row, col, matrix[row][col]);
      else
        lc.setLed(0, row, col, 0);

      if (matrix[row][col] == bombId) {
        if (bombBlinker > bombBlinkerDelay) {
          lc.setLed(0, row, col, matrix[row][col]);
        }
        else {
          lc.setLed(0, row, col, 0);
        }
      }

      if (matrix[row][col] == enemyId) {
        if (enemyBlinker % 3 == 0) {
          lc.setLed(0, row, col, matrix[row][col]);
        }
        else {
          lc.setLed(0, row, col, 0);
        }
      }

      if (matrix[row][col] == playerId) {
        if (playerBlinker > playerBlinkerDelay) {
          lc.setLed(0, row, col, matrix[row][col]);
        }
        else {
          lc.setLed(0, row, col, 0);
        }
      }
    }
  }

  bombBlinker++;
  if (bombBlinker == bombBlinkerMaxTime) {
    bombBlinker = 0;
  }
  
  enemyBlinker++;
  if (enemyBlinker == enemyMoveTimer + 1) {
    enemyBlinker = 0;
  }
  
  playerBlinker++;
  if (playerBlinker == playerBlinkerMaxTime) {
    playerBlinker = 0;
  }
}
