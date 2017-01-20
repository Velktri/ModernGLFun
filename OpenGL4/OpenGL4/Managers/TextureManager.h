#pragma once
#include <vector>
#include "../Models/Texture.h"

class TextureManager {
public:
	TextureManager();
	~TextureManager();

private:
	void BuildTextures();
	std::vector<Texture*> TextureList;
};

