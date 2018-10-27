#pragma once

#include "Deck.h"
#include "Table.h"
#include "Computer.h"
#include "Human.h"
#include <algorithm>
#include "Strategy.h"


/* What does a round have?
    - Deck of cards
    - Table
    - A function to figure out if the round has ended
*/

class Round{
public:
  Round(int humanTournamentScore, int computerTournamentScore);
  Round(Deck * deck, Table * table, Player *capturedLast, Computer *compPlayer, Human *humanPlayer, int round_count, string nextPlayer, int humanTournamentScore, int computerTournamentScore);

  void playARound(bool serialized = false);  

  // Selectors
  inline Human * getHuman() const { return m_humanPlayer; }
  inline Computer * getComputer() const { return m_computerPlayer; }

private:
  Deck * m_deckForTheRound;
  Table * m_table;
  Player * m_playerWhoCapturedLast;
  Computer * m_computerPlayer;
  Human * m_humanPlayer;
  int m_humanTournamentScore;
  int m_computerTournamentScore;
  static int roundCount;  
  static bool m_turn; //If turn is true, it is human player's turn. If false, it is computer player's turn
  
  void setTurn(string whoseTurn);
  void setFirstPlayer(bool serialized=false);
  void toss();
  int getHumanPlayersTossChoice();
  void printWhoPlaysFirst();
  void setTheRoundUp();
  void dealCards(Player * playerToDealCardTo, int numCardsToDeal);
  void playATurn(Player * aPlayer);
  Player * getOpponentPlayer(Player * aPlayer);
  void displayGameStatus(Player * aPlayer);
  void saveGame();
  void quitGame();
  void makeAMove(Player * aPlayer, Strategy * moveStrategy);
  bool build(Player * aPlayer, Strategy * moveStrategy);
  bool createSingleBuild(Player * aPlayer, Strategy * moveStrategy);
  bool checkForHoldCard(int cardFromHandIndex, Player * aPlayer);
  bool verifySingleBuild(vector <int> & indexOfLooseCards, int cardFromHandIndex, Player * aPlayer);
  int sum(vector <int> a_vector);
  bool removeTableCards(vector <int> & setOfCards);
  Build * getNewlyCreatedSingleBuild(vector<int> & indexOfLooseCards, int cardFromHandIndex, Player *aPlayer);
  void successfulMultiBuild(Player * aPlayer, int cardFromHandIndex, int oldBuildIndex, vector <int> & indexOfLooseCards, Build * newBuild);
  bool successfulSingleBuild( vector <int> & indexOfLooseCards, int cardFromHandIndex, Player * aPlayer );
  bool createMultiBuild(Player * aPlayer, Strategy * moveStrategy);
  int getAPlayersBuild(Player * aPlayer);

  int getALooseCard(Player * aPlayer, vector <int> & indexOfLooseCards);
  bool validatePotentialLooseCard(int potentialLooseCard, Player * aPlayer, vector <int> & indexOfLooseCards);
  bool increaseBuildValue(Player *aPlayer, Strategy * moveStrategy);
  bool capture(Player * aPlayer, Strategy * moveStrategy);
  bool captureSetOfCards(Player * aPlayer, int cardFromHandIndex, Strategy * moveStrategy);
  void printCaptureMessage(vector <int> tableCardsThatMustBeCaptured);
  bool captureSingleBuild(Player * aPlayer, int cardFromHandIndex, Strategy * moveStrategy);
  bool captureMultiBuild(Player * aPlayer, int cardFromHandIndex, Strategy * moveStrategy);
  bool captureIndividualCards(Player * aPlayer, int cardFromHandIndex);
  bool trail(Player * trail,  Strategy * moveStrategy);
  bool hasIndividualCards(Player * aPlayer, int cardFromHandIndex);
  void printHandAndPile();
  void continueTheRound();
  bool roundOver();
  void endRound();
  void updateScore();
  void updateScoreBasedOnTheBiggestPile();
  void updateScoreBasedOnMostSpades();
  void updateScoreBasedOnTenOfDiamonds();
  void updatePointsBasedOnACard(char suite, char face, int pts, Player * aPlayer);
  void updateScoreBasedOnTwoOfSpades();
  void updateScoreBasedOnAces();
  void determineTheWinner();
  
};
