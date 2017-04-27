#include "Player.h"
#include <iostream>
#include <string>

// Constructor
Player::Player(int playerNum) {
	_playerNum = playerNum; //0 is A 1 is B
	_playerScore = 0; //updated by notify
	_finishedAttacks = false; 
	_numActiveShips = 5;
}

int Player::getplayerNum() const
{
	return _playerNum;
}

int Player::getPlayerScore() const
{
	return _playerScore;
}

void Player::incrementPlayerScore(int points)
{
	if (points > 0)
	{
		_playerScore += points;
	}
}

bool Player::finishedAttacks() const
{
	return _finishedAttacks;
}

bool Player::isDefeated() const
{
	//todo: is this neccessary  ? if GameBoard oversees everything
	return _numActiveShips <= 0;
}

void Player::setBoard(int player,const char** board, int numRows, int numCols) {
	for (int i = 0; i < numRows; i++) {
		_board.push_back(board[i]);
	}
}

void Player::printPlayerBoard()
{
	for (vector<string>::const_iterator rowIter = _board.begin(); rowIter != _board.end(); ++rowIter)
		cout << *rowIter << endl;
}