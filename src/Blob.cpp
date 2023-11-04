#include "Blob.h"

#include "Grid.h"

void Blob::grow()
{
	// select random element from perimeter
	const Vec2i perimeter_coord = selectionRandomElementFromPerimeter();

	// grow in random valid direction from chosen perimeter cell
	// take coordinate of newly grown cell
	const Vec2i& new_cell_pos = growInRandomDirection(perimeter_coord);

	const int neighbor_count = g.countNonEmptyNeighbors(new_cell_pos);

	// add it to perimeter
	perimeter.insert({ new_cell_pos, neighbor_count });

	// increment neighbor count of all cells adjacent to new cell
	std::array<Vec2i, 4> new_cell_neighbor_coords = g.getAdjacentCoords(new_cell_pos);

	for (const Vec2i& new_cell_neighbor_coord : new_cell_neighbor_coords)
	{
		if (!Math::in_bounds(new_cell_neighbor_coord, g.dim))
			continue;

		const Cell& neightbor_cell = g.getCell(
			Math::to_index(new_cell_neighbor_coord, g.dim)
		);
		if (!neightbor_cell.isEmpty())
		{
			// if any of the adjacent cells now have 4 neighbors, remove from perimeter
			if (perimeter.find(new_cell_neighbor_coord) != perimeter.end())
			{
				const int nc = g.countNonEmptyNeighbors(new_cell_neighbor_coord);
				perimeter.at(new_cell_neighbor_coord) = nc;

				if (perimeter.at(new_cell_neighbor_coord) >= 4)
					perimeter.erase(new_cell_neighbor_coord);
			}
		}
	}
}

bool Blob::fillCell(const Vec2i& coord, Cell::Type contents)
{
	if (!Math::in_bounds(coord, g.dim))
		return false;

	const int cell_index = Math::to_index(coord, g.dim);
	Cell& cell = g.getCell(cell_index);

	const bool stale_contents = cell.getType() == contents;

	if (stale_contents)
		return false;

	cell.setType(contents);
	cell.setID(id);
	g.setCell(cell_index, cell);

	const int neighbor_count = g.countNonEmptyNeighbors(coord);

	perimeter.insert({ coord, neighbor_count });

	return true;
}

const Vec2i Blob::selectionRandomElementFromPerimeter() const
{
	int random_index = Math::rng(0, perimeter.size());

	auto it = perimeter.begin();

	std::advance(it, random_index);

	return it->first;
}

bool Blob::isValidFill(const Vec2i& coord, Cell::Type contents) const
{
	if (!Math::in_bounds(coord, g.dim))
		return false;

	const int cell_index = Math::to_index(coord, g.dim);
	Cell& cell = g.getCell(cell_index);

	const bool stale_contents = cell.getType() == contents;

	if (stale_contents)
		return false;

	return true;
}

void Blob::spawn(const Vec2i& p)
{
	fillCell(p, Cell::Type::Filled);
}

const Vec2i Blob::growInRandomDirection(const Vec2i& coord)
{
	const std::array<Vec2i, 4> adj_coords = g.getAdjacentCoords(coord);
	std::vector<Vec2i> valid_growth_coords;;

	for (const Vec2i& adj_coord : adj_coords)
	{
		if (isValidFill(adj_coord, Cell::Type::Filled))
		{
			valid_growth_coords.push_back(adj_coord);
		}
	}

	if (valid_growth_coords.size() == 0)
		return coord;

	int random_growing_cell_index = Math::rng(0, valid_growth_coords.size());

	const Vec2i& coord_to_grow = valid_growth_coords[random_growing_cell_index];

	fillCell(coord_to_grow, Cell::Type::Filled);

	return coord_to_grow;
}
