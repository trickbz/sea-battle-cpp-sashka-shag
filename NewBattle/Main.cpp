#include"Func.h"
#include<ctime>
#include<time.h>
#include<windows.h>
using namespace std;
void main()
{
	SetConsoleCP(437);
	SetConsoleOutputCP(437);
	const int boardSize = 10;
	const int shipsCount = 10;
	int shipSizes[] { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	Board boardMy = CreateBoard(boardSize);
	Ship* shipsMy = InitShipArray(shipSizes, shipsCount);
	Ship* shipsComp = InitShipArray(shipSizes, shipsCount);

	//FillAuto(shipsMy, shipsCount, boardMy);
	boardMy.mode = board_mode::PLACING_MODE;
	FillManual(shipsMy, shipsCount, boardMy);
	PrintBoard(shipsMy, shipsCount, boardMy, boardSize, true, false);

}