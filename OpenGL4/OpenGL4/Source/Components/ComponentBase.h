#pragma once
#include <vector>
#include <string>
#include "Models/Asset.h"

class Shader;
class Camera;
class Transforms;

class ComponentBase
{
public:
	ComponentBase(Asset* InParent);
	~ComponentBase();

	void AddComponent(ComponentBase* InComponent);
	void TranslateComponent(float x, float y, float z);
	void RotateComponent(float x, float y, float z);
	void ScaleComponent(float x, float y, float z);
	virtual void Render(Shader* shader);


	/** GETTERS - SETTERS */
	std::string GetName();
	Asset* GetParentAsset();
	std::vector<ComponentBase*> GetComponents();



protected:
	Transforms* ComponentTransform;
	std::string Name;
	Asset* ParentAsset;
	std::vector<ComponentBase*> Components;
};