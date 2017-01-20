#pragma once
#include <vector>
#include "../Models/Asset.h"

class AssetManager {
public:
	AssetManager();
	~AssetManager();

	void DrawAssets(Shader* AssetShader);

private:
	void BuildAssets();
	std::vector<Asset*> AssetList;
};

