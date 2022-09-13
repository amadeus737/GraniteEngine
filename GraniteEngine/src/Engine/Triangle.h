#pragma once

#include "Core.h"

namespace Granite
{
	template<class T>
	class GRANITE_API Triangle
	{
	public:
		Triangle() { }

		Triangle(T v0, T v1, T v2)
		{
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
		}

		T vertices[3];
	};
}