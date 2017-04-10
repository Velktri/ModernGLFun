#pragma once
#include <vector>
#include <unordered_map>

class Shader;
class Asset;
class Mesh;
class Light;
class Camera;
class Texture;

class Manager {
public:
	Manager();
	~Manager();

	std::vector<Mesh*> GetMeshList();
	Shader* GetSceneShader();
	Shader* GetAssetShader();
	Shader* GetLightShader();
	Shader* GetScreenShader();
	Shader* GetDefaultShader();
	Shader* GetCurrentShader();
	void SetCurrentShader(Shader* s);
	std::vector<Shader*> GetUserShaderList();
	Asset* GetSelectedAsset();
	void SetSelectedAsset(Asset* InAsset);


	void ShadeAssets(Camera* WorldCamera, std::vector<Light*> Lights, Shader* InCurrentShader);

	void DrawAssets(Shader* AssetShader);
	void BuildAsset(std::string path);
	void BuildAsset();
	std::vector<Asset*> GetAssets();

	std::vector<Light*> GetLights();
	void ShadeLights(Camera* WorldCamera, Shader* LightShader);
	void Draw(Shader* shader);

private:
	/* Assets */
	std::unordered_map<Shader*, std::vector<Asset*>> AssetMap;
	std::vector<Asset*> AssetList;
	Asset* SelectedAsset;

	/* Shader */
	Shader* SceneShader;
	Shader* AssetShader;
	Shader* LightShader;
	Shader* ScreenShader;
	Shader* DefaultShader;

	Shader* CurrentShader;

	bool bIsWireFrame;

	std::vector<Shader*> UserShaderList;
	std::vector<Shader*> SystemShaderList;
	void BuildShaders();

	/* Textures */
	void BuildTexture(std::string path);
	std::vector<Texture*> TextureList;

	/* Lights */
	std::vector<Light*> LightsList;
	void BuildLights();

	/* Meshes */
	std::vector<Mesh*> MeshList;
};

