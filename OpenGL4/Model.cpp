#include "Model.h"



Model::Model(GLuint ShaderProgram) {
	/*GLfloat vertices[] = {
		//  Position                       Color       
		-0.5f,   0.5f,   0.0f,     0.0f,   1.0f,   0.0f,
		 0.5f,   0.5f,   0.0f,     1.0f,   0.0f,   1.0f,
		 0.5f,  -0.5f,   0.0f,     0.0f,   1.0f,   1.0f,
		-0.5f,  -0.5f,   0.0f,     1.0f,   1.0f,   0.0f,
		 0.0f,   0.0f,   1.0f,     1.0f,   0.0f,   0.0f,
	};

	GLfloat elements[] = {
		0, 1, 2,
		2, 3, 0,

		0, 3, 4,
		1, 0, 4,
		2, 1, 4,
		3, 2, 4
	};*/

	GLfloat vertArr[] = {
		 0.5f,  0.5f, 0.0f,  
		 0.5f, -0.5f, 0.0f,  
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,  
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	vertSize = sizeof(vertArr) / sizeof(vertArr[0]);;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertArr), vertArr, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*GLint posAttrib = glGetAttribLocation(ShaderProgram, "position");
	GLint colAttrib = glGetAttribLocation(ShaderProgram, "color");


	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(posAttrib);

	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(colAttrib);*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	vertices = std::vector<GLfloat>(vertArr, vertArr + sizeof(vertArr) / sizeof(vertArr[0]));
	//elementsPtr = &elements[0];
}

Model::~Model() {
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Model::Transpose(float x, float y, float z) {
	for (std::size_t i = 0; i < vertSize; i++) {
		int axis = (i + 1) % 3;
		if (axis == 1) {
			vertices[i] += x;
		} else if (axis == 2) {
			vertices[i] += y;
		} else if (axis == 3) {
			vertices[i] += z;
		}
	}

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Model::PrintVerts() {
	printf("Model: ");
	for (std::size_t i = 0; i < vertSize; i++) {
		if (i % 3 == 0) {
			printf("\n");
		}
		printf("%f, ", vertices[i]);
	}
	printf("\n\n");
}
