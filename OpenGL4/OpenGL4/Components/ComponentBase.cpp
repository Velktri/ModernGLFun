#include "ComponentBase.h"
#include <iostream>
#include <vector>

ComponentBase::ComponentBase()
{
	ParentAsset = NULL;
}

ComponentBase::~ComponentBase()
{
}

void ComponentBase::AddComponent(ComponentBase* InComponent)
{

}


std::string ComponentBase::GetName() { return Name; }
Asset* ComponentBase::GetParentAsset() { return ParentAsset; }
std::vector<ComponentBase*> ComponentBase::GetComponents() { return Components; }

void ComponentBase::SetParentAsset(Asset* InParent) { ParentAsset = InParent; }