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

	testColor = glm::vec3(0.1568f, 0.2745f, 0.3137f);
	testRoughness = 1.0f;
	testMetallic = 0.8f;
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

void Asset::SetWorldSpace(glm::mat4 InWorldSpace) 
{ 
	AssetTransform->WorldSpaceOrientation = InWorldSpace;
	for (ComponentBase* comp : Root->GetComponents())
	{
		comp->UpdateTransforms(InWorldSpace);
	}
}


/** Getters */
glm::vec3 Asset::GetOrigin() { return AssetTransform->OriginPoint; }
GLuint Asset::GetAssetID() { return AssetID; }
glm::mat4 Asset::GetWorldSpace() { return AssetTransform->WorldSpaceOrientation; }
ComponentBase* Asset::GetRoot() { return Root; }
Manager* Asset::GetManager() { return MyManager; }
