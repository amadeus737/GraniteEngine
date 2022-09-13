#include "Raycaster.h"
#include <math.h>

#define texture_size 32

void Raycaster::Raycast(float fov)
{
	// Setup size of marker plot points -- don't allow odd values	
	int _markerSize = 8;
	if (_markerSize % 2 == 1) _markerSize++;

	// Initialize values
	_rx = 0.0f;
	_ry = 0.0f;

	// Get the (i,j) coordinates of the cell containing the _player
	int p_i = (int)_player.x();
	int p_j = (int)_player.y();

	// Color the tile that the _player is currently in red
	_frameBuffer.DrawRect(p_i * _map.cellsize(), p_j * _map.cellsize(), _map.cellsize(), _map.cellsize(), Granite::Color(100u, 100u, 100u));

	int column = 0;
	for (float rayAngle = -DR * fov / 2; rayAngle <= DR * fov / 2; rayAngle += DR)
	{
		// Calculate new angle within FOV, clamped within {0, 2*PI}
		float angle = ClampAngle(_player.angle() + rayAngle, 0, 2 * PI);

		// Cache trig values to avoid unnecessary recalculation
		_cos_a = cos(angle);
		_sin_a = sin(angle);
		_tan_a = _sin_a / _cos_a;

		// Cast rays
		float ver_dist = CastVerticalRay();
		float hor_dist = CastHorizontalRay();

		// The shorter of the two ray distances (ver or hor) is our ray of closest intersection. Use distance and intersection type to calculate shading.
		float distance = 0.0;
		float shade = 1.0f;
		if (ver_dist < hor_dist)
		{
			_rx = _ver_rx;
			_ry = _ver_ry;
			distance = ver_dist;
			shade = 0.5f;
		}
		else
		{
			// Making the horizontal walls just a tad darker with the shade value
			// gives the impression of lighting calculations
			_rx = _hor_rx;
			_ry = _hor_ry;
			distance = hor_dist;
			shade = 0.6f;
		}

		// Further scale shading based on the distance, normalized by the player visibility distance
		float shadeScale = (_player.visibilityDistance() - distance / _player.visibilityDistance());
		shade *= shadeScale;
		if (shade < 0.001f) shade = 0.0f;
		if (shade > 0.999f) shade = 1.0f;

		Draw2Drays();
		DrawWallColumn(column, 8, shade);

		// This variable tracks which column is being drawn
		column++;
	}
}

// Check vertical intersections
float Raycaster::CastVerticalRay()
{
	_dof = 0;
	float dx = 0.0;
	float dy = 0.0;

	if (_cos_a > 0.001)
	{
		// looking left
		_ver_rx = (int)_player.x() + 1.00001;
		_ver_ry = (_player.x() - _ver_rx) * _tan_a + _player.y();
		dx = 1;
		dy = -dx * _tan_a;
	}
	else if (_cos_a < -0.001)
	{
		// looking right
		_ver_rx = (int)_player.x() - 0.000001;
		_ver_ry = (_player.x() - _ver_rx) * _tan_a + _player.y();
		dx = -1;
		dy = -dx * _tan_a;
	}
	else
	{
		// looking straight up or down
		_ver_rx = _player.x();
		_ver_ry = _player.y();
		_dof = _max_dof;
	}

	float distance = 999999;
	while (_dof < _max_dof)
	{
		int tile_x = (int)_ver_rx;
		int tile_y = (int)_ver_ry;

		int index = tile_y * _map.width() + tile_x;
		if (index > 0 && index < _map.width() * _map.height() && _walls[index] > 0)
		{
			_dof = _max_dof;
			distance = (_ver_rx - _player.x()) / _cos_a;
		}
		else
		{
			_ver_rx += dx;
			_ver_ry += dy;
			_dof++;
		}
	}

	return distance;
}

// Check horizontal intersections
float Raycaster::CastHorizontalRay()
{
	_dof = 0;
	float dx = 0.0;
	float dy = 0.0;

	if (_sin_a > 0.001)
	{
		// looking up
		_hor_ry = (int)_player.y() - 0.00001;
		_hor_rx = (_player.y() - _hor_ry) / _tan_a + _player.x();
		dy = -1;
		dx = -dy / _tan_a;
	}
	else if (_sin_a < -0.001)
	{
		// looking down
		_hor_ry = (int)_player.y() + 1.00001;
		_hor_rx = (_player.y() - _hor_ry) / _tan_a + _player.x();
		dy = 1;
		dx = -dy / _tan_a;
	}
	else
	{
		// look straight left or right
		_hor_rx = _player.x();
		_hor_ry = _player.y();
		_dof = _max_dof;
	}

	float distance = 999999;
	while (_dof < _max_dof)
	{
		int tile_x = (int)_hor_rx;
		int tile_y = (int)_hor_ry;

		int index = tile_y * _map.width() + tile_x;
		if (index > 0 && index < _map.width() * _map.height() && _walls[index] > 0)
		{
			_dof = _max_dof;
			distance = (_hor_rx - _player.x()) / _cos_a;
		}
		else
		{
			_hor_rx += dx;
			_hor_ry += dy;
			_dof++;
		}
	}

	return distance;
}

void Raycaster::Draw2Drays()
{
	// Convert from dimensionless map indices to actual map coordinates
	float p_x = _player.x() * _map.cellsize();
	float p_y = _player.y() * _map.cellsize();
	float ray_x = _rx * _map.cellsize();
	float ray_y = _ry * _map.cellsize();

	// Draw the current ray and highlight the tile that the player is standing on
	_frameBuffer.DrawLine(p_x, p_y, ray_x, ray_y, 4, Granite::Color::Yellow());
	_frameBuffer.DrawRect(ray_x - 4, ray_y - 4, 8, 8, Granite::Color::Orange());
}

void Raycaster::DrawWallColumn(int column, int width, float shade)
{
	if (width % 2 != 0) width++;

	// Fix the fisheye effect
	float distance = (_rx - _player.x()) * _player.direction().x - (_ry - _player.y()) * -_player.direction().y;

	// Calculate wall height for this ray
	int wallHeight = int(_map.scaledHeight() / distance);
	int wallOffset = 150;

	float wallTex_y_step = (float)texture_size / (float)wallHeight;
	float wallTex_y_offset = 0;

	int wallStart = -wallHeight / 2 + _map.scaledHeight() / 2 + wallOffset;
	if (wallStart < 0) wallStart = 0;
	int wallEnd = wallHeight / 2 + _map.scaledHeight() / 2 + wallOffset;
	if (wallEnd >= _map.scaledHeight())
	{
		wallEnd = _map.scaledHeight() - 1;
		wallTex_y_offset = (wallStart - wallOffset - _map.scaledHeight() / 2.0 + wallHeight / 2.0);
	}

	// Draw the wall for this ray
	float wallTex_y = wallTex_y_offset * wallTex_y_step;
	for (int y = wallStart; y < wallEnd; y++)
	{
		float c = 255 * _wallTextures[(int)(wallTex_y) * 32];

		for (int x = -width / 2; x <= width / 2; x++)
		{
			_frameBuffer.DrawPixel(column * width + 530 - x,
				y,
				Granite::Color(c, c, c) * shade);
		}

		wallTex_y += wallTex_y_step;
	}
}

float Raycaster::ClampAngle(float angle, float min, float max)
{
	if (angle > (max - 1))
		angle -= max;

	if (angle < min)
		angle += max;

	return angle;
}

// Make sure intersection coordinates stay within the _map!
bool Raycaster::ClampToMap()
{
	bool hitWall = false;

	float m = (_ry - _player.y()) / (_rx - _player.x());
	float b = _player.x() - m * _player.y();

	if (_rx < _markerSize / 2)
	{
		_rx = _markerSize / 2;
		_ry = m * _rx + b;
		hitWall = true;
	}

	if (_rx + _markerSize > _map.scaledWidth() - 1 - _markerSize / 2)
	{
		_rx = _map.scaledWidth() - 1 - _markerSize / 2;
		_ry = m * _rx + b;
		hitWall = true;
	}

	if (_ry < _markerSize / 2)
	{
		_ry = _markerSize / 2;
		_rx = (_ry - b) / m;
		hitWall = true;
	}

	if (_ry + _markerSize > _map.scaledHeight() - 1 - _markerSize / 2)
	{
		_ry = _map.scaledHeight() - 1 - _markerSize / 2;
		_rx = (_ry - b) / m;
		hitWall = true;
	}

	return hitWall;
}