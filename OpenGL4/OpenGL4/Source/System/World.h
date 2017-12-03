#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "openvr.h"

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

	void RenderWorld(glm::vec2 FrameSize);
	void RenderColorWorld(glm::vec2 FrameSize);
	void ClearLines();

	Timer* GetTimer();
	Camera* GetCamera();
	std::vector<Light*> GetLights();
	Manager* GetManager();
	void InitVR(vr::IVRSystem* InHMD);

private:
	Camera* UserCamera;
	VR_HMD* VRCamera;
	Grid* GridFloor;
	Gizmo* SelectionGizmo;
	Manager* MyManager;
	Timer* WorldClock;

	int GRIDRADIUS_X = 10;
	int GRIDRADIUS_Y = 10;
	float GRIDSPACING = 1.0;

	void RenderSystemEntities();
	void RenderUserEntities();

	bool bInitVR;

	std::vector<Element*> SystemElements;
};

