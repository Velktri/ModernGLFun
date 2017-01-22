#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "../Models/Shader.h"
#include "../Camera.h"
#include "../Lights/Light.h"

class ShaderManager {
public:
	ShaderManager();
	~ShaderManager();

	Shader* GetSceneShader();
	Shader* GetAssetShader();
	Shader* GetLightShader();
	Shader* GetScreenShader();
	void ShadeAssets(Camera* WorldCamera, std::vector<Light*> Lights);

private:
	Shader* SceneShader;
	Shader* AssetShader;
	Shader* LightShader;
	Shader* ScreenShader;

	std::vector<Shader*> ShaderList;
	void BuildShaders();
};

