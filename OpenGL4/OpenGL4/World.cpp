#include "World.h"



World::World(GLuint width, GLuint height) {
	WorldCamera = new Camera(glm::vec3(0.0f, 10.0f, 20.0f));
	WorldCamera->SetProjection(glm::perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.01f, 1000.0f));

	ShaderManagement = new ShaderManager();
	TextureManagement = new TextureManager();
	AssetManagement = new AssetManager(ShaderManagement->GetDefaultShader());
	LightManagement = new LightManager();

	Scene = new Grid(GRIDRADIUS_X, GRIDRADIUS_Y, GRIDSPACING);
	bIsClockRunning = false;
	DeltaTime = 0.0f;
	LastFrame = 0.0f;
}


World::~World() {
	Scene->~Grid();
	ShaderManagement->~ShaderManager();
	TextureManagement->~TextureManager();
	AssetManagement->~AssetManager();
	LightManagement->~LightManager();
}

Camera* World::GetCamera() {
	return WorldCamera;
}

std::vector<Light*> World::GetLights() {
	return LightManagement->GetLights();
}

void World::RenderWorld() {
	/* Draw Scene */
	Scene->Draw(ShaderManagement->GetSceneShader(), WorldCamera);

	/* Draw Assets */
	ShaderManagement->SetCurrentShader(NULL);
	for each (Shader* s in ShaderManagement->GetUserShaderList()) {
		ShaderManagement->ShadeAssets(WorldCamera, GetLights(), s);
		AssetManagement->DrawAssets(s);
	}

	/* Draw Lights */
	LightManagement->ShadeLights(WorldCamera, ShaderManagement->GetLightShader());
	LightManagement->Draw(ShaderManagement->GetLightShader());
}

void World::RenderScreen() {
	ShaderManagement->GetScreenShader()->Use();

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

AssetManager* World::GetAssetManager() {
	return AssetManagement;
}

Asset* World::GetSelectedAsset() {
	return SelectedAsset;
}

void World::SetSelectedAsset(Asset* InAsset) {
	SelectedAsset = InAsset;
}

GLfloat World::GetDeltaTime() {
	return DeltaTime;
}

GLfloat World::GetTime() {
	return LastFrame;
}


