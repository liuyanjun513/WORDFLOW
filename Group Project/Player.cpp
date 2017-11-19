#include "Player.hpp"
Player::Player(){
	
}
Player::Player(string name){
	this->name=name;
	this->score=0;
}

void Player::setScore(int score){
	this->score=score;
}
int Player::getScore(){
	return this->score;
}
Player::~Player(){}



