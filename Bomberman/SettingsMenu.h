#pragma once
#include "Menu.h"

class SettingsMenu : public Menu {
  bool started = false;
  bool editingName = false;
  bool editingLevel = false;
  bool editingContrast = false;
  bool editingLCDBrightness = false;
  bool editingMatrixBrightness = false;
  String playerName = "";
  int level = 0;
  int contrast = 0;
  int currentPos = 0;
  int lcdBrightness = 0;
  int matrixBrightness = 0;
  char currentChar = 'A';
  const int blinkingInterval = 200;
  unsigned int long long  blinkingTime = 0;
  public:
    SettingsMenu(int number, String *list) : Menu(number, list) {};

    void update(int);

    bool isEditingName() override;

    void editName(int, int, bool);
    
    bool isEditingLevel() override;

    void editLevel(int, int, bool);

    bool isEditingContrast() override;

    void editContrast(int, int, bool);

    bool isEditingLCDBrightness() override;

    void editLCDBrightness(int, int, bool);

    bool isEditingMatrixBrightness() override;

    void editMatrixBrightness(int, int, bool);

    void displayIcon(uint64_t);
};
