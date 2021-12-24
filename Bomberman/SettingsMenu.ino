#include "SettingsMenu.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;
extern LedControl lc;

void SettingsMenu::update(int index) {
  switch (index) {
    case editingNameIndex:
      editingName = true;
      break;
    case editingLevelIndex:
      editingLevel = true;
      break;
    case editingContrastIndex:
      editingContrast = true;
      break;
    case editingLCDBrightnessIndex:
      editingLCDBrightness = true;
      break;
    case editingMatrixBrightnessIndex:
      editingMatrixBrightness = true;
      break;
    case resettingScoresIndex:
      resettingScores = true;
      break;
    case disablingSoundIndex:
      disablingSound = true;
      break;
    case backSettingsIndex:
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
    for (int i = nameAddress; i < nameAddress + maxNameLength; i++) {
      char character = EEPROM.read(i);
      if (character != 0) {
        playerName += character;
      }
    }
    if (!playerName.length())
      playerName = "A";

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Enter Name:"));
    lcd.setCursor(1, 1);
    lcd.print(playerName);
    started = true;
    currentPos = playerName.length() - 1;
    lcd.setCursor(currentPos + 1, 1);
    currentChar = playerName[currentPos];
  }

  if (pos == negative) {
    if (currentPos) {
      playerName.remove(currentPos, 1);
      currentPos--;
      currentChar = playerName[currentPos];
    }
  }

  if (pos == positive) {
    if (currentPos < maxNameLength - 1) { 
      playerName[currentPos] = currentChar;
      lcd.print(currentChar);
      currentPos++;
      if (currentPos > playerName.length() - 1) {
        currentChar = 'A';
        playerName += currentChar;
      }
      else currentChar = playerName[currentPos];
    }
  }

  if (letter == positive) {
    if (currentChar == 'A') {
      currentChar = 'Z';
    }
    else {
      currentChar--;
    }
  }

  if (letter == negative) {
    if (currentChar == 'Z') {
      currentChar = 'A';
    }
    else {
      currentChar++;
    }
  }


  if (millis() - blinkingTime > blinkingInterval) {
    if (playerName[currentPos] == ' ') playerName[currentPos] = currentChar;
    else playerName[currentPos] = ' ';
    lcd.print(playerName[currentPos]);
    lcd.setCursor(currentPos + 1, 1);
    blinkingTime = millis();
  }

  if (finished) {
    playerName[currentPos] = currentChar;
    Serial.println(playerName);
    for (int i = nameAddress; i < nameAddress + maxNameLength; i++) {
      if (playerName.length() > i - nameAddress) {
        EEPROM.put(i, playerName[i - nameAddress]);
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
    level = EEPROM.read(levelAddress);
    if (!level)
      level = 1;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Enter Level:"));
    lcd.setCursor(1, 1);
    lcd.print(level);
  }

  if (increment == positive) {
    if (level == maxLevel)
      level = 1;
    else
      level++;
    lcd.setCursor(1, 1);
    lcd.print(level);
    lcd.print(F(" "));
  }

  if (increment == negative) {
    if (level == 1)
      level = maxLevel;
    else
      level--;
    lcd.setCursor(1, 1);
    lcd.print(level);
    lcd.print(F(" "));
  }

  if (exponent == negative) {
    if (level + levelExponent <= maxLevel)
      level += levelExponent;
    else
      level -= levelExponent;
    lcd.setCursor(1, 1);
    lcd.print(level);
    lcd.print(F(" "));
  }

  if (exponent == positive) {
    if (level - levelExponent >= 1)
      level -= levelExponent;
    else
      level += levelExponent;
    lcd.setCursor(1, 1);
    lcd.print(level);
    lcd.print(F(" "));
  }

  if (finished) {
    started = false;
    editingLevel = false;
    EEPROM.put(levelAddress, level);
  }
}

bool SettingsMenu::isEditingContrast() {
  return editingContrast;
}

void SettingsMenu::editContrast(int exponent, int increment, bool finished) {
  if (!started) {
    started = true;
    contrast = EEPROM.read(contrastAddress);
    
    if (!contrast) {
      contrast = defaultContrastValue;
    }
    
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Enter Contrast:"));
    lcd.setCursor(1, 1);
    lcd.print(contrast);
  }

  if (increment == positive) {
    if (contrast == maxSettingValue) {
      contrast = 1;
    }
    else {
      contrast++;
    }
    
    lcd.setCursor(1, 1);
    lcd.print(contrast);
    lcd.print(F("   "));
    analogWrite(contrastPin, contrast);
  }

  if (increment == negative) {
    if (contrast == 1) {
      contrast = maxSettingValue;
    }
    else {
      contrast--;
    }
    
    lcd.setCursor(1, 1);
    lcd.print(contrast);
    lcd.print(F("   "));
    analogWrite(contrastPin, contrast);
  }

  if (exponent == negative) {
    if (contrast + settingExponent <= maxSettingValue) {
      contrast += settingExponent;
    }
    else {
      contrast -= (maxSettingValue - settingExponent);
    }
    
    lcd.setCursor(1, 1);
    lcd.print(contrast);
    lcd.print(F("   "));
    analogWrite(contrastPin, contrast);
  }

  if (exponent == positive) {
    if (contrast - settingExponent >= 1) {
      contrast -= settingExponent;
    }
    else {
      contrast += (maxSettingValue - settingExponent);
    }
    
    lcd.setCursor(1, 1);
    lcd.print(contrast);
    lcd.print(F("   "));
    analogWrite(contrastPin, contrast);
  }

  if (finished) {
    started = false;
    editingContrast = false;
    EEPROM.put(contrastAddress, contrast);
  }
}

bool SettingsMenu::isEditingLCDBrightness() {
  return editingLCDBrightness;
}

void SettingsMenu::editLCDBrightness(int exponent, int increment, bool finished) {
  if (!started) {
    started = true;
    lcdBrightness = EEPROM.read(lcdBrightnessAddress);
    if (!lcdBrightness)
      lcdBrightness = defaultLCDBrightnessValue;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("LCD Brightness:"));
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
  }

  if (increment == positive) {
    if (lcdBrightness == maxSettingValue) {
      lcdBrightness = 1;
    }
    else {
      lcdBrightness++;
    }
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
    lcd.print(F("   "));
    analogWrite(brightnessPin, lcdBrightness);
  }

  if (increment == negative) {
    if (lcdBrightness == 1) {
      lcdBrightness = maxSettingValue;
    }
    else {
      lcdBrightness--;
    }
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
    lcd.print(F("   "));
    analogWrite(brightnessPin, lcdBrightness);
  }

  if (exponent == negative) {
    if (lcdBrightness + settingExponent <= maxSettingValue) {
      lcdBrightness += settingExponent;
    }
    else {
      lcdBrightness -= (maxSettingValue - settingExponent);
    }
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
    lcd.print(F("   "));
    analogWrite(brightnessPin, lcdBrightness);
  }

  if (exponent == positive) {
    if (lcdBrightness - settingExponent >= 1)
      lcdBrightness -= settingExponent;
    else
      lcdBrightness += (maxSettingValue - settingExponent);
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
    lcd.print(F("   "));
    analogWrite(brightnessPin, lcdBrightness);
  }

  if (finished) {
    started = false;
    editingLCDBrightness = false;
    EEPROM.put(lcdBrightnessAddress, lcdBrightness);
  }
}

bool SettingsMenu::isEditingMatrixBrightness() {
  return editingMatrixBrightness;
}

void SettingsMenu::editMatrixBrightness(int exponent, int increment, bool finished) {
  if (!started) {
    displayIcon(brightnessIcon);
    started = true;
    matrixBrightness = EEPROM.read(matrixBrightnessAddress);
    
    if (!matrixBrightness) {
      matrixBrightness = defaultMatrixBrightnessValue;
    }

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Matrix Brightness:"));
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
  }

  if (increment == positive) {
    if (matrixBrightness == maxMatrixBrightnessValue) {
      matrixBrightness = 0;
    }
    else {
      matrixBrightness++;
    }
    
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
    lcd.print(F("   "));
    lc.setIntensity(0, matrixBrightness);
  }

  if (increment == negative) {
    if (matrixBrightness == 0) {
      matrixBrightness = maxMatrixBrightnessValue;
    }
    else {
      matrixBrightness--;
    }
    
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
    lcd.print(F("   "));
    lc.setIntensity(0, matrixBrightness);
  }

  if (exponent == negative) {
    if (matrixBrightness + matrixBrightnessExponent <= maxMatrixBrightnessValue) {
      matrixBrightness += matrixBrightnessExponent;
    }
    else {
      matrixBrightness -= (maxMatrixBrightnessValue - matrixBrightnessExponent);
    }
    
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
    lcd.print(F("   "));
    lc.setIntensity(0, matrixBrightness);
  }

  if (exponent == positive) {
    if (matrixBrightness - matrixBrightnessExponent >= 0) {
      matrixBrightness -= matrixBrightnessExponent;
    }
    else {
      matrixBrightness += (maxMatrixBrightnessValue - matrixBrightnessExponent);
    }
    
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
    lcd.print(F("   "));
    lc.setIntensity(0, matrixBrightness);
  }

  if (finished) {
    displayIcon(settingsIcon);
    started = false;
    editingMatrixBrightness = false;
    EEPROM.put(matrixBrightnessAddress, matrixBrightness);
  }
}

bool SettingsMenu::isResettingScores() {
  return resettingScores;
}

void SettingsMenu::resetScores(int exponent, int increment, bool finished) {
  if (!started) {
    started = true;
    choice = false;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Are you sure?"));
    lcd.setCursor(0, 1);
    lcd.print(F(">NO  YES"));
  }

  if (increment == positive && !choice) {
    choice = true;
    lcd.setCursor(0, 1);
    lcd.print(F(" "));
    lcd.setCursor(4, 1);
    lcd.print(F(">"));
  }

  if (increment == negative && choice) {
    choice = false;
    lcd.setCursor(0, 1);
    lcd.print(F(">"));
    lcd.setCursor(4, 1);
    lcd.print(F(" "));
  }

  if (finished) {
    if (choice) {
      for (byte i = 0; i < numberOfHighScores; i++) {
        for (byte j = 0; j < maxNameLength + sizeof(int); j++) {
          EEPROM.write(highScoreAddress + j + i * (maxNameLength + sizeof(int)) , 0);
        }
      }
    }
    started = false;
    resettingScores = false;
  }
}

bool SettingsMenu::isDisablingSound() {
  return disablingSound;
}

void SettingsMenu::disableSound(int exponent, int increment, bool finished) {
  if (!started) {
    started = true;
    choice = false;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Disable Sound?"));
    lcd.setCursor(0, 1);
    lcd.print(F(">NO  YES"));
  }

  if (increment == positive && !choice) {
    choice = true;
    lcd.setCursor(0, 1);
    lcd.print(F(" "));
    lcd.setCursor(4, 1);
    lcd.print(F(">"));
  }

  if (increment == negative && choice) {
    choice = false;
    lcd.setCursor(0, 1);
    lcd.print(F(">"));
    lcd.setCursor(4, 1);
    lcd.print(F(" "));
  }

  if (finished) {
    EEPROM.put(soundsDisabledAddress, choice);
    started = false;
    disablingSound = false;
  }
}

void SettingsMenu::displayIcon(uint64_t icon) {
  for (int i = 0; i < numberOfBytes; i++) {
    byte row = (icon >> i * numberOfBytes) & 0xFF;
    
    for (int j = 0; j < numberOfBytes; j++) {
      lc.setLed(0, i, j, bitRead(row, j));
    }
    
  }
}
