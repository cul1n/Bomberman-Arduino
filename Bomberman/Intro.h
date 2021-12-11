#pragma once
#include "State.h"

class Intro : public State {
  bool started = false;
  int numberOfLines;
  String *listOfLines;
  long startTime;
  int waitTime = 3000;
  public:
    Intro(int number, String *list) : numberOfLines(number), listOfLines(list) {}

    bool introSequence() override;
    
    void render(int, int);
};
