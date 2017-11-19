#include "MyFile.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <cstddef>
#include <map>
using namespace std;
MyFile::MyFile(){

}

void MyFile::readScore(){
	ifstream in;
	string input;
	in.open("score.txt");
	//getline(in,input);
	while(in){
		string name,s;
		int count=0;
		int score;
		in.sync();
        getline(in,input);
        if(input==""){
			break;
		}
        for(int i=0;i<input.length();i++){
        	if(input.at(i)=='('){
        		count++;
        		continue;
			}
			if(input.at(i)==')'){
				continue;
			}
			if(count==1){ //   (name)
				name=name+input.at(i);
			}
			if(count==2){
				s=s+input.at(i);
			}
		}
		score=stoi(s);
		m.insert(pair<string,int>(name,score));
	}    
}



MyFile::~MyFile(){
	
}
