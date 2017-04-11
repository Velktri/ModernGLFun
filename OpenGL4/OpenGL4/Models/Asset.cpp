#include "Asset.h"
#include "../Models/Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include <iostream>

Asset::Asset() {
	OriginPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	StaticMesh= NULL;
	TextureMap = NULL;
}

Asset::Asset(std::string path) {
	OriginPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	TextureMap = NULL;
	loadModel(path);
	(meshes[0]) ? StaticMesh = meshes[0] : StaticMesh = NULL;
	
}

Asset::~Asset() {

}

void Asset::Draw(Shader* shader) {
	GLuint i = 0;
	for (i; i < meshes.size(); i++) {
		meshes[i]->Draw(shader);
	}

	if (i == 0) {
		StaticMesh->Draw(shader);
	}
}


Mesh* Asset::GetMesh() {
	return StaticMesh;
}

void Asset::SetMesh(Mesh* InMesh) {
	StaticMesh = InMesh;
	meshes.push_back(InMesh);
}

Texture* Asset::GetTexture() {
	return TextureMap;
}

void Asset::TranslateAsset(float x, float y, float z) {
	orientation = glm::translate(orientation, glm::vec3(x, y, z));
	OriginPoint = glm::vec3(OriginPoint.x + x, OriginPoint.y + y, OriginPoint.z + z);
}

void Asset::RotateAsset(float x, float y, float z) {
	if (x > 0) {
		orientation = glm::rotate(orientation, x, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if (y > 0) {
		orientation = glm::rotate(orientation, y, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (z > 0) {
		orientation = glm::rotate(orientation, z, glm::vec3(0.0f, 0.0f, 1.0f));
	}
}

void Asset::ScaleAsset(float x, float y, float z) {
	orientation = glm::scale(orientation, glm::vec3(x, y, z));
}


glm::vec3 Asset::GetOrigin() {
	return OriginPoint;
}

std::vector<Mesh*> Asset::GetMeshes() {
	return meshes;
}

void Asset::loadModel(std::string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('\\'));
	//Name = "Object";
	processNode(scene->mRootNode, scene);
}

void Asset::processNode(aiNode* node, const aiScene* scene) {
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Asset::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0]) {
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		} else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return new Mesh(vertices, indices, textures, this);
}

std::vector<Texture> Asset::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture tex = Texture(directory + '\\' + str.C_Str());
		tex.SetType(typeName);
		textures.push_back(tex);
	}
	return textures;
}
