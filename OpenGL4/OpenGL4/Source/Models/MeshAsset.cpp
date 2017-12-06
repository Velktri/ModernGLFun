#include "MeshAsset.h"
#include "Components/MeshComponent.h"

MeshAsset::MeshAsset(GLuint InAssetID, Manager* InManager, std::string path) : Asset(InAssetID, InManager)
{
	GetRoot()->AddComponent(new MeshComponent(this, path));
}


MeshAsset::~MeshAsset()
{
}
