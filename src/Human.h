#pragma once

#include "Player.h"

class Human : public Player{
public:
  static const int INVALID_INT_INPUT = 5000; // None of the int inputs that user enters can be 5000 in this class or in the game itself
    
  // Selector
  inline string getPlayerType() const { return "human"; }
  
  // Utility functions 
  int getGameMenuChoice(); 
  static int getNum(const string & potentialNumber); 

  // Utility functions to get game related options that user selected
  void displayGameMenu();
  inline const int menuChoiceHelp() { return Strategy::ASK_FOR_HELP; }
  inline const int menuChoiceQuitGame() { return Strategy::QUIT_THE_GAME; }
  int getMoveChoice(Strategy * moveStrategy);
  int getACardFromThePlayersHand(Strategy * moveStrategy);
  int getCaptureChoice(Strategy * moveStrategy);
  vector<int> getSingleBuildToCapture(Strategy * moveStrategy);
  vector<int> getMultiBuildToCapture(Strategy * moveStrategy);
  int getBuildOption(Strategy * moveStrategy);
  int getAnOpponentsBuildIndex(Strategy * moveStrategy);
  vector <int> selectLooseCardsFromTable(Strategy * moveStrategy);

  void moveSummary(Strategy * moveStrategy);  // Utility function used for help

private:
  // This class inherits all member variables from its parent.
  static const int MIN_MENU_CHOICE = 1;
  static const int MAX_MENU_CHOICE = 4;
  static const int STOP_SELECTING = -1;

  bool hasRepeats(int potentialValue, const vector<int> & a_vector);
};