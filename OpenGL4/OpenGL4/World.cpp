#include "World.h"
#include "Camera.h"
#include "Models\Grid.h"
#include "Manager.h"
#include "Line.h"
#include "Lights\Light.h"
#include "Models\Asset.h"
#include "Models\Shader.h"
#include "Curve.h"

World::World(GLuint width, GLuint height) {
	WorldCamera = new Camera(glm::vec3(0.0f, 10.0f, 20.0f));
	WorldCamera->SetProjection(glm::perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.01f, 1000.0f));

	Scene = new Grid(GRIDRADIUS_X, GRIDRADIUS_Y, GRIDSPACING);
	bIsClockRunning = false;
	DeltaTime = 0.0f;
	LastFrame = 0.0f;

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
	/* Draw Scene */
	Scene->Draw(MyManager->GetSceneShader(), WorldCamera);

	/* Draw Debug Lines */
	if (line) {
		line->Draw(MyManager->GetSceneShader(), WorldCamera);
	}

	if (curve) {
		curve->Draw(MyManager->GetSceneShader(), WorldCamera);
	}


	/* Draw Assets */
	MyManager->SetCurrentShader(NULL);
	for each (Shader* s in MyManager->GetUserShaderList()) {
		MyManager->ShadeAssets(WorldCamera, GetLights(), s);
		MyManager->DrawAssets(s);
	}

	/* Draw Lights */
	MyManager->ShadeLights(WorldCamera, MyManager->GetLightShader());
	MyManager->Draw(MyManager->GetLightShader());
}

void World::RenderScreen() {
	MyManager->GetScreenShader()->Use();

}

void World::StartClock() {
	if (!bIsClockRunning) {
		TimeStart = std::chrono::high_resolution_clock::now();
		bIsClockRunning = true;
	}
}

void World::StopClock() {
	if (bIsClockRunning) {
		bIsClockRunning = false;
	}
	DeltaTime = 0;
	LastFrame = 0;

}

void World::UpdateClock() {
	if (bIsClockRunning) {
		TimeNow = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(TimeNow - TimeStart).count();
		DeltaTime = time - LastFrame;
		LastFrame = time;
	}
}

void World::SetManager(Manager* m) {
	MyManager = m;
}

GLfloat World::GetDeltaTime() {
	return DeltaTime;
}

GLfloat World::GetTime() {
	return LastFrame;
}

void World::CreateCurve() {
	curve = new Curve();
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
	if (line)
	{
		delete line;
		line = new Line(lStart, lEnd);
	} else {
		line = new Line(lStart, lEnd);
	}

	return NULL;
}
