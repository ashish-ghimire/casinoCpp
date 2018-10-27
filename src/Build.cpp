#include "Build.h"

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/

/* *********************************************************************
Function Name: Build
Purpose: Default constructor. To define initial values for all member variables.
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
Build::Build(){
    m_numericValue = 0;
    // Don't need to explicitly initialize values for the string member variable 
    // and the vector member variable as string and vector classes do that 
}

/* *********************************************************************
Function Name: Build
Purpose: Custom constructor. To define initial values for all member variables.
Parameters: buildCards, a vector of Card pointers that contains cards that 
            could compose a build. Passed by value
            owner, a string variable used to initialize the owner of the build
            value, an int value used to initialize the numeric value of the build
Return Value: None
Assistance Received: none
********************************************************************* */ 
Build::Build(vector<Card *> buildCards, string owner, int value){
    m_cardsInABuild = buildCards;
    m_owner = owner;
    m_numericValue = value;
}

/* *********************************************************************
Function Name: addCardToABuild
Purpose: A mutator used to add a card's reference to a build 
Parameters: aCard, a card's reference that can be added to a build
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Build::addCardToABuild(Card * aCard){
    if(aCard == NULL)
        return;
    
    m_cardsInABuild.push_back(aCard);
    m_numericValue += aCard->getNumericValue();
}

/* *********************************************************************
Function Name: setOwner
Purpose: A mutator used to initialize member variable, m_owner
Parameters: ownername, a string variable that can be used to initiliaze the build's owner
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Build::setOwner(string ownerName){
    m_owner = ownerName;
}

/* *********************************************************************
Function Name: printBuild
Purpose: A utility function used to print the contents of the build. If the build has
        6 of Clubs, 7 of Diamonds and Ace of Hearts, the build contents will be 
        printed like this: [C6 D7 HA] 
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Build::printBuild(){
    cout << "[ ";
    for(int i = 0; i < m_cardsInABuild.size(); i++){
        m_cardsInABuild[i]->printCard();
    }
    cout << "] ";
}

/* *********************************************************************
Function Name: writeBuildToFile
Purpose: A utility function used to write the contents of the build to a file. If the build has
        6 of Clubs, 7 of Diamonds and Ace of Hearts, the build contents will be 
        written to the file like this: [C6 D7 HA] 
Parameters: gameFile, an output file stream object passed by reference so that this function
            can write to the same file represented by the parameter 
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Build::writeBuildToFile(ofstream &gameFile){
    gameFile << "[";
    for(int i = 0; i < m_cardsInABuild.size(); i++){
        m_cardsInABuild[i]->writeCardToFile(gameFile);
    }
    gameFile << "]";
}

