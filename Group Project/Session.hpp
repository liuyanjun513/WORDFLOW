#ifndef Session_hpp
#define Session_hpp
#include "Player.hpp"
#include "Dictionary.hpp"
#include "Letter.hpp"
#include <stdio.h>
#include <string>
using namespace std;

class Session{
protected:
    Player* me;//session's player
    Dictionary* scrabble;//scrabble dictionary
    char letters[20];//stores players usable letters
    int used[20];//tracks which letters have already been used
    float specialSpots[6][2];
    string grid[13][13];//stores grid contents
    int values[26] = {1,2,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};//holds letter points
    int vowels[21]={0,0,0,0,4,4,4,4,4,4,8,8,8,8,14,14,14,14,20,20,24};//holds vowel alpha locations w probs
    int consonants[55]={1,1,2,2,3,3,3,3,5,5,6,6,6,7,7,9,10,11,11,11,
        11,12,12,13,13,13,13,13,13,15,15,16,17,17,17,17,17,17,18,18,18,
        18,19,19,19,19,19,19,21,21,22,22,23,25};//holds consonant alpha locations with probabolities
    char alpha[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
        'P','Q','R','S','T','U','V','W','X','Y','Z'};//holds alphabet
    int direction = 0;//1=Right & 2=Down
    int xpos=0;//holds x & y starting positions for words
    int ypos=0;
    string word;//holds word in char array with end
    string newWord;
    int turn=0;//stores players turn count
    int score=0;//stores players score
    int hS=0;//stores highest scoring word score
    string hSW="";//stores players highest scoring word
    
    //new variable
    int startX=-1,startY=-1;   // coordinate for start point
    int endX=-1,endY=-1;       // coordinate for end point
    bool theFirstTime=true;
    char extraLetters[20];
    int isExist[20];
public:
    bool play=true;//keeps track of game and whther to conitinue
    Session();//contructor
    Session(Player* x, Dictionary* d);//contructor
    ~Session();//destroyer
    void setUp();//sets up board
    void xyTOxy();//sets up start and end coordinates
    void lettGen(int n);//generates letters
    void bonusGen();//generates bonus blocks
    void mix();//scrambles letters
    void move(string g);//controls turn
    void extract(string g);//gets players insctruction for move
    void placeWord();//places word onto board if possible
    void updateScore();//updates score after turn
    void updateLetters();//updates letters after turn
    void refresh();//refreshes all tarckers for each turn
    bool fits();//check if word fits on grid
    bool check(char c);
    void print();//prints session grid
    
    
    //new fun
    int getScore();
};
#endif
