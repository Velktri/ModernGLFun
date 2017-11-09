#pragma once
#include <GL/glew.h>
#include <vector>
#include "Element.h"

class Asset;
class Shader;

class Mesh : public Element {
public:
	Mesh(std::vector<Vertex> InVertices, std::vector<GLuint> InIndices);
	~Mesh();
};

