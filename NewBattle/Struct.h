#pragma once
#include"Enums.h"

struct Coord {
	int x;
	int y;
};

struct ShipField {
	Coord coord;
	field_type type;
};

struct Ship {
	int size;
	ShipField* fields;
	int direction;
	int index;

	Ship() {}
	
	Ship(int shipSize, int shipIndex) {
		size = shipSize;
		index = shipIndex;
		fields = new ShipField[shipSize];
		for (int i = 0; i < shipSize; i++) {
			direction = direction::DOWN;
			ShipField field;
			field.type = field_type::ALIVE;
			field.coord.x = -1;
			field.coord.y = -1;
			fields[i] = field;
		}
	}
};

struct Board {
	Board() {
		Coord initialCrosshairCoords;
		initialCrosshairCoords.x = 0;
		initialCrosshairCoords.y = 0;
		crosshairCoords = initialCrosshairCoords;
	}
	int size;
	int** fields;
	Coord crosshairCoords;
	int mode;
};