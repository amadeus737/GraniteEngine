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
		_map(m),
		_wallStartCoords(),
		_wallEndCoords()
	{ }

	void Raycast(bool draw2Dmap = false);
	void Setup(bool draw2Dmap);
	float Calculate_Horizontal_Intersections();
	float Calculate_Vertical_Intersections();
	void Cache_Trig_Values(float rayAngle);
	void Calculate_Shading(float dist_hor, float dist_ver);

	void Draw2Drays();
	void DrawWallColumn(int column, int wallOffset, float texScale_x = 1.0f, float texScale_y = 1.0f, bool draw2Dmap = false);
	void DrawFloors(int wallOffset, float texScale_x = 1.0f, float texScale_y = 1.0f, bool draw2Dmap = false);
	void DrawCeilings(int wallOffset, float texScale_x = 1.0f, float texScale_y = 1.0f, bool draw2Dmap = false);
	
	float ClampAngle(float angle, float min, float max);
	bool ClampToMap();

private:
	Player& _player;
	const Map& _map;
	std::vector<int> _wallStartCoords;
	std::vector<int> _wallEndCoords;
	float _angle = 0.0f;
	float _cos_a = 0.0f;
	float _sin_a = 0.0f;
	float _tan_a = 0.0f;
	float _rx = 0.0f;
	float _ry = 0.0f;
	int _dof = 0;
	int _max_dof = 8;
	float _ver_rx = 0.0f;
	float _ver_ry = 0.0f;
	float _hor_rx = 0.0f;
	float _hor_ry = 0.0f;
	float _shade = 0.0f;
	WallHitType _wallHitType = WallHitType::NONE;
	int _h_texture_index = 0;
	int _v_texture_index = 0;
	int _texture_index = 0;	
	float _markerSize = 8;
	int _drawWidth = 8;
};