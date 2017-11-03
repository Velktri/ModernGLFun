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

/** 
An empty asset with no components.
Base class for all Assets.
*/
class Asset 
{
public:
	Asset(GLuint InAssetID, Manager* InManager);
	~Asset();
	void Render(Shader* shader, Camera* WorldCamera);
	void TranslateAsset(float x, float y, float z);
	void RotateAsset(float x, float y, float z);
	void ScaleAsset(float x, float y, float z);


	std::string Name;

	/** Getters */
	glm::vec3 GetOrigin();
	GLuint GetAssetID();
	glm::mat4 GetWorldSpace();
	ComponentBase* GetRoot();
	Manager* GetManager();

private:
	glm::vec3 OriginPoint;
	glm::mat4 WorldSpaceOrientation;
	GLuint AssetID;
	ComponentBase* Root;
	Manager* MyManager;
};

