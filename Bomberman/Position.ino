#include "Position.h"

Position::Position() {
  posX = 0;
  posY = 0;
}

Position::Position(byte x, byte y) {
  posX = x;
  posY = y;
}

byte Position::getPosX() {
  return posX;
}

byte Position::getPosY() {
  return posY;
}

void Position::setPosX(byte x) {
  posX += x;
}

void Position::setPosY(byte y) {
  posY += y;
}
