#pragma once
#include <WString.h>

class State {
 public:
  virtual void update(int index) {}
  virtual void render(int index, int lastIndex) {}
  virtual int getNumberOfOptions() {}
  virtual bool isEditingName() { return false; }
  virtual void editName(int, int, bool) {}
  virtual bool isEditingLevel() { return false; }
  virtual void editLevel(int, int, bool) {}
  virtual bool isEditingContrast() { return false; }
  virtual void editContrast(int, int, bool) {}
  virtual bool isEditingLCDBrightness() {return false;}
  virtual void editLCDBrightness(int, int, bool) {}
  virtual bool isEditingMatrixBrightness() {return false;}
  virtual void editMatrixBrightness(int, int, bool) {}
  virtual bool introSequence() {return false;}
  virtual bool isPlaying() {return false;}
  virtual void playerController(int, int, bool) {}
};


enum class GameState {
  Intro,
  MainMenu,
  HighScoreMenu,
  Credits,
  SettingsMenu,
  InGame,
};

State& getGameState();
void setGameState(GameState newState);
