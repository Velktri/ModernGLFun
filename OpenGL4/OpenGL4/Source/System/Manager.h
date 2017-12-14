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
class Material;
class Resource;

class Manager {
public:
	Manager();
	~Manager();

	Asset* BuildAsset(std::string path = "");
	void BuildPrimative(Primatives InType);
	void ShadeLights(glm::mat4 InViewProjection, Shader* LightShader);
	void Draw(Shader* shader);
	void CheckForSelection(int InID);
	void AddAssetToPool(Asset* InAsset);

	template<class T>
	T* CheckPool(std::string path);

	template<class T>
	T* SpawnToPool(std::string path);


	/* GETTERS */
	std::vector<Element*> GetMeshList();
	Shader* GetSystemShader();
	Shader* GetLightShader();
	Shader* GetDefaultShader();
	Shader* GetCurrentShader();
	Texture* GetDefaultTexture();
	Material* GetDefaultMaterial();
	std::vector<Shader*> GetUserShaderList();
	Asset* GetSelectedAsset();
	std::vector<Asset*> GetAssets();
	int GetAssetListSize();
	std::vector<Asset*> GetAssetsFromMap(Shader* InShader);
	std::vector<Light*> GetLights();
	void SetSystemShader(glm::mat4 InViewProjection);
	void SetSelectedAsset(Asset* InAsset);
	void SetPickerShader();
	void SetCurrentShader(Shader* s);

	glm::vec3 testColor;
	float testRoughness;
	float testMetallic;

private:
	// @TODO: move assets to world class.

	/* Assets */
	std::unordered_map<Shader*, std::vector<Asset*>> AssetMap;
	std::vector<Asset*> AssetList;
	Asset* SelectedAsset;

	/* Pools */
	std::unordered_map<std::string, Resource*> ResourcePool;

	/* Shader */
	Shader* SystemShader;
	Shader* LightShader;
	Shader* DefaultShader;
	Shader* PickerShader;
	Shader* CurrentShader;
	std::vector<Shader*> UserShaderList;
	std::vector<Shader*> SystemShaderList;
	void BuildShaders();

	/* Material */
	Material* DefaultMaterial;
	std::vector<Material*> MaterialList;
	Resource* BuildMaterial();

	/* Textures */
	std::vector<Texture*> TextureList;
	Texture* DefaultTexture;
	Resource* BuildTexture(std::string path);

	/* Lights */
	std::vector<Light*> LightsList;
	void BuildLights();

	/* Meshes */
	std::vector<Element*> MeshList;
	Resource* ProcessMesh(std::string path);
};


template<class T>
T* Manager::CheckPool(std::string path)
{
	if (ResourcePool.find(path) != ResourcePool.end()) 
	{ 
		return reinterpret_cast<T*>(ResourcePool.at(path)); 
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
			T* NewMesh = reinterpret_cast<T*>(ProcessMesh(path));
			ResourcePool.emplace(path, NewMesh);
			return NewMesh;
		}
		else if (std::is_same<T, Texture>::value)
		{
			T* NewTexture = reinterpret_cast<T*>(BuildTexture(path));
			ResourcePool.emplace(path, NewTexture);
			return NewTexture;
		}
		else if (std::is_same<T, Material>::value)
		{
			T* NewMaterial = reinterpret_cast<T*>(BuildMaterial());
			ResourcePool.emplace(path, NewMaterial);
			return NewMaterial;
		}
	}
	return NULL;
}
