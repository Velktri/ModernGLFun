#include "MeshComponent.h"
#include "ComponentBase.h"
#include "ModelData/Mesh.h"
#include "Models/Asset.h"
#include "System/Manager.h"
#include "Models/Shader.h"
#include "System/Material.h"
#include "System/Transforms.h"

MeshComponent::MeshComponent(Asset* InParent, std::string path) : ComponentBase(InParent)
{
	MeshData = NULL;
	MeshMaterial = GetParentAsset()->GetManager()->GetDefaultMaterial();
	InitAndSet(path);
}


MeshComponent::~MeshComponent()
{

}

void MeshComponent::Render(Shader* InShader)
{
	if (MeshData && GetParentAsset()->Name != "MeshAsset_") //@TODO: add toggle visibility for assets
	{
		glUniform3f(InShader->ShaderList["color"], Color.x, Color.y, Color.z);
		glUniformMatrix4fv(InShader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(ComponentTransform->WorldSpaceOrientation));
		MeshMaterial->ShadeMesh(this, MeshData->HasTextureCoords());
		MeshData->Render(InShader);
	}

	ComponentBase::Render(InShader);
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

void MeshComponent::SetMesh(Element* InElement) { MeshData = InElement; }