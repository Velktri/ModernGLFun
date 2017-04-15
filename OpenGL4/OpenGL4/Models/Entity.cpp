#include "Entity.h"
#include "Shader.h"
#include "../Camera.h"
#include <iostream>

Entity::Entity() {
	type = NONE;
	ParentAsset = NULL;
}

Entity::~Entity() {
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Entity::Init() {
	try {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		if (indices.size() > 0) {
			glGenBuffers(1, &EBO);
		}

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

		if (indices.size() > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
		}

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

		if (type == MESH) {
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
		}

		glBindVertexArray(0);

	} catch (const std::exception &exc) {
		std::cerr << exc.what();
	}
}

void Entity::Draw(Shader* shader, Camera* WorldCamera) {
	if (type != MESH) {
		glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(glm::mat4()));
	}

	glUniform1i(shader->ShaderList["type"], type);

	if (indices.size() > 0) {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	} else {
		glBindVertexArray(VAO);
		(type == CURVE) ? glDrawArrays(GL_LINE_STRIP, 0, vertices.size()) : glDrawArrays(GL_LINES, 0, vertices.size());
		glBindVertexArray(0);
	}
}

std::string Entity::GetName() {
	return Name;
}

Asset* Entity::GetParentAsset() {
	return ParentAsset;
}

int Entity::GetType() {
	return type;
}
