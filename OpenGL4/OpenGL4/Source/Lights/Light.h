#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
public:
	Light(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f));
	~Light();

	glm::vec3 WorldPosition;
	glm::vec3 Color;
	int indicesSize;

	GLuint GetVAO();
	glm::mat4 GetOrientation();

	void Translate(float x, float y, float z);
	void BuildLight();
	void Draw();

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	glm::mat4 orientation;
};

