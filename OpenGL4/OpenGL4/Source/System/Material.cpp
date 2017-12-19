#include "Material.h"
#include "Models/Shader.h"
#include "System/Manager.h"
#include "Lights/Light.h"
#include "ModelData/Texture.h"
#include "Components/MeshComponent.h"
#include "System/Manager.h"

Material::Material(Manager* InManager, Shader* InShader)
{
	CoreShader = InShader;
	OwningManager = InManager;

	Albedo = NULL;
	Normal = NULL;
	Metallic = NULL;
	Roughness =	NULL;
	AO = NULL;
}

Material::~Material()
{

}

void Material::ShadeMesh(MeshComponent* OwningComponent, bool bHasUVs)
{
	Texture* DefaultTexture = OwningManager->GetDefaultTexture();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, (Albedo && bHasUVs) ? Albedo->GetTexture() : DefaultTexture->GetTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, (Normal && bHasUVs) ? Normal->GetTexture() : DefaultTexture->GetTexture());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, (Metallic && bHasUVs) ? Metallic->GetTexture() : DefaultTexture->GetTexture());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, (Roughness && bHasUVs) ? Roughness->GetTexture() : DefaultTexture->GetTexture());
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, (AO && bHasUVs) ? AO->GetTexture() : DefaultTexture->GetTexture());

	Asset* ParentAsset = OwningComponent->GetParentAsset();
	glUniform3f(CoreShader->ShaderList["colorTest"], ParentAsset->testColor.x, ParentAsset->testColor.y, ParentAsset->testColor.z);
	glUniform1f(CoreShader->ShaderList["roughnessTest"], ParentAsset->testRoughness);
	glUniform1f(CoreShader->ShaderList["metallicTest"], ParentAsset->testMetallic);
}

void Material::ApplyTexture(MaterialTextures InType, Texture* InTexture)
{
	switch (InType)
	{
		case MaterialTextures::EAlbedo:
			Albedo = InTexture;
			break;
		case MaterialTextures::ENormal:
			Normal = InTexture;
			break;
		case MaterialTextures::EMetallic:
			Metallic = InTexture;
			break;
		case MaterialTextures::ERoughness:
			Roughness = InTexture;
			break;
		case MaterialTextures::EAO:
			AO = InTexture;
			break;
		default:
			InTexture = NULL;
			break;
	}
}
