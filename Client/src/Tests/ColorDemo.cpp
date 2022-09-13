#include "ColorDemo.h"

ColorDemo::ColorDemo()
	:
	_map(),
	_rng(std::random_device()())
{ }

void ColorDemo::Update()
{
	std::uniform_int_distribution<int> colorDist(0, 255);
	for (int y = 0; y < _map.height(); y++)
	{
		for (int x = 0; x < _map.width(); x++)
		{
			Coordinates coords = { x, y };
			Granite::Color c(colorDist(_rng), colorDist(_rng), colorDist(_rng));
			_map.DrawCell(coords, c);
		}
	}
}