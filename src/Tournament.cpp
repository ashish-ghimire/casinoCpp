#include "Tournament.h"

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/


/* *********************************************************************
Function Name: Tournament
Purpose: Default constructor. To define initial values for all member variables.
          This function does so by dynmaically creating new Human and Computer objects
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
Tournament::Tournament(){
  m_computerPlayer = new Computer();
  m_humanPlayer = new Human();
}

/* *********************************************************************
Function Name: ~Tournament
Purpose: Default destructor. To release the resources used by Human and Computer member
        variables
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
Tournament::~Tournament(){
  delete m_computerPlayer;
  delete m_humanPlayer;
}

/* *********************************************************************
Function Name: startTheGame
Purpose: To determine whether the tournament should be loaded from a file or not
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Tournament::startGame(){
  cout << " ------------------ CASINO -------------------------------------------" << endl;
  
  char loadGame;
  srand(time(NULL));

  do{
    cout << "\n Do you want to load the game from an existing file (y/n)? \n";
    cin >> loadGame;
    loadGame = tolower(loadGame);
  } while(!(loadGame == 'y' || loadGame == 'n') );
  
  switch(loadGame){
    case 'y': loadTheTournament();
      break;
    case 'n': playTheTournamentWithoutSerialization();
      break;
  }
}

/* *********************************************************************
Function Name: loadtheTournament
Purpose: To set up the tournament so that the tournament can be played based
        on information from a file

Parameters: None
Return Value: None
Local variables:
          roundNumber - An int variable which will contain the round number in the torunament
          computerScore - An int variable which will contain computer's tournament score
          humanScore- An int variable which will contain computer's tournament score
          nextPlayer - A string variable which will determine who plays next in the round
          humanHand- a vector of string. Stores the strings that could form cards
                    to be added to a human player's hand
          humanPile - a vector of string. Stores the strings that could form cards
                    to be added to a human player's pile
          computerHand- a vector of string. Stores the strings that could form cards
                    to be added to a computer player's hand
          computerPile - a vector of string. Stores the strings that could form cards
                    to be added to a computer player's hand
          computersSingleBuilds - Vector of vector of string. Passed by reference. This vector will contain 
                    one or more vectors of string. Each vector of string represents a single build
          vector < vector <vector <string> > > computersMultiBuilds;
          humansSingleBuilds- Vector of vector of string. Passed by reference. This vector will contain 
                    one or more vectors of string. Each vector of string represents a single build
          vector <vector <vector <string> > > humansMultiBuilds;
          vector <vector <string> > tableSingleBuilds;
          vector <vector <vector <string> > > tableMultiBuilds;
          tableLooseCards- a vector of string. Stores the strings that could form loose
                    cards to be added to the table
          deckCards - a vector of string. Stores the strings that could form cards
                    to be added to a deck
          playerWhoCapturedLast - Determines the player who captured last. Can be either
                    "human" or "computer"
Algorithm: Ask the user for the name of the file to load the game from. Call a function to parse 
          the file. The called function sets up the values for the local variables described above. 
          After local variables have proper values, use those values to create human, computer, table, deck, build,
          multi build objects. BAsed on the objects created, play a round. If the game is not over in that round,
          play additional rounds. Additional rounds will be played without serialization
Assistance Received: none
********************************************************************* */ 
void Tournament::loadTheTournament(){
  //Get file path from the player
  string filePath;
  cout << "Please enter the file path : ";

  while(cin.peek()=='\n'){
    cin.ignore();
  }
  getline(cin, filePath);

  int roundNumber;
  int computerScore;
  int humanScore;
  string nextPlayer;
  vector <string> humanHand;
  vector <string> humanPile;
  vector <string> computerHand;
  vector <string> computerPile;
  vector <vector <string> > computersSingleBuilds;
  vector < vector <vector <string> > > computersMultiBuilds;
  vector <vector <string> > humansSingleBuilds;
  vector <vector <vector <string> > > humansMultiBuilds; //Assumes the user has only one multibuild
  vector <vector <string> > tableSingleBuilds;
  vector <vector <vector <string> > > tableMultiBuilds;
  vector <string> tableLooseCards;
  vector <string> deckCards;
  string playerWhoCapturedLast;

  loadSerializationFile(filePath, roundNumber, computerScore, humanScore, nextPlayer, humanHand, humanPile, computerHand, computerPile, computersSingleBuilds, computersMultiBuilds, humansSingleBuilds, humansMultiBuilds, tableSingleBuilds, tableMultiBuilds, tableLooseCards, deckCards, playerWhoCapturedLast);

  // Set the tournament players' scores
  m_computerPlayer->setScore(computerScore);
  m_humanPlayer->setScore(humanScore);

  // Create table
  Table * table = new Table();   

  Human * humanPlayer = new Human();
  Computer * computerPlayer = new Computer();
  Player * whoCapturedLast;

  createPlayer(humanPlayer, humanHand, humanPile, humansSingleBuilds, humansMultiBuilds, table, "human");
  createPlayer(computerPlayer, computerHand, computerPile, computersSingleBuilds, computersMultiBuilds, table, "computer");

  if( playerWhoCapturedLast == "Human" )
    whoCapturedLast = humanPlayer;
  else
    whoCapturedLast = computerPlayer;

  addLooseCards(tableLooseCards, table);

  // Create Deck
  vector <Card*> cardsInADeck;
  createDeck(cardsInADeck, deckCards);
  Deck * deck = new Deck(cardsInADeck);
  
  //First Round after serialization
  Round newRound(deck, table, whoCapturedLast, computerPlayer, humanPlayer, roundNumber, nextPlayer, m_humanPlayer->getScore(), m_computerPlayer->getScore() );    
  newRound.playARound(true); // True indicates that the game is serialized (loaded from file)

  // After each round is over, update the player scores here
  m_humanPlayer->updateScore(newRound.getHuman()->getScore());
  m_computerPlayer->updateScore(newRound.getComputer()->getScore());

  printScores();

  //Subsequent rounds after serialization
  playTheTournamentWithoutSerialization();
}

/* *********************************************************************
Function Name: loadSerializationFile
Purpose: To open a text file and read information from the file line by line. 
        The function looks for specific key words in a line and based on the 
        keyword, calls other utility functions to parse a specific part of the
        file
Parameters:
        string filePath,
        roundNumber - An integer variable passed by reference. It's value is modofied based on 
                    "Round Number:" phrase in the the textfile
        computerScore - An integer variable passed by reference. It's value is modofied based on 
                    "Score:" phrase in the the textfile. Represents tournament score of computer player
        humanScore - An integer variable passed by reference. It's value is modofied based on 
                    "Score:" phrase in the the textfile. Represents tournament score of human player
        nextPlayer - An integer variable passed by reference. It's value is modofied based on 
                    "next player:" phrase in the the textfile. Represents who plays next
        humanHand - a vector of string. Passed by reference. Stores the strings that could form cards
                    to be added to a human player's hand
        humanPile - a vector of string. Passed by reference. Stores the strings that could form cards
                    to be added to a human player's pile
        computerHand - a vector of string. Passed by reference. Stores the strings that could form cards
                    to be added to a computer player's hand
        computerPile - a vector of string. Passed by reference. Stores the strings that could form cards
                    to be added to a computer player's pile
        computersSingleBuilds - Vector of vector of string. Passed by reference. This vector will contain 
                    one or more vectors of string. Each vector of string represents a single build
        vector < vector <vector <string> > > & computersMultiBuilds - Contains 0 or more vector of 
                    vector of string. Each vector of vector of string represents a multibuild.
                    Each vector of string represents a single build. Each string in the vector
                    represents a card. 6 of Hearts would be represeneted as H6
        vector <vector <string> > & humansSingleBuilds- This vector will contain one or more
                    vectors of string. Each vector of string represents a single build
        vector <vector <vector <string> > > & humansMultiBuilds - Contains 0 or more vector of 
                    vector of string. Each vector of vector of string represents a multibuild.
                    Each vector of string represents a single build. Each string in the vector
                    represents a card. 6 of Hearts would be represeneted as H6
        tableSingleBuilds - Vector of vector of string. Passed by reference. This vector will contain 
                    one or more vectors of string. Each vector of string represents a single build
        vector <vector <vector <string> > > & tableMultiBuilds - Contains 0 or more vector of 
                    vector of string. Each vector of vector of string represents a multibuild.
                    Each vector of string represents a single build. Each string in the vector
                    represents a card. 6 of Hearts would be represeneted as H6
        tableLooseCards - a vector of string. Passed by reference. Zero or more string vaiables representing
                    cards are added to the vector. Eg: string representation of 6 of hearts would be HA
        deckCards - a vector of string. Passed by reference. Zero or more string vaiables representing
                    cards are added to the vector. Eg: string representation of 6 of hearts would be HA
        playerWhoCapturedLast - a string variable. Passed by reference. This variable will contain the name
                    of the player who captured last in the current round. Name can be either "human" or
                    "computer"
Return Value: None
Local variables:
        inFile - an ifstream file that represents the file that can be opened for reading.
        oneline - string variable that represents a line in a text file
        computerScoreLine - a specific line in the text file that has information about computer player's score
        omputerHandLine - a specific line in the text file that has information about computer player's hand
        computerPileLine -a specific line in the text file that has information about computer player's pile
        humanScoreLine - a specific line in the text file that has information about human player's score
        humanHandLine - a specific line in the text file that has information about human player's hand
        humanPileLine - a specific line in the text file that has information about human player's pile

Algorithm: Open a file if it exists. Parse the file line by line. When parsing each line, look if a certain 
          keyword exists in a line. Call appropriate fucnction for fine tuned parsing based on the keyword.
          For instance, if the line has keyword, "Table", call appropriate functions to set up components
          of a table like single build, multi build and loose cards.
Assistance Received: none
********************************************************************* */  
void Tournament::loadSerializationFile(string filePath, int & roundNumber, int & computerScore, int & humanScore, string & nextPlayer, vector<string> & humanHand, vector<string> & humanPile, vector <string> & computerHand, vector <string> & computerPile, vector <vector <string> > & computersSingleBuilds, vector < vector <vector <string> > > & computersMultiBuilds, vector <vector <string> > & humansSingleBuilds, vector <vector <vector <string> > > & humansMultiBuilds, vector <vector <string> > & tableSingleBuilds, vector <vector <vector <string> > > & tableMultiBuilds, vector <string> & tableLooseCards, vector <string> & deckCards, string & playerWhoCapturedLast){
  ifstream inFile;
  string oneline;
  string computerScoreLine;
  string computerHandLine;
  string computerPileLine;
  string humanScoreLine;
  string humanHandLine;
  string humanPileLine;

  inFile.open( filePath.c_str() );

  if (!inFile) {
    cout << "File not found. Unable to open file";
    exit(1); // terminate with error
  }

  while (getline(inFile, oneline)) {
    stringstream line(oneline);

    string word;
    line >> word;

    if(word.find("Table") != string::npos){
      while(line >> word){
      
        if(word.length() == 1 && word == "[")
            // parse multibuild(arguments);
            parseMultiBuild(tableMultiBuilds, line, word);
        else if(word[0] == '[' && word.length() > 2)
            // parse singleBuild(arguments);
            parseSingleBuild(tableSingleBuilds, line, word);
        else
          //loose cards
          tableLooseCards.push_back(word);
      }

    }

    if( word.find("Build") != string::npos) {
      string ignoreThisWord;
      line >> ignoreThisWord;
      line >> word;

      if(word.length() == 1 && (word == "[" ) ){
        //Parse multi build(arguments);
        if(oneline.find("Human") != string::npos)// Means the build belongs to human
          parseMultiBuild(humansMultiBuilds, line, word);
        else
          parseMultiBuild(computersMultiBuilds, line, word);
      }
      else{
        // Parse single build(arguments);
        if(oneline.find("Human") != string::npos) // Means the build belongs to human
          parseSingleBuild(humansSingleBuilds, line, word);
        else
          parseSingleBuild(computersSingleBuilds, line, word);
      }
    }

    if(word.find("Capturer") != string::npos )
      line >> playerWhoCapturedLast;

    if(word.find("Round") != string::npos)
      line >> roundNumber;

    if(word.find("Computer") != string::npos) // PArse next 3 lines
      parsePlayerData(inFile, computerScoreLine, computerHandLine, computerPileLine, computerScore, computerHand, computerPile);

    if(word.find("Human") != string::npos) //Parsenext 3 lines
      parsePlayerData(inFile, humanScoreLine, humanHandLine, humanPileLine, humanScore, humanHand, humanPile);

    //Deck cards
    if(word.find("Deck") != string::npos){
      while (line >> word)
        deckCards.push_back(word);
    }

    if(word.find("Next") != string::npos){
      line >> word;
      line >> word;
      nextPlayer = word;
    }
  }

  inFile.close();
}
  
/* *********************************************************************
Function Name: parseMultiBuild
Purpose: To extract hand and pile related data from a file and add it to 
        the vectors representing the pile and the hand
Parameters:
        vector <vector <vector <string> > > & allMultiBuilds. Contains 0 or more vector of 
                    vector of string. Each vector of vector of string represents a multibuild.
                    Each vector of string represents a single build. Each string in the vector
                    represents a card. 6 of Hearts would be represeneted as H6
        string represents one multibuild
        line - a stringstream object passed by reference for efficiency
        word - a string object passed by reference. As the function reads
              from the line, content of word is changed
Return Value: None
Local variables:
        vector<vector <string> > multi - This vector represents a multi-build, which may have
                two or more vector of strings (build) inside it.
        wordLength - an int variable that stores the length of the string. If the length of a word
                is stored in a variable like this, you don't need to repeatedly call string.length
        previousWord - a string variable that stores the previous word read in a line. Say, a line
                looks like this: "I like OOP" If we are currently reading word OOP from the line,
                previousWord would store "like"
        card - a string variable that represents a card. 6 of hearts would be represented as H6    
Algorithm: Say, you have a line representing a multibuild which looks like this 
          [ [D5 D2] [H5 H2] ] .This function starts reading words in the line one by one starting 
          from [D5 . Any part of the word that is not '[' or ']' is added to the variable card. Then, 
          the card is added to the vector of string, singlBuild. This continues until we read the word
          whose last character is ] . Eg: word, D2]. Once we reach this point, we stop adding card to the
          singlBuild and add the singleBuild to multi. If there are more single builds, the function
          parses them. This process continues until we read a word, "]". This means that, we are stone 
          parsing a multiBuild. Then, the multiBuild we just parsed is added to a vector of vector of vector
          of string variable. This variable represents all multiBuilds in the tournament.
Assistance Received: none
********************************************************************* */  
void Tournament::parseMultiBuild(vector <vector <vector <string> > > & allMultiBuilds, stringstream & line, string & word){
  vector<vector <string> > multi;
  int wordLength;
  string previousWord;
  line >> word;

  do{
      // For each single build
      vector <string> singlBuild;

      do{
          wordLength = word.length();
          string card;

          for(int i = 0; i < wordLength; i++ ){
              if(word[i] != '[' && word[i] != ']' )
                card += word[i];
          }

          singlBuild.push_back(card);
          previousWord = word;

          line >> word;

      } while(previousWord[wordLength - 1] != ']' );

      multi.push_back(singlBuild);

  } while(!(word.length() == 1 && word == "]"));

  allMultiBuilds.push_back(multi); 
}

/* *********************************************************************
Function Name: parseSingleBuild
Purpose: To extract all possible single builds from a text file. And store each
          build as a vector of string where each string eement in the vector 
          will be a card. Eg: 6 of hearts would be represented as H6
Parameters:
        vector <vector <string> > & allSingleBuilds - This vector will contain
               a vector of string. Each vector of string represents a single build
        line - a stringstream object passed by reference for efficiency
        word - a string object passed by reference. As the function reads
              from the line, content of word is changed
Return Value: None
Assistance Received: none
********************************************************************* */
void Tournament::parseSingleBuild(vector <vector <string> > & allSingleBuilds, stringstream & line, string & word){
  vector<string> singlBuild;
  string previousWord;
  int wordLength;

  do{
      wordLength = word.length();
      string card;

      for(int i = 0; i < wordLength; i++ ){
        if(word[i] != '[' && word[i] != ']')
          card += word[i];
      }

      singlBuild.push_back(card);
      previousWord = word;

      if( ']' != previousWord[wordLength - 1] )
        line >> word;

  } while( ']' != previousWord[wordLength - 1] );

  allSingleBuilds.push_back(singlBuild);  
}

/* *********************************************************************
Function Name: parsePlayerData
Purpose: To extract hand and pile related data from a file and add it to 
        the vectors representing the pile and the hand
Parameters:
        inFile - ifstream object passed by reference. Contents of file not modified
        playerScoreLine - a string from which a player's score can be extracted 
        playerHandLine - a string from which a player's hand can be extracted
        playerPileLine - a string from which a player's pile can be extracted
        playerScore - an int, passed by reference. It's value is modified based
                    on score extracted from playerScoreLine
        playerHand - a vector of string, vector passed by reference. Content modified
                    based on hand extracted from playerHandLine
        playerPile - a vector of string, vector passed by reference. Content modified
                    based on pile extracted from playerHandLine
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Tournament::parsePlayerData(ifstream & inFile, string playerScoreLine, string playerHandLine, string playerPileLine, int & playerScore, vector <string> & playerHand, vector <string> & playerPile){
  getline(inFile, playerScoreLine);
  stringstream temp(playerScoreLine);
  string tempWord;
  temp >> tempWord >> playerScore;

  getline(inFile, playerHandLine);
  stringstream temp2(playerHandLine);
  temp2 >> tempWord;
  while(temp2 >> tempWord){
      playerHand.push_back(tempWord);
  }

  getline(inFile, playerPileLine);
  stringstream temp3(playerPileLine);
  temp3 >> tempWord;
  while(temp3 >> tempWord){
      playerPile.push_back(tempWord);
  }
}


/* *********************************************************************
Function Name: createPlayer
Purpose: To set up the member contents of a Player object
Parameters:
        player - a Player pointer variable. Used to set the player's pile, hand, single builds, multibuilds
        playerHand - a const vector of string passed by reference for efficiency. Represents player's hand
                    but in string version. Contents would be in the form of  HA, C6, D7 (suite character followed
                    by a face value)  
        playerPile - a const vector of string passed by reference for efficiency. Represents player's pile
                    but in string version. Contents would be in the form of  HA, C6, D7 (suite character followed
                    by a face value)  
        const vector <vector <string> > & playersSingleBuilds - contains multiple vectors which have stored card in string formats
        const vector <vector <vector <string> > > & playersMultiBuilds - contains multiple multiple vectors of string. Each multi multivector
                  represents a multi build. Each multiple vector of string represents a build.
                  Each string the vector represents a card in form like this, C6
        Table * table - reference to the table created in this torunament. This function adds builds to the table.
        string playerType - can be either human or computer
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Tournament::createPlayer(Player * player, const vector <string> & playerHand, const vector <string> & playerPile, const vector <vector <string> > & playersSingleBuilds, const vector <vector <vector <string> > > & playersMultiBuilds, Table * table, string playerType){
  vector <Card*> hand;
  vector <Card*> pile;
  vector <Build*> singleBuilds;
  vector <MultiBuild*> multiBuilds;
  createCardsFromStrings(playerHand, hand);
  createCardsFromStrings(playerPile, pile);
  createSingleBuilds(playersSingleBuilds, singleBuilds, playerType); 
  createMultiBuilds(playersMultiBuilds, multiBuilds, playerType); 

  player->setPile(pile);
  player->setHand(hand);
  player->setBuild(singleBuilds);
  player->setMultiBuild(multiBuilds);

  addBuildsToTable(singleBuilds, multiBuilds, table);
}


/* *********************************************************************
Function Name: addBuildsToTable
Purpose: To add single and multi builds to the table used in this tournament
Parameters:
        const vector <Build*> & singleBuilds 
        const vector <MultiBuild*> & multibuilds
        Table * table - a reference to the table used in this torunament. This functions adds single and multi
                      builds to the table used by the tournament
Return Value: None
Assistance Received: none
********************************************************************* */
void Tournament::addBuildsToTable(const vector <Build*> & singleBuilds, const vector <MultiBuild*> & multibuilds, Table * table){
  //Add single builds to table
  for(int i = 0; i < singleBuilds.size(); i++){
    table->addABuild(singleBuilds[i]);
  }

  //Add multi builds to table
  for(int i = 0; i < multibuilds.size(); i++){
    table->addAMultiBuild(multibuilds[i]);
  }
}

/* *********************************************************************
Function Name: addLooseCards
Purpose: This function goes through each individual element of a vector of string. Each element of
        the vector represents a card in string form. Eg: Ace of hearts woule be represented as HA.
        The function dynamically creates a card object from its string representation and adds 
        the reference to the card created to the table
Parameters:
        const vector <string> & tableLooseCards - Each element of the vector represents a card in 
                                                string form. Eg: Ace of hearts woule be represented as HA.
        Table * table - a reference to the table used in this tournament. This functions adds loose cards
                      to the table used in the tournament
Return Value: None
Assistance Received: none
********************************************************************* */
void Tournament::addLooseCards(const vector <string> & tableLooseCards, Table * table){
  for(int i = 0; i < tableLooseCards.size(); i++){
    char suite = tableLooseCards[i][0];
    char face = tableLooseCards[i][1];
    Card * newCard = new Card(suite, face);
    table->addALooseCard(newCard);
  }
}

/* *********************************************************************
Function Name: createDeck
Purpose: This function goes through each individual element of a vector of string. Each element of
        the vector represents a card in string form. Eg: Ace of hearts woule be represented as HA.
        The function dynamically creates a card object from its string representation and adds 
        the reference to a vector Card object 
Parameters:
        const vector <string> & deckCards - Each element of the vector represents a card in string form.
                                             Eg: Ace of hearts woule be represented as HA.
        vector <Card*> & cardsInADeck - Each element in this vector represents a Card object. The cards 
                                      can be used to populate a deck sed in the tournament
Return Value: None
Assistance Received: none
********************************************************************* */
void Tournament::createDeck(vector <Card*> & cardsInADeck, const vector <string> & deckCards){

  for(int i = 0; i < deckCards.size(); i++){
    char suite = deckCards[i][0];
    char face = deckCards[i][1];
    Card * card = new Card(suite, face);
    cardsInADeck.push_back(card);
  }
}

/* *********************************************************************
Function Name: printScores
Purpose: To print the scores of the players in the tournament
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Tournament::printScores(){
  cout << "\n\nIn this tournament, the human player has " << m_humanPlayer->getScore() << " points \n";
  cout << "In this tournament, the computer player has " << m_computerPlayer->getScore() << " points \n\n\n";
}

/* *********************************************************************
Function Name: gameOver
Purpose: To check if the tournament has ended
Parameters: None
Return Value: true if the tournament is over. false otherwise
Assistance Received: none
********************************************************************* */
bool Tournament::gameOver(){
  if( m_humanPlayer->getScore() >= THRESHOLD_MAX_SCORE || m_computerPlayer->getScore() >= THRESHOLD_MAX_SCORE )
    return true;
  return false;
}

/* *********************************************************************
Function Name: playTheTournamentWithoutSerialization
Purpose: To start and play a new round. The new round is independent of the 
          serialization file
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Tournament::playTheTournamentWithoutSerialization(){
  while(!gameOver() ){
    // Play a round
    Round newRound(m_humanPlayer->getScore(), m_computerPlayer->getScore());

    newRound.playARound();

    // After each round is over, update the player scores here
    m_humanPlayer->updateScore(newRound.getHuman()->getScore());
    m_computerPlayer->updateScore(newRound.getComputer()->getScore());
    printScores();
  }

  // Determine the winner
  cout << determineTheWinner() << endl;
}

/* *********************************************************************
Function Name: createCardsFromStrings
Purpose: To go through a vector of strings and populate a vector of Card objects
Parameters: 
          const vector <string> & stringOfCards - Each element of the vector represents 
                      a card in string form. Eg: Ace of hearts woule be represented as HA.
          vector<Card*> & newCards - Each element in this vector represents a Card object. 
Return Value: None
Assistance Received: none
********************************************************************* */
void Tournament::createCardsFromStrings(const vector <string> & stringOfCards, vector<Card*> & newCards){
  char possibleSuite,
    possibleFace;
  for(int i = 0; i < stringOfCards.size(); i++){
    possibleSuite = stringOfCards[i][0];
    possibleFace = stringOfCards[i][1];
    Card * newCard = new Card(possibleSuite, possibleFace);
    newCards.push_back(newCard);
  }
}

/* *********************************************************************
Function Name: createSingleBuilds
Purpose: To go through a vector of vector of string and populate a vector of Build objects
Parameters: 
          const vector <vector <string> > & stringOfSingleBuilds - vector of vector of string objects
                    from which a Build objects are created 
          singleBuilds - a vector of build objects. Passed by reference. Dynamically
                        created build objects are added to the vector
          playerType - const string object passed by reference for efficiency reasons.
                      Value can be either "human" or "player"
Return Value: None
Assistance Received: none
********************************************************************* */
void Tournament::createSingleBuilds(const vector <vector <string> > & stringOfSingleBuilds, vector <Build*> & singleBuilds, const string & playerType){
  // Build(vector<Card *> buildCards, string owner, int value)
  for(int i = 0; i < stringOfSingleBuilds.size(); i++){
    vector <Card*> cards;
    createCardsFromStrings(stringOfSingleBuilds[i], cards);
    int buildValue = getNumericValueOfAGroupOfCards(cards);
    Build * newSingleBuild = new Build(cards, playerType, buildValue);
    singleBuilds.push_back(newSingleBuild);
  }
}

/* *********************************************************************
Function Name: getNumericValueOfAGroupOfCards
Purpose: To go through a vector of card objects, compute numeric value of each card object, add 
        the numeric value of each card object and return the sum
Parameters: 
          cards - vector of Card objects
          
Return Value: value - an int value that is a summation of numeric values of all cards
                      in the vector of cards
Assistance Received: none
********************************************************************* */
int Tournament::getNumericValueOfAGroupOfCards(vector<Card *> cards){
  int value = 0;

  for(int i = 0; i < cards.size(); i++){
    Card * card = cards[i];
    value += (card->getNumericValue());
  }

  return value;
}

/* *********************************************************************
Function Name: getNumericValueOfAGroupOfCards
Purpose: To go through a vector of card objects, compute numeric value of each card object, add 
        the numeric value of each card object and return the sum
Parameters: 
          cards - vector of Card objects
          
Return Value: value - an int value that is a summation of numeric values of all cards
                      in the vector of cards
Assistance Received: none
********************************************************************* */
void Tournament::createMultiBuilds(const vector <vector <vector <string> > > & stringOfplayersMultiBuilds, vector <MultiBuild*> & multiBuilds, const string & playerType){
  // MultiBuild(vector<Build*> multiBuilds, string owner)
  for(int i = 0; i < stringOfplayersMultiBuilds.size(); i++){
    vector <Build*> groupOfBuilds;
    createSingleBuilds(stringOfplayersMultiBuilds[i], groupOfBuilds, playerType);
    MultiBuild * newMultiBuild = new MultiBuild(groupOfBuilds, playerType);
    multiBuilds.push_back(newMultiBuild);
  }
}

/* *********************************************************************
Function Name: determineTheWinner
Purpose: To check the scores of both players. Find out which player has more score
          and display the winner as the player with the higher score
Parameters: None          
Return Value: string indicating the result of the game
Assistance Received: none
********************************************************************* */
string Tournament::determineTheWinner(){
  cout << "\n---------------------GAME OVER-------------------------\n";
  if(m_humanPlayer->getScore() == m_computerPlayer->getScore() )
    return "\nThe tournament ended in a tie\n";
  if(m_humanPlayer->getScore() > m_computerPlayer->getScore())
    return "\nThe human player won the tournament\n";

  return "\nThe computer player won the tournament \n";
}