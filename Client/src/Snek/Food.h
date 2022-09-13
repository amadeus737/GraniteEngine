#pragma once

#include <GraniteEngine.h>
#include "../Common/Map.h"
#include "Snake.h"
#include <random>

class Food
{
public:
	// Constructor
	Food(std::mt19937& rng, const Map& map, const Snake& snake);

	// Getter
	const Coordinates& coords() const;

	// Class Methods
	void Respawn(std::mt19937& rng, const Map& map, const Snake& snake);
	void Draw(Map& map, bool clear = false) const;

private:
	static constexpr Granite::Color _color = Granite::Color(255, 0, 0);
	Coordinates _coords;
};