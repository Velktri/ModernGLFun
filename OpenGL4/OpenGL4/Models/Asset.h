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
class Element;
class Texture;
class Shader;
class Camera;

class Asset {
public:
	Asset();
	~Asset();
	void Render(Shader* shader, Camera* WorldCamera);
	void AddComponent(Element* InMesh);
	void TranslateAsset(float x, float y, float z);
	void RotateAsset(float x, float y, float z);
	void ScaleAsset(float x, float y, float z);


	std::string Name;

	/** Getters */
	glm::vec3 GetOrigin();
	std::vector<Element*> GetComponents();
	GLuint GetAssetID();
	glm::mat4 GetWorldSpace();

private:
	std::vector<Element*> Components;
	glm::vec3 OriginPoint;
	glm::mat4 WorldSpaceOrientation;
	GLuint AssetID;
};

