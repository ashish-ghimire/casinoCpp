#include "Table.h"

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/

/* *********************************************************************
Function Name: Table
Purpose: Default constructor. To define initial values for all member variables
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */ 
Table::Table(){
    m_looseCards.clear();
    m_builds.clear();
    m_multiBuilds.clear();
}

/* *********************************************************************
Function Name: Table
Purpose: Custom constructor. To define initial values for all member variables using
        the parameters passed to this function
Parameters: 
        looseCards - a vector of Card* objects used to initilize a member variable
        builds - a vector of Build* objects used to initilize a member variable
        multibuilds - a vector of MultiBuild* objects used to initilize a member variable
Return Value: None
Assistance Received: none
********************************************************************* */ 
Table::Table(vector<Card*> looseCards, vector<Build*> builds, vector<MultiBuild*> multibuilds){
    m_looseCards = looseCards;
    m_builds = builds;
    m_multiBuilds = multibuilds;
}

/* *********************************************************************
Function Name: getLooseCardsWithGivenNumericValue
Purpose: A selector that returns the indices of those loose cards 
        whose numeric value equals the parameter's value
Parameters: 
        numericValue - an int variable passed by value that indicates the numeric
                value of a card that the function should look for
Return Value: indicesOfLooseCards - a vector of int elements where each int element
                represents the index of a loose card on the table
Assistance Received: none
********************************************************************* */ 
vector<int> Table::getLooseCardsWithGivenNumericValue(int numericValue) const{
    vector<int> indicesOfLooseCards; 

    for(int i = 0; i < m_looseCards.size(); i++){
        Card * card = m_looseCards[i];
        if(card->getNumericValue() == numericValue){
            indicesOfLooseCards.push_back(i);
        }
    }
    
    return indicesOfLooseCards;
}

/* *********************************************************************
Function Name: getSingleBuildsWithGivenNumericValue
Purpose: A selector that returns the indices of those single builds 
        whose numeric value equals the parameter's value
Parameters: 
        numericValue - an int variable passed by value that indicates the numeric
                value of a build that this function should look for
Return Value: indicesOfSingleBuilds - a vector of int elements where each int element
                represents the index of a build on the table whose value
                equals the value of the parameter
Assistance Received: none
********************************************************************* */ 
vector<int> Table::getSingleBuildsWithGivenNumericValue(int numericValue) const{
    vector <int> indicesOfSingleBuilds; 

    for(int i = 0; i < m_builds.size(); i++){
        if(numericValue == m_builds[i]->getNumericValue() )
            indicesOfSingleBuilds.push_back(i);
    }

    return indicesOfSingleBuilds;
}

/* *********************************************************************
Function Name: getMultiBuildsWithGivenNumericValue
Purpose: A selector that returns the indices of those multi builds 
        whose numeric value equals the parameter's value
Parameters: 
        numericValue - an int variable passed by value that indicates the numeric
                value of a multibuild that this function should look for
Return Value: indicesOfMultiBuilds - a vector of int elements where each int element
                represents the index of a multibuild on the table whose value
                equals the value of the parameter
Assistance Received: none
********************************************************************* */ 
vector<int> Table::getMultiBuildsWithGivenNumericValue(int numericValue) const{
    vector<int> indicesOfMultiBuilds; 

    for(int i = 0; i < m_multiBuilds.size(); i++){
        if(numericValue == m_multiBuilds[i]->getNumericValue() )
            indicesOfMultiBuilds.push_back(i);
    }

    return indicesOfMultiBuilds;
}

/* *********************************************************************
Function Name: getNumericValueOfLooseCards
Purpose: A selector that returns the numeric value of all loose cards on the table
Parameters: 
        numericValue - an int variable passed by value that indicates the numeric
                value of a multibuild that this function should look for
Return Value: 
        numericLooseCards - a vector of int elements where each int element
                represents the numeric value of a loose card on the table
Assistance Received: none
********************************************************************* */
vector<int> Table::getNumericValueOfLooseCards()const{
    vector <int> numericLooseCards;

    for(int i = 0; i < m_looseCards.size(); i++){
        numericLooseCards.push_back(m_looseCards[i]->getNumericValue());
    }

    return numericLooseCards;
}

/* *********************************************************************
Function Name: addALooseCard
Purpose: A mutator that adds a reference to a loose card to the table
Parameters: 
        c - a pointer variable that stores a reference to a card
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::addALooseCard(Card * c){
    if(c != NULL)
        m_looseCards.push_back(c);
}

/* *********************************************************************
Function Name: addABuild
Purpose:: A mutator that adds a reference to a build to the table
Parameters: 
        b - a pointer variable that stores a reference to a build
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::addABuild(Build * b){
    m_builds.push_back(b);
}

/* *********************************************************************
Function Name: addAMultiBuild
Purpose:: A mutator that adds a reference to a multibuild to the table
Parameters: 
        m - a pointer variable that stores a reference to a multibuild
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::addAMultiBuild(MultiBuild * m){
    m_multiBuilds.push_back(m);
}

/* *********************************************************************
Function Name: removeALooseCard
Purpose:: A mutator that removes a reference to a loosecard from the table
Parameters: 
        looseCardIndex - an integer variable that should be a vector index of
                one of the loose cards on the table (loose cards in m_loosecards)
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::removeALooseCard(int looseCardIndex){
    if(looseCardIndex < 0 || looseCardIndex >= m_looseCards.size()){
        cout << "Can't remove the index. Invalid \n";
        return;
    }
     m_looseCards.erase(m_looseCards.begin() + looseCardIndex);
}

/* *********************************************************************
Function Name: removeABuild
Purpose:: A mutator that removes a reference to a build from the table
Parameters: 
        index - an integer variable that should be a vector index of
                one of the builds on the table (builds in m_builds)
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::removeABuild(int index){
    m_builds.erase(m_builds.begin() + index);
}

/* *********************************************************************
Function Name: removeABuild
Purpose:: A mutator that removes a reference to a build from the table
Parameters: 
        build - a pointer to a build variable that is on the table 
                (builds in m_builds). Passed by value
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::removeABuild(Build *build){
    vector<Build*>::iterator position = find(m_builds.begin(), m_builds.end(), build);
  
    if (position != m_builds.end()) // == m_build.end() means the element was not found
        m_builds.erase(position);
}

/* *********************************************************************
Function Name: removeAMultiBuild
Purpose:: A mutator that removes a reference to a multi build from the table
Parameters: 
        index - an integer variable that should be a vector index of
                one of the builds on the table (loose cards in m_multiBuilds)
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::removeAMultiBuild(int index){
    m_multiBuilds.erase(m_multiBuilds.begin() + index);
}

/* *********************************************************************
Function Name: removeABuild
Purpose:: A mutator that removes a reference to a build from the table
Parameters: 
        m - a pointer to a multibuild variable that is on the table 
                (multibuilds in m_multibuilds). Passed by value
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::removeAMultiBuild(MultiBuild * m){
    vector<MultiBuild*>::iterator position = find(m_multiBuilds.begin(), m_multiBuilds.end(), m);
  
    if (position != m_multiBuilds.end()) // == m_build.end() means the element was not found
        m_multiBuilds.erase(position);
}

/* *********************************************************************
Function Name: printTable
Purpose:: Print the table in this format  [ [C6 S3] [S9] ] [D5 D2] C8 CJ HA 
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::printTable(){
    printMultiBuilds();
    printSingleBuilds();
    printLooseCards();
}

/* *********************************************************************
Function Name: printMultiBuilds
Purpose:: Print the multibuilds in this format  [ [C6 S3] [S9] ]
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::printMultiBuilds(){
    for(int i = 0; i < m_multiBuilds.size(); i++){
        m_multiBuilds[i]->printMultiBuild();
    }
}

/* *********************************************************************
Function Name: printSingleBuilds
Purpose:: Print the single builds in this format  [C6 S3] [D5 D2]
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::printSingleBuilds(){
    for(int i = 0; i < m_builds.size(); i++){
        m_builds[i]->printBuild();
    }
}

/* *********************************************************************
Function Name: printLooseCards
Purpose:: Print the loose cards in this format  C6 HA DX
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::printLooseCards(){
    for(int i = 0; i < m_looseCards.size(); i++){
        m_looseCards[i]->printCard();
    }
}

/* *********************************************************************
Function Name: printLooseCards
Purpose:: Print the loose cards in this format  C6 (0) HA (1) DX (2).
            Valid indices can be from 0 to (number of table loose cards) - 1
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::printLooseCardsIndexed(){
    for(int i = 0; i < m_looseCards.size(); i++){
        m_looseCards[i]->printCard();
        cout << "( " << i << " )";
    }
}

/* *********************************************************************
Function Name: printSingleBuildsIndexed
Purpose:: Print the single builds in this format  [D5 D7] (0) [HA HX] (1)
            Valid indices can be from 0 to (number of table single builds) - 1
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::printSingleBuildsIndexed(){
    for(int i = 0; i < m_builds.size(); i++){
        m_builds[i]->printBuild();
        cout << "( " << i << " )";
    }
}

/* *********************************************************************
Function Name: :printMultiBuildsIndexed
Purpose:: Print the multi builds in this format  [ [C6 S3] [S9] ] (0) [ [HA HX] [D7 S4] ] (1)
            Valid indices can be from 0 to (number of table multi builds) - 1
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::printMultiBuildsIndexed(){
    for(int i = 0; i < m_multiBuilds.size(); i++){
        m_multiBuilds[i]->printMultiBuild();
        cout << "( " << i << " )";
    }
}

/* *********************************************************************
Function Name: printBuildsAndBuildOwners
Purpose: Print the builds and their owners in this format:

             Multiple builds:
            [ [C6 S3] [S9] ]        Owner : Computer
            [ [S2 H1] [D3] ]        Owner : Human

            Single Builds:
            [S9]                    Owner : Human

Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::printBuildsAndBuildOwners(){
    //Print all multiple builds on the table and their owners
    cout << "\n Multiple builds: \n";
    printMultiBuildsAndBuildOwners();

    //Print all the single builds on the table and their owners
    cout << "\n Single builds : \n";
    printSingleBuildsAndBuildOwners();
}

/* *********************************************************************
Function Name: printMultiBuildsAndBuildOwners
Purpose: Print the multi builds and their owners in this format:
        [ [C6 S3] [S9] ]        Owner : Computer
        [ [S2 H1] [D3] ]        Owner : Human

Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::printMultiBuildsAndBuildOwners(){
    for(int i = 0; i < m_multiBuilds.size(); i++){
        m_multiBuilds[i]->printMultiBuild();
        cout << " Owner : " << m_multiBuilds[i]->getOwner();
        cout << endl;
    }
}

/* *********************************************************************
Function Name: printSingleBuildsAndBuildOwners
Purpose: Print the single builds and their owners in this format:
        [S9]                    Owner : Human
        [D3 S2]                 Owner : Computer

Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::printSingleBuildsAndBuildOwners(){
    for(int i = 0; i < m_builds.size(); i++){
        m_builds[i]->printBuild();
        cout << " Owner : " << m_builds[i]->getOwner();
        cout << endl;
    }
}

/* *********************************************************************
Function Name: writeTableToFile
Purpose:  Write the builds and their owners to a file in this format:

            Multiple builds:
            [ [C6 S3] [S9] ]        Owner : Computer
            [ [S2 H1] [D3] ]        Owner : Human

            Single Builds:
            [S9]                    Owner : Human

Parameters: gameFile, an ofstream object passed by reference. Passed by 
            reference for efficiency and for modifying the file the caller
            called this function with 
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::writeTableToFile( ofstream &gameFile ){
    writeMultiBuildsToFile(gameFile);
    writeSingleBuildsToFile(gameFile);
    writeLooseCardsToFile(gameFile);
}

/* *********************************************************************
Function Name: writeLooseCardsToFile
Purpose:  Write the loose to a file in this format:
            CA H6 DX

Parameters: gameFile, an ofstream object passed by reference. Passed by 
            reference for efficiency and for modifying the file the caller
            called this function with 
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::writeLooseCardsToFile(ofstream &gameFile){
    for(int i = 0; i < m_looseCards.size(); i++){
        m_looseCards[i]->writeCardToFile(gameFile);
    }
}

/* *********************************************************************
Function Name: writeSingleBuildsToFile
Purpose:  Write the single builds to a file in this format:  [C6 S3] [D5 D2]
            
Parameters: gameFile, an ofstream object passed by reference. Passed by 
            reference for efficiency and for modifying the file the caller
            called this function with 
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::writeSingleBuildsToFile(std::ofstream &gameFile){
    for(int i = 0; i < m_builds.size(); i++){
        m_builds[i]->writeBuildToFile(gameFile);
    }
}

/* *********************************************************************
Function Name: writeMultiBuildsToFile
Purpose:  Write the multi builds to a file in this format:
         [ [C6 S3] [S9] ] [ [HA HX] [D7 S4] ]
            
Parameters: gameFile, an ofstream object passed by reference. Passed by 
            reference for efficiency and for modifying the file the caller
            called this function with 
Return Value: None
Assistance Received: none
********************************************************************* */
void Table::writeMultiBuildsToFile(std::ofstream &gameFile){
    for(int i = 0; i < m_multiBuilds.size(); i++){
        m_multiBuilds[i]->writeMultiBuildToFile(gameFile);
    }
}

/* *********************************************************************
Function Name: hasALooseCardWithGivenNumericValue
Purpose: Figure out if there is a loose card on the table with a given
            numeric value. The numeric value is determined by the function's
            parameter
            
Parameters: numericValue, an integer variable passed by value
Return Value: true if there is a loose card on the table whose value
            equals to the value specified by numericValue. false otherwise
Assistance Received: none
********************************************************************* */
bool Table::hasALooseCardWithGivenNumericValue(int numericValue){
    for(int i = 0; i < m_looseCards.size(); i++){
        Card * card = m_looseCards[i];
        if(card->getNumericValue() == numericValue){
            return true;
        }
    }

    return false;
}

/* *********************************************************************
Function Name: hasASingleBuildWithGivenNumericValue
Purpose: Figure out if there is a build on the table with a given
            numeric value. The numeric value is determined by the function's
            parameter
            
Parameters: numericValue, an integer variable passed by value
Return Value: true if there is a build on the table whose value
            equals to the value specified by numericValue. false otherwise
Assistance Received: none
********************************************************************* */
bool Table::hasASingleBuildWithGivenNumericValue(int numericValue){
    for(int i = 0; i < m_builds.size(); i++){
        if(numericValue == m_builds[i]->getNumericValue() )
            return true;
    }
    return false;
}

/* *********************************************************************
Function Name: hasAMultiBuildWithGivenNumericValue
Purpose: Figure out if there is a multi build on the table with a given
            numeric value. The numeric value is determined by the function's
            parameter
            
Parameters: numericValue, an integer variable passed by value
Return Value: true if there is a multibuild on the table whose value
            equals to the value specified by numericValue. false otherwise
Assistance Received: none
********************************************************************* */
bool Table::hasAMultiBuildWithGivenNumericValue(int numericValue)
{
    for(int i = 0; i < m_multiBuilds.size(); i++){
        if(numericValue == m_multiBuilds[i]->getNumericValue() )
            return true;
    }

    return false;
}

