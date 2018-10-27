#pragma once

#include "Player.h"

class Computer : public Player{
public:
  // Selector
  inline string getPlayerType() const { return "computer"; }

  // Utility functions to get game related options that user selected
  void displayGameMenu();
  int getGameMenuChoice();

  // In the function below, INVALID_INDEX is a dummy choice. This is a function the computer
  // will never use but it is important to have this short function for the code to be polymorphic 
  // in classes that use this class
  inline const int menuChoiceHelp() { return INVALID_INDEX; } 
  inline const int menuChoiceQuitGame() { return MAX_MENU_CHOICE; }

  int getMoveChoice(Strategy * moveStrategy);
  int getACardFromThePlayersHand(Strategy * moveStrategy);
  int getCaptureChoice(Strategy * moveStrategy);
  vector<int> getSingleBuildToCapture(Strategy * moveStrategy);
  vector<int> getMultiBuildToCapture(Strategy * moveStrategy);
  int getBuildOption(Strategy * moveStrategy);
  int getAnOpponentsBuildIndex(Strategy * moveStrategy);
  vector <int> selectLooseCardsFromTable(Strategy * moveStrategy);

  void moveSummary(Strategy * moveStrategy);

private:
  static const int MIN_MENU_CHOICE = 1;
  static const int MAX_MENU_CHOICE = 3;

  bool canCaptureIndividualLooseCards();
};
