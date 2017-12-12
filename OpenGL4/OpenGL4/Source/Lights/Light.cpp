#include "Light.h"

Light::Light() {
	WorldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	BuildLight();

	Color = glm::vec3(150.0f, 150.0f, 150.0f);
}

Light::Light(glm::vec3 Position) {
	WorldPosition = Position;
	BuildLight();

	Color = glm::vec3(150.0f, 150.0f, 150.0f);
}


Light::~Light() {
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

GLuint Light::GetVAO() {
	return VAO;
}

glm::mat4 Light::GetOrientation() {
	return orientation;
}

void Light::Translate(float x, float y, float z) {
	WorldPosition += glm::vec3(x, y, z);
	orientation = glm::translate(orientation, glm::vec3(x, y, z));
}

void Light::BuildLight() {
	GLfloat vertArr[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	orientation = glm::translate(orientation, WorldPosition);
}

void Light::Draw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}
