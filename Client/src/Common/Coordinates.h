#pragma once

class Coordinates
{
public:
	bool operator==(const Coordinates& c) const;
	void Add(const Coordinates& c);

	int x;
	int y;
};