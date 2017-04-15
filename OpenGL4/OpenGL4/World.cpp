#include "World.h"
#include "Camera.h"
#include "Manager.h"
#include "Models\Grid.h"
#include "Models\Line.h"
#include "Models\Asset.h"
#include "Models\Shader.h"
#include "Models\Curve.h"
#include "Lights\Light.h"
#include "Timer.h"

World::World(GLuint width, GLuint height) {
	WorldCamera = new Camera(glm::vec3(0.0f, 10.0f, 20.0f));
	WorldCamera->SetProjection(glm::perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.01f, 1000.0f));

	Scene = new Grid(GRIDRADIUS_X, GRIDRADIUS_Y, GRIDSPACING);
	SystemElements.push_back(Scene);

	WorldClock = new Timer();

	SceneWidth = width;
	SceneHeight = height;
}

World::~World() {
	Scene->~Grid();
}

Camera* World::GetCamera() {
	return WorldCamera;
}

std::vector<Light*> World::GetLights() {
	return MyManager->GetLights();
}

void World::RenderWorld() {
	RenderSystemEntities();
	RenderUserEntities();
}

void World::RenderColorWorld() {

	MyManager->SetPickerShader();
	Shader* shader = MyManager->GetCurrentShader();
	shader->Use();
	
	glUniformMatrix4fv(shader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetViewMatrix()));
	glUniformMatrix4fv(shader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(glm::scale(WorldCamera->GetProjection(), glm::vec3(1, -1, 1))));

	for each (Asset* mod in MyManager->GetAssets()) {
		int r = (mod->AssetID & 0x000000FF) >> 0;
		int g = (mod->AssetID & 0x0000FF00) >> 8;
		int b = (mod->AssetID & 0x00FF0000) >> 16;
		glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(mod->orientation));
		glUniform4f(shader->ShaderList["PickingColor"], r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
		mod->Draw(shader, WorldCamera);
	}
}
//
//void World::StartClock() {
//	WorldClock->Start();
//}
//
//void World::StopClock() {
//	WorldClock->Stop();
//}
//
//void World::UpdateClock() {
//	WorldClock->Update();
//}

void World::SetManager(Manager* m) {
	MyManager = m;
}
//
//GLfloat World::GetDeltaTime() {
//	return WorldClock->GetDeltaTime();
//}
//
//GLfloat World::GetTime() {
//	return WorldClock->GetTime();
//}

Timer* World::GetTimer() {
	return WorldClock;
}

void World::CreateCurve() {
	SystemElements.push_back(new Curve());
}

Asset* World::CastRaytrace(glm::vec2 DeviceCoords) {
	glm::vec2 ScreenSize = glm::vec2(SceneWidth, SceneHeight);
	DeviceCoords.y -= 237.0f;

	float x = (2.0f * DeviceCoords.x) / ScreenSize.x - 1.0f;
	float y = 1.0f - (2.0f * DeviceCoords.y) / ScreenSize.y;
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

void World::RenderSystemEntities() {
	MyManager->SetSystemShader(WorldCamera);
	for each (Entity* e in SystemElements) {
		e->Draw(MyManager->GetSceneShader(), WorldCamera);
	}
}

void World::RenderUserEntities() {

	/* Draw Assets */
	MyManager->SetCurrentShader(NULL);
	for each (Shader* s in MyManager->GetUserShaderList()) {
		MyManager->ShadeAssets(WorldCamera, GetLights(), s);
		MyManager->DrawAssets(WorldCamera, s);
	}

	/* Draw Lights */
	MyManager->ShadeLights(WorldCamera, MyManager->GetLightShader());
	MyManager->Draw(MyManager->GetLightShader());
}
