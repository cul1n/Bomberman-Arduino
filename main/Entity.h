#ifndef ENTITY_H
#define ENTITY_H
#include "Position.h"

class Entity {
  protected:
    Position pos;

  public:
    Entity();
    
    Entity(byte, byte);
    
    Position getPos();

    void modifyPos(byte, byte);
};

class Player : public Entity {
  byte health;
  byte bombs;
  byte runningSpeed;

  public:
    Player(byte, byte);    
};

#endif
