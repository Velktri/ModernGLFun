#pragma once
#include <GL/glew.h>
#include <vector>
#include "Element.h"

class Mesh : public Element 
{
public:
	Mesh(std::vector<Vertex> InVertices, std::vector<GLuint> InIndices, bool InHasTextureCoords = false);
	~Mesh();
};