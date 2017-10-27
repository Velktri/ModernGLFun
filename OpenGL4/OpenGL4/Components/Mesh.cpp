#include "Mesh.h"
#include "../Models/Asset.h"

Mesh::Mesh(std::vector<Vertex> InVertices, std::vector<GLuint> InIndices, Asset* InAsset) {
	vertices = InVertices;
	indices = InIndices;
	ParentAsset = InAsset;
	type = MESH;

	Init();

}

Mesh::~Mesh() {
}