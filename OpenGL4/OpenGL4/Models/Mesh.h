#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Asset;
class Texture;
class Shader;

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
	std::string GetName();

private:
	GLuint VAO, VBO, EBO;
	void SetupMesh();
	Asset* ParentAsset;
	std::string Name;
};

