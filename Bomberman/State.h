#pragma once

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
  virtual bool isGameOver() {return false;}
  virtual void updateList(byte) {}
  virtual bool isResettingScores() {return false;}
  virtual void resetScores(int, int, bool) {}
  virtual bool isDisablingSound() {return false;}
  virtual void disableSound(int, int, bool) {}
};


enum class GameState {
  Intro,
  MainMenu,
  HighScoreMenu,
  Credits,
  SettingsMenu,
  InGame,
  GameOver,
};

State& getGameState();
void setGameState(GameState newState);
