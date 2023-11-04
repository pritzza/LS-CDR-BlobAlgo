#pragma once

#include <cstdint>
#include <memory>

#include "Cell.h"

#include <vector>
#include <iostream>
#include <array>

#include "util/Math.h"

enum Direction
{
    NorthWest,
    North,
    NorthEast,
    West,
    East,
    SouthWest,
    South,
    SouthEast,
    NUM_DIRECTIONS
};

//struct Neighbors
//{
//    std::array<Cell*, NUM_DIRECTIONS> cells;
//    int centerIndex{ -1 };
//};

// just a grid of cells
struct Grid
{
    // print message when attempting to access out of bound element
    const bool DEBUG_MODE = false;

    Cell defaultCell{};
    inline static const Cell OUT_OF_BOUNDS_CELL{ Cell::Type::Filled, Cell::INVALID_ID };

    int width;
    int height;
    Vec2i dim;

    std::vector<Cell> cells;

    Grid(int width, int height)
        :
        width{ width },
        height{ height },
        dim{ width, height },
        cells{ width * height }
    {}

    // returns index of given type cell from a given point
    // returns -1 if cell of type does not exist in grid
    // TODO: TEST
    int findNearestCell(int index, Cell::Type type);

    void clear();

    std::array<Cell, 4> getAdjacentCells(int index);
    std::array<Cell, 4> getAdjacentCells(const Vec2i& p);
    std::array<Vec2i, 4> getAdjacentCoords(const Vec2i& p);
    int countNonEmptyNeighbors(const Vec2i& p);

    //Neighbors getNeighbors(int index) const;

    Cell& getCell(int index);
    Cell& getCell(int x, int y);

    void setCell(int i, const Cell& c);
    void setCell(int x, int y, const Cell& c);

    int getWidth()  const { return width; }
    int getHeight() const { return height; }
    int getSize()   const { return cells.size(); }
};