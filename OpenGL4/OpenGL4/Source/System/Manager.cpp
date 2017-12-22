#include "Manager.h"
#include "Camera.h"
#include "Models/MeshAsset.h"
#include "Models/Shader.h"
#include "ModelData/Texture.h"
#include "ModelData/Mesh.h"
#include "ModelData/Element.h"
#include "ModelData/Curve.h"
#include "ModelData/Line.h"
#include "System/Material.h"
#include "Components/MeshComponent.h"
#include "Models\Gizmo.h"
#include "ModelData\Grid.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Lights/SunLight.h"
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


	GridFloor = new Grid(GRIDRADIUS_X, GRIDRADIUS_Y, GRIDSPACING);
	SystemElements.push_back(GridFloor);


	DefaultTexture = reinterpret_cast<Texture*>(BuildTexture("Models/Textures/Default.png"));
	DefaultLightTexture = reinterpret_cast<Texture*>(BuildTexture("Models/Textures/Light.png"));
	DefaultMaterial = reinterpret_cast<Material*>(BuildMaterial());

	/* ------------------- TEMP -------------------------*/
	BuildLight(LampType::POINTLIGHT, glm::vec3(5.0f, 5.0f, 0.0f));
	BuildLight(LampType::POINTLIGHT, glm::vec3(-5.0f, 1.0f, -2.0f));
	BuildLight(LampType::POINTLIGHT, glm::vec3(2.0f, 5.0f, 4.0f));

	LightsList[0]->Color = glm::vec3(200, 0, 0);
	LightsList[1]->Color = glm::vec3(0, 200, 0);
	LightsList[2]->Color = glm::vec3(0, 0, 200);


	BuildTexture("Models/Textures/Albedo.png");
	BuildTexture("Models/Textures/Normal.png");
	BuildTexture("Models/Textures/Metallic.png");
	BuildTexture("Models/Textures/Roughness.png");
	/* ---------------------------------------------------*/

	SelectionGizmo = new Gizmo(GetAssetListSize(), this);
	Asset* NewAsset = BuildPrimative(Primatives::Cylinder);
	NewAsset->TranslateAsset(glm::vec3(5.0f, 0.0f, 0.0f));
}


Manager::~Manager()
{
	/* Shaders */
	for each (Shader* s in UserShaderList)   {  s->~Shader();  }
	for each (Shader* s in SystemShaderList) {  s->~Shader();  }

	/* Assets */
	for each (Asset* mod in UserAssetList)  {  mod->~Asset();  }

	/* Textures */
	for each (Texture* t in TextureList)  {  t->~Texture();  }

	/* Lights */
	for each (Light* l in LightsList)  {  l->~Light();  }

	/* System */
	for (Element* e : SystemElements)
	{
		if (e) { e->~Element(); }
	}
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

	LightShader->Use();
	glUniform1i(glGetUniformLocation(LightShader->GetShader(), "LampTexture"), 1);

	DefaultShader->Use();
	glUniform1i(glGetUniformLocation(DefaultShader->GetShader(), "albedoMap"), 1);
	glUniform1i(glGetUniformLocation(DefaultShader->GetShader(), "normalMap"), 2);
	glUniform1i(glGetUniformLocation(DefaultShader->GetShader(), "metallicMap"), 3);
	glUniform1i(glGetUniformLocation(DefaultShader->GetShader(), "roughnessMap"), 4);
	glUniform1i(glGetUniformLocation(DefaultShader->GetShader(), "aoMap"), 5);
}

void Manager::SetSystemShader(glm::mat4 InViewProjection)
{
	SystemShader->Use();
	glUniformMatrix4fv(SystemShader->ShaderList["ViewProjection"], 1, GL_FALSE, glm::value_ptr(InViewProjection));
}

Asset* Manager::BuildAsset(std::string path, bool bIsUserAsset)
{
	Asset* SpawnedAsset = NULL;
	if (path == "")
	{
		SpawnedAsset = new Asset(UserAssetList.size(), this);
		SpawnedAsset->Name = std::string("Asset_" + UserAssetList.size());
	}
	else
	{
		SpawnedAsset = new MeshAsset(UserAssetList.size(), this, path);
		SpawnedAsset->Name = std::string("MeshAsset_" + UserAssetList.size());
	}

	if (bIsUserAsset) { AddAssetToPool(SpawnedAsset); }

	return SpawnedAsset;
}

Asset* Manager::BuildPrimative(Primatives InType)
{
	Asset* SpawnedAsset = NULL;
	std::string AssetNum = std::to_string(UserAssetList.size());
	switch (InType)
	{
		case Primatives::Cube:
			SpawnedAsset = new MeshAsset(UserAssetList.size(), this, "Models/Primitives/cube.obj");
			SpawnedAsset->Name = std::string("Cube_" + AssetNum);
			break;
		case Primatives::Plane:
			SpawnedAsset = new MeshAsset(UserAssetList.size(), this, "Models/Primitives/plane.obj");
			SpawnedAsset->Name = std::string("Plane_" + AssetNum);
			break;
		case Primatives::Sphere:
			SpawnedAsset = new MeshAsset(UserAssetList.size(), this, "Models/Primitives/Sphere.obj");
			SpawnedAsset->Name = std::string("Sphere_" + AssetNum);
			break;
		case Primatives::Cylinder:
			SpawnedAsset = new MeshAsset(UserAssetList.size(), this, "Models/Primitives/cylinder.obj");
			SpawnedAsset->Name = std::string("Cylinder_" + AssetNum);
			break;
		case Primatives::ECurve:
			SpawnedAsset = new MeshAsset(UserAssetList.size(), this);
			dynamic_cast<MeshComponent*>(SpawnedAsset->GetRoot()->GetComponents()[0])->SetMesh(new Curve());
			SpawnedAsset->Name = std::string("Curve_" + AssetNum);
			break;
		case Primatives::Smooth:
			SpawnedAsset = new MeshAsset(UserAssetList.size(), this, "Models/Primitives/smoothSphere.obj");
			SpawnedAsset->Name = std::string("SmoothSphere_" + AssetNum); // @TODO: change along with enum in future.
			break;
		default:
			SpawnedAsset = new MeshAsset(UserAssetList.size(), this);
			SpawnedAsset->Name = std::string("Mesh_" + AssetNum);
			break;
	}

	AddAssetToPool(SpawnedAsset);
	return SpawnedAsset;
}

void Manager::DrawLights(glm::mat4 InView, glm::mat4 InProjection) // @TODO: consider moving to world class.
{
	LightShader->Use();
	glUniformMatrix4fv(LightShader->ShaderList["Projection"], 1, GL_FALSE, glm::value_ptr(InProjection));
	glUniformMatrix4fv(LightShader->ShaderList["View"], 1, GL_FALSE, glm::value_ptr(InView));

	for each (Light* light in LightsList)
	{
		glUniformMatrix4fv(LightShader->ShaderList["Model"], 1, GL_FALSE, glm::value_ptr(light->GetOrientation()));
		glUniform3f(LightShader->ShaderList["LampColor"], light->Color.x / 255, light->Color.y / 255, light->Color.z / 255);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, light->GetLightTexture()->GetTexture());
		light->Draw();
	}
}

void Manager::CheckForSelection(int InID)
{
	printf("InID = %d\n", InID);
	if (SelectedAsset && InID >= UserAssetList.size() && InID != 0x00FFFFFF)
	{
		printf("Selected Gizmo.\n");

		if (InID == UserAssetList.size())
		{
			SelectionGizmo->ActiveAxis = ActiveHandle::Translate_X;
		}
		else if (InID == UserAssetList.size() + 1)
		{
			SelectionGizmo->ActiveAxis = ActiveHandle::Translate_Y;
		}
		else if (InID == UserAssetList.size() + 2)
		{
			SelectionGizmo->ActiveAxis = ActiveHandle::Translate_Z;
		}
	}
	else
	{
		SelectedAsset = NULL;
		for (Asset* a : UserAssetList)
		{
			if (a->GetAssetID() == InID) { UpdateSelectedAsset(a); }
		}
	}
}

void Manager::AddAssetToPool(Asset* InAsset)
{
	if (InAsset)
	{
		AssetMap[DefaultShader].push_back(InAsset);
		UserAssetList.push_back(InAsset);
	}
}

Element* Manager::DrawLine(glm::vec3 StartPoint, glm::vec3 EndPoint)
{
	Element* NewLine = new Line(StartPoint, EndPoint);
	SystemElements.push_back(NewLine);
	return NewLine;
}

Element* Manager::DrawLine(glm::vec3 StartPoint, glm::vec3 LineDirection, float Distance)
{
	Element* NewLine = new Line(StartPoint, StartPoint + (LineDirection * (-1 * Distance)));
	SystemElements.push_back(NewLine);
	return NewLine;
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

Light* Manager::BuildLight(LampType InType, glm::vec3 InPosition)
{
	Light* NewLight = NULL;
	switch (InType)
	{
		case LampType::POINTLIGHT:
			NewLight = new PointLight(DefaultLightTexture, InPosition);
			break;
		case LampType::SPOTLIGHT:
			NewLight = new SpotLight(DefaultTexture, InPosition);
			break;
		case LampType::SUNLIGHT:
			NewLight = new SunLight(DefaultTexture, InPosition);
			break;
		default:
			break;
	}

	LightsList.push_back(NewLight);
	return NewLight;
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



void Manager::ClearLines()
{
	for each (Element* e in SystemElements)
	{
		if (e->GetType() == ShaderType::LINE)
		{
			e->~Element();
		}
	}
}


void Manager::UpdateSelectedAsset(Asset* InAsset)
{
	SelectedAsset = InAsset;
	if (SelectedAsset)
	{
		SelectionGizmo->AttachAsset(SelectedAsset);
	}
}

std::vector<Texture*> Manager::GetTextures() { return TextureList; }
std::vector<Element*> Manager::GetMeshList() { return MeshList; }
Shader* Manager::GetSystemShader() { return SystemShader; }
Shader* Manager::GetLightShader() { return LightShader; }
Shader* Manager::GetDefaultShader() { return DefaultShader; }
Shader* Manager::GetCurrentShader() { return CurrentShader; }
Texture* Manager::GetDefaultTexture() { return DefaultTexture; }
Material* Manager::GetDefaultMaterial() { return DefaultMaterial; }
std::vector<Shader*> Manager::GetUserShaderList() { return UserShaderList; }
Asset* Manager::GetSelectedAsset() { return SelectedAsset; }
std::vector<Asset*> Manager::GetUserAssets() { return UserAssetList; }
int Manager::GetAssetListSize() { return UserAssetList.size(); }
std::vector<Asset*> Manager::GetAssetsFromMap(Shader* InShader) { return AssetMap[InShader]; }
std::vector<Light*> Manager::GetLights() { return LightsList; }
void Manager::SetCurrentShader(Shader* s) { CurrentShader = s; }
void Manager::SetPickerShader() { CurrentShader = PickerShader; }
Gizmo* Manager::GetGizmo() { return SelectionGizmo; }
std::vector<Element*> Manager::GetSystemElements() { return SystemElements; }