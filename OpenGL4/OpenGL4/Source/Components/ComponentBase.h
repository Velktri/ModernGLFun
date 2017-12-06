#pragma once
#include <vector>
#include <string>

class Asset;
class Shader;
class Camera;

class ComponentBase
{
public:
	ComponentBase(Asset* InParent);
	~ComponentBase();

	void AddComponent(ComponentBase* InComponent);
	virtual void Render(Shader* shader);


	/** GETTERS - SETTERS */
	std::string GetName();
	Asset* GetParentAsset();
	std::vector<ComponentBase*> GetComponents();

protected:
	std::string Name;
	Asset* ParentAsset;
	std::vector<ComponentBase*> Components;
};