#pragma once
#include "Components/ComponentBase.h"

class Mesh;

class MeshComponent : public ComponentBase
{
public:
	MeshComponent(Asset* InParent, std::string path = "");
	~MeshComponent();

	virtual void Render(Shader* shader) override;

private:
	Mesh* MeshData;

	void InitAndSet(std::string path);
};

