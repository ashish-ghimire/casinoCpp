#include "MultiBuild.h"

/*
 ************************************************************
 * Name:     Ashish Ghimire                                 *
 * Project:  Project 1 - Casino                             *
 * Class:    CMPS 366 Organization Of Programming Languages *
 * Date:     October 23, 2018                               *
 ************************************************************
*/

/* *********************************************************************
Function Name: MultiBuild
Purpose: Default constructor. To define initial values for all member variables
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
MultiBuild::MultiBuild(){
    m_multiBuild.clear();
    m_owner = "";
}

/* *********************************************************************
Function Name: MultiBuild
Purpose: Custom constructor. To define initial values for all member variables based
        on the parameters of this function
Parameters: multiBuilds, a vector of Build pointers. The vector is passed by value
            owner, a string, passed by value, that will be used to initialize the
            multiBuild's owner
Return Value: None
Assistance Received: none
********************************************************************* */
MultiBuild::MultiBuild(vector<Build *> multiBuilds, string owner){
    m_multiBuild = multiBuilds;
    m_owner = owner;
}

/* *********************************************************************
Function Name: addToMultiBuild
Purpose: to add a build to the multibuild
Parameters: build, passed by value
Return Value: None
Assistance Received: none
********************************************************************* */
void MultiBuild::addToMultiBuild(Build * build){
    m_multiBuild.push_back(build);
}

/* *********************************************************************
Function Name: setOwner
Purpose: to initialize the value of the multibuild's owner
Parameters: ownerName, passed by value
Return Value: None
Assistance Received: none
********************************************************************* */
void MultiBuild::setOwner(string ownerName){
    m_owner = ownerName;
}

/* *********************************************************************
Function Name: getNumericValue, a selector function
Purpose: to obtain the numeric value of the multibuild.
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
int MultiBuild::getNumericValue() const{
    if(m_multiBuild.empty() )
        return INVALID_MULTIBUILD_NUMERIC_VALUE ;   //Invalid

    return m_multiBuild[0]->getNumericValue(); 
}

/* *********************************************************************
Function Name: getMultiBuildSize, a selector function
Purpose: to obtain the number of cards in the multibuild
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
int MultiBuild::getMultiBuildSize() const{
    int numCardsInAMultiBuild = 0; 

    for(int i = 0; i < m_multiBuild.size(); i++){
        Build * b = m_multiBuild[i];
        numCardsInAMultiBuild += b->getBuildSize();
    }

    return numCardsInAMultiBuild;
}

/* *********************************************************************
Function Name: printMultiBuild
Purpose: To print the contents of the multi build. Say the multi build has
        build, A with cards, 2 of Hearts and ace of Diamonds. Suppose the 
        multi-build also has build, B with cards, Ace of Hearts and 2 of spades,
        The multi-build will be printed like this: [[DA H2] [HA S2]]
Parameters: None
Return Value: None
Assistance Received: none
********************************************************************* */
void MultiBuild::printMultiBuild(){
    cout << "[ ";
    
    for(int i = 0; i < m_multiBuild.size(); i++){
        m_multiBuild[i]->printBuild();
    }
    cout << " ] ";
}

/* *********************************************************************
Function Name: writeMultiBuildToFile
Purpose: To write the contents of the multi build to a file. 
        Say the multi build has build, A with cards, 2 of Hearts and ace of Diamonds.
        Suppose the multi-build also has build, B with cards, Ace of Hearts and
        2 of spades, The multi-build will be written like this: [[DA H2] [HA S2]]
Parameters: gameFile, passed by reference. gameFile's content will be modified as 
        the multibuild (in the format written 2 lines above) will be added to the file
Return Value: None
Assistance Received: none
********************************************************************* */
void MultiBuild::writeMultiBuildToFile(ofstream &gameFile){
    gameFile << "[ ";

    for(int i = 0; i < m_multiBuild.size(); i++){
        m_multiBuild[i]->writeBuildToFile(gameFile);
    }
    gameFile << "] ";
}