#include <cstdlib>
#include "GameUtils.h"
#include <vector>
#include <filesystem>
#include "GameBoard.h"
#include "GameManager.h"
#include "NaivePlayer.h"
using namespace std;
#define NUM_PLAYERS 2
#define A_NUM 0
#define B_NUM 1

int main(int argc, char* argv[])
{
	bool debug[]{ false,true };
	if (debug[0]) { //debug start
		vector<string> inputFiles = { "", "", "" }; //[battle board, dll 1, dll 2]
		vector<string> dllNames = { "", "" };
		vector<string> messages;
		string searchDir = argc >= 1 ? "" : argv[1]; //todo: check if quiet\path
		/* get DLLs and board file */
		int err = GameUtils::getInputFiles(inputFiles, messages, dllNames, argc, searchDir);
		if (err) { return EXIT_FAILURE; }
		/* init game board data structure */
		GameBoard game_board(10, 10);
		game_board.init(inputFiles[0].c_str());
		/* get player boards */
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
		players[A_NUM]; // = new PredictedPlayer(); //=GetAlgorithm();
		players[A_NUM]->setBoard(A_NUM, const_cast<const char **>(playerBoardA), 12, 12);
		if (players[A_NUM]->init(searchDir))
		{
			GameBoard::freeBoard(playerBoardA, 12, 12);
			GameBoard::freeBoard(playerBoardB, 12, 12);
			return EXIT_FAILURE;
		}
		GameBoard::freeBoard(playerBoardA, 12, 12);
		/* init player B */
		players[B_NUM]; // = new PredictedPlayer(); //=GetAlgorithm();
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
		manager.gameOver(winner);
		/* freeing resources */
		delete players[A_NUM];
		delete players[B_NUM];
	} //debug end

	if(debug[1])
	{
		vector<string> inputFiles = { "", "", "" }; //[battle board, dll 1, dll 2]
		vector<string> dllNames = { "", "" };
		vector<string> messages;
		string searchDir = argc >= 1 ? "" : argv[1]; //todo: check if quiet\path
													 /* get DLLs and board file */
		int err = GameUtils::getInputFiles(inputFiles, messages, dllNames, argc, searchDir);
		if (err) { return EXIT_FAILURE; }
		/* init game board data structure */
		GameBoard game_board(10, 10);
		game_board.init(inputFiles[0].c_str());
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
	}
	return EXIT_SUCCESS;

}
