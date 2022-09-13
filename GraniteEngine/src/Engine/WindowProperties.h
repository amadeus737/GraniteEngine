#pragma once

#include "Core.h"
#include <string>

namespace Granite
{
	class GRANITE_API WindowProperties
	{
	public:
		WindowProperties()
			:
			_title(L"Granite Engine"),
			_width(800),
			_height(600),
			_left(10),
			_top(100)
		{ }

		WindowProperties(std::wstring wt, int w, int h, int l, int t)
			:
			_title(wt),
			_width(w),
			_height(h),
			_left(l),
			_top(t)
		{ }

		void Assign(std::wstring wt, int w, int h, int l, int t)
		{
			_title = wt;
			_width = w;
			_height = h;
			_left = l;
			_top = t;
		}

		std::wstring title() { return _title; }
		int width() { return _width; }
		int height() { return _height; }
		int left() { return _left; }
		int top() { return _top; }

	private:
		std::wstring _title;
		int _width;
		int _height;
		int _left;
		int _top;
	};
}