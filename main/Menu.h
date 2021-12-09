#pragma once
#include "State.h"

class Menu : public State {
  protected:
    int numberOfOptions;
    String *listOfOptions;
  public:
    Menu(int number, String *list) : numberOfOptions(number), listOfOptions(list) {}
    
    void show();
    
    void render(int index, int lastIndex);

    int getNumberOfOptions();
    
};
