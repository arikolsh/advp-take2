#include <cstdlib>
#include <iostream>
#include "GameUtils.h"
#include "Player.h"
#include <vector>
#include <filesystem>
#include "GameBoardManager.h"
#define SUCCESS 0
#define ERROR -1
using namespace std;
#define MAX_PATH 1024
#define NUM_PLAYERS 2
#define OPPONENT(i) (1 - i)
#define PLAYER(i) ((i==0) ? "A" : "B")
#define A_NUM 0
#define B_NUM 1
void GameOver(int winner, Player*(players)[2]);


int main(int argc, char* argv[])
{

	////////////// FLOW //////////////

	//GET INPUT FILES: DLL, BOARD //done

	//INIT GAME_BOARD INSTANCE//done

	//DECLARE PLAYERS, INIT PLAYERS ARRAY

	//GET ALGORITHMS

	//GET PLAYER BOARD A

	//GET PLAYER BOARD B

	//SETBOARD() IN EACH PLAYER

	//INIT() PLAYERS -> IF PREDICTED FIND IN PATH ATTACK FILES //todo: brainstorm how to handle only one attack file

	//START GAME ITERATIONS

	////////////// END OF FLOW //////////////

	//Variables to be used:
	//int err, i, winner = -1;
	//Player* players[2];
	//pair<int, int> attackPoint;
	//pair <AttackResult, int> attackResultAndScore, ownGoalResultAndScore;
	//char boards[3][FULL_BOARD_LEN][FULL_BOARD_LEN]; /* [playerA's board, playerB's board, full board]  */

	vector<string> inputFiles = { "", "", "" }; //[battle board, dll 1, dll 2]
	vector<string> dllNames = { "", "" };
	vector<string> messages;

	//Prepare the game:
	int err = GameUtils::getInputFiles(inputFiles, messages, dllNames, argc, argv);
	if (err == ERROR)
	{
		return EXIT_FAILURE;
	}
	GameBoardManager game_board_manager(10, 10);
	game_board_manager.init(inputFiles[0].c_str());
	char** playerBoardA = game_board_manager.getPlayerBoard(A_NUM);
	if (playerBoardA == nullptr)
	{
		return EXIT_FAILURE;
	}
	char** playerBoardB = game_board_manager.getPlayerBoard(B_NUM);
	if (playerBoardB == nullptr)
	{
		GameBoardManager::freeBoard(playerBoardA,12,12);
		return EXIT_FAILURE;
	}
	///todo: getPlayerBoards
	//todo: players declare
	//todo: get algorithms
	//todo: players setboard() and init()
	GameBoardManager::freeBoard(playerBoardA, 12, 12);
	GameBoardManager::freeBoard(playerBoardB, 12, 12);
	//todo: start iterations

	////Initialize player A and player B:
	//for (i = 0; i < 2; i++)
	//{
	//	players[i] = new Player(i, inputFiles[i+1].c_str());
	//	//GameUtils::fillPlayerBoard(i, boards[i], boards[2]);
	//	players[i]->setBoard(boards[i], FULL_BOARD_LEN, FULL_BOARD_LEN);
	//}

	///*Start the Game with playerA.
	//* Each player declares his next attack.
	//* Then, his enemy executes the attack and returns the AttackResult.
	//* If the player hits an enemy's ship, he gets another turn!
	//* If it's a 'Miss' the game checks for an 'own goal'.
	//* Game is over once a player loses all his ships, or all attackes were taken.*/
	//i = 0;
	//while (true)
	//{		
	//	//Player declares his next attack:
	//	attackPoint = players[i]->attack();
	//	
	//	//Check if player executed all attacks, If so, Game over (only scores are printed):
	//	if (players[i]->finishedAttacks() || attackPoint.first == -1 )
	//	{
	//		if (players[OPPONENT(i)]->finishedAttacks())
	//		{
	//			break; //Both players finished their attack files --> Game Over
	//		}
	//		//otherwise, pass turn to opponent:
	//		i = OPPONENT(i); 
	//		continue; 
	//	}

	//	//Executes the player's attack on the opponent's board:
	//	attackResultAndScore = players[OPPONENT(i)]->execute(attackPoint);
	//	
	//	//Update score in case of a 'Sink' result:
	//	//(Incremented only if score > 0)
	//	players[i]->incrementPlayerScore(attackResultAndScore.second);

	//	//check for an 'own goal': in case of a miss on the opponents board, 
	//	//we check for a self hit on the attacker's board:
	//	if(attackResultAndScore.first == AttackResult::Miss)
	//	{
	//		ownGoalResultAndScore = players[i]->execute(attackPoint);
	//		if (ownGoalResultAndScore.first != AttackResult::Miss) //It's an own goal..
	//		{
	//			//In case of a 'Sink' increment the opponents score:
	//			players[OPPONENT(i)]->incrementPlayerScore(ownGoalResultAndScore.second);
	//			//Notify both players on Attack Result 
	//			//(own goal is considered as if the opponent attacked):
	//			players[i]->notifyOnAttackResult(OPPONENT(i), attackPoint.first, attackPoint.second, ownGoalResultAndScore.first);
	//			players[OPPONENT(i)]->notifyOnAttackResult(OPPONENT(i), attackPoint.first, attackPoint.second, ownGoalResultAndScore.first);
	//			//If all the player's ships are destroyed it's GameOver (by own goal):
	//			if (players[i]->isDefeated())
	//			{
	//				winner = OPPONENT(i); //Opponent wins :(
	//				break;
	//			}
	//		}
	//		i = OPPONENT(i);
	//		continue;
	//	}
	//	
	//	//Notify both players on Attack Result:
	//	players[i]->notifyOnAttackResult(i, attackPoint.first, attackPoint.second, attackResultAndScore.first);
	//	players[OPPONENT(i)]->notifyOnAttackResult(i, attackPoint.first, attackPoint.second, attackResultAndScore.first);
	//	
	//	//If all enemy's ships were destroyed it's GameOver:
	//	if (players[OPPONENT(i)]->isDefeated())
	//	{
	//		winner = i;
	//		break;
	//	}

	//	//If "Sink' or 'Hit' (for the first time) --> Current player gets another turn:
	//	if ( (attackResultAndScore.first == AttackResult::Hit 
	//		|| attackResultAndScore.first == AttackResult::Sink) 
	//		&& attackResultAndScore.second != -1 )
	//	{
	//		continue;
	//	}

	//	i = OPPONENT(i); //Otherwise, pass turn to the opponent
	//}

	////Output the Game results:
	//GameOver(winner, players);
	//
	////Destroy players:
	//for (i = 0; i < 2; i++) { delete players[i]; }

	return EXIT_SUCCESS;
}






void GameOver(int winner, Player*(players)[2])
{
	if (winner != -1) //We have a winner
	{
		auto WinnerChar = winner == 0 ? "A" : "B";
		cout << "Player " << WinnerChar << " won" << endl;
	}
	cout << "Points:" << endl;
	cout << "Player A: " << players[0]->getPlayerScore() << endl;
	cout << "Player B: " << players[1]->getPlayerScore() << endl;
}