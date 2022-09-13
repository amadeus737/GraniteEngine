#pragma once

#include "Core.h"

namespace Granite
{
	class GRANITE_API Game
	{
	public:
		Game() { }

		virtual void Start() { }
		virtual void Input() { }
		virtual void Update() { }
		virtual void Render() { }
	};

	Game* LaunchGame();
}