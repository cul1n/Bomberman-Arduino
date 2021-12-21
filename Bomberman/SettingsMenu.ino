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
      resettingScores = true;
      break;
    case 6:
      disablingSound = true;
      break;
    case 7:
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

  if (pos == -1) {
    if (currentPos) {
      playerName.remove(currentPos, 1);
      currentPos--;
      currentChar = playerName[currentPos];
    }
  }

  if (pos == 1) {
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

  if (letter == 1) {
    if (currentChar == 'A') {
      currentChar = 'Z';
    }
    else {
      currentChar--;
    }
  }

  if (letter == -1) {
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

  if (increment == +1) {
    if (level == 10)
      level = 1;
    else
      level++;
    lcd.setCursor(1, 1);
    lcd.print(level);
    lcd.print(F(" "));
  }

  if (increment == -1) {
    if (level == 1)
      level = 10;
    else
      level--;
    lcd.setCursor(1, 1);
    lcd.print(level);
    lcd.print(F(" "));
  }

  if (exponent == -1) {
    if (level + 5 <= 10)
      level += 5;
    else
      level -= 5;
    lcd.setCursor(1, 1);
    lcd.print(level);
    lcd.print(F(" "));
  }

  if (exponent == 1) {
    if (level - 5 >= 1)
      level -= 5;
    else
      level += 5;
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
    if (!contrast)
      contrast = 128;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Enter Contrast:"));
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
    lcd.print(F("   "));
    analogWrite(contrastPin, contrast);
  }

  if (increment == -1) {
    if (contrast == 1)
      contrast = 255;
    else
      contrast--;
    lcd.setCursor(1, 1);
    lcd.print(contrast);
    lcd.print(F("   "));
    analogWrite(contrastPin, contrast);
  }

  if (exponent == -1) {
    if (contrast + 25 <= 255)
      contrast += 25;
    else
      contrast -= 230;
    lcd.setCursor(1, 1);
    lcd.print(contrast);
    lcd.print(F("   "));
    analogWrite(contrastPin, contrast);
  }

  if (exponent == 1) {
    if (contrast - 25 >= 1)
      contrast -= 25;
    else
      contrast += 230;
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
      lcdBrightness = 128;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("LCD Brightness:"));
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
    lcd.print(F("   "));
    analogWrite(brightnessPin, lcdBrightness);
  }

  if (increment == -1) {
    if (lcdBrightness == 1)
      lcdBrightness = 255;
    else
      lcdBrightness--;
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
    lcd.print(F("   "));
    analogWrite(brightnessPin, lcdBrightness);
  }

  if (exponent == -1) {
    if (lcdBrightness + 25 <= 255)
      lcdBrightness += 25;
    else
      lcdBrightness -= 230;
    lcd.setCursor(1, 1);
    lcd.print(lcdBrightness);
    lcd.print(F("   "));
    analogWrite(brightnessPin, lcdBrightness);
  }

  if (exponent == 1) {
    if (lcdBrightness - 25 >= 1)
      lcdBrightness -= 25;
    else
      lcdBrightness += 230;
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
    if (!matrixBrightness)
      matrixBrightness = 10;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Matrix Brightness:"));
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
    lcd.print(F("   "));
    lc.setIntensity(0, matrixBrightness);
  }

  if (increment == -1) {
    if (matrixBrightness == 0)
      matrixBrightness = 15;
    else
      matrixBrightness--;
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
    lcd.print(F("   "));
    lc.setIntensity(0, matrixBrightness);
  }

  if (exponent == -1) {
    if (matrixBrightness + 5 <= 15)
      matrixBrightness += 5;
    else
      matrixBrightness -= 10;
    lcd.setCursor(1, 1);
    lcd.print(matrixBrightness);
    lcd.print(F("   "));
    lc.setIntensity(0, matrixBrightness);
  }

  if (exponent == 1) {
    if (matrixBrightness - 5 >= 0)
      matrixBrightness -= 5;
    else
      matrixBrightness += 10;
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

  if (increment == +1 && !choice) {
    choice = true;
    lcd.setCursor(0, 1);
    lcd.print(F(" "));
    lcd.setCursor(4, 1);
    lcd.print(F(">"));
  }

  if (increment == -1 && choice) {
    choice = false;
    lcd.setCursor(0, 1);
    lcd.print(F(">"));
    lcd.setCursor(4, 1);
    lcd.print(F(" "));
  }

  if (finished) {
    if (choice) {
      for (byte i = 0; i < 3; i++) {
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

  if (increment == 1 && !choice) {
    choice = true;
    lcd.setCursor(0, 1);
    lcd.print(F(" "));
    lcd.setCursor(4, 1);
    lcd.print(F(">"));
  }

  if (increment == -1 && choice) {
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
