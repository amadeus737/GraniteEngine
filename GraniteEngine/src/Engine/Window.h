#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include "Core.h"
#include "Color.h"
#include "Keyboard.h"
#include "FrameBuffer.h"
#include "WindowProperties.h"

namespace Granite
{
	class GRANITE_API Window
	{
	public:
		Window();
		Window(HINSTANCE instance, std::wstring title, int width, int height, int left, int top, wchar_t* args);
		~Window();

		bool IsActive() const;
		bool IsMinimized() const;
		void ShowMessageBox(const std::wstring& title, const std::wstring& message, UINT type = MB_OK) const;
		void Kill() { _isRunning = false;  PostQuitMessage(0); }

		void RenderBuffer(class FrameBuffer& buffer);

		bool ProcessMessage();
		const std::wstring& GetArgs() const { return _args; }

		bool IsRunning() { return _isRunning; }

		void Reformat(WindowProperties properties);

		int ScreenWidth() { return _screenWidth; }
		int ScreenHeight() { return _screenHeight; }

		Keyboard keyboard;

	private:
		static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI HandleMessageHelper(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM w, LPARAM l);

	private:
		static constexpr const wchar_t* _windowName = L"Granite Engine Render Window";
		HINSTANCE _instance = nullptr;
		std::wstring _args;
		HDC _deviceContext;
		HWND _hWnd;
		bool _isRunning = true;
		int _screenWidth;
		int _screenHeight;
	};
}