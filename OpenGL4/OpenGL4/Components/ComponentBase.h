#pragma once
#include <vector>
#include <string>

class Asset;

class ComponentBase
{
public:
	ComponentBase();
	~ComponentBase();

	void AddComponent(ComponentBase* InComponent);

	/** GETTERS - SETTERS */
	std::string GetName();
	Asset* GetParentAsset();
	void SetParentAsset(Asset* InParent);
	std::vector<ComponentBase*> GetComponents();

protected:
	std::string Name;
	Asset* ParentAsset;
	std::vector<ComponentBase*> Components;
};