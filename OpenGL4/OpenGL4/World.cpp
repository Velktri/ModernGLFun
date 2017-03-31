#include "World.h"

World::World(GLuint width, GLuint height) {
	WorldCamera = new Camera(glm::vec3(0.0f, 10.0f, 20.0f));
	WorldCamera->SetProjection(glm::perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.01f, 1000.0f));

	Scene = new Grid(GRIDRADIUS_X, GRIDRADIUS_Y, GRIDSPACING);
	bIsClockRunning = false;
	DeltaTime = 0.0f;
	LastFrame = 0.0f;
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

Asset* World::GetSelectedAsset() {
	return SelectedAsset;
}

void World::SetSelectedAsset(Asset* InAsset) {
	SelectedAsset = InAsset;
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


