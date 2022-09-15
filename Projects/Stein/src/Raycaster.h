#pragma once

#include <GraniteEngine.h>
#include <vector>
#include "Player.h"

#define PI 3.141926535
#define DR 0.0174533 // 1 deg in rads

using namespace Granite;

enum class WallHitType { NONE, HORIZONTAL, VERTICAL };

class Raycaster
{
public:
	Raycaster(Player& p, const Map& m)
		:
		_player(p),
		_map(m)
	{ }

	void Raycast(float fov, bool draw2Dmap = false);

	float CastHorizontalRay();
	float CastVerticalRay();

	void Draw2Drays();
	void DrawWallColumn(WallHitType hitType, float angle, int column, int width, float shade, bool draw2Dmap = false);

	float ClampAngle(float angle, float min, float max);
	bool ClampToMap();

private:
	Player& _player;
	const Map& _map;
	float _cos_a = 0.0f;
	float _sin_a = 0.0f;
	float _tan_a = 0.0f;
	float _rx = 0.0f;
	float _ry = 0.0f;
	float _ver_rx = 0.0f;
	float _ver_ry = 0.0f;
	float _hor_rx = 0.0f;
	float _hor_ry = 0.0f;
	float _markerSize = 8;
	int _h_texture_index = 0;
	int _v_texture_index = 0;
	int _texture_index = 0;
	int _dof = 0;
	int _max_dof = 8;
};