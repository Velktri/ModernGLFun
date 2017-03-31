#include "Manager.h"



Manager::Manager() {
	BuildShaders();
	CurrentShader = DefaultShader;
	AssetMap[DefaultShader] = std::vector<Asset*>();
	BuildLights();
}


Manager::~Manager() {
	/* Shaders */
	for each (Shader* s in UserShaderList) {
		s->~Shader();
	}

	for each (Shader* s in SystemShaderList) {
		s->~Shader();
	}

	/* Assets */
	for each (Asset* mod in AssetList) {
		mod->~Asset();
	}

	/* Textures */
	for each (Texture* t in TextureList) {
		t->~Texture();
	}

	/* Lights */
	for each (Light* l in LightsList) {
		l->~Light();
	}
}

Shader* Manager::GetSceneShader() {
	return SceneShader;
}

Shader* Manager::GetAssetShader() {
	return AssetShader;
}

Shader* Manager::GetLightShader() {
	return LightShader;
}

Shader* Manager::GetScreenShader() {
	return ScreenShader;
}

Shader* Manager::GetDefaultShader() {
	return DefaultShader;
}

Shader* Manager::GetCurrentShader() {
	return CurrentShader;
}

void Manager::SetCurrentShader(Shader* s) {
	CurrentShader = s;
}

std::vector<Shader*> Manager::GetUserShaderList() {
	return UserShaderList;
}

void Manager::ShadeAssets(Camera* WorldCamera, std::vector<Light*> Lights, Shader* InCurrentShader) {
	if (InCurrentShader != CurrentShader) {
		CurrentShader = InCurrentShader;
		CurrentShader->Use();

		glUniformMatrix4fv(CurrentShader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetViewMatrix()));
		glUniformMatrix4fv(CurrentShader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetProjection()));
		glm::vec3 pos = WorldCamera->GetPosition();
		glUniform3f(CurrentShader->ShaderList["cameraPos"], pos.x, pos.y + 1, pos.z);
		//glm::vec3 pos = WorldCamera->GetPosition();
		//glUniform3f(AssetShader->ShaderList["viewPos"], pos.x, pos.y, pos.z);
		//glUniform3f(AssetShader->ShaderList["dirLight.position"], 0.0f, 0.0f, 0.0f);
		//glUniform3f(AssetShader->ShaderList["dirLight.ambient"], 0.05f, 0.05f, 0.05f);
		//glUniform3f(AssetShader->ShaderList["dirLight.diffuse"], 1.0f, 1.0f, 1.0f);
		//glUniform3f(AssetShader->ShaderList["dirLight.specular"], 0.5f, 0.5f, 0.5f);
		//glUniform1i(AssetShader->ShaderList["material.diffuse"], 0);
		//glUniform1i(AssetShader->ShaderList["material.specular"], 1);
		//glUniform1f(AssetShader->ShaderList["material.shininess"], 32.0f);

		//glUniform3f(AssetShader->ShaderList["pointLight.position"], Lights[0]->WorldPosition.x, Lights[0]->WorldPosition.y, Lights[0]->WorldPosition.z);
		//glUniform3f(AssetShader->ShaderList["pointLight.ambient"], 0.05f, 0.05f, 0.05f);
		//glUniform3f(AssetShader->ShaderList["pointLight.diffuse"], 0.8f, 0.8f, 0.8f);
		//glUniform3f(AssetShader->ShaderList["pointLight.specular"], 1.0f, 1.0f, 1.0f);
		//glUniform1f(AssetShader->ShaderList["pointLight.constant"], 1.0f);
		//glUniform1f(AssetShader->ShaderList["pointLight.linear"], 0.09);
		//glUniform1f(AssetShader->ShaderList["pointLight.quadratic"], 0.032);
	}
}

void Manager::BuildShaders() {
	SceneShader = new Shader("assets/Shaders/Scene.vert", "assets/Shaders/Scene.frag");
	//AssetShader = new Shader("assets/Shaders/Lighting.vert", "assets/Shaders/Lighting.frag");
	LightShader = new Shader("assets/Shaders/Lamp.vert", "assets/Shaders/Lamp.frag");
	ScreenShader = new Shader("assets/Shaders/Screen.vert", "assets/Shaders/Screen.frag");
	DefaultShader = new Shader("assets/Shaders/Default.vert", "assets/Shaders/Default.frag");

	SystemShaderList.push_back(SceneShader);
	//ShaderList.push_back(AssetShader);
	SystemShaderList.push_back(LightShader);
	SystemShaderList.push_back(ScreenShader);
	UserShaderList.push_back(DefaultShader);
}

void Manager::DrawAssets(Shader* Shader) {
	for each (Asset* mod in AssetMap[Shader]) {
		glUniformMatrix4fv(Shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(mod->orientation));
		mod->Draw(Shader);
	}
}

void Manager::BuildAsset(std::string path) {
	Asset* a = new Asset(path);

	char label[128];
	sprintf_s(label, "Object_%d", AssetList.size());
	a->Name = label;
	AssetMap[DefaultShader].push_back(a);
	AssetList.push_back(a);
}

std::vector<Asset*> Manager::GetAssets() {
	return AssetList;
}


std::vector<Light*> Manager::GetLights() {
	return LightsList;
}

void Manager::ShadeLights(Camera* WorldCamera, Shader* LightShader) {
	LightShader->Use();
	glUniformMatrix4fv(LightShader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetViewMatrix()));
	glUniformMatrix4fv(LightShader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetProjection()));
}

void Manager::Draw(Shader* shader) {
	for each (Light* li in LightsList) {
		glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(li->GetOrientation()));
		li->Draw();
	}
}

void Manager::BuildTexture(std::string path) {
	TextureList.push_back(new Texture(path));
}

void Manager::BuildLights() {
	LightsList.push_back(new Light(glm::vec3(-3.0f, 2.0f, 0.0f)));
}
