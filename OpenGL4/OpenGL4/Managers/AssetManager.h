#pragma once
#include <vector>
#include "../Models/Asset.h"

class AssetManager {
public:
	AssetManager(Shader* InDefaultShader);
	~AssetManager();

	void DrawAssets(Shader* AssetShader);
	void BuildAsset(std::string path);
	std::vector<Asset*> GetAssets();

private:
	std::unordered_map<Shader*, std::vector<Asset*>> AssetMap;
	std::vector<Asset*> AssetList;
	Shader* DefaultShader;
};

