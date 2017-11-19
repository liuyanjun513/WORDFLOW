#ifndef WordFlow_hpp
#define WordFlow_hpp
#include "Session.hpp"
#include <stdio.h>

class WordFlow{
protected:
    Session topScores[20];
public:
    WordFlow();
    ~WordFlow();
    void listH();
    void listL();
    void addS();
    void MainMenu();
};
#endif
