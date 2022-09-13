#include "Snake.h"

void Snake::Segment::InitializeHead(const Coordinates& location)
{
	_coords = location;
	_color = Snake::_headColor;
}

void Snake::Segment::InitializeBody(Granite::Color c)
{
	_color = c;
}

void Snake::Segment::Follow(const Segment& next)
{
	_coords = next._coords;
}

void Snake::Segment::Move(const Coordinates& delta)
{
	assert(abs(delta.x) + abs(delta.y) == 1);
	_coords.Add(delta);
}

void Snake::Segment::Draw(Map& map, bool clear) const
{
	map.DrawCell(_coords, _color, clear);
}

Snake::Snake(const Coordinates& startLocation)
{
	InitializeSnake(startLocation);
}

void Snake::InitializeSnake(const Coordinates& startLocation)
{
	constexpr int nBodyColors = 4;
	constexpr Granite::Color bodyColors[nBodyColors] =
	{
		{Granite::Color(10, 100, 32)},
		{Granite::Color(10, 130, 48)},
		{Granite::Color(18, 160, 48)},
		{Granite::Color(10, 130, 48)}
	};

	for (int i = 0; i < _maxSegments; i++)
		_segments[i].InitializeBody(bodyColors[i % nBodyColors]);

	_segments[0].InitializeHead(startLocation);
}

void Snake::Move(const Coordinates& delta)
{
	for (int i = _nSegments - 1; i > 0; i--)
		_segments[i].Follow(_segments[i - 1]);

	_segments[0].Move(delta);
}

Coordinates Snake::GetNextHeadLocation(const Coordinates& delta) const
{
	Coordinates next(_segments[0].coords());
	next.Add(delta);
	return next;
}

void Snake::Reset(const Coordinates& spawnLocation)
{
	InitializeSnake(spawnLocation);
	_nSegments = 1;
}

void Snake::Grow()
{
	if (_nSegments < _maxSegments)
	{
		++_nSegments;
	}
}

void Snake::Draw(Map& map) const
{
	for (int i = 0; i < _nSegments; i++)
		_segments[i].Draw(map, false);
}

bool Snake::InTileExcludeEnd(const Coordinates& c) const
{
	for (int i = 0; i < _nSegments - 1; i++)
	{
		if (_segments[i].coords() == c)
			return true;
	}

	return false;
}

bool Snake::InTile(const Coordinates& c) const
{
	for (int i = 0; i < _nSegments; i++)
	{
		if (_segments[i].coords() == c)
			return true;
	}

	return false;
}