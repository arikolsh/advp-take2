#pragma once

#include <fstream>
#include "IBattleshipGameAlgoV2.h"

using namespace std;

class PredictedPlayer : public IBattleshipGameAlgo
{
public:

	// Class constructor
	explicit PredictedPlayer(int playerNum, string attackFilePath);

	// Class distructor
	~PredictedPlayer();

	// ask player for his move
	std::pair<int, int> attack() override;

private:
	ifstream _attackFile; //The input file with all attacks to be executed
	bool _attackFileOpened;
};