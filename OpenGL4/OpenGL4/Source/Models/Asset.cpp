#include "Asset.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "System/Manager.h"
#include "Components/ComponentBase.h"
#include "System/Camera.h"
#include <iostream>


Asset::Asset(GLuint InAssetID, Manager* InManager)
{
	AssetID = InAssetID;
	MyManager = InManager;
	OriginPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	Root = new ComponentBase(this);
}

Asset::~Asset()
{
	Root->~ComponentBase();
}

void Asset::Render(Shader* shader)
{
	Root->Render(shader);
}

void Asset::TranslateAsset(float x, float y, float z)
{
	WorldSpaceOrientation = glm::translate(WorldSpaceOrientation, glm::vec3(x, y, z));
	OriginPoint = glm::vec3(OriginPoint.x + x, OriginPoint.y + y, OriginPoint.z + z);
}

void Asset::RotateAsset(float x, float y, float z)
{
	if (x > 0) {  WorldSpaceOrientation = glm::rotate(WorldSpaceOrientation, x, glm::vec3(1.0f, 0.0f, 0.0f));  }
	if (y > 0) {  WorldSpaceOrientation = glm::rotate(WorldSpaceOrientation, y, glm::vec3(0.0f, 1.0f, 0.0f));  }
	if (z > 0) {  WorldSpaceOrientation = glm::rotate(WorldSpaceOrientation, z, glm::vec3(0.0f, 0.0f, 1.0f));  }
}

void Asset::ScaleAsset(float x, float y, float z)
{
	WorldSpaceOrientation = glm::scale(WorldSpaceOrientation, glm::vec3(x, y, z));
}


/** Getters */
glm::vec3 Asset::GetOrigin() { return OriginPoint; }
GLuint Asset::GetAssetID() { return AssetID; }
glm::mat4 Asset::GetWorldSpace() { return WorldSpaceOrientation; }
ComponentBase* Asset::GetRoot() { return Root; }
Manager* Asset::GetManager() { return MyManager; }
