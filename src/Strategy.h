#pragma once

#include "Table.h"
#include "Player.h"
#include <climits>
#include <map>

class Strategy{
public:
    // The symbolic constants below are necessary for controlling the game. They are public because
    // they are used in other classes (Human and Round) as well. These constants can't be directly
    // or indirectly (through mutators) manipulated from outside this class
    static const int SAVE_THE_GAME = 1;
    static const int MAKE_A_MOVE = 2;
    static const int ASK_FOR_HELP = 3;
    static const int QUIT_THE_GAME = 4;
    static const int BUILD = 1;
    static const int CAPTURE = 2;
    static const int TRAIL = 3;
    static const int CREATE_SINGLE_BUILD = 1;
    static const int CREATE_MULTI_BUILD = 2;
    static const int INCREASE_OPPONENTS_BUILD = 3;
    static const int CAPTURE_SET_OF_LOOSE_CARDS = 1;
    static const int CAPTURE_SINGLE_BUILD = 2;
    static const int CAPTURE_MULTI_BUILD = 3;
    static const int CAPTURE_INDIVIDUAL_LOOSE_CARDS = 4;
    static const int QUIT_CAPTURING = 5;

    // Constructors
    Strategy();
    Strategy(Table * table, vector <Card*> hand, vector <Build*> opponentsBuilds, vector <MultiBuild*> opponentsMultiBuilds);
    
    // Selectors
    inline int getAction() const { return m_action; }
    inline int getCardToPlay() const { return m_cardToPlay; }
    inline string getReasonForAction() const { return m_reasonForAction; }
    inline vector <int> getSingleBuildsToCapture() const { return m_singleBuildsToCapture; }
    inline vector <int> getMultiBuildsToCapture() const { return m_multiBuildsToCapture; }
    inline Table * getGameTable() const { return m_gameTable; }
    inline vector <Build*> getOpponentsBuild() const { return m_opponentsBuilds; }
    inline int getBuildOption() const { return m_buildOption; } //Can be either CREATE_BUILD or INCREASE_OPPONENTS_BUILD
    inline int getOpponentsBuildToIncrease() const { return m_opponentsBuildToIncrease; }
    inline vector<int> getSetOfLooseCardsToCapture() const { return m_oneSetOfLooseCardsToCapture; }
    inline vector<int> getLooseCardsForSingleBuild() const { return m_looseCardsForSingleBuild; }
    inline vector <int> getOneSetOfLooseCardsToCapture() const { return m_oneSetOfLooseCardsToCapture; }

    // Mutator
    inline void setTable(Table * a_table) { m_gameTable = a_table; } 

    // Other utility functions
    void setMoveOption();    
    int whatToCapture();
    void printOpponentsBuildIndexed();
    void recalculateSetOfLooseCards();
    void summarizeMove();

private:
    // Member variables
    int m_action; //Can be BUILD, CAPTURE or TRAIL
    int m_cardToPlay; //This is the (hand) index of the optimal card that can be played. Index can be 0-3
    string m_reasonForAction; // This explains why the Strategy class suggests the action it suggests
    Table * m_gameTable;  // The class will base strategy for players based on this table
    vector <Card*> m_hand; 
    vector <Build*> m_opponentsBuilds;
    vector <MultiBuild*> m_opponentsMultiBuilds;
    bool m_capturedSingleBuild;
    bool m_capturedMultiBuild;
    vector <int> m_looseCardsToCapture;  //Indices of loose cards in the table that can be captured by the player
    vector <int> m_multiBuildsToCapture;
    vector <int> m_singleBuildsToCapture;
    vector <vector<int> > m_setOfLooseCardsToCapture;
    vector <int> m_oneSetOfLooseCardsToCapture;
    vector <int> m_looseCardsForSingleBuild;
    int m_buildOption;
    int m_opponentsBuildToIncrease;     
    map<Card*,int> m_mapFromIndicesToValues;

    // Utility functions
    bool capture();
    void setUpMap(vector <Card*> a_vector);

    //This function returns a set (B) of set of numbers, where the sum of numbers in each individual set == target 
    vector <vector<int> > getCombinations(const vector<Card*> & originalListOfNumbers, int target);

    //Similar to the function above. But this function includes sets only if they have more than one element
    vector <vector<int> > getCombinationsPart2(const vector<Card*> & originalListOfNumbers, int target);
    
    //The function below is the recursive function getCombinations and getCombinationsPart2 depend on 
    void combos(const vector<Card*> & originalListOfNumbers, vector<Card*> &partial, int target, vector <vector<int> > & giantVector, vector<int> & partialIndices);
    int sum(const vector<Card*> & aVector);
    int possibleNumCardsCapturedInMultiBuild(const vector <int> & multiBuildIndices);
    int possibleNumCardsCapturedInSingleBuild(const vector <int> & singleBuildIndices);
    void sortBuildCapture();
    void sortMultiBuildCapture();

    bool canIncreaseOpponentsBuild();
    bool canCreateNewBuilds();
    vector <int> getLargestSet(const vector<vector<int> > & multipleSets);

    void setUpTrail();

    void summarizeBuild();
    void summarizeCapture();
};