#include "GameManager.h"
#include <iostream>
#define NUM_SHIP_TYPES 4
#define NUM_PLAYERS 2
#define A_NUM 0
#define B_NUM 1

GameManager::GameManager(GameBoard* gameBoard, bool isQuiet)
{
	_playersNumActiveShips = { 0, 0 };
	_playerScores = { 0, 0 };
	_currentPlayer = A_NUM; //player A starts the game
	_gameBoard = gameBoard;
	_isQuiet = isQuiet;
}

int GameManager::getPlayerScore(int player) const
{
	return player == A_NUM ? _playerScores.first : _playerScores.second;
}

bool GameManager::isPlayerDefeated(int player) const
{
	return player == A_NUM ? _playersNumActiveShips.first <= 0 : _playersNumActiveShips.second <= 0;
}

int GameManager::getCurrentPlayer() const
{
	return _currentPlayer;
}

/* Search for the attack point in shipsMap:
* If attack point is not in map --> it's a Miss!
* Else, hit the ship in map by taking one off the ship life counter
* If ship life is 0 (already sank) consider a Miss.
* If ship life has now become 0 --> it's a Sink!
* Else, return Hit. */
AttackResult GameManager::executeAttack(int attackedPlayerNum, pair<int, int> attack)
{
	auto shipsMap = _gameBoard->getShipsMap();
	auto found = shipsMap.find(attack);
	if (found == shipsMap.end()) //attack point not in map --> Miss
	{
		//cout << "Miss" << endl;
		_currentPlayer = attackedPlayerNum;
		return AttackResult::Miss;
	}
	auto ship = found->second.first; //attack point is in map --> get the ship
	auto shipWasHit = found->second.second;

	if (shipWasHit == true) //Not the first hit on this specific cell (i,j)
	{
		//pass turn to opponent
		_currentPlayer = attackedPlayerNum;
		if (ship->getLife() == 0) //ship already sank.. Miss
		{
			//cout << "Miss (hit a sunken ship)" << endl;
			return AttackResult::Miss;
		}
		//		cout << "Hit (ship was already hit before but still has'nt sunk..)" << endl;
		return AttackResult::Hit; //you don't get another turn if cell was already hit
	}

	ship->hit(); //Hit the ship (Take one off the ship life)
	found->second.second = true; //Mark cell as a 'Hit'
								 //cout << "Hit ship " << ship->getType() << endl;
	int shipType = ship->getType();
	if (isOwnGoal(attackedPlayerNum, shipType))
	{
		//in case of own goal pass turn to opponent
		_currentPlayer = attackedPlayerNum;
	}
	if (ship->getLife() == 0) //It's a Sink
	{
		if (attackedPlayerNum == A_NUM)
		{
			_playersNumActiveShips.first--;
		}
		else
		{
			_playersNumActiveShips.second--;
		}
		//update player points
		//playerB's ship was the one that got hit
		//add points to playerA
		if (shipType == tolower(shipType))
		{
			_playerScores.first += ship->getSinkPoints();
		}
		//playerA's ship was the one that got hit
		//add points to playerB
		else
		{
			_playerScores.second += ship->getSinkPoints();
		}
		//cout << "Sink! Score: " << ship->getSinkPoints() << endl;
		return AttackResult::Sink;
	}
	return AttackResult::Hit; //Hit
}

bool GameManager::isOwnGoal(int attackedPlayerNum, char shipType)
{
	return attackedPlayerNum == A_NUM && shipType != toupper(shipType)
		|| attackedPlayerNum == B_NUM && shipType != tolower(shipType);
}


void GameManager::gameOver(int winner) const
{
	if (winner != -1) //We have a winner
	{
		cout << "Player " << (winner == A_NUM ? "A" : "B") << " won" << endl;
	}
	cout << "Points:" << endl;
	cout << "Player A: " << _playerScores.first << endl;
	cout << "Player B: " << _playerScores.second << endl;
}

int GameManager:: runGame(IBattleshipGameAlgo* players[NUM_PLAYERS])
{

	// Each player declares his next attack.
	// Then, his enemy executes the attack and returns the AttackResult.
	// If the player hits an enemy's ship, he gets another turn
	// If the player make an own goal, he doesn't get another turn
	// Game is over once a player loses all his ships, or all attackes were taken.
	int currentPlayerNum, winner = -1;
	//finishedAttacks[i] is true iff players[i] finished all his attacks
	bool finishedAttacks[NUM_PLAYERS] = { false,false };
	pair<int, int> attackPoint;
	AttackResult attackResult;
	while (true)
	{
		currentPlayerNum = getCurrentPlayer();
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
		attackResult = executeAttack(1 - currentPlayerNum, attackPoint);
		players[A_NUM]->notifyOnAttackResult(currentPlayerNum, attackPoint.first, attackPoint.second, attackResult);
		players[B_NUM]->notifyOnAttackResult(currentPlayerNum, attackPoint.first, attackPoint.second, attackResult);
		//check for defeated players
		if (isPlayerDefeated(1 - currentPlayerNum))
		{
			//current player sunk all opponent's ships
			winner = currentPlayerNum; //winner is the current player
			break;
		}
		if (isPlayerDefeated(currentPlayerNum))
		{
			//current player sunk his own last ship
			winner = 1 - currentPlayerNum; //winner is the opponent
			break;
		}
	}
	return winner;
}
