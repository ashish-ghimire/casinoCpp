#pragma once

#include "MultiBuild.h"
#include <algorithm>

class Table{
public:
    Table();
    Table(vector <Card*> looseCards, vector<Build*> builds, vector<MultiBuild*> multibuilds);

    // Selectors
    inline vector <Card*> getLooseCards() const { return m_looseCards; }
    inline vector <Build*> getSingleBuilds() const { return m_builds; }
    inline vector <MultiBuild*> getMultiBuilds() const { return m_multiBuilds; } 
    vector <int> getLooseCardsWithGivenNumericValue(int numericValue) const;
    vector <int> getSingleBuildsWithGivenNumericValue(int numericValue) const;
    vector <int> getMultiBuildsWithGivenNumericValue(int numericValue) const;
    vector <int> getNumericValueOfLooseCards() const; //Numeric representation of all loose cards on the table;

    // Mutators
    void addALooseCard(Card * c);
    void addABuild(Build * b);
    void addAMultiBuild(MultiBuild * m); 
    void removeALooseCard(int index);
    void removeABuild(int index);
    void removeABuild(Build * build);
    void removeAMultiBuild(int index);
    void removeAMultiBuild(MultiBuild * m);

    // Utility Functions
    void printTable();
    void printLooseCardsIndexed();
    void printSingleBuildsIndexed();
    void printMultiBuildsIndexed();
    void printBuildsAndBuildOwners();
    
    void writeTableToFile(ofstream & gameFile);
    void writeLooseCardsToFile(ofstream & gameFile);
    void writeSingleBuildsToFile(ofstream & gameFile);
    void writeMultiBuildsToFile(ofstream & gameFile);
    
    bool hasALooseCardWithGivenNumericValue(int numericValue);
    bool hasASingleBuildWithGivenNumericValue(int numericValue);
    bool hasAMultiBuildWithGivenNumericValue(int numericValue);


private:
    // Table is a collection of loose cards, builds and multi-builds
    vector <Card*> m_looseCards; 
    vector <Build*> m_builds;
    vector <MultiBuild*> m_multiBuilds;  

    void printMultiBuildsAndBuildOwners();
    void printSingleBuildsAndBuildOwners();
    void printLooseCards();
    void printSingleBuilds();
    void printMultiBuilds();
};