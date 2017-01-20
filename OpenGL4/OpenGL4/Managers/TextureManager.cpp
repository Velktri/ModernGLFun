#include "TextureManager.h"



TextureManager::TextureManager() {
	BuildTextures();
}


TextureManager::~TextureManager() {
	for each (Texture* t in TextureList) {
		t->~Texture();
	}
}

void TextureManager::BuildTextures() {
	TextureList.push_back(new Texture("OpenGL4/assets/Textures/WoodenBox_diff.png"));
	TextureList.push_back(new Texture("OpenGL4/assets/Textures/WoodenBox_spec.png"));

}
