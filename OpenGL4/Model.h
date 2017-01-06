#pragma once
#include <GL/glew.h>
#include <stdio.h>
#include <vector>

class Model {
public:
	Model(GLuint ShaderProgram);
	~Model();

	std::vector<GLfloat> vertices;
	//GLfloat* elementsPtr;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void Transpose(float x, float y, float z);
	void PrintVerts();

	int vertSize;

};

