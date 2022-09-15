#include "Player.h"
#include <math.h>

#define PI 3.1415926535

Player::Player(Granite::Vector2f startPosition)
	:
	_position(startPosition),
	_direction(Granite::Vector2f(0, 0))
{
	_angle = 0.1f;
}

Player::Player(Granite::Vector2f startPosition, float startAngle)
	:
	_position(startPosition),
	_direction(Granite::Vector2f(0, 0))
{
	_angle = startAngle;
}

void Player::MoveX(float speed)
{
	_position.x += speed * _direction.x;
}

void Player::MoveY(float speed)
{
	_position.y += speed * _direction.y;
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

	_direction = Granite::Vector2f(cos(_angle), -sin(_angle));
}

void Player::DrawOnMap2D(int scale, int size, Granite::Color color) const
{
	GraniteFrameBuffer.DrawRect(_position.x * scale - size / 2, _position.y * scale - size / 2, size, size, color);
	GraniteFrameBuffer.DrawLine(_position.x * scale, _position.y * scale, _position.x * scale + 15 * _direction.x, _position.y * scale + 15 * _direction.y, 3, color);
}
