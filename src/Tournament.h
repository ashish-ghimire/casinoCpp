#include "Round.h"
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <sstream>

class Tournament{
public:
  Tournament();
  ~Tournament();
  
  void startGame();

private:
  static const int THRESHOLD_MAX_SCORE = 21; 

  //Member variables
  Computer * m_computerPlayer;
  Human * m_humanPlayer;

  //Utility functions
  void loadTheTournament();
  void loadSerializationFile(string filePath, int & roundNumber, int & computerScore, int & humanScore, string & nextPlayer, vector<string> & humanHand, vector<string> & humanPile, vector <string> & computerHand, vector <string> & computerPile, vector <vector <string> > & computersSingleBuilds, vector < vector <vector <string> > > & computersMultiBuilds, vector <vector <string> > & humansSingleBuilds, vector <vector <vector <string> > > & humansMultiBuilds, vector <vector <string> > & tableSingleBuilds, vector <vector <vector <string> > > & tableMultiBuilds, vector <string> & tableLooseCards, vector <string> & deckCards, string & playerWhoCapturedLast);

  void parseMultiBuild(vector <vector <vector <string> > > & allMultiBuilds, stringstream & line, string & word);
  void parseSingleBuild(vector <vector <string> > & allSingleBuilds, stringstream & line, string & word);
  void parsePlayerData(ifstream & inFile, string playerScoreLine, string playerHandLine, string playerPileLine, int & playerScore, vector <string> & playerHand, vector <string> & playerPile);

  void createPlayer(Player * player, const vector <string> & playerHand, const vector <string> & playerPile, const vector <vector <string> > & playersSingleBuilds, const vector <vector <vector <string> > > & playersMultiBuilds, Table * table, string playerType);
  
  void createCardsFromStrings(const vector <string> & stringOfCards, vector<Card*> & newCards);
  void createSingleBuilds(const vector <vector <string> > & stringOfSingleBuilds, vector <Build*> & singleBuilds, const string & playerType);
  int getNumericValueOfAGroupOfCards(vector <Card*> cards);
  void createMultiBuilds(const vector <vector <vector <string> > > & stringOfplayersMultiBuilds, vector <MultiBuild*> & multiBuilds, const string & playerType); 
  void addBuildsToTable(const vector <Build*> & singleBuilds, const vector <MultiBuild*> & multibuilds, Table * table);

  void addLooseCards(const vector <string> & tableLooseCards, Table * table);
  void createDeck(vector <Card*> & cardsInADeck, const vector <string> & deckCards);
  void printScores();
  bool gameOver();

  void playTheTournamentWithoutSerialization();
  string determineTheWinner();
};