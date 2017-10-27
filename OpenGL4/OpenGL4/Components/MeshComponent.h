#pragma once
#include "Components/ComponentBase.h"

class Mesh;

class MeshComponent : public ComponentBase
{
public:
	MeshComponent();
	~MeshComponent();

private:
	Mesh* MeshData;
};

