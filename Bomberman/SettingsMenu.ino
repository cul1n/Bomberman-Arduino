#include "SettingsMenu.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;
extern LedControl lc;

void SettingsMenu::update(int index) {
  switch (index) {
    case 0:
      editingName = true;
      break;
    case 1:
      editingLevel = true;
      break;
    case 2:
      editingContrast = true;
      break;
    case 3:
      editingLCDBrightness = true;
      break;
    case 4:
      editingMatrixBrightness = true;
      break;
    case 5:
      setGameState(GameState::MainMenu);
      break;
    default:
      break;
  }
}

bool SettingsMenu::isEditingName() {
  return editingName;
}

void SettingsMenu::editName(int pos, int letter, bool finished) {
  if (!started) {
    playerName = "";
    for (int i = 0; i < 6; i++) {
      char c = EEPROM.read(i);
      if (c != 0) {
        playerName += c;
      }
    }
    if (!playerName.length())
      playerName = "A";
      
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Enter Name:");
    lcd.setCursor(1,1);
    lcd.print(playerName);
    started = true;
    currentPos = playerName.length() - 1;
    lcd.setCursor(currentPos + 1, 1);
    currentChar = playerName[currentPos];
  }

  if (pos == -1) {
    if (currentPos) {
      playerName.remove(currentPos, 1);
      currentPos--;
      currentChar = playerName[currentPos];
    }
  }

  if (pos == 1) {
    if (currentPos < 6) {
      playerName[currentPos] = currentChar;
      lcd.print(currentChar);
      currentPos++;
      if (currentPos > playerName.length()-1) {
        currentChar = 'A';
        playerName += currentChar;
      }
      else currentChar = playerName[currentPos];
    }
  }

  if (letter == -1) {
    if (currentChar == 'A')
      currentChar = 'Z';
    else
      currentChar--;
  }

  if (letter == 1) {
    if (currentChar == 'Z')
      currentChar = 'A';
    else
      currentChar++;
  }
  

  if(millis() - blinkingTime > blinkingInterval){
    if (playerName[currentPos] == ' ') playerName[currentPos] = currentChar;
    else playerName[currentPos] = ' ';
    lcd.print(playerName[currentPos]);
    lcd.setCursor(currentPos + 1, 1);
    blinkingTime = millis();
  }
  
  if (finished) {
    playerName[currentPos] = currentChar;
    for(int i = 0; i < 6; i++){
      if(playerName.length() > i){
        EEPROM.put(i, playerName[i]);
      }
      else EEPROM.put(i, 0);
    }
    started = false;
    editingName = false; 
    playerName = "";
  }
}

bool SettingsMenu::isEditingLevel() {
  return editingLevel;
}

void SettingsMenu::editLevel(int exponent, int increment, bool finished) {
  if (!started) {
    started = true;
    level = EEPROM.read(11);
    if (!level) 
      level = 1;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Enter Level:");
    lcd.setCursor(1, 1);
    lcd.print(level);
  }

  if (increment == +1) {
    if (level == 10) 
      level = 1;
    else 
      level++;
    lcd.setCursor(1, 1);
    lcd.print(level);
    lcd.print(" ");
  }

  if (increment == -1) {
    if (level == 1) 
      level = 10;
    else 
      level--;
    lcd.setCursor(1, 1);
    lcd.print(level);
    lcd.print(" ");
  }

  if (exponent == 1) {
    if (level + 5 <= 10)
      level += 5;
    else
      level -= 5;
    lcd.setCursor(1, 1);
    lcd.print(level); 
    lcd.print(" ");
  }

  if (exponent == -1) {
    if (level - 5 >= 1)
      level -= 5;
    else
      level += 5;
    lcd.setCursor(1, 1);
    lcd.print(level);
    lcd.print(" ");
  }
  
  if (finished) {
    started = false;
    editingLevel = false;
    EEPROM.put(11, level);
  } 
}

bool SettingsMenu::isEditingContrast() {
   return editingContrast;
}

void SettingsMenu::editContrast(int exponent, int increment, bool finished) {
  if (!started) {
    started = true;
    contrast = EEPROM.read(20);
    if (!contrast) 
      contrast = 128;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Enter Contrast:");
    lcd.setCursor(1, 1);
    lcd.print(contrast);
  }

  if (increment == +1) {
    if (contrast == 255) 
      contrast = 1;
    else 
      contrast++;
    lcd.setCursor(1, 1);
    lcd.print(contrast);
    lcd.print("   ");
    //analogWrite(6, contrast);
  }

  if (increment == -1) {
    if (contrast == 1) 
      contrast = 255;
    else 
      contrast--;
    lcd.setCursor(1, 1);
    lcd.print(contrast);
    lcd.print("   ");
    //analogWrite(6, contrast);
  }

  if (exponent == 1) {
    if (contrast + 25 <= 255)
      contrast += 25;
    else
      contrast -= 230;
    lcd.setCursor(1, 1);
    lcd.print(contrast); 
    lcd.print("   ");
    //analogWrite(6, contrast);
  }

  if (exponent == -1) {
    if (contrast - 25 >= 1)
      contrast -= 25;
    else
      contrast += 230;
    lcd.setCursor(1, 1);
    lcd.print(contrast);
    lcd.print("   ");
    //analogWrite(6, contrast);
  }
  
  if (finished) {
    started = false;
    editingContrast = false;
    EEPROM.put(20, contrast);
  } 
}

bool SettingsMenu::isEditingLCDBrightness() {
   return editingLCDBrightness;
}

void SettingsMenu::editLCDBrightness(int exponent, int increment, bool finished) {
  if (!started) {
    started = true;
    lcdBrightness = EEPROM.read(25);
    if (!lcdBrightness) 
      lcdBrightness = 128;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("LCD Brightness:");
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
  }

  if (increment == +1) {
    if (lcdBrightness == 255) 
      lcdBrightness = 1;
    else 
      lcdBrightness++;
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
    lcd.print("   ");
    analogWrite(6, lcdBrightness);
  }

  if (increment == -1) {
    if (lcdBrightness == 1) 
      lcdBrightness = 255;
    else 
      lcdBrightness--;
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
    lcd.print("   ");
    analogWrite(6, lcdBrightness);
  }

  if (exponent == 1) {
    if (lcdBrightness + 25 <= 255)
      lcdBrightness += 25;
    else
      lcdBrightness -= 230;
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness); 
    lcd.print("   ");
    analogWrite(6, lcdBrightness);
  }

  if (exponent == -1) {
    if (lcdBrightness - 25 >= 1)
      lcdBrightness -= 25;
    else
      lcdBrightness += 230;
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
    lcd.print("   ");
    analogWrite(6, lcdBrightness);
  }
  
  if (finished) {
    started = false;
    editingLCDBrightness = false;
    EEPROM.put(25, lcdBrightness);
  } 
}

bool SettingsMenu::isEditingMatrixBrightness() {
   return editingMatrixBrightness;
}

void SettingsMenu::editMatrixBrightness(int exponent, int increment, bool finished) {
  if (!started) {
    started = true;
    matrixBrightness = EEPROM.read(30);
    if (!matrixBrightness) 
      matrixBrightness = 10;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Matrix Brightness:");
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
  }

  if (increment == +1) {
    if (matrixBrightness == 15) 
      matrixBrightness = 0;
    else 
      matrixBrightness++;
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
    lcd.print("   ");
    lc.setIntensity(0, matrixBrightness);
  }

  if (increment == -1) {
    if (matrixBrightness == 0) 
      matrixBrightness = 15;
    else 
      matrixBrightness--;
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
    lcd.print("   ");
    lc.setIntensity(0, matrixBrightness);
  }

  if (exponent == 1) {
    if (matrixBrightness + 5 <= 15)
      matrixBrightness += 5;
    else
      matrixBrightness -= 10;
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness); 
    lcd.print("   ");
    lc.setIntensity(0, matrixBrightness);
  }

  if (exponent == -1) {
    if (matrixBrightness - 5 >= 0)
      matrixBrightness -= 5;
    else
      matrixBrightness += 10;
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
    lcd.print("   ");
    lc.setIntensity(0, matrixBrightness);
  }
  
  if (finished) {
    started = false;
    editingMatrixBrightness = false;
    EEPROM.put(30, matrixBrightness);
  } 
}
