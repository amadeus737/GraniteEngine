#pragma once

#include "Core.h"
#include "Triangle.h"
#include <vector>

namespace Granite
{
	template <typename T>
	class GRANITE_API Mesh
	{
	public:
		Mesh()
			:
			triangles()
		{ }

		std::vector<Triangle<T>> triangles;
	};
}