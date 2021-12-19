#pragma once
#include "Menu.h"

class MainMenu : public Menu {
  public:
    MainMenu(int number, String *list) : Menu(number, list) {};
    
    void update(int);

    void updateList(byte) override;

    void displayIcon(uint64_t);
  
};
