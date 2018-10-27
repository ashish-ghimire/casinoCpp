#pragma once

#include "Card.h"
#include <queue>
#include <fstream>
#include <algorithm>

class Deck{
public:
    // Constructors
    Deck();
    Deck(vector <Card*> deckCards);

    // Destructor
    ~Deck();       

    bool addNextCard(Card * cardToAdd); //Mutator
    Card * getNextCard(); //Accessor. Can only be used to access one card at a time 

    void printDeckCards();
    void writeDeckCardsToFile(ofstream & gameFile);
    bool isEmpty();

private:
    vector <Card*> m_cardsInDeck;
    int m_nextCardIndex;  

    void populateDeck(bool loadFromFile, const char * filePath);
    void shuffleTheDeck();
};