#include <cstdlib>
#include "GameUtils.h"
#include <vector>
#include <filesystem>
#include "GameBoard.h"
#include "GameManager.h"
#include <iostream>


#include "SmartPlayer.h"
#include "PredictedPlayer.h"


#define NUM_PLAYERS 2
#define A_NUM 0
#define B_NUM 1
#define ROWS 10
#define COLS 10

using namespace std;

void getArgs(int argc, char** argv, bool& isQuiet, int& delay, string& searchDir)
{
	vector<string> argsVector(argv, argv + argc);
	for (int i = 1; i < argc; i++)
	{

		if (argsVector[i] == "-quiet")
		{
			isQuiet = true;
		}
		else if (argsVector[i] == "-delay")
		{
			delay = stoi(argsVector[++i]);
		}
		else
		{
			searchDir = argsVector[i];
			cout << searchDir;
		}
	}
	if(isQuiet)
	{
		delay = 0;
	}
}

int main(int argc, char* argv[])
{
	bool debug[]{ true,false,false };

	//SmartPlayer::TEST_PLAYER();
	//return 0;

	if (debug[0]) { //debug start
		vector<string> inputFiles = { "", "", "" }; //[battle board, dll 1, dll 2]
		vector<string> dllNames = { "", "" };
		vector<string> messages;
		bool isQuiet = false; //default
		int delay = 500; //default, milliseconds
		string searchDir = ""; //default
		char **playerBoardA, **playerBoardB;

		/* get command line arguments if any */
		getArgs(argc, argv, isQuiet, delay, searchDir);

		/* get DLLs and board file */
		int err = GameUtils::getInputFiles(inputFiles, messages, dllNames, searchDir);
		if (err) { return EXIT_FAILURE; }

		/* init game board data structure */
		GameBoard game_board(ROWS, COLS);
		game_board.init(inputFiles[0].c_str());

		/* get player boards */
		playerBoardA = game_board.getPlayerBoard(A_NUM);
		if (playerBoardA == nullptr) { return EXIT_FAILURE; }
		playerBoardB = game_board.getPlayerBoard(B_NUM);
		if (playerBoardB == nullptr)
		{
			GameBoard::freeBoard(playerBoardA, 12, 12);
			return EXIT_FAILURE;
		}

		/* init game manager */
		GameManager manager(&game_board, isQuiet, delay);

		/* declare players */
		IBattleshipGameAlgo* players[NUM_PLAYERS];

		//todo: get algorithms

		/* init player A */
		players[A_NUM] = new PredictedPlayer(A_NUM);
		if (players[A_NUM]->init(searchDir) == false)
		{

			//Print something on each EXIT_FAILURE

			/* free resources */
			GameBoard::freeBoard(playerBoardA, 12, 12);  // fix to 10,10 !!
			GameBoard::freeBoard(playerBoardB, 12, 12);
			delete players[A_NUM];
			return EXIT_FAILURE;
		}
		players[A_NUM]->setBoard(A_NUM, const_cast<const char **>(playerBoardA), 12, 12); //should send 10,10 since the Bodek will call setboard with (10,10)..
		GameBoard::freeBoard(playerBoardA, 12, 12); //Not needed once A has set his own board

		/* init player B */
		players[B_NUM] = new PredictedPlayer(B_NUM);
		if (players[B_NUM]->init(searchDir) == false)
		{
			/* free resources */
			GameBoard::freeBoard(playerBoardB, 12, 12);
			delete players[A_NUM];
			delete players[A_NUM];
			return EXIT_FAILURE;
		}
		players[B_NUM]->setBoard(B_NUM, const_cast<const char **>(playerBoardB), 12, 12); //should send 10,10 since the Bodek will call setboard with (10,10)..
		GameBoard::freeBoard(playerBoardB, 12, 12); //Not needed once B has set his own board

		/* game execution */
		int winner = manager.runGame(players);

		/* output results */
		manager.gameOver(winner);

		/* freeing resources */
		delete players[A_NUM];
		delete players[B_NUM];
	} //debug end

	if (debug[1])
	{
		bool isQuiet = false; //default
		int delay = 500; //default, milliseconds
		string searchDir = ""; //default
		getArgs(argc, argv, isQuiet, delay, searchDir);
		vector<string> inputFiles = { "", "", "" }; //[battle board, dll 1, dll 2]
		vector<string> dllNames = { "", "" };
		vector<string> messages;
													 /* get DLLs and board file */
		int err = GameUtils::getInputFiles(inputFiles, messages, dllNames, searchDir);
		if (err) { return EXIT_FAILURE; }

		/* init game board data structure */
		GameBoard game_board(ROWS, COLS);
		game_board.init(inputFiles[0].c_str());
		//game_board.printBoard(true);

		/* get player boards */
		char** playerBoardA = game_board.getPlayerBoard(A_NUM);
		if (playerBoardA == nullptr) { return EXIT_FAILURE; }
		//GameBoard::printBoard(playerBoardA,12,12,false);
		game_board.draw();
	}
	return EXIT_SUCCESS;

}
