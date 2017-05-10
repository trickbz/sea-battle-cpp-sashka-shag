#include"Func.h"
#include<iostream>
#include<ctime>
#include<conio.h>
#include<windows.h>

using namespace std;

void FillAuto(Ship* ships, int shipsCount, Board board)
{
	int boardSize = board.size;
	for (int i = 0; i < boardSize; ++i)
	{
		Ship ship = ships[i];
		SetShip(ships, shipsCount, ship, boardSize);
	}
	board.mode = board_mode::GAMING_MODE;
}

void FillManual(Ship* ships, int shipsCount, Board board)
{
	int boardSize = board.size;
	for (int i = 0; i < boardSize; ++i)
	{
		Ship ship = ships[i];
		Ship shipPrev = ships[i];
		PlaceShipOnInitialPlace(ship);

		while (true)
		{
			ClearConsole();
			PrintBoard(ships, shipsCount, board, board.mode == board_mode::SELECTING_MODE);
			int keyCode = _getch();
			if (keyCode == codes::ENTER) {
				if (board.mode == board_mode::SELECTING_MODE) {
					board.mode = board_mode::EDITING_MODE;
					Ship* selectedShip = GetShipByCoord(ships, shipsCount, board.crosshairCoords);
					if (selectedShip != NULL) {
						ship = *selectedShip;
					}
				} else if (board.mode == board_mode::EDITING_MODE) {
					if (CanPlaceShip(ships, shipsCount, ship, boardSize)) {
						board.mode = board_mode::PLACING_MODE;
						ship = shipPrev;
						PlaceShipOnInitialPlace(ship);
					}
				}
				else {
					if (CanPlaceShip(ships, shipsCount, ship, boardSize))
						break;
				}
			}

			if (keyCode == codes::FIRST_CODE)
			{
				keyCode = _getch();
				int direction = GetDirectionByArrowKeyCode(keyCode);
				if (board.mode == board_mode::SELECTING_MODE) {
					MoveCrossHair(board, direction);
				}
				else if (board.mode == board_mode::EDITING_MODE || board.mode == board_mode::PLACING_MODE)
					MoveShip(ship, keyCode, boardSize);
					
			}

			if (keyCode == codes::SPACE) {
				int nextDirection = ship.direction == direction::DOWN ? direction::RIGHT : direction::DOWN;
				Coord* rotatedShipCoords = GenerateShipCoords(ship.fields[0].coord, ship.size, nextDirection, boardSize);
				if (AreAllCoordsInsideBoard(rotatedShipCoords, ship.size)) {
					for (int i = 1; i < ship.size; i++) {
						ship.direction = nextDirection;
						ship.fields[i].coord = rotatedShipCoords[i];
					}
				}
			}

			if ((keyCode == codes::E || keyCode == codes::e) && board.mode == board_mode::PLACING_MODE) {
				board.mode = board_mode::SELECTING_MODE;
				ClearShipCoords(ship);
			}
		}
	}

	board.mode = board_mode::GAMING_MODE;
}

Ship* InitShipArray(int* shipSizes, int shipsCount) {
	Ship* ships = new Ship[shipsCount];
	for (int i = 0; i < shipsCount; i++) {
		ships[i] = Ship(shipSizes[i], i);
	}
	return ships;
}

void MoveShip(Ship ship, int keyCode, int boardSize) {
	int shipSize = ship.size;
	Coord* nextCoords = new Coord[shipSize];
	ShipField* fields = ship.fields;
	bool canMoveShip = true;
	int direction = GetDirectionByArrowKeyCode(keyCode);
	for (int i = 0; i < shipSize; i++) {
		Coord coord = fields[i].coord;
		Coord nextCoord = NextCoord(coord, direction, boardSize);
		if (IsCoordInsideBoard(nextCoord)) {
			nextCoords[i] = nextCoord;
		}
		else {
			canMoveShip = false;
			break;
		}
	}

	if (canMoveShip) {
		for (int i = 0; i < shipSize; i++) {
			fields[i].coord = nextCoords[i];
		}
	}
}

Board CreateBoard(int size, bool isHumanBoard)
{
	Board board = Board();
	board.size = size;
	int** fields = new int*[size];
	for (int i = 0; i < size; ++i)
	{
		fields[i] = new int[size] {0};
	}
	board.fields = fields;
	return board;
}

void SetShip(Ship* ships, int shipsCount, Ship ship, int boardSize)
{
	srand(time(NULL));
	int direction;
	int shipSize = ship.size;
	while (true)
	{
		Coord coord;
		coord.x = rand() % 10;
		coord.y = rand() % 10;
		direction = rand() % 4;
		bool isShipValid = true;
		Coord* shipCoords = new Coord[shipSize];
		for (int i = 0; i < shipSize; ++i)
		{
			if (CanSetPoint(ships, shipsCount, boardSize, coord, direction, i == 0))
			{
				shipCoords[i] = coord;
				coord = NextCoord(coord, direction, boardSize);
				if (coord.x == -1 || coord.y == -1) {
					isShipValid = false;
					break;
				}
			}
			else 
			{
				isShipValid = false;
				break;

			}
		}

		if (isShipValid)
		{
			for (int i = 0; i < shipSize; ++i)
			{
				ship.direction = direction;
				ship.fields[i].coord = shipCoords[i];
				ship.fields[i].type = field_type::ALIVE;
			}
			break;
		}
	}
}

Coord NextCoord(Coord start, int direction, int boardSize)
{
	Coord nextCoord;
	nextCoord.x = -1;
	nextCoord.y = -1;
	switch (direction)
	{
	case direction::UP:
		nextCoord.x = start.x;
		nextCoord.y = start.y - 1 >= 0 ? start.y - 1 : -1;
		break;
	case direction::RIGHT:
		nextCoord.x = start.x + 1 < boardSize ? start.x + 1 : -1;
		nextCoord.y = start.y;
		break;
	case direction::DOWN:
		nextCoord.x = start.x;
		nextCoord.y = start.y + 1 < boardSize ? start.y + 1 : -1;
		break;
	case direction::LEFT:
		nextCoord.x = start.x - 1 >= 0 ? start.x - 1 : -1;
		nextCoord.y = start.y;
		break;
	case direction::UP_RIGHT:
		nextCoord.x = start.x + 1 < boardSize ? start.x + 1 : -1;
		nextCoord.y = start.y - 1 >= 0 ? start.y - 1 : -1;
		break;
	case direction::RIGHT_DOWN:
		nextCoord.x = start.x + 1 < boardSize ? start.x + 1 : -1;
		nextCoord.y = start.y + 1 < boardSize ? start.y + 1 : -1;
		break;
	case direction::DOWN_LEFT:
		nextCoord.y = start.y + 1 < boardSize ? start.y + 1 : -1;
		nextCoord.x = start.x - 1 >= 0 ? start.x - 1 : -1;
		break;
	case direction::LEFT_UP:
		nextCoord.x = start.x - 1 >= 0 ? start.x - 1 : -1;
		nextCoord.y = start.y - 1 >= 0 ? start.y - 1 : -1;
		break;
	default:
		break;
	}
	return nextCoord;
}

bool CanSetPoint(Ship* ships, int shipsCount, int boardSize, Coord start, int direction, bool ignoreReverseDirection, int shipIndexToIgnore)
{
	if (IsBoardFieldEmpty(ships, boardSize, start, shipIndexToIgnore))
	{
		int reverseDirection;
		if (direction > 1)
			reverseDirection = direction - 2;
		else
			reverseDirection = direction + 2;
		for (int direction = 0; direction < 8; ++direction)
		{
			if (ignoreReverseDirection == false && direction == reverseDirection) {
				continue;
			}
			Coord tmp = NextCoord(start, direction, boardSize);
			if (!IsCoordInsideBoard(tmp))
				continue;
			if (!IsBoardFieldEmpty(ships, shipsCount, tmp, shipIndexToIgnore))
				return false;
		}
		return true;
	}
	else
		return false;
}

void PrintBoard(Ship* ships, int shipsCount, Board board, bool drawCrosshair)
{
	int boardSize = board.size;
	for (int i = 0; i < boardSize; ++i)
	{
		for (int j = 0; j < boardSize; ++j)
		{
			Coord coord;
			coord.y = i;
			coord.x = j;
			ShipField* shipField = GetShipFieldByCoord(ships, shipsCount, coord);

			if (drawCrosshair && board.crosshairCoords.y == i && board.crosshairCoords.x == j) {
				cout << static_cast<char>(symbols::CROSSHAIR);
			}
			else {
				if (shipField != nullptr) {
					if ((*shipField).type == field_type::ALIVE) {
						cout << static_cast<char>(symbols::SHIP_ALIVE);
					}
				}
				else {
					cout << static_cast<char>(symbols::BOARD_EMPTY);
				}
			}
		}
		cout << endl;
	}
	cout << endl;
}

Ship* GetShipByCoord(Ship* ships, int shipsCount, Coord coord) {
	for (int i = 0; i < shipsCount; i++) {
		Ship ship = ships[i];
		for (int j = 0; j < ship.size; j++) {
			ShipField shipField = ship.fields[j];
			Coord fieldCoord = shipField.coord;
			if (fieldCoord.x == coord.x && fieldCoord.y == coord.y) {
				return &ship;
			}
		}
	}
	return NULL;
}

ShipField* GetShipFieldByCoord(Ship* ships, int shipsCount, Coord coord, int shipIndexToIgnore) {
	for (int i = 0; i < shipsCount; i++) {
		Ship ship = ships[i];
		for (int j = 0; j < ship.size; j++) {
			ShipField shipField = ship.fields[j];
			Coord fieldCoord = shipField.coord;
			if (shipIndexToIgnore != -1 && ship.index == shipIndexToIgnore)
				continue;
			if (fieldCoord.x == coord.x && fieldCoord.y == coord.y) {
				return &shipField;
			}
		}
	}
	return NULL;
}

bool IsBoardFieldEmpty(Ship* ships, int shipsCount, Coord coord, int shipIndexToIgnore) {
	ShipField* field = GetShipFieldByCoord(ships, shipsCount, coord, shipIndexToIgnore);
	return field == NULL;
}

bool IsCoordInsideBoard(Coord coord) {
	return coord.x != -1 && coord.y != -1;
}

int GetDirectionByArrowKeyCode(int keyCode) {
	int direction;
	switch (keyCode)
	{
	case codes::KEY_RIGHT:
		direction = direction::RIGHT;
		break;
	case codes::KEY_LEFT:
		direction = direction::LEFT;
		break;
	case codes::KEY_DOWN:
		direction = direction::DOWN;
		break;
	case codes::KEY_UP:
		direction = direction::UP;
		break;
	}
	return direction;
}

bool CanPlaceShip(Ship* ships, int shipsCount, Ship ship, int boardSize) {
	bool canPlaceShip = true;
	int shipSize = ship.size;
	for (int i = 0; i < shipSize; i++) {
		Coord fieldCoord = ship.fields[i].coord;
		bool ignoreReverseDirection = i == 0;
		if (!CanSetPoint(ships, shipsCount, boardSize, fieldCoord, ship.direction, ignoreReverseDirection, ship.index)) {
			canPlaceShip = false;
			break;
		}
	}
	return canPlaceShip;
}

Coord* GenerateShipCoords(Coord startCoord, int shipSize, int shipDirection, int boardSize) {
	Coord* coords = new Coord[shipSize];
	coords[0] = startCoord;
	Coord coordCurrent = startCoord;
	for (int i = 1; i < shipSize; i++) {
		Coord nextCoord = NextCoord(coordCurrent, shipDirection, boardSize);
		coords[i] = nextCoord;
		coordCurrent = nextCoord;
	}
	return coords;
}

bool AreAllCoordsInsideBoard(Coord* coords, int coordsArraySize) {
	for (int i = 0; i < coordsArraySize; i++) {
		Coord coord = coords[i];
		if (!IsCoordInsideBoard(coord))
			return false;
	}
	return true;
}

void ClearConsole() 
{
	system("cls");
	SetConsoleCP(437);
	SetConsoleOutputCP(437);
}

void MoveCrossHair(Board &board, int direction) {
	Coord nextCoord = NextCoord(board.crosshairCoords, direction, board.size);
	if (IsCoordInsideBoard(nextCoord)) {
		board.crosshairCoords = nextCoord;
	}
}

void ClearShipCoords(Ship ship)
{
	for (int i = 0; i < ship.size; i++) {
		ship.fields[i].coord.x = -1;
		ship.fields[i].coord.y = -1;
	}
}

void PlaceShipOnInitialPlace(Ship ship)
{
	ship.fields[0].coord.x = 0;
	ship.fields[0].coord.y = 0;
	Coord base = ship.fields[0].coord;
	for (int i = 1; i < ship.size; i++) {
		ship.fields[i].coord.x = 0;
		ship.fields[i].coord.y = i;
	}
}