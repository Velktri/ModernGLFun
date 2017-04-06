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
class Texture;
class Shader;

class Asset {
public:
	Asset();
	Asset(std::string path);
	~Asset();
	void Draw(Shader* shader);

	Mesh* GetMesh();
	void SetMesh(Mesh* InMesh);
	Texture* GetTexture();

	std::string Name;
	glm::mat4 orientation;


	void TranslateAsset(float x, float y, float z);
	void RotateAsset(float x, float y, float z);
	void ScaleAsset(float x, float y, float z);
	glm::vec3 GetOrigin();
	std::vector<Mesh*> GetMeshes();

private:
	std::vector<Mesh*> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;
	glm::vec3 OriginPoint;
	Mesh* StaticMesh;
	Texture* TextureMap;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

