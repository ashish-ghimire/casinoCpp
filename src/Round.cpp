#include "Round.h"

int Round::roundCount = 0;
//If turn is true, it is human player's turn. If false, it is computer player's turn
bool Round::m_turn = true; 

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/

/* *********************************************************************
Function Name: Round
Purpose: Custom constructor1. To define initial values for all member variables
Parameters: humanTournamentScore - An integer passed by value
            computerTournamentScore - An integer passed by value
Return Value: None
Assistance Received: none
********************************************************************* */
Round::Round(int humanTournamentScore, int computerTournamentScore) {
    m_playerWhoCapturedLast = NULL;
    //m_deckForTheRound = new Deck(true, "populateTheDeck.txt");
    m_deckForTheRound = new Deck();
    m_computerPlayer = new Computer();
    m_humanPlayer = new Human();
    m_table = new Table();
    m_humanTournamentScore = humanTournamentScore;
    m_computerTournamentScore = computerTournamentScore;
}

/* *********************************************************************
Function Name: Round
Purpose: Custom constructor2. To define initial values for all member variables
Parameters: 
        deck - A Deck pointer that forms the deck for this round
        table - A Table pointer that forms the table for this round
        capturedLast - a Player * value used to determine who captured last
        compPlayer - A computer pointer used to initiliaze computer player
        humanPlayer - A human pointer used to initialize human player
        round_count - An integer variable. Passed by value. Determines the round
                      the game is currently on
        nextPlayer - a string variable used to determine who next player should be
                    in this round
        humanTournamentScore - an integer variable. PAssed by value. Used to
                    initialize the overall tournament score the human player has 
        computerTournamentScore - an integer variable. Passed by value. Used to 
                    initialize score the computer player has 
Return Value: None
Assistance Received: none
********************************************************************* */
Round::Round(Deck * deck, Table * table, Player *capturedLast, Computer *compPlayer, Human *humanPlayer, int round_count, string nextPlayer, int humanTournamentScore, int computerTournamentScore){
    m_table = table;
    m_playerWhoCapturedLast = capturedLast;
    m_computerPlayer = compPlayer;
    m_humanPlayer = humanPlayer;
    roundCount = round_count;
    //Do something about the deck as well
    m_deckForTheRound = deck;
    setTurn(nextPlayer);
    m_humanTournamentScore = humanTournamentScore;
    m_computerTournamentScore = computerTournamentScore;
}

/* *********************************************************************
Function Name: Round
Purpose: To switch the turn of a plaer 
Parameters: whoseTurn - a string variable passed by value
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::setTurn(string whoseTurn){
    std::transform(whoseTurn.begin(), whoseTurn.end(), whoseTurn.begin(), ::tolower);

    if( whoseTurn == m_humanPlayer->getPlayerType() )
        m_turn = true;
    else
        m_turn = false;
}

/* *********************************************************************
Function Name: Round
Purpose: To determine who should play first in all rounds of the tournament
Parameters: whoseTurn - a string variable passed by value
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::setFirstPlayer(bool serialized /* = false*/){
    if(serialized)
        return;

    if(roundCount == 0)
        toss();
}

/* *********************************************************************
Function Name: toss
Purpose: To determine who plays first in the first round of the tournament
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::toss(){
  int toss = rand() % 2; 
  int humanPlayersTossChoice = getHumanPlayersTossChoice();

  if(toss == humanPlayersTossChoice)
    m_turn = true;  
  else
    m_turn = false;
}

/* *********************************************************************
Function Name: getHumanPlayersTossChoice
Purpose: To let the player select either 0 or 1, and return the result
Parameters: None
Return Value: tossChoice, an integer value
Assistance Received: none
********************************************************************* */
int Round::getHumanPlayersTossChoice(){
  int tossChoice;
  string dummyInput;

  do{
    cout << "\nHuman player, please enter your toss choice \n";
    cout << "Enter 0 : for head \n";
    cout << "Enter 1 : for tail \n";
    cin >> dummyInput;
    tossChoice = Human::getNum(dummyInput);

  } while( !( 0 == tossChoice|| 1 == tossChoice) );

  return tossChoice;
}

/* *********************************************************************
Function Name: printWhoPlaysFirst
Purpose: To print who plays first and who plays second
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::printWhoPlaysFirst(){
    cout << endl;
    if(m_turn){
        cout << m_humanPlayer->getPlayerType() << " plays first \n";
        cout << m_computerPlayer->getPlayerType() << " plays second \n";
    }
    else{
        cout << m_computerPlayer->getPlayerType() << " plays first \n";
        cout << m_humanPlayer->getPlayerType() << " plays second \n";
    }
}

/* *********************************************************************
Function Name: playARound
Purpose: To set the round up. And allow a player to play based on whose turn it is
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::playARound(bool serialized /*= false*/){
    setFirstPlayer(serialized);
    printWhoPlaysFirst();

    // The SETUP
    if(!serialized)
        setTheRoundUp();

    // Play a card. firstPlayer plays the card first followed by the second player
    do{
        if(m_turn)
            playATurn(m_humanPlayer);
        else
            playATurn(m_computerPlayer);
        
        if((m_humanPlayer->getHand().empty() && m_computerPlayer->getHand().empty() ))
            continueTheRound(); //Deal more cards

    } while( !roundOver() );

    // At this point, the round is over. Time to update players' score.
    endRound();
    roundCount++;
}

/* *********************************************************************
Function Name: setTheRoundUp
Purpose: To deal cards to human and computer player and place initial loose 
        cards on the table 
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::setTheRoundUp(){

    //Initially, deal first four cards to the human player
    dealCards(m_humanPlayer, 4);

    //Then, deal the next four cards to the computer player
    dealCards(m_computerPlayer, 4);

    // Place the next four cards in the deck face up on the table
    for(int i = 0; i < 4; i++){
        Card * c = m_deckForTheRound->getNextCard();
        m_table->addALooseCard(c);
    }
}

/* *********************************************************************
Function Name: dealCards
Purpose: To deal certain number of cards to certain player
Parameters: playerToDealTo - a pointer to a Player variable
            numCardsToDeal - an integer variable passed by value
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::dealCards(Player * playerToDealCardTo, int numCardsToDeal){

    for(int i = 0; i < numCardsToDeal; i++){
        Card * c = m_deckForTheRound->getNextCard();
        playerToDealCardTo->addToHand(c);
    }
}

/* *********************************************************************
Function Name: playATurn
Purpose: To display a status of the game and ask the user to select 
        an option to continue with or exit the game
Parameters: aPlayer - a player pointer that represents who is playing this
            turn
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::playATurn(Player * aPlayer){
    //Initialize strategy for this turn
    Player * opponent = getOpponentPlayer(aPlayer);

    Strategy * moveStrategy = new Strategy(m_table, aPlayer->getHand(), opponent->getBuild(), opponent->getMultiBuild() );
    int gameMenuChoice;

    //Before every turn, display the status of the game
    displayGameStatus(aPlayer);

    // Before each player plays, the game menu must be displayed and processed.
    // The game menu is different for human and computer player
    do{
        aPlayer->displayGameMenu();
        gameMenuChoice = aPlayer->getGameMenuChoice();

        if(gameMenuChoice == aPlayer->menuChoiceHelp())    // Make this a bit clearer
            aPlayer->moveSummary(moveStrategy);       //Move summary means help

        if(gameMenuChoice == Strategy::SAVE_THE_GAME)
            saveGame();

    } while(!(gameMenuChoice == Strategy::MAKE_A_MOVE || gameMenuChoice == aPlayer->menuChoiceQuitGame() ));
    
    if(gameMenuChoice == Strategy::MAKE_A_MOVE)
        makeAMove(aPlayer, moveStrategy);
    else
        quitGame();

    // If the user has not yet madeAMove, don't change turn
    delete moveStrategy;
}

/* *********************************************************************
Function Name: getOpponentPlayer
Purpose: To return the opponent
Parameters: aPlayer - a pointer to a Player variable  
Return Value: Return pointer to human player if parameter is computer player
              Return pointer to computer player if parameter is human player
Assistance Received: none
********************************************************************* */
Player *Round::getOpponentPlayer(Player * aPlayer){
    if(aPlayer == m_computerPlayer)
        return m_humanPlayer;
    return m_computerPlayer;
}

/* *********************************************************************
Function Name: displayGameStatus
Purpose: To display player's hand, the table and the builds and multibuilds 
         the players have
Parameters: aPlayer - a pointer to a Player variable  
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::displayGameStatus(Player *aPlayer){
    cout << "\nThis is " << aPlayer->getPlayerType() << "'s turn \n";
    cout << "\nThe game status \n";
    cout << "Your hand : \n";
    aPlayer->printHand();

    cout << "\n The Table \n";
    m_table->printTable();

    cout << "\n Builds : \n";
    m_table->printBuildsAndBuildOwners();
}

/* *********************************************************************
Function Name: saveGame
Purpose: To save the game into a text file and exit 
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
/*
Save the game in this format: 

Round: 3
  
Computer:
   Score: 17
   Hand: H5 H6 D4 D7 
   Pile: SX SQ SK D6 H8
 
Human:
   Score: 14
   Hand: SA S4 CA C9
   Pile: DJ DA C3 C5

Table:  [ [C6 S3] [S9] ] [D5 H2] C8 CJ HA

Build Owner: [ [C6 S3] [S9] ] Human
Build Owner: [D5 H2] Computer

Last Capturer: Human

Deck: S7 D3 D5 H2 H3 S5 D8 C2 H9 CX CQ CK HJ S2 S6 D9 DX DQ DK D2 HX HQ HK C4 C7 S8 SJ H4 H7 

Next Player: Human


*/
void Round::saveGame(){
    vector <MultiBuild*> tableMultiBuilds = m_table->getMultiBuilds();
    vector <Build*> tableSingleBuilds = m_table->getSingleBuilds();

    ofstream gameFile;

    gameFile.open ("savedGame.txt");

    gameFile << "Round: " << roundCount << endl;

    gameFile << "\nComputer: \n";
    gameFile << "\tScore: " << m_computerTournamentScore << endl;

    gameFile << "\tHand: ";
    m_computerPlayer->writeHandToFile(gameFile);

    gameFile << "\tPile: ";
    m_computerPlayer->writePileToFile(gameFile);

    gameFile << "\nHuman: \n";
    gameFile << "\tScore: " << m_humanTournamentScore << endl;
    gameFile << "\tHand: ";
    m_humanPlayer->writeHandToFile(gameFile);
    gameFile << "\tPile: ";
    m_humanPlayer->writePileToFile(gameFile);

    gameFile << "\nTable: ";
    m_table->writeTableToFile(gameFile);
    gameFile << "\n";

    //write all table multi builds to file 
    for(int i = 0; i < tableMultiBuilds.size(); i++){
        gameFile << "\nBuild Owner: ";
        tableMultiBuilds[i]->writeMultiBuildToFile(gameFile);
        string playerType = tableMultiBuilds[i]->getOwner();
        playerType[0] = toupper(playerType[0]);
        gameFile << " " << playerType;
    }

    // write all table single builds to file
    for(int i = 0; i < tableSingleBuilds.size(); i++){
        gameFile << "\nBuild Owner: ";
        tableSingleBuilds[i]->writeBuildToFile(gameFile);
        string playerType = tableSingleBuilds[i]->getOwner();
        playerType[0] = toupper(playerType[0]);
        gameFile << " " << playerType;
    }

    gameFile << "\n\nLast Capturer: ";
    
    if(m_playerWhoCapturedLast != NULL){
        string playerType = m_playerWhoCapturedLast->getPlayerType();
        playerType[0] = toupper(playerType[0]);
        gameFile << playerType;
    }
        
    gameFile << "\n\nDeck: ";
    m_deckForTheRound->writeDeckCardsToFile(gameFile);
    gameFile << "\n\nNext Player: ";

    if(m_turn){
       string playerType = m_humanPlayer->getPlayerType();
        playerType[0] = toupper(playerType[0]);
        gameFile << playerType;
    }
    else{
        string playerType = m_computerPlayer->getPlayerType();
        playerType[0] = toupper(playerType[0]);
        gameFile << playerType;
    }

    gameFile.close();
    cout << "\n\n Game Saved! \n\n";
    quitGame();   
}

/* *********************************************************************
Function Name: quitGame
Purpose: To safely exit from the game
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::quitGame(){
    cout << "\n\n\n Exiting.. Thank you for playing this game - @DesignedByAsh \n\n\n";

    exit(1); // terminate 
}

/* *********************************************************************
Function Name: makeAMove
Purpose: To allow a user to either build, capture or trail
Parameters: Player * aPlayer 
            Strategy * moveStrategy
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::makeAMove(Player * aPlayer, Strategy * moveStrategy){
    cout << "\n The " << aPlayer->getPlayerType() << " chose to make a move \n";

    if(aPlayer->getPlayerType() == "computer")
        aPlayer->moveSummary(moveStrategy);

    bool validBuild = false,
        validCapture = false,
        validTrail = false;
    Player * opponent = getOpponentPlayer(aPlayer);

    do{
        int moveChoice = aPlayer->getMoveChoice(moveStrategy);

        switch( moveChoice ){
            case Strategy::BUILD:   
                validBuild = build(aPlayer, moveStrategy);
                break;
            case Strategy::CAPTURE:
                validCapture = capture(aPlayer, moveStrategy);
                break;
            case Strategy::TRAIL:
                validTrail = trail(aPlayer, moveStrategy);
                break;
        }
        
    } while( !(validBuild || validCapture || validTrail) );

    // Show the updated pile and hand
    printHandAndPile();
    m_turn = !m_turn;       
}

/* *********************************************************************
Function Name: build
Purpose: To allow a player to exercise build related options. The player    
         can choose to either create a single build, create a multi build
         or increase the value of an opponent's single build
Parameters: None
Return Value: true if the player successfully exercised one of the build 
            options. false otherwise
Assistance Received: none
********************************************************************* */
bool Round::build(Player * aPlayer, Strategy * moveStrategy){
    cout << "\n The " << aPlayer->getPlayerType() << " chose to build \n";
    int buildChoice = aPlayer->getBuildOption(moveStrategy);

    bool legitSingleBuild = false;
    bool legitMultiBuild = false;
    bool legitIncreaseBuildValue = false;

    switch( buildChoice ){
        case Strategy::CREATE_SINGLE_BUILD: // Create single build
            legitSingleBuild = createSingleBuild(aPlayer, moveStrategy);
            break;
        case Strategy::CREATE_MULTI_BUILD: // Create multiple builds
            legitMultiBuild = createMultiBuild(aPlayer, moveStrategy);
            break;
        case Strategy::INCREASE_OPPONENTS_BUILD: // Increase the value of an already existing build
            legitIncreaseBuildValue = increaseBuildValue(aPlayer, moveStrategy);
            break;
    }
    return legitSingleBuild || legitMultiBuild || legitIncreaseBuildValue;
}

/* *********************************************************************
Function Name: createSingleBuild
Purpose: To allow a player to create a single build
Parameters: 
        aPlayer - a Player pointer 
        moveStrategy - a Strategy pointer used in order to access 
                      strategies a player might need
Return Value: true if the player successfully create a single build. false
            otherwise
Local variables: 
        cardFromHandIndex - an integer variable used to store the index of 
                a card the player chose to play
        indexOfLooseCards - a vector of integer where each integer element is
                an index of a card on the player's hand
        sumOfCurrentBuild - an integer variable used to check if player already 
                has a build whose numeric value is equal to the numeric value 
        oldBuildIndex - an integer variable that represents a build index of 
                a build if the player has a single build whose value is equal 
                to the numeric value of tha card he is trying to play. If the
                player does not have such build, an invalid index is returned
        oldMultiBuildIndex -  an integer variable that represents an index of
                a mutlibuild if the player has a multibuild whose value is equal 
                to the numeric value of the card he is trying to play. If the
                player does not have such multibuild, an invalid index 
                is returned             
Algorithm: Get a valid played card from the players hand. Get an index of table 
           loose cards the user selected. Get the sum of numeric value of table
           loose cards and played card. Check if the player already has a multi
           build with numeric value equal to the sum calculated earlier. If the
           user has such multibuild, create a new single build and add the build
           to the multibuild. IF not, check if the player has a single build with
           value equal to the sum calculated. If the user has such a single build,
           create a new single build with loose cards and the played card. Then, 
           call a function to create a multibuild and add both builds to the 
           multibuild. If the user does not have any builds or multi builds
           with numeric value with numeric value equal to the value of played card,
           build a single build
Assistance Received: none
********************************************************************* */
bool Round::createSingleBuild(Player *aPlayer, Strategy * moveStrategy){
    cout << "\n The " << aPlayer->getPlayerType() << " chose to build a single build \n";

    int cardFromHandIndex = aPlayer->getACardFromThePlayersHand(moveStrategy);

    if( !checkForHoldCard(cardFromHandIndex, aPlayer) )
        return false;
    
    vector <int> indexOfLooseCards = aPlayer->selectLooseCardsFromTable(moveStrategy);

    if(!verifySingleBuild(indexOfLooseCards, cardFromHandIndex, aPlayer) ){
        cout << "\n The single build move you tried to make is invalid. Try again \n";
        return false;
    }

    //If there is already an existing build whose numeric value is equal to that of the current build, create a multi build instead
    int sumOfCurrentBuild = sum(indexOfLooseCards) + aPlayer->getHand()[cardFromHandIndex]->getNumericValue();
    int oldBuildIndex = aPlayer->getBuildIndex(sumOfCurrentBuild);
    int oldMultiBuildIndex = aPlayer->getMultiBuildIndex(sumOfCurrentBuild);

    //MEaning the player already has a multibuild whose capture value == capture value of our new build
    if(oldMultiBuildIndex != aPlayer->getInvalidIndex() ){ 
        Build * newBuild = getNewlyCreatedSingleBuild(indexOfLooseCards, cardFromHandIndex, aPlayer);
        aPlayer->getMultiBuild()[oldMultiBuildIndex]->addToMultiBuild(newBuild);
        return removeTableCards(indexOfLooseCards);
    }

    //If there is already an existing build whose numeric value is equal to that of the current build, create a multi build instead
    if(oldBuildIndex != aPlayer->getInvalidIndex()){ 
        Build * newBuild = getNewlyCreatedSingleBuild(indexOfLooseCards, cardFromHandIndex, aPlayer);
        successfulMultiBuild(aPlayer, cardFromHandIndex, oldBuildIndex, indexOfLooseCards, newBuild);
        return true;
    }

    // If the player does not have any single or multi build whose numeric value == that of the new potential build, just create a single build
    return successfulSingleBuild( indexOfLooseCards, cardFromHandIndex, aPlayer );
}

/* *********************************************************************
Function Name: checkForHoldCard
Purpose: To check if a played card is a hold card meaning the card 
            is necessary to hold in a hand to create a build
Parameters: None
Return Value: true if the played card is legit. false otherwise
Assistance Received: none
********************************************************************* */
bool Round::checkForHoldCard(int cardFromHandIndex, Player * aPlayer){
    if( aPlayer->isHoldCard(cardFromHandIndex) ){
        cout << "\n You can't play "; 
        aPlayer->getHand()[cardFromHandIndex]->printCard();
        cout << " because the card is the hold card for a build or a multi-build \n";
        return false;
    }
    return true;
}

/* *********************************************************************
Function Name: verifySingleBuild
Purpose: To check if the sum of numeric loose cards and played card
        is equal to the value of a card in the player's hand
Parameters: vector<int> & indexOfLooseCards - Passed by reference for efficiency
            cardFromHandIndex - an integer passed by value  that represents 
            the index of a card in the player's hand
            aPlayer - a pointer to a Player variable 
Return Value: true if the played card is legit. false otherwise
Assistance Received: none
********************************************************************* */
bool Round::verifySingleBuild(vector<int> & indexOfLooseCards, int cardFromHandIndex, Player *aPlayer){
    int sumOfSelectedLooseCards = 0;

    for(int i = 0; i < indexOfLooseCards.size(); i++){
        sumOfSelectedLooseCards += ( m_table->getLooseCards()[indexOfLooseCards[i]] )->getNumericValue();
    }

    int sumOfLooseCardsAndCardFromHand = sumOfSelectedLooseCards + aPlayer->getHand()[cardFromHandIndex]->getNumericValue();

    return aPlayer->handHasACardWithAGivenNumericValue(cardFromHandIndex, sumOfLooseCardsAndCardFromHand, true); //true indicates that ace should be treated as 14
}


/* *********************************************************************
Function Name: sum
Purpose: To check if the sum of numeric loose cards and played card
        is equal to the value of a card in the player's hand
Parameters: vector<int> a_vector - contains integer of indices of 
            loose cards on the table
Return Value: an integer value that represents sum of numeric values
                of cards represented by indices in a_vector
Assistance Received: none
********************************************************************* */
int Round::sum(vector <int> a_vector){
    int sum1 = 0;
    vector<Card*> tableLooseCards = m_table->getLooseCards();

    for(int i = 0; i < a_vector.size(); i++){
        int index = a_vector[i];
        sum1 += tableLooseCards[index]->getNumericValue();
    }
    return sum1;
}

/* *********************************************************************
Function Name: getNewlyCreatedSingleBuild
Purpose: To 
Parameters: vector<int> &indexOfLooseCards - contains integer of indices
            of loose cards on the table
Return Value: an integer value that represents sum of numeric values
                of cards represented by indices in a_vector
Assistance Received: none
********************************************************************* */
Build * Round::getNewlyCreatedSingleBuild(vector<int> & indexOfLooseCards, int cardFromHandIndex, Player *aPlayer){
    //Create a single build
    Build * singleBuild = new Build;

    singleBuild->setOwner(aPlayer->getPlayerType());

    // Add the card that the player played to the build
    Card * a = aPlayer->getHand()[cardFromHandIndex];
    singleBuild->addCardToABuild(a);

    // Add the looseCards on the table to the build
    for(int i = 0; i < indexOfLooseCards.size(); i++){
        int looseCardIndex = indexOfLooseCards[i];
        Card * b = m_table->getLooseCards()[looseCardIndex];
        singleBuild->addCardToABuild(b);
    }

    aPlayer->removeHoldCard(cardFromHandIndex);
    // Remove card from the player's hand
    aPlayer->removeFromHand(cardFromHandIndex);

    return singleBuild;
}


/* *********************************************************************
Function Name: removeTableCards
Purpose: To remove 0 or more cards which have their indices in the parameter
Parameters: setOfCards - a vector of int elements
Return Value: None
Assistance Received: none
********************************************************************* */
bool Round::removeTableCards(vector<int> & setOfCards){

    if(setOfCards.size() == 0)
        return false;

    int previousIndex = INT_MAX;

    for(int i = 0; i < setOfCards.size(); i++) {
        int index = setOfCards[i];

        if(index > previousIndex){
            m_table->removeALooseCard(index - i);
        }
        else{
            m_table->removeALooseCard(index);
        }
        previousIndex = index;
    }

    return true;
}

/* *********************************************************************
Function Name: successfulMultiBuild
Purpose: To set the member variables of a newly created multibuild
Parameters: Player * aPlayer,
         int cardFromHandIndex,
         int oldBuildIndex,
         vector <int> & indexOfLooseCards -contains integer of indices
            of loose cards on the table
         Build * newBuild
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::successfulMultiBuild(Player * aPlayer, int cardFromHandIndex, int oldBuildIndex, vector <int> & indexOfLooseCards, Build * newBuild){
    //Create a multiBuild
    MultiBuild * m = new MultiBuild();
    m->addToMultiBuild(newBuild);
    Build * oldBuild = aPlayer->getBuild()[oldBuildIndex];
    m->addToMultiBuild(oldBuild);

    //Set owner of the multiBuild
    m->setOwner(aPlayer->getPlayerType());

    //Remove old build reference from player
    aPlayer->removeBuild(oldBuildIndex);

    //Remove old build reference from table
    m_table->removeABuild(oldBuild);

    // Remove previous loose cards from the table
    removeTableCards(indexOfLooseCards);

    // Let player have a reference for the new multi build
    aPlayer->addAMultiBuild(m);

    // Let the table have a reference for the new multi build
    m_table->addAMultiBuild(m);
}

/* *********************************************************************
Function Name: successfulSingleBuild
Purpose: To set the member variables of a newly created multibuild
Parameters: Player * aPlayer,
         int cardFromHandIndex,
         int oldBuildIndex,
         vector <int> & indexOfLooseCards -contains integer of indices
            of loose cards on the table
         Build * newBuild
Return Value: None
Assistance Received: none
********************************************************************* */
bool Round::successfulSingleBuild(vector<int> & indexOfLooseCards, int cardFromHandIndex, Player *aPlayer){
    //Create a single build
    Build * singleBuild = getNewlyCreatedSingleBuild(indexOfLooseCards, cardFromHandIndex, aPlayer);

    // Add the single build to the table and to the player
    m_table->addABuild(singleBuild);
    aPlayer->addABuild(singleBuild);

    //Remove previous loose cards from the table as they are not loose cards anymore
    return removeTableCards(indexOfLooseCards);
}

/* *********************************************************************
Function Name: createMultiBuild
Purpose: To create a multibuild if the user can do so
Parameters: aPlayer - a Player pointer 
        moveStrategy - a Strategy pointer used in order to access 
                      strategies a player might need
Return Value: true if the user successfully created a multibuild.
              false otherwise
Assistance Received: none
********************************************************************* */
bool Round::createMultiBuild(Player *aPlayer, Strategy * moveStrategy){
    cout << "\n The " << aPlayer->getPlayerType() << " chose to build a multi build \n";
    if( !aPlayer->hasSingleBuild() ){
        cout << "Invalid attempt! You can only build a multi build if you have a single build \n";
        return false;
    }

    int buildIndex = getAPlayersBuild(aPlayer);    // get a build that the player already has
    int cardFromHandIndex = aPlayer->getACardFromThePlayersHand(moveStrategy);

    if( !checkForHoldCard(cardFromHandIndex, aPlayer) )
        return false;

    Card * playedCard = aPlayer->getHand()[cardFromHandIndex];
    int sum = playedCard->getNumericValue();

    bool tableHasLooseCards = !m_table->getLooseCards().empty();

    int buildNumericValue = m_table->getSingleBuilds()[cardFromHandIndex]->getNumericValue();
    vector <int> indexOfLooseCards;
    int looseCardIndex;

    while(sum < buildNumericValue && tableHasLooseCards){
        looseCardIndex = getALooseCard(aPlayer, indexOfLooseCards);
        Card * looseCard = m_table->getLooseCards()[looseCardIndex];
        sum += looseCard->getNumericValue();
        indexOfLooseCards.push_back(looseCardIndex);
        tableHasLooseCards = m_table->getLooseCards().size() == indexOfLooseCards.size();
    }

    if(sum == buildNumericValue){
        Build * newBuild = getNewlyCreatedSingleBuild(indexOfLooseCards, cardFromHandIndex, aPlayer);
        successfulMultiBuild(aPlayer, cardFromHandIndex, buildIndex, indexOfLooseCards, newBuild);
        return true;
    }

    cout << "Invalid multi-build attempt! \n";
    return false;
}

/* *********************************************************************
Function Name: getAPlayersBuild
Purpose: To return an index of a player's build
Parameters: aPlayer - a Player pointer 
Return Value: an integer value representing an index of a build
Assistance Received: none
********************************************************************* */
int Round::getAPlayersBuild(Player *aPlayer){
    int buildIndex;
    string dummyInput;

    do{
        cout << "   Select a build. The builds you can select are displayed below: \n";
        cout << "   Type the build index. Build index is the integer in the parenthesis to the right of the build \n";
        aPlayer->printSingleBuildsIndexed(); //Print opponentsBuild

        cin >> dummyInput;
        buildIndex = Human::getNum(dummyInput);

    } while( !(buildIndex >= 0 && buildIndex <= aPlayer->getBuild().size()) );

    return buildIndex;
}


/* *********************************************************************
Function Name: getALooseCard
Purpose: To return an index of a loose card from the table
Parameters: aPlayer - a Player pointer 
            vector <int> & indexOfLooseCards - Contains the indices
                    of loose cards the user has already selected.
Return Value: an integer value representing an index of a loose card
                that the user has not yet selected
Assistance Received: none
********************************************************************* */
int Round::getALooseCard(Player * aPlayer, vector <int> & indexOfLooseCards){
    int looseCard;
    bool validLooseCard;
    string dummyInput;

    do{
        cout << "   Select a loose card from the table \n";
        m_table->printLooseCardsIndexed();
        cin >> dummyInput;
        looseCard = Human::getNum(dummyInput);
        validLooseCard = validatePotentialLooseCard(looseCard, aPlayer, indexOfLooseCards);

        if(!validLooseCard ){
            cout << "\n Invalid loose card \n";
        }

    } while(!validLooseCard);

    return looseCard;
}

/* *********************************************************************
Function Name: validatePotentialLooseCard
Purpose: To check if the loose card that the user tried to select is valid
Parameters: aPlayer - a Player pointer 
            vector <int> & indexOfLooseCards - Contains the indices
                    of loose cards the user has already selected.
            potentialLooseCard - integer passed by value. This is the
                    loose card that should be checked
Return Value: true of the loose card is valid. false otherwise
Assistance Received: none
********************************************************************* */
// Check if the potentialLooseCard is one of the loose cards on the table
bool Round::validatePotentialLooseCard(int potentialLooseCard, Player * aPlayer, vector <int> & indexOfLooseCards){
    if(potentialLooseCard < 0 || potentialLooseCard >= m_table->getLooseCards().size())
        return false;

    if ( find(indexOfLooseCards.begin(), indexOfLooseCards.end(), potentialLooseCard) != indexOfLooseCards.end() )
        return false;  //The user has already selected that card before

    return true;
}

/* *********************************************************************
Function Name: increaseBuildValue
Purpose: To allow the user to increase an opponent's build if she can do so
Parameters: aPlayer - a Player pointer 
            moveStrategy - a Strategy pointer used in order to access 
                      strategies a player might need
Return Value: true if the player legitimately increased an opponent's build
              false otherwise
Assistance Received: none
********************************************************************* */
bool Round::increaseBuildValue(Player *aPlayer, Strategy * moveStrategy){
    //Check if the opponent has a single build. If not, return
    Player * opponent = getOpponentPlayer(aPlayer);

    if( !opponent->hasSingleBuild() ){
        cout << "Your opponent does not have a build. So, you can't increase the opponent's build\n ";
        return false;
    }

    displayGameStatus(aPlayer);
    cout << "\n The " << aPlayer->getPlayerType() << " chose to increase the value of an existing single build \n";

    int cardFromHandIndex = aPlayer->getACardFromThePlayersHand(moveStrategy);

    if( !checkForHoldCard(cardFromHandIndex, aPlayer) )
        return false;

    int buildIndex = aPlayer->getAnOpponentsBuildIndex(moveStrategy);

    Build * opponentBuild = opponent->getBuild()[buildIndex];
    int buildSum = opponentBuild->getNumericValue();
    int cardFromHandValue = aPlayer->getHand()[cardFromHandIndex]->getNumericValue();

    if(aPlayer->handHasACardWithAGivenNumericValue(cardFromHandIndex, buildSum + cardFromHandValue, true)){ //Ace is treated as 14
        opponentBuild->addCardToABuild(aPlayer->getHand()[cardFromHandIndex]);
        aPlayer->removeHoldCard(cardFromHandIndex);
        aPlayer->removeFromHand(cardFromHandIndex);
        opponentBuild->setOwner(aPlayer->getPlayerType());
        return true;
    }
    else
        cout << "\n The move is invalid. Try again \n";

    return false;
}

/* *********************************************************************
Function Name: capture
Purpose: To allow the user to capture either sets of loose cards, builds,
         multibuilds or loose cards on the table
Parameters: aPlayer - a Player pointer 
            moveStrategy - a Strategy pointer used in order to access 
                      strategies a player might need
Return Value: true if the player captured at least one card. false otherwise
Assistance Received: none
********************************************************************* */
bool Round::capture(Player *aPlayer, Strategy * moveStrategy){

    int cardFromHandIndex = aPlayer->getACardFromThePlayersHand(moveStrategy);
    Card * handCard = aPlayer->getHand()[cardFromHandIndex];

    //Start by capturing all loose cards that match the played card's numeric value
    bool capturedAtLeastOneCard = false; 

    int captureChoice;
    bool capturedSetOfCards = false,
        capturedSingleBuild = false,
        capturedIndividualCards = false,
        capturedMultiBuild = false,
        canQuit = false;

    const int QUIT_CAPTURING = 5;
    // Don't make this a loop. Change the design
    do{
        captureChoice = aPlayer->getCaptureChoice(moveStrategy);

        switch(captureChoice){
            case Strategy::CAPTURE_SET_OF_LOOSE_CARDS: //Capture set of cards
                capturedSetOfCards = captureSetOfCards(aPlayer, cardFromHandIndex, moveStrategy);
                if(!capturedAtLeastOneCard)
                    capturedAtLeastOneCard = capturedSetOfCards;
                break;
            case Strategy::CAPTURE_SINGLE_BUILD: // Capture single build
                capturedSingleBuild = captureSingleBuild(aPlayer, cardFromHandIndex, moveStrategy);
                if(!capturedAtLeastOneCard)
                    capturedAtLeastOneCard = capturedSingleBuild;
                break;
            case Strategy::CAPTURE_MULTI_BUILD: // Capture multi build
                capturedMultiBuild = captureMultiBuild(aPlayer, cardFromHandIndex, moveStrategy);
                if(!capturedAtLeastOneCard)
                    capturedAtLeastOneCard = capturedMultiBuild;
                break;
            case Strategy::CAPTURE_INDIVIDUAL_LOOSE_CARDS: // Capture individual loose cards
                capturedIndividualCards = captureIndividualCards(aPlayer, cardFromHandIndex);
                if(!capturedAtLeastOneCard)
                    capturedAtLeastOneCard = capturedIndividualCards;
                break;
        }

        if(captureChoice == QUIT_CAPTURING){
            if( m_table->hasALooseCardWithGivenNumericValue(handCard->getNumericValue()) ){
                cout << "Error. You can't quit capturing because you still need to capture an individual loose card with the value equal to the played card \n";
                canQuit = false;
            }
            else
                canQuit = true;
        }

    } while( !(captureChoice == QUIT_CAPTURING && canQuit));

    //You must remove the the played card from the players  hand and add it to his pile if the player has captured any card
    if(capturedAtLeastOneCard){
        aPlayer->addToPile(handCard);
        aPlayer->removeHoldCard(cardFromHandIndex);
        aPlayer->removeFromHand(cardFromHandIndex);
        m_playerWhoCapturedLast = aPlayer;
    }
        
    return capturedAtLeastOneCard;
}

/* *********************************************************************
Function Name: captureSetOfCards
Purpose: To allow the user to capture a set of loose cards
Parameters: aPlayer - a Player pointer 
            moveStrategy - a Strategy pointer used in order to access 
                      strategies a player might need
            cardFromHandIndex - an integer passed by value. Represents 
                      the index of a card in a player's hand
Return Value: true if the player captured at least one card. false otherwise
Assistance Received: none
********************************************************************* */
bool Round::captureSetOfCards(Player *aPlayer, int cardFromHandIndex, Strategy * moveStrategy){
    if( !checkForHoldCard(cardFromHandIndex, aPlayer) )
        return false;

    moveStrategy->setTable(m_table);
    moveStrategy->recalculateSetOfLooseCards();

    vector <int> oneSetOfLooseCards = aPlayer->selectLooseCardsFromTable(moveStrategy);

    Card * playedCard = aPlayer->getHand()[cardFromHandIndex];

    if( sum(oneSetOfLooseCards) != playedCard->getNumericValue(true) ) { //true indicates that the value of Ace should be 14, not 1
        cout << "Error! The sum of the set of loose cards you selected don't match the numeric value of the played card \n";
        return false;
    }

    for(int i = 0; i < oneSetOfLooseCards.size(); i++){
        Card * c = m_table->getLooseCards()[oneSetOfLooseCards[i] ];
        aPlayer->addToPile(c);
    }

    //Remove the captured cards from the table
    printCaptureMessage(oneSetOfLooseCards);
    return removeTableCards(oneSetOfLooseCards);
}

/* *********************************************************************
Function Name: printCaptureMessage
Purpose: To display the loose cards represented by the indices in the 
            parameter
Parameters: vector <int> tableCardsThatMustBeCaptured 
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::printCaptureMessage(vector <int> tableCardsThatMustBeCaptured){
    if(tableCardsThatMustBeCaptured.size() > 0)
        cout << "\n The following loose cards were captured from the table \n";

    //Right outside for loop
    for(int i = 0; i < tableCardsThatMustBeCaptured.size(); i++){
        int indexOfLooseCardOnTable = tableCardsThatMustBeCaptured[i];
        Card * c = m_table->getLooseCards()[indexOfLooseCardOnTable];
        c->printCard();
    }
}

/* *********************************************************************
Function Name: captureSingleBuild
Purpose: To allow the user to capture one or more single builds
Parameters: aPlayer - a Player pointer 
            moveStrategy - a Strategy pointer used in order to access 
                      strategies a player might need
            cardFromHandIndex - an integer passed by value. Represents 
                      the index of a card in a player's hand
Return Value: true if the player captured at least one build. false otherwise
Assistance Received: none
********************************************************************* */
bool Round::captureSingleBuild(Player *aPlayer, int cardFromHandIndex, Strategy * moveStrategy){
    if( m_table->getSingleBuilds().empty() )
        return false;

    cout << "The " << aPlayer->getPlayerType() << " choose to capture a single build \n";

    vector<int> tableBuildIndices = aPlayer->getSingleBuildToCapture(moveStrategy);
    if(tableBuildIndices.size() > m_table->getSingleBuilds().size())
        return false;

    // Check if the card that the player is trying to play is a hold card
    bool holdCard = aPlayer->isHoldCard(cardFromHandIndex);
    int numBuildsCaptured = 0;
    int previousIndex = 0;

    for(int i = 0; i < tableBuildIndices.size(); i++){
        int tableBuildIndex = tableBuildIndices[i];

        if(tableBuildIndex > previousIndex)
            tableBuildIndex -= 1;

        Build * selectedBuild = m_table->getSingleBuilds()[tableBuildIndex];
        
        if(holdCard && !aPlayer->hasAGivenSingleBuild(selectedBuild) )
            break;

        int buildValue = selectedBuild->getNumericValue();
        int cardNumericValue = aPlayer->getHand()[cardFromHandIndex]->getNumericValue(true); //true means the value of ace should be 14

        if(cardNumericValue == buildValue) // The player can capture the build
        {
            //Add the cards in the build to the player's pile
            aPlayer->addToPile(selectedBuild);

            // Remove the reference the table has to the build
            m_table->removeABuild(tableBuildIndex);

            // Remove the reference that the player has to the build
            if(aPlayer->getPlayerType() == selectedBuild->getOwner())
                aPlayer->removeBuild(selectedBuild);
            else
                getOpponentPlayer(aPlayer)->removeBuild(selectedBuild);

            // Deallocate the memory associate with the build
            delete selectedBuild;
            cout << "Successfully captured one single build \n";
            numBuildsCaptured++;
        }
        else
            break;
        previousIndex = tableBuildIndex;
    }

    if(numBuildsCaptured > 0)
        return true;

    cout << "\nNo single build captured \n";
    return false;
}

/* *********************************************************************
Function Name: captureMultiBuild
Purpose: To allow the user to capture one or more single builds
Parameters: aPlayer - a Player pointer 
            moveStrategy - a Strategy pointer used in order to access 
                      strategies a player might need
            cardFromHandIndex - an integer passed by value. Represents 
                      the index of a card in a player's hand
Return Value: true if the player captured at least one multibuild. false 
              otherwise
Assistance Received: none
********************************************************************* */
bool Round::captureMultiBuild(Player *aPlayer, int cardFromHandIndex, Strategy * moveStrategy){
    if( m_table->getMultiBuilds().empty() )
        return false;

    cout << "The " << aPlayer->getPlayerType() << " choose to capture a multi build \n";

    vector <int> tableMultiBuildIndices = aPlayer->getMultiBuildToCapture(moveStrategy);

    if(tableMultiBuildIndices.size() > m_table->getMultiBuilds().size())
        return false;

    // Check if the card that the player is trying to play is a hold card
    bool holdCard = aPlayer->isHoldCard(cardFromHandIndex);
    int numMultiBuildsCaptured = 0;
    int previousIndex = 0;

    for(int i = 0; i < tableMultiBuildIndices.size(); i++){
        int tableMultiBuildIndex = tableMultiBuildIndices[i];

        if(tableMultiBuildIndex > previousIndex)
            tableMultiBuildIndex -= 1;

        MultiBuild * selectedMultiBuild = m_table->getMultiBuilds()[tableMultiBuildIndex];

        if(holdCard && !aPlayer->hasAGivenMultiBuild(selectedMultiBuild) )
            break;

        int buildValue = selectedMultiBuild->getNumericValue();
        int cardNumericValue = aPlayer->getHand()[cardFromHandIndex]->getNumericValue(true);  //true means the value of ace should be 14

        if(cardNumericValue == buildValue){
            // Add the cards in the multi build to the players pile
            aPlayer->addToPile(selectedMultiBuild);

            // Remove the reference the table has to the multi build
            m_table->removeAMultiBuild(tableMultiBuildIndex);

            // Remove the reference that the player has to the multi build
            if(aPlayer->getPlayerType() == selectedMultiBuild->getOwner())
                aPlayer->removeMultiBuild(selectedMultiBuild);
            else
                getOpponentPlayer(aPlayer)->removeMultiBuild(selectedMultiBuild);

            cout << aPlayer->getPlayerType() << " captured a multibuild \n";
            numMultiBuildsCaptured++;
            // Deallocate the memory associated with the multi-build
            delete selectedMultiBuild;
        }
        else
            break;    
        previousIndex = tableMultiBuildIndex;
    }

    if(numMultiBuildsCaptured > 0)
        return true;

    cout << "\nNo multi build captured \n";
    return false;
}

/* *********************************************************************
Function Name: captureIndividualCards
Purpose: To allow the user to capture one or loose cards whose value
            equal to the value of card that the player played
Parameters: aPlayer - a Player pointer 
            cardFromHandIndex - an integer passed by value. Represents 
                      the index of a card in a player's hand
Return Value: true if the player captured at least one multibuild. false 
              otherwise
Assistance Received: none
********************************************************************* */
bool Round::captureIndividualCards(Player *aPlayer, int cardFromHandIndex){
    if( !checkForHoldCard(cardFromHandIndex, aPlayer) )
        return false;

    Card * cardFromPlayersHand = aPlayer->getHand()[cardFromHandIndex];
    vector<Card*> looseCardsOnTable = m_table->getLooseCards();

    vector <int> tableCardsThatMustBeCaptured;

    for(int i = 0; i < looseCardsOnTable.size(); i++){
        if(looseCardsOnTable[i]->getNumericValue() == cardFromPlayersHand->getNumericValue() ) {
            tableCardsThatMustBeCaptured.push_back(i);
            aPlayer->addToPile(looseCardsOnTable[i]);  //Adding the cards that should be captured to the players pile
        }
    }

    //You must let the player know that individual cards with value equal to played card have been captured
    printCaptureMessage(tableCardsThatMustBeCaptured);

    //Remove the captured cards from the table
    return removeTableCards(tableCardsThatMustBeCaptured);
}


/* *********************************************************************
Function Name: trail
Purpose: To allow the user to trail
Parameters: aPlayer - a Player pointer 
            moveStrategy - a Strategy pointer used in order to access 
                      strategies a player might need
Return Value: true if the player successfully trailed. false
              otherwise
Assistance Received: none
********************************************************************* */
bool Round::trail(Player * aPlayer,  Strategy * moveStrategy){
    cout << "The " << aPlayer->getPlayerType() << " choose to trail \n";

    int cardFromHandIndex = aPlayer->getACardFromThePlayersHand(moveStrategy);

    if( !checkForHoldCard(cardFromHandIndex, aPlayer) )
        return false;

    if( hasIndividualCards( aPlayer, cardFromHandIndex) ){
        cout << "\n You can't trail because your hand has a card that matches the numeric value of a loose card on the table \n";
        return false;
    }

    //  Trailing option is not available to the owner of a build
    // - since that player can play the card matching the build to capture it or work on a multiple build.

    if(aPlayer->hasSingleBuild() || aPlayer->hasMultiBuild() ){
        cout << "\n You own either a single build or a multibuild. So you cant trail \n";
        return false;
    }

    Card * trailedCard = aPlayer->getHand()[cardFromHandIndex];
    // The player can trail.
    // Add the played card to the table
    m_table->addALooseCard( trailedCard );

    cout << "The " << aPlayer->getPlayerType() << " player trailed "
         << trailedCard->getSuiteCharacter() << "" << trailedCard->getFaceValue() << endl; 

    // Remove the played card from the players hand
    aPlayer->removeFromHand(cardFromHandIndex);

    return true;
}

/* *********************************************************************
Function Name: hasIndividualCards
Purpose: To check if the table has any loose cards whose value is equal
            to the card the player played
Parameters: aPlayer - a Player pointer 
            cardFromHandIndex - an integer passed by value. Represents 
                      the index of a card in a player's hand
Return Value: true if the table has a loose card like the one define
              in purpose. false otherwise
Assistance Received: none
********************************************************************* */
bool Round::hasIndividualCards(Player * aPlayer, int cardFromHandIndex){
    Card * cardFromPlayersHand = aPlayer->getHand()[cardFromHandIndex];
    vector <Card *> looseCardsOnTable = m_table->getLooseCards();

    for(int i = 0; i < looseCardsOnTable.size(); i++){
        if(looseCardsOnTable[i]->getNumericValue() == cardFromPlayersHand->getNumericValue() ) {
            return true;
        }
    }
    return false;
}

/* *********************************************************************
Function Name: printHandAndPile
Purpose: To print both player's pile and hands, deck and the table
Parameters: aPlayer - a Player pointer 
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::printHandAndPile(){
    cout << "\n-------------------------------------------------------------\n";

    cout << "Human Player's pile :\n";
    m_humanPlayer->printPile();

    cout << "\nComputer Player's pile :\n";
    m_computerPlayer->printPile();
    
    cout << "\n The Table :\n";
    m_table->printTable();
    cout  << "\n";

    cout << "\n The Deck :\n";
    m_deckForTheRound->printDeckCards();
    cout  << "\n";

    cout << "--------------------------------------------------------------\n";
}

/* *********************************************************************
Function Name: continueTheRound
Purpose: Continue playing the round by dealing cards to the players
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::continueTheRound(){
    //Each time all the players have played all their cards, four more cards are dealt
    // first to the human player and then to the computer from the remaining deck
    // and the round continues.
    if(!roundOver() ){
        dealCards(m_humanPlayer, 4);
        dealCards(m_computerPlayer, 4);
    }
}

/* *********************************************************************
Function Name: roudnOver
Purpose: To determine if players have played all the cards and deck is empty
Parameters: None
Return Value: true if the round is over. false otherwise
Assistance Received: none
********************************************************************* */
bool Round::roundOver(){
    return m_deckForTheRound->isEmpty() && m_humanPlayer->getHand().empty() && m_computerPlayer->getHand().empty();
}

/* *********************************************************************
Function Name: endRound
Purpose: To successfully end the round by following these rules.
        The round ends when the players have played all the cards and the deck 
        is empty. Any cards that remain on the table are taken by the last 
        player that made a capture.The piles of both the players are printed
        at the end of the round. 
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::endRound(){
    cout << m_playerWhoCapturedLast->getPlayerType() << endl;
    if(m_playerWhoCapturedLast != NULL){
        //Add remaining loose cards on the table to the m_playerWhoCapturedLast's pile 
        vector<Card*> tableLooseCards = m_table->getLooseCards();
        vector <int> temp;
        for(int i = 0; i < tableLooseCards.size(); i++){
            Card * c = tableLooseCards[i];
            m_playerWhoCapturedLast->addToPile(c);
            temp.push_back(i);
        }
        removeTableCards(temp);

        //Add remaining cards in a build on the table to the m_playerWhoCapturedLast's pile 
        vector <Build*> tableSingleBuilds = m_table->getSingleBuilds();
        for(int i = 0; i < tableSingleBuilds.size(); i++){
            Build * b = tableSingleBuilds[i];
            m_playerWhoCapturedLast->addToPile(b);
        }

        //Add remainging cards in a multi-build on the table to the m_playerWhoCapturedLast's pile
        vector <MultiBuild*> tableMultiBuilds = m_table->getMultiBuilds();
        for(int i = 0; i < tableMultiBuilds.size(); i++){
            MultiBuild * m = tableMultiBuilds[i];
            m_playerWhoCapturedLast->addToPile(m);
        }

        //Change turn based on plaer who captured last. The player that captured last in round x, will play first in round x + 1 if round x is not the last round
        setTurn(m_playerWhoCapturedLast->getPlayerType());
    }
    
    updateScore();
    determineTheWinner();
    printHandAndPile();
}

/* *********************************************************************
Function Name: updateScore
Purpose: To update the score based on the game rules which are:
When a round ends, the points earned by each player are calculated based 
on the cards in each player's pile:

    - The player with the most cards in the pile gets 3 points. In the event 
    of a tie, neither player gets points.
    - The player with the most spades gets 1 point. In the event of a tie,
    neither player gets points.
    - The player with 10 of Diamonds gets 2 points.
    - The player with 2 of Spades gets 1 point.
    - Each player gets one point per Ace.

Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
/*

*/
void Round::updateScore(){ //Based on the players' piles

  updateScoreBasedOnTheBiggestPile();
  updateScoreBasedOnMostSpades();
  updateScoreBasedOnTenOfDiamonds();
  updateScoreBasedOnTwoOfSpades();
  updateScoreBasedOnAces();
}

/* *********************************************************************
Function Name: updateScoreBasedOnTheBiggestPile
Purpose: To update the score based on based on this rule:

    - The player with the most cards in the pile gets 3 points. In the event 
    of a tie, neither player gets points.
    
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::updateScoreBasedOnTheBiggestPile(){
  if( m_computerPlayer->getPile().size() > m_humanPlayer->getPile().size() )
    m_computerPlayer->updateScore(3);
  else if( m_humanPlayer->getPile().size() > m_computerPlayer->getPile().size() )
    m_humanPlayer->updateScore(3);
  
  cout << "\n"<< m_humanPlayer->getPlayerType() << " has " << m_humanPlayer->getPile().size() << " cards in their pile \n";
  cout <<  m_computerPlayer->getPlayerType() << " has " << m_computerPlayer->getPile().size() << " in their pile\n";
}

/* *********************************************************************
Function Name: updateScoreBasedOnMostSpades
Purpose: To update the score based on based on this rule:


    The player with the most spades gets 1 point. In the event of a tie, 
    neither player gets points.

Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::updateScoreBasedOnMostSpades(){
  int spadesCountHumanPlayer = 0;
  int spadesCountComputerPlayer = 0;

  vector <Card*> humanPlayerPile = m_humanPlayer->getPile();
  vector <Card*> computerPlayerPile = m_computerPlayer->getPile();

  for(int i = 0; i < humanPlayerPile.size(); i++){
    Card * card = humanPlayerPile[i];
    if(card->getSuiteCharacter() == card->getSpade() )         
      spadesCountHumanPlayer += 1;
  }

  for(int j = 0; j < computerPlayerPile.size(); j++){
    Card * card = computerPlayerPile[j];
    if(card->getSuiteCharacter() == card->getSpade() )         
      spadesCountComputerPlayer += 1;
  }

  if(spadesCountComputerPlayer > spadesCountHumanPlayer)
    m_computerPlayer->updateScore(1);
  else if(spadesCountHumanPlayer > spadesCountComputerPlayer)
    m_humanPlayer->updateScore(1);

  cout << "\n"<< m_humanPlayer->getPlayerType() << " has " << spadesCountHumanPlayer << " spades \n";
  cout << m_computerPlayer->getPlayerType() << " has " << spadesCountComputerPlayer << " spades\n";
}


/* *********************************************************************
Function Name: updateScoreBasedOnTenOfDiamonds
Purpose: To update the score based on based on this rule:
   The player with 10 of Diamonds gets 2 points.
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::updateScoreBasedOnTenOfDiamonds(){

  Card * anyCard = m_humanPlayer->getPile()[0];
  updatePointsBasedOnACard(anyCard->getDiamond(), anyCard->getFaceFor10(), 2, m_humanPlayer );
  updatePointsBasedOnACard(anyCard->getDiamond(), anyCard->getFaceFor10(), 2, m_computerPlayer );
}

/* *********************************************************************
Function Name: updatePointsBasedOnACard
Purpose: To update the score based on a card and the number of points to
        add to the score
Parameters: char suite - represents suite character of a card
            char face - reprsents face value of a card
            int pts - represents the amount of points to add to a player's score
            Player * aPlayer - player whose points needs to be updated
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::updatePointsBasedOnACard(char suite, char face, int pts, Player * aPlayer){
    vector <Card *> playerPile = aPlayer->getPile();

    for(int i = 0; i < playerPile.size(); i++){
        Card * c = playerPile[i];
        if( c->getSuiteCharacter() == suite && c->getFaceValue() == face ){   // USe symbolic constant
            aPlayer->updateScore(pts);  //Use symbolic constant
            return; 
        }
    }
}

/* *********************************************************************
Function Name: updateScoreBasedOnTwoOfSpades
Purpose: To update the score based on based on this rule:
   The player with 2 of Spades gets 1 point.
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::updateScoreBasedOnTwoOfSpades(){

  Card * anyCard = m_humanPlayer->getPile()[0];
  updatePointsBasedOnACard(anyCard->getSpade(), '2', 1, m_humanPlayer );
  updatePointsBasedOnACard(anyCard->getSpade(), '2', 1, m_computerPlayer );
}

/* *********************************************************************
Function Name: updateScoreBasedOnAces
Purpose: To update the score based on based on this rule:
   Each player gets one point per Ace.
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::updateScoreBasedOnAces(){
  vector <Card *> humanPlayerPile = m_humanPlayer->getPile();
  vector <Card *> computerPlayerPile = m_computerPlayer->getPile();

  for(int i = 0; i < humanPlayerPile.size(); i++){
      Card * c = humanPlayerPile[i];
      if(c->getFaceValue() == c->getAce() )
        m_humanPlayer->updateScore(1);
  }

  for(int i = 0; i < computerPlayerPile.size(); i++){
    Card * c = computerPlayerPile[i];
      if(c->getFaceValue() == c->getAce() )
        m_computerPlayer->updateScore(1);
  }
}

/* *********************************************************************
Function Name: determineTheWinner
Purpose: To look at the score of both the players and to figure out who
         won the round
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Round::determineTheWinner(){
    int humanPlayerScore = m_humanPlayer->getScore();
    int computerPlayerScore = m_computerPlayer->getScore();
    
    cout << endl;
    cout << m_humanPlayer->getPlayerType() << " player got " << humanPlayerScore << " points in this round \n";
    cout << m_computerPlayer->getPlayerType() << " player got " << computerPlayerScore << " points in this round \n";

    if(humanPlayerScore > computerPlayerScore){
        cout << "Human player won the round \n";
    }
    else if(computerPlayerScore > humanPlayerScore)
        cout << "Computer player won the round \n";
    else 
        cout << "It's a tie !!!!!!! \n";
}



