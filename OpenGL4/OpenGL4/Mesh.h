#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <sstream>
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture*> textures;


	Mesh(std::vector<Vertex> InVertices, std::vector<GLuint> InIndices, std::vector<Texture*> InTextures);
	~Mesh();

	void Draw(Shader* shader);

private:
	GLuint VAO, VBO, EBO;
	void SetupMesh();

};

