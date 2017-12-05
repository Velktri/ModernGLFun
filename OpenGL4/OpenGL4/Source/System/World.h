#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Camera;
class VR_HMD;
class Grid;
class Manager;
class Light;
class Asset;
class Element;
class Timer;
class Gizmo;

class World {
public:
	World(Manager* InManager);
	~World();

	//void CreateCurve();
	Asset* CastRaytrace(glm::vec2 DeviceCoords, glm::vec2 SceneSize);

	void RenderWorld(Camera* InCamera, glm::vec2 FrameSize);
	void RenderColorWorld(Camera* InCamera, glm::vec2 FrameSize);
	void ClearLines();

	std::vector<Light*> GetLights();
	Manager* GetManager();

private:
	Grid* GridFloor;
	Gizmo* SelectionGizmo;
	Manager* MyManager;

	int GRIDRADIUS_X = 10;
	int GRIDRADIUS_Y = 10;
	float GRIDSPACING = 1.0;

	std::vector<Element*> SystemElements;
};

