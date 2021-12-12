#pragma once
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
  unsigned long long int invincibilityTime;
  int invincibilityDuration;

  public:
    Player(byte, byte); 

    byte getNumberOfBombs();

    byte getPlayerHealth();

    void loseHealth();
};

class Bomb : public Entity {
  unsigned long long int spawnTime;
  int duration;

  public:
    Bomb();
    
    Bomb(byte, byte);

    bool stillActive();
};

class Explosion : public Entity {
  byte spread;
  byte directionOfSpread;
  bool resolved;
  unsigned long long int spawnTime;
  int duration;

  public:
    Explosion();

    Explosion(byte, byte, byte, byte);

    byte getSpread();

    byte getDirection();

    bool isResolved();

    void setResolved();

    bool stillActive();
};

class Wall : public Entity {
  bool breakable;

  public:
    Wall();

    Wall(byte, byte, bool);

    bool isBreakable();
};
