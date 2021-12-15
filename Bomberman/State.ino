#include "State.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "HighScoreMenu.h"
#include "Credits.h"
#include "Intro.h"
#include "InGame.h"
#include "GameOver.h"

String listMainMenu[4] = {"Play", "Settings", "High Score", "Credits"};
String listHighScore[5] = {"High Scores:", "NON 0", "NON 0", "NON 0", "Back"};
String listSettingsMenu[6] = {"Name", "Level", "Contrast", "LCD Brightness", "Mtrx Brghtnss", "Back"};
String listCredits[4] = {"Bomberman", "Calin Hirhui", "Link", "Back"};
String listIntro[2] = {"Bomberman", "Loading.."};

Intro intro(2, listIntro);
MainMenu mainMenu(4, listMainMenu);
HighScoreMenu highScoreMenu(5, listHighScore);
Credits credits(4, listCredits);
SettingsMenu settingsMenu(6, listSettingsMenu);
InGame inGame;
GameOver gameOver;


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

    case GameState::GameOver:
      currentState = &gameOver;
      break;
      
    default:
      currentState = &mainMenu;
      break;
  }
}
