#include "FrameBuffer.h"

namespace Granite
{
	FrameBuffer::FrameBuffer()
		:
		val(nullptr),
		_width(800),
		_height(600),
		_bytesPerPixel(4),
		_info(BITMAPINFO())
	{
		SetInfo(800, 600, 1, 32, BI_RGB);
	}

	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, unsigned int bytesPerPixel)
		:
		val(VirtualAlloc(0, width* height* bytesPerPixel, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)),
		_width(width),
		_height(height),
		_bytesPerPixel(bytesPerPixel),
		_info(BITMAPINFO())
	{
		SetInfo(width, height, 1, bytesPerPixel * 8, BI_RGB);
	}

	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
		:
		FrameBuffer(width, height, 4)
	{
		SetInfo(width, height, 1, 32, BI_RGB);
	}

	FrameBuffer::FrameBuffer(WindowProperties properties, unsigned int bytesPerPixel)
		:
		val(VirtualAlloc(0, properties.width()* properties.height()* bytesPerPixel, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)),
		_width(properties.width()),
		_height(properties.height()),
		_bytesPerPixel(bytesPerPixel),
		_info(BITMAPINFO())
	{
		SetInfo(properties.width(), properties.height(), 1, bytesPerPixel * 8, BI_RGB);
	}

	FrameBuffer::FrameBuffer(FrameBuffer&& source)
		:
		val(std::move(source.val)),
		_width(source._width),
		_height(source._height),
		_bytesPerPixel(source._bytesPerPixel)
	{}

	FrameBuffer::~FrameBuffer()
	{}

	void FrameBuffer::BeginFrame(bool autoClear)
	{
		if (autoClear)
			Clear(Color::Red());
	}

	void FrameBuffer::EndFrame()
	{
	}

	void FrameBuffer::Clear(Color color)
	{
		uint32_t* pixel = (uint32_t*)val;
		for (int index = 0; index < _width * _height; ++index)
		{
			*pixel++ = color.GetColorCode();
		}
	}

	void FrameBuffer::Resize(int width, int height)
	{
		_width = width;
		_height = height;

		val = nullptr;
		val = VirtualAlloc(0, width * height * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		SetInfo(width, height, 1, 32, BI_RGB);
	}

	uint32_t* FrameBuffer::GetPixel(unsigned int x, unsigned int y) const
	{
		assert(x >= 0);
		assert(y >= 0);
		assert(x < _width);
		assert(y < _height);

		uint32_t* pixel = (uint32_t*)val;
		pixel += y * _width + x;
		return pixel;
	}

	void FrameBuffer::DrawPixel(unsigned int x, unsigned int y, Color c, bool clear)
	{
		if (x < 0) x = 0;
		if (x > _width - 1) x = _width - 1;
		if (y < 0) y = 0;
		if (y > _height - 1) y = _height - 1;

		if (clear) Clear(Color::Red());

		uint32_t* pixel = (uint32_t*)val;
		pixel += y * _width + x;
		*pixel = c.GetColorCode();
	}

	void FrameBuffer::DrawPixelAlpha(unsigned int x, unsigned int y, Color c, bool clear)
	{
		if (x < 0) x = 0;
		if (x > _width - 1) x = _width - 1;
		if (y < 0) y = 0;
		if (y > _height - 1) y = _height - 1;

		if (clear) Clear(Color::Red());

		// load source pixel
		uint32_t* pixel = (uint32_t*)val;
		pixel += y * _width + x;
		const Color d = *pixel;

		// blend channels
		const unsigned char rsltRed = (c.r() * c.a() + d.r() * (255u - c.a())) / 256u;
		const unsigned char rsltGreen = (c.g() * c.a() + d.g() * (255u - c.a())) / 256u;
		const unsigned char rsltBlue = (c.b() * c.a() + d.b() * (255u - c.a())) / 256u;

		// pack channels back into pixel and fire pixel onto FrameBuffer
		DrawPixel(x, y, { rsltRed,rsltGreen,rsltBlue }, false);
	}

	void FrameBuffer::DrawRect(int x, int y, int width, int height, Color c, bool clear)
	{
		if (x < 0) x = 0;
		if (x + width > _width - 1) x = _width - 1 - width;
		if (y < 0) y = 0;
		if (y + height > _height - 1) y = _height - 1 - height;

		if (clear) Clear(Color::Red());

		for (int i = x; i < x + width; i++)
		{
			for (int j = y; j < y + height; j++)
			{
				uint32_t* pixel = (uint32_t*)val;
				pixel += j * _width + i;
				*pixel = c.GetColorCode();
			}
		}
	}

	void FrameBuffer::DrawLine(int x1, int y1, int x2, int y2, int thickness, Color c, bool clear)
	{
		if (clear) Clear(Color::Red());

		int halfPixels = 0;
		int drawMode = -1;
		if (thickness > 1)
		{
			halfPixels = thickness / 2;
			drawMode = 0;
		}
		if (thickness > 1 && thickness % 2 == 1)
		{
			halfPixels = (thickness - 1) / 2 + 1;
			drawMode = 0;
		}

		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1;
		dy = y2 - y1;
		dx1 = abs(dx);
		dy1 = abs(dy);
		px = 2 * dy1 - dx1;
		py = 2 * dx1 - dy1;

		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = x1; y = y1; xe = x2;
			}
			else
			{
				x = x2; y = y2; xe = x1;
			}

			if (drawMode == -1)
			{
				DrawPixel(x, y, c);
			}
			else
			{
				DrawRect(x - halfPixels, y - halfPixels, 2 * halfPixels, 2 * halfPixels, c);
			}

			for (i = 0; x < xe; i++)
			{
				x = x + 1;
				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				if (drawMode == -1)
				{
					DrawPixel(x, y, c);
				}
				else
				{
					DrawRect(x - halfPixels, y - halfPixels, 2 * halfPixels, 2 * halfPixels, c);
				}
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = x1; y = y1; ye = y2;
			}
			else
			{
				x = x2; y = y2; ye = y1;
			}

			if (drawMode == -1)
			{
				DrawPixel(x, y, c);
			}
			else
			{
				DrawRect(x - halfPixels, y - halfPixels, 2 * halfPixels, 2 * halfPixels, c);
			}

			for (i = 0; y < ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				if (drawMode == -1)
				{
					DrawPixel(x, y, c);
				}
				else
				{
					DrawRect(x - halfPixels, y - halfPixels, 2 * halfPixels, 2 * halfPixels, c);
				}
			}
		}
	}

	void FrameBuffer::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int thickness, Color c, bool clear)
	{
		if (clear) Clear(Color::Red());

		DrawLine(x0, y0, x1, y1, thickness, c, false);
		DrawLine(x1, y1, x2, y2, thickness, c, false);
		DrawLine(x2, y2, x0, y0, thickness, c, false);
	}

	unsigned int FrameBuffer::GetWidth() const
	{
		return _width;
	}

	unsigned int FrameBuffer::GetHeight() const
	{
		return _height;
	}
	unsigned int FrameBuffer::GetBytesPerPixel() const
	{
		return _bytesPerPixel;
	}

	void FrameBuffer::SetInfo(int ScreenWidth, int ScreenHeight, int planes, int bitcount, DWORD compression)
	{
		_info.bmiHeader.biSize = sizeof(_info.bmiHeader);
		_info.bmiHeader.biWidth = ScreenWidth;
		_info.bmiHeader.biHeight = -ScreenHeight;
		_info.bmiHeader.biPlanes = planes;
		_info.bmiHeader.biBitCount = bitcount;
		_info.bmiHeader.biCompression = compression;
	}

	const BITMAPINFO* FrameBuffer::info()
	{
		return &_info;
	}
}