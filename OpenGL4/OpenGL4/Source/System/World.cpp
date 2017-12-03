#include "World.h"
#include "Camera.h"
#include "VR_HMD.h"
#include "Manager.h"
#include "Timer.h"
#include "ModelData\Grid.h"
#include "ModelData\Line.h"
#include "ModelData\Curve.h"
#include "Models\Asset.h"
#include "Models\Shader.h"
#include "Lights\Light.h"
#include "ModelData\Gizmo.h"

World::World(Manager* InManager)
{
	MyManager = InManager;
	UserCamera = new Camera(glm::vec3(0.0f, 10.0f, 20.0f));
	GridFloor = new Grid(GRIDRADIUS_X, GRIDRADIUS_Y, GRIDSPACING);
	SelectionGizmo = new Gizmo();
	SystemElements.push_back(GridFloor);
	SystemElements.push_back(SelectionGizmo);

	WorldClock = new Timer();
	bInitVR = false;
}

World::~World()
{
	for (Element* e : SystemElements)
	{
		e->~Element();
	}
	UserCamera->~Camera();
	WorldClock->~Timer();
}

void World::InitVR(vr::IVRSystem* InHMD)
{
	VRCamera = new VR_HMD(this, InHMD);
	bInitVR = true;
}

void World::RenderWorld(glm::vec2 FrameSize)
{
	UserCamera->UpdatePerspective(FrameSize);
	RenderSystemEntities();
	RenderUserEntities();
	if (bInitVR) { VRCamera->Render(); }
}

void World::RenderColorWorld(glm::vec2 FrameSize)
{
	UserCamera->UpdatePerspective(FrameSize);
	MyManager->SetPickerShader();
	Shader* shader = MyManager->GetCurrentShader();
	shader->Use();
	glUniformMatrix4fv(shader->ShaderList["ViewProjection"], 1, GL_FALSE, glm::value_ptr(glm::scale(UserCamera->GetProjection(), glm::vec3(1, -1, 1)) * UserCamera->GetViewMatrix()));

	for each (Asset* mod in MyManager->GetAssets())
	{
		int r = (mod->GetAssetID() & 0x000000FF) >> 0;
		int g = (mod->GetAssetID() & 0x0000FF00) >> 8;
		int b = (mod->GetAssetID() & 0x00FF0000) >> 16;
		glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(mod->GetWorldSpace()));
		glUniform4f(shader->ShaderList["PickingColor"], r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
		mod->Render(shader);
	}
}

// @TODO: rebuild for assets
//void World::CreateCurve()  
//{
//	SystemElements.push_back(new Curve());
//}

/* @TODO: Ray Trace is aiming slightly lower than it should. 
		  The borders between regions are throwing the positions off by a couple of pixels

   @TODO: split into two functions. one to change coords from device to world space and other to ray cast.
*/
Asset* World::CastRaytrace(glm::vec2 DeviceCoords, glm::vec2 SceneSize)
{
	printf("%f, %f  ", DeviceCoords.x, DeviceCoords.y);

	float x = (2.0f * DeviceCoords.x) / SceneSize.x - 1.0f;
	float y = 1.0f - (2.0f * DeviceCoords.y) / SceneSize.y;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);

	glm::vec4 ray_eye = glm::inverse(UserCamera->GetProjection()) * ray_clip;
	ray_eye.z = -1.0f;
	ray_eye.w = 0.0f;

	glm::vec4 a = glm::vec4(glm::inverse(UserCamera->GetViewMatrix()) * ray_eye);
	glm::vec3 ray_wor = glm::vec3(a.x, a.y, a.z);
	ray_wor = glm::normalize(ray_wor);

	glm::vec3 lStart = UserCamera->GetPosition();
	glm::vec3 lEnd = lStart + (ray_wor * 100.0f);

	SystemElements.push_back(new Line(lStart, lEnd));

	return NULL;
}

void World::RenderSystemEntities()
{
	MyManager->SetSystemShader(UserCamera);
	for each (Element* e in SystemElements)
	{
		e->Render(MyManager->GetSceneShader());
	}
}

void World::RenderUserEntities()
{
	/* Draw Assets */
	MyManager->SetCurrentShader(NULL);
	for each (Shader* s in MyManager->GetUserShaderList())
	{
		MyManager->ShadeAssets(UserCamera, GetLights(), s);
		MyManager->DrawAssets(UserCamera, s);
	}

	/* Draw Lights */
	MyManager->ShadeLights(UserCamera, MyManager->GetLightShader());
	MyManager->Draw(MyManager->GetLightShader());
}

void World::ClearLines()
{
	for each (Element* e in SystemElements)
	{
		if (e->GetType() == ShaderType::LINE)
		{
			e->~Element();
		}
	}
}



Camera* World::GetCamera() { return UserCamera; }
std::vector<Light*> World::GetLights() { return MyManager->GetLights(); }
Manager* World::GetManager() { return MyManager; }
Timer* World::GetTimer() { return WorldClock; }