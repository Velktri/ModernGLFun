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

	CoreShader->Use();
	glUniform1i(glGetUniformLocation(CoreShader->GetShader(), "albedoMap"), 1);
	glUniform1i(glGetUniformLocation(CoreShader->GetShader(), "normalMap"), 2);
	glUniform1i(glGetUniformLocation(CoreShader->GetShader(), "metallicMap"), 3);
	glUniform1i(glGetUniformLocation(CoreShader->GetShader(), "roughnessMap"), 4);
	glUniform1i(glGetUniformLocation(CoreShader->GetShader(), "aoMap"), 5);
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

	std::vector<Light*> LightsList = OwningManager->GetLights();
	for (int i = 0; i < LightsList.size(); i++)
	{
		glUniform3fv(glGetUniformLocation(CoreShader->GetShader(), "lightPositions"), 1, &LightsList[i]->WorldPosition[0]);
		glUniform3fv(glGetUniformLocation(CoreShader->GetShader(), "lightColors"), 1, &LightsList[i]->Color[0]);
	}
}

