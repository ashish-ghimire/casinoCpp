#include "Computer.h"

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/

/* *********************************************************************
Function Name: displayGameMenu
Purpose: Display a menu to the user based on menu options defined in Strategy class
        The menu could look something like this:

        1. Save the game
        2. Make a move
        3. Quit the game

Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Computer::displayGameMenu(){
  cout << "\n" << Strategy::SAVE_THE_GAME << ". Save the game\n";
  cout << Strategy::MAKE_A_MOVE << ". Make a move \n";
  cout << MAX_MENU_CHOICE << ". Quit the game \n"; 
}

/* *********************************************************************
Function Name: getGameMenuChoice
Purpose: To return the game option user has selected. Options can include
        saving the game, making a move or quitting the game
Parameters: None
Return Value: menuChoice, an integer variable representing the menu option 
          selected by the user
Assistance Received: none
********************************************************************* */ 
int Computer::getGameMenuChoice(){
  int menuChoice; 

  do{
    cout << "Please enter a menu choice (" << MIN_MENU_CHOICE << " - " << MAX_MENU_CHOICE << ")" << endl;
    cin >> menuChoice;
  } while( !(menuChoice >= MIN_MENU_CHOICE && menuChoice <= MAX_MENU_CHOICE) );
  
  return menuChoice;
}

/* *********************************************************************
Function Name: getMoveChoice
Purpose: To compute the computer player's move choice. The player can either
        build, capture or trail according to the rules of the game
Parameters: a pointer to a Strategy object used to access the "optimal" action
            the user should exercise
Return Value: valid move choice selected by the user, an integer
Assistance Received: none
********************************************************************* */
int Computer::getMoveChoice(Strategy * moveStrategy){ 
  return moveStrategy->getAction();
}

/* *********************************************************************
Function Name: getACardFromThePlayersHand
Purpose: To get an index of a card from the player's hand. 
Parameters: a pointer to a Strategy object used to access the "optimal" card that
            the computer can play
Return Value: valid card index selected by the user, an integer
Assistance Received: none
********************************************************************* */
int Computer::getACardFromThePlayersHand(Strategy * moveStrategy){
  return moveStrategy->getCardToPlay();
}

/* *********************************************************************
Function Name: getCaptureChoice
Purpose: To get valid integer value that represents the type of capture the
        user wants to make
Parameters: a pointer to a Strategy object used to get the "optimal" capture 
            option the user should exercise
Return Value: valid capture choice, an integer
Assistance Received: none
********************************************************************* */
int Computer::getCaptureChoice(Strategy *moveStrategy){
  return moveStrategy->whatToCapture();
}

/* *********************************************************************
Function Name: getSingleBuildToCapture
Purpose: To select indices of builds the user should capture
Parameters: a pointer to a Strategy object used to get the "optimal" single builds
          the user can capture
Return Value: valid indices of the single build the user wants to capture, a vector of 
              integer variables where each integer represents an index of a single build
              on the game table
Assistance Received: none
********************************************************************* */
vector<int> Computer::getSingleBuildToCapture(Strategy *moveStrategy){
  return moveStrategy->getSingleBuildsToCapture();
}

/* *********************************************************************
Function Name: getMultiBuildToCapture
Purpose: To select indices of multi builds the user should capture
Parameters: a pointer to a Strategy object used to get the "optimal" multi builds
          the user can capture
Return Value: valid indices of the multi builds the user wants to capture, a vector of 
              integer variables where each integer represents an index of a multi build
              on the game table
Assistance Received: none
********************************************************************* */
vector<int> Computer::getMultiBuildToCapture(Strategy *moveStrategy){
  return moveStrategy->getMultiBuildsToCapture();
}

/* *********************************************************************
Function Name: getBuildOption
Purpose: To select a valid build option to either create a single build,
        create a multi-build or increase the opponent's build
Parameters: a pointer to a Strategy object used to get the "optimal" build option
            the user should exercise
Return Value: legit build choice, an integer variable
Assistance Received: none
********************************************************************* */
int Computer::getBuildOption(Strategy *moveStrategy){
  return moveStrategy->getBuildOption();
}

/* *********************************************************************
Function Name: getAnOpponentsBuildIndex
Purpose: To select a valid index that represents the opponents build. Say the 
        opponent has builds [D5 H2] (0) [D3 S2] (1). Selecting 1 would mean 
        that the user has selcted the build [D3 S2] 
Parameters: a pointer to a Strategy object used to access a function in Strategy class
Return Value:  Legit build index, an integer variable
Assistance Received: none
********************************************************************* */
int Computer::getAnOpponentsBuildIndex(Strategy *moveStrategy){
  return moveStrategy->getOpponentsBuildToIncrease();
}

/* *********************************************************************
Function Name: selectLooseCardsFromTable
Purpose: To allow the user to select indices of loose cards
Parameters: a pointer to a Strategy object used to get the loose cards based
            on the game table
Return Value: valid indices of the loose cards the user wants to select
Assistance Received: none
********************************************************************* */
vector<int> Computer::selectLooseCardsFromTable(Strategy *moveStrategy){
  if(moveStrategy->getAction() == moveStrategy->BUILD)
    return moveStrategy->getLooseCardsForSingleBuild();

  return moveStrategy->getOneSetOfLooseCardsToCapture();
}

/* *********************************************************************
Function Name: moveSummary
Purpose: To display a summary of moves the computer player made
Parameters: a pointer to a Strategy object used to access the function 
            that sumarizes what move the player made
Return Value: None
Assistance Received: none
********************************************************************* */
void Computer::moveSummary(Strategy *moveStrategy){
  cout << "\n***********************************************************\n";
  cout << "\nThe computer decided to ";
  moveStrategy->summarizeMove();
  cout << "\n***********************************************************\n";
}