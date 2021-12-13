#include<LiquidCrystal.h>
#include "State.h"
#include<EEPROM.h>
#include "LedControl.h"

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;


const int RS = 8;
const int enable = 9;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

const int xPin = A0;
const int yPin = A1;
const int swPin = 7;

const int nameAddress = 200;

LiquidCrystal lcd(RS,enable,d4,d5,d6,d7);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER


byte hourGlassGlyph[] = {
  B11111,
  B10001,
  B01010,
  B00100,
  B01010,
  B10001,
  B11111,
  B11111
};

byte heartGlyph[] = {
  B00000,
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000 
};

byte bombGlyph[] = {
  B00000,
  B01000,
  B10100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110
};


long int lastMoved = millis();
int moveInterval = 200;
int index = 0;
int lastIndex = -1;
int options = 4;
bool change = false;
const int minThreshold = 200;
const int maxThreshold = 800;
bool lastSwState = HIGH;
bool nextMenu = false;
bool currentlyInMain = true;
bool finished = false;
int pos, letter = 0;

void setup() {
  //analogWrite(6, 30);
  analogWrite(6, 128);
  pinMode(swPin, INPUT_PULLUP);
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 5); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  lcd.createChar(0, hourGlassGlyph);
  lcd.createChar(1, heartGlyph);
  lcd.createChar(2, bombGlyph);
  
  Serial.begin(9600);

  randomSeed(analogRead(13));
  
  getGameState().render(0, -1);
}

void loop() {
  if (getGameState().isPlaying()) {
    if (millis() - lastMoved > moveInterval) {
      editOption();
      lastMoved = millis();
    }
    getGameState().playerController(pos, letter, finished);
    pos = 0;
    letter = 0;
    finished = false;
  }
  else if (getGameState().isGameOver()) {
    if (millis() - lastMoved > moveInterval) {
      editOption();
      lastMoved = millis();
    }
    
    if (finished) 
      getGameState().render(0, 1);
    else 
      getGameState().render(0, 0);

    options = getGameState().getNumberOfOptions();
    pos = 0;
    letter = 0;
    finished = false; 
  }
  
  else if (getGameState().introSequence() ){
    getGameState().render(0, 0);
  }
  
  else if (getGameState().isEditingName()) {
    if (millis() - lastMoved > moveInterval) {
      editOption();
      lastMoved = millis();
    }
    getGameState().editName(pos, letter, finished);
    pos = 0;
    letter = 0;
    finished = false;
  }
  else if (getGameState().isEditingLevel()) {
    if (millis() - lastMoved > moveInterval) {
      editOption();
      lastMoved = millis();
    }
    getGameState().editLevel(pos, letter, finished);
    pos = 0;
    letter = 0;
    finished = false;
  }
  else if (getGameState().isEditingContrast()) {
    if (millis() - lastMoved > moveInterval) {
      editOption();
      lastMoved = millis();
    }
    getGameState().editContrast(pos, letter, finished);
    pos = 0;
    letter = 0;
    finished = false;
  }
  
  else if (getGameState().isEditingLCDBrightness()) {
    if (millis() - lastMoved > moveInterval) {
      editOption();
      lastMoved = millis();
    }
    getGameState().editLCDBrightness(pos, letter, finished);
    pos = 0;
    letter = 0;
    finished = false;
  } 
  else if (getGameState().isEditingMatrixBrightness()) {
    if (millis() - lastMoved > moveInterval) {
      editOption();
      lastMoved = millis();
    }
    getGameState().editMatrixBrightness(pos, letter, finished);
    pos = 0;
    letter = 0;
    finished = false;
  } 
  else {
    //Serial.println(index);
    if (millis() - lastMoved > moveInterval) {
      updateMenu();
      lastMoved = millis();
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
  
}

void editOption() {
  int yValue = analogRead(yPin);
  int xValue = analogRead(xPin);
  bool swState = digitalRead(swPin);
  pos = 0;
  letter = 0;
  finished = false;
  
  if (yValue > maxThreshold) {
    letter = +1;
  }

  if (yValue < minThreshold) {
    letter = -1;
  }

  if (xValue > maxThreshold) {
    pos = +1;
  }

  if (xValue < minThreshold) {
    pos = -1;
  }

  if (swState == LOW && swState != lastSwState) {
    finished = true;
  }

  lastSwState = swState;
}


void updateMenu() {
  int yValue = analogRead(yPin);
  bool swState = digitalRead(swPin);
  lastIndex = index;
  
  if (yValue > maxThreshold) {
    if (index == 0) {
      //index = options - 1;
    } 
    else {
      index--;
    }
    change = true;
  }

  if (yValue < minThreshold) {
    if (index == options - 1) {
      //index = 0;
    } 
    else {
      index++;
    }
    change = true;
  }

  if (swState == LOW && swState != lastSwState) {
    nextMenu = true;
  }

  lastSwState = swState;
}
