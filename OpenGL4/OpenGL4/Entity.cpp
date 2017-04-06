#include "Entity.h"
#include "Models/Shader.h"
#include "Camera.h"
#include <iostream>

Entity::Entity() {
	type = NONE;
}

Entity::~Entity() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Entity::Init() {
	try {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	} catch (const std::exception &exc) {
		std::cerr << exc.what();
	}
}

void Entity::Draw(Shader* shader, Camera* WorldCamera) {
	shader->Use();
	glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(glm::mat4()));
	glUniformMatrix4fv(shader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetViewMatrix()));
	glUniformMatrix4fv(shader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetProjection()));
	glUniform1i(shader->ShaderList["type"], type);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, vertSize);
}
