#pragma once

#include "Core.h"
#include "Color.h"
#include "Window.h"
#include "WindowProperties.h"
#include <Windows.h>
#include <string>
#include <assert.h>
#include <memory>

namespace Granite
{
	class GRANITE_API FrameBuffer
	{
	public:
		// Constructors
		FrameBuffer();
		FrameBuffer(unsigned int width, unsigned int height, unsigned int bytesPerPixel);
		FrameBuffer(unsigned int width, unsigned int height);
		FrameBuffer(WindowProperties properties, unsigned int bytesPerPixel);
		FrameBuffer(FrameBuffer&& source);

		// Move and coplayer.y() constructors (deleted)
		FrameBuffer(FrameBuffer&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;

		// Destructor
		~FrameBuffer();

		void Resize(int width, int height);

		// Getters
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		unsigned int GetBytesPerPixel() const;
		const BITMAPINFO* info();

		// Setters
		void SetInfo(int ScreenWidth, int ScreenHeight, int planes, int bitcount, DWORD compression);

		// Frame functions
		void BeginFrame(bool autoClear = true);
		void EndFrame();

		// Draw functions
		void Clear(Color color);
		uint32_t* GetPixel(unsigned int x, unsigned int y) const;
		void DrawPixel(unsigned int x, unsigned int y, Color c, bool clear = false);
		void DrawPixelAlpha(unsigned int x, unsigned int y, Color c, bool clear = false);
		void DrawRect(int x, int y, int width, int height, Color c, bool clear = false);
		void DrawLine(int x1, int y1, int x2, int y2, int thickness, Color c, bool clear = false);
		void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int thickness, Color c, bool clear = false);

	public:
		void* val;

	private:
		unsigned int _width;
		unsigned int _height;
		unsigned int _bytesPerPixel;
		BITMAPINFO _info;
	};
}