#pragma once

#include <GraniteEngine.h>
#include "Map.h"
#include "Engine/Vector2.h"

class Player
{
public:
	Player(Granite::Vector2f startPosition);
	Player(Granite::Vector2f startPosition, float fov, float startAngle);

	Granite::Vector2f position() const { return _position; }
	Granite::Vector2f direction() const { return _direction; }
	Granite::Vector2f perpdirection() const { return _perpDirection; }
	float angle() const { return _angle; }
	float fov() const { return _fov; }
	float x() const { return _position.x; }
	float y() const { return _position.y; }
	float dir_x() const { return _direction.x; }
	float dir_y() const { return _direction.y; }
	float perpdir_x() const { return _perpDirection.x; }
	float perpdir_y() const { return _perpDirection.y; }
	float visibilityDistance() const { return _visibilityDistance; }

	void SetFOV(float fov) { _fov = fov; _tan_half_fov = tan(0.5f * fov); }
	void SetVisibilityDistance(float d) { _visibilityDistance = d; }
	void CalculateFOVposition(float distance, Granite::Vector2f& fovPos0, Granite::Vector2f& fovPos1) const;
	void Move(Granite::Vector2f velocity);
	void Rotate(const float deltaAngle);

	void DrawOnMap2D(int scale, int size, Granite::Color color) const;

private:
	Granite::Vector2f _position;
	Granite::Vector2f _direction;
	Granite::Vector2f _perpDirection;
	float _tan_half_fov = 0.0f;
	float _fov = 0.0f;
	float _angle = 0.0f;
	float _turnRate = 2.0f;
	float _visibilityDistance = 2.5f;
};