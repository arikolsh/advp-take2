#pragma once
#include <vector>
#include <map>
#include <memory>
#include "Ship.h"
#include "IBattleshipGameAlgoV2.h"
using namespace std;
class GameBoard { //todo: add comments
public:
	GameBoard(int rows, int cols);
	~GameBoard();
	int init(string path);
	vector<string> getFullBoard() const;
	char** getPlayerBoard(int player) const;
	void printBoard(bool fullPrint) const;
	map<pair<int, int>, pair<shared_ptr<Ship>, bool> > getShipsMap();
	static void freeBoard(char** board, int rows, int cols);
private:
	int _rows;
	int _cols;
	/* The full board for gui purposes*/
	vector<string> _fullBoard;
	/* The game board is implemented using a private map.
	* Each map entry holds a board coordinate (like a matrix cell) as key,
	* and a ship object as value. Since each ship can take a few cells,
	* different keys may hold the same ship object. */
	map<pair<int, int>, pair<shared_ptr<Ship>, bool> > _shipsMap;
	/*disable copy ctor*/
	GameBoard(const GameBoard& that) = delete;
	/* fill fullBoard with empty cell symbol. */
	void cleanBoard(char** board) const;
	/* return 2d char array. return null ptr if error. */
	char** getCleanBoard(bool clean) const;
	/* fill board with data from board file in path. */
	int fillBoardFromFile(string path);
	/* check if board is valid and return 0 if so. return -1 if invalid and
	* print proper messages. */
	int validateBoard();
	/* remove ships with wrong shape or size from board. returns -1 if there are
	* invalid ships and prints proper messages. */
	bool markInvalidShips(vector<string> boardCpy);
	/* Each player board is prepared in advance and hidden from the opponent!
	* fillPlayerBoard gets the full board with both players ships,
	* and fills the given player's board with his ships only. */
	int fillMapWithShips();
};
