#pragma once
#include "ModelData/Resource.h"
#include <GL/glew.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader;
class Manager;
class Texture;
class MeshComponent;

enum class MaterialTextures
{
	EAlbedo,
	ENormal,
	EMetallic,
	ERoughness,
	EAO
};

class Material : public Resource
{
public:
	Material(Manager* InManager, Shader* InShader);
	~Material();

	void ShadeMesh(MeshComponent* OwningComponent, bool bHasUVs);
	void ApplyTexture(MaterialTextures InType, Texture* InTexture);

	/* Texture Inputs */
	Texture* Albedo;
	Texture* Normal;
	Texture* Metallic;
	Texture* Roughness;
	Texture* AO;

private:
	//std::vector<GLuint> MaterialTextures;
	Shader* CoreShader;
	Manager* OwningManager;
};