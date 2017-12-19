#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "openvr.h"

class Camera;
class VR_HMD;
class World;
class Manager;
class Timer;
class Gizmo;
class Grid;
class Element;
class Asset;

struct CameraSet
{
	Camera* Perspective;
	Camera* Top;
	Camera* Front;
	Camera* Side;
	VR_HMD* VRCamera;

	CameraSet()
	{
		Perspective = NULL;
		Top = NULL;
		Front = NULL;
		Side = NULL;
		VRCamera = NULL;
	}
};


/*
 * This class acts as a hub that keeps track of the active camera 
 * and world states.  The Universe will be able to send out relevant
 * data to other classes about the cameras and worlds.
 */
class Universe
{
public:
	Universe(Manager* InManager);
	~Universe();

	std::vector<World*> Worlds;
	World* ActiveWorld;

	void InitVR();
	Asset* CastRaytrace(Camera* InCamera, glm::vec2 DeviceCoords, glm::vec2 SceneSize);
	void ConvertScreenToWorldSpace(glm::vec2 InScreenSelection, glm::vec2 InSceneSpace, glm::mat4 InvertedViewProjection, glm::vec3& WorldPosition, glm::vec3& WorldDirection);
	void ClearLines();


	/* GETTERS */
	Timer* GetUniversalTimer();
	Manager* GetManager();
	CameraSet GetCamaras();
	std::vector<Element*> GetSystemElements();
	void RenderVR();

	void GetSelectionResults(glm::vec2 Coords, glm::vec2 SceneSize);
private:
	CameraSet UserCameras;
	Manager* MyManager;
	Timer* UniversalTimer;
	void InitCameras();
	void InitWorlds();

	Grid* GridFloor;

	int GRIDRADIUS_X = 20;
	int GRIDRADIUS_Y = 20;
	float GRIDSPACING = 1.0;

	std::vector<Element*> SystemElements;
};

