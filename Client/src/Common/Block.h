#pragma once

#include <GraniteEngine.h>

class Block
{
public:
	// Constructor
	Block();

	// Getters
	int x();
	int y();
	int width();
	int height();
	Granite::Color color();

	// Class methods
	bool HitScreenLR();
	bool HitScreenUD();
	void SetSpeed(int x_amount, int y_amount);

private:
	int _x;
	int _y;
	int _vx;
	int _vy;
	int _width;
	int _height;
	Granite::Color _color;
	bool _hitScreenLR;
	bool _hitScreenUD;
	static constexpr int _minWidth = 10;
	static constexpr int _maxWidth = 200;
	static constexpr int _minHeight = 10;
	static constexpr int _maxHeight = 200;
	static constexpr int _maxVelocity = 10;
};