#pragma once

#include <GraniteEngine.h>
#include <assert.h>
#include <vector>
#include "Coordinates.h"

class Map
{
public:
	// Getters
	int scaledWidth() const;
	int scaledHeight() const;
	int width() const;
	int height() const;
	int cellsize() const;
	int offsetx() const;
	int offsety() const;

	// Setters
	void SetWidth(int w);
	void SetHeight(int h);
	void SetOffset_x(int x);
	void SetOffset_y(int y);
	void SetBorderProperties(int borderWidth, int borderPadding);
	void SetCellProperties(int cellWidth, int cellPadding);

	// Draw functions
	void DrawCell(const Coordinates& coords, Granite::Color c, bool clear = false) const;
	void DrawCellBorder(const Coordinates& coords, int thickness, Granite::Color c) const;
	void DrawBorder();

	// Other
	bool InMap(const Coordinates& c) const;

private:
	int _cellsize = 20;
	int _cellPadding = 1;
	int _width = 25;
	int _height = 25;
	int _borderWidth = 4;
	int _borderPadding = 2;
	int _xoffset = 70;
	int _yoffset = 50;
	static constexpr Granite::Color _borderColor = Granite::Color::Blue();
};