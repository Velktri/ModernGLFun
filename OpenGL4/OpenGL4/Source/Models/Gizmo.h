#pragma once
#include "Asset.h"

class MeshComponent;

class Gizmo : public Asset {
public:
	Gizmo(GLuint InAssetID, Manager* InManager);
	~Gizmo();


	virtual void Render(Shader* shader) override;

private:
	MeshComponent* TranslateX;
	MeshComponent* TranslateY;
	MeshComponent* TranslateZ;
};

