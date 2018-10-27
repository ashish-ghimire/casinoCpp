#include "Player.h"

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/

/* *********************************************************************
Function Name: Player
Purpose: Default constructor. To define initial values for all member variables
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
Player::Player(){
  m_score = 0;

  // You don't need to clear the variables below as they are vectors
  // and all vectors are empty when the object is created. However, the
  // rubric said to initialize all member variables. So, I am initializing 
  // the member variables below to make sure the professor does not take
  // points off for not following the guidelines in the rubric.
  m_hand.clear();
  m_pile.clear();
  m_build.clear();
  m_multiBuild.clear();
}

/* *********************************************************************
Function Name: Player
Purpose: Custom constructor. To define initial values for all member variables
Parameters: 
          pile - a vector of Card * objects passed by value.
          hand - a vector of Card* objects passed by value
          multibuild - a vector of MultiBuild* objects passed by value.
          pts- an integer variable that represents the no. of points
                the player object should have when ititialized
Return Value: None
Assistance Received: none
********************************************************************* */ 
Player::Player(vector<Card*> pile, vector <Card*> hand, vector<Build*> build, vector <MultiBuild*> multibuild, int pts){
  m_pile = pile, 
  m_hand = hand;
  m_build = build;
  m_multiBuild = multibuild;
  m_score = pts;
}

/* *********************************************************************
Function Name: updateScore
Purpose: A mutator used to update the score a player object has 
Parameters: 
         pts - an integer variable passed by value. Represents the number of
              points that should be added to a player's score
Return Value: true if pts is a legitimate value. false otherwise
Assistance Received: none
********************************************************************* */ 
bool Player::updateScore(int pts){
  if(pts > 50)
    return false;

  m_score += pts;
  return true;
}

/* *********************************************************************
Function Name: addToPile
Purpose: A mutator used to add a card reference to the player's pile
Parameters: 
         c- a reference to a card object
Return Value: true if card to be added to the pile is not null. false otherwise
Assistance Received: none
********************************************************************* */ 
bool Player::addToPile(Card * c){
  if(c == NULL)
    return false; 

  m_pile.push_back(c);
  return true;
}

/* *********************************************************************
Function Name: addToPile
Purpose: A mutator used to add all cards in a build to the player's pile
Parameters: 
         b- a reference to a Build object
Return Value: true if the parameter is not legitimate(not NULL). false
              otherwise
Assistance Received: none
********************************************************************* */ 
bool Player::addToPile(Build *b){
  if(b == NULL)
    return false;

  vector <Card*> cardsInTheSelectedBuild = b->getBuildCards();
  for(int i = 0; i < cardsInTheSelectedBuild.size(); i++ ){
    addToPile(cardsInTheSelectedBuild[i]);
  }
  return true;
}

/* *********************************************************************
Function Name: addToPile
Purpose: A mutator used to add all cards in a multibuild to the player's pile
Parameters: 
         m- a reference to a MultiBuild object
Return Value: true if the parameter is not legitimate(not NULL). false
              otherwise
Assistance Received: none
********************************************************************* */ 
bool Player::addToPile(MultiBuild *m){
  if( m == NULL)
    return false;

  vector <Build*> allBuilds = m->getMultiBuild();
  for(int i = 0; i < allBuilds.size(); i++ )
    addToPile(allBuilds[i] );

  return true;
}

/* *********************************************************************
Function Name: addToHand
Purpose: A mutator used to add a card to the player's hand
Parameters: 
         c- a reference to a Card object
Return Value: true if more cards can still be added to the hand. false
          otherwise
Assistance Received: none
********************************************************************* */ 
bool Player::addToHand(Card * c){
  if( m_hand.size() >= MAX_CARDS_IN_A_HAND )
    return false;

  m_hand.push_back(c);
  return true;
}

/* *********************************************************************
Function Name: addABuild
Purpose: A mutator used to add a build to the player's vector of builds
Parameters: 
         b- a reference to a Build object
Return Value: true if the parameter is not NULL. false otherwise
Assistance Received: none
********************************************************************* */ 
bool Player::addABuild(Build *b){
  if(b == NULL)
    return false;

  m_build.push_back(b);
  // When creating a build, there should be a card in your hand whose numeric
  // value equals the sum of numeric values of cards in a build. I call the 
  // card to hold in your hand, the hold card. The function below configures
  // hold cards for a build
  setHoldCardsForSingleBuild( b->getNumericValue() );
  return true;
}

/* *********************************************************************
Function Name: setHoldCardsForSingleBuild
Purpose:  When creating a build, there should be a card in your hand whose numeric
          value equals the sum of numeric values of cards in a build. I call the 
          card to hold in your hand, the hold card. The function below configures
          hold cards for a single build
Parameters: 
         buildSum- an integer passed by value. It determines what should the value of 
                  the hold card be
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Player::setHoldCardsForSingleBuild(int buildSum){
  for(int i = 0; i < m_hand.size(); i++){
    if(m_hand[i]->getNumericValue() == buildSum){
      m_holdCardsForSingleBuilds.insert(pair <int, Card*> (buildSum, m_hand[i]));
    }
  }
}

/* *********************************************************************
Function Name: addAMultiBuild
Purpose: A mutator used to add a multibuild to the player's vector of multibuilds
Parameters: 
         m- a reference to a MultiBuild object
Return Value: true if the parameter is not NULL. false otherwise
Assistance Received: none
********************************************************************* */ 
bool Player::addAMultiBuild(MultiBuild *m){
  if(m == NULL)
    return false;

  m_multiBuild.push_back(m);
  setHoldCardsForMultiBuild( m->getNumericValue() );
  return true;
}

/* *********************************************************************
Function Name: setHoldCardsForMultiBuild
Purpose:  When creating a build, there should be a card in your hand whose numeric
          value equals the sum of numeric values of cards in a build. I call the 
          card to hold in your hand, the hold card. The function below configures
          hold cards for a multi build
Parameters: 
         multiBuildSum- an integer passed by value. It determines what should the value of 
                  the hold card be
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Player::setHoldCardsForMultiBuild(int multiBuildSum){
  for(int i = 0; i < m_hand.size(); i++){
    if(m_hand[i]->getNumericValue() == multiBuildSum){
      m_holdCardsForMultiBuilds.insert(pair <int, Card*> (multiBuildSum, m_hand[i]));
    }
  }
}

/* *********************************************************************
Function Name: removeFromHand
Purpose:: A mutator that removes a reference to a hand card 
Parameters: 
        looseCardIndex - an integer variable that should be a vector index of
                one of the hand cards the player has (cards in m_hand)
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::removeFromHand(int handIndex){
  if(handIndex < 0 || handIndex > m_hand.size()) {
    cout << "Invalid index \n";
    return;
  }
  m_hand.erase(m_hand.begin() + handIndex);
}

/* *********************************************************************
Function Name: removeBuild
Purpose:: A mutator that removes a reference to a build that the player has
Parameters: 
        index - an integer variable that should be a vector index of
                one of the builds the player has (builds in m_build)
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::removeBuild(int index){
  if(index < 0 || index > m_build.size()) {
    cout << "Invalid index \n";
    return;
  }
  Build * buildToRemove = m_build[index];
  m_holdCardsForSingleBuilds.erase( buildToRemove->getNumericValue() ); // Remove all map elements that match the build's numeric value 

  m_build.erase(m_build.begin() + index);
}

/* *********************************************************************
Function Name: removeBuild
Purpose:: A mutator that removes a reference to a build that the player has
Parameters: 
        build - a pointer to a build variable that the player has
                (builds in m_build)
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::removeBuild(Build *build){
  m_holdCardsForSingleBuilds.erase( build->getNumericValue() );
  m_build.erase(remove(m_build.begin(), m_build.end(), build), m_build.end());
}

/* *********************************************************************
Function Name: removeMultiBuild
Purpose:: A mutator that removes a reference to a multibuild that the player has
Parameters: 
        index - an integer variable that should be a vector index of
                one of the multibuilds the player has (multibuilds in m_multiBuild)
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::removeMultiBuild(int index){
  if(index < 0 || index > m_multiBuild.size()) {
    cout << "Invalid index \n";
    return;
  }
  m_holdCardsForMultiBuilds.erase(m_multiBuild[index]->getNumericValue() );
  m_multiBuild.erase(m_multiBuild.begin() + index);
}


/* *********************************************************************
Function Name: removeMultiBuild
Purpose:: A mutator that removes a reference to a multibuild that the player has
Parameters: 
        multibuild - a pointer to a multibuild variable that the player has
                (multibuuilds in m_multiBuild)
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::removeMultiBuild(MultiBuild *multiBuild){
  vector<MultiBuild*>::iterator position = find(m_multiBuild.begin(), m_multiBuild.end(), multiBuild);

  if( position != m_multiBuild.end()) // == m_multiBuild.end() means the element was not found
  {
    m_holdCardsForMultiBuilds.erase(multiBuild->getNumericValue());
    m_multiBuild.erase(position);
  }
}

/* *********************************************************************
Function Name: removeHoldCard
Purpose:: To remove a hold card associated with a single or a multi build. 
          When creating a build, there should be a card in your hand whose numeric
          value equals the sum of numeric values of cards in a build. I call the 
          card to hold in your hand, the hold card.
Parameters: 
        cardFromHandIndex - an integer passed by value that should be a vector index
                of one of the hand cards the player has (cards in m_hand)
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::removeHoldCard(int cardFromHandIndex){
  Card * card = m_hand[cardFromHandIndex];  
  int holdValue = card->getNumericValue();

  multimap <int, Card*> :: iterator itr;
  for(itr = m_holdCardsForMultiBuilds.begin(); itr != m_holdCardsForMultiBuilds.end(); itr++){
    if(itr->first == holdValue && itr->second == card){
      // erase(iterator position) – Removes the element at the position pointed by the iterator
      m_holdCardsForMultiBuilds.erase(itr); 
      break;
    }
  }

  multimap <int, Card*> :: iterator itr2;
  for(itr = m_holdCardsForSingleBuilds.begin(); itr != m_holdCardsForSingleBuilds.end(); itr++){
    if(itr->first == holdValue && itr->second == card){
      // erase(iterator position) – Removes the element at the position pointed by the iterator
      m_holdCardsForSingleBuilds.erase(itr); 
      break;
    }
  }  
}

/* *********************************************************************
Function Name: printHand
Purpose:  To print the cards in a player's hand in a user-friendly format.
          If the hand has six of Hearts, 7 of Diamods and 3 of Clubs.
          The hand would be printed as
          H6 D7 C3

Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Player::printHand(){
  for(int i = 0; i < m_hand.size(); i++){
    m_hand[i]->printCard();
  }
  cout << endl;
}

/* *********************************************************************
Function Name: printHandIndexed
Purpose:  To print the cards in a player's hand along with each card's indices.
          If the hand has six of Hearts, 7 of Diamods and 3 of Clubs.
          The hand would be printed as
          H6 (0) D7 (1) C3 (2)

Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Player::printHandIndexed(){
  for(int i = 0; i < m_hand.size(); i++){
    m_hand[i]->printCard();
    cout << "(" << i << ") ";
  }
  cout << endl;
}


/* *********************************************************************
Function Name: printSingleBuildsIndexed
Purpose:: Print the single builds in this format  [D5 D7] (0) [HA HX] (1)
            Valid indices can be from 0 to (number of player's single builds) - 1
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::printSingleBuildsIndexed(){
  for(int i = 0; i < m_build.size(); i++){
    m_build[i]->printBuild();
    cout << "( " << i << " )";
  }
}

/* *********************************************************************
Function Name: :printMultiBuildsIndexed
Purpose:: Print the multi builds in this format  [ [C6 S3] [S9] ] (0) [ [HA HX] [D7 S4] ] (1)
            Valid indices can be from 0 to (number of player's multi builds) - 1
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::printMultiBuildsIndexed(){
    for(int i = 0; i < m_multiBuild.size(); i++){
        m_multiBuild[i]->printMultiBuild();
        cout << "( " << i << " )";
    }
}

/* *********************************************************************
Function Name: printPile
Purpose:  To print the cards in a pile in a user-friendly format. If the pile
          has six of Hearts, 7 of Diamods and 3 of Clubs. The pile would be 
          printed as H6 D7 C3 
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Player::printPile(){
  for(int i = 0; i < m_pile.size(); i++){
    m_pile[i]->printCard();
  }
  cout << endl;
}

/* *********************************************************************
Function Name: isHoldCard
Purpose::  
          When creating a build, there should be a card in your hand whose numeric
          value equals the sum of numeric values of cards in a build. I call the 
          card to hold in your hand, the hold card. The function below checks if 
          a card is a hold card
Parameters: 
        cardFromHandIndex - an integer passed by value that should be a vector index
                of one of the hand cards the player has (cards in m_hand)
Return Value: true if the index specified by the parameter belongs to a hold card
              false otherwise
Assistance Received: none
********************************************************************* */
bool Player::isHoldCard(int cardFromHandIndex){
  if( isHoldCardForSingleBuild(cardFromHandIndex) || isHoldCardForMultiBuild(cardFromHandIndex) )
    return true;
  
  return false;
}

/* *********************************************************************
Function Name: isHoldCardForSingleBuild
Purpose::  
          When creating a build, there should be a card in your hand whose numeric
          value equals the sum of numeric values of cards in a build. I call the 
          card to hold in your hand, the hold card. The function below checks if 
          a card is a hold card for any single builds the player has
Parameters: 
        cardFromHandIndex - an integer passed by value that should be a vector index
                of one of the hand cards the player has (cards in m_hand)
Return Value: true if the index specified by the parameter belongs to a hold card
              associated with a single build. false otherwise
Assistance Received: none
********************************************************************* */
bool Player::isHoldCardForSingleBuild(int cardFromHandIndex){
  Card * card = m_hand[cardFromHandIndex];
  // getNumericValue(true) indicates the value of an Ace card should be 14
  int numHoldCardsSingleBuild = m_holdCardsForSingleBuilds.count(card->getNumericValue(true)); 

  if( 1 == numHoldCardsSingleBuild )
    return true;
  
  return false;
}

/* *********************************************************************
Function Name: isHoldCardForMultiBuild
Purpose::  
          When creating a build, there should be a card in your hand whose numeric
          value equals the sum of numeric values of cards in a build. I call the 
          card to hold in your hand, the hold card. The function below checks if 
          a card is a hold card for any multi builds the player has
Parameters: 
        cardFromHandIndex - an integer passed by value that should be a vector index
                of one of the hand cards the player has (cards in m_hand)
Return Value: true if the index specified by the parameter belongs to a hold card
              associated with a multi build. false otherwise
Assistance Received: none
********************************************************************* */
bool Player::isHoldCardForMultiBuild(int cardFromHandIndex){
  Card * card = m_hand[cardFromHandIndex];
  // getNumericValue(true) indicates that the value of Ace should be 14
  int numHoldCardsMultiBuild = m_holdCardsForMultiBuilds.count( card->getNumericValue(true) );

  if(1 == numHoldCardsMultiBuild)
    return true;
  
  return false;
}

/* *********************************************************************
Function Name: writeHandToFile
Purpose:  To write the cards in a hand to a file. If the hand
          has six of Hearts, 7 of Diamods and 3 of Clubs. The hand would be 
          written as 
          H6 D7 C3

Parameters: gameFile- an ofstream object passed by reference. It's contents
            are modofied with the contents from hand written in format 
            described in Purpose
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Player::writeHandToFile(std::ofstream &gameFile){
  for(int i = 0; i < m_hand.size(); i++){
    m_hand[i]->writeCardToFile(gameFile);
  }
  gameFile << endl;
}

/* *********************************************************************
Function Name: writePileToFile
Purpose:  To write the cards in a pile to a file. If the pile
          has six of Hearts, 7 of Diamods and 3 of Clubs. The pile would be 
          written as 
          H6 D7 C3

Parameters: gameFile- an ofstream object passed by reference. It's contents
            are modofied with the contents from pile written in format 
            described in Purpose
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Player::writePileToFile(ofstream & gameFile){
  for(int i = 0; i < m_pile.size(); i++){
    m_pile[i]->writeCardToFile(gameFile);
  }
  gameFile << endl;
}

/* *********************************************************************
Function Name: handHasACardWithAGivenNumericValue
Purpose:: To check if the player's hand contains a card with a numeric value
          specified by the parameter in this function
Parameters: 
        indexToAvoid - an integer value that tells the function to not look at this
                      index
        requiredNumericValue - An integer variable that specifies the build's
                    numeric value to look for.
        aceIsFourteen - A boolean value that is used to determine the value of
                      an Ace card. If aceIsFourteen is false, the value of
                      an Ace card is 1. Else it is 14.
Return Value: true if there is a card in player's hand whose numeric value 
Assistance Received: none
********************************************************************* */
bool Player::handHasACardWithAGivenNumericValue(int indexToAvoid, int requiredNumericValue, bool aceIsFourteen){
  for(int i = 0; i < m_hand.size(); i++){        
        if(i != indexToAvoid){
            if( m_hand[i]->getNumericValue(aceIsFourteen) == requiredNumericValue)
                return true;
        }
  }    

  return false;
}

/* *********************************************************************
Function Name: getBuildIndex
Purpose::  To check if the player has a build with a numeric value
          specified by the parameter in this function
Parameters: 
        numericValue - an integer passed by value. Specifies the build's
                    numeric value to look for.
Return Value: integer. Returns the index of the build in m_build vector if
            the build vector contains a build with numeric value equal to the
            parameter's value. Returns INVALID_INDEX otherwise
Assistance Received: none
********************************************************************* */
int Player::getBuildIndex(int numericValue){
  for(int i = 0; i < m_build.size(); i++){
    if(m_build[i]->getNumericValue() == numericValue)
      return i;
  }
  return INVALID_INDEX;
}

/* *********************************************************************
Function Name: getMultiBuildIndex
Purpose::  To check if the player has a multi build with a numeric value
          specified by the parameter in this function
Parameters: 
        numericValue - an integer passed by value. Specifies the multibuild's
                    numeric value to look for.
Return Value: integer. Returns the index of the build in m_multiBuild vector if
            the multiBuild vector contains a multibuild with numeric value equal
            to the parameter's value. Returns INVALID_INDEX otherwise
Assistance Received: none
********************************************************************* */
int Player::getMultiBuildIndex(int numericValue){
  for(int i = 0; i < m_multiBuild.size(); i++){
    if(m_multiBuild[i]->getNumericValue() == numericValue )
      return i;
  }
  return INVALID_INDEX;
}

bool Player::hasAGivenSingleBuild(Build *build){
  vector<Build*>::iterator position = find(m_build.begin(), m_build.end(), build);
  if(position == m_build.end() ) //Not found
    return false;
  return true;
}

bool Player::hasAGivenMultiBuild(MultiBuild *multibuild){
  vector<MultiBuild*>::iterator position = find(m_multiBuild.begin(), m_multiBuild.end(), multibuild);
  if(position == m_multiBuild.end() ) //Not found
    return false;
  return true;  
}
