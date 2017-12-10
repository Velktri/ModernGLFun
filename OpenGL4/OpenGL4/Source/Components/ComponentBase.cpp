#include "ComponentBase.h"
#include "System/Transforms.h"
#include <iostream>
#include <vector>

ComponentBase::ComponentBase(Asset* InParent)
{
	ParentAsset = InParent;
	ComponentTransform = new Transforms();
}

ComponentBase::~ComponentBase()
{
	for (ComponentBase* Comp : Components)
	{
		if (Comp) { Comp->~ComponentBase(); }
	}

	ComponentTransform->~Transforms();
}

void ComponentBase::Render(Shader* shader)
{
	for (ComponentBase* comp : Components)
	{
		if (comp)
		{
			comp->Render(shader);
		}
	}
}

void ComponentBase::AddComponent(ComponentBase* InComponent)
{
	if (InComponent) { Components.push_back(InComponent); }
}

void ComponentBase::TranslateComponent(float x, float y, float z)
{
	ComponentTransform->Translate(x, y, z);
}

void ComponentBase::RotateComponent(float x, float y, float z)
{
	ComponentTransform->Rotate(x, y, z);
}

void ComponentBase::ScaleComponent(float x, float y, float z)
{
	ComponentTransform->Scale(x, y, z);
}

std::string ComponentBase::GetName() { return Name; }
Asset* ComponentBase::GetParentAsset() { return ParentAsset; }
std::vector<ComponentBase*> ComponentBase::GetComponents() { return Components; }