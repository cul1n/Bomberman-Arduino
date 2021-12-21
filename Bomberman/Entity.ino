#include "Entity.h"

Entity::Entity() : pos(0, 0) {}

Entity::Entity(byte x, byte y) : pos(x, y) {}

Position Entity::getPos() {
  return pos;
}

void Entity::modifyPos(byte x, byte y) {
  pos.addPosX(x);
  pos.addPosY(y);
}

void Entity::setPos(byte x, byte y) {
  pos.setPosX(x);
  pos.setPosY(y);
}

Player::Player(byte x, byte y) : Entity(x, y) {
  bombs = 1; 
  health = 1;
  explosionSpread = 1;
  invincibilityTime = millis();
  invincibilityDuration = 1000;
}

byte Player::getNumberOfBombs() {
  return bombs;
}

byte Player::getPlayerHealth() {
  return health;
}

byte Player::getExplosionSpread() {
  return explosionSpread;
}

void Player::loseHealth() {
  if (millis() - invincibilityTime > invincibilityDuration) {
    byte damageTaken = EEPROM.read(statsDamageTakenAddress);
    damageTaken++;
    EEPROM.put(statsDamageTakenAddress, damageTaken);
    playSound(damageTakenFrequency, damageTakenDuration);
    health--;
    invincibilityTime = millis();
  }
}

void Player::setStats(byte health, byte bombs, byte explosionSpread) {
  this->health = health;
  this->bombs = bombs;
  this->explosionSpread = explosionSpread;
}

Enemy::Enemy() : Entity(0, 0) {
  directionOfMovement = 0;
}

Enemy::Enemy(byte x, byte y, byte dir) : Entity(x, y) {
  directionOfMovement = dir;
}

byte Enemy::getDirection() {
  return directionOfMovement;
}

void Enemy::setDirection(byte dir) {
  directionOfMovement = dir;
}


Bomb::Bomb() : Entity(0, 0) {
  spawnTime = millis();
  duration = bombTimer;  
}

Bomb::Bomb(byte x, byte y) : Entity(x, y) {
  spawnTime = millis();
  duration = bombTimer;
}

bool Bomb::stillActive() {
  if (millis() - spawnTime >= duration) {
    return false;
  }
  return true;
}

Explosion::Explosion() : Entity(0, 0) {
  spread = defaultExplosionSpread;
  directionOfSpread = 0;
  resolved = false;
  spawnTime = 0;
  duration = explosionTimer;
}

Explosion::Explosion(byte x, byte y, byte spread, byte directionOfSpread) : Entity(x, y) {
  this->spread = spread;
  this->directionOfSpread = directionOfSpread;
  if (spread == 0)
    this->resolved = true;
  else
    this->resolved = false;
  this->spawnTime = 0;
  this->duration = explosionTimer;
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
  if (spawnTime >= duration) {
    return false;
  }
  spawnTime++;
  return true;
}
