#include "ComponentBase.h"
#include "System/Transforms.h"
#include <iostream>
#include <vector>

ComponentBase::ComponentBase(Asset* InParent)
{
	ParentAsset = InParent;
	ComponentTransform = new Transforms();
	Color = glm::vec3();
}

ComponentBase::~ComponentBase()
{
	for (ComponentBase* Comp : Components)
	{
		if (Comp) { delete Comp; }
	}

	delete ComponentTransform;
}

void ComponentBase::Render(Shader* shader)
{
	for (ComponentBase* comp : Components)
	{
		if (comp) { comp->Render(shader); }
	}
}

void ComponentBase::AddComponent(ComponentBase* InComponent)
{
	if (InComponent) { Components.push_back(InComponent); }
}

void ComponentBase::TranslateComponent(float x, float y, float z) // @TODO: when components are editable look into possible orientation updating issues.
{
	ComponentTransform->RelativePosition += glm::vec3(x, y, z);
}

void ComponentBase::RotateComponent(float x, float y, float z) // @TODO: when components are editable look into possible orientation updating issues.
{
	ComponentTransform->RelativeRotation += glm::vec3(x, y, z);
}

void ComponentBase::ScaleComponent(float x, float y, float z) // @TODO: when components are editable look into possible orientation updating issues.
{
	ComponentTransform->RelativeScale += glm::vec3(x, y, z);
}

void ComponentBase::UpdateTransforms(glm::mat4 InWorldTransform)
{
	ComponentTransform->WorldSpaceOrientation = InWorldTransform;

	ComponentTransform->Translate(ComponentTransform->RelativePosition);
	ComponentTransform->Rotate(ComponentTransform->RelativeRotation);
	ComponentTransform->Scale(ComponentTransform->RelativeScale);

	for (ComponentBase* comp : Components)
	{
		comp->UpdateTransforms(InWorldTransform);
	}
}

std::string ComponentBase::GetName() { return Name; }
Transforms* ComponentBase::GetTransforms() { return ComponentTransform; }
Asset* ComponentBase::GetParentAsset() { return ParentAsset; }
std::vector<ComponentBase*> ComponentBase::GetComponents() { return Components; }