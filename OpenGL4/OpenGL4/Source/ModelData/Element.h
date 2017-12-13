#pragma once
#include <GL/glew.h>
#include "../HelperTypes.h"
#include <vector>

class Asset;
class Texture;

enum ShaderType {
	NONE = 0,
	GRID = 1,
	LINE = 2,
	CURVE = 3,
	GIZMO = 4,
	MESH = 5
};


class Element {
public:
	Element();
	~Element();

	void Render(class Shader* shader);



	/* GETTERS */
	int GetType();
	bool HasTextureCoords();
	glm::vec3 Color;

protected:
	void Init();

	GLuint VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	bool BhasTexCoords;


	ShaderType type;
};