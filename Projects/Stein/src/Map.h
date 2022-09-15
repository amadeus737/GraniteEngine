#pragma once

#include <GraniteEngine.h>
#include <assert.h>
#include <vector>

class Coordinates
{
public:
	void Add(const Coordinates& c)
	{
		x += c.x;
		y += c.y;
	}

	bool operator==(const Coordinates& c) const
	{
		return x == c.x && y == c.y;
	}

	int x;
	int y;
};

class Map
{
public:
	Map()
		:
		_walls(),
		_wallTextures()
	{ }

	// Getters
	int scaledWidth() const;
	int scaledHeight() const;
	int width() const;
	int height() const;
	int cellsize() const;
	int offsetx() const;
	int offsety() const;
	const std::vector<int>& GetWalls() const { return _walls; }
	const std::vector<unsigned char>& GetWallTextures() const { return _wallTextures; };

	// Setters
	void SetWidth(int w);
	void SetHeight(int h);
	void SetOffset_x(int x);
	void SetOffset_y(int y);
	void SetCellProperties(int cellWidth, int cellPadding);
	void SetWalls(const std::vector<int>& walls) { _walls = walls; }
	void SetWallsAt(int index, int value) { _walls[index] = value; }
	void SetWallTextures(const std::vector<unsigned char>& wallTextures) { _wallTextures = wallTextures; }

	// Draw functions
	void DrawCell(const Coordinates& coords, Granite::Color c, bool clear = false) const;

	// Other
	bool InMap(const Coordinates& c) const;

private:
	std::vector<int> _walls;
	std::vector<unsigned char> _wallTextures;
	int _cellsize = 20;
	int _cellPadding = 1;
	int _width = 25;
	int _height = 25;
	int _xoffset = 70;
	int _yoffset = 50;
};

