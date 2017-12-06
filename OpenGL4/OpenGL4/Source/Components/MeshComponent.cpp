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

void MeshComponent::Render(Shader* shader)
{
	if (MeshData)
	{
		MeshData->Render(shader);
	}

	ComponentBase::Render(shader);
}

void MeshComponent::SetMesh(Element* InElement)
{
	if (InElement)
	{
		//Manager* MyManager = GetParentAsset()->GetManager();
		//if (MyManager)
		//{
		//	MeshData = GetParentAsset()->GetManager()->CheckPool<Element>(path);
		//	if (!MeshData)
		//	{
		//		MeshData = MyManager->SpawnToPool<Element>(path);
		//	}
		//}
		MeshData = InElement;
	}
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
