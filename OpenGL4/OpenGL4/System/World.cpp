#include "World.h"
#include "Camera.h"
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
	WorldCamera = new Camera(glm::vec3(0.0f, 10.0f, 20.0f));

	Scene = new Grid(GRIDRADIUS_X, GRIDRADIUS_Y, GRIDSPACING);
	SelectionGizmo = new Gizmo();
	SystemElements.push_back(Scene);
	SystemElements.push_back(SelectionGizmo);

	WorldClock = new Timer();
}

World::~World()
{
	for (Element* e : SystemElements)
	{
		e->~Element();
	}
	WorldCamera->~Camera();
	WorldClock->~Timer();
}

void World::RenderWorld(glm::vec2 FrameSize)
{
	WorldCamera->SetProjection(glm::perspective(45.0f, (GLfloat) FrameSize.x / (GLfloat) FrameSize.y, 0.01f, 1000.0f));
	RenderSystemEntities();
	RenderUserEntities();
}

void World::RenderColorWorld(glm::vec2 FrameSize)
{
	WorldCamera->SetProjection(glm::perspective(45.0f, (GLfloat) FrameSize.x / (GLfloat) FrameSize.y, 0.01f, 1000.0f));
	MyManager->SetPickerShader();
	Shader* shader = MyManager->GetCurrentShader();
	shader->Use();

	glUniformMatrix4fv(shader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetViewMatrix()));
	glUniformMatrix4fv(shader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(glm::scale(WorldCamera->GetProjection(), glm::vec3(1, -1, 1))));

	for each (Asset* mod in MyManager->GetAssets())
	{
		int r = (mod->GetAssetID() & 0x000000FF) >> 0;
		int g = (mod->GetAssetID() & 0x0000FF00) >> 8;
		int b = (mod->GetAssetID() & 0x00FF0000) >> 16;
		glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(mod->GetWorldSpace()));
		glUniform4f(shader->ShaderList["PickingColor"], r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
		mod->Render(shader, WorldCamera);
	}
}

void World::CreateCurve()
{
	SystemElements.push_back(new Curve());
}

/* @TODO: Ray Trace is aiming slightly lower than it should. 
		  The borders between regions are throwing the positions off by a couple of pixels
*/
Asset* World::CastRaytrace(glm::vec2 DeviceCoords, glm::vec2 SceneSize)
{
	printf("%f, %f\n", DeviceCoords.x, DeviceCoords.y);

	float x = (2.0f * DeviceCoords.x) / SceneSize.x - 1.0f;
	float y = 1.0f - (2.0f * DeviceCoords.y) / SceneSize.y;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);

	glm::vec4 ray_eye = glm::inverse(WorldCamera->GetProjection()) * ray_clip;
	ray_eye.z = -1.0f;
	ray_eye.w = 0.0f;

	glm::vec4 a = glm::vec4(glm::inverse(WorldCamera->GetViewMatrix()) * ray_eye);
	glm::vec3 ray_wor = glm::vec3(a.x, a.y, a.z);
	ray_wor = glm::normalize(ray_wor);

	glm::vec3 lStart = WorldCamera->GetPosition();
	glm::vec3 lEnd = lStart + (ray_wor * 100.0f);

	SystemElements.push_back(new Line(lStart, lEnd));

	return NULL;
}

void World::RenderSystemEntities()
{
	MyManager->SetSystemShader(WorldCamera);
	for each (Element* e in SystemElements)
	{
		e->Render(MyManager->GetSceneShader(), WorldCamera);
	}
}

void World::RenderUserEntities()
{
	/* Draw Assets */
	MyManager->SetCurrentShader(NULL);
	for each (Shader* s in MyManager->GetUserShaderList())
	{
		MyManager->ShadeAssets(WorldCamera, GetLights(), s);
		MyManager->DrawAssets(WorldCamera, s);
	}

	/* Draw Lights */
	MyManager->ShadeLights(WorldCamera, MyManager->GetLightShader());
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

Camera* World::GetCamera() { return WorldCamera; }
std::vector<Light*> World::GetLights() { return MyManager->GetLights(); }
Timer* World::GetTimer() { return WorldClock; }