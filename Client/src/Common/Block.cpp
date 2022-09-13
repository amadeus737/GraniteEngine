#include "Block.h"

Block::Block()
	:
	_x(100),
	_y(200),
	_vx(0),
	_vy(0),
	_width(50),
	_height(50),
	_color(Granite::Color::White()),
	_hitScreenLR(false),
	_hitScreenUD(false)
{ }

void Block::SetSpeed(int x_amount, int y_amount)
{
	// First, increment the velocity
	_vx = x_amount;
	_vy = y_amount;

	// Next, make sure that velocities do not exceed maximum allowable limits
	if (_vx > _maxVelocity) _vx = _maxVelocity;
	if (_vy > _maxVelocity) _vy = _maxVelocity;
	if (_vx < -_maxVelocity) _vx = -_maxVelocity;
	if (_vy < -_maxVelocity) _vy = -_maxVelocity;

	// Use the new velocity to increment the position
	_x += _vx;
	_y += _vy;

	// Make sure that we aren't exceeding the bounds of the screen
	_hitScreenLR = false;
	_hitScreenUD = false;
	if (_x + _width >= _window.ScreenWidth())
	{
		_vx = 0;
		_x = _window.ScreenWidth() - 1 - _width;
		_hitScreenLR = true;
	}

	if (_x < 0)
	{
		_vx = 0;
		_x = 0;
		_hitScreenLR = true;
	}

	if (_y < 0)
	{
		_vy = 0;
		_y = 0;
		_hitScreenUD = true;
	}

	if (_y + _height >= _window.ScreenHeight())
	{
		_vy = 0;
		_y = _window.ScreenHeight() - 1 - _height;
		_hitScreenUD = true;
	}
}

int Block::x() { return _x; }
int Block::y() { return _y; }
int Block::width() { return _width; }
int Block::height() { return _height; }
Granite::Color Block::color() { return _color; }
bool Block::HitScreenLR() { return _hitScreenLR; }
bool Block::HitScreenUD() { return _hitScreenUD; }