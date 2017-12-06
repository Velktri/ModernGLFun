#include "Universe.h"
#include "Manager.h"
#include "World.h"
#include "Timer.h"
#include "Camera.h"
#include "VR_HMD.h"

Universe::Universe(Manager* InManager)
{
	MyManager = InManager;
	UniversalTimer = new Timer();

	InitCameras();
	InitWorlds();
}


Universe::~Universe()
{
	if (UserCameras.Perspective) { UserCameras.Perspective->~Camera(); }
	if (UserCameras.Top) { UserCameras.Top->~Camera(); }
	if (UserCameras.Front) { UserCameras.Front->~Camera(); }
	if (UserCameras.Side) { UserCameras.Side->~Camera(); }
	if (UserCameras.VRCamera) { UserCameras.VRCamera->~VR_HMD(); }

	for (World* w : Worlds)
	{
		if (w) { w->~World(); }
	}

	if (UniversalTimer) { UniversalTimer->~Timer(); }
}

void Universe::InitCameras()
{
	UserCameras = CameraSet();
	UserCameras.Perspective = new Camera(this, glm::vec3(0.0f, 10.0f, 20.0f));
	// @TODO: Add Quad view
}

void Universe::InitWorlds()
{
	Worlds.push_back(new World(MyManager));
	ActiveWorld = Worlds[0];
}


void Universe::InitVR()
{
	vr::EVRInitError VRError = vr::VRInitError_None;
	class vr::IVRSystem* HMD = vr::VR_Init(&VRError, vr::VRApplication_Scene);

	if (VRError != vr::VRInitError_None)
	{
		HMD = NULL;
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(VRError));
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "VR_Init Failed", buf, NULL);
	}

	UserCameras.VRCamera = new VR_HMD(this, HMD);
}



Timer* Universe::GetUniversalTimer() { return UniversalTimer; }
Manager* Universe::GetManager() { return MyManager; }
CameraSet Universe::GetCamaras() { return UserCameras; }