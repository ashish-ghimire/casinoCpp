#include "Card.h"

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/

/* *********************************************************************
Function Name: Card
Purpose: Default constructor. To define initial values for all member variables
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
Card::Card(){
    m_suiteCharacter = ' ';
    m_faceValue = ' ';
}

/* *********************************************************************
Function Name: Card
Purpose: Custom constructor. To define initial values for all member variables
Parameters: 
            suite, a char that can be used to try to initialize data member m_suiteCharacter by calling setSuiteCharacter
            face, a char that can be used to try to initialize data member m_faceValue by calling setFaceCharacter
Return Value: None
Assistance Received: none
********************************************************************* */ 
Card::Card(char suite, char face){
    setSuiteCharacter(suite);
    setFaceCharacter(face);
}


/* *********************************************************************
Function Name: setFaceCharacter
Purpose: To verfiy if the parameter passed is a legitimate value. If it is a legitimate value,
        the function assigns the parameter's value to member variable, m_faceValue
Parameters: 
            face, a char that can be used to try to initialize data member m_faceValue
Return Value: true if parameter, face is a legitimate value. false otherwise
Assistance Received: none
********************************************************************* */ 
bool Card::setFaceCharacter(char face){
    if( !( (face >= ASCIICodeFor2 && face <= ASCIICodeFor9) || face == FACE_FOR_10 || face == JACK || face == QUEEN || face == KING || face == ACE) )
        return false;
    
    m_faceValue = face;
    return true;
}

/* *********************************************************************
Function Name: setSuiteCharacter
Purpose: To verfiy if the parameter passed is a legitimate value. If it is a legitimate value,
        the function assigns the parameter's value to member variable, m_suiteValue
Parameters: 
            suite, a char that can be used to try to initialize data member m_suiteCharacter
Return Value: true if parameter, suite is a legitimate value. false otherwise
Assistance Received: none
********************************************************************* */ 
bool Card::setSuiteCharacter(char suite){
    if( !(suite == SPADE || suite == CLUB || suite == DIAMOND || suite == HEART) )
        return false;

    m_suiteCharacter = suite;
    return true;
}

/* *********************************************************************
Function Name: printCard
Purpose: To print the components of a card, in this format , "C4 " where C is the suite character of the card
        and 4 is the face value of the card
Parameters: 
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Card::printCard(){
    cout << m_suiteCharacter << m_faceValue << " ";
}


/* *********************************************************************
Function Name: writeCardToFile
Purpose: To write the components of a card to a file, in this format , "C4 " where C is the suite 
        character of the card and 4 is the face value of the card
Parameters: gameFile, an ofstream object passed by reference so that its contents can be modified
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Card::writeCardToFile(ofstream &gameFile){
    gameFile << m_suiteCharacter << m_faceValue << " ";
}

/* *********************************************************************
Function Name: getNumericValue
Purpose: Compute the numeric value of a card based on a card's face value. 
        Kings are valued at 13, Queens at 12, and Jacks at 11. Aces can be valued at 14 or 1, as convenient. 
        Other cards are valued at their face value. 
Parameters: aceIsFourteen. A boolean parameter passed by value. If aceIsFourteen is true, value of an ace card is 14. 
            Else, it is one. 
Return Value: None
Assistance Received: none
********************************************************************* */ 
int Card::getNumericValue(bool aceIsFourteen) const{
    int numericValue = 0;

    switch(m_faceValue){
        case ACE:
        {
            if(aceIsFourteen)
                numericValue = 14;
            else
                numericValue = 1;
        }
            break;
        case FACE_FOR_10: numericValue = 10;
            break;
        case JACK: numericValue = 11;
            break;
        case QUEEN: numericValue = 12;
            break;    
        case KING: numericValue = 13;
            break;
        case '2': numericValue = 2;
            break;
        case '3': numericValue = 3;
            break;
        case '4': numericValue = 4;
            break;
        case '5': numericValue = 5;
            break;
        case '6': numericValue = 6;
            break;
        case '7': numericValue = 7;
            break;
        case '8': numericValue = 8;
            break;
        case '9': numericValue = 9;
            break;
    }

    return numericValue;
}


/* *********************************************************************
Function Name: getCardWeight
Purpose: To determine how important a card is in the context of the game. A card's weight (importance)
        is determined based on the game's scoring guidelines
Parameters: None 
Return Value: weight, an int variable
Algorithm: 
    Weight assingned based on scoring criteria that is:
    
    The player with the most cards in the pile gets 3 points. In the event of a tie, neither player gets points.
    The player with the most spades gets 1 point. In the event of a tie, neither player gets points.
    The player with 10 of Diamonds gets 2 points.
    The player with 2 of Spades gets 1 point.
    Each player gets one point per Ace. 

    Weight of 10 of diamonds = 10
    Weight of 2 of spades = 8
    Weight of any Ace = 5 
    Weight of any spade = 2
    Weight of any other card = 0
Assistance Received: none
********************************************************************* */ 
int Card::getCardWeight(){
    int weight = 0;

    if(m_suiteCharacter == DIAMOND && m_faceValue == FACE_FOR_10)
        weight += 10;

    if(m_suiteCharacter == SPADE && m_faceValue == '2')
        weight += 8;
    
    if(m_faceValue == ACE)
        weight += 5;

    if(m_suiteCharacter == SPADE)
        weight += 2;

    return weight;
}

/* *********************************************************************
Function Name: getStringRepresentation
Purpose: To represent a card in a readable version.
Parameters: 
Return Value: a card's string representation. For example, card C4 would return 4 of Clubs
Assistance Received: none
********************************************************************* */ 
string Card::getStringRepresentation(){
    string representation = "";
    string face = "";
    string suite = "";

    switch(m_faceValue){
        case ACE:
            face = "ace";
            break;
        case KING:
            face = "king";
            break;
        case QUEEN:
            face = "queen";
            break;
        case JACK:
            face = "jack";
            break;
        case FACE_FOR_10:
            face += "10";
        default:
            face += m_faceValue;
            break;
    }

    switch(m_suiteCharacter){
        case SPADE:
            suite = "Spades";
            break;
        case DIAMOND:
            suite = "Diamonds";
            break;
        case HEART:
            suite = "Hearts"; 
            break;
        case CLUB:
            suite = "Clubs";
            break;
    }

    return face + " of " + suite;
}
