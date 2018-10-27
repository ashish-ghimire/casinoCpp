#include "Strategy.h"

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/

/* *********************************************************************
Function Name: Strategy
Purpose: Default constructor. To define initial values for all member variables
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
Strategy::Strategy(){
    m_action = 0;
    m_cardToPlay = 5; // Invalid value
    m_reasonForAction = "";
    m_capturedSingleBuild = false;
    m_capturedMultiBuild = false;
    m_buildOption = 0;

    //Invalid value as opponents is unlikely to have 1000 single builds in one round 
    m_opponentsBuildToIncrease = 1000; 
}

/* *********************************************************************
Function Name: Strategy
Purpose: Custom constructor. To define initial values for all member variables
Parameters: 
            table - a pointer to a Table object used to initiliaze this class's Table
                    member variable
            hand - a vector of Card * objects used to initiliaze m_hand
            opponentsBuilds - a vector of Build * objects used to iniliaze m_opponentsBuilds
Return Value: None
Assistance Received: none
********************************************************************* */ 
Strategy::Strategy(Table * table, vector<Card *> hand, vector <Build *> opponentsBuilds, vector <MultiBuild*> opponentsMultiBuilds  ){
    m_action = 0;
    m_cardToPlay = 5; // Invalid value
    m_reasonForAction = "";
    m_gameTable = table;
    m_hand = hand;
    m_opponentsBuilds = opponentsBuilds;
    m_opponentsMultiBuilds = opponentsMultiBuilds;
    m_capturedSingleBuild = false;
    m_capturedMultiBuild = false;
    m_buildOption = 0;

    //Invalid value as opponents is unlikely to have 1000 single builds in one round 
    m_opponentsBuildToIncrease = 1000; 
    m_looseCardsForSingleBuild.clear();
    m_looseCardsToCapture.clear();
    setMoveOption();
}

/*
This function will return move option. Move option can either be 
    
*/

/* *********************************************************************
Function Name: setMoveOption
Purpose: This function decides the optimal move option for the player in a turn
Parameters: None
Return Value: an integer variable which can equal to BUILD, CAPTURE or TRAIL
Assistance Received: none
********************************************************************* */ 
void Strategy::setMoveOption(){

    if( capture() ){
        m_action = CAPTURE;
        return;
    }

    if(canCreateNewBuilds() ){
        m_action = BUILD;
        return;
    }

    if( canIncreaseOpponentsBuild() ){
         m_action = BUILD;
         return;
    }

    setUpTrail();
    m_action = TRAIL;
    m_reasonForAction += "You can neither capture nor build. So, you should trail. \n";
}

/* *********************************************************************
Function Name: capture
Purpose: To check if the player can capture any loose cards, single or multi
        builds or sets of cards
Parameters: None
Return Value: true if the player can capture anything. false otherwise
Assistance Received: none
********************************************************************* */ 
bool Strategy::capture(){
    int maxCapture = 0;
    vector<int> individualLooseCardIndices; //Loose card indices based on the table
    vector <int> multiBuildIndices;
    vector <int> singleBuildIndices;
    vector <Card*> tableLooseCards = m_gameTable->getLooseCards();
    setUpMap(m_gameTable->getLooseCards());

    for(int i = 0; i < m_hand.size(); i++){
    
        int possibleNumberOfCaptures = 0;
        Card * handCard = m_hand[i];

        //Get individualooseCard Indices 
        individualLooseCardIndices = m_gameTable->getLooseCardsWithGivenNumericValue(handCard->getNumericValue());
        possibleNumberOfCaptures += individualLooseCardIndices.size();

        //Get set of loose cards ----- Need to implement this
        vector<vector <int> > setOfLooseCards = getCombinationsPart2(tableLooseCards, handCard->getNumericValue(true)); //true means value of ace should be 14
        for(int k = 0; k < setOfLooseCards.size(); k++){
            possibleNumberOfCaptures += setOfLooseCards[i].size();
        }

        //Get multibuildIndices
        multiBuildIndices = m_gameTable->getMultiBuildsWithGivenNumericValue(handCard->getNumericValue(true));
        possibleNumberOfCaptures += possibleNumCardsCapturedInMultiBuild(multiBuildIndices);

        //Get singleBuildIndices
        singleBuildIndices = m_gameTable->getSingleBuildsWithGivenNumericValue(handCard->getNumericValue(true)); 
        possibleNumberOfCaptures += possibleNumCardsCapturedInSingleBuild(singleBuildIndices);

        if(possibleNumberOfCaptures > maxCapture){
            m_looseCardsToCapture = individualLooseCardIndices;
            m_multiBuildsToCapture = multiBuildIndices;
            m_singleBuildsToCapture = singleBuildIndices;
            m_cardToPlay = i;
            m_setOfLooseCardsToCapture = setOfLooseCards;
            maxCapture = possibleNumberOfCaptures;
        }

        if(!m_singleBuildsToCapture.empty())
            sortBuildCapture();

        if(!m_multiBuildsToCapture.empty())
            sortMultiBuildCapture();
    }

    if(maxCapture == 0)
        return false;
    return true;
}

/* *********************************************************************
Function Name: setUpMap
Purpose: To set up member variable, m_mapFromIndicesToValues which is 
        an unordered map which is a map from a Card object to an integer value.
Parameters: a_vector, a vector of Card * objects
Return Value: none
Assistance Received: none
********************************************************************* */
void Strategy::setUpMap(vector <Card*> a_vector){
    for(int i = 0; i < a_vector.size(); i++){
        m_mapFromIndicesToValues[a_vector[i]] = i;
    }
}

/* *********************************************************************
Function Name: getCombinationsPart2
Purpose: This function returns a set of set of integers, where each set is of size  
            greater than 1.
Parameters: originalListOfNumbers - a const vector of Card * objects passed by reference
            for efficiency
            target - an integer variable passed by value that specifies what the cards
                in each individual set should add up to
Return Value: setOfSetOfNumbers - a vector of vector of int where each vector of int is
            of size > 1 and  ontains a set of integer values. Each integer value is 
            an index of the card  
Assistance Received: none
********************************************************************* */
vector <vector<int> > Strategy::getCombinationsPart2(const vector<Card*> & originalListOfNumbers, int target){
    vector <vector<int> > setOfSetOfNumbers;
    vector <Card*> partial;
    vector <int> partialIndices; 

    combos(originalListOfNumbers, partial, target, setOfSetOfNumbers, partialIndices);

    vector<vector <int> > newSet;

    for(int i = 0; i < setOfSetOfNumbers.size(); i++){
        if(setOfSetOfNumbers[i].size() > 1)
            newSet.push_back(setOfSetOfNumbers[i]);
    }

    return newSet;
}

/* *********************************************************************
Function Name: combos
Purpose: This function populates a set of set of numbers, where the 
        sum of numbers in each individual set == target as specified in the
        parameter 
Parameters: originalListOfNumbers - a const vector of Card * objects passed by reference
            for efficiency
            partial - a vector of Card* objects passed by reference for efficiency
            giantVector - a vector of vector of int where each vector of int is
            of size > 1 and  contains a set of integer values. Each integer value is 
            an index of the card . Passed by reference. 
            target - an integer variable passed by value that specifies what the cards
                in each individual set should add up to
            partialIndices- a vector of integers that contains indices of cards
Return Value: None
Assistance Received: none
********************************************************************* */
void Strategy::combos(const vector<Card*> & originalListOfNumbers, vector<Card*> &partial, int target, vector <vector<int> > & giantVector, vector<int> & partialIndices){
    int sum1 = sum(partial);

    if(sum1 == target)
        giantVector.push_back(partialIndices);
    
    if(sum1 > target)
        return;

    for(int i = 0; i <originalListOfNumbers.size(); i++){
        Card * temp = originalListOfNumbers[i];
        int temp2 = m_mapFromIndicesToValues[temp];
        vector<Card*> remainingElements;

        for(int j = i + 1; j < originalListOfNumbers.size(); j++){
            remainingElements.push_back(originalListOfNumbers[j]);
        }

        vector<Card*> partial2 = partial;
        partial2.push_back(temp);
        
        vector<int> partialIndices2 = partialIndices;
        partialIndices2.push_back(temp2);

        combos(remainingElements, partial2, target, giantVector, partialIndices2);
    }
}

/* *********************************************************************
Function Name: sum
Purpose: To calculate a sum of numeric values of cards found in the parameter
Parameters: aVector - a const vector of Card* objects passed by reference for
            efficiency
Return Value: None
Assistance Received: none
********************************************************************* */
int Strategy::sum(const vector<Card*> & aVector){
    int sum = 0;
    for(int i = 0; i < aVector.size(); i++){
        sum += aVector[i]->getNumericValue();
    }
    return sum;
}

/* *********************************************************************
Function Name: possibleNumCardsCapturedInMultiBuild
Purpose: To calculate the number of cards in each multibuild represented by
        the multibuild index in the paramter and return the summation 
        of number cards in all the multibuilds 
Parameters: multiBuildIndice - a const vector of int elements passed by
            reference for efficiency
Return Value: None
Assistance Received: none
********************************************************************* */
int Strategy::possibleNumCardsCapturedInMultiBuild(const vector<int> & multiBuildIndices){
    int possibleNumberOfCaptures = 0;

    for(int i = 0; i < multiBuildIndices.size(); i++){
        int index = multiBuildIndices[i];
        MultiBuild * m = m_gameTable->getMultiBuilds()[index];
        possibleNumberOfCaptures += (m->getMultiBuildSize());
    }

    return possibleNumberOfCaptures;
}

/* *********************************************************************
Function Name: possibleNumCardsCapturedInSingleBuild
Purpose: To calculate the number of cards in each build represented by
        the build index in the paramter and return the summation 
        of number cards in all the singlebuilds 
Parameters: multiBuildIndice - a const vector of int elements passed by
            reference for efficiency
Return Value: None
Assistance Received: none
********************************************************************* */
int Strategy::possibleNumCardsCapturedInSingleBuild(const vector<int> & singleBuildIndices){
    int possibleNumberOfCaptures = 0;

    for(int i = 0; i < singleBuildIndices.size(); i++){
        int index = singleBuildIndices[i];
        Build * b = m_gameTable->getSingleBuilds()[index];
        possibleNumberOfCaptures += b->getBuildSize();
    }

    return possibleNumberOfCaptures;
}

/* *********************************************************************
Function Name: sortBuildCapture
Purpose: To sort a member vector that contains indices of all single builds
         the player can capture in such a way such that the player's builds
         appear before her opponent's builds
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Strategy::sortBuildCapture(){
    vector <int> sortedBuild;
    vector <int> oppBuildToCapture;

    for(int i = 0; i < m_singleBuildsToCapture.size(); i++){
        int indexOfBuild = m_singleBuildsToCapture[i];
        Build * buildToLook = m_gameTable->getSingleBuilds()[indexOfBuild];
        
        vector<Build*>::iterator position = find(m_opponentsBuilds.begin(), m_opponentsBuilds.end(), buildToLook);
        if(position == m_opponentsBuilds.end() ) //Not found
            sortedBuild.push_back(indexOfBuild);
        else
            oppBuildToCapture.push_back(indexOfBuild);
    }

    for(int i = 0; i < oppBuildToCapture.size(); i++)
        sortedBuild.push_back(oppBuildToCapture[i]);

    m_singleBuildsToCapture = sortedBuild;
}

/* *********************************************************************
Function Name: sortMultiBuildCapture
Purpose: To sort a member vector that contains indices of all multi builds
         the player can capture in such a way such that the player's multi 
         build indices appear before her opponent's multi builds indices
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Strategy::sortMultiBuildCapture(){
    vector <int> sortedBuild;
    vector <int> oppBuildToCapture;

    for(int i = 0; i < m_multiBuildsToCapture.size(); i++){
        int indexOfBuild = m_multiBuildsToCapture[i];
        MultiBuild * buildToLook = m_gameTable->getMultiBuilds()[indexOfBuild];
        
        vector<MultiBuild*>::iterator position = find(m_opponentsMultiBuilds.begin(), m_opponentsMultiBuilds.end(), buildToLook);
        if(position == m_opponentsMultiBuilds.end() ) //Not found
            sortedBuild.push_back(indexOfBuild);
        else
            oppBuildToCapture.push_back(indexOfBuild);
    }

    for(int i = 0; i < oppBuildToCapture.size(); i++)
        sortedBuild.push_back(oppBuildToCapture[i]);

    m_multiBuildsToCapture = sortedBuild;        
}

/* *********************************************************************
Function Name: canIncreaseOpponentsBuild
Purpose: To check if the player can increase a single build that belongs 
        to her opponent
Parameters: None
Return Value: true if the player can increase an opponent's build.
            false otherwise
Assistance Received: none
********************************************************************* */
bool Strategy::canIncreaseOpponentsBuild(){
    int highestNumOfCardsInABuild = 0;

    for(int i = 0; i < m_opponentsBuilds.size(); i++){
        Build * oppBuild = m_opponentsBuilds[i];
        int buildValue = oppBuild->getNumericValue();

        for(int j = 0; j < m_hand.size(); j++){    //j denotes the card that could potentially be incorporated into the increased build
            for(int k = 0; k < m_hand.size(); k++){  //k denotes the card whose value should equal the value of the increased build
                if(k != j){
                    if(buildValue + m_hand[j]->getNumericValue() == m_hand[k]->getNumericValue(true) ){ //The player can potentially increase an opponents build
                        if( oppBuild->getBuildSize() > highestNumOfCardsInABuild ){ //Select the largest build
                            m_cardToPlay = j;
                            //Build index to increase == i
                            m_opponentsBuildToIncrease = i;
                            highestNumOfCardsInABuild = oppBuild->getBuildSize();
                            m_buildOption = INCREASE_OPPONENTS_BUILD;
                        }
                    }
                }
                
            }
        }
    }
    if(highestNumOfCardsInABuild > 0) //MEaning there is at least one build (a build has more than one card) that the user can increase
        return true;

    return false;
}


/* *********************************************************************
Function Name: canCreateNewBuilds
Purpose: To check if the player can create a single build 
Parameters: None
Return Value: true if the player can create a single build. 
            false otherwise
Assistance Received: none
********************************************************************* */
bool Strategy::canCreateNewBuilds(){
    vector <Card*> tableLooseCards = m_gameTable->getLooseCards();
    setUpMap(m_gameTable->getLooseCards());

    for(int playCard = 0; playCard < m_hand.size(); playCard++ ){
        for(int holdCard = 0; holdCard < m_hand.size(); holdCard++){            
            if(playCard != holdCard){
               int target = m_hand[holdCard]->getNumericValue(true) - m_hand[playCard]->getNumericValue();
               vector< vector <int> > multipleSetOfLooseCards = getCombinations(tableLooseCards, target);

               //Compute biggest setOfLooseCards
               vector <int> biggestSetOfLooseCards = getLargestSet(multipleSetOfLooseCards);

               if(biggestSetOfLooseCards.size() > m_looseCardsForSingleBuild.size()){
                    m_looseCardsForSingleBuild = biggestSetOfLooseCards;
                    m_cardToPlay = playCard;
               }
            }
        }
    }

    if(m_looseCardsForSingleBuild.size() > 0 ){
        m_buildOption = CREATE_SINGLE_BUILD;
        return true;
    }
        
    return false;
}

/* *********************************************************************
Function Name: getCombinations
Purpose: This function returns a set of set of integers, where each element in each
        set represents an index of a card on the player's hand
Parameters: originalListOfNumbers - a const vector of Card * objects passed by reference
            for efficiency
            target - an integer variable passed by value that specifies what the cards
                in each individual set should add up to
Return Value: setOfSetOfNumbers - a vector of vector of int where each vector of int
            contains a set of integer values. Each integer value is 
            an index of the card  
Assistance Received: none
********************************************************************* */
vector <vector<int> > Strategy::getCombinations(const vector<Card*> & originalListOfNumbers, int target){
    vector <vector<int> > setOfSetOfNumbers;
    vector <Card*> partial;
    vector <int> partialIndices;

    combos(originalListOfNumbers, partial, target, setOfSetOfNumbers, partialIndices);

    return setOfSetOfNumbers;
}

/* *********************************************************************
Function Name: getLargestSet
Purpose: To determine the largest vector from a set of vectors 
Parameters: originalListOfNumbers - a const vector of Card * objects passed by reference
            for efficiency
            target - an integer variable passed by value that specifies what the cards
                in each individual set should add up to
Return Value: setOfSetOfNumbers - a vector of vector of int where each vector of int
            contains a set of integer values. Each integer value is 
            an index of the card  
Assistance Received: none
********************************************************************* */
vector <int> Strategy::getLargestSet(const vector<vector<int> > & multipleSets){
    vector <int> largest;

    for(int i = 0; i < multipleSets.size(); i++){
        if(multipleSets[i].size() > largest.size())
            largest = multipleSets[i];
    }

    return largest;
}

/* *********************************************************************
Function Name: setUpTrail
Purpose: To identify the correct card to choose for trail
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Strategy::setUpTrail(){

    /*
        Compute weight for every card in hand. The higher the weight, higher the value of the card. 
        Trail the card with the lowest weight
    */
    int lowestWeight = INT_MAX;

    for(int i = 0; i < m_hand.size(); i++){
        Card * c = m_hand[i];
        if(c->getCardWeight() < lowestWeight ){
            m_cardToPlay = i;
            lowestWeight = c->getCardWeight();
        }
    }
}


/* *********************************************************************
Function Name: whatToCapture
Purpose: To identify the correct capture option the player should select.
        The option can be to capture multibuild
Parameters: None
Return Value: an integer, identifying the correct capture option the player
            should select. 
Assistance Received: none
********************************************************************* */
int Strategy::whatToCapture() {

    if(m_multiBuildsToCapture.size() > 0 && !m_capturedMultiBuild){
        m_capturedMultiBuild = true;
        return CAPTURE_MULTI_BUILD;
    }

    if(m_singleBuildsToCapture.size() > 0 && !m_capturedSingleBuild ){
        m_capturedSingleBuild = true;
        return CAPTURE_SINGLE_BUILD;
    }

    //Whereas you can capture multibuilds, single builds or individual cards
    // all at once, you need to capture set of cards one set at a time
    // The three lines below remove a set once it has been captured
    if(m_setOfLooseCardsToCapture.size() > 0){
        m_setOfLooseCardsToCapture.erase(m_setOfLooseCardsToCapture.begin() + 0);
        return CAPTURE_SET_OF_LOOSE_CARDS;
    }

    if( m_looseCardsToCapture.size() > 0){
        m_looseCardsToCapture.clear();
        return CAPTURE_INDIVIDUAL_LOOSE_CARDS;
    }

    return QUIT_CAPTURING;
}


void Strategy::printOpponentsBuildIndexed(){
    for(int i = 0; i < m_opponentsBuilds.size(); i++){
        m_opponentsBuilds[i]->printBuild();
        cout << "( " << i << " )";
    }
}

/* *********************************************************************
Function Name: recalculateSetOfLooseCards
Purpose: To recalculate indices of loose cards based on the table 
        loose cards and the played card. This is necessary if the table
        is updated
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Strategy::recalculateSetOfLooseCards(){
    vector <Card*> tableLooseCards = m_gameTable->getLooseCards();
    setUpMap(m_gameTable->getLooseCards());

    vector<vector<int> > looseCardIndices = getCombinationsPart2(tableLooseCards, m_hand[m_cardToPlay]->getNumericValue(true)); // true means ace value will be 14

    vector <int> temp;

    if(looseCardIndices.size() > 0)
        m_oneSetOfLooseCardsToCapture = looseCardIndices[0];
    else
        m_oneSetOfLooseCardsToCapture.clear();
}

/* *********************************************************************
Function Name: summarizeMove
Purpose: To summarize the entire move
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Strategy::summarizeMove(){
    string moveReason;
    string moveDescription;

    cout << "play " << m_hand[m_cardToPlay]->getStringRepresentation() << " to: " << endl;

    switch(m_action){
        case BUILD:
            summarizeBuild();
            break;
        case CAPTURE:
            summarizeCapture();
            break;
        case TRAIL:
            cout << "\n to trail";
            cout << "\nReason: Because the player can't capture or build \n\n";
            break;
    }
}

/* *********************************************************************
Function Name: summarizeBuild
Purpose: To summarize all the build actions taken
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Strategy::summarizeBuild(){
    if(m_buildOption == CREATE_SINGLE_BUILD){
        cout << "\n create a build using the following loose cards from table : " << endl;

        for(int i = 0; i < m_looseCardsForSingleBuild.size(); i++){
            int tableLooseCardIndex = m_looseCardsForSingleBuild[i];
            m_gameTable->getLooseCards()[tableLooseCardIndex]->printCard();
        }
        cout <<"\n Reason : To possibly capture more cards in the future \n\n";
    }
    else{
        cout << "\n Reason : increase the following build that belongs to your opponent " << endl;
        m_opponentsBuilds[m_opponentsBuildToIncrease]->printBuild();
        cout << "\n Reason : To take over opponents build, to annoy your opponent and to possibly capture more cards in the future \n\n";
    }    
}

/* *********************************************************************
Function Name: summarizeCapture
Purpose: To summarize all the capture actions taken
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Strategy::summarizeCapture(){
    if(!m_looseCardsToCapture.empty()){
        cout << "\n capture the following loose cards from the table \n";
        for(int i = 0; i < m_looseCardsToCapture.size(); i++ ){
            int index = m_looseCardsToCapture[i];
            m_gameTable->getLooseCards()[index]->printCard();
        }
    }
    if(!m_setOfLooseCardsToCapture.empty()){
        if(!m_looseCardsToCapture.empty() ){
            vector <int> temp = m_setOfLooseCardsToCapture[0];
            if( find(temp.begin(), temp.end(), m_looseCardsToCapture[0] ) != temp.end()  )
                cout << "\n\t\t OR";  
                //Meaning there is a card that is in both individualLooseCards and in set of cards. This can only happen if there is an Ace on the
                // table that can be used to capture an individual loose card and a set of cards. It is user's choice what he wants to capture
        } 

        cout << "\n capture the following sets of cards from the table \n";
        for(int i = 0; i < m_setOfLooseCardsToCapture.size(); i++){
            vector <int> oneSet = m_setOfLooseCardsToCapture[i];
            for(int j = 0; j < oneSet.size(); j++){
                int index = oneSet[j];
                m_gameTable->getLooseCards()[index]->printCard();
            }
        }
    }

    if(!m_singleBuildsToCapture.empty()){
        cout << "\n capture the following single builds from the table \n";
        for(int i = 0; i < m_singleBuildsToCapture.size(); i++){
            int buildIndex = m_singleBuildsToCapture[i];
            m_gameTable->getSingleBuilds()[buildIndex]->printBuild();
        }
    }

    if(!m_multiBuildsToCapture.empty() ){
        cout << "\n capture the following multi builds from the table \n";
        for(int i = 0; i < m_multiBuildsToCapture.size(); i++){
            int multiBuildIndex = m_multiBuildsToCapture[i];
            m_gameTable->getMultiBuilds()[multiBuildIndex]->printMultiBuild();
        }
    }
    cout << "\nReason: To capture as many cards as possible \n\n";
}

