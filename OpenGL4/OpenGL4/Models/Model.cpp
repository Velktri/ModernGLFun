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
		   // Positions              // Colors       
		 0.0f,  0.0f,  0.0f,     1.0f,  0.0f,  0.0f,   
		 0.0f,  0.0f,  1.0f,     0.0f,  1.0f,  0.0f,   
		 0.0f,  1.0f,  0.0f,     0.0f,  0.0f,  1.0f,   
		 0.0f,  1.0f,  1.0f,     1.0f,  1.0f,  0.0f,   
							   						   
		 1.0f,  0.0f,  0.0f,     1.0f,  0.0f,  0.0f,   
		 1.0f,  0.0f,  1.0f,     0.0f,  1.0f,  0.0f,   
		 1.0f,  1.0f,  0.0f,     0.0f,  0.0f,  1.0f,   
		 1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  0.0f, 
	};

	GLuint indices[] = {
		0, 1, 5,  
		0, 4, 5,  
		0, 2, 6,  
		0, 4, 6,
		0, 2, 3,
		0, 1, 3,
		4, 5, 7,
		4, 6, 7,
		5, 1, 3,
		5, 7, 3,
		2, 3, 7,
		2, 6, 7,
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

void Model::GenerateArrayModel() {
	GLfloat vertArr[] = {
		// Positions                // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
							     					     
		-0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
							     					     
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
							     					     
		 0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
							     					     
		-0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
							     					     
		-0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	vertSize = sizeof(vertArr) / sizeof(vertArr[0]);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertArr), vertArr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	vertices = std::vector<GLfloat>(vertArr, vertArr + sizeof(vertArr) / sizeof(vertArr[0]));
}

void Model::TranslateModel(float x, float y, float z) {
	orientation = glm::translate(orientation, glm::vec3(x, y, z));
}

void Model::RotateModel() {
	orientation = glm::rotate(orientation, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Model::Draw() {

	if (diffuseTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	}

	if (specularTexture != 0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexture);
	}

	if (EBO != 0) {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	} else {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertSize);
	}
	glBindVertexArray(0);
}

void Model::AttachDiffuseTexture(Texture* inTexture) {
	diffuseTexture = inTexture->GetTexture();
}

void Model::AttachSpecularTexture(Texture* inTexture) {
	specularTexture = inTexture->GetTexture();
}
