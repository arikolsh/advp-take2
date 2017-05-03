#include <string>
#include <vector>
#include<iostream>
#include<stdlib.h>
#include <stdio.h>  
#include <windows.h>
#define SUB_COLOR FOREGROUND_GREEN

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//returns the current attributes
WORD GetConsoleTextAttribute(HANDLE hCon)
{
	CONSOLE_SCREEN_BUFFER_INFO con_info;
	GetConsoleScreenBufferInfo(hCon, &con_info);
	return con_info.wAttributes;
}

void main()
{
	char * a[] = { "xxx","xxx","xxx" };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const int saved_colors = GetConsoleTextAttribute(hConsole);

	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	std::cout << "This text should be blue" << std::endl;
	SetConsoleTextAttribute(hConsole, SUB_COLOR | FOREGROUND_INTENSITY);
	//SetConsoleTextAttribute(hConsole, saved_colors); //return to black
	std::vector<std::string> v(a,a+3);
	for(int i=0;i<3;i++)
	{
		for (int j = 0; j < 3; j++)
		{
			gotoxy(i, j);
			std::cout << v[i][j];
		}
		std::cout << std::endl;
	}

	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			gotoxy(j, i);
			if (i == 2 && j == 1)
			{
				std::cout << '%';
			}
			else
			{
				std::cout << v[i][j];

			}
		}
		std::cout << std::endl;
	}
	gotoxy(1, 1);
	std::cout << "&";
	gotoxy(0, 3);

}
