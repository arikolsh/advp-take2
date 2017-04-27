#include <cstdlib>
#include <iostream>
#include "GameUtils.h"
#include "Player.h"
#include <vector>
#include <filesystem>
#include "GameBoard.h"
#include "GameManager.h"
using namespace std;
#define NUM_PLAYERS 2
#define A_NUM 0
#define B_NUM 1
void GameOver(int winner, GameManager& manager);

int main(int argc, char* argv[])
{
	bool debug = false;
	if (debug) { //debug start
		int currentPlayerNum, winner = -1;
		//finishedAttacks[i] is true iff players[i] finished all his attacks
		bool finishedAttacks[NUM_PLAYERS] = { false,false };
		pair<int, int> attackPoint;
		AttackResult attackResult;
		vector<string> inputFiles = { "", "", "" }; //[battle board, dll 1, dll 2]
		vector<string> dllNames = { "", "" };
		vector<string> messages;
		string searchDir = argc >= 1 ? "" : argv[1]; //todo: check if quiet\path
		////////// get DLLs and board file //////////
		int err = GameUtils::getInputFiles(inputFiles, messages, dllNames, argc, searchDir);
		if (err)
		{
			return EXIT_FAILURE;
		}
		GameBoard game_board(10, 10);
		game_board.init(inputFiles[0].c_str());
		char** playerBoardA = game_board.getPlayerBoard(A_NUM);
		if (playerBoardA == nullptr)
		{
			return EXIT_FAILURE;
		}
		char** playerBoardB = game_board.getPlayerBoard(B_NUM);
		if (playerBoardB == nullptr)
		{
			GameBoard::freeBoard(playerBoardA, 12, 12);
			return EXIT_FAILURE;
		}
		GameManager manager(&game_board);
		IBattleshipGameAlgo * players[NUM_PLAYERS];
		//todo: get algorithms

		////////// init player A //////////
		players[A_NUM]; // = new PredictedPlayer(); //=GetAlgorithm();
		players[A_NUM]->setBoard(A_NUM, const_cast<const char **>(playerBoardA), 12, 12);
		if (players[A_NUM]->init(searchDir))
		{
			GameBoard::freeBoard(playerBoardA, 12, 12);
			GameBoard::freeBoard(playerBoardB, 12, 12);
			return EXIT_FAILURE;
		}
		GameBoard::freeBoard(playerBoardA, 12, 12);

		////////// init player B //////////
		players[B_NUM]; // = new PredictedPlayer(); //=GetAlgorithm();
		players[B_NUM]->setBoard(B_NUM, const_cast<const char **>(playerBoardB), 12, 12);
		if (players[B_NUM]->init(searchDir))
		{
			GameBoard::freeBoard(playerBoardB, 12, 12);
			return EXIT_FAILURE;
		}
		GameBoard::freeBoard(playerBoardB, 12, 12);

		// Each player declares his next attack.
		// Then, his enemy executes the attack and returns the AttackResult.
		// If the player hits an enemy's ship, he gets another turn
		// If the player make an own goal, he doesn't get another turn
		// Game is over once a player loses all his ships, or all attackes were taken.

		////////// game execution //////////
		while (true)
		{
			currentPlayerNum = manager.getCurrentPlayer();
			//Player declares his next attack:
			attackPoint = players[currentPlayerNum]->attack();
			if (attackPoint.first == -1 && attackPoint.second == -1)
			{
				finishedAttacks[currentPlayerNum] = true;
			}
			if (finishedAttacks[0] && finishedAttacks[1])
			{ //both players finished all their attacks
				break;
			}
			attackResult = manager.executeAttack(1 - currentPlayerNum, attackPoint);
			players[A_NUM]->notifyOnAttackResult(currentPlayerNum, attackPoint.first, attackPoint.second, attackResult);
			players[B_NUM]->notifyOnAttackResult(currentPlayerNum, attackPoint.first, attackPoint.second, attackResult);
			//check for defeated players
			if (manager.isPlayerDefeated(1 - currentPlayerNum))
			{
				//current player sunk all opponent's ships
				winner = currentPlayerNum; //winner is the current player
				break;
			}
			if (manager.isPlayerDefeated(currentPlayerNum))
			{
				//current player sunk his own last ship
				winner = 1 - currentPlayerNum; //winner is the opponent
				break;
			}
		}

		////////// output results //////////
		GameOver(winner, manager);

		////////// freeing resources //////////
		delete players[A_NUM];
		delete players[B_NUM];
	} //debug end
	return EXIT_SUCCESS;

}

void GameOver(int winner, GameManager& manager)
{
	if (winner != -1) //We have a winner
	{
		cout << "Player " << (winner == A_NUM ? "A" : "B") << " won" << endl;
	}
	cout << "Points:" << endl;
	cout << "Player A: " << manager.getPlayerScore(A_NUM) << endl;
	cout << "Player B: " << manager.getPlayerScore(B_NUM) << endl;
}