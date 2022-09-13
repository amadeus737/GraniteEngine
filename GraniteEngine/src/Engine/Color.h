#pragma once

#include <stdint.h>
#include "Core.h"

namespace Granite
{
	class GRANITE_API Color
	{
	public:
		// Constructors
		constexpr Color() : _colorCode() {}
		constexpr Color(const Color& c) : _colorCode(c._colorCode) {}
		constexpr Color(unsigned int code) : _colorCode(code) {}
		constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b)
			: _colorCode((x << 24u) | (r << 16u) | (g << 8u) | b) {}
		constexpr Color(unsigned char r, unsigned char g, unsigned char b)
			: _colorCode((r << 16u) | (g << 8u) | b) {}
		constexpr Color(Color c, unsigned char x)
			: Color((x << 24u) | c.GetColorCode()) {}

		// Operators
		Color& operator =(Color c);
		Color operator /(const int divisor);
		const Color operator*(float v);

		// Test equality function
		bool IsEqual(const Color& c);

		// Getters
		unsigned int GetColorCode();
		constexpr unsigned char x() const;
		constexpr unsigned char a() const;
		constexpr unsigned char r() const;
		constexpr unsigned char g() const;
		constexpr unsigned char b() const;

		// Setters
		void set_x(unsigned char x);
		void set_a(unsigned char a);
		void set_r(unsigned char r);
		void set_g(unsigned char g);
		void set_b(unsigned char b);

		// Pre-defined
		static constexpr Color White() { return Color(255u, 255u, 255u); }
		static constexpr Color Black() { return Color(0u, 0u, 0u); }
		static constexpr Color MediumGray() { return Color(0x60u, 0x60u, 0x60u); }
		static constexpr Color Gray() { return Color(0x80u, 0x80u, 0x80u); }
		static constexpr Color LightGray() { return Color(0xD3u, 0xD3u, 0xD3u); }
		static constexpr Color Red() { return Color(255u, 0u, 0u); }
		static constexpr Color Green() { return  Color(0u, 255u, 0u); }
		static constexpr Color Blue() { return  Color(0u, 0u, 255u); }
		static constexpr Color LightBlue() { return Color(0u, 128u, 255u); }
		static constexpr Color Yellow() { return  Color(255u, 255u, 0u); }
		static constexpr Color Orange() { return Color(255u, 165u, 0u); }
		static constexpr Color Cyan() { return Color(0u, 255u, 255u); }
		static constexpr Color Magenta() { return Color(255u, 0u, 255u); }
		static constexpr Color Purple() { return Color(180u, 0u, 180u); }
		static constexpr Color Granite() { return Color(0x33u, 0x33u, 0x33u); }

	private:
		unsigned int _colorCode;
	};
}