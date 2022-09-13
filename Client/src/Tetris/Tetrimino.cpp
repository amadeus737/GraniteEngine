#include "Tetrimino.h"

int Tetrimino::xmin() { return _xmin; }
int Tetrimino::xmax() { return _xmax; }
int Tetrimino::ymin() { return _ymin; }
int Tetrimino::ymax() { return _ymax; }

Granite::Color& Tetrimino::Color()
{
	return _color;
}

Coordinates Tetrimino::bottom_coord()
{
	return _bottom_coord;
}

Coordinates& Tetrimino::top_coord()
{
	return _top_coord;
}

Coordinates& Tetrimino::left_coord()
{
	return _left_coord;
}

Coordinates& Tetrimino::right_coord()
{
	return _right_coord;
}

Coordinates& Tetrimino::GetCoordinate(int i)
{
	return _tileLocations[i];
}

TetriminoIdentifier Tetrimino::GetIdentifier()
{
	return _TetriminoIdentifier;
}

void Tetrimino::Rotate(int rotation)
{
	switch (_TetriminoIdentifier)
	{
	case TetriminoIdentifier::I:
		Spawn_I(rotation);
		break;

	case TetriminoIdentifier::O:
		Spawn_O(rotation);
		break;

	case TetriminoIdentifier::T:
		Spawn_T(rotation);
		break;

	case TetriminoIdentifier::S:
		Spawn_S(rotation);
		break;

	case TetriminoIdentifier::Z:
		Spawn_Z(rotation);
		break;

	case TetriminoIdentifier::J:
		Spawn_J(rotation);
		break;

	case TetriminoIdentifier::L:
		Spawn_L(rotation);
		break;
	}
}

void Tetrimino::Spawn_Random(std::mt19937& rng)
{
	// Seven unique tetriminoes (non-rotated)s
	std::uniform_int_distribution<int> Tetrimino_dist(0, 6);

	// Pick a Tetrimino randomly
	int random_Tetrimino_index = Tetrimino_dist(rng);

	switch (random_Tetrimino_index)
	{
	case 0:
		Spawn_I(0);
		_TetriminoIdentifier = TetriminoIdentifier::I;
		break;

	case 1:
		Spawn_O(0);
		_TetriminoIdentifier = TetriminoIdentifier::O;
		break;

	case 2:
		Spawn_T(0);
		_TetriminoIdentifier = TetriminoIdentifier::T;
		break;

	case 3:
		Spawn_S(0);
		_TetriminoIdentifier = TetriminoIdentifier::S;
		break;

	case 4:
		Spawn_Z(0);
		_TetriminoIdentifier = TetriminoIdentifier::Z;
		break;

	case 5:
		Spawn_J(0);
		_TetriminoIdentifier = TetriminoIdentifier::J;
		break;

	case 6:
		Spawn_L(0);
		_TetriminoIdentifier = TetriminoIdentifier::L;
		break;
	}
}

void Tetrimino::Spawn_I(int rotation)
{
	_TetriminoIdentifier = TetriminoIdentifier::I;
	_color = I_color;

	_xmin = 9999;
	_xmax = -9999;
	_ymin = 9999;
	_ymax = -9999;
	for (int i = 0; i < 4; i++)
	{
		switch (rotation)
		{
		case 0:
		case 180:
			_tileLocations[i] = _I_0[i];
			break;

		case 90:
		case 270:
			_tileLocations[i] = _I_90[i];
			break;
		}

		if (_tileLocations[i].x < _xmin)
		{
			_xmin = _tileLocations[i].x;
			_left_coord.x = _xmin;
			_left_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].x > _xmax)
		{
			_xmax = _tileLocations[i].x;
			_right_coord.x = _xmax;
			_right_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].y < _ymin)
		{
			_ymin = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymin;
		}

		if (_tileLocations[i].y > _ymax)
		{
			_ymax = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymax;
		}
	}
}

void Tetrimino::Spawn_O(int rotation)
{
	_TetriminoIdentifier = TetriminoIdentifier::O;
	_color = O_color;

	_xmin = 9999;
	_xmax = -9999;
	_ymin = 9999;
	_ymax = -9999;
	for (int i = 0; i < 4; i++)
	{
		_tileLocations[i] = _O_0[i];

		if (_tileLocations[i].x < _xmin)
		{
			_xmin = _tileLocations[i].x;
			_left_coord.x = _xmin;
			_left_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].x > _xmax)
		{
			_xmax = _tileLocations[i].x;
			_right_coord.x = _xmax;
			_right_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].y < _ymin)
		{
			_ymin = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymin;
		}

		if (_tileLocations[i].y > _ymax)
		{
			_ymax = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymax;
		}
	}
}

void Tetrimino::Spawn_T(int rotation)
{
	_TetriminoIdentifier = TetriminoIdentifier::T;
	_color = T_color;

	_xmin = 9999;
	_xmax = -9999;
	_ymin = 9999;
	_ymax = -9999;
	for (int i = 0; i < 4; i++)
	{
		switch (rotation)
		{
		case 0:
			_tileLocations[i] = _T_0[i];
			break;

		case 90:
			_tileLocations[i] = _T_90[i];
			break;

		case 180:
			_tileLocations[i] = _T_180[i];
			break;

		case 270:
			_tileLocations[i] = _T_270[i];
			break;
		}

		if (_tileLocations[i].x < _xmin)
		{
			_xmin = _tileLocations[i].x;
			_left_coord.x = _xmin;
			_left_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].x > _xmax)
		{
			_xmax = _tileLocations[i].x;
			_right_coord.x = _xmax;
			_right_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].y < _ymin)
		{
			_ymin = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymin;
		}

		if (_tileLocations[i].y > _ymax)
		{
			_ymax = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymax;
		}
	}
}

void Tetrimino::Spawn_S(int rotation)
{
	_TetriminoIdentifier = TetriminoIdentifier::S;
	_color = S_color;

	_xmin = 9999;
	_xmax = -9999;
	_ymin = 9999;
	_ymax = -9999;
	for (int i = 0; i < 4; i++)
	{
		switch (rotation)
		{
		case 0:
		case 180:
			_tileLocations[i] = _S_0[i];
			break;

		case 90:
		case 270:
			_tileLocations[i] = _S_90[i];
			break;
		}

		if (_tileLocations[i].x < _xmin)
		{
			_xmin = _tileLocations[i].x;
			_left_coord.x = _xmin;
			_left_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].x > _xmax)
		{
			_xmax = _tileLocations[i].x;
			_right_coord.x = _xmax;
			_right_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].y < _ymin)
		{
			_ymin = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymin;
		}

		if (_tileLocations[i].y > _ymax)
		{
			_ymax = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymax;
		}
	}
}

void Tetrimino::Spawn_Z(int rotation)
{
	_TetriminoIdentifier = TetriminoIdentifier::Z;
	_color = Z_color;

	_xmin = 9999;
	_xmax = -9999;
	_ymin = 9999;
	_ymax = -9999;
	for (int i = 0; i < 4; i++)
	{
		switch (rotation)
		{
		case 0:
		case 180:
			_tileLocations[i] = _Z_0[i];
			break;

		case 90:
		case 270:
			_tileLocations[i] = _Z_90[i];
			break;
		}

		if (_tileLocations[i].x < _xmin)
		{
			_xmin = _tileLocations[i].x;
			_left_coord.x = _xmin;
			_left_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].x > _xmax)
		{
			_xmax = _tileLocations[i].x;
			_right_coord.x = _xmax;
			_right_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].y < _ymin)
		{
			_ymin = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymin;
		}

		if (_tileLocations[i].y > _ymax)
		{
			_ymax = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymax;
		}
	}
}

void Tetrimino::Spawn_J(int rotation)
{
	_TetriminoIdentifier = TetriminoIdentifier::J;
	_color = J_color;

	_xmin = 9999;
	_xmax = -9999;
	_ymin = 9999;
	_ymax = -9999;
	for (int i = 0; i < 4; i++)
	{
		switch (rotation)
		{
		case 0:
			_tileLocations[i] = _J_0[i];
			break;

		case 90:
			_tileLocations[i] = _J_90[i];
			break;

		case 180:
			_tileLocations[i] = _J_180[i];
			break;

		case 270:
			_tileLocations[i] = _J_270[i];
			break;
		}

		if (_tileLocations[i].x < _xmin)
		{
			_xmin = _tileLocations[i].x;
			_left_coord.x = _xmin;
			_left_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].x > _xmax)
		{
			_xmax = _tileLocations[i].x;
			_right_coord.x = _xmax;
			_right_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].y < _ymin)
		{
			_ymin = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymin;
		}

		if (_tileLocations[i].y > _ymax)
		{
			_ymax = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymax;
		}
	}
}

void Tetrimino::Spawn_L(int rotation)
{
	_TetriminoIdentifier = TetriminoIdentifier::L;
	_color = L_color;

	_xmin = 9999;
	_xmax = -9999;
	_ymin = 9999;
	_ymax = -9999;
	for (int i = 0; i < 4; i++)
	{
		switch (rotation)
		{
		case 0:
			_tileLocations[i] = _L_0[i];
			break;

		case 90:
			_tileLocations[i] = _L_90[i];
			break;

		case 180:
			_tileLocations[i] = _L_180[i];
			break;

		case 270:
			_tileLocations[i] = _L_270[i];
			break;
		}

		if (_tileLocations[i].x < _xmin)
		{
			_xmin = _tileLocations[i].x;
			_left_coord.x = _xmin;
			_left_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].x > _xmax)
		{
			_xmax = _tileLocations[i].x;
			_right_coord.x = _xmax;
			_right_coord.y = _tileLocations[i].y;
		}

		if (_tileLocations[i].y < _ymin)
		{
			_ymin = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymin;
		}

		if (_tileLocations[i].y > _ymax)
		{
			_ymax = _tileLocations[i].y;
			_top_coord.x = _tileLocations[i].x;
			_top_coord.y = _ymax;
		}
	}
}

void Tetrimino::Draw(Map& map, const Coordinates& coords, Granite::Color colorOverride)
{
	for (int i = 0; i < 4; i++)
	{
		map.DrawCell(ADD(coords, _tileLocations[i]), colorOverride.IsEqual(Granite::Color::Black()) ? _color : colorOverride);
	}
}

void Tetrimino::DrawGhost(Map& map, int thickness, const Coordinates& coords)
{
	for (int i = 0; i < 4; i++)
	{
		map.DrawCellBorder(ADD(coords, _tileLocations[i]), thickness, _color);
	}
}