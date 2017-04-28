#pragma once

#include <fstream>
#include "IBattleshipGameAlgoV2.h"
#include <vector>

using namespace std;

class PredictedPlayer : public IBattleshipGameAlgo
{
public:

	// Class constructor
	explicit PredictedPlayer(int playerNum);

	// Class distructor
	~PredictedPlayer();

	/* Called once to allow init from files if needed
	* returns whether the init succeeded or failed */
	bool init(const std::string& path) override;

	// ask player for his move
	std::pair<int, int> attack() override;

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override = 0;	// notify on last move result

private:

	int _playerNum;
	string _attackFilePath; //The input filePath with all attacks to be executed
	ifstream _attackFile; //The file with all attacks to be executed
	vector<pair <int, int>> _playerAttacks; //A vector with all player attacks
	bool _sharingAttacks; //Indicates the case of 2 players sharing 1 attack file
	int _attackPosition; //Next attack index in _playerAttacks vector

	void getAttacksFromFile();
	static bool IsValidAttack(pair<int, int> attack);
	int PredictedPlayer::fetchInputFiles(vector<string> & attackFiles, vector<string> & messages, const string path) const;
	static int PredictedPlayer::execCmd(char const * cmd, string & shellRes);
	static void PredictedPlayer::showMessages(vector<string> messages);
	int PredictedPlayer::getInputFiles(vector<string> & attackFiles, string searchDir) const;
	void SetAttackFilePath(int playerNum, const string& path);
	pair<int, int> GetAttackPair(string& line) const;
};