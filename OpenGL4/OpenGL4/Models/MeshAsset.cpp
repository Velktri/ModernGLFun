#include "MeshAsset.h"
#include "Components/MeshComponent.h"

MeshAsset::MeshAsset(GLuint InAssetID, Manager* InManager, std::string path) : Asset(InAssetID, InManager)
{
	MeshComponent* MeshComp = new MeshComponent(this, path);
	GetRoot()->AddComponent(MeshComp);
}


MeshAsset::~MeshAsset()
{
}
