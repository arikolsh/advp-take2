#pragma once

#include "IBattleshipGameAlgoV2.h"
#include <vector>

using namespace std;

class SmartPlayer : public IBattleshipGameAlgo
{
public:

	// Class constructor
	explicit SmartPlayer(int playerNum);

	// Class distructor
	~SmartPlayer();

	/* Called once to allow init from files if needed
	* returns whether the init succeeded or failed */
	bool init(const std::string& path) override;

	// called once to notify player on his board
	void setBoard(int player, const char** board, int numRows, int numCols) override;

	// ask player for his move
	std::pair<int, int> attack() override;

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;	// notify on last move result

private:

	// Private fields:
	int _playerNum;
	vector<pair <int, int>> _playerAttacks; //A vector with all player attacks
	pair<int, int> _attackPosition; //Position in board
	int _attackIndex; //Next attack index in _playerAttacks vector
	vector<string> _board; //Holds all player's ships + 'x' mark for every cell that shouldn't be attacked
	
	// Private functions:
	bool potentialHit(int row, int col); //check if the cell is empty and in addition check :down, up, left, right, upper left, upper right, down left, down right
};