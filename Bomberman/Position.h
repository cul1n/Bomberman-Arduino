#pragma once
#include "LedControl.h"

class Position {
  byte posX, posY;
  
  public:
    Position();
    
    Position(byte, byte);
    
    byte getPosX();

    byte getPosY();

    void addPosX(byte);

    void addPosY(byte);

    void setPosX(byte);

    void setPosY(byte);
};
