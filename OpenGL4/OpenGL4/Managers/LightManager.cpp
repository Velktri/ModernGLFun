#include "LightManager.h"



LightManager::LightManager() {
	BuildLights();
}


LightManager::~LightManager() {
	for each (Light* l in LightsList) {
		l->~Light();
	}
}

std::vector<Light*> LightManager::GetLights() {
	return LightsList;
}

void LightManager::ShadeLights(Camera* WorldCamera, Shader* LightShader) {
	LightShader->Use();
	glUniformMatrix4fv(LightShader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetViewMatrix()));
	glUniformMatrix4fv(LightShader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetProjection()));
}

void LightManager::Draw(Shader* shader) {
	for each (Light* li in LightsList) {
		glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(li->GetOrientation()));
		li->Draw();
	}
}

void LightManager::BuildLights() {
	LightsList.push_back(new Light(glm::vec3(-3.0f, 2.0f, 0.0f)));

}
