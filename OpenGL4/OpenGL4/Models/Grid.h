#pragma once
#include "../Models/Shader.h"
#include "../Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Grid {
public:
	Grid(int RowRadius, int ColumnRadius, float spacing);
	~Grid();

	void Draw(Shader* shader, Camera* worldCamera);

private:
	GLuint VAO;
	GLuint VBO;
	std::vector<GLfloat> vertices;
	int vertSize;
	glm::mat4 orientation;
};