#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <sstream>
#include "../Models/Shader.h"
#include "../Models/Texture.h"
#include "../Camera.h"
#include "../Lights/Light.h"
#include "Asset.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Asset;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;


	Mesh(std::vector<Vertex> InVertices, std::vector<GLuint> InIndices, std::vector<Texture> InTextures, Asset* InAsset);
	~Mesh();

	void Draw(Shader* shader);
	Asset* GetParentAsset();

private:
	GLuint VAO, VBO, EBO;
	void SetupMesh();
	Asset* ParentAsset;

};

