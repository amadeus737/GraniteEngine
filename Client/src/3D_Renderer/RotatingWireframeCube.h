#pragma once

#include <GraniteEngine.h>
#include <vector>

struct vec3d
{
	float x, y, z;
};

struct triangle
{
	vec3d p[3];
};

struct mesh
{
	std::vector<triangle> tris;
};

struct mat4x4
{
	float m[4][4] = { 0 };
};

class RotatingWireframeCube : GraniteGame
{
public:
	RotatingWireframeCube() { }

	void Start();
	void Update();

private:
	void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m);

private:
	mesh meshCube;
	mat4x4 matProj;
	float fTheta = 0.0f;
};
