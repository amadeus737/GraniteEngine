#include "FrameTimer.h"

namespace Granite
{
	FrameTimer::FrameTimer()
	{
		last = std::chrono::steady_clock::now();
	}

	void FrameTimer::Reset()
	{
		last = std::chrono::steady_clock::now();
	}

	double FrameTimer::CalculateFrameTime()
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();

		const std::chrono::duration<double> frameTime = last - old;

		return frameTime.count();
	}
}