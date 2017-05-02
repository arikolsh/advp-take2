#pragma once
#include <vector>
#include<iostream>
#include <windows.h>
#define SUB_COLOR FOREGROUND_YELLOW
//#define SUB_COLOR FOREGROUND_RED
//#define SUB_COLOR FOREGROUND_BLUE
//#define SUB_COLOR FOREGROUND_GREEN

using namespace std;
class GameViewer
{
public:
	static void showBoard(vector<string> board);
	static void showHit(int i, int j);
	static void showAttack(int i, int j);
private:
	static void gotoxy(int x, int y);
	static WORD GetConsoleTextAttribute(HANDLE hCon);
};
