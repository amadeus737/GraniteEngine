#pragma once

#ifdef GRANITE_PLATFORM_WINDOWS

#include <GraniteEngine.h>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include "FrameTimer.h"

// External
extern Granite::Game* Granite::LaunchGame();
extern Granite::WindowProperties _windowProperties;
extern Granite::FrameBuffer _frameBuffer;
extern Granite::Window _window;

// Global variables
Granite::Game* _game;
Granite::FrameTimer _frameTimer = Granite::FrameTimer();

// FPS stuff
int _fpsFrameCount = 0.0f;
float _fpsTimeCount = 0.0f;
void PrintWelcomeMessage();
void CalculateFrameTime();

// Windows entry point...not actually used as entry point to project because the project is configured as
// a console project. Instead, the main() function below is the entry point, which calls this entry point.
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR cmdLine, int cmdShow)
{
	PrintWelcomeMessage();

	// First, get a pointer to the game and run the start function
	_game = Granite::LaunchGame();
	_game->Start();

	// Main loop
	while (_window.IsRunning())
	{
		_frameBuffer.BeginFrame(false);
		_window.ProcessMessage();
		CalculateFrameTime();
		_game->Input();
		_game->Update();
		_game->Render();
		_window.RenderBuffer(_frameBuffer);
		_frameBuffer.EndFrame();
	}

	return 0;
}

void PrintWelcomeMessage()
{
	printf("==============================================================================\n");
	printf("* Welcome to Granite Engine - v0.0a!                                         *\n");
	printf("* Created By: John Chapman                                                   *\n");
	printf("------------------------------------------------------------------------------\n");
	printf("* This software is free to modify, redistribute, and recompile commercially. *\n");
	printf("* The creator of this software does not provide any warranty and is free of  *\n");
	printf("* liability associated with this software.                                   *\n");
	printf("*                                                                            *\n");
	printf("* This preamble is only stated as a courtesy. Please see the full terms of   *\n");
	printf("* of the license which this software:                                        *\n");
	printf("*    - Apache License 2.0 (http://www.apache.org/licenses/)                  *\n");
	printf("*                                                                            *\n");
	printf("* For more information on this project, check out the Github repository:     *\n");
	printf("*   https://github.com/amadeus737/GraniteEngine                              *\n");
	printf("==============================================================================\n");
	printf("\n");
}

void CalculateFrameTime()
{
	_deltaTime = _frameTimer.CalculateFrameTime();
	_time += _deltaTime;

	if (_fpsFrameCount < _framesBeforeFPSupdate)
	{
		_fpsTimeCount += _deltaTime;
		_fpsFrameCount++;
	}
	else
	{
		_FPS = (float)_fpsFrameCount / _fpsTimeCount;

		_fpsFrameCount = 0;
		_fpsTimeCount = 0;

		if (_printFPS)
		{
			//	system("CLS");
			printf("\rFPS = %f", _FPS);
		}
	}
}

// Console entry point
int main()
{
	return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLineW(), SW_SHOWNORMAL);
}

#endif