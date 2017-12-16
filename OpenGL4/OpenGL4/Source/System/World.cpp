#include "World.h"
#include "Universe.h"
#include "Camera.h"
#include "Manager.h"
#include "Timer.h"
#include "Models\Gizmo.h"
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

void World::RenderWorld(glm::vec3 InCameraPosition, glm::mat4 InView, glm::mat4 InProjection, glm::vec2 FrameSize)
{
	glm::mat4 InViewProjection = InProjection * InView;
	glEnable(GL_DEPTH_TEST);
	/* User Rendering */
	MyManager->SetCurrentShader(NULL);
	for each (Shader* Shade in MyManager->GetUserShaderList())
	{
		Shade->Use();
		glUniformMatrix4fv(Shade->ShaderList["ViewProjection"], 1, GL_FALSE, glm::value_ptr(InViewProjection));
		glUniform3f(Shade->ShaderList["cameraPos"], InCameraPosition.x, InCameraPosition.y, InCameraPosition.z);

		std::vector<Light*> LightsList = MyManager->GetLights();
		for (int i = 0; i < LightsList.size(); i++)
		{
			glUniform3fv(glGetUniformLocation(Shade->GetShader(), std::string("lightPositions[" + std::to_string(i) + ']').c_str()), LightsList.size(), &LightsList[i]->WorldPosition[0]);
			glUniform3fv(glGetUniformLocation(Shade->GetShader(), std::string("lightColors[" + std::to_string(i) + ']').c_str()), LightsList.size(), &LightsList[i]->Color[0]);
		}

		for each (Asset* mod in MyManager->GetAssetsFromMap(Shade))
		{
			glUniformMatrix4fv(Shade->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(mod->GetWorldSpace()));
			mod->Render(Shade);
		}
	}

	MyManager->DrawLights(InView, InProjection);


	/* System Rendering */
	MyManager->SetSystemShader(InViewProjection);
	for each (Element* e in OwningUniverse->GetSystemElements())
	{
		glUniform3f(MyManager->GetSystemShader()->ShaderList["color"], e->Color.x, e->Color.y, e->Color.z);
		e->Render(MyManager->GetSystemShader());
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	OwningUniverse->GetGizmo()->Render(MyManager->GetSystemShader());
}

void World::RenderColorWorld(Camera* InCamera, glm::vec2 FrameSize)
{
	MyManager->SetPickerShader();
	Shader* shader = MyManager->GetCurrentShader();
	shader->Use();
	glUniformMatrix4fv(shader->ShaderList["ViewProjection"], 1, GL_FALSE, glm::value_ptr(InCamera->GetViewProjection()));

	// @TODO: add gizmo selection

	for each (Asset* mod in MyManager->GetAssets())
	{
		int r = ((mod->GetAssetID() + 100) & 0x000000FF) >> 0;
		int g = ((mod->GetAssetID() + 100) & 0x0000FF00) >> 8;
		int b = ((mod->GetAssetID() + 100) & 0x00FF0000) >> 16;
		glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(mod->GetWorldSpace()));
		glUniform4f(shader->ShaderList["PickingColor"], r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
		mod->Render(shader);
	}
}


std::vector<Light*> World::GetLights() { return MyManager->GetLights(); }
Manager* World::GetManager() { return MyManager; }