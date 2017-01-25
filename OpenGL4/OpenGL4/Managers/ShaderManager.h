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
	Shader* GetDefaultShader();

	Shader* GetCurrentShader();
	void SetCurrentShader(Shader* s);

	std::vector<Shader*> GetUserShaderList();

	void ShadeAssets(Camera* WorldCamera, std::vector<Light*> Lights, Shader* InCurrentShader);

private:
	Shader* SceneShader;
	Shader* AssetShader;
	Shader* LightShader;
	Shader* ScreenShader;
	Shader* DefaultShader;

	Shader* CurrentShader;

	std::vector<Shader*> UserShaderList;
	std::vector<Shader*> SystemShaderList;
	void BuildShaders();
};

