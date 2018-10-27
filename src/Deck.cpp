#include "Deck.h"

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/

/* *********************************************************************
Function Name: Deck
Purpose: Default constructor. To define initial values for all member variables
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
Deck::Deck(){
    m_nextCardIndex = 0;
    // Manipulate the second parameter in the function below to 
    // load the deck of cards from a different file
    populateDeck(true, "populateTheDeck.txt"); 
    shuffleTheDeck();
}

/* *********************************************************************
Function Name: Deck
Purpose: Custom constructor. To define initial values for all member variables
Parameters: deckCards, vector of cards passed by value. This vector will be used to initialize the deck
Return Value: None
Assistance Received: none
********************************************************************* */ 
Deck::Deck(vector <Card*> deckCards){
    m_cardsInDeck = deckCards;
    m_nextCardIndex = 0;
}

/* *********************************************************************
Function Name: ~Deck
Purpose: Destructor. To deallocate the resources used by each card in the deck
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
Deck::~Deck(){
    for(int i = 0; i < m_cardsInDeck.size(); i++)
        delete m_cardsInDeck[i];
}

/* *********************************************************************
Function Name: addNextCard
Purpose: To add a reference to a card to the deck. 
Parameters: cardToAdd, a reference to a Card object. cardToAdd is passed by value
Return Value: None
Assistance Received: none
********************************************************************* */ 
bool Deck::addNextCard(Card * cardToAdd){
    if(cardToAdd == NULL)
        return false;

    m_cardsInDeck.push_back(cardToAdd);
    return true;
}

/* *********************************************************************
Function Name: getNextCard
Purpose: To get a reference to a card on the deck. 
Parameters: None
Return Value: toReturn, a reference to the card that is dealt next from the deck
Assistance Received: none
********************************************************************* */ 
Card * Deck::getNextCard() {
    Card * toReturn = NULL; 
    if( !isEmpty() ){
        toReturn = m_cardsInDeck[m_nextCardIndex];
        m_nextCardIndex++;
    }
    return toReturn;
}

/* *********************************************************************
Function Name: shuffleTheDeck
Purpose: To shuffle the vector representating a deck
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Deck::shuffleTheDeck(){
    random_shuffle(m_cardsInDeck.begin(), m_cardsInDeck.end());
}

/* *********************************************************************
Function Name: populateDeck
Purpose: To add cards to a deck from a file
Parameters: None
Return Value: None
Local variables: 
            inFile - the file which will be read in order to extract information about cards
            rawCard - a string of length 2 that will be used to construct a card. An example 
                    of a rawCard is C4. C is potentially a suite character while 4 is the face 
                    value of a card
Algorithm: 
        Open an input file. If file is not found, terminate dislaying proper error message.
        The, read text from the input file one line at a time
        There will be information about a card in one line on the input file.
        Use the information in a line to create a card object.
        Add the reference to the card object to the deck
Assistance Received: none
********************************************************************* */ 
void Deck::populateDeck(bool loadFromFile, const char * filePath){
    if(loadFromFile){
        // Open a file 
        ifstream inFile;
        string rawCard;
    
        inFile.open(filePath);
        if (!inFile) {
            cout << "File not found. Unable to open file";
            exit(1); // terminate with error
        }
        
        while (inFile >> rawCard) {
            Card * newCard = new Card;
            newCard->setSuiteCharacter(rawCard[0]);
            newCard->setFaceCharacter(rawCard[1]);
            m_cardsInDeck.push_back(newCard);   
        }
        
        inFile.close();
    }
}

/* *********************************************************************
Function Name: printDeckCard
Purpose: To print the entire deck of cards in a readable format. If the deck has the 4 of hearts, 6 of clubs
        and 7 of Diamonds, this function will print the deck like this:
        H4, C6, D7
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Deck::printDeckCards(){
    for(int i = m_nextCardIndex; i < m_cardsInDeck.size(); i++){
        m_cardsInDeck[i]->printCard();
    }
}

/* *********************************************************************
Function Name: writeDeckCardsToFile
Purpose: Custom constructor. To define initial variables for all member variables
Parameters: gameFile, an output file stream object passed by reference so that this function 
            can write to the file exactly at the part the caller of this function
            wants to write and modify its contents
Return Value: None
Assistance Received: none
********************************************************************* */ 
void Deck::writeDeckCardsToFile(ofstream &gameFile){
    for(int i = m_nextCardIndex; i < m_cardsInDeck.size(); i++){
        m_cardsInDeck[i]->writeCardToFile(gameFile);
    }
}

/* *********************************************************************
Function Name: isEmpty
Purpose: To determine if the vector representing the deck is empty. 
Parameters: 
Return Value: None
Assistance Received: none
********************************************************************* */ 
bool Deck::isEmpty(){
    if( m_nextCardIndex >= m_cardsInDeck.size() )
        return true;
    
    return false;
}