#pragma once
#include <GL/glew.h>
#include "../Models/Shader.h"
#include "Mesh.h"
#include <vector>
#include <string>
#include <iostream>

/* Assimp */
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

class Mesh;


class Asset {
public:
	Asset(std::string path);
	~Asset();
	void Draw(Shader* shader);

	std::string Name;
	glm::mat4 orientation;

	void TranslateAsset(float x, float y, float z);
	void RotateAsset(float x, float y, float z);
	void ScaleAsset(float x, float y, float z);
	glm::vec3 GetOrigin();

private:
	std::vector<Mesh*> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;
	glm::vec3 OriginPoint;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

