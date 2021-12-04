#include "LedControl.h" //  need the library

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

const int xPin = A0;
const int yPin = A1;
const int swPin = 6;


byte xPos = 0;
byte yPos = 0;

byte xLastPos = 0;
byte yLastPos = 0;

const int minThreshold = 200;
const int maxThreshold = 600;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER

byte matrixBrightness = 2;

unsigned int long long lastMoved = 0;
const int moveInterval = 100;

bool matrixChanged = true;
const byte matrixSize = 8;

bool matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};

class Position {
  byte posX, posY;
  
  public:
    Position() {
      posX = 0;
      posY = 0;
    }
    
    Position(byte x, byte y) {
      posX = x;
      posY = y;
    }
    
    byte getPosX() {
      return posX;
    }

    byte getPosY() {
      return posY;
    }

    void setPosX(byte x) {
      posX += x;
    }

    void setPosY(byte y) {
      posY += y;
    }
};


class Entity {
  protected:
    Position pos;

  public:
    Entity(byte x = 0, byte y = 0) : pos(x, y) {
         
    }
    
    Position getPos() {
      return pos;
    }

    void modifyPos(byte x, byte y) {
      pos.setPosX(x);
      pos.setPosY(y);
    }
};

class Player : public Entity {
  byte health;
  byte bombs;
  byte runningSpeed;

  public:
    Player(byte x, byte y) : Entity(x, y) {
    
    }
    
};

class List {
public:
    byte length;
    Entity data[16];

    List() {
      length = 0;
    }

    Entity getItem(byte index) {
      if (index >= length) return;
      return data[index];
    }
    
    void append(Entity item) {
        if (length < 16) data[length++] = item;
    }
    
    void remove(byte index) {
        if (index >= length) return;
        memmove(&data[index], &data[index+1], (length - index - 1) * sizeof(*data));
        length--;
    }
};

List bombs;

Player p(0,0);
bool lastSwState = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(swPin, INPUT_PULLUP);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  matrix[xPos][yPos] = 1;
}

void loop() {
//  // Debug screen
//  Serial.print(p.getPos().getPosX());
//  Serial.print(" ");
//  Serial.print(p.getPos().getPosY());
//  Serial.println();
//  Serial.println(bombs.length);
  // game logic 
  if (millis() - lastMoved > moveInterval) {
    updatePositions();
    lastMoved = millis();
  }
  if (matrixChanged == true) {
    // matrix display logic
    updateDisplay();
    matrixChanged = false;
  }
}

void updateDisplay() {
  for (int i = 0; i < bombs.length; i++) {
    matrix[bombs.getItem(i).getPos().getPosX()][bombs.getItem(i).getPos().getPosY()] = 1;
  }
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

bool bombSpawned = false;

void updatePositions() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  bool swState = digitalRead(swPin);
  
  xLastPos = p.getPos().getPosX();
  yLastPos = p.getPos().getPosY();

  if (swState == LOW && swState != lastSwState) {
    Entity bomb(p.getPos().getPosX(), p.getPos().getPosY());
    Serial.println("Bomb Spawned");
    bombSpawned = true;
    if (bombs.length > 2) {
      matrix[bombs.getItem(0).getPos().getPosX()][bombs.getItem(0).getPos().getPosY()] = 0;
      bombs.remove(0);
      bombs.append(bomb);
      matrix[bomb.getPos().getPosX()][bomb.getPos().getPosY()] = 1;
    }
    else {
      matrix[bomb.getPos().getPosX()][bomb.getPos().getPosY()] = 1;
      bombs.append(bomb);
    }
  }
  
  if (xValue < minThreshold) {
    if (p.getPos().getPosX() < matrixSize - 1) {
      p.modifyPos(1, 0);
    } 
    else {
      p.modifyPos(-7, 0);
    }
  }

  if (xValue > maxThreshold) {
    if (p.getPos().getPosX() > 0) {
      p.modifyPos(-1, 0);
    }
    else {
      p.modifyPos(7, 0);
    }
  }

  if (yValue > maxThreshold) {
    if (p.getPos().getPosY() < matrixSize - 1) {
      p.modifyPos(0, 1);
    } 
    else {
      p.modifyPos(0, -7);
    }
  }

  if (yValue < minThreshold) {
    if (p.getPos().getPosY() > 0) {
      p.modifyPos(0, -1);
    }
    else {
      p.modifyPos(0, 7);
    }
  }

  lastSwState = swState;
  
  if (xLastPos != p.getPos().getPosX() || yLastPos != p.getPos().getPosY()) {
    matrixChanged = true;
    matrix[p.getPos().getPosX()][p.getPos().getPosY()] = 1;
    if (bombSpawned) {
      bombSpawned = false;
    }
    else {
      matrix[xLastPos][yLastPos] = 0;
    }
  }
}
