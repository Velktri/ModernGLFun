#pragma once
#include <chrono>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Camera;
class Grid;
class Manager;
class Line;
class Light;
class Asset;
class Curve;

class World {
public:
	World(GLuint width, GLuint height);
	~World();

	Camera* GetCamera();
	std::vector<Light*> GetLights();

	void RenderWorld();
	void RenderColorWorld();
	void RenderScreen();
	void StartClock();
	void StopClock();
	void UpdateClock();
	void SetManager(Manager* m);

	GLfloat GetDeltaTime();
	GLfloat GetTime();
	void CreateCurve();
	Curve* GetCurve();

	Asset* CastRaytrace(glm::vec2 DeviceCoords);

private:
	Camera* WorldCamera;
	Grid* Scene;
	Manager* MyManager;

	std::chrono::time_point<std::chrono::steady_clock> TimeStart;
	std::chrono::time_point<std::chrono::steady_clock> TimeNow;
	bool bIsClockRunning;

	int GRIDRADIUS_X = 10;
	int GRIDRADIUS_Y = 10;
	float GRIDSPACING = 1.0;

	float DeltaTime;
	float LastFrame;

	GLuint SceneWidth;
	GLuint SceneHeight;

	Line* line;
	Curve* curve;
};

