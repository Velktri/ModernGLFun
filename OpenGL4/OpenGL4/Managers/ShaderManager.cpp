#include "ShaderManager.h"



ShaderManager::ShaderManager() {
	BuildShaders();
}


ShaderManager::~ShaderManager() {
	for each (Shader* s in ShaderList) {
		s->~Shader();
	}
}

Shader* ShaderManager::GetSceneShader() {
	return SceneShader;
}

Shader* ShaderManager::GetAssetShader() {
	return AssetShader;
}

Shader* ShaderManager::GetLightShader() {
	return LightShader;
}

void ShaderManager::ShadeAssets(Camera* WorldCamera, std::vector<Light*> Lights) {
	AssetShader->Use();
	glUniformMatrix4fv(AssetShader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetViewMatrix()));
	glUniformMatrix4fv(AssetShader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetProjection()));
	glm::vec3 pos = WorldCamera->GetPosition();
	glUniform3f(AssetShader->ShaderList["viewPos"], pos.x, pos.y, pos.z);
	glUniform3f(AssetShader->ShaderList["dirLight.position"], 0.0f, 0.0f, 0.0f);
	glUniform3f(AssetShader->ShaderList["dirLight.ambient"], 0.05f, 0.05f, 0.05f);
	glUniform3f(AssetShader->ShaderList["dirLight.diffuse"], 1.0f, 1.0f, 1.0f);
	glUniform3f(AssetShader->ShaderList["dirLight.specular"], 0.5f, 0.5f, 0.5f);
	glUniform1i(AssetShader->ShaderList["material.diffuse"], 0);
	glUniform1i(AssetShader->ShaderList["material.specular"], 1);
	glUniform1f(AssetShader->ShaderList["material.shininess"], 32.0f);

	glUniform3f(AssetShader->ShaderList["pointLight.position"], Lights[0]->WorldPosition.x, Lights[0]->WorldPosition.y, Lights[0]->WorldPosition.z);
	glUniform3f(AssetShader->ShaderList["pointLight.ambient"], 0.05f, 0.05f, 0.05f);
	glUniform3f(AssetShader->ShaderList["pointLight.diffuse"], 0.8f, 0.8f, 0.8f);
	glUniform3f(AssetShader->ShaderList["pointLight.specular"], 1.0f, 1.0f, 1.0f);
	glUniform1f(AssetShader->ShaderList["pointLight.constant"], 1.0f);
	glUniform1f(AssetShader->ShaderList["pointLight.linear"], 0.09);
	glUniform1f(AssetShader->ShaderList["pointLight.quadratic"], 0.032);

}

void ShaderManager::BuildShaders() {
	SceneShader = new Shader("assets/Shaders/Scene.vert", "assets/Shaders/Scene.frag");
	AssetShader = new Shader("assets/Shaders/Lighting.vert", "assets/Shaders/Lighting.frag");
	LightShader = new Shader("assets/Shaders/Lamp.vert", "assets/Shaders/Lamp.frag");

	ShaderList.push_back(SceneShader);
	ShaderList.push_back(AssetShader);
	ShaderList.push_back(LightShader);
}
