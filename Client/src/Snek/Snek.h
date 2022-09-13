#pragma once

#include <GraniteEngine.h>
#include <random>
#include "../Common/Map.h"
#include "Snake.h"
#include "Food.h"

class Snek : GraniteGame
{
public:
	// Constructor
	Snek();

	// Class methods
	void Start();
	void RestartGame();
	void Input();
	void Update();
	void Render();

private:
	Map _map;
	std::mt19937 _rng;
	Snake _snake;
	Food _food;
	Coordinates _delta = { 1,0 };
	static constexpr float _movePeriod = 0.2f; // number of frames needed before move update
	float _moveCounter = 0.0f;
	bool _gameStarted = false;
	bool _gameOver = false;
	int _gameOverDrawCounts = 0;
};