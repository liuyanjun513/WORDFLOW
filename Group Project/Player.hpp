#ifndef Player_hpp
#define Player_hpp
#include <iostream>
#include <stdio.h>
using namespace std;

class Player{
	protected:
	    string name;
	    int score;
	public:
		Player();
	    Player(string name);
	    void setScore(int score);
	    int getScore(); 
	    ~Player();
    
    
};
#endif
