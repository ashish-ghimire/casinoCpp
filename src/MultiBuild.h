#pragma once

#include "Build.h"

class MultiBuild{
private:
    // Use this variable to represent an error value because a multiBuild can not have a negative value
    static const int INVALID_MULTIBUILD_NUMERIC_VALUE = -1; 

    vector <Build*> m_multiBuild;
    string m_owner;   // A build must be owned by either a computer or a human player
                        // If the build is owned by a human player, owner will be "human"
                        // Else, owner will be "computer"
public:
    // Constructors
    MultiBuild();
    MultiBuild(vector<Build*> multiBuilds, string owner);

    // Mutators 
    void addToMultiBuild(Build * build);
    void setOwner(string ownerName);

    // Selectors
    string getOwner() const { return m_owner; }
    vector <Build*> getMultiBuild() const { return m_multiBuild; }
    int getMultiBuildSize() const;
    int getNumericValue() const;

    void printMultiBuild();
    void writeMultiBuildToFile(ofstream & gameFile);
};