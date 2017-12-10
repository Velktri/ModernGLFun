#include "Gizmo.h"
#include "Components/MeshComponent.h"

Gizmo::Gizmo(GLuint InAssetID, Manager* InManager) : Asset(InAssetID, InManager)
{
	TranslateX = new MeshComponent(this, "Models/Primitives/TranslationGizmo.obj");
	TranslateX->RotateComponent(90.0f, 0.0f, 0.0f);

	TranslateY = new MeshComponent(this, "Models/Primitives/TranslationGizmo.obj");

	TranslateZ = new MeshComponent(this, "Models/Primitives/TranslationGizmo.obj");
	TranslateZ->RotateComponent(0.0f, 0.0f, 90.0f);

	GetRoot()->AddComponent(TranslateX);
	GetRoot()->AddComponent(TranslateY);
	GetRoot()->AddComponent(TranslateZ);
}

Gizmo::~Gizmo()
{

}

void Gizmo::Render(Shader* shader)
{
	// @TODO: build logic to show/hide different gizmo types
	Asset::Render(shader);
}

