#include "AssetManager.h"



AssetManager::AssetManager(Shader* InDefaultShader) {
	DefaultShader = InDefaultShader;
	AssetMap[DefaultShader] = std::vector<Asset*>();
}


AssetManager::~AssetManager() {
	for each (Asset* mod in AssetList) {
		mod->~Asset();
	}
}

void AssetManager::DrawAssets(Shader* Shader) {
	for each (Asset* mod in AssetMap[Shader]) {
		glUniformMatrix4fv(Shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(mod->orientation));
		mod->Draw(Shader);
	}
}

void AssetManager::BuildAsset(std::string path) {
	Asset* a = new Asset(path);

	char label[128];
	sprintf_s(label, "Object_%d", AssetList.size());
	a->Name = label;
	AssetMap[DefaultShader].push_back(a);
	AssetList.push_back(a);
}

std::vector<Asset*> AssetManager::GetAssets() {
	return AssetList;
}
