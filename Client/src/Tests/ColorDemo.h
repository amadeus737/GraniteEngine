#pragma once

#include <GraniteEngine.h>
#include <random>
#include "../Common/Map.h"

class ColorDemo : GraniteGame
{
public:
	// Constructor
	ColorDemo();

	// Class methods
	void Update();

private:
	Map _map;
	std::mt19937 _rng;
};