#pragma once

#include <GraniteEngine.h>
#include <assert.h>
#include "../Common/Map.h"

class Snake
{
private:
	class Segment
	{
	public:
		// Getter
		const Coordinates& coords() const { return _coords; }

		// Class methods
		void InitializeHead(const Coordinates& location);
		void InitializeBody(Granite::Color c);
		void Follow(const Segment& next);
		void Move(const Coordinates& delta);
		void Draw(Map& map, bool clear = false) const;

	private:
		Coordinates _coords;
		Granite::Color _color;
	};

public:
	// Constructor
	Snake(const Coordinates& startLocation);

	// Class methods
	void InitializeSnake(const Coordinates& startLocation);
	void Move(const Coordinates& delta);
	Coordinates GetNextHeadLocation(const Coordinates& delta) const;
	void Reset(const Coordinates& spawnLocation);
	void Grow();
	void Draw(Map& map) const;
	bool InTileExcludeEnd(const Coordinates& c) const;
	bool InTile(const Coordinates& c) const;

private:
	static constexpr Granite::Color _headColor = Granite::Color(255, 255, 0);
	static constexpr Granite::Color _bodyColor = Granite::Color(0, 255, 0);
	static constexpr int _maxSegments = 100;
	int _nSegments = 1;
	Segment _segments[_maxSegments];
};