#pragma once

#include "IBattleshipGameAlgoV2.h"
#include <map>
#include "Ship.h"
#include "GameUtils.h"
#include <fstream>
#include <memory>

using namespace std;

class Player : public IBattleshipGameAlgo {
public:
	// Class constructor
	explicit Player(int playerNum);

	//void setBoard(const char **board, int numRows, int numCols) override;
	void Player::setBoard(int player, const char** board, int numRows, int numCols) override;

	void printPlayerBoard();

	int getplayerNum() const;

	int getPlayerScore() const;

	void incrementPlayerScore(int points);

	//Set to true when a player finishes all his attacks in file
	bool finishedAttacks() const;

	//Set to true by player if enemy destroyed all his ships
	bool isDefeated() const;


private:
	int _playerNum;
	int _playerScore;
	bool _finishedAttacks;
	int _numActiveShips; //number of active ships
	vector<string> _board;
};