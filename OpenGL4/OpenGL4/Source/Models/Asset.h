#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

/* Assimp */
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

class Mesh;
class ComponentBase;
class Texture;
class Shader;
class Camera;
class Manager;
class Transforms;

/** 
An empty asset with no components.
Base class for all Assets.
*/
class Asset 
{
public:
	Asset(GLuint InAssetID, Manager* InManager);
	~Asset();
	virtual void Render(Shader* shader);
	void TranslateAsset(glm::vec3 InVector);
	void RotateAsset(glm::vec3 InVector);
	void ScaleAsset(glm::vec3 InVector);


	std::string Name;

	/** Getters */
	glm::vec3 GetOrigin();
	GLuint GetAssetID();
	glm::mat4 GetWorldSpace();
	void SetWorldSpace(glm::mat4 InWorldSpace);
	ComponentBase* GetRoot();
	Manager* GetManager();
	Transforms* GetAssetTransforms();

	glm::vec3 testColor;
	float testRoughness;
	float testMetallic;

protected:
	Transforms* AssetTransform;
	GLuint AssetID;
	ComponentBase* Root;
	Manager* MyManager;

	void UpdateComponentOrientation();
};

