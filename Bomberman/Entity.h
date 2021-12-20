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

    void setPos(byte, byte);
};

class Player : public Entity {
  byte health;
  byte bombs;
  byte explosionSpread;
  unsigned long long int invincibilityTime;
  int invincibilityDuration;

  public:
    Player(byte, byte); 

    byte getNumberOfBombs();

    byte getPlayerHealth();

    byte getExplosionSpread();

    void loseHealth();

    void setStats(byte, byte, byte);
};

class Enemy : public Entity {
  byte directionOfMovement;

  public:
    Enemy();
    
    Enemy(byte, byte, byte);

    byte getDirection();

    void setDirection(byte);
  
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
  unsigned long spawnTime;
  // TO DO: maybe smaller type
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
