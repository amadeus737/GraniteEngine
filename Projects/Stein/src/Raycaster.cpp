#include "Raycaster.h"
#include <math.h>

#define texture_size 32

void Raycaster::Raycast(bool draw2Dmap)
{
	// Initialize some values
	Setup(draw2Dmap);

	if (draw2Dmap)
	{
		DrawCeilings(0, 0.3f, 0.3f, draw2Dmap);
		DrawFloors(0, 0.3f, 0.3f, draw2Dmap);
	}

	// Now loop over the rays within the player's FOV
	int column = 0;
	float degreeIncrement = 0.5f;
	_drawWidth = degreeIncrement * GraniteWindow.ScreenWidth() / (_player.fov() * 180 / PI);
	for (float rayAngle = -DR * _player.fov() * 180/PI / 2; rayAngle <= DR * _player.fov() * 180/PI / 2; rayAngle += degreeIncrement * DR)
	{
		// Calculate angle relative to player, clapm it to (0, 2*PI), cache cos, sin, and tan of it
		Cache_Trig_Values(rayAngle);

		// Cast rays, returning the distance of the ray from the player's position to the wall intersection (or max dof)
		float dist_hor = Calculate_Horizontal_Intersections();
		float dist_ver = Calculate_Vertical_Intersections();

		// Calculate shading needed for walls
		Calculate_Shading(dist_hor, dist_ver);

		// Draw 2D / 3D scene as requested by user
		if (draw2Dmap) Draw2Drays();

		DrawWallColumn(column, 0, 0.5f, 0.5f, draw2Dmap);
		
		// This variable tracks which column is being drawn
		column++;
	}

	if (!draw2Dmap)
	{
		DrawCeilings(0, 0.2f, 0.2f, draw2Dmap);
		DrawFloors(0, 0.2f, 0.2f, draw2Dmap);
	}
}

void Raycaster::Setup(bool draw2Dmap)
{
	// Setup size of plot points -- don't allow odd values	
	int _pointSize = 8;
	if (_pointSize % 2 == 1) _pointSize++;

	// Initialize ray coordinatevalues
	_rx = 0.0f;
	_ry = 0.0f;

	// Get the (i,j) coordinates of the cell containing the player's current position
	int p_i = (int)_player.x();
	int p_j = (int)_player.y();

	// Color the tile that the _player is currently in red
	if (draw2Dmap)
	{
	//	GraniteFrameBuffer.DrawRect(p_i * _map.cellsize(), p_j * _map.cellsize(), _map.cellsize(), _map.cellsize(), Granite::Color(100u, 100u, 100u));
	}

	_wallEndCoords.clear();
	_wallStartCoords.clear();
}

void Raycaster::Cache_Trig_Values(float rayAngle)
{
	// Calculate new angle within FOV, clamped within {0, 2*PI}
	rayAngle = ClampAngle(rayAngle, 0, 2 * PI);
	_angle = ClampAngle(_player.angle() + rayAngle, 0, 2 * PI);

	// Cache trig values to avoid unnecessary recalculation
	_cos_a = cos(_angle);
	_sin_a = sin(_angle);
	_tan_a = _sin_a / _cos_a;
}

void Raycaster::Calculate_Shading(float dist_hor, float dist_ver)
{
	// Initialize variables to defaults
	_wallHitType = WallHitType::NONE;
	_shade = 1.0f;
	float distance = 0.0;

	// The shorter of the two ray distances (ver or hor) is our ray of closest intersection. Use distance and intersection type to calculate shading.
	if (dist_ver < dist_hor)
	{
		distance = dist_ver;
		_rx = _ver_rx;
		_ry = _ver_ry;
		_shade = 0.3f;
		_wallHitType = WallHitType::VERTICAL;
		_texture_index = _v_texture_index;
	}
	else
	{
		// Making the horizontal walls just a tad darker with the shade value
		// gives the impression of lighting calculations
		distance = dist_hor;
		_rx = _hor_rx;
		_ry = _hor_ry;
		_shade = 1.0f;
		_wallHitType = WallHitType::HORIZONTAL;
		_texture_index = _h_texture_index;
	}

	// Further scale shading based on the distance, normalized by the player visibility distance
	float depthShading = (_player.visibilityDistance() - distance / _player.visibilityDistance());
	_shade *= depthShading;

	// Clamp shading between 0 and 1 since its just a multiplier
	if (_shade < 0.001f) _shade = 0.0f;
	if (_shade > 0.999f) _shade = 1.0f;
}

// Check vertical intersections
float Raycaster::Calculate_Vertical_Intersections()
{
	// Initialize values
	_dof = 0;         // depth-of-field (how many tiles out we have searched)
	float dx = 0.0;   // increment along x needed to reach next vertical intersection (beyond first intersection)
	float dy = 0.0;   // increment along y needed to reach next vertical intersection (beyond first intersection)

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
		if (index > 0 && index < _map.width() * _map.height() && _map.GetWalls()[index] > 0)
		{
			_dof = _max_dof;
			distance = (_ver_rx - _player.x()) / _cos_a;
			_v_texture_index = _map.GetWalls()[index] - 1;
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
float Raycaster::Calculate_Horizontal_Intersections()
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
		if (index > 0 && index < _map.width() * _map.height() && _map.GetWalls()[index] > 0)
		{
			_dof = _max_dof;
			distance = (_hor_rx - _player.x()) / _cos_a;
			_h_texture_index = _map.GetWalls()[index] - 1;
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
	return;

	// Convert from dimensionless map indices to actual map coordinates
	float p_x = _player.x() * _map.cellsize();
	float p_y = _player.y() * _map.cellsize();
	float ray_x = _rx * _map.cellsize();
	float ray_y = _ry * _map.cellsize();

	// Draw the current ray and highlight the tile that the player is standing on
	GraniteFrameBuffer.DrawLine(p_x, p_y, ray_x, ray_y, 4, Granite::Color::Yellow());
	GraniteFrameBuffer.DrawRect(ray_x - 4, ray_y - 4, 8, 8, Granite::Color::Orange());
}

void Raycaster::DrawFloors(int wallOffset, float texScale_x, float texScale_y, bool draw2Dmap)
{
	Granite::Vector2f fov0, fov1;
	_player.CalculateFOVposition(1, fov0, fov1);

	for (int y = GraniteWindow.ScreenHeight()/2 + wallOffset; y < GraniteWindow.ScreenHeight(); y++)
	{
		// Current y pos compared to horizon
		int dy = y - GraniteWindow.ScreenHeight() / 2 - wallOffset;
		float cam_z = (float)GraniteWindow.ScreenHeight() / 2.0f;
		float rowDist = cam_z / (float)dy;

		float step_x = rowDist * (fov1.x - fov0.x) / (float)(GraniteWindow.ScreenWidth());
		float step_y = rowDist * (fov1.y - fov0.y) / (float)(GraniteWindow.ScreenWidth());

		float floor_x = _player.x() + rowDist * fov0.x;
		float floor_y = _player.y() + rowDist * fov0.y;

		int floor_i = (int)(floor_x);
		int floor_j = (int)(floor_y);

		for (int x = 0; x < GraniteWindow.ScreenWidth(); x += _drawWidth)
		{
			int tex_u = (int)(texture_size * (floor_x - (float)floor_i) / texScale_x) & (texture_size - 1);
			int tex_v = (int)(texture_size * (floor_y - (float)floor_j) / texScale_y) & (texture_size - 1);

			int checkerBoardPattern = (int(floor_x) + int(floor_y)) % 2;
			int diagonalPattern = (int(floor_x + floor_y)) % 2;
			int floorTexture;
			if (diagonalPattern == 0) floorTexture = 1;
			else floorTexture = 2;
			
			float c = 255 * _map.GetWallTextures()[(int)(tex_v)*texture_size + (int)(tex_u)+32 * 32 * floorTexture] * 0.7f;

			if (draw2Dmap || y >= _wallEndCoords[x/_drawWidth])
			{
				for (int i = -_drawWidth / 2; i < _drawWidth / 2; i++)
				{
					GraniteFrameBuffer.DrawPixel(x + (draw2Dmap ? 530 : 0) - i, y, Granite::Color(c, c / 2, c / 2) * (float)(1.5f * (y - (GraniteWindow.ScreenHeight() / 2 + wallOffset)) / (GraniteWindow.ScreenHeight())));
				}
			}

			floor_x += step_x * _drawWidth;
			floor_y += step_y * _drawWidth;
		}
	}
}

void Raycaster::DrawCeilings(int wallOffset, float texScale_x, float texScale_y, bool draw2Dmap)
{
	Granite::Vector2f fov0, fov1;
	_player.CalculateFOVposition(1, fov0, fov1);

	for (int y = 0; y < GraniteWindow.ScreenHeight() / 2 + wallOffset; y++)
	{
		// Current y pos compared to horizon
		int dy = y - GraniteWindow.ScreenHeight() / 2 - wallOffset;
		float cam_z = (float)GraniteWindow.ScreenHeight() / 2.0f;
		float rowDist = cam_z / (float)dy;

		float step_x = rowDist * (fov1.x - fov0.x) / (float)GraniteWindow.ScreenWidth();
		float step_y = rowDist * (fov1.y - fov0.y) / (float)GraniteWindow.ScreenWidth();

		float floor_x = _player.x() + rowDist * fov0.x;
		float floor_y = _player.y() + rowDist * fov0.y;

		int floor_i = (int)(floor_x);
		int floor_j = (int)(floor_y);

		for (int x = 0; x < GraniteWindow.ScreenWidth(); x += _drawWidth)
		{
			if (draw2Dmap || y <= _wallStartCoords[x/_drawWidth])
			{
				int tex_u = (int)(texture_size * (floor_x - (float)floor_i) / texScale_x) & (texture_size - 1);
				int tex_v = (int)(texture_size * (floor_y - (float)floor_j) / texScale_y) & (texture_size - 1);

				float c = 255 * _map.GetWallTextures()[(int)(tex_v)*texture_size + (int)(tex_u)+32 * 32] * 0.3f;

				for (int i = -_drawWidth / 2; i < _drawWidth / 2; i++)
				{
					GraniteFrameBuffer.DrawPixel(x + (draw2Dmap ? 530 : 0) - i, y, Granite::Color(c/2, c, c / 2) * (float)(3.0f * (GraniteWindow.ScreenHeight() / 2 + wallOffset - y) / (GraniteWindow.ScreenHeight())));
				}
			}

			floor_x += step_x * _drawWidth;
			floor_y += step_y * _drawWidth;
		}
	}
}

void Raycaster::DrawWallColumn(int column, int wallOffset, float texScale_x, float texScale_y, bool draw2Dmap)
{
	// Fix the fisheye effect
	float distance = (_rx - _player.x()) * _player.direction().x - (_ry - _player.y()) * -_player.direction().y;

	// Calculate wall height for this ray
	int wallHeight = int(GraniteWindow.ScreenHeight() / distance);
	
	// Calculate starting y-coordinate for top of wall
	int wallStart = -wallHeight / 2 + GraniteWindow.ScreenHeight() / 2 + wallOffset;
	if (wallStart < 0) wallStart = 0;

	// Calculate y-coordinate for bottom of wall
	int wallEnd = wallHeight / 2 + GraniteWindow.ScreenHeight() / 2 + wallOffset;
	if (wallEnd >= GraniteWindow.ScreenHeight())	wallEnd = GraniteWindow.ScreenHeight() - 1;

	int tex_u = 0;
	int tex_v = 0;
	if (_wallHitType == WallHitType::HORIZONTAL)
	{
		tex_u = (int)(_rx / texScale_x * _map.cellsize() / 2.0) % texture_size;

		if (_angle > PI)
		{
			tex_u = texture_size - 1 - tex_u;
		}
	}
	else
	{
		tex_u = (int)(_ry / texScale_x * _map.cellsize() / 2.0) % texture_size;

		if (_angle > PI/2 && _angle < 3*PI/2)
		{
			tex_u = texture_size - 1 - tex_u;
		}
	}

	float tex_step = 32.0f / wallHeight;
	float tex_pos = (wallStart - wallOffset - GraniteWindow.ScreenHeight() / 2.0f + wallHeight / 2.0f) * tex_step;
	for (int y = wallStart; y < wallEnd; y++)
	{
		tex_v = (int)(tex_pos / texScale_y) & 31;

		float c = 255 * _map.GetWallTextures()[(int)(tex_v) * 32 + (int)(tex_u) + 32 * 32 * _texture_index] * _shade;

		for (int x = -_drawWidth / 2; x < _drawWidth / 2; x++)
		{
			GraniteFrameBuffer.DrawPixel(column * _drawWidth + (draw2Dmap ? 530 : 0) - x, y, Granite::Color(c, c, c/2));
		}

		tex_pos += tex_step;
	}

	if (wallStart < 0) wallStart = 0;
	if (wallEnd > GraniteWindow.ScreenHeight()) wallEnd = GraniteWindow.ScreenHeight() - 1;

	_wallStartCoords.push_back(wallStart);
	_wallEndCoords.push_back(wallEnd);
	_depthBuffer.push_back(distance * distance);
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