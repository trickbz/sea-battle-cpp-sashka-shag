#pragma once
#include"Struct.h"

void FillAuto(Ship* ships, int shipsCount, Board board);
void FillManual(Ship* ships, int shipsCount, Board board);
void SetShip(Ship* ships, int shipsCount, Ship ship, int boardSize);
Coord NextCoord(Coord start, int direction, int boardSize);
bool CanSetPoint(Ship* ships, int shipsCount, int boardSize, Coord start, int direction, bool ignorePrevField, int shipIndexToIgnore = -1);
void PrintBoard(Ship* ships, int shipsCount, Board board, bool drawCrosshair);
Board CreateBoard(int size, bool isHumanBoard);
void MoveShip(Ship ship, int keyCode, int boardSize);
Ship* InitShipArray(int* shipSizes, int shipsCount);
Ship* GetShipByCoord(Ship* ships, int shipsCount, Coord coord);
ShipField* GetShipFieldByCoord(Ship* ships, int shipsCount, Coord coord, int shipIndexToIgnore = -1);
bool IsCoordInsideBoard(Coord coord);
bool IsBoardFieldEmpty(Ship* ships, int shipsCount, Coord coord, int shipIndexToIgnore = -1);
int GetDirectionByArrowKeyCode(int keyCode);
bool CanPlaceShip(Ship* ships, int shipsCount, Ship ship, int boardSize);
Coord* GenerateShipCoords(Coord startCoord, int shipSize, int shipDirection, int boardSize);
bool AreAllCoordsInsideBoard(Coord* coords, int coordsArraySize);
void ClearConsole();
void MoveCrossHair(Board &board, int direction);
void ClearShipCoords(Ship ship);
void PlaceShipOnInitialPlace(Ship ship);