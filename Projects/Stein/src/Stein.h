#pragma once

#include <GraniteEngine.h>
#include <GraniteGame.h>
#include <vector>
#include <math.h>
#include "Map.h"
#include "Player.h"
#include "Raycaster.h"

#define PI 3.141926535

class Stein : GraniteGame
{
public:
	Stein();

	void Start();
	void Input();
	void Update();
	void Render();

private:
	void CheckWallCollisions(int& fwd_x, int& fwd_y, int& back_x, int& back_y, bool& isDoor, int& doorIndex);
	void DrawMap();

private:
	Map _map;
	Player _player;
	std::vector<int> _walls;
	Raycaster _raycaster;
	Granite::Vector2f _playerFwdVelocity;
	Granite::Vector2f _playerBwdVelocity;
	bool _wantsToDraw2Dmap = false;
	bool _wantsToOpenDoor = false;
	bool _draw2Dmap = false;
	float _keyCooldownTime = 0.7f;
	float _keyTimer = 0.0f;
};
