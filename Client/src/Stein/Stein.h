#pragma once

#include <GraniteEngine.h>
#include <vector>
#include <math.h>
#include "../Common/Map.h"
#include "Player.h"
#include "Raycaster.h"

#define PI 3.141926535

class Stein : GraniteGame
{
public:
	// Constructor
	Stein();

	// Class methods
	void Start();
	void Restart();
	void Input();
	void Update();
	void Render();

private:
	void CheckWallCollisions(int& fwd_x, int& fwd_y, int& back_x, int& back_y);
	void DrawMap();

private:
	Map _map;
	Player _player;
	std::vector<int> _walls;
	std::vector<unsigned char> _wallTextures;
	Raycaster _raycaster;
	Granite::Vector2f _playerFwdVelocity;
	Granite::Vector2f _playerBwdVelocity;
};
