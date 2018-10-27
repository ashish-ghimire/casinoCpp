#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector> 
#include <fstream>

using namespace std;

class Card{
public:
  Card();
  Card( char suite, char face);

  // Mutators
  bool setSuiteCharacter(char suite);
  bool setFaceCharacter(char face);

  // Selectors
  int getNumericValue(bool aceIsFourteen = false) const;
  inline char getSuiteCharacter() const { return m_suiteCharacter; }
  inline char getFaceValue() const { return m_faceValue; }

  // Utility functions
  inline char getAce() const { return ACE; }
  inline char getSpade() const { return SPADE; }
  inline char getDiamond() const { return DIAMOND; }
  inline char getFaceFor10() const { return FACE_FOR_10; }
  void printCard();
  void writeCardToFile(ofstream & gameFile);
  int getCardWeight();
  string getStringRepresentation();

private:
  //m_suiteCharacter can be either S (Spade), C (Club), D(Diamnond), H(Heart)
  char m_suiteCharacter; 
  
  // m_faceValue can be 1-9/X/J/Q/K/A - 
  char m_faceValue; 

  // Symbolic constants for face value for cards
  static const int ASCIICodeFor2 = 50;
  static const int ASCIICodeFor9 = 57;
  static const char FACE_FOR_10 = 'X';
  static const char JACK = 'J';
  static const char QUEEN = 'Q';
  static const char KING = 'K';
  static const char ACE = 'A';

  // Symbolic constants for suite characters for cards
  static const char SPADE = 'S';
  static const char CLUB = 'C';
  static const char DIAMOND = 'D';
  static const char HEART = 'H';
};