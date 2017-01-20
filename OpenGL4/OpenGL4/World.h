#pragma once
#include <chrono>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include"Models\Grid.h"
#include "Managers\ShaderManager.h"
#include "Managers\AssetManager.h"
#include "Managers\TextureManager.h"
#include "Managers\LightManager.h"

class World {
public:
	World(GLuint width, GLuint height);
	~World();

	Camera* GetCamera();
	std::vector<Light*> GetLights();

	void RenderWorld();
	void StartClock();
	void UpdateClock();

	GLfloat GetDeltaTime();
	GLfloat GetTime();

private:
	Camera* WorldCamera;
	Grid* Scene;
	ShaderManager* ShaderManagement;
	AssetManager* AssetManagement;
	TextureManager* TextureManagement;
	LightManager* LightManagement;
	std::chrono::time_point<std::chrono::steady_clock> TimeStart;
	std::chrono::time_point<std::chrono::steady_clock> TimeNow;
	bool bIsClockRunning;

	int GRIDRADIUS_X = 10;
	int GRIDRADIUS_Y = 10;
	float GRIDSPACING = 1.0;

	float DeltaTime;
	float LastFrame;
};

