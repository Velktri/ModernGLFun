#pragma once
#include <chrono>
#include <GL/glew.h>

class Timer {
public:
	Timer();
	~Timer();

	void Start();
	void Stop();
	void Update();

	GLfloat GetDeltaTime();
	GLfloat GetTime();

private:
	std::chrono::time_point<std::chrono::steady_clock> TimeStart;
	std::chrono::time_point<std::chrono::steady_clock> TimeNow;
	bool bIsClockRunning;

	float DeltaTime;
	float LastFrame;
};

