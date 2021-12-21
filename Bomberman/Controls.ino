#include "Controls.h"

void Controls::editOption() {
  int yValue = analogRead(yPin);
  int xValue = analogRead(xPin);
  bool swState = digitalRead(swPin);
  pos = 0;
  letter = 0;
  finished = false;
  
  if (yValue > maxThreshold) {
    letter = negative;
  }

  if (yValue < minThreshold) {
    letter = positive;
  }

  if (xValue > maxThreshold) {
    pos = positive;
  }

  if (xValue < minThreshold) {
    pos = negative;
  }

  if (swState == LOW && swState != lastSwState) {
    finished = true;
  }

  lastSwState = swState;
}

void Controls::updateMenu() {
  int xValue = analogRead(xPin);
  bool swState = digitalRead(swPin);
  lastIndex = index;
  
  if (xValue > maxThreshold) {
    if (index == options - 1) {
      //index = 0;
    } 
    else {
      playSound(menuChangeFrequency, menuChangeDuration);
      index++;
    }
    change = true;    
  }

  if (xValue < minThreshold) {
    if (index == 0) {
      //index = options - 1;
    } 
    else {
      playSound(menuChangeFrequency, menuChangeDuration);
      index--;
    }
    change = true;
  }

  if (swState == LOW && swState != lastSwState) {
    playSound(menuChoiceFrequency, menuChoiceDuration);
    nextMenu = true;
  }

  lastSwState = swState;
}
