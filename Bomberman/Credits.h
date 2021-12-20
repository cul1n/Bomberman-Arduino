#pragma once
#include "Menu.h"

class Credits : public Menu {
  byte counter;
  public:
    Credits(int number, String *list) : Menu(number, list) {counter = 0;};

    void update(int);

    void updateList(byte) override;
};
