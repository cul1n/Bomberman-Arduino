#ifndef POSITION_H
#define POSITION_H
#include "LedControl.h"

class Position {
  byte posX, posY;
  
  public:
    Position();
    
    Position(byte, byte);
    
    byte getPosX();

    byte getPosY();

    void setPosX(byte);

    void setPosY(byte);
};

#endif
