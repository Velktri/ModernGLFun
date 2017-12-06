#pragma once
#include <vector>
#include "openvr.h"

class Camera;
class VR_HMD;
class World;
class Manager;
class Timer;

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

	/* GETTERS */
	Timer* GetUniversalTimer();
	Manager* GetManager();
	CameraSet GetCamaras();

private:
	CameraSet UserCameras;
	Manager* MyManager;
	Timer* UniversalTimer;
	void InitCameras();
	void InitWorlds();
};

