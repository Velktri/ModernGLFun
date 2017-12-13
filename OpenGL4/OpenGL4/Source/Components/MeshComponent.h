#pragma once
#include "Components/ComponentBase.h"

class Element;
class Material;

class MeshComponent : public ComponentBase
{
public:
	MeshComponent(Asset* InParent, std::string path = "");
	~MeshComponent();

	virtual void Render(Shader* shader) override;
	void SetMesh(Element* InElement);
	Material* MeshMaterial;

private:
	Element* MeshData;

	void InitAndSet(std::string path);
};

