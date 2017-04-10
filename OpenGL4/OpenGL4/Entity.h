#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

enum ShaderType {
	NONE = 0,
	GRID = 1,
	LINE = 2,
	CURVE = 3
};

class Entity {
public:
	Entity();
	~Entity();

	void Draw(class Shader* shader, class Camera* WorldCamera);

protected:
	void Init();

	GLuint VAO, VBO, EBO;

	std::vector<GLfloat> vertices;
	int vertSize;
	int type;
};