#include "Manager.h"
#include "Camera.h"
#include "Models/MeshAsset.h"
#include "Models/Shader.h"
#include "ModelData/Texture.h"
#include "ModelData/Mesh.h"
#include "ModelData/Element.h"
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
	bIsWireFrame = false;
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

void Manager::ShadeAssets(Camera* WorldCamera, std::vector<Light*> Lights, Shader* InCurrentShader)
{
	if (InCurrentShader != CurrentShader)
	{
		CurrentShader = InCurrentShader;
		CurrentShader->Use();

		glUniformMatrix4fv(CurrentShader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetViewMatrix()));
		glUniformMatrix4fv(CurrentShader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetProjection()));
		glm::vec3 pos = WorldCamera->GetPosition();
		glUniform3f(CurrentShader->ShaderList["cameraPos"], pos.x, pos.y + 1, pos.z);
		//glm::vec3 pos = WorldCamera->GetPosition();
		//glUniform3f(AssetShader->ShaderList["viewPos"], pos.x, pos.y, pos.z);
		//glUniform3f(AssetShader->ShaderList["dirLight.position"], 0.0f, 0.0f, 0.0f);
		//glUniform3f(AssetShader->ShaderList["dirLight.ambient"], 0.05f, 0.05f, 0.05f);
		//glUniform3f(AssetShader->ShaderList["dirLight.diffuse"], 1.0f, 1.0f, 1.0f);
		//glUniform3f(AssetShader->ShaderList["dirLight.specular"], 0.5f, 0.5f, 0.5f);
		//glUniform1i(AssetShader->ShaderList["material.diffuse"], 0);
		//glUniform1i(AssetShader->ShaderList["material.specular"], 1);
		//glUniform1f(AssetShader->ShaderList["material.shininess"], 32.0f);

		//glUniform3f(AssetShader->ShaderList["pointLight.position"], Lights[0]->WorldPosition.x, Lights[0]->WorldPosition.y, Lights[0]->WorldPosition.z);
		//glUniform3f(AssetShader->ShaderList["pointLight.ambient"], 0.05f, 0.05f, 0.05f);
		//glUniform3f(AssetShader->ShaderList["pointLight.diffuse"], 0.8f, 0.8f, 0.8f);
		//glUniform3f(AssetShader->ShaderList["pointLight.specular"], 1.0f, 1.0f, 1.0f);
		//glUniform1f(AssetShader->ShaderList["pointLight.constant"], 1.0f);
		//glUniform1f(AssetShader->ShaderList["pointLight.linear"], 0.09);
		//glUniform1f(AssetShader->ShaderList["pointLight.quadratic"], 0.032);
	}
}

void Manager::BuildShaders()
{
	SceneShader = new Shader("assets/Shaders/Scene.vert", "assets/Shaders/Scene.frag");
	PickerShader = new Shader("assets/Shaders/Picker.vert", "assets/Shaders/Picker.frag");
	//AssetShader = new Shader("assets/Shaders/Lighting.vert", "assets/Shaders/Lighting.frag");
	LightShader = new Shader("assets/Shaders/Lamp.vert", "assets/Shaders/Lamp.frag");
	ScreenShader = new Shader("assets/Shaders/Screen.vert", "assets/Shaders/Screen.frag");
	DefaultShader = new Shader("assets/Shaders/Default.vert", "assets/Shaders/Default.frag");

	SystemShaderList.push_back(SceneShader);
	SystemShaderList.push_back(PickerShader);
	//ShaderList.push_back(AssetShader);
	SystemShaderList.push_back(LightShader);
	SystemShaderList.push_back(ScreenShader);
	UserShaderList.push_back(DefaultShader);
}

void Manager::DrawAssets(Camera* WorldCamera, Shader* Shader)
{
	for each (Asset* mod in AssetMap[Shader])
	{
		glUniformMatrix4fv(Shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(mod->GetWorldSpace()));
		mod->Render(Shader, WorldCamera);
	}
}

void Manager::SetSystemShader(Camera* WorldCamera)
{
	SceneShader->Use();
	glUniformMatrix4fv(SceneShader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetViewMatrix()));
	glUniformMatrix4fv(SceneShader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetProjection()));
}

void Manager::BuildAsset(std::string path)
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
}

void Manager::BuildPrimative(Primatives InType)
{
	Asset* SpawnedAsset = NULL;
	std::string AssetNum = std::to_string(AssetList.size());
	switch (InType)
	{
		case Cube:
			SpawnedAsset = new MeshAsset(AssetList.size(), this, "assets/Models/Primitives/cube.obj");
			SpawnedAsset->Name = std::string("Cube_" + AssetNum);
			break;
		case Plane:
			SpawnedAsset = new MeshAsset(AssetList.size(), this, "assets/Models/Primitives/plane.obj");
			SpawnedAsset->Name = std::string("Plane_" + AssetNum);
			break;
		case Sphere:
			SpawnedAsset = new MeshAsset(AssetList.size(), this, "assets/Models/Primitives/Sphere.obj");
			SpawnedAsset->Name = std::string("Sphere_" + AssetNum);
			break;
		case Cylinder:
			SpawnedAsset = new MeshAsset(AssetList.size(), this, "assets/Models/Primitives/cylinder.obj");
			SpawnedAsset->Name = std::string("Cylinder_" + AssetNum);
			break;
		case Curve:
			//SpawnedAsset = new MeshAsset(AssetList.size(), this, "assets/Models/Primitives/plane.obj");
			//SpawnedAsset->Name = std::string("Plane_" + AssetNum);
			break;
		case Smooth:
			SpawnedAsset = new MeshAsset(AssetList.size(), this, "assets/Models/Primitives/smoothSphere.obj");
			SpawnedAsset->Name = std::string("SmoothSphere_" + AssetNum); // @TODO: change along with enum in future.
			break;
		default:
			SpawnedAsset = new MeshAsset(AssetList.size(), this);
			SpawnedAsset->Name = std::string("Cube_" + AssetNum);
			break;
	}

	AddAssetToPool(SpawnedAsset);
}

void Manager::ShadeLights(Camera* WorldCamera, Shader* LightShader)
{
	LightShader->Use();
	glUniformMatrix4fv(LightShader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetViewMatrix()));
	glUniformMatrix4fv(LightShader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(WorldCamera->GetProjection()));
}

void Manager::Draw(Shader* shader)
{
	for each (Light* li in LightsList)
	{
		glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(li->GetOrientation()));
		li->Draw();
	}
}

// @TODO: rewrite
void Manager::CheckForSelection(int InID)
{
	bool bIsFound = false;
	for each (Asset* a in AssetList)
	{
		if (a->GetAssetID() == InID)
		{
			SelectedAsset = a;
			bIsFound = true;
		}
	}

	if (!bIsFound)
	{
		SelectedAsset = NULL;
	}


	if (SelectedAsset)
	{
		printf("%s\n", SelectedAsset->Name.c_str());
	}
	else
	{
		printf("Null Selection.\n");
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



void Manager::BuildTexture(std::string path)
{
	TextureList.push_back(new Texture(path));
}

void Manager::BuildLights()
{
	LightsList.push_back(new Light(glm::vec3(-3.0f, 2.0f, 0.0f)));
}

std::vector<Element*> Manager::GetMeshList() { return MeshList; }
Shader* Manager::GetSceneShader() { return SceneShader; }
Shader* Manager::GetAssetShader() { return AssetShader; }
Shader* Manager::GetLightShader() { return LightShader; }
Shader* Manager::GetScreenShader() { return ScreenShader; }
Shader* Manager::GetDefaultShader() { return DefaultShader; }
Shader* Manager::GetCurrentShader() { return CurrentShader; }
std::vector<Shader*> Manager::GetUserShaderList() { return UserShaderList; }
Asset* Manager::GetSelectedAsset() { return SelectedAsset; }
std::vector<Asset*> Manager::GetAssets() { return AssetList; }
std::vector<Light*> Manager::GetLights() { return LightsList; }
void Manager::SetCurrentShader(Shader* s) { CurrentShader = s; }
void Manager::SetPickerShader() { CurrentShader = PickerShader; }
void Manager::SetSelectedAsset(Asset* InAsset) { SelectedAsset = InAsset; }

Mesh* Manager::ProcessMesh(std::string path) // @TODO: design system for multi-mesh imports (FBX system too)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

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
			indices.push_back(face.mIndices[j]);
	}

	//if (mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//}

	Mesh* NewMesh = new Mesh(vertices, indices);
	if (NewMesh) { MeshPool.emplace(path, NewMesh); }
	return NewMesh;
}

//std::vector<Texture> Asset::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
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