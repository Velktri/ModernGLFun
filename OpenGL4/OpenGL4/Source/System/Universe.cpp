#include "Universe.h"
#include "ModelData\Grid.h"
#include "Models\Gizmo.h"
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
	SelectionGizmo = new Gizmo(MyManager->GetAssetListSize(), MyManager);
	SystemElements.push_back(GridFloor);
	//SystemElements.push_back(SelectionGizmo);

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

/* 
@TODO: Ray Trace is aiming slightly lower than it should. 
The borders between regions are throwing the positions off by a couple of pixels
*/
Asset* Universe::CastRaytrace(Camera* InCamera, glm::vec2 DeviceCoords, glm::vec2 SceneSize)
{
	glm::vec3 WorldPosition;
	glm::vec3 WorldDirection;
	ConvertScreenToWorldSpace(DeviceCoords, SceneSize, glm::inverse(InCamera->GetViewProjection()), WorldPosition, WorldDirection);

	glm::vec3 lEnd = WorldPosition + (WorldDirection * -100.0f);

	SystemElements.push_back(new Line(WorldPosition, lEnd));

	return NULL;
}

void Universe::ConvertScreenToWorldSpace(glm::vec2 InScreenSelection, glm::vec2 InSceneSpace, glm::mat4 InvertedViewProjection, glm::vec3& WorldPosition, glm::vec3& WorldDirection)
{
	// Get pixel coordinates into projection space
	float NormalizedX = ((2.0f * InScreenSelection.x) / InSceneSpace.x) - 1.0f;
	float NormalizedY = 1.0f - ((2.0f * InScreenSelection.y) / InSceneSpace.y);

	const glm::vec4 ProjectionRayStart = glm::vec4(NormalizedX, NormalizedY, 1.0f, 1.0f);
	const glm::vec4 ProjectionRayEnd = glm::vec4(NormalizedX, NormalizedY, 0.5f, 1.0f);

	const glm::vec4 HomogeneousRayStart = InvertedViewProjection * ProjectionRayStart;
	const glm::vec4 HomogeneousRayEnd = InvertedViewProjection * ProjectionRayEnd;

	glm::vec3 RayStartWorldSpace(HomogeneousRayStart.x, HomogeneousRayStart.y, HomogeneousRayStart.z);
	glm::vec3 RayEndWorldSpace(HomogeneousRayEnd.x, HomogeneousRayEnd.y, HomogeneousRayEnd.z);

	// divide vectors by W to undo any projection and get the 3-space coordinate 
	if (HomogeneousRayStart.w != 0.0f)	{ RayStartWorldSpace /= HomogeneousRayStart.w; }
	if (HomogeneousRayEnd.w != 0.0f)	{ RayEndWorldSpace /= HomogeneousRayEnd.w; }


	WorldPosition = RayEndWorldSpace;

	// check normalization safety
	glm::vec3 DirCheck = RayEndWorldSpace - RayStartWorldSpace;
	WorldDirection = ((DirCheck.x * DirCheck.x + DirCheck.y * DirCheck.y + DirCheck.z * DirCheck.z) == 0) ? glm::vec3() : glm::normalize(RayEndWorldSpace - RayStartWorldSpace);
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
Gizmo* Universe::GetGizmo() { return SelectionGizmo; }