#include "Map.h"

int Map::width() const { return _width; }
int Map::height() const { return _height; }
int Map::scaledWidth() const { return _cellsize * _width; }
int Map::scaledHeight() const { return _cellsize * _height; }
int Map::cellsize() const { return _cellsize; }
int Map::offsetx() const { return _xoffset; }
int Map::offsety() const { return _yoffset; }

void Map::SetWidth(int w) { _width = w; }
void Map::SetHeight(int h) { _height = h; }
void Map::SetOffset_x(int x) { _xoffset = x; }
void Map::SetOffset_y(int y) { _yoffset = y; }
void Map::SetCellProperties(int cellWidth, int cellPadding) { _cellsize = cellWidth; _cellPadding = cellPadding; }

bool Map::InMap(const Coordinates& c) const
{
	return c.x >= 0 && c.x < _width&& c.y >= 0 && c.y < _height;
}

void Map::DrawCell(const Coordinates& coords, Granite::Color c, bool clear) const
{
	assert(coords.x >= 0);
	assert(coords.x < _width);
	assert(coords.y >= 0);
	assert(coords.y < _height);

	const int offset_x = _xoffset;
	const int offset_y = _yoffset;

	GraniteFrameBuffer.DrawRect(coords.x * _cellsize + offset_x + _cellPadding,
		coords.y * _cellsize + offset_y + _cellPadding,
		_cellsize - 2 * _cellPadding,
		_cellsize - 2 * _cellPadding,
		c, clear);
}