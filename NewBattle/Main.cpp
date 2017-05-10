#include"Func.h"
#include<ctime>
#include<time.h>
#include<windows.h>
#include <conio.h>
using namespace std;
void main()
{
	SetConsoleCP(437);
	SetConsoleOutputCP(437);
	const int boardSize = 10;
	const int shipsCount = 10;
	int shipSizes[] { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	Board boardHuman = CreateBoard(boardSize, true);
	Board boardComp = CreateBoard(boardSize, false);
	Ship* shipsHuman = InitShipArray(shipSizes, shipsCount);
	Ship* shipsComp = InitShipArray(shipSizes, shipsCount);
	FillAuto(shipsComp, shipsCount, boardComp);
	FillAuto(shipsHuman, shipsCount, boardHuman);
	//boardMy.mode = board_mode::PLACING_MODE;
	//FillManual(shipsMy, shipsCount, boardMy);

	while (true)
	{
		PrintBoard(shipsHuman, shipsCount, boardHuman, false);
		PrintBoard(shipsComp, shipsCount, boardComp, false);
		_getch();
		system("pause");
	}
}