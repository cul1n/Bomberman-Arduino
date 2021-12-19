#pragma once
#include "Menu.h"

class HighScoreMenu : public Menu {
  public:
    HighScoreMenu(int number, String *list) : Menu(number, list) {};

    void update(int);
    
    void updateList(byte) override;
};
