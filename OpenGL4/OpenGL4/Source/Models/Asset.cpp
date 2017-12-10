#include "Asset.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "System/Manager.h"
#include "Components/ComponentBase.h"
#include "System/Camera.h"
#include "System/Transforms.h"
#include <iostream>


Asset::Asset(GLuint InAssetID, Manager* InManager)
{
	AssetID = InAssetID;
	MyManager = InManager;
	AssetTransform = new Transforms();
	Root = new ComponentBase(this);
}

Asset::~Asset()
{
	Root->~ComponentBase();
	AssetTransform->~Transforms();
}

void Asset::Render(Shader* shader)
{
	Root->Render(shader);
}

void Asset::TranslateAsset(float x, float y, float z)
{
	AssetTransform->Translate(x, y, z);
}

void Asset::RotateAsset(float x, float y, float z)
{
	AssetTransform->Rotate(x, y, z);
}

void Asset::ScaleAsset(float x, float y, float z)
{
	AssetTransform->Scale(x, y, z);
}


/** Getters */
glm::vec3 Asset::GetOrigin() { return AssetTransform->OriginPoint; }
GLuint Asset::GetAssetID() { return AssetID; }
glm::mat4 Asset::GetWorldSpace() { return AssetTransform->WorldSpaceOrientation; }
void Asset::SetWorldSpace(glm::mat4 InWorldSpace) { AssetTransform->WorldSpaceOrientation = InWorldSpace; }
ComponentBase* Asset::GetRoot() { return Root; }
Manager* Asset::GetManager() { return MyManager; }
