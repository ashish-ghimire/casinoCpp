#pragma once

#include "Card.h"
#include "Build.h"
#include "MultiBuild.h"
#include <algorithm>
#include "Table.h"
#include "Strategy.h"

class Player{
public:
  Player();
  Player(vector<Card*> pile, vector <Card*> hand, vector<Build*> build, vector <MultiBuild*> multibuild, int pts);

  virtual void displayGameMenu() = 0; // Game Menu will be different for computer and humnan player
  virtual int getGameMenuChoice() = 0; // Because game menu will be different, menu choices will be different for computer and human player
  
  // Selectors
  inline int getScore() const{ return m_score; }
  inline vector <Card*> getPile() const { return m_pile; }
  inline vector <Card*> getHand() const { return m_hand; }
  inline vector <Build*> getBuild() const{ return m_build; }
  inline vector <MultiBuild*> getMultiBuild() const{ return m_multiBuild; }
  
  // Mutators 
  bool updateScore(int pts);
  inline void setPile(vector <Card*> pile) { m_pile = pile; }
  inline void setHand(vector <Card*> hand) { m_hand = hand; }
  inline void setScore(int pts) { m_score = pts; }
  inline void setBuild(vector <Build*> build ) { m_build = build; }
  inline void setMultiBuild(vector <MultiBuild*> multi) { m_multiBuild = multi; }
  bool addToHand(Card * c);
  bool addToPile(Card * c);
  bool addToPile(Build * b);
  bool addToPile(MultiBuild * m);
  bool addABuild(Build * b);
  bool addAMultiBuild(MultiBuild * m);
  void removeFromHand(int handIndex);
  void removeBuild(int index);
  void removeBuild(Build * build);
  void removeMultiBuild(int index);
  void removeMultiBuild(MultiBuild * multiBuild);
  void removeHoldCard(int cardFromHandIndex);

  virtual string getPlayerType() const = 0; // This function returns a string that is either "computer" or "human"
  virtual const int menuChoiceHelp() = 0;
  virtual const int menuChoiceQuitGame() = 0;
  virtual int getMoveChoice(Strategy * moveStrategy) = 0;
  virtual int getACardFromThePlayersHand(Strategy * moveStrategy) = 0;
  virtual int getCaptureChoice(Strategy * moveStrategy) = 0;
  virtual vector<int> getSingleBuildToCapture(Strategy * moveStrategy) = 0;
  virtual vector<int> getMultiBuildToCapture(Strategy * moveStrategy) = 0;
  virtual int getBuildOption(Strategy * moveStrategy) = 0;
  virtual int getAnOpponentsBuildIndex(Strategy * moveStrategy) = 0;
  virtual vector <int> selectLooseCardsFromTable(Strategy * moveStrategy) = 0;
  virtual void moveSummary(Strategy * moveStrategy) = 0;

  void printHand();
  void printHandIndexed();
  void printSingleBuildsIndexed();
  void printMultiBuildsIndexed();
  void printPile();
  bool isHoldCard(int cardFromHandIndex);
  
  void writeHandToFile(ofstream & gameFile);
  void writePileToFile(ofstream & gameFile);

  //A player has a build if his vector of build pointers is not empty
  inline bool hasSingleBuild(){ return !m_build.empty(); } 

  //A player has a mutli build if his vector of multibuild pointers is not empty
  inline bool hasMultiBuild(){ return !m_multiBuild.empty(); }

  bool handHasACardWithAGivenNumericValue(int indexToAvoid, int requiredNumericValue, bool aceIsFourteen = false);
  int getBuildIndex(int numericValue);
  int getMultiBuildIndex(int numericValue);
  bool hasAGivenSingleBuild(Build * build);
  bool hasAGivenMultiBuild(MultiBuild * multibuild);


  inline int getInvalidIndex() const { return INVALID_INDEX; } 

protected:
  static const int MAX_CARDS_IN_A_HAND = 4;
  static const int INVALID_INDEX = 5000;

  int m_score;
  vector <Card*> m_pile; 
  vector <Card*> m_hand;
  vector <Build*> m_build;
  vector <MultiBuild *> m_multiBuild;

  // When creating a build, there should be a card in your hand whose numeric
  // value equals the sum of numeric values of cards in a build. I call the 
  // card to hold in your hand, the hold card. The multimap below maps 
  // the sum of a build to one or more Cards whose numeric value equals to the sum (hold cards)
  multimap<int, Card*> m_holdCardsForSingleBuilds;

  // The multimap below maps the sum of a build to one or more Cards whose
  // numeric value equals to the sum. Cards whose numeric value equals to the
  // sum are called hold cards
  multimap<int, Card*> m_holdCardsForMultiBuilds;

private:
  void setHoldCardsForSingleBuild(int buildSum);
  void setHoldCardsForMultiBuild(int multiBuildSum);
  bool isHoldCardForSingleBuild(int cardFromHandIndex);
  bool isHoldCardForMultiBuild(int cardFromHandIndex);
};