#pragma once

#include <GraniteEngine.h>
#include <vector>
#include "Player.h"

#define PI 3.141926535
#define DR 0.0174533 // 1 deg in rads

using namespace Granite;

class Raycaster
{
public:
	Raycaster(Player& p, const Map& m, const std::vector<int>& w, const std::vector<unsigned char>& wt)
		:
		_player(p),
		_map(m),
		_walls(w),
		_wallTextures(wt)
	{ }

	void Raycast(float fov);

	float CastHorizontalRay();
	float CastVerticalRay();

	void Draw2Drays();
	void DrawWallColumn(int column, int width, float shade);

	float ClampAngle(float angle, float min, float max);
	bool ClampToMap();

private:
	Player& _player;
	const Map& _map;
	const std::vector<int>& _walls;
	const std::vector<unsigned char>& _wallTextures;
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
	int _dof = 0;
	int _max_dof = 8;
};