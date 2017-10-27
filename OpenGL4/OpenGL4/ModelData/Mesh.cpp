#include "Mesh.h"
#include "../Models/Asset.h"

Mesh::Mesh(std::vector<Vertex> InVertices, std::vector<GLuint> InIndices)
{
	vertices = InVertices;
	indices = InIndices;
	type = MESH;

	Init();
}

Mesh::~Mesh()
{
}