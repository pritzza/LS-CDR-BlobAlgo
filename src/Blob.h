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

	// meant for placing first cell of blob
	void spawn(const Vec2i& p);

	// grows blob by 1 cell
	void grow();
	
	// reduces blob's perimeter cells to cells that 
	// are really only apart of its perimeter
	// (this should be happening automatically but there's
	// a bug in the algorithm/code, but this vastly speeds
	// up program if called reguarly)
	void cullPerimeter();

	float perimeterAreaRatio() const;

	int getID() const { return id; }
	int getArea() const { return area; }

private:
	Grid& g;

	std::unordered_map<Vec2i, int> perimeter;
	int id;
	int area{ 0 };

private:
	// cell at coord grows in random direction
	const Vec2i growInRandomDirection(const Vec2i& coord);

	bool fillCell(const Vec2i& coord, Cell::Type contents);
	bool isValidFill(const Vec2i& coord, Cell::Type contents) const;

	const Vec2i selectionRandomElementFromPerimeter();


};