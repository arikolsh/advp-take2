#include "NaivePlayer.h"

#define EMPTY_CELL ' '

using namespace std;

NaivePlayer::NaivePlayer(int playerNum)
{
	//manual initialization isn't needed for _player_attcks and _board (auto initialization for vectors)
	_playerNum = playerNum;
	_attackPosition = 0;
	_rows = 0;
	_cols = 0;
	_attackPosition = 0;
}

void NaivePlayer::setBoard(int player, const char ** board, int numRows, int numCols)
{
	_rows = numRows;
	_cols = numCols;
	for (int i = 0; i < numRows; i++) {
		_board.push_back(board[i]);
	}
}
bool NaivePlayer::isIsolated(int row, int col)
{
	//check the cell itself and in addition check :down, up, left, right, upper left, upper right, down left, down right
	return !(_board[row][col] != EMPTY_CELL ||
		_board[row + 1][col] != EMPTY_CELL || // down
		_board[row - 1][col] != EMPTY_CELL || // up
		_board[row][col - 1] != EMPTY_CELL || // left
		_board[row][col + 1] != EMPTY_CELL || // right
		_board[row - 1][col - 1] != EMPTY_CELL || // upper left
		_board[row - 1][col + 1] != EMPTY_CELL || // upper right
		_board[row + 1][col - 1] != EMPTY_CELL || // down left
		_board[row + 1][col + 1] != EMPTY_CELL);  // down right
}

bool NaivePlayer::init(const std::string & path)
{
	int i = 1, j = 1;
	// go from (1,1) until (11, 11) inclusive for both
	while (i < _rows - 1)
	{
		j = 1;
		while (j < _cols - 1)
		{
			if (isIsolated(i, j) == false)
			{
				j += 2;
				continue;
			}
			_playerAttacks.push_back(pair<int, int>(i, j));
			j++;
		}
		i++;
	}
	return true;
}

pair<int, int> NaivePlayer::attack()
{
	if (_playerAttacks.size() == 0)
	{
		return pair<int, int>(-1, -1);
	}
	return _playerAttacks[++_attackPosition]; //TODO: check that it's actually return the value in the index and just then added 1 to it
}

void NaivePlayer::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	//Do nothing for now
}

NaivePlayer::~NaivePlayer()
{
	//Do nothing for now
}