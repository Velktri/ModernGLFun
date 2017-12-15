#include "Manager.h"
#include "Camera.h"
#include "Models/MeshAsset.h"
#include "Models/Shader.h"
#include "ModelData/Texture.h"
#include "ModelData/Mesh.h"
#include "ModelData/Element.h"
#include "ModelData/Curve.h"
#include "System/Material.h"
#include "Components/MeshComponent.h"
#include "Lights/Light.h"
#include <iostream>

/* Assimp */
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

Manager::Manager()
{
	BuildShaders();
	CurrentShader = DefaultShader;
	AssetMap[DefaultShader] = std::vector<Asset*>();

	DefaultTexture = reinterpret_cast<Texture*>(BuildTexture("Models/Textures/Default.png"));
	DefaultMaterial = reinterpret_cast<Material*>(BuildMaterial());

	BuildLights(); // @TEMP

	BuildTexture("Models/Textures/Albedo.png");
	BuildTexture("Models/Textures/Normal.png");
	BuildTexture("Models/Textures/Metallic.png");
	BuildTexture("Models/Textures/Roughness.png");
}


Manager::~Manager()
{
	/* Shaders */
	for each (Shader* s in UserShaderList)   {  s->~Shader();  }
	for each (Shader* s in SystemShaderList) {  s->~Shader();  }

	/* Assets */
	for each (Asset* mod in AssetList)  {  mod->~Asset();  }

	/* Textures */
	for each (Texture* t in TextureList)  {  t->~Texture();  }

	/* Lights */
	for each (Light* l in LightsList)  {  l->~Light();  }
}

void Manager::BuildShaders()
{
	SystemShader = new Shader("Shaders/System.vert", "Shaders/System.frag");
	PickerShader = new Shader("Shaders/Picker.vert", "Shaders/Picker.frag");
	LightShader = new Shader("Shaders/Lamp.vert", "Shaders/Lamp.frag");
	DefaultShader = new Shader("Shaders/PBR.vert", "Shaders/PBR.frag");

	SystemShaderList.push_back(SystemShader);
	SystemShaderList.push_back(PickerShader);
	SystemShaderList.push_back(LightShader);

	UserShaderList.push_back(DefaultShader);
}

void Manager::SetSystemShader(glm::mat4 InViewProjection)
{
	SystemShader->Use();
	glUniformMatrix4fv(SystemShader->ShaderList["ViewProjection"], 1, GL_FALSE, glm::value_ptr(InViewProjection));
}

Asset* Manager::BuildAsset(std::string path)
{
	Asset* SpawnedAsset = NULL;
	if (path == "")
	{
		SpawnedAsset = new Asset(AssetList.size(), this);
		SpawnedAsset->Name = std::string("Asset_" + AssetList.size());
	}
	else
	{
		SpawnedAsset = new MeshAsset(AssetList.size(), this, path);
		SpawnedAsset->Name = std::string("MeshAsset_" + AssetList.size());
	}

	AddAssetToPool(SpawnedAsset);

	return SpawnedAsset;
}

void Manager::BuildPrimative(Primatives InType)
{
	Asset* SpawnedAsset = NULL;
	std::string AssetNum = std::to_string(AssetList.size());
	switch (InType)
	{
		case Primatives::Cube:
			SpawnedAsset = new MeshAsset(AssetList.size(), this, "Models/Primitives/cube.obj");
			SpawnedAsset->Name = std::string("Cube_" + AssetNum);
			break;
		case Primatives::Plane:
			SpawnedAsset = new MeshAsset(AssetList.size(), this, "Models/Primitives/plane.obj");
			SpawnedAsset->Name = std::string("Plane_" + AssetNum);
			break;
		case Primatives::Sphere:
			SpawnedAsset = new MeshAsset(AssetList.size(), this, "Models/Primitives/Sphere.obj");
			SpawnedAsset->Name = std::string("Sphere_" + AssetNum);
			break;
		case Primatives::Cylinder:
			SpawnedAsset = new MeshAsset(AssetList.size(), this, "Models/Primitives/cylinder.obj");
			SpawnedAsset->Name = std::string("Cylinder_" + AssetNum);
			break;
		case Primatives::ECurve:
			SpawnedAsset = new MeshAsset(AssetList.size(), this);
			dynamic_cast<MeshComponent*>(SpawnedAsset->GetRoot()->GetComponents()[0])->SetMesh(new Curve());
			SpawnedAsset->Name = std::string("Curve_" + AssetNum);
			break;
		case Primatives::Smooth:
			SpawnedAsset = new MeshAsset(AssetList.size(), this, "Models/Primitives/smoothSphere.obj");
			SpawnedAsset->Name = std::string("SmoothSphere_" + AssetNum); // @TODO: change along with enum in future.
			break;
		default:
			SpawnedAsset = new MeshAsset(AssetList.size(), this);
			SpawnedAsset->Name = std::string("Cube_" + AssetNum);
			break;
	}

	AddAssetToPool(SpawnedAsset);
}

void Manager::ShadeLights(glm::mat4 InViewProjection, Shader* LightShader)
{
	LightShader->Use();
	glUniformMatrix4fv(LightShader->ShaderList["ViewProjection"], 1, GL_FALSE, glm::value_ptr(InViewProjection));
}

void Manager::Draw(Shader* shader)
{
	for each (Light* li in LightsList)
	{
		glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(li->GetOrientation()));
		li->Draw();
	}
}

void Manager::CheckForSelection(int InID)
{
	SelectedAsset = NULL;
	for (Asset* a : AssetList)
	{
		if (a->GetAssetID() == InID) { SelectedAsset = a; }
	}
}

void Manager::AddAssetToPool(Asset* InAsset)
{
	if (InAsset)
	{
		AssetMap[DefaultShader].push_back(InAsset);
		AssetList.push_back(InAsset);
	}
}

Resource* Manager::BuildMaterial()
{
	Material* NewMaterial = new Material(this, DefaultShader);
	if (NewMaterial) { MaterialList.push_back(NewMaterial); }
	return NewMaterial;
}

Resource* Manager::BuildTexture(std::string path)
{
	Texture* NewTexture = new Texture(path);
	if (NewTexture) { TextureList.push_back(NewTexture); }
	return NewTexture;
}

void Manager::BuildLights()
{
	LightsList.push_back(new Light(glm::vec3(-5.0f, 5.0f, 0.0f)));
}

Resource* Manager::ProcessMesh(std::string path) // @TODO: design system for multi-mesh imports (FBX system too)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	bool bHasTextureCoords = false;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return NULL;
	}
	std::string directory = path.substr(0, path.find_last_of('\\'));
	aiNode* node = scene->mRootNode;
	aiMesh* mesh = scene->mMeshes[0];

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			bHasTextureCoords = true;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//if (mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//}

	return new Mesh(vertices, indices, bHasTextureCoords);
}

//std::vector<Texture> Manager::ProcessTexture(aiMaterial* mat, aiTextureType type, std::string typeName)
//{
//	std::vector<Texture> textures;
//	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
//	{
//		aiString str;
//		mat->GetTexture(type, i, &str);
//		Texture tex = Texture(directory + '\\' + str.C_Str());
//		tex.SetType(typeName);
//		textures.push_back(tex);
//	}
//	return textures;
//}





std::vector<Element*> Manager::GetMeshList() { return MeshList; }
Shader* Manager::GetSystemShader() { return SystemShader; }
Shader* Manager::GetLightShader() { return LightShader; }
Shader* Manager::GetDefaultShader() { return DefaultShader; }
Shader* Manager::GetCurrentShader() { return CurrentShader; }
Texture* Manager::GetDefaultTexture() { return DefaultTexture; }
Material* Manager::GetDefaultMaterial() { return DefaultMaterial; }
std::vector<Shader*> Manager::GetUserShaderList() { return UserShaderList; }
Asset* Manager::GetSelectedAsset() { return SelectedAsset; }
std::vector<Asset*> Manager::GetAssets() { return AssetList; }
int Manager::GetAssetListSize() { return AssetList.size(); }
std::vector<Asset*> Manager::GetAssetsFromMap(Shader* InShader) { return AssetMap[InShader]; }
std::vector<Light*> Manager::GetLights() { return LightsList; }
void Manager::SetCurrentShader(Shader* s) { CurrentShader = s; }
void Manager::SetPickerShader() { CurrentShader = PickerShader; }
void Manager::SetSelectedAsset(Asset* InAsset) { SelectedAsset = InAsset; }