#include "MeshComponent.h"
#include "ComponentBase.h"
#include "ModelData/Mesh.h"
#include "Models/Asset.h"
#include "System/Manager.h"

MeshComponent::MeshComponent(Asset* InParent, std::string path) : ComponentBase(InParent)
{
	MeshData = NULL;
	InitAndSet(path);
}


MeshComponent::~MeshComponent()
{
}

void MeshComponent::Render(Shader* shader, Camera* WorldCamera)
{
	if (MeshData)
	{
		MeshData->Render(shader, WorldCamera);
	}

	ComponentBase::Render(shader, WorldCamera);
}

void MeshComponent::InitAndSet(std::string path)
{
	if (path != "" && GetParentAsset())
	{
		Manager* MyManager = GetParentAsset()->GetManager();
		if (MyManager)
		{
			MeshData = GetParentAsset()->GetManager()->CheckPool<Mesh>(path);
			if (!MeshData)
			{
				MeshData = MyManager->SpawnToPool<Mesh>(path);
			}
		}
	}
}
