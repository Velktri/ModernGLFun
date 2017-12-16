#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "HelperTypes.h"

class Texture;

class Light {
public:
	Light(Texture* InTexture, glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f));
	~Light();

	glm::vec3 WorldPosition;
	glm::vec3 Color;
	int indicesSize;

	void Translate(float x, float y, float z);
	void BuildLight();
	void Draw();


	/* GETTERS */
	GLuint GetVAO();
	glm::mat4 GetOrientation();
	Texture* GetLightTexture();
	LampType GetType();
	void SetLightTexture(Texture* InTexture);

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	glm::mat4 orientation;
	Texture* LightBillboard;
	LampType type;
};

