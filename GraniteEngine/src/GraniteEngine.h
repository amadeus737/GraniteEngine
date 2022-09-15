#pragma once

#include "Engine/Game.h"
#include "Engine/Color.h"
#include "Engine/Keyboard.h"
#include "Engine/Window.h"
#include "Engine/WindowProperties.h"
#include "Engine/GraniteMath.h"
#include "Engine/FrameBuffer.h"
#include "Engine/Triangle.h"
#include "Engine/Vector2.h"
#include "Engine/Mesh.h"

#define GraniteGame public Granite::Game
#define ConfigureWindow(s,w,h,l,t) Granite::WindowProperties properties = Granite::WindowProperties(s, w, h, l, t); _window.Reformat(properties); _frameBuffer.Resize(w,h);
#define ShowFPS(x) _printFPS = x
#define Run(x) return new x()
#define GraniteFrameBuffer _frameBuffer
#define GraniteWindow _window
#define GraniteKeyboard _window.keyboard
#define GraniteTime _time
#define GraniteDeltaTime _deltaTime
#define GraniteFPS _FPS
#define NumFramesForFPScalc(x) _framesBeforeFPSupdate = x

#ifndef GRANITE_INIT
#define GRANITE_INIT

inline bool _printFPS = false;
inline int _framesBeforeFPSupdate = 200;
inline double _FPS = 0.0f;
inline double _deltaTime = 0.0f;
inline double _time = 0.0f;

inline Granite::FrameBuffer _frameBuffer = Granite::FrameBuffer(800, 600, 4);
inline Granite::Window _window = Granite::Window(GetModuleHandle(NULL), L"Granite Engine", 800, 600, 100, 100, GetCommandLineW());

#endif