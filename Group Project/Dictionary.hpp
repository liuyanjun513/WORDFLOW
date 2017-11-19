#ifndef Dictionary_hpp
#define Dictionary_hpp
#include <stdio.h>
#include <string>
using namespace std;

class Dictionary{
protected:
    char alpha[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M',
        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};//holds alphabet
public:
    Dictionary();
    ~Dictionary();
    void fill();
    bool isWord(string word);
};
#endif
