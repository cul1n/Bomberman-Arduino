#include "Controls.h"

void Controls::editOption() {
  int yValue = analogRead(yPin);
  int xValue = analogRead(xPin);
  bool swState = digitalRead(swPin);
  xMovement = 0;
  yMovement = 0;
  action = false;
  
  if (yValue > maxThreshold) {
    yMovement = negative;
  }

  if (yValue < minThreshold) {
    yMovement = positive;
  }

  if (xValue > maxThreshold) {
    xMovement = positive;
  }

  if (xValue < minThreshold) {
    xMovement = negative;
  }

  if (swState == LOW && swState != lastSwState) {
    action = true;
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

void Controls::playSound(int frequency, int duration) {
  bool boolType = false;
  if (!EEPROM.get(soundsDisabledAddress, boolType)) {
    tone(buzzerPin, frequency, duration);
  }
}
