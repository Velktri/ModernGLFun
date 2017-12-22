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
class Gizmo;
class Grid;

class Manager {
public:
	Manager();
	~Manager();

	Asset* BuildAsset(std::string path = "", bool bIsUserAsset = true);
	Asset* BuildPrimative(Primatives InType);
	void DrawLights(glm::mat4 InView, glm::mat4 InProjection);
	void CheckForSelection(int InID);
	void AddAssetToPool(Asset* InAsset);
	Element* DrawLine(glm::vec3 StartPoint, glm::vec3 EndPoint);
	Element* DrawLine(glm::vec3 StartPoint, glm::vec3 LineDirection, float Distance);
	void ClearLines();

	template<class T>
	T* CheckPool(std::string path);

	template<class T>
	T* SpawnToPool(std::string path);


	/* GETTERS */
	std::vector<Element*> GetMeshList();
	std::vector<Texture*> GetTextures();
	Gizmo* GetGizmo();
	Shader* GetSystemShader();
	Shader* GetLightShader();
	Shader* GetDefaultShader();
	Shader* GetCurrentShader();
	Texture* GetDefaultTexture();
	Material* GetDefaultMaterial();
	std::vector<Shader*> GetUserShaderList();
	Asset* GetSelectedAsset();
	std::vector<Asset*> GetUserAssets();
	int GetAssetListSize();
	std::vector<Asset*> GetAssetsFromMap(Shader* InShader);
	std::vector<Light*> GetLights();
	void SetSystemShader(glm::mat4 InViewProjection);
	void UpdateSelectedAsset(Asset* InAsset);
	void SetPickerShader();
	void SetCurrentShader(Shader* s);
	std::vector<Element*> GetSystemElements();

	glm::vec3 testColor;
	float testRoughness;
	float testMetallic;

private:
	/* Assets */
	std::unordered_map<Shader*, std::vector<Asset*>> AssetMap;
	std::vector<Asset*> UserAssetList;
	Asset* SelectedAsset;
	Gizmo* SelectionGizmo;
	

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
	Texture* DefaultLightTexture;
	Resource* BuildTexture(std::string path);

	/* Lights */
	std::vector<Light*> LightsList;
	Light*  BuildLight(LampType InType, glm::vec3 InPosition = glm::vec3());

	/* Meshes */
	std::vector<Element*> MeshList;
	Resource* ProcessMesh(std::string path);

	/* System */
	Grid* GridFloor;

	int GRIDRADIUS_X = 20;
	int GRIDRADIUS_Y = 20;
	float GRIDSPACING = 1.0;

	std::vector<Element*> SystemElements;
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
