#include "Dictionary.hpp"
#include <fstream>
#include <istream>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

Dictionary::Dictionary(){ fill();}
Dictionary::~Dictionary(){}

void Dictionary::fill(){
    string word; int firstL=0; int secondL=0; int full=0;
    ifstream scrabDict;// create a file input stream
    scrabDict.open("Dictionary1.txt");// Open file
    getline(scrabDict, word);//gets first line
    while(word!="0"){//searches till end of file and adds course contents to timetable
        while(secondL<25&&word.at(1)!=alpha[secondL]){secondL++; full=0;}//organizes by second letter
        dict[firstL][secondL][full]=word; full++;//adds word
        getline(scrabDict, word);//gets next line
    }
    scrabDict.close();//Close file
}

    bool real = false;
    int first=0; int second=0;//checking word in doctionary
    for(int h=0;h<26;h++){
    }
               real = true;
       }
   }
   return real;
}
