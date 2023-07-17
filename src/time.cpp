#include "time.h"

float Time::deltaTime = 0.0f;
float Time::currentFrame = 0.0f;
float Time::previousFrame = 0.0f;

void Time::calculateDeltaTime()
{
	currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - previousFrame;
	previousFrame = currentFrame;
}