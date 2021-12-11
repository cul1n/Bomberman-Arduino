#include "State.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "HighScoreMenu.h"
#include "Credits.h"
#include "Intro.h"
#include "InGame.h"

String listMainMenu[4] = {"Play", "Settings", "High Score", "Credits"};
String listHighScore[5] = {"High Scores:", "1.NON 0000", "2.NON 000", "3.NON 000", "Back"};
String listSettingsMenu[6] = {"Name", "Level", "Contrast", "LCD Brightness", "Mtrx Brghtnss", "Back"};
String listCredits[4] = {"Bomberman", "Calin Hirhui", "Link", "Back"};
String ListIntro[2] = {"Bomberman", "Loading.."};

Intro intro(2, ListIntro);
MainMenu mainMenu(4, listMainMenu);
HighScoreMenu highScoreMenu(5, listHighScore);
Credits credits(4, listCredits);
SettingsMenu settingsMenu(6, listSettingsMenu);
InGame inGame;


State* currentState = &intro;

State& getGameState() { return *currentState; }

void setGameState(GameState newState) {
  currentState->render(0, -1);

  switch (newState) {
    case GameState::Intro:
      currentState = &intro;
      break;

    case GameState::HighScoreMenu:
      currentState = &highScoreMenu;
      break;

    case GameState::Credits:
      currentState = &credits;
      break;

    case GameState::SettingsMenu:
      currentState = &settingsMenu;
      break;

    case GameState::InGame:
      currentState = &inGame;
      break;

    default:
      currentState = &mainMenu;
      break;
  }
}
