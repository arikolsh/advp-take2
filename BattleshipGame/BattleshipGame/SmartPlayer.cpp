#include "SmartPlayer.h"
#include "IBattleshipGameAlgoV2.h"
#include <algorithm>
#include <vector>


#define EMPTY_CELL '_'
#define MARKED 'x'
#define MARKED_LINE "xxxxxxxxxxxx"


//Constructor
SmartPlayer::SmartPlayer(int playerNum)
{
	_playerNum = playerNum;
	_playerAttacks = {};
	_attackPosition = {-1, -1};
	_attackIndex = 0;
	_board = {};
}

//Destructor
SmartPlayer::~SmartPlayer()
{
	//Empty for now
}

// Gets a 10 x 10 board and copies all ships to a private 12 x 12 board
void SmartPlayer::setBoard(int player, const char** board, int numRows, int numCols)
{
	auto marked_line = string(numCols + 2, MARKED); //marked_line = "xxxx...x"
	for (int i = 0; i < numRows + 2; i++)
	{
		_board.push_back(marked_line);
	}
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (board[i][j] != EMPTY_CELL)
			{
				_board[i][j] = MARKED;

				//Mark the surroundings

			}
			_board[i][j] = board[i][j];
		}
	}
}

void SmartPlayer::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
}


bool SmartPlayer::init(const std::string& path)
{
	return true;
}


std::pair<int, int> SmartPlayer::attack()
{
	return _playerAttacks[_attackIndex];
}

bool SmartPlayer::potentialHit(int row, int col)
{
	//check the cell itself and in addition check :down, up, left, right, upper left, upper right, down left, down right
	return (_board[row][col] == EMPTY_CELL &&
		_board[row + 1][col] == EMPTY_CELL && // down
		_board[row - 1][col] == EMPTY_CELL && // up
		_board[row][col - 1] == EMPTY_CELL && // left
		_board[row][col + 1] == EMPTY_CELL && // right
		_board[row - 1][col - 1] == EMPTY_CELL && // upper left
		_board[row - 1][col + 1] == EMPTY_CELL && // upper right
		_board[row + 1][col - 1] == EMPTY_CELL && // down left
		_board[row + 1][col + 1] == EMPTY_CELL);  // down right
}