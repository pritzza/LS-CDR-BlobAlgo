#include "Grid.h"

int Grid::findNearestCell(int index, Cell::Type type)
{
    const int xPos = index % width;
    const int yPos = index / width;

    float minDistance = width * height;
    int minDistanceIndex{ -1 };

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            // dont look at the cell given
            if (x == xPos && y == yPos)
                continue;

            Cell& c{ getCell(x, y) };

            if (c.getType() == type)
            {
                const int displacementX{ x - xPos };
                const int displacementY{ y - yPos };

                const float distance = std::hypotf(displacementX, displacementX);

                if (distance < minDistance)
                {
                    minDistance = distance;
                    minDistanceIndex = x + (y * width);
                }
            }
        }

    return minDistanceIndex;
}

std::array<Vec2i, 4> Grid::getAdjacentCoords(const Vec2i& p)
{
    Vec2i above{ p.x,     p.y - 1 };
    Vec2i below{ p.x,     p.y + 1 };
    Vec2i right{ p.x + 1, p.y };
    Vec2i left{ p.x - 1, p.y };

    return std::array<Vec2i, 4>{ above, right, below, left };
}

int Grid::countNonEmptyNeighbors(const Vec2i& p)
{
    std::array<Cell,4> neighbors = getAdjacentCells(p);
    int count = 0;
    for (const Cell& c : neighbors)
        if (!c.isEmpty())
            count++;

    return count;
}

std::array<Cell, 4> Grid::getAdjacentCells(int index)
{
    int x = Math::to_x(index, width, height);
    int y = Math::to_y(index, width, height);
    return getAdjacentCells({ x, y });
}

std::array<Cell, 4> Grid::getAdjacentCells(const Vec2i& p)
{
    assert(Math::in_bounds(p, dim));

    std::array<Cell, 4> adj_cells;

    std::array<Vec2i, 4> adj_coords{ getAdjacentCoords(p) };

    for (int i = 0; i < adj_coords.size(); ++i)
    {
        const Vec2i& coord = adj_coords[i];

        if (Math::in_bounds(coord, dim))
        {
            int index = Math::to_index(coord, dim);

            const Cell& c = this->getCell(index);

            adj_cells[i] = c;
        }
        else
        {
            adj_cells[i] = OUT_OF_BOUNDS_CELL;
        }

    }

    return adj_cells;
}


void Grid::clear()
{
    for (Cell& c : cells)
        c.reset();
}

//Neighbors Grid::getNeighbors(int index) const
//{
//    Neighbors n;
//
//    // todo
//
//    return Neighbors();
//}

Cell& Grid::getCell(int index)
{
    bool isInBounds = (index >= 0 && index < cells.size());

    if (isInBounds)
        return cells[index];
    else
    {
        if (DEBUG_MODE)
            std::cout << ("DEBUG: attempting to access out of bounds element");

        return defaultCell;
    }
}

Cell& Grid::getCell(int x, int y)
{
    int index = x + (y * width);
    return getCell(index);
}

void Grid::setCell(int index, const Cell& c)
{
    bool isInBounds = (index >= 0 && index < cells.size());

    if (isInBounds)
        cells[index] = c;
    else
    {
        if (DEBUG_MODE)
            std::cout << ("DEBUG: attempting to access out of bounds element");
    }
}

void Grid::setCell(int x, int y, const Cell& c)
{
    int index = x + (y * width);
    setCell(index, c);
}