#include "Timer.h"

Timer::Timer() {
	bIsClockRunning = false;
	DeltaTime = 0.0f;
	LastFrame = 0.0f;
}

Timer::~Timer() {
}

void Timer::Start() {
	if (!bIsClockRunning) {
		TimeStart = std::chrono::high_resolution_clock::now();
		bIsClockRunning = true;
	}
}

void Timer::Stop() {
	if (bIsClockRunning) {
		bIsClockRunning = false;
	}
	DeltaTime = 0;
	LastFrame = 0;
}

void Timer::Update() {
	if (bIsClockRunning) {
		TimeNow = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(TimeNow - TimeStart).count();
		DeltaTime = time - LastFrame;
		LastFrame = time;
	}
}

GLfloat Timer::GetDeltaTime() {
	return DeltaTime;
}

GLfloat Timer::GetTime() {
	return LastFrame;
}