#include "Session.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <math.h>
#include <ctype.h>
#include <cmath>
#include <time.h>
#include <string.h>
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
    xyTOxy();      //add special spots
    bonusGen();   //add special spots
    lettGen(20);
    print();
    for(int x=0;x<13;x++){
        for(int y=0;y<13;y++){
            if(grid[x][y].length()==0){
                grid[x][y]=" ";
            }
        }
    }
    //cout<<"the end of setup!"<<endl;
}
//------------------------------------------------------------------------------------------------------------------------GET PLAYER MOVE INFO
void Session::extract(string command){   // A10 R snowball
    int end=0; int t=0; //tracks collected turn and word info
    char attempt[13];//holds letters to attempted word
    
    if(command.length()==0){
    	cout<<"Please enter a command"<<endl;
		return; 
	} 
    if(command.compare("ENDGAME")==0){
    	play=false;
    	return;
	}
    
    while(check(command.at(t))==true){
		t++; 
		if(t==command.length()){break;}}   // separate A10
    if(t>=4 || t==1){
    	cout<<"Wrong command format 1!  Please enter again!"<<endl;
    	return;
	}
	
	bool flag=false;

	//check the first part (A10) XX/XXX is valid or not 
    for(int i=0;i<13;i++){     
    	if(toupper(command.at(0))==alpha[i]){
    		flag=true;
    		break;
		}
	}
	if(flag==false){   // invalid, row is not from A to M
		cout<<"Wrong command format! May out of boundary or wrong character! Please enter again!"<<endl;
		return;
	}
	
	flag=false;
	if(t==2){    //A0-A9
		for(int i=0;i<=9;i++){
			if(command.at(1)-'0'-1==i){
				flag=true;
				break;
			}
		}
		if(flag==false){   // invalid, col is not from 0-9    
			cout<<"Wrong command format! Please enter the right number of the postion! Please enter again!"<<endl;
			return;
		}
	}

	
	
	flag=false; //A10-A13
	if(t==3){
		for(int i=0;i<=2;i++){
			if(command.at(2)-'0'-1==i){
				flag=true;
				break;
			}
		}
		if(flag==false){   // invalid, col is not from 10-12
			cout<<"Wrong command format! Please enter the right number of the postion! Please enter again!"<<endl;
			return;
		}
	}

    
    flag=false;
	ypos=toupper(command.at(0))-'A';    // the x coordinate of the initial position of a word
	if(t==3){
		xpos=(command.at(1)-'0')*10+(command.at(2)-'0'-1);
	}else{
		xpos=command.at(1)-'0'-1;
	}
	if(theFirstTime==true){   //have to start at the start position
		//cout<<"  ypos="<<ypos<<"  xpos="<<xpos<<"  startY="<<startY<<"  startX="<<startX<<endl;
		if(ypos==startY && xpos==startX ){
			flag=true;
		}
		if(flag==false){
			cout<<"Please choose the start postion (ST) to start your game!"<<endl;
			return;
		}
	}

	
    while(check(command.at(t))==false){   //detect space and illegal character
		if(command.at(t)==' '){
			t++;
		}else{
			cout<<"Wrong format 2! Illegal character! Please enter again!"<<endl;
			return;
		}
		if(t==command.length()){break;}
	}    
	
	//detect if the direction is correct
    if(toupper(command.at(t))=='R'){direction=1;
    } else if(toupper(command.at(t))=='D'){direction=2;}
    else{
    	cout<<"Please enter the right move ( R or D) !"<<endl;
    	return;
	}
    t++;
    
	flag=false; 
    while(!flag){
		for(int i=0;i<26;i++){
			if(toupper(command.at(t))==alpha[i]){
				flag=true;
				break;
			}else if(command.at(t)==' '){
				t++;
				break;
			}
		}
	}
	if(flag==false){
		cout<<"Wrong format 333! Illegal character! Please enter again!"<<endl;
		return;
	}		

	
	int wordL=0,temp=t; // the length of a word
	flag=false;
	
	word="";
	while(temp<command.length()){
		for(int i=0;i<26;i++){
			if(toupper(command.at(temp))==alpha[i]){
				flag=true;
				word=word+(char)toupper(command.at(temp));
				temp++;
				wordL++;
				break;
			}
		}
		if(flag==false){
			cout<<"Wrong format! Illegal character in the word! Please enter again!"<<endl;
		}else{
			flag=false;
			continue;
		}
	}
	
	
	int extra=0;
	for(int i=0;i<wordL;i++){
		flag=false;
		for(int w=0;w<20;w++){//cycles usable letters and grabs word
            if(toupper(word.at(i))==letters[w]){//checks for match and unused
                if(used[w]!=1){
                	flag=true;
					used[w]=1;//tracks used leters
					break;	
				}
            }
        }
        if(flag==false){
        	extraLetters[extra]=word.at(i);      // used letters don't included in scrabble for now, 
			isExist[extra]=-1;					// letters could be in pre-existing word. 
			extra++;
		}
	}
	if(theFirstTime==true)
		theFirstTime=false;
		
	//cout<<" ---------------word---------------"<<word<<endl;
    move(word);
    
}
//------------------------------------------------------------------------------------------------------------------------MOVE
void Session::move(string attempt){//makes move
    if(fits()){
    	attempt=newWord;
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
    specialSpots[0][1]=rand() % 4;
    startX=specialSpots[0][0];
    startY=specialSpots[0][1];
    grid[startX][startY]="ST";   //start point
    specialSpots[1][0] = 9+(rand() % 4);
    specialSpots[1][1] = 9+(rand() % 4);
    endX=specialSpots[1][0];
    endY=specialSpots[0][1];
    grid[(int)specialSpots[1][0]][(int)specialSpots[1][1]]="ED";   //end point
}
//------------------------------------------------------------------------------------------------------------------------BONUS GENERATOR
void Session::bonusGen(){
    srand((unsigned)time(0)); int y=-1;
    bool canput[5]={false,false,false,false,false};
    string perks[12]={"+10","+10","+10","+10","+20","+20","+20","+30","+30","+50","DL", "DW"};
    for(int x=2;x<7;x++){
        y++;
        while(canput[y]==false){//ensures spots are sufficiently far away
            specialSpots[x][0] = rand() % 13;
            specialSpots[x][1] = rand() % 13;
            if((specialSpots[x][0]!=startX && specialSpots[x][1]!=startY)&&
               (specialSpots[x][0]!=endY && specialSpots[x][1]!=endY)){
                if(x==2){
                    canput[y]=true;
                    grid[(int)specialSpots[x][0]][(int)specialSpots[x][1]]="+10";//perks[rand() % 10];
                } else if(x==3 && sqrt(pow(specialSpots[3][0]-specialSpots[2][0],2)+pow(specialSpots[3][1]-specialSpots[2][1],2))>3){
                    canput[y]=true;
                    grid[(int)specialSpots[x][0]][(int)specialSpots[x][1]]="+30";//perks[rand() % 10];
                } else if(x==4 && sqrt(pow(specialSpots[4][0]-specialSpots[2][0],2)+pow(specialSpots[4][1]-specialSpots[2][1],2))>3
                          && sqrt(pow(specialSpots[3][0]-specialSpots[4][0],2)+pow(specialSpots[3][1]-specialSpots[4][1],2))>3){
                    canput[y]=true;
                    grid[(int)specialSpots[x][0]][(int)specialSpots[x][1]]="+50";//perks[rand() % 2+10];
                } else if(x==5&&sqrt(pow(specialSpots[3][0]-specialSpots[5][0],2)+pow(specialSpots[3][1]-specialSpots[5][1],2))>3
                          &&sqrt(pow(specialSpots[5][0]-specialSpots[2][0],2)+pow(specialSpots[5][1]-specialSpots[2][1],2))>3
                          &&sqrt(pow(specialSpots[5][0]-specialSpots[4][0],2)+pow(specialSpots[5][1]-specialSpots[4][1],2))>3){
                    canput[y]=true;
                    grid[(int)specialSpots[x][0]][(int)specialSpots[x][1]]="DL";
                } else if(x==6 &&sqrt(pow(specialSpots[3][0]-specialSpots[6][0],2)+pow(specialSpots[3][1]-specialSpots[6][1],2))>3
                          &&sqrt(pow(specialSpots[6][0]-specialSpots[2][0],2)+pow(specialSpots[6][1]-specialSpots[2][1],2))>3
                          &&sqrt(pow(specialSpots[6][0]-specialSpots[4][0],2)+pow(specialSpots[6][1]-specialSpots[4][1],2))>3
						  && sqrt(pow(specialSpots[6][0]-specialSpots[5][0],2)+pow(specialSpots[6][1]-specialSpots[5][1],2))>3){
					canput[y]=true;
					grid[(int)specialSpots[x][0]][(int)specialSpots[x][1]]="DW"; 	
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
    bool flag=false;
    char poss[24]={'A','B','C','D','E','F','G','H','I','J','K','L','M','1','2','3','4','5','6','7','8','9','0','R'};
    for(int y=0;y<24;y++){
        if(toupper(c)==poss[y]){flag=true;break;}
    }
    return flag;
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
	//cout<<"direction============="<<direction;
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
//------------------------------------------------------------------------------------------------------------------------Get current player score
int Session::getScore(){
	return score;
}
//------------------------------------------------------------------------------------------------------------------------CHECK WORD FITS
bool Session::fits(){
    bool fit = true;
    if(xpos<0||xpos>12||ypos>12||ypos<0){
		fit=false;
    } else if(direction==1&&word.length()+xpos>12){
		fit=false;
    } else if(direction==2&&word.length()+ypos>12){
		fit=false;
    } else{
    	
    	int sameChar=0;    // count the number of the new word and the pre-existing word are same or not    	
		if(direction==1){
			
			//from start to the end the new word have to connect with the pre-existing word
			fit=false;
			for(int i=0;i<word.length();i++){
				if(grid[xpos+i][ypos].at(0)!=' ' && grid[xpos+i][ypos].length()==1 || grid[xpos+i][ypos].compare("ST")==0){
				 	fit=true;
				 	break;
				 }
			}
			if(fit==false){
				cout<<"The new word has to connect to the pre-existing words.!! Please enter again!"<<endl;
				return fit;
			}
			 
			for(int i=0;i<word.length();i++){
				if(grid[xpos+i][ypos].at(0)==' ' || grid[xpos+i][ypos].at(0)=='+' || 
				   (grid[xpos+i][ypos].at(0)=='S' && grid[xpos+i][ypos].length()==2) || (grid[xpos+i][ypos].at(0)=='E' && grid[xpos+i][ypos].length()==2) ||
				 (grid[xpos+i][ypos].at(0)=='D' && grid[xpos][ypos+i].length()==2 &&  (grid[xpos+i][ypos].at(1)=='W' || grid[xpos+i][ypos].at(1)=='L'))){   //empty, +score , DL and DW grid is all "canput" grid.
					continue;
				}else{
					if(toupper(word.at(i))!=grid[xpos+i][ypos].at(0)){   //new word character is not match with the pre-existing word
						fit=false;
						return fit;
					}else{
						sameChar++;
						bool flag=false;
						for(int j=0;j<20;j++){
							if(toupper(word.at(i))==letters[j] && used[j]==1){  // reset the letter because it is already existed
								used[j]=0;
								flag=true;
								break;
							}
						}
						if(flag==false){
							for(int j=0;j<strlen(extraLetters);j++){
								if(toupper(word.at(i))==extraLetters[j] && isExist[j]==-1){
									isExist[j]=1;
									flag=true;
									break;
								}
							}
						}
						if(flag==false){
							cout<<"The character you use may not exist on scrabble or board"<<endl;
							fit=false;
							return fit;
							
						}
					}
				}
			}
			
		}else{   // direction 2
			//from start to the end the new word have to connect with the pre-existing word
			//cout<<"  **********************IN HERE"<<endl;
			fit=false;
			for(int i=0;i<word.length();i++){
				if(grid[xpos][ypos+i].at(0)!=' ' && grid[xpos][ypos+i].length()==1 || grid[xpos][ypos+i].compare("ST")==0){
				 	fit=true;
				 	break;
				 }
			}
			if(fit==false){
				cout<<"The new word has to connect to the pre-existing words.!! Please enter again!"<<endl;
				return fit;
			}
			 
			for(int i=0;i<word.length();i++){
				if(grid[xpos][ypos+i].at(0)==' ' || grid[xpos][ypos+i].at(0)=='+' || 
				   (grid[xpos][ypos+i].at(0)=='S' && grid[xpos][ypos+i].length()==2) || (grid[xpos][ypos+i].at(0)=='E' && grid[xpos][ypos+i].length()==2)||
				 (grid[xpos][ypos+i].at(0)=='D' && grid[xpos][ypos+i].length()==2 && (grid[xpos][ypos+i].at(1)=='W' || grid[xpos][ypos+i].at(1)=='L'))){   //empty, +score , DL and DW grid is all "canput" grid.
					continue;
				}else{
					//cout<<word.at(i)<<"   *****  "<<grid[xpos][ypos+i]<<endl;
					if(toupper(word.at(i))!=grid[xpos][ypos+i].at(0)){   //new word character is not match with the pre-existing word
						fit=false;
						return fit;
					}else{
						sameChar++;
						bool flag=false;
						for(int j=0;j<20;j++){
							if(toupper(word.at(i))==letters[j] && used[j]==1){  // reset the letter because it is already existed
								used[j]=0;
								flag=true;
								break;
							}
						}
						if(flag==false){
							for(int j=0;j<strlen(extraLetters);j++){
								if(toupper(word.at(i))==extraLetters[j] && isExist[j]==-1){
									isExist[j]=1;
									flag=true;
									break;
								}
							}
						}
						if(flag==false){
							cout<<"The character you use may not exist on scrabble or board"<<endl;
							fit=false;
							return fit;
							
						}
					}
				}
			}
		}
		//cout<<"samechar="<<sameChar<<"  word.length()="<<word.length()<<endl;
		if(sameChar==word.length()){
			cout<<"You can not just retype the pre-existing word! Please enter again!"<<endl;
			fit=false;
			return fit;
		}
		
		
		
		//check if the new word is legal after fill in the grid
		newWord=word;
		if(fit!=false){
			if(direction==1){
				int txpos=xpos-1;
				while(txpos>=0){
					int flag=false;
					for(int i=0;i<26;i++){
						if(grid[txpos][ypos].at(0)==alpha[i] && grid[txpos][ypos].length()==1){
							flag=true;
						}
					}
					if(flag==true){
						newWord=grid[txpos][ypos].at(0)+newWord;
						txpos--;
						continue;
					}else{
						break;
					}
				}
				txpos=xpos+word.length();
				while(txpos<=12){
					int flag=false;
					for(int i=0;i<26;i++){
						if(grid[txpos][ypos].length()==1 && grid[txpos][ypos].at(0)==alpha[i]){
							flag=true;
						}
					}
					if(flag==true){
						newWord=newWord+grid[txpos][ypos].at(0);
						txpos++;
						continue;
					}else{
						break;
					}
				}
			}else{   //direction 2
				int typos=ypos-1;
				while(typos>=0){
					int flag=false;
					for(int i=0;i<26;i++){
						if(grid[xpos][typos].at(0)==alpha[i] && grid[xpos][typos].length()==1){
							flag=true;
						}
					}
					if(flag==true){
						newWord=grid[xpos][typos].at(0)+newWord;
						typos--;
						continue;
					}else{
						break;
					}
				}
				typos=ypos+word.length();
				while(typos<=12){
					int flag=false;
					for(int i=0;i<26;i++){
						if(grid[xpos][typos].length()==1 && grid[xpos][typos].at(0)==alpha[i]){
							flag=true;
						}
					}
					if(flag==true){
						newWord=newWord+grid[xpos][typos].at(0);
						typos++;
						continue;
					}else{
						break;
					}
				}
			}
			//cout<<"isWord:"<< scrabble->isWord(newWord)<<endl;
			if(scrabble->isWord(newWord)){
				fit=true;
			}else{
				fit=false;
				cout<<"It is not a word after it is filled in the grid! Please enter again!"<<endl;
			}
			
		}
		
    }
    return fit;
}
//------------------------------------------------------------------------------------------------------------------------REFRESH
void Session::refresh(){//clears all trackers for next turn
    direction=0; word=""; xpos=0; ypos=0; turn++;
    for(int y=0;y<20;y++){used[y]=0; extraLetters[y]=0; isExist[y]=0;}
    
}
//------------------------------------------------------------------------------------------------------------------------PRINT
void Session::print(){
	
    //prints header
    cout<<"\n|-----------------------------------------------------------------------------------------------------------|\n";
    cout<<"|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t|\n";
    cout<<"|\tTurn: "<<turn<<"\t\t\t\t\t\t\t\t\tW  O  R  D   F  L  O  W \t\t\t\t\t\t\tScore: "<<score<<"\t|\n";
    cout<<"|\t\t\t\t\t\t\t\t\t\t   -------------------------\t\t\t\t\t\t\t\t\t\t|\n";
    cout<<"|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t|\n";
    cout<<"|\t\t  1    2    3    4    5    6    7    8    9    10   11   12   13\t\t\t  Letter Values\t\t\t|\n";
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








