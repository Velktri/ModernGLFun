#pragma once
#include <vector>
#include "../Lights/Light.h"
#include "../Models/Shader.h"
#include "../Camera.h"

class LightManager {
public:
	LightManager();
	~LightManager();

	std::vector<Light*> GetLights();
	void ShadeLights(Camera* WorldCamera, Shader* LightShader);
	void Draw(Shader* shader);

private:
	std::vector<Light*> LightsList;
	void BuildLights();
};

