#include "World.h"
#include "Universe.h"
#include "Camera.h"
#include "Manager.h"
#include "Timer.h"
#include "ModelData\Element.h"
#include "Models\Asset.h"
#include "Models\Shader.h"
#include "Lights\Light.h"

World::World(Universe* InUniverse, Manager* InManager)
{
	OwningUniverse = InUniverse;
	MyManager = InManager;
}

World::~World()
{

}

void World::RenderWorld(glm::vec3 InCameraPosition, glm::mat4 InViewProjection, glm::vec2 FrameSize)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	/* System Rendering */
	MyManager->SetSystemShader(InViewProjection);
	for each (Element* e in OwningUniverse->GetSystemElements())
	{
		e->Render(MyManager->GetSceneShader());
	}


	/* User Rendering */
	MyManager->SetCurrentShader(NULL);
	for each (Shader* s in MyManager->GetUserShaderList())
	{
		MyManager->ShadeAssets(InCameraPosition, InViewProjection, GetLights(), s);
		MyManager->DrawAssets(s);
	}

	MyManager->ShadeLights(InViewProjection, MyManager->GetLightShader());
	MyManager->Draw(MyManager->GetLightShader());
}

void World::RenderColorWorld(Camera* InCamera, glm::vec2 FrameSize)
{
	MyManager->SetPickerShader();
	Shader* shader = MyManager->GetCurrentShader();
	shader->Use();
	glUniformMatrix4fv(shader->ShaderList["ViewProjection"], 1, GL_FALSE, glm::value_ptr(glm::scale(InCamera->GetProjection(), glm::vec3(1, -1, 1)) * InCamera->GetViewMatrix()));

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


std::vector<Light*> World::GetLights() { return MyManager->GetLights(); }
Manager* World::GetManager() { return MyManager; }