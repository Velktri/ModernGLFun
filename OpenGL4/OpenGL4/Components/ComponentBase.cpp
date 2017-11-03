#include "ComponentBase.h"
#include <iostream>
#include <vector>

ComponentBase::ComponentBase(Asset* InParent)
{
	ParentAsset = InParent;
}

ComponentBase::~ComponentBase()
{
	for (ComponentBase* Comp : Components)
	{
		if (Comp) { Comp->~ComponentBase(); }
	}
}

void ComponentBase::Render(Shader* shader, Camera* WorldCamera)
{
	for (ComponentBase* comp : Components)
	{
		if (comp)
		{
			comp->Render(shader, WorldCamera);
		}
	}
}

void ComponentBase::AddComponent(ComponentBase* InComponent)
{
	if (InComponent) { Components.push_back(InComponent); }
}

std::string ComponentBase::GetName() { return Name; }
Asset* ComponentBase::GetParentAsset() { return ParentAsset; }
std::vector<ComponentBase*> ComponentBase::GetComponents() { return Components; }