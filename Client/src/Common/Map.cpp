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
void Map::SetBorderProperties(int borderWidth, int borderPadding) { _borderWidth = borderWidth; _borderPadding = borderPadding; }
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

	const int offset_x = _xoffset + _borderWidth + _borderPadding;
	const int offset_y = _yoffset + _borderWidth + _borderPadding;

	_frameBuffer.DrawRect(coords.x * _cellsize + offset_x + _cellPadding,
		coords.y * _cellsize + offset_y + _cellPadding,
		_cellsize - 2 * _cellPadding,
		_cellsize - 2 * _cellPadding,
		c, clear);
}

void Map::DrawCellBorder(const Coordinates& coords, int thickness, Granite::Color c) const
{
	assert(coords.x >= 0);
	assert(coords.x < _width);
	assert(coords.y >= 0);
	assert(coords.y < _height);

	const int offset_x = _xoffset + _borderWidth + _borderPadding;
	const int offset_y = _yoffset + _borderWidth + _borderPadding;

	// Draw left edge of tile
	_frameBuffer.DrawRect(coords.x * _cellsize + offset_x + _cellPadding,
		coords.y * _cellsize + offset_y + _cellPadding,
		thickness,
		_cellsize,
		c, false);

	// Draw top edge of tile
	_frameBuffer.DrawRect(coords.x * _cellsize + offset_x + _cellPadding,
		coords.y * _cellsize + offset_y + _cellPadding,
		_cellsize,
		thickness,
		c, false);

	// Draw right edge of tile
	_frameBuffer.DrawRect(coords.x * _cellsize + offset_x + _cellPadding + _cellsize,
		coords.y * _cellsize + offset_y + _cellPadding,
		thickness,
		_cellsize,
		c, false);

	// Draw top edge of tile
	_frameBuffer.DrawRect(coords.x * _cellsize + offset_x + _cellPadding,
		coords.y * _cellsize + offset_y + _cellPadding + _cellsize,
		_cellsize + thickness,
		thickness,
		c, false);
}

void Map::DrawBorder()
{
	const int top = _yoffset;
	const int left = _xoffset;
	const int bottom = top + (_borderWidth + _borderPadding) * 2 + _height * _cellsize;
	const int right = left + (_borderWidth + _borderPadding) * 2 + _width * _cellsize;

	// top
	_frameBuffer.DrawRect(left, top, right - left, _borderWidth, _borderColor, false);
	// left
	_frameBuffer.DrawRect(left, top + _borderWidth, _borderWidth, bottom - top - 2 * _borderWidth, _borderColor, false);
	// right
	_frameBuffer.DrawRect(right - _borderWidth, top + _borderWidth, _borderWidth, bottom - top - 2 * _borderWidth, _borderColor, false);
	// bottom
	_frameBuffer.DrawRect(left, bottom - _borderWidth, right - left, _borderWidth, _borderColor, false);
}