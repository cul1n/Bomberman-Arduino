#include<LiquidCrystal.h>
#include "State.h"
#include<EEPROM.h>
#include "LedControl.h"

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

const int RS = 8;
const int enable = 9;
const int contrastPin = 1;
const int brightnessPin = 6;
const int d4 = 13; // era 5
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int buzzerPin = 5;

const int xPin = A0;
const int yPin = A1;
const int swPin = 7;

const byte maxNameLength = 6;
const byte nameAddress = 200;
const byte levelAddress = nameAddress + maxNameLength;
const byte contrastAddress = levelAddress + sizeof(int);
const byte lcdBrightnessAddress = contrastAddress + sizeof(int);
const byte matrixBrightnessAddress = lcdBrightnessAddress + sizeof(int);
const byte highScoreAddress = matrixBrightnessAddress + sizeof(int);
const byte statsAddress = highScoreAddress + 3 * (maxNameLength + sizeof(int));


LiquidCrystal lcd(RS,enable,d4,d5,d6,d7);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER



const uint64_t animation[] PROGMEM = {
  0x387c7c7c38120f02,
  0x387c7c7c38140e04,
  0x387c7c7c38100c0c,
  0x387c7c7c38103810,
  0x387c7c7c38100000,
  0x7e7e7e7e7e7e0000,
  0xffffffffffffff00,
  0xffffffffffffffff
};

const int lengthOfAnimation = sizeof(animation)/8;

const uint64_t playIcon = 0x3c4a99b9b9994a3c;
const uint64_t settingsIcon = 0x000456a7a7560400;
const uint64_t highScoreIcon = 0x3c18187ebdbdff3c;
const uint64_t creditsIcon = 0x1818001870667e3c;


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

byte bonusSpreadGlyph[] = {
  B01110,
  B00100,
  B00100,
  B01110,
  B10001,
  B11001,
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
  analogWrite(contrastPin, 20);
  analogWrite(brightnessPin, 60);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 5); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  lcd.createChar(0, hourGlassGlyph);
  lcd.createChar(1, heartGlyph);
  lcd.createChar(2, bombGlyph);
  lcd.createChar(3, bonusSpreadGlyph);
  
  Serial.begin(9600);

  randomSeed(analogRead(13));

  getGameState().render(0, -1);

  for (int index = 0; index < lengthOfAnimation; index++) {
    uint64_t image;
    memcpy_P(&image, &animation[index], 8);
    
    for (int i = 0; i < 8; i++) {
      byte row = (image >> i * 8) & 0xFF;
      for (int j = 0; j < 8; j++) {
        lc.setLed(0, i, j, bitRead(row, j));
      }
    }
    delay(375);
  }
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
    letter = -1;
  }

  if (yValue < minThreshold) {
    letter = +1;
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
      tone(buzzerPin, 2000, 100);
      index--;
    }
    change = true;
  }

  if (yValue < minThreshold) {
    if (index == options - 1) {
      //index = 0;
    } 
    else {
      tone(buzzerPin, 2000, 100);
      index++;
    }
    change = true;
  }

  if (swState == LOW && swState != lastSwState) {
    tone(buzzerPin, 1000, 150);
    nextMenu = true;
  }

  lastSwState = swState;
}
