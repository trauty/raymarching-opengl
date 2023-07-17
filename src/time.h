#pragma once

#pragma once

#include "glfw3.h"

namespace Time
{
	extern float deltaTime;
	extern float currentFrame;
	extern float previousFrame;

	void calculateDeltaTime();
}