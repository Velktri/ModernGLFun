#pragma once
#include <GL/glew.h>
#include <vector>
#include "Entity.h"

class Asset;
class Shader;

class Mesh : public Entity {
public:
	Mesh(std::vector<Vertex> InVertices, std::vector<GLuint> InIndices, Asset* InAsset);
	~Mesh();
};

