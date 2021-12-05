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

Player::Player(byte x, byte y) : Entity(x, y) {}
