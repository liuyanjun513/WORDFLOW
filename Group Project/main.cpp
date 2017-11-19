#include "WordFlow.hpp"
#include "Dictionary.hpp"
#include "MyFile.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;
template <typename T1, typename T2>
struct less_second {
	typedef pair<T1, T2> type;
	bool operator ()(type const& a, type const& b) const {
	    return a.second >= b.second;
	}
};
int main(void) {
    WordFlow* one = new WordFlow(); int x=0;
    while(x==0){
        one->MainMenu();
        Dictionary* scrabble = new Dictionary();
        scrabble->test();
        string gameState; 
		cin>>gameState;		
        if(gameState=="1"){
            //-------------add get name thing to add player*************
            cout<<"Please enter your name:";
            string name;
            cin.ignore();
            cin.sync();
            while(1){
            	getline(cin,name);
            	if(name.length()==0){
					cout<<"Please enter a name!!"<<endl;
					cout<<"Please enter your name:";
					continue;
				}else{
					break;
				}
			}
            Player* p=new Player(name);
            Session* a = new Session(p,scrabble);
            string command;
            cin.ignore();
            cin.sync();
            while(a->play){
                cin.sync();
                getline (cin,command);
                a->extract(command);
            }
            
            //set or update the score 
            p->setScore(a->getScore());
            //cout<<name<<p->getScore();
            MyFile myFile;
            myFile.readScore();
            map<string,int> m=myFile.getMap();
            map<string,int>::iterator it=m.find(name);
            if(it!=m.end()){
            	if(it->second<p->getScore()){
            		it->second=p->getScore();
            		cout<<it->first<<"   "<<it->second<<endl;
            		
				}
				ofstream out;
				out.open("score.txt");
				for(it=m.begin();it!=m.end();++it){
					if(it->first=="") break;
					out<<"("<< it->first <<")"<<"("<< it->second <<")"<<endl;
				}
					
			}else{
				ofstream out;
				out.open("score.txt",ios_base::app);
				out<<"("<< name <<")"<<"("<< p->getScore() <<")"<<endl;
			}

        }else if(gameState=="2"){
            
        }else if(gameState=="3"){   
        	//https://stackoverflow.com/questions/2453425/how-can-i-sort-a-map-by-its-second-parameter
			//sort map
        	MyFile myFile;
        	myFile.readScore();
        	map<string,int> m=myFile.getMap();
			
			vector<pair<string, int> > mapcopy(m.begin(), m.end());
			sort(mapcopy.begin(), mapcopy.end(), less_second<string, int>());
			int rank=1;
			cout<<"          "<<"Rank"<<"     Name               Score"<<endl; 
			for (vector<pair<string,int>>::iterator it=mapcopy.begin();it!=mapcopy.end();++it){
				cout <<"           "<<rank<<"     "<<it->first << "         " << it->second << endl;
				rank++;				
			}
		} else if(gameState=="0"){break;}
    }
    cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    return 0;
}
