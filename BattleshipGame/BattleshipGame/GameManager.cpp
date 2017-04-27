#include "GameManager.h"
#define NUM_SHIP_TYPES 4
#define A_NUM 0
#define B_NUM 1

GameManager::GameManager(GameBoard* gameBoard)
{
	_playersNumActiveShips = { 0, 0 };
	_playerScores = { 0, 0 };
	_currentPlayer = A_NUM; //player A starts the game
	_gameBoard = gameBoard;
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
		//		//As mentioned on the forum, we can choose to return 'Miss' in this case. 
		//		//Can easily change this behaviour to 'Hit' in next exercise..
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