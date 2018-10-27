#pragma once

#include "Card.h"

class Build{
public:
    // Constructors
    Build();
    Build(vector <Card*> buildCards, string owner, int value);
    
    // Selectors
    inline string getOwner() const { return m_owner; }
    inline vector <Card*> getBuildCards() const { return m_cardsInABuild; }
    inline int getBuildSize() { return m_cardsInABuild.size(); }
    inline int getNumericValue() const { return m_numericValue; } ; //A build's numeric value is the sum of numeric value of all cards in the build

    // Mutators
    void addCardToABuild(Card *aCard);
    void setOwner(string ownerName);

    // Utility functions
    void printBuild();
    void writeBuildToFile(ofstream & gameFile);

private:
    vector <Card*> m_cardsInABuild;
    string m_owner;   // A build must be owned by either a computer or a human player
                        // If the build is owned by a human player, owner will be "human"
                        // Else, owner will be "computer"
    int m_numericValue;
};