#pragma once

#include "Cell.h"

#include "util/Math.h"

#include <unordered_map>

class Grid;

class Blob {

public:
	Blob(Grid& g, int id)
		:
		g{ g },
		id{ id }
	{}

	void grow();
	int getID() { return id; }
	void spawn(const Vec2i& p);

//private:
	Grid& g;

	std::unordered_map<Vec2i, int> perimeter;
	int id;

//private:
	// cell at coord grows in random direction
	const Vec2i growInRandomDirection(const Vec2i& coord);

	bool fillCell(const Vec2i& coord, Cell::Type contents);
	bool isValidFill(const Vec2i& coord, Cell::Type contents) const;

	const Vec2i selectionRandomElementFromPerimeter() const;

};