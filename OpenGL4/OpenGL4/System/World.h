#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Camera;
class Grid;
class Manager;
class Light;
class Asset;
class Element;
class Timer;
class Gizmo;

class World {
public:
	World(GLuint width, GLuint height);
	~World();

	Camera* GetCamera();
	std::vector<Light*> GetLights();
	void SetManager(Manager* m);

	void RenderWorld();
	void RenderColorWorld();
	void RenderSystemEntities();
	void RenderUserEntities();

	Timer* GetTimer();

	void CreateCurve();
	Asset* CastRaytrace(glm::vec2 DeviceCoords);


private:
	Camera* WorldCamera;
	Grid* Scene;
	Gizmo* SelectionGizmo;
	Manager* MyManager;

	int GRIDRADIUS_X = 10;
	int GRIDRADIUS_Y = 10;
	float GRIDSPACING = 1.0;

	GLuint SceneWidth;
	GLuint SceneHeight;

	Timer* WorldClock;

	std::vector<Element*> SystemElements;
};

