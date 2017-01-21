#include "AssetManager.h"



AssetManager::AssetManager() {
	BuildAssets();
	//AssetList[0]->ScaleAsset(0.2f, 0.2f, 0.2f);
	//AssetList[1]->TranslateAsset(5.0f, 0.0f, 0.0f);

}


AssetManager::~AssetManager() {
	for each (Asset* mod in AssetList) {
		mod->~Asset();
	}
}

void AssetManager::DrawAssets(Shader* AssetShader) {
	for each (Asset* mod in AssetList) {
		glUniformMatrix4fv(AssetShader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(mod->orientation));
		mod->Draw(AssetShader);
	}
}

void AssetManager::BuildAssets() {
	//AssetList.push_back(new Asset("assets/Models/Custom/nanosuit/nanosuit.obj"));
	//AssetList.push_back(new Asset("assets/Models/Custom/lamborgini/Avent.obj"));

}
