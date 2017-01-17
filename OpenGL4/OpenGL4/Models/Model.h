#pragma once
/* GLM */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <stdio.h>
#include <vector>
#include "../Texture.h"

class Model {
public:
	Model();
	~Model();

	std::vector<GLfloat> vertices;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	int vertSize;
	int indicesSize;

	glm::mat4 orientation;

	void GenerateModel();
	void GenerateArrayModel();
	void TranslateModel(float x, float y, float z);
	void RotateModel();
	void Draw();
	void AttachDiffuseTexture(Texture* inTexture);
	void AttachSpecularTexture(Texture* inTexture);

private:
	GLuint diffuseTexture;
	GLuint specularTexture;
};

