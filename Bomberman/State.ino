#include "State.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "HighScoreMenu.h"
#include "Credits.h"
#include "Intro.h"
#include "InGame.h"
#include "GameOver.h"

String listMainMenu[mainMenuNumberOfOptions] = {"Play", "Settings", "High Score", "Credits"};
String listHighScore[highScoreNumberOfOptions] = {"High Scores:", noPlayer, noPlayer, noPlayer, "Back"};
String listSettingsMenu[settingsNumberOfOptions] = {"Name", "Level", "Contrast", "LCD Brightness", "Mtrx Brghtnss", "Reset HiScore", "Sound", "Back"};
String listCredits[creditsNumberOfOptions] = {"Bomberman", "Calin Hirhui", "github.com/cul1n/Bomberman-Arduino  ", "Back"};
String listIntro[introNumberOfOptions] = {"Bomberman", "Loading.."};

Intro intro(introNumberOfOptions, listIntro);
MainMenu mainMenu(mainMenuNumberOfOptions, listMainMenu);
HighScoreMenu highScoreMenu(highScoreNumberOfOptions, listHighScore);
Credits credits(creditsNumberOfOptions, listCredits);
SettingsMenu settingsMenu(settingsNumberOfOptions, listSettingsMenu);
InGame inGame;
GameOver gameOver;


State* currentState = &intro;

State& getGameState() { return *currentState; }

void setGameState(GameState newState) {

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
