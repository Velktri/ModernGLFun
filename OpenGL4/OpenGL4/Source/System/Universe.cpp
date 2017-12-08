#include "Universe.h"
#include "ModelData\Grid.h"
#include "ModelData\Gizmo.h"
#include "ModelData\Line.h"
#include "Manager.h"
#include "World.h"
#include "Timer.h"
#include "Camera.h"
#include "VR_HMD.h"

Universe::Universe(Manager* InManager)
{
	MyManager = InManager;
	UniversalTimer = new Timer();

	GridFloor = new Grid(GRIDRADIUS_X, GRIDRADIUS_Y, GRIDSPACING);
	SelectionGizmo = new Gizmo();
	SystemElements.push_back(GridFloor);
	SystemElements.push_back(SelectionGizmo);

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

	for (Element* e : SystemElements)
	{
		if (e) { e->~Element(); }
	}
}

void Universe::InitCameras()
{
	UserCameras = CameraSet();
	UserCameras.Perspective = new Camera(this, glm::vec3(0.0f, 10.0f, 20.0f));
	// @TODO: Add Quad view
}

void Universe::InitWorlds()
{
	Worlds.push_back(new World(this, MyManager));
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

	if (!UserCameras.VRCamera->InitCompositor())
	{
		printf("%s - Failed to initialize VR Compositor!\n", __FUNCTION__);
	}
}

void Universe::RenderVR()
{
	if (UserCameras.VRCamera)
	{
		UserCameras.VRCamera->Render();
	}
}

/* @TODO: Ray Trace is aiming slightly lower than it should. 
The borders between regions are throwing the positions off by a couple of pixels

@TODO: split into two functions. one to change coords from device to world space and other to ray cast.

*/
Asset* Universe::CastRaytrace(Camera* InCamera, glm::vec2 DeviceCoords, glm::vec2 SceneSize)
{
	printf("%f, %f  ", DeviceCoords.x, DeviceCoords.y);

	float x = (2.0f * DeviceCoords.x) / SceneSize.x - 1.0f;
	float y = 1.0f - (2.0f * DeviceCoords.y) / SceneSize.y;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);

	glm::vec4 ray_eye = glm::inverse(InCamera->GetProjection()) * ray_clip;
	ray_eye.z = -1.0f;
	ray_eye.w = 0.0f;

	glm::vec4 a = glm::vec4(glm::inverse(InCamera->GetViewMatrix()) * ray_eye);
	glm::vec3 ray_wor = glm::vec3(a.x, a.y, a.z);
	ray_wor = glm::normalize(ray_wor);

	glm::vec3 lStart = InCamera->GetPosition();
	glm::vec3 lEnd = lStart + (ray_wor * 100.0f);

	SystemElements.push_back(new Line(lStart, lEnd));

	return NULL;
}

void Universe::ClearLines()
{
	for each (Element* e in SystemElements)
	{
		if (e->GetType() == ShaderType::LINE)
		{
			e->~Element();
		}
	}
}


Timer* Universe::GetUniversalTimer() { return UniversalTimer; }
Manager* Universe::GetManager() { return MyManager; }
CameraSet Universe::GetCamaras() { return UserCameras; }
std::vector<Element*> Universe::GetSystemElements() { return SystemElements; }