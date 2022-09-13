#pragma once

#include <chrono>
#include "Core.h"

namespace Granite
{
	class GRANITE_API FrameTimer
	{
	public:
		// Constructor
		FrameTimer();

		// Class methods
		void Reset();
		double CalculateFrameTime();

	private:
		std::chrono::steady_clock::time_point last;
	};
}