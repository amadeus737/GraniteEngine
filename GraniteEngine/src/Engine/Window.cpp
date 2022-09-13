#include "Window.h"
#include <assert.h>
#include <wchar.h>

namespace Granite
{
	Window::Window()
		:
		_instance(nullptr),
		_args(L""),
		_deviceContext(nullptr),
		_hWnd(nullptr),
		_isRunning(true),
		_screenWidth(800),
		_screenHeight(600)
	{ }

	Window::Window(HINSTANCE instance, std::wstring title, int width, int height, int left, int top, wchar_t* args)
		:
		_instance(instance),
		_args(args),
		_deviceContext(nullptr),
		_hWnd(nullptr),
		_isRunning(true),
		_screenWidth(width),
		_screenHeight(height)
	{
		// Setup screen dimensions
		_screenWidth = width;
		_screenHeight = height;

		// Register window class
		WNDCLASSEXW wc = { sizeof(WNDCLASSEX), CS_CLASSDC, HandleMessageSetup, 0, 0, _instance, nullptr, nullptr, nullptr, nullptr, _windowName, nullptr };

		// Assign icons and cursor
		wc.hIconSm = nullptr;
		wc.hIcon = nullptr;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

		// Register the window class
		RegisterClassExW(&wc);

		// Create the window
		RECT rect;
		rect.left = left;
		rect.right = _screenWidth + rect.left;
		rect.top = top;
		rect.bottom = _screenHeight + rect.top;
		AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
		_hWnd = CreateWindowW(_windowName, title.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, _instance, this);

		// Throw exception if something went terribly wrong
		if (!_hWnd)
		{
			MessageBoxW(0, L"CreateWindowEx failed", 0, 0);
		}

		// Get device context
		_deviceContext = GetDC(_hWnd);

		// Show and update window
		ShowWindow(_hWnd, SW_SHOWDEFAULT);
		UpdateWindow(_hWnd);
	}

	void Window::Reformat(WindowProperties properties)
	{
		// Setup screen dimensions
		_screenWidth = properties.width();
		_screenHeight = properties.height();

		// Reset window position
		SetWindowPos(_hWnd, nullptr, properties.left(), properties.top(), properties.width(), properties.height(), SWP_SHOWWINDOW);

		SetWindowTextA(_hWnd, (LPCSTR)properties.title().c_str());
	}

	Window::~Window()
	{
		// unregister window class
		UnregisterClassW(_windowName, _instance);
	}

	bool Window::IsActive() const
	{
		return GetActiveWindow() == _hWnd;
	}

	bool Window::IsMinimized() const
	{
		return IsIconic(_hWnd) != 0;
	}

	void Window::ShowMessageBox(const std::wstring& title, const std::wstring& message, UINT type) const
	{
		MessageBoxW(_hWnd, message.c_str(), title.c_str(), type);
	}

	bool Window::ProcessMessage()
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				return false;
			}
		}

		return true;
	}

	LRESULT WINAPI Window::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = reinterpret_cast<Window*>(pCreate->lpCreateParams);

			// sanity check
			assert(pWnd != nullptr);

			// set WinAPI-managed user data to store ptr to window class
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageHelper));

			// forward message to window class handler
			return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
		}

		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	LRESULT WINAPI Window::HandleMessageHelper(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// retrieve ptr to window class
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		// forward message to window class handler
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMessage(HWND window, UINT msg, WPARAM wp, LPARAM lp)
	{
		wchar_t out_msg[32];
		LRESULT result;
		switch (msg)
		{
		case WM_CLOSE:
			_isRunning = false;
			break;

		case WM_DESTROY:
			_isRunning = false;
			PostQuitMessage(0);
			break;

		case WM_KILLFOCUS:
			keyboard.ClearState();
			break;

		case WM_KEYDOWN:
			swprintf_s(out_msg, L"WM_KEYDOWN: 0x%x = %d\n", wp, wp);
			OutputDebugStringW(out_msg);
			if (!(lp & 0x40000000) || keyboard.AutorepeatIsEnabled()) // no thank you on the autorepeat
			{
				keyboard.OnKeyPressed(static_cast<unsigned char>(wp));
			}
			break;

		case WM_KEYUP:
			swprintf_s(out_msg, L"WM_KEYUP: 0x%x\n", wp);
			OutputDebugStringW(out_msg);
			keyboard.OnKeyReleased(static_cast<unsigned char>(wp));
			break;

		case WM_CHAR:
			swprintf_s(out_msg, L"WM_CHAR: %c\n", (wchar_t)wp);
			OutputDebugStringW(out_msg);
			keyboard.OnChar(static_cast<unsigned char>(wp));
			break;
		}

		return DefWindowProcW(window, msg, wp, lp);
	}

	void Window::RenderBuffer(FrameBuffer& buffer)
	{
		StretchDIBits(_deviceContext, 0, 0, _screenWidth, _screenHeight, 0, 0, _screenWidth, _screenHeight, buffer.val, buffer.info(), DIB_RGB_COLORS, SRCCOPY);
	}
}
