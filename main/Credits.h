#pragma once
#include "Menu.h"

class Credits : public Menu {
  public:
    Credits(int number, String *list) : Menu(number, list) {};

    void update(int);
};
