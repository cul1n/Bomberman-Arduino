#include "Entity.h"

Entity::Entity() : pos(0, 0) {}

Entity::Entity(byte x, byte y) : pos(x, y) {}

Position Entity::getPos() {
  return pos;
}

void Entity::modifyPos(byte x, byte y) {
  pos.setPosX(x);
  pos.setPosY(y);
}

Player::Player(byte x, byte y) : Entity(x, y) {bombs = 3;}

byte Player::getNumberOfBombs() {
  return bombs;
}

Bomb::Bomb() : Entity(0, 0) {
  spawnTime = millis();
  duration = 3000;  
}

Bomb::Bomb(byte x, byte y) : Entity(x, y) {
  spawnTime = millis();
  duration = 3000;  
}

bool Bomb::stillActive() {
  if (millis() - spawnTime >= duration) {
    return false;
  }
  return true;
}

Explosion::Explosion() : Entity(0, 0) {
  spread = 1;
  directionOfSpread = 0;
  resolved = false;
  spawnTime = millis();
  duration = 1000; 
}

Explosion::Explosion(byte x, byte y, byte spread, byte directionOfSpread) : Entity(x, y) {
  this->spread = spread;
  this->directionOfSpread = directionOfSpread;
  if (spread == 0)
    this->resolved = true;
  else
    this->resolved = false;
  this->spawnTime = millis();
  this->duration = 1000; 
}

byte Explosion::getSpread() {
  return spread;
}

byte Explosion::getDirection() {
  return directionOfSpread;
}

bool Explosion::isResolved() {
  return resolved;
}

void Explosion::setResolved() {
  resolved = true;
}

bool Explosion::stillActive() {
  if (millis() - spawnTime >= duration) {
    return false;
  }
  return true;
}
