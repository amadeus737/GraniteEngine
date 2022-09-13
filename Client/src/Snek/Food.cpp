#include "Food.h"

Food::Food(std::mt19937& rng, const Map& map, const Snake& snake)
{
	Respawn(rng, map, snake);
}

void Food::Respawn(std::mt19937& rng, const Map& map, const Snake& snake)
{
	std::uniform_int_distribution<int> xdist(0, map.width() - 1);
	std::uniform_int_distribution<int> ydist(0, map.height() - 1);

	// Find a position not occupied by the snake
	Coordinates newCoords;
	do
	{
		newCoords.x = xdist(rng);
		newCoords.y = ydist(rng);
	} while (snake.InTile(newCoords));

	_coords = newCoords;
}

void Food::Draw(Map& map, bool clear) const
{
	map.DrawCell(_coords, _color, clear);
}

const Coordinates& Food::coords() const { return _coords; }