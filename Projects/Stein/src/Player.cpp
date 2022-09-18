#include "Player.h"
#include <math.h>

#define PI 3.1415926535

Player::Player(Granite::Vector2f startPosition)
	:
	_position(startPosition),
	_direction(Granite::Vector2f(0, 0)),
	_perpDirection(Granite::Vector2f(0, 0)),
	_fov(60.0f * PI / 180.0f),
	_angle(0.1f)
{ 
	_tan_half_fov = tan(0.5f * _fov);
}

Player::Player(Granite::Vector2f startPosition, float fov, float startAngle)
	:
	_position(startPosition),
	_direction(Granite::Vector2f(0, 0)),
	_perpDirection(Granite::Vector2f(0, 0)),
	_fov(fov * PI / 180),
	_angle(startAngle * PI / 180)
{ 
	_tan_half_fov = tan(0.5f * _fov);
}

void Player::Move(Granite::Vector2f velocity)
{
	_position += velocity * _direction;
}

void Player::Rotate(float deltaAngle)
{
	_angle += _turnRate * deltaAngle;

	if (_angle < 0)      _angle += 2 * PI;
	if (_angle > 2 * PI) _angle -= 2 * PI;

	// Note perp direction is derived as cross product with vector z = k^ ... 
	// i.e., we have dir = (d0)i^ + (d1)j^ and z = k^. Cross product result between 
	// dir x z = (d1 * 1 - 0 * 0)i^ + (0 * 0 - d0 * 1)j^ + (d0 * 0 - d1 * 0)k^...
	// So, perpDirection = (d1)i^ + (-d0)j^.
	// Also, looking at a right triangle with angle fov / 2 and dir vector adjacent
	// and perpDir vector opposite this angle, it is easy to see that in terms of
	// magnitude of these vectors, we have:
	//   tan(fov/2) = ||perpDir|| / ||dir||   (where || x || is the magnitude of x).
	// Then, 
	//   ||perpDir|| = ||dir|| * tan(fov/2)...this is why the _tan_half_fov is multiplied
	// below.
	_direction = Granite::Vector2f(cos(_angle), -sin(_angle));
	_perpDirection = Granite::Vector2f(_direction.y * _tan_half_fov, -_direction.x * _tan_half_fov);
}

void Player::CalculateFOVposition(float distance, Granite::Vector2f& fovPos0, Granite::Vector2f& fovPos1) const
{
	fovPos0.x = distance * _direction.x - distance * _perpDirection.x;
	fovPos0.y = distance * _direction.y - distance * _perpDirection.y;
	fovPos1.x = distance * _direction.x + distance * _perpDirection.x;
	fovPos1.y = distance * _direction.y + distance * _perpDirection.y;
}

void Player::DrawOnMap2D(int scale, int size, Granite::Color color) const
{
	GraniteFrameBuffer.DrawRect(_position.x * scale - size / 2, _position.y * scale - size / 2, size, size, color);
	GraniteFrameBuffer.DrawLine(_position.x * scale, _position.y * scale, _position.x * scale + 15 * _direction.x, _position.y * scale + 15 * _direction.y,
	 3, color);

	Granite::Vector2f fovPos0, fovPos1;
	CalculateFOVposition(2.0f, fovPos0, fovPos1);

	GraniteFrameBuffer.DrawLine(_position.x * scale, _position.y * scale, _position.x * scale + 2 * scale * _direction.x, _position.y * scale + 2 * scale * _direction.y, 2, Granite::Color::Red());

	GraniteFrameBuffer.DrawLine(_position.x * scale, _position.y * scale, scale * (_position.x + fovPos0.x), scale * (_position.y + fovPos0.y), 2, Granite::Color::Yellow());
	GraniteFrameBuffer.DrawLine(_position.x * scale, _position.y * scale, scale * (_position.x + fovPos1.x), scale * (_position.y + fovPos1.y), 2, Granite::Color::Yellow());

	GraniteFrameBuffer.DrawLine(scale * (_position.x + 2 * _direction.x), scale * (_position.y + 2 * _direction.y), scale * (_position.x + fovPos0.x), scale * (_position.y + fovPos0.y), 2, Granite::Color::Blue());
	GraniteFrameBuffer.DrawLine(scale * (_position.x + 2 * _direction.x), scale * (_position.y + 2 * _direction.y), scale * (_position.x + fovPos1.x), scale * (_position.y + fovPos1.y), 2, Granite::Color::Blue());
}
