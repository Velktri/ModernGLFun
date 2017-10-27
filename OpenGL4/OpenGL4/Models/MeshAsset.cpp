#include "MeshAsset.h"
#include "Components/MeshComponent.h"

MeshAsset::MeshAsset()
{
	MeshComponent* MeshComp = new MeshComponent();
	GetRoot()->AddComponent(MeshComp);
}


MeshAsset::~MeshAsset()
{
}
