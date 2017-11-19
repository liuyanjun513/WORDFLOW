#include "Session.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <math.h>
#include <ctype.h>
#include <cmath>
using namespace std;
//------------------------------------------------------------------------------------------------------------------------CONSTRUCTORS
Session::Session(){}
Session::Session(Player* x, Dictionary* d){
    me=x;
    scrabble=d;
    setUp();
}
Session::~Session(){}

//------------------------------------------------------------------------------------------------------------------------SETUP
void Session::setUp(){
    xyTOxy();
    bonusGen();
    lettGen(20);
    print();
    for(int x=0;x<13;x++){
        for(int y=0;y<13;y++){
            if(grid[x][y]==""){
                grid[x][y]=" ";
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------------GET PLAYER MOVE INFO
void Session::extract(string g){
    int end=0; int t=0; //tracks collected turn and word info
    char attempt[13];//holds letters to attempted word
    while(check(g.at(t))==false){t++;}
    if(g.at(t)=='1'){ mix(); t=(int)g.length();print();
    } else if(g.at(t)=='0'){t=(int)g.length();play=false;}
    while(check(g.at(t))==false){t++;}
    for(int i=0;i<13;i++){
        if(g.at(t)==alpha[i]){ypos=i;i=13;t++;}
    }
    if(check(g.at(t))==true){
        if(check(g.at(t+1))==true){
            xpos=((int)g.at(t+1))-39;t++;
        }else{xpos=((int)g.at(t))-49;t++;}
    }
    while(check(g.at(t))==false){t++;}
    if(toupper(g.at(t))=='R'){direction=1;
    } else if(toupper(g.at(t))=='D'){direction=2;}
    t++;
    for(int f=t;f<g.length()-2;f++){
        for(int w=0;w<20;w++){//cycles usable letters and grabs word
            if(t<g.length()&&toupper(g.at(t))==letters[w]&&used[w]!=1){//checks for match and unused
                used[w]=1;//tracks used leters
                attempt[end]=letters[w];//adds char to word
                //f=(int)g.length();//tracks word size/string char/ letters
                end++; w=-1; t++;
            }
        }
        t++;
    }
    word=attempt;
    move(attempt);
}
//------------------------------------------------------------------------------------------------------------------------MOVE
void Session::move(string attempt){//makes move
    if(fits()){
        if(scrabble->isWord(attempt)){
            updateScore();
            placeWord();
            updateLetters();
            refresh();
            print();
        } else {cout<<"\n\t\t\tThat is not a real word. Try again.\n\t\t\t\t";}
    } else{cout<<"\n\t\t\tThat word doesn't fit onto the pre-existing grid. Try again.\n\t\t\t\t";}
}
//------------------------------------------------------------------------------------------------------------------------START & END GENERATOR
void Session::xyTOxy(){//sets up start and end coordinates
    bool far=false;
    srand((unsigned)time(0));
    specialSpots[0][0]= rand() % 4;
    specialSpots[0][1]=rand() % 13;
    grid[(int)specialSpots[0][0]][(int)specialSpots[0][1]]="@";
    while(far==false){//ensures start and end are sufficiently far away
        specialSpots[1][0] = rand() % 13;
        specialSpots[1][1] = rand() % 13;
        if(sqrt(pow(specialSpots[1][0]-specialSpots[0][0],2)+pow(specialSpots[1][1]-specialSpots[0][1],2))>9){
            far=true;
            grid[(int)specialSpots[1][0]][(int)specialSpots[1][1]]="@";
        }
    }
}
//------------------------------------------------------------------------------------------------------------------------BONUS GENERATOR
void Session::bonusGen(){
    srand((unsigned)time(0)); int y=0;
    bool canput[4]={false,false,false,false};
    string perks[12]={"+10","+10","+10","+10","+20","+20","+20","+30","+30","+50","DL", "DW"};
    for(int x=2;x<6;x++){
        y++;
        while(canput[y]==false){//ensures spots are sufficiently far away
            specialSpots[x][0] = rand() % 13;
            specialSpots[x][1] = rand() % 13;
            if((specialSpots[x][0]!=specialSpots[0][0]&&specialSpots[x][1]!=specialSpots[0][1])&&
               (specialSpots[x][0]!=specialSpots[1][0]&&specialSpots[x][1]!=specialSpots[1][1])){
                if(x==2){
                    canput[y]=true;
                    grid[(int)specialSpots[x][0]][(int)specialSpots[x][1]]=perks[rand() % 10];
                } else if(x==3&&sqrt(pow(specialSpots[x][0]-specialSpots[2][0],2)+pow(specialSpots[x][1]-specialSpots[2][1],2))>3){
                    canput[y]=true;
                    grid[(int)specialSpots[x][0]][(int)specialSpots[x][1]]=perks[rand() % 10];
                } else if(x==4&&sqrt(pow(specialSpots[4][0]-specialSpots[2][0],2)+pow(specialSpots[4][1]-specialSpots[2][1],2))>3
                          &&sqrt(pow(specialSpots[3][0]-specialSpots[4][0],2)+pow(specialSpots[3][1]-specialSpots[4][1],2))>3){
                    canput[y]=true;
                    grid[(int)specialSpots[x][0]][(int)specialSpots[x][1]]=perks[rand() % 2+10];
                } else if(x==5&&sqrt(pow(specialSpots[3][0]-specialSpots[5][0],2)+pow(specialSpots[3][1]-specialSpots[5][1],2))>3
                          &&sqrt(pow(specialSpots[5][0]-specialSpots[2][0],2)+pow(specialSpots[5][1]-specialSpots[2][1],2))>3
                          &&sqrt(pow(specialSpots[5][0]-specialSpots[4][0],2)+pow(specialSpots[5][1]-specialSpots[4][1],2))>3){
                    canput[y]=true;
                    grid[(int)specialSpots[x][0]][(int)specialSpots[x][1]]=perks[rand() % 2+10];
                }
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------------LETTER GENERATOR
void Session::lettGen(int n){
    for(int u=(20-n);u<(int)(n*0.45);u++){//adds random vowels
        letters[u]=alpha[vowels[rand() % 21]];
    }
    for(int t=(int)(n*0.45)+(20-n);t<20;t++){//adds consonants
        letters[t]=alpha[consonants[rand() % 55]];
    }
    mix();//mixes letters
}
//------------------------------------------------------------------------------------------------------------------------UPDATE LETTERS
void Session::updateLetters(){//updates letters
    if(used[19]==1){letters[19]=' ';}
    for(int y=18;y>-1;y--){
        if(used[y]==1){
            for(int s=y;s<18;s++){letters[s]=letters[s+1];}
        }
    }
    lettGen((int)word.length());
}
//------------------------------------------------------------------------------------------------------------------------CHECK LETTERS
bool Session::check(char c){
    bool yes=false;
    char poss[24]={'A','B','C','D','E','F','G','H','I','J','K','L','M','1','2','3','4','5','6','7','8','9','0','R'};
    for(int y=0;y<24;y++){
        if(toupper(c)==poss[y]){yes=true;}
    }
    return yes;
}
//------------------------------------------------------------------------------------------------------------------------MIX LETTERS
void Session::mix(){//mixes letters
    for(int y=0;y<20;y++){
        char temp = letters[y];
        int random = rand() % 20;
        letters[y]=letters[random];
        letters[random]=temp;
    }
}
//------------------------------------------------------------------------------------------------------------------------PLACE WORD
void Session::placeWord(){//places word onto grid
    for(int j=0;j<(int)word.length();j++){
        if(direction==1){ grid[j+xpos][ypos]=word.at(j);
        } else if(direction==2){ grid[xpos][ypos+j]=word.at(j);}
    }
}
//------------------------------------------------------------------------------------------------------------------------UPDATE SCORE
void Session::updateScore(){
    int turnscore=0; bool dword=false;
    for(int h=0;h<word.length();h++){
        for(int i=0;i<26;i++){
            if(word.at(h)==alpha[i]){turnscore+=values[h];}
            if(direction==1){
                if(grid[h+xpos][ypos].at(0)=='+'){turnscore+=(((int)(grid[h+xpos][ypos].at(1)))-49)*10;
                }else if(grid[h+xpos][ypos].find("DL")){turnscore+=values[h];
                }else if(grid[h+xpos][ypos].find("DW")){ dword=true;}
            } else if(direction==2){
                    if(grid[xpos][h+ypos].at(0)=='+'){turnscore+=(((int)(grid[xpos][h+ypos].at(1)))-49)*10;
                    }else if(grid[xpos][h+ypos].find("DL")){ turnscore+=values[h];
                    }else if(grid[xpos][h+ypos].find("DW")){ dword=true;}
            }
        }
    }
    if(word.length()==13){turnscore+=1000;
    } else if(word.length()>10){turnscore+=1000;
    } else if(word.length()>7){turnscore*=10;
    } else if(word.length()>5){turnscore*=6;
    } else if(word.length()>4){turnscore*=4;
    } else if(word.length()>3){turnscore*=2;
    } else if(dword==true){turnscore*=2;}
    score+=turnscore;
}
//------------------------------------------------------------------------------------------------------------------------CHECK WORD FITS
bool Session::fits(){
    bool fit = true;
    if(xpos<0||xpos>12||ypos>12||ypos<0){fit=false;
    } else if(direction==1&&word.length()+xpos>12){fit=false;
    } else if(direction==2&&word.length()+ypos>12){fit=false;
    } else{
        for(int g=0;g<word.length()-1;g++){
            if(direction==1&&(grid[xpos+g][ypos].at(0)!=word.at(g)&&grid[xpos+g][ypos].at(0)!='@'&&grid[xpos+g][ypos].at(0)!='+')){
                if(grid[xpos+g][ypos].at(0)=='D'&&grid[xpos+g][ypos].length()==1){//checks if DW
                    fit=false;
                }
            } else if(direction==2&&(grid[xpos][ypos+g].at(0)!=word.at(g)&&grid[xpos+g][ypos].at(0)!='@'&&grid[xpos+g][ypos].at(0)!='+')){
                if(grid[xpos+g][ypos].at(0)=='D'&&grid[xpos+g][ypos].length()==1){//checks if DW
                    fit=false;
                }
            }
        }
    }
    return fit;
}
//------------------------------------------------------------------------------------------------------------------------REFRESH
void Session::refresh(){//clears all trackers for next turn
    direction=0; word=""; xpos=0; ypos=0; turn++;
    for(int y=0;y<20;y++){used[y]=0;}
}
//------------------------------------------------------------------------------------------------------------------------PRINT
void Session::print(){
    //prints header
    cout<<"\n|-----------------------------------------------------------------------------------------------------------|\n";
    cout<<"|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t|\n";
    cout<<"|\tTurn: "<<turn<<"\t\t\t\t\t\t\t\t\tW  O  R  D   F  L  O  W \t\t\t\t\t\t\tScore: "<<score<<"\t|\n";
    cout<<"|\t\t\t\t\t\t\t\t\t\t   -------------------------\t\t\t\t\t\t\t\t\t\t|\n";
    cout<<"|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t|\n";
    cout<<"|\t\t   1\t2\t 3\t  4\t   5\t6\t 7\t  8\t   9   10\t11\t 12\t  13\t\t\t  Letter Values\t\t\t|\n";
    cout<<"|\t\t|----|----|----|----|----|----|----|----|----|----|----|----|----|\t\t\t\t\t\t\t\t\t|\n";
    for(int i=0;i<13;i++){//prints grid
        cout<<"|\t "<<alpha[i]<<"\t|";
        for(int y=0;y<13;y++){
            if(grid[y][i].length()==1){
                cout<<" "<<grid[y][i]<<"  |";
            } else if(grid[y][i].length()==2){
                cout<<" "<<grid[y][i]<<" |";
            } else if(grid[y][i].length()==3){
                cout<<grid[y][i]<<" |";
            }else{cout<<"    |";}
        }
        cout<<"\t\t\t"<<alpha[i]<<" - "<<values[i]<<"\t\t"<<alpha[i+13]<<" - "<<values[i+13]<<"\t\t|\n";
        cout<<"|\t\t|----|----|----|----|----|----|----|----|----|----|----|----|----|\t\t\t\t\t\t\t\t\t|\n";
    }
    cout<<"|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t|\n";
    cout<<"|\t\t\t\t\t|-------------------------------------------|\t\t\t\t\t\t\t\t\t\t\t|\n";
    cout<<"|\t\t\t\t\t|";
    for(int r=0;r<20;r++){//prints all letters
        if(r==10){cout<<"\t|\t\t\t\t\t\tEnter 1 to\t\t\t|\n|\t\t\t\t\t|";}
        cout<<"\t"<<letters[r];
    }
    cout<<"\t|\t\t\t\t\tScramble Letters\t\t|\n|\t\t\t\t\t|-------------------------------------------|\t\t\t\t\t\t\t\t\t\t\t|";
    cout<<"\n|-----------------------------------------------------------------------------------------------------------|\n";
    cout<<"|  Enter Coordinates, Direction (Right-R or Down-D) & Word With Spaces Between | Example: \"A10 R Snowball\"  |";
    cout<<"\n|-----------------------------------------------------------------------------------------------------------|\n";
    cout<<"\n\t\t\t\t\t";
    /*for(int o=0;o<6;o++){
        cout<<specialSpots[o][0]<<"\t"<<specialSpots[o][1]<<"\tSS\n";
        cout<<grid[(int)specialSpots[o][0]][(int)specialSpots[o][1]]<<"\tGRID\n";
    }*/
}








