#pragma once
#include "Asset.h"

class MeshComponent;

class Gizmo : public Asset {
public:
	Gizmo(GLuint InAssetID, Manager* InManager);
	~Gizmo();

	MeshComponent* TranslateX;
	MeshComponent* TranslateY;
	MeshComponent* TranslateZ;

	virtual void Render(Shader* shader) override;
};

