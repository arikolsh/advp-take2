#include "PredictedPlayer.h"
#include "IBattleshipGameAlgoV2.h"
#include <iostream>
#include <algorithm>
#include <sstream>

#define SHELL_RES 4096
#define MODE "r"
#define SEARCH_DEFAULT_CMD "dir /b /a-d 2> nul"
#define WRONG_PATH "Wrong path: "
#define MISSING_ATTACK_FILE "Missing attack file for player (*.attack) looking in path: "
#define ATTACK_EXTENSION "attack"
#define WORKING_DIR "Working Directory"
#define ATTACKS_FILES_MIN 1
#define EMPTY ""
#define SUCCESS 0
#define ERROR -1

//Constructor
PredictedPlayer::PredictedPlayer(int playerNum)
{
	_playerNum = playerNum;
	_attackFilePath = EMPTY;
	_playerAttacks = {};
	_attackPosition = 0;
}

//Destructor
PredictedPlayer::~PredictedPlayer()
{
	//Do nothing for now
}

/* Gets the input directory Path with all attack files
* Updates the _attackFilePath to the relevant attack file if provided */
bool PredictedPlayer::init(const std::string& path)
{
	SetAttackFilePath(_playerNum, path);
	return _attackFilePath != EMPTY ? true : false;
}

void PredictedPlayer::SetAttackFilePath(int playerNum, const string& dirPath)
{
	vector<string> attackFiles = { EMPTY, EMPTY };
	switch (getAttackFiles(attackFiles, dirPath))
	{
	case 1:
		_attackFilePath = attackFiles[0];
		return;
	case 2:
		_attackFilePath = attackFiles[playerNum];
		return;
	default:
		_attackFilePath = EMPTY;
	}
}


std::pair<int, int> PredictedPlayer::attack()
{
	return _playerAttacks[++_attackPosition];
}


void PredictedPlayer::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{

}


//Get all legal attacks (pairs of valid ints) that exist in attackFile:
/* This method reads the next attack point from the given attack file
* and returns is as a pair <int, int>)
* The functions checks if the current pair is valid and if not,
* according to Guidelines, proceeds to the next line in the file */
void PredictedPlayer::getAttacksFromFile()
{
	pair<int, int> attack;
	string line;
	ifstream attackFile; //The file with all attacks to be executed

	attackFile.open(_attackFilePath);
	if (!attackFile.is_open())
	{
		cout << "Error: failed to open player attack file" << endl;
		return;
	}

	// Read entire attack file and insert all legal attacks to a vector
	while (getline(attackFile, line)) //checked: getline catch \r\n and \n.
	{
		attack = getAttackPair(line);
		if (!IsValidAttack(attack))
		{
			continue;
		}
		//Push current valid attack into _playerAttacks vector:
		_playerAttacks.push_back(attack);
	}
	//Finished inserting all attacks into _playerAttacks vector:
	attackFile.close();
}


bool PredictedPlayer::IsValidAttack(pair<int, int> attack)
{
	return attack.first != -1 && attack.second != -1;
}


pair<int, int> PredictedPlayer::getAttackPair(string& line) const
{
	pair<int, int> attack = { -1,-1 };
	int i, j;
	pair<string, string> rowCol;
	size_t idx = 0, delimIndex; //holds the position of the next character after the number

	delimIndex = line.find(',');
	if (delimIndex == string::npos) //string::npos returns when ',' was not found
	{
		return attack;
	}
	rowCol = std::make_pair(line.substr(0, delimIndex), line.substr(delimIndex + 1));
	//remove ' ' from the end of the key part
	while (!rowCol.first.empty() && rowCol.first.back() == ' ')
	{
		rowCol.first.erase(rowCol.first.length() - 1, 1);
	}
	//remove leading ' ' in the value part
	while (!rowCol.second.empty() && rowCol.second.front() == ' ')
	{
		rowCol.second.erase(0, 1);
	}
	// validate each part has no more than 2 chars
	if (rowCol.first.size() > 2 || rowCol.second.size() > 2)
	{
		return attack;
	}

	//check if each part is a number
	try
	{
		i = stoi(rowCol.first, &idx);
		if (rowCol.first.size() == 2 && rowCol.first[idx] != 0)
		{
			return attack;
		}
	}
	catch (const std::invalid_argument& ia)
	{
		(void)ia;
		return attack;
	}
	try
	{
		j = stoi(rowCol.second, &idx);
		if (rowCol.second.size() == 2 && rowCol.second[idx] != 0)
		{
			return attack;
		}
	}
	catch (const std::invalid_argument& ia)
	{
		(void)ia;
		return attack;
	}

	// check that each number is in the legal range:
	if (i > 10 || j > 10 || i < 1 || j < 1)
	{
		return attack; // Invalid according to Guidelines
	}

	// Valid attack:
	attack.first = i;
	attack.first = j;
	return attack;
}


int PredictedPlayer::getAttackFiles(vector<string> & attackFiles, string searchDir) const
{
	int numOfFiles;
	vector<string> messages;

	//If we got a system error:
	if (fetchInputFiles(attackFiles, messages, searchDir) == ERROR)
	{
		cout << "Error: failed to fetch input files from super-file container" << endl;
		return ERROR;
	}

	//If missing somthing:
	if (messages.size() != 0)
	{
		showMessages(messages);
		return ERROR;
	}

	//Return the number of attack files found
	if (attackFiles[0] != "" && attackFiles[1] != "")
	{
		numOfFiles = 2;
	}
	else if (attackFiles[0] != "")
	{
		numOfFiles = 1;
	}
	else
	{
		numOfFiles = 0;
	}

	return numOfFiles;
}


void PredictedPlayer::showMessages(vector<string> messages)
{
	for (auto it = messages.begin(); it != messages.end(); ++it)
	{
		cout << *it;
	}
}


int PredictedPlayer::execCmd(char const * cmd, string & shellRes)
{
	FILE * pPipe;
	int exitCode;
	char shellTmpRes[SHELL_RES] = { 0 };
	pPipe = _popen(cmd, MODE);
	if (pPipe == nullptr)	//if an error occured
	{
		return ERROR;
	}
	while (fgets(shellTmpRes, sizeof(shellTmpRes), pPipe))
	{
		shellRes += string(shellTmpRes);
		shellRes += "\n"; // add EOL
	}
	/* Close pipe and get shelexit code */
	exitCode = _pclose(pPipe); // returns -1 in case of a _popen failure otherwise returns the shell exit code
	return exitCode;
}


int PredictedPlayer::fetchInputFiles(vector<string> & attackFiles, vector<string> & messages, const string path) const
{
	int hasAttackFile = 0, opRes;
	size_t delimIndex;
	const char * searchCmd;
	string s_search_cmd, pathToFiles, line, fileExtension, shellRes;
	ostringstream tmpSearchCmd, tmpPathToFiles, wrongPath, missingAttackFile, missingBAttckFile, fullPath;
	vector<string> tmpAttackFiles;

	if (path.empty() == true)
	{
		//prepare messsages
		wrongPath << WRONG_PATH << WORKING_DIR << endl;
		missingAttackFile << MISSING_ATTACK_FILE << WORKING_DIR << endl;
		opRes = execCmd(SEARCH_DEFAULT_CMD, shellRes);
		if (ERROR == opRes)
		{
			return ERROR;
		}
		if (1 == opRes)
		{
			messages.push_back(wrongPath.str());
			return SUCCESS;
		}
	}
	else
	{
		fullPath << path << "\\";
		//prepare messsages
		wrongPath << WRONG_PATH << path << endl;
		missingAttackFile << MISSING_ATTACK_FILE << path << endl;
		/* build the dir command: with arguments /b /a-d. if this command fail(op res == 1) there are 2 options:
		1. wrong or non-exist path - in this case present wrong path messasge and return.
		2. the dir exist but it is empty  - in this case we need to collect the "missing files warnings" and return,
		therfore we need to continue and open the output file.
		*/
		tmpSearchCmd << "dir \"" << path << "\" /b /a-d 2> nul";
		s_search_cmd = tmpSearchCmd.str();
		searchCmd = s_search_cmd.c_str();
		opRes = execCmd(searchCmd, shellRes);
		if (ERROR == opRes)
		{
			return ERROR;
		}
		if (1 == opRes)
		{
			// build the new dir command: with arguments /b /a. if also this command fail(op res == 1) than it's a wrong path
			shellRes.clear(); // clear the shellcmd content
			tmpSearchCmd.str(EMPTY);
			tmpSearchCmd << "dir " << path << " /b /a 2> nul";
			s_search_cmd = tmpSearchCmd.str();
			searchCmd = s_search_cmd.c_str();
			opRes = execCmd(searchCmd, shellRes);
			if (ERROR == opRes)
			{
				return ERROR;
			}
			if (1 == opRes)
			{
				messages.push_back(wrongPath.str());
				return SUCCESS;
			}
		}
	}
	//in this point we act in the same logic for the two cases
	istringstream  inputFilesContainer(shellRes); // declaration must to be dynamic 
	while (getline(inputFilesContainer, line))
	{
		delimIndex = line.find('.');
		if (delimIndex == string::npos) {	//string::npos returns when '.' was not found
			continue;
		}
		if (line.substr(delimIndex + 1).compare(ATTACK_EXTENSION) == 0)
		{
			// if we read from the working directory
			if (path.empty() == true)
			{
				tmpAttackFiles.push_back(line);
				hasAttackFile++;
				continue;
			}
			// if we read from a given path
			fullPath << line;
			tmpAttackFiles.push_back(fullPath.str());
			hasAttackFile++;
			fullPath.str(EMPTY);
			fullPath << path << "\\";
			continue;
		}
		fullPath.str(EMPTY);
		fullPath << path << "\\";
	}
	//check flags
	if (hasAttackFile < ATTACKS_FILES_MIN) // if we have less than ATTACKS_FILES_MIN files
	{
		messages.push_back(missingAttackFile.str());
	}
	if (messages.size() == 0)
	{
		//lexicographic order of the attacks files
		sort(tmpAttackFiles.begin(), tmpAttackFiles.end());
		attackFiles[0] = tmpAttackFiles[0];
		attackFiles[1] = tmpAttackFiles.size() > 1 ? tmpAttackFiles[1] : EMPTY;
	}
	return SUCCESS;
}