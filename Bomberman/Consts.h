#pragma once

// Hardware: pins for LCD, matrix, joystick, buzzer
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

const int RS = 8;
const int enable = 9;
const int contrastPin = 1;
const int brightnessPin = 6;
const int d4 = 13;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int buzzerPin = 5;

const int xPin = A0;
const int yPin = A1;
const int swPin = 7;

// EEPROM: addresses for user preferences and stats
const byte maxNameLength = 6;
const byte nameAddress = 200;
const byte levelAddress = nameAddress + maxNameLength;
const byte contrastAddress = levelAddress + sizeof(int);
const byte lcdBrightnessAddress = contrastAddress + sizeof(int);
const byte matrixBrightnessAddress = lcdBrightnessAddress + sizeof(int);
const byte highScoreAddress = matrixBrightnessAddress + sizeof(int);
const byte statsLevelAddress = highScoreAddress + 3 * (maxNameLength + sizeof(int));
const byte statsHighScoreAddress = statsLevelAddress + sizeof(byte);
const byte statsScoreAddress = statsHighScoreAddress + sizeof(bool);
const byte statsTimeAddress = statsScoreAddress + sizeof(int);
const byte statsBombsAddress = statsTimeAddress + sizeof(int);
const byte statsDamageTakenAddress = statsBombsAddress + sizeof(int);
const byte statsSpreadAddress = statsDamageTakenAddress + sizeof(byte);
const byte soundsDisabledAddress = statsSpreadAddress + sizeof(bool);

// Used types for EEPROM.get()
const int intType = 0;
const bool boolType = false;
const byte byteType = 0;

// Images: intro animation, icons for the menu, glyphs for in-game display
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
const byte numberOfBytes = 8;
const int animationDelay = 375;

const uint64_t playIcon PROGMEM = 0x3c4a99b9b9994a3c;
const uint64_t settingsIcon PROGMEM = 0x000456a7a7560400;
const uint64_t highScoreIcon PROGMEM = 0x3c18187ebdbdff3c;
const uint64_t creditsIcon PROGMEM = 0x1818001870667e3c;
const uint64_t brightnessIcon PROGMEM = 0x18183c5a5a5a423c;

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

const byte timeChar = 0;
const byte heartChar = 1;
const byte bombChar = 2;
const byte bonusSpreadChar = 3;

// Joystick thresholds, wait time before next input, input types
const int minThreshold = 200;
const int maxThreshold = 800;
const int moveInterval = 200;
const int positive = 1;
const int negative = -1;

// Sound frequencies and durations for buzzer;
const int menuChangeFrequency = 2000;
const int menuChangeDuration = 100;
const int menuChoiceFrequency = 1000;
const int menuChoiceDuration = 150;
const int damageTakenFrequency = 200;
const int damageTakenDuration = 300;
const int noMoneyFrequency = 500;
const int noMoneyDuration = 150;
const int bombPlacedFrequency = 1000;
const int bombPlacedDuration = 100;
const int nextLevelFrequency = 2500;
const int nextLevelDuration = 100;
const int bombExplosionFrequency = 100;
const int bombExplosionDuration = 400;

// Constats used in-game
const int bombTimer = 3000;
const byte explosionTimer = 50;
const byte defaultExplosionSpread = 1;
const byte deafultNumberOfBombs = 1;
const byte defaultHealth = 3;
const byte maxExplosionSpread = 2;
const byte maxNumberOfBombs = 3;
const byte maxLevel = 10;
const byte numberOfItems = 3;
const byte matrixSize = 8;
const byte enemySpawnRange = 2;
const int millisInSecond = 1000;
const int scoreThreshold = 999;

const byte shopChance = 6;
const byte healthShopIndex = 0;
const byte bombShopIndex = 1;
const byte spreadShopIndex = 2;
const byte basePrice = 25;

const byte lastLevelIncrease = 5;
const byte baseTime = 25;
const byte enemyMoveTimer = 18;
const byte numberOfDirections = 4;
const byte playerBlinkerDelay = 5;
const byte playerBlinkerMaxTime = 25;
const byte bombBlinkerDelay = 3;
const byte bombBlinkerMaxTime = 10;

const int enemyScore = 20;
const int wallScore = 5;

const byte longListLength = 18;
const byte shortListLength = 3;

// ID's used for in-game map
const byte bombId = 1;
const byte explosionId = 2;
const byte breakableWallId = 3;
const byte solidWallId = 4;
const byte playerId = 5;
const byte gateId = 6;
const byte enemyId = 7;

// Consts used for menu and settings
const byte numberOfHighScores = 3;
const String noPlayer = "NON 0";
const byte numberOfScreens = 2;
const int defaultContrastValue = 60;
const int defaultLCDBrightnessValue = 100;
const int defaultMatrixBrightnessValue = 10;
