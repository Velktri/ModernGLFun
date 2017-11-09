#pragma once
#include <vector>
#include <unordered_map>
#include "HelperTypes.h"

class Shader;
class Asset;
class Light;
class Camera;
class Texture;
class Element;
class Mesh;

class Manager {
public:
	Manager();
	~Manager();

	void ShadeAssets(Camera* WorldCamera, std::vector<Light*> Lights, Shader* InCurrentShader);
	void DrawAssets(Camera* WorldCamera, Shader* AssetShader);
	void BuildAsset(std::string path = "");
	void BuildPrimative(Primatives InType);
	void ShadeLights(Camera* WorldCamera, Shader* LightShader);
	void Draw(Shader* shader);
	void CheckForSelection(int InID);
	void AddAssetToPool(Asset* InAsset);

	template<class T>
	T* CheckPool(std::string path);

	template<class T>
	T* SpawnToPool(std::string path);


	/* GETTERS */
	std::vector<Element*> GetMeshList();
	Shader* GetSceneShader();
	Shader* GetAssetShader();
	Shader* GetLightShader();
	Shader* GetScreenShader();
	Shader* GetDefaultShader();
	Shader* GetCurrentShader();
	std::vector<Shader*> GetUserShaderList();
	Asset* GetSelectedAsset();
	std::vector<Asset*> GetAssets();
	std::vector<Light*> GetLights();
	void SetSystemShader(Camera* WorldCamera);
	void SetSelectedAsset(Asset* InAsset);
	void SetPickerShader();
	void SetCurrentShader(Shader* s);

private:
	/* Assets */
	std::unordered_map<Shader*, std::vector<Asset*>> AssetMap;
	std::vector<Asset*> AssetList;
	Asset* SelectedAsset;

	/* Pools */
	std::unordered_map<std::string, Mesh*> MeshPool;

	/* Shader */
	Shader* SceneShader;
	Shader* AssetShader;
	Shader* LightShader;
	Shader* ScreenShader;
	Shader* DefaultShader;
	Shader* PickerShader;

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
	std::vector<Element*> MeshList;
	Mesh* ProcessMesh(std::string path);
};


template<class T>
T* Manager::CheckPool(std::string path)
{
	if (std::is_same<T, Mesh>::value)
	{
		if (MeshPool.find(path) != MeshPool.end()) { return MeshPool.at(path); }
	}

	return NULL;
}

template<class T>
T* Manager::SpawnToPool(std::string path)
{
	if (path != "")
	{
		if (std::is_same<T, Mesh>::value)
		{
			T* NewMesh = ProcessMesh(path);
			MeshPool.emplace(path, NewMesh);
			return NewMesh;
		}
	}
	return NULL;
}