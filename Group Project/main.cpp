#include "WordFlow.hpp"
#include "Dictionary.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;

int main(void) {
    WordFlow* one = new WordFlow(); int x=0;
    while(x==0){
        one->MainMenu();
        Player* y= new Player();
        Dictionary* scrabble = new Dictionary();
        string g; cin>>g;
        if(g=="1"){
            //-------------add get name thing to add player*************
            Session* a = new Session(y,scrabble);
            string h;
            cin.ignore();
            cin.sync();
            while(a->play){
                cin.sync();
                getline (cin,h);
                a->extract(h);
            }
        }else if(g=="2"){
            
        }else if(g=="3"){
            
        } else if(g=="0"){x=0;}
    }
    cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    return 0;
}
