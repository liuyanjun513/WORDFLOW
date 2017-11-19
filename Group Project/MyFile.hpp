#ifndef MYFILE_H
#define MYFILE_H
#include <map>
#include "Player.hpp"
using namespace std;
class MyFile{
	protected:
		map<string,int> m;
	public:
		MyFile();
		~MyFile();
		void readScore();
		map<string,int> getMap(){
			return m;
		}
	
	
};
#endif
