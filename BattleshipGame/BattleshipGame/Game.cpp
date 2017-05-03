#include <cstdlib>
#include "GameUtils.h"
#include <vector>
#include <filesystem>
#include "GameBoard.h"
#include "GameManager.h"
#include <iostream>
#include "NaivePlayer.h"
using namespace std;
#define NUM_PLAYERS 2
#define A_NUM 0
#define B_NUM 1
#define ROWS 10
#define COLS 10

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
	if (isQuiet)
	{
		delay = 0;
	}
}

int main(int argc, char* argv[])
{
	bool debug[]{ false,true,false };

	if (debug[0]) { //debug start
		vector<string> inputFiles = { "", "", "" }; //[battle board, dll 1, dll 2]
		vector<string> dllNames = { "", "" };
		vector<string> messages;
		bool isQuiet = false; //default
		int delay = 500; //default, milliseconds
		string searchDir = ""; //default

							   /* get command line arguments if any */
		getArgs(argc, argv, isQuiet, delay, searchDir);

		/* get DLLs and board file */
		int err = GameUtils::getInputFiles(inputFiles, messages, dllNames, searchDir);
		if (err) { return EXIT_FAILURE; }

		/* init game board data structure */
		GameBoard game_board(ROWS, COLS);
		game_board.init(inputFiles[0].c_str());

		/* get player boards */
		char** playerBoardA = game_board.getPlayerBoard(A_NUM);
		if (playerBoardA == nullptr) { return EXIT_FAILURE; }
		char** playerBoardB = game_board.getPlayerBoard(B_NUM);
		if (playerBoardB == nullptr)
		{
			GameBoard::freeBoard(playerBoardA, ROWS, COLS);
			return EXIT_FAILURE;
		}

		/* init game manager */
		GameManager manager(&game_board, isQuiet, delay);

		/* declare players */
		IBattleshipGameAlgo * players[NUM_PLAYERS];

		//todo: get algorithms
		/* init player A */
		players[A_NUM]; // = new PredictedPlayer(); //=GetAlgorithm();
		players[A_NUM]->setBoard(A_NUM, const_cast<const char **>(playerBoardA), ROWS, COLS);
		if (players[A_NUM]->init(searchDir))
		{
			GameBoard::freeBoard(playerBoardA, ROWS, COLS);
			GameBoard::freeBoard(playerBoardB, ROWS, COLS);
			return EXIT_FAILURE;
		}
		GameBoard::freeBoard(playerBoardA, ROWS, COLS);

		/* init player B */
		players[B_NUM]; // = new PredictedPlayer(); //=GetAlgorithm();
		players[B_NUM]->setBoard(B_NUM, const_cast<const char **>(playerBoardB), ROWS, COLS);
		if (players[B_NUM]->init(searchDir))
		{
			GameBoard::freeBoard(playerBoardB, ROWS, COLS);
			return EXIT_FAILURE;
		}
		GameBoard::freeBoard(playerBoardB, ROWS, COLS);

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
		game_board.printBoard(false);
		char** playerBoardA = game_board.getPlayerBoard(A_NUM);
		if (playerBoardA == nullptr) { return EXIT_FAILURE; }
		char** playerBoardB = game_board.getPlayerBoard(B_NUM);
		if (playerBoardB == nullptr)
		{
			GameBoard::freeBoard(playerBoardA, 12, 12);
			return EXIT_FAILURE;
		}
		/* init game manager */
		GameManager manager(&game_board);
		/* declare players */
		IBattleshipGameAlgo * players[NUM_PLAYERS];
		//todo: get algorithms
		/* init player A */
		players[A_NUM] = new NaivePlayer(A_NUM);
		players[A_NUM]->setBoard(A_NUM, const_cast<const char **>(playerBoardA), 12, 12);
		if (players[A_NUM]->init(searchDir))
		{
			GameBoard::freeBoard(playerBoardA, 12, 12);
			GameBoard::freeBoard(playerBoardB, 12, 12);
			return EXIT_FAILURE;
		}
		GameBoard::freeBoard(playerBoardA, 12, 12);
		/* init player B */
		players[B_NUM] = new NaivePlayer(B_NUM);
		players[B_NUM]->setBoard(B_NUM, const_cast<const char **>(playerBoardB), 12, 12);
		if (players[B_NUM]->init(searchDir))
		{
			GameBoard::freeBoard(playerBoardB, 12, 12);
			return EXIT_FAILURE;
		}
		GameBoard::freeBoard(playerBoardB, 12, 12);
		/* game execution */
		int winner = manager.runGame(players);
		/* output results */
		//GameBoard::printBoard(playerBoardA,12,12,false);
		game_board.draw();
	}
	return EXIT_SUCCESS;

}
