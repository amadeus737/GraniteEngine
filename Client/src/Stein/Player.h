#pragma once

#include <GraniteEngine.h>
#include "../Common/Map.h"
#include "Engine/Vector2.h"

class Player
{
public:
	Player(Granite::Vector2f startPosition);
	Player(Granite::Vector2f startPosition, float startAngle);

	Granite::Vector2f position() const { return _position; }
	Granite::Vector2f direction() const { return _direction; }
	float angle() const { return _angle; }
	float x() const { return _position.x; }
	float y() const { return _position.y; }
	float dir_x() const { return _direction.x; }
	float dir_y() const { return _direction.y; }
	float visibilityDistance() const { return _visibilityDistance; }

	void SetVisibilityDistance(float d) { _visibilityDistance = d; }

	void MoveX(const float speed);
	void MoveY(const float speed);
	void Move(Granite::Vector2f velocity);
	void Rotate(const float deltaAngle);

	void DrawOnMap2D(int scale, int size, Granite::Color color) const;

private:
	Granite::Vector2f _position;
	Granite::Vector2f _direction;
	float _angle = 0.0f;
	float _turnRate = 2.0f;
	float _visibilityDistance = 2.5f;
};