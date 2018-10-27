#include "Human.h"

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/

/* *********************************************************************
Function Name: getGameMenuChoice
Purpose: To return the game option user has selected. Options can include
        saving the game, making a move, asking for help or quitting the game
Parameters: None
Return Value: menuChoice, an integer variable representing the menu option 
          selected by the user
Assistance Received: none
********************************************************************* */ 
int Human::getGameMenuChoice(){
  int menuChoice; 
  string dummyMenuChoice;

  do{
    cout << "Please enter a menu choice (" << MIN_MENU_CHOICE << " - " << MAX_MENU_CHOICE << ")" << endl;
    cin >> dummyMenuChoice;

    //Verifying the input to make sure that the user enters an integer value
    menuChoice = getNum(dummyMenuChoice); 
  } while( !(menuChoice >= MIN_MENU_CHOICE && menuChoice <= MAX_MENU_CHOICE) );
  
  return menuChoice;
}

/* *********************************************************************
Function Name: getNum
Purpose: Check 
if the parameter recieved is a one digit integer (-ve or +ve).
Parameters: potentialNumber a const string variable passed by reference for efficiency
Return Value: a valid one digit integer if potential number is valid. Else return
            an integer that corresponds to an error value 
Assistance Received: none
********************************************************************* */
int Human::getNum(const string & potentialNumber){
  // A string representing a potentialNumber can't be longer than 2 character. 
  // 1 character if potentialNumber is +ve. Two characters if it is -ve
  if(potentialNumber.length() > 2 ) 
    return INVALID_INT_INPUT;

  if( 2 == potentialNumber.length() ){  //Possibly a negative number
    if(potentialNumber[0] != '-')
        return INVALID_INT_INPUT;
        
    string temp;
    temp += potentialNumber[1]; // Extracting the digit from the negative number
    int tempNum = getNum(temp);
    if( INVALID_INT_INPUT != tempNum)
      return -1 * tempNum;

    return tempNum; 
  }    

  char firstChar = potentialNumber[0];

  // 48 = asciiCodeFor 0 . 57 = asciiCode for 9  
  if( firstChar >= 48 && firstChar <= 57 ) 
    //Say the user entered 9. Ascii code of 9 is 57. We are returning (57 - 48) = 9, which is the value we should  be returning
    return firstChar - 48; 
        
  return INVALID_INT_INPUT;  
}

/* *********************************************************************
Function Name: displayGameMenu
Purpose: Display a menu to the user based on menu options defined in Strategy class
        The menu could look something like this:

        1. Save the game
        2. Make a move
        3. Ask for help (only before human player plays)
        4. Quit the game

Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Human::displayGameMenu(){
  cout << "\n" << Strategy::SAVE_THE_GAME << ". Save the game\n";
  cout << Strategy::MAKE_A_MOVE << ". Make a move \n";
  cout << Strategy::ASK_FOR_HELP << ". Ask for help \n";
  cout << Strategy::QUIT_THE_GAME << ". Quit the game \n";
}

/* *********************************************************************
Function Name: getMoveChoice
Purpose: To compute the human player's move choice. The player can either
        build, capture or trail according to the rules of the game
Parameters: a pointer to a Strategy object
Return Value: valid move choice selected by the user
Assistance Received: none
********************************************************************* */
int Human::getMoveChoice(Strategy * moveStrategy){
  int moveChoice;
  string dummyMoveChoice;

    do{
        cout << "\n You can either build, capture or trail \n";
        cout << "Please select : \n";
        cout << Strategy::BUILD << ") To build \n";
        cout << Strategy::CAPTURE << ") To capture \n";
        cout << Strategy::TRAIL << ") To trail \n";
        cin >> dummyMoveChoice;
        moveChoice = getNum(dummyMoveChoice); //getting valid single digit integer

    } while( !(moveChoice >= Strategy::BUILD && moveChoice <= Strategy::TRAIL) );

    return moveChoice;
}

/* *********************************************************************
Function Name: getACardFromThePlayersHand
Purpose: To get an index of a card from the player's hand. The user will select
         the index based on the prompt displayed in this function
Parameters: a pointer to a Strategy object
Return Value: valid card index selected by the user
Assistance Received: none
********************************************************************* */
int Human::getACardFromThePlayersHand(Strategy * moveStrategy){
  int cardFromHandIndex;
  string dummyInput;

    do{
        cout << "   Select a card from your hand. Your hand is displayed below: \n";
        cout << "   Type the card index. Card index is the integer in the parenthesis to the right of the card in your hand \n";
        printHandIndexed();

        cin >> dummyInput;
        cardFromHandIndex = getNum(dummyInput);

    } while( !(cardFromHandIndex >= 0 && cardFromHandIndex < m_hand.size()) );

    return cardFromHandIndex;
}

/* *********************************************************************
Function Name: getCaptureChoice
Purpose: To get valid integer value that represents the type of capture the
        user wants to make
Parameters: a pointer to a Strategy object
Return Value: valid cpture choice selected by the user
Assistance Received: none
********************************************************************* */
int Human::getCaptureChoice(Strategy *moveStrategy){
  int captureChoice;
  string dummyInput;

  do{
      cout << "\n Press one of the following capture options (" << Strategy::CAPTURE_SET_OF_LOOSE_CARDS << " -" << Strategy::QUIT_CAPTURING << ") \n";
      cout << "Press \n";
      cout << "      " << Strategy::CAPTURE_SET_OF_LOOSE_CARDS << ") To capture set of cards \n";
      cout << "      " << Strategy::CAPTURE_SINGLE_BUILD << ") To capture single build \n";
      cout << "      " << Strategy::CAPTURE_MULTI_BUILD << ") To capture multi build \n";
      cout << "      " << Strategy::CAPTURE_INDIVIDUAL_LOOSE_CARDS << ") To capture individual loose cards \n";
      cout << "      " << Strategy::QUIT_CAPTURING << ") To quit \n";
      cin >> dummyInput;
      captureChoice = getNum(dummyInput);

  } while(!(captureChoice >= Strategy::CAPTURE_SET_OF_LOOSE_CARDS && captureChoice <= Strategy::QUIT_CAPTURING)  );

  return captureChoice;
}

/* *********************************************************************
Function Name: getSingleBuildToCapture
Purpose: To allow the user to select indices of builds they want to capture
Parameters: a pointer to a Strategy object
Return Value: valid indices of the single build the user wants to capture
Local variables: 
          buildIndices - a vector of int elements. Each int element should 
                        correspond to an index of a single build the user
                        wants to capture
          gameTable - a reference to the game's table used for efficiency reasons
          buildIndex - an integer. Each buildIndex element should 
                      correspond to an index of a single build the user
                      wants to capture
          dummyInput - a string value to which input will be read first before
                      asisgning the input from dummyInput to buildIndex
          invalidEntry - a boolean flag. The value is true if user selected a 
                      legitimate build index. false otherwise
Algortithm: Display table builds with corresponding indices the user can select.
            Prompt the user to enter a buildIndex until he/she wants to stop
            selecting. Verify each index the user entered and if the index is 
            valid, insert the indiex to the vector, buildIndices which is to be
            returned to the caller of this function 
Assistance Received: none
********************************************************************* */
vector<int> Human::getSingleBuildToCapture(Strategy *moveStrategy){
  vector <int> buildIndices;
  Table * gameTable = moveStrategy->getGameTable();
  int buildIndex = INVALID_INT_INPUT;
  bool invalidEntry = false;
  string dummyInput;

  while( invalidEntry || buildIndex != STOP_SELECTING ) {
    invalidEntry = false;

    cout << "   Select a build one at a time. The builds you can select are displayed below: \n";
    cout << "   Type the build index. Build index is the integer in the parenthesis to the right of the build \n";
    cout << "   To stop selecting press -1 \n";
    gameTable->printSingleBuildsIndexed();

    cin >> dummyInput;
    buildIndex = getNum(dummyInput); 

    if( !( buildIndex >= STOP_SELECTING && buildIndex < (int) gameTable->getSingleBuilds().size() ) )
      invalidEntry = true;

    if(hasRepeats(buildIndex, buildIndices))
      invalidEntry = true;

    if(invalidEntry)
      cout << "\n Invalid entry. Input again \n";
    else{
      if(buildIndex != STOP_SELECTING)
        buildIndices.push_back(buildIndex);
    }
  }

  return buildIndices;
}

/* *********************************************************************
Function Name: hasRepeats
Purpose: To check if a vector has a specific element in it 
Parameters: 
        a_vector -a const vector of integers passed by reference for efficiency. 
        potentialValue - an integer value passed by value. potentialValue represents
                  the value that should be searched inside a_vector
Return Value: true if the vector has the element specified in the parameter, 
              potentialValue. false otherwise
Assistance Received: none
********************************************************************* */
bool Human::hasRepeats(int potentialValue, const vector<int> & a_vector){ 
  if ( find(a_vector.begin(), a_vector.end(), potentialValue) != a_vector.end() ) //Repeat
    return true; 

  return false;
}

/* *********************************************************************
Function Name: getMultiBuildToCapture
Purpose: To allow the user to select indices of multibuilds they want to capture
Parameters: a pointer to a Strategy object
Return Value: valid indices of the multi builds the user wants to capture
Local variables: 
          multibuildIndices - a vector of int elements. Each int element should 
                        correspond to an index of a multi build the user
                        wants to capture
          gameTable - a reference to the game's table used for efficiency reasons
          multibuildIndex - an integer. Each multibuildIndex element should 
                      correspond to an index of a multi build the user
                      wants to capture
          dummyInput - a string value to which input will be read first before
                      asisgning the input from dummyInput to multiBuildIndex
          invalidEntry - a boolean flag. The value is true if user selected a 
                      legitimate build index. false otherwise
Algortithm: Display table multi builds with corresponding indices the user can select.
            Prompt the user to enter a multiBuildIndex until he/she wants to stop
            selecting. Verify each index the user entered and if the index is 
            valid, insert the index to the vector, multiBuildIndices which is to be
            returned to the caller of this function 
Assistance Received: none
********************************************************************* */
vector<int> Human::getMultiBuildToCapture(Strategy *moveStrategy){

  vector <int> multiBuildIndices;
  Table * gameTable = moveStrategy->getGameTable();
  int multiBuildIndex = INVALID_INT_INPUT;
  
  bool invalidEntry = false;
  string dummyInput;

  while(multiBuildIndex != STOP_SELECTING || invalidEntry ) {
    invalidEntry = false;
    cout << "   Select a multi build one at a time. The multi builds you can select are displayed below: \n";
    cout << "   Type the multi build index. Multi Build index is the integer in the parenthesis to the right of the multi build \n";
    cout << "   To stop selecting press -1 \n";
    gameTable->printMultiBuildsIndexed();

    cin >> dummyInput;
    multiBuildIndex = getNum(dummyInput);

    if( !(multiBuildIndex >= STOP_SELECTING && multiBuildIndex < (int) gameTable->getMultiBuilds().size() ) ){
      invalidEntry = true;
    }

    if(hasRepeats(multiBuildIndex, multiBuildIndices))
      invalidEntry = true;

    if(invalidEntry)
      cout << "\n Invalid entry. Input again \n";
    else{
      if(multiBuildIndex != STOP_SELECTING)
        multiBuildIndices.push_back(multiBuildIndex);
    }
  }

  return multiBuildIndices;  
}

/* *********************************************************************
Function Name: getBuildOption
Purpose: To select a valid build option to either create a single build,
        create a multi-build or increase the opponent's build
Parameters: a pointer to a Strategy object used
Return Value: legit build choice 
Assistance Received: none
********************************************************************* */
int Human::getBuildOption(Strategy *moveStrategy){
  int buildChoice;
  string dummyInput;

    do{
        cout << "\n Please select one of the following build options \n";
        cout << Strategy::CREATE_SINGLE_BUILD << " Create a single build \n";
        cout << Strategy::CREATE_MULTI_BUILD << " Create a multiple build \n";
        cout << Strategy::INCREASE_OPPONENTS_BUILD << " Increase an opponent's build \n";
        cin >> dummyInput;
        buildChoice = getNum(dummyInput);
    } while( !(buildChoice >= Strategy::CREATE_SINGLE_BUILD && buildChoice <= Strategy::INCREASE_OPPONENTS_BUILD) );

  return buildChoice;
}

/* *********************************************************************
Function Name: getAnOpponentsBuildIndex
Purpose: To select a valid index that represents the opponents build. Say the 
        opponent has builds [D5 H2] (0) [D3 S2] (1). Selecting 1 would mean 
        that the user has selcted the build [D3 S2] 
Parameters: a pointer to a Strategy object used to access a function in Strategy class
Return Value: Legit build index, an integer variable
Assistance Received: none
********************************************************************* */
int Human::getAnOpponentsBuildIndex(Strategy *moveStrategy){
  int buildIndex;
  string dummyInput;

    do{
        cout << "   Select a build. The builds you can select are displayed below: \n";
        cout << "   Type the build index. Build index is the integer in the parenthesis to the right of the build \n";
        moveStrategy->printOpponentsBuildIndexed(); 
        cin >> dummyInput;
        buildIndex = getNum(dummyInput);

    } while( !(buildIndex >= 0 && buildIndex < moveStrategy->getOpponentsBuild().size())  );

    return buildIndex;
}

/* *********************************************************************
Function Name: selectLooseCardsFromTable
Purpose: To allow the user to select indices of loose cards they want to select
Parameters: a pointer to a Strategy object
Return Value: valid indices of the loose cards the user wants to select
Local variables: 
          indexOfLooseCards - a vector of int elements. Each int element should 
                        correspond to an index of a table loose card the user
                        wants to select
          gameTable - a reference to the game's table used for efficiency reasons
          looseCardIndex - an integer. Each looseCardIndex element should 
                      correspond to an index of a loose card the user
                      wants to select
          dummyInput - a string value to which input will be read first before
                      asisgning the input from dummyInput to another variable
          invalidEntry - a boolean flag. The value is true if user selected a 
                      legitimate loose card index. false otherwise
Algortithm: Display table loose cards with corresponding indices the user can select.
            Prompt the user to enter a looseCardIndex until he/she wants to stop
            selecting. Verify each index the user entered and if the index is 
            valid, insert the index to the vector, indexOfLooseCards which is to be
            returned to the caller of this function 
Assistance Received: none
********************************************************************* */
vector<int> Human::selectLooseCardsFromTable(Strategy *moveStrategy){
  vector <int> indexOfLooseCards;
  Table * gameTable = moveStrategy->getGameTable();
  int looseCardIndex; 
  bool invalidEntry = false;
  string dummyInput; 

  while(looseCardIndex != STOP_SELECTING || invalidEntry ) {
    invalidEntry = false;

    cout << "   Select a loose card one at a time. The loose cards you can select are displayed below: \n";
    cout << "   Type the loose card index. Loose index is the integer in the parenthesis to the right of the loose card \n";
    cout << "   To stop selecting press -1 \n";
    gameTable->printLooseCardsIndexed();

    cin >> dummyInput;
    looseCardIndex = getNum(dummyInput);

    if( looseCardIndex < STOP_SELECTING || looseCardIndex >= (int) gameTable->getLooseCards().size() ){
      invalidEntry = true;
    }

    if(hasRepeats(looseCardIndex, indexOfLooseCards)){
      invalidEntry = true;
    }

    if(invalidEntry)
      cout << "\n Invalid entry. Input again \n";
    else{
      if(looseCardIndex != STOP_SELECTING)
        indexOfLooseCards.push_back(looseCardIndex);
    }
  }

  return indexOfLooseCards;
}

/* *********************************************************************
Function Name: moveSummary
Purpose: To display help for the human player
Parameters: a pointer to a Strategy object used to access the function 
            that sumarizes what move the player should make
Return Value: None
Assistance Received: none
********************************************************************* */
void Human::moveSummary(Strategy * moveStrategy){
   cout << "\n***********************************************************\n";
  cout << "\nYou should ";
  moveStrategy->summarizeMove();
  
   cout << "\n Note: When capturing the cards, be sure to follow the exact"
        << " order of capturing as shown above \n";
   cout << "\n***********************************************************\n";
}