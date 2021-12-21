#include "Credits.h"

void Credits::update(int index) {
  switch (index) {
    case 3:
      setGameState(GameState::MainMenu);
      break;
    default:
      break;
  }
}

void Credits::updateList(byte index) {
  // Using the counter variable, the text has half the speed and is more readable
  if (index == 2 && counter % 2 == 0) {
    String link = listOfOptions[index];
    char firstChar = link[0];
    for(int i = 0; i < link.length() - 1; i++ ) {
      link[i] = link[i + 1];
    }
    link[link.length() - 1] = firstChar;
    listOfOptions[index] = link;
    lcd.setCursor(1,0);
    lcd.print(listOfOptions[index]);
  }
  counter++;
}
