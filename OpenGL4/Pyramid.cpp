#include "Pyramid.h"



Pyramid::Pyramid() {
	GLfloat vertArr[] = {
		//  Position           
		-1.0f,   1.0f,   0.0f,
		 1.0f,   1.0f,   0.0f,
		 1.0f,  -1.0f,   0.0f,
		-1.0f,  -1.0f,   0.0f,
		 0.0f,   0.0f,   1.0f,
	};

	GLfloat indices[] = {
		0, 1, 2,
		2, 3, 0,

		0, 3, 4,
		1, 0, 4,
		2, 1, 4,
		3, 2, 4
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	vertices = std::vector<GLfloat>(vertArr, vertArr + sizeof(vertArr) / sizeof(vertArr[0]));
}


Pyramid::~Pyramid() {
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
