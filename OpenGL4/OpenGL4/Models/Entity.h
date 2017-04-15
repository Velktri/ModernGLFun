#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Asset;
class Texture;

enum ShaderType {
	NONE = 0,
	GRID = 1,
	LINE = 2,
	CURVE = 3,
	GIZMO = 4,
	MESH = 5
};

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Entity {
public:
	Entity();
	~Entity();

	void Draw(class Shader* shader, class Camera* WorldCamera);
	std::string GetName();
	Asset* GetParentAsset();
	int GetType();

protected:
	void Init();

	GLuint VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	int type;
	std::string Name;
	Asset* ParentAsset;
};