#include "Light.h"

Light::Light(Texture* InTexture, glm::vec3 Position)
{
	type = LampType::NONE;
	WorldPosition = Position;
	LightBillboard = InTexture;
	BuildLight();
}

Light::~Light()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Light::Translate(float x, float y, float z)
{
	WorldPosition += glm::vec3(x, y, z);
	orientation = glm::translate(orientation, glm::vec3(x, y, z));
}

void Light::BuildLight() 
{
	GLfloat vertArr[] = {
		 0.25f,  0.25f,  0.0f,		1.0f, 0.0f,
		 0.25f, -0.25f,  0.0f,		1.0f, 1.0f,
		-0.25f,  0.25f,  0.0f,		0.0f, 0.0f,
		-0.25f, -0.25f,  0.0f,		0.0f, 1.0f,
	};

	GLuint indices[] = {
		0, 1, 3,
		0, 2, 3,
	};

	indicesSize = sizeof(indices) / sizeof(indices[0]);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertArr), vertArr, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));

	glBindVertexArray(0);

	orientation = glm::translate(orientation, WorldPosition);
	Color = glm::vec3(150.0f, 0.0f, 0.0f);
}

void Light::Draw() 
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}

GLuint Light::GetVAO() { return VAO; } 
glm::mat4 Light::GetOrientation() { return orientation; }
Texture* Light::GetLightTexture() { return LightBillboard; }
LampType Light::GetType() { return type; }

void Light::SetLightTexture(Texture* InTexture)
{
	if (!LightBillboard) { LightBillboard = InTexture; }
}
