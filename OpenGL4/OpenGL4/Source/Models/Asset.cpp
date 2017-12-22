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

void Asset::TranslateAsset(glm::vec3 InVector)
{
	AssetTransform->Translate(InVector);
	UpdateComponentOrientation();
}

void Asset::RotateAsset(glm::vec3 InVector)
{
	AssetTransform->Rotate(InVector);
	UpdateComponentOrientation();
}

void Asset::ScaleAsset(glm::vec3 InVector)
{
	AssetTransform->Scale(InVector);
	UpdateComponentOrientation();
}

void Asset::SetWorldSpace(glm::mat4 InWorldSpace) 
{ 
	AssetTransform->WorldSpaceOrientation = InWorldSpace;
	UpdateComponentOrientation();
}

void Asset::UpdateComponentOrientation()
{
	for (ComponentBase* comp : Root->GetComponents())
	{
		comp->UpdateTransforms(AssetTransform->WorldSpaceOrientation);
	}
}


/** Getters */
glm::vec3 Asset::GetOrigin() { return AssetTransform->GetPosition(); }
GLuint Asset::GetAssetID() { return AssetID; }
glm::mat4 Asset::GetWorldSpace() { return AssetTransform->WorldSpaceOrientation; }
ComponentBase* Asset::GetRoot() { return Root; }
Manager* Asset::GetManager() { return MyManager; }
Transforms* Asset::GetAssetTransforms() { return AssetTransform; }