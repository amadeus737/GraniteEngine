#pragma once

#include "Core.h"
#include <math.h>

namespace Granite
{
	constexpr float PI = 3.14159265f;
	constexpr double PI_DBL = 3.1415926535897932;

	template<typename T>
	constexpr T sqr(const T& x)
	{
		return x * x;
	}

	template<typename T>
	T clamp_angle_2PI(T theta)
	{
		// Divide by 2 * PI and get the remainder
		const T remainder = fmod(theta, (T)2.0 * (T)PI_DBL);
		return (remainder > (T)PI_DBL) ? (remainder - (T)2.0 - (T)PI_DBL) : remainder;
	}

	template<typename T>
	T maxval(T a, T b)
	{
		if (a > b)
			return a;
		else
			return b;
	}

	template<typename T>
	T minval(T a, T b)
	{
		if (a < b)
			return a;
		else
			return b;
	}

	template<typename T>
	constexpr T lerp(const T& src, const T& dest, float lerpVal)
	{
		return src + (dest - src) * lerpVal;
	}

	template<typename T>
	constexpr T deg_to_rad(T deg)
	{
		return deg * PI / (T)180.0;
	}
}