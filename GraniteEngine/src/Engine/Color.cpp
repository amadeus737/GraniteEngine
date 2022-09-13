#pragma once

#include "Color.h"

namespace Granite
{
	Color& Color::operator =(Color c)
	{
		_colorCode = c.GetColorCode();
		return *this;
	}

	Color Color::operator /(const int divisor)
	{
		set_r(this->r() / divisor);
		set_g(this->g() / divisor);
		set_b(this->b() / divisor);
		set_a(this->a() / divisor);

		return Color(*this);
	}

	const Color Color::operator*(float v)
	{
		set_r((float)r() * v);
		set_g((float)g() * v);
		set_b((float)b() * v);
		set_a((float)a() * v);

		return Color(*this);
	}

	bool Color::IsEqual(const Color& c)
	{
		return _colorCode == c._colorCode;
	}

	unsigned int Color::GetColorCode() { return _colorCode; }

	constexpr unsigned char Color::x() const { return _colorCode >> 24u; }
	constexpr unsigned char Color::a() const { return x(); }
	constexpr unsigned char Color::r() const { return (_colorCode >> 16u) & 0xFFu; }
	constexpr unsigned char Color::g() const { return (_colorCode >> 8u) & 0xFFu; }
	constexpr unsigned char Color::b() const { return _colorCode & 0xFFu; }

	void Color::set_x(unsigned char x) { _colorCode = (_colorCode & 0xFFFFFFu) | (x << 24u); }
	void Color::set_a(unsigned char a) { set_x(a); }
	void Color::set_r(unsigned char r) { _colorCode = (_colorCode & 0xFF00FFFFu) | (r << 16u); }
	void Color::set_g(unsigned char g) { _colorCode = (_colorCode & 0xFFFF00FFu) | (g << 8u); }
	void Color::set_b(unsigned char b) { _colorCode = (_colorCode & 0xFFFFFF00u) | b; }
}