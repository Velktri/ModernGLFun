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

class Manager {
public:
	Manager();
	~Manager();

	//void ShadeAssets(glm::vec3 InCameraPosition, glm::mat4 InViewProjection, std::vector<Light*> Lights, Shader* InCurrentShader);
	void DrawAssets(glm::vec3 InCameraPosition, glm::mat4 InViewProjection, Shader* AssetShader);
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
	std::vector<Light*> GetLights();
	void SetSystemShader(glm::mat4 InViewProjection);
	void SetSelectedAsset(Asset* InAsset);
	void SetPickerShader();
	void SetCurrentShader(Shader* s);

	glm::vec3 testColor;
	float testRoughness;
	float testMetallic;
	Texture* BuildTexture(std::string path); //TEMP

private:
	// @TODO: create texture, shader, and material pools.
	// @TODO: move assets to world class.

	/* Assets */
	std::unordered_map<Shader*, std::vector<Asset*>> AssetMap;
	std::vector<Asset*> AssetList;
	Asset* SelectedAsset;



	/* Pools */
	std::unordered_map<std::string, Mesh*> MeshPool;

	/* Shader */
	Shader* SystemShader;
	Shader* LightShader;
	Shader* DefaultShader;
	Shader* PickerShader;
	Shader* CurrentShader;

	/* Material */
	Material* DefaultMaterial;

	bool bIsWireFrame;

	std::vector<Shader*> UserShaderList;
	std::vector<Shader*> SystemShaderList;
	void BuildShaders();

	/* Textures */
	//Texture* BuildTexture(std::string path);
	std::vector<Texture*> TextureList;
	Texture* DefaultTexture;

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
