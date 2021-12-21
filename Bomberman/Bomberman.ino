#include<LiquidCrystal.h>
#include "State.h"
#include<EEPROM.h>
#include "LedControl.h"
#include "Controls.h"
#include "Consts.h"

// LCD and Matrix instances
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER

// Variables used for Menu 
long int lastMoved = millis();
int index = 0;
int lastIndex = -1;
int options = 4;
bool change = false;
bool lastSwState = HIGH;
bool nextMenu = false;
bool finished = false;
int pos, letter = 0;
byte screen = 0;

Controls controls;

void setup() {
  pinMode(swPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  
  lcd.begin(16,2); // set up the LCD's number of columns and rows:

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.clearDisplay(0);// clear screen

  lcd.createChar(timeChar, hourGlassGlyph);
  lcd.createChar(heartChar, heartGlyph);
  lcd.createChar(bombChar, bombGlyph);
  lcd.createChar(bonusSpreadChar, bonusSpreadGlyph);

  if (EEPROM.get(contrastAddress, intType)) {
    analogWrite(contrastPin, EEPROM.get(contrastAddress, intType)); 
  }
  else {
    analogWrite(contrastPin, defaultContrastValue);
  }

  if (EEPROM.get(lcdBrightnessAddress, intType)) {
    analogWrite(brightnessPin, EEPROM.get(lcdBrightnessAddress, intType)); 
  }
  else {
    analogWrite(brightnessPin, defaultLCDBrightnessValue);
  }

  if (EEPROM.get(matrixBrightnessAddress, intType)) {
    lc.setIntensity(0, EEPROM.get(matrixBrightnessAddress, intType)); 
  }
  else {
    lc.setIntensity(0, defaultMatrixBrightnessValue);
  }

  Serial.begin(9600);

  // Using random value generated by unused pin for the seed
  randomSeed(analogRead(contrastPin)); 

  getGameState().render(0, -1);

  // Intro animation
  for (int index = 0; index < lengthOfAnimation; index++) {
    uint64_t image;
    memcpy_P(&image, &animation[index], numberOfBytes);
    
    for (int i = 0; i < numberOfBytes; i++) {
      byte row = (image >> i * numberOfBytes) & 0xFF;
      for (int j = 0; j < numberOfBytes; j++) {
        lc.setLed(0, i, j, bitRead(row, j));
      }
    }
    delay(animationDelay);
  }
}

void loop() {
  if (getGameState().isPlaying()) {
    if (millis() - lastMoved > moveInterval) {
      controls.editOption();
      lastMoved = millis();
    }
    
    getGameState().playerController(pos, letter, finished);
  }
  
  else if (getGameState().isGameOver()) {
    if (millis() - lastMoved > moveInterval) {
      controls.editOption();
      lastMoved = millis();
    }
    
    if (finished) {
      screen++;
    }
    
    getGameState().render(0, screen);

    if (screen == numberOfScreens) {
      screen = 0;
    }

    options = getGameState().getNumberOfOptions();
  }
  
  else if (getGameState().introSequence() ){
    getGameState().render(0, 0);
  }
  
  else if (getGameState().isEditingName()) {
    if (millis() - lastMoved > moveInterval) {
      controls.editOption();
      lastMoved = millis();
    }
    
    getGameState().editName(letter, pos, finished);
    
  }
  
  else if (getGameState().isEditingLevel()) {
    if (millis() - lastMoved > moveInterval) {
      controls.editOption();
      lastMoved = millis();
    }
    
    getGameState().editLevel(pos, letter, finished);
  }
  
  else if (getGameState().isEditingContrast()) {
    if (millis() - lastMoved > moveInterval) {
      controls.editOption();
      lastMoved = millis();
    }
    
    getGameState().editContrast(pos, letter, finished);
  }
  
  else if (getGameState().isEditingLCDBrightness()) {
    if (millis() - lastMoved > moveInterval) {
      controls.editOption();
      lastMoved = millis();
    }
    
    getGameState().editLCDBrightness(pos, letter, finished);
  }
  
  else if (getGameState().isEditingMatrixBrightness()) {
    if (millis() - lastMoved > moveInterval) {
      controls.editOption();
      lastMoved = millis();
    }
    
    getGameState().editMatrixBrightness(pos, letter, finished);
  }
  
  else if (getGameState().isResettingScores()) {
    if (millis() - lastMoved > moveInterval) {
      controls.editOption();
      lastMoved = millis();
    }
    
    getGameState().resetScores(pos, letter, finished);
  }
  
  else {
    if (millis() - lastMoved > moveInterval) {
      controls.updateMenu();
      lastMoved = millis();
      getGameState().render(index, lastIndex);
    }
  
    if (change) {
      getGameState().render(index, lastIndex);
      change = false;
    }
    
    if (nextMenu) {
      nextMenu = false;
      change = true;
      getGameState().update(index);
      index = 0;
      lastIndex = -1;
      options = getGameState().getNumberOfOptions();
    }
  }

  pos = 0;
  letter = 0;
  finished = false;
}

void playSound(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
}
