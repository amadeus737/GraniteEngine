#include "Coordinates.h"

void Coordinates::Add(const Coordinates& c)
{
	x += c.x;
	y += c.y;
}

bool Coordinates::operator==(const Coordinates& c) const
{
	return x == c.x && y == c.y;
}