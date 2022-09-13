#pragma once

#include <GraniteEngine.h>
#include <random>
#include "../Common/Map.h"

#define ADD(a,b) { a.x + b.x, a.y + b.y }

enum class TetriminoIdentifier
{
	NONE, I, T, O, S, Z, J, L
};

class Tetrimino
{
public:
	// Getters
	int xmin();
	int xmax();
	int ymin();
	int ymax();
	Granite::Color& Color();
	Coordinates bottom_coord();
	Coordinates& top_coord();
	Coordinates& left_coord();
	Coordinates& right_coord();
	Coordinates& GetCoordinate(int i);
	TetriminoIdentifier GetIdentifier();

	// Methods
	void Rotate(int rotation);

	// Spawn functions
	void Spawn_Random(std::mt19937& rng);
	void Spawn_I(int rotation);
	void Spawn_O(int rotation);
	void Spawn_T(int rotation);
	void Spawn_S(int rotation);
	void Spawn_Z(int rotation);
	void Spawn_J(int rotation);
	void Spawn_L(int rotation);

	// Drawing functions
	void Draw(Map& map, const Coordinates& coords, Granite::Color colorOverride = Granite::Color::Black());
	void DrawGhost(Map& map, int thickness, const Coordinates& coords);

	// Tetrimino colors
	static constexpr Granite::Color I_color = Granite::Color(50u, 255u, 255u);
	static constexpr Granite::Color O_color = Granite::Color(255u, 255u, 50u);
	static constexpr Granite::Color T_color = Granite::Color(255u, 50u, 255u);
	static constexpr Granite::Color S_color = Granite::Color(50u, 255u, 50u);
	static constexpr Granite::Color Z_color = Granite::Color(255u, 50u, 50u);
	static constexpr Granite::Color J_color = Granite::Color(50u, 100u, 255u);
	static constexpr Granite::Color L_color = Granite::Color(255u, 200u, 50u);

private:
	static constexpr Coordinates _I_0[4] = { {0,0}, {1,0}, {2,0}, {3,0} };
	static constexpr Coordinates _I_90[4] = { {0,0}, {0,1}, {0,2}, {0,3} };
	static constexpr Coordinates _O_0[4] = { {1,0}, {2,0}, {1,1}, {2,1} };
	static constexpr Coordinates _T_0[4] = { {0,0}, {1,0}, {2,0}, {1,1} };
	static constexpr Coordinates _T_90[4] = { {0,1}, {1,0}, {1,1}, {1,2} };
	static constexpr Coordinates _T_180[4] = { {1,0}, {0,1}, {1,1}, {2,1} };
	static constexpr Coordinates _T_270[4] = { {1,0}, {1,1}, {1,2}, {2,1} };
	static constexpr Coordinates _S_0[4] = { {1,0}, {2,0}, {0,1}, {1,1} };
	static constexpr Coordinates _S_90[4] = { {1,0}, {1,1}, {2,1}, {2,2} };
	static constexpr Coordinates _Z_0[4] = { {0,0}, {1,0}, {1,1}, {2,1} };
	static constexpr Coordinates _Z_90[4] = { {1,1}, {1,2}, {2,0}, {2,1} };
	static constexpr Coordinates _J_0[4] = { {2,0}, {2,1}, {1,2}, {2,2} };
	static constexpr Coordinates _J_90[4] = { {0,0}, {0,1}, {1,1}, {2,1} };
	static constexpr Coordinates _J_180[4] = { {1,0}, {2,0}, {1,1}, {1,2} };
	static constexpr Coordinates _J_270[4] = { {0,0}, {1,0}, {2,0}, {2,1} };
	static constexpr Coordinates _L_0[4] = { {1,0}, {1,1}, {1,2}, {2,2} };
	static constexpr Coordinates _L_90[4] = { {2,0}, {0,1}, {1,1}, {2,1} };
	static constexpr Coordinates _L_180[4] = { {1,0}, {2,0}, {2,1}, {2,2} };
	static constexpr Coordinates _L_270[4] = { {0,0}, {1,0}, {2,0}, {0,1} };
	Granite::Color _color;
	Coordinates _tileLocations[4];
	int _xmin;
	int _xmax;
	int _ymin;
	int _ymax;
	Coordinates _bottom_coord;
	Coordinates _top_coord;
	Coordinates _left_coord;
	Coordinates _right_coord;
	TetriminoIdentifier _TetriminoIdentifier = TetriminoIdentifier::NONE;
};