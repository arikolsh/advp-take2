#include "SmartPlayer.h"
#include "IBattleshipGameAlgoV2.h"
#include "ship.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>

#define EMPTY_CELL '-'
#define MARKED_CELL 'x'
#define INVALID_ATTACK { -1 , -1 } 
#define START_POINT { 1 , 1 }
#define A_NUM 0
#define B_NUM 1

bool print_mode = true;

//Constructor
SmartPlayer::SmartPlayer() : _attacking_state(Routine), _attack({-1,-1})
{
}

//Destructor
SmartPlayer::~SmartPlayer()
{
	//Empty for now
}

void SmartPlayer::setBoard(int player, const char** board, int numRows, int numCols)
{
	_playerNum = player;
	_rows = numRows, _cols = numCols;
	copyBoard(board); // Copy all player's ships to _board
	markPotentialHits(); // Mark all cells that are a "potential hit" (might hold an opponent ship)
	_pos = START_POINT;
	if(print_mode)
	{
		cout << endl << "player: " << _playerNum << endl;
		printBoard(false);
	}
	cout << endl;
}

// Copy all player's ships to _board (which is initialized with EMPTY_CELLs)
void SmartPlayer::copyBoard(const char** board)
{
	char cell;
	//First, Create a board of size numRows+2 x numCols+2 filled with EMPTY_CELLs:
	auto empty_line = string(_cols + 2, EMPTY_CELL); //empty_line = "_____..._"
	for (int i = 0; i < _rows + 2; i++)
	{
		_board.push_back(empty_line);
	}

	// Copy all player's ships to _board
	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _cols; j++)
		{
			cell = board[i][j];
			if (!Ship::isShip(cell)) { continue; } // Skip non-ship cells

			if ((_playerNum == A_NUM && cell == toupper(cell)) ||
				(_playerNum == B_NUM && cell == tolower(cell)))
			{
				_board[i + 1][j + 1] = cell; // Copy valid ship char
			}
		}
	}
}

/* _board is (_rows+2)x(_cols+2) so Loop only over non-padding cells.
* If a cell is isolated, it might hold an opponent ship
* so mark this cell as a potential hit 'x' */
void SmartPlayer::markPotentialHits()
{
	for (int i = 1; i < _rows + 1; i++)
	{
		for (int j = 1; j < _cols + 1; j++)
		{
			//_board[i][j] = potentialHit(i, j) ? MARKED_CELL : EMPTY_CELL;
			if(potentialHit(i, j))
			{
				_board[i][j] = MARKED_CELL;
			}
		}
	}
}

//check the cell itself and in addition check :down, up, left, right, upper left, upper right, down left, down right
bool SmartPlayer::potentialHit(int row, int col)
{
	if (_board[row][col] != EMPTY_CELL
		&& _board[row][col] != MARKED_CELL)
	{
		return false;
	}
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (abs(i) == abs(j)) { continue; } //Skip the diagonals
			if (_board[row + i][col + j] != EMPTY_CELL 
				&& _board[row + i][col + j] != MARKED_CELL)
			{
				return false; //Found one of the own player's ship in the surroundings
			}
		}
	}
	return true;
}

/*
 * OPTION 2
bool SmartPlayer::potentialHit(int row, int col)
{
	return
		(_board[row][col] == EMPTY_CELL || _board[row][col] == MARKED_CELL) && //the given cell
		(_board[row + 1][col] == EMPTY_CELL || _board[row + 1][col] == MARKED_CELL) && // down
		(_board[row - 1][col] == EMPTY_CELL || _board[row - 1][col] == MARKED_CELL) && // up
		(_board[row][col - 1] == EMPTY_CELL || _board[row][col - 1] == MARKED_CELL) && // left
		(_board[row][col + 1] == EMPTY_CELL || _board[row][col + 1] == MARKED_CELL) && // right
		(_board[row - 1][col - 1] == EMPTY_CELL || _board[row - 1][col - 1] == MARKED_CELL) && // upper left
		(_board[row - 1][col + 1] == EMPTY_CELL || _board[row - 1][col + 1] == MARKED_CELL) && // upper right
		(_board[row + 1][col - 1] == EMPTY_CELL || _board[row + 1][col - 1] == MARKED_CELL) && // down left
		(_board[row + 1][col + 1] == EMPTY_CELL || _board[row + 1][col + 1] == MARKED_CELL); // down right
}
*/


void SmartPlayer::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{

	//remember to check which player attacked
	//check own goal

	_lastAttack = { row, col, result , player };

	if(isOwnGoal(row, col, player))
	{
		return; // Should never enter this "if" block
	}

	if (player == _playerNum) // This player attacked
	{
		switch (result)
		{
		case AttackResult::Miss:
			switch (_attacking_state)
			{
			case Routine: // Keep attacking in Routine state until getting a 'Hit'
				//_attacking_state = Routine; 
				break;
			case Hunting_x: // Failed to hit on X direction --> Try to sink on Y direction
				_attacking_state = Hunting_y;
				break;
			case Hunting_y: // Failed to sink on both X and Y directions --> Back to Routine state
				_attacking_state = Routine;
				break;
			}
			break;

		case AttackResult::Hit:
			switch (_attacking_state)
			{
			case Routine:
				_attacking_state = Hunting_x; // Try to attack on X direction in order to sink this ship
				break;
			case Hunting_x:
				//_attacking_state = Hunting_x; // Found ship direction to be X --> attack until sink
				break;
			case Hunting_y: // Found ship direction to be Y --> attack until sink
				//_attacking_state = Hunting_y;
				break;
			}
			break;

		case AttackResult::Sink:
			switch (_attacking_state)
			{
			case Routine:
				//_attacking_state = Routine; // Update cells arround the sinked ship and keep attacking in Routine state
				break;
			case Hunting_x:
				_attacking_state = Routine; // Succeeded to sink ship on X direction --> return to Routine state
				break;
			case Hunting_y: // Succeeded to sink ship on Y direction --> return to Routine state
				_attacking_state = Routine;
				break;
			}
			break;
		}
	}

	else // Opponent attacked this player
	{
		switch (result)
		{
		case AttackResult::Miss:
			// Smile and wave :)
			break;

		case AttackResult::Hit:
			// If opponent hit his own ship, it revealed a target that smart player will try to sink:
			if (isOpponentOwnGoal(row, col, player))
			{
				_attacking_state = Hunting_x; // Try to sink ship on X direction
			}
			else
			{
				//Opponent hit a smart player's ship, nothing too do..
			}
			break;

		case AttackResult::Sink:
			// If opponent sank his own ship, update valid attack spots:
			if (isOpponentOwnGoal(row, col, player))
			{
				/////////////////////;
			}
			break;
		}
	}


}

bool SmartPlayer::isOwnGoal(int row, int col, int player) const
{
	return player == _playerNum && 
		_board[row + 1][col + 1] != EMPTY_CELL && 
		_board[row + 1][col + 1] != MARKED_CELL;
}

bool SmartPlayer::isOpponentOwnGoal(int row, int col, int player) const
{
	return player != _playerNum &&
		(_board[row + 1][col + 1] == EMPTY_CELL ||
		_board[row + 1][col + 1] == MARKED_CELL);
}

bool SmartPlayer::init(const std::string& path)
{
	return true;
}


std::pair<int, int> SmartPlayer::attack()
{
	//Find next marked cell:
	char cell;
	_attack = INVALID_ATTACK;
	if (_pos.first == -1) // Finished attacks
	{
		return _attack;
	}
	for (int i = _pos.first; i < _rows + 1; i++)
	{
		for (int j = _pos.second; j < _cols + 1; j++)
		{
			cell = _board[i][j];
			if (cell == MARKED_CELL) //Found next attack
			{
				_attack = { i, j };
				updatePosition(i,j);
				break;
			}
		}
	}
	return _attack;
}


void SmartPlayer::updatePosition(int i, int j)
{
	if (i < _rows+1 && j < _cols)
	{
		_pos = { i, j + 1 }; // Next column
		return;
	}
	if(i < _rows && j == _cols) // Next line
	{
		_pos = { i + 1, j };
		return;
	}
	// Finished attacks
	_pos = INVALID_ATTACK;
}


void SmartPlayer::printBoard(bool fullPrint) const
{
	for (int i = 0; i < _rows+2; i++) {
		for (int j = 0; j < _cols+2; j++) {
			cout << _board[i][j];
		}
		cout << endl;
	}
	/*
	int start, rowEnd, colEnd;
	start = fullPrint ? 0 : 1;
	rowEnd = fullPrint ? _rows+2 : _rows+1;
	colEnd = fullPrint ? _cols+2 : _cols+1;
	for (int i = start; i < rowEnd; i++) {
		for (int j = start; j < colEnd; j++) {
			cout << _board[i][j];
		}
		cout << endl;
	}
	 */
}
/*
void SmartPlayer::TEST_PLAYER()
{
	IBattleshipGameAlgo* smartPlayer = new SmartPlayer();
	char** boardTest = new char*[10];
	for (size_t i = 0; i < 10; i++)
	{
		boardTest[i] = new char[10];
	}
	boardTest[0][0] = 'd';
	boardTest[9][4] = 'b';
	boardTest[5][3] = 'b';
	boardTest[3][5] = 'm';
	boardTest[6][6] = 'm';
	boardTest[4][4] = 'm';
	boardTest[2][2] = 'd';
	boardTest[9][9] = 'd';
	smartPlayer->setBoard(0, const_cast<const char**>(boardTest), 10, 10);
}
*/
