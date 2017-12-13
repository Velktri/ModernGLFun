#include "Mesh.h"
#include "../Models/Asset.h"

Mesh::Mesh(std::vector<Vertex> InVertices, std::vector<GLuint> InIndices, bool InHasTextureCoords)
{
	vertices = InVertices;
	indices = InIndices;
	BhasTexCoords = InHasTextureCoords;
	type = MESH;

	Init();
}

Mesh::~Mesh()
{
}