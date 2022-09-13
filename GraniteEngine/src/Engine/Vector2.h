#pragma once

#include "Core.h"
#include <math.h>

namespace Granite
{
	class GRANITE_API Vector2d
	{
	public:
		Vector2d() = default;
		Vector2d(double x, double y)
			:
			x(x),
			y(y)
		{}

		Vector2d& operator=(const Vector2d& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		Vector2d operator+(const Vector2d& rhs) const
		{
			return Vector2d(*this) += rhs;
		}

		Vector2d& operator+=(const Vector2d& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2d& operator-=(const Vector2d& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2d operator*(const double& rhs) const
		{
			return Vector2d(*this) *= rhs;
		}

		const double sqrMagnitude()
		{
			return (x * x) + (y * y);
		}

		const double magnitude()
		{
			return sqrt(sqrMagnitude());
		}

		Vector2d& normalize()
		{
			const double length = magnitude();
			x /= length;
			y /= length;
			return *this;
		}

		Vector2d normalized() const
		{
			Vector2d norm = *this;
			norm.normalize();
			return norm;
		}

		Vector2d& operator*=(const double& rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}

		double x;
		double y;
	};

	class GRANITE_API Vector2f
	{
	public:
		Vector2f() = default;
		Vector2f(float x, float y)
			:
			x(x),
			y(y)
		{}

		Vector2f& operator=(const Vector2f& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		Vector2f& operator+=(const Vector2f& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2f& operator-=(const Vector2f& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2f operator*(const float& rhs) const
		{
			return Vector2f(*this) *= rhs;
		}

		Vector2f operator*(Vector2f& rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			return Vector2f(*this);
		}

		const float sqrMagnitude()
		{
			return (x * x) + (y * y);
		}

		const float magnitude()
		{
			return sqrtf(sqrMagnitude());
		}

		Vector2f& operator*=(const float& rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}

		float x;
		float y;
	};

	class GRANITE_API Vector2i
	{
	public:
		Vector2i() = default;
		Vector2i(int x, int y)
			:
			x(x),
			y(y)
		{}

		Vector2i& operator=(const Vector2i& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		Vector2i& operator+=(const Vector2i& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2i& operator-=(const Vector2i& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2i operator*(const int& rhs) const
		{
			return Vector2i(*this) *= rhs;
		}

		Vector2i& operator*=(const int& rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}

		int x;
		int y;
	};
}