#include "Model.h"



Model::Model() {

}

Model::~Model() {
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Model::GenerateModel() {
	GLfloat vertArr[] = {
		// Positions         // Colors       
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	   // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,	   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,	   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,	   // Top Left 

		0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 0.0f,	   // Top Right
		0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,	   // Bottom Right
		-0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,	   // Bottom Left
		-0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,	   // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3,  // Second Triangle

		4, 5, 7,  // First Triangle
		5, 6, 7   // Second Triangle
	};

	vertSize = sizeof(vertArr) / sizeof(vertArr[0]);
	indicesSize = sizeof(indices) / sizeof(indices[0]);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertArr), vertArr, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	vertices = std::vector<GLfloat>(vertArr, vertArr + sizeof(vertArr) / sizeof(vertArr[0]));
}

void Model::TranslateModel(float x, float y, float z) {
	orientation = glm::translate(orientation, glm::vec3(x, y, z));
}

void Model::RotateModel() {
	orientation = glm::rotate(orientation, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
